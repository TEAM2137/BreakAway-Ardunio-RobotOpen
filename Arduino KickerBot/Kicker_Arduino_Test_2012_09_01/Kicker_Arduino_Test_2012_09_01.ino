#include <SPI.h>
#include <Ethernet.h>
#include <RobotOpen.h>


/* I/O Setup */
USBJoystick usb1('0');  // Assign the logitech USBJoystick object to bundle 0
int lastButtonState = LOW;
int buttonState;
int DeInput;
int lastdeimput = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;
int toggletrue = LOW;
int SD1 = LOW;

void setup()
{
  /* Initiate comms */
  RobotOpen.begin();
  pinMode(SIDECAR_DIGITAL1, OUTPUT);
  pinMode(SIDECAR_DIGITAL2, OUTPUT);
  pinMode(SIDECAR_DIGITAL3, OUTPUT);
  pinMode(SIDECAR_DIGITAL4, OUTPUT);
  pinMode(SIDECAR_DIGITAL5, INPUT);
}


/* This is your primary robot loop - all of your code
 * should live here that allows the robot to operate
 */
void enabled() {
  // Constantly update PWM values with joystick values
  RobotOpen.setPWM(SIDECAR_PWM1, usb1.makePWM(ANALOG_LEFTY, INVERT));
  RobotOpen.setPWM(SIDECAR_PWM2, usb1.makePWM(ANALOG_LEFTY, INVERT));
  RobotOpen.setPWM(SIDECAR_PWM3, usb1.makePWM(ANALOG_RIGHTY, INVERT));
  RobotOpen.setPWM(SIDECAR_PWM4, usb1.makePWM(ANALOG_RIGHTY, INVERT));
  int reading = usb1.getBtn(BTN1, NORMAL);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }
  DeInput = reading;
  if (DeInput == HIGH && lastdeimput == LOW && SD1 == HIGH) {
       SD1 = LOW;
      toggletrue = HIGH;
    }
  if (DeInput == HIGH && lastdeimput == LOW && SD1 == LOW && toggletrue == LOW) {
     SD1 = HIGH;
  }
  lastdeimput = DeInput;
  toggletrue = LOW;
 digitalWrite(SIDECAR_DIGITAL3, lastdeimput);
} 



/* This is called while the robot is disabled
 * You must make sure to set all of your outputs
 * to safe/disable values here
 */
void disabled() {
  digitalWrite(SIDECAR_DIGITAL2, LOW);
  digitalWrite(SIDECAR_DIGITAL3, LOW);
  SD1 = LOW;
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
