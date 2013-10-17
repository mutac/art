import processing.serial.*;
import java.lang.reflect.*;
import java.util.*;

class Delegate
{
  public Delegate(Object obj, String methodName)
  {
    mMethodTable = new HashMap();
    mObj = obj;
    mMethodName = methodName;
  }

  public void invoke()
  {
    methodInvoke(new Object [0]);
  }

  public void invoke(Object arg0)
  {
    methodInvoke(new Object [] {arg0});
  }

  private void methodInvoke(Object[] argv)
  {
    Class [] types = new Class[argv.length];
    for (int idx = 0; idx < argv.length; idx++)
    {
      types[idx] = argv.getClass();
    }

    Method meth = getMethod(types);

    try
    {
      if (meth != null)
      {
        meth.invoke(mObj, argv);
      }
    }
    catch (Exception e)
    {
    }
  }

  private Method getMethod(Class[] argv)
  {
    if (mMethodTable.containsKey(argv))
    {
      return (Method)mMethodTable.get(argv);
    }
    else
    {
      Method meth = null;

      try
      {
        meth = mObj.getClass().getMethod(mMethodName, argv);
      }
      catch (Exception e)
      {
      }

      mMethodTable.put(argv, meth);
      return meth;
    }
  }

  private HashMap mMethodTable;
  private Object mObj;
  private String mMethodName;
}

class DelegateList
{
}

class Encoder extends PApplet
{
  public Encoder(String iname, int irate)
  {
    mSerial = new Serial(this, iname, irate);
    mSerial.buffer(2);
  }

  public void serialEvent(Serial p)
  {
    int msb = p.read();
    int lsb = p.read();

    int encoderValue = (msb << 8) | lsb;
  }

  private Serial mSerial;
}

Encoder myPort;
int sEncoderValue = 0;

void setup()
{
  size(800, 100);

  if (frame != null)
  {
    frame.setResizable(true);
  }

  String portName = Serial.list()[0];
  println(portName);

  myPort = new Encoder(portName, 9600);
}

boolean isSet(int value, int bit)
{
  if ((value & (1 << bit)) != 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void draw()
{
  background(250);

  int kNumBits = 14;
  int kBitSpacing = 10;
  int kDiameter = (width - ((kNumBits - 1) * kBitSpacing)) / kNumBits;

  int kGraphicHeight = kDiameter;
  int kGraphicWidth = (kDiameter * kNumBits) + (kBitSpacing * (kNumBits -
1));

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

void serialEvent(Serial p)
{
  int msb = p.read();
  int lsb = p.read();

  sEncoderValue = (msb << 8) | lsb;
}
