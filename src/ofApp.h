/*
 *   BLEURGH MACHINE
 */

#pragma once

#include "ofMain.h"
#include "ofxAppUtils.h"
#include "ofxGuiExtended2.h"
#include "ofxPostProcessing.h"
#include "ofxSortableList.h"
#include "ofxAssets.h"

using namespace ofxAssets;

// optional ofxGUI control panel for transformer,
// uncomment this if you're using ofxGui in this project
#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
#include "ofxTransformPanel.h"
#endif

//General Audio
#define OUT_CHANNELS 2
#define IN_CHANNELS 2
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 256
#define NUM_WINDOWS 64

//Filterbank
#define BANDWITH  1.0
#define LIN2dB(x) (double)(20. * log10(x))

#define HOST "localhost"
#define SEND_PORT 12345
#define RECEIVE_PORT 54321

// we only specify a pointer to the Particle Scene here, so tell the compiler
// that it exists as a class and we can include the class header in testApp.cpp

class Scene05;
class Scene06;
class Scene08;

/// make sure you check out main.cpp for how to run this app with app utils!
///
/// controls:
///     - L & R arrows to change scenes (note: wait for 2s transition)
///     - D arrow to change to "no scene" & U arrow to return to last scene
///     - 'd' key to toggle debug mode and enable control panel & warp editor
///     - 'm' key to mirror the render area on the x axis
///     - 'n' key to mirror the render area on the y axis
///     - 'a' key to enable aspect ratio scaling
///     - 'c' key to enable auto centering, only applied if apsect ratio scaling
///           is on and the quad warper is off
///     - 'q' key to toggle to quad warper
///     - 'f' to toggle fullscreen mode
///     - '-' & '=': remove/add particles when the particle scene is running
///     - 'o' to toggle scene overlap
///
class ofApp : public ofxApp { // <-- inherits ofxApp instead of ofBaseApp
    
public:
    
    ofApp()  {}
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void setupGui();
    void setupPanels();
    void updatePanels();
    
    // rendering transformer
    ofxTransformer transformer;
    
    // handles the scenes
    ofxSceneManager sceneManager;
    int lastScene;
    
    // keep a pointer to the Particle Scene, so we can modify it more easily
    Scene05 *scene05;
    Scene06 *scene06;
    Scene08 *scene08;
    
#ifdef HAVE_OFX_GUI
    ofxTransformPanel panel;
#endif
    
    bool guiShow, guiPlay;
    
    // OFX AUDIO ANALYZER
    
    void audioIn(float *input, int bufferSize, int nChannels);
    float smooth(float last, float value,  float amnt);
    ofSoundStream soundStream;

    float *left;
    float *right;
    float *spectrum1;
    int spectrumSize;
    float intenSmthAmnt, pitchSmthAmnt, specSmthAmnt;
    float smthPower, smthEnergy, smthRms;
    float smthPitch, smthConfidence, smthSalience;
    float smthHfc, smthCentroid, smthCmplx, smthInharmon;
    
    float maxEnergy, minPitch, maxPitch, maxHfc, maxCentroid, maxSpectralComp, maxOnHfc, maxOnCmpx, maxOnFlux, maxDct;
    
    string panelName;
    
    // OFX GUI EXTENDED
    
    ofxGui gui;
    
    ofxGuiPanel *menuAudioPanel, *menuCameraPanel, *menuSettingsPanel, *menuSettingsGuiPanel, *menuEffectsPanel, *menuBarPanel, *menuScenePanel, *scenesPanel;
    ofxGuiGroup2 *powerGroup, *beatGroup, *pitchGroup, *melodyGroup, *spectralGroup, *menuAudioGroup, *menuCameraGroup, *menuTransformGroup, *menuSettingsGroup, *menuEffectsGroup, *menuSettingsGuiGroup, *menuSettingsGuiPanelsGroup, *labelsGroup, *scenesGroup;
    
    ofParameter<bool> energyToggleVal;
    ofParameter <float> energySliderVal, energyMultiplierSliderVal, intenSmoothSliderVal;
    ofParameter <float> energyPlotterVal, energyWaveformVal;
    ofParameter <float> rmsSliderVal;
    
    ofParameter<bool> beatToggleVal;
    ofParameter <float> onsetHfcSliderVal, onsetComplexSliderVal, onsetFluxSliderVal, onsetAlphaSliderVal, onsetThresholdSliderVal, specSmoothSliderVal;
    ofParameter <bool> onsetsToggleVal;
    
    ofParameter<bool> pitchToggleVal;
    ofParameter <float> pitchConfSliderVal, salienceSliderVal, pitchSmoothSliderVal, tuningSliderVal;
    ofParameter <float> pitchPlotterVal;
    
    ofParameter<bool> melodyToggleVal;
    ofParameter <float> spectrumPlotterVal, melBandsPlotterVal, mfccPlotterVal, hpcpPlotterVal;
    
    ofParameter<bool> spectralToggleVal;
    ofParameter <float> hfcPlotterVal, centroidPlotterVal, specCmplxPlotterVal, inharmonPlotterVal;
    
    ofParameter<bool> showHelp;
    ofParameter <float> sweepSpeedSliderVal;
    ofParameter<bool> aButton, anotherButton;
    
    ofColor color;
    
    ofParameter<bool> lineIn1MuteToggleVal, lineIn2MuteToggleVal;
    ofParameter <float> lineIn1VolumeSliderVal, lineIn2VolumeSliderVal;
    
    ofParameter<string> sceneName;
    ofParameter<string> activeName;
    ofParameter<int> activeIndex;
    
    ofParameter<bool> isEmptyScene;
    
    //color panel
    ofxGuiPanel* colorPanel;
    ofxGuiGroup2 *colorToggles;
    
    ofParameterGroup colorParameters;
    ofParameter<bool> color0;
    ofParameter<bool> color1;
    ofParameter<bool> color2;
    
    ofParameter <bool> unlockPanels;
    ofParameter <void> cleanUpPanelsButton;
    ofParameter <void> quitButton;
    
    ofParameter<bool> hasBleachBypass, hasBloom, hasContrast, hasConvolution, hasDofAlt, hasDof, hasEdge, hasFakeSSS, hasFxaa, hasGodRays, hasHorizontalTiltShift, hasHsbShift, hasKaleidoscope, hasLimbDarkening, hasLUT, hasNoiseWarp, hasPixelate, hasRender, hasRGBShift, hasRimHighliting, hasSSAO, hasToon, hasVerticalTiltShift, hasZoomBlur;
    
    void toggleGroupHeader(bool & val);
    void cleanUpPanels();
    void setHeaderColors(int& index);
    void quitApp();
    
    ofTrueTypeFont ttFont12, ttFont16;
        
    ofLight light;
    
    ofEasyCam cam;
    ofParameter<int> cameraDistance;
    ofParameter<bool> hasAutoCam;
    ofxPostProcessing post;
    
    ofxSortableList *effect_toggles;
    void removed(RemovedElementData& data);
    void moved(MovingElementData& data);
    string status;
    
};
