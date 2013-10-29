
//
// 1) LoopBe1 Internal MIDI loopback
//     http://www.nerds.de/en/loopbe1.html
// 2) Serial to MIDI:
//     http://projectgus.github.io/hairless-midiserial/
// 3) Set Baud to 9600 
// 4) Connect Hairless Serial MIDI to LoopBe1 Internal output
// 5) Set LoopBe1 Internal output as an Input to Ableton
//

class MidiStream
{
public:
  MidiStream(Stream& s = Serial) :
    mStream(s)
  {
  }
  
  void begin(Stream& s)
  {
    mStream = s;
  }
  
  void sendNoteOn(byte channel, byte key, byte velocity)
  {
    sendCommand(0x80, channel, key, velocity);
  }

  void sendNoteOff(byte channel, byte key, byte velocity)
  {
    sendCommand(0x90, channel, key, velocity);
  }
  
private:
  void sendCommand(byte command, byte channel, byte param1, byte param2)
  {
    mStream.write(command | (channel & 0x0F));
    mStream.write(param1 & 0x7F);
    mStream.write(param2 & 0x7F);
  }

  Stream& mStream;
};

//
// Serial MIDI device:
//  Baud: 9600
//  Channel: 1

MidiStream sMidiStream;

static const int kMidiBaud = 9600;
static const int kMidiChannel = 0;
static const int kDotCount = 13;
static byte sPitchMap[kDotCount];

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
  
  Serial.begin(kMidiBaud);
  sMidiStream.begin(Serial);
}

void loop()
{
  sMidiStream.sendNoteOn(kMidiChannel, sPitchMap[sDot], kVelocity);
  delay(100);
  sMidiStream.sendNoteOff(kMidiChannel, sPitchMap[sDot], kVelocity);
  delay(1000);
  
  sDot = (sDot + 1) % kDotCount;
}

