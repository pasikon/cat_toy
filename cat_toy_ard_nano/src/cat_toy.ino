
#include <Servo.h>
#include <SoftwareSerial.h>


// TONES  ==========================================
// Start by defining the relationship between
//       note, period, &  frequency.
#define  c     3830    // 261 Hz
#define  d     3400    // 294 Hz
#define  e     3038    // 329 Hz
#define  f     2864    // 349 Hz
#define  g     2550    // 392 Hz
#define  a     2272    // 440 Hz
#define  b     2028    // 493 Hz
#define  C     1912    // 523 Hz
// Define a special note, 'R', to represent a rest
#define  R     0

#define buzzerPin 11
#define buttonPin 12

#define laserPin A3
#define servoPin A2
#define servoPinB A1

int speakerOut = 11;

int buttonState = 0;         // variable for reading the pushbutton status

Servo servo;
Servo servoB;

SoftwareSerial espSerial(9, 8); // RX, TX
String playMode = "auto";

int servoAngle = 0;   // servo position in degrees

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[],
//  which sets each note's relative length (higher #, longer note)
int melody[] = {  C,  b,  g,  C,  b,   e,  R,  C,  c,  g, a, C };
int beats[]  = { 16, 16, 16,  8,  8,  16, 32, 16, 16, 16, 8, 8 };
int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.

// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 10;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;


// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(speakerOut, HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    }
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);
  servoB.attach(servoPinB);
  pinMode(buzzerPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(laserPin, LOW);
  digitalWrite(buzzerPin, HIGH);

  espSerial.begin(9600);

}


void loop() {

  buttonState = digitalRead(buttonPin);

  String espData = espSerial.readStringUntil('\n');
  if (espData.indexOf("auto") != -1) {
    digitalWrite(laserPin, LOW);
    playMode = "auto";
  } else if (espData.indexOf("manual") != -1) {
    playMode = "manual";
  }

  if (buttonState == LOW && playMode == "auto") {
    // Set up a counter to pull from melody[] and beats[]
    for (int i = 0; i < MAX_COUNT; i++) {
      tone_ = melody[i];
      beat = beats[i];

      duration = beat * tempo; // Set up timing

      playTone();
      // A pause between notes...
      delayMicroseconds(pause);
    }
    digitalWrite(buzzerPin, HIGH);

    digitalWrite(laserPin, HIGH);
    doServo();
    digitalWrite(laserPin, LOW);
  }

  if (playMode == "manual") {
    digitalWrite(laserPin, HIGH);
    String servoDo = espSerial.readStringUntil('\n');
    if (servoDo.indexOf("left") != -1) {
      left(10);
    } else if (servoDo.indexOf("right") != -1) {
      right(10);
    } else if (servoDo.indexOf("up") != -1) {

    } else if (servoDo.indexOf("down") != -1) {

    }
  }


}

void servoStairs() {

  for (servoAngle = 10; servoAngle < 70; servoAngle++) {
    servo.write(servoAngle);
    delay(70);
  }

  delay(5000);

  timesLeftRight(5, 10, 300);

  for (servoAngle = 70; servoAngle > 10; servoAngle--) {
    servo.write(servoAngle);
    delay(70);
  }

}

void timesLeftRight(int times, int degs, int delayy) {
  int servoBAng = servoB.read();
  for (int xox = 0; xox <= times; xox++) {
    int sb = servoBAng;
    while (servoBAng > sb - degs) {
      delay(random(70, 300));
      servoB.write(servoBAng);
      servoBAng--;
    }
    sb = servoB.read();
    while (servoBAng < sb + degs) {
      delay(random(70, 300));
      servoB.write(servoBAng);
      servoBAng++;
    }
    delay(2000);
  }
}

void left(int degs) {
  int servoBAng = servoB.read();
  int sb = servoBAng;
  while (servoBAng > sb - degs) {
    delay(75);
    servoB.write(servoBAng);
    servoBAng--;
  }
}

void right(int degs) {
  int servoBAng = servoB.read();
  int sb = servoBAng;
  while (servoBAng < sb + degs) {
    delay(75);
    servoB.write(servoBAng);
    servoBAng++;
  }
}

void doServo()
{
  //control the servo's direction and the position of the motor

  //  servo.write(45);      // Turn SG90 servo Left to 45 degrees
  //  servoB.write(45);
  //  delay(1000);          // Wait 1 second
  //  servo.write(90);      // Turn SG90 servo back to 90 degrees (center position)
  //  servoB.write(90);
  //  delay(1000);          // Wait 1 second
  //  servo.write(135);     // Turn SG90 servo Right to 135 degrees
  //  delay(1000);          // Wait 1 second
  //  servo.write(90);      // Turn SG90 servo back to 90 degrees (center position)
  //  delay(1000);

  //end control the servo's direction and the position of the motor


  //control the servo's speed

  //  //if you change the delay value (from example change 50 to 10), the speed of the servo changes
  for (servoAngle = 15; servoAngle < 120; servoAngle++) //move the micro servo from 0 degrees to 180 degrees
  {
    servo.write(servoAngle);
    delay(70);
  }
  //
  //  for (servoAngle = 100; servoAngle > 0; servoAngle--) //now move back the micro servo from 0 degrees to 180 degrees
  //  {
  //    servo.write(servoAngle);
  //    delay(10);
  //  }
  //end control the servo's speed
}
