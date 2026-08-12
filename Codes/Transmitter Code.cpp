#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

uint8_t receiverMAC[] = {0x84, 0xF3, 0xEB, 0xXX, 0xXX, 0xXX}; // replace

char command;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Wire.begin(D2, D1);

  mpu.initialize();

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW failed");
    return;
  }

  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  if (ay > 8000) command = 'F';
  else if (ay < -8000) command = 'B';
  else if (ax > 8000) command = 'R';
  else if (ax < -8000) command = 'L';
  else command = 'S';

  esp_now_send(receiverMAC, (uint8_t *)&command, sizeof(command));

  Serial.println(command);

  delay(200);
}