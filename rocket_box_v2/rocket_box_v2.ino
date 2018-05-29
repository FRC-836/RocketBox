#include "DebounceTimer.h"

#define OUTPUT_ENABLED

// Deadman switch to control firing authority to prevent accidental / unpermitted firing 
DebounceTimer deadmanSwitch(10);

// Gigantic red FIRE button
DebounceTimer fireButton(7);

// Rocker controlled enabled switches
DebounceTimer leftEnableSwitch(8);
DebounceTimer rightEnableSwitch(9);

// Outputs
const int FIRE_LIGHT_PIN = 2;
const int LEFT_FIRE_PIN = 3;
const int RIGHT_FIRE_PIN = 4;

// Time that solenoids will be open
const int FIRE_TIME = 200;

// Delay after firing to allow system to recharge
const int FIRE_DELAY = 2000;

// Defaults to HIGH because SainSmart is trigged on logic low
int fireLightState = HIGH;
int prevFireLightState = HIGH;

// Prevents fire button from being held down
int prevFired = 0;

void setup() 
{
  //begin serial communication
  Serial.begin(9600);

  //setup the output pins
  pinMode(FIRE_LIGHT_PIN, OUTPUT);
  pinMode(LEFT_FIRE_PIN, OUTPUT);
  pinMode(RIGHT_FIRE_PIN, OUTPUT);

  //set the outputs to a known state
  digitalWrite(FIRE_LIGHT_PIN, HIGH);
  digitalWrite(LEFT_FIRE_PIN, HIGH);
  digitalWrite(RIGHT_FIRE_PIN, HIGH);
}

void loop() 
{
  int leftEnabledState = leftEnableSwitch.debounceSignal(2000);
  #ifdef OUTPUT_ENABLED
  Serial.print("left enable switch: ");
  Serial.print(leftEnabledState);
  Serial.print("\t");
  #endif
  int rightEnabledState = rightEnableSwitch.debounceSignal(2000);
  #ifdef OUTPUT_ENABLED
  Serial.print("right enable switch: ");
  Serial.print(rightEnabledState);
  Serial.print("\t");
  #endif
  int deadmanSwitchState = deadmanSwitch.debounceSignal(2000);
  #ifdef OUTPUT_ENABLED
  Serial.print("deadman switch: ");
  Serial.print(deadmanSwitchState);
  Serial.print("\t");
  #endif
  
  int fire = fireButton.debounceSignal(2000);
  #ifdef OUTPUT_ENABLED
  Serial.print("fireButton: ");
  Serial.print(fire);
  Serial.print("\t");
  #endif
  
  // If either of the rockets are enabled and the deadman switch is pressed
  if( deadmanSwitchState == HIGH && ( ( leftEnabledState == HIGH ) || ( rightEnabledState == HIGH ) ) )
  {
    fireLightState = LOW;
  } //end  if( deadmanSwitchState == HIGH && ( ( leftEnabledState == HIGH ) || ( rightEnabledState == HIGH ) ) )
  else
  {
    fireLightState = HIGH;
  } //end  else

  // If the previous state of the fire enable is different then the current state
  if( prevFireLightState != fireLightState )
  {
    // Update the fire enable state
    prevFireLightState = fireLightState;
    digitalWrite(FIRE_LIGHT_PIN, fireLightState);

    if( fireLightState == LOW )
    {
      Serial.println("Fire Button Enabled");
    } //end  if( fireLightState == LOW )
    else
    {
      Serial.println("Fire Button Disabled");
    } //end  else
  } //end  if( prevFireLightState != fireLightState )

  // If the fire button as been pressed and not held down for multiple loops
  if( fire == HIGH && prevFired == 0) // If fire button is pressed
  {
    // If fire enable state is low (low for SainSmart logic)
    if( fireLightState == LOW )
    {
      if( leftEnabledState == HIGH && rightEnabledState == LOW )
      {
        // Only firing left rocket
        Serial.println("Firing Left Rocket");
        digitalWrite(LEFT_FIRE_PIN, LOW);
        delay(FIRE_TIME);
        digitalWrite(LEFT_FIRE_PIN, HIGH);
        delay(FIRE_DELAY);
        prevFired = 1;
      } //end  if( leftEnabledState == HIGH && rightEnabledState == LOW )
      else if( leftEnabledState == LOW && rightEnabledState == HIGH )
      {
        // Only firing right rocket
        Serial.println("Firing Right Rocket");
        digitalWrite(RIGHT_FIRE_PIN, LOW);
        delay(FIRE_TIME);
        digitalWrite(RIGHT_FIRE_PIN, HIGH);
        delay(FIRE_DELAY);
        prevFired = 1;
      } //end  else if( leftEnabledState == LOW && rightEnabledState == HIGH )
      else if( leftEnabledState == HIGH && rightEnabledState == HIGH )
      {
        // Fire both rockets!
        Serial.println("Firing Both Rockets");
        digitalWrite(LEFT_FIRE_PIN, LOW);
        digitalWrite(RIGHT_FIRE_PIN, LOW);
        delay(FIRE_TIME);
        digitalWrite(LEFT_FIRE_PIN, HIGH);
        digitalWrite(RIGHT_FIRE_PIN, HIGH);
        delay(FIRE_DELAY);
        prevFired = 1;
      } //end  else if( leftEnabledState == HIGH && rightEnabledState == HIGH )
    } //end  if( fireLightState == LOW )
  } //end  if( fire == HIGH && prevFired == 0) // If fire button is pressed
  // If the fire button is not pressed during the current loop, reset prevFired
  else if (fire == LOW)
  {
    prevFired = 0;
  } //end  else if (fire == LOW)
  Serial.println();
}

