
#include <MIDI.h>

//
// Serial MIDI device:
//  Baud: 31250
//  Channel: 1

static const int sMidiChannel = 1;
static const int sDotCount = 13;
static char sPitchMap[sDotCount];

static const int sMidiBegin = 30;
static const int sMidiEnd = 85;

void setup()
{
  for (int dot = 0; dot < sDotCount; dot++)
  {
    sPitchMap[dot] = map(dot, 0, sDotCount - 1, sMidiBegin, sMidiEnd);
  }
  
  MIDI.begin(sMidiChannel);
}

void loop()
{
}

