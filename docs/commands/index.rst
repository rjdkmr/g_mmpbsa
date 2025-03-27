g_mmpbsa commands
====================

.. list-table:: List of sub-commands available in g_mmpbsa
    :widths: 1, 4
    :header-rows: 1
    :name: commands-table

    * - Command
      - Function

    * - `run <run.html>`_
      - Run MM/PBSA calculation using trajectory and tpr file.

    * - `apbs <apbs.html>`_
      - apbs program used in PB calculation.
      
    * - `energy2bfac <energy2bfac.html>`_
      - Generate PDB file where decomposed binding energy is written in B-factor column.

    * - `average <average.html>`_
      - Calculate final average binding energy including all energy terms. Supports multiple complexes at once.
      
    * - `correlation <correlation.html>`_
      - Same as average, but also calculates correlation between predicted and experimental binding energies.
      
    * - `decompose <decompose.html>`_
      - Calculate final average decomposed energies of residues with plots.
    
.. toctree::
   :maxdepth: 1
   :caption: Contents

   run <run.rst>
   apbs <apbs.rst>
   energy2bfac <energy2bfac.rst>
   average <average.rst>
   correlation <correlation.rst>
   decompose <decompose.rst>