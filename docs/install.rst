Download and Installation
=========================

Quick Installation using ``pip``
--------------------------------

It is **recommended** method to install g_mmpbsa.

Following Python versions are supported:

* Python 3.9
* Python 3.10
* Python 3.11
* Python 3.12

**No dependency on GROMACS and APBS. Install and use it.**

On Linux
~~~~~~~~

Distributions with **glibc version 2.28 or later** are supported. Some of the supported distributions are:

* Debian 10+
* Ubuntu 18.10+
* Fedora 29+
* CentOS/RHEL 8+

Use following steps to install gmx_clusterByFeatrues:

.. code:: bash

    sudo python3 -m pip install g-mmpbsa

Installation from source code
-----------------------------

Conda can be used to install ``g_mmpbsa`` from source code. Please follow the steps below:

.. code-block:: bash

    # Clone the repository and setup development environment
    git clone --recursive https://github.com/rjdkmr/g_mmpbsa.git
    cd g_mmpbsa
    conda env create -y --prefix ./venv --file environment.yaml # Create development conda environment
    conda activate ./venv # Activate the environment
    bash -i scripts/build_in_conda.sh # all build and setup steps are in this script
