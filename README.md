### Introduction

g_mmpbsa is developed using two widely used open source software i.e. [GROMACS][GROMACS] and [APBS][APBS] and it has similar user interface like other GROMACS tools.
It is now upgraded as Python package for easy installation and usage.

g_mmpbsa contains several [sub-commands](https://g-mmpbsa.readthedocs.io/en/latest/commands/index.html) to carry out binding energy calculation using MM/PBSA method and
energetic contribution of each residue to the binding using energy decomposition scheme.

Now g_mmpbsa can be directly installed on **Linux** and **MacOS** using ``pip`` or ``pipx`` and it supports all the GROMACS versions.

#### For complete documentation, please visit [g_mmpbsa](https://g-mmpbsa.readthedocs.io/).
#### Please post problems and queries in [g_mmpbsa forum](https://groups.google.com/d/forum/g_mmpbsa).

***

#### Please always cite following two publications:

* Kumari _et al_ (2014) [g_mmpbsa - A GROMACS tool for high-throughput MM-PBSA calculations][g_mmpbsa paper]. _J. Chem. Inf. Model._ 54:1951-1962.

* Baker _et al._ (2001) [Electrostatics of nanosystems: Application to microtubules and the ribosome][apbs paper]. _Proc. Natl. Acad. Sci. USA_  98:10037-10041.

* Eisenhaber _et al._ (1995) [The double cubic lattice method: Efficient approaches to numerical integration of surface area and volume and to dot surface contouring of molecular assemblies][sasa paper] _J. Comput. Chem._ 16:273-284.

***

[OSDD]: http://www.osdd.net/
[GROMACS]: http://www.gromacs.org/
[APBS]: http://www.poissonboltzmann.org/
[forum]: https://groups.google.com/d/forum/g_mmpbsa
[g_mmpbsa paper]: http://pubs.acs.org/doi/abs/10.1021/ci500020m
[apbs paper]: http://www.pnas.org/content/98/18/10037.abstract
[gromacs paper]: http://bioinformatics.oxfordjournals.org/content/29/7/845.abstract
[sasa paper]: http://onlinelibrary.wiley.com/doi/10.1002/jcc.540160303/abstract