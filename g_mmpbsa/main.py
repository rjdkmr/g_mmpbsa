# This file is part of g_mmpbsa.
#
# Copyright (C) 2013-2021 Rashmi Kumari and Andrew Lynn
# Copyright (C) 2022-2025 Rajendra Kumar and Rashmi Kumari
#
# g_mmpbsa is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# g_mmpbsa is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with g_mmpbsa.  If not, see <http://www.gnu.org/licenses/>.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
 
import sys
import os
from .g_mmpbsa import internal_apbs_exist

def main():
    options = {'run': 'Run MM/PBSA calculation using trajectory and tpr file',
               'energy2bfac': 'Generate PDB file where decomposed binding energy is written in B-factor column',
               'average': 'Calculate final average binding energy including all energy terms. Supports multiple complexes at once.',
               'correlation'   : 'Same as average, but also calculates correlation between predicted and experimental binding energies.',
               'decompose': 'Calculate final average decomposed energies of residues with plots.',
              }

    program = sys.argv[0]
    if internal_apbs_exist():
        os.environ['APBS'] = f'{program} apbs' # set-up APBS env which will be later used in mmpbsa
        options['apbs'] = 'Run APBS calculation using APBS included with this package.'
 
    if len(sys.argv)<=1:
        show_help(options)
        sys.exit(-1)

    if sys.argv[1] not in options:
        print(' ERROR: "{0}" is not an accepted option.\n' .format(sys.argv[1]))
        show_help(options)
        sys.exit(-1)

    if sys.argv[1] == 'run':
        from .g_mmpbsa import mmpbsa
        if 'APBS' not in os.environ and not internal_apbs_exist():
            raise RuntimeError('APBS is not set in the environment. Please install and set APBS environment variable.')
        mmpbsa([program + ' run'] + sys.argv[2:])
        
    if sys.argv[1] == 'apbs':
        if not internal_apbs_exist():
            raise RuntimeError('APBS is not included with this package.')
        from .g_mmpbsa import apbs
        apbs(['apbs'] + sys.argv[2:])
    
    if sys.argv[1] == 'energy2bfac':
        from .g_mmpbsa import energy2bfac
        energy2bfac([program + ' energy2bfac'] + sys.argv[2:])
        
    if sys.argv[1] == 'average':
        from . import average
        sys.argv = sys.argv[1:]
        average.main()
        
    if sys.argv[1] == 'correlation':
        from . import correlation
        sys.argv = sys.argv[1:]
        correlation.main()
        
    if sys.argv[1] == 'decompose':
        from . import decompose
        sys.argv = sys.argv[1:]
        decompose.main()
        
def show_help(options):
    print(' ==============================')
    print(' Usage:')
    print(' g_mmpbsa <Option>\n')
    print(' ---------------------')
    print(' Use following options:')
    print(' ---------------------\n')

    for tool in options:
        print('\t{0} : {1}\n'.format(tool, options[tool]))

    print(' ==============================')


if __name__=="__main__":
    main()
