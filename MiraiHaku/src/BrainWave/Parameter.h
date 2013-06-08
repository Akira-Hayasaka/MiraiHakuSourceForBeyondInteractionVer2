//
//  Parameter.h
//  emptyExample
//
//  Created by Akira Hayasaka on 10/1/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_Parameter_h
#define emptyExample_Parameter_h

#include "ofMain.h"
#include "ofxTweenLite.h"
#include "ofxFX.h"

class Parameter
{
public:
    
    Parameter()
    {
        reset();        
        
        barWidth = 380;
        
        paramCol = ofColor(53, 220, 221, 150);
        origCol = paramCol;
    }
    
    void reset()
    {
        sigQuality = 0.0;        
        attention = 0.0;
        
        atChangeTime = 0.0;
        distAw = 0.0;
        prevAw = 0.0;
        currAw = 0.0;
        
        barWidth = 380;
        
        timeStr = "0:00";      
        bDrawTime = false;
        beginTime = 0.0;
        
        paramCol = origCol;
    }
    
    void setup()
    {
        cntFont.loadFont("font/AyatoNumC.ttf", 25);
        cntFont.setLetterSpacing(1.5);
        
        brain.loadImage("paramImgs/brain.png");
        brainGlow.loadImage("paramImgs/glowBG.png");
        grid.loadImage("paramImgs/grid.png");
        
        glowOffset = ofPoint(20, 10);        
        glow.allocate(grid.getWidth(), grid.getHeight());
        
        float rad = 70;
        float wid = 3;
        circle.setArcResolution(60);
        circle.arc(ofPoint(0, 0), rad, rad, 0, 360);
        circle.setColor(ofColor(32, 82, 138, 180));
        circle.close();
        circle.arc(ofPoint(0, 0), rad - wid, rad - wid, 0, 360);
        circle.setColor(ofColor(32, 100, 100, 180));        
    }
    
    void update()
    {
        float cur = ofGetElapsedTimef();
        float progress = cur - atChangeTime;
        progress = ofClamp(progress, 0.0, 1.0);
        if (progress > 0.9) prevAw = currAw;
        currAw = ofxTweenLite::tween(prevAw, distAw, progress, OF_EASE_SINE_INOUT);
     
        if (bDrawTime)
        {        
            int min = (ofGetElapsedTimef() - beginTime) / 60;
            int sec = (ofGetElapsedTimef() - beginTime) - (min * 60);
            string secStr = ofToString(sec);
            if (sec < 10)
                secStr = "0" + secStr;
            timeStr = ofToString(min) + ":" + secStr;
        }
        
        int red = ofMap(currAw, 0, 255, origCol.r, ofColor::red.r, true);
        int green = ofMap(currAw, 0, 255, origCol.g, ofColor::red.g, true);        
        int blue = ofMap(currAw, 0, 255, origCol.b, ofColor::red.b, true);                
        paramCol = ofColor(red, green, blue, 255);
        
        updateGlowComponent(); 
    }
    
    void draw()
    {
        ofPushMatrix();
        ofTranslate(glowOffset);
        
        grid.draw(0, 0);
        
        brainGlow.draw(0, 32);
        brain.draw(brainGlow.getWidth()/2 - brain.getWidth()/2, 
                   brainGlow.getHeight()/2 - brain.getHeight()/2 + 32);
        
        drawGlowComponent();
        
        ofPopMatrix();     
    }
    
    void beginTimeIncre()
    {
        if (!bDrawTime)
            beginTime = ofGetElapsedTimef();
        bDrawTime = true;
    }
    
    void setAttention(float _attention)
    {
        attention = _attention;
        distAw = ofMap(attention, 0.0, 100.0, 0, barWidth);
        atChangeTime = ofGetElapsedTimef();
    }    
    
    void setSigQuality(float _sigQuality)
    {
        sigQuality = _sigQuality;
    }    
    
    
private:
    
    void updateGlowComponent()
    {
        glow.begin();
        drawBar(); 
        
        if (bDrawTime)
        {
            ofPushStyle();
            ofColor fontCol = origCol;
            fontCol.a = 255;
            ofSetColor(fontCol);
            cntFont.drawString(timeStr, 220, 140);
            ofPopStyle();    
        }
        
        ofPushMatrix();
        ofTranslate(110, 130);
        ofRotateY(sin((ofGetElapsedTimef() + 10) * 0.3) * 360);  
        ofRotateX(cos((ofGetElapsedTimef() + 10) * 0.4) * 360);
        circle.draw(0, 0);        
        ofPopMatrix();

        ofPushMatrix();
        ofTranslate(110, 130);
        ofRotateX(sin(ofGetElapsedTimef() * 0.3) * 360);  
        ofRotateY(cos(ofGetElapsedTimef() * 0.2) * 360);
        circle.draw(0, 0);        
        ofPopMatrix();        
        
        glow.end();
        glow.setRadius(sin(ofGetElapsedTimef()) * 35);
        glow.update();          
    }
    
    void drawGlowComponent()
    {
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        glow.draw();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPopStyle();        
    }
    
    void drawBar()
    {
        ofPoint orig(100, 175);
        float w = currAw;
        float h = 20;    
        ofColor start(0, 0, 0, 0);
        ofColor end(paramCol);
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);        
        
		mesh.addVertex(ofVec2f(orig.x, orig.y));
		mesh.addVertex(ofVec2f(orig.x + w, orig.y));
		mesh.addVertex(ofVec2f(orig.x + w, orig.y + h));
		mesh.addVertex(ofVec2f(orig.x, orig.y + h));
		mesh.addColor(start);        
		mesh.addColor(end);
		mesh.addColor(end);        
		mesh.addColor(start);
        
        glDepthMask(false);
        mesh.draw();
        glDepthMask(true);        
    }
    
    ofxGlow glow;
    ofPoint glowOffset;
    
    ofColor paramCol;
    ofColor origCol;
    
    ofTrueTypeFont cntFont;    
    ofImage brain;
    ofImage brainGlow;
    ofImage grid;
    
    bool bDrawTime;
    float beginTime;
    string timeStr;
    
    float barWidth;
    
    float sigQuality;
    float attention;
    
    float atChangeTime;
    float distAw;    
    float prevAw;
    float currAw;    
    
    ofPath circle;
    
};

#endif
