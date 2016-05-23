//
//  Scene04.h
//  BleurghMachine
//
//  Created by Clément Girault on 26/04/2016.
//
//


#pragma once

#include <ofxAppUtils.h>

class Scene04 : public ofxFadeScene {
    
public:
    
    // set the scene name through the base class initializer
    Scene04() : ofxFadeScene("Scene04 Moonscape") {
        setSingleSetup(true); // do not call setup each time the scene is loaded
        setFade(0, 0); // 0 millisecond fade in/out
    }
    
    ofApp *mainApp = (ofApp *)ofxGetAppPtr();
    
    // helper functions
    
    ofFloatImage img;
    ofVboMesh mesh;
    ofLight light;
    
    // scene setup
    void setup() {
        
//        ofBackground(20);
//        ofSetFrameRate(24);
        
        img.load("assets/3D/nyc-small.exr");
        
//        light.enable();
//        light.setPosition(+500, 0, 0);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        int skip = 1;
        int width = img.getWidth();
        int height = img.getHeight();
        for(int y = 0; y < height - skip; y += skip) {
            for(int x = 0; x < width - skip; x += skip) {
                ofVec3f nw = getVertexFromImg(img, x, y);
                ofVec3f ne = getVertexFromImg(img, x + skip, y);
                ofVec3f sw = getVertexFromImg(img, x, y + skip);
                ofVec3f se = getVertexFromImg(img, x + skip, y + skip);
                
                addFace(mesh, nw, ne, se, sw);
            }
        }
        
        
    }
    
    // called when scene is entering, this is just a demo and this
    // implementation is not required for this class
    void updateEnter() {
        
        // called on first enter update
        if(isEnteringFirst()) {
            ofLogNotice("Scene 04") << "update enter";
            mainApp->sceneName = "Scene 04";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
        
        // finished entering?
        if(!isEntering()) {
            ofLogNotice("Scene 04") << "update enter done";
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
            ofLogNotice("Scene 04") << "update exit";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
        
        // finished exiting?
        if(!isExiting()) {
            ofLogNotice("Scene 04") << "update exit done";
        }
    }
    
    // draw
    void draw() {
        ofEnableLighting;
        light.enable();
        light.setPosition(+500, 0, 0);
        mainApp->post.begin(mainApp->cam);
        ofScale(1, -1, 1);
        ofRotateX(60);
        ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2, 0);
        ofSetColor(110);
        ofEnableDepthTest();
        mesh.draw();
        ofDisableDepthTest();
        mainApp->post.end();
        ofDisableLighting;
    }
    
    // cleanup
    void exit() {

    }
    
    //Some helper functions
    //--------------------------------------------------------------
    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
        ofVec3f normal = ((b - a).cross(c - a)).normalize();
        mesh.addNormal(normal);
        mesh.addVertex(a);
        mesh.addNormal(normal);
        mesh.addVertex(b);
        mesh.addNormal(normal);
        mesh.addVertex(c);
    }
    
    //--------------------------------------------------------------
    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
        addFace(mesh, a, b, c);
        addFace(mesh, a, c, d);
    }
    
    //--------------------------------------------------------------
    ofVec3f getVertexFromImg(ofFloatImage& img, int x, int y) {
        return ofVec3f(x, y, 100 * img.getColor(x, y).getBrightness());
    }

    
};
