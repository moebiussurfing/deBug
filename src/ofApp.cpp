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
#include "ofApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    
    ofSetFullscreen( true );
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetFrameRate( 24 );
    ofSetVerticalSync(true);
    ofBackground(10);
    
    status = "";
    
    activeName.set("element name", "");
    activeIndex.set("element index", -1);
    
    ofSetWindowShape(1920, 1200);
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);
    
    
    ttFont12 = font("NewMediaFett.ttf", 12);
    ttFont16 = font("NewMediaFett.ttf", 16);
    //    ttFont.load("assets/fonts/NewMediaFett.ttf", 12);
    //     font("fontName", 30).drawString("...", 10, 10);
    
    // setup the render size (working area)
    transformer.setRenderSize(ofGetWidth(), ofGetHeight());
    //
    //	// turn on transform origin translation and scaling to screen size,
    //	// disable quad warping, and enable aspect ratio and centering when scaling
    transformer.setTransforms(true, true, false, true, true);
    //
    //	// set the ofxApp transformer so it's automatically applied in draw()
    setTransformer(&transformer);
    //
#ifdef HAVE_OFX_GUI
    // setup transform panel with transformer pointer,
    // loads settings & quad warper xml files if found
    panel.setup(&transformer);
#endif
    
    // load scenes & save pointers

    scene05 = (Scene05*) sceneManager.add(new Scene05());
    scene06 = (Scene06*) sceneManager.add(new Scene06());
    scene08 = (Scene08*) sceneManager.add(new Scene08());
    
    sceneManager.setup(true); // true = setup all the scenes now (not on the fly)
    ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
    
    // start with a specific scene
    // set now to true in order to ignore the scene fade and change now
    sceneManager.gotoScene("Scene 01", true);
    lastScene = sceneManager.getCurrentSceneIndex();
    
    // overlap scenes when transitioning
    sceneManager.setOverlap(true);
    
    // attach scene manager to this ofxApp so it's called automatically,
    // you can also call the callbacks (update, draw, keyPressed, etc) manually
    // if you don't set it
    //
    // you can also turn off the auto sceneManager update and draw calls with:
    // setSceneManagerUpdate(false);
    // setSceneManagerDraw(false);
    //
    // the input callbacks in your scenes will be called if they are implemented
    //
    setSceneManager(&sceneManager);
    
    isEmptyScene = !sceneManager.getCurrentScene();
    
    //////  General Audio Settings
    
    int outChannels = OUT_CHANNELS;
    int inChannels = IN_CHANNELS;
    int sampleRate = SAMPLE_RATE;
    int bufferSize = BUFFER_SIZE;
    int ticksPerBuffer = bufferSize/64;
    //    nBandsToGet = NUM_WINDOWS;
    
    //////  Essentia
    
    maxEnergy = 230.0;
    minPitch = 130.0;//-C3
    maxPitch = 2093.0;//-C7
    maxOnHfc = 536000;
    maxOnCmpx = 15.0;
    maxOnFlux = 15.0;
    maxHfc = 1000.0;
    maxCentroid = sampleRate/2;
    maxSpectralComp = 40.0;
    maxDct = 300.0;
    
    setupGui();
    
    guiShow = true;
    guiPlay = true;
    
    showHelp.set("Help  h", false);
    
    hasAutoCam.set("Camera Automation c", false);
    cameraDistance.set("Camera Distance", 120, 0, 2000);
    cam.setDistance(cameraDistance);
    
    //    sceneName = "Press h for help";
    
}

//--------------------------------------------------------------
void ofApp::update() {
    
    // the current scene is automatically updated before this function
    
#ifdef HAVE_OFX_GUI
    // update the transform panel when in debug mode
    if(isDebug()) {
        panel.update();
    }
#endif
    
    if (guiPlay) {
        updateGui();
    }
    
    if (guiShow) {
        menuBarPanel->getVisible().set(true);
        menuAudioPanel->getVisible().set(true);
        menuCameraPanel->getVisible().set(true);
        menuEffectsPanel->getVisible().set(true);
        menuSettingsPanel->getVisible().set(true);
        menuBarPanel->getVisible().set(true);
        scenesPanel->getVisible().set(true);
        
    } else {
        menuBarPanel->getVisible().set(false);
        menuAudioPanel->getVisible().set(false);
        menuCameraPanel->getVisible().set(false);
        menuEffectsPanel->getVisible().set(false);
        menuSettingsPanel->getVisible().set(false);
        menuBarPanel->getVisible().set(false);
        scenesPanel->getVisible().set(false);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    // the current scene is automatically drawn before this function
    
    //    ofDrawBitmapString(status,400, 20); // Sortable Effect List Debug
    
    // show the render area edges with a white rect
    if(isDebug()) {
        ofNoFill();
        ofSetColor(255);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofDrawRectangle(1, 1, getRenderWidth()-2, getRenderHeight()-2);
        ofFill();
    }
    
    if(showHelp) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        ss << "(f): Toggle Fullscreen"<<endl<<"(\'): Draw Solid Shapes"<<endl<<"(;): Draw Wireframes"<<endl;
        ss <<"(,): Draw Normals"<<endl;
        ss <<"(h): Toggle help."<<endl <<"(/): Camera Automation."<<endl;
        ofSetColor(0,200);
        ttFont12.drawString(ss.str().c_str(), ofGetWidth()/2-100, ofGetHeight()/2-100);
        ofSetColor(250);
        ttFont12.drawString(ss.str().c_str(), ofGetWidth()/2-101, ofGetHeight()/2-101);
    }
    
    // CAM AUTOMATION
    if (hasAutoCam) {
        float time = ofGetElapsedTimef();
        float longitude = 10*time;
        float latitude = 10*sin(time*0.8);
        float radius = cameraDistance + 50*sin(time*0.4);
        cam.orbit( longitude, latitude, radius, ofPoint(0,0,0) );
    }
    
    
    // drop out of the auto transform space back to OF screen space
    transformer.pop();
    
    //	#ifdef HAVE_OFX_GUI
    //		// draw the transform panel when in debug mode
    //		if(isDebug()) {
    //			panel.draw();
    //		}
    //	#endif
    
    // then go back to the auto transform space
    //
    // this is actually done automatically if the transformer is set but
    // included here for completeness
    transformer.push();
    
    // the warp editor is drawn automatically after this function
    
}

void ofApp::setupGui(){
    
    //////  ofParameters
    
    energyToggleVal.set("Enable", false);
    energyMultiplierSliderVal.set("Multiplier", 0.5, 0.1, 1.0);
    intenSmoothSliderVal.set("Smoothing", 0.0, 0.0, 1.0);
    energySliderVal.set("Energy", 0.0, 0.0, maxEnergy);
    rmsSliderVal.set("RMS", log10(0.001), log10(0.001), log10(1.0));
    //    powerSliderVal.set("Power", log10(0.001), log10(0.000001), log10(1.0));
    
    pitchToggleVal.set("Enable", false);
    pitchPlotterVal.set("Pitch", minPitch, minPitch, maxPitch);
    pitchConfSliderVal.set("Pitch Confidence", 0.0, 0.0, 1.0);
    pitchSmoothSliderVal.set("Smoothing", 0.0, 0.0, 1.0);
    salienceSliderVal.set("Salience", 0.0, 0.0, 1.0);
    
    beatToggleVal.set("Enable", false);
    onsetAlphaSliderVal.set("Alpha", 0.1, 0.0, 1.0);
    onsetThresholdSliderVal.set("Threshold", 0.02, 0.0, 1.0);
    onsetHfcSliderVal.set("Onset HFC", 0.0, log10(1.0), log10(maxOnHfc));
    onsetComplexSliderVal.set("Onset COMPLEX", 0.0, 0.0, maxOnCmpx);
    onsetFluxSliderVal.set("Onset FLUX", 0.0, 0.0, maxOnFlux);
    onsetsToggleVal.set("Onsets", false);
    
    melodyToggleVal.set("Enable", false);
    spectrumPlotterVal.set("Spectrum Plotter", log10(0.001), log10(0.001), log10(1.0));
    melBandsPlotterVal.set("MelBands Plotter", log10(0.001), log10(0.001), log10(1.0));
    mfccPlotterVal.set("MFCP Plotter", 0.0, 0.0, maxDct);
    hpcpPlotterVal.set("HPCP Plotter", 0.0, 0.0, 1.0);
    
    spectralToggleVal.set("Enable", false);
    hfcPlotterVal.set("HFC", 0.0, 0.0, maxHfc);
    centroidPlotterVal.set("Centroid", 0.0, 0.0, maxCentroid);
    specCmplxPlotterVal.set("Spectral Complexity", 0.0, 0.0, maxSpectralComp);
    inharmonPlotterVal.set("Inharmonicity", 0.0, 0.0, 1.0);
    specSmoothSliderVal.set("Spectral Smoothing", 0.0, 0.0, 1.0);
    
    hasBleachBypass.set("Bleach Bypass", false);
    hasBloom.set("Bloom", false);
    hasContrast.set("Contrast", false);
    hasConvolution.set("Convolution", false);
    hasDofAlt.set("Depth of Field ALt.", false);
    hasDof.set("Depth of Field", false);
    hasEdge.set("Edge", false);
    hasFakeSSS.set("FakesSSS", false);
    hasFxaa.set("FXAA anti-aliasing", false);
    hasGodRays.set("God Rays", false);
    hasHorizontalTiltShift.set("Horiz. Tilt Shift", false);
    hasHsbShift.set("HSB Shift", false);
    hasKaleidoscope.set("Kaleidoscope", false);
    hasLimbDarkening.set("Limb Darkening", false);
    hasLUT.set("Look Up Table", false);
    hasNoiseWarp.set("Noise Warp", false);
    hasPixelate.set("Pixelate", false);
    hasRender.set("Render", false);
    hasRGBShift.set("RGB Shift", false);
    hasRimHighliting.set("Rim Highlighting", false);
    hasSSAO.set("SSAO", false);
    hasToon.set("Toon", false);
    hasVerticalTiltShift.set("Vert. Tilt Shift", false);
    hasZoomBlur.set("Zoom Blur", false);
    
    unlockPanels.set("Unlock Panels", false);
    cleanUpPanelsButton.set("Clean Panels");
    quitButton.set("quit");
    
    //////  Panel Setup
    
    gui.setupFlexBoxLayout();
    
    menuAudioPanel = gui.addPanel("AUDIO FACTORY");
    menuAudioPanel->loadTheme("assets/themes/theme_bleurgh.json");
    
    menuAudioPanel->setPosition(0,0);
    menuAudioPanel->setShowHeader(false);
    
    
    //////  Custom element styling. TODO: move to theme
    
    ofJson toggleTheme = {{"show-name", false}, {"width", "10%"}};
    ofJson postToggleTheme = {{"fill-color", "rgba(220,80,50,0.5)"}, {"width", "90%"}};
    ofJson postToggle2Theme = {{"fill-color", "rgba(220,80,50,0.5)"}, {"width", "45%"}};
    ofJson groupTheme = {{"flex-direction", "row"}, {"flex-wrap", "wrap"}, {"width", 320}, {"align-content", "space-between"}};
    
    //////  Groups
    
    menuAudioGroup = menuAudioPanel->addGroup("Audio Analyzer");
    
    powerGroup = menuAudioGroup->addGroup("POWER", groupTheme);
    powerGroup->add(energyToggleVal, toggleTheme);
    powerGroup->add(energyMultiplierSliderVal, postToggle2Theme);
    powerGroup->add(intenSmoothSliderVal, postToggle2Theme);
    powerGroup->add<ofxGuiValuePlotter>(energySliderVal);
    powerGroup->add(rmsSliderVal);
    powerGroup->minimize();
    
    pitchGroup = menuAudioGroup->addGroup("PITCH", groupTheme);
    pitchGroup->add(pitchToggleVal, toggleTheme);
    pitchGroup->add(pitchSmoothSliderVal, postToggleTheme);
    pitchGroup->add<ofxGuiValuePlotter>(pitchPlotterVal);
    pitchGroup->add(pitchConfSliderVal);
    pitchGroup->add(salienceSliderVal);
    //    pitchGroup->add(tuningSliderVal);
    pitchGroup->minimize();
    
    beatGroup = menuAudioGroup->addGroup("BEAT", groupTheme);
    beatGroup->add(beatToggleVal, toggleTheme);
    beatGroup->add(onsetAlphaSliderVal, postToggle2Theme);
    beatGroup->add(onsetThresholdSliderVal, postToggle2Theme);
    beatGroup->add(onsetHfcSliderVal);
    beatGroup->add(onsetComplexSliderVal);
    beatGroup->add(onsetFluxSliderVal);
    
    beatGroup->add(onsetsToggleVal, ofJson({{"type", "radio"}, {"height", 60}}));
    beatGroup->minimize();
    
    melodyGroup = menuAudioGroup->addGroup("MELODY", groupTheme);
    melodyGroup->add(melodyToggleVal);
    melodyGroup->add<ofxGuiValuePlotter>(spectrumPlotterVal);
    melodyGroup->add<ofxGuiValuePlotter>(melBandsPlotterVal);
    melodyGroup->add<ofxGuiValuePlotter>(mfccPlotterVal);
    melodyGroup->add<ofxGuiValuePlotter>(hpcpPlotterVal);
    melodyGroup->minimize();
    
    spectralGroup = menuAudioGroup->addGroup("SPECTRAL", groupTheme);
    spectralGroup->add(spectralToggleVal, toggleTheme);
    spectralGroup->add(specSmoothSliderVal, postToggleTheme);
    spectralGroup->add<ofxGuiValuePlotter>(hfcPlotterVal);
    spectralGroup->add<ofxGuiValuePlotter>(centroidPlotterVal);
    spectralGroup->add<ofxGuiValuePlotter>(specCmplxPlotterVal);
    spectralGroup->add<ofxGuiValuePlotter>(inharmonPlotterVal);
    spectralGroup->minimize();
    
    menuAudioGroup->minimize();
    
    //////  Menubar panels
    
    //// 1st Menu Dropdown (CAMERA)
    
    menuCameraPanel = gui.addPanel("Camera", ofJson({{"width", 200}, {"border-color", "rgba(0,0,0,0.0)"}}));
    menuCameraPanel->loadTheme("assets/themes/theme_bleurgh.json");
    menuCameraPanel->setPosition(menuAudioPanel->getShape().getTopRight()+ofPoint(0,0));
    menuCameraPanel->setShowHeader(false);
    
    menuCameraGroup = menuCameraPanel->addGroup("Camera", ofJson({{"background-color", "rgba(0,0,0,0.2)"}}));
    menuCameraGroup->add(cameraDistance);
    menuCameraGroup->add(hasAutoCam);
    menuCameraGroup->minimize();
    
    //// 2nd Menu Dropdown (EFFECTS)
    
    menuEffectsPanel = gui.addPanel("Effects", ofJson({{"width", 200}, {"border-color", "rgba(0,0,0,0.0)"}}));
    menuEffectsPanel->loadTheme("assets/themes/theme_bleurgh.json");
    menuEffectsPanel->setPosition(menuCameraPanel->getShape().getTopRight()+ofPoint(0,0));
    menuEffectsPanel->setShowHeader(false);
    menuEffectsGroup = menuEffectsPanel->addGroup("Effects", groupTheme);
    effect_toggles = menuEffectsGroup->add<ofxSortableList>("Post Processing");
    effect_toggles->add(hasBleachBypass);
    effect_toggles->add(hasBloom);
    effect_toggles->add(hasContrast);
    effect_toggles->add(hasConvolution);
    effect_toggles->add(hasDof);
    effect_toggles->add(hasEdge);
    effect_toggles->add(hasFxaa);
    effect_toggles->add(hasGodRays);
    effect_toggles->add(hasHorizontalTiltShift);
    effect_toggles->add(hasHsbShift);
    effect_toggles->add(hasKaleidoscope);
    effect_toggles->add(hasLimbDarkening);
    effect_toggles->add(hasLUT);
    effect_toggles->add(hasNoiseWarp);
    effect_toggles->add(hasPixelate);
    effect_toggles->add(hasToon);
    effect_toggles->add(hasVerticalTiltShift);
    effect_toggles->add(hasZoomBlur);
    
    menuTransformGroup = menuEffectsGroup->addGroup("Transformer", groupTheme);
    menuTransformGroup->add<ofxGuiLabel>("text without parameter");
    menuTransformGroup->minimize();
    menuEffectsGroup->minimize();
    
    //// 3rd Menu Dropdown (SETTINGS)
    
    menuSettingsPanel = gui.addPanel("Settings", ofJson({{"width", 200}, {"border-color", "rgba(0,0,0,0.0)"}}));
    menuSettingsPanel->loadTheme("assets/themes/theme_bleurgh.json");
    menuSettingsPanel->setPosition(menuEffectsPanel->getShape().getTopRight()+ofPoint(0,0));
    menuSettingsPanel->setShowHeader(false);
    
    menuSettingsGroup = menuSettingsPanel->addGroup("Settings", ofJson({{"background-color", "rgba(0,0,0,0.2)"}}));
    menuSettingsGroup->add(showHelp);
    menuSettingsGroup->add(quitButton); //  button to quit app
    
    labelsGroup = menuSettingsGroup->addGroup("labels");
    labelsGroup->add(activeName);
    labelsGroup->add<ofxGuiIntLabel>(activeIndex);
    labelsGroup->add<ofxGuiLabel>("text without parameter");
    labelsGroup->minimize();
    
    menuSettingsGuiGroup = menuSettingsGroup->addGroup("GUI", ofJson({{"background-color", "rgba(0,0,0,0.2)"}}));
    menuSettingsGuiPanelsGroup = menuSettingsGuiGroup->addGroup("Panels", ofJson({{"background-color", "rgba(0,0,0,0.2)"}}));
    menuSettingsGuiPanelsGroup->add(unlockPanels); //  toggle to show or hide header
    menuSettingsGuiPanelsGroup->add(cleanUpPanelsButton); //  toggle to clean panels
    
    colorParameters.setName("Header Colours");
    colorParameters.add(color0.set("black",false));
    colorParameters.add(color1.set("orange",false));
    colorParameters.add(color2.set("transparent",false));
    
    colorToggles = menuSettingsGuiPanelsGroup->addGroup("Header Colour");
    colorToggles->add(colorParameters);
    colorToggles->setExclusiveToggles(true);
    colorToggles->minimize();
    
    menuSettingsGuiPanelsGroup->minimize();
    menuSettingsGroup->minimize();
    
    //// Last Menu Dropdown (SCENE)
    
    menuScenePanel = gui.addPanel("Scene", ofJson({{"width", ofGetWidth()}}));
    menuScenePanel->loadTheme("assets/themes/theme_menu.json");
    menuScenePanel->setPosition(ofGetWidth()-330, 0);
    menuScenePanel->setShowHeader(false);
    menuScenePanel->addSpacer(0, 32);
    
    //////  Menubar background hack
    
    menuBarPanel = gui.addPanel("menubar", ofJson({{"width", ofGetWidth()},{"flex-direction", "row"}, {"background-color", "rgba(0,0,0,0.2)"}}));
    menuBarPanel->loadTheme("assets/themes/theme_menu.json");
    menuBarPanel->setPosition(0,0);
    menuBarPanel->addSpacer(0, 32);
    
    //////  Scenes panel
    
    scenesPanel = gui.addPanel("Scenes", ofJson({{"width", ofGetWidth()}, {"background-color", "rgba(0,0,0,0.5)"}, {"show-header", false}}));
    scenesPanel->loadTheme("assets/themes/theme_menu.json");
    scenesPanel->setPosition(0,ofGetHeight()-32);
    scenesPanel->add(sceneName);
    
    
    //    // add labels with random width to the first page
    //    for(int i = 0; i < 5; i++){
    //        menuScenePanel->addLabel(ofToString(i), ofJson({
    //            {"border-width", 1},
    //            {"width", ofRandom(30, 130)}
    //        }));
    //    }
    
    //////  adding listeners
    
    unlockPanels.addListener(this, &ofApp::toggleGroupHeader);
    cleanUpPanelsButton.addListener(this, &ofApp::cleanUpPanels);
    quitButton.addListener(this, &ofApp::quitApp);
    colorToggles->getActiveToggleIndex().addListener(this, &ofApp::setHeaderColors);
    colorToggles->setActiveToggle(3);
    
    ofAddListener(effect_toggles->elementRemoved, this, &ofApp::removed);
    ofAddListener(effect_toggles->elementMoved, this, &ofApp::moved);
    
}

void ofApp::quitApp() {
    
    ofExit();
}

void ofApp::updateGui(){
    
}

void ofApp::setHeaderColors(int& index){
    
    activeIndex = index;
    activeName = colorParameters.get(index).getName();
    
    ofColor c;
    switch(index){
        case 0: c = ofColor::black; break;
        case 1: c = ofColor(220,80,50); break;
        default: case 2: c = ofColor(0,0,0,0); break;
    }
    
    menuAudioPanel->getHeader()->setBackgroundColor(c);
    menuCameraPanel->getHeader()->setBackgroundColor(c);
    menuEffectsPanel->getHeader()->setBackgroundColor(c);
    menuSettingsPanel->getHeader()->setBackgroundColor(c);
    scenesPanel->getHeader()->setBackgroundColor(c);
    menuScenePanel->getHeader()->setBackgroundColor(c);
    
    color = c;
}

void ofApp::toggleGroupHeader(bool & val){
    
    menuAudioPanel->setShowHeader(val);
    menuCameraPanel->setShowHeader(val);
    menuEffectsPanel->setShowHeader(val);
    menuSettingsPanel->setShowHeader(val);
    scenesPanel->setShowHeader(val);
    menuScenePanel->setShowHeader(val);
}

void ofApp::cleanUpPanels(){
    
    menuAudioPanel->setPosition(0,0);
    menuCameraPanel->setPosition(menuAudioPanel->getShape().getTopRight()+ofPoint(0,0));
    menuEffectsPanel->setPosition(menuCameraPanel->getShape().getTopRight()+ofPoint(0,0));
    menuSettingsPanel->setPosition(menuEffectsPanel->getShape().getTopRight()+ofPoint(0,0));
    menuScenePanel->setPosition(ofGetWidth()-330, 0);
    scenesPanel->setPosition(0,ofGetHeight()-32);
}


float ofApp::smooth(float last, float value,  float amnt){
    
    float result;
    result = last * amnt;
    result += (1-amnt) * value;
    return result;
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    for (int i = 0; i < bufferSize; i++){
        left[i]	= input[i*nChannels];
        right[i]	= input[i*nChannels+1];
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    cout<<"Closing..."<<endl;
    
    // clean up:
    ofRemoveListener(effect_toggles->elementRemoved, this, &ofApp::removed);
    ofRemoveListener(effect_toggles->elementMoved, this, &ofApp::moved);
    quitButton.removeListener(this, &ofApp::quitApp);
    unlockPanels.removeListener(this, &ofApp::toggleGroupHeader);
    cout<<"Listeners removed"<<endl;
    ofSoundStreamStop();
    ofSoundStreamClose();
    cout<<"SoundStream Stopped"<<endl;
    
    cout<<"*******************************"<<endl;
    cout<<"CLOSED SUCCESFULLY!--"<<endl;
    cout<<"*******************************"<<endl;
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
    switch (key) {
            
        case 'd':
            bDebug = !bDebug;
            break;
            
        case 'a':
            transformer.setAspect(!transformer.getAspect());
            break;
            
        case 'b':
            transformer.setCentering(!transformer.getCentering());
            break;
            
        case 'c':
            hasAutoCam = !hasAutoCam;
            break;
            
        case 'm':
            transformer.setMirrorX(!transformer.getMirrorX());
            break;
            
        case 'n':
            transformer.setMirrorY(!transformer.getMirrorY());
            break;
            
        case 'q':
            transformer.setWarp(!transformer.getWarp());
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case OF_KEY_LEFT:
            sceneManager.prevScene();
            break;
            
        case OF_KEY_RIGHT:
            sceneManager.nextScene();
            break;
            
        case '0':
            isEmptyScene = !isEmptyScene;
            if(sceneManager.getCurrentScene()) { // returns NULL if no scene selected
                lastScene = sceneManager.getCurrentSceneIndex();
            }
            if (!isEmptyScene) {
                sceneManager.noScene();
            } else {
                sceneManager.gotoScene(lastScene);
            }
            break;
            
        case 'o':
            sceneManager.setOverlap(!sceneManager.getOverlap());
            break;
            
        case ' ':
            if (!guiShow){
                guiShow = true;
            } else {
                guiShow = false;
            }
            break;
            
        case 'p':
            if (!guiPlay){
                guiPlay = true;
            } else {
                guiPlay = false;
            }
            break;
            
        case '5':
            if(sceneManager.getCurrentScene() != scene05) {
                sceneManager.gotoScene(4);
            }
            break;
            
            
        case '6':
            if(sceneManager.getCurrentScene() != scene06) {
                sceneManager.gotoScene(5);
            }
            break;
            
        case '8':
            if(sceneManager.getCurrentScene() != scene08) {
                sceneManager.gotoScene(7);
            }
            break;
            
        case '/':
            hasAutoCam = !hasAutoCam;
            break;
            
        case 'h':
            showHelp = !showHelp;
            break;
            
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    // transformer.setNewScreenSize() is automatically called if the transformer is set
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//gets called by "sortable_panel" when an item is dragged outside of panel
void ofApp::removed(RemovedElementData& data) {
    status += "removed element with index " + ofToString(data.index) + "\n";
}

//gets called by "sortable_panel" when items get moved
void ofApp::moved(MovingElementData &data) {
    status += "moved element from index " + ofToString(data.old_index) + " to index " + ofToString(data.new_index) + "\n";
}
