Change log
==========

Version 3.0
------------
* ``g_mmpbsa`` is now a Python package containing different tools.

* It can be installed by pip command: ``python -m pip install g-mmpbsa``.

* ``apbs`` is also included as sub-command in ``g_mmpbsa``.

* Supports any GROMACS version.

* Also available on MacOS apart from Linux.
  
* Supports Python 3.9, 3.10, 3.11, 3.12 and 3.13.

Removed features from previous versions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*   Removed internal APBS linking, now APBS is included as a sub-command in the package.

*   Removed van der Waal radii options. Now by default only Bondii radii is used. Any missing radius is taken from force-field parameters.

*   Weeks-Chandler-Andersen (WCA) non-polar solvation model is removed.


Version 2.0
-----------

*   Supports **GROMACS 2021**  and all previous versions.

*   Supports any **APBS** versions - now as an **external** tool

*   Inherits APBS capability of parallel computation using **OpenMP**.

*   Options for several non-polar solvation model such as SASA and SAV.

*   Options for calculating contribution of each residue in the net binding energy.

*   Simultaneous computations of binding energy components and residue wise energy contribution, and thus it is computationally less expensive.

*   This tool can be modified and/or redistributed under terms of GNU public license.

Version 1.6
-----------

1. Added support for Gromacs-5.0.x versions

2. Automatic detection of Gromacs versions. No need to write Gromacs version during compilation.

3. Fixed -rvdw option

4. Fixed warning "Setting option -pdie more than once!"

5. Python scripts works with both Python 2.7.x and and Python 3.x

6. Tried to fixed Segmentation errors in Binary Packages (Happens due to different CPU architecture)



Version 1.5
-----------

1. Added support for external APBS program. Now, g_mmpbsa could be used with MPIRUN and external APBS program for parallel computations on HPC.

2. New option "-silent" to supress messages from external APBS program.

3. Added support to install with APBS-1.4.x version.

4. New input kewwords for polar-solvation parameter: mg-type  = mg-auto or mg-para

5. Migrated to cmake build system for installation.

6. Minor bug-fixes.


Version 1.1
------------

1. Update for MM calculation. Now this calculation is very fast and does not require large memory.

2. New option "-incl_14" is added for MM calculation. This option enable the exclusion of 1-2 and 1-3 bonded atom-pairs, and the scaling of 1-4 interactions.

3. Minor update and bug-fixes
