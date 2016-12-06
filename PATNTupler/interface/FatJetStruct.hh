#ifndef FATJETSTRUCT_HH
#define FATJETSTRUCT_HH

#include <TObject.h>

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct FatJetStruct: public TObject{
    FatJetStruct(); ///< default constructor
    virtual ~FatJetStruct(){} ///< destructor
    float et;///< et
    float pt;/// < pt
    float mass;/// < mass
    float eta;///< eta 
    float phi;/// < phi
    float jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    float userFloat_ak8PFJets_CHSPrunedLinks;///< Pruned jets
    float userFloat_ak8PFJets_CHSTrimmedLinks;///< Trimed jets
    float userFloat_ak8PFJets_CHSFilteredLinks;///< Filtered jets
    float userFloat_cmsTopTag_PFJets_CHSLinksAK8;///< CMS Top Tagger
    float numberOfDaughters;///< Number of Jet daughters
    float chargedMultiplicity;///< charged multiplicity
    float neutralHadronEnergyFraction;///< neutralHadronEnergyFraction
    float HFHadronEnergyFraction;///< HFHadronEnergyFraction
    float neutralEmEnergyFraction;///< neutralEmEnergyFraction
    float chargedEmEnergyFraction;///<chargedEmEnergyFraction
    float chargedHadronEnergyFraction;///<chargedHadronEnergyFraction

    double NjettinessAK8_tau1; ///<  Access the n-subjettiness variables
    double NjettinessAK8_tau2;///<  Access the n-subjettiness variables
    double NjettinessAK8_tau3;///<  Access the n-subjettiness variables
    double CHSsoftdrop_mass;// access to soft drop mass
    double CHSpruned_mass;// access to pruned mass
    /*double puppi_pt;
    double puppi_mass;
    double puppi_eta;
    double puppi_phi;
    double puppi_tau1;
    double puppi_tau2;
    double puppi_tau3;*/


    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(FatJetStruct,1)//Struct class definition
  };

}


#endif
