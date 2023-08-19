# 1 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
# 2 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino" 2
# 3 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino" 2


//iniciamos el panel y le asignamos la sigla lcd para llamar a las funciones de la libreria
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Entradas digitales



//salida digital


//entrada analogica



int LecturaSensorNivel, LecturaSensorPresion;



int EstadoBoton1 = 0;
int EstadoBoton2 = 0;
int EstadoSumado = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(15, 0x03);
  pinMode(2, 0x01);
  pinMode(4, 0x01);
  pinMode(34, 0x01);
  pinMode(35, 0x01);
  lcd.init();
  lcd.backlight();

  // Crear la tarea del Core 0
  xTaskCreatePinnedToCore(
    core0Task, // Función de la tarea
    "Core 0 Task", // Nombre de la tarea
    10000, // Tamaño de la pila
    
# 43 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino" 3 4
   __null
# 43 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
       , // Parámetros de la tarea
    1, // Prioridad
    
# 45 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino" 3 4
   __null
# 45 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
       , // Manejador de la tarea
    0 // Número del core (0 para Core 0)
  );

  // Crear la tarea del Core 1
  xTaskCreatePinnedToCore(
    core1Task, // Función de la tarea
    "Core 1 Task", // Nombre de la tarea
    10000, // Tamaño de la pila
    
# 54 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino" 3 4
   __null
# 54 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
       , // Parámetros de la tarea
    1, // Prioridad
    
# 56 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino" 3 4
   __null
# 56 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
       , // Manejador de la tarea
    1 // Número del core (1 para Core 1)
  );
}

// Declaración de la tarea del Core 0
void core0Task(void *parameter) {
  while (true) {
    // Leer el estado del botón 1
    EstadoBoton1 = digitalRead(2);
    // Leer el estado del botón 2
    EstadoBoton2 = digitalRead(4);
    LecturaSensorNivel = analogRead(34);
    LecturaSensorPresion = analogRead(35);
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
# 94 "E:\\GitHub\\Sistema con mapeado\\Sistema con mapeado.ino"
  }
}

// Declaración de la tarea del Core 1
void core1Task(void *parameter) {
  while (true) {
    // No es necesario modificar el código en el Core 1
    if (EstadoBoton1 == 0x1) {
      EstadoSumado = EstadoSumado + 1;
      EstadoSumado = EstadoSumado - 1;

      Serial.println(EstadoSumado);

      digitalWrite(15, 0x1); // Enciende el LED
      Serial.println("Prendido");
      delay(2500);
    }
    if (EstadoBoton2 == 0x1) {
      EstadoSumado = EstadoSumado - 1;
      digitalWrite(15, 0x0); // Apaga el LED
      Serial.println(EstadoSumado);
      Serial.println("Apagado");
      delay(2500);
    }

  }
}



void loop() {
  // Leer el estado del botón 1
  EstadoBoton1 = digitalRead(2);

  if (EstadoBoton1 == 0x1) {
    EstadoSumado = EstadoSumado + 1;
    Serial.println(EstadoSumado);

    digitalWrite(15, 0x1); // Enciende el LED
    Serial.println("Prendido");
    delay(250);
  }

  // Leer el estado del botón 2
  EstadoBoton2 = digitalRead(4);

  if (EstadoBoton2 == 0x1) {
    digitalWrite(15, 0x0); // Apaga el LED
    Serial.println("Apagado");
    delay(250);
  }
}
