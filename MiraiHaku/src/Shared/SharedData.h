//
//  SharedData.h
//  emptyExample
//
//  Created by Akira Hayasaka on 9/29/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_SharedData_h
#define emptyExample_SharedData_h

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "ofxThinkGear.h"
#include "ThinkGearEmu.h"
#include "KinectResource.h"
#include "CameraResource.h"
#include "SoundResource.h"
#include "Parameter.h"

#define USE_EMU

using namespace Apex;

class SharedData
{
    
public:
    
    void setup()
    {
        thinkGear.setup();        
        kinectResource.setup();
        cameraResource.setup();
        soundResource.setup();
        brainParam.setup();   
        
#ifdef USE_EMU           
        bGoodSigQuality = true;
#else
        bGoodSigQuality = false;
#endif
        
        ofAddListener(thinkGear.attentionChangeEvent, this, &SharedData::attentionListener);        
    }
    
    void reset()
    {
        brainParam.reset();

#ifdef USE_EMU           
        bGoodSigQuality = true;
#else
        bGoodSigQuality = false;
#endif
    }
    
    void update()
    {
        thinkGear.update();        
        kinectResource.update();
        cameraResource.update();
        soundResource.update();
        
#ifdef USE_EMU          
        if (ofGetKeyPressed('g'))
            bGoodSigQuality = true;
        else if (ofGetKeyPressed('b'))
            bGoodSigQuality = false;
#else
        if (thinkGear.getSignalQuality() == 0)
            bGoodSigQuality = true;
        else
            bGoodSigQuality = false;        
#endif
        
        brainParam.setSigQuality(thinkGear.getSignalQuality());
        brainParam.update();        
    }
    
    void drawBrainParam()
    {
        brainParam.draw();   
        if (stateMachine->getCurrentStateName() == "MainInteraction")
            brainParam.beginTimeIncre();
    }
    
    ofxStateMachine<SharedData>* stateMachine;    
    
#ifdef USE_EMU    
    ThinkGearEmu thinkGear;    
#else
    ofxThinkGear thinkGear;
#endif    
    
    bool bGoodSigQuality;
    
    KinectResource kinectResource;
    CameraResource cameraResource;
    SoundResource soundResource;
    Parameter brainParam;    
    
    ofEvent<float> attentionForMainInteractionEvent;
    
    void attentionListener(float &param)
    {
        brainParam.setAttention(param);        
        ofNotifyEvent(attentionForMainInteractionEvent, param);     
    }
        
    
    // state specific fields
    // Instruction
    bool bFinishInstruction;
    void resetInstruction()
    {
        bFinishInstruction = false;
    }
    
    // check brain wave
    bool bFinishCheckBrainWave;
    void resetCheckBrainWave()
    {
        bFinishCheckBrainWave = false;
    }
    
    // MainInteraction
    float beginTime;
    bool bFinish1Phase;
    bool bFinish2Phase;
    bool bFinishMainInteraction;
    void resetMainInteraction()
    {
        beginTime = 0.0;
        bFinish1Phase = false;
        bFinish2Phase = false;
        bFinishMainInteraction = false;
    }

};

#endif
