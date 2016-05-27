//
//  Scene08.h
//  BleurghMachine
//
//  Created by Clément Girault on 26/04/2016.
//
//

#pragma once


#include <ofxAppUtils.h>

class Scene08 : public ofxFadeScene {
    
public:

    ofApp *mainApp = (ofApp *)ofxGetAppPtr();
    
    ofxGuiGroup *scene08Group;
    
    ofParameter<int> sphereResolution;
    ofParameter<float> sphereRadius;
    ofParameter<bool> isFilled, isWireframe;
    
//--------------------------------------------------------------
    // set the scene name through the base class initializer
    Scene08() : ofxFadeScene("Scene08 Mesh Spheres") {
        setSingleSetup(true); // do not call setup each time the scene is loaded
        setFade(0, 0); // 0 millisecond fade in/out
    }
    
//--------------------------------------------------------------
    // scene setup
    void setup() {
        
        sphereRadius.set("Sphere Radius", ofGetWidth()/3, 0.0, ofGetWidth());
        sphereResolution.set("Sphere Resolution", 4, 0, 6);
        isFilled.set("Fill", true);
        isWireframe.set("Wireframe", false);
        
        scene08Group = mainApp->menuScenePanel->addGroup("Scene 08");
        scene08Group->add(sphereRadius);
        scene08Group->add(sphereResolution);
        scene08Group->add(isFilled);
        
        // scene's panel starts off hidden
        scene08Group->getVisible().set(false);
        
        
    }
//--------------------------------------------------------------
    // called when scene is entering, this is just a demo and this
    // implementation is not required for this class
    void updateEnter() {
        
        // called on first enter update
        if(isEnteringFirst()) {
            ofLogNotice("Scene08") << "update enter";
            mainApp->sceneName = "Scene 08";
        }
        
        // scene's panel gets shown
        scene08Group = mainApp->menuScenePanel->addGroup("Scene 08");
        scene08Group->add(sphereRadius);
        scene08Group->add(sphereResolution);
        scene08Group->add(isFilled);
        scene08Group->add(isWireframe);
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
        
        // finished entering?
        if(!isEntering()) {
            ofLogNotice("Scene08") << "update enter done";
//            scene08Gui->setVisible(true);
        }
    }
//--------------------------------------------------------------
    // normal update
    void update() {

    }
    
    
//--------------------------------------------------------------
    // called when scene is exiting, this is just a demo and this
    // implementation is not required for this class
    void updateExit() {
        
        // called on first exit update
        if(isExitingFirst()) {
            ofLogNotice("Scene 08") << "update exit";
            scene08Group->getVisible().set(false); // scene's panel gets hidden
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
        
        // finished exiting?
        if(!isExiting()) {
            ofLogNotice("Scene 08") << "update exit done";
        }
    }
//--------------------------------------------------------------
    // draw
    void draw() {
        
    }

//--------------------------------------------------------------
    // cleanup
    void exit() {
        
    }
//--------------------------------------------------------------

    void keyPressed(int key) {
        
        switch(key) {
            case '\'':
                isFilled = !isFilled;
                break;
            case ';':
                isWireframe = !isWireframe;
                break;
        }
    }
};
