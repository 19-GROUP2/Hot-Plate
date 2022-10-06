#define ZERO 2
#define SENSOR A0
#define VR A1
#define SENSOR_MAX 794
#define SENSOR_MIN 761
// #define SENSOR_MAX 1023
// #define SENSOR_MIN 0
#define ERROR_MAX 50.0
#define TRIAC 4
int T = 10;

float getTmpReq()
{
    return analogRead(VR) * 70.0 / 1023.0 + 30.0;
}

float getTmpNow()
{
    return (analogRead(SENSOR) - SENSOR_MIN) * 70.0 / (float)(SENSOR_MAX - SENSOR_MIN) + 30.0;
}

int calcAngle()
{
    float error = getTmpReq() - getTmpNow();
    // Serial.print("R ");
    // Serial.println(getTmpReq());
    // Serial.print("N ");
    // Serial.println(getTmpNow());
    Serial.println(error);
    int angle = (ERROR_MAX - error) * (float)T / ERROR_MAX;
    if (angle > T)
        angle = T;
    else if (angle < 0)
        angle = 0;
    return angle;
}

void setup()
{

    pinMode(VR, INPUT);
    pinMode(SENSOR, INPUT);

    Serial.begin(9600);
    Serial.println("start");
}

void loop()
{
    int angle = calcAngle();
    // Serial.println(angle);
    delay(200);
}
