
/**
 * @note must be a public member of the package in order for the
 * serialEvent() method to be invoked via reflection from Serial.
 */
public class Encoder extends PApplet
{
  public Encoder(String portName, int baudRate)
  {
    mOnNewEncoderValue = new DelegateList();
    
    mSerial = new Serial(this, portName, baudRate);
    mSerial.buffer(2);
  }
  
  public DelegateList onNewEncoderValue()
  {
    return mOnNewEncoderValue;
  }
  
  void serialEvent(Serial p)
  {
    int msb = p.read();
    int lsb = p.read();

    int encoderValue = (msb << 8) | lsb;
    mOnNewEncoderValue.invoke(encoderValue);
  }

  private Serial mSerial;
  private DelegateList mOnNewEncoderValue;
}

