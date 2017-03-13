
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

  char* outRootFile = argv[1];

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

      std::cout << std::endl << std::endl << "Event " << evtIdx << std::endl;
      std::cout << std::endl << "(run " << evtInfo->runNum << ", lumi sec" << evtInfo->lumiSec << ", event no " << evtInfo->evtNum << ")" << std::endl;

      const std::vector<ran::NtElectron> electronVec(eleBranchValue->begin(), eleBranchValue->end());
      const std::vector<ran::NtMuon> muonVec(muonBranchValue->begin(), muonBranchValue->end());
      const std::vector<ran::NtJet> jetVec(jetBranchValue->begin(), jetBranchValue->end());
      const std::vector<ran::NtFatJet> fatJetVec(fatJetBranchValue->begin(), fatJetBranchValue->end());

      std::cout << "   " << jetVec.size() << " jets" << std::endl;
      double ht = 0.0;
      for (const ran::NtJet& jet : jetVec) {
        std::cout << "   -> pT=" << jet.pt() << "\tET=" << jet.et() << "\teta=" << jet.eta() << "\tphi=" << jet.phi() << std::endl;
        std::cout << "      mass=" << jet.mass() << std::endl;
        if ( (jet.pt() >= 40.0) && (jet.eta() <= 3.0) )
          ht += jet.pt();
      }
      std::cout << "   HT = " << ht << std::endl;

      std::cout << "   " << fatJetVec.size() << " fat jets" << std::endl;
      for (const ran::NtFatJet& fatJet : fatJetVec) {
        std::cout << "   -> pT=" << fatJet.pt() << "\tET=" << fatJet.et() << "\teta=" << fatJet.eta() << "\tphi=" << fatJet.phi() << std::endl;
        std::cout << "      masses: " << fatJet.mass() << ", " << fatJet.CHSsoftdrop_mass() << ", " << fatJet.CHSpruned_mass() << std::endl;
        std::cout << "      double b-tag descriminator = " << fatJet.pfBoostedDoubleSecondaryVertexAK8BJetTags() << std::endl;
      }

      // HT: eta < 3.0, pt > 40.0

      evtIdx++;
    }

    std::cout << "Out of the event loop for file '" << inputFilePath << "'" << std::endl;
  }
  std::cout << "Finished reading all input files now; processed " << evtIdx << " events in total" << std::endl;

  
  //return 0;
}
