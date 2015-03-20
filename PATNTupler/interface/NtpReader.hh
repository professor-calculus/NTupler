#ifndef NTPREADER_HH
#define NTPREADER_HH

//Root HEADERS
#include <TTree.h>
#include <TString.h>

//STL HEADERS
#include <vector>

//BOOST HEADERS
#include <boost/shared_ptr.hpp>

//STL HEADERS
#include <string>

//RAL PARTICLE HEADERS
#include "../interface/EventInfo.hh"
#include "../interface/NtElectron.hh"
#include "../interface/NtMuon.hh"
#include "../interface/NtJet.hh"
#include "../interface/NtMet.hh"

//ROOT headers
#include "TChain.h"

using boost::shared_ptr;
using ran::ElectronStruct;
using ran::MuonStruct;
using ran::JetStruct;
using std::string;

class NtpReader{
public:
  NtpReader(TTree* inTree) : ntpEvtNum(0), maxEvents(inTree->GetEntries()), evtTree(inTree){}///< Constructor
  NtpReader(TTree* inTree, const TString& branchName) : ntpEvtNum(0), maxEvents(inTree->GetEntries()), evtTree(inTree){ ///< Constructor
    setEventInfoBranch(branchName);
  }
  NtpReader(string listOfFiles, string treeName);///< Constructor
  unsigned int runNum() const {return evtInfo->runNum;}//return the run number
  unsigned int evtNum() const {return evtInfo->evtNum;}//return the event number
  unsigned int lumiSec() const {return evtInfo->lumiSec;}//return the lumisec
  void setEventInfoBranch(const TString& branchName);///< Sets the event info branch
  void setEntryInfo();///< sets the event information
  template <typename T, typename U> std::vector<T> getParticleCollection(const TString& branchName);
  std::vector<ran::NtElectron> testBranch2(const TString& branchName);
  bool isLastEntry(); ///< Checks if last event in the ntuple
  unsigned int getLastEntry(){return maxEvents;}
  unsigned int getEntryNumber() const {return ntpEvtNum;} ///< Returns the current event number
  void nextEntry(){++ntpEvtNum; setEntryInfo();}///< Increment to next event  and set eventinfo
  void reseEntry(){ntpEvtNum = 0; setEntryInfo();}///< Reset to first event and set eventinfo
  void setEntryNumber(unsigned int numValue){ ntpEvtNum = numValue; setEntryInfo();} ///< Set the event number  and set eventinfo
private:
  unsigned int ntpEvtNum;///< Ntuple Event number
  unsigned int maxEvents; ///< Maximum number of events in the ntuple
  TTree* evtTree;///< Ntuple Tree
  TBranch* eventInfoBranch; ///< Event info branch
  TBranch* electronBranch; ///< Electron branch
  TBranch* jetBranch; ///< Jet branch
  TBranch* muonBranch; ///< Muon branch
  shared_ptr<ran::EventInfo> evtInfo;
};

inline void NtpReader::setEntryInfo(){
  eventInfoBranch->GetEntry(ntpEvtNum);
}

template <typename T, typename U> 
std::vector<T> NtpReader::getParticleCollection(const TString& branchName){
  std::vector<U>* particleVector = new std::vector<U>(); //memory management, need the new
  
  TBranch* particleBranch = evtTree->GetBranch(branchName);
  particleBranch->SetAddress(&particleVector);
  particleBranch->GetEntry(ntpEvtNum);

  std::vector<T> ntpParticleVector(particleVector->begin(), particleVector->end());
  
  //release memory, may need to be careful about this, should relase it at next event
  //delete particleVector;

  return ntpParticleVector; 
}

void fillE(std::vector<ran::NtElectron>*, 
	   const std::vector<ran::ElectronStruct>*);///< Fill a vector of NtElectrons from a vector of ElectronStructs




#endif
