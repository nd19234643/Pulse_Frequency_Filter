static int CHECK1_Pin = 2;

unsigned char Flow_Init = 0;
unsigned char counter= 0;

unsigned char Period[3]= {0, 0, 0};
unsigned char Pos[2]= {0, 0};
unsigned char Neg[2]= {0, 0};


void setup()
{
  pinMode(CHECK1_Pin, INPUT_PULLUP);
  
  Serial.begin(115200);   // USB Debug
  Serial1.begin(115200);  // UART Communication 
}


void loop()
{
  // Initial Information
  if (Flow_Init == 0)
  {
    delay(2000);  
    Flow_Init = 1;
  }

  delay(20); 
  int sensorVal_1 = digitalRead(CHECK1_Pin); 
  float lowValueOfFrequency = 0.6; // unit: Hz
  float highValueOfFrequency = 1.5; // unit: Hz

  checkFrequencyRange(lowValueOfFrequency, highValueOfFrequency, sensorVal_1);
}

void checkFrequencyRange(float lowValue, float highValue, int sensorVal)
{
  float countForHighValue = ((1 / highValue) * 1000 / 20);
  float countForLowValue = ((1/ lowValue) * 1000 / 20);

  // Count
  if (sensorVal == LOW)
  {
    Neg[0] = Neg[0] + 1;
    if (Pos[0] > 0)
      Pos[1] = 1;     
  }
  else
  {
    Pos[0] = Pos[0] + 1;
    if (Neg[0] > 0)
      Neg[1] = 1;
  }

  // Clear data
  if (Pos[0] > countForLowValue)
  {
    Pos[0]= 0;
    Period[0]= 0; 
  }
  if (Neg[0] > countForLowValue)
  {
    Neg[0]= 0;
    Period[1]= 0; 
  }

  // Record count information for HIGH
  if (Pos[1] == 1)
  {
//    Serial.print('%');
//    Serial.println(Pos[0], DEC);
    Period[0] = Pos[0]; 
    Pos[0] = 0;
    Pos[1] = 0; 
  }

  // Record count information for LOW
  if (Neg[1] == 1)
  {
//    Serial.print('%');
//    Serial.println(Neg[0], DEC);
    Period[1] = Neg[0];
    Neg[0] = 0;
    Neg[1] = 0;
  }

  // Check frequency range
  counter++;
  if (counter >= 10) // 200 ms
  {
    Period[2] = Period[0] + Period[1];
    
    if ((Period[2] >= countForHighValue) && (Period[2] <= countForLowValue))
    {
      Serial.print('%');
//      Serial.println(Period[2], DEC);
      Serial.println('0');
    }
    else
    {
      Serial.print('%');
//      Serial.println(Period[2], DEC);
      Serial.println('1');  
    }

    counter = 0;
  }
}



