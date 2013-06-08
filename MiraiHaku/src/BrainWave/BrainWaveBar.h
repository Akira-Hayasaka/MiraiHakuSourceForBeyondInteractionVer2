//
//  BrainWaveBar.h
//  emptyExample
//
//  Created by Akira Hayasaka on 9/4/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_BrainWaveBar_h
#define emptyExample_BrainWaveBar_h

#include "ofMain.h"
#include "ofxTweenLite.h"

class BrainWaveBar
{
public:
    
    BrainWaveBar()
    {
        attention = 0.0;
        meditation = 0.0;
        sigQuality = 0.0;
        
        distAw = 0.0;
        prevAw = 0.0;
        currAw = 0.0;
        distMw = 0.0;
        prevMw = 0.0;
        currMw = 0.0;
        
        atChangeTime = 0.0;
        meChangeTime = 0.0;        
    }
    
    void setup(float w, float h)
    {
        font.loadFont("font/DroidSansMono.ttf", 20);        
        width = w;
        height = h;
    }
    
    void update()
    {
        float cur = ofGetElapsedTimef();
        float progress = cur - atChangeTime;
        progress = ofClamp(progress, 0.0, 1.0);
        if (progress > 0.9) prevAw = currAw;
        currAw = ofxTweenLite::tween(prevAw, distAw, progress, OF_EASE_SINE_INOUT);
        
        progress = cur - meChangeTime;
        progress = ofClamp(progress, 0.0, 1.0);
        if (progress > 0.9) prevMw = currMw;
        currMw = ofxTweenLite::tween(prevMw, distMw, progress, OF_EASE_SINE_INOUT);           
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetColor(ofColor::black);
        string qStr = "";
        if (sigQuality == 0.0)
        {
            qStr = "good";
        }
        else 
        {
            qStr = "poor (" + ofToString(sigQuality) + ")";
        }
        font.drawString("signal quality = " + qStr, 10, 40);    
        ofPopStyle();
        
        
        ofPushMatrix();
        ofTranslate(0, 30);
        ofPushStyle();
        ofSetColor(ofColor::black);
        font.drawString("Attention", 10, height/4 - 10);
        ofNoFill();
        ofRect(1, height/4, width - 2, 60);
        ofSetColor(ofColor::magenta, ofMap(currAw, 0.0, width, 50, 255));
        ofFill();    
        ofRect(0, height/4, currAw, 59);
        if (attention > 0.0)
        {
            ofSetColor(ofColor::black, ofMap(currAw, 0.0, width, 50, 255));
            font.drawString(ofToString(attention), 10, height/4 + 40);
        }
        ofPopStyle();
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(0, -30);
        ofPushStyle();
        ofSetColor(ofColor::black);
        font.drawString("Meditation", 10, height-(height/4) - 10); 
        ofNoFill();
        ofRect(1, height-(height/4), width - 2, 60);    
        ofSetColor(ofColor::cyan, ofMap(currMw, 0.0, width, 50, 255));
        ofFill();
        ofRect(0, height-(height/4), currMw, 59);
        if (meditation > 0.0)
        {
            ofSetColor(ofColor::black, ofMap(currMw, 0.0, width, 50, 255));
            font.drawString(ofToString(meditation), 10, height-(height/4) + 40);
        }    
        ofPopStyle();
        ofPopMatrix();        
    }
    
    void setAttention(float _attention)
    {
        attention = _attention;
        distAw = ofMap(attention, 0.0, 100.0, 0, width);
        atChangeTime = ofGetElapsedTimef();
    }
    
    void setMeditation(float _meditation)
    {
        meditation = _meditation;
        distMw = ofMap(meditation, 0.0, 100.0, 0, width);
        meChangeTime = ofGetElapsedTimef();           
    }
    
    void setSigQuality(float _sigQuality)
    {
        sigQuality = _sigQuality;
    }
    
private:
    
    ofTrueTypeFont font;    
    
    float width;
    float height;
    
    float attention;
    float meditation;
    float sigQuality;
    
    float atChangeTime;
    float meChangeTime;
    float distAw;    
    float prevAw;
    float currAw;
    float distMw;
    float prevMw;
    float currMw;    
};

#endif
