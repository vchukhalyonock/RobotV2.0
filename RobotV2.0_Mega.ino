#include <Key.h>
#include <Keypad.h>
#include <FlexiTimer2.h>
#include <LiquidCrystal_I2C.h>

#define REAR_SONAR_TRIGER_PIN 22
#define REAR_SONAR_ECHO_PIN 23

#define FRONT_SONAR_TRIGER_PIN 24
#define FRONT_SONAR_ECHO_PIN 25

#define KEYBOARD_BLINKER 35

const byte ROWS = 5;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'a', 'b', '#', '*'},
  {'1' ,'2', '3', 'U'},
  {'4', '5', '6', 'D'},
  {'7', '8', '9', 's'},
  {'L', '0', 'R', 'e'}
};

byte rowPins[ROWS] = {37, 39, 41, 43, 45}; 
byte colPins[COLS] = {53, 51, 49, 47};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal_I2C lcd(0x27, 20, 4);
int rearDistance = 0;
int frontDistance = 0;

byte cursorPosition = 0;

void setup() {
  FlexiTimer2::set(2, timerInterupt);
  FlexiTimer2::start();
  
  pinMode(REAR_SONAR_TRIGER_PIN, OUTPUT);
  pinMode(REAR_SONAR_ECHO_PIN, INPUT);

  pinMode(FRONT_SONAR_TRIGER_PIN, OUTPUT);
  pinMode(FRONT_SONAR_ECHO_PIN, INPUT);

  pinMode(KEYBOARD_BLINKER, OUTPUT);
  digitalWrite(KEYBOARD_BLINKER, HIGH);
  
  lcd.init();
  lcd.backlight();
  lcd.print("Home Robot Project");
  lcd.setCursor(0, 2);
  lcd.print("Front Distance:");
  lcd.setCursor(0, 3);
  lcd.print("Rear Distance:");

  //keypad.addEventListener(keypadEvent);
  digitalWrite(KEYBOARD_BLINKER, LOW);

  Serial.begin(9600);
  Serial.println("Reset");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    lcd.setCursor(cursorPosition, 1);
    lcd.print(key);
    Serial.print(key);
    digitalWrite(KEYBOARD_BLINKER, HIGH);
    delay(50);
    digitalWrite(KEYBOARD_BLINKER, LOW);
    if(cursorPosition > 18) {
        lcd.setCursor(0, 1);
        lcd.print("                    ");
        cursorPosition = 0;
      } else {
        cursorPosition++;
      }
  }
  
  int rdistance = rearDistance *0.034/2;
  int fdistance = frontDistance *0.034/2;


  lcd.setCursor(15, 2);
  lcd.print("   ");
  lcd.setCursor(15, 2);
  lcd.print(fdistance);
  lcd.setCursor(15, 3);
  lcd.print("   ");
  lcd.setCursor(15, 3);
  lcd.print(rdistance);
  //rearDistance = getRearDistance();
  //delay(100);
}

int getRearDistance() {
  digitalWrite(REAR_SONAR_TRIGER_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(REAR_SONAR_TRIGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(REAR_SONAR_TRIGER_PIN, LOW);
  return pulseIn(REAR_SONAR_ECHO_PIN, HIGH, 5000);
}

int getFrontDistance() {
  digitalWrite(FRONT_SONAR_TRIGER_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(FRONT_SONAR_TRIGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(FRONT_SONAR_TRIGER_PIN, LOW);
  return pulseIn(FRONT_SONAR_ECHO_PIN, HIGH, 5000);
}

void timerInterupt() {
  rearDistance = getRearDistance();
  frontDistance = getFrontDistance();
}
