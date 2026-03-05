const int GREEN_LED = 8;     
const int GREEN_LED_2 = 10;  
const int RED_LED = 9;       

String inputString = "";         
boolean stringComplete = false;   

bool eyesOpen = false;
bool eyesClosed = false;
unsigned long previousMillis = 0;
const long blinkInterval = 1000;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);          

  pinMode(GREEN_LED, OUTPUT);
  pinMode(GREEN_LED_2, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(GREEN_LED_2, LOW);
  digitalWrite(RED_LED, LOW);
  
  delay(1000);
  
  Serial.println("ARDUINO:READY");
  blinkAllLEDs(3, 200);
}

void loop() {
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
  
  if (stringComplete) {
    processCommand();
    inputString = "";
    stringComplete = false;
  }

  if (eyesClosed) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      digitalWrite(RED_LED, !digitalRead(RED_LED));
    }
  } else {
    digitalWrite(RED_LED, LOW);
  }

  delay(5);
}

void processCommand() {
  inputString.trim();
  
  Serial.print("Arduino Received: '");
  Serial.print(inputString);
  Serial.println("'");

  if (inputString == "EYES_OPEN") {
    eyesOpen = true;
    eyesClosed = false;

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(GREEN_LED_2, HIGH);
    digitalWrite(RED_LED, LOW);
    Serial.println("LED:GREEN_ON");
  }
  else if (inputString == "EYES_CLOSED") {
    eyesOpen = false;
    eyesClosed = true;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(GREEN_LED_2, LOW);
    // RED LED will blink in loop
    Serial.println("LED:RED_BLINKING");
  }
  else if (inputString == "BOTH_OFF") {
    eyesOpen = false;
    eyesClosed = false;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(GREEN_LED_2, LOW);
    digitalWrite(RED_LED, LOW);
    Serial.println("LED:BOTH_OFF");
  }
  else if (inputString == "TEST") {
    runTestSequence();
  }
  else if (inputString == "PING") {
    Serial.println("PONG");
  }
  else {
    Serial.print("UNKNOWN COMMAND: ");
    Serial.println(inputString);
  }
}

void blinkAllLEDs(int count, int duration) {
  for(int i = 0; i < count; i++) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(GREEN_LED_2, HIGH);
    digitalWrite(RED_LED, HIGH);
    delay(duration);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(GREEN_LED_2, LOW);
    digitalWrite(RED_LED, LOW);
    delay(duration);
  }
}

void runTestSequence() {
  Serial.println("TEST:START");
  
  digitalWrite(GREEN_LED, HIGH);
  delay(500);
  digitalWrite(GREEN_LED, LOW);

  digitalWrite(GREEN_LED_2, HIGH);
  delay(500);
  digitalWrite(GREEN_LED_2, LOW);
  
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  
  blinkAllLEDs(2, 100);
  
  Serial.println("TEST:COMPLETE");
}