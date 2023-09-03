#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// iniciamos el panel y le asignamos la sigla lcd para llamar a las funciones de la libreria
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Entradas digitales
#define Boton1 2
#define Boton2 4

// salida digital
#define Led 15

// entrada analogica
#define SensorNivel 34
#define SensorPresion 35

float LecturaSensorNivel, LecturaSensorPresion;

int EstadoBoton1 = 0;
int EstadoBoton2 = 0;
int EstadoSumado = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(Led, OUTPUT);
  pinMode(Boton1, INPUT);
  pinMode(Boton2, INPUT);
  pinMode(SensorNivel, INPUT);
  pinMode(SensorPresion, INPUT);
  lcd.init();
  lcd.backlight();

  // Crear la tarea del Core 0
  xTaskCreatePinnedToCore(
      core0Task,     // Función de la tarea
      "Core 0 Task", // Nombre de la tarea
      10000,         // Tamaño de la pila
      NULL,          // Parámetros de la tarea
      1,             // Prioridad
      NULL,          // Manejador de la tarea
      0              // Número del core (0 para Core 0)
  );

  // Crear la tarea del Core 1
  xTaskCreatePinnedToCore(
      core1Task,     // Función de la tarea
      "Core 1 Task", // Nombre de la tarea
      10000,         // Tamaño de la pila
      NULL,          // Parámetros de la tarea
      1,             // Prioridad
      NULL,          // Manejador de la tarea
      1              // Número del core (1 para Core 1)
  );
}

// Declaración de la tarea del Core 0
void core0Task(void *parameter)
{
  while (true)
  {

    LecturaSensorNivel = (analogRead(SensorNivel));
    LecturaSensorNivel= LecturaSensorNivel - 45;
    LecturaSensorPresion = analogRead(SensorPresion);
    //float MapeoNivel = (1.74 * LecturaSensorNivel * 100.0) / 9216;
    float MapeoNivel = map(LecturaSensorNivel,520,2200,0,35);//520
    Serial.print(LecturaSensorNivel);
    // MapeoNivel = MapeoNivel-5;
    int MapeoPresion = map(LecturaSensorPresion, 700, 1238, 0, 400);
    MapeoPresion = (MapeoPresion / 2) - 20;
    MapeoPresion = MapeoPresion - 70;
    if (MapeoPresion < 50)
    {
      MapeoPresion = 0;
    }

    // if (MapeoNivel > 20)
    // {
    //   MapeoNivel = MapeoNivel + 6;
    // }
    // if (MapeoNivel < 10)
    // {
    //   MapeoNivel = MapeoNivel + 5;
    // }
    // if(MapeoNivel<5){
    //   MapeoNivel = MapeoNivel + 0;
    // }
    // if(MapeoNivel < 0 ){
    //   MapeoNivel = 0;
    // }
    // MapeoNivel = MapeoNivel + 8;

    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Presion: ");
    lcd.print(MapeoPresion);
    lcd.print("mBr");
    Serial.print("Presion: ");
    Serial.println(MapeoPresion);
    Serial.print("PresionA: ");
    Serial.println(LecturaSensorPresion);
    lcd.setCursor(0, 1);
    lcd.print("Nivel: ");
    lcd.print(MapeoNivel);
    lcd.print(" cm");
    Serial.print("Nivel: ");
    Serial.println(MapeoNivel);
    Serial.print("NivelA: ");
    Serial.println(LecturaSensorNivel);

    delay(1000);
  }
}

// Declaración de la tarea del Core 1
void core1Task(void *parameter)
{
  while (true)
  {
    // Leer el estado del botón 1
    EstadoBoton1 = digitalRead(Boton1);
    // Leer el estado del botón 2
    EstadoBoton2 = digitalRead(Boton2);
    // No es necesario modificar el código en el Core 1
    if (EstadoBoton1 == HIGH)
    {
      EstadoSumado = EstadoSumado + 1;
      EstadoSumado = EstadoSumado - 1;

      Serial.println(EstadoSumado);

      digitalWrite(Led, HIGH); // Enciende el LED
      Serial.println("Prendido");
      delay(500);
    }
    if (EstadoBoton2 == HIGH)
    {
      EstadoSumado = EstadoSumado - 1;
      digitalWrite(Led, LOW); // Apaga el LED
      Serial.println(EstadoSumado);
      Serial.println("Apagado");
      delay(500);
    }
  }
}

void loop()
{
}
