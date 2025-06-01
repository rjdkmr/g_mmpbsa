Getteing started with MM/PBSA calculation
=========================================

g_mmpbsa command contains several sub-commands. The main command to run MM/PBSA calculation is ``g_mmpbsa run``.
All the input options should be provided on command line depending on the type of calculation.
To calculate for single atomic group (e.g. only protein, only ligand, only specific region of protein etc.) 
one group is required to choose from index file, whereas for complex (e.g. protein-ligand, protein-protein etc.) 
two separate group is required to choose from index file.

For more details about ``g_mmpbsa run`` command, visit `usage <commands/run.html>`_ or type following command:

::

    g_mmpbsa run -h


.. warning:: On MacOS, external APBS is required to run this command. 
             If APBS is not installed, the command will fail with an error message.

             * Use **Homebrew to install APBS** on MacOS (``brew install brewsci/bio/apbs``).
             * Define APBS environment variable to point to the APBS executable, e.g. ``export APBS=$(brew --prefix apbs)/bin/apbs``.


**Only molecular mechanics (vdw and electrostatic) vacuum energy with energy decomposition**

::

    g_mmpbsa run -f traj.xtc -s topol.tpr -n index.ndx -unit1 Protein -unit2 Ligand \
                 -mme -mm energy_MM.xvg -decomp -mmcon contrib_MM.dat
          
Here ``traj.xtc`` is the trajectory file, ``topol.tpr`` is the topology file,
``index.ndx`` is the index file, ``Protein`` and ``Ligand`` are the group names 
of protein and ligand in the index file.
``energy_MM.xvg`` is the output file for MM energy and ``contrib_MM.dat`` 
is the output file for energy decomposition.

**Only polar solvation energy with energy decomposition**

::

    g_mmpbsa run -f traj.xtc -s topol.tpr -i mmpbsa.mdp -n index.ndx \
                 -unit1 Protein -unit2 Ligand -nomme -pbsa -decomp \
                 -pol polar.xvg -pcon contrib_pol.dat

``polar.xvg`` is the output file for polar-solvation energy and ``contrib_pol.dat`` is the output file for energy decomposition.
An example `mmpbsa.mdp <https://github.com/rjdkmr/g_mmpbsa/blob/master/test/polar_orig/mmpbsa.mdp>`_ file is provided.



**Only non-polar solvation energy with energy decomposition**

::

    g_mmpbsa run -f traj.xtc -s topol.tpr -i mmpbsa.mdp -n index.ndx \
                 -unit1 Protein -unit2 Ligand -nomme -pbsa -decomp \
                 -apol apolar.xvg -apcon contrib_apol.dat

``apolar.xvg`` is the output file for non-polar-solvation energy and ``contrib_apol.dat`` is the output file for energy decomposition.

*   An example `mmpbsa.mdp <https://github.com/rjdkmr/g_mmpbsa/blob/master/test/sasa_orig/mmpbsa.mdp>`_ file is provided for SASA only model.
*   An example `mmpbsa.mdp <https://github.com/rjdkmr/g_mmpbsa/blob/master/test/sav_orig/mmpbsa.mdp>`_ file is provided for SAV only model.


**All energetic term with energy decomposition**

::

    g_mmpbsa run -f traj.xtc            -s topol.tpr \
                 -i mmpbsa.mdp          -n index.ndx -pbsa \
                 -mm energy_MM.xvg      -pol polar.xvg \
                 -apol apolar.xvg       -decomp \
                 -mmcon contrib_MM.dat  -pcon contrib_pol.dat \
                 -apcon contrib_apol.dat 
              

----

Parallel computation using ``OpenMP``
-------------------------------------
g_mmpbsa inherits OpenMP parallel computation implemented in APBS and also implement MM energy calculations in parallel.
This parallel support is enabled by default and all processors/core will be used during runtime.

To control the usage of processors/cores, number of threads can be changed by defining environment variable. For example in bash, one can write following command:

::

    export OMP_NUM_THREADS=X

where ``X`` is number of core/processors.

.. warning:: **Not available on MacOS**.
             APBS installed using Homebrew does not support OpenMP parallel computation on MacOS.

