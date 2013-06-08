//
//  ThinkGearEmu.h
//  emptyExample
//
//  Created by Akira Hayasaka on 9/4/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_ThinkGearEmu_h
#define emptyExample_ThinkGearEmu_h

#include "ofMain.h"

class ThinkGearEmu
{
public:
    
    ThinkGearEmu()
    {
        startTime = 0.0;
        prevTime = 0.0;
        currentTime = 0.0;
        attention = 50.0;
        meditation = 50.0;
    }
    
    void setup()
    {
        startTime = ofGetElapsedTimef();
    }
    
    void update()
    {
        currentTime = floor(ofGetElapsedTimef() - startTime);
        
        float diff = currentTime - prevTime;
        
        if (diff > 0.5)
        {
            float ainc = ofRandom(-20, 20);
            if (attention + ainc > 100.0 ||
                attention + ainc < 0.0)
            {
                ainc *= -1;                
            }
            attention += ainc;
            attention = floor(attention);
            
//            attention = ofMap(ofGetMouseX(), 0, ofGetScreenWidth(), 0, 100, true);
            
            ofNotifyEvent(attentionChangeEvent, attention);
            
            float minc = ofRandom(-20, 20);
            if (meditation + minc > 100.0 ||
                meditation + minc < 0.0)
            {
                minc *= -1;
            }
            meditation += minc;
            meditation = floor(meditation);
            ofNotifyEvent(meditationChangeEvent, meditation);
            
            prevTime = currentTime;
        }
    }
    
    float getSignalQuality()
    {
        return 0.0;
    }   
    
    float getAttention()
    {
        return attention;
    }
    
    float getMeditation()
    {
        return meditation;
    }
    
    ofEvent<float> attentionChangeEvent;
    ofEvent<float> meditationChangeEvent;    
    
private:
    
    float startTime;
    float prevTime;
    float currentTime;
    float attention;
    float meditation;
    
};

#endif
