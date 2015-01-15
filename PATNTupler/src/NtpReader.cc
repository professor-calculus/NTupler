#include "../interface/NtpReader.hh"
#include <fstream>

using std::ifstream;

NtpReader::NtpReader(string listOfFiles, string treeName){

  TChain* chain =  new TChain(treeName.c_str()); //memory management?

  //need to read list of files and chain the contents
  ifstream iFile(listOfFiles.c_str());
  string line;

  while (getline(iFile,line)){
    chain->Add(line.c_str());
  }

  evtTree = chain;
  maxEvents = chain->GetEntries();
  ntpEvtNum = 0;

}

std::vector<ran::NtElectron>  NtpReader::testBranch2(const TString& branchName){
  std::vector<ran::ElectronStruct>*  electronVector = new std::vector<ElectronStruct>();
  TBranch* electronBranch = evtTree->GetBranch(branchName);
  electronBranch->SetAddress(&electronVector);
  electronBranch->GetEntry(ntpEvtNum);

  std::vector<ran::NtElectron> ntpElectronVector;
  //reserve vector size at least the same size as vector to be copied
  ntpElectronVector.reserve(electronVector->size());
  //loop over source vector and fill new vector
  for (std::vector<ran::ElectronStruct>::const_iterator iter = electronVector->begin();
       iter != electronVector->end(); ++iter){
    ran::NtElectron elec(*iter);
    //ran::NtElectron elec(*iter);
    ntpElectronVector.push_back(elec);
  }

  return ntpElectronVector;
}

void NtpReader::setEventInfoBranch(const TString& branchName){
  evtInfo = shared_ptr< ran::EventInfo >(new ran::EventInfo());
  eventInfoBranch = evtTree->GetBranch(branchName);
  eventInfoBranch->SetAddress(&evtInfo);
}

void NtpReader::setEntryInfo(){
  eventInfoBranch->GetEntry(ntpEvtNum);
}

bool NtpReader::isLastEntry(){
  if (maxEvents == ntpEvtNum){
    return true;
  } else {
    return false;
  }
}

void fillE(std::vector<ran::NtElectron>* ralECollection, 
	   const std::vector<ran::ElectronStruct>* eCollection){
  //reserve vector size at least the same size as vector to be copied
  ralECollection->reserve(eCollection->size());
  for (std::vector<ran::ElectronStruct>::const_iterator iter = eCollection->begin();
       iter != eCollection->end(); ++iter){
    ran::NtElectron elec(*iter);
    ralECollection->push_back(elec);
  }
}
