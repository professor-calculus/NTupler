#ifndef NTTRACK_HH
#define NTTRACK_HH

//RAL PARTICLE HEADERS
#include "../interface/TrackStruct.hh"

namespace ran{
  class NtTrack{
  public:
    NtTrack() : track(0){} ///< Default constructor
    NtTrack(const ran::TrackStruct& anTrack) : track(&anTrack){} ///< Constructor
    float pt() const {return track->pt;} ///<Particle pt
    float et() const {return track->et;} ///<Particle et
    float eta() const {return track->eta;}///<Particle eta
    float phi() const {return track->phi;}///<Particle phi

    bool fromPV() const {return track->fromPV;}
    int pdgId() const {return track->pdgId;}
    
  private:
    const ran::TrackStruct* track;

  };
}

#endif