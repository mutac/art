
const int kDelayMs = 500;
const int kValueTableSize = 14 * 2;
uint16_t sValueTable[kValueTableSize];

void setup()
{
  Serial.begin(9600);
  
  for (int idx = 0; idx < kValueTableSize; idx++)
  {
    unsigned int shift = idx % (sizeof(int) * 8);
    sValueTable[idx] = 1 << shift;
  }
}

void loop()
{
  static int sTableIdx = 0;
  
  Serial.write(highByte(sValueTable[sTableIdx]));
  Serial.write(lowByte(sValueTable[sTableIdx]));
  delay(kDelayMs);
  
  sTableIdx = (sTableIdx + 1) % kValueTableSize;
}

