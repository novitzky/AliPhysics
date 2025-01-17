
// For: Net Lambda fluctuation analysis via traditional method
// By: Ejiro Naomi Umaka Apr 2018
// Updated jun 10


#include "AliAnalysisManager.h"
#include "AliInputEventHandler.h"
#include "AliESDEvent.h"
#include "AliAODEvent.h"
#include "AliMCEvent.h"
#include "AliAODTrack.h"
#include "AliESDtrack.h"
#include "AliEventCuts.h"
#include "AliExternalTrackParam.h"
#include "AliAnalysisFilter.h"
#include "AliVMultiplicity.h"
#include "AliAnalysisUtils.h"
#include "AliAODMCParticle.h"
#include "AliStack.h"
#include "AliPIDResponse.h"
#include "AliMCEventHandler.h"
#include "AliV0vertexer.h"
#include "AliESDv0Cuts.h"
#include "AliMultSelection.h"
#include "TMath.h"
#include "TFile.h"
#include "TList.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TH3F.h"
#include "THnSparse.h"


#include "AliAnalysisTaskNetLambdaTrad.h"

ClassImp(AliAnalysisTaskNetLambdaTrad)

//-----------------------------------------------------------------------------
AliAnalysisTaskNetLambdaTrad::AliAnalysisTaskNetLambdaTrad(const char* name) :
AliAnalysisTaskSE(name),
fESD(0x0),
fPIDResponse(0x0),
fEventCuts(0),
fListHist(0x0),
fHistEventCounter(0x0),
fHistCentrality(0x0),



f3fHistCentVsInvMassLambda1point0(0x0),
f3fHistCentVsInvMassLambda1point0Masscut(0x0),

f3fHistCentVsInvMassAntiLambda1point0(0x0),
f3fHistCentVsInvMassAntiLambda1point0Masscut(0x0),
f3fHistCentVsInvMassLambda1point0bkg(0x0),
f3fHistCentVsInvMassAntiLambda1point0bkg(0x0),

fCentrality(-1),
fNptBins(23),

fEvSel(AliVEvent::kINT7),
fPtBinNplusNminusCh(NULL)


{
    Info("AliAnalysisTaskNetLambdaTrad","Calling Constructor");
    
    DefineInput(0,TChain::Class());
    DefineOutput(1,TList::Class());
    
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AliAnalysisTaskNetLambdaTrad::UserCreateOutputObjects()
{
    fListHist = new TList();
    fListHist->SetOwner();
    
    fHistEventCounter = new TH1D( "fHistEventCounter", ";Evt. Sel. Step;Count",2,0,2);
    fHistEventCounter->GetXaxis()->SetBinLabel(1, "Processed");
    fHistEventCounter->GetXaxis()->SetBinLabel(2, "Selected");
    fListHist->Add(fHistEventCounter);
    
    fHistCentrality = new TH1D( "fHistCentrality", "fHistCentrality",100,0,100);
    fListHist->Add(fHistCentrality);
    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    const Int_t CentbinNum = 81;
    Double_t CentBins[CentbinNum+1];
    for(Int_t ic = 0; ic <= CentbinNum; ic++) CentBins[ic] = ic - 0.5;
    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //pt binning
    Double_t LambdaPtBins[24] = {0.9,1.0,1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,4.2, 4.4};

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Double_t MassBins[103]
    = {1.0788,1.0796,1.0804,1.0812,1.082,1.0828,1.0836,1.0844,1.0852,1.086,1.0868,1.0876,1.0884,1.0892,1.09,1.0908,1.0916,1.0924,1.0932,1.094,1.0948,1.0956,1.0964,1.0972,1.098,1.0988,1.0996,1.1004,1.1012,1.102,
        1.1028,1.1036,1.1044,1.1052,1.106,1.1068,1.1076,1.1084,1.1092,1.11,1.1108,1.1116,1.1124,1.1132,1.114,1.1148,1.1156,1.1164,1.1172,1.118,1.1188,1.1196,1.1204,1.1212,1.122,1.1228,1.1236,1.1244,
        1.1252,1.126,1.1268,1.1276,1.1284,1.1292,1.13,1.1308,1.1316,1.1324,1.1332,1.134,1.1348,1.1356,1.1364,1.1372,1.138,1.1388,1.1396,1.1404,1.1412,1.142,1.1428,1.1436,1.1444,1.1452,1.146,1.1468,1.1476,
        1.1484,1.1492,1.15,1.1508,1.1516,1.1524,1.1532,1.154,1.1548,1.1556,1.1564,1.1572,1.158,1.1588,1.1596,1.1604};
    Long_t Massbinnumb = sizeof(MassBins)/sizeof(Double_t) - 1;
    
    //V0 hists//
    
    
    f3fHistCentVsInvMassLambda1point0 = new TH3F("f3fHistCentVsInvMassLambda1point0","Cent vs. #Lambda Inv Mass vs. pT",CentbinNum, CentBins, Massbinnumb,MassBins,fNptBins, LambdaPtBins);
    fListHist->Add(f3fHistCentVsInvMassLambda1point0);
    
    f3fHistCentVsInvMassLambda1point0Masscut = new TH3F("f3fHistCentVsInvMassLambda1point0Masscut","Cent vs. #Lambda Inv Mass vs. pT",CentbinNum, CentBins, Massbinnumb,MassBins,fNptBins, LambdaPtBins);
    fListHist->Add(f3fHistCentVsInvMassLambda1point0Masscut);
    
    //////////////////
    f3fHistCentVsInvMassAntiLambda1point0 = new TH3F("f3fHistCentVsInvMassAntiLambda1point0","Cent vs. #bar{#Lambda} Inv Mass vs. pT",CentbinNum, CentBins, Massbinnumb,MassBins,fNptBins, LambdaPtBins);
    fListHist->Add(f3fHistCentVsInvMassAntiLambda1point0);
    
    f3fHistCentVsInvMassAntiLambda1point0Masscut = new TH3F("f3fHistCentVsInvMassAntiLambda1point0Masscut","Cent vs. #bar{#Lambda} Inv Mass vs. pT",CentbinNum, CentBins, Massbinnumb,MassBins,fNptBins, LambdaPtBins);
    fListHist->Add(f3fHistCentVsInvMassAntiLambda1point0Masscut);
    
    f3fHistCentVsInvMassLambda1point0bkg = new TH3F("f3fHistCentVsInvMassLambda1point0bkg","Cent vs. #Lambda Inv Mass vs. pT bkg",CentbinNum, CentBins, Massbinnumb,MassBins,fNptBins, LambdaPtBins);
    fListHist->Add(f3fHistCentVsInvMassLambda1point0bkg);
    
    f3fHistCentVsInvMassAntiLambda1point0bkg = new TH3F("f3fHistCentVsInvMassAntiLambda1point0bkg","Cent vs. #bar{#Lambda} Inv Mass vs. pT bkg",CentbinNum, CentBins, Massbinnumb,MassBins,fNptBins, LambdaPtBins);
    fListHist->Add(f3fHistCentVsInvMassAntiLambda1point0bkg);
    
    
    
    
    
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //THNSPARSE BINNING
    
    const Int_t dim = 47; //23 pt bins*2 + 1 cent bin
    Int_t bin[dim];
    bin[0] = 81;
    for(Int_t ibin = 1; ibin < dim; ibin++) bin[ibin] = 500;
    Double_t min[dim];
    for(Int_t jbin = 0; jbin < dim; jbin++) min[jbin] =  -0.5;
    Double_t max[dim];
    max[0] = 80.5;
    for(Int_t jbin = 1; jbin < dim; jbin++) max[jbin] = 499.5;
    

    
    fPtBinNplusNminusCh = new THnSparseI("fPtBinNplusNminusCh","cent-nlambda-nantilambda masscut", dim, bin, min, max);
    fListHist->Add(fPtBinNplusNminusCh);
    
    
    PostData(1,fListHist);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AliAnalysisTaskNetLambdaTrad::UserExec(Option_t *)
{
    
    const Int_t dim = fNptBins*2;
    
    Int_t ptChEta1point0[dim];
    
    
    for(Int_t idx = 0; idx < dim; idx++)
    {
        ptChEta1point0[idx] = 0.;
    }
    
    if (!fInputEvent) return;
    
    fESD = dynamic_cast<AliESDEvent*>(InputEvent());
    if (!fESD) return;
    
    
    fPIDResponse = fInputHandler->GetPIDResponse();
    if(!fPIDResponse) return;
    
    Double_t lMagneticField = -10;
    lMagneticField = fESD->GetMagneticField();
    
    
    AliMultSelection *MultSelection = (AliMultSelection*) fInputEvent->FindListObject("MultSelection");
    if(!MultSelection) return;
    
    if(!(fInputHandler->IsEventSelected() & fEvSel)) return;
    
    Double_t vVtx[3];
    
    AliVVertex *vvertex = (AliVVertex*)fInputEvent->GetPrimaryVertex();
    if (!vvertex) return;
    vVtx[0] = vvertex->GetX();
    vVtx[1] = vvertex->GetY();
    vVtx[2] = vvertex->GetZ();
    
    if(vVtx[2] < -10. || vVtx[2] > 10.) return;
    
    fCentrality = MultSelection->GetMultiplicityPercentile("V0M");
    
    if( fCentrality < 0 || fCentrality >=80 ) return;
    if (!fEventCuts.AcceptEvent(fInputEvent)) return;//pileup cut
    
    fHistEventCounter->Fill(1.5);
    fHistCentrality->Fill(fCentrality);
    
    const AliESDVertex *lPrimaryBestESDVtx     = fESD->GetPrimaryVertex();
    Double_t lBestPrimaryVtxPos[3]          = {-100.0, -100.0, -100.0};
    lPrimaryBestESDVtx->GetXYZ( lBestPrimaryVtxPos );
    
    Int_t nV0 = 0;
    Double_t fMinV0Pt = 0.5;
    Double_t fMaxV0Pt = 4.5;
    nV0 = fESD->GetNumberOfV0s();
    AliESDv0 *esdv0 = 0x0;
    AliESDtrack *esdpTrack = 0x0;
    AliESDtrack *esdnTrack = 0x0;
    
    for(Int_t iV0 = 0; iV0 < nV0; iV0++)
    {
        esdv0 = 0x0;
        esdpTrack = 0x0;
        esdnTrack = 0x0;
        
        Double_t  vertx[3];
        
        Float_t invMassLambda = -999, invMassAntiLambda = -999;
        Float_t V0pt = -999, eta = -999, pmom = -999;
        Float_t ppt = -999,  peta = -999, posprnsg = -999, pospion =-999, v0Radius =-999, lRapLambda=-999;
        Float_t npt = -999,  neta = -999, negprnsg = -999, negpion =-999;
        Bool_t  ontheflystat = kFALSE;
        Float_t dcaPosToVertex = -999, dcaNegToVertex = -999, dcaDaughters = -999, dcaV0ToVertex = -999, cosPointingAngle = -999;
        
        esdv0 = fESD->GetV0(iV0);
        if(!esdv0) continue;
        esdpTrack =  (AliESDtrack*)fESD->GetTrack(TMath::Abs(esdv0->GetPindex()));
        if(!esdpTrack) continue;
        esdnTrack = (AliESDtrack*)fESD->GetTrack(TMath::Abs(esdv0->GetNindex()));
        if(!esdnTrack) continue;
        
        if(esdpTrack->Charge() == esdnTrack->Charge())
        {
            continue;
        }
        esdv0->GetXYZ(vertx[0], vertx[1], vertx[2]); //decay vertex
        v0Radius = TMath::Sqrt(vertx[0]*vertx[0]+vertx[1]*vertx[1]);
        
        lRapLambda  = esdv0->RapLambda();
        V0pt = esdv0->Pt();
        if ((V0pt<fMinV0Pt)||(fMaxV0Pt<V0pt)) continue;
//         if(TMath::Abs(lRapLambda)> 0.5 ) continue;
        
        
        //--------------------------------------------------------------------Track selection-------------------------------------------------------------------------------------------------------------------------------------
        
        Float_t lPosTrackCrossedRows = esdpTrack->GetTPCClusterInfo(2,1);
        Float_t lNegTrackCrossedRows = esdnTrack->GetTPCClusterInfo(2,1);
        
        fTreeVariableLeastNbrCrossedRows = (Int_t) lPosTrackCrossedRows;
        if( lNegTrackCrossedRows < fTreeVariableLeastNbrCrossedRows )
            fTreeVariableLeastNbrCrossedRows = (Int_t) lNegTrackCrossedRows;
        
        if( !(esdpTrack->GetStatus() & AliESDtrack::kTPCrefit)) continue;
        if( !(esdnTrack->GetStatus() & AliESDtrack::kTPCrefit)) continue;
        
        if ( ( ( esdpTrack->GetTPCClusterInfo(2,1) ) < 70 ) || ( ( esdnTrack->GetTPCClusterInfo(2,1) ) < 70 ) ) continue;
        
        if( esdpTrack->GetKinkIndex(0)>0 || esdnTrack->GetKinkIndex(0)>0 ) continue;
        
        if( esdpTrack->GetTPCNclsF()<=0 || esdnTrack->GetTPCNclsF()<=0 ) continue;
        
        Float_t lPosTrackCrossedRowsOverFindable = lPosTrackCrossedRows / ((double)(esdpTrack->GetTPCNclsF()));
        Float_t lNegTrackCrossedRowsOverFindable = lNegTrackCrossedRows / ((double)(esdnTrack->GetTPCNclsF()));
        
        fTreeVariableLeastRatioCrossedRowsOverFindable = lPosTrackCrossedRowsOverFindable;
        if( lNegTrackCrossedRowsOverFindable < fTreeVariableLeastRatioCrossedRowsOverFindable )
            fTreeVariableLeastRatioCrossedRowsOverFindable = lNegTrackCrossedRowsOverFindable;
        
        if ( fTreeVariableLeastRatioCrossedRowsOverFindable < 0.8 ) continue;
        //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        
        pmom = esdv0->P();
        eta = esdv0->Eta();
        ppt = esdpTrack->Pt();
        peta = esdpTrack->Eta();
        npt = esdnTrack->Pt();
        neta = esdnTrack->Eta();
        ontheflystat = esdv0->GetOnFlyStatus();
        
        dcaPosToVertex = TMath::Abs(esdpTrack->GetD(lBestPrimaryVtxPos[0],
                                                    lBestPrimaryVtxPos[1],
                                                    lMagneticField) );
        
        dcaNegToVertex = TMath::Abs(esdnTrack->GetD(lBestPrimaryVtxPos[0],
                                                    lBestPrimaryVtxPos[1],
                                                    lMagneticField) );
        
        cosPointingAngle = esdv0->GetV0CosineOfPointingAngle(lBestPrimaryVtxPos[0],lBestPrimaryVtxPos[1],lBestPrimaryVtxPos[2]);
        dcaDaughters = esdv0->GetDcaV0Daughters();
        dcaV0ToVertex = esdv0->GetD(lBestPrimaryVtxPos[0],lBestPrimaryVtxPos[1],lBestPrimaryVtxPos[2]);
        
        esdv0->ChangeMassHypothesis(3122);
        invMassLambda = esdv0->GetEffMass();
        esdv0->ChangeMassHypothesis(-3122);
        invMassAntiLambda = esdv0->GetEffMass();
        
        posprnsg = fPIDResponse->NumberOfSigmasTPC(esdpTrack, AliPID::kProton);
        negprnsg = fPIDResponse->NumberOfSigmasTPC(esdnTrack, AliPID::kProton);
        pospion  = fPIDResponse->NumberOfSigmasTPC( esdpTrack, AliPID::kPion );
        negpion  = fPIDResponse->NumberOfSigmasTPC( esdnTrack, AliPID::kPion );
        
        if(TMath::Abs(peta) > 0.8) continue;
        if(TMath::Abs(neta) > 0.8) continue;
        if(cosPointingAngle < 0.99) continue;
        if(dcaDaughters > 0.8) continue;
        if(v0Radius < 5.0) continue;
        if(v0Radius > 200.) continue;
        if(TMath::Abs(eta) > 0.5) continue;
        
        
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Int_t iptbin = GetPtBin(V0pt);
        
        if( ontheflystat == 0 )
        {
            if(dcaV0ToVertex < 0.25 && dcaNegToVertex > 0.25 && dcaPosToVertex >  0.1  && TMath::Abs(posprnsg)  <= 3. && TMath::Abs(negpion)  <= 3.) //Default
            {
                
                f3fHistCentVsInvMassLambda1point0->Fill(fCentrality,invMassLambda,V0pt);
                if(invMassLambda > 1.11 && invMassLambda < 1.122)
                {
                    ptChEta1point0[iptbin] += 1;
                    f3fHistCentVsInvMassLambda1point0Masscut->Fill(fCentrality,invMassLambda,V0pt);
                }
                if(invMassLambda > 1.126 && invMassLambda < 1.138)
                {
                    f3fHistCentVsInvMassLambda1point0bkg->Fill(fCentrality,invMassLambda,V0pt);

                }
                
            }
            
            
            // Bar-L
            if(dcaV0ToVertex < 0.25 && dcaNegToVertex > 0.1 && dcaPosToVertex >  0.25 && TMath::Abs(negprnsg)  <= 3. && TMath::Abs(pospion)  <= 3.) //default
            {
                
                f3fHistCentVsInvMassAntiLambda1point0->Fill(fCentrality,invMassAntiLambda,V0pt);
                if(invMassAntiLambda > 1.11 && invMassAntiLambda < 1.122)
                {
                    ptChEta1point0[iptbin+fNptBins] += 1;
                    f3fHistCentVsInvMassAntiLambda1point0Masscut->Fill(fCentrality,invMassAntiLambda,V0pt);
                }
                if(invMassAntiLambda > 1.126 && invMassAntiLambda < 1.14)
                {
                    f3fHistCentVsInvMassAntiLambda1point0bkg->Fill(fCentrality,invMassAntiLambda,V0pt);

                }
            }
            
        }// zero onfly V0
    }// end of V0 loop
    ///////////////////
    
    Double_t ptContainer[dim+1];
    ptContainer[0] = (Double_t)fCentrality;
    for(Int_t i = 1; i <= dim; i++)
    {
        ptContainer[i] = ptChEta1point0[i-1];
    }
    fPtBinNplusNminusCh->Fill(ptContainer);
    
    
    
    PostData(1,fListHist);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Int_t AliAnalysisTaskNetLambdaTrad::GetPtBin(Double_t pt)
{
    Int_t bin = -1;
    
    Double_t LambdaPtBins[24] = {0.9,1.0,1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,4.2, 4.4};

    
    for(Int_t iBin = 0; iBin < fNptBins; iBin++)
    {
        
        if( iBin == fNptBins-1){
            if( pt >= LambdaPtBins[iBin] && pt <= LambdaPtBins[iBin+1]){
                bin = iBin;
                break;
            }
        }
        else{
            if( pt >= LambdaPtBins[iBin] && pt < LambdaPtBins[iBin+1]){
                bin = iBin;
                break;
                
            }
        }
    }
    
    return bin;
    
}


