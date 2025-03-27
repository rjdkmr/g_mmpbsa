``g_mmpbsa decompose``
======================

This scripts calculate final contribution energy of each residue from individual energetic terms obtained from the g_mmpbsa

Command summary
---------------

.. code-block:: bash

    g_mmpbsa decompose [-h] [-m contrib_MM.dat] [-p contrib_pol.dat] [-a contrib_apol.dat] [-bs]
                       [-nbs 500] [-ct 999] [-o final_contrib_energy.dat] 
                       [-ocsv final_contrib_energy.csv] [-om energyMapIn.dat]

----

Description
-----------


``-h, --help``
~~~~~~~~~~~~~~

show this help message and exit

----
    
``-m contrib_MM.dat, --molmech contrib_MM.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Molecular Mechanics energy contribution file obtained from g_mmpbsa.

----

``-p contrib_pol.dat, --polar contrib_pol.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Polar solvation energy contribution file obtained from g_mmpbsa.

----

``-a contrib_apol.dat, --apolar contrib_apol.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Non-Polar solvation energy contribution file obtained from g_mmpbsa.

----

``-bs, --bootstrap``
~~~~~~~~~~~~~~~~~~~~~~

If given, Enable Boot Strap analysis to calculate standard error.

----

``-nbs 500, --nbstep 500``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Number of boot strap steps for average energy and standard error calculation.

----

``-ct 999, --cutoff 999``
~~~~~~~~~~~~~~~~~~~~~~~~~

Absolute Cutoff in kJ/mol - Residues with energy values above and below this value will be in output
while other residues will be filtered out from the output.
If its value is 999, all residues energy will be in output.

This option is useful to filter-out the residues that are either not 
contributing or contributing very less to the binding energy.

----

``-o final_contrib_energy.dat, --output final_contrib_energy.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Final output file containing binding energy contribution of each residue.

----

``-ocsv final_contrib_energy.csv, --outputCSV final_contrib_energy.csv``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Final Decomposed Energy File im CSV format


``-om energyMapIn.dat, --outmap energyMapIn.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It is input file for `energy2bfac`. It can be used with `energy2bfac` to map energy on structure for visualization.
