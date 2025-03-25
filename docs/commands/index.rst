g_mmpbsa commands
====================

.. list-table:: List of sub-commands available in g_mmpbsa
    :widths: 1, 4
    :header-rows: 1
    :name: commands-table

    * - Command
      - Function

    * - `run <commands/cluster.html>`_
      - Run MM/PBSA calculation using trajectory and tpr file.

    * - `apbs <commands/apbs.html>`_
      - apbs program used in PB calculation.
      
    * - `energy2bfac <commands/distmat.html>`_
      - Generate PDB file where decomposed binding energy is written in B-factor column.

    * - `average <commands/matplot.html>`_
      - Calculate final average binding energy including all energy terms. Supports multiple complexes at once.
      
    * - `correlation <commands/hole.html>`_
      - Same as average, but also calculates correlation between predicted and experimental binding energies.
      
    * - `decompose <commands/holeplot.html>`_
      - Calculate final average decomposed energies of residues with plots.
    
.. toctree::
   :maxdepth: 1
   :caption: Contents

   run <run.rst>
   apbs <apbs.rst>