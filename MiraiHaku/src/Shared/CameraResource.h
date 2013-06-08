//
//  CameraResource.h
//  emptyExample
//
//  Created by Akira Hayasaka on 9/29/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef emptyExample_CameraResource_h
#define emptyExample_CameraResource_h

//#define USE_WEBCAM

#include "ofMain.h"
#include "ofxEdsdk.h"


class CameraResource
{
public:
    
    void setup()
    {
#ifndef USE_WEBCAM        
        camera.setup();        
#else
        camera.initGrabber(1056, 704);
#endif
    }
    
    void update()
    {
        camera.update();
        
#ifndef USE_WEBCAM            
        liveTex.setFromPixels(camera.getLivePixels());
#else
        liveTex.setFromPixels(camera.getPixels(), 
                              camera.getWidth(), camera.getHeight(), 
                              OF_IMAGE_COLOR);
#endif
    }
    
#ifndef USE_WEBCAM
    ofxEdsdk::Camera camera;   
#else
    ofVideoGrabber camera;
#endif
    
    ofImage liveTex;
};

#endif
