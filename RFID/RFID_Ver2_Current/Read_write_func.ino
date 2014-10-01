void Write_MFRC522(uchar addr, uchar val,uchar csPin)
{
digitalWrite(csPin, LOW);
//地址格式：0XXXXXX0
SPI.transfer((addr<<1)&0x7E);
SPI.transfer(val);
digitalWrite(csPin, HIGH);
}


uchar Read_MFRC522(uchar addr, uchar csPin)
{
  uchar val;
digitalWrite(csPin, LOW);
//地址格式：1XXXXXX0
SPI.transfer(((addr<<1)&0x7E) | 0x80);
val =SPI.transfer(0x00);
digitalWrite(csPin, HIGH);
return val;
}

void SetBitMask(uchar reg, uchar mask,uchar csPin)
{
  uchar tmp;
  tmp = Read_MFRC522(reg,csPin);
  Write_MFRC522(reg, tmp | mask,csPin); // set bit mask
}

void ClearBitMask(uchar reg, uchar mask,uchar csPin)
{
  uchar tmp;
  tmp = Read_MFRC522(reg,csPin);
  Write_MFRC522(reg, tmp & (~mask),csPin); // clear bit mask
}
