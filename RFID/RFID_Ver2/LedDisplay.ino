static uchar ledMaping[]= {2,3,16,17,18,19};

void Led_display(uchar LedNum,boolean flag)
{
  #if 1
    digitalWrite(ledMaping[CS0_PIN-LedNum], flag);
 #else
 #endif
}

void Led_Init()
{
#if 1
  int i=0;
  for(i=0;i<sizeof(ledMaping);i++)
  {
      pinMode(ledMaping[i],OUTPUT); 
      digitalWrite(ledMaping[i], HIGH);
  }
#else
  hc595_init(LedReset, LedDataOut, LedClock, LedLatch);
#endif
}
