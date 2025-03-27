Input Keywords for Polar Solvation Energy
=========================================


polar
-----

::

    polar          = yes

This will allow the calculation of polar solvation energy. The value can change to “no” if one doesn’t want to do polar calculation.

----
    
cfac
-----

::

    cfac           = 2

The factor by which molecular dimensions should expand to get a coarse grid dimensions. For detail see help of a APBS script `apbs-1.3-source/tools/manip/psize.py` (psize.py -h).

----

fadd
-----

::

    fadd           = 20

The amount (in Å) to add to molecular dimensions to get a fine grid dimensions. For detail see help of a APBS script `apbs-1.3-source/tools/manip/psize.py` (psize.py -h).

----

gridspace
----------

::

    gridspace      = 0.2
    
It specifies the value (in Å)  for  fine grid spacing. For detail see help of a APBS script `apbs-1.3-source/tools/manip/psize.py` (psize.py -h).

----
    
gmemceil
---------

::

    gmemceil       = 4000
    
Usage: Sets memory (in MB) which will be used per-processor for a calculation. For detail see help of a APBS script `apbs-1.3-source/tools/manip/psize.py` (psize.py -h).

----

PBsolver
---------

::

    PBsolver        = npbe

This specifies whether linear or nonlinear Poisson Boltzmann equation should be solved. The accepted keywords are `lpbe` and `npbe` for linear and traditional non-linear PB equation, respectively. The effects of different value on the polar calculation is checked during this implementation.

----

mg-type
--------

::

    mg-type        = mg-auto

How multigrid PB calculation should be performed? **Accepted keywords:** ``mg-auto`` and ``mg-para``.

.. note::
    * More details about ``mg-auto`` is provided `here on APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/elec/mg-auto.html>`_.
    * More details about ``mg-para`` is provided `here on APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/elec/mg-para.html>`_.



pcharge
-------

::

    pcharge        = 1

The charge of positive ions in bulk solution.

----

prad
----

::

    prad           = 0.95
    
Radius of positive ions.

----

pconc
-----
::

    pconc          = 0.150

Concentration of positive ion.

----

ncharge
-------

::

    ncharge        = -1

The charge of negative ions in bulk solution.

----

nrad
----
    
::

    nrad           = 1.81

Radius of negative ion.

----

nconc
-----

    nconc          = 0.150

Concentration of negative ion.

----

pdie
----
::

    pdie           = 4
    
The value of solute dielectric constant. This can be change depending on the solute used for calculation. For highly charged solute high dielectric value will produce more accurate polar solvation energy.

----

sdie
-----
::

    sdie           = 80

The value of solvent dielectric constant. 

----

vdie
-----
::

    vdie           = 1

The value of vacuum dielectric constant.

----

srad
-----
::

    srad           = 1.4 
    
This specify the radius (in Å)  of solvent molecules. This is used in case of probe-based surface definition.

.. note:: More details can be found on `APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/generic/srad.html>`_.


----

swin
-----

::

    swin           = 0.30

This specify the value for cubic spline window for spline-based surface definitions.
Not used when probe-based surface are used in calculation.

.. note:: More details can be found on `APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/generic/swin.html>`_.

-----

srfm 
-----

::

    srfm           = smol

This specify the model used to construct the dielectric and ion-accessibility coefficients.
The accepted keywords are ``mol``, ``smol``, ``spl2`` and ``spl4`` and it may affect the 
polar energy calculation.

.. note:: More details can be found on `APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/generic/srfm.html>`_.

----

sdens
------

::

    sdens          = 10

Specify the number of grid points per :math:`\text{Å}^2` for constructing the molecular surface or solvent accessible surface.
Not taken in consideration when ``srad = 0.0`` or ``srfm = spl2``.

.. note:: More details can be found on `APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/generic/sdens.html>`_.

----

temp
-----
::

    temp           = 300

This specify the temperature used for Poisson-Boltzmann calculation.

.. note:: More details can be found on `APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/generic/temp.html>`_.

----

chgm
-----
::

    chgm            = spl4

This specify the method used to map the biomolecular point charges to the grid for a 
multigrid Poisson-Boltzmann calculation. 
The accepted keywords are ``spl0``, ``spl2`` and ``spl4``. 
The effects of these keywords on energy are not tested in this implementation.

.. note:: More details can be found on `APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/generic/chgm.html>`_.

-----

bcfl
-----
::

    bcfl            = mdh

It specifies the type of boundary conditions used to solve the Poisson-Boltzmann equation.
The accepted keywords are ``zero``, ``sdh``, ``mdh``, ``focus``, and ``map``. 
However, use of ``focus``, and ``map`` will terminate g_mmpbsa with error. 
The change in ``bcfl`` keywords may affect the polar energy calculation.
The effects of these keywords on energy are not tested in this implementation.

.. note:: More details can be found on `APBS documentation <https://apbs.readthedocs.io/en/latest/using/input/old/generic/bcfl.html>`_.

