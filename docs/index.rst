
.. |GROMACS| raw:: html

   <a href="http://www.gromacs.org/" target="_blank">GROMACS</a>

.. |APBS| raw:: html
   
   <a href="http://www.poissonboltzmann.org/" target="_blank">APBS</a>

.. |g_mmpbsa publication| raw:: html

   <a href="http://pubs.acs.org/doi/abs/10.1021/ci500020m" target="_blank">g_mmpbsa - A GROMACS tool for high-throughput MM-PBSA calculations</a>

.. |APBS publication| raw:: html
   
   <a href="http://www.pnas.org/content/98/18/10037.abstract" target="_blank">Electrostatics of nanosystems: Application to microtubules and the ribosome</a>

.. |SASA publication| raw:: html
   
   <a href="http://onlinelibrary.wiley.com/doi/10.1002/jcc.540160303/abstract" target="_blank">The double cubic lattice method: Efficient approaches to numerical integration of surface area and volume and to dot surface contouring of molecular assemblies</a>

.. |g_mmpbsa forum| raw:: html

   <a href="https://groups.google.com/d/forum/g_mmpbsa" target="_blank">g_mmpbsa forum</a>

Introduction
=============

g_mmpbsa is developed using two widely used open source software i.e. |GROMACS| and |APBS| and it has similar user interface like other GROMACS tools.

The tool calculates components of binding energy using MM-PBSA method except the entropic term and energetic contribution of each residue to the binding using energy decomposition scheme.

The output from the tool is used further as input in python scripts which is provided in this package, to get the final binding energy and energetic contribution of each residue.

Kindly post problems and queries in **|g_mmpbsa forum|**, we will try our best to provide the solution.


----

Please always cite following three publications
------------------------------------------------

* Kumari *et al* (2014) |g_mmpbsa publication| *J. Chem. Inf. Model.* 54:1951-1962.

* Baker *et al* (2001) |APBS publication| *Proc. Natl. Acad. Sci. USA*  98:10037-10041.

* Eisenhaber *et al.* (1995) |SASA publication| *J. Comput. Chem.* 16:273-284.


.. toctree::
   :maxdepth: 1
   :caption: Contents

      Download and Installation <install.rst>
      Getting started with MM/PBSA calculation <quick-run.rst>
      g_mmpbsa commands <commands/index>
      Solvation Energy Parameters <parameters/index>

