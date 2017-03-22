// If power is lost and timer restarts, you can add timestamps in Google Sheets
// =(TIME(3,48,23)+TIME(0,45,3))

#include <LiquidCrystal.h>
#include <SD.h>

LiquidCrystal lcd(A4, A5, 5, 4, 3, 2);

// SPI SD Card Pins
// MOSI = Pin 11
// MISO = Pin 12
// SCLK = Pin 13
int SD_CS_Pin = 8;

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
  digitalWrite(SD_CS_Pin, HIGH);
  pinMode(SD_CS_Pin, OUTPUT);

  // Initialize Card
  if (!SD.begin(SD_CS_Pin)) {
    Serial.println("Card Failure");
    return;
  } else {
    Serial.println("Card Ready");
  }

  // Write log file header
  // This happens every time you power on the Arduino.
  logFile = SD.open("BRDLOG.TXT", FILE_WRITE);
  logFile.close();

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
  
  // If the temp is above the desired temp.
  // Round the temp to 0.20 increments, to avoid
  // 0.1 changes flicking the light on and off repeatedly.
  if (Tc > Tdesired) {
    TcRounded = round(Tc * 5.0) / 5.0;
  } else {
    TcRounded = Tc;
  }

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
  logFile = SD.open("BRDLOG.TXT", FILE_WRITE);
  if (logFile) {
    Serial.println("\{\"stage\": \"Pre-Ferment\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.println("\{\"stage\": \"Pre-Ferment\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logAutolyse() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.TXT", FILE_WRITE);
  if (logFile) {
    Serial.println("\{\"stage\": \"Autolyse\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.println("\{\"stage\": \"Autolyse\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logBulkFerment() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.TXT", FILE_WRITE);
  if (logFile) {
    Serial.println("\{\"stage\": \"Bulk Ferment\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.println("\{\"stage\": \"Bulk Ferment\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logBenchRest() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.TXT", FILE_WRITE);
  if (logFile) {
    Serial.println("\{\"stage\": \"Bench Rest\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.println("\{\"stage\": \"Bench Rest\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.close();
  } else {
    Serial.println("Couldn't open log file");
  }
}

void logProofing() {
  unsigned long time = millis() - loggingTurnedOnAt;
  timeToShow = formatTime(time);
  // Write data to log file
  logFile = SD.open("BRDLOG.TXT", FILE_WRITE);
  if (logFile) {
    Serial.println("\{\"stage\": \"Proofing\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
    logFile.println("\{\"stage\": \"Proofing\", \"time\": \"" + timeToShow + "\", \"temp\": " + Tc + "\},");
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
