#pragma once

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "SharedData.h"

#include "WorkFlow.h"
#include "Instruction.h"
#include "MainInteraction.h"
#include "CheckBrainWave.h"

#include "GUI.h"

using namespace Apex;

class MiraiHaku : public ofBaseApp
{
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    
    ofxStateMachine<SharedData> stateMachine;    
    SharedData sharedData; 
    WorkFlow workflow;
    
    GUI gui;
};
