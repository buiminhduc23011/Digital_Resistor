#define UD_PIN  2     // Output pin for up/down control
#define INC_PIN 3     // Output pin for increment control
#define CS_PIN  4     // Output pin for chip select control
int voltageSamplePin = A2;  // Analog input pin for voltage sample
int maxValue;  // Maximum resistance value (set this as needed)
int targetValue;
int currentValue;
int scaledValue;

void stepUp() {
  digitalWrite(UD_PIN, HIGH);
  digitalWrite(INC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);  // Enable the chip
  delayMicroseconds(1);
  digitalWrite(INC_PIN, LOW);  // Trigger increment
  delayMicroseconds(1);
  digitalWrite(INC_PIN, HIGH);  // Return to idle state
  delayMicroseconds(1);
  digitalWrite(CS_PIN, HIGH);  // Disable the chip and save position
}

void stepDown() {
  digitalWrite(UD_PIN, LOW);
  digitalWrite(INC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);  // Enable the chip
  delayMicroseconds(1);
  digitalWrite(INC_PIN, LOW);  // Trigger decrement
  delayMicroseconds(1);
  digitalWrite(INC_PIN, HIGH);  // Return to idle state
  delayMicroseconds(1);
  digitalWrite(CS_PIN, HIGH);  // Disable the chip and save position
}

void setup() {
  Serial.begin(9600);
  pinMode(INC_PIN, OUTPUT);
  pinMode(UD_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  
  digitalWrite(CS_PIN, HIGH);  // Initialize chip to disabled state
  currentValue = 0;  // Initial slider position
  scaledValue = 0;   // Initial scaled value
}

void loop() {
  // Scale the analog read value to match the maximum value
  scaledValue = analogRead(voltageSamplePin) / maxValue;
  targetValue = scaledValue * maxValue;
  
  // Adjust the currentValue to approach the targetValue
  if (currentValue < targetValue) {
    stepUp();
    currentValue++;
  } else if (currentValue > targetValue) {
    stepDown();
    currentValue--;
  }
  
  // Clamp currentValue within valid range
  currentValue = constrain(currentValue, 0, maxValue);
  
  delay(10);  // Small delay for stability
}
