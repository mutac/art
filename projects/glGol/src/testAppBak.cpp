#include "testApp.h"
#include <cstdlib>
#include <algorithm>

//--------------------------------------------------------------
void testApp::setup()
{
  ofSetLogLevel(OF_LOG_VERBOSE);

  ofEnableAlphaBlending();

  curFboIdx = 0;
  fboCount = 2;

  int height = Height;
  int width = Width;

  for (int idx = 0; idx < fboCount; idx++)
  {
    mFbos[idx].allocate(width, height, GL_RGBA);
    mFbos[idx].begin();
      ofClear(0, 0, 0, 255);
      ofSetColor(0, 0, 0, 255);
      ofRect(0, 0, width, height);
      /*
      ofSetCircleResolution(60);
      ofSetColor(0, 0, 0, 0);
      ofCircle(width / 2, height / 2,
               (std::min(width, height) / 2) - 20);
      */
    mFbos[idx].end();

    ofLogVerbose() << "Buffer " << idx
      << ": textureCount=" << mFbos[idx].getNumTextures();
  }

  mShader.load("", "shaders/test.frag");
  mShader.bindDefaults();

  if (!mShader.isLoaded())
  {
    ofLog() << "Failed to load shader.";
    std::exit(1);
  }

  mImage.loadImage("images/skin.jpg");
}

//--------------------------------------------------------------
ofFbo* testApp::curFbo()
{
  return &mFbos[curFboIdx];
}

//--------------------------------------------------------------
ofFbo* testApp::nextFbo()
{
  curFboIdx = (curFboIdx + 1) % fboCount;
  return &mFbos[curFboIdx];
}

//--------------------------------------------------------------
void testApp::update()
{
}

//--------------------------------------------------------------
void testApp::draw()
{
  int height = Height;
  int width = Width;

  ofFbo* lastFbo = curFbo();
  ofFbo* fbo = nextFbo();

  lastFbo->begin();
    ofClear(0, 0, 0, 255);

    ofSetColor(255, 255, 255, 255);
    mImage.draw(0, 0);

    //ofSetColor(0, 0, 0);
    //ofCircle(width / 2, height / 2, 40);
  lastFbo->end();

  fbo->begin();
    ofClear(0, 0, 0, 255);

    mShader.begin();
      ofSetColor(255, 255, 255, 255);
      ofSetCircleResolution(60);
      ofCircle(width / 2, height / 2,
               (std::min(width, height) / 2) - 20);

      mShader.setUniformTexture("tex0", lastFbo->getTextureReference(), 1);
      mShader.setUniform2f("resolution", width, height);
      mShader.setUniform1f("time", ofGetElapsedTimef());
      mShader.setUniform2f("mouse", (float)mouseX / width, (float)mouseY / height);
    mShader.end();
  fbo->end();

  fbo->draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
