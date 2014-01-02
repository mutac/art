#include "pingPongFbo.h"

pingPongFbo::pingPongFbo()
{
  mSrc = &mBufs[0];
  mDst = &mBufs[1];
}

void pingPongFbo::allocate(int width, int height, int internalformat, int numSamples)
{
  for (int idx = 0; idx < kNumBufs; idx++)
  {
    mBufs[idx].allocate(width, height, internalformat, numSamples);
    mBufs[idx].begin();
      ofClear(0, 0, 0, 0);
    mBufs[idx].end();
  }
}

void pingPongFbo::allocate(ofFbo::Settings settings)
{
  for (int idx = 0; idx < kNumBufs; idx++)
  {
    mBufs[idx].allocate(settings);
    mBufs[idx].begin();
      ofClear(0, 0, 0, 0);
    mBufs[idx].end();
  }
}

void pingPongFbo::swap()
{
  ofFbo* temp = mSrc;
  mSrc = mDst;
  mDst = temp;
}
