
//STL headers
#include <iostream>
#include <vector>
#include <fstream>

// Unix headers
#include <sys/stat.h>

//ROOT HEADERS
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h> 
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

//RAL PARTICLE HEADERS
#include "NtpReader.hh"
#include "GoodLumiChecker.hh"
#include "TriggerPathToIndex.hh"
#include "deltaR.h"


using std::cout;
using std::ifstream;
using std::ofstream;


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
		totNumEvtsAnalyzed_ = nEvtsTotal;  numEvtsPass_ = mainAnaTree_->GetEntries(); }
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
	Double_t treeVar_weight_;
	Double_t treeVar_genWeight_;
	Double_t treeVar_puWeight_;
//	Double_t treeVar_xsecWeight_;

	UInt_t treeVar_runNum_;
	UInt_t treeVar_lumiSec_;
	UInt_t treeVar_evtNum_;

	Bool_t treeVar_trgDecision_;
	UInt_t treeVar_nVtx_;

	TLorentzVector* treeVar_fatJetA_p4Ptr_; TLorentzVector treeVar_fatJetA_p4_;
	TLorentzVector* treeVar_fatJetB_p4Ptr_; TLorentzVector treeVar_fatJetB_p4_;

	Double_t treeVar_fatJetA_doubleBtagDiscrim_;
	Double_t treeVar_fatJetA_mass_;
	Double_t treeVar_fatJetA_softDropMass_;
	Double_t treeVar_fatJetA_prunedMass_;
	Float_t treeVar_fatJetA_electronEnergyFraction_;
	Float_t treeVar_fatJetA_muonEnergyFraction_;
	Float_t treeVar_fatJetA_nSubjettinessTau1_;
	Float_t treeVar_fatJetA_nSubjettinessTau2_;
	Double_t treeVar_fatJetB_doubleBtagDiscrim_;
	Double_t treeVar_fatJetB_mass_;
	Double_t treeVar_fatJetB_softDropMass_;
	Double_t treeVar_fatJetB_prunedMass_;
	Float_t treeVar_fatJetB_electronEnergyFraction_;
	Float_t treeVar_fatJetB_muonEnergyFraction_;
	Float_t treeVar_fatJetB_nSubjettinessTau1_;
	Float_t treeVar_fatJetB_nSubjettinessTau2_;

	Float_t treeVar_ht_;

	UInt_t treeVar_nrSlimJets_;
	TLorentzVector* treeVar_jetA_p4Ptr_; TLorentzVector treeVar_jetA_p4_;
	TLorentzVector* treeVar_jetB_p4Ptr_; TLorentzVector treeVar_jetB_p4_;


public:
	FatDoubleBJetPairTree(const std::string& treeName, const std::string& fileName) :
		TreeHandlerBase(treeName, "Tree of double-b fat jet candidates ("+treeName+")", fileName),
		treeVar_fatJetA_p4Ptr_( &treeVar_fatJetA_p4_ ),
		treeVar_fatJetB_p4Ptr_( &treeVar_fatJetB_p4_ ),
		treeVar_jetA_p4Ptr_( &treeVar_jetA_p4_ ),
		treeVar_jetB_p4Ptr_( &treeVar_jetB_p4_ )
	{
		// Setting up branches for event-level info ...
		mainAnaTree_->Branch("weight",     &treeVar_weight_,     "weight/D");
		mainAnaTree_->Branch("genWeight",  &treeVar_genWeight_,  "genWeight/D");
		mainAnaTree_->Branch("puWeight",   &treeVar_puWeight_,   "puWeight/D");
//		mainAnaTree_->Branch("xsecWeight", &treeVar_xsecWeight_, "xsecWeight/D");

		mainAnaTree_->Branch("run",    &treeVar_runNum_,   "run/i");
		mainAnaTree_->Branch("lumi",   &treeVar_lumiSec_,   "lumi/i");
		mainAnaTree_->Branch("evtNum", &treeVar_evtNum_,   "evtNum/i");

		mainAnaTree_->Branch("trgDecision", &treeVar_trgDecision_, "trgDecision/O");
		mainAnaTree_->Branch("nVtx", &treeVar_nVtx_, "nVtx/i");

        // Setting up fat jet branches
		mainAnaTree_->Branch("fatJetA_p4", &treeVar_fatJetA_p4Ptr_);
		mainAnaTree_->Branch("fatJetB_p4", &treeVar_fatJetB_p4Ptr_);

		mainAnaTree_->Branch("fatJetA_doubleBtagDiscrim", &treeVar_fatJetA_doubleBtagDiscrim_,   "fatJetA_doubleBtagDiscrim/D");
		mainAnaTree_->Branch("fatJetB_doubleBtagDiscrim", &treeVar_fatJetB_doubleBtagDiscrim_,   "fatJetB_doubleBtagDiscrim/D");

		mainAnaTree_->Branch("fatJetA_mass", &treeVar_fatJetA_mass_,   "fatJetA_mass/D");
		mainAnaTree_->Branch("fatJetB_mass", &treeVar_fatJetB_mass_,   "fatJetB_mass/D");

		mainAnaTree_->Branch("fatJetA_prunedMass", &treeVar_fatJetA_prunedMass_,   "fatJetA_prunedMass/D");
		mainAnaTree_->Branch("fatJetB_prunedMass", &treeVar_fatJetB_prunedMass_,   "fatJetB_prunedMass/D");

		mainAnaTree_->Branch("fatJetA_softDropMass", &treeVar_fatJetA_softDropMass_,   "fatJetA_softDropMass/D");
		mainAnaTree_->Branch("fatJetB_softDropMass", &treeVar_fatJetB_softDropMass_,   "fatJetB_softDropMass/D");

		mainAnaTree_->Branch("fatJetA_electronEnergyFraction", &treeVar_fatJetA_electronEnergyFraction_, "fatJetA_electronEnergyFraction/F");
		mainAnaTree_->Branch("fatJetB_electronEnergyFraction", &treeVar_fatJetB_electronEnergyFraction_, "fatJetB_electronEnergyFraction/F");

		mainAnaTree_->Branch("fatJetA_muonEnergyFraction", &treeVar_fatJetA_muonEnergyFraction_, "fatJetA_muonEnergyFraction/F");
		mainAnaTree_->Branch("fatJetB_muonEnergyFraction", &treeVar_fatJetB_muonEnergyFraction_, "fatJetB_muonEnergyFraction/F");

		mainAnaTree_->Branch("fatJetA_nSubjettinessTau1", &treeVar_fatJetA_nSubjettinessTau1_, "fatJetA_nSubjettinessTau1/F");
		mainAnaTree_->Branch("fatJetA_nSubjettinessTau2", &treeVar_fatJetA_nSubjettinessTau2_, "fatJetA_nSubjettinessTau2/F");
		mainAnaTree_->Branch("fatJetB_nSubjettinessTau1", &treeVar_fatJetB_nSubjettinessTau1_, "fatJetB_nSubjettinessTau1/F");
		mainAnaTree_->Branch("fatJetB_nSubjettinessTau2", &treeVar_fatJetB_nSubjettinessTau2_, "fatJetB_nSubjettinessTau2/F");

		mainAnaTree_->Branch("ht", &treeVar_ht_, "ht/F");

		mainAnaTree_->Branch("nrSlimJets", &treeVar_nrSlimJets_, "nrSlimJets/i");
		mainAnaTree_->Branch("slimJetA_p4", &treeVar_jetA_p4Ptr_);
		mainAnaTree_->Branch("slimJetB_p4", &treeVar_jetB_p4Ptr_);
	}

	~FatDoubleBJetPairTree(){}

	void fillTree(const ran::EventInfo& evtInfo, const ran::NtFatJet& fatJetA, const ran::NtFatJet& fatJetB, float ht, const std::vector<ran::NtJet>& slimJets, bool trigDecision)
	{
		// FIXME : Fill in weights with actual values
        treeVar_weight_ = 1.0;
        treeVar_genWeight_ = 1.0;
        treeVar_puWeight_ = 1.0;
//        treeVar_xsecWeight_ = 1.0;


        treeVar_runNum_ = evtInfo.runNum;
        treeVar_lumiSec_ = evtInfo.lumiSec;
        treeVar_evtNum_ = evtInfo.evtNum;

		treeVar_trgDecision_ = trigDecision;
		// FIXME : Fill in nVtx with actual values
		treeVar_nVtx_  = 0;


		treeVar_fatJetA_p4_.SetPtEtaPhiE(fatJetA.pt(), fatJetA.eta(), fatJetA.phi(), fatJetA.et() * cosh(fatJetA.eta()));
		treeVar_fatJetB_p4_.SetPtEtaPhiE(fatJetB.pt(), fatJetB.eta(), fatJetB.phi(), fatJetB.et() * cosh(fatJetB.eta()));

		treeVar_fatJetA_doubleBtagDiscrim_ = fatJetA.pfBoostedDoubleSecondaryVertexAK8BJetTags();
		treeVar_fatJetB_doubleBtagDiscrim_ = fatJetB.pfBoostedDoubleSecondaryVertexAK8BJetTags();

		treeVar_fatJetA_mass_ = fatJetA.mass();
		treeVar_fatJetB_mass_ = fatJetB.mass();

		treeVar_fatJetA_prunedMass_ = fatJetA.CHSpruned_mass();
		treeVar_fatJetB_prunedMass_ = fatJetB.CHSpruned_mass();

		treeVar_fatJetA_softDropMass_ = fatJetA.CHSsoftdrop_mass();
		treeVar_fatJetB_softDropMass_ = fatJetB.CHSsoftdrop_mass();

		treeVar_fatJetA_nSubjettinessTau1_ = fatJetA.NjettinessAK8_tau1();
		treeVar_fatJetA_nSubjettinessTau2_ = fatJetA.NjettinessAK8_tau2();
		treeVar_fatJetB_nSubjettinessTau1_ = fatJetB.NjettinessAK8_tau1();
		treeVar_fatJetB_nSubjettinessTau2_ = fatJetB.NjettinessAK8_tau2();

		treeVar_fatJetA_electronEnergyFraction_ = fatJetA.electronEnergyFraction();
		treeVar_fatJetB_electronEnergyFraction_ = fatJetB.electronEnergyFraction();

		treeVar_fatJetA_muonEnergyFraction_ = fatJetA.muonEnergyFraction();
		treeVar_fatJetB_muonEnergyFraction_ = fatJetB.muonEnergyFraction();

		treeVar_ht_ = ht;

		treeVar_nrSlimJets_ = slimJets.size();
		if (slimJets.size() > 0)
			treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(0).pt(), slimJets.at(0).eta(), slimJets.at(0).phi(), slimJets.at(0).et() * cosh(slimJets.at(0).eta()) );
		else
			treeVar_jetA_p4_.SetPtEtaPhiE(0, 0, 0, 0);
		if (slimJets.size() > 1)
			treeVar_jetA_p4_.SetPtEtaPhiE(slimJets.at(1).pt(), slimJets.at(1).eta(), slimJets.at(1).phi(), slimJets.at(1).et() * cosh(slimJets.at(1).eta()) );
		else
			treeVar_jetB_p4_.SetPtEtaPhiE(0, 0, 0, 0);

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

int main(int argc, char** argv){


  //OK, parse the command line in an very elementary way`
  //read in from command line
  if ( (argc < 3) || (argc > 4) ){
    std::cout << "Need to provide at least two arguments eg.\n";
    std::cout << argv[0] << " <output.root> <list of files> <lumi json>\n";
    return -1;
  }

  const std::string outputFilePath = argv[1];


  // Does the file containing the list of input files exist?
  if (!does_file_exist(string(argv[2]))){
    std::cout << "File "+string(argv[2])+" does not exist\n";
    return -1;
  }

  bool isMC(true);
  std::string jsonFile;
  if (argc == 4){
    isMC = false;
    isMC = isMC;
    jsonFile = string(argv[3]);
    //exit if the json file does not exist
    if (!does_file_exist(jsonFile)){
      std::cout << "File "+jsonFile+" does not exist\n";
      return -1;
    }
  }

 
      
  // Setup for main analysis - read list of input files 
  tsw::FatDoubleBJetPairTree doubleBFatJetPairTree("doubleBFatJetPairTree", outputFilePath);

  std::cout << "Reading file list: " << argv[2] << "\n";
  ifstream inputFiles(argv[2]);

  size_t evtIdx = 0;
  string inputFilePath;
  while (getline(inputFiles, inputFilePath))
  {
    // Set up TTreeReader for this input file
    std::cout << std::endl << " *** NEW INPUT FILE: " << inputFilePath << std::endl;
    TFile* inputFile = TFile::Open(inputFilePath.c_str());

    TTreeReader treeReader("demo/EventDataTree", inputFile);

    TTreeReaderValue<ran::EventInfo> evtInfo(treeReader, "evtInfo");
    if (evtInfo.GetSetupStatus() < 0) {
      std::cout << "ERROR setting up reader for event info branch (status = " << evtInfo.GetSetupStatus() << ")" << std::endl;
//      return 1;
    }
    TTreeReaderValue<std::vector<ran::ElectronStruct>> eleBranchValue(treeReader, "electronCollection");
    if (eleBranchValue.GetSetupStatus() < 0) {
      std::cout << "ERROR setting up reader for electronCollection branch (status = " << eleBranchValue.GetSetupStatus() << ")" << std::endl;
//      return 1;
    }
    TTreeReaderValue<std::vector<ran::MuonStruct>> muonBranchValue(treeReader, "muonCollection");
    if (muonBranchValue.GetSetupStatus() < 0) {
      std::cout << "ERROR setting up reader for muonCollection branch (status = " << muonBranchValue.GetSetupStatus() << ")" << std::endl; 
//      return 1;
    }
    TTreeReaderValue<std::vector<ran::FatJetStruct>> fatJetBranchValue(treeReader, "fatjetCollection");
    if (fatJetBranchValue.GetSetupStatus() < 0) {
      std::cout << "ERROR setting up reader for fat jet branch (status = " << fatJetBranchValue.GetSetupStatus() << ")" << std::endl; 
//      return 1;
    }

    TTreeReaderValue<std::vector<ran::JetStruct>> jetBranchValue(treeReader, "jetCollection");
    if (jetBranchValue.GetSetupStatus() < 0) {
      std::cout << "ERROR setting up reader for jet branch (status = " << jetBranchValue.GetSetupStatus() << ")" << std::endl;
//      return 1;
    }

    // Loop over the events
    while (treeReader.Next()) {

      // std::cout << std::endl << std::endl << "Event " << evtIdx << std::endl;
      // std::cout << std::endl << "(run " << evtInfo->runNum << ", lumi sec" << evtInfo->lumiSec << ", event no " << evtInfo->evtNum << ")" << std::endl;

      const std::vector<ran::NtElectron> electronVec(eleBranchValue->begin(), eleBranchValue->end());
      const std::vector<ran::NtMuon> muonVec(muonBranchValue->begin(), muonBranchValue->end());
      const std::vector<ran::NtJet> jetVec(jetBranchValue->begin(), jetBranchValue->end());
      std::vector<ran::NtFatJet> fatJetVec(fatJetBranchValue->begin(), fatJetBranchValue->end());
      // std::sort(fatJetVec.begin(), fatJetVec.end(), [](const ran::NtFatJet& a, const ran::NtFatJet& b) {return b.pt() < a.pt();} );
      std::sort(fatJetVec.begin(), fatJetVec.end(), [](const ran::NtFatJet& a, const ran::NtFatJet& b) {return b.pfBoostedDoubleSecondaryVertexAK8BJetTags() < a.pfBoostedDoubleSecondaryVertexAK8BJetTags();} );


      // std::cout << "   " << jetVec.size() << " jets" << std::endl;
      // HT calculation: Only consider jets with eta < 3.0, pt > 40.0
      double ht = 0.0;
      for (const ran::NtJet& jet : jetVec) {
        // std::cout << "   -> pT=" << jet.pt() << "\tET=" << jet.et() << "\teta=" << jet.eta() << "\tphi=" << jet.phi() << std::endl;
        // std::cout << "      mass=" << jet.mass() << std::endl;
        if ( (jet.pt() >= 40.0) && (jet.eta() <= 3.0) )
          ht += jet.pt();
      }
      // std::cout << "   HT = " << ht << std::endl;

      // std::cout << "   " << fatJetVec.size() << " fat jets" << std::endl;
      for (const ran::NtFatJet& fatJet : fatJetVec) {
        // std::cout << "   -> pT=" << fatJet.pt() << "\tET=" << fatJet.et() << "\teta=" << fatJet.eta() << "\tphi=" << fatJet.phi() << std::endl;
        // std::cout << "      masses: " << fatJet.mass() << ", " << fatJet.CHSsoftdrop_mass() << ", " << fatJet.CHSpruned_mass() << std::endl;
        // std::cout << "      double b-tag descriminator = " << fatJet.pfBoostedDoubleSecondaryVertexAK8BJetTags() << std::endl;
      }

      if (fatJetVec.size() >= 2) {
        const ran::NtFatJet& fatJetA = fatJetVec.at(0);
        const ran::NtFatJet& fatJetB = fatJetVec.at(1);

        std::vector<ran::NtJet> slimJets;
        for (const ran::NtJet& jet : jetVec) {
          if (deltaR2(jet.eta(), jet.phi(), fatJetA.eta(), fatJetA.phi()) < (1.4 * 1.4))
          	continue;
          else if (deltaR2(jet.eta(), jet.phi(), fatJetB.eta(), fatJetB.phi()) < (1.4 * 1.4))
          	continue;
          slimJets.push_back(jet);
      	}
        std::sort(slimJets.begin(), slimJets.end(), [](const ran::NtJet& a, const ran::NtJet& b) {return b.pt() < a.pt();} );

      	doubleBFatJetPairTree.fillTree(*evtInfo, fatJetA, fatJetB, ht, slimJets, false);
      }

      evtIdx++;
    }

    std::cout << "Out of the event loop for file '" << inputFilePath << "'" << std::endl;
  }

  std::cout << "Finished reading all input files now; processed " << evtIdx << " events in total" << std::endl;

  doubleBFatJetPairTree.setEventCounter(evtIdx);
  doubleBFatJetPairTree.saveToFile();

  //return 0;
}
