//
//  MainInteraction.h
//  MiraiHaku
//
//  Created by Akira Hayasaka on 10/16/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef MiraiHaku_MainInteraction_h
#define MiraiHaku_MainInteraction_h

#include "ofMain.h"
#include "ofxFadable.h"
#include "ofxState.h"
#include "SharedData.h"

#include "MainMovie.h"
#include "UserContour.h"

static float mainInteractionFadeDur = 2.0;
static float scrabDur = 24.0;

using namespace Apex;

class MainInteraction : public ofxState<SharedData>, ofxFadableBase
{
public:
    
    string getName()
    {
        return "MainInteraction";
    }  
    
    void setup(int stateID)
    {
        mainMov.setup();
        contour.setup(&getSharedData());        
        getSharedData().bFinish2Phase = false;
        ofAddListener(getSharedData().attentionForMainInteractionEvent, this, &MainInteraction::attentionForMainInteractionListener);
        ofAddListener(mainMov.finish2PhaseEvent, this, &MainInteraction::finish2PhaseListener);      
        badSigMov.loadMovie("movs/Miraihaku_headset_1015_264.mov");
        badSigMov.stop();
    }    
    
    void update()
    {
        updateFade();
        
        mainMov.update();
        
        if (getSharedData().bFinish2Phase)
            contour.update();
        
        checkFinish1Phase();
        
        if (!getSharedData().bFinishMainInteraction &&
            mainMov.player.getCurrentFrame() >= mainMov.player.getTotalNumFrames()-1)
        {
            sharedData->bFinishMainInteraction = true;        
        }
        
        
        // bad sig mov
        if (!getSharedData().bGoodSigQuality && !bPlayingBadSigMov)
        {
            bPlayingBadSigMov = true;
            badSigMov.setFrame(0);
            badSigMov.play();
        }
        else if (getSharedData().bGoodSigQuality && bPlayingBadSigMov)
        {
            bPlayingBadSigMov = false;
            badSigMov.setFrame(0);
            badSigMov.stop();            
        }
        
         if (bPlayingBadSigMov)
             badSigMov.update();
        
        if (ofGetKeyPressed(OF_KEY_RIGHT))
            mainMov.finish1Phase();          
    }    
    
    void draw()
    {         
        ofPushStyle();
        ofSetColor(255, getAlphaInt());
        mainMov.draw();
        ofPopStyle();
        
        if (getSharedData().bFinish2Phase)
            contour.draw();    
        
        // bad sig mov
        if (bPlayingBadSigMov)
        {
            ofPushStyle();
            ofSetColor(255, 200);
            badSigMov.draw(0, 0, ofGetWidth(), ofGetHeight());
            ofPopStyle();
        }
    }                
    
    void stateEnter()
    {
        setAlpha(0.0);
        setFadeSeconds(mainInteractionFadeDur);
        fadeIn();
        
        mainMov.reset();
        getSharedData().beginTime = ofGetElapsedTimef();
        getSharedData().bFinish1Phase = false;
        mainMov.player.play();
    }
    
    void stateExit()
    {
        mainMov.player.stop(); 
        bPlayingBadSigMov = false;
        badSigMov.setFrame(0);
        badSigMov.stop();
    }    
    
private:
    
    void attentionForMainInteractionListener(float &param)
    {
        mainMov.setBrainValue(param);
        contour.setAtChange(param);          
    }
    
    void checkFinish1Phase()
    {
        float sec = ofGetElapsedTimef() - getSharedData().beginTime;
        if (sec > scrabDur && !getSharedData().bFinish1Phase)
        {
            mainMov.finish1Phase();
            mainMov.bFinish1Phase = true;
        }
    }

    void finish2PhaseListener(bool &param)
    {
        getSharedData().bFinish2Phase = true;
    }
    
    
    MainMovie mainMov; 
    UserContour contour;  
    ofxQTKitVideoPlayer badSigMov;
    bool bPlayingBadSigMov;
    
};

#endif
