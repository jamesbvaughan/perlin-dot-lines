#include "ofApp.h"

int windowWidth;
int windowHeight;
float gapBetweenDots;
int frame = 0;
int lines = 20;
int dotsPerLine = 200;
int dotRadius = 2;
float waveSpeed = 0.004;
float lineSpread = 0.03;
float wavelength = 0.005;
float flowSpeed = -0.003;

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackgroundHex(0x000000);
  windowWidth = ofGetWidth();
  windowHeight = ofGetHeight();
  gapBetweenDots = ((float) windowWidth) / dotsPerLine;

	midiIn.listInPorts();
	midiIn.openPort(1);
	midiIn.addListener(this);
}

//--------------------------------------------------------------
void ofApp::update(){
  frame++;
}

//--------------------------------------------------------------
float noise;
void ofApp::draw(){
  for (int line = 0; line < lines; line++) {
    for (int dot = 0; dot < dotsPerLine + 2; dot++) {
      noise = ofSignedNoise(
        waveSpeed * frame,
        lineSpread * line,
        wavelength * dot + flowSpeed * frame
      );

      ofDrawCircle(
        dot * gapBetweenDots,
        ofMap(noise, -1.0, 1.0, 0.0, (float) windowHeight),
        dotRadius
      );
    }
  }

  ofSetColor(ofColor::white);
  ofDrawBitmapString("Wave Speed: " +    ofToString(waveSpeed),   10, 30);
  ofDrawBitmapString("Line Spread: " +   ofToString(lineSpread),  10, 50);
  ofDrawBitmapString("Flow Speed: " +    ofToString(-flowSpeed),  10, 70);
  ofDrawBitmapString("Wavelength: " +    ofToString(wavelength),  10, 90);
  ofDrawBitmapString("Dot Size: " +      ofToString(dotRadius),   10, 110);
  ofDrawBitmapString("Lines: " +         ofToString(lines),       10, 130);
  ofDrawBitmapString("Dots Per Line: " + ofToString(dotsPerLine), 10, 150);
}

//--------------------------------------------------------------
void ofApp::exit() {
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
  switch(msg.control) {
    case 10:
      waveSpeed = ofMap(msg.value, 0.0, 127.0, 0.0, 0.05);
      break;
    case 74:
      lineSpread = ofMap(msg.value, 0.0, 127.0, 0.0, 0.05);
      break;
    case 71:
      flowSpeed = ofMap(msg.value, 0.0, 127.0, 0.0, -0.03);
      break;
    case 76:
      wavelength = ofMap(msg.value, 0.0, 127.0, 0.0, 0.02);
      break;
    case 77:
      dotRadius = (int) ofMap(msg.value, 0.0, 127.0, 0.0, 64.0);
      break;
    case 93:
      lines = (int) ofMap(msg.value, 0.0, 127.0, 0.0, 256.0);
      break;
    case 73:
      dotsPerLine = (int) ofMap(msg.value, 0.0, 127.0, 0.0, 512.0);
      gapBetweenDots = (windowWidth / dotsPerLine) + 2;
      break;
  }
}
