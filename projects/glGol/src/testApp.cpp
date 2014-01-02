#include "testApp.h"
#include <cstdlib>
#include <algorithm>

void testApp::loadShaders()
{
  mGameShader.load("shaders/procShader.vert", "shaders/gameOfLife.frag");
  if (!mGameShader.isLoaded())
  {
    ofLog() << "Failed to load shader.";
    std::exit(1);
  }
  mWarpShader.load("shaders/procShader.vert", "shaders/warp3.frag");
  if (!mWarpShader.isLoaded())
  {
    ofLog() << "Failed to load shader.";
    std::exit(1);
  }
  mLenseShader.load("shaders/procShader.vert", "shaders/barrel.frag");
  if (!mLenseShader.isLoaded())
  {
    ofLog() << "Failed to load shader.";
    std::exit(1);
  }
}

//--------------------------------------------------------------
void testApp::setup()
{
  ofSetLogLevel(OF_LOG_VERBOSE);

  //ofEnableAlphaBlending();

  int height = Height;
  int width = Width;

  mMouseDown = false;
  mReset = false;
  mSpawn = true;
  mDoRotate = true;

  mAngle = 0.0;
  mFrame = 0;
  mMouseDx = 0.0;
  mMouseX0 = mouseX;
  mMouseX0Time = ofGetElapsedTimef();

  mFbos.allocate(width, height, GL_RGBA);
  mRenderFbo.allocate(width, height, GL_RGBA);
  mPlane.set(width * 2, height * 2); // why does this need to be x2?

  loadShaders();
}

//--------------------------------------------------------------
void testApp::update()
{
  int height = Height;
  int width = Width;

  float nMouseX = (float)mouseX / width;
  float nMouseY = (float)mouseY / height;

  //
  // Seed frame
  //
  if (mReset)
  {
    mFbos.src().begin();
      ofClear(0, 0, 0, 0);
    mFbos.src().end();
    mReset = false;
  }

  mMouseDx = std::abs(mMouseX0 - mouseX) / std::abs(mMouseX0Time - ofGetElapsedTimef() + 1.0);
  mMouseX0Time = ofGetElapsedTimef();
  mMouseX0 = mouseX;

  //
  // Compute next generation
  //
  mFbos.dst().begin();
    ofClear(0, 0, 0, 0);

    mGameShader.begin();
      // Pass previous generation in to the shader
      ofTexture& srcTex = mFbos.src().getTextureReference();
      mGameShader.setUniformTexture("lastFrame", srcTex, srcTex.getTextureData().textureID);
      mGameShader.setUniform2f("resolution", width, height);
      mGameShader.setUniform1f("time", ofGetElapsedTimef());
      mGameShader.setUniform2f("mouse", nMouseX, nMouseY);
      mGameShader.setUniform2f("dMouse", mMouseDx, 0.0);
      mGameShader.setUniform1i("frame", ofGetFrameNum());

      // Apply shader to the entire frame
      ofFill();
      mPlane.draw();
    mGameShader.end();
  mFbos.dst().end();

  mRenderFbo.begin();
    mWarpShader.begin();
      mWarpShader.setUniform2f("resolution", width, height);
      mWarpShader.setUniform1f("time", ofGetElapsedTimef());
      mWarpShader.setUniform2f("mouse", nMouseX, nMouseY);
      mWarpShader.setUniform2f("dMouse", mMouseDx, 0.0);
      mWarpShader.setUniform1i("frame", ofGetFrameNum());

      // Draw the computed generation, and
      // prepare for the next.
      mFbos.dst().draw(0, 0);
    mWarpShader.end();

    /*
    mLenseShader.begin();
      mLenseShader.setUniform2f("resolution", width, height);
      mLenseShader.setUniform1f("time", ofGetElapsedTimef());
      mLenseShader.setUniform2f("mouse", nMouseX, nMouseY);
      mLenseShader.setUniform2f("dMouse", mMouseDx, 0.0);

      mRenderFbo.draw(0, 0);
    mLenseShader.end();
    */
  mRenderFbo.end();

/*
  mRenderFbo.begin();
    mLenseShader.begin();
      mLenseShader.setUniform2f("resolution", width, height);
      mLenseShader.setUniform1f("time", ofGetElapsedTimef());
      mLenseShader.setUniform2f("mouse", nMouseX, nMouseY);
      mLenseShader.setUniform2f("dMouse", mMouseDx, 0.0);

      mRenderFbo.draw(0, 0);
    mLenseShader.end();
  mRenderFbo.end();
*/

  mFbos.swap();
}

//--------------------------------------------------------------
void testApp::draw()
{
  float height = Height;
  float width = Width;

  float nMouseX = (float)mouseX / width;
  float nMouseY = (float)mouseY / height;

  ofPushMatrix();
    ofTranslate(width/2, height/2, 0);

    if(mDoRotate)
    {
      ofRotate(mAngle, 0, 0, 1);
    }

    ofPushMatrix();
      ofSetColor(255, 255, 255, 255);
      mRenderFbo.draw(-width / 2, -height / 2);
    ofPopMatrix();
  ofPopMatrix();

  //
  // Draw help text
  //

  ofSetColor(255);
  int txtX = 10;
  int txtY = 20;
  int txtDeltaY = 10;

  std::string frameCount = "Frame: " + ofToString(mFrame);
  ofDrawBitmapString(frameCount, txtX, txtY);
  txtY += txtDeltaY;

  ofDrawBitmapString("'r' to reset", txtX, txtY);
  txtY += txtDeltaY;

/*
  if (mSpawn)
  {
    ofDrawBitmapString("'s' to turn spawning off", txtX, txtY);
    txtY += txtDeltaY;
  }
  else
  {
    ofDrawBitmapString("'s' to turn spawning on", txtX, txtY);
    txtY += txtDeltaY;
  }
*/

  // Keep the frame count for debugging
  mFrame++;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
  if (key == 'r')
  {
    mReset = true;
    loadShaders();
  }
  else if(key == 's')
  {
    mSpawn = !mSpawn;
    ofLogVerbose() << "Spawn: " << (mSpawn ? "True" : "False");
  }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
  float height = Height;
  float width = Width;

  float nMouseX = (float)mouseX / width;
  float nMouseY = (float)mouseY / height;
  ofLogVerbose() << "MouseX: " << nMouseX;

  mAngle = ofMap(mouseX, 0, width, 0, 360);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
  mMouseDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
  mMouseDown = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
}
