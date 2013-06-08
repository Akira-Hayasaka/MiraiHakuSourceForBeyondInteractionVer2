//
//  WorkFlow.h
//  MiraiHaku
//
//  Created by Akira Hayasaka on 10/16/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef MiraiHaku_WorkFlow_h
#define MiraiHaku_WorkFlow_h

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "SharedData.h"

using namespace Apex;

class WorkFlow 
{
public:
    
    void setup(ofxStateMachine<SharedData>* stateMachine, SharedData* sharedData)
    {
        this->stateMachine = stateMachine;
        this->sharedData = sharedData;
        
        sharedData->resetInstruction();
        sharedData->resetCheckBrainWave();        
        sharedData->resetMainInteraction();
        
        prevStateName = "";
    }
    
    void update()
    {
        progressFlow();
    }
    
private:
    
    void progressFlow()
    {   
        // Instruction
        if (stateMachine->getCurrentStateName() == "Instruction")
        {
            if (sharedData->bFinishInstruction)
            {
                changeState("CheckBrainWave");
                sharedData->resetInstruction();
            }
        }
        
        // Check brain wave
        else if (stateMachine->getCurrentStateName() == "CheckBrainWave")
        {
            if (sharedData->bFinishCheckBrainWave)
            {
                if (sharedData->bGoodSigQuality)
                {
                    changeState("MainInteraction");
                    sharedData->resetCheckBrainWave();
                }
                else
                {
                    changeState("Instruction");
                    sharedData->resetCheckBrainWave();                    
                }
            }
        }
        
        // main interaction
        else if (stateMachine->getCurrentStateName() == "MainInteraction")
        {
            if (sharedData->bFinishMainInteraction)
            {
                changeState("Instruction");
                sharedData->resetMainInteraction();
                sharedData->reset();                
            }
        }    
    }
    
    void changeState(string stateName)
    {
        prevStateName = stateMachine->getCurrentStateName();
        stateMachine->changeState(stateName);
    }
    
    string prevStateName;
    ofxStateMachine<SharedData>* stateMachine;    
    SharedData* sharedData;     
};

#endif
