#ifndef OBJECTIDCALCULATIONS_H
#define OBJECTIDCALCULATIONS_H

//STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <random>

//ROOT HEADERS
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h> 
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

//RAL PARTICLE HEADERS
#include "deltaR.h"
#include "rhoEACorrections.h"

namespace ObjectID{

    // ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

    double relPFIsoR03 (ran::NtMuon muon, float rho, int year) {
        double CHIso = TMath::Max(0., muon.pfIsoR03_sumChgHadPt() - rho*RhoEACorr::getEA_CH(muon.eta(), year));
        double NHIso = TMath::Max(0., muon.pfIsoR03_sumNeutHadPt() - rho*RhoEACorr::getEA_NH(muon.eta(), year));
        double PhotonIso = TMath::Max(0., muon.pfIsoR03_sumPhtEt() - rho*RhoEACorr::getEA_PH(muon.eta(), year));
        double relIso = (CHIso + std::max(0., NHIso + PhotonIso - 0.5*muon.pfIsoR03_sumPUPt()))/(muon.pt());

        return relIso;
    }

    double relPFIsoR04 (ran::NtMuon muon, float rho, int year) {
        double CHIso = TMath::Max(0., muon.pfIsoR04_sumChgHadPt() - rho*RhoEACorr::getEA_CH(muon.eta(), year));
        double NHIso = TMath::Max(0., muon.pfIsoR04_sumNeutHadPt() - rho*RhoEACorr::getEA_NH(muon.eta(), year));
        double PhotonIso = TMath::Max(0., muon.pfIsoR04_sumPhtEt() - rho*RhoEACorr::getEA_PH(muon.eta(), year));
        double relIso = (CHIso + std::max(0., NHIso + PhotonIso - 0.5*muon.pfIsoR04_sumPUPt()))/(muon.pt());

        return relIso;
    }

    double electronPFIsolation(std::vector<ran::NtTrack> pfcands,
                            const ran::NtElectron ptcl,
                            double r_iso_min, double r_iso_max, double kt_scale,
                            bool charged_only, float rho, int year) {
        
        if (ptcl.pt() < 5) return 9999.;

        double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
        if (fabs(ptcl.eta())>1.479) {deadcone_ch = 0.015; deadcone_pu = 0.015; deadcone_ph = 0.08;}

        double iso_nh(0.); double iso_ch(0.); 
        double iso_ph(0.); double iso_pu(0.);
        double ptThresh(0.);
        double r_iso = TMath::Max(r_iso_min,TMath::Min(r_iso_max, kt_scale/ptcl.pt()));
        for (unsigned int iTrk = 0; iTrk < pfcands.size(); iTrk++) {
        ran::NtTrack pfc = pfcands[iTrk];
        if (abs(pfc.pdgId())<7) continue;

        double dr = deltaR(pfc, ptcl);
        if (dr > r_iso) continue;
        
        //////////////////  NEUTRALS  /////////////////////////
        if (pfc.charge()==0){
            if (pfc.pt()>ptThresh) {
            /////////// PHOTONS ////////////
            if (abs(pfc.pdgId())==22) {
                if(dr < deadcone_ph) continue;
                iso_ph += pfc.pt();
            /////////// NEUTRAL HADRONS ////////////
            } else if (abs(pfc.pdgId())==130) {
                if(dr < deadcone_nh) continue;
                iso_nh += pfc.pt();
            }
            }
            //////////////////  CHARGED from PV  /////////////////////////
        } else if (pfc.fromPV()>1){
            if (abs(pfc.pdgId())==211) {
            if(dr < deadcone_ch) continue;
            iso_ch += pfc.pt();
            }
            //////////////////  CHARGED from PU  /////////////////////////
        } else {
            if (pfc.pt()>ptThresh){
            if(dr < deadcone_pu) continue;
            iso_pu += pfc.pt();
            }
        }
        }
        iso_ch = TMath::Max(0., iso_ch - rho*RhoEACorr::getEA_CH(ptcl.eta(), year));
        iso_nh = TMath::Max(0., iso_nh - rho*RhoEACorr::getEA_NH(ptcl.eta(), year));
        iso_ph = TMath::Max(0., iso_ph - rho*RhoEACorr::getEA_PH(ptcl.eta(), year));
        double iso(0.);
        if (charged_only){
        iso = iso_ch;
        } else {
        iso = iso_ph + iso_nh;
        iso -= 0.5*iso_pu;
        if (iso>0) iso += iso_ch;
        else iso = iso_ch;
        }
        iso = iso/ptcl.pt();

        return iso;
    }

    double muonPFIsolation(std::vector<ran::NtTrack> pfcands,
                            const ran::NtMuon ptcl,  
                            double r_iso_min, double r_iso_max, double kt_scale,
                            bool charged_only) {
        
        if (ptcl.pt() < 5) return 9999.;

        double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
        deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;  

        double iso_nh(0.); double iso_ch(0.); 
        double iso_ph(0.); double iso_pu(0.);
        double ptThresh(0.5);
        double r_iso = TMath::Max(r_iso_min,TMath::Min(r_iso_max, kt_scale/ptcl.pt()));
        for (unsigned int iTrk = 0; iTrk < pfcands.size(); iTrk++) {
        ran::NtTrack pfc = pfcands[iTrk];
        if (abs(pfc.pdgId())<7) continue;

        double dr = deltaR(pfc, ptcl);
        if (dr > r_iso) continue;
        
        //////////////////  NEUTRALS  /////////////////////////
        if (pfc.charge()==0){
            if (pfc.pt()>ptThresh) {
            /////////// PHOTONS ////////////
            if (abs(pfc.pdgId())==22) {
                if(dr < deadcone_ph) continue;
                iso_ph += pfc.pt();
            /////////// NEUTRAL HADRONS ////////////
            } else if (abs(pfc.pdgId())==130) {
                if(dr < deadcone_nh) continue;
                iso_nh += pfc.pt();
            }
            }
            //////////////////  CHARGED from PV  /////////////////////////
        } else if (pfc.fromPV()>1){
            if (abs(pfc.pdgId())==211) {
            if(dr < deadcone_ch) continue;
            iso_ch += pfc.pt();
            }
            //////////////////  CHARGED from PU  /////////////////////////
        } else {
            if (pfc.pt()>ptThresh){
            if(dr < deadcone_pu) continue;
            iso_pu += pfc.pt();
            }
        }
        }
        double iso(0.);
        if (charged_only){
        iso = iso_ch;
        } else {
        iso = iso_ph + iso_nh;
        iso -= 0.5*iso_pu;
        if (iso>0) iso += iso_ch;
        else iso = iso_ch;
        }
        iso = iso/ptcl.pt();

        return iso;
    }

    double photonPFIsolation(std::vector<ran::NtTrack> pfcands,
                            const ran::NtPhoton ptcl,  
                            bool charged_only, float rho, int year) {
        
        if (ptcl.pt() < 5) return 9999.;

        double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
        deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;

        double iso_nh(0.); double iso_ch(0.); 
        double iso_ph(0.); double iso_pu(0.);
        double ptThresh(0.5);
        double r_iso = 0.3;
        for (unsigned int iTrk(0); iTrk < pfcands.size(); iTrk++) {
        ran::NtTrack pfc = pfcands[iTrk];
        if (abs(pfc.pdgId())<7) continue;

        double dr = deltaR(pfc, ptcl);
        if (dr > r_iso) continue;
        
        //////////////////  NEUTRALS  /////////////////////////
        if (pfc.charge()==0){
            if (pfc.pt()>ptThresh) {
            /////////// PHOTONS ////////////
            if (abs(pfc.pdgId())==22) {
                if(dr < deadcone_ph) continue;
                iso_ph += pfc.pt();
            /////////// NEUTRAL HADRONS ////////////
            } else if (abs(pfc.pdgId())==130) {
                if(dr < deadcone_nh) continue;
                iso_nh += pfc.pt();
            }
            }
            //////////////////  CHARGED from PV  /////////////////////////
        } else if (pfc.fromPV()>1){
            if (abs(pfc.pdgId())==211) {
            if(dr < deadcone_ch) continue;
            iso_ch += pfc.pt();
            }
            //////////////////  CHARGED from PU  /////////////////////////
        } else {
            if (pfc.pt()>ptThresh){
            if(dr < deadcone_pu) continue;
            iso_pu += pfc.pt();
            }
        }
        }
        iso_ch = TMath::Max(0., iso_ch - rho*RhoEACorr::getEA_CH(ptcl.eta(), year));
        iso_nh = TMath::Max(0., iso_nh - rho*RhoEACorr::getEA_NH(ptcl.eta(), year));
        iso_ph = TMath::Max(0., iso_ph - rho*RhoEACorr::getEA_PH(ptcl.eta(), year));
        double iso(0.);
        if (charged_only){
        iso = iso_ch;
        } else {
        iso = iso_ph + iso_nh;
        iso -= 0.5*iso_pu;
        if (iso>0) iso += iso_ch;
        else iso = iso_ch;
        }
        iso = iso/ptcl.pt();

        return iso;
    }

    double trackPFIsolation(std::vector<ran::NtTrack> pfcands,
                            const ran::NtTrack ptcl,  
                            bool charged_only) {
        
        if (ptcl.pt() < 5) return 9999.;

        double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
        deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;

        double iso_nh(0.); double iso_ch(0.); 
        double iso_ph(0.); double iso_pu(0.);
        double ptThresh(0.5);
        double r_iso = 0.3;
        for (unsigned int iTrk = 0; iTrk < pfcands.size(); iTrk++) {
        ran::NtTrack pfc = pfcands[iTrk];
        if (abs(pfc.pdgId())<7) continue;

        double dr = deltaR(pfc, ptcl);
        if (dr > r_iso) continue;
        
        //////////////////  NEUTRALS  /////////////////////////
        if (pfc.charge()==0){
            if (pfc.pt()>ptThresh) {
            /////////// PHOTONS ////////////
            if (abs(pfc.pdgId())==22) {
                if(dr < deadcone_ph) continue;
                iso_ph += pfc.pt();
            /////////// NEUTRAL HADRONS ////////////
            } else if (abs(pfc.pdgId())==130) {
                if(dr < deadcone_nh) continue;
                iso_nh += pfc.pt();
            }
            }
            //////////////////  CHARGED from PV  /////////////////////////
        } else if (pfc.fromPV()>1){
            if (abs(pfc.pdgId())==211) {
            if(dr < deadcone_ch) continue;
            iso_ch += pfc.pt();
            }
            //////////////////  CHARGED from PU  /////////////////////////
        } else {
            if (pfc.pt()>ptThresh){
            if(dr < deadcone_pu) continue;
            iso_pu += pfc.pt();
            }
        }
        }
        double iso(0.);
        if (charged_only){
        iso = iso_ch;
        } else {
        iso = iso_ph + iso_nh;
        iso -= 0.5*iso_pu;
        if (iso>0) iso += iso_ch;
        else iso = iso_ch;
        }
        iso = iso/ptcl.pt();

        return iso;
    }



    std::vector<ran::NtMuon> looseMuons (std::vector<ran::NtMuon> muons, float rho, int year) {
        std::vector<ran::NtMuon> looseMu;
        for(unsigned int i=0; i<muons.size(); i++)
        {
            if(muons[i].isLooseMuon()  && relPFIsoR04(muons[i], rho, year) < 0.25)
            {
                looseMu.push_back(muons[i]);
            }
        }
        return looseMu;
    }

    std::vector<ran::NtMuon> tightMuons (std::vector<ran::NtMuon> muons, float rho, int year) {
        std::vector<ran::NtMuon> tightMu;
        for(unsigned int i=0; i<muons.size(); i++)
        {
            if(muons[i].isTightMuon() && relPFIsoR03(muons[i], rho, year) < 0.15)
            {
                tightMu.push_back(muons[i]);
            }
        }
        return tightMu;
    }

    bool jetIDLoose (ran::NtJet jet) {
        // looseJetID = (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((abs(eta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || abs(eta)>2.4) && abs(eta)<=2.7
        bool jetIsLoose = false;
        if (abs(jet.eta()) < 2.7)
        {
            jetIsLoose = ( (jet.neutralHadronEnergyFraction() < 0.99 && jet.neutralEmEnergyFraction() < 0.99 && jet.numberOfDaughters() > 1)
                        && ((abs(jet.eta()) < 2.4 && jet.chargedHadronEnergyFraction() > 0 && jet.chargedMultiplicity() > 0 && jet.chargedEmEnergyFraction() < 0.99) || abs(jet.eta()) > 2.4) );
        }
        else if (abs(jet.eta()) < 3.0)
        {
            jetIsLoose = ( jet.neutralEmEnergyFraction() > 0.01 && jet.neutralHadronEnergyFraction() < 0.98 && jet.neutralMultiplicity() > 2 );
        }
        else
        {
            jetIsLoose = ( jet.neutralEmEnergyFraction() < 0.9 && jet.neutralMultiplicity() > 10);
        }
        return jetIsLoose;
    }

} // closes namespace ObjectID
#endif