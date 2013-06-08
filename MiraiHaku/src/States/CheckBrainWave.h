//
//  CheckBrainWave.h
//  MiraiHaku
//
//  Created by Akira Hayasaka on 10/16/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef MiraiHaku_CheckBrainWave_h
#define MiraiHaku_CheckBrainWave_h

#include "ofMain.h"
#include "ofxState.h"
#include "ofxFadable.h"
#include "ofxQTKitVideoPlayer.h"
#include "ofxTrueTypeFontUC.h"
#include "SharedData.h"

static const float checkBrainWaveFadeDur = 2.0;
static const float checkBrainWaveSec = 15.0;

using namespace Apex;

class CheckBrainWave : public ofxState<SharedData>, ofxFadableBase
{
public:
    
    string getName()
    {
        return "CheckBrainWave";
    }  
    
    void setup(int stateID)
    {
        player.loadMovie("movs/Miraihaku_headset_1015_264.mov");
        brain.loadImage("imgs/IC_brain_grafic1016_b.png");
        brain.resize(ofGetWidth(), ofGetHeight());
        font.loadFont("font/MSPGothic.ttf", 20);
        beginTime = 0.0;
    }
    
    void update()
    {
        updateFade();
        
        player.update();
        
        if (!getSharedData().bFinishCheckBrainWave &&
            ofGetElapsedTimef() - beginTime > checkBrainWaveSec)
            sharedData->bFinishCheckBrainWave = true;        
    }
    
    void draw()
    {         
        ofPushStyle();      
        ofSetColor(255, getAlphaInt());
        player.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
        
        int maxBrainAlpha = 50;        
        int brainFlashed = maxBrainAlpha * 0.7;
        if (getSharedData().bGoodSigQuality)
            brainFlashed = powf(1 - (sin(ofGetElapsedTimef()*7.0)*0.5+0.5),2.0) * maxBrainAlpha;        
        
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);          
        ofSetColor(255, maxBrainAlpha - brainFlashed);
        brain.draw(0, 0);        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);                
        ofPopStyle();          
        
        int maxFontAlpha = 255;
        int fontFlashed = maxFontAlpha * 0.7;
        if (!getSharedData().bGoodSigQuality)
            fontFlashed = powf(1 - (sin(ofGetElapsedTimef()*5.0)*0.5+0.5),2.0) * maxFontAlpha;
        int fontXOffset = 90;
        int fontYOffset = 154;
        
        if (!getSharedData().bGoodSigQuality)
        {
            wstring wstr = L"脳波検知中・・・";
            ofRectangle bbox = font.getStringBoundingBox(wstr, 0, 0);
            bbox.x = ofGetWidth()/2 - bbox.width + 90;
            bbox.y = ofGetHeight()/2 + fontYOffset;
            ofPushStyle();
            ofSetColor(44, 205, 28, maxFontAlpha - fontFlashed);
            font.drawString(wstr, bbox.x, bbox.y);
            ofPopStyle();            
        }
        else
        {
            wstring wstr = L"検知完了";
            ofRectangle bbox = font.getStringBoundingBox(wstr, 0, 0);
            bbox.x = ofGetWidth()/2 - bbox.width + 50;
            bbox.y = ofGetHeight()/2 + fontYOffset;
            ofPushStyle();
            ofSetColor(0, 227, 252, maxFontAlpha);
            font.drawString(wstr, bbox.x, bbox.y);
            ofPopStyle();              
        }
    }                  
    
    void stateEnter()
    {
        setAlpha(0.0);
        setFadeSeconds(checkBrainWaveFadeDur);
        fadeIn();
        
        player.setFrame(0);
        player.play();
        beginTime = ofGetElapsedTimef();
    }
    
    void stateExit()
    {
        player.stop();
    }    
    
private:
    
    ofxQTKitVideoPlayer player;
    ofImage brain;    
    ofxTrueTypeFontUC font;
    
    float beginTime;
};


#endif
