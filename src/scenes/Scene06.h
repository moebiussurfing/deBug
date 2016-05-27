/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include <ofxAppUtils.h>

class Scene06 : public ofxFadeScene {

	public:

		// set the scene name through the base class initializer
		Scene06() : ofxFadeScene("Scene06 Lines"){
            setSingleSetup(false); // call setup each time the scene is loaded
            setFade(0, 0); // 0 millisecond fade in/out
		}
    
        ofApp *mainApp = (ofApp *)ofxGetAppPtr();

		// scene setup
		void setup() {
            ofBackground(20);
			lines.push_back(new Line(Line::HORZ));
			lines.push_back(new Line(Line::VERT));
            lines.push_back(new Line(Line::RANDOM));
            lines.push_back(new Line(Line::RANDOM2));
		}

		// called when scene is entering, this is just a demo and this
		// implementation is not required for this class
		void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				ofLogNotice("Scene 06") << "update enter";
                mainApp->sceneName = "Scene 06";
			}
		
//            // scene's panel gets shown
//            scene06Group->getVisible().set(true);
            
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateEnter();
			
			// finished entering?
			if(!isEntering()) {
				ofLogNotice("Scene 06") << "update enter done";
			}
		}

		// normal update
		void update() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->update();
			}
		}

		// called when scene is exiting, this is just a demo and this
		// implementation is not required for this class
		void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				ofLogNotice("Scene 06") << "update exit";
			}
			
//            // scene's panel gets hidden
//            scene06Group->getVisible().set(false);
            
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateExit();
			
			// finished exiting?
			if(!isExiting()) {
				ofLogNotice("Scene 06") << "update exit done";
			}
		}

		// draw
		void draw() {
			ofSetLineWidth(5);
			ofSetColor(255, 255, 255, 255*alpha);	// alpha member variable for fade in/out
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->draw();
			}
			ofSetLineWidth(1);
		}
		
		// cleanup
		void exit() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				Line *l = lines[i];
				delete l;
			}
			lines.clear();
		}

		// line class		
		class Line {

			public:
			
				Line(int type) {
					this->type = (Type) type;
				}
				
				void update() {
				
					// get a pointer to the parent app for data access,
					// here used to get the size of the render area
					// you can also cast the ofxApp reference to your own derived
					// class to pass custom data:
					//
					// TestApp *testApp = (TestApp*) (ofxGetAppPtr());
					//
					// NOTE: you must use "ofxGetAppPtr()" <-- note the "x",
					// this is a replacement for "ofGetAppPtr()" which does not
					// return the pointer to the correct app instance
					//
					ofxApp *app = ofxGetAppPtr();
					
					switch(type) {
						case HORZ:
							pos1.set(0, ofNoise(ofGetElapsedTimef())*app->getRenderHeight());
							pos2.set(app->getRenderWidth(), ofNoise(ofGetElapsedTimef())*app->getRenderHeight());
							break;
						case VERT:
							pos1.set(ofNoise(ofGetElapsedTimef())*app->getRenderHeight(), 0);
							pos2.set(ofNoise(ofGetElapsedTimef())*app->getRenderHeight(), app->getRenderHeight());
							break;
                        case RANDOM:
                            pos1.set(ofNoise(ofGetElapsedTimef())*app->getRenderHeight(), ofRandom(0,app->getRenderHeight()));
                            pos2.set(ofNoise(ofGetElapsedTimef())*ofRandom(0,app->getRenderHeight()), app->getRenderHeight());
                            break;
                        case RANDOM2:
                            pos1.set(ofNoise(ofGetElapsedTimef())*app->getRenderHeight(), ofRandom(0,app->getRenderHeight()));
                            pos2.set(ofRandom(0,app->getRenderWidth()), app->getRenderHeight());
                            break;
					}
				}
				
				void draw() {
					ofDrawLine(pos1, pos2);
				}
				
				enum Type {
					HORZ,
					VERT,
                    RANDOM,
                    RANDOM2
				};
				int type;
				
				ofVec2f pos1, pos2;				
		};
		
		// lines
		vector<Line*> lines;
};
