#pragma once

#include "ofMain.h"

class pingPongFbo
{
public:
  pingPongFbo();

  void allocate(int width, int height, int internalformat = GL_RGBA, int numSamples = 0);
	void allocate(ofFbo::Settings settings = ofFbo::Settings());

	void swap();

	ofFbo& src()
	{
	  return *mSrc;
	}

	ofFbo& dst()
	{
	  return *mDst;
	}

private:
  static const int kNumBufs = 2;
  ofFbo* mSrc;
  ofFbo* mDst;
  ofFbo mBufs[kNumBufs];
};

