/*
 * AliAnalysisTaskFemtoPlotration.cxx
 *
 *  Created on: 24 Nov 2017
 *      Author: bernhardhohlweger
 */

#include "AliAnalysisTaskLightN.h"
//#include "AliLog.h"
ClassImp(AliAnalysisTaskLightN)

AliAnalysisTaskLightN::AliAnalysisTaskLightN()
:AliAnalysisTaskSE()
,fTrackBufferSize(0)
,fMVPileUp(false)
,fEvtCutQA(false)
,fIsMC(false)
,fname()
,fAnalysisParticle()
,fQA()
,fEvtCutsParticle()
,fEvtHistListParticle(0)
,fTrackCutsProton()
,fAntiTrackCutsProton()
,fTrackCutsDeuteron()
,fAntiTrackCutsDeuteron()
,fTrackCutHistListProton(0)
,fTrackCutHistMCListProton(0)
,fAntiTrackCutHistListProton(0)
,fAntiTrackCutHistMCListProton(0)
,fTrackCutHistListDeuteron(0)
,fTrackCutHistMCListDeuteron(0)
,fAntiTrackCutHistListDeuteron(0)
,fAntiTrackCutHistMCListDeuteron(0)
{}

AliAnalysisTaskLightN::AliAnalysisTaskLightN(const char *name,bool isMC)
:AliAnalysisTaskSE(name)
,fTrackBufferSize(0)
,fMVPileUp(false)
,fEvtCutQA(false)
,fIsMC(isMC)
,fname(name)
,fAnalysisParticle()
,fQA()
,fEvtCutsParticle()
,fEvtHistListParticle(0)
,fTrackCutsProton()
,fAntiTrackCutsProton()
,fTrackCutsDeuteron()
,fAntiTrackCutsDeuteron()
,fTrackCutHistListProton(0)
,fTrackCutHistMCListProton(0)
,fAntiTrackCutHistListProton(0)
,fAntiTrackCutHistMCListProton(0)
,fTrackCutHistListDeuteron(0)
,fTrackCutHistMCListDeuteron(0)
,fAntiTrackCutHistListDeuteron(0)
,fAntiTrackCutHistMCListDeuteron(0)
{
    DefineOutput(1, TList::Class());  //Output for the Event Class and Pair Cleaner
    DefineOutput(2, TList::Class());  //Output for the Event Cuts
    DefineOutput(3, TList::Class());  //Output for the Track Cuts (Proton)
    DefineOutput(4, TList::Class());  //Output for the Antitrack Cuts (Proton)
    DefineOutput(5, TList::Class());  //Output for the Track Cuts(Deuteron)
    DefineOutput(6, TList::Class());  //Output for the AntiTrack Cuts (Deuteron)
    DefineOutput(7, TList::Class());  //test systrackT Proton
    DefineOutput(8, TList::Class());  //test systrackT antiProton
    DefineOutput(9, TList::Class());  //test systrackL Deuteron
    DefineOutput(10, TList::Class());  //test systrackL antiDeuteron
    DefineOutput(11, TList::Class());  //test systrackT Proton
    DefineOutput(12, TList::Class());  //test systrackT antiProton
    DefineOutput(13, TList::Class());  //test systrackL Deuteron
    DefineOutput(14, TList::Class());  //test systrackL antiDeuteron
    DefineOutput(15, TList::Class());  //test systrackT Proton
    DefineOutput(16, TList::Class());  //test systrackT antiProton
    DefineOutput(17, TList::Class());  //test systrackL Deuteron
    DefineOutput(18, TList::Class());  //test systrackL antiDeuteron
    DefineOutput(19, TList::Class());  //test systrackT Proton
    DefineOutput(20, TList::Class());  //test systrackT antiProton
    DefineOutput(21, TList::Class());  //test systrackL Deuteron
    DefineOutput(22, TList::Class());  //test systrackL antiDeuteron
    if (fIsMC) {
        DefineOutput(23, TList::Class());  //Output for the Track Cut MC Info (Proton)
        DefineOutput(24, TList::Class()); //Output for the AntiTrack Cut MC Info (Proton)
        DefineOutput(25, TList::Class()); //Output for the Track Cut MC Info (Deuteron)
        DefineOutput(26, TList::Class()); //Output for the AntiTrack Cut MC Info (Deuteron)
    }
}

AliAnalysisTaskLightN::~AliAnalysisTaskLightN() {
    if (fAnalysisParticle) {
        delete fAnalysisParticle;
    }
}

void AliAnalysisTaskLightN::UserCreateOutputObjects() {
    
    
    //Initialization for the analyis of all particles
    fAnalysisParticle=new AliLightNAnalysis();
    fAnalysisParticle->SetTrackBufferSize(fTrackBufferSize);
    fAnalysisParticle->SetMVPileUp(fMVPileUp);
    fAnalysisParticle->SetEvtCutQA(fEvtCutQA);
    //Set the event cuts
    if (fEvtCutsParticle) {
        fAnalysisParticle->SetEventCutsParticle(fEvtCutsParticle);
    } else {
        AliFatal("Event cuts missing!");
    }
    
    //Set the track cuts for all particles to be analyzed
    if (fTrackCutsProton) {
        fAnalysisParticle->SetTrackCutsProton(fTrackCutsProton);
    } else {
        AliFatal("TrackCuts missing!");
    }
    if (fAntiTrackCutsProton) {
        fAnalysisParticle->SetAntiTrackCutsProton(fAntiTrackCutsProton);
    } else {
        AliFatal("Antitrack cuts missing");
    }
    if (fTrackCutsDeuteron) {
        fAnalysisParticle->SetTrackCutsDeuteron(fTrackCutsDeuteron);
    } else {
        AliFatal("TrackCuts missing!");
    }
    if (fAntiTrackCutsDeuteron) {
        fAnalysisParticle->SetAntiTrackCutsDeuteron(fAntiTrackCutsDeuteron);
    } else {
        AliFatal("Antitrack cuts missing");
    }
    
    //Initialize the cuts and the histograms
    fAnalysisParticle->Init();
    
    //Outputs for the event selection
    if (fAnalysisParticle->GetQAList()) {
        fQA=fAnalysisParticle->GetQAList();
    } else {
        AliFatal("QA Histograms not available");
    }
    if (fAnalysisParticle->GetEventCutHists()) {
        fEvtHistListParticle=fAnalysisParticle->GetEventCutHists();
    } else {
        AliFatal("Event Cut Histograms not available");
    }
    
    //Outputs for (anti)proton
    if (fAnalysisParticle->GetTrackCutHistsProton()) {
        fTrackCutHistListProton=fAnalysisParticle->GetTrackCutHistsProton();
    } else {
        AliFatal("Event Cut Histograms not available");
    }
    if (fAnalysisParticle->GetAntitrackCutHistsProton()) {
        fAntiTrackCutHistListProton=fAnalysisParticle->GetAntitrackCutHistsProton();
    } else {
        AliFatal("Event Cut Histograms not available");
    }
    if(strcmp(fname,"LightN")==0)PostData(1,fQA);
    if(strcmp(fname,"LightN")==0)PostData(2,fEvtHistListParticle);
    if(strcmp(fname,"LightN")==0)PostData(3,fTrackCutHistListProton);
    if(strcmp(fname,"LightN")==0)PostData(4,fAntiTrackCutHistListProton);
    
    if (fIsMC) {
        if (fTrackCutsProton->GetMCQAHists()) {
            fTrackCutHistMCListProton=fTrackCutsProton->GetMCQAHists();
        } else {
            AliFatal("No Track Cut MC Histograms!");
        }
        if (fAntiTrackCutsProton->GetMCQAHists()) {
            fAntiTrackCutHistMCListProton=fAntiTrackCutsProton->GetMCQAHists();
        } else {
            AliFatal("No Antitrack Cut MC Histograms!");
        }
        if(strcmp(fname,"LightN")==0)PostData(23,fTrackCutHistMCListProton);
        if(strcmp(fname,"LightN")==0)PostData(24,fAntiTrackCutHistMCListProton);
    }
    
    
    //Outputs for (anti)deuterons
    if (fAnalysisParticle->GetTrackCutHistsDeuteron()) {
        fTrackCutHistListDeuteron=fAnalysisParticle->GetTrackCutHistsDeuteron();
    } else {
        AliFatal("Event Cut Histograms not available");
    }
    if (fAnalysisParticle->GetAntitrackCutHistsDeuteron()) {
        fAntiTrackCutHistListDeuteron=fAnalysisParticle->GetAntitrackCutHistsDeuteron();
    } else {
        AliFatal("Event Cut Histograms not available");
    }
    if(strcmp(fname,"LightN")==0)PostData(5,fTrackCutHistListDeuteron);
    if(strcmp(fname,"LightN")==0)PostData(6,fAntiTrackCutHistListDeuteron);
    
    if (fIsMC) {
        if (fTrackCutsDeuteron->GetMCQAHists()) {
            fTrackCutHistMCListDeuteron=fTrackCutsDeuteron->GetMCQAHists();
        } else {
            AliFatal("No Track Cut MC Histograms!");
        }
        if (fAntiTrackCutsDeuteron->GetMCQAHists()) {
            fAntiTrackCutHistMCListDeuteron=fAntiTrackCutsDeuteron->GetMCQAHists();
        } else {
            AliFatal("No Antitrack Cut MC Histograms!");
        }
        if(strcmp(fname,"LightN")==0)PostData(25,fTrackCutHistMCListDeuteron);
        if(strcmp(fname,"LightN")==0)PostData(26,fAntiTrackCutHistMCListDeuteron);
    }
    
    //Systematics
    if(strcmp(fname,"systematics1")==0)PostData(7,fTrackCutHistListProton);
    if(strcmp(fname,"systematics1")==0)PostData(8,fAntiTrackCutHistListProton);
    if(strcmp(fname,"systematics1")==0)PostData(9,fTrackCutHistListDeuteron);
    if(strcmp(fname,"systematics1")==0)PostData(10,fAntiTrackCutHistListDeuteron);
    
    if(strcmp(fname,"systematics2")==0)PostData(11,fTrackCutHistListProton);
    if(strcmp(fname,"systematics2")==0)PostData(12,fAntiTrackCutHistListProton);
    if(strcmp(fname,"systematics2")==0)PostData(13,fTrackCutHistListDeuteron);
    if(strcmp(fname,"systematics2")==0)PostData(14,fAntiTrackCutHistListDeuteron);
    
    if(strcmp(fname,"systematics3")==0)PostData(15,fTrackCutHistListProton);
    if(strcmp(fname,"systematics3")==0)PostData(16,fAntiTrackCutHistListProton);
    if(strcmp(fname,"systematics3")==0)PostData(17,fTrackCutHistListDeuteron);
    if(strcmp(fname,"systematics3")==0)PostData(18,fAntiTrackCutHistListDeuteron);
    
    if(strcmp(fname,"systematics4")==0)PostData(19,fTrackCutHistListProton);
    if(strcmp(fname,"systematics4")==0)PostData(20,fAntiTrackCutHistListProton);
    if(strcmp(fname,"systematics4")==0)PostData(21,fTrackCutHistListDeuteron);
    if(strcmp(fname,"systematics4")==0)PostData(22,fAntiTrackCutHistListDeuteron);
    
}

void AliAnalysisTaskLightN::UserExec(Option_t *) {
    AliAODEvent *Event=static_cast<AliAODEvent*>(fInputEvent);
    if (!Event) {
        AliFatal("No Input Event");
    } else {
        
        //Execute the Analysis ((anti)p and (anti)d)
        fAnalysisParticle->Make(Event);
        if (fAnalysisParticle->GetQAList()) {
            fQA=fAnalysisParticle->GetQAList();
        } else {
            AliFatal("QA Histograms not available");
        }
        if (fAnalysisParticle->GetEventCutHists()) {
            fEvtHistListParticle=fAnalysisParticle->GetEventCutHists();
        } else {
            AliFatal("Event Cut Histograms not available");
        }
        if (fAnalysisParticle->GetTrackCutHistsProton()) {
            fTrackCutHistListProton=fAnalysisParticle->GetTrackCutHistsProton();
        } else {
            AliFatal("Track Cut Histograms not available");
        }
        if (fAnalysisParticle->GetAntitrackCutHistsProton()) {
            fAntiTrackCutHistListProton=fAnalysisParticle->GetAntitrackCutHistsProton();
        } else {
            AliFatal("AntiTrack Cut Histograms not available");
        }
        if(strcmp(fname,"LightN")==0)PostData(1,fQA);
        if(strcmp(fname,"LightN")==0)PostData(2,fEvtHistListParticle);
        if(strcmp(fname,"LightN")==0)PostData(3,fTrackCutHistListProton);
        if(strcmp(fname,"LightN")==0)PostData(4,fAntiTrackCutHistListProton);
        if (fIsMC) {
            if (fTrackCutsProton->GetMCQAHists()) {
                fTrackCutHistMCListProton=fTrackCutsProton->GetMCQAHists();
            } else {
                AliFatal("No Track Cut MC Histograms!");
            }
            if (fAntiTrackCutsProton->GetMCQAHists()) {
                fAntiTrackCutHistMCListProton=fAntiTrackCutsProton->GetMCQAHists();
            } else {
                AliFatal("No Antitrack Cut MC Histograms!");
            }
            if(strcmp(fname,"LightN")==0)PostData(23,fTrackCutHistMCListProton);
            if(strcmp(fname,"LightN")==0)PostData(24,fAntiTrackCutHistMCListProton);
        }
        
        if (fAnalysisParticle->GetTrackCutHistsDeuteron()) {
            fTrackCutHistListDeuteron=fAnalysisParticle->GetTrackCutHistsDeuteron();
        } else {
            AliFatal("Track Cut Histograms not available");
        }
        if (fAnalysisParticle->GetAntitrackCutHistsDeuteron()) {
            fAntiTrackCutHistListDeuteron=fAnalysisParticle->GetAntitrackCutHistsDeuteron();
        } else {
            AliFatal("AntiTrack Cut Histograms not available");
        }
        
        if(strcmp(fname,"LightN")==0)PostData(5,fTrackCutHistListDeuteron);
        if(strcmp(fname,"LightN")==0)PostData(6,fAntiTrackCutHistListDeuteron);
        if (fIsMC) {
            if (fTrackCutsDeuteron->GetMCQAHists()) {
                fTrackCutHistMCListDeuteron=fTrackCutsDeuteron->GetMCQAHists();
            } else {
                AliFatal("No Track Cut MC Histograms!");
            }
            if (fAntiTrackCutsDeuteron->GetMCQAHists()) {
                fAntiTrackCutHistMCListDeuteron=fAntiTrackCutsDeuteron->GetMCQAHists();
            } else {
                AliFatal("No Antitrack Cut MC Histograms!");
            }
            if(strcmp(fname,"LightN")==0)PostData(25,fTrackCutHistMCListDeuteron);
            if(strcmp(fname,"LightN")==0)PostData(26,fAntiTrackCutHistMCListDeuteron);
        }
        
        //Systematics
        if(strcmp(fname,"systematics1")==0)PostData(7,fTrackCutHistListProton);
        if(strcmp(fname,"systematics1")==0)PostData(8,fAntiTrackCutHistListProton);
        if(strcmp(fname,"systematics1")==0)PostData(9,fTrackCutHistListDeuteron);
        if(strcmp(fname,"systematics1")==0)PostData(10,fAntiTrackCutHistListDeuteron);
        
        if(strcmp(fname,"systematics2")==0)PostData(11,fTrackCutHistListProton);
        if(strcmp(fname,"systematics2")==0)PostData(12,fAntiTrackCutHistListProton);
        if(strcmp(fname,"systematics2")==0)PostData(13,fTrackCutHistListDeuteron);
        if(strcmp(fname,"systematics2")==0)PostData(14,fAntiTrackCutHistListDeuteron);
        
        if(strcmp(fname,"systematics3")==0)PostData(15,fTrackCutHistListProton);
        if(strcmp(fname,"systematics3")==0)PostData(16,fAntiTrackCutHistListProton);
        if(strcmp(fname,"systematics3")==0)PostData(17,fTrackCutHistListDeuteron);
        if(strcmp(fname,"systematics3")==0)PostData(18,fAntiTrackCutHistListDeuteron);
        
        if(strcmp(fname,"systematics4")==0)PostData(19,fTrackCutHistListProton);
        if(strcmp(fname,"systematics4")==0)PostData(20,fAntiTrackCutHistListProton);
        if(strcmp(fname,"systematics4")==0)PostData(21,fTrackCutHistListDeuteron);
        if(strcmp(fname,"systematics4")==0)PostData(22,fAntiTrackCutHistListDeuteron);
        
    }
}
