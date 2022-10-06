#define INP 2

int pre = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
void setup()
{
    // put your setup code here, to run once:
    //  pinMode(INP,INPUT_PULLUP);
    pinMode(INP, INPUT);
    Serial.begin(9600);
    Serial.println("start");
}

void loop()
{
    // put your main code here, to run repeatedly:
    int now = digitalRead(INP);
    if (now != pre)
    {
        t1 = millis();
        Serial.println(t1 - t2);
        pre = now;
        t2 = t1;
    }
}
