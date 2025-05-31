#!/bin/bash
set -e -x

CWD=`pwd`

brew install gettext pyenv boost suite-sparse cmake superlu metis lapack arpack openblas
brew cleanup

cd external
mkdir gmx_installed
mkdir apbs_installed
mkdir fetk_installed

wget https://github.com/Electrostatics/FETK/archive/refs/tags/1.9.3.tar.gz
tar -xzf 1.9.3.tar.gz
ls -lhrt
cd FETK-1.9.3
mkdir build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=${CWD}/external/fetk_installed \
    -DCMAKE_C_FLAGS="-Wno-error=implicit-int" \
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DBLA_STATIC=OFF

make || exit 1
make install

ls -lhrt ${CWD}/external/fetk_installed/
ls -lhrt ${CWD}/external/fetk_installed/share

cd $CWD/external

cd apbs/
if [ -d build ]; then
    rm -rf build
fi

sed -i -e 's/include\(ImportFETK\)//g' CMakeLists.txt
sed -i -e 's/import\_fetk\(\$\{FETK_VERSION\}\)//g' CMakeLists.txt
cat CMakeLists.txt

mkdir build && cd build

cmake .. \
  -DCMAKE_INSTALL_INCLUDEDIR="include" \
  -DBUILD_DOC=OFF \
  -DAPBS_STATIC_BUILD=OFF  \
  -DBUILD_TOOLS=OFF \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX=${CWD}/external/apbs_installed \
  -DENABLE_PYGBE=OFF \
  -DENABLE_BEM=OFF \
  -DENABLE_iAPBS=ON \
  -DENABLE_GEOFLOW=OFF \
  -DENABLE_OPENMP=OFF \
  -DENABLE_PBAM=OFF \
  -DENABLE_PBSAM=OFF \
  -DENABLE_PYTHON=OFF \
  -DENABLE_TESTS=OFF \
  -DFETK_VERSION=57195e55351e04ce6ee0ef56a143c996a9aee7e2 \
  -DGET_NanoShaper=OFF \
  -DCMAKE_C_FLAGS="-fpermissive -I${CWD}/external/fetk_installed/include -Wno-error=incompatible-pointer-types" \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DAPBS_LIBS="mc;maloc" \
  -DCMAKE_MODULE_PATH=${CWD}/external/fetk_installed/share/fetk/cmake \
  -DCMAKE_EXE_LINKER_FLAGS=-L${CWD}/external/fetk_installed/lib

make || exit 1
make install

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
