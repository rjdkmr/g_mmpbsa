#!/usr/bin/env python
#
# This file is part of g_mmpbsa
#
# Author: Rajendra Kumar
#
#
# g_mmpbsa is a free software: you can redistribute it and/or modify
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
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#============================================================================

# Always prefer setuptools over distutils
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext, customize_compiler
import subprocess
import sys
import setuptools
import os
import glob
import sysconfig

sys.path.append(os.path.dirname(__file__))
here = os.path.abspath(os.path.dirname(__file__))

gromacs_flags = None
apbs_flags = None
extensions = None 
        
def check_gromacs_dirs():
    ''' Check for GROMACS directories and flags
    '''
    import pkgconfig
    
    # If gromacs not in pkgconfig, return from here
    if not pkgconfig.exists('libgromacs'):
        return
    
    out = dict()
    
    cppflags_t = pkgconfig.re.split('\s+', pkgconfig.cflags('libgromacs'))
    out['cppflags'] = []
    out['ldflags'] = []
    out['include'] = []
    out['lib_dirs'] = []
    out['libs'] = []
    
    # Extract include directory and CXXFLAGS
    for flags in cppflags_t:
        if '-I' in flags:
            out['include'].append(flags[2:])
        else:
            out['cppflags'].append(flags)
            
    # Extract lib directory and LDFLAGS
    ldflags_t = pkgconfig.re.split('\s+', pkgconfig.libs('libgromacs'))
    for flags in ldflags_t:
        if '-L' in flags:
            out['lib_dirs'].append(flags[2:])
        elif '-l' in flags:
            out['libs'].append(flags[2:])
        else:
            out['ldflags'].append(flags)
     
    return out

def include_gromacs_source_headers():
    global  gromacs_flags
    if 'GMX_SRC' not in os.environ:
        raise LookupError('GMX_SRC environment variable not found...')
        
    gmx_src = os.path.join(os.environ['GMX_SRC'], 'src')
    gromacs_flags['include'].append(gmx_src)
    for dir_name in glob.glob(f'{gmx_src}/gromacs/*/include'):
        gromacs_flags['include'].append(dir_name)
    

def extract_gromacs_flags():
    ''' Extract gromacs include, lib and other flags for compilation
    '''
    global  gromacs_flags
    
    # At first check if gromacs is already available in standard path
    if 'GMX_INSTALL' not in os.environ:
        gromacs_flags = check_gromacs_dirs()
        if gromacs_flags is not None:
            raise LookupError('Gromacs package not found... Use GMX_INSTALL environment variable to provide GROMACS path.')
    else:
        # If gromacs is not available at standard path check for GMX_INSTALL environment variable,
        # add it to pkg-config and then extract GROMACS directories and flags
        gmx_install = os.environ['GMX_INSTALL']
        if not os.path.isdir(gmx_install):
            raise LookupError('GROMACS directory {0} not exist...'.format(gmx_install))
        # Check lib name: it could be lib or lib64
        lib_dir = None
        for entry in os.listdir(gmx_install):
            if 'lib' in entry:
                lib_dir = entry
                break
        if lib_dir is None:
            raise LookupError(f'GROMACS lib directory not found in "{gmx_install}"...')
        
        os.environ['PKG_CONFIG_PATH'] = os.path.join(gmx_install, lib_dir, 'pkgconfig')
        gromacs_flags = check_gromacs_dirs()
    if gromacs_flags is None:
        raise LookupError("gromacs package not found")
        
    include_gromacs_source_headers()

def populate_apbs_flags():
    global apbs_flags
    apbs_flags = dict()
    apbs_flags['include'] = []
    apbs_flags['lib_dirs'] = []
    apbs_flags['ldflags'] = []

    if sys.platform == 'darwin':
        return
    
    if not 'APBS_INSTALL' in os.environ:
        raise LookupError('APBS_INSTALL environment variable not found...')
        
    apbs_install = os.environ['APBS_INSTALL']
    if not os.path.isdir(apbs_install):
        raise LookupError('APBS directory {0} not exist...'.format(apbs_install))
    
    # check lib64 or lib dir exit
    apbs_lib_dir = None
    for entry in os.listdir(apbs_install):
        if 'lib' in entry:
            apbs_lib_dir = entry
            break

    if apbs_lib_dir is None:
        raise LookupError(f'APBS lib directory not found in "{apbs_install}"...')
    
    apbs_lib_dir = os.path.join(apbs_install, apbs_lib_dir)
    libapbs_routines = None
    for entry in os.listdir(apbs_lib_dir):
        if 'libapbs_routines' in entry:
            libapbs_routines = os.path.join(apbs_lib_dir, entry)
            break

    if libapbs_routines is None:
        raise LookupError(f'libapbs_routines.so or libapbs_routines.a file not found in "{apbs_lib_dir}"...')
    
    print("Found libapbs_routines: ", libapbs_routines)

    expected_libs = [
        'libumfpack',
        'libspqr',
        'libcholmod',
        'libccolamd',
        'libcamd', 
        'libcolamd',
        'libamd',
        'libopenblas',
        'libopenblasp',
        'libsuitesparseconfig',
        'libarpack',
        'libmetis',
        'libmc',
        'libpunc',
        'libmaloc',
        'libvf2c',
        'libcgcode',
        'libsuperlu',
        'libiapbs',
        'libapbs_mg',
        'libapbs_fem',
        'libapbs_pmgc',
        'libapbs_generic',
        'libapbs_routines',
        ]
    
    libs_flags = []
    if 'CONDA_PREFIX' in os.environ:
        conda_prefix = os.environ['CONDA_PREFIX']

        for lib in expected_libs:
            path_to_lib = None
            if os.path.isfile(os.path.join(conda_prefix, 'lib', lib + '.so')):
                path_to_lib = os.path.join(conda_prefix, 'lib', lib + '.so')
            if os.path.isfile(os.path.join(conda_prefix, 'lib64', lib + '.so')):
                path_to_lib = os.path.join(conda_prefix, 'lib64', lib + '.so')
            if os.path.isfile(os.path.join(apbs_install, apbs_lib_dir, f'{lib}.so')):
                path_to_lib = os.path.join(apbs_install, apbs_lib_dir, f'{lib}.so')

            if path_to_lib is not None:
                libs_flags.append(f'-l{lib[3:]}')
            
    if sys.platform == 'linux' and len(libs_flags) == 0:
        process = subprocess.run(['ldd', libapbs_routines], capture_output=True, universal_newlines=True)
        if process.returncode != 0:
            raise RuntimeError('Error while running ldd on libapbs.so. Please check if APBS is installed correctly.')
        print("ldd output:", process.stdout)
        for lib in expected_libs:
            if lib in process.stdout:
                libs_flags.append(f'-l{lib[3:]}')  # Remove 'lib' prefix
        libs_flags.append('-lapbs_routines')


    apbs_flags['include'] += [os.path.join(apbs_install, 'include')]
    apbs_flags['lib_dirs'] += [os.path.join(apbs_install, apbs_lib_dir)]
    apbs_flags['ldflags'] += libs_flags

class get_pybind_include(object):
    """Helper class to determine the pybind11 include path

    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)

def get_extensions():
    '''Extensions that are need to be compiled
    '''
    global extensions
    src_file_names = ['src/pywrapper.cpp', 'src/mmpbsa.cpp', 'src/energy2bfac.cpp', 'src/nsc.cpp']

    if sys.platform == 'linux':
        src_file_names.append('src/apbs.cpp')

    extensions = [ Extension(
        'g_mmpbsa.g_mmpbsa',
        src_file_names,
        include_dirs=[ get_pybind_include(), get_pybind_include(user=True), 
                      'src', ] + gromacs_flags['include'] + apbs_flags['include'],
        library_dirs=gromacs_flags['lib_dirs'] + apbs_flags['lib_dirs'],
        libraries=gromacs_flags['libs'],
        runtime_library_dirs = gromacs_flags['lib_dirs']  + apbs_flags['lib_dirs'],
        language='c++',
        extra_link_args= gromacs_flags['ldflags'] + apbs_flags['ldflags'],
        ),]


# As of Python 3.6, CCompiler has a `has_flag` method.
# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    """Return the -std=c++[11/14] compiler flag.

    The c++14 is prefered over c++11 (when it is available).
    """
    if has_flag(compiler, '-std=c++17'):
        return '-std=c++17'
    elif has_flag(compiler, '-std=c++14'):
        return '-std=c++14'
    elif has_flag(compiler, '-std=c++11'):
        return '-std=c++11'
    else:
        raise RuntimeError('Unsupported compiler -- at least C++11 support '
                           'is needed!')


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    def build_extensions(self):
        
        # Check for -stdlib=libc++ on macos-clang
        if sys.platform == 'darwin':
            #check if compiler is clang or gcc
            cc_var = sysconfig.get_config_var("CC")
            if 'clang' in cc_var:
                self.c_opts['unix'] += ['-stdlib=libc++']
            elif 'g++' in cc_var or 'gcc' in cc_var:
                self.c_opts['unix'] += ['-stdlib=libstdc++']
            else:
                raise RuntimeError('Unsupported compiler type: {0}'.format(cc_var))

        if sys.platform == 'linux':
            self.c_opts['unix'] += [ '-static-libstdc++'] # Got From https://github.com/pypa/manylinux/issues/118
            self.c_opts['unix'] += [ '-DAPBS_INTERNAL' ]

        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())
        for ext in self.extensions:
            ext.extra_compile_args += opts

        if has_flag(self.compiler, '-fopenmp'):
            for ext in self.extensions:
                ext.extra_compile_args.append('-fopenmp')
                ext.extra_link_args.append('-fopenmp')

        # Remove "-Wstrict-prototypes" flags
        customize_compiler(self.compiler)
        try:
            self.compiler.compiler_so.remove("-Wstrict-prototypes")
        except (AttributeError, ValueError):
            pass

        build_ext.build_extensions(self)

populate_apbs_flags()
extract_gromacs_flags()
get_extensions()
setup(
    ext_modules=extensions,
    cmdclass={'build_ext': BuildExt},
    packages=find_packages(),
    include_package_data=True,
)
