
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int ThermistorPin = 0;
int relayPin = 10;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc;
float Tdesired = 27.0;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

bool logging = false;

String fermentationTypes[] = {"Pre-Ferment", "Autolyse", "Bulk Ferment", "Bench Rest", "Proofing"};
int selectedItem = 0;
int arrayCount = sizeof(fermentationTypes) / sizeof(fermentationTypes[0]);

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);

  Serial.print("Elapsed, ");
  
  // Print out the header for the CSV
  for (int iterator = 0; iterator < arrayCount; iterator++) {
    if (iterator != (arrayCount - 1)) {
      Serial.print(fermentationTypes[iterator] + ", ");
    } else {
      Serial.println(fermentationTypes[iterator]);
    }
  }

  setLoggingOff();
}

void loop() {

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Tc = round(Tc*10)/10.0;

  lcd.print(Tc,1);
  lcd.print((char)223);
  lcd.print("C");
  lcd.print(" -> ");
  lcd.print(Tdesired,1);
  lcd.print((char)223);
  lcd.print("C");

  if (Tc < Tdesired) {
      heaterOn();
  } else {
      heaterOff();
  }

  showPhase();

  if (logging == true) {
    lcd.setCursor(14, 1);
    lcd.print("ON");
    log();
  } else {
    lcd.setCursor(13, 1);
    lcd.print("OFF");
  }

  delay(1000);
}

//  cyclePhase();
//  cycleLogging();

void cycleLogging() {
  if (logging == true) {
    setLoggingOff();
  } else {
    setLoggingOn();
  }
}

void setLoggingOn() {
  logging = true;
}

void setLoggingOff() {
  logging = false;
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
  Serial.print(formatTime(millis()));
  Serial.print(", ");
  Serial.print(Tc);
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(", ");
  Serial.println(", ");
}

void logAutolyse() {
  Serial.print(formatTime(millis()));
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(Tc);
  Serial.print(", ");
  Serial.print(", ");
  Serial.println(", ");
}

void logBulkFerment() {
  Serial.print(formatTime(millis()));
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(Tc);
  Serial.print(", ");
  Serial.println(", ");
}

void logBenchRest() {
  Serial.print(formatTime(millis()));
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(Tc);
  Serial.println(", ");
}

void logProofing() {
  Serial.print(formatTime(millis()));
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(", ");
  Serial.println(Tc);
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
//  lcd.setCursor(0, 1);
//  lcd.print("Heater on");
}

void heaterOff() {
  digitalWrite(relayPin, LOW);
//  lcd.setCursor(0, 1);
//  lcd.print("Heater off");
}
