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
struct compareLeptonPt{
  bool operator()(std::vector<ran::NtElectron>::const_iterator lhs, std::vector<ran::NtElectron>::const_iterator rhs){
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
  TH1F mumuMass("mumuMass","mu mu Mass",100,0,200); 
  mumuMass.SetDirectory(&fout);
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

    TBranch* jetBranch = evtTree->GetBranch("jetCollection"); //load jet collection
    jetBranch->SetAddress(&jetVector);

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
      branch->GetEntry(i); // set tree object for each event i
      evtBranch->GetEntry(i); // set tree object for each event i
      

	//can do the same with muons and jets
        //Select Muons
	muonBranch->GetEntry(i); // set tree object eMeEvent for each event i

	//unique_ptr<std::vector<ran::NtMuon> > ralMuVector(new std::vector<ran::NtMuon>(muonVector->begin(), muonVector->end()));
        std::vector<ran::NtMuon> ralMuVector(muonVector->begin(), muonVector->end());

	std::vector<const ran::NtMuon*> selectedMuonsPos;
	std::vector<const ran::NtMuon*> selectedMuonsNeg;
	if(!ralMuVector.empty()){//if it is not empty

	  for (const ran::NtMuon& muon : ralMuVector){
	    if (muon.tuneP_exists()){
	      if (muon.tuneP_pt() > 25.0){
		if (fabs(muon.tuneP_eta()) < 2.1){
		  if (muon.tuneP_charge() > 0){
		    selectedMuonsPos.push_back(&muon);
		  } else {
		    selectedMuonsNeg.push_back(&muon);
		  }
		}//eta cut
	      }//pt cut
	    }//tune P muons
	    //std::cout << "muon size: " << ralMuVector->size() << "\n";
	    //std::cout << "muon pt: "<< (ralMuVector->at(0)).pt() << "\n";
	  }
	}	

	jetBranch->GetEntry(i); // set tree object for each event i
 
	std::vector<const ran::NtJet*> bCandJets;
	std::vector<const ran::NtJet*> forwardJets;
	std::vector<ran::NtJet>  ralJetVector(jetVector->begin(), jetVector->end());
	for (const ran::NtJet& jet : ralJetVector){
	  if (jet.pt()> 30.0){
	    if (fabs(jet.eta()) < 2.4){
	      bCandJets.push_back(&jet);
	    }
	    if ((fabs(jet.eta()) > 2.4) && (fabs(jet.eta()) < 4.7)){
	      forwardJets.push_back(&jet);
	    }
	  }
	}

	if (!forwardJets.empty()){
	  if (bCandJets.size() == 1){
	    if (bCandJets[0]->pfCombinedMVAV2BJetTags() > -0.715){
	      if (!selectedMuonsPos.empty()){
		if (!selectedMuonsNeg.empty()){
		  TLorentzVector mu1, mu2, res4v; 
		  mu1.SetPtEtaPhiM( selectedMuonsPos[0]->tuneP_pt() ,  selectedMuonsPos[0]->tuneP_eta(),  selectedMuonsPos[0]->tuneP_phi(), 0.105658);
		  mu2.SetPtEtaPhiM( selectedMuonsNeg[0]->tuneP_pt() ,  selectedMuonsNeg[0]->tuneP_eta(),  selectedMuonsNeg[0]->tuneP_phi(), 0.105658);
		  res4v = mu1+ mu2;
		  double mass = res4v.M();
		  mumuMass.Fill(mass);
		}// at least one negative muon	  
	      } // at least one postive muon
	    }//
	  }//seleted jets
	}//At least one forward jet       	
	//}// Certified event?
    }//loop over events 
    std::cout << "Closing file: " << *fIter << "\n";
  }//loop over files

  fout.cd();
  mumuMass.Write();
  fout.Write();

  //graphicsPlease.Run();
  
  //return 0;
}
