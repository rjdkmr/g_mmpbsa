#!/bin/bash
set -e -x

CWD=`pwd`

brew install gettext pyenv
brew install brewsci/bio/apbs
brew cleanup
export APBS=$(brew --prefix apbs)/bin/apbs
cd test

eval "$(pyenv init -)"
pyenv install --list
PYVERS=("3.9" "3.10" "3.11" "3.12")
PYTHONS=()
for PYVER in ${PYVERS[@]}
do
    # Install the latest release of the specified Python version using pyenv.
    PYVER="$(pyenv install --list | grep -E "^\\s*$PYVER" | sort -n -t. -k3 | tail -n1)"
    pyenv install $PYVER
    pyenv global $PYVER
    PYTHONS+=($PYVER)
    echo $PYVER
    echo $(python --version)
    python -m pip install --upgrade pip
    python -m pip install g-mmpbsa
    python -c "import g_mmpbsa; print('=====\nTEST -- g_mmpbsa GROMACS version: ', g_mmpbsa.gmx_version, '\n=====')"
    g_mmpbsa run -f data/1EBZ.xtc -s data/1EBZ.tpr -n data/1EBZ.ndx -pdie 2 -decomp -unit1 Protein -unit2 BEC
    g_mmpbsa run -f data/1EBZ.xtc -s data/1EBZ.tpr -n data/1EBZ.ndx -i polar_orig/mmpbsa.mdp -e 1000 -nomme -pbsa -decomp -unit1 Protein -unit2 BEC -pol polar.xvg -pcon contrib_pol.dat
    g_mmpbsa run -f data/1EBZ.xtc -s data/1EBZ.tpr -n data/1EBZ.ndx -i sasa_orig/mmpbsa.mdp -nomme -pbsa -decomp -unit1 Protein -unit2 BEC -apol sasa.xvg -apcon sasa_contrib.dat
    cat polar.xvg
    cat sasa.xvg
    cat energy_MM.xvg
    ls -lhrt
done
pyenv global system