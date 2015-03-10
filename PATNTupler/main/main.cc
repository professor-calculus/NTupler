//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>

//ROOT HEADERS
#include <TFile.h>
#include <TTree.h>
#include <TString.h>


//BOOST HEADERS

#include <boost/shared_ptr.hpp>

//RAL PARTICLE HEADERS
#include "NtpReader.hh"
//#include "../interface/EventInfo.hh" //del this

using std::cout;
using boost::shared_ptr; //DROP BOOST AND USE C++11
using ran::ElectronStruct;
using ran::MuonStruct;
using ran::JetStruct;

int main(){

  //Example of two ways to access the data.
  //1. Using helper class NtpReader
  //2. Standard ROOT DIY

  //1. Method 1. Using NtpReader

  //need to read list of files and put contents into a vector
  string listOfFiles("test.list");
  ifstream iFile(listOfFiles.c_str());
  string line;
  std::vector<string> vectorOfFiles;

  while (getline(iFile,line)){
    vectorOfFiles.push_back(line);
  }

  //loop over files

  for (std::vector<string>::const_iterator iter = vectorOfFiles.begin();
       iter != vectorOfFiles.end(); ++iter){//loop over ntuple files
    shared_ptr<TFile> g = shared_ptr<TFile> (new TFile((*iter).c_str() ));

    TTree *eventtTree = (TTree*)g->Get("demo/EventDataTree"); //get the tree

    NtpReader testE(eventtTree, "evtInfo"); //instantiate NtpReader which takes tree to analyse as input
    //testE.setEventInfoBranch("evtInfo");//Specify the branches you want to look at. Each branch has its own setter method. Need to move this to the constructor
    cout << "Num of Entries is: " <<  testE.getLastEntry() << "\n";

  
    for (testE.setEntryNumber(0); testE.getEntryNumber() < testE.getLastEntry(); testE.nextEntry()){//loop over events in a file
      /*
      cout << "Entry number is: " << testE.getEntryNumber() << "\n";
      cout <<"Run Number is: " << testE.runNum() << "\n";
      cout <<"Event Number is: " << testE.evtNum() << "\n";

      //lets test the new method

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

      */
      //at the end of the loop
      //testE.nextEntry();//increment eventnumber in ntuple
    }//end of loop over number of events in a file
  }//end of loop over ntuple files
  

  //2. Method 2. DIY ROOT

  TString fileName = "../plugins/nTuple2.root"; //specify root file to run over
                                                   // will write a better way to run over a list of files
  shared_ptr<TFile> f = shared_ptr<TFile> (new TFile(fileName));

  TTree *evtTree = (TTree*)f->Get("demo/EventDataTree"); //get the tree

  //We need to set up a pointer to  a vector of particle pointers
  //using smart pointers so looks messy. Won't bother using typedefs to reduce the mess
  shared_ptr<std::vector<ran::ElectronStruct> > electronVector = shared_ptr<std::vector<ran::ElectronStruct> > (new std::vector<ElectronStruct>());
  shared_ptr<std::vector<ran::MuonStruct> > muonVector = shared_ptr<std::vector<ran::MuonStruct> > (new std::vector<MuonStruct>());
  shared_ptr<std::vector<ran::JetStruct> > jetVector = shared_ptr<std::vector<ran::JetStruct> > (new std::vector<JetStruct>());

  //std::vector<ran::ElectronStruct>* electronVector = new std::vector<ElectronStruct>();  
  //evtTree->SetBranchAddress("electronCollection",&electronVector); 

  TBranch* branch = evtTree->GetBranch("electronCollection"); //load electron collection
  branch->SetAddress(&electronVector);

  TBranch* muonBranch = evtTree->GetBranch("muonCollection"); //load muon collection
  muonBranch->SetAddress(&muonVector);

  TBranch* jetBranch = evtTree->GetBranch("jetCollection"); //load jet collection
  jetBranch->SetAddress(&jetVector);

  const unsigned int numEvents = branch->GetEntries(); //All branches have the same number of entries

   cout << "Number of Entries is: " << numEvents << "\n";

  //loop over tree
  for (unsigned int i = 0; i < numEvents; ++i){
    branch->GetEntry(i); // set tree object for each event i
    //Electron collection contain structs (ElectronStruct). We want to use our own electon class which is composed of the struct (NtElectron) 
    //    shared_ptr<std::vector<ran::NtElectron> > ralEVector = shared_ptr<std::vector<ran::NtElectron> > (new std::vector<ran::NtElectron>());
    shared_ptr<std::vector<ran::NtElectron> > ralEVector = shared_ptr<std::vector<ran::NtElectron> > (new std::vector<ran::NtElectron>(electronVector->begin(), electronVector->end()));
    //This function generates a vector of NtElectron from a vector of ElectronStruct
    //fillE(ralEVector.get(),electronVector.get());
    //fillParticleVector<ran::NtElectron, ran::ElectronStruct>(ralEVector.get(),electronVector.get());

    for (std::vector<ran::ElectronStruct>::const_iterator iter = electronVector->begin();
	 iter != electronVector->end(); ++iter){ //look at electrons using structs in ntuple
      std::cout << "ev pt: " << (*iter).pt << "\n";
    }

    for (std::vector<ran::NtElectron>::const_iterator iter = ralEVector->begin();
	 iter != ralEVector->end(); ++iter){ //look at the electrons using our electron class
      std::cout << "rv pt: " << (*iter).pt() << "\n";
    }

    //can do the same with muons and jets
    
    muonBranch->GetEntry(i); // set tree object eMeEvent for each event i

    shared_ptr<std::vector<ran::NtMuon> > ralMuVector = shared_ptr<std::vector<ran::NtMuon> > (new std::vector<ran::NtMuon>(muonVector->begin(), muonVector->end()));
    if(!ralMuVector->empty()){//if it is not empty
      std::cout << "muon size: " << ralMuVector->size() << "\n";
      std::cout << "muon pt: "<< (ralMuVector->at(0)).pt() << "\n";
    }

    /*
      if(!muonVector->empty()){//if it is not empty
      std::cout << "muon size: " << muonVector->size() << "\n";
      std::cout << "muon pt: "<< (muonVector->at(0)).pt << "\n";
    }
    jetBranch->GetEntry(i); // set tree object eMeEvent for each event i
    if(!jetVector->empty()){//if it is not empty
      std::cout << "jet size: " << jetVector->size() << "\n";
      std::cout << "jet et: "<< (jetVector->at(0)).et << "\n";
      }*/
  }

  
  return 0;
}
