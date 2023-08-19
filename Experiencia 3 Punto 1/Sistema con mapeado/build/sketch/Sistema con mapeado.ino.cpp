#include <Arduino.h>
#line 1 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//iniciamos el panel y le asignamos la sigla lcd para llamar a las funciones de la libreria
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Entradas digitales
#define Boton1 2
#define Boton2 4

//salida digital
#define Led 15

//entrada analogica
#define SensorNivel 34
#define SensorPresion 35

int LecturaSensorNivel, LecturaSensorPresion;



int EstadoBoton1 = 0;
int EstadoBoton2 = 0;
int EstadoSumado = 0;

#line 27 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
void setup();
#line 62 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
void core0Task(void *parameter);
#line 98 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
void core1Task(void *parameter);
#line 124 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
void loop();
#line 27 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
void setup() {
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
void core0Task(void *parameter) {
  while (true) {
    // Leer el estado del botón 1
    EstadoBoton1 = digitalRead(Boton1);
    // Leer el estado del botón 2
    EstadoBoton2 = digitalRead(Boton2);
    LecturaSensorNivel = analogRead(SensorNivel);
    LecturaSensorPresion = analogRead(SensorPresion);
    int MapeoNivel = map(LecturaSensorNivel, 760, 1840, 0, 35);
    int MapeoPresion = map(LecturaSensorPresion, 580, 1200, 0, 400);
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(": ");
    lcd.print(MapeoPresion);
    lcd.print("mBr");


    lcd.setCursor(0, 1);
    lcd.print("Nivel: ");
    lcd.print(MapeoNivel);
    lcd.print(" cm");

    delay(25);








  }
}

// Declaración de la tarea del Core 1
void core1Task(void *parameter) {
  while (true) {
    // No es necesario modificar el código en el Core 1
    if (EstadoBoton1 == HIGH) {
      EstadoSumado = EstadoSumado + 1;
      EstadoSumado = EstadoSumado - 1;

      Serial.println(EstadoSumado);

      digitalWrite(Led, HIGH);  // Enciende el LED
      Serial.println("Prendido");
      delay(2500);
    }
    if (EstadoBoton2 == HIGH) {
      EstadoSumado = EstadoSumado - 1;
      digitalWrite(Led, LOW);   // Apaga el LED
      Serial.println(EstadoSumado);
      Serial.println("Apagado");
      delay(2500);
    }

  }
}



void loop() {
  // Leer el estado del botón 1
  EstadoBoton1 = digitalRead(Boton1);

  if (EstadoBoton1 == HIGH) {
    EstadoSumado = EstadoSumado + 1;
    Serial.println(EstadoSumado);

    digitalWrite(Led, HIGH);  // Enciende el LED
    Serial.println("Prendido");
    delay(250);
  }

  // Leer el estado del botón 2
  EstadoBoton2 = digitalRead(Boton2);

  if (EstadoBoton2 == HIGH) {
    digitalWrite(Led, LOW);   // Apaga el LED
    Serial.println("Apagado");
    delay(250);
  }
}

