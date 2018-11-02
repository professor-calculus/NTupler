//STL headers
#include <iostream>
#include <vector>
#include <fstream>

// Unix headers
#include <sys/stat.h>

//ROOT HEADERS
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h> 
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TMath.h>

//RAL PARTICLE HEADERS
#include "NtpReader.hh"
#include "GoodLumiChecker.hh"
#include "TriggerPathToIndex.hh"
#include "deltaR.h"
#include "DoubleBTagSF.h"
#include "rhoEACorrections.h"
#include "IsrWeights.h"
#include "SoftDropPuppiSF.h"

using std::cout;
using std::ifstream;
using std::ofstream;



/*
Compilation:
within main directory run
$ gmake -j 8 <executableNameSetInGNUmakefile>

Executing:
./<executableNameSetInGNUmakefile> <outputRootFilename> <pathToListOfInputFiles> <sampleType> <runInstructions> <lumiJson>

NB:
runInstructions = "local" (for local runs)
runInstructions = "batch" (for running on batch systems)
the lumi json file does not need to be included
*/



namespace tsw {

/*!
 * \class TreeHandlerBase
 * \brief Base class for tree handlers that implements all common functionality: saveToFile method, destruction of all trees
 */
class TreeHandlerBase{
public:
	// CTOR & DTOR
	TreeHandlerBase(const std::string& mainTreeName, const std::string& mainTreeDescription, const std::string& fileName);
	virtual ~TreeHandlerBase();

	// PUBLIC METHODS
	void setEventCounter(unsigned int nEvtsTotal){
		totNumEvtsAnalyzed_ = nEvtsTotal;
		numEvtsPass_ = mainAnaTree_->GetEntries();
		std::cout << "Number of events run over: " << nEvtsTotal << "   Number of events passed: " << numEvtsPass_ << std::endl;
	}
	void saveToFile();

	std::string fileName() const { return fileName_; }

protected:
	TTree* addTree(const std::string& treeName, const std::string& treeDescription ); ///< Returns TTree with specified name/description on heap and registers it for saving to file

	static bool isMadgraphDrellYan(const std::string& fileName) { return (fileName.find("DY")!=std::string::npos && (fileName.find("MG")!=std::string::npos || fileName.find("MADGRAPH")!=std::string::npos) ); }

private:
	// PRIVATE MEMBERS
	std::string fileName_;
	TFile* outputFile_;
	TTree* eventCountTree_;

	UInt_t numEvtsPass_;
	UInt_t totNumEvtsAnalyzed_;

protected:
	// PROTECTED MEMBERS
	TTree* mainAnaTree_;
	std::vector<TTree*> otherTrees_;
};


TreeHandlerBase::TreeHandlerBase(const std::string& mainTreeName, const std::string& mainTreeDescription, const std::string& fileName) :
	fileName_( fileName ),
	outputFile_( new TFile(fileName.c_str(), "RECREATE") ),
	eventCountTree_( new TTree("eventCountTree", "Tree containing orig event counts per file.") ),
	numEvtsPass_(0), totNumEvtsAnalyzed_(0),
	mainAnaTree_( new TTree(mainTreeName.c_str(), mainTreeDescription.c_str()) )
{
	mainAnaTree_->SetDirectory( outputFile_ );
	eventCountTree_->SetDirectory( outputFile_ );

	eventCountTree_->Branch("nEvtsPass", &numEvtsPass_, "nEvtsPass/i");
	eventCountTree_->Branch("nEvtsRunOver", &totNumEvtsAnalyzed_, "nEvtsRunOver/i");
}

TreeHandlerBase::~TreeHandlerBase()
{
	delete outputFile_; // TTrees themselves should not be deleted since they reside in the file (??)
}

void TreeHandlerBase::saveToFile()
{
	// Firstly fill eventCountTree_
	eventCountTree_->Fill();

	// Then, write TTrees to file ...
	outputFile_->cd();
	mainAnaTree_->Write();
	eventCountTree_->Write();
	for(size_t idx=0; idx<otherTrees_.size(); idx++)
		otherTrees_.at(idx)->Write();
	outputFile_->Close();
}

TTree* TreeHandlerBase::addTree(const std::string& treeName, const std::string& treeDescription )
{
	TTree* treePtr = new TTree( treeName.c_str(), treeDescription.c_str() );
	treePtr->SetDirectory( outputFile_ );
	otherTrees_.push_back(treePtr);
	return treePtr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FatDoubleBJetPairTree: Simple class for generating trees containing info about
//                        two fat jets with highest double-b tag discriminant from each event containing at least two fat jets

class FatDoubleBJetPairTree : public TreeHandlerBase {
private:
	// PRIVATE MEMBERS

	// For the event & kinematic branches ...
	Double_t treeVar_weight_combined_;
	
	Double_t treeVar_weight_dbtTag_;
	Double_t treeVar_weight_dbtTagUp_;
	Double_t treeVar_weight_dbtTagDown_;

	Double_t treeVar_weight_isr_;
	Double_t treeVar_weight_isrUp_;
	Double_t treeVar_weight_isrDown_;

	UInt_t treeVar_runNum_;
	UInt_t treeVar_lumiSec_;
	UInt_t treeVar_evtNum_;

	Bool_t treeVar_trgDecision_;
	UInt_t treeVar_nPU_;
	UInt_t treeVar_nGluino_;

	UInt_t treeVar_nHiggsTobb_;
	Double_t treeVar_DelRHiggs_bb_1_;
	Double_t treeVar_DelRHiggs_bb_2_;

	TLorentzVector* treeVar_fatJetA_p4Ptr_; TLorentzVector treeVar_fatJetA_p4_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jecUncUp_; TLorentzVector treeVar_fatJetA_p4_jecUncUp_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jecUncDown_; TLorentzVector treeVar_fatJetA_p4_jecUncDown_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jerUncUp_; TLorentzVector treeVar_fatJetA_p4_jerUncUp_;
	TLorentzVector* treeVar_fatJetA_p4Ptr_jerUncDown_; TLorentzVector treeVar_fatJetA_p4_jerUncDown_;

	TLorentzVector* treeVar_fatJetB_p4Ptr_; TLorentzVector treeVar_fatJetB_p4_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jecUncUp_; TLorentzVector treeVar_fatJetB_p4_jecUncUp_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jecUncDown_; TLorentzVector treeVar_fatJetB_p4_jecUncDown_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jerUncUp_; TLorentzVector treeVar_fatJetB_p4_jerUncUp_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_jerUncDown_; TLorentzVector treeVar_fatJetB_p4_jerUncDown_;

	Double_t treeVar_fatJetA_doubleBtagDiscrim_;
	Double_t treeVar_fatJetA_mass_;
	Double_t treeVar_fatJetA_eta_;
	Double_t treeVar_fatJetA_phi_;
	Double_t treeVar_fatJetA_softDropMassCHS_;
	Double_t treeVar_fatJetA_softDropMassPuppi_ = 0;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmsUncUp_ = 0;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmsUncDown_ = 0;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmrUncUp_ = 0;
	Double_t treeVar_fatJetA_softDropMassPuppi_jmrUncDown_ = 0;
	Double_t treeVar_fatJetA_prunedMass_;
	Float_t treeVar_fatJetA_electronEnergyFraction_;
	Float_t treeVar_fatJetA_muonEnergyFraction_;
	Float_t treeVar_fatJetA_nSubjettinessTau1_;
	Float_t treeVar_fatJetA_nSubjettinessTau2_;
	Double_t treeVar_fatJetB_doubleBtagDiscrim_;
	Double_t treeVar_fatJetB_mass_;
	Double_t treeVar_fatJetB_eta_;
	Double_t treeVar_fatJetB_phi_;
	Double_t treeVar_fatJetB_softDropMassCHS_;
	Double_t treeVar_fatJetB_softDropMassPuppi_ = 0;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmsUncUp_ = 0;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmsUncDown_ = 0;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmrUncUp_ = 0;
	Double_t treeVar_fatJetB_softDropMassPuppi_jmrUncDown_ = 0;
	Double_t treeVar_fatJetB_prunedMass_;
	Float_t treeVar_fatJetB_electronEnergyFraction_;
	Float_t treeVar_fatJetB_muonEnergyFraction_;
	Float_t treeVar_fatJetB_nSubjettinessTau1_;
	Float_t treeVar_fatJetB_nSubjettinessTau2_;

	Float_t treeVar_ht_;
	Float_t treeVar_ht_jecUncUp_;
	Float_t treeVar_ht_jecUncDown_;
	Float_t treeVar_ht_jerUncUp_;
	Float_t treeVar_ht_jerUncDown_;

	Float_t treeVar_mht_;
	Float_t treeVar_mht_jecUncUp_;
	Float_t treeVar_mht_jecUncDown_;
	Float_t treeVar_mht_jerUncUp_;
	Float_t treeVar_mht_jerUncDown_;

	Float_t treeVar_mht_phi_;
	Float_t treeVar_mht_phi_jecUncUp_;
	Float_t treeVar_mht_phi_jecUncDown_;
	Float_t treeVar_mht_phi_jerUncUp_;
	Float_t treeVar_mht_phi_jerUncDown_;

	Float_t treeVar_bJetA_tag_score_;
	Float_t treeVar_bJetB_tag_score_;
	Float_t treeVar_bJetC_tag_score_;
	Float_t treeVar_bJetD_tag_score_;

	UInt_t treeVar_nrSlimJets_;
	UInt_t treeVar_nrSlimJetsByBtagScore_;
	UInt_t treeVar_nrSlimLooseBJets_;
	UInt_t treeVar_nrSlimMediumBJets_;
	UInt_t treeVar_nrSepSlimJets_;
	UInt_t treeVar_nrSepSlimLooseBJets_;
	UInt_t treeVar_nrSepSlimMediumBJets_;
	UInt_t treeVar_nrFatJets_;
	UInt_t treeVar_nrLooseMuons_;
	UInt_t treeVar_nrTightMuons_;
	UInt_t treeVar_nrElectrons_;
	UInt_t treeVar_nrPhotons_;
	UInt_t treeVar_nrTracks_;

	UInt_t treeVar_nrPrefireJets_;
	UInt_t treeVar_nrPrefirePhotons_;

	// Slim jets
	TLorentzVector* treeVar_jetA_p4Ptr_; TLorentzVector treeVar_jetA_p4_;
	TLorentzVector* treeVar_jetA_p4Ptr_jecUncUp_; TLorentzVector treeVar_jetA_p4_jecUncUp_;
	TLorentzVector* treeVar_jetA_p4Ptr_jecUncDown_; TLorentzVector treeVar_jetA_p4_jecUncDown_;
	TLorentzVector* treeVar_jetA_p4Ptr_jerUncUp_; TLorentzVector treeVar_jetA_p4_jerUncUp_;
	TLorentzVector* treeVar_jetA_p4Ptr_jerUncDown_; TLorentzVector treeVar_jetA_p4_jerUncDown_;

	TLorentzVector* treeVar_jetB_p4Ptr_; TLorentzVector treeVar_jetB_p4_;
	TLorentzVector* treeVar_jetB_p4Ptr_jecUncUp_; TLorentzVector treeVar_jetB_p4_jecUncUp_;
	TLorentzVector* treeVar_jetB_p4Ptr_jecUncDown_; TLorentzVector treeVar_jetB_p4_jecUncDown_;
	TLorentzVector* treeVar_jetB_p4Ptr_jerUncUp_; TLorentzVector treeVar_jetB_p4_jerUncUp_;
	TLorentzVector* treeVar_jetB_p4Ptr_jerUncDown_; TLorentzVector treeVar_jetB_p4_jerUncDown_;

	// b jets
	TLorentzVector* treeVar_bjetA_p4Ptr_; TLorentzVector treeVar_bjetA_p4_;
	TLorentzVector* treeVar_bjetA_p4Ptr_jecUncUp_; TLorentzVector treeVar_bjetA_p4_jecUncUp_;
	TLorentzVector* treeVar_bjetA_p4Ptr_jecUncDown_; TLorentzVector treeVar_bjetA_p4_jecUncDown_;
	TLorentzVector* treeVar_bjetA_p4Ptr_jerUncUp_; TLorentzVector treeVar_bjetA_p4_jerUncUp_;
	TLorentzVector* treeVar_bjetA_p4Ptr_jerUncDown_; TLorentzVector treeVar_bjetA_p4_jerUncDown_;

	TLorentzVector* treeVar_bjetB_p4Ptr_; TLorentzVector treeVar_bjetB_p4_;
	TLorentzVector* treeVar_bjetB_p4Ptr_jecUncUp_; TLorentzVector treeVar_bjetB_p4_jecUncUp_;
	TLorentzVector* treeVar_bjetB_p4Ptr_jecUncDown_; TLorentzVector treeVar_bjetB_p4_jecUncDown_;
	TLorentzVector* treeVar_bjetB_p4Ptr_jerUncUp_; TLorentzVector treeVar_bjetB_p4_jerUncUp_;
	TLorentzVector* treeVar_bjetB_p4Ptr_jerUncDown_; TLorentzVector treeVar_bjetB_p4_jerUncDown_;

	TLorentzVector* treeVar_bjetC_p4Ptr_; TLorentzVector treeVar_bjetC_p4_;
	TLorentzVector* treeVar_bjetC_p4Ptr_jecUncUp_; TLorentzVector treeVar_bjetC_p4_jecUncUp_;
	TLorentzVector* treeVar_bjetC_p4Ptr_jecUncDown_; TLorentzVector treeVar_bjetC_p4_jecUncDown_;
	TLorentzVector* treeVar_bjetC_p4Ptr_jerUncUp_; TLorentzVector treeVar_bjetC_p4_jerUncUp_;
	TLorentzVector* treeVar_bjetC_p4Ptr_jerUncDown_; TLorentzVector treeVar_bjetC_p4_jerUncDown_;

	TLorentzVector* treeVar_bjetD_p4Ptr_; TLorentzVector treeVar_bjetD_p4_;
	TLorentzVector* treeVar_bjetD_p4Ptr_jecUncUp_; TLorentzVector treeVar_bjetD_p4_jecUncUp_;
	TLorentzVector* treeVar_bjetD_p4Ptr_jecUncDown_; TLorentzVector treeVar_bjetD_p4_jecUncDown_;
	TLorentzVector* treeVar_bjetD_p4Ptr_jerUncUp_; TLorentzVector treeVar_bjetD_p4_jerUncUp_;
	TLorentzVector* treeVar_bjetD_p4Ptr_jerUncDown_; TLorentzVector treeVar_bjetD_p4_jerUncDown_;

	// Electrons
	TLorentzVector* treeVar_electronA_p4Ptr_; TLorentzVector treeVar_electronA_p4_;
	TLorentzVector* treeVar_electronB_p4Ptr_; TLorentzVector treeVar_electronB_p4_;

	// Muons
	TLorentzVector* treeVar_muonA_p4Ptr_; TLorentzVector treeVar_muonA_p4_;
	TLorentzVector* treeVar_muonB_p4Ptr_; TLorentzVector treeVar_muonB_p4_;

public:
	FatDoubleBJetPairTree(const std::string& treeName, const std::string& fileName) :
		TreeHandlerBase(treeName, "Tree of double-b fat jet candidates ("+treeName+")", fileName),
		
		treeVar_fatJetA_p4Ptr_( &treeVar_fatJetA_p4_ ),
		treeVar_fatJetA_p4Ptr_jecUncUp_( &treeVar_fatJetA_p4_jecUncUp_ ),
		treeVar_fatJetA_p4Ptr_jecUncDown_( &treeVar_fatJetA_p4_jecUncDown_ ),
		treeVar_fatJetA_p4Ptr_jerUncUp_( &treeVar_fatJetA_p4_jerUncUp_ ),
		treeVar_fatJetA_p4Ptr_jerUncDown_( &treeVar_fatJetA_p4_jerUncDown_ ),

		treeVar_fatJetB_p4Ptr_( &treeVar_fatJetB_p4_ ),
		treeVar_fatJetB_p4Ptr_jecUncUp_( &treeVar_fatJetB_p4_jecUncUp_ ),
		treeVar_fatJetB_p4Ptr_jecUncDown_( &treeVar_fatJetB_p4_jecUncDown_ ),
		treeVar_fatJetB_p4Ptr_jerUncUp_( &treeVar_fatJetB_p4_jerUncUp_ ),
		treeVar_fatJetB_p4Ptr_jerUncDown_( &treeVar_fatJetB_p4_jerUncDown_ ),

		treeVar_jetA_p4Ptr_( &treeVar_jetA_p4_ ),
		treeVar_jetA_p4Ptr_jecUncUp_( &treeVar_jetA_p4_jecUncUp_ ),
		treeVar_jetA_p4Ptr_jecUncDown_( &treeVar_jetA_p4_jecUncDown_ ),
		treeVar_jetA_p4Ptr_jerUncUp_( &treeVar_jetA_p4_jerUncUp_ ),
		treeVar_jetA_p4Ptr_jerUncDown_( &treeVar_jetA_p4_jerUncDown_ ),

		treeVar_jetB_p4Ptr_( &treeVar_jetB_p4_ ),
		treeVar_jetB_p4Ptr_jecUncUp_( &treeVar_jetB_p4_jecUncUp_ ),
		treeVar_jetB_p4Ptr_jecUncDown_( &treeVar_jetB_p4_jecUncDown_ ),
		treeVar_jetB_p4Ptr_jerUncUp_( &treeVar_jetB_p4_jerUncUp_ ),
		treeVar_jetB_p4Ptr_jerUncDown_( &treeVar_jetB_p4_jerUncDown_ ),

		treeVar_bjetA_p4Ptr_( &treeVar_bjetA_p4_ ),
		treeVar_bjetA_p4Ptr_jecUncUp_( &treeVar_bjetA_p4_jecUncUp_ ),
		treeVar_bjetA_p4Ptr_jecUncDown_( &treeVar_bjetA_p4_jecUncDown_ ),
		treeVar_bjetA_p4Ptr_jerUncUp_( &treeVar_bjetA_p4_jerUncUp_ ),
		treeVar_bjetA_p4Ptr_jerUncDown_( &treeVar_bjetA_p4_jerUncDown_ ),

		treeVar_bjetB_p4Ptr_( &treeVar_bjetB_p4_ ),
		treeVar_bjetB_p4Ptr_jecUncUp_( &treeVar_bjetB_p4_jecUncUp_ ),
		treeVar_bjetB_p4Ptr_jecUncDown_( &treeVar_bjetB_p4_jecUncDown_ ),
		treeVar_bjetB_p4Ptr_jerUncUp_( &treeVar_bjetB_p4_jerUncUp_ ),
		treeVar_bjetB_p4Ptr_jerUncDown_( &treeVar_bjetB_p4_jerUncDown_ ),

		treeVar_bjetC_p4Ptr_( &treeVar_bjetC_p4_ ),
		treeVar_bjetC_p4Ptr_jecUncUp_( &treeVar_bjetC_p4_jecUncUp_ ),
		treeVar_bjetC_p4Ptr_jecUncDown_( &treeVar_bjetC_p4_jecUncDown_ ),
		treeVar_bjetC_p4Ptr_jerUncUp_( &treeVar_bjetC_p4_jerUncUp_ ),
		treeVar_bjetC_p4Ptr_jerUncDown_( &treeVar_bjetC_p4_jerUncDown_ ),

		treeVar_bjetD_p4Ptr_( &treeVar_bjetD_p4_ ),
		treeVar_bjetD_p4Ptr_jecUncUp_( &treeVar_bjetD_p4_jecUncUp_ ),
		treeVar_bjetD_p4Ptr_jecUncDown_( &treeVar_bjetD_p4_jecUncDown_ ),
		treeVar_bjetD_p4Ptr_jerUncUp_( &treeVar_bjetD_p4_jerUncUp_ ),
		treeVar_bjetD_p4Ptr_jerUncDown_( &treeVar_bjetD_p4_jerUncDown_ ),

		treeVar_electronA_p4Ptr_( &treeVar_electronA_p4_ ),
		treeVar_electronB_p4Ptr_( &treeVar_electronB_p4_ ),
		treeVar_muonA_p4Ptr_( &treeVar_muonA_p4_ ),
		treeVar_muonB_p4Ptr_( &treeVar_muonB_p4_ )
	{
		mainAnaTree_->Branch("weight_combined",     &treeVar_weight_combined_,     "weight_combined/D");
		
		mainAnaTree_->Branch("weight_dbtTag",     &treeVar_weight_dbtTag_,     "weight_dbtTag/D");
		mainAnaTree_->Branch("weight_dbtTagUp",   &treeVar_weight_dbtTagUp_,   "weight_dbtTagUp/D");
		mainAnaTree_->Branch("weight_dbtTagDown", &treeVar_weight_dbtTagDown_, "weight_dbtTagDown/D");

		mainAnaTree_->Branch("weight_isr",     &treeVar_weight_isr_,     "weight_isr/D");
		mainAnaTree_->Branch("weight_isrUp",   &treeVar_weight_isrUp_,   "weight_isrUp/D");
		mainAnaTree_->Branch("weight_isrDown", &treeVar_weight_isrDown_, "weight_isrDown/D");

		mainAnaTree_->Branch("run",    &treeVar_runNum_,   "run/i");
		mainAnaTree_->Branch("lumi",   &treeVar_lumiSec_,   "lumi/i");
		mainAnaTree_->Branch("evtNum", &treeVar_evtNum_,   "evtNum/i");

		mainAnaTree_->Branch("trgDecision", &treeVar_trgDecision_, "trgDecision/O");
		mainAnaTree_->Branch("nPU", &treeVar_nPU_, "nPU/i");
		mainAnaTree_->Branch("nGluino", &treeVar_nGluino_, "nGluino/i");

		mainAnaTree_->Branch("nHiggsTobb", &treeVar_nHiggsTobb_, "nHiggsTobb/i");
		mainAnaTree_->Branch("DelR_bb_Higgs1", &treeVar_DelRHiggs_bb_1_, "DelR_bb_Higgs1/D");
		mainAnaTree_->Branch("DelR_bb_Higgs2", &treeVar_DelRHiggs_bb_1_, "DelR_bb_Higgs2/D");

		mainAnaTree_->Branch("fatJetA_p4", &treeVar_fatJetA_p4Ptr_);
		mainAnaTree_->Branch("fatJetA_p4_jecUncUp", &treeVar_fatJetA_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("fatJetA_p4_jecUncDown", &treeVar_fatJetA_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("fatJetA_p4_jerUncUp", &treeVar_fatJetA_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("fatJetA_p4_jerUncDown", &treeVar_fatJetA_p4Ptr_jerUncDown_);

		mainAnaTree_->Branch("fatJetB_p4", &treeVar_fatJetB_p4Ptr_);
		mainAnaTree_->Branch("fatJetB_p4_jecUncUp", &treeVar_fatJetB_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("fatJetB_p4_jecUncDown", &treeVar_fatJetB_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("fatJetB_p4_jerUncUp", &treeVar_fatJetB_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("fatJetB_p4_jerUncDown", &treeVar_fatJetB_p4Ptr_jerUncDown_);

		mainAnaTree_->Branch("fatJetA_doubleBtagDiscrim", &treeVar_fatJetA_doubleBtagDiscrim_,   "fatJetA_doubleBtagDiscrim/D");
		mainAnaTree_->Branch("fatJetB_doubleBtagDiscrim", &treeVar_fatJetB_doubleBtagDiscrim_,   "fatJetB_doubleBtagDiscrim/D");

		mainAnaTree_->Branch("fatJetA_mass", &treeVar_fatJetA_mass_,   "fatJetA_mass/D");
		mainAnaTree_->Branch("fatJetB_mass", &treeVar_fatJetB_mass_,   "fatJetB_mass/D");

		mainAnaTree_->Branch("fatJetA_eta", &treeVar_fatJetA_eta_,   "fatJetA_eta/D");
		mainAnaTree_->Branch("fatJetB_eta", &treeVar_fatJetB_eta_,   "fatJetB_eta/D");

		mainAnaTree_->Branch("fatJetA_phi", &treeVar_fatJetA_phi_,   "fatJetA_phi/D");
		mainAnaTree_->Branch("fatJetB_phi", &treeVar_fatJetB_phi_,   "fatJetB_phi/D");

		mainAnaTree_->Branch("fatJetA_prunedMass", &treeVar_fatJetA_prunedMass_,   "fatJetA_prunedMass/D");
		mainAnaTree_->Branch("fatJetB_prunedMass", &treeVar_fatJetB_prunedMass_,   "fatJetB_prunedMass/D");

		mainAnaTree_->Branch("fatJetA_softDropMassCHS", &treeVar_fatJetA_softDropMassCHS_,   "fatJetA_softDropMassCHS/D");
		mainAnaTree_->Branch("fatJetB_softDropMassCHS", &treeVar_fatJetB_softDropMassCHS_,   "fatJetB_softDropMassCHS/D");

		mainAnaTree_->Branch("fatJetA_softDropMassPuppi", &treeVar_fatJetA_softDropMassPuppi_,   "fatJetA_softDropMassPuppi/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmsUncUp", &treeVar_fatJetA_softDropMassPuppi_jmsUncUp_,   "fatJetA_softDropMassPuppi_jmsUncUp/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmsUncDown", &treeVar_fatJetA_softDropMassPuppi_jmsUncDown_,   "fatJetA_softDropMassPuppi_jmsUncDown/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmrUncUp", &treeVar_fatJetA_softDropMassPuppi_jmrUncUp_,   "fatJetA_softDropMassPuppi_jmrUncUp/D");
		mainAnaTree_->Branch("fatJetA_softDropMassPuppi_jmrUncDown", &treeVar_fatJetA_softDropMassPuppi_jmrUncDown_,   "fatJetA_softDropMassPuppi_jmrUncDown/D");
		
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi", &treeVar_fatJetB_softDropMassPuppi_,   "fatJetB_softDropMassPuppi/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmsUncUp", &treeVar_fatJetB_softDropMassPuppi_jmsUncUp_,   "fatJetB_softDropMassPuppi_jmsUncUp/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmsUncDown", &treeVar_fatJetB_softDropMassPuppi_jmsUncDown_,   "fatJetB_softDropMassPuppi_jmsUncDown/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmrUncUp", &treeVar_fatJetB_softDropMassPuppi_jmrUncUp_,   "fatJetB_softDropMassPuppi_jmrUncUp/D");
		mainAnaTree_->Branch("fatJetB_softDropMassPuppi_jmrUncDown", &treeVar_fatJetB_softDropMassPuppi_jmrUncDown_,   "fatJetB_softDropMassPuppi_jmrUncDown/D");

		mainAnaTree_->Branch("fatJetA_electronEnergyFraction", &treeVar_fatJetA_electronEnergyFraction_, "fatJetA_electronEnergyFraction/F");
		mainAnaTree_->Branch("fatJetB_electronEnergyFraction", &treeVar_fatJetB_electronEnergyFraction_, "fatJetB_electronEnergyFraction/F");

		mainAnaTree_->Branch("fatJetA_muonEnergyFraction", &treeVar_fatJetA_muonEnergyFraction_, "fatJetA_muonEnergyFraction/F");
		mainAnaTree_->Branch("fatJetB_muonEnergyFraction", &treeVar_fatJetB_muonEnergyFraction_, "fatJetB_muonEnergyFraction/F");

		mainAnaTree_->Branch("fatJetA_nSubjettinessTau1", &treeVar_fatJetA_nSubjettinessTau1_, "fatJetA_nSubjettinessTau1/F");
		mainAnaTree_->Branch("fatJetA_nSubjettinessTau2", &treeVar_fatJetA_nSubjettinessTau2_, "fatJetA_nSubjettinessTau2/F");
		mainAnaTree_->Branch("fatJetB_nSubjettinessTau1", &treeVar_fatJetB_nSubjettinessTau1_, "fatJetB_nSubjettinessTau1/F");
		mainAnaTree_->Branch("fatJetB_nSubjettinessTau2", &treeVar_fatJetB_nSubjettinessTau2_, "fatJetB_nSubjettinessTau2/F");

		// Sums
		mainAnaTree_->Branch("bJetA_tag_score", &treeVar_bJetA_tag_score_, "bJetA_tag_score/F");
		mainAnaTree_->Branch("bJetB_tag_score", &treeVar_bJetB_tag_score_, "bJetB_tag_score/F");
		mainAnaTree_->Branch("bJetC_tag_score", &treeVar_bJetC_tag_score_, "bJetC_tag_score/F");
		mainAnaTree_->Branch("bJetD_tag_score", &treeVar_bJetD_tag_score_, "bJetD_tag_score/F");
		
		mainAnaTree_->Branch("ht", &treeVar_ht_, "ht/F");
		mainAnaTree_->Branch("ht_jecUncUp", &treeVar_ht_jecUncUp_, "ht_jecUncUp/F");
		mainAnaTree_->Branch("ht_jecUncDown", &treeVar_ht_jecUncDown_, "ht_jecUncDown/F");
		mainAnaTree_->Branch("ht_jerUncUp", &treeVar_ht_jerUncUp_, "ht_jerUncUp/F");
		mainAnaTree_->Branch("ht_jerUncDown", &treeVar_ht_jerUncDown_, "ht_jerUncDown/F");

		mainAnaTree_->Branch("mht", &treeVar_mht_, "mht/F");
		mainAnaTree_->Branch("mht_jecUncUp", &treeVar_mht_jecUncUp_, "mht_jecUncUp/F");
		mainAnaTree_->Branch("mht_jecUncDown", &treeVar_mht_jecUncDown_, "mht_jecUncDown/F");
		mainAnaTree_->Branch("mht_jerUncUp", &treeVar_mht_jerUncUp_, "mht_jerUncUp/F");
		mainAnaTree_->Branch("mht_jerUncDown", &treeVar_mht_jerUncDown_, "mht_jerUncDown/F");

		mainAnaTree_->Branch("mht_phi", &treeVar_mht_phi_, "mht/F");
		mainAnaTree_->Branch("mht_phi_jecUncUp", &treeVar_mht_phi_jecUncUp_, "mht_jecUncUp/F");
		mainAnaTree_->Branch("mht_phi_jecUncDown", &treeVar_mht_phi_jecUncDown_, "mht_jecUncDown/F");
		mainAnaTree_->Branch("mht_phi_jerUncUp", &treeVar_mht_phi_jerUncUp_, "mht_jerUncUp/F");
		mainAnaTree_->Branch("mht_phi_jerUncDown", &treeVar_mht_phi_jerUncDown_, "mht_jerUncDown/F");

		mainAnaTree_->Branch("nrSlimJets", &treeVar_nrSlimJets_, "nrSlimJets/i");
		mainAnaTree_->Branch("nrSlimJetsByBtagScore", &treeVar_nrSlimJetsByBtagScore_, "nrSlimJetsByBtagScore/i");
		mainAnaTree_->Branch("nrSlimLooseBJets", &treeVar_nrSlimLooseBJets_, "nrSlimLooseBJets/i");
		mainAnaTree_->Branch("nrSlimMediumBJets", &treeVar_nrSlimMediumBJets_, "nrSlimMediumBJets/i");
		mainAnaTree_->Branch("nrSepSlimJets", &treeVar_nrSepSlimJets_, "nrSepSlimJets/i");
		mainAnaTree_->Branch("nrSepSlimLooseBJets", &treeVar_nrSepSlimLooseBJets_, "nrSepSlimLooseBJets/i");
		mainAnaTree_->Branch("nrSepSlimMediumBJets", &treeVar_nrSepSlimMediumBJets_, "nrSepSlimMediumBJets/i");
		mainAnaTree_->Branch("nrFatJets", &treeVar_nrFatJets_, "nrFatJets/i");
		mainAnaTree_->Branch("nrLooseMuons", &treeVar_nrLooseMuons_, "nrLooseMuons/i");
		mainAnaTree_->Branch("nrTightMuons", &treeVar_nrTightMuons_, "nrTightMuons/i");
		mainAnaTree_->Branch("nrElectrons", &treeVar_nrElectrons_, "nrElectrons/i");
		mainAnaTree_->Branch("nrPhotons", &treeVar_nrPhotons_, "nrPhotons/i");
		mainAnaTree_->Branch("nrTracks", &treeVar_nrTracks_, "nrTracks/i");
		mainAnaTree_->Branch("nrPrefireJets", &treeVar_nrPrefireJets_, "nrPrefireJets/i");
		mainAnaTree_->Branch("nrPrefirePhotons", &treeVar_nrPrefirePhotons_, "nrPrefirePhotons/i");

		// Slim jets
		mainAnaTree_->Branch("slimJetA_p4", &treeVar_jetA_p4Ptr_);
		mainAnaTree_->Branch("slimJetA_p4_jecUncUp", &treeVar_jetA_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("slimJetA_p4_jecUncDown", &treeVar_jetA_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("slimJetA_p4_jerUncUp", &treeVar_jetA_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("slimJetA_p4_jerUncDown", &treeVar_jetA_p4Ptr_jerUncDown_);

		mainAnaTree_->Branch("slimJetB_p4", &treeVar_jetB_p4Ptr_);
		mainAnaTree_->Branch("slimJetB_p4_jecUncUp", &treeVar_jetB_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("slimJetB_p4_jecUncDown", &treeVar_jetB_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("slimJetB_p4_jerUncUp", &treeVar_jetB_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("slimJetB_p4_jerUncDown", &treeVar_jetB_p4Ptr_jerUncDown_);

		// b-tagged jets, in descending b-tag score order
		mainAnaTree_->Branch("bJetA_p4", &treeVar_bjetA_p4Ptr_);
		mainAnaTree_->Branch("bJetA_p4_jecUncUp", &treeVar_bjetA_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("bJetA_p4_jecUncDown", &treeVar_bjetA_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("bJetA_p4_jerUncUp", &treeVar_bjetA_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("bJetA_p4_jerUncDown", &treeVar_bjetA_p4Ptr_jerUncDown_);

		mainAnaTree_->Branch("bJetB_p4", &treeVar_bjetB_p4Ptr_);
		mainAnaTree_->Branch("bJetB_p4_jecUncUp", &treeVar_bjetB_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("bJetB_p4_jecUncDown", &treeVar_bjetB_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("bJetB_p4_jerUncUp", &treeVar_bjetB_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("bJetB_p4_jerUncDown", &treeVar_bjetB_p4Ptr_jerUncDown_);

		mainAnaTree_->Branch("bJetC_p4", &treeVar_bjetC_p4Ptr_);
		mainAnaTree_->Branch("bJetC_p4_jecUncUp", &treeVar_bjetC_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("bJetC_p4_jecUncDown", &treeVar_bjetC_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("bJetC_p4_jerUncUp", &treeVar_bjetC_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("bJetC_p4_jerUncDown", &treeVar_bjetC_p4Ptr_jerUncDown_);

		mainAnaTree_->Branch("bJetD_p4", &treeVar_bjetD_p4Ptr_);
		mainAnaTree_->Branch("bJetD_p4_jecUncUp", &treeVar_bjetD_p4Ptr_jecUncUp_);
		mainAnaTree_->Branch("blimJetD_p4_jecUncDown", &treeVar_bjetD_p4Ptr_jecUncDown_);
		mainAnaTree_->Branch("bJetD_p4_jerUncUp", &treeVar_bjetD_p4Ptr_jerUncUp_);
		mainAnaTree_->Branch("bJetD_p4_jerUncDown", &treeVar_bjetD_p4Ptr_jerUncDown_);

		// Electrons
		mainAnaTree_->Branch("electronA_p4", &treeVar_electronA_p4Ptr_);
		mainAnaTree_->Branch("electronB_p4", &treeVar_electronB_p4Ptr_);

		// Muons
		mainAnaTree_->Branch("muonA_p4", &treeVar_muonA_p4Ptr_);
		mainAnaTree_->Branch("muonB_p4", &treeVar_muonB_p4Ptr_);
	}

	~FatDoubleBJetPairTree(){}

	void fillTree(const std::string& sampleType, const ran::EventInfo& evtInfo, const ran::NtFatJet& fatJetA, const ran::NtFatJet& fatJetB, const float& ht, const float& ht_jecUncUp,
					const float& ht_jecUncDown, const float& ht_jerUncUp, const float& ht_jerUncDown, const float& mht, const float& mht_jecUncUp, const float& mht_jecUncDown,
					const float& mht_jerUncUp, const float& mht_jerUncDown, const float& mht_phi, const float& mht_phi_jecUncUp, const float& mht_phi_jecUncDown,
					const float& mht_phi_jerUncUp, const float& mht_phi_jerUncDown, const std::vector<ran::NtJet>& slimJets, const std::vector<ran::NtJet>& allSlimJets, const std::vector<ran::NtJet>& slimJetsByBtagScore,
					const std::vector<ran::NtJet>& looseBJets, const std::vector<ran::NtJet>& allLooseBJets, const std::vector<ran::NtJet>& mediumBJets, const std::vector<ran::NtJet>& allMediumBJets, unsigned int nrFatJets,
					const std::vector<ran::NtElectron>& centralElectrons, const std::vector<ran::NtMuon>& tightMuons, unsigned int nrLooseMuons, unsigned int nrTightMuons,
					const int nrElectrons, const int nrPhotons, const int nrTracks,
					const bool& trigDecision, const int& nPU, int nISR, const int& nGluino, const int& nHiggs2bb, const float& DelRHiggs2bb_1, const float& DelRHiggs2bb_2, const double& D_factor, const unsigned int yearOfRun,
					const int& nrPrefireJets, const int& nrPrefirePhotons)
	{
		// Hack: Muon mass for TLorentzVector...
		double muonMass = 0.105658;

		// DO THE WEIGHTS

		if (sampleType == "SIGNAL"){

			if (nrFatJets > 1)
			{
				treeVar_weight_dbtTag_ = DoubleBTagSF::getDbtTagScaleFactor_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
				treeVar_weight_dbtTagUp_ = DoubleBTagSF::getDbtTagScaleFactorUp_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
				treeVar_weight_dbtTagDown_ = DoubleBTagSF::getDbtTagScaleFactorDown_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
			}
			else if(nrFatJets == 1)
			{
				treeVar_weight_dbtTag_ = DoubleBTagSF::getDbtTagScaleFactor_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), 0, -1, yearOfRun);
				treeVar_weight_dbtTagUp_ = DoubleBTagSF::getDbtTagScaleFactorUp_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), 0, -1, yearOfRun);
				treeVar_weight_dbtTagDown_ = DoubleBTagSF::getDbtTagScaleFactorDown_signal(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), 0, -1, yearOfRun);
			}
			else
			{
				treeVar_weight_dbtTag_ = 1.0;
				treeVar_weight_dbtTagUp_ = 1.0;
				treeVar_weight_dbtTagDown_ = 1.0;
			}

			// calculate isr weights
			if (nISR > 6) nISR = 6;
			double corr_isr = D_factor * IsrWeights::GetWeight(nISR, yearOfRun);
			treeVar_weight_isr_ = corr_isr;
			treeVar_weight_isrUp_ = corr_isr + 0.5 * (corr_isr - 1);
			treeVar_weight_isrDown_ = corr_isr - 0.5 * (corr_isr - 1);

			// combined nominal scale factor weights
			treeVar_weight_combined_ = treeVar_weight_dbtTag_ * treeVar_weight_isr_;
		}
		
		else if (sampleType == "TTJETS"){

			if (nrFatJets > 1)
			{
				treeVar_weight_dbtTag_ = DoubleBTagSF::getDbtTagScaleFactor_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
				treeVar_weight_dbtTagUp_ = DoubleBTagSF::getDbtTagScaleFactorUp_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
				treeVar_weight_dbtTagDown_ = DoubleBTagSF::getDbtTagScaleFactorDown_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), fatJetB.pt(), fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags(), yearOfRun);
			}
			else if(nrFatJets == 1)
			{
				treeVar_weight_dbtTag_ = DoubleBTagSF::getDbtTagScaleFactor_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), 0, -1, yearOfRun);
				treeVar_weight_dbtTagUp_ = DoubleBTagSF::getDbtTagScaleFactorUp_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), 0, -1, yearOfRun);
				treeVar_weight_dbtTagDown_ = DoubleBTagSF::getDbtTagScaleFactorDown_ttbar(fatJetA.pt(), fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags(), 0, -1, yearOfRun);
			}
			treeVar_weight_isr_ = 1.0;
			treeVar_weight_isrUp_ = 1.0;
			treeVar_weight_isrDown_ = 1.0;
			
			// combined nominal scale factor weights
			treeVar_weight_combined_ = treeVar_weight_dbtTag_;
		}

		else{
			treeVar_weight_dbtTag_ = 1.0;
			treeVar_weight_dbtTagUp_ = 1.0;
			treeVar_weight_dbtTagDown_ = 1.0;

			treeVar_weight_isr_ = 1.0;
			treeVar_weight_isrUp_ = 1.0;
			treeVar_weight_isrDown_ = 1.0;

			// combined nominal scale factor weights
			treeVar_weight_combined_ = 1.0;
		}


		// DO THE VARIABLES

		treeVar_runNum_ = evtInfo.runNum;
		treeVar_lumiSec_ = evtInfo.lumiSec;
		treeVar_evtNum_ = evtInfo.evtNum;

		treeVar_trgDecision_ = trigDecision;
		treeVar_nPU_ = nPU;
		treeVar_nGluino_ = nGluino;
		treeVar_nHiggsTobb_ = nHiggs2bb;
		treeVar_DelRHiggs_bb_1_ = DelRHiggs2bb_1;
		treeVar_DelRHiggs_bb_2_ = DelRHiggs2bb_2;
		treeVar_nrFatJets_ = nrFatJets;
		treeVar_nrLooseMuons_ = nrLooseMuons;
		treeVar_nrTightMuons_ = nrTightMuons;
		treeVar_nrElectrons_ = nrElectrons;
		treeVar_nrPhotons_ = nrPhotons;
		treeVar_nrTracks_ = nrTracks;
		treeVar_nrPrefireJets_ = nrPrefireJets;
		treeVar_nrPrefirePhotons_ = nrPrefirePhotons;

		if (nrFatJets == 0)
		{
			treeVar_fatJetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_fatJetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_fatJetA_doubleBtagDiscrim_ = -2;
			treeVar_fatJetB_doubleBtagDiscrim_ = -2;

			treeVar_fatJetA_mass_ = -1;
			treeVar_fatJetB_mass_ = -1;

			treeVar_fatJetA_eta_ = 0;
			treeVar_fatJetB_eta_ = 0;

			treeVar_fatJetA_phi_ = 0;
			treeVar_fatJetB_phi_ = 0;

			treeVar_fatJetA_prunedMass_ = 0;
			treeVar_fatJetB_prunedMass_ = 0;

			treeVar_fatJetA_softDropMassCHS_ = 0;
			treeVar_fatJetB_softDropMassCHS_ = 0;

			treeVar_fatJetA_nSubjettinessTau1_ = 0;
			treeVar_fatJetA_nSubjettinessTau2_ = 0;
			treeVar_fatJetB_nSubjettinessTau1_ = 0;
			treeVar_fatJetB_nSubjettinessTau2_ = 0;

			treeVar_fatJetA_electronEnergyFraction_ = 0;
			treeVar_fatJetB_electronEnergyFraction_ = 0;

			treeVar_fatJetA_muonEnergyFraction_ = 0;
			treeVar_fatJetB_muonEnergyFraction_ = 0;
		}
		else if(nrFatJets == 1)
		{
			{
			treeVar_fatJetA_p4_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_fatJetA_doubleBtagDiscrim_ = fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags();
			treeVar_fatJetB_doubleBtagDiscrim_ = -2;

			treeVar_fatJetA_mass_ = fatJetA.mass();
			treeVar_fatJetB_mass_ = -1;

			treeVar_fatJetA_eta_ = fatJetA.eta();
			treeVar_fatJetB_eta_ = 0;

			treeVar_fatJetA_phi_ = fatJetA.phi();
			treeVar_fatJetB_phi_ = 0;

			treeVar_fatJetA_prunedMass_ = fatJetA.CHSpruned_mass();
			treeVar_fatJetB_prunedMass_ = 0;

			treeVar_fatJetA_softDropMassCHS_ = fatJetA.CHSsoftdrop_mass();
			treeVar_fatJetB_softDropMassCHS_ = 0;

			treeVar_fatJetA_nSubjettinessTau1_ = fatJetA.NjettinessAK8_tau1();
			treeVar_fatJetA_nSubjettinessTau2_ = fatJetA.NjettinessAK8_tau2();
			treeVar_fatJetB_nSubjettinessTau1_ = 0;
			treeVar_fatJetB_nSubjettinessTau2_ = 0;

			treeVar_fatJetA_electronEnergyFraction_ = fatJetA.electronEnergyFraction();
			treeVar_fatJetB_electronEnergyFraction_ = 0;

			treeVar_fatJetA_muonEnergyFraction_ = fatJetA.muonEnergyFraction();
			treeVar_fatJetB_muonEnergyFraction_ = 0;
		}
		}
		else
		{
			treeVar_fatJetA_p4_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetB_p4_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));

			treeVar_fatJetA_doubleBtagDiscrim_ = fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags();
			treeVar_fatJetB_doubleBtagDiscrim_ = fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags();

			treeVar_fatJetA_mass_ = fatJetA.mass();
			treeVar_fatJetB_mass_ = fatJetB.mass();

			treeVar_fatJetA_eta_ = fatJetA.eta();
			treeVar_fatJetB_eta_ = fatJetB.eta();

			treeVar_fatJetA_phi_ = fatJetA.phi();
			treeVar_fatJetB_phi_ = fatJetB.phi();

			treeVar_fatJetA_prunedMass_ = fatJetA.CHSpruned_mass();
			treeVar_fatJetB_prunedMass_ = fatJetB.CHSpruned_mass();

			treeVar_fatJetA_softDropMassCHS_ = fatJetA.CHSsoftdrop_mass();
			treeVar_fatJetB_softDropMassCHS_ = fatJetB.CHSsoftdrop_mass();

			treeVar_fatJetA_nSubjettinessTau1_ = fatJetA.NjettinessAK8_tau1();
			treeVar_fatJetA_nSubjettinessTau2_ = fatJetA.NjettinessAK8_tau2();
			treeVar_fatJetB_nSubjettinessTau1_ = fatJetB.NjettinessAK8_tau1();
			treeVar_fatJetB_nSubjettinessTau2_ = fatJetB.NjettinessAK8_tau2();

			treeVar_fatJetA_electronEnergyFraction_ = fatJetA.electronEnergyFraction();
			treeVar_fatJetB_electronEnergyFraction_ = fatJetB.electronEnergyFraction();

			treeVar_fatJetA_muonEnergyFraction_ = fatJetA.muonEnergyFraction();
			treeVar_fatJetB_muonEnergyFraction_ = fatJetB.muonEnergyFraction();
		}

		treeVar_ht_ = ht;
		treeVar_mht_ = mht;
		treeVar_mht_phi_ = mht_phi;
		treeVar_nrSlimJets_ = allSlimJets.size();
		treeVar_nrSlimLooseBJets_ = allLooseBJets.size();
		treeVar_nrSlimMediumBJets_ = allMediumBJets.size();
		treeVar_nrSepSlimJets_ = slimJets.size();
		treeVar_nrSepSlimLooseBJets_ = looseBJets.size();
		treeVar_nrSepSlimMediumBJets_ = mediumBJets.size();
		treeVar_nrSlimJetsByBtagScore_ = slimJetsByBtagScore.size();

		if (sampleType != "DATA"){
			if (nrFatJets == 0)
			{
				treeVar_fatJetA_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_fatJetA_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_fatJetA_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_fatJetA_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_fatJetB_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_fatJetB_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_fatJetB_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_fatJetB_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}
			else
			{
				treeVar_fatJetA_p4_jecUncUp_.SetPtEtaPhiE(fatJetA.pt() * (1.0 + fatJetA.jecUncertainty()), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * (1.0 + fatJetA.jecUncertainty()) );
				treeVar_fatJetA_p4_jecUncDown_.SetPtEtaPhiE(fatJetA.pt() * (1.0 - fatJetA.jecUncertainty()), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * (1.0 - fatJetA.jecUncertainty()) );
				treeVar_fatJetA_p4_jerUncUp_.SetPtEtaPhiE(fatJetA.pt() * fatJetA.jerUncUp(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * fatJetA.jerUncUp() );
				treeVar_fatJetA_p4_jerUncDown_.SetPtEtaPhiE(fatJetA.pt() * fatJetA.jerUncDown(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()) * fatJetA.jerUncDown() );

				if (nrFatJets > 1)
				{
					treeVar_fatJetB_p4_jecUncUp_.SetPtEtaPhiE(fatJetB.pt() * (1.0 + fatJetB.jecUncertainty()), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * (1.0 + fatJetB.jecUncertainty()) );
					treeVar_fatJetB_p4_jecUncDown_.SetPtEtaPhiE(fatJetB.pt() * (1.0 - fatJetB.jecUncertainty()), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * (1.0 - fatJetB.jecUncertainty()) );
					treeVar_fatJetB_p4_jerUncUp_.SetPtEtaPhiE(fatJetB.pt() * fatJetB.jerUncUp(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * fatJetB.jerUncUp() );
					treeVar_fatJetB_p4_jerUncDown_.SetPtEtaPhiE(fatJetB.pt() * fatJetB.jerUncDown(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()) * fatJetB.jerUncDown() );
				}
			}

			treeVar_ht_jecUncUp_ = ht_jecUncUp;
			treeVar_ht_jecUncDown_ = ht_jecUncDown;
			treeVar_ht_jerUncUp_ = ht_jerUncUp;
			treeVar_ht_jerUncDown_ = ht_jerUncDown;

			treeVar_mht_jecUncUp_ = mht_jecUncUp;
			treeVar_mht_jecUncDown_ = mht_jecUncDown;
			treeVar_mht_jerUncUp_ = mht_jerUncUp;
			treeVar_mht_jerUncDown_ = mht_jerUncDown;

			treeVar_mht_phi_jecUncUp_ = mht_phi_jecUncUp;
			treeVar_mht_phi_jecUncDown_ = mht_phi_jecUncDown;
			treeVar_mht_phi_jerUncUp_ = mht_phi_jerUncUp;
			treeVar_mht_phi_jerUncDown_ = mht_phi_jerUncDown;

			if (slimJets.size() > 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecUncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 + slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 + slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jecUncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 - slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 - slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jerUncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncUp(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncUp() );
				treeVar_jetA_p4_jerUncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncDown(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncDown() );

				treeVar_jetB_p4_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jecUncUp_.SetPtEtaPhiE(slimJets.at(1).pt() * (1.0 + slimJets.at(1).jecUncertainty()), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * (1.0 + slimJets.at(1).jecUncertainty()) );
				treeVar_jetB_p4_jecUncDown_.SetPtEtaPhiE(slimJets.at(1).pt() * (1.0 - slimJets.at(1).jecUncertainty()), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * (1.0 - slimJets.at(1).jecUncertainty()) );
				treeVar_jetB_p4_jerUncUp_.SetPtEtaPhiE(slimJets.at(1).pt() * slimJets.at(1).jerUncUp(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * slimJets.at(1).jerUncUp() );
				treeVar_jetB_p4_jerUncDown_.SetPtEtaPhiE(slimJets.at(1).pt() * slimJets.at(1).jerUncDown(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) * slimJets.at(1).jerUncDown() );
			}
			else if (slimJets.size() == 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecUncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 + slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 + slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jecUncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * (1.0 - slimJets.at(0).jecUncertainty()), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * (1.0 - slimJets.at(0).jecUncertainty()) );
				treeVar_jetA_p4_jerUncUp_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncUp(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncUp() );
				treeVar_jetA_p4_jerUncDown_.SetPtEtaPhiE(slimJets.at(0).pt() * slimJets.at(0).jerUncDown(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) * slimJets.at(0).jerUncDown() );

				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}
			else {
				treeVar_jetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}

			// Slim jets by b-tag score (excluding those too close to DBT AK8 jets)

			treeVar_bJetA_tag_score_ = -1;
			treeVar_bJetB_tag_score_ = -1;
			treeVar_bJetC_tag_score_ = -1;
			treeVar_bJetD_tag_score_ = -1;

			treeVar_bjetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_bjetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_bjetC_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_bjetD_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			if (slimJetsByBtagScore.size() > 0){
				treeVar_bJetA_tag_score_ = slimJetsByBtagScore.at(0).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetA_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) );
				treeVar_bjetA_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt() * (1.0 + slimJetsByBtagScore.at(0).jecUncertainty()), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) * (1.0 + slimJetsByBtagScore.at(0).jecUncertainty()) );
				treeVar_bjetA_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt() * (1.0 - slimJetsByBtagScore.at(0).jecUncertainty()), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) * (1.0 - slimJetsByBtagScore.at(0).jecUncertainty()) );
				treeVar_bjetA_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt() * slimJetsByBtagScore.at(0).jerUncUp(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) * slimJetsByBtagScore.at(0).jerUncUp() );
				treeVar_bjetA_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt() * slimJetsByBtagScore.at(0).jerUncDown(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) * slimJetsByBtagScore.at(0).jerUncDown() );
			}
			if (slimJetsByBtagScore.size() > 1){
				treeVar_bJetB_tag_score_ = slimJetsByBtagScore.at(1).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetB_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) );
				treeVar_bjetB_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt() * (1.0 + slimJetsByBtagScore.at(1).jecUncertainty()), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) * (1.0 + slimJetsByBtagScore.at(1).jecUncertainty()) );
				treeVar_bjetB_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt() * (1.0 - slimJetsByBtagScore.at(1).jecUncertainty()), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) * (1.0 - slimJetsByBtagScore.at(1).jecUncertainty()) );
				treeVar_bjetB_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt() * slimJetsByBtagScore.at(1).jerUncUp(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) * slimJetsByBtagScore.at(1).jerUncUp() );
				treeVar_bjetB_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt() * slimJetsByBtagScore.at(1).jerUncDown(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) * slimJetsByBtagScore.at(1).jerUncDown() );
			}
			if (slimJetsByBtagScore.size() > 2){
				treeVar_bJetC_tag_score_ = slimJetsByBtagScore.at(2).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetC_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) );
				treeVar_bjetC_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt() * (2.0 + slimJetsByBtagScore.at(2).jecUncertainty()), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) * (2.0 + slimJetsByBtagScore.at(2).jecUncertainty()) );
				treeVar_bjetC_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt() * (2.0 - slimJetsByBtagScore.at(2).jecUncertainty()), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) * (2.0 - slimJetsByBtagScore.at(2).jecUncertainty()) );
				treeVar_bjetC_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt() * slimJetsByBtagScore.at(2).jerUncUp(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) * slimJetsByBtagScore.at(2).jerUncUp() );
				treeVar_bjetC_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt() * slimJetsByBtagScore.at(2).jerUncDown(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) * slimJetsByBtagScore.at(2).jerUncDown() );
			}
			if (slimJetsByBtagScore.size() > 3){
				treeVar_bJetD_tag_score_ = slimJetsByBtagScore.at(3).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetD_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) );
				treeVar_bjetD_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt() * (3.0 + slimJetsByBtagScore.at(3).jecUncertainty()), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) * (3.0 + slimJetsByBtagScore.at(3).jecUncertainty()) );
				treeVar_bjetD_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt() * (3.0 - slimJetsByBtagScore.at(3).jecUncertainty()), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) * (3.0 - slimJetsByBtagScore.at(3).jecUncertainty()) );
				treeVar_bjetD_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt() * slimJetsByBtagScore.at(3).jerUncUp(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) * slimJetsByBtagScore.at(3).jerUncUp() );
				treeVar_bjetD_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt() * slimJetsByBtagScore.at(3).jerUncDown(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) * slimJetsByBtagScore.at(3).jerUncDown() );
			}

			// Electrons
			if (centralElectrons.size() > 1){
				treeVar_electronA_p4_.SetPtEtaPhiE(centralElectrons.at(0).pt(), centralElectrons.at(0).eta(), centralElectrons.at(0).phi(), centralElectrons.at(0).et() * cosh(centralElectrons.at(0).eta()) );
				treeVar_electronB_p4_.SetPtEtaPhiE(centralElectrons.at(1).pt(), centralElectrons.at(1).eta(), centralElectrons.at(1).phi(), centralElectrons.at(1).et() * cosh(centralElectrons.at(1).eta()) );
			}
			else if (centralElectrons.size() == 1){
				treeVar_electronA_p4_.SetPtEtaPhiE(centralElectrons.at(0).pt(), centralElectrons.at(0).eta(), centralElectrons.at(0).phi(), centralElectrons.at(0).et() * cosh(centralElectrons.at(0).eta()) );
				treeVar_electronB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			}
			else {
				treeVar_electronA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_electronB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			}

			// Muons
			if (tightMuons.size() > 1){
				treeVar_muonA_p4_.SetPtEtaPhiM(tightMuons.at(0).pt(), tightMuons.at(0).eta(), tightMuons.at(0).phi(), muonMass );
				treeVar_muonB_p4_.SetPtEtaPhiM(tightMuons.at(1).pt(), tightMuons.at(1).eta(), tightMuons.at(1).phi(), muonMass );
			}
			else if (tightMuons.size() == 1){
				treeVar_muonA_p4_.SetPtEtaPhiM(tightMuons.at(0).pt(), tightMuons.at(0).eta(), tightMuons.at(0).phi(), muonMass );
				treeVar_muonB_p4_.SetPtEtaPhiM(0, 0, 0, muonMass);
			}
			else {
				treeVar_muonA_p4_.SetPtEtaPhiM(0, 0, 0, muonMass);
				treeVar_muonB_p4_.SetPtEtaPhiM(0, 0, 0, muonMass);
			}

			// fatJetA SoftDropMass+PUPPI
			if (nrFatJets > 0)
			{
				std::random_device rdA;
				std::mt19937 e2A(rdA());
				double mcMassResolutionToUseA = SoftDropPuppiSF::get_mcMassResolution(yearOfRun);
				if (mcMassResolutionToUseA > 0.333333 * fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun)) mcMassResolutionToUseA = 0.333333 * fabs(fatJetA.PUPPIsoftdrop_mass()) * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun);
				std::normal_distribution<> distA(0, mcMassResolutionToUseA);
				double gaussRandomA = distA(e2A);
				
				// nominal resolution and JMS
				if (SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) > 1.0){
					double smearingValue = gaussRandomA * sqrt(SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) - 1);
					treeVar_fatJetA_softDropMassPuppi_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
					treeVar_fatJetA_softDropMassPuppi_jmsUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue : 0.0;
					treeVar_fatJetA_softDropMassPuppi_jmsUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue : 0.0;
				}
				else {
					treeVar_fatJetA_softDropMassPuppi_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
					treeVar_fatJetA_softDropMassPuppi_jmsUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) : 0.0;
					treeVar_fatJetA_softDropMassPuppi_jmsUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) : 0.0;
				}
				
				// JMR_UP
				if (SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) > 1.0){
					double smearingValue = gaussRandomA * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) - 1);
					treeVar_fatJetA_softDropMassPuppi_jmrUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
				}
				else {
					treeVar_fatJetA_softDropMassPuppi_jmrUncUp_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 : 0.0;
				}
				
				// JMR_DOWN
				if (SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) > 1.0){
					double smearingValue = gaussRandomA * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) - 1);
					treeVar_fatJetA_softDropMassPuppi_jmrUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
				}
				else {
					treeVar_fatJetA_softDropMassPuppi_jmrUncDown_ = fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetA.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
				}

				if (nrFatJets > 1)
				{

					// fatJetB SoftDropMass+PUPPI
					std::random_device rdB;
					std::mt19937 e2B(rdB());
					double mcMassResolutionToUseB = SoftDropPuppiSF::get_mcMassResolution(yearOfRun);
					if (mcMassResolutionToUseB > 0.333333 * fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun)) mcMassResolutionToUseB = 0.333333 * fabs(fatJetB.PUPPIsoftdrop_mass()) * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun);
					std::normal_distribution<> distB(0, mcMassResolutionToUseB);
					double gaussRandomB = distB(e2B);
					
					// nominal resolution and JMS
					if (SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) > 1.0){
						double smearingValue = gaussRandomB * sqrt(SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSF(yearOfRun) - 1);
						treeVar_fatJetB_softDropMassPuppi_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
						treeVar_fatJetB_softDropMassPuppi_jmsUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) + smearingValue : 0.0;
						treeVar_fatJetB_softDropMassPuppi_jmsUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) + smearingValue : 0.0;
					}
					else {
						treeVar_fatJetB_softDropMassPuppi_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
						treeVar_fatJetB_softDropMassPuppi_jmsUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFUp(yearOfRun) : 0.0;
						treeVar_fatJetB_softDropMassPuppi_jmsUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSFDown(yearOfRun) : 0.0;
					}
					
					// JMR_UP
					if (SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) > 1.0){
						double smearingValue = gaussRandomB * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFUp(yearOfRun) - 1);
						treeVar_fatJetB_softDropMassPuppi_jmrUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
					}
					else {
						treeVar_fatJetB_softDropMassPuppi_jmrUncUp_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 : 0.0;
					}
					
					// JMR_DOWN
					if (SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) > 1.0){
						double smearingValue = gaussRandomB * sqrt(SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) * SoftDropPuppiSF::get_jetMassResolutionSFDown(yearOfRun) - 1);
						treeVar_fatJetB_softDropMassPuppi_jmrUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) + smearingValue : 0.0;
					}
					else {
						treeVar_fatJetB_softDropMassPuppi_jmrUncDown_ = fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) > 0.0 ? fatJetB.PUPPIsoftdrop_mass() * SoftDropPuppiSF::get_jetMassScaleSF(yearOfRun) : 0.0;
					}
				}
			}


		} // closes 'if' sampleType does NOT equal DATA
		
		else {
			treeVar_fatJetA_p4_jecUncUp_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetA_p4_jecUncDown_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetA_p4_jerUncUp_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
			treeVar_fatJetA_p4_jerUncDown_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));

			treeVar_fatJetB_p4_jecUncUp_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));
			treeVar_fatJetB_p4_jecUncDown_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));
			treeVar_fatJetB_p4_jerUncUp_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));
			treeVar_fatJetB_p4_jerUncDown_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));

			treeVar_ht_jecUncUp_ = ht;
			treeVar_ht_jecUncDown_ = ht;
			treeVar_ht_jerUncUp_ = ht;
			treeVar_ht_jerUncDown_ = ht;

			treeVar_mht_jecUncUp_ = mht;
			treeVar_mht_jecUncDown_ = mht;
			treeVar_mht_jerUncUp_ = mht;
			treeVar_mht_jerUncDown_ = mht;

			treeVar_mht_phi_jecUncUp_ = mht_phi;
			treeVar_mht_phi_jecUncDown_ = mht_phi;
			treeVar_mht_phi_jerUncUp_ = mht_phi;
			treeVar_mht_phi_jerUncDown_ = mht_phi;

			if (slimJets.size() > 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecUncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecUncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerUncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerUncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );

				treeVar_jetB_p4_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jecUncUp_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jecUncDown_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jerUncUp_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
				treeVar_jetB_p4_jerUncDown_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
			}
			else if (slimJets.size() == 1){
				treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecUncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jecUncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerUncUp_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
				treeVar_jetA_p4_jerUncDown_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );

				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}
			else {
				treeVar_jetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetA_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				
				treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_jetB_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			}

			// Slim jets by b-tag score (excluding those too close to DBT AK8 jets)

			treeVar_bJetA_tag_score_ = -1;
			treeVar_bJetB_tag_score_ = -1;
			treeVar_bJetC_tag_score_ = -1;
			treeVar_bJetD_tag_score_ = -1;

			treeVar_bjetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetA_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_bjetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetB_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_bjetC_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetC_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			treeVar_bjetD_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jecUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jecUncDown_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jerUncUp_.SetPtEtaPhiE(0, 0, 0, 0);
			treeVar_bjetD_p4_jerUncDown_.SetPtEtaPhiE(0, 0, 0, 0);

			if (slimJetsByBtagScore.size() > 0){
				treeVar_bJetA_tag_score_ = slimJetsByBtagScore.at(0).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetA_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) );
				treeVar_bjetA_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) );
				treeVar_bjetA_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) );
				treeVar_bjetA_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) );
				treeVar_bjetA_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(0).pt(), slimJetsByBtagScore.at(0).eta(), slimJetsByBtagScore.at(0).phi(), slimJetsByBtagScore.at(0).et() * cosh(slimJetsByBtagScore.at(0).eta()) );
			}
			if (slimJetsByBtagScore.size() > 1){
				treeVar_bJetB_tag_score_ = slimJetsByBtagScore.at(1).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetB_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) );
				treeVar_bjetB_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) );
				treeVar_bjetB_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) );
				treeVar_bjetB_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) );
				treeVar_bjetB_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(1).pt(), slimJetsByBtagScore.at(1).eta(), slimJetsByBtagScore.at(1).phi(), slimJetsByBtagScore.at(1).et() * cosh(slimJetsByBtagScore.at(1).eta()) );
			}
			if (slimJetsByBtagScore.size() > 2){
				treeVar_bJetC_tag_score_ = slimJetsByBtagScore.at(2).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetC_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) );
				treeVar_bjetC_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) );
				treeVar_bjetC_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) );
				treeVar_bjetC_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) );
				treeVar_bjetC_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(2).pt(), slimJetsByBtagScore.at(2).eta(), slimJetsByBtagScore.at(2).phi(), slimJetsByBtagScore.at(2).et() * cosh(slimJetsByBtagScore.at(2).eta()) );
			}
			if (slimJetsByBtagScore.size() > 3){
				treeVar_bJetD_tag_score_ = slimJetsByBtagScore.at(3).pfCombinedInclusiveSecondaryVertexV2BJetTags();
				treeVar_bjetD_p4_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) );
				treeVar_bjetD_p4_jecUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) );
				treeVar_bjetD_p4_jecUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) );
				treeVar_bjetD_p4_jerUncUp_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) );
				treeVar_bjetD_p4_jerUncDown_.SetPtEtaPhiE(slimJetsByBtagScore.at(3).pt(), slimJetsByBtagScore.at(3).eta(), slimJetsByBtagScore.at(3).phi(), slimJetsByBtagScore.at(3).et() * cosh(slimJetsByBtagScore.at(3).eta()) );
			}

			// Electrons
			if (centralElectrons.size() > 1){
				treeVar_electronA_p4_.SetPtEtaPhiE(centralElectrons.at(0).pt(), centralElectrons.at(0).eta(), centralElectrons.at(0).phi(), centralElectrons.at(0).et() * cosh(centralElectrons.at(0).eta()) );
				treeVar_electronB_p4_.SetPtEtaPhiE(centralElectrons.at(1).pt(), centralElectrons.at(1).eta(), centralElectrons.at(1).phi(), centralElectrons.at(1).et() * cosh(centralElectrons.at(1).eta()) );
			}
			else if (centralElectrons.size() == 1){
				treeVar_electronA_p4_.SetPtEtaPhiE(centralElectrons.at(0).pt(), centralElectrons.at(0).eta(), centralElectrons.at(0).phi(), centralElectrons.at(0).et() * cosh(centralElectrons.at(0).eta()) );
				treeVar_electronB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			}
			else {
				treeVar_electronA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
				treeVar_electronB_p4_.SetPtEtaPhiE(0, 0, 0, 0);
			}

			// Muons
			if (tightMuons.size() > 1){
				treeVar_muonA_p4_.SetPtEtaPhiM(tightMuons.at(0).pt(), tightMuons.at(0).eta(), tightMuons.at(0).phi(), muonMass );
				treeVar_muonB_p4_.SetPtEtaPhiM(tightMuons.at(1).pt(), tightMuons.at(1).eta(), tightMuons.at(1).phi(), muonMass );
			}
			else if (tightMuons.size() == 1){
				treeVar_muonA_p4_.SetPtEtaPhiM(tightMuons.at(0).pt(), tightMuons.at(0).eta(), tightMuons.at(0).phi(), muonMass );
				treeVar_muonB_p4_.SetPtEtaPhiM(0, 0, 0, muonMass);
			}
			else {
				treeVar_muonA_p4_.SetPtEtaPhiM(0, 0, 0, muonMass);
				treeVar_muonB_p4_.SetPtEtaPhiM(0, 0, 0, muonMass);
			}	

			if (nrFatJets > 0)
			{
				const double fatJetA_softDropMassPuppi_ALL = fatJetA.PUPPIsoftdrop_mass() > 0.0 ? fatJetA.PUPPIsoftdrop_mass() : 0.0;
				treeVar_fatJetA_softDropMassPuppi_ = fatJetA_softDropMassPuppi_ALL;
				treeVar_fatJetA_softDropMassPuppi_jmsUncUp_ = fatJetA_softDropMassPuppi_ALL;
				treeVar_fatJetA_softDropMassPuppi_jmsUncDown_ = fatJetA_softDropMassPuppi_ALL;
				treeVar_fatJetA_softDropMassPuppi_jmrUncUp_ = fatJetA_softDropMassPuppi_ALL;
				treeVar_fatJetA_softDropMassPuppi_jmrUncDown_ = fatJetA_softDropMassPuppi_ALL;

				const double fatJetB_softDropMassPuppi_ALL = fatJetB.PUPPIsoftdrop_mass() > 0.0 ? fatJetB.PUPPIsoftdrop_mass() : 0.0;
				treeVar_fatJetB_softDropMassPuppi_ = fatJetB_softDropMassPuppi_ALL;
				treeVar_fatJetB_softDropMassPuppi_jmsUncUp_ = fatJetB_softDropMassPuppi_ALL;
				treeVar_fatJetB_softDropMassPuppi_jmsUncDown_ = fatJetB_softDropMassPuppi_ALL;
				treeVar_fatJetB_softDropMassPuppi_jmrUncUp_ = fatJetB_softDropMassPuppi_ALL;
				treeVar_fatJetB_softDropMassPuppi_jmrUncDown_ = fatJetB_softDropMassPuppi_ALL;
			}

		} // closes 'else' sampleType is DATA

		// And finally fill the tree ...
		mainAnaTree_->Fill();
	}
};

}



//Functor for ordering leptons by pt
struct compareElectronPt{
	bool operator()(std::vector<ran::NtElectron>::const_iterator lhs, std::vector<ran::NtElectron>::const_iterator rhs){
		return (lhs->pt() > rhs->pt()); //Sort highest pt first
	}
};

struct compareMuonPt{
	bool operator()(const ran::NtMuon* lhs, const ran::NtMuon* rhs){
		return (lhs->pt() > rhs->pt()); //Sort highest pt first
	}
};

template <typename T>
struct compareParticlePt{
	bool operator()(const T* lhs, const T* rhs){
		return (lhs->pt() > rhs->pt()); //Sort highest pt first
	}
};

inline bool does_file_exist (const std::string& name) {
	struct stat buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

std::string getOutputDirFromOutputFile(std::string outputFile)
{
    std::string outputDirectory = "DID_NOT_PROVIDE_OUTPUT_DIR";
    
    // strip the output directory from the outputfile path
    std::string forwardSlash = "/";
    for (size_t c = outputFile.size()-1; c > 0; --c){
        if (outputFile[c] == forwardSlash[0]){
            outputDirectory = outputFile.substr(0, c+1);
            break;
        }
    }
    return outputDirectory;
}

double relPFIsoR03 (ran::NtMuon muon, float rho, int year) {
	double CHIso = TMath::Max(0., muon.pfIsoR03_sumChgHadPt() - rho*RhoEACorr::getEA_CH(muon.eta(), year));
	double NHIso = TMath::Max(0., muon.pfIsoR03_sumNeutHadPt() - rho*RhoEACorr::getEA_NH(muon.eta(), year));
	double PhotonIso = TMath::Max(0., muon.pfIsoR03_sumPhtEt() - rho*RhoEACorr::getEA_PH(muon.eta(), year));
	double relIso = (CHIso + std::max(0., NHIso + PhotonIso - 0.5*muon.pfIsoR03_sumPUPt()))/(muon.pt());

	return relIso;
}

double relPFIsoR04 (ran::NtMuon muon, float rho, int year) {
	double CHIso = TMath::Max(0., muon.pfIsoR04_sumChgHadPt() - rho*RhoEACorr::getEA_CH(muon.eta(), year));
	double NHIso = TMath::Max(0., muon.pfIsoR04_sumNeutHadPt() - rho*RhoEACorr::getEA_NH(muon.eta(), year));
	double PhotonIso = TMath::Max(0., muon.pfIsoR04_sumPhtEt() - rho*RhoEACorr::getEA_PH(muon.eta(), year));
	double relIso = (CHIso + std::max(0., NHIso + PhotonIso - 0.5*muon.pfIsoR04_sumPUPt()))/(muon.pt());

	return relIso;
}

double electronPFIsolation(std::vector<ran::NtTrack> pfcands,
                        const ran::NtElectron ptcl,
                        double r_iso_min, double r_iso_max, double kt_scale,
                        bool charged_only, float rho, int year) {
	
	if (ptcl.pt() < 5) return 9999.;

	double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
    if (fabs(ptcl.eta())>1.479) {deadcone_ch = 0.015; deadcone_pu = 0.015; deadcone_ph = 0.08;}

    double iso_nh(0.); double iso_ch(0.); 
    double iso_ph(0.); double iso_pu(0.);
    double ptThresh(0.);
    double r_iso = TMath::Max(r_iso_min,TMath::Min(r_iso_max, kt_scale/ptcl.pt()));
    for (unsigned int iTrk = 0; iTrk < pfcands.size(); iTrk++) {
	  ran::NtTrack pfc = pfcands[iTrk];
      if (abs(pfc.pdgId())<7) continue;

      double dr = deltaR(pfc, ptcl);
      if (dr > r_iso) continue;
      
      //////////////////  NEUTRALS  /////////////////////////
      if (pfc.charge()==0){
        if (pfc.pt()>ptThresh) {
          /////////// PHOTONS ////////////
          if (abs(pfc.pdgId())==22) {
            if(dr < deadcone_ph) continue;
            iso_ph += pfc.pt();
	    /////////// NEUTRAL HADRONS ////////////
          } else if (abs(pfc.pdgId())==130) {
            if(dr < deadcone_nh) continue;
            iso_nh += pfc.pt();
          }
        }
        //////////////////  CHARGED from PV  /////////////////////////
      } else if (pfc.fromPV()>1){
        if (abs(pfc.pdgId())==211) {
          if(dr < deadcone_ch) continue;
          iso_ch += pfc.pt();
        }
        //////////////////  CHARGED from PU  /////////////////////////
      } else {
        if (pfc.pt()>ptThresh){
          if(dr < deadcone_pu) continue;
          iso_pu += pfc.pt();
        }
      }
    }
	iso_ch = TMath::Max(0., iso_ch - rho*RhoEACorr::getEA_CH(ptcl.eta(), year));
	iso_nh = TMath::Max(0., iso_nh - rho*RhoEACorr::getEA_NH(ptcl.eta(), year));
	iso_ph = TMath::Max(0., iso_ph - rho*RhoEACorr::getEA_PH(ptcl.eta(), year));
    double iso(0.);
    if (charged_only){
      iso = iso_ch;
    } else {
      iso = iso_ph + iso_nh;
      iso -= 0.5*iso_pu;
      if (iso>0) iso += iso_ch;
      else iso = iso_ch;
    }
    iso = iso/ptcl.pt();

    return iso;
}

double muonPFIsolation(std::vector<ran::NtTrack> pfcands,
                        const ran::NtMuon ptcl,  
                        double r_iso_min, double r_iso_max, double kt_scale,
                        bool charged_only) {
	
	if (ptcl.pt() < 5) return 9999.;

	double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
    deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;  

    double iso_nh(0.); double iso_ch(0.); 
    double iso_ph(0.); double iso_pu(0.);
    double ptThresh(0.5);
    double r_iso = TMath::Max(r_iso_min,TMath::Min(r_iso_max, kt_scale/ptcl.pt()));
    for (unsigned int iTrk = 0; iTrk < pfcands.size(); iTrk++) {
	  ran::NtTrack pfc = pfcands[iTrk];
      if (abs(pfc.pdgId())<7) continue;

      double dr = deltaR(pfc, ptcl);
      if (dr > r_iso) continue;
      
      //////////////////  NEUTRALS  /////////////////////////
      if (pfc.charge()==0){
        if (pfc.pt()>ptThresh) {
          /////////// PHOTONS ////////////
          if (abs(pfc.pdgId())==22) {
            if(dr < deadcone_ph) continue;
            iso_ph += pfc.pt();
	    /////////// NEUTRAL HADRONS ////////////
          } else if (abs(pfc.pdgId())==130) {
            if(dr < deadcone_nh) continue;
            iso_nh += pfc.pt();
          }
        }
        //////////////////  CHARGED from PV  /////////////////////////
      } else if (pfc.fromPV()>1){
        if (abs(pfc.pdgId())==211) {
          if(dr < deadcone_ch) continue;
          iso_ch += pfc.pt();
        }
        //////////////////  CHARGED from PU  /////////////////////////
      } else {
        if (pfc.pt()>ptThresh){
          if(dr < deadcone_pu) continue;
          iso_pu += pfc.pt();
        }
      }
    }
    double iso(0.);
    if (charged_only){
      iso = iso_ch;
    } else {
      iso = iso_ph + iso_nh;
      iso -= 0.5*iso_pu;
      if (iso>0) iso += iso_ch;
      else iso = iso_ch;
    }
    iso = iso/ptcl.pt();

    return iso;
}

double photonPFIsolation(std::vector<ran::NtTrack> pfcands,
                        const ran::NtPhoton ptcl,  
                        bool charged_only, float rho, int year) {
	
	if (ptcl.pt() < 5) return 9999.;

	double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
    deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;

    double iso_nh(0.); double iso_ch(0.); 
    double iso_ph(0.); double iso_pu(0.);
    double ptThresh(0.5);
    double r_iso = 0.3;
    for (unsigned int iTrk(0); iTrk < pfcands.size(); iTrk++) {
	  ran::NtTrack pfc = pfcands[iTrk];
      if (abs(pfc.pdgId())<7) continue;

      double dr = deltaR(pfc, ptcl);
      if (dr > r_iso) continue;
      
      //////////////////  NEUTRALS  /////////////////////////
      if (pfc.charge()==0){
        if (pfc.pt()>ptThresh) {
          /////////// PHOTONS ////////////
          if (abs(pfc.pdgId())==22) {
            if(dr < deadcone_ph) continue;
            iso_ph += pfc.pt();
	    /////////// NEUTRAL HADRONS ////////////
          } else if (abs(pfc.pdgId())==130) {
            if(dr < deadcone_nh) continue;
            iso_nh += pfc.pt();
          }
        }
        //////////////////  CHARGED from PV  /////////////////////////
      } else if (pfc.fromPV()>1){
        if (abs(pfc.pdgId())==211) {
          if(dr < deadcone_ch) continue;
          iso_ch += pfc.pt();
        }
        //////////////////  CHARGED from PU  /////////////////////////
      } else {
        if (pfc.pt()>ptThresh){
          if(dr < deadcone_pu) continue;
          iso_pu += pfc.pt();
        }
      }
    }
	iso_ch = TMath::Max(0., iso_ch - rho*RhoEACorr::getEA_CH(ptcl.eta(), year));
	iso_nh = TMath::Max(0., iso_nh - rho*RhoEACorr::getEA_NH(ptcl.eta(), year));
	iso_ph = TMath::Max(0., iso_ph - rho*RhoEACorr::getEA_PH(ptcl.eta(), year));
    double iso(0.);
    if (charged_only){
      iso = iso_ch;
    } else {
      iso = iso_ph + iso_nh;
      iso -= 0.5*iso_pu;
      if (iso>0) iso += iso_ch;
      else iso = iso_ch;
    }
    iso = iso/ptcl.pt();

    return iso;
}

double trackPFIsolation(std::vector<ran::NtTrack> pfcands,
                        const ran::NtTrack ptcl,  
                        bool charged_only) {
	
	if (ptcl.pt() < 5) return 9999.;

	double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
    deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;

    double iso_nh(0.); double iso_ch(0.); 
    double iso_ph(0.); double iso_pu(0.);
    double ptThresh(0.5);
    double r_iso = 0.3;
    for (unsigned int iTrk = 0; iTrk < pfcands.size(); iTrk++) {
	  ran::NtTrack pfc = pfcands[iTrk];
      if (abs(pfc.pdgId())<7) continue;

      double dr = deltaR(pfc, ptcl);
      if (dr > r_iso) continue;
      
      //////////////////  NEUTRALS  /////////////////////////
      if (pfc.charge()==0){
        if (pfc.pt()>ptThresh) {
          /////////// PHOTONS ////////////
          if (abs(pfc.pdgId())==22) {
            if(dr < deadcone_ph) continue;
            iso_ph += pfc.pt();
	    /////////// NEUTRAL HADRONS ////////////
          } else if (abs(pfc.pdgId())==130) {
            if(dr < deadcone_nh) continue;
            iso_nh += pfc.pt();
          }
        }
        //////////////////  CHARGED from PV  /////////////////////////
      } else if (pfc.fromPV()>1){
        if (abs(pfc.pdgId())==211) {
          if(dr < deadcone_ch) continue;
          iso_ch += pfc.pt();
        }
        //////////////////  CHARGED from PU  /////////////////////////
      } else {
        if (pfc.pt()>ptThresh){
          if(dr < deadcone_pu) continue;
          iso_pu += pfc.pt();
        }
      }
    }
    double iso(0.);
    if (charged_only){
      iso = iso_ch;
    } else {
      iso = iso_ph + iso_nh;
      iso -= 0.5*iso_pu;
      if (iso>0) iso += iso_ch;
      else iso = iso_ch;
    }
    iso = iso/ptcl.pt();

    return iso;
}



std::vector<ran::NtMuon> looseMuons (std::vector<ran::NtMuon> muons, float rho, int year) {
	std::vector<ran::NtMuon> looseMu;
	for(unsigned int i=0; i<muons.size(); i++)
	{
		if(muons[i].isLooseMuon()  && relPFIsoR04(muons[i], rho, year) < 0.25)
		{
			looseMu.push_back(muons[i]);
		}
	}
	return looseMu;
}

std::vector<ran::NtMuon> tightMuons (std::vector<ran::NtMuon> muons, float rho, int year) {
	std::vector<ran::NtMuon> tightMu;
	for(unsigned int i=0; i<muons.size(); i++)
	{
		if(muons[i].isTightMuon() && relPFIsoR03(muons[i], rho, year) < 0.15)
		{
			tightMu.push_back(muons[i]);
		}
	}
	return tightMu;
}

bool jetIDLoose (ran::NtJet jet) {
	// looseJetID = (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((abs(eta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || abs(eta)>2.4) && abs(eta)<=2.7
	bool jetIsLoose = false;
	if (abs(jet.eta()) < 2.7)
	{
		jetIsLoose = ( (jet.neutralHadronEnergyFraction() < 0.99 && jet.neutralEmEnergyFraction() < 0.99 && jet.numberOfDaughters() > 1)
					  && ((abs(jet.eta()) < 2.4 && jet.chargedHadronEnergyFraction() > 0 && jet.chargedMultiplicity() > 0 && jet.chargedEmEnergyFraction() < 0.99) || abs(jet.eta()) > 2.4) );
	}
	else if (abs(jet.eta()) < 3.0)
	{
		jetIsLoose = ( jet.neutralEmEnergyFraction() > 0.01 && jet.neutralHadronEnergyFraction() < 0.98 && jet.neutralMultiplicity() > 2 );
	}
	else
	{
		jetIsLoose = ( jet.neutralEmEnergyFraction() < 0.9 && jet.neutralMultiplicity() > 10);
	}
	return jetIsLoose;
}

int main(int argc, char** argv){

    ///////////////////////////////////////////////////
	// Set Run Parameters
    unsigned int outputEvery = 20000;
    ///////////////////////////////////////////////////

    // Initial setup from command line inputs /////////
    if (argc != 6){
        std::cout << "Provided wrong number of arguments, the format should be:" << std::endl;
        std::cout << argv[0] << " <outputRootFilename> <pathToListOfInputFiles> <sampleType> <yearOfRun> <runInstructionString>" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    char* outputRootFile = argv[1];

    // Determine whether we are running locally or on batch system
    bool batchMode;
    std::string runInstructions(argv[5]);
    if (runInstructions == "local") batchMode = false;
    else if (runInstructions == "batch") batchMode = true;
    else {
        std::cout << "You have not provided a correct input for the runInstructions" << std::endl;
        std::cout << "Your options are 'local' or 'batch'" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    // Check that the .list file exists
    std::string listFilename(argv[2]);
    if (!does_file_exist(listFilename)){
        std::cout << "File provided containing list of input files,  " + listFilename + "  does not exist" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    // Check you have provided a valid sample type - used for different scale factors
    std::string sampleType(argv[3]);
    if (sampleType != "DATA" && sampleType != "SIGNAL" && sampleType != "TTJETS" && sampleType != "OTHER_MC"){
        std::cout << "SampleType provided is not valid" << std::endl;
        std::cout << "Use either DATA, SIGNAL, TTJETS, OTHER_MC" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    // Check you have provided a valid yearOfRun - used for different scale factors
    unsigned int yearOfRun;
    std::string yearOfRunString(argv[4]);
    if (yearOfRunString == "2016") yearOfRun = 2016;
    else if (yearOfRunString == "2017") yearOfRun = 2017;
    else{
        std::cout << "yearOfRun provided is not valid" << std::endl;
        std::cout << "Use either 2016 or 2017" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }

    if (batchMode == false){
        // Check the directory of the outputRootFile does not yet exist
        // Then make this directory
        // Then copy the code used into the same directory (this could be out of sync if you edit after compilation)
        // Then copy the list of input files used into the same directory
        std::string outputDirectory = getOutputDirFromOutputFile(outputRootFile);
        if (outputDirectory == "DID_NOT_PROVIDE_OUTPUT_DIR"){
			std::cout << "Did not provide a new directory for the output file" << std::endl;
            std::cout << "Exiting..." << std::endl;
            return -1;        	
        }
        bool makeDir = !(std::system(Form("mkdir %s",outputDirectory.c_str())));
        if (makeDir == false){
            std::cout << "The chosen output directory cannot be created, it may already exist or not have a parent" << std::endl;
            std::cout << "Exiting..." << std::endl;
            return -1;
        }
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/main/mainNMSSM.cc %s",outputDirectory.c_str()));
        std::system(Form("cp %s %s",listFilename.c_str(),outputDirectory.c_str()));
    }

    // find number of files that we are running over
	ifstream totalFileCount(argv[2]);
	unsigned int numberOfFiles = 0;
	std::string line;
	while (getline(totalFileCount, line)) numberOfFiles++;

	// Setup for main analysis - read list of input files 
	tsw::FatDoubleBJetPairTree doubleBFatJetPairTree("doubleBFatJetPairTree", outputRootFile);
	std::cout << "Reading file list: " << argv[2] << "\n";
	ifstream inputFiles(argv[2]);
	size_t evtIdx = 0;
	std::string inputFilePath;
	unsigned int fileCount = 0;
	while (getline(inputFiles, inputFilePath)){

		fileCount++;
		// Set up TTreeReader for this input file
		std::cout << std::endl << " *** NEW INPUT FILE: " << inputFilePath;
		TFile* inputFile = TFile::Open(inputFilePath.c_str());
		std::cout << "   [file has been loaded]" << std::endl;
		TTreeReader treeReader("demo/EventDataTree", inputFile);

		TTreeReaderValue<ran::EventInfo> evtInfo(treeReader, "evtInfo");
		// if (evtInfo.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for event info branch (status = " << evtInfo.GetSetupStatus() << ")" << std::endl;
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::ElectronStruct>> eleBranchValue(treeReader, "electronCollection");
		// if (eleBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for electronCollection branch (status = " << eleBranchValue.GetSetupStatus() << ")" << std::endl;
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::MuonStruct>> muonBranchValue(treeReader, "muonCollection");
		// if (muonBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for muonCollection branch (status = " << muonBranchValue.GetSetupStatus() << ")" << std::endl; 
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::PhotonStruct>> photonBranchValue(treeReader, "photonCollection");
		// if (muonBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for muonCollection branch (status = " << muonBranchValue.GetSetupStatus() << ")" << std::endl; 
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::TrackStruct>> trackBranchValue(treeReader, "trackCollection");
		// if (muonBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for muonCollection branch (status = " << muonBranchValue.GetSetupStatus() << ")" << std::endl; 
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::FatJetStruct>> fatJetBranchValue(treeReader, "fatjetCollection");
		// if (fatJetBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for fat jet branch (status = " << fatJetBranchValue.GetSetupStatus() << ")" << std::endl; 
			// return 1;
		// }
		TTreeReaderValue<std::vector<ran::JetStruct>> jetBranchValue(treeReader, "jetCollection");
		// if (jetBranchValue.GetSetupStatus() < 0) {
			// std::cout << "ERROR setting up reader for jet branch (status = " << jetBranchValue.GetSetupStatus() << ")" << std::endl;
			// return 1;
		// }

		TTreeReaderValue<std::vector<unsigned int>> recordedTriggerValue(treeReader, "recordedTriggers");

		TTreeReaderValue<int> nPU_tree(treeReader, "nPU");
		TTreeReaderValue<int> nISR_tree(treeReader, "nISR");
		TTreeReaderValue<int> nHiggs2bb_tree(treeReader, "nHiggs2bb"); // HACK: need to comment out this line if working on DATA or QCD (the ntuples are missing nGluino info)
		TTreeReaderValue<float> DelRHiggs2bb_1_tree(treeReader, "Higgs2bbDelR1"); // HACK: need to comment out this line if working on DATA or QCD (the ntuples are missing nGluino info)
		TTreeReaderValue<float> DelRHiggs2bb_2_tree(treeReader, "Higgs2bbDelR2"); // HACK: need to comment out this line if working on DATA or QCD (the ntuples are missing nGluino info)
		TTreeReaderValue<int> nGluino_tree(treeReader, "nGluino"); // HACK: need to comment out this line if working on DATA or QCD (the ntuples are missing nGluino info)
		TTreeReaderValue<float> rhoMiniIso_tree(treeReader, "RhoMiniIso");

		// Get the 'D' factor for ISR - NOTE THAT THIS IS PER INPUT FILE, NOT THE FULL SAMPLE !
		double D_factor = 0.0;
		if (sampleType == "SIGNAL"){
			TTree * T = (TTree*)inputFile->Get("demo/EventDataTree");
			TH1D * h_nISR = new TH1D("h_nISR", "", 6, 0, 6);
			T->Draw("nISR>>h_nISR");
			double D_numerator = h_nISR->GetEntries();
			double D_denominator = 0.0;
			for (unsigned int i = 0; i < 7; ++i){
				unsigned int iBin = i + 1;
				D_denominator += IsrWeights::GetWeight(i, yearOfRun) * h_nISR->GetBinContent(iBin);
			}
			D_factor = D_numerator / D_denominator;
			std::cout << "D_factor: " << D_factor << std::endl; 
		}
	

		// Loop over the events
		while (treeReader.Next()) {

			const std::vector<ran::NtElectron> electronVec(eleBranchValue->begin(), eleBranchValue->end());
			const std::vector<ran::NtMuon> muonVec(muonBranchValue->begin(), muonBranchValue->end());
			const std::vector<ran::NtPhoton> photonVec(photonBranchValue->begin(), photonBranchValue->end());
			const std::vector<ran::NtTrack> trackVec(trackBranchValue->begin(), trackBranchValue->end());
			const std::vector<ran::NtJet> jetVec(jetBranchValue->begin(), jetBranchValue->end());
			std::vector<ran::NtFatJet> fatJetVec(fatJetBranchValue->begin(), fatJetBranchValue->end());
			// std::sort(fatJetVec.begin(), fatJetVec.end(), [](const ran::NtFatJet& a, const ran::NtFatJet& b) {return b.pt() < a.pt();} );
			std::sort(fatJetVec.begin(), fatJetVec.end(), [](const ran::NtFatJet& a, const ran::NtFatJet& b) {return b.pfBoostedDoubleSecondaryVertexAK8BJetTags() < a.pfBoostedDoubleSecondaryVertexAK8BJetTags();} );

			const std::vector<unsigned int> recordedTriggerVec(recordedTriggerValue->begin(), recordedTriggerValue->end());
			bool doesEventPassTrigger = false;
			for (const unsigned int& recordedTrigger : recordedTriggerVec){
				if (recordedTrigger != 0){
					doesEventPassTrigger = true;
					break;
				}
			}

			const int nPU = *nPU_tree;
			const int nISR = *nISR_tree;
			//const int nGluino = 0; // HACK: use this option if working on DATA or QCD (the ntuples are missing nGluino info)
			//const int nHiggs2bb = 0; // HACK: use this option if working on DATA or QCD (the ntuples are missing nGluino info)
			//const float DelR_bb_Higgs1 = -1; // HACK: use this option if working on DATA or QCD (the ntuples are missing nGluino info)
			//const float DelR_bb_Higgs2 = -1; // HACK: use this option if working on DATA or QCD (the ntuples are missing nGluino info)
			const int nGluino = *nGluino_tree;
			const int nHiggs2bb = *nHiggs2bb_tree;
			const float DelR_bb_Higgs1 = *DelRHiggs2bb_1_tree;
			const float DelR_bb_Higgs2 = *DelRHiggs2bb_2_tree;
			const float rho = *rhoMiniIso_tree;

			if (sampleType != "DATA" && yearOfRun == 2017 && nPU < 2) continue; // to veto the zeroPU events in 94X simulation
			// if (nPU < 28) continue; // HACK: if you only want to use a sample of particular PU

			// Jets which pass ID constraints:
			std::vector<ran::NtJet> goodJets;
			int nPrefireJets = 0;
			for (const ran::NtJet& jet : jetVec)
			{
				if (jetIDLoose(jet))
				{
					goodJets.push_back(jet);
					if ( jet.pt() > 100 && abs(jet.eta()) > 2.25 && abs(jet.eta()) < 3.0 ) nPrefireJets++;
				}
			}

			// HT calculation: Only consider jets with |eta| < 3.0, pt > 40.0
			double ht = 0.0;
			double ht_jecUncUp = 0.0;
			double ht_jecUncDown = 0.0;
			double ht_jerUncUp = 0.0;
			double ht_jerUncDown = 0.0;
			for (const ran::NtJet& jet : jetVec) {

				if ( fabs(jet.eta()) <= 3.0 ){

					if ( jet.pt() >= 40.0 ) ht += jet.pt();
					if ( jet.pt() * ( 1.0 + jet.jecUncertainty() ) >= 40.0 ) ht_jecUncUp += jet.pt() * ( 1.0 + jet.jecUncertainty() );
					if ( jet.pt() * ( 1.0 - jet.jecUncertainty() ) >= 40.0 ) ht_jecUncDown += jet.pt() * ( 1.0 - jet.jecUncertainty() );
					if ( jet.pt() * jet.jerUncUp() >= 40.0 ) ht_jerUncUp += jet.pt() * jet.jerUncUp();
					if ( jet.pt() * jet.jerUncDown() >= 40.0 ) ht_jerUncDown += jet.pt() * jet.jerUncDown();
				}
			}

			// Missing-HT calculation: Only consider jets with |eta| < 3.0, pt > 40.0 (Same jets as for scalar HT)
			double mht_x = 0.0;
			double mht_y = 0.0;
			double mht_jecUncUp_x = 0.0;
			double mht_jecUncUp_y = 0.0;
			double mht_jecUncDown_x = 0.0;
			double mht_jecUncDown_y = 0.0;
			double mht_jerUncUp_x = 0.0;
			double mht_jerUncUp_y = 0.0;
			double mht_jerUncDown_x = 0.0;
			double mht_jerUncDown_y = 0.0;

			double mht = 0.0;
			double mht_jecUncUp = 0.0;
			double mht_jecUncDown = 0.0;
			double mht_jerUncUp = 0.0;
			double mht_jerUncDown = 0.0;

			double mht_phi = 0.0;
			double mht_phi_jecUncUp = 0.0;
			double mht_phi_jecUncDown = 0.0;
			double mht_phi_jerUncUp = 0.0;
			double mht_phi_jerUncDown = 0.0;

			for (const ran::NtJet& jet : jetVec) {

				if ( fabs(jet.eta()) <= 3.0 ){

					if ( jet.pt() >= 40.0 ) mht_x += jet.pt() * -1 * TMath::Cos(jet.phi());
					if ( jet.pt() * ( 1.0 + jet.jecUncertainty() ) >= 40.0 ) mht_jecUncUp_x += jet.pt() * -1 * TMath::Cos(jet.phi()) * ( 1.0 + jet.jecUncertainty() );
					if ( jet.pt() * ( 1.0 - jet.jecUncertainty() ) >= 40.0 ) mht_jecUncDown_x += jet.pt() * -1 * TMath::Cos(jet.phi()) * ( 1.0 - jet.jecUncertainty() );
					if ( jet.pt() * jet.jerUncUp() >= 40.0 ) mht_jerUncUp_x += jet.pt() * -1 * TMath::Cos(jet.phi()) * jet.jerUncUp();
					if ( jet.pt() * jet.jerUncDown() >= 40.0 ) mht_jerUncDown_x += jet.pt() * -1 * TMath::Cos(jet.phi()) * jet.jerUncDown();

					if ( jet.pt() >= 40.0 ) mht_y += jet.pt() * TMath::Sin(jet.phi());
					if ( jet.pt() * ( 1.0 + jet.jecUncertainty() ) >= 40.0 ) mht_jecUncUp_y += jet.pt() * TMath::Sin(jet.phi()) * ( 1.0 + jet.jecUncertainty() );
					if ( jet.pt() * ( 1.0 - jet.jecUncertainty() ) >= 40.0 ) mht_jecUncDown_y += jet.pt() * TMath::Sin(jet.phi()) * ( 1.0 - jet.jecUncertainty() );
					if ( jet.pt() * jet.jerUncUp() >= 40.0 ) mht_jerUncUp_y += jet.pt() * TMath::Sin(jet.phi()) * jet.jerUncUp();
					if ( jet.pt() * jet.jerUncDown() >= 40.0 ) mht_jerUncDown_y += jet.pt() * TMath::Sin(jet.phi()) * jet.jerUncDown();
				}
			}

			mht = TMath::Sqrt(mht_y*mht_y + mht_x*mht_x);
			//std::cout << mht << std::endl;
			mht_jecUncUp = TMath::Sqrt(mht_jecUncUp_y*mht_jecUncUp_y + mht_jecUncUp_x*mht_jecUncUp_x);
			mht_jecUncDown = TMath::Sqrt(mht_jecUncDown_y*mht_jecUncDown_y + mht_jecUncDown_x*mht_jecUncDown_x);
			mht_jerUncUp = TMath::Sqrt(mht_jerUncUp_y*mht_jerUncUp_y + mht_jerUncUp_x*mht_jerUncUp_x);
			mht_jerUncDown = TMath::Sqrt(mht_jerUncDown_y*mht_jerUncDown_y + mht_jerUncDown_x*mht_jerUncDown_x);

			mht_phi = TMath::ATan2(mht_y, mht_x);
			mht_phi_jecUncUp = TMath::ATan2(mht_jecUncUp_y, mht_jecUncUp_x);
			mht_phi_jecUncDown = TMath::ATan2(mht_jecUncDown_y, mht_jecUncDown_x);
			mht_phi_jerUncUp = TMath::ATan2(mht_jerUncUp_y, mht_jerUncUp_x);
			mht_phi_jerUncDown = TMath::ATan2(mht_jerUncDown_y, mht_jerUncDown_x);


			std::vector<ran::NtFatJet> centralFatJetVec; // get the *central* fatJets with pT > 300GeV
			for (const ran::NtFatJet& fatJet : fatJetVec) {
				if ( fabs(fatJet.eta()) < 2.4 && fatJet.PUPPIsoftdrop_mass() > -1.0 ) centralFatJetVec.push_back(fatJet);
			}


			// Do the leptons
			std::vector<ran::NtElectron> centralElectrons;
			std::vector<ran::NtElectron> isolatedElectrons;
			std::vector<ran::NtMuon> centralMuons;
			std::vector<ran::NtMuon> looseMu;
			std::vector<ran::NtPhoton> centralPhotons;
			std::vector<ran::NtPhoton> isolatedPhotons;
			std::vector<ran::NtTrack> chargedTracks;
			std::vector<ran::NtTrack> isolatedTracks;

			for (const ran::NtElectron& electron : electronVec) {
				if (fabs(electron.eta()) < 2.5) centralElectrons.push_back(electron);
			}

			for (const ran::NtMuon& muon : muonVec) {
				if (fabs(muon.eta()) < 2.5 && muon.pt() > 10.) centralMuons.push_back(muon);
			}

			for (const ran::NtPhoton& photon : photonVec) {
				if (fabs(photon.eta()) < 2.5 && photon.pt() > 25.) centralPhotons.push_back(photon);
			}

			for (const ran::NtTrack& track : trackVec) {
				if( abs(track.eta() < 2.5) && track.pt() > 10 && track.fromPV()> 1 && track.pdgId() == 211)
				{
					chargedTracks.push_back(track);
				}
			}

			// Electron ID and mini isolation
			for (unsigned int ilep(0); ilep < centralElectrons.size(); ilep++) {
				ran::NtElectron lep = centralElectrons[ilep];
				double miniso = electronPFIsolation(trackVec, lep, 0.05, 0.2, 10., false, rho, yearOfRun);
				if( miniso < 0.1 && lep.passHEEPID() )
				{
					isolatedElectrons.push_back(lep);
				}
			}

			// Loose muon ID and mini isolation
			for (unsigned int ilep(0); ilep < centralMuons.size(); ilep++) {
				ran::NtMuon lep = centralMuons[ilep];
				double miniso = muonPFIsolation(trackVec, lep, 0.05, 0.2, 10., false);
				if( miniso < 0.2 && lep.isLooseMuon() )
				{
					looseMu.push_back(lep);
				}
			}

			// Loose photon ID and rel isolation
			int nPrefirePhotons = 0;
			for (unsigned int ilep(0); ilep < centralPhotons.size(); ilep++) {
				ran::NtPhoton lep = centralPhotons[ilep];
				// 2016 photon ID criteria
				if( !lep.passElectronVeto() ) continue;
				if( lep.hadronicOverEm() > 0.05 ) continue;
				if( lep.isEB() )
				{
					if( lep.sigmaIetaIeta() > 0.0102 ) continue;
					if( lep.pfIso_chHadrIso() > 3.32 ) continue;
					if( lep.pfIso_neuHadrIso() > ( 1.92 + 0.014*lep.pt() + 0.000019*lep.pt()*lep.pt() ) ) continue;
					if( lep.pfIso_photonIso() > ( 0.81 + 0.0053*lep.pt() ) ) continue;
				}
				else if( lep.isEE() )
				{
					if( lep.sigmaIetaIeta() > 0.0274 ) continue;
					if( lep.pfIso_chHadrIso() > 1.97 ) continue;
					if( lep.pfIso_neuHadrIso() > ( 11.86 + 0.014*lep.pt() + 0.000025*lep.pt()*lep.pt() ) ) continue;
					if( lep.pfIso_photonIso() > ( 0.83 + 0.0034*lep.pt() ) ) continue;
				}
				else continue;

				double miniso = photonPFIsolation(trackVec, lep, false, rho, yearOfRun);
				if(miniso < 0.2)
				{
					isolatedPhotons.push_back(lep);
					if (lep.pt() > 50 && abs(lep.eta()) > 2.25 && abs(lep.eta()) < 3.0 ) nPrefirePhotons++;
				}
			}

			// Track isolation
			for (unsigned int ilep(0); ilep < chargedTracks.size(); ilep++) {
				ran::NtTrack lep = chargedTracks[ilep];
				double miniso = trackPFIsolation(trackVec, lep, false);
				if(miniso < 0.1)
				{
					isolatedTracks.push_back(lep);
				}
			}

			std::vector<ran::NtMuon> tightMu = tightMuons(centralMuons, rho, yearOfRun);

			// Number of fat jets
			unsigned int nFatJets = centralFatJetVec.size();

			// Number of muons
			unsigned int nLooseMuons = looseMu.size();
			unsigned int nTightMuons = tightMu.size();
			unsigned int nIsolatedPhotons = isolatedPhotons.size();
			unsigned int nIsolatedElectrons = isolatedElectrons.size();
			unsigned int nIsolatedTracks = isolatedTracks.size();

			std::vector<ran::NtJet> slimJets;
			std::vector<ran::NtJet> slimLooseBJets;
			std::vector<ran::NtJet> slimMediumBJets;
			std::vector<ran::NtJet> allSlimJets;
			std::vector<ran::NtJet> allSlimLooseBJets;
			std::vector<ran::NtJet> allSlimMediumBJets;
			std::vector<ran::NtJet> slimJetsByBtagScore;

			if (nFatJets > 1) {
			// if (centralFatJetVec.size() >= 2 && ht > 1200.0) { // HACK: INCLUDE HT CUT TO KEEP SOME DATASETS TRIM
			// if (centralFatJetVec.size() >= 2 && ht > 1499.0) { // HACK: INCLUDE HT CUT TO KEEP SOME DATASETS TRIM
				const ran::NtFatJet& fatJetA = centralFatJetVec.at(0);
				const ran::NtFatJet& fatJetB = centralFatJetVec.at(1);

				for (const ran::NtJet& jet : goodJets) {
					if (fabs(jet.eta())>2.4 || jet.pt() < 40.0)
						continue;
					allSlimJets.push_back(jet);
					if (jet.pfCombinedInclusiveSecondaryVertexV2BJetTags() > 0.8484)
                        allSlimMediumBJets.push_back(jet);
					if (jet.pfCombinedInclusiveSecondaryVertexV2BJetTags() > 0.5426)
                        allSlimLooseBJets.push_back(jet);
					if (deltaR2(jet.eta(), jet.phi(), fatJetA.eta(), fatJetA.phi()) < (1.4 * 1.4) && fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags() > 0.3)
						continue;
					else if (deltaR2(jet.eta(), jet.phi(), fatJetB.eta(), fatJetB.phi()) < (1.4 * 1.4) && fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags() > 0.3)
						continue;
					slimJetsByBtagScore.push_back(jet);
					if (jet.pfCombinedInclusiveSecondaryVertexV2BJetTags() > 0.8484)
						slimMediumBJets.push_back(jet);
					if (jet.pfCombinedInclusiveSecondaryVertexV2BJetTags() > 0.5426)
                        slimLooseBJets.push_back(jet);
					if (deltaR2(jet.eta(), jet.phi(), fatJetA.eta(), fatJetA.phi()) < (1.4 * 1.4))
						continue;
					else if (deltaR2(jet.eta(), jet.phi(), fatJetB.eta(), fatJetB.phi()) < (1.4 * 1.4))
						continue;
					slimJets.push_back(jet);
				}
				// Sort the jets by pt, but the b-jets by b-tag discriminator score
				std::sort(slimJets.begin(), slimJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pt() < a.pt();} );
				std::sort(slimJetsByBtagScore.begin(), slimJetsByBtagScore.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pfCombinedInclusiveSecondaryVertexV2BJetTags() < a.pfCombinedInclusiveSecondaryVertexV2BJetTags();} );
				std::sort(allSlimJets.begin(), allSlimJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pt() < a.pt();} );
				std::sort(slimLooseBJets.begin(), slimLooseBJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pfCombinedInclusiveSecondaryVertexV2BJetTags() < a.pfCombinedInclusiveSecondaryVertexV2BJetTags();} );
				std::sort(allSlimLooseBJets.begin(), allSlimLooseBJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pfCombinedInclusiveSecondaryVertexV2BJetTags() < a.pfCombinedInclusiveSecondaryVertexV2BJetTags();} );
				std::sort(slimMediumBJets.begin(), slimMediumBJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pfCombinedInclusiveSecondaryVertexV2BJetTags() < a.pfCombinedInclusiveSecondaryVertexV2BJetTags();} );
				std::sort(allSlimMediumBJets.begin(), allSlimMediumBJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pfCombinedInclusiveSecondaryVertexV2BJetTags() < a.pfCombinedInclusiveSecondaryVertexV2BJetTags();} );
				std::sort(centralElectrons.begin(), centralElectrons.end(), [](const ran::NtElectron& a, const ran::NtElectron& b) {return b.pt() < a.pt();} );
				std::sort(centralMuons.begin(), centralMuons.end(), [](const ran::NtMuon& a, const ran::NtMuon& b) {return b.pt() < a.pt();} );

				// Fat Jets ordered such that 1/2 events have fatJetA with highest DBT discriminator score, the other half have fatJetB with the highest DBT score
				// But it doesn't matter since there's only one AK8 jet: set both to be that jet but look out for this in the cut and count code!
				doubleBFatJetPairTree.fillTree(sampleType, *evtInfo, fatJetA, fatJetB, ht, ht_jecUncUp, ht_jecUncDown, ht_jerUncUp, ht_jerUncDown, mht, mht_jecUncUp, mht_jecUncDown, mht_jerUncUp, mht_jerUncDown, mht_phi, mht_phi_jecUncUp, mht_phi_jecUncDown, mht_phi_jerUncUp, mht_phi_jerUncDown, slimJets, allSlimJets, slimJetsByBtagScore, slimLooseBJets, allSlimLooseBJets, slimMediumBJets, allSlimMediumBJets, nFatJets, centralElectrons, tightMu, nLooseMuons, nTightMuons, nIsolatedElectrons, nIsolatedPhotons, nIsolatedTracks, doesEventPassTrigger, nPU, nISR, nGluino, nHiggs2bb, DelR_bb_Higgs1, DelR_bb_Higgs2, D_factor, yearOfRun, nPrefireJets, nPrefirePhotons);

			}

			// event counter
            if (outputEvery!=0 ? (evtIdx % outputEvery == 0) : false){
				std::cout << "  File " << fileCount << " of " << numberOfFiles;
				std::cout << ": processing event: " << evtIdx << std::endl;
			}
			evtIdx++;
		}

		std::cout << "Out of the event loop for file '" << inputFilePath << "'" << std::endl;
	}

	std::cout << "Finished reading all input files now; processed " << evtIdx << " events in total" << std::endl;

	doubleBFatJetPairTree.setEventCounter(evtIdx);
	doubleBFatJetPairTree.saveToFile();

	return 0;
}
