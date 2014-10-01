void MFRC522_Init(uchar csPin)
{
  digitalWrite(SS_PIN,HIGH);
  MFRC522_Reset(csPin);
  
  //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  Write_MFRC522(TModeReg, 0x8D,csPin); //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  Write_MFRC522(TPrescalerReg, 0x3E,csPin); //TModeReg[3..0] + TPrescalerReg
  Write_MFRC522(TReloadRegL, 30,csPin);
  Write_MFRC522(TReloadRegH, 0,csPin);
  Write_MFRC522(TxAutoReg, 0x40,csPin); //100%ASK
  Write_MFRC522(ModeReg, 0x3D,csPin); //CRC 初始值0x6363 ???

  AntennaOn(csPin); //打开天线
}

void MFRC522_Reset(uchar csPin)
{
  Write_MFRC522(CommandReg, PCD_RESETPHASE,csPin);
}

/*
* 函 数 名：AntennaOn
* 功能描述：开启天线,每次启动或关闭天险发射之间应至少有1ms 的间隔
* 输入参数：无
* 返 回 值：无
*/
void AntennaOn(uchar csPin)
{
  uchar temp=0x00;
  temp = Read_MFRC522(TxControlReg,csPin);
  if (!(temp & 0x03))
  {
    SetBitMask(TxControlReg, 0x03,csPin);
  }
  temp = Read_MFRC522(TxControlReg,csPin);
}

uchar MFRC522_Request(uchar reqMode, uchar *TagType,uchar csPin)
{
  uchar status;
  uint backBits; //接收到的数据位数
  Write_MFRC522(BitFramingReg, 0x07,csPin); 
  TagType[0] = reqMode;
  status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits,csPin);
  if ((status != MI_OK) || (backBits != 0x10))
  {
    status = MI_ERR;
  }
  return status;
  }

uchar MFRC522_ToCard(uchar command, 
                     uchar *sendData, 
                     uchar sendLen, 
                     uchar *backData,
                     uint *backLen,
                     uchar csPin
                     )
{
  uchar status = MI_ERR;
  uchar irqEn = 0x00;
  uchar waitIRq = 0x00;
  uchar lastBits;
  uchar n;
  uint i;
  switch (command)
  {
    case PCD_AUTHENT: //认证卡密
    {
      irqEn = 0x12;
      waitIRq = 0x10;
      break;
    }
    case PCD_TRANSCEIVE: //发送FIFO 中数据
    {
      irqEn = 0x77;
      waitIRq = 0x30;
      break;
    }
    default:
    break;
  }
  Write_MFRC522(CommIEnReg, irqEn|0x80,csPin); //允许中断请求
  ClearBitMask(CommIrqReg, 0x80,csPin); //清除所有中断请求位
  Write_MFRC522(CommandReg, PCD_IDLE,csPin); //NO action;取消当前命令 ???
  SetBitMask(FIFOLevelReg, 0x80,csPin); //FlushBuffer=1, FIFO 初始化

  //向FIFO 中写入数据
  for (i=0; i<sendLen; i++)
  {
    Write_MFRC522(FIFODataReg, backData[i],csPin);
  }
  //执行命令
  Write_MFRC522(CommandReg, command,csPin);
  if (command == PCD_TRANSCEIVE)
  {
    SetBitMask(BitFramingReg, 0x80,csPin); //StartSend=1,transmission of data starts
  }
  
  //等待接收数据完成
  i = 2000; //i 根据时钟频率调整，操作M1 卡最大等待时间25ms ???
  do
  {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = Read_MFRC522(CommIrqReg,csPin);
    i--;
  }
  while ((i!=0) && !(n&0x01) && !(n&waitIRq));
  ClearBitMask(BitFramingReg, 0x80,csPin); //StartSend=0
 
  if (i != 0)
  {
    if(!(Read_MFRC522(ErrorReg,csPin) & 0x1B)) //BufferOvfl Collerr CRCErr ProtecolErr
    {
      status = MI_OK;
      if (n & irqEn & 0x01)
      {
        status = MI_NOTAGERR; //??
      }
      if (command == PCD_TRANSCEIVE)
      {
        n = Read_MFRC522(FIFOLevelReg,csPin);
        lastBits = Read_MFRC522(ControlReg,csPin) & 0x07;
        if (lastBits)
        {
          *backLen = (n-1)*8 + lastBits;
        }
        else
        {
          *backLen = n*8;
        }
        
        if (n == 0)
        {
          n = 1;
        }
        if (n > MAX_LEN)
        {
          n = MAX_LEN;
        }
        //读取FIFO 中接收到的数据
        for (i=0; i<n; i++)
        {
          backData[i] = Read_MFRC522(FIFODataReg,csPin);
        }
       }
    }
    else
    {
      status = MI_ERR;
    }
    SetBitMask(ControlReg,0x80,csPin); //timer stops
    Write_MFRC522(CommandReg, PCD_IDLE,csPin);
    return status;
  }
  
}

