//
//  fftLiveScene.h
//  BleurghMachine
//
//  Created by Clément Girault on 26/04/2016.
//
//


#pragma once

#include <ofxAppUtils.h>

class newScene : public ofxFadeScene {
    
public:
    
    // set the scene name through the base class initializer
    newScene() : ofxFadeScene("New Scene") {
        setSingleSetup(false); // call setup each time the scene is loaded
        setFade(1000, 0); // 1 second fade in/out
    }
    
    // scene setup
    void setup() {
        
        ofBackground(20);
        ofSetFrameRate(24);
        
        
    }
    
    // called when scene is entering, this is just a demo and this
    // implementation is not required for this class
    void updateEnter() {
        
        // called on first enter update
        if(isEnteringFirst()) {
            ofLogNotice("MeshScene") << "update enter";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
        
        // finished entering?
        if(!isEntering()) {
            ofLogNotice("MeshScene") << "update enter done";
        }
    }
    
    // normal update
    void update() {
        
    }
    
    // called when scene is exiting, this is just a demo and this
    // implementation is not required for this class
    void updateExit() {
        
        // called on first exit update
        if(isExitingFirst()) {
            ofLogNotice("fftLScene") << "update exit";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
        
        // finished exiting?
        if(!isExiting()) {
            ofLogNotice("fftLiveScene") << "update exit done";
        }
    }
    
    // draw
    void draw() {

    }
    
    // cleanup
    void exit() {

    }
    
};
