#!/bin/bash

CWD=$(pwd)

cd $CWD/external
mkdir apbs_installed
mkdir gmx_installed
export APBS_INSTALL=$CWD/external/apbs_installed

cd apbs/
if [ -d build ]; then
    rm -rf build
fi

sed -i -e 's/message(FATAL_ERROR "OpenMP cannot be used with a static build")//' CMakeLists.txt

mkdir build && cd build

cmake .. \
  -DCMAKE_INSTALL_INCLUDEDIR="include" \
  -DBUILD_DOC=OFF \
  -DAPBS_STATIC_BUILD=ON  \
  -DBUILD_TOOLS=OFF \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX=${APBS_INSTALL} \
  -DENABLE_PYGBE=OFF \
  -DENABLE_BEM=OFF \
  -DENABLE_iAPBS=OFF \
  -DENABLE_GEOFLOW=OFF \
  -DENABLE_OPENMP=ON \
  -DENABLE_PBAM=OFF \
  -DENABLE_PBSAM=OFF \
  -DENABLE_PYTHON=OFF \
  -DENABLE_TESTS=OFF \
  -DFETK_VERSION=57195e55351e04ce6ee0ef56a143c996a9aee7e2 \
  -DGET_NanoShaper=OFF \
  -DCMAKE_C_FLAGS="-fpermissive" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DCMAKE_VERBOSE_MAKEFILE=ON \

make > make.log 2>&1 || exit 1
make install

cd $CWD/external/gromacs
if [ -d build ]; then
    rm -rf build
fi
mkdir build && cd build

export GMX_PATH=$CWD/external/gmx_installed
export GMX_SRC=$CWD/external/gromacs

cmake .. -DGMX_SIMD=NONE -DGMX_GPU=off -DGMXAPI=OFF -DGMX_INSTALL_LEGACY_API=on \
             -DGMX_FFT_LIBRARY=fftpack -DCMAKE_INSTALL_PREFIX=${GMX_PATH}
            
make -j12
make install
