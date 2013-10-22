
#include <MIDI.h>

//
// 1) LoopBe1 Internal MIDI loopback
//     http://www.nerds.de/en/loopbe1.html
// 2) Serial to MIDI:
//     http://projectgus.github.io/hairless-midiserial/
// 3) Connect Hairless Serial MIDI to LoopBe1 Internal output
// 4) Set LoopBe1 Internal output as an Input to Ableton
// 

//
// Serial MIDI device:
//  Baud: 38400
//  Channel: 1

static const int kMidiBaud = 115200;
static const int kMidiChannel = 1;
static const int kDotCount = 13;
static char sPitchMap[kDotCount];

static const int kMidiBegin = 30;
static const int kMidiEnd = 85;
static const int kVelocity = 64;

static int sDot = 0;

void setup()
{
  // Seed pitch map table with something...
  for (int dot = 0; dot < kDotCount; dot++)
  {
    sPitchMap[dot] = map(dot, 0, kDotCount - 1, kMidiBegin, kMidiEnd);
  }
  
  MIDI.begin(kMidiBaud, kMidiChannel);
}

void loop()
{
  MIDI.sendNoteOn(sPitchMap[sDot], kVelocity, kMidiChannel);
  delay(100);
  //MIDI.sendNoteOff(sPitchMap[sDot], kVelocity, kMidiChannel);
  delay(1000);
  
  sDot = (sDot + 1) % kDotCount;
}

