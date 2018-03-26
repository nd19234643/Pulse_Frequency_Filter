#define DELAY_TIME 20 // unit: ms
#define CHECK_INPIN 2

unsigned char count = 0;

unsigned char Period[3]= {0, 0, 0};
unsigned char Pos[2]= {0, 0};
unsigned char Neg[2]= {0, 0};

unsigned char tempPositiveVal = 0;
unsigned char tempNegativeVal = 0;
unsigned char positiveVal = 0;
unsigned char negativeVal = 0;


void setup()
{
  pinMode(CHECK_INPIN, INPUT_PULLUP);
  
  Serial.begin(115200);   // USB Debug
  Serial1.begin(115200);  // UART Communication 
}

void loop()
{
  delay(DELAY_TIME); 
  int val = digitalRead(CHECK_INPIN); 
  float lowFrequency = 0.6; // unit: Hz
  float highFrequency = 1.5; // unit: Hz

  checkFrequencyRange(lowFrequency, highFrequency, val);
  
  // Leon Huang design
//  checkFrequencyRange_2(lowFrequency, highFrequency, val);
}

void checkFrequencyRange(float lowValue, float highValue, int sensorVal)
{
  // 67 count ~ 167 count
  float lowerBound = ((1 / highValue) * 1000 / 20);
  float upperBound = ((1/ lowValue) * 1000 / 20);

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
  if (Pos[0] > upperBound) // > 167 count
  {
    Pos[0]= 0;
    Period[0]= 0; 
  }
  if (Neg[0] > upperBound) // > 167 count
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
  count++;
  if (count >= 10) // 200 ms 檢查一次
  {
    Period[2] = Period[0] + Period[1];
    
    if ((Period[2] >= lowerBound) && (Period[2] <= upperBound))
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

    count = 0;
  }
}

// Leon Huang design
void checkFrequencyRange_2(float lowValue, float highValue, int sensorVal)
{
  // 67 count ~ 167 count
  float lowerBound = ((1 / highValue) * 1000 / 20);
  float upperBound = ((1/ lowValue) * 1000 / 20);

  // Count
  if (sensorVal == LOW)
  {
    tempNegativeVal += 1;
    
    if (tempPositiveVal > 0)
    {
      positiveVal = tempPositiveVal;
      tempPositiveVal = 0;
    }
  }
  else
  {
    tempPositiveVal += 1;
    
    if (tempNegativeVal > 0) 
    {
      negativeVal = tempNegativeVal;
      tempNegativeVal = 0;
    }
  }

  // Clear data
  if (tempNegativeVal > upperBound) // > 167 count
  {
    tempNegativeVal = 0;
    negativeVal = 0;
  }
  if (tempPositiveVal > upperBound) // > 167 count
  {
    tempPositiveVal = 0;
    positiveVal = 0;
  }

  count++;
  if (count >= 10) // 200 ms 檢查一次
  {
    unsigned char sum = negativeVal + positiveVal;
    
    if ((sum >= lowerBound) && (sum <= upperBound))
    {
      Serial.print('%');
//      Serial.println(sum, DEC);
      Serial.println('0');
    }
    else
    {
      Serial.print('%');
//      Serial.println(sum, DEC);
      Serial.println('1');  
    }

    count = 0;
  }
}



