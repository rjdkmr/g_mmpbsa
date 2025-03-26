``energy2bfac``
================

This tool maps the binding energy contribution of each residue on the structure.
The energy will be written in the B-factor field of the output PDB file/s.
These PDB files can be used with any molecular visualizer and residues can be 
colored according to their energetic contribution. The molecular visualizer
should support method to color residues by the B-factor values.

.. warning:: ``tpr/tpx`` file should not contain PBC broken molecule.
              One may check by generating a PDB file with following command:
              ``editconf -f topol.tpr -o check.pdb``.
              Check `check.pdb` file through visualization.

**Execute following command to get full help**

.. code-block:: bash

    g_mmpbsa energy2bfac -h

Command summary 
----------------

.. code-block:: bash

    g_mmpbsa energy2bfac [-s [<.tpr/.gro/...>]] [-i [<.dat>]] [-n [<.ndx>]]
                         [-c [<.pdb>]] [-s1 [<.pdb>]] [-s2 [<.pdb>]] [-b <time>]
                         [-e <time>] [-dt <time>] [-tu <enum>] [-[no]w]

Options summary
-----------------

::

    Options to specify input files:

    -s      [<.tpr/.gro/...>]  (topol.tpr)
            Structure+mass(db): tpr gro g96 pdb brk ent
    -i      [<.dat>]           (decomp_energy.dat)
            Generic data file
    -n      [<.ndx>]           (index.ndx)      (Opt.)
            Index file

    Options to specify output files:

    -c      [<.pdb>]           (complex.pdb)    (Opt.)
            Protein data bank file
    -s1     [<.pdb>]           (subunit_1.pdb)  (Opt.)
            Protein data bank file
    -s2     [<.pdb>]           (subunit_2.pdb)  (Opt.)
            Protein data bank file

    Other options:

    -b      <time>             (0)
            Time of first frame to read from trajectory (default unit ps)
    -e      <time>             (0)
            Time of last frame to read from trajectory (default unit ps)
    -dt     <time>             (0)
            Only use frame when t MOD dt = first time (default unit ps)
    -tu     <enum>             (ps)
            Unit for time values: fs, ps, ns, us, ms, s
    -[no]w                     (no)
            View output .xvg, .xpm, .eps and .pdb files


Description
-----------
::

    -s topol.tpr

Input tpr/tpx file of molecule

----

::

    -n index.ndx

Input atomic index file. User will get choice to select atomic groups

----

::

    -i decomp_energy.dat

File containing energy contribution of each residue obtained 
from `g_mmpbsa decompose <decompose.html>`_. One can use ``-i energyMapIn.dat`` directly 
for the input.

----

::

    -c complex.pdb

Output PDB file of molecular complex taken for the calculation,
e.g. protein-ligand, protein-DNA or protein-protein.

----

::

    -s1 subunit_1.pdb

Output PDB file of first sub-unit in accordance with first atomic group chosen 
through index file. 

----

::

    -s2 subunit_2.pdb 

Output PDB file of second sub-unit in accordance with second atomic group chosen 
through index file. 

