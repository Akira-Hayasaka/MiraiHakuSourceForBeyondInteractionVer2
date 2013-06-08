#include "MiraiHaku.h"

void MiraiHaku::setup()
{
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofBackground(0);
    
    sharedData.setup();    
    
    stateMachine.addState(new Instruction(), &sharedData);
    stateMachine.addState(new CheckBrainWave(), &sharedData);
    stateMachine.addState(new MainInteraction(), &sharedData);  
    sharedData.stateMachine = &stateMachine;
    
    stateMachine.changeState("Instruction");    
    
    workflow.setup(&stateMachine, &sharedData);    
    gui.setup(&stateMachine, &sharedData);   
}

void MiraiHaku::update()
{
    sharedData.update();
    gui.update();
    stateMachine.update();
    workflow.update();
}

void MiraiHaku::draw()
{
    stateMachine.draw();         
    sharedData.drawBrainParam();    
    gui.draw();
    
    ofSetWindowTitle("fps = " + ofToString((int)ofGetFrameRate()) + 
                     " sigQ = " + ofToString(sharedData.thinkGear.getSignalQuality()));
}

void MiraiHaku::keyPressed(int key){}
void MiraiHaku::keyReleased(int key){}
void MiraiHaku::mouseMoved(int x, int y){}
void MiraiHaku::mouseDragged(int x, int y, int button){}
void MiraiHaku::mousePressed(int x, int y, int button){}
void MiraiHaku::mouseReleased(int x, int y, int button){}
void MiraiHaku::windowResized(int w, int h){}
void MiraiHaku::gotMessage(ofMessage msg){}
void MiraiHaku::dragEvent(ofDragInfo dragInfo){}