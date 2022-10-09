#define ZERO 2
#define SENSOR A0
#define VR A1
#define TRIAC 4
#define SENSOR_MAX 794
#define SENSOR_MIN 761
// #define SENSOR_MAX 1023
// #define SENSOR_MIN 0
#define ERROR_MAX 50.0

#include <OneWire.h>
#include <DallasTemperature.h>
// #include <math.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensor(&oneWire);

// PID constant values
float Kp = 0, Kd = 0;
float pre_error = 0;
float scale = 0;

int zeroPreState = 0;
int tiracState = 0;
int newCycle = 0;
int angle = 0;
unsigned long count = 0;
unsigned long zeroNowTime = 0;
unsigned long zeroPreTime = 0;
unsigned long triacOnTime = 0;
unsigned long T = 10;

float getTmpReq()
{
    return analogRead(VR) * 70.0 / 1023.0 + 30.0;
}

float getTmpNow()
{
    // return (analogRead(SENSOR) - SENSOR_MIN) * 70.0 / (float)(SENSOR_MAX - SENSOR_MIN) + 30.0;

    float tmpNow = sensor.getTempCByIndex(0);
    sensor.requestTemperatures();
    return tmpNow;
}

float PID(float target, float current)
{
    float error = target - current;

    if (error < 0)
    {
        return 0;
    }

    else
    {
        float P = error * Kp;
        float D = (error - pre_error) * Kd;

        float correction = P + D;
        pre_error = error;
        return correction;
    }
}

int calcAngle()
{
    float tmpReq = getTmpReq();
    float tmpNow = getTmpNow();
    float error = tmpReq - tmpNow;

    //    angle = (ERROR_MAX - error) * (float)T / ERROR_MAX;
    //    if (angle > T)
    //        angle = T;
    //    else if (angle < 0)
    //        angle = 0;

    float angle_correction = PID(tmpReq, tmpNow);
    if (angle_correction == 0)
        angle = T;
    else
    {
        angle = round(T - scale * angle_correction);
        if (angle < 0)
            angle = 0; // triac FULL ON
        else if (angle > (T - 1))
            angle = T; // triac OFF
    }

    // if ((millis() - count) > 500)
    // {
    //     count = millis();
    Serial.print("R ");
    Serial.println(tmpReq);
    Serial.print("N ");
    Serial.println(tmpNow);
    Serial.print("E ");
    Serial.println(error);
    Serial.print("A ");
    Serial.println(angle);
    // }

    return angle;
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
            if (angle < 9)
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
        // Serial.println(T);
        zeroPreState = zeroNowState;
        zeroPreTime = zeroNowTime;
        // count++;
    }
}

void setup()
{

    pinMode(VR, INPUT);
    pinMode(SENSOR, INPUT);
    pinMode(ZERO, INPUT);
    pinMode(TRIAC, OUTPUT);

    Serial.begin(9600);
    Serial.println("start");

    sensor.begin();
    sensor.setResolution(9);
    sensor.setWaitForConversion(false);
    sensor.requestTemperatures(); // Send the command to get temperatures
    delay(1000);
}

void loop()
{
    updateTime();
    if ((millis() - count) > 2000)
    {
        count = millis();
        calcAngle();
        //      Serial.println(T);
    }
    updateTriac();
}
