
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
    sendCommand(0x90, channel, key, velocity);
  }

  void sendNoteOff(byte channel, byte key, byte velocity)
  {
    sendCommand(0x80, channel, key, velocity);
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

class MusicBoxPip
{
public:
  static const int kMaxTime = 1000;
  static const int kMinVelocity = 1;
  static const int kMaxVelocity = 128;
  
  enum Transition
  {
    eTransitioned_None,
    eTransitioned_On,
    eTransitioned_Off
  };

public:
  MusicBoxPip() :
    mPin(0),
    mVelocity(0),
    mWasOn(false),
    mStartTime(0)
  {
  }
  
  void begin(int pin)
  {
    mPin = pin;
    pinMode(mPin, INPUT);
    mWasOn = isOnNow();
    mStartTime = millis();
  }
  
  Transition checkTransition()
  {
    bool isOn = isOnNow();
    Transition transition = eTransitioned_None;
    
    if (!mWasOn && isOn)
    {
      calculateTransitionVelocity();
      transition = eTransitioned_On;
    }
    else if(mWasOn && !isOn)
    {
      calculateTransitionVelocity();
      transition = eTransitioned_Off;
    }
    
    mWasOn = isOn;
    return transition;
  }
  
  int transitionVelocity()
  {
    return mVelocity;
  }
  
private:

  bool isOnNow()
  {
    return digitalRead(mPin) <= 0;
  }

  void calculateTransitionVelocity()
  {
    unsigned long now = millis();
    unsigned long elapsed = now - mStartTime;
    mStartTime = now;
    
    if (elapsed > kMaxTime)
    {
      elapsed = kMaxTime;
    }
    
    mVelocity = map(kMaxTime - elapsed, 0, kMaxTime, kMinVelocity, kMaxVelocity);
  }

  int mPin;
  bool mWasOn;
  unsigned long mStartTime;
  int mVelocity;
};

//
// Serial MIDI device:
//  Baud: 9600
//  Channel: 1

MidiStream sMidiStream;

static const int kMidiBaud = 9600;
static const int kMidiChannel = 0;

static const int kMidiNoteBegin = 52;
static const int kMidiNoteEnd = 58;

static const int kPipCount = 6;
static MusicBoxPip sMusicBoxPips[kPipCount];
static byte sPitchMap[kPipCount];
static int sPinMap[] = {13, 12, 11, 10, 9, 8};

void setup()
{
  
  // Seed pitch map table with something...
  for (int pip = 0; pip < kPipCount; pip++)
  {
    sMusicBoxPips[pip].begin(sPinMap[pip]);
    sPitchMap[pip] = map(pip, 0, kPipCount - 1, kMidiNoteBegin, kMidiNoteEnd);
  }
  
  Serial.begin(kMidiBaud);
  sMidiStream.begin(Serial);
}

void loop()
{
  for (int pip = 0; pip < kPipCount; pip++)
  {
    MusicBoxPip::Transition transition = sMusicBoxPips[pip].checkTransition();
    
    if (transition == MusicBoxPip::eTransitioned_On)
    {
      sMidiStream.sendNoteOn(kMidiChannel, sPitchMap[pip], sMusicBoxPips[pip].transitionVelocity());
    }
    else if (transition == MusicBoxPip::eTransitioned_Off)
    {
      sMidiStream.sendNoteOff(kMidiChannel, sPitchMap[pip], sMusicBoxPips[pip].transitionVelocity());
    }
  }
}


