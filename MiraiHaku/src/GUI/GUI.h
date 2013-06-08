//
//  GUI.h
//  emptyExample
//
//  Created by Akira Hayasaka on 9/29/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_GUI_h
#define emptyExample_GUI_h

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxStateMachine.h"
#include "SharedData.h"

class GUI
{
public:
    
    void setup(ofxStateMachine<SharedData>* stateMachine, SharedData* sharedData)
    {
        this->stateMachine = stateMachine;
        this->sharedData = sharedData;        
        
        gui.ofxControlPanel::setup("Control Panel", 10, 10, ofGetWidth()-20, ofGetHeight()-35);
        
        gui.addPanel("Settings", 6, false);        
        
        stateNames = stateMachine->getStateNames();
        int stateIdx = ofFind(stateNames, stateMachine->getCurrentStateName());
        ofLog() << "stateIdx = " << stateIdx;
        gui.ofxControlPanel::addTextDropDown("StateList", "StateList", stateIdx, stateNames);        
        
        gui.setWhichColumn(0);
        gui.addLabel("Kinect settings");          
        gui.addToggle("thresh w/ CV?", "thresh_cv_tag", true);
        gui.addSlider("near threshold", "n_thesh_tag", 255, 0, 255, true);
        gui.addSlider("far threshold", "f_thesh_tag", 143, 0, 255, true);  
        
        gui.addLabel("");        
        gui.addLabel("Depth hole filter");        
        gui.addToggle("Fill Using History?", "FILL_USING_HISTORY", 0);
        gui.addSlider("Depth History", "N_HISTORY",	2, 0, 16, true);        
        gui.addToggle("Fill Using Closing?", "FILL_USING_CLOSING", 0);
        gui.addSlider("# Closing Passes", "N_CLOSING_PASSES", 1, 0, 5,  true);        
        gui.addToggle("Fill Using Contours?", "FILL_USING_CONTOURS", 0);	
        gui.addSlider("Max Area to Fill", "MAX_HOLE_AREA", 400, 1, 10000, true);
        gui.addSlider("Max #Holes to Fill",	"MAX_N_CONTOURS_TO_FILL", 100, 0, 1000, true); 
        
        float kinectRatio = 0.5;
        
        gui.setWhichColumn(1);
        gui.addDrawableRect("cv gray Near", 
                            &sharedData->kinectResource.grayThreshNear, 
                            220 * kinectRatio, 170 * kinectRatio);        
        gui.addDrawableRect("cv gray Far", 
                            &sharedData->kinectResource.grayThreshFar, 
                            220 * kinectRatio, 170 * kinectRatio);                
        gui.addDrawableRect("cv gray Depth", 
                            &sharedData->kinectResource.grayImage, 
                            220 * kinectRatio, 170 * kinectRatio);
        gui.setWhichColumn(2);    
        gui.addDrawableRect("kinect colorTex", 
                            &sharedData->kinectResource.kinect.getTextureReference(), 
                            220 * kinectRatio, 170 * kinectRatio);        
        gui.addDrawableRect("kinect depthTex", 
                            &sharedData->kinectResource.kinect.getDepthTextureReference(), 
                            220 * kinectRatio, 170 * kinectRatio);        
        gui.addDrawableRect("contour image", 
                            &sharedData->kinectResource.contourFinder, 
                            220 * kinectRatio, 170 * kinectRatio);   
        
        float camRatio = 0.4;
        
        gui.setWhichColumn(3);
        gui.addDrawableRect("Live Pixels", 
                            &sharedData->cameraResource.liveTex, 
                            1056 * camRatio, 704 * camRatio);          
        
        gui.enableEvents();
        ofAddListener(gui.ofxControlPanel::createEventGroup("StateList"), 
                      this, &GUI::onStateListChanged);        
        
        ofAddListener(gui.createEventGroup("thresh_cv_tag"), 
                      this, &GUI::onKinectThreshWithCV);
        ofAddListener(gui.createEventGroup("n_thesh_tag"), 
                      this, &GUI::onKinectNearThreashold);
        ofAddListener(gui.createEventGroup("f_thesh_tag"), 
                      this, &GUI::onKinectFarThreashold);        
        
        ofAddListener(gui.createEventGroup("FILL_USING_HISTORY"), 
                      this, &GUI::onFILL_USING_HISTORY);
        ofAddListener(gui.createEventGroup("FILL_USING_CLOSING"), 
                      this, &GUI::onFILL_USING_CLOSING);        
        ofAddListener(gui.createEventGroup("FILL_USING_CONTOURS"), 
                      this, &GUI::onFILL_USING_CONTOURS);     
        ofAddListener(gui.createEventGroup("N_HISTORY"), 
                      this, &GUI::onN_HISTORY);             
        ofAddListener(gui.createEventGroup("N_CLOSING_PASSES"), 
                      this, &GUI::onN_CLOSING_PASSES);                     
        ofAddListener(gui.createEventGroup("MAX_HOLE_AREA"), 
                      this, &GUI::onMAX_HOLE_AREA); 
        ofAddListener(gui.createEventGroup("MAX_N_CONTOURS_TO_FILL"), 
                      this, &GUI::onMAX_N_CONTOURS_TO_FILL);   
        
        gui.ofxControlPanel::loadSettings("settings.xml");
        feedSavedSettingData();
        gui.hide();        
    }
    
    void update()
    {
        if (isDisplaying())
        {
            ofEventArgs fake;
            gui.update(fake);
        }
    }
    
    void draw()
    {
        if (isDisplaying())
        {
            ofEventArgs fake;
            gui.draw(fake);
        }
    }
    
    bool isDisplaying()
    {
        return !gui.hidden;
    }
    
    
private:
    
    ofxAutoControlPanel gui;
    ofxStateMachine<SharedData>* stateMachine;    
    SharedData* sharedData;  
    
    vector<string> stateNames;
    
    void feedSavedSettingData()
    {
        sharedData->kinectResource.bThreshWithOpenCV = gui.getValueB("thresh_cv_tag");        
        sharedData->kinectResource.nearThreshold = gui.getValueI("n_thesh_tag");
        sharedData->kinectResource.farThreshold = gui.getValueI("f_thesh_tag");
        sharedData->kinectResource.FILL_USING_HISTORY = gui.getValueB("FILL_USING_HISTORY");
        sharedData->kinectResource.FILL_USING_CLOSING = gui.getValueB("FILL_USING_CLOSING");
        sharedData->kinectResource.FILL_USING_CONTOURS = gui.getValueB("FILL_USING_CONTOURS");    
        sharedData->kinectResource.N_HISTORY = gui.getValueI("N_HISTORY");
        sharedData->kinectResource.N_CLOSING_PASSES = gui.getValueI("N_CLOSING_PASSES");
        sharedData->kinectResource.MAX_HOLE_AREA = gui.getValueI("MAX_HOLE_AREA");
        sharedData->kinectResource.MAX_N_CONTOURS_TO_FILL = gui.getValueI("MAX_N_CONTOURS_TO_FILL");        
    }    
    
    void onStateListChanged(guiCallbackData& data)
    {
        stateMachine->changeState(stateNames.at(data.getInt(0)));
    }    
    
    void onKinectThreshWithCV(guiCallbackData& data)
    {
        sharedData->kinectResource.bThreshWithOpenCV = gui.getValueB("thresh_cv_tag");
    }
    
    void onKinectNearThreashold(guiCallbackData& data)
    {
        sharedData->kinectResource.nearThreshold = data.getInt(0);
    }
    
    void onKinectFarThreashold(guiCallbackData& data)
    {
        sharedData->kinectResource.farThreshold = data.getInt(0);
    }     
    
    void onFILL_USING_HISTORY(guiCallbackData& data)
    {
        sharedData->kinectResource.FILL_USING_HISTORY = gui.getValueB("FILL_USING_HISTORY");
    }
    
    void onFILL_USING_CLOSING(guiCallbackData& data)
    {
        sharedData->kinectResource.FILL_USING_CLOSING = gui.getValueB("FILL_USING_CLOSING");        
    }    
    
    void onFILL_USING_CONTOURS(guiCallbackData& data)
    {
        sharedData->kinectResource.FILL_USING_CONTOURS = gui.getValueB("FILL_USING_CONTOURS");        
    } 
    
    void onN_HISTORY(guiCallbackData& data)
    {
        sharedData->kinectResource.N_HISTORY = data.getInt(0);
    } 
    
    void onN_CLOSING_PASSES(guiCallbackData& data)
    {
        sharedData->kinectResource.N_CLOSING_PASSES = data.getInt(0);        
    } 
    
    void onMAX_HOLE_AREA(guiCallbackData& data)
    {
        sharedData->kinectResource.MAX_HOLE_AREA = data.getInt(0);        
    } 
    
    void onMAX_N_CONTOURS_TO_FILL(guiCallbackData& data)
    {
        sharedData->kinectResource.MAX_N_CONTOURS_TO_FILL = data.getInt(0);        
    }      
};

#endif
