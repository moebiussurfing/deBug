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

class Scene07 : public ofxFadeScene {

	public:

		// set the scene name through the base class initializer
		Scene07() : ofxFadeScene("Scene07 Particles") {
			particles.setAutoRemove(false);	// don't remove particles if dead
            setSingleSetup(true); // do not call setup each time the scene is loaded
            setFade(0, 0); // 0 millisecond fade in/out
		}
    
        ofApp *mainApp = (ofApp *)ofxGetAppPtr(); //Uncomment to access ofApp variables.

		// scene setup
		void setup() {
            ofBackground(20);
			// make some particles
			for(unsigned int i = 0; i < s_maxNumParticles/2; ++i) {
				particles.addParticle(new Particle());
			}
            ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 200);
		}

		// called when scene is entering, this is just a demo and this
		// implementation is not required for this class
		void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				ofLogNotice("Scene 07") << "update enter";
                mainApp->sceneName = "Scene 07";
			}
		
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateEnter();
			
			// finished entering?
			if(!isEntering()) {
				ofLogNotice("Scene 07") << "update enter done";
			}
		}

		// normal update
		void update() {
			particles.update();
		}

		// called when scene is exiting, this is just a demo and this
		// implementation is not required for this class
		void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				ofLogNotice("Scene 07") << "update exit";
			}
			
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateExit();
			
			// finished exiting?
			if(!isExiting()) {
				ofLogNotice("Scene 07") << "update exit done";
			}
		}

		// draw
		void draw() {
			ofFill();
			ofSetRectMode(OF_RECTMODE_CENTER);
//			ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 255*alpha);
			
			particles.draw();
		}
		
		// cleanup
		void exit() {
			particles.clear();
            ofSetRectMode(OF_RECTMODE_CORNER);
		}
		
		// add/remove particles
		void addOneParticle() {
			if(particles.size() < s_maxNumParticles) {
				particles.addParticle(new Particle());
			}
		}
		
		void removeOneParticle() {
			if(particles.size() > 1) { // leave 1 lonely particle ...
				particles.popLastParticle();
			}
		}

		// particle class		
		class Particle : public ofxParticle {

			public:
            
                ofColor particleColor = ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), ofRandom(120, 220));
			
				Particle() {
				
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
					app = ofxGetAppPtr();
				
					// ofxParticle is derived from ofRectangle
					// so these variables are built in
					width = ofRandom(10, 40);
					height = width;
					x = ofRandom(width/2, app->getRenderWidth());
					y = ofRandom(height/2, app->getRenderHeight());
					
					vel.set(ofRandom(-5, 5), ofRandom(-5, 5));
				}
				
				void update() {
				
					// calc new pos
					x += vel.x;
					y += vel.y;
					
					// bounce on x axis
					if(x < width/2) {
						x = width/2;
						vel.x = -vel.x;
					}
					else if(x > app->getRenderWidth()-width/2) {
						x = app->getRenderWidth()-width/2;
						vel.x = -vel.x;
					}
					
					// bounce on y axis
					if(y < height/2) {
						y = height/2;
						vel.y = -vel.y;
					}
					else if(y > app->getRenderHeight()-height/2) {
						y = app->getRenderHeight()-height/2;
						vel.y = -vel.y;
					}
				}
				
				void draw() {
                    ofSetColor(particleColor);
					ofDrawRectangle(*this); // <- use this object as an ofRectangle
				}
				
				ofxApp *app;
				ofVec2f vel;
		};
		
		// particle manager to wrangle our little ones
		ofxParticleManager particles;
		
		// max number of allowed particles
		static const int s_maxNumParticles = 100;
};
