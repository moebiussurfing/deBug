//
//  Scene05.h
//  BleurghMachine
//
//  Created by Clément Girault on 26/04/2016.
//
//


#pragma once

#include <ofxAppUtils.h>

class Scene05 : public ofxFadeScene {
    
public:
    
    // set the scene name through the base class initializer
    Scene05() : ofxFadeScene("Scene05 Wireframe") {
        setSingleSetup(true); // do not call setup each time the scene is loaded
        setFade(0, 0); // 0 millisecond fade in/out
    }
    
    ofApp *mainApp = (ofApp *)ofxGetAppPtr();
    
    ofSpherePrimitive sphere;
    ofMesh planet;
    
    float time;
    
    ofxGuiPanel *scene05Panel;
    ofxGuiGroup *scene05Group;
    
    ofParameter<int> sphereResolution;
    ofParameter<int> sphereRadius;
    
    ofParameter<bool> isFilled, isWireframe;
    
    // scene setup
    void setup() {
        
//        ofBackground(20);
//        ofSetFrameRate(24);
        
        sphere.setPosition(0, 0, 0);
        
        sphereRadius.set("Sphere Radius", 250, 0, ofGetWidth());
        sphereResolution.set("Sphere Resolution", 4, 2, 8);
        isFilled.set("Fill", true);
        isWireframe.set("Wireframe  w", true);
        
        scene05Panel = mainApp->gui.addPanel("Scene 05", ofJson({{"flex-direction", "row"}, {"flex-wrap", "wrap"}}));
        scene05Panel->loadTheme("assets/themes/theme_bleurgh.json");
        
        scene05Panel->setPosition(ofGetWidth()-330, 0);
        scene05Panel->setShowHeader(false);
        
        scene05Group = scene05Panel->addGroup("Scene 05");
        
        scene05Group->add(sphereRadius);
        scene05Group->add(sphereResolution);
        
        // scene's panel starts off hidden
        scene05Panel->getVisible().set(false);
        
        planet = sphere.getMesh();
        
    }
    
    // called when scene is entering, this is just a demo and this
    // implementation is not required for this class
    void updateEnter() {
        
        // called on first enter update
        if(isEnteringFirst()) {
            ofLogNotice("Scene 05") << "update enter";
            mainApp->sceneName = "Scene 05";
        }
        
        // scene's panel gets shown
        scene05Panel->getVisible().set(true);
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
        
        // finished entering?
        if(!isEntering()) {
            ofLogNotice("Scene 05") << "update enter done";
        }
    }
    
    // normal update
    void update() {
        
        vector<ofVec3f>& verts = planet.getVertices();
        for(unsigned int i = 0; i < verts.size(); i++){
            verts[i].x += ofSignedNoise(verts[i].x*ofRandom(-1.,1), verts[i].y/ofRandom(-1.,1),verts[i].z/ofRandom(-1.,1), ofGetElapsedTimef());
            verts[i].y += ofSignedNoise(verts[i].z*ofRandom(-1.,1), verts[i].x/ofRandom(-1.,1),verts[i].y/ofRandom(-1.,1), ofGetElapsedTimef());
            verts[i].z += ofSignedNoise(verts[i].y*ofRandom(-1.,1), verts[i].z/ofRandom(-1.,1),verts[i].x/ofRandom(-1.,1), ofGetElapsedTimef());
        }
        
    }
    
    // called when scene is exiting, this is just a demo and this
    // implementation is not required for this class
    void updateExit() {
        
        // called on first exit update
        if(isExitingFirst()) {
            ofLogNotice("Scene 05") << "update exit";
        }
        
        // scene's panel gets hidden
        scene05Panel->getVisible().set(false);
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
        
        // finished exiting?
        if(!isExiting()) {
            ofLogNotice("Scene 05") << "update exit done";
        }
    }
    
    // draw
    void draw() {
        
        sphere.setRadius(sphereRadius);
        sphere.setResolution(sphereResolution);
        
        mainApp->post.begin(mainApp->cam);
        
        planet.drawWireframe();
        sphere.drawWireframe();
        ofSetColor(150,10);
        planet.drawFaces();
        ofSetColor(250);

        mainApp->post.end();

    }
    
    // cleanup
    void exit() {

    }
    
    //--------------------------------------------------------------
    
    void keyPressed(int key) {
        
        switch(key) {
            case 'k':
                isFilled = !isFilled;
                break;
            case ';':
                isWireframe = !isWireframe;
                break;

        }
    }
    
};
