#include <vector>
#include "../interface/EventInfo.hh"
#include "../interface/Particles.hh"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class std::vector<ULong64_t>;
#pragma link C++ class std::vector< std::vector<float> >;
#pragma link C++ class std::vector< std::vector<bool> >;
#pragma link C++ class ran::EventInfo+;
#pragma link C++ class ran::ElectronStruct+;
#pragma link C++ class ran::MuonStruct+;
#pragma link C++ class ran::PhotonStruct+;
#pragma link C++ class ran::TrackStruct+;
#pragma link C++ class ran::JetStruct+;
#pragma link C++ class ran::FatJetStruct+;
#pragma link C++ class ran::MetStruct+;
#pragma link C++ class std::vector< ran::ElectronStruct>;
#pragma link C++ class std::vector< ran::MuonStruct>;
#pragma link C++ class std::vector< ran::PhotonStruct>;
#pragma link C++ class std::vector< ran::TrackStruct>;
#pragma link C++ class std::vector< ran::JetStruct>;
#pragma link C++ class std::vector< ran::FatJetStruct>;
#pragma link C++ class std::vector< ran::MetStruct>;

#endif
