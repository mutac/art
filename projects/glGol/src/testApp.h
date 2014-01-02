#pragma once

#include "ofMain.h"
#include "pingPongFbo.h"

class testApp : public ofBaseApp
{
public:
  static const int Width = 512;
  static const int Height = 512;

public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void loadShaders();
private:
		ofShader mGameShader;
		ofShader mWarpShader;
		ofShader mLenseShader;

		pingPongFbo mFbos;
		ofPlanePrimitive mPlane;
		ofFbo mRenderFbo;

    bool mMouseDown;
    bool mSpawn;
    bool mReset;
    bool mDoRotate;

    float mAngle;
    float mMouseDx;
    int mMouseX0;
    float mMouseX0Time;

    int mFrame;
};

