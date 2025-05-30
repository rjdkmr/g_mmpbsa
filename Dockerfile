FROM quay.io/pypa/manylinux_2_28_x86_64

RUN yum install -y epel-release
RUN dnf install -y  https://repo.almalinux.org/almalinux/8/devel/x86_64/os/Packages/suitesparse-static-4.4.6-11.el8.x86_64.rpm

RUN dnf install -y \
unzip \
wget \
arpack-devel \
arpack-static \
f2c \
eigen3-devel \
boost-devel \
openblas-serial64 \
openblas-static \
openblas-devel \
lapack-devel \
lapack-static \
suitesparse-devel 

COPY external/gromacs /app-src/external/gromacs
COPY external/apbs /app-src/external/apbs
COPY src /app-src/src
COPY scripts /app-src/scripts
COPY pyproject.toml /app-src/pyproject.toml
COPY setup.py /app-src/setup.py
COPY README.md /app-src/README.md
copy g_mmpbsa /app-src/g_mmpbsa
copy .git /app-src/.git

WORKDIR /app-src
RUN bash scripts/static_dep_build_docker.sh

ENV APBS_INSTALL=/app-src/external/apbs_installed
ENV GMX_INSTALL=/app-src/external/gmx_installed
ENV GMX_SRC=/app-src/external/gromacs

CMD ["/workspace/build/src/g_mmpbsa", "-h"]