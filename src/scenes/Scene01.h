//
//  Scene01.h
//  BleurghMachine
//
//  Created by Clément Girault on 26/04/2016.
//
//


#pragma once

#include <ofxAppUtils.h>
#include "ofxFilterbank.h"
#include "ofApp.h"


class Scene01 : public ofxFadeScene {
    
public:
    
    ofApp *mainApp = (ofApp *)ofxGetAppPtr();

    int bufferSize = BUFFER_SIZE;
    float sampleRate = SAMPLE_RATE;
    int outChan = OUT_CHANNELS;
    int inChan = IN_CHANNELS;
    int ticksPerBuffer = bufferSize/64;;
    
    ofxFilterbank filterBank;

    
    // set the scene name through the base class initializer
    Scene01() : ofxFadeScene("Scene01 Filterbank") {
        setSingleSetup(true); // do not call setup each time the scene is loaded
        setFade(0, 0); // 0 millisecond fade in/out
    }

    // scene setup
    void setup() {
        
        int midiMin = 21;
        int midiMax = 108;

        filterBank.setup(bufferSize, midiMin, midiMax, inChan, BANDWITH, sampleRate, 1.0);
        filterBank.setColor(ofColor::orange);
        
        threshold.set("Threshold", 0.01, 0.006, 0.05);
        pitchDev.set("Pitch Deviation", 0.0, 0.0, 1.0);
        maskAmnt.set("Mask Amount", 0.0, 0.0, 1.0);
        smoothAmnt.set("Smoothing", 0.7, 0.0, 1.0);
////        estimateMax.set("Estimate Max", 0.02, 0.0, 0.05);
    
        
        scene01Panel = mainApp->gui.addPanel("Scene 01");
        scene01Panel->loadTheme("theme_bleurgh.json");
        
        scene01Panel->setPosition(ofGetWidth()-330, 0);
        scene01Panel->setShowHeader(false);
        
        scene01Group = scene01Panel->addGroup("Scene01");
        scene01Group->add(threshold);
        scene01Group->add(pitchDev);
        scene01Group->add(maskAmnt);
        scene01Group->add(smoothAmnt);
//        aGroup->add(estimateMax);

        // scene's panel starts off hidden
        scene01Panel->getVisible().set(false);

    }
    
    // called when scene is entering, this is just a demo and this
    // implementation is not required for this class
    void updateEnter() {
        
        // called on first enter update
        if(isEnteringFirst()) {
            ofLogNotice("Scene 01") << "update enter";
            mainApp->sceneName = "Scene 01";
        }
        
        // scene's panel gets shown
        scene01Panel->getVisible().set(true);
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
        
        // finished entering?
        if(!isEntering()) {
            ofLogNotice("Scene 01") << "update enter done";
        }
    }
    

    void audioIn(float * input, int bufferSize, int nChannels){
        
        //Analyze Input Buffer with ofxFilterbank
        filterBank.analyze(input);
        
    }

    
    // normal update
    void update() {
        
        filterBank.threshold = threshold;
        filterBank.pitchDev = pitchDev;
        filterBank.maskAmnt = maskAmnt;
        filterBank.smoothAmnt = smoothAmnt;
//        filterBank.estimateMax = estimateMax;
        
    }
    
    
    
    // called when scene is exiting, this is just a demo and this
    // implementation is not required for this class
    void updateExit() {
        
        // called on first exit update
        if(isExitingFirst()) {
            ofLogNotice("Scene 01") << "update exit";
        }
        
        // scene's panel gets hidden
        scene01Panel->getVisible().set(false);
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
        
        // finished exiting?
        if(!isExiting()) {
            ofLogNotice("Scene 01") << "update exit done";
        }
    }
    
    // draw
    void draw() {

        int filterBankWidth = ofGetWidth();
        int filterBankHeight = ofGetHeight();
        
//        float multip = (filterBankWidth/2)/bufferSize; // FIXME this should give 3.75 in my case (1920/2/256) but is not working.
        float multip = 3.75; // FIXME see above
        // draw the left input channel:

        //Draw FilterBank
        {
            ofPushStyle();
            ofPushMatrix();
            ofTranslate (ofGetWidth()/2-filterBankWidth/2,0,0);
            filterBank.draw(filterBankWidth,filterBankHeight);
            ofPopMatrix();
            ofPopStyle();
        }
        ofSetColor(0);

        string reportString =  "Sampling Rate: "+ ofToString(SAMPLE_RATE) +"\nBuffer size: "+ ofToString(bufferSize);
        mainApp->ttFont12.drawString(reportString, ofGetWidth()/2-filterBankWidth/2, filterBankHeight+220);
        
    }
    
    // cleanup
    void exit() {
//        filterBank.exit();
    }
    
    ofxGui scene01Gui;
    ofxGuiPanel* scene01Panel;
    ofxGuiGroup *scene01Group;

    ofParameterGroup scene01GuiParameters;
    ofParameter<float> threshold;
    ofParameter<float> pitchDev;
    ofParameter<float> maskAmnt;
    ofParameter<float> smoothAmnt;
//    ofParameter<float> estimateMax;
   
    
};
