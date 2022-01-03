#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

#include <gromacs/trajectoryanalysis.h>
#include "gromacs/trajectoryanalysis/topologyinformation.h"
#include "gromacs/topology/exclusionblocks.h"
#include "gromacs/utility/futil.h"

#include "gromacs/utility/filestream.h"
#include "gromacs/utility/cstringutil.h"
#include "gromacs/fileio/readinp.h"
#include "gromacs/fileio/warninp.h"

using namespace gmx;

/**
* @brief Data structure to hold information about non-bonded interactions
* @author Rashmi Kumari
*/
typedef struct	{
    int nr_nb; /**< Number of non-bonded pairs excluding 1-2 and 1-3 pairs */
    int nr_14; /**<  Number of 1-4 pairs */
    int **pairNB;  /**< 2D array of of size `(nr_nb, 2)` containing all atom index of non-bonded pairs */
    int **pair14;  /**< 2D array of size `(nr_14, 2)` containing all 1-4 pairs atom index */
    int *pairtype; /**< array of pair-types for 1-4 Lenard-Jones parameters */
    bool *bItsA14; /**< To classify that first Atom of pairs is from subunit A in 1-4 pair list */
    bool *bItsA;   /**< To classify that first Atom of pairs is from subunit A in non-bonded pair list */
    bool *bItsB14; /**< To classify that second Atom of pairs is from subunit B in 1-4 pair list */
    bool *bItsB;   /**< To classify that second Atom of pairs is from subunit B in non-bonded pair list */

} t_non_bonded;

/**
* @brief Data structure to hold information about polar solvation energy calculations
* @author Rashmi Kumari
*/
typedef struct {
    real cfac;            /**< Factor by which molecular dimensions should expand to get a coarse grid dimensions */
    real gridspace;       /**< Fine grid spacing in Angstrom */
    real gmemceil;        /**<  Maximum memory (MB) available*/
    real fadd;            /**<  The amount (in Å) to add to molecular dimensions to get a fine grid dimensions */
    real ofrac;           /**<  Used in mg-para: Overlap in mesh during parallel calculations */
    bool bParallel;   /**<  Whether parallel calculation is required */
    bool bFocus;      /**<  Whether focus type is enabled */

    int mg_type;          /**<  multi-grid calculation type */
    ivec dime;            /**<  grid dimenstions */
    ivec pdime;           /**<  grid dimenstions for parallel calculations */
    rvec cglen;           /**<  coarse grid lengths */
    rvec cgcent;          /**<  center of coarse-grid box */
    rvec fglen;           /**<  fine grid lengths */
    rvec fgcent;          /**<  center of fine grid box */
    int pbsolver;         /**<  PB Solver type: linear or non-linear */
    int bcfl;             /**<  Type of boundary conditions */
    real pcharge;         /**<  Magnitude of positive charge */
    real ncharge;         /**<  Magnitude of negative charge */
    real prad;            /**<  Radius of positive charged ion */
    real nrad;            /**<  Radius of negative charged ion */
    real pconc;           /**<  Concentration of positive charge */
    real nconc;           /**<  Concentration of negative charge */
    real pdie;            /**<  Solute dielectric constant */
    real sdie;            /**<  Solvent dielectric constant */
    real vdie;            /**<  Vacuum or reference dielectric constant*/
    int srfm;             /**<  To construct the dielectric and ion-accessibility coefficients */
    int chgm;             /**<  To map the biomolecular point charges to the grid */
    real sdens;           /**<  Number of grid points per Å^2 for constructing the molecular surface or solvent accessible surface */
    real srad;            /**<  Radius (in Å) of solvent molecules */
    real swin;            /**<  Value for cubic spline window for spline-based surface definitions */
    real temp;            /**<  Temperature in K */

    // Apolar calculation
    real   press;       /**<  Solvent pressure proportionality term of SAV model */
    real   savconst;    /**<  Offset or constant of SAV model */
    real   savrad;      /**<  Solvent radius for SAV */
    real   gamma;       /**<  Surface tension proportionality term of SASA model */
    real   sasaconst;   /**<  Offset or constant of SASA mode */
    real   sasrad;      /**<  Solvent radius for SASA */

} t_pbsa_inputs;


/**
* @brief index for radius values in 2D array
* @ingroup PBSA_PREP
* @author Rashmi Kumari
*/
enum { eSASRAD, /**< For SASA model */
       eSAVRAD, /**< For SAV model */
       eWCARAD, /**< For WCA model */
     };

/**
* @brief index for PB solver
* @ingroup PBSA_INPUT
* @author Rashmi Kumari
*/
enum {eLPBE, /**< Linear solver */ eNPBE  /**< Non-linear solver */};
static const char *PBsolver[] = { "lpbe", "npbe", NULL };

/**
* @brief index of keywords to boundary condition keywords
* @ingroup PBSA_INPUT
* @author Rashmi Kumari
*/
enum {ezero, esdh, emdh};
static const char *bcfl_words[] = { "zero", "sdh", "mdh", NULL };

/**
* @brief index of keywords to method for mapping the biomolecular point charges to the grid
* @ingroup PBSA_INPUT
* @author Rashmi Kumari
*/
enum { espl2, espl4, emol, esmol};
static const char *srfm_words[] = { "spl2", "spl4",  "mol", "smol", NULL};

enum {espl0 = 2};
static const char *chgm_words[] = {"spl2", "spl4", "spl0", NULL};

enum { sacc };
static const char *APsrfm_words[] = { "sacc", NULL};

enum { mg_auto, mg_para };
static const char *mg_words[] = { "mg-auto", "mg-para", NULL};

/*! \brief
 * Template class to serve as a basis for user analysis tools.
 */
class AnalysisTemplate : public TrajectoryAnalysisModule
{
public:
    AnalysisTemplate();

    virtual void initOptions(IOptionsContainer  *options,  TrajectoryAnalysisSettings *settings);
    virtual void optionsFinished(TrajectoryAnalysisSettings * settings);
    virtual void initAnalysis(const TrajectoryAnalysisSettings &settings, const TopologyInformation &top);
    virtual void analyzeFrame(int frnr, const t_trxframe &fr, t_pbc *pbc, TrajectoryAnalysisModuleData *pdata);
    virtual void finishAnalysis(int nframes);
    virtual void writeOutput();

private:
    class ModuleData;

    real                             rvdw_;
    real                             pdie_;
    int                              ndots_;
    bool                             bDIFF_;
    bool                             bMM_;
    bool                             bDCOMP_;
    bool                             bPBSA_;
    bool                             bFocus_;
    bool                             bIncl14_;
    bool                             bVerbose_;

    std::string                      fnMDP_;
    std::string                      fnDist_;
    std::string                      fnVacMM_;
    std::string                      fnDecompMM_;
    std::string                      fnDecompPol_;
    std::string                      fnDecompAPol_;
    std::string                      fnPolar_;
    std::string                      fnAPolar_;

    Selection                        selA_;
    Selection                        selB_;

    FILE                             *fDecompMM_;
    FILE                             *fDecompAPol_;
    FILE                             *fDecompPol_;

    t_non_bonded                     paramNonBond_;
    t_pbsa_inputs                    pbsaInputKwords_;
    bool                             bPolar_ = false;
    bool                             bApolar_ = false;

    AnalysisNeighborhood             nb_;
    AnalysisData                     mmEnergyData_;

    // Varaible related to topology
    const gmx_mtop_t                 *mtop_;
    const gmx_localtop_t             *localtop_;
    const t_atoms                    *atoms_;

    //Variable for index file
    int *isize_, foc_isize;
    const int **index_, *foc_index;

    std::vector<bool>                 bResA_ = { false }; // wheather a residue in subunit A
    std::vector<bool>                 bResB_ = { false }; // wheather a residue in subunit B
    
    std::vector<real>                 radiiA_;
    std::vector<real>                 radiiB_;
    std::vector<real>                 radiiAB_;


    std::vector<real>              EEnergyFrame_ = { 0 };
    std::vector<real>              VdwEnergyFrame_ = { 0 };

    void buildNonBondedPairList();
    void readPBSAInputs();
    void vaccumMMFull(rvec *x);
    void vaccumMMWithoutExclusions(rvec *x);
    void prepareOutputFiles(const TrajectoryAnalysisSettings *settings);
    void assignRadius();

};


AnalysisTemplate::AnalysisTemplate()
{
    registerAnalysisDataset(&mmEnergyData_, "mmEnergy");
}

void
AnalysisTemplate::initOptions(IOptionsContainer          *options,
                              TrajectoryAnalysisSettings *settings)
{
    static const char *const desc[] = {
        "g_mmpbsa calculates relative binding free energy using the MM-PBSA method for "
        "bio-molecular associations such as protein-protein, protein-ligand, protein-DNA etc. "
        "It calculates three components of the binding energy in separate files, so that",
        "user will have choice to calculate MM, PB and SA energy values according to their ",
        "objective. It also calculates contribution of each residue to the net binding energy",
        "and provides information about important contributing residues to the molecular",
        "association.\n\n",
        "For more detail, see please visit <http://rashmikumari.github.io/g_mmpbsa>"
    };

    settings->setHelpText(desc);

    options->addOption(FileNameOption("i")
                       .legacyType(efMDP).inputFile()
                       .store(&fnMDP_).defaultBasename("pbsa")
                       .description("Input parameters for mmpbsa calculations."));

    options->addOption(FileNameOption("mm")
                       .filetype(eftPlot).outputFile()
                       .store(&fnVacMM_).defaultBasename("energy_MM")
                       .description("Vaccum MM energy as a function of time"));

    options->addOption(FileNameOption("pol")
                       .filetype(eftPlot).outputFile()
                       .store(&fnPolar_).defaultBasename("polar")
                       .description("Polar solvation energy as a function of time"));

    options->addOption(FileNameOption("apol")
                       .filetype(eftPlot).outputFile()
                       .store(&fnAPolar_).defaultBasename("apolar")
                       .description("Apolar solvation energy as a function of time"));

    options->addOption(FileNameOption("mmcon")
                       .filetype(eftGenericData).outputFile()
                       .store(&fnDecompMM_).defaultBasename("contrib_MM")
                       .description("Vacuum MM energy contribution to binding"));

    options->addOption(FileNameOption("pcon")
                       .filetype(eftGenericData).outputFile()
                       .store(&fnDecompPol_).defaultBasename("contrib_pol")
                       .description("Polar solvation energy contribution to binding"));


    options->addOption(FileNameOption("apcon")
                       .filetype(eftGenericData).outputFile()
                       .store(&fnDecompAPol_).defaultBasename("contrib_apol")
                       .description("Apolar solvation energy contribution to binding"));

    options->addOption(FileNameOption("o")
                       .filetype(eftPlot).outputFile()
                       .store(&fnDist_).defaultBasename("avedist")
                       .description("Average distances from reference group"));

    options->addOption(SelectionOption("unit1")
                       .store(&selA_).required()
                       .description("Select protein or first group"));

    options->addOption(SelectionOption("unit2")
                       .store(&selB_)
                       .description("Select ligand or second group"));

    options->addOption(RealOption("rvdw").store(&rvdw_).defaultValue(0.1)
                       .description("Default van der Waal radius (in nm) if radius not found for any atom-types)"));

    options->addOption(RealOption("pdie").store(&pdie_).defaultValue(1)
                       .description("Dielectric constant of solute. Should be same as of polar solvation"));

    options->addOption(IntegerOption("ndots").store(&ndots_).defaultValue(24)
                       .description("Number of dots per sphere in the calculation of SASA, more dots means more accuracy"));

    options->addOption(BooleanOption("mme").store(&bMM_).defaultValue(true)
                       .description("To calculate vacuum molecular mechanics energy"));

    options->addOption(BooleanOption("pbsa").store(&bPBSA_).defaultValue(false)
                       .description("To calculate polar and/or non-polar solvation energy"));

    options->addOption(BooleanOption("diff").store(&bDIFF_).defaultValue(true)
                       .description("Calculate the energy difference between two group otherwise only calculates for one group"));

    options->addOption(BooleanOption("decomp").store(&bDCOMP_).defaultValue(false)
                       .description("Number of dots per sphere in the calculation of SASA, more dots means more accuracy"));

    options->addOption(BooleanOption("incl_14").store(&bIncl14_).defaultValue(false)
                       .description("Include 1-4 atom-pairs, exclude 1-2 and 1-3 atom pairs during MM calculation. Should be \"yes\" when groups are bonded with each other."));

    options->addOption(BooleanOption("focus").store(&bFocus_).defaultValue(false)
                       .description("To enable focusing on the specfic region of molecule, group of atoms must be provided in index file"));

    options->addOption(BooleanOption("silent").store(&bVerbose_).defaultValue(false)
                       .description("Display messages, output and errors from external APBS program"));

    settings->setFlag(TrajectoryAnalysisSettings::efRequireTop);
    settings->setFlag(settings->efNoUserPBC,true);
    settings->setFlag(settings->efNoUserRmPBC, true);
}

void AnalysisTemplate::optionsFinished(TrajectoryAnalysisSettings *)
{
    if(bDIFF_)  {
        snew(isize_, 3);
        snew(index_, 3);
    } else {
        snew(isize_, 1);
        snew(index_, 1);
    }

    if ((!bDIFF_) && (bDCOMP_)) {
        printf("\n\nWARNING: For single group calculations, decompositon cannot be used, switching it off!   \n\n");
        bDCOMP_ = false;
    }

    if ((fnDecompMM_.empty()) && (bDCOMP_)) {
        GMX_THROW(InconsistentInputError("Decompositon requested, however. -mmcon option is missing. Aborting!!!\n"));
    }

    if ((!bPBSA_) && (!bMM_))   {
        GMX_THROW(InconsistentInputError("No calculation opted. Use either \"-pbsa\" or \"-mm\" option.\n"));
    }

    if ((bMM_) && (!bDIFF_) && (!bIncl14_))
    {
        printf("\n\nWARNING: For single group calculations, 1-4 interactions are also included. \n\n");
        bIncl14_ = TRUE;

    }

    if ((bPBSA_) && (fnMDP_.empty())) {
        GMX_THROW(InconsistentInputError("Input parameter file for the PBSA calculation is missing, use \"-i\" option\n"));
    }

    if(bPBSA_) readPBSAInputs();
}


void
AnalysisTemplate::initAnalysis(const TrajectoryAnalysisSettings &settings,
                               const TopologyInformation         &topInfo)
{
    if (!selA_.isValid()) {
        GMX_THROW(InconsistentInputError("No atoms are selected in first group!"));
    }

    if ((!selB_.isValid()) && (bDIFF_)) {
        GMX_THROW(InconsistentInputError("-diff is requested but no atoms are selected in second group!"));
    }

    mtop_ = topInfo.mtop();
    localtop_ = topInfo.expandedTopology();
    atoms_ = topInfo.atoms();

    // Building complex atom indices
    if (bDIFF_) {
        isize_[0] = selA_.atomCount();
        isize_[1] = selB_.atomCount();
        isize_[2] = isize_[0] + isize_[1];

        index_[0] = selA_.atomIndices().data();
        index_[1] = selB_.atomIndices().data();

        int *tmpIndex;
        snew(tmpIndex, isize_[0] + isize_[1]);
        for (int i = 0; i < isize_[0]; i++)
            tmpIndex[i] = index_[0][i];
        for (int i = 0; i < isize_[1]; i++)
            tmpIndex[i + isize_[0]] = index_[1][i];

        index_[2] = tmpIndex;

    }

    {   // building bResA_ and bResB_ array here
        if (bDCOMP_) {
            int nres = 0 ; //nres = total number of residue
            std::vector<int> resnmr(atoms_->nres); //resnmr = residue number
            std::vector<int> ResIstart(atoms_->nres);  // Atom indeices at which each residue start
            int prev_res, curr_res;

            bResA_.resize(atoms_->nres);
            bResB_.resize(atoms_->nres);
            std::fill(bResA_.begin(), bResA_.end(), false);
            std::fill(bResB_.begin(), bResB_.end(), false);

            resnmr[0] = atoms_->resinfo[atoms_->atom[index_[2][0]].resind].nr;
            nres = 1;
            ResIstart[0] = index_[2][0];
            prev_res = atoms_->atom[index_[2][0]].resind;
            bResA_[atoms_->atom[index_[2][0]].resind] = TRUE;
            for(int i=0; i<isize_[2]; i++)
            {
                curr_res = atoms_->atom[index_[2][i]].resind;
                if(curr_res != prev_res)
                {
                    nres++;
                    resnmr[nres-1] = atoms_->resinfo[curr_res].nr;
                    ResIstart[nres-1] = index_[2][i];

                    if(i<isize_[0])
                        bResA_[atoms_->atom[index_[2][i]].resind] = TRUE;
                    else
                        bResB_[atoms_->atom[index_[2][i]].resind] = TRUE;

                    prev_res = curr_res;
                }
            }
        }
    }

    prepareOutputFiles(&settings);

    if (bIncl14_) buildNonBondedPairList();
}


void
AnalysisTemplate::analyzeFrame(int frnr, const t_trxframe &fr, t_pbc *pbc,
                               TrajectoryAnalysisModuleData *pdata)
{
    AnalysisDataHandle   mmh         = pdata->dataHandle(mmEnergyData_);

    // Electrostatic energy calculation
    if (bMM_) {
        if (bIncl14_) {
            vaccumMMFull(fr.x);
        } else {
            vaccumMMWithoutExclusions(fr.x);
        }



        mmh.startFrame(frnr, fr.time);
        if (bDIFF_) {
            for(int i=0; i<3; i++)  {
                mmh.setPoint((i*2), VdwEnergyFrame_[i]);
                mmh.setPoint((i*2)+1, EEnergyFrame_[i]);
            }
            mmh.setPoint(6, (VdwEnergyFrame_[2] + EEnergyFrame_[2]) - (EEnergyFrame_[0]+VdwEnergyFrame_[0]+EEnergyFrame_[1]+VdwEnergyFrame_[1]));
        } else {
            mmh.setPoint(0, VdwEnergyFrame_[0]);
            mmh.setPoint(1, EEnergyFrame_[0]);
            mmh.setPoint(2, EEnergyFrame_[0] + VdwEnergyFrame_[0]);
        }
        mmh.finishFrame();


        if(bDCOMP_) {
            fprintf(fDecompMM_, "%15.3lf", fr.time);

            for(int i = 0; i < atoms_->nres; i++)
                if( (bResA_[i]) || (bResB_[i]) )
                    fprintf(fDecompMM_, "%15.3lf", (EEnergyFrame_[i+3] + VdwEnergyFrame_[i+3])/2);
            fprintf(fDecompMM_, "\n");
            fflush(fDecompMM_);
        }
    }




}


void
AnalysisTemplate::finishAnalysis(int /*nframes*/)
{
}


void
AnalysisTemplate::writeOutput()
{

}

void AnalysisTemplate::buildNonBondedPairList()
{

    int i, j=0, k,l, ii, jj;
    const int *index;
    bool *globalIndex;
    int isize=0, splitIndex=0;
    gmx_bool bExclude=FALSE, bLJ14=FALSE;

    paramNonBond_.nr_nb = 1;
    paramNonBond_.nr_14 = 1;
    paramNonBond_.pairNB = (int**) malloc(sizeof(int*));
    paramNonBond_.pair14 = (int**) malloc(sizeof(int*));
    paramNonBond_.pairtype = (int*) malloc(sizeof(int));

    if(bDIFF_)
    {
        snew(paramNonBond_.bItsA,1);
        snew(paramNonBond_.bItsB,1);
        snew(paramNonBond_.bItsA14,1);
        snew(paramNonBond_.bItsB14,1);

        index = index_[2];
        isize = isize_[2];
        splitIndex = isize_[0];
    } else {
        index = index_[0];
        isize = isize_[0];
    }

    snew(globalIndex, atoms_->nr);
    for(i = 0; i < atoms_->nr; i ++)
        globalIndex[i] = false;

    for(i = 0; i < isize; i++)
        globalIndex[index[i]] = true;

    k = 0;
    while(k < localtop_->idef.il[F_LJ14].size())
    {
        ii = localtop_->idef.il[F_LJ14].iatoms[k+1];
        jj = localtop_->idef.il[F_LJ14].iatoms[k+2];

        if ((globalIndex[ii]) && (globalIndex[jj]))
        {
            paramNonBond_.pairtype = (int*) realloc(paramNonBond_.pairtype,(paramNonBond_.nr_14*sizeof(int)));
            paramNonBond_.pairtype[paramNonBond_.nr_14-1] = k;

            paramNonBond_.pair14 = (int**) realloc(paramNonBond_.pair14, (paramNonBond_.nr_14*sizeof(int*)));
            paramNonBond_.pair14[paramNonBond_.nr_14-1] = (int*) malloc(2*sizeof(int));
            paramNonBond_.pair14[paramNonBond_.nr_14-1][0] = ii;
            paramNonBond_.pair14[paramNonBond_.nr_14-1][1] = jj;
            bLJ14=TRUE;

            if(bDIFF_)	{
                srenew(paramNonBond_.bItsA14, paramNonBond_.nr_14);
                srenew(paramNonBond_.bItsB14, paramNonBond_.nr_14);
                paramNonBond_.bItsA14[paramNonBond_.nr_14-1] = FALSE;
                paramNonBond_.bItsB14[paramNonBond_.nr_14-1] = FALSE;

                if(( ii < splitIndex) && (  jj  < splitIndex ))
                {
                    paramNonBond_.bItsA14[paramNonBond_.nr_14-1] = TRUE;
                    paramNonBond_.bItsB14[paramNonBond_.nr_14-1] = FALSE;
                }

                if(( ii >= splitIndex) && ( jj >= splitIndex))
                {
                    paramNonBond_.bItsB14[paramNonBond_.nr_14-1] = TRUE;
                    paramNonBond_.bItsA14[paramNonBond_.nr_14-1] = FALSE;
                }
            }
            paramNonBond_.nr_14++;
        }
        k= k+3;
    }


    float progress=0;
    for(i = 0; i < isize; i++)
    {
        for(j=0; j < i; j++)	{
            bExclude=FALSE;

            for(l = 0; l < localtop_->excls[index[i]].size(); l++) {
                if(index[j] == localtop_->excls[index[i]][l])
                {
                    bExclude=TRUE;
                    break;
                }
            }

            if(!bExclude)	{
                paramNonBond_.pairNB = (int**) realloc(paramNonBond_.pairNB, (paramNonBond_.nr_nb*sizeof(int*)));
                paramNonBond_.pairNB[paramNonBond_.nr_nb-1] = (int*) malloc(2*sizeof(int));
                paramNonBond_.pairNB[paramNonBond_.nr_nb-1][0] = index[i];
                paramNonBond_.pairNB[paramNonBond_.nr_nb-1][1] = index[j];

                if(bDIFF_)	{
                    srenew(paramNonBond_.bItsA,paramNonBond_.nr_nb);
                    srenew(paramNonBond_.bItsB,paramNonBond_.nr_nb);
                    paramNonBond_.bItsA[paramNonBond_.nr_nb-1] = FALSE;
                    paramNonBond_.bItsB[paramNonBond_.nr_nb-1] = FALSE;
                    if(( i <  splitIndex) && ( j  <  splitIndex))
                    {
                        paramNonBond_.bItsA[paramNonBond_.nr_nb-1] = TRUE;
                        paramNonBond_.bItsB[paramNonBond_.nr_nb-1] = FALSE;
                    }
                    if(( i  >= splitIndex) && ( j  >= splitIndex))
                    {
                        paramNonBond_.bItsB[paramNonBond_.nr_nb-1] = TRUE;
                        paramNonBond_.bItsA[paramNonBond_.nr_nb-1] = FALSE;
                    }
                }
                paramNonBond_.nr_nb++;
            }
        }
        progress = ((float)i/(float)isize) * 100;
        fprintf(stderr,"\r %5.0f %% completed...",progress);
        fflush(stdout);
    }
    printf("\n Finished pair generation....\nTotal %d 1-4 pairs and %d non-bonded pairs generated.\n\n",paramNonBond_.nr_14-1,paramNonBond_.nr_nb-1);

    paramNonBond_.nr_14 = paramNonBond_.nr_14-1;
    paramNonBond_.nr_nb = paramNonBond_.nr_nb-1;
}

void AnalysisTemplate::vaccumMMFull(rvec *x)
{
    rvec dx;
    real colmb_factor = 138.935485;
    double qi, qj, c6, c12, c6j, c12j,rij, c6ij, c12ij;
    int itypeA, itypeB, ntype = mtop_->atomtypes.nr;
    int i, j,k,l,n;
    int atomA, atomB, resA, resB;
    real TempEE, TempVdw;
    int nres = atoms_->nres;

    std::vector<real> EERes(nres, 0.0), VdwRes(nres, 0.0);

    if(bDIFF_)
    {
        if (EEnergyFrame_.size() != nres+3)
            EEnergyFrame_.resize(nres+3);
        if(VdwEnergyFrame_.size() != nres+3)
            VdwEnergyFrame_.resize(nres+3);
    }

    std::fill(EEnergyFrame_.begin(), EEnergyFrame_.end(), 0.0);
    std::fill(VdwEnergyFrame_.begin(), VdwEnergyFrame_.end(), 0.0);

    for(i=0; i<paramNonBond_.nr_nb; i++)	{
        atomA = paramNonBond_.pairNB[i][0];
        atomB = paramNonBond_.pairNB[i][1];
        resA = atoms_->atom[atomA].resind;
        resB = atoms_->atom[atomB].resind;

        rij = sqrt(pow((x[atomA][0]-x[atomB][0]),2)+pow((x[atomA][1]-x[atomB][1]),2) + pow((x[atomA][2]-x[atomB][2]),2));

        itypeA = atoms_->atom[atomA].type;
        itypeB = atoms_->atom[atomB].type;

        if(itypeA<=itypeB)	{
            c6 = localtop_->idef.iparams[itypeA*ntype+itypeB].lj.c6;
            c12 = localtop_->idef.iparams[itypeA*ntype+itypeB].lj.c12;
        }
        else	{
            c6 = localtop_->idef.iparams[itypeB*ntype+itypeA].lj.c6;
            c12 = localtop_->idef.iparams[itypeB*ntype+itypeA].lj.c12;
        }
        TempEE = ((colmb_factor/pdie_) * (atoms_->atom[atomA].q*atoms_->atom[atomB].q))/rij;
        TempVdw = (c12/pow(rij,12)) - (c6/pow(rij,6));

        if(bDIFF_)	{
            if(paramNonBond_.bItsA[i])
            {
                EEnergyFrame_[0] += TempEE;
                VdwEnergyFrame_[0] += TempVdw;
            }
            if(paramNonBond_.bItsB[i])
            {
                EEnergyFrame_[1] += TempEE;
                VdwEnergyFrame_[1] += TempVdw;
            }
            EEnergyFrame_[2] += TempEE;
            VdwEnergyFrame_[2] += TempVdw;
        }
        else
        {
            EEnergyFrame_[0] += TempEE;
            VdwEnergyFrame_[0] += TempVdw;
        }

        if(bDCOMP_)
            if((!paramNonBond_.bItsA[i]) && (!paramNonBond_.bItsB[i]))
            {
                EERes[resA] += TempEE;
                EERes[resB] += TempEE;
                VdwRes[resA] += TempVdw;
                VdwRes[resB] += TempVdw;
            }
    }
    for(i=0; i<paramNonBond_.nr_14; i++)
    {
        atomA = paramNonBond_.pair14[i][0];
        atomB = paramNonBond_.pair14[i][1];
        resA = atoms_->atom[atomA].resind;
        resB = atoms_->atom[atomB].resind;

        rij = sqrt(pow((x[atomA][0]-x[atomB][0]),2)+pow((x[atomA][1]-x[atomB][1]),2) + pow((x[atomA][2]-x[atomB][2]),2));

        c6 = localtop_->idef.iparams[localtop_->idef.il[F_LJ14].iatoms[paramNonBond_.pairtype[i]]].lj14.c6A;
        c12 = localtop_->idef.iparams[localtop_->idef.il[F_LJ14].iatoms[paramNonBond_.pairtype[i]]].lj14.c12A;

        TempEE = mtop_->ffparams.fudgeQQ * ((colmb_factor/pdie_) * (atoms_->atom[atomA].q*atoms_->atom[atomB].q))/rij;
        TempVdw = (c12/pow(rij,12)) - (c6/pow(rij,6));

        if(bDIFF_)
        {
            if(paramNonBond_.bItsA14[i])
            {
                EEnergyFrame_[0] += TempEE;
                VdwEnergyFrame_[0] += TempVdw;
            }
            if(paramNonBond_.bItsB14[i])
            {
                EEnergyFrame_[1] += TempEE;
                VdwEnergyFrame_[1] += TempVdw;
            }
            EEnergyFrame_[2] += TempEE;
            VdwEnergyFrame_[2] += TempVdw;
        }
        else
        {
            EEnergyFrame_[0] += TempEE;
            VdwEnergyFrame_[0] += TempVdw;
        }
        if (bDCOMP_)
            if((!paramNonBond_.bItsA14[i]) && (!paramNonBond_.bItsB14[i]))
            {
                EERes[resA] += TempEE;
                EERes[resB] += TempEE;
                VdwRes[resA] += TempVdw;
                VdwRes[resB] += TempVdw;
            }
    }

    if(bDCOMP_)
        for (i=0; i<nres; i++)
        {
            EEnergyFrame_[i+3] = EERes[i];
            VdwEnergyFrame_[i+3] = VdwRes[i];
        }

}

void AnalysisTemplate::vaccumMMWithoutExclusions(rvec *x)
{
    int i, j=0;
    real colmb_factor = 138.935485;
    double c6, c12, rij;
    int itypeA, itypeB, ntype = mtop_->atomtypes.nr;
    int resA, resB;
    real TempEE, TempVdw;
    int nres = atoms_->nres;

    std::vector<real> EERes(nres, 0.0), VdwRes(nres, 0.0);

    if (EEnergyFrame_.size() != nres+3)
        EEnergyFrame_.resize(nres+3);
    if(VdwEnergyFrame_.size() != nres+3)
        VdwEnergyFrame_.resize(nres+3);

    std::fill(EEnergyFrame_.begin(), EEnergyFrame_.end(), 0.0);
    std::fill(VdwEnergyFrame_.begin(), VdwEnergyFrame_.end(), 0.0);

    for(i=0; i<isize_[0]; i++)
    {
        for(j=0; j<isize_[1]; j++)	{

            resA = atoms_->atom[index_[0][i]].resind;
            resB = atoms_->atom[index_[1][j]].resind;

            rij = sqrt(pow((x[index_[0][i]][0]-x[index_[1][j]][0]),2)+pow((x[index_[0][i]][1]-x[index_[1][j]][1]),2) + pow((x[index_[0][i]][2]-x[index_[1][j]][2]),2));

            itypeA = atoms_->atom[index_[0][i]].type;
            itypeB = atoms_->atom[index_[1][j]].type;

            if(itypeA<=itypeB)	{
                c6 = localtop_->idef.iparams[itypeA*ntype+itypeB].lj.c6;
                c12 = localtop_->idef.iparams[itypeA*ntype+itypeB].lj.c12;
            }
            else	{
                c6 = localtop_->idef.iparams[itypeB*ntype+itypeA].lj.c6;
                c12 = localtop_->idef.iparams[itypeB*ntype+itypeA].lj.c12;
            }
            TempEE = ((colmb_factor/pdie_) * (atoms_->atom[index_[0][i]].q * atoms_->atom[index_[1][j]].q))/rij;
            TempVdw = (c12/pow(rij,12)) - (c6/pow(rij,6));

            EEnergyFrame_[2] += TempEE;
            VdwEnergyFrame_[2] += TempVdw;

            if(bDCOMP_)
            {
                EERes[resA] += TempEE;
                EERes[resB] += TempEE;
                VdwRes[resA] += TempVdw;
                VdwRes[resB] += TempVdw;
            }
        }
    }

    if(bDCOMP_)
        for (i=0; i<nres; i++)
        {
            EEnergyFrame_[i+3] = EERes[i];
            VdwEnergyFrame_[i+3] = VdwRes[i];
        }
}


void AnalysisTemplate::prepareOutputFiles(const TrajectoryAnalysisSettings *settings)
{
    if (bMM_)  {
        int mmColumnCount = (bDIFF_) ? 7 : 3;
        std::vector<std::string> mmLegends;

        if (bDIFF_) {
            mmLegends.push_back(std::string(selA_.name()) +  " VdW Energy");
            mmLegends.push_back(std::string(selA_.name()) +  " Elec. Energy");
            mmLegends.push_back(std::string(selB_.name()) +  " VdW Energy");
            mmLegends.push_back(std::string(selB_.name()) +  " Elec. Energy");
            if(bIncl14_) {
                mmLegends.push_back(std::string(selA_.name()) + "+" + std::string(selB_.name()) + " VdW Energy");
                mmLegends.push_back(std::string(selA_.name()) + "+" + std::string(selB_.name()) + " Elec. Energy");
            } else {
                mmLegends.push_back(std::string(selA_.name()) + "-" + std::string(selB_.name()) + " VdW Energy");
                mmLegends.push_back(std::string(selA_.name()) + "-" + std::string(selB_.name()) + " Elec. Energy");
            }
            mmLegends.push_back(std::string(selA_.name()) + "-" + std::string(selB_.name()) + " Total Energy");
        } else {
            mmLegends.push_back(std::string(selA_.name()) +  " VdW Energy");
            mmLegends.push_back(std::string(selA_.name()) +  " Elec. Energy");
            mmLegends.push_back(std::string(selA_.name()) +  " Total Energy");
        }

        mmEnergyData_.setColumnCount(0, mmColumnCount);
        {
            AnalysisDataPlotModulePointer plotm(new AnalysisDataPlotModule(settings->plotSettings()));
            plotm->setFileName(fnVacMM_);
            plotm->setTitle("Vaccum MM Energy)");
            plotm->setXAxisIsTime();
            plotm->setYLabel("Energy (kJ/mol)");
            for (size_t i = 0; i < mmColumnCount; ++i)
            {
                plotm->appendLegend(mmLegends[i]);
            }
            mmEnergyData_.addModule(plotm);
        }

        if (bDCOMP_) {
            fDecompMM_ = gmx_ffopen(fnDecompMM_, "w");
            fprintf(fDecompMM_, "# Time\t");
            for(int i=0; i < atoms_->nres; i++)
            {
                if((bResA_[i]) || (bResB_[i])) {
                    fprintf(fDecompMM_,"%s-%d\t", *(atoms_->resinfo[i].name), atoms_->resinfo[i].nr);
                }
            }
            fprintf(fDecompMM_,"\n");
        }
    }
}

void AnalysisTemplate::readPBSAInputs()
{
    warninp_t wi;
    gmx_bool bAllowWarnings=FALSE;
    int maxwarning = 99;


    // Start reading input file
    wi = init_warning(bAllowWarnings, maxwarning);
    gmx::TextInputFile     stream(fnMDP_);
    std::vector<t_inpfile> inp = read_inpfile(&stream, fnMDP_.c_str(), wi);

    //To check for polar solvation calculation
    std::string polar = get_estr(&inp, "polar", nullptr);
    if (polar == "yes") bPolar_ = true;

    std::string apolar = get_estr(&inp, "apolar", nullptr);
    if (polar == "yes") bApolar_ = true;


    if(bPolar_)
    {
        //Psize keywords
        pbsaInputKwords_.cfac      = get_ereal(&inp, "cfac", 2, wi);
        pbsaInputKwords_.gridspace = get_ereal(&inp, "gridspace", 0.5, wi);
        pbsaInputKwords_.gmemceil  = get_ereal(&inp, "gmemceil", 400, wi);
        pbsaInputKwords_.fadd      = get_ereal(&inp, "fadd", 20, wi);
        pbsaInputKwords_.ofrac     = get_ereal(&inp, "ofrac", 0.1, wi);

        //Polar Keywords
        pbsaInputKwords_.mg_type   = get_eeenum(&inp, "mg-type", mg_words, wi);
        pbsaInputKwords_.pcharge   = get_ereal(&inp, "pcharge", 0,    wi);
        pbsaInputKwords_.ncharge   = get_ereal(&inp, "ncharge", 0,    wi);
        pbsaInputKwords_.prad      = get_ereal(&inp, "prad",    0,    wi);
        pbsaInputKwords_.nrad      = get_ereal(&inp, "nrad",    0,    wi);
        pbsaInputKwords_.pconc     = get_ereal(&inp, "pconc",   0,    wi);
        pbsaInputKwords_.nconc     = get_ereal(&inp, "nconc",   0,    wi);
        pbsaInputKwords_.pdie      = get_ereal(&inp, "pdie",    4,    wi);
        pbsaInputKwords_.sdie      = get_ereal(&inp, "sdie",    78.4, wi);
        pbsaInputKwords_.vdie      = get_ereal(&inp, "vdie",    1,    wi);
        pbsaInputKwords_.srad      = get_ereal(&inp, "srad",    1.4,  wi); // same for SASA and SAV
        pbsaInputKwords_.swin      = get_ereal(&inp, "swin",    0.30, wi);
        pbsaInputKwords_.sdens     = get_ereal(&inp, "sdens",   10,   wi);
        pbsaInputKwords_.temp      = get_ereal(&inp, "temp",    300,  wi);
        pbsaInputKwords_.srfm      = get_eeenum(&inp, "srfm", srfm_words, wi);
        pbsaInputKwords_.chgm      = get_eeenum(&inp, "chgm", chgm_words, wi);
        pbsaInputKwords_.bcfl      = get_eeenum(&inp, "bcfl", bcfl_words, wi);
        pbsaInputKwords_.pbsolver  = get_eeenum(&inp, "PBsolver", PBsolver, wi);

        pbsaInputKwords_.gamma     = get_ereal(&inp, "gamma",    0.030096,  wi);
        pbsaInputKwords_.sasaconst = get_ereal(&inp, "sasconst",    0,  wi);

        pbsaInputKwords_.press     = get_ereal(&inp, "press",       0,  wi);
        pbsaInputKwords_.savconst  = get_ereal(&inp, "savconst",    0,  wi);

    }

}

void AnalysisTemplate::assignRadius()
{
    std::map<std::string, real> radiusDef = {
        {"o", 1.520 },
        {"s", 1.830 },
        {"n", 1.550 },
        {"c", 1.700 },
        {"h", 1.2   },
        {"p", 1.8 },
        {"f", 1.470},
        {"i", 2.060},
        {"cl", 1.770},
        {"br", 1.920},
        {"ca", 1.770},
        {"cb", 1.770},
        {"cc", 1.770},
        {"cn", 1.770},
        {"cr", 1.770},
        {"cv", 1.770},
        {"cw", 1.770},
        {"c*", 1.770},
        {"cd", 1.770},
        {"ha", 1.000},
        {"h4", 1.000},
        {"h5", 1.000},
        {"mw", 0.050}, // virtual-sites pr sigma-holes
    };
    
    int itype, ntype = mtop_->ffparams.atnr;
    real c6, c12, sig6, rad = -1;
    std::string atomtype, atomname, atomtype2;
    for (int i = 0; i < atoms_->nr; i++) {      // get charge and radius for all atoms
        rad = -1;
        
        // calculate radius from force-field parameters
        // if radius is not found later on, force-field radius will be used
        itype = atoms_->atom[i].type;
        c12 = localtop_->idef.iparams[itype * ntype + itype].lj.c12;
        c6 =  localtop_->idef.iparams[itype * ntype + itype].lj.c6;
        if ((c6 != 0) && (c12 != 0)) {
          sig6 = c12 / c6;
          rad = 0.5 * pow(sig6, 1.0 / 6.0);
        }
        rad *= 10; //Conversion of nano meter to angstroms
        
        
        // Try to find radius based on atomname and atomtype
        atomtype = *(atoms_->atomtype[i]);
        std::transform(atomtype.begin(), atomtype.end(), atomtype.begin(), [](unsigned char c){ return std::tolower(c); });
        atomname = *(atoms_->atomname[i]);
        std::transform(atomname.begin(), atomname.end(), atomname.begin(), [](unsigned char c){ return std::tolower(c); });
        
        if (radiusDef.count(atomname) > 0) // first based on atomname - mostly single charecter atomname
            rad = radiusDef[atomname];
        else if (radiusDef.count(atomtype) > 0) // second based on atomtype - two charecters name
            rad = radiusDef[atomtype];
        else
            if (atomtype.length() > 1) // check whether first two charecters of atomtype matches
                atomtype2 = atomtype[0]+atomtype[1];
                if (radiusDef.count(atomtype2) > 0)
                    rad = radiusDef[atomtype];
      
        // Assigned charge and radius to ocuppancy and bfactor field
        atoms_->pdbinfo[i].occup = atoms_->atom[i].q;
        atoms_->pdbinfo[i].bfac = rad;
    }
    
    // First index group
    for (int i = 0; i < isize_[0]; i++)
        radiiA_.push_back((atoms_->pdbinfo[index_[0][i]].bfac + pbsaInputKwords_.srad)/10);
        
    if (bDIFF_) {
        // Second index group
        for (int i = 0; i < isize_[1]; i++)
            radiiB_.push_back((atoms_->pdbinfo[index_[1][i]].bfac + pbsaInputKwords_.srad)/10);
        
        // combined index group
        radiiAB_.insert( radiiAB_.end(), radiiA_.begin(), radiiA_.end() );
        radiiAB_.insert( radiiAB_.end(), radiiB_.begin(), radiiB_.end() );
    }
    
    
}



/*! \brief
 * The main function for the analysis template.
 */
int
main(int argc, char *argv[])
{
    return gmx::TrajectoryAnalysisCommandLineRunner::runAsMain<AnalysisTemplate>(argc, argv);
}
