Non-polar Solvation Parameters
==============================

apolar
------

::
    
    apolar            = yes


This will specify whether to do non-polar calculation. Value ``yes`` will allow the calculation 
whereas value ``no`` will disable the calculation.

----

SASA model
----------

gamma
~~~~~

::

    gamma            =  0.02267

This specifies the surface tension proportionality term (:math:`\text{kJ mol}^{-1} \text{Å}^{-2}`) of the 
solvent. With ``gamma = 0``, SASA energy is **NOT** calculated.


sasaconst
~~~~~~~~~

::

    sasaconst        = 3.84928

Offset or constant :math:`c` in (:math:`\text{kJ mol}^{-1}`) from :math:`E =  \gamma . \text{SASA} + c`

sasrad
~~~~~~

::

    sasrad           = 1.4

Solvent probe radius in Å to calculate solvent accessible surface area.


----

SAV model
---------


press
~~~~~~~~~

::

    press            = 0.234304

This specifies the solvent pressure proportionality term (:math:`\text{kJ mol}^{-1} \text{Å}^{-3}`). 
With ``press = 0``, SAV energy is **NOT** calculated.

----

savrad
~~~~~~~~~

::

    savrad           = 1.29

Solvent probe radius in Å to calculate solvent accessible volume.

----

savconst
~~~~~~~~~

::

    savconst         = 0

Offset or constant :math:`c` in (:math:`\text{kJ mol}^{-1}`) from :math:`E =  \gamma . \text{SAV} + c`