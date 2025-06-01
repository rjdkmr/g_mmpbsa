#!/bin/bash
set -e -x

CWD=`pwd`

brew install gettext pyenv
brew install brewsci/bio/apbs
brew cleanup

cd external
mkdir gmx_installed

cd $CWD/external/gromacs
if [ -d build ]; then
    rm -rf build
fi
mkdir build && cd build

export GMX_INSTALL=${CWD}/external/gmx_installed
export GMX_SRC=${CWD}/external/gromacs

cmake .. -DGMX_SIMD=NONE -DGMX_GPU=off -DGMXAPI=OFF -DGMX_INSTALL_LEGACY_API=on \
         -DGMX_FFT_LIBRARY=fftpack -DCMAKE_INSTALL_PREFIX=${GMX_INSTALL} -DGMX_OPENMP=OFF
            
make
make install

cd ${CWD}

# build the wheels for each Python version
mkdir wheelhouse
eval "$(pyenv init -)"
pyenv install --list
PYVERS=("3.9" "3.10" "3.11" "3.12")
PYTHONS=()
for PYVER in ${PYVERS[@]}
do
    # Install the latest release of the specified Python version using pyenv.
    PYVER="$(pyenv install --list | grep -E "^\\s*$PYVER" | sort -n -t. -k3 | tail -n1)"
    pyenv install $PYVER
    pyenv global $PYVER
    PYTHONS+=($PYVER)
    echo $PYVER
    echo $(python --version)
    python -m pip install --upgrade pip
    python -m pip install -r ${CWD}/dev-requirements.txt
    python -m pip install delocate

    # install and build wheels
    GMX_INSTALL=${GMX_INSTALL} GMX_SRC=${GMX_SRC} python -m pip install -v --no-deps --no-cache-dir .
    otool -L build/lib.*/g_mmpbsa/*.so
    install_name_tool -change @rpath/libgromacs.10.dylib ${GMX_INSTALL}/lib/libgromacs.dylib build/lib.*/g_mmpbsa/*.so
    GMX_INSTALL=${GMX_INSTALL} GMX_SRC=${GMX_SRC} APBS_INSTALL=${APBS_INSTALL} python -m pip wheel -v -w wheelhouse/ --no-deps --no-cache-dir .
    python -m pip uninstall -y g_mmpbsa
    rm -rf build
done
pyenv global system

# repair the wheels
mkdir fixed_wheelhouse
delocate-listdeps wheelhouse/*.whl
delocate-wheel -w fixed_wheelhouse -v wheelhouse/*.whl
delocate-listdeps fixed_wheelhouse/*.whl

# install from wheels and test the wheels
for PYTHON in ${PYTHONS[@]}
do
    pyenv global $PYTHON
    echo $(python --version)
    python -m pip install -v --no-deps --no-cache-dir fixed_wheelhouse/*.whl
    python -c "import g_mmpbsa; print('=====\nTEST -- g_mmpbsa GROMACS version: ', g_mmpbsa.gmx_version, '\n=====')"
done
pyenv global system

ls -lrt fixed_wheelhouse/
