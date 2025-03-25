g_mmpbsa run
=============

``g_mmpbsa run`` calculates binding energy of biomolecular associations like protein-protein, 
protein-ligand protein-DNA etc using MM/PBSA. It gives the different component of energy 
term in separate file so that user will have choice to have either MM, PB and SA energy 
values or all energies according to their objective. The tool also gives residue wise 
contribution to total binding energy which will provide information about important 
contributing residues to the molecular association.

**Execute following command to get full help**

.. code-block:: bash

    g_mmpbsa run -h

Command summary 
----------------

.. code-block:: bash

    g_mmpbsa run [-f [<.xtc/.trr/...>]] [-s [<.tpr/.gro/...>]] [-n [<.ndx>]]
                 [-i [<.mdp>]] [-ipdb [<.pdb>]] [-opdb [<.pdb>]] [-mm [<.xvg>]]
                 [-pol [<.xvg>]] [-apol [<.xvg>]] [-mmcon [<.dat>]]
                 [-pcon [<.dat>]] [-apcon [<.dat>]] [-o [<.xvg>]] [-os [<.csv>]]
                 [-ores [<.csv>]] [-b <time>] [-e <time>] [-dt <time>]
                 [-tu <enum>] [-fgroup <selection>] [-xvg <enum>] [-sf <file>]
                 [-selrpos <enum>] [-seltype <enum>] [-unit1 <selection>]
                 [-unit2 <selection>] [-rvdw <real>] [-pdie <real>]
                 [-ndots <int>] [-[no]mme] [-[no]ddc] [-[no]pbsa] [-[no]diff]
                 [-[no]decomp] [-[no]incl_14] [-[no]focus] [-[no]silent]

Options summary
-----------------

.. list-table:: Options to specify input files
    :widths: 1, 1, 4
    :header-rows: 1
    :name: input-files-table-run
    :stub-columns: 1
    :align: left

    * - Option
      - Default
      - File type

    * - `-f [<.xtc/.trr/...>] <run.html#f-traj-xtc>`_
      - traj.xtc
      - Input trajectory or single configuration: xtc trr cpt gro g96 pdb tng
    
    * - `-s [<.tpr>] <run.html#s-topol-tpr>`_  
      - topol.tpr
      - Input structure: tpr
    
    * - `-n [<.ndx>] <run.html#n-index-ndx>`_
      - index.ndx
      - Extra index groups. Should contain index group of first and second sub-units.
    
    * - `-i [<.mdp>] <run.html#i-mmpbsa-mdp>`_
      - mmpbsa.mdp
      - Input parameters for PBSA calculations.
    
    * - `-ipdb [<.pdb>] <run.html#ipdb-input-pdb>`_
      - input.pdb
      - Input pdb file to dump residue energy in b-factor field. If not
        provided, input tpr file will be used.

.. list-table:: Options to specify output files
    :widths: 1, 1, 4
    :header-rows: 1
    :name: output-files-table-run
    :stub-columns: 1
    :align: left

    * - Option
      - Default
      - File type
    
    * - `-opdb [<.pdb>] <run.html#opdb-energy-pdb>`_
      - energy.pdb
      - Output PDB file with residue energy in b-factor field.
 
    * - `-mm [<.xvg>] <run.html#mm-energy-mm-xvg>`_
      - energy_MM.xvg
      - Vaccum MM energy as a function of time
 
    * - `-pol [<.xvg>] <run.html#pol-polar-xvg>`_
      - polar.xvg
      - Polar solvation energy as a function of time
    
    * - `-apol [<.xvg>] <run.html#apol-apolar-xvg>`_ 
      - apolar.xvg
      - Apolar solvation energy as a function of time
    
    * - `-mmcon [<.dat>] <run.html#mmcon-residues-mm-dat>`_
      - residues_MM.dat
      - Vacuum MM energy contribution to binding
    
    * - `-pcon [<.dat>] <run.html#pcon-residues-polar-dat>`_
      - residues_polar.dat
      - Polar solvation energy contribution to binding
 
    * - `-apcon  [<.dat>] <run.html#apcon-residues-apolar-dat>`_
      - residues_apolar.dat
      - Apolar solvation energy contribution to binding
 
    * - `-o [<.xvg>] <run.html#o-binding-energy-xvg>`_
      - binding_energy.xvg
      - Final binding energy and its components
  
    * - `-os [<.csv>] <run.html#os-energy-summary-csv>`_
      - energy_summary.csv
      - Summary of binding energy over all frames
    
    * - `-ores [<.csv>] <run.html#ores-residues-energy-summary-csv>`_
      - residues_energy_summary.csv
      - Summary of binding energy contributions of residues over all frames


.. list-table:: Other options
    :widths: 3, 1, 5
    :header-rows: 1
    :name: other-options-table-run
    :align: left

    * - Option
      - Default
      - Description
  
    * - ``-b <time>``
      - 0
      - First frame (ps) to read from trajectory
    * - ``-e <time>``
      - 0
      - Last frame (ps) to read from trajectory
    * - ``-dt <time>``
      - 0
      - Only use frame if t MOD dt == first time (ps)
    * - ``-tu <enum>``
      - ps
      - Unit for time values: ``fs``, ``ps``, ``ns``, ``us``, ``ms``, ``s``
    * - ``-fgroup <selection>``
      - 
      - Atoms stored in the trajectory file (if not set, assume first N atoms)
    * - ``-xvg <enum>``
      - xmgrace
      - Plot formatting for xvg file: ``xmgrace``, ``xmgr``, ``none``
    * - ``-sf <file>``
      - 
      - Provide selections from files
    * - ``-selrpos <enum>``
      - atom
      - Selection reference positions: atom, res_com, res_cog, mol_com,
           mol_cog, whole_res_com, whole_res_cog, whole_mol_com,
           whole_mol_cog, part_res_com, part_res_cog, part_mol_com,
           part_mol_cog, dyn_res_com, dyn_res_cog, dyn_mol_com, dyn_mol_cog
    * - ``-seltype <enum>``
      - atom
      - Default selection output positions: atom, res_com, res_cog,
           mol_com, mol_cog, whole_res_com, whole_res_cog, whole_mol_com,
           whole_mol_cog, part_res_com, part_res_cog, part_mol_com,
           part_mol_cog, dyn_res_com, dyn_res_cog, dyn_mol_com, dyn_mol_cog
    * - ``-unit1 <selection>``
      - 
      - Select protein or first group
    * - ``-unit2 <selection>``
      - 
      - Select ligand or second group
    * - ``-rvdw <real>``
      - 0.1
      - Default van der Waal radius (in nm) if radius not found for any atom-types.
    * - ``-pdie <real>``
      - 1.0
      - Dielectric constant of solute. Should be same as of polar solvation
    * - ``-ndots  <int>``
      - 24
      - Number of dots per sphere in the calculation of SASA, more dots means more accuracy
    * - ``-[no]mme``
      - yes
      - To calculate vacuum molecular mechanics energy
    * - ``-[no]ddc``
      - no
      - To enable distance dependent dielectric constant
    * - ``-[no]pbsa``
      - no
      - To calculate polar and/or non-polar solvation energy
    * - ``-[no]diff``
      - yes
      - Calculate the energy difference between two group otherwise only calculates for one group
    * - ``-[no]decomp``
      - no
      - Number of dots per sphere in the calculation of SASA, more dots means more accuracy
    * - ``-[no]incl_14``
      - no
      - Include 1-4 atom-pairs, exclude 1-2 and 1-3 atom pairs during MM calculation. Should be "yes" when groups are bonded with each other.
    * - ``-[no]focus``
      - no
      - To enable focusing on the specfic region of molecule, group of atoms must be provided in index file
    * - ``-[no]silent``
      - no
      - Display messages, output and errors from external APBS program


Options to specify input files
--------------------------------

``-f traj.xtc``
~~~~~~~~~~~~~~~

Input trajectory xtc/trr format file. 

 .. warning:: Trajectory should be PBC corrected and molecule should not be PBC broken. 
              To make molecule whole in trajectory, please follow these linksP:

              * `PBC <https://manual.gromacs.org/current/user-guide/terminology.html#periodic-boundary-conditions>`_
              * `gmx trjconv <https://manual.gromacs.org/current/onlinehelp/gmx-trjconv.html>`_


``-s topol.tpr``
~~~~~~~~~~~~~~~~
Input tpr/tpx file of molecule.

.. note:: ``-s`` **only accept tpr/tpx** file as input. PDB/GRO file is not allowed as it does 
          not contain force-field parameters required for the MM/PBSA calculations.


``-n index.ndx``
~~~~~~~~~~~~~~~~

Input atomic index file. The idex file should contain index groups of first (``-unit1``) and second (``unit2``) sub-units.

``-i mmpbsa.mdp``
~~~~~~~~~~~~~~~~~
Input parameter file for polar and non-polar solvation energy.
For more details about accepted keywords and options,
follow these two links:

* `Polar-Solvation Parameters <../parameters/polar.rst>`_
* `Non-polar Solvation Keywords <../parameters/apolar.rst>`_


``-ipdb input.pdb``
~~~~~~~~~~~~~~~~~~~
Optional input pdb file to dump residue energy in b-factor field. 
If not provided, input tpr file will be used.

Options to specify output files
--------------------------------

``-opdb energy.pdb``
~~~~~~~~~~~~~~~~~~~~~
Output PDB file with residue energy in b-factor field.

``-mm energy_MM.xvg``
~~~~~~~~~~~~~~~~~~~~~~

van der Waal and electrostatic energy of the selected atom group/s.  

* With `-nodiff` option, only one index group can be selected. In this case, this file contains vacuum MM energy of this selected group.
  Always **USE** ``-incl_14`` option for single group calculations.

* By default, two groups can be selected, and this file contains only interaction energy between two groups.
  Energy of each group and thier complex is not calculated. 

* However, with ``-incl_14`` option, vacuum MM energy components for each group and their 
  complex is calculated. Final binding energy can be calculated later using the  
  `g_mmpbsa average <average.html>`_ sub-command.

``-pol polar.xvg``
~~~~~~~~~~~~~~~~~~~
Polar solvation energy of the selected atoms group/s.

* With ``-nodiff`` option, only one index group can be selected.
  In this case, this file contains energy of this selected group. 

* By default, two groups can be selected, and this file contains energy of each group 
  and their complex.

``-apol apolar.xvg``
~~~~~~~~~~~~~~~~~~~~~
Non-polar solvation energy of the selected atoms group/s. 

* With ``-nodiff`` option, only one index group can be selected.
  In this case, this file contains energy of this selected group. 

* By default, two groups can be selected, and this file contains energy of each 
  group and their complex.

``-mmcon residues_MM.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vacuum MM van der Waals and electrostatic energy contribution per residue per frame/snapshot.

``-pcon residues_polar.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Polar solvation energy contribution per residue frame wise.

``-apcon residues_apolar.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Non-polar solvation energy contribution per residue frame wise.

``-o binding_energy.xvg``
~~~~~~~~~~~~~~~~~~~~~~~~~~
Final binding energy and its components frame wise.

``-os energy_summary.csv``
~~~~~~~~~~~~~~~~~~~~~~~~~~
Summary of all energy terms as average and standard deviation.

``-ores residues_energy_summary.csv``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Summary of binding energy contributions (both average and standard deviation) 
of residues over all frames.