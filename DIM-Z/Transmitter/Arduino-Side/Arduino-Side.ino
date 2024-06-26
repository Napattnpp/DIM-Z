#include "SystemFile.h"

unsigned long currentTime = 0;
unsigned long previousTime[2];

void setup() {
  Serial.begin(115200);

  compassModule.init();
  servoModule.init();

  myNrf.init();
  // gps.init();

  //! Initialize dht the last one !//
  mqx.init();
  dhtx.init();
}

void loop() {
  currentTime = millis();
  compassModule.init();

  //--------------------------------------------------------------------- Collect and Send data to datacenter ---------------------------------------------------------------------//
  if (currentTime - previousTime[0] >= 1000) {
    // Get temp, humidity, co, lpg and smoke value
    dhtx.get();
    mqx.get();

    // Get current location
    // gps.getCoordinate();

    // Send data to Datacenter
    myNrf.sendData(dhtx.temperature, dhtx.humidity, mqx.co, mqx.lpg, mqx.smoke, gps.latitude, gps.longitude);

    previousTime[0] = currentTime;
  }

  //-------------------------------------------------------------------------------- Rotate camera --------------------------------------------------------------------------------//
  servoModule.resolution360();

  //------------------------------------------------------------------------ Receive data from raspberry pi ------------------------------------------------------------------------//
  if (Serial.available()) {
    String data = Serial.readString();

    if (data == "@rp|ai$NOD;") {
      // No detect //
      log("(rp) No detect");
    } else if (data == "@rp|ai$FID;") {
      // Fire is detected //
      log("(rp) Fire is detected");
      int key = 0;
      onDetected(key);
    } else if (data == "@rp|ai$SID;") {
      // Smoke is detected //
      log("(rp) Smoke is detected");
      int key = 1;
      onDetected(key);
    }
  }
}