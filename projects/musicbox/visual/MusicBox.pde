
// 1) Download loopbe1 midi loopback driver for win7
//     http://www.nerds.de/en/loopbe1.html
// 2) Downlaodd themidibus
//     Tools -> Add Tool... -> The Midi Bus
//

import themidibus.*; 

MidiBus sMidi;
Encoder sEncoder;
int sEncoderValue = 0;
int kNumBits = 14;
int[] kPitchMap;
int kBeginMidiPitch = 40;
int kEndMidiPitch = 95;

void setup()
{
  size(800, 100);

  if (frame != null)
  {
    frame.setResizable(true);
  }

  println(Serial.list());
  String portName = Serial.list()[0];
  println(portName);
  
  kPitchMap = new int[kNumBits];
  for (int bit = 0; bit < kNumBits; bit++)
  {
    kPitchMap[bit] = (int)map(bit, 0, 13, kBeginMidiPitch, kEndMidiPitch);
    println("Dot " + bit + " -> Pitch " + kPitchMap[bit]);
  }
  
  sEncoder = new Encoder(portName, 9600);
  sEncoder.onNewEncoderValue().addListener(this, "displayEncoder");
  sEncoder.onNewEncoderValue().addListener(this, "sendMidi");
  
  sMidi = new MidiBus(this, -1, "LoopBe Internal MIDI");
  
  MidiBus.list();
}

boolean isSet(int value, int bit)
{
  return (value & (1 << bit)) != 0;
}

void draw()
{
  background(250);

  int kBitSpacing = 10;
  int kDiameter = (width - ((kNumBits - 1) * kBitSpacing)) / kNumBits;

  int kGraphicHeight = kDiameter;
  int kGraphicWidth = (kDiameter * kNumBits) + (kBitSpacing * (kNumBits - 1));

  int kStartY = (kDiameter / 2) + ((height - kGraphicHeight) / 2);
  int kStartX = (kDiameter / 2) + ((width - kGraphicWidth) / 2);

  for (int bit = 0; bit < kNumBits; bit++)
  {
    int cX = kStartX + (bit * (kBitSpacing + kDiameter));
    int cY = kStartY;

    if (isSet(sEncoderValue, kNumBits - bit - 1))
    {
      fill(204, 102, 0);
    }
    else
    {
      fill(155, 20);
    }

    ellipse(cX, cY, kDiameter, kDiameter);
  }
}

void displayEncoder(int v)
{
  //println("app.onNewEncoderValue(" + v + ")");
  sEncoderValue = v;
}

void sendMidi(int v)
{
  
  for (int bit = 0; bit < kNumBits; bit++)
  {
    if (isSet(v, bit))
    {
      sMidi.sendNoteOn(0, kPitchMap[bit], 127);
    }
    else
    {
      sMidi.sendNoteOff(0, kPitchMap[bit], 127);
    }
  }
}

