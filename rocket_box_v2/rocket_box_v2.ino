
// Deadman switch to control firing authority to prevent accidental / unpermitted firing 
const int deadmanSwitchPin = 10;

// Gigantic red FIRE button
const int firePin = 7;

// Rocker controlled enabled switches
const int leftEnabledPin = 8;
const int rightEnabledPin = 9;

// Outputs
const int fireLightPin = 2;
const int leftFirePin = 3;
const int rightFirePin = 4;

// Time that solenoids will be open
const int fireTime = 200;

// Delay after firing to allow system to recharge
const int fireDelay = 2000;

// Defaults to HIGH because SainSmart is trigged on logic low
int fireLightState = HIGH;
int prevFireLightState = HIGH;

// Prevents fire button from being held down
int prevFired = 0;


void setup() {

  Serial.begin(9600);

  digitalWrite(fireLightPin, HIGH);
  digitalWrite(leftFirePin, HIGH);
  digitalWrite(rightFirePin, HIGH);

  pinMode(fireLightPin, OUTPUT);
  pinMode(leftFirePin, OUTPUT);
  pinMode(rightFirePin, OUTPUT);

  pinMode(deadmanSwitchPin, INPUT);
  pinMode(firePin, INPUT);
  pinMode(leftEnabledPin, INPUT);
  pinMode(rightEnabledPin, INPUT); 

}

void loop() {

  int leftEnabledState = digitalRead(leftEnabledPin);
  int rightEnabledState = digitalRead(rightEnabledPin);

  int deadmanSwitchState = digitalRead(deadmanSwitchPin);

  int fire = digitalRead(firePin);

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
    digitalWrite(fireLightPin, fireLightState);

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
        digitalWrite(leftFirePin, LOW);
        delay(fireTime);
        digitalWrite(leftFirePin, HIGH);
        delay(fireDelay);
        prevFired = 1;
      }
      else if( leftEnabledState == LOW && rightEnabledState == HIGH )
      {
        // Only firing right rocket
        Serial.println("Firing Right Rocket");
        digitalWrite(rightFirePin, LOW);
        delay(fireTime);
        digitalWrite(rightFirePin, HIGH);
        delay(fireDelay);
        prevFired = 1;
      }
      else if( leftEnabledState == HIGH && rightEnabledState == HIGH )
      {
        // Fire both rockets!
        Serial.println("Firing Both Rockets");
        digitalWrite(leftFirePin, LOW);
        digitalWrite(rightFirePin, LOW);
        delay(fireTime);
        digitalWrite(leftFirePin, HIGH);
        digitalWrite(rightFirePin, HIGH);
        delay(fireDelay);
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
