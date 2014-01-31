#include <SPI.h>
#include <Ethernet.h>
#include <RobotOpen.h>


/* I/O Setup */
USBJoystick usb1('0');  // Assign the logitech USBJoystick object to bundle 0


void setup()
{
  /* Initiate comms */
  RobotOpen.begin();
}


/* This is your primary robot loop - all of your code
 * should live here that allows the robot to operate
 */
void enabled() {
  // Constantly update PWM values with joystick values
  int leftDrive = usb1.makePWM(ANALOG_RIGHTY, INVERT) - (127 - usb1.makePWM(ANALOG_RIGHTX, NORMAL));
  int rightDrive = usb1.makePWM(ANALOG_RIGHTY, INVERT) + (127 - usb1.makePWM(ANALOG_RIGHTX, NORMAL));
  
  if (leftDrive > 255)
    leftDrive = 255;
  else if (leftDrive < 0)
    leftDrive = 0;
  if (rightDrive > 255)
    rightDrive = 255;
  else if (rightDrive < 0)
    rightDrive = 0;
  
  RobotOpen.setPWM(SIDECAR_PWM1, 255 - leftDrive);
  RobotOpen.setPWM(SIDECAR_PWM2, rightDrive);
}


/* This is called while the robot is disabled
 * You must make sure to set all of your outputs
 * to safe/disable values here
 */
void disabled() {
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