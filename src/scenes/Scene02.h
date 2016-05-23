//
//  Scene02.h
//  BleurghMachine
//
//  Created by Clément Girault on 26/04/2016.
//
//


#pragma once

#include <ofxAppUtils.h>


class Scene02 : public ofxFadeScene {
    
public:
    
    ofApp *mainApp = (ofApp *)ofxGetAppPtr();
    
    float intenSmthAmnt, pitchSmthAmnt, specSmthAmnt;
    float smthPower, smthEnergy, smthRms;
    float smthPitch, smthConfidence, smthSalience;
    float smthHfc, smthCentroid, smthCmplx, smthInharmon;
    
    // set the scene name through the base class initializer
    Scene02() : ofxFadeScene("Scene02 Aubio") {
        setSingleSetup(true); // do not call setup each time the scene is loaded
        setFade(0, 0); // 0 millisecond fade in/out
    }
    
    // scene setup
    void setup() {
        
    }
    
    // called when scene is entering, this is just a demo and this
    // implementation is not required for this class
    void updateEnter() {
        
        // called on first enter update
        if(isEnteringFirst()) {
            ofLogNotice("Scene 02") << "update enter";
            mainApp->sceneName = "Scene 02";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
        
        // finished entering?
        if(!isEntering()) {
            ofLogNotice("Scene 02") << "update enter done";
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
            ofLogNotice("Scene 02") << "update exit";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
        
        // finished exiting?
        if(!isExiting()) {
            ofLogNotice("Scene 02") << "update exit done";
        }
    }
    
    // draw
    void draw() {
        //        ofSetColor(ofColor(255,200,50));
        //        for(int i = 0; i < spectrumSize; i++){
        //            float h = ofMap(spectrum1[i], log10(0.001), log10(1.0), 0.0, 1.0);
        //            float x = ofGetWidth()/float(spectrumSize);
        //            x *= i;
        //            float y = ofGetHeight()*.75;
        //            ofDrawLine(x,y,x,y-500*h);
        //
        //        }
        
        ofSetColor(ofColor::red);
        float radius = 10 + smthRms*200;
        float xpos = ofGetWidth() *.5-400;
        float ypos = ofGetHeight();
        ypos -= ofGetHeight()*.5 * smthRms*10;
        xpos += 200;
        ofDrawCircle(xpos, ypos, radius);
        
        
        ofSetColor(ofColor::green);
        float radius2 = 10 + smthEnergy;
        float ypos2 = ofGetHeight();
        ypos2 -= ofGetHeight()*.5 * smthEnergy/10;
        xpos += 100;
        ofDrawCircle(xpos, ypos2, radius2);
        
        ofSetColor(ofColor::blue);
        float radius3 = 10 + smthPower*200;
        float ypos3 = ofGetHeight();
        ypos3 -= ofGetHeight()*.5 * smthPower*100;
        xpos += 100;
        ofDrawCircle(xpos, ypos3, radius3);
        
        ofSetColor(ofColor::orange);
        float radius4 = 10 + smthPitch/100;
        float ypos4 = ofGetHeight();
        ypos4 -= ofGetHeight()*.5 * smthPitch/5000;
        xpos += 100;
        ofDrawCircle(xpos, ypos4, radius4);

    }
    
    // cleanup
    void exit() {

    }
    
};
