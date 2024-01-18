#include <Arduino.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const char DEVICE_LOGIN_NAME[] = "Igrese Su Device Login Aqui";

const char SSID[] = "Ingrese el nombre de su red WIFI aqui";             // Network SSID (name)
const char PASS[] = "Ingrese la contraseña de su red WiFi Aqui";    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[] = "Ingrese la secret key aqui"; // Secret device password

LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t cm;
uint8_t mBr;

struct PlantaDeProcesos{

  //OUTPUT
  uint8_t botonOn;
  uint8_t BotonOff;
  uint8_t SalidaRele;
  uint8_t SensorNivel;

  //Valores de estanque
  uint8_t ValorMinimoEnEstanque;
  uint8_t ValorMaximoEnEstanque;
  uint8_t ResuladoMapeoNivel;
  uint8_t LecturaNivel;
  uint8_t SensorNivel;

  //Valores de Presion
  uint8_t SensorPresion;
  uint8_t ValorMinimoPresion;
  uint8_t ValorMaximoPresion;
  uint8_t LecturaPresion;
  uint8_t ResultadoMapeoPresion;
};

PlantaDeProcesos SensorPresion={
  .SensorPresion = 35,
  .ValorMinimoEnEstanque = 500,
  .ValorMaximoEnEstanque = 1200
};


PlantaDeProcesos SensorNivel = {
  .botonOn = 2,
  .BotonOff = 4,
  .SalidaRele = 15,
  .SensorNivel = 34,
  .ValorMinimoPresion = 760,
  .ValorMaximoPresion = 1840,


};

//Funciones creadas por arduino Cloud 
void SensadoNivel(PlantaDeProcesos &proceso);
void ImprimirLCD(PlantaDeProcesos &proceso);
void onMBrChange(PlantaDeProcesos &proceso);
void onCmChange(PlantaDeProcesos &proceso);
void initProperties();
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
//variables creadas por Aruidno IOT Cloud


void setup()
{
  pinMode(SensorNivel.botonOn,INPUT);
  pinMode(SensorNivel.BotonOff, INPUT);
  pinMode(SensorNivel.SalidaRele,OUTPUT);
  pinMode(SensorNivel.SensorNivel,OUTPUT);
  pinMode(SensorNivel.SensorPresion,OUTPUT);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
}
void loop()
{
  SensadoNivel(SensorNivel);
  ImprimirLCD(SensorNivel);
  onMBrChange(SensorPresion);
  onCmChange(SensorNivel);
  ArduinoCloud.update();
}


void initProperties()
{ 

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  //Quitar comentario cuando se encuentre en arduino Cloud IDE 
  // ArduinoCloud.addProperty(cm, READWRITE, ON_CHANGE, onCmChange);
  // ArduinoCloud.addProperty(mBr, READWRITE, ON_CHANGE, onMBrChange);
}
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);


void SensadoNivel(PlantaDeProcesos *proceso){

  proceso->SensorNivel = analogRead(proceso->SensorNivel);
  proceso->ResuladoMapeoNivel = map(proceso->LecturaNivel, proceso->ValorMinimoEnEstanque, proceso->ValorMaximoEnEstanque, 0, 35);
  proceso->LecturaPresion = analogRead(proceso->SensorPresion);
  proceso->ResultadoMapeoPresion = map(proceso->LecturaPresion, proceso->ValorMinimoPresion, proceso->ValorMaximoPresion, 0, 400);

}

void SensorPresion(PlantaDeProcesos *proceso){
  proceso->LecturaPresion = analogRead(proceso->SensorPresion);
  proceso->ResultadoMapeoPresion = map(proceso->LecturaPresion, proceso->ValorMinimoPresion, proceso->ValorMaximoPresion, 0, 400);
}
void ImprimirLCD(PlantaDeProcesos *proceso){
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Presion: ");
  lcd.print(proceso->ResultadoMapeoPresion);
  lcd.print("mBr");
  lcd.setCursor(0, 1);
  lcd.print("Nivel: ");
  lcd.print(proceso->ResuladoMapeoNivel);
  lcd.print(" cm");
  delay(1000);

}




void onMBrChange(PlantaDeProcesos *proceso)
{
  mBr = proceso->ResultadoMapeoPresion;
}


void onCmChange(PlantaDeProcesos *proceso)
{
  cm = proceso->ResuladoMapeoNivel;
}
