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
  TH1F mumuMass("mumuMass","mu mu Mass",100,0,200); 
  TTree *mumuTree = new TTree("evtTree","mumu data");
  double resMass{0};
  float posPt{0};
  float negPt{0};
  mumuTree->Branch("resMass",&resMass);
  mumuTree->Branch("posPt",&posPt);
  mumuTree->Branch("negPt",&negPt);
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
        std::vector<const ran::NtMuon*> selectedMuonsTwenty;
	if(!ralMuVector.empty()){//if it is not empty

	  for (const ran::NtMuon& muon : ralMuVector){
	    if (muon.tuneP_exists()){
	      if (muon.isGlobalMuon() && muon.isTrackerMuon()){
		if (muon.tuneP_pt() > 20.0){ 
		  if (muon.numMatchedMuonStns() > 1){
		    if (muon.globTrk_numberOfValidMuonHits() > 0){
		      if (muon.globTrk_numberOfValidPixelHits() > 0){
			if (muon.globTrk_normalisedChi2() < 10){
			  if (muon.inTrk_exists()){
			    if (muon.isolR03_sumPt()/muon.inTrk_pT() < 0.1){
			      if (muon.globTrk_dxy() < 0.2){
				if (muon.globTrk_dz() < 0.5){
				  if (fabs(muon.tuneP_eta()) < 2.1){
				    selectedMuonsTwenty.push_back(&muon);
				    if (muon.tuneP_pt() > 25.0){ 
				      if (muon.tuneP_charge() > 0){
					selectedMuonsPos.push_back(&muon);
				      } else {
					selectedMuonsNeg.push_back(&muon);
				      }//charge
				    }//pt > 25GeV
				  }//eta cut
				}//dz cut
			      }//impact parameter
			    }//isolation cut			    
			  }//inner track muons
			}// chi sqred trk cut
		      }//valid pixel hits
		    }//number of valid muon hits
		  }//number of matched muon stations	      
		}//pt cut > 20
	      }//global and tracker muon
	    }//tune P muons
	  }//loop ovver muons
	}//Do we have any muons. Should skip to next event if we don't

	if (selectedMuonsPos.size() > 0 && selectedMuonsNeg.size() > 0){ //need at least 2 muons
	jetBranch->GetEntry(i); // set tree object for each event i
 
	std::vector<const ran::NtJet*> bCandJets;
	std::vector<const ran::NtJet*> forwardJets;
	std::vector<ran::NtJet>  ralJetVector(jetVector->begin(), jetVector->end());
	for (const ran::NtJet& jet : ralJetVector){
	  bool goodJet{true};
	  if (jet.pt()> 30.0){
	    if (fabs(jet.eta()) < 2.4){
	      if (jet.neutralHadronEnergyFraction() < 1.0 ){ 
		if (jet.neutralEmEnergyFraction() < 1.0){
		  if (jet.chargedEmEnergyFraction() < 1.0){
		    if (jet.chargedHadronEnergyFraction() > 0.0){
		      if (jet.chargedMultiplicity() > 0){
			//check delta R for all muons
			for (const ran::NtMuon* ptTwentyMuon : selectedMuonsTwenty){
			  if (fabs(deltaR(jet.eta(),jet.phi(),ptTwentyMuon->eta(), ptTwentyMuon->globTrk_phi())) < 0.5){//need to use basic phi rather than global trk
			    goodJet = false;
			  }//delta R separation
			}//loop over muons
			if (goodJet){
			  bCandJets.push_back(&jet);
			}//Good jet
		      }//charged multiplicity
		    }//charged hadron enrgy fraction
		  }//charged energy fraction
		}//neutral em nrg fraction
	      }//nuetral hadron enrgy fraction
	    }//eta cut
	    if ((fabs(jet.eta()) > 2.4) && (fabs(jet.eta()) < 4.7)){
	      forwardJets.push_back(&jet);
	    }//eta cut
	  }//put cut
	}//loop over jets

	if (!forwardJets.empty()){
	  if (bCandJets.size() == 1){
	    if (bCandJets[0]->pfCombinedMVAV2BJetTags() > -0.5884){//loose WP 10% mistag rate
	      if (!selectedMuonsPos.empty()){// don't need this anymore
		if (!selectedMuonsNeg.empty()){//don't need this anymore
		  TLorentzVector mu1, mu2, res4v; 
		  //Sort the muons by pt, highest first! Think they are sorted by default but do it just to be safe
		  
		  std::sort(selectedMuonsPos.begin(), selectedMuonsPos.end(), compareParticlePt<ran::NtMuon>());
                  std::sort(selectedMuonsNeg.begin(), selectedMuonsNeg.end(), compareParticlePt<ran::NtMuon>() );
		 
		  mu1.SetPtEtaPhiM( selectedMuonsPos[0]->tuneP_pt() ,  selectedMuonsPos[0]->tuneP_eta(),  selectedMuonsPos[0]->tuneP_phi(), 0.105658);
		  mu2.SetPtEtaPhiM( selectedMuonsNeg[0]->tuneP_pt() ,  selectedMuonsNeg[0]->tuneP_eta(),  selectedMuonsNeg[0]->tuneP_phi(), 0.105658);
		  res4v = mu1+ mu2;
		  double mass = res4v.M();
		  resMass = mass;
		  posPt = selectedMuonsPos[0]->tuneP_pt();
		  negPt = selectedMuonsNeg[0]->tuneP_pt();
		  mumuMass.Fill(mass);
		  mumuTree->Fill();
		  resMass = resMass;
		}// at least one negative muon	  
	      } // at least one postive muon
	    }//
	  }//seleted jets
	}//At least one forward 
	}//At least two muons
    }//loop over events 
    std::cout << "Closing file: " << *fIter << "\n";
  }//loop over files

  fout.cd();
  mumuMass.Write();
  //evtTree->Write();
  fout.Write();

  //graphicsPlease.Run();
  
  //return 0;
}
