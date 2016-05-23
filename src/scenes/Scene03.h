//
//  Scene03.h
//  BleurghMachine
//
//  Created by Clément Girault on 26/04/2016.
//
//


#pragma once

#include <ofxAppUtils.h>

class Scene03 : public ofxFadeScene {
    
public:
    
    // set the scene name through the base class initializer
    Scene03() : ofxFadeScene("Scene03 Mesh knot") {
        setSingleSetup(true); // do not call setup each time the scene is loaded
        setFade(0, 0); // 0 millisecond fade in/out
    }
    
    ofApp *mainApp = (ofApp *)ofxGetAppPtr();
    
    ofMesh mesh;
    ofLight light;
    ofMaterial material;
    
    //The circle parameters
    float Rad = 25;              //Radius of circle
    float circleStep = 3;        //Step size for circle motion
    int circleN = 40;            //Number of points on the circle
    
    //Current circle state
    ofPoint pos;                //Circle center
    ofPoint axeX, axyY, axyZ;   //Circle's coordinate system
    
    // scene setup
    void setup() {
        
        pos = ofPoint( 0, 0, 0 );  //Start from center of coordinate
        axeX = ofPoint( 1, 0, 0 ); //Set initial coordinate system
        axyY = ofPoint( 0, 1, 0 );
        axyZ = ofPoint( 0, 0, 1 );
        
    }
    
    // called when scene is entering, this is just a demo and this implementation is not required for this class
    void updateEnter() {
        
        // called on first enter update
        if(isEnteringFirst()) {
            ofLogNotice("Scene 03") << "update enter";
            mainApp->sceneName = "Scene 03";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateEnter();
        
        // finished entering?
        if(!isEntering()) {
            ofLogNotice("Scene 03") << "update enter done";
        }
    }
    
    // normal update
    void update() {
        addRandomCircle( mesh );
    }
    
    // called when scene is exiting, this is just a demo and this implementation is not required for this class
    void updateExit() {
        
        // called on first exit update
        if(isExitingFirst()) {
            ofLogNotice("Scene 03") << "update exit";
        }
        
        // fade scene calculates normalized alpha value for us
        ofxFadeScene::updateExit();
        
        // finished exiting?
        if(!isExiting()) {
            ofLogNotice("Scene 03") << "update exit done";
        }
    }
    
    // draw
    void draw() {
        
        ofEnableLighting();
        
        light.setPosition(ofGetWidth()/2, ofGetHeight()/2, 600);
        light.enable();
        material.begin();
        
        ofEnableDepthTest();    //Enable z-buffering
        
        mainApp->post.begin(mainApp->cam); // START OFAPP CAMERA WITH OFXPOSTPROCESSING. Use instead of easyCam.begin();
        
        ofPushMatrix();  //Store the coordinate system
        
        //Calculate the rotation angle
        float time = ofGetElapsedTimef(); //Get time in seconds
        float angle = time * 20;          //Compute the angle. We rotate at speed 20 degrees per second
        ofRotate( angle, 0, 1, 0 );       //Rotate the coordinate system along y-axis
        
        ofTranslate( -mesh.getCentroid() ); //Shift the coordinate center so the mesh will be drawn in the screen center
        
        mesh.draw();
        
        ofPopMatrix();  //Restore the coordinate system
        
        mainApp->post.end(); // END OFAPP CAMERA WITH OFXPOSTPROCESSING. Use instead of easyCam.end();
        
        ofDisableDepthTest();
        material.end();
        light.disable();
        ofDisableLighting();
    }
    
    // cleanup
    void exit() {

    }
    
    void addRandomCircle( ofMesh &mesh ){
        float time = ofGetElapsedTimef();    //Time since app started
        
        //Parameters – twisting and rotating angles and color
        float twistAngle = 5.0 * ofSignedNoise( time * 0.3 + 332.4 );
        float rotateAngle = 1.5;
        ofFloatColor color( ofNoise( time * 0.05 ),
                           ofNoise( time * 0.1 ),
                           ofNoise( time * 0.15 ));
        color.setSaturation( 1.0 );  //Make the color maximally colorful
        
        //Rotate the coordinate system of the circle
        axeX.rotate( twistAngle, axyZ );
        axyY.rotate( twistAngle, axyZ );
        
        axeX.rotate( rotateAngle, axyY );
        axyZ.rotate( rotateAngle, axyY );
        
        //Move the circle on a step
        ofPoint move = axyZ * circleStep;
        pos += move;
        
        // Add vertices
        for (int i=0; i<circleN; i++) {
            float angle = float(i) / circleN * TWO_PI;
            float x = Rad * cos( angle );
            float y = Rad * sin( angle );
            //We would like to distort this point to make the knot's surface embossed
            float distort = ofNoise( x * 0.2, y * 0.2,
                                    time * 0.2 + 30 );
            distort = ofMap( distort, 0.2, 0.8, 0.8, 1.2 );
            x *= distort;
            y *= distort;
            
            ofPoint p = axeX * x + axyY * y + pos;
            mesh.addVertex( p );
            mesh.addColor( color );
        }
        
        // Add triangles
        int base = mesh.getNumVertices() - 2 * circleN;
        
        // Check if it is not the first step and we really need to add the triangles
        if ( base >= 0 ) {
            for (int i=0; i<circleN; i++) {
                int a = base + i;
                int b = base + (i + 1) % circleN;
                int c = circleN  + a;
                int d = circleN  + b;
                mesh.addTriangle( a, b, d ); //Clock-wise
                mesh.addTriangle( a, d, c );  
            }
            setNormals( mesh ); //Update normals
        }
    }
    
    // Universal function which sets normals for the triangle mesh
    void setNormals( ofMesh &mesh ){
        
        int nV = mesh.getNumVertices(); //The number of vertices
        int nT = mesh.getNumIndices() / 3; //The number of triangles
        
        vector<ofPoint> norm( nV ); //Array for the normals
        
        //Scan all the triangles. For each triangle add its normal to norm's vectors of triangle's vertices
        for (int t=0; t<nT; t++) {
            //Get indices of the triangle t
            int i1 = mesh.getIndex( 3 * t );
            int i2 = mesh.getIndex( 3 * t + 1 );
            int i3 = mesh.getIndex( 3 * t + 2 );
            
            //Get vertices of the triangle
            const ofPoint &v1 = mesh.getVertex( i1 );
            const ofPoint &v2 = mesh.getVertex( i2 );
            const ofPoint &v3 = mesh.getVertex( i3 );
            
            ofPoint dir = ( (v2 - v1).getCrossed( v3 - v1 ) ).getNormalized(); //Compute the triangle's normal
            
            //Accumulate it to norm array for i1, i2, i3
            norm[ i1 ] += dir;
            norm[ i2 ] += dir;
            norm[ i3 ] += dir;
        }
        
        //Normalize the normal's length
        for (int i=0; i<nV; i++) {
            norm[i].normalize();
        }
        
        //Set the normals to mesh
        mesh.clearNormals();
        mesh.addNormals( norm );
    }
    
};
