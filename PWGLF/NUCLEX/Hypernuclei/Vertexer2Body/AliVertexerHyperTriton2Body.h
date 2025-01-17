#ifndef AliVertexerHyperTriton2Body_H
#define AliVertexerHyperTriton2Body_H


#include <TNamed.h>


#include <vector>
#include <AliESDtrackCuts.h>
#include <AliESDv0.h>

class AliPIDResponse;

class AliVertexerHyperTriton2Body : public TNamed {
public:
    AliVertexerHyperTriton2Body();
    virtual ~AliVertexerHyperTriton2Body() {}

    void SetResetInitialPositions( Bool_t lOpt = kTRUE ){
        //Highly experimental, use with care!
        fkResetInitialPositions = lOpt;
    }
    void SetDoImprovedDCAV0DauPropagation( Bool_t lOpt = kTRUE ){
        //Highly experimental, use with care!
        fkDoImprovedDCAV0DauPropagation = lOpt;
    }
    void SetDoMaterialCorrections( Bool_t lOpt = kTRUE ){
        //Highly experimental, use with care!
        fkDoMaterialCorrection = lOpt;
    }
    void SetXYCase1Preoptimization( Bool_t lOpt = kTRUE ){
        //Highly experimental, use with care!
        fkXYCase1 = lOpt;
    }
    void SetXYCase2Preoptimization( Bool_t lOpt = kTRUE ){
        //Highly experimental, use with care!
        fkXYCase2 = lOpt;
    }
    
    void SetDoV0Refit ( Bool_t lDoV0Refit = kTRUE) {
        fkDoV0Refit = lDoV0Refit;
    }
    
//---------------------------------------------------------------------------------------
//Setters for the V0 Vertexer Parameters
    void SetV0VertexerMaxChisquare   ( Double_t lParameter ) {
        fV0VertexerSels[0] = lParameter;
    }
    void SetV0VertexerDCAFirstToPV   ( Double_t lParameter ) {
        fV0VertexerSels[1] = lParameter;
    }
    void SetV0VertexerDCASecondtoPV  ( Double_t lParameter ) {
        fV0VertexerSels[2] = lParameter;
    }
    void SetV0VertexerDCAV0Daughters ( Double_t lParameter ) {
        fV0VertexerSels[3] = lParameter;
    }
    void SetV0VertexerCosinePA       ( Double_t lParameter ) {
        fV0VertexerSels[4] = lParameter;
    }
    void SetV0VertexerMinRadius      ( Double_t lParameter ) {
        fV0VertexerSels[5] = lParameter;
    }
    void SetV0VertexerMaxRadius      ( Double_t lParameter ) {
        fV0VertexerSels[6] = lParameter;
    }
//---------------------------------------------------------------------------------------
    void SetMinPtV0     ( Float_t lMinPt ) {
        fMinPtV0 = lMinPt;
    }
    void SetMaxPtV0     ( Float_t lMaxPt ) {
        fMaxPtV0 = lMaxPt;
    }
    void SetUseMonteCarloAssociation( Bool_t lOpt = kTRUE) {
        fkMonteCarlo=lOpt;
    }
//---------------------------------------------------------------------------------------
    void SetUseImprovedFinding(){
        fkDoImprovedDCAV0DauPropagation = kTRUE;
        fkDoV0Refit = kTRUE;
        fkXYCase1 = kTRUE;
        fkXYCase2 = kTRUE;
    }
//---------------------------------------------------------------------------------------
    void SetUseDefaultFinding(){
        fkDoImprovedDCAV0DauPropagation = kFALSE;
        fkDoV0Refit = kFALSE;
        fkXYCase1 = kFALSE;
        fkXYCase2 = kFALSE;
    }
//---------------------------------------------------------------------------------------
    //Functions for analysis Bookkeepinp
    // 1- Configure standard vertexing
    void SetupStandardVertexing();
    void SetupLooseVertexing();
//---------------------------------------------------------------------------------------
    //Re-vertex V0s
    std::vector<AliESDv0> Tracks2V0vertices(AliESDEvent *event,AliPIDResponse* pid);

    //Helper functions
    Double_t Det(Double_t a00, Double_t a01, Double_t a10, Double_t a11) const;
    Double_t Det(Double_t a00,Double_t a01,Double_t a02,
                 Double_t a10,Double_t a11,Double_t a12,
                 Double_t a20,Double_t a21,Double_t a22) const;
    void Evaluate(const Double_t *h, Double_t t,
                  Double_t r[3],  //radius vector
                  Double_t g[3],  //first defivatives
                  Double_t gg[3]); //second derivatives
    void CheckChargeV0(AliESDv0 *v0);
    //---------------------------------------------------------------------------------------
    //Improved DCA V0 Dau
    Double_t GetDCAV0Dau ( AliExternalTrackParam *pt, AliExternalTrackParam *nt, Double_t &xp, Double_t &xn, Double_t b, Double_t lNegMassForTracking=0.139, Double_t lPosMassForTracking=0.139);
    void GetHelixCenter(const AliExternalTrackParam *track,Double_t center[2], Double_t b);
    //---------------------------------------------------------------------------------------

    AliESDtrackCuts* fHe3Cuts;  //->
    AliESDtrackCuts* fPiCuts;   //->
private:

    Bool_t fkDoV0Refit;
    int fMaxIterationsWhenMinimizing;
    bool fkPreselectX;
    Bool_t fkXYCase1; //Circles-far-away case pre-optimization switch
    Bool_t fkXYCase2; //Circles-touch case pre-optimization switch (cowboy/sailor duality resolution)
    Bool_t fkResetInitialPositions; 
    Bool_t fkDoImprovedDCAV0DauPropagation;
    Bool_t fkDoMaterialCorrection; //Replace AliExternalTrackParam::PropagateTo with AliTrackerBase::PropagateTrackTo
    
    //Master MC switch
    Bool_t fkMonteCarlo; //do MC association in vertexing
    
    Float_t fMinPtV0; //minimum pt above which we keep candidates in TTree output
    Float_t fMaxPtV0; //maximum pt below which we keep candidates in TTree output

    Double_t fMinXforXYtest; //min X allowed for XY-plane preopt test
    
    Double_t  fV0VertexerSels[7];        // Array to store the 7 values for the different selections V0 related
    
    
    
    AliVertexerHyperTriton2Body(const AliVertexerHyperTriton2Body&);            // not implemented
    AliVertexerHyperTriton2Body& operator=(const AliVertexerHyperTriton2Body&); // not implemented

    ClassDef(AliVertexerHyperTriton2Body, 1);
    //1: first implementation
};

#endif
