//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

//ROOT HEADERS
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h> 
#include <TH1F.h>
#include <TApplication.h>

//RAL PARTICLE HEADERS
#include "NtpReader.hh"
#include "TriggerPathToIndex.hh"

using std::cout;
using std::ifstream;
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

int main(int argc, char** argv){

   //read in from command line
   if (argc < 2){
     std::cout << "Need to provide one argument eg.\n";
     std::cout << argv[0] << " <list of files>\n";
     return -1;
   }

   if (argc > 2){
     std::cout << "Need to provide one argument eg.\n";
     std::cout << argv[0] << " <list of files>\n";
     return -1;
   }
   //NEED TO RETURN AN ERROR IF FILE DOES NOT EXIST
  //need to read list of files and put contents into a vector
  string listOfFiles(argv[1]);
  std::cout << "Reading file: " << listOfFiles << "\n";
  ifstream iFile(listOfFiles.c_str());
  string line;
  std::vector<string> vectorOfFiles;

  while (getline(iFile,line)){
    vectorOfFiles.push_back(line);
  }

  TApplication graphicsPlease("graphicsPlease", &argc, argv); //This messes with argv
 
  //Example of two ways to access the data.
  //1. Using helper class NtpReader
  //2. Standard ROOT DIY

  //1. Method 1. Using NtpReader
  ///*
  //loop over files
  /*
  for (std::vector<string>::const_iterator iter = vectorOfFiles.begin();
       iter != vectorOfFiles.end(); ++iter){//loop over ntuple files
    unique_ptr<TFile> g = unique_ptr<TFile> (new TFile((*iter).c_str() ));

    TTree *eventtTree = (TTree*)g->Get("demo/EventDataTree"); //get the tree

    NtpReader testE(eventtTree, "evtInfo"); //instantiate NtpReader which takes tree to analyse as input
    //testE.setEventInfoBranch("evtInfo");//Specify the branches you want to look at. Each branch has its own setter method. Need to move this to the constructor
    cout << "Num of Entries is: " <<  testE.getLastEntry() << "\n";

    
    for (testE.setEntryNumber(0); testE.getEntryNumber() < testE.getLastEntry(); testE.nextEntry()){//loop over events in a file
      
      cout << "Entry number is: " << testE.getEntryNumber() << "\n";
      cout <<"Run Number is: " << testE.runNum() << "\n";
      cout <<"Event Number is: " << testE.evtNum() << "\n";

      //let's test the new method

      std::vector<ran::NtElectron> elist = testE.getParticleCollection<ran::NtElectron, ran::ElectronStruct>("electronCollection");
    
      if (!elist.empty()){
	cout << "New member EventNumber: " << testE.getEntryNumber() << "\n";
	cout << "electron Pt: " << (elist.at(0)).pt() << "\n";     //print pt
	//cout << "HEEP et: " << (elist.at(0)).heep_et() << "\n";
      }
    
      std::vector<ran::NtJet> jlist = testE.getParticleCollection<ran::NtJet, ran::JetStruct>("jetCollection");
      if (!jlist.empty()){
	cout << "Jet Et: " << (jlist.at(0)).et() << "\n"; //print et
      }
    
      std::vector<ran::NtMuon> mlist = testE.getParticleCollection<ran::NtMuon, ran::MuonStruct>("muonCollection");
      if (!mlist.empty()){
	cout << "Muon pt: " << (mlist.at(0)).pt() << "\n"; //print et
      }
      
      
      //at the end of the loop
      //testE.nextEntry();//increment eventnumber in ntuple
    }//end of loop over number of events in a file
  }//end of loop over ntuple files
  //
  */

  //2. Method 2. DIY ROOT
  unique_ptr<TH1F> eeMass(new TH1F("eeMass","ee Mass",400,0,2000)); 
  std::cout << "size of vector: " << vectorOfFiles.size() << "\n";
  for (std::vector<string>::const_iterator fIter = vectorOfFiles.begin();
       fIter != vectorOfFiles.end(); ++fIter){//loop over ntuple files
    unique_ptr<TFile> f = unique_ptr<TFile> (new TFile((*fIter).c_str() ));
    std::cout << "Opening file: " << *fIter << "\n";
    TTree *evtTree = (TTree*)f->Get("demo/EventDataTree"); //get the tree
    TTree *trigTree = (TTree*)f->Get("demo/TriggerPathsTree"); //get the tree
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

    TBranch* fatjetBranch = evtTree->GetBranch("fatjetCollection"); //load jet collection
    fatjetBranch->SetAddress(&fatjetVector);

    TBranch* metBranch = evtTree->GetBranch("metCollection"); //load met collection
    metBranch->SetAddress(&metVector);

    TBranch* trigFlagBranch = evtTree->GetBranch("recordedTriggers"); //load met collection
    trigFlagBranch->SetAddress(&triggerFlagsVector);

    TBranch* trigBranch = trigTree->GetBranch("triggerPaths"); //load trigger collection
    trigBranch->SetAddress(&triggerPathVector);

    const unsigned int trigEntryNum = trigBranch->GetEntries()-1;

     //Which trigger bit corresponds to trigger path HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v
    TriggerPathToIndex tpti(*triggerPathVector); //Load in the list of trigger paths recoreded
    unsigned int trigIndex = tpti.getTrigIndex("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v");

    const unsigned int numEvents = branch->GetEntries(); //All evtTree branches *should* have the same number of entries

    std::cout << "Number of Entries is: " << numEvents << "\n";

    //loop over tree
    for (unsigned int i = 0; i < numEvents; ++i){
      branch->GetEntry(i); // set tree object for each event i
      evtBranch->GetEntry(i); // set tree object for each event i
      //Electron collection contain structs (ElectronStruct). We want to use our own electon class which is composed of the struct (NtElectron) 

      //cout << "Event Number is: " << evtObj->evtNum << "\n";
      //cout << "Run Number is: " << evtObj->runNum << "\n";
      //cout << "Lumi Sec is: " << evtObj->lumiSec << "\n";

      unique_ptr<std::vector<ran::NtElectron> > ralEVector(new std::vector<ran::NtElectron>(electronVector->begin(), electronVector->end()));
      //This function generates a vector of NtElectron from a vector of ElectronStruct
      //fillE(ralEVector.get(),electronVector.get());
      //fillParticleVector<ran::NtElectron, ran::ElectronStruct>(ralEVector.get(),electronVector.get());

      std::vector<std::vector<ran::NtElectron>::const_iterator> heepElectrons;
      for (std::vector<ran::NtElectron>::const_iterator iter = ralEVector->begin();
	   iter != ralEVector->end(); ++iter){ //look at the electrons using our electron class
	//std::cout << "rv pt: " << iter->pt() << "\n";

	if (iter->heep_cutCode() == 0){
	cout << "HEEP cut codes: " <<  iter->heep_cutCode() << "\n";
        cout << "HEEP cut et: " <<  iter->heep_et() << "\n";
	cout << "HEEP scEta: " <<  iter->heep_scEt() << "\n";
        cout << "GSF pt: " <<  iter->pt() << "\n";
        cout << "GSF eta: " <<  iter->eta() << "\n";
	cout << "GSF phi: " <<  iter->phi() << "\n";
	cout << "GSF scEta: " <<  iter->scEta() << "\n";
	}
	//Store electrons that pass HEEP cutcodes
	if ( !(iter->heep_cutCode()) ){
	  if (iter-> heep_et() >35.0){
	    
	    trigFlagBranch->GetEntry(i);
	    //std::cout << "First Trigger bit is: " << int(triggerFlagsVector->at(0)) << "\n";
	    trigBranch->GetEntry(trigEntryNum);//Entries should be the same
	    //std::cout << "First Trigger is: " << triggerPathVector->at(0) << "\n";	   
	    //if (int(triggerFlagsVector->at(trigIndex))){
	      heepElectrons.push_back(iter);
	      //}
	  }
	}     
      }

      //do we have at least two HEEP electrons    
      if (heepElectrons.size() > 1){
	//OK order them by pt. Highest first      
	//std::sort(heepElectrons.begin(), heepElectrons.end(), compareLeptonPt{}); //Think they are already sorted
	TLorentzVector ele1, ele2, res4v; 
	ele1.SetPtEtaPhiM( heepElectrons[0]->pt() ,  heepElectrons[0]-> heep_eta(),  heepElectrons[0]-> heep_phi(), 0.000511);
	ele2.SetPtEtaPhiM( heepElectrons[1]->pt() ,  heepElectrons[1]-> heep_eta(),  heepElectrons[1]-> heep_phi(), 0.000511);
	res4v = ele1+ ele2;
	double mass = res4v.M();
	eeMass->Fill(mass);
      }

      //can do the same with muons and jets
    
      muonBranch->GetEntry(i); // set tree object eMeEvent for each event i

      unique_ptr<std::vector<ran::NtMuon> > ralMuVector(new std::vector<ran::NtMuon>(muonVector->begin(), muonVector->end()));
      if(!ralMuVector->empty()){//if it is not empty
	//std::cout << "muon size: " << ralMuVector->size() << "\n";
	//std::cout << "muon pt: "<< (ralMuVector->at(0)).pt() << "\n";
      }

      jetBranch->GetEntry(i); // set tree object for each event i

      unique_ptr<std::vector<ran::NtJet> > ralJetVector(new std::vector<ran::NtJet>(jetVector->begin(), jetVector->end()));
      if(!ralJetVector->empty()){//if it is not empty
	//std::cout << "jet size: " << ralJetVector->size() << "\n";
	//std::cout << "jet pt: "<< (ralJetVector->at(0)).pt() << "\n";
      }

      fatjetBranch->GetEntry(i); // set tree object for each event i

      unique_ptr<std::vector<ran::NtFatJet> > ralfatJetVector(new std::vector<ran::NtFatJet>(fatjetVector->begin(), fatjetVector->end()));
      if(!ralfatJetVector->empty()){//if it is not empty
	//std::cout << "fat jet size: " << ralfatJetVector->size() << "\n";
	//std::cout << "fat jet pt: "<< (ralfatJetVector->at(0)).pt() << "\n";
      }

      metBranch->GetEntry(i); // set tree object for each event i

      unique_ptr<std::vector<ran::NtMet> > ralMetVector(new std::vector<ran::NtMet>(metVector->begin(), metVector->end()));
      if(!ralMetVector->empty()){//if it is not empty
	//std::cout << "met size: " << ralMetVector->size() << "\n";
	//std::cout << "met pt: "<< (ralMetVector->at(0)).pt() << "\n";
      }      
    }
  }//loop over files

  eeMass->Draw("");
  std::cout << "\n\n\nType Ctrl + C to return to the prompt\n";
  graphicsPlease.Run();
  
  //return 0;
}
