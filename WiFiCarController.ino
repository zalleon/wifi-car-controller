#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk (Auth Token)
char auth[] = "";

// Wifi auth params
char ssid[] = "";
char pass[] = "";

// Forward and backward movement motor
#define ENA D3      //ENA
#define MOTORA_1 D4 //IN1
#define MOTORA_2 D5 //IN2


// Left right movement motor
#define MOTORB_1 D6 //IN3
#define MOTORB_2 D7 //IN4
#define ENB D8      //ENB

int jInact = 512;

// Inactive position
int minInact = 30;

void setup()
{
  // Pin setup
  pinMode(ENA, OUTPUT);
  pinMode(MOTORA_1, OUTPUT);
  pinMode(MOTORA_2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(MOTORB_1, OUTPUT);
  pinMode(MOTORB_2, OUTPUT);

  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);

  Serial.begin(9600);
  
  // Connect to blink
  Blynk.begin(auth, ssid, pass);
}

// Read Joiystick data. Pin: V1 and Merge mode
BLYNK_WRITE(V1) {
  int jY = param[0].asInt();
  int jX = param[1].asInt();

  // Print incoming params
  Serial.print("X: ");
  Serial.print(jX);
  Serial.print(" Y: ");
  Serial.println(jY);

  int xSpeed = 0;
  int ySpeed = 0;

  // Back
  if (jY < jInact) {
    ySpeed = jInact - jY;
    if (ySpeed < minInact) {
      ySpeed = 0;
    } else {
      ySpeed = map(ySpeed, 0, 512, 0, 900); // ~ 6.4v

      digitalWrite(MOTORA_1, LOW);
      digitalWrite(MOTORA_2, HIGH);
      analogWrite(ENA, ySpeed);

      Serial.println("<<--BACK-->>");
    }
  }

  // Forward
  if (jY > jInact) {
    ySpeed = jY - jInact;
    if (ySpeed < minInact) {
      ySpeed = 0;
    } else {
      ySpeed = map(ySpeed, 0, 512, 0, 1024); // ~ 7.4v

      digitalWrite(MOTORA_1, HIGH);
      digitalWrite(MOTORA_2, LOW);
      analogWrite(ENA, ySpeed);

      Serial.println("<<--FORWARD-->>");
    }
  }

  if (ySpeed == 0) {
    digitalWrite(MOTORA_1, LOW);
    digitalWrite(MOTORA_2, LOW);
  }
  
  // Left
  if (jX < jInact) {
    xSpeed = jInact - jX;
    if (xSpeed < minInact) {
      xSpeed = 0;
    } else {
      xSpeed = map(xSpeed, 0, 512, 0, 690); //690 ~ 5v

      digitalWrite(MOTORB_1, HIGH);
      digitalWrite(MOTORB_2, LOW);
      digitalWrite(ENB, xSpeed);
      
      Serial.println("<<--LEFT-->>");
    }
  }

  // Right
  if (jX > jInact) {
    xSpeed = jX - jInact;
    if (xSpeed < minInact) {
      xSpeed = 0;
    } else {
      xSpeed = map(xSpeed, 0, 512, 0, 690); //690 ~ 5v

      digitalWrite(MOTORB_1, LOW);
      digitalWrite(MOTORB_2, HIGH);
      digitalWrite(ENB, xSpeed);

      Serial.println("<<--RIGHT-->>");
    }
  }

  if (xSpeed == 0) {
    digitalWrite(MOTORB_1, LOW);
    digitalWrite(MOTORB_2, LOW);
  }

  Serial.print("xSpeed: ");
  Serial.print(xSpeed);
  Serial.print(" ySpeed: ");
  Serial.println(ySpeed);
}

// MAIN CODE
void loop()
{
  Blynk.run();
}
