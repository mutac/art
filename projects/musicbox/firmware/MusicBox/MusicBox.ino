
//
// 1) LoopBe1 Internal MIDI loopback
//     http://www.nerds.de/en/loopbe1.html
// 2) Serial to MIDI:
//     http://projectgus.github.io/hairless-midiserial/
// 3) Set Baud to 9600 
// 4) Connect Hairless Serial MIDI to LoopBe1 Internal output
// 5) Set LoopBe1 Internal output as an Input to Ableton
//

/**
 */
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

/**
 */
class Key
{
public:
  static const unsigned int kMaxTime = 1000;
  static const int kMinVelocity = 1;
  static const int kMaxVelocity = 128;
  
  enum Transition
  {
    eTransitioned_None,
    eTransitioned_On,
    eTransitioned_Off
  };

public:
  Key() :
    mPin(0),
    mWasOn(false),
    mStartTime(0),
    mVelocity(0)
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

/**
 */
template <int KeyCount>
class KeyWiring
{
public:
  KeyWiring()
  {
  }
  
  void begin(const int pinFromKey[KeyCount])
  {
    for (int key = 0; key < KeyCount; key++)
    {
      mPinFromKey[key] = pinFromKey[key];
    }
  }
  
  int pin(int key)
  {
    return mPinFromKey[key];
  }
  
private:
  int mPinFromKey[KeyCount];
};

/**
 */
template <int KeyCount>
class PitchMap
{
public:
  PitchMap()
  {
  }
  
  void begin(byte startPitch, byte stopPitch)
  {
    for (int key = 0; key < KeyCount; key++)
    {
      mPitchMap[key] = map(key, 0, KeyCount - 1, startPitch, stopPitch);
    }
  }
  
  byte pitch(int key)
  {
    return mPitchMap[key];
  }
  
private:
  byte mPitchMap[KeyCount];
};

/**
 */
template <int KeyCount>
class KeyBoard
{
public:
  KeyBoard() :
    mPitchMap(NULL),
    mMidiStream(NULL)
  {
  }
  
  void begin(KeyWiring<KeyCount>& keyWiring, PitchMap<KeyCount>& pitchMap, MidiStream& stream)
  {
    mPitchMap = &pitchMap;
    mMidiStream = &stream;
    
    for (int key = 0; key < KeyCount; key++)
    {
      mKey[key].begin(keyWiring.pin(key));
    }
  }
  
  int keyCount()
  {
    return KeyCount;
  }
  
  void sampleKeys(int channel = 0)
  {
    for (int key = 0; key < KeyCount; key++)
    {
      Key::Transition transition = mKey[key].checkTransition();
      
      if (transition == Key::eTransitioned_On)
      {
        mMidiStream->sendNoteOn(channel, mPitchMap->pitch(key), mKey[key].transitionVelocity());
      }
      else if (transition == Key::eTransitioned_Off)
      {
        mMidiStream->sendNoteOff(channel, mPitchMap->pitch(key), mKey[key].transitionVelocity());
      }
    }
  }
  
private:
  Key mKey[KeyCount];
  PitchMap<KeyCount>* mPitchMap;
  MidiStream* mMidiStream;
};

//
// Serial MIDI device:
//  Baud: 9600
//  Channel: 1
static const int kMidiBaud = 9600;
static const int kMidiChannel = 0;

static const int kMidiNoteBegin = 52;
static const int kMidiNoteEnd = 58;

static const int kKeyCount = 6;

static MidiStream sMidiStream;
static KeyWiring<kKeyCount> sKeyWiring;
static KeyBoard<kKeyCount> sKeyboard;
static PitchMap<kKeyCount> sPitchMap;

void setup()
{
  int pins[kKeyCount] = {13, 12, 11, 10, 9, 8};
  sKeyWiring.begin(pins);
  
  sPitchMap.begin(kMidiNoteBegin, kMidiNoteEnd);
  
  Serial.begin(kMidiBaud);
  sMidiStream.begin(Serial);
  
  sKeyboard.begin(sKeyWiring, sPitchMap, sMidiStream);
}

void loop()
{
  sKeyboard.sampleKeys(kMidiChannel);
}


