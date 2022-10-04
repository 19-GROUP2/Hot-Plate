#define ZERO 2
#define SENSOR 3
#define TRIAC 4

int zeroPreState = 0;
int tiracState = 0;
int newCycle = 0;
int angle = 5;
unsigned long zeroNowTime = 0;
unsigned long zeroPreTime = 0;
unsigned long triacOnTime = 0;
unsigned long T = 0;

void setup()
{
  // put your setup code here, to run once:
  //  pinMode(ZERO,INPUT_PULLUP);
  pinMode(ZERO, INPUT);
  pinMode(SENSOR, INPUT);
  pinMode(TRIAC, OUTPUT);
  Serial.begin(9600);
  Serial.println("start");
}

void loop()
{
  // put your main code here, to run repeatedly:
  updateTime();
  updateTriac();
}

void updateTriac()
{
  unsigned long nowTime = millis();
  if (tiracState)
  {
    if ((nowTime - triacOnTime) > 0)
    {
      tiracState = 0;
      digitalWrite(TRIAC, tiracState);
    }
  }
  else
  {
    if (newCycle && (nowTime - zeroNowTime) > angle)
    {
      tiracState = 1;
      newCycle = 0;
      digitalWrite(TRIAC, tiracState);
      triacOnTime = nowTime;
    }
  }
}

void updateTime()
{
  int zeroNowState = digitalRead(ZERO);
  if (zeroNowState != zeroPreState)
  {
    tiracState = 0;
    newCycle = 1;
    digitalWrite(TRIAC, tiracState);
    zeroNowTime = millis();
    T = zeroNowTime - zeroPreTime;
    Serial.println(T);
    zeroPreState = zeroNowState;
    zeroPreTime = zeroNowTime;
  }
}
