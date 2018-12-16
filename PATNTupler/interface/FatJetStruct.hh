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
    float jecUncertainty;///<the uncertainty in the JEC applied
    float jerUncUp;///<the uncertainty in the JER (up)
    float jerUncDown;///<the uncertainty in the JER (down)
    float userFloat_ak8PFJets_CHSPrunedLinks;///< Pruned jets
    float userFloat_ak8PFJets_CHSTrimmedLinks;///< Trimed jets
    float userFloat_ak8PFJets_CHSFilteredLinks;///< Filtered jets
    float userFloat_cmsTopTag_PFJets_CHSLinksAK8;///< CMS Top Tagger
    float numberOfDaughters;///< Number of Jet daughters
    float chargedMultiplicity;///< charged multiplicity
    int neutralMultiplicity;///< neutral multiplicity
    float muonEnergyFraction;///< muon energy fraction
    float electronEnergyFraction;///< electron energy fraction
    float neutralHadronEnergyFraction;///< neutralHadronEnergyFraction
    float HFHadronEnergyFraction;///< HFHadronEnergyFraction
    float neutralEmEnergyFraction;///< neutralEmEnergyFraction
    float chargedEmEnergyFraction;///<chargedEmEnergyFraction
    float chargedHadronEnergyFraction;///<chargedHadronEnergyFraction
    float pfBoostedDoubleSecondaryVertexAK8BJetTags;///< double b tag discrim

    double NjettinessAK8_tau1; ///<  Access the n-subjettiness variables
    double NjettinessAK8_tau2;///<  Access the n-subjettiness variables
    double NjettinessAK8_tau3;///<  Access the n-subjettiness variables
    double CHSsoftdrop_mass;// access to soft drop mass
    double CHSpruned_mass;// access to pruned mass
    double PUPPIsoftdrop_mass;// access to soft drop mass (calc uses PUPPI)
    /*double puppi_pt;
    double puppi_mass;
    double puppi_eta;
    double puppi_phi;
    double puppi_tau1;
    double puppi_tau2;
    double puppi_tau3;*/

    // DBT tagging variables
    float dbtVar_z_ratio;
    float dbtVar_trackSipdSig_3;
    float dbtVar_trackSipdSig_2;
    float dbtVar_trackSipdSig_1;
    float dbtVar_trackSipdSig_0;
    float dbtVar_trackSipdSig_1_0;
    float dbtVar_trackSipdSig_0_0;
    float dbtVar_trackSipdSig_1_1;
    float dbtVar_trackSipdSig_0_1;
    float dbtVar_trackSip2dSigAboveCharm_0;
    float dbtVar_trackSip2dSigAboveBottom_0;
    float dbtVar_trackSip2dSigAboveBottom_1;
    float dbtVar_tau1_trackEtaRel_0;
    float dbtVar_tau1_trackEtaRel_1;
    float dbtVar_tau1_trackEtaRel_2;
    float dbtVar_tau0_trackEtaRel_0;
    float dbtVar_tau0_trackEtaRel_1;
    float dbtVar_tau0_trackEtaRel_2;
    float dbtVar_tau_vertexMass_0;
    float dbtVar_tau_vertexEnergyRatio_0;
    float dbtVar_tau_vertexDeltaR_0;
    float dbtVar_tau_flightDistance2dSig_0;
    float dbtVar_tau_vertexMass_1;
    float dbtVar_tau_vertexEnergyRatio_1;
    float dbtVar_tau_flightDistance2dSig_1;
    float dbtVar_jetNTracks;
    float dbtVar_nSV;

    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(FatJetStruct,2)//Struct class definition
  };

}


#endif
