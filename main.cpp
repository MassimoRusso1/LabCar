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
}

void loop() {
  // Example: Turn on output 1 (Light) for 5 seconds
  turnOnLight(0x01, 5000); // Output ID 0x01 for 5000 milliseconds (5 seconds)

  // Add more sequences as needed
}

void turnOnLight(uint8_t outputID, unsigned long duration) {
  // Turn on the light
  sendCANMessage(outputID, true);
  delay(duration);

  // Turn off the light
  sendCANMessage(outputID, false);
}

void sendCANMessage(uint8_t outputID, bool state) {
  canMsg.can_id  = 0x123;          // Replace with appropriate CAN ID for the IX3212 PDM
  canMsg.can_dlc = 2;              // Data length
  canMsg.data[0] = outputID;       // Output ID
  canMsg.data[1] = state ? 0xFF : 0x00;  // State: 0xFF for ON, 0x00 for OFF

  mcp2515.sendMessage(&canMsg);
}
