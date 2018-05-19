// Deadman switch to control firing authority to prevent accidental / unpermitted firing 
const int DEADMAN_SWITCH_PIN = 10;

// Gigantic red FIRE button
const int FIRE_PIN = 7;

// Rocker controlled enabled switches
const int LEFT_ENABLE_PIN = 8;
const int RIGHT_ENABLE_PIN = 9;

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


void setup() {

  Serial.begin(9600);

  digitalWrite(FIRE_LIGHT_PIN, HIGH);
  digitalWrite(LEFT_FIRE_PIN, HIGH);
  digitalWrite(RIGHT_FIRE_PIN, HIGH);

  pinMode(FIRE_LIGHT_PIN, OUTPUT);
  pinMode(LEFT_FIRE_PIN, OUTPUT);
  pinMode(RIGHT_FIRE_PIN, OUTPUT);

  pinMode(DEADMAN_SWITCH_PIN, INPUT);
  pinMode(FIRE_PIN, INPUT);
  pinMode(LEFT_ENABLE_PIN, INPUT);
  pinMode(RIGHT_ENABLE_PIN, INPUT); 

}

void loop() {

  int leftEnabledState = digitalRead(LEFT_ENABLE_PIN);
  int rightEnabledState = digitalRead(RIGHT_ENABLE_PIN);

  int deadmanSwitchState = digitalRead(DEADMAN_SWITCH_PIN);

  int fire = digitalRead(FIRE_PIN);

  // If either of the rockets are enabled and the deadman switch is pressed
  if( deadmanSwitchState == HIGH && ( ( leftEnabledState == HIGH ) || ( rightEnabledState == HIGH ) ) )
  {
    fireLightState = LOW;
  }
  else
  {
    fireLightState = HIGH;
  }

  // If the previous state of the fire enable is different then the current state
  if( prevFireLightState != fireLightState )
  {
    // Update the fire enable state
    prevFireLightState = fireLightState;
    digitalWrite(FIRE_LIGHT_PIN, fireLightState);

    if( fireLightState == LOW )
    {
      Serial.println("Fire Button Enabled");
    }
    else
    {
      Serial.println("Fire Button Disabled");
    }
  }

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
      }
      else if( leftEnabledState == LOW && rightEnabledState == HIGH )
      {
        // Only firing right rocket
        Serial.println("Firing Right Rocket");
        digitalWrite(RIGHT_FIRE_PIN, LOW);
        delay(FIRE_TIME);
        digitalWrite(RIGHT_FIRE_PIN, HIGH);
        delay(FIRE_DELAY);
        prevFired = 1;
      }
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
      }
    }
  }
  // If the fire button is not pressed during the current loop, reset prevFired
  else if (fire == LOW)
  {
    prevFired = 0;
  }

  

}
