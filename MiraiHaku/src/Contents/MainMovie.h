//
//  MainMovie.h
//  emptyExample
//
//  Created by Akira Hayasaka on 9/4/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_MainMovie_h
#define emptyExample_MainMovie_h

#include "ofMain.h"
#include "ofxQTKitVideoPlayer.h"
#include "ofxTweenLite.h"

static float scrabMinPct = 0.0465;
static float scrabMaxPct = 0.0752075;
static float contourBeginSec = 14.0;

class MainMovie
{
public:
    
    MainMovie()
    {
        changeTime = 0.0;
        destPct = scrabMinPct;
        prevPct = scrabMinPct;
        currPct = scrabMinPct;
        bFinish1Phase = false;
    }
    
    void setup()
    {
        player.loadMovie("movs/Miraihaku_sample1019.mov");
        player.setPosition(destPct);   
        player.setLoopState(OF_LOOP_NONE);
        player.stop();
        reset();
    }
    
    void reset()
    {
        changeTime = 0.0;
        destPct = scrabMinPct;
        prevPct = scrabMinPct;
        currPct = scrabMinPct;
        bFinish1Phase = false;        
        bSendFinish2PhaseEvent = false;  
        player.setFrame(0);  
        bFinish0Phase = false;
    }
    
    void update()
    {
        if (player.getPosition() >= scrabMinPct && !bFinish0Phase)
        {
            bFinish0Phase = true;
            player.setPosition(scrabMinPct);
            player.stop();
        }
            
        if (!bFinish1Phase && bFinish0Phase)
        {
            float cur = ofGetElapsedTimef();
            float progress = cur - changeTime;
            progress = ofClamp(progress, 0.0, 1.0);
            if (progress > 0.95) 
            {
                prevPct = currPct;
                progress = 1.0;
            }
            currPct = ofxTweenLite::tween(prevPct, destPct, progress, OF_EASE_LINEAR_INOUT);        
            player.setPosition(currPct);    
        }   
        
        if (player.getCurrentFrame() == player.getTotalNumFrames()-1)
            player.stop();        
        
        player.update();                           
        
        if (player.getPositionInSeconds() > contourBeginSec &&
            !bSendFinish2PhaseEvent)
        {
            bSendFinish2PhaseEvent = true;
            bool fake = true;
            ofNotifyEvent(finish2PhaseEvent, fake);
        }
    }
    
    void draw()
    {
        player.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    void setBrainValue(float _val)
    {
        float min = 0;
        float max = 100;
        float clamped = ofClamp(_val, min, max);
        destPct = ofMap(clamped, min, max, scrabMinPct, scrabMaxPct, true);
        changeTime = ofGetElapsedTimef();
    }
    
    void finish1Phase()
    {
        if (!bFinish1Phase)
            player.play();
        
        bFinish1Phase = true;
    }
    
    float getPositionInSeconds()
    {
        return player.getPositionInSeconds();
    }
    
    ofEvent<bool> finish2PhaseEvent;
    bool bSendFinish2PhaseEvent;
    
    ofxQTKitVideoPlayer player;
    float changeTime;
    float destPct;
    float prevPct;
    float currPct;
    
    bool bFinish0Phase;
    bool bFinish1Phase;
};

#endif
