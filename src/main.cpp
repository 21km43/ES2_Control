#include <Adafruit_INA228.h>
#include <Wire.h>

const int RESET_PIN = 7;

const uint8_t DCDC18_ADDR = 0x0B;
const uint8_t DCDC09_ADDR = 0x08;
uint16_t dcdc18_value = 280;
uint16_t dcdc09_value = 100;
float dcdc18_voltage = 1.8f;
float dcdc09_voltage = 0.9f;

Adafruit_INA228 ina228_18 = Adafruit_INA228();
Adafruit_INA228 ina228_09 = Adafruit_INA228();

void setDCDCVoltage(uint8_t address, uint16_t value) {
  Wire.beginTransmission(address);
  Wire.write(0x12);
  Wire.write(value > 0xff ? 0x01 : 0x00);
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x13);
  Wire.write(value & 0xff);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  Serial.begin(115200);
  Wire.begin();

  // 1.8V DCDCの設定
  setDCDCVoltage(DCDC18_ADDR, dcdc18_value);

  // 0.9V DCDCの設定
  setDCDCVoltage(DCDC09_ADDR, dcdc09_value);

  // INA228の設定
  while (!ina228_18.begin(0x40)) {
    Serial.println("INA228 1.8V not found!");
    delay(1000);
  }
  while (!ina228_09.begin(0x45)) {
    Serial.println("INA228 0.9V not found!");
    delay(1000);
  }

  ina228_18.setShunt(0.002, 10.0);
  ina228_18.setAveragingCount(INA228_COUNT_16);
  ina228_18.setVoltageConversionTime(INA228_TIME_150_us);

  ina228_09.setShunt(0.002, 10.0);
  ina228_09.setAveragingCount(INA228_COUNT_16);
  ina228_09.setVoltageConversionTime(INA228_TIME_150_us);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'H' || command == 'h') {
      digitalWrite(RESET_PIN, HIGH);
      Serial.println("HIGH");
    }
    if (command == 'L' || command == 'l') {
      digitalWrite(RESET_PIN, LOW);
      Serial.println("LOW");
    }
    if (command == 'R' || command == 'r') {
      digitalWrite(RESET_PIN, LOW);
      delay(200);
      digitalWrite(RESET_PIN, HIGH);
      Serial.println("RESET");
    }

    if (command == '1' && dcdc09_voltage > 0.81f) {
      dcdc09_value -= 1;
      dcdc09_voltage -= 0.005;
      setDCDCVoltage(DCDC09_ADDR, dcdc09_value);

      Serial.print("VCC: ");
      Serial.print(dcdc09_voltage, 3);
      Serial.println(" V");
    }

    if (command == '2' && dcdc09_voltage < 0.99f) {
      dcdc09_value += 1;
      dcdc09_voltage += 0.005;
      setDCDCVoltage(DCDC09_ADDR, dcdc09_value);

      Serial.print("VCC: ");
      Serial.print(dcdc09_voltage, 3);
      Serial.println(" V");
    }

    if (command == '3' && dcdc18_voltage > 1.62f) {
      dcdc18_value -= 1;
      dcdc18_voltage -= 0.005;
      setDCDCVoltage(DCDC18_ADDR, dcdc18_value);

      Serial.print("VCCIO: ");
      Serial.print(dcdc18_voltage, 3);
      Serial.println(" V");
    }

    if (command == '4' && dcdc18_voltage < 1.98f) {
      dcdc18_value += 1;
      dcdc18_voltage += 0.005;
      setDCDCVoltage(DCDC18_ADDR, dcdc18_value);

      Serial.print("VCCIO: ");
      Serial.print(dcdc18_voltage, 3);
      Serial.println(" V");
    }

    if (command == 'C' || command == 'c') {
      Serial.print("VDDIO :");
      Serial.print(ina228_18.getBusVoltage_V(), 3);
      Serial.print("Current: ");
      Serial.println(ina228_18.getCurrent_mA(), 3);
      Serial.print("VDD: ");
      Serial.print(ina228_09.getBusVoltage_V(), 3);
      Serial.print("Current: ");
      Serial.println(ina228_09.getCurrent_mA(), 3);
    }
  }

    if (command == 'M' || command == 'm') {
      Serial.println("MEASUREMENT START");
      digitalWrite(RESET_PIN, LOW);
      delay(200);
      digitalWrite(RESET_PIN, HIGH);

      Serial.println("Time,VCCIO,Current18,VCC,Current09");

      int s = millis();
      int t = 0;
      while (t < 500) {
        t = millis() - s;
        Serial.print(t);
        Serial.print(",");
        Serial.print(ina228_18.getBusVoltage_V(), 3);
        Serial.print(",");
        Serial.print(ina228_18.getCurrent_mA(), 3);
        Serial.print(",");
        Serial.print(ina228_09.getBusVoltage_V(), 3);
        Serial.print(",");
        Serial.print(ina228_09.getCurrent_mA(), 3);
        Serial.println();
      }
      Serial.println("MEASUREMENT END");
    }
  }
}
