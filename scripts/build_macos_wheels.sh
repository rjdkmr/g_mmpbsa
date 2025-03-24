#!/bin/bash
set -e -x

CWD=`pwd`

brew install gcc@14 libomp gsl fftw pyenv eigen boost suite-sparse openblas cmake superlu arpack
brew install brewsci/bio/apbs
# brew link --force openblas libomp
brew cleanup

ls -lhrt /opt/homebrew/opt/apbs/
ls -lhrt /opt/homebrew/opt/apbs/lib
ls -lhrt /opt/homebrew/opt/apbs/include

cd external
mkdir apbs_installed
mkdir gmx_installed
mkdir fetk_installed

#export CC=gcc-14
#export CXX=g++-14
#export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:/opt/homebrew/opt/openblas/lib/pkgconfig"
#export LDFLAGS="${LDFLAGS} -L/opt/homebrew/opt/libomp/lib -L/opt/homebrew/opt/openblas/lib"
#export CPPFLAGS="${CPPFLAGS} -fpermissive -I/opt/homebrew/opt/libomp/include -I/opt/homebrew/opt/openblas/include"
#export CFLAGS="${CFLAGS} -fpermissive -I/opt/homebrew/opt/libomp/include -I/opt/homebrew/opt/openblas/include"
#export CXXFLAGS="${CXXFLAGS} -fpermissive -I/opt/homebrew/opt/libomp/include -I/opt/homebrew/opt/openblas/include"

cd apbs
mkdir build && cd build

cmake .. \
  -DCMAKE_INSTALL_INCLUDEDIR="include" \
  -DBUILD_DOC=OFF \
  -DAPBS_STATIC_BUILD=OFF  \
  -DBUILD_TOOLS=OFF \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=${APBS_INSTALL} \
  -DENABLE_PYGBE=OFF \
  -DENABLE_BEM=OFF \
  -DENABLE_iAPBS=ON \
  -DENABLE_GEOFLOW=OFF \
  -DENABLE_OPENMP=OFF \
  -DENABLE_PBAM=OFF \
  -DENABLE_PBSAM=OFF \
  -DENABLE_PYTHON=OFF \
  -DENABLE_TESTS=OFF \
  -DGET_NanoShaper=OFF \
  -DFETK_VERSION="57195e55351e04ce6ee0ef56a143c996a9aee7e2" \
  -DCMAKE_VERBOSE_MAKEFILE=ON

make 
make install

cd $CWD/external/gromacs
if [ -d build ]; then
    rm -rf build
fi
mkdir build && cd build

export GMX_INSTALL=${CWD}/external/gmx_installed
export GMX_SRC=${CWD}/external/gromacs

cmake .. -DCMAKE_CC_COMIPLER=gcc-14 -DCMAKE_CXX_COMIPLER=g++-14 \
         -DGMX_SIMD=NONE -DGMX_GPU=off -DGMXAPI=OFF -DGMX_INSTALL_LEGACY_API=on \
         -DGMX_FFT_LIBRARY=fftpack -DCMAKE_INSTALL_PREFIX=${GMX_INSTALL}
            
make
make install

cd ${CWD}

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
done
pyenv global system

mkdir wheels
mkdir fixed_wheels
for PYTHON in ${PYTHONS[@]}
do
    pyenv global $PYTHON
    echo $(python --version)
    GMX_INSTALL=${GMX_INSTALL} GMX_SRC=${GMX_SRC} python -m pip install -v --no-deps --no-cache-dir .
    otool -L build/lib.*/gmx_clusterByFeatures/*.so
    install_name_tool -change @rpath/libgromacs.2.dylib ${GMX_INSTALL}/lib/libgromacs.dylib build/lib.*/g_mmpbsa/*.so
    GMX_INSTALL=${GMX_INSTALL} GMX_SRC=${GMX_SRC} APBS_INSTALL=${APBS_INSTALL} python -m pip wheel -v -w temp_wheels/ --no-deps --no-cache-dir .
    python -m pip uninstall -y g_mmpbsa
    python -m pip install -v --no-deps --no-cache-dir temp_wheels/*.whl
    cp temp_wheels/*.whl wheels/.
    rm -rf build
    rm -rf temp_wheels
done

delocate-listdeps wheels/*.whl
delocate-wheel -w fixed_wheels -v wheels/*.whl
delocate-listdeps fixed_wheels/*.whl
pyenv global system

ls -lrt fixed_wheels/
