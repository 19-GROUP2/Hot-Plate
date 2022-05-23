# include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

int heat=A5;
int pin=3;

void setup() {
  pinMode(heat, OUTPUT);
  pinMode(pin, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  int temp=10;
  analogWrite(heat, temp);
  int r = analogRead(pin);
  Serial.println(r); 
  lcd.setCursor(0, 0);
  lcd.print(r);
  delay(500); 
}
