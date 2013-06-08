//
//  KinectResource.h
//  emptyExample
//
//  Created by Akira Hayasaka on 9/29/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_KinectResource_h
#define emptyExample_KinectResource_h

#define USE_PALYER

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "DepthHoleFiller.h"

#ifndef USE_PALYER
#include "ofxKinect.h"
#else
#include "ofxKinectPlayer.h"
#endif

const static int kinectWidth = 640;
const static int kinectHeight = 480;

class KinectResource
{
public:
    
    void setup()
    {
#ifndef USE_PALYER        
        kinect.setRegistration(false);
//        kinect.init();
        kinect.init(false, false); // disable video image (faster fps)
        kinect.open();      
#else
        kinect.setup("kinectMov/1348887249-1.dat", true);
#endif       
        
        DHF.setDimensions(kinectWidth, kinectHeight);
        DHF.setDepthHistory(1);         
        
        grayImage.allocate(kinectWidth, kinectHeight);
        grayThreshNear.allocate(kinectWidth, kinectHeight);
        grayThreshFar.allocate(kinectWidth, kinectHeight);         

        bThreshWithOpenCV = true;  
        
        nearThreshold = 230;
        farThreshold = 70;
        
        FILL_USING_HISTORY = false;
        FILL_USING_CLOSING = false;
        FILL_USING_CONTOURS = false;
        N_HISTORY = 2;
        N_CLOSING_PASSES = 1;
        MAX_HOLE_AREA = 400;
        MAX_N_CONTOURS_TO_FILL = 100;        
    }
    
    void update()
    {
        updateKinect();
    }
    
    void updateKinect()
    {
        kinect.update();
        
        if(kinect.isFrameNew()) 
        {    
            grayImage.setFromPixels(kinect.getDepthPixels(), kinectWidth, kinectHeight);
            
            if(bThreshWithOpenCV) 
            {
                grayThreshNear = grayImage;
                grayThreshFar = grayImage;
                grayThreshNear.threshold(nearThreshold, true);
                grayThreshFar.threshold(farThreshold);
                cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), 
                      grayImage.getCvImage(), NULL);
            }
            else 
            {
                unsigned char * pix = grayImage.getPixels();
                
                int numPixels = grayImage.getWidth() * grayImage.getHeight();
                
                for(int i = 0; i < numPixels; i++) 
                {
                    if(pix[i] < nearThreshold && pix[i] > farThreshold) 
                    {
                        pix[i] = 255;
                    } 
                    else 
                    {
                        pix[i] = 0;
                    }
                }
            }
            
            
            if (FILL_USING_HISTORY)
            {
                int depthHistory = N_HISTORY;
                DHF.setDepthHistory(depthHistory); 
                DHF.updatePreProcessingDepthHistory(grayImage);
                DHF.fillHolesUsingHistory(grayImage); 
            }
            
            if (FILL_USING_CLOSING)
            {
                int nClosingPasses = N_CLOSING_PASSES;
                DHF.performMorphologicalClose(grayImage, nClosingPasses);
            }
            
            if (FILL_USING_CONTOURS)
            {
                int maxContourHoleToFillArea = MAX_HOLE_AREA;
                int maxNContoursToFill = MAX_N_CONTOURS_TO_FILL;
                DHF.fillHolesUsingContourFinder(grayImage, maxContourHoleToFillArea, maxNContoursToFill);
            }            
            
            
            grayImage.flagImageChanged();
            contourFinder.findContours(grayImage, 
                                       1000, (kinectWidth*kinectHeight)/2, 20, 
                                       false);
        }         
    }
    
    vector<ofPolyline> getContourPolyLine(float simplify = 0.3)
    {
        vector<ofPolyline> result;
        for (int i = 0; i < contourFinder.nBlobs; i++)
        {
            ofPolyline pl;
            vector<ofPoint> pts = contourFinder.blobs.at(i).pts;
            for (int k = 0; k < pts.size(); k++)
            {
                pl.addVertex(pts.at(k));
            }
            pl.setClosed(true);
            pl.simplify(simplify);            
            pl = pl.getResampledByCount(pl.getVertices().size() / 2);
            pl = pl.getSmoothed(3);
            result.push_back(pl);
        }
        return result;
    }
    
    float getWidth()
    {
        return kinect.getWidth();
    }
    
    float getHeight()
    {
        return kinect.getHeight();
    }
    
    
#ifndef USE_PALYER
    ofxKinect kinect;  
#else
    ofxKinectPlayer kinect;
#endif
    
	ofxCvGrayscaleImage grayImage; 
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;    
	ofxCvContourFinder  contourFinder;
	
	bool    bThreshWithOpenCV;
    
	int     nearThreshold;
	int     farThreshold;
        
    // depth hole filter
	DepthHoleFiller DHF;      
    bool    FILL_USING_HISTORY;
    bool    FILL_USING_CLOSING;
    bool    FILL_USING_CONTOURS;
    int     N_HISTORY;
    int     N_CLOSING_PASSES;
    int     MAX_HOLE_AREA;
    int     MAX_N_CONTOURS_TO_FILL;
    
};

#endif
