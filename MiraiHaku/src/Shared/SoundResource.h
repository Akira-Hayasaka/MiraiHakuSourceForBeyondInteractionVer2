//
//  SoundResource.h
//  emptyExample
//
//  Created by Akira Hayasaka on 10/4/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_SoundResource_h
#define emptyExample_SoundResource_h

#include "ofMain.h"
#include "ofxOnsetDetection.h"

class SoundResource
{
public:
    
    void setup()
    {
        sound.loadSound("sounds/background_ambient_beat.mp3");
        sound.setLoop(true);
        sound.play();
        
        nBandsToGet = 128*8; 
        
        int smoothNum = 8192;
        fftSmoothed = new float[smoothNum];
        for (int i = 0; i < smoothNum; i++)
        {
            fftSmoothed[i] = 0;
        }
        
        binMin = 0;
        binMax = 64;        
        
        onsetD.setup();          
    }
    
    void update()
    {
        float* val = ofSoundGetSpectrum(nBandsToGet);
        for (int i = 0;i < nBandsToGet; i++)
        {
            fftSmoothed[i] *= 0.96f;
            if (fftSmoothed[i] < val[i]) 
                fftSmoothed[i] = val[i];	
        }    
    }
    
    float* getSpectrum()
    {
        return fftSmoothed;
    }
    
    int getBinMin()
    {
        return binMin;
    }
    
    int getBinMax()
    {
        return binMax;
    }
        
    ofSoundPlayer sound;
    float* fftSmoothed;
    int nBandsToGet;
    ofxOnsetDetection onsetD;    
    int binMin;
    int binMax;
    
    
};

#endif
