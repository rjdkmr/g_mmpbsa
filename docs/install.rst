Download and Installation
=========================

Quick Installation using ``pip`` or ``pipx``
--------------------------------------------

It is **recommended** method to install g_mmpbsa.

Following Python versions are supported:

* Python 3.9
* Python 3.10
* Python 3.11
* Python 3.12
* Python 3.13


On Linux
~~~~~~~~

**No dependency on GROMACS and APBS. Install and use it.**

Distributions with **glibc version 2.28 or later** are supported. Some of the supported distributions are:

* Debian 10+
* Ubuntu 18.10+
* Fedora 29+
* CentOS/RHEL 8+

Using ``pip``
++++++++++++++
Use following steps to install g_mmpbsa using ``pip``:

.. code:: bash

    sudo python3 -m pip install g-mmpbsa


Using ``pipx``
++++++++++++++
In newer Linux distributions, it is not recommended to install Python packages 
using ``pip`` directly.
Instead, it is recommended to use `pipx <https://pipx.pypa.io/>`_ to install 
Python packages in isolated environments.

.. code:: bash

    sudo python3 -m pip install --user pipx
    python3 -m pipx ensurepath
    source ~/.bashrc # or source ~/.zshrc
    pipx install g-mmpbsa

On MacOS
~~~~~~~~~~
**No dependency on GROMACS. However, APBS should be installed externally.**

Following MacOS versions are supported:

* MacOS 13 (Ventura)
* MacOS 14 (Sonoma)
* MacOS 15 (Sequoia)

On MacOS, **external APBS is required** to run the ``g_mmpbsa``. If APBS is not installed, 
the command will fail with an error message. Use **Homebrew to install APBS** on MacOS
as follows.

Using ``pip``
++++++++++++++

.. code:: bash

    brew install brewsci/bio/apbs # Install APBS using Homebrew
    sudo python3 -m pip install g-mmpbsa


Using ``pipx``
++++++++++++++

Alternatively, `pipx <https://pipx.pypa.io/>`_ can be used to install g_mmpbsa in an
isolated environment:

.. code:: bash

    brew install pipx             # Install pipx using Homebrew
    brew install brewsci/bio/apbs # Install APBS using Homebrew
    pipx install g-mmpbsa

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

.. warning::
    The above steps are only tested on **Linux**.