const int stepPin = 5; 
const int dirPin = 2; 
const int enPin = 8;
float calibration = 26.842;
const int analogInPin = A1;
int sensorValue = 0;
const int s1 = 12;
const int s2 = 7;
float b;
int initialVolume = 0.08;
float minVol = 0.05;
float maxVol = 0.25;
float data[] = {-1 , -1};
int mVmL = 200;
void setup() {
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  pinMode(s1,INPUT);
  pinMode(s2,INPUT);
  backward();
  initialize();
  flush(3);
  Serial.println("TITRATION STARTS");
}
void loop() {
  float* d = titration(minVol, maxVol, mVmL, data[0], data[1]);
  data[0] = d[0];
  data[1] = d[1];
  delay(1000);
}
float* titration( float minVol, float maxVol, float mVmL, float prevDelMV, int prevDelVolume){
  float mv1 = readMV();
  int volume=0;
  if(prevDelMV == -1)
    volume = initialVolume; 
  else
    volume = mVmL/(prevDelMV/prevDelVolume);
  
  if(volume<minVol)
    forward(minVol);
  else if(volume>maxVol)
    forward(maxVol);
  else 
    forward(volume);
  delay(3000);
  float mv2 = readMV();
  float data[] = {mv2-mv1, volume};
  return data;
}

float readMV()
{
  int sensorValue = analogRead(A1);
  float voltage = sensorValue * (5.0 / 1023.0);
  return voltage;
}
void initialize()
{
  int switch1 = digitalRead(s1);
  while(switch1==1)
  {
    forward();
    switch1 = digitalRead(s1);
    Serial.println(switch1);
  }
}

void flush(int n)
{
  int switch1 = digitalRead(s1);
  int switch2 = digitalRead(s2);
  for(int i=1;i<=n;i++)
  {

    switch2 = digitalRead(s2);
    while(switch2==1)
    {
      backward();
      switch2 = digitalRead(s2);
    }
    switch1 = digitalRead(s1);
    while(switch1==1)
    {
      forward();
      switch1 = digitalRead(s1);
    }  
  }
}

void goBack()
{
  int switcher = digitalRead(s2);
  while(switcher==1)
  {
    backward();
    switcher = digitalRead(s2);
  }
}

int backward()
{
  digitalWrite(dirPin,LOW); 
  for(int x = 0; x < 800; x++) {
    int switcher = digitalRead(s2);
    if(switcher==0)
      return 0;
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(1500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(1500);
  }
  return 1;
}

int forward(float vol)
{
  digitalWrite(dirPin,HIGH); 
  for(int x = 0; x < 800*vol*0.6; x++)
  {
    int switcher = digitalRead(s1);
    if(switcher==0)
       goback();
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1500); 
  }
  return 1;
}
int forward()
{
  digitalWrite(dirPin,HIGH); 
  for(int x = 0; x < 800; x++)
  {
    int switcher = digitalRead(s1);
    if(switcher==0)
      return 0;
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1500); 
  }
  return 1;
}




// float dderivative(float pH1, float pH2, float pH3)
// {
//   return (pH3 - 2*pH2 + pH1)*10/9;
// }
// float* getPh(int calibration)
// {
//   int temp;
//   int buf[10];
//   for(int i=0;i<10;i++)
//   {
//     buf[i]=analogRead(analogInPin);
//     delay(30);
//   }
//   for(int i=0;i<9;i++)
//   {
//     for(int j=i+1;j<10;j++)
//     {
//       if(buf[i]>buf[j])
//       {
//         temp=buf[i];
//         buf[i]=buf[j];
//         buf[j]=temp;
//       }
//     }
//   }
//   unsigned long int avgValue1=0, avgValue2=0, avgValue3=0;
//   for(int i=1;i<7;i++)
//   {
//     avgValue1+=buf[i];
//     avgValue2+=buf[i+1];
//     avgValue3+=buf[i+2];
//   }
    
//   float pHVol1=(float)avgValue1*(4.93)/1024/6;
//   float pHVol2=(float)avgValue2*(4.93)/1024/6;
//   float pHVol3=(float)avgValue3*(4.93)/1024/6;
//   float phValue1 = -5.263* pHVol1 + calibration;
//   float phValue2 = -5.263* pHVol2 + calibration;
//   float phValue3 = -5.263* pHVol3 + calibration;
//   float phVal[] = {phValue1, phValue2, phValue3};
//   return phVal;

// }

