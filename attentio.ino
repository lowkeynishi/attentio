const int pirSensorPin = 2;

// RGB pins
const int redPin   = 12;
const int greenPin = 11;
const int bluePin  = 13;

// HC-SR04 pins
const int trigPin = 4;
const int echoPin = 5;

// Buzzer
const int buzzerPin = 8;

// PIR state
int pirSensorValue = LOW;
int lastPirValue = LOW;

// Activity score
int activityScore = 0;
const int maxActivityScore = 100;

unsigned long lastMotionEventTime = 0;
const unsigned long minEventGap = 500;   // debounce between counted motion events

unsigned long lastDecayTime = 0;
const unsigned long decayInterval = 3000; // score decays every 3 second
const int decayAmount = 5;
const int motionAddAmount = 15;

// Ultrasonic
long duration = 0;
float distanceCm = 0;

// Startup warm-up
unsigned long startTime;
const unsigned long warmupTime = 60000;   // 60 seconds

// Green-zone hysteresis
bool greenZoneActive = false;
const float greenEnterMin = 32.0;
const float greenEnterMax = 78.0;
const float greenExitMin  = 28.0;
const float greenExitMax  = 85.0;

// LED state tracking
enum LedState { LED_BLUE, LED_GREEN, LED_YELLOW, LED_RED };
LedState currentLedState = LED_BLUE;
LedState lastPrintedLedState = (LedState)(-1);

// Distance print timer
unsigned long lastDistancePrintTime = 0;
const unsigned long distancePrintInterval = 1000;

// Score print timer
unsigned long lastScorePrintTime = 0;
const unsigned long scorePrintInterval = 1000;

bool warmupFinishedPrinted = false;

void setColor(bool red, bool green, bool blue) {
  digitalWrite(redPin, red ? HIGH : LOW);
  digitalWrite(greenPin, green ? HIGH : LOW);
  digitalWrite(bluePin, blue ? HIGH : LOW);
}

void applyLedState(LedState state) {
  currentLedState = state;

  if (state == LED_RED) {
    setColor(true, false, false);
  } else if (state == LED_YELLOW) {
    setColor(true, true, false);
  } else if (state == LED_GREEN) {
    setColor(false, true, false);
  } else {
    setColor(false, false, true);
  }
}

void printLedStateIfChanged() {
  if (currentLedState != lastPrintedLedState) {
    lastPrintedLedState = currentLedState;

    Serial.print("LED -> ");
    if (currentLedState == LED_RED) {
      Serial.println("RED");
    } else if (currentLedState == LED_YELLOW) {
      Serial.println("YELLOW");
    } else if (currentLedState == LED_GREEN) {
      Serial.println("GREEN");
    } else {
      Serial.println("BLUE");
    }
  }
}

float readDistanceCm() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return (duration * 0.0343) / 2.0;
}

void setup() {
  Serial.begin(9600);

  pinMode(pirSensorPin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzerPin, OUTPUT);

  applyLedState(LED_BLUE);
  startTime = millis();

  Serial.println("System starting...");
  Serial.println("PIR warming up for 60 seconds...");
}

void loop() {
  unsigned long now = millis();

  // Warm-up phase
  if (now - startTime < warmupTime) {
    applyLedState(LED_BLUE);
    noTone(buzzerPin);
    printLedStateIfChanged();
    return;
  }

  // Print only once when warm-up is done
  if (!warmupFinishedPrinted) {
    Serial.println("PIR warm-up finished. System is now active.");
    warmupFinishedPrinted = true;
  }

  // Read PIR
  pirSensorValue = digitalRead(pirSensorPin);

  // Add score only on rising edge
  if (pirSensorValue == HIGH && lastPirValue == LOW) {
    if (now - lastMotionEventTime >= minEventGap) {
      activityScore += motionAddAmount;
      if (activityScore > maxActivityScore) {
        activityScore = maxActivityScore;
      }

      lastMotionEventTime = now;

      Serial.print("Motion detected. Activity score = ");
      Serial.println(activityScore);
    }
  }

  lastPirValue = pirSensorValue;

  // Decay score gradually
  if (now - lastDecayTime >= decayInterval) {
    lastDecayTime = now;

    if (activityScore > 0) {
      activityScore -= decayAmount;
      if (activityScore < 0) {
        activityScore = 0;
      }
    }
  }

  // Read distance
  distanceCm = readDistanceCm();

  // Print distance once per second
  if (now - lastDistancePrintTime >= distancePrintInterval) {
    lastDistancePrintTime = now;

    Serial.print("Distance: ");
    if (distanceCm < 0) {
      Serial.println("No reading");
    } else {
      Serial.print(distanceCm);
      Serial.println(" cm");
    }
  }

  // Print score once per second
  if (now - lastScorePrintTime >= scorePrintInterval) {
    lastScorePrintTime = now;

    Serial.print("Activity score: ");
    Serial.println(activityScore);
  }

  // Green zone hysteresis
  if (distanceCm > 0) {
    if (!greenZoneActive) {
      if (distanceCm >= greenEnterMin && distanceCm <= greenEnterMax) {
        greenZoneActive = true;
      }
    } else {
      if (distanceCm < greenExitMin || distanceCm > greenExitMax) {
        greenZoneActive = false;
      }
    }
  } else {
    greenZoneActive = false;
  }

  // Main output logic
  if (distanceCm > 0 && distanceCm < 30) {
    applyLedState(LED_RED);
    tone(buzzerPin, 2500);
  }
  else if (activityScore >= 75) {
    applyLedState(LED_RED);
    noTone(buzzerPin);
  }
  else if (activityScore >= 35) {
    applyLedState(LED_YELLOW);
    noTone(buzzerPin);
  }
  else if (greenZoneActive) {
    applyLedState(LED_GREEN);
    noTone(buzzerPin);
  }
  else {
    applyLedState(LED_BLUE);
    noTone(buzzerPin);
  }

  printLedStateIfChanged();

  delay(200);
}