#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10); // CS Pin

void setup() {
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ); // Set bitrate to 250 kbps
  mcp2515.setNormalMode();

  Serial.println("CAN Initialized");

  // Beispiel: Konfigurieren Sie den PWM-Modus für einen Ausgang
  configureOutput(0x01, 500); // Konfigurieren Sie Ausgang 1 mit 500 Hz PWM
}

void loop() {
  // Beispiel: Schalten Sie Ausgang 1 (Licht) für 5 Sekunden ein
  turnOnOutput(0x01, 5000); // Ausgang 1 für 5000 Millisekunden (5 Sekunden) einschalten

  // Weitere Sequenzen können hier hinzugefügt werden
}

void configureOutput(uint8_t outputID, uint16_t pwmFrequency) {
  // Beispiel für die Konfigurationsnachricht
  canMsg.can_id  = 0x1FF;          // Ersetzen Sie dies durch die entsprechende CAN-ID für die Konfiguration
  canMsg.can_dlc = 3;              // Datenlänge
  canMsg.data[0] = outputID;       // Ausgangs-ID
  canMsg.data[1] = pwmFrequency & 0xFF;       // Niedriges Byte der PWM-Frequenz
  canMsg.data[2] = (pwmFrequency >> 8) & 0xFF; // Hohes Byte der PWM-Frequenz

  mcp2515.sendMessage(&canMsg);
}

void turnOnOutput(uint8_t outputID, unsigned long duration) {
  // Ausgang einschalten
  sendCANMessage(outputID, true);
  delay(duration);

  // Ausgang ausschalten
  sendCANMessage(outputID, false);
}

void sendCANMessage(uint8_t outputID, bool state) {
  canMsg.can_id  = 0x123;          // Ersetzen Sie dies durch die entsprechende CAN-ID für den IX3212 PDM
  canMsg.can_dlc = 2;              // Datenlänge
  canMsg.data[0] = outputID;       // Ausgangs-ID
  canMsg.data[1] = state ? 0xFF : 0x00;  // Zustand: 0xFF für EIN, 0x00 für AUS

  mcp2515.sendMessage(&canMsg);
}
