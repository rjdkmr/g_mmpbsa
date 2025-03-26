``g_mmpbsa average``
====================

This script calculates the average binding energy and its standard deviation/error from the output files, which are obtained from g_mmpbsa.

Command summary
---------------

.. code-block:: bash

    g_mmpbsa average [-h] [-mt] [-mf metafile.dat] [-m energy_MM.xvg]
                     [-p polar.xvg] [-a apolar.xvg] [-bs] [-nbs 500]
                     [-of full_energy.dat] [-os summary_energy.dat]
                     [-om meta_energy.dat]



Description
-----------

``-h, --help``
~~~~~~~~~~~~~~

show this help message and exit

----

``-mt, --multiple``
~~~~~~~~~~~~~~~~~~~~

If given, calculates for multiple complexes. Need metafile containing path of energy files

----

``-mf metafile.dat, --metafile metafile.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
Metafile containing path to energy files of each complex in a row obtained from g_mmpbsa in following order: `[MM file] [Polar file] [ Non-polar file]`

----

``-m energy_MM.xvg, --molmech energy_MM.xvg``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vacuum Molecular Mechanics energy file obtained from g_mmpbsa.

----

``-p polar.xvg, --polar polar.xvg``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Polar solvation energy file obtained from g_mmpbsa.

----

``-a apolar.xvg, --apolar apolar.xvg``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Non-Polar solvation energy file obtained from g_mmpbsa.

----

``-bs, --bootstrap``
~~~~~~~~~~~~~~~~~~~~~

If given, Enable Boot Strap analysis to calculate standard error.

----

``-nbs 500, --nbstep 500``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Number of boot strap steps for average energy and standard error calculation.

----

``-of full_energy.dat, --outfr full_energy.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Energy File: All energy components in function of time.

----

``-os summary_energy.dat, --outsum summary_energy.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Final Energy File: Summary of energy components.

----

``-om meta_energy.dat, --outmeta meta_energy.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Final Energy File for Multiple Complexes: Complex wise net binding energy.