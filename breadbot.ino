#include <LiquidCrystal.h>
#include <SD.h>

// test

LiquidCrystal lcd(A4, A5, 5, 4, 3, 2);

//SPI SD Card Pins
//MOSI = Pin 11
//MISO = Pin 12
//SCLK = PIN 13
int CS_pin = 8;

int ThermistorPin = 0;
int relayPin = 10;
int cycleButtonPin = 6;
int logButtonPin = 7;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, TcRounded;
float Tdesired = 27.0;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
unsigned long loggingTurnedOnAt;

String timeToShow;

File logFile;

bool logging = false;

String fermentationTypes[] = {"Pre-Ferment", "Autolyse", "Bulk Ferment", "Bench Rest", "Proofing"};
String fermentationTypesAbbreviated[] = {"Pre-Fer", "Auto", "Bulk", "Rest", "Proof"};
int selectedItem = 0;
int arrayCount = sizeof(fermentationTypes) / sizeof(fermentationTypes[0]);

void setup() {
  Serial.begin(9600);
  
  pinMode(relayPin, OUTPUT);
  pinMode(cycleButtonPin, INPUT);
  digitalWrite(cycleButtonPin, HIGH);
  pinMode(logButtonPin, INPUT);
  digitalWrite(logButtonPin, HIGH);
  
  Serial.println("Initializing Card");
  digitalWrite(CS_pin, HIGH);
  pinMode(CS_pin, OUTPUT);

  // Initialize Card
  if (!SD.begin(CS_pin)) {
    Serial.println("Card Failure");
    return;
  }
  Serial.println("Card Ready");

  // Write log file header
  // This happens every time you power on the Arduino.
  logFile = SD.open("BRDLOG.CSV", FILE_WRITE);
  if (logFile) {
    logFile.print("Elapsed, ");
    Serial.print("Elapsed, ");
    // Print out the header for the CSV
    for (int iterator = 0; iterator < arrayCount; iterator++) {
      if (iterator != (arrayCount - 1)) {
        logFile.print(fermentationTypes[iterator] + ", ");
        Serial.print(fermentationTypes[iterator] + ", ");
      } else {
        logFile.println(fermentationTypes[iterator]);
        Serial.println(fermentationTypes[iterator]);
      }
    }
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }

  setLoggingOff();
}

void loop() {

  if (!digitalRead(cycleButtonPin) == true) {
    cyclePhase();
  }

  if (!digitalRead(logButtonPin) == true) {
    toggleLogging();
  }

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Tc = round(Tc*10)/10.0;
  
  // This rounds the temp to 0.25 increments, so that hopefully 0.1 changes don't flick the light on and off repeatedly.
  TcRounded = round(Tc*4.0)/4.0;

  lcd.print(Tc,1);
  lcd.print((char)223);
  lcd.print("C");
  lcd.print(" -> ");
  lcd.print(Tdesired,1);
  lcd.print((char)223);
  lcd.print("C");

  if (TcRounded < Tdesired) {
      heaterOn();
  } else {
      heaterOff();
  }

  showPhase();

  if (logging == true) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(fermentationTypesAbbreviated[selectedItem]);
    lcd.setCursor(7, 1);
    lcd.print(" " + timeToShow);
    log();
  }

  delay(1000);
}

void toggleLogging() {
  if (logging == true) {
    setLoggingOff();
  } else {
    setLoggingOn();
  }
}

void setLoggingOn() {
  loggingTurnedOnAt = millis();
  logging = true;
}

void setLoggingOff() {
  logging = false;
  timeToShow = "00:00:00";
}

void showPhase() {
  lcd.setCursor(0, 1);
  lcd.print(fermentationTypes[selectedItem]);
}

void cyclePhase() {
  setLoggingOff();
  if (selectedItem == (arrayCount - 1)) {
    selectedItem = 0;
  } else {
    selectedItem += 1;
  }
}

void log() {
  if (selectedItem == 0) {
    logPreFerment();
  } else if (selectedItem == 1) {
    logAutolyse();
  } else if (selectedItem == 2) {
    logBulkFerment();
  } else if (selectedItem == 3) {
    logBenchRest();
  } else if (selectedItem == 4) {
    logProofing();
  }
}

void logPreFerment() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.CSV", FILE_WRITE);
  if (logFile) {
    Serial.print(timeToShow);
    Serial.print(", ");
    Serial.print(Tc);
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(", ");
    Serial.println(", ");
    logFile.print(timeToShow);
    logFile.print(", ");
    logFile.print(Tc);
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(", ");
    logFile.println(", ");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logAutolyse() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.CSV", FILE_WRITE);
  if (logFile) {
    Serial.print(timeToShow);
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(Tc);
    Serial.print(", ");
    Serial.print(", ");
    Serial.println(", ");
    logFile.print(timeToShow);
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(Tc);
    logFile.print(", ");
    logFile.print(", ");
    logFile.println(", ");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logBulkFerment() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.CSV", FILE_WRITE);
  if (logFile) {
    Serial.print(timeToShow);
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(Tc);
    Serial.print(", ");
    Serial.println(", ");
    logFile.print(timeToShow);
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(Tc);
    logFile.print(", ");
    logFile.println(", ");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logBenchRest() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.CSV", FILE_WRITE);
  if (logFile) {
    Serial.print(timeToShow);
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(Tc);
    Serial.println(", ");
    logFile.print(timeToShow);
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(Tc);
    logFile.println(", ");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logProofing() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.CSV", FILE_WRITE);
  if (logFile) {
    Serial.print(timeToShow);
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(", ");
    Serial.print(", ");
    Serial.println(Tc);
    logFile.print(timeToShow);
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(", ");
    logFile.print(", ");
    logFile.println(Tc);
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

String formatTime(unsigned long time) {
  float h, m, s, ms;
  unsigned long over;
  
  h = int(time / 3600000);
  over = time % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;

  String formattedTime;
  
  // display the results
  if (h<10) formattedTime += 0;
  formattedTime += int(h);
  formattedTime += ":";
  if (m<10) formattedTime += 0;
  formattedTime += int(m);
  formattedTime += ":";
  if (s<10) formattedTime += 0;
  formattedTime += int(s);

  return formattedTime;
}

void heaterOn() {
  digitalWrite(relayPin, HIGH);
}

void heaterOff() {
  digitalWrite(relayPin, LOW);
}
