#include <SPI.h>
#include <Ethernet.h>
#include <RobotOpen.h>

/* I/O Setup */
USBJoystick usb1('0');  // Assign the logitech USBJoystick object to bundle 0
//==================
int lastJsBtn3 = LOW;
int JsBtn3;
int DeJsBtn3;
int lastDeJsBtn3 = LOW;
int JsBtn3Toggle = LOW;
int BallMotor = LOW;
//==============================
int lastJsBtn1 = LOW;
int JsBtn1;
int DeJsBtn1;
int lastDeJsBtn1 = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;
int toggletrue = LOW;
int SD1 = LOW;
//==============================
int Enabled = LOW;
int LastEnabled = LOW;
int Enabled1Shot = LOW;
//===============================
int CaseVar = 1;
long AENT;
long ASTALL = 1000;
long BENT;
long BSTALL = 1000;
long CENT;
long CSTALL = 1000;
int ShootBtn;
int LOHI;
// Variables for the Pressure Switch and Compressor logic. 
int PS;
int DePS;
int lastPS;
int Compressor = LOW;
long PSdebounceDelay = 500;
long PSlastDebounceTime = 0;
// Varialbes for Cheesy Drive
int DriveSelect;
int Throttle = 0;
int Wheel = 0;
float QTSens = 1.2;
float STSens = 1.5;
float RLAdjustment = 0;
int RHPower; 
int LHPower; 


void setup()
{
  /* Initiate comms */
  RobotOpen.begin();
  Serial.begin(9600); // Serial Monitor 
  pinMode(SIDECAR_DIGITAL1, OUTPUT);
  pinMode(SIDECAR_DIGITAL2, OUTPUT);
  pinMode(SIDECAR_DIGITAL3, OUTPUT);
  pinMode(SIDECAR_DIGITAL4, OUTPUT);
  pinMode(SIDECAR_DIGITAL5, OUTPUT);
  pinMode(SIDECAR_DIGITAL6, OUTPUT);
  pinMode(SIDECAR_DIGITAL7, INPUT);
  pinMode(SIDECAR_DIGITAL8, INPUT);
}

/* This is your primary robot loop - all of your code
 * should live here that allows the robot to operate
 */

void enabled() {
  Enabled = HIGH;
  if (Enabled == HIGH && LastEnabled == LOW) {
    Enabled1Shot = HIGH;
  }
  else 
  {
    Enabled1Shot = LOW;
  }
  if (Enabled1Shot = HIGH) { 
    int DriveSelect = digitalRead(SIDECAR_DIGITAL8);
  }

  if (DriveSelect == HIGH) { 
    // Do Tank Drive
    // Constantly update PWM values with joystick values
    RobotOpen.setPWM(SIDECAR_PWM1, usb1.makePWM(ANALOG_LEFTY, INVERT));
    RobotOpen.setPWM(SIDECAR_PWM2, usb1.makePWM(ANALOG_LEFTY, INVERT));
    RobotOpen.setPWM(SIDECAR_PWM3, usb1.makePWM(ANALOG_RIGHTY, INVERT));
    RobotOpen.setPWM(SIDECAR_PWM4, usb1.makePWM(ANALOG_RIGHTY, INVERT));
  }
  else; 
  // Do HALO Drive with QuickTurn
  {
    Throttle = usb1.getIndex(ANALOG_LEFTY)-127;
    Wheel = usb1.getIndex(ANALOG_RIGHTX)-127;
    // The Value returned from the Joystick is between 0 and 255, subracting 127 makes it -127 to 128)
    // We like approx. 10% Dead band around 0 on the sticks, so we will apply that first. 
    if (Throttle < 12 && Throttle > -12) {
      Throttle = 0;
    }
    if (Wheel < 12 && Wheel > -12) {
      Wheel = 0;
    }
    // Now need to check if QuickTurn can be active, ie throttle is less than 25%
    if (Throttle < 32 && Throttle > -32) {
      //Quickturn can be used.
      RLAdjustment = Wheel * QTSens;
    }
    else
    {
      // SpeedTurn to be used.
      RLAdjustment = abs(Throttle) * Wheel * STSens;
    }
    LHPower = Throttle - RLAdjustment + 127;
    RHPower = Throttle + RLAdjustment + 127;
    LHPower = constrain(LHPower, 0, 255);
    RHPower = constrain(RHPower, 0, 255);
    RobotOpen.setPWM(SIDECAR_PWM1, LHPower);
    RobotOpen.setPWM(SIDECAR_PWM2, LHPower);
    RobotOpen.setPWM(SIDECAR_PWM3, RHPower);
    RobotOpen.setPWM(SIDECAR_PWM4, RHPower);
  }

  // Dbounce the Pressure Switch 
  int PS = digitalRead (SIDECAR_DIGITAL7);
  if (PS != lastPS) {
    PSlastDebounceTime = millis();
  }
  if ((millis() - PSlastDebounceTime) > PSdebounceDelay) {
    DePS = PS;
  }
  lastPS = PS;
  Compressor = ! DePS; // Compressor Runs when Pressure Switch is not on. 

  //Debounce the Short/Long Toggle 
  int reading = usb1.getBtn(BTN1, NORMAL);
  if (reading != lastJsBtn1) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    JsBtn1 = reading;
  }
  DeJsBtn1 = reading;

  // TOGGLE Code for Digital Output 1 =====================================
  if (DeJsBtn1 == HIGH && lastDeJsBtn1 == LOW && SD1 == HIGH) {
    SD1 = LOW;
    toggletrue = HIGH;
  }
  if (DeJsBtn1 == HIGH && lastDeJsBtn1 == LOW && SD1 == LOW && toggletrue == LOW) {
    SD1 = HIGH;
    LOHI = 1;
  }
  lastDeJsBtn1 = DeJsBtn1;
  toggletrue = LOW;

  // Dbounce the Ball Gatherer 
  int BallMotor = usb1.getBtn(BTN3, NORMAL);
  if (BallMotor != lastJsBtn3) {
    lastDeJsBtn3 = millis();
  }
  if ((millis() - lastDeJsBtn3) > debounceDelay) {
    JsBtn3 = BallMotor;
  }
  DeJsBtn3 = BallMotor;
  // TOGGLE Code for JsBtn3  =====================================
  if (DeJsBtn3 == HIGH && lastDeJsBtn3 == LOW && JsBtn3Toggle == HIGH) {
    JsBtn3Toggle = LOW;
    toggletrue = HIGH;
  }
  if (DeJsBtn1 == HIGH && lastDeJsBtn1 == LOW && JsBtn3Toggle == LOW && toggletrue == LOW) {
    JsBtn3Toggle = HIGH;
  }
  lastDeJsBtn3 = DeJsBtn3;
  toggletrue = LOW;
  // ===============================================================  
  if (JsBtn3Toggle == HIGH) {
    RobotOpen.setPWM(SIDECAR_PWM5, 203); // 203=0.6 in labview, Austin Tai said that was what they used. 
  }
  else
  {
    RobotOpen.setPWM(SIDECAR_PWM5, 127);

  }
  //========================================================================
  switch (CaseVar) {
  case 1:
    AENT = millis();
    CaseVar = 2;
    break;
  case 2:
    digitalWrite(SIDECAR_DIGITAL2, LOW);
    digitalWrite(SIDECAR_DIGITAL3, LOW);
    if ((AENT + ASTALL) < millis()) {
      CaseVar = 3;
    }
    break;
  case 3:
    BENT =millis();
    CaseVar = 4;
    LOHI = 0;
    break;
  case 4:
    digitalWrite(SIDECAR_DIGITAL2, HIGH);
    digitalWrite(SIDECAR_DIGITAL3, LOW);
    if ((BENT + BSTALL) < millis()) {
      CaseVar = 5;
    }
    break;
  case 5:
    ShootBtn = usb1.getBtn(BTN2, NORMAL);
    digitalWrite(SIDECAR_DIGITAL2, LOW);
    digitalWrite(SIDECAR_DIGITAL3, LOW);
    if (ShootBtn == HIGH) {
      CaseVar = 6;
    }
    if (LOHI == 1) {
      CaseVar = 3;
    }
    break;
  case 6:
    CENT = millis();
    CaseVar = 7;
    break;
  case 7:
    digitalWrite(SIDECAR_DIGITAL2, LOW);
    digitalWrite(SIDECAR_DIGITAL3, HIGH);
    if ((CENT + CSTALL) < millis()) {
      CaseVar = 1;
    }
    break;
  }
} 



/* This is called while the robot is disabled
 * You must make sure to set all of your outputs
 * to safe/disable values here
 */
void disabled() {
  SD1 = LOW;
  digitalWrite(SIDECAR_DIGITAL2, LOW);
  digitalWrite(SIDECAR_DIGITAL3, LOW);
  digitalWrite(SIDECAR_DIGITAL4, LOW);
  digitalWrite(SIDECAR_DIGITAL5, LOW);
  Compressor = LOW;
  Enabled = LOW;
  // PWMs are automatically disabled
}


/* This loop ALWAYS runs - only place code here that can run during a disabled state
 * This is also a good spot to put driver station publish code
 * You can use either publishAnalog, publishDigital, publishByte, publishShort, or publishLong
 * Specify a bundle ID with a single character (a-z, A-Z, 0-9) - Just make sure not to use the same twice!
 */
void timedtasks() {
  RobotOpen.publishAnalog(ANALOG0, 'A');   // Bundle A
  RobotOpen.publishAnalog(ANALOG1, 'B');   // Bundle B
  RobotOpen.publishAnalog(ANALOG2, 'C');   // Bundle C
  RobotOpen.publishAnalog(ANALOG3, 'D');   // Bundle D
  RobotOpen.publishAnalog(ANALOG4, 'E');   // Bundle E
  RobotOpen.publishAnalog(ANALOG5, 'F');   // Bundle F
  digitalWrite(SIDECAR_DIGITAL1, SD1);
  digitalWrite(SIDECAR_DIGITAL6, Compressor);
  LastEnabled = Enabled;  
  //==================
  Serial.print("The BallMotor PWM = ");
  Serial.println(JsBtn3Toggle);
  //===================
}


/* This is the main program loop that keeps comms operational
 * There's no need to touch anything here!!!
 */
void loop() {
  RobotOpen.pollDS();
  if (RobotOpen.enabled())
    enabled();
  else
    disabled();
  timedtasks();
  RobotOpen.outgoingDS();
}












