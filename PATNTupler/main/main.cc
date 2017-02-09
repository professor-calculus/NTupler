//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <fstream>

#include <sys/stat.h>

//ROOT HEADERS
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h> 
#include <TH1F.h>


//RAL PARTICLE HEADERS
#include "NtpReader.hh"
#include "GoodLumiChecker.hh"
#include "TriggerPathToIndex.hh"
#include "deltaR.h"

using std::cout;
using std::ifstream;
using std::ofstream;

using ran::ElectronStruct;
using ran::MuonStruct;
using ran::JetStruct;
using ran::FatJetStruct;
using ran::MetStruct;
using ran::TriggerPathToIndex;

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
   if (argc < 3){
     std::cout << "Need to provide at least two arguments eg.\n";
     std::cout << argv[0] << " <output.root> <list of files> <lumi json>\n";
     return -1;
   }

   if (argc > 4){
     std::cout << "Need to provide at least one argument, maybe two eg.\n";
     std::cout << argv[0] << "<output.root> <list of files> <lumi json>\n";
     return -1;
   }

   //TString outRootFile(argv[1]);
   char* outRootFile = argv[1];
   //Does the files exist

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

  //need to read list of files and put contents into a vector
  string listOfFiles(argv[2]);
  std::cout << "Reading file list: " << listOfFiles << "\n";
  ifstream iFile(listOfFiles.c_str());
  string line;
  std::vector<string> vectorOfFiles;

  while (getline(iFile,line)){
    vectorOfFiles.push_back(line);
  }
 
  //Example of two ways to access the data.
  //1. Using helper class NtpReader
  //2. Standard ROOT DIY
    
      
  //2. Method 2. DIY ROOT
  TFile fout(outRootFile,"recreate");
  //TFile f("out.root","new");
  //GoodLumiChecker glc(jsonFile);//object to check if event passed certification
    
  for (std::vector<string>::const_iterator fIter = vectorOfFiles.begin();
       fIter != vectorOfFiles.end(); ++fIter){//loop over ntuple files
    unique_ptr<TFile> f = unique_ptr<TFile> (new TFile((*fIter).c_str() ));
    std::cout << "Opening file: " << *fIter << "\n";
    TTree *evtTree = (TTree*)f->Get("demo/EventDataTree"); //get the tree
    //TTree *trigTree = (TTree*)f->Get("demo/TriggerPathsTree"); //get the tree
    //We need to set up a pointer to  a vector of particle pointers (forgot what i originally meant here!)
    //using smart pointers so looks messy. Won't bother using typedefs to reduce the mess
    unique_ptr<ran::EventInfo> evtObj(new ran::EventInfo());
    unique_ptr<std::vector<ran::ElectronStruct> > electronVector(new std::vector<ElectronStruct>());
    unique_ptr<std::vector<ran::MuonStruct> > muonVector(new std::vector<MuonStruct>());
    unique_ptr<std::vector<ran::JetStruct> > jetVector(new std::vector<JetStruct>());
    unique_ptr<std::vector<ran::FatJetStruct> > fatjetVector(new std::vector<FatJetStruct>());
    unique_ptr<std::vector<ran::MetStruct> > metVector(new std::vector<MetStruct>());
    unique_ptr<std::vector<char> > triggerFlagsVector(new std::vector<char>());

    unique_ptr<std::vector<string> > triggerPathVector(new std::vector<string>());

    TBranch* evtBranch = evtTree->GetBranch("evtInfo"); //load event info collection
    evtBranch->SetAddress(&evtObj);

    TBranch* branch = evtTree->GetBranch("electronCollection"); //load electron collection
    branch->SetAddress(&electronVector);

    TBranch* muonBranch = evtTree->GetBranch("muonCollection"); //load muon collection
    muonBranch->SetAddress(&muonVector);

    TBranch* jetBranch = evtTree->GetBranch("fatjetCollection"); //load jet collection
    jetBranch->SetAddress(&fatjetVector);

    //TBranch* fatjetBranch = evtTree->GetBranch("fatjetCollection"); //load jet collection
    //fatjetBranch->SetAddress(&fatjetVector);

    //TBranch* metBranch = evtTree->GetBranch("metCollection"); //load met collection
    //metBranch->SetAddress(&metVector);
    
    //TBranch* trigFlagBranch = evtTree->GetBranch("recordedTriggers"); //load met collection
    //trigFlagBranch->SetAddress(&triggerFlagsVector);

    //TBranch* trigBranch = trigTree->GetBranch("triggerPaths"); //load trigger collection
    //trigBranch->SetAddress(&triggerPathVector);
    
    //const unsigned int trigEntryNum = trigBranch->GetEntries()-1;
    
    const unsigned int numEvents = branch->GetEntries(); //All evtTree branches *should* have the same number of entries

    std::cout << "Number of Entries is: " << numEvents << "\n";
    
    //loop over tree
    for (unsigned int i = 0; i < numEvents; ++i){

      std::cout  << "Event info: " << evtObj->runNum << "\t" << evtObj->evtNum << "\t" << evtObj->lumiSec << "\n";

      evtBranch->GetEntry(i); // set tree object for each event i
      jetBranch->GetEntry(i); // set tree object for each event i
 
      std::vector<ran::NtFatJet>  ralJetVector(fatjetVector->begin(), fatjetVector->end());
      for (const ran::NtFatJet& jet : ralJetVector){
	std::cout  << jet.pt() << "\t" << jet.et() << "\t" << jet.eta() << "\t" << jet.phi() << "\t" << jet.mass() << "\t" << jet.CHSsoftdrop_mass() << "\t" << jet.CHSpruned_mass() << "\t" << jet.pfBoostedDoubleSecondaryVertexAK8BJetTags() <<"\n";	   	   
      }//loop over jets
	     

	
    }//loop over events 
    std::cout << "Closing file: " << *fIter << "\n";
  }//loop over files

  fout.cd();
  //evtTree->Write();
  fout.Write();

  //graphicsPlease.Run();
  
  //return 0;
}
