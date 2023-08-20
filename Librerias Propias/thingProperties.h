//#include "arduino_secrets.h"
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "643411c7-7ef6-4e93-b1aa-f3cf381c469c";

const char SSID[]               = "SECRET_SSID";    // Network SSID (name)
const char PASS[]               = "SECRET_OPTIONAL_PASS";    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = "SECRET_DEVICE_KEY";    // Secret device password

void onCmChange();
void onMBrChange();

int cm;
int mBr;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(cm, READWRITE, ON_CHANGE, onCmChange);
  ArduinoCloud.addProperty(mBr, READWRITE, ON_CHANGE, onMBrChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);


