``g_mmpbsa correlation``
=========================

It performs same operation as ``g_mmpbsa average`` but also calculates correlation between predicted 
and experimental binding energies. It used the same input files as ``g_mmpbsa average`` with the additional 
file containing experimental binding free energies.

It also fits a straight line to the data and calculates the correlation coefficient (R).

Command summary
---------------

::

    g_mmpbsa correlation [-h] [-mt] [-mf metafile.dat] [-m energy_MM.xvg] [-p polar.xvg] 
                         [-a apolar.xvg] [-bs] [-nbs NBSTEP] [-of full_energy.dat]
                         [-os summary_energy.dat] [-om meta_energy.dat] 
                         [-ep enplot.png] [-cd corrdist.dat] [-cp corrdist.png]

Description
-----------

``-mt, --multiple``
~~~~~~~~~~~~~~~~~~~~ 

If given, calculate for multiple complexes. Need Metafile containing path of energy files

----

``-mf metafile.dat, --metafile metafile.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Metafile containing path to energy files of each complex in a row obtained from g_mmpbsa 
in following order: [MM file] [Polar file] [ Non-polar file] [Ki]
Ki Should be in NanoMolar (nM)

----

``-m energy_MM.xvg, --molmech energy_MM.xvg``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vacuum Molecular Mechanics energy file obtained from g_mmpbsa

----

``-p polar.xvg, --polar polar.xvg``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Polar solvation energy file obtained from g_mmpbsa

----

``-a apolar.xvg, --apolar apolar.xvg``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Non-Polar solvation energy file obtained from g_mmpbsa

----

``-bs, --bootstrap``
~~~~~~~~~~~~~~~~~~~~~
If given, Enable Boot Strap analysis

----

``-nbs NBSTEP, --nbstep NBSTEP``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Number of boot strap steps for average energy calculation

----

``-of full_energy.dat, --outfr full_energy.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Energy File: Energy components frame wise

----

``-os summary_energy.dat, --outsum summary_energy.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Final Energy File: Full Summary of energy components

----

``-om meta_energy.dat, --outmeta meta_energy.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Final Energy File for Multiple Complexes: Complex wise final binding nergy

----

``-ep enplot.png, --enplot enplot.png``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Experimental Energy vs Calculated Energy Correlation Plot

----

``-cd corrdist.dat, --corrdist corrdist.dat``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Correlation distribution data from bootstrapping

----

``-cp corrdist.png, --corrplot corrdist.png``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Plot of correlation distribution