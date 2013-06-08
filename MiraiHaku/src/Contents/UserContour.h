//
//  UserContour.h
//  emptyExample
//
//  Created by Akira Hayasaka on 10/3/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_UserContour_h
#define emptyExample_UserContour_h

#include "ofMain.h" 
#include "ofxFadable.h"
#include "SharedData.h"
#include "ofxFX.h"
#include "ofxCurvesTool.h"

static const float minDist = 0.6;
static const float maxDist = 1.5;
static const float minSimplfyAmt = 0.5;
static const float maxSimplfyAmt = 8.0;

class UserContour : public ofxFadableBase
{
public:
    
    void setup(SharedData* _sharedData)
    {
        sharedData = _sharedData;
        
		tgt.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, ofFbo::maxSamples());
		tmp.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, ofFbo::maxSamples());   
        
        glow.allocate(tmp.getWidth(), tmp.getHeight());
        
		string fragShaderSrc = 
		"#extension GL_ARB_texture_rectangle : enable\n"
		"uniform sampler2DRect mask;"
		"uniform sampler2DRect tex;"
		"void main()"
		"{"
		"	vec3 col = texture2DRect(tex, gl_TexCoord[0].st).rgb;"
		"	float alpha = texture2DRect(mask, gl_TexCoord[0].st).a;"
		"	gl_FragColor = vec4(col, alpha);"
		"}";
		mask.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
		mask.linkProgram();    
        
        init();
        setAlpha(0);
        setFadeSeconds(4);
        fadeIn();
        
        onsetTime = 0.0;
        curvesTool.setup();
        curvesTool.load("curves/curves.yml");    
        atChangeTime = 0.0;
        distAw = 0.0;
        prevAw = 0.0;
        currAw = 0.0; 
        attention = 0.0;
    }
    
    void update()
    {
        updateFade();

        float scale = checkOnset();
        float cur = ofGetElapsedTimef();
        float progress = cur - atChangeTime;
        progress = ofClamp(progress, 0.0, 1.0);
        if (progress > 0.9) prevAw = currAw;
        currAw = ofxTweenLite::tween(prevAw, distAw, progress, OF_EASE_SINE_INOUT);    
        float scaleFactor = currAw * currAw * currAw;
        
        scale *= scaleFactor;      


        // Full HD
//        float x_beforeScale = ofGetWidth()/2 - sharedData->kinectResource.kinect.getWidth()/2 - 100;
//        float y_beforeScale = ofGetHeight()/2 - sharedData->kinectResource.kinect.getHeight()/2 ;        
        // 1024, 768
        float x_beforeScale = ofGetWidth()/2;
        float y_beforeScale = ofGetHeight()/2 - 50;
        float x_afterScale = -sharedData->kinectResource.kinect.getWidth()/2;
        float y_afterScale = -sharedData->kinectResource.kinect.getHeight()/2 - 50;
        
        beginMask();
        ofPushStyle();
        ofSetColor(ofColor::cyan);

        ofPushMatrix();
        ofTranslate(x_beforeScale, y_beforeScale);
        ofScale(scale, scale);        
        ofTranslate(x_afterScale, y_afterScale);
        drawContour();
        ofPopMatrix();
        ofPopStyle();     
        endMask();
    }
    
    void draw()
    {
        drawEffect(sharedData->cameraResource.liveTex.getTextureReference());
    }
    
    void setAtChange(float param)
    {
        attention = param;
        distAw = ofMap(param, 0.0, 100.0, minDist, maxDist);
        atChangeTime = ofGetElapsedTimef();
    }
    
private:
    
	void beginMask()
	{
		tgt.begin();
		ofClear(0);
	}
    
	void endMask()
	{
		tgt.end();
	}    
    
    void drawContour()
    {
        lines.clear();
        float simpleAmt = ofMap(currAw, minDist, maxDist, minSimplfyAmt, maxSimplfyAmt);
        lines = sharedData->kinectResource.getContourPolyLine(simpleAmt);
        
//        bindSpectrum();
        
        for (int i = 0; i < lines.size(); i++)
        {
            vector<ofPoint> pts = lines.at(i).getVertices();
            ofBeginShape(); 
            
            for (int j = 0; j < pts.size(); j++)
            {
                if (j == 0)
                {
                    ofCurveVertex(pts[0].x, pts[0].y);
                    ofCurveVertex(pts[0].x, pts[0].y);
                } 
                else if (j == pts.size()-1)
                {
                    ofCurveVertex(pts[j].x, pts[j].y);
                    ofCurveVertex(pts[0].x, pts[0].y);
                    ofCurveVertex(pts[0].x, pts[0].y);
                }
                else 
                {
                    ofCurveVertex(pts[j].x, pts[j].y);
                }        
            }
            ofEndShape();  
        }
    }
    
    float checkOnset()
    {
//        if (sharedData->soundResource.onsetD.isOnsetting(ofSoundGetSpectrum(sharedData->soundResource.nBandsToGet)))
//        {
//            onsetTime = ofGetElapsedTimef();
//        }    
//        
//        float max = 1.3;
//        float min = 1.0;    
//        
//        if (onsetTime != 0.0)
//        {
//            float diff = ofGetElapsedTimef() - onsetTime;
//            int idx = ofMap(diff, 0.0, 0.4, 0, 256, true);
//            float val = ofMap(curvesTool[idx], 0, 256, min, max, true);
//            return val;
//        }
//        else
//        {
//            return min;
//        }
        
        return 1.0;
    }    
    
    void bindSpectrum()
    {
        float* fftSmoothed = sharedData->soundResource.getSpectrum();
        float multFactor = 5.0;        
        multFactor *= multFactor;
        
        for (int i = 0; i < lines.size(); i++)
        {
            int fftIdx = sharedData->soundResource.getBinMin();            
            
            vector<ofPoint> pts = lines.at(i).getVertices();
            for (int j = 0; j < pts.size(); j++)
            {                
                ofVec2f orig = lines.at(i).getBoundingBox().getCenter();
                ofVec2f dir = ofVec2f(orig - pts.at(j)).normalize();
                
                if (fftIdx > sharedData->soundResource.getBinMax())
                    fftIdx = sharedData->soundResource.getBinMin();
                
                pts.at(j) -= dir * (fftSmoothed[fftIdx] * multFactor);
                lines.at(i).clear();
                lines.at(i).addVertexes(pts); 
                lines.at(i).close();       
                    
                fftIdx++;
            }
        }
    }
    
	void drawEffect(ofTexture& maskedImage)
	{       
        tmp.begin();
        ofClear(0);
		mask.begin();
		mask.setUniformTexture("mask", tgt, 1);
		mask.setUniform1i("tex", 0);
		ofEnableAlphaBlending();
		maskedImage.draw(0, 0, ofGetWidth(), ofGetHeight());
		mask.end();
        tmp.end();
        
        glow.begin();
        ofPushStyle();
        ofSetColor(255, getAlphaInt());
        tmp.draw(0, 0);
        ofPopStyle();
        glow.end();
        glow.setRadius(sin(ofGetElapsedTimef()) * 95);
        glow.update(); 
        
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);        
        glow.draw();
        glow.draw();
        glow.draw();        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA); 
        
        ofSetColor(255, getAlphaInt() * 0.8);
        tmp.draw(0, 0);       
        ofPopStyle();  
	}     
    
    ofFbo tgt;
    ofFbo tmp;
    ofShader mask;
    
    SharedData* sharedData;
    
    vector<ofPolyline> lines;
    
    ofxGlow glow;    
    
    ofxCurvesTool curvesTool;  
    float attention;
    float onsetTime;    
    float atChangeTime;    
    float distAw;    
    float prevAw;
    float currAw;    
};

#endif
