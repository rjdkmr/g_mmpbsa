#!/usr/bin/python
#
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
#

from __future__ import absolute_import, division, print_function
from builtins import range
from builtins import object

import re
import sys
import numpy as np
import argparse
import os
import csv

def main():
    args = ParseOptions()
    #File => Frame wise component energy
    try:
        frame_wise = open(args.outfr, 'w')
    except:
        raise IOError ('Could not open file {0} for writing. \n' .format(args.outfr))

    frame_wise.write('#Time E_VdW_mm(Protein)\tE_Elec_mm(Protein)\tE_Pol(Protein)\tE_Apol(Protein)\tE_VdW_mm(Ligand)\tE_Elec_mm(Ligand)\tE_Pol(Ligand)\tE_Apol(Ligand)\tE_VdW_mm(Complex)\tE_Elec_mm(Complex)\tE_Pol(Complex)\tE_Apol(Complex)\tDelta_E_mm\tDelta_E_Pol\tDelta_E_Apol\tDelta_E_binding\n')
    #Complex Energy
    c = []
    if args.multiple:
        MmFile, PolFile, APolFile = ReadMetafile(args.metafile)
        for i in range(len(MmFile)):
            cTmp = Complex(MmFile[i],PolFile[i],APolFile[i])
            cTmp.CalcEnergy(args,frame_wise,i)
            c.append(cTmp)
    else:
        cTmp = Complex(args.molmech,args.polar,args.apolar)
        cTmp.CalcEnergy(args,frame_wise,0)
        c.append(cTmp)
    #Summary in output files => "--outsum" and "--outmeta" file options
    Summary_Output_File(c, args)
    summary_output_csv(c, args)

class Complex(object):
    def __init__(self,MmFile,PolFile,APolFile):
        self.TotalEn = []
        self.Vdw, self.Elec, self.Pol, self.Sas, self.Sav =[], [], [], [], []
        self.MmFile = MmFile
        self.PolFile = PolFile
        self.APolFile = APolFile
        self.AvgEnBS = []
        self.CI = []
        self.FinalAvgEnergy = 0
        self.StdErr = 0

    def CalcEnergy(self,args,frame_wise,idx):
        mmEn = ReadData(self.MmFile,n=7)
        polEn = ReadData(self.PolFile,n=4)
        apolEn = ReadData(self.APolFile,n=10)
        CheckEnData(mmEn,polEn,apolEn)

        time, MM, Vdw, Elec, Pol, Apol, Sas, Sav = [], [], [], [], [], [], [], []
        for i in range(len(mmEn[0])):
            #Vacuum MM
            Energy = mmEn[5][i] + mmEn[6][i] - (mmEn[1][i] + mmEn[2][i] + mmEn[3][i] + mmEn[4][i])
            MM.append(Energy)
            Energy = mmEn[5][i] - (mmEn[1][i] + mmEn[3][i])
            Vdw.append(Energy)
            Energy = mmEn[6][i] - (mmEn[2][i] + mmEn[4][i])
            Elec.append(Energy)
            # Polar
            Energy = polEn[3][i] - (polEn[1][i] + polEn[2][i])
            Pol.append(Energy)
            #Non-polar
            Energy = apolEn[3][i] + apolEn[6][i] - (apolEn[1][i] + apolEn[2][i] + apolEn[4][i] + apolEn[5][i])
            Apol.append(Energy)
            Energy = apolEn[3][i] - (apolEn[1][i] + apolEn[2][i])
            Sas.append(Energy)
            Energy = apolEn[6][i] - (apolEn[4][i] + apolEn[5][i])
            Sav.append(Energy)
            #Final Energy
            time.append(mmEn[0][i])
            Energy = MM[i] + Pol[i] + Apol[i]
            self.TotalEn.append(Energy)

        # Writing frame wise component energy to file
        frame_wise.write('\n#Complex %d\n' % ( (idx+1)))
        for i in range(len(time)):
            frame_wise.write('%15.3lf %15.3lf %15.3lf %15.3lf %15.3lf' % (time[i], mmEn[1][i], mmEn[2][i], polEn[1][i], (apolEn[1][i] + apolEn[4][i])))
            frame_wise.write('%15.3lf %15.3lf %15.3lf %15.3lf'         %          (mmEn[3][i], mmEn[4][i], polEn[2][i], (apolEn[2][i] + apolEn[5][i])))
            frame_wise.write('%15.3lf %15.3lf %15.3lf %15.3lf'         %          (mmEn[5][i], mmEn[6][i], polEn[3][i], (apolEn[3][i] + apolEn[6][i])))
            frame_wise.write('%15.3lf %15.3lf %15.3lf %15.3lf\n'         % (MM[i], Pol[i], Apol[i], self.TotalEn[i]))

        #Bootstrap analysis energy components
        if(args.bootstrap):
            bsteps = args.nbstep
            avg_energy, error = BootStrap(Vdw,bsteps)
            self.Vdw.append(avg_energy)
            self.Vdw.append(error)
            avg_energy, error = BootStrap(Elec,bsteps)
            self.Elec.append(avg_energy)
            self.Elec.append(error)
            avg_energy, error = BootStrap(Pol,bsteps)
            self.Pol.append(avg_energy)
            self.Pol.append(error)
            avg_energy, error = BootStrap(Sas,bsteps)
            self.Sas.append(avg_energy)
            self.Sas.append(error)
            avg_energy, error = BootStrap(Sav,bsteps)
            self.Sav.append(avg_energy)
            self.Sav.append(error)
            
            #Bootstrap => Final Average Energy
            self.AvgEnBS, AvgEn, EnErr, CI = ComplexBootStrap(self.TotalEn,bsteps)
            self.FinalAvgEnergy = AvgEn
            self.StdErr = EnErr
            self.CI = CI
        #If not bootstrap then average and standard deviation
        else:
            self.Vdw.append(np.mean(Vdw))
            self.Vdw.append(np.std(Vdw))
            self.Elec.append(np.mean(Elec))
            self.Elec.append(np.std(Elec))
            self.Pol.append(np.mean(Pol))
            self.Pol.append(np.std(Pol))
            self.Sas.append(np.mean(Sas))
            self.Sas.append(np.std(Sas))
            self.Sav.append(np.mean(Sav))
            self.Sav.append(np.std(Sav))
            self.FinalAvgEnergy = np.mean(self.TotalEn)
            self.StdErr = np.std(self.TotalEn)


def Summary_Output_File(AllComplex,args):
    try:
        fs = open(args.outsum,'w')
    except:
        raise IOError ('Could not open file {0} for writing. \n' .format(args.outsum))

    if args.multiple:
        try:
            fm = open(args.outmeta,'w')
        except:
            raise IOError ('Could not open file {0} for writing. \n' .format(args.outmeta))
        fm.write('# Complex_Number\t\tTotal_Binding_Energy\t\tError\n')

    for n in range(len(AllComplex)):
        fs.write('\n\n#Complex Number: %4d\n' % (n+1))
        fs.write('===============\n   SUMMARY   \n===============\n\n')
        fs.write('\n van der Waal energy      = %15.3lf   +/-  %7.3lf kJ/mol\n' % (AllComplex[n].Vdw[0], AllComplex[n].Vdw[1]))
        fs.write('\n Electrostattic energy    = %15.3lf   +/-  %7.3lf kJ/mol\n' % (AllComplex[n].Elec[0],AllComplex[n].Elec[1]))
        fs.write('\n Polar solvation energy   = %15.3lf   +/-  %7.3lf kJ/mol\n' % (AllComplex[n].Pol[0], AllComplex[n].Pol[1]))
        fs.write('\n SASA energy              = %15.3lf   +/-  %7.3lf kJ/mol\n' % (AllComplex[n].Sas[0], AllComplex[n].Sas[1]))
        fs.write('\n SAV energy               = %15.3lf   +/-  %7.3lf kJ/mol\n' % (AllComplex[n].Sav[0], AllComplex[n].Sav[1]))
        fs.write('\n Binding energy           = %15.3lf   +/-  %7.3lf kJ/mol\n' % (AllComplex[n].FinalAvgEnergy, AllComplex[n].StdErr))
        fs.write('\n===============\n    END     \n===============\n\n')

        if args.multiple:
            fm.write('%5d %15.3lf %7.3lf\n' % (n+1 , AllComplex[n].FinalAvgEnergy, AllComplex[n].StdErr))

def summary_output_csv(AllComplex,args):
    try:
        fs = open(args.outcsv,'w')
    except:
        raise IOError ('Could not open file {0} for writing. \n' .format(args.outcsv))

    writer = csv.writer(fs)
    if AllComplex.length > 1:
        writer.writerow(['Complex ID','VdW Energy','VdW Error','Elec Energy','Elec Error','Pol Energy','Pol Error','SASA Energy','SASA Error','SAV Energy','SAV Error','Binding Energy','Binding Error'])
        for i, c in enumerate(AllComplex):
            writer.writerow([i+1,c.Vdw[0],c.Vdw[1],c.Elec[0],c.Elec[1],c.Pol[0],c.Pol[1],c.Sas[0],c.Sas[1],c.Sav[0],c.Sav[1],c.FinalAvgEnergy,c.StdErr])
    else:
        writer.writerow(["Energy",  "Average", "StD/Error",])
        writer.writerow(["VdW", AllComplex[0].Vdw[0], AllComplex[0].Vdw[1]])
        writer.writerow(["Elec", AllComplex[0].Elec[0], AllComplex[0].Elec[1]])
        writer.writerow(["Pol", AllComplex[0].Pol[0], AllComplex[0].Pol[1]])
        writer.writerow(["SASA", AllComplex[0].Sas[0], AllComplex[0].Sas[1]])
        writer.writerow(["SAV", AllComplex[0].Sav[0], AllComplex[0].Sav[1]])
        writer.writerow(["Binding", AllComplex[0].FinalAvgEnergy, AllComplex[0].StdErr])
        
    fs.close()

def CheckEnData(mmEn,polEn,apolEn):
    frame = len(mmEn[0])
    for i in range(len(mmEn)):
        if(len(mmEn[i]) != frame):
            raise ValueError("In MM file, size of columns are not equal.")

    for i in range(len(polEn)):
        if(len(polEn[i]) != frame):
            raise ValueError("In Polar file, size of columns are not equal.")

    for i in range(len(apolEn)):
        if(len(apolEn[i]) != frame):
            raise ValueError("In APolar file, size of columns are not equal.")


def ParseOptions():
    parser = argparse.ArgumentParser(prog='g_mmpbsa average', description='Calculate final average binding energy including all energy terms. Supports multiple complexes at once.')
    parser.add_argument("-mt", "--multiple", help='If given, calculate for multiple complexes. Need Metafile containing path of energy files', action="store_true")
    parser.add_argument("-mf", "--metafile", help='Metafile containing path to energy files of each complex in a row obtained from g_mmpbsa in following order: \
                                                       [MM file] [Polar file] [ Non-polar file] ',action="store", default='metafile.dat', metavar='metafile.dat')
    parser.add_argument("-m", "--molmech", help='Vacuum Molecular Mechanics energy file obtained from g_mmpbsa',action="store", default='energy_MM.xvg', metavar='energy_MM.xvg')
    parser.add_argument("-p", "--polar", help='Polar solvation energy file obtained from g_mmpbsa',action="store",default='polar.xvg', metavar='polar.xvg')
    parser.add_argument("-a", "--apolar", help='Non-Polar solvation energy file obtained from g_mmpbsa',action="store",default='apolar.xvg',metavar='apolar.xvg')
    parser.add_argument("-bs", "--bootstrap", help='If given, Enable Boot Strap analysis',action="store_true")
    parser.add_argument("-nbs", "--nbstep", help='Number of boot strap steps for average energy calculation',action="store", type=int, default=500, metavar=500)
    parser.add_argument("-of", "--outfr", help='Energy File: All energy components frame wise',action="store",default='full_energy.dat', metavar='full_energy.dat')
    parser.add_argument("-os", "--outsum", help='Final Energy File: Full Summary of energy components',action="store",default='summary_energy.dat', metavar='summary_energy.dat')
    parser.add_argument("-om", "--outmeta", help='Final Energy File for Multiple Complexes: Complex wise final binding nergy',action="store",default='meta_energy.dat',metavar='meta_energy.dat')
    parser.add_argument("-ocsv", "--outcsv", help='Final Energy File in CSV formate: Full Summary of energy components in CSV format.',action="store",default='summary_energy.csv', metavar='summary_energy.csv')

    if len(sys.argv) < 2:
        print('ERROR: No input files. Need help!!!')
        parser.print_help()
        sys.exit(1)

    args = parser.parse_args()

    if args.multiple:
        if not os.path.exists(args.metafile):
            print('\nERROR: {0} not found....\n' .format(args.metafile))
            parser.print_help()
            sys.exit(1)
    else:
        if not os.path.exists(args.molmech):
            print('\nERROR: {0} not found....\n' .format(args.molmech))
            parser.print_help()
            sys.exit(1)
        if not os.path.exists(args.polar):
            print('\nERROR: {0} not found....\n' .format(args.polar))
            parser.print_help()
            sys.exit(1)
        if not os.path.exists(args.apolar):
            print('\nERROR: {0} not found....\n' .format(args.apolar))
            parser.print_help()
            sys.exit(1)

    return args

def ReadData(FileName,n=2):
    try:
        infile = open(FileName,'r')
    except:
        raise IOError('Could not open file {0} for reading. \n' .format(FileName))

    x, data = [],[]
    for line in infile:
        line = line.rstrip('\n')
        if not line.strip():
            continue
        if(re.match('#|@',line)==None):
            temp = line.split()
            data.append(np.array(temp))
    for j in range(0,n):
        x_temp =[]
        for i in range(len(data)):
            try:
                value = float(data[i][j])
            except:
                raise FloatingPointError('\nCould not convert {0} to floating point number.. Something is wrong in {1}..\n' .format(data[i][j], FileName))

            x_temp.append(value)
        x.append(x_temp)
    return x

def ComplexBootStrap(x,step=1000):
    avg =[]
    x = np.array(x)
    n = len(x)
    idx = np.random.randint(0,n,(step,n))
    sample_x = x[idx]
    avg = np.sort(np.mean(sample_x,1))
    CI_min = avg[int(0.005*step)]
    CI_max = avg[int(0.995*step)]
    #print('Energy = %13.3f; Confidance Interval = (-%-5.3f / +%-5.3f)\n' % (np.mean(avg), (np.mean(avg)-CI_min), (CI_max-np.mean(avg))))
    return avg, np.mean(avg), np.std(avg), [(np.mean(avg)-CI_min), (CI_max-np.mean(avg))]

def BootStrap (x,step=1000):
    if(np.mean(x)) == 0:
        return 0.000, 0.000
    else:
        avg =[]
        x = np.array(x)
        n = len(x)
        idx = np.random.randint(0,n,(step,n))
        sample_x = x[idx]
        avg = np.sort(np.mean(sample_x,1))
        return np.mean(avg),np.std(avg)

def find_nearest_index(array,value):
    idx = (np.abs(array-value)).argmin()
    return idx

def ReadMetafile(metafile):
    MmFile,PolFile, APolFile = [], [], []
    FileList = open(metafile,'r')
    for line in FileList:
        line = line.rstrip('\n')
        if not line.strip():
            continue
        temp = line.split()
        MmFile.append(temp[0])
        PolFile.append(temp[1])
        APolFile.append(temp[2])

        if not os.path.exists(temp[0]):
            raise IOError('Could not open file {0} for reading. \n' .format(temp[0]))

        if not os.path.exists(temp[1]):
            raise IOError('Could not open file {0} for reading. \n' .format(temp[1]))

        if not os.path.exists(temp[2]):
            raise IOError('Could not open file {0} for reading. \n' .format(temp[2]))

    return MmFile, PolFile, APolFile

if __name__=="__main__":
    main()
