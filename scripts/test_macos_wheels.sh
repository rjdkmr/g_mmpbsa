#!/bin/bash
set -e -x

CWD=`pwd`

brew install pipx
brew install brewsci/bio/apbs
brew cleanup
export APBS=$(brew --prefix apbs)/bin/apbs

cd test

pipx install g-mmpbsa==3.0.7
g_mmpbsa run -f data/1EBZ.xtc -s data/1EBZ.tpr -n data/1EBZ.ndx -pdie 2 -decomp -unit1 Protein -unit2 BEC
g_mmpbsa run -f data/1EBZ.xtc -s data/1EBZ.tpr -n data/1EBZ.ndx -i polar_orig/mmpbsa.mdp -e 1000 -nomme -pbsa -decomp -unit1 Protein -unit2 BEC -pol polar.xvg -pcon contrib_pol.dat
g_mmpbsa run -f data/1EBZ.xtc -s data/1EBZ.tpr -n data/1EBZ.ndx -i sasa_orig/mmpbsa.mdp -nomme -pbsa -decomp -unit1 Protein -unit2 BEC -apol sasa.xvg -apcon sasa_contrib.dat
cat polar.xvg
cat sasa.xvg
cat energy_MM.xvg
ls -lhrt
