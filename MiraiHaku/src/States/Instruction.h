//
//  Instruction.h
//  MiraiHaku
//
//  Created by Akira Hayasaka on 10/16/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef MiraiHaku_Instruction_h
#define MiraiHaku_Instruction_h

#include "ofMain.h"
#include "ofxState.h"
#include "ofxFadable.h"
#include "ofxQTKitVideoPlayer.h"
#include "SharedData.h"

static const float instrctionFadeDur = 2.0;

using namespace Apex;

class Instruction : public ofxState<SharedData>, ofxFadableBase
{
public:
    
    string getName()
    {
        return "Instruction";
    }  
    
    void setup(int stateID)
    {
        player.loadMovie("movs/Miraihaku_screensaver_1019.mov");
        player.setFrame(0);        
        player.setLoopState(OF_LOOP_NONE);        
        player.stop();
    }    
    
    void update()
    {
        updateFade();
        player.update();
        
        if (!getSharedData().bFinishInstruction &&
//            player.getCurrentFrame() >= player.getTotalNumFrames()-1)
            player.getIsMovieDone())
            sharedData->bFinishInstruction = true;
    }    
    
    void draw()
    {         
        ofPushStyle();
        ofSetColor(255, getAlphaInt());
        player.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
    }                
    
    void stateEnter()
    {
        setAlpha(0.0);
        setFadeSeconds(instrctionFadeDur);
        fadeIn();
        
        player.setFrame(0);
        player.play();
    }
    
    void stateExit()
    {
        player.setFrame(0);        
        player.stop();
    }    
    
private:
    
    ofxQTKitVideoPlayer player;
};

#endif
