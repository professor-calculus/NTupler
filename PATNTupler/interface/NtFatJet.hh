#ifndef NTFATJET_HH
#define NTFATJET_HH

#include "../interface/FatJetStruct.hh"

namespace ran{
  class NtFatJet{
  public:
    NtFatJet() : jet(0){} ///< Default constructor
    NtFatJet(const ran::FatJetStruct& aJet) : jet(&aJet){} ///< Constructor
    float pt() const {return jet->pt;} ///<Particle pt
    float et() const {return jet->et;} ///<Particle et
    float eta() const {return jet->eta;}///<Particle eta
    float phi() const {return jet->phi;}///<Particle phi
    float mass() const {return jet->mass;} ///<Particle mass
    
    float jecFactor_unCorrected() const {return jet->jecFactor_unCorrected;}///< factor applied to jet pt to get uncorrect jet pt
    float jecUncertainty() const {return jet->jecUncertainty;}///< the uncertainty in the JEC applied
    float userFloat_ak8PFJets_CHSPrunedLinks() const {return jet->userFloat_ak8PFJets_CHSPrunedLinks;}///< Pruned jets
    float userFloat_ak8PFJets_CHSTrimmedLinks() const {return jet-> userFloat_ak8PFJets_CHSTrimmedLinks;}///< Trimed jets
    float userFloat_ak8PFJets_CHSFilteredLinks() const {return jet->userFloat_ak8PFJets_CHSFilteredLinks;}///< Filtered jets
    float userFloat_cmsTopTag_PFJets_CHSLinksAK8() const {return jet->userFloat_cmsTopTag_PFJets_CHSLinksAK8;}///< CMS Top Tagger
    float numberOfDaughters() const {return jet->numberOfDaughters;}///< Number of Jet daughters
    float chargedMultiplicity() const {return jet->chargedMultiplicity;}///< charged multiplicity
    int neutralMultiplicity() const {return jet->neutralMultiplicity;}///< neutral multiplicity
    float muonEnergyFraction() const {return jet->muonEnergyFraction;}///< muonHadronEnergyFraction
    float electronEnergyFraction() const {return jet->electronEnergyFraction;}///< electronHadronEnergyFraction
    float neutralHadronEnergyFraction() const {return jet->neutralHadronEnergyFraction;}///< neutralHadronEnergyFraction
    float HFHadronEnergyFraction() const {return jet->HFHadronEnergyFraction;}///< HFHadronEnergyFraction
    float neutralEmEnergyFraction() const {return jet->neutralEmEnergyFraction;}///< neutralEmEnergyFraction
    float chargedEmEnergyFraction() const {return jet->chargedEmEnergyFraction;}///<chargedEmEnergyFraction
    float chargedHadronEnergyFraction() const {return jet->chargedHadronEnergyFraction;}///<chargedHadronEnergyFraction

    float pfBoostedDoubleSecondaryVertexAK8BJetTags() const {return jet->pfBoostedDoubleSecondaryVertexAK8BJetTags;}///< double b tag discrim

    double NjettinessAK8_tau1() const {return jet->NjettinessAK8_tau1;}///<  Access the n-subjettiness variables
    double NjettinessAK8_tau2() const {return jet->NjettinessAK8_tau2;}///<  Access the n-subjettiness variables
    double NjettinessAK8_tau3() const {return jet->NjettinessAK8_tau3;}///<  Access the n-subjettiness variables
    double CHSsoftdrop_mass() const {return jet->CHSsoftdrop_mass;}// access to soft drop mass
    double CHSpruned_mass() const {return jet->CHSpruned_mass;}// access to pruned mass
    double PUPPIsoftdrop_mass() const {return jet->CHSsoftdrop_mass;}// access to soft drop mass (calc uses PUPPI)

    /*double puppi_pt() const {return jet->puppi_pt;}
    double puppi_mass() const {return jet->puppi_mass;}
    double puppi_eta() const {return jet->puppi_eta;}
    double puppi_phi() const {return jet->puppi_phi;}
    double puppi_tau1() const {return jet->puppi_tau1;}
    double puppi_tau2() const {return jet->puppi_tau2;}
    double puppi_tau3() const {return jet->puppi_tau3;}*/

    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::FatJetStruct* jet;

  };
}

#endif
