/*En esta práctica simularemos un puente H
empregando relés STDP
Data 28/02/2026
Autor:Celso Otal Sanles*/

//Declaramos las variables globales

// DEFINICIÓN DE PINES
const int BOTON = 7;          // Pin del pulsador
const int RELE1 = 13;         // Relé 1 del puente H
const int RELE2 = 12;        // Relé 2 del puente H
const int RELE3 = 11;         // Relé 3 del puente H
const int RELE4 = 10;         // Relé 4 del puente H

// VARIABLES GLOBALES
int estadoActual = 0;         // Estado actual del sistema
int estadoBotonAnterior = HIGH;  // Estado anterior del botón
unsigned long tiempoInicio = 0;  // Tiempo de inicio del estado actual
const unsigned long TIMEOUT = 15000; // 15 segundos en milisegundos
void setup() {
  // Configurar pines de entrada
  pinMode(BOTON, INPUT_PULLUP);  // Pulsador con resistencia pull-up interna
  
  // Configurar pines de salida
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);
  pinMode(RELE3, OUTPUT);
  pinMode(RELE4, OUTPUT);
  
  
  // Inicializar todos los relés apagados (freno por inercia)
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, LOW);
  digitalWrite(RELE3, LOW);
  digitalWrite(RELE4, LOW);
  
  // Iniciar comunicación serial para debug
  Serial.begin(9600);
  Serial.println("Sistema iniciado - Freno por inercia");
}
void loop() {
  // Leer estado del botón
  int estadoBoton = digitalRead(BOTON);
  
  // Detectar cuando se presiona el botón (flanco descendente)
  if (estadoBoton == LOW && estadoBotonAnterior == HIGH) {
    // Cambiar al siguiente estado
    estadoActual++;
    if (estadoActual > 3) {
      estadoActual = 0;
    }
    
    // Reiniciar el temporizador
    tiempoInicio = millis();
    
    // Aplicar el nuevo estado
    aplicarEstado(estadoActual);
    
    // Esperar un poco para evitar rebotes
    delay(200);
  }
  
  // Guardar estado del botón para la próxima iteración
  estadoBotonAnterior = estadoBoton;
  
  // Verificar timeout de 15 segundos
  if (estadoActual != 4) {  // Si no está en freno por inercia
    unsigned long tiempoActual = millis();
    if (tiempoActual - tiempoInicio >= TIMEOUT) {
      // Cambiar a freno por inercia
      estadoActual = 4;
      aplicarEstado(estadoActual);
      Serial.println("TIMEOUT - Freno por inercia");
    }
  }
}
void aplicarEstado(int estado) {
  switch(estado) {
    case 0:  // GIRO DERECHA
      // Configuración puente H para giro derecha
      digitalWrite(RELE1, HIGH);
      digitalWrite(RELE2, LOW);
      digitalWrite(RELE3, LOW);
      digitalWrite(RELE4, HIGH);
      
      Serial.println("Estado: GIRO DERECHA");
      break;
      
    case 1:  // FRENO INMEDIATO (después de giro derecha)
      // Cortocircuitar motor para freno rápido
      digitalWrite(RELE1, HIGH);
      digitalWrite(RELE2, HIGH);
      digitalWrite(RELE3, LOW);
      digitalWrite(RELE4, LOW);
      
      Serial.println("Estado: FRENO INMEDIATO");
      break;
      
    case 2:  // GIRO IZQUIERDA
      // Configuración puente H para giro izquierda
      digitalWrite(RELE1, LOW);
      digitalWrite(RELE2, HIGH);
      digitalWrite(RELE3, HIGH);
      digitalWrite(RELE4, LOW);
      
      Serial.println("Estado: GIRO IZQUIERDA");
      break;
      
    case 3:  // FRENO INMEDIATO (después de giro izquierda)
      // Cortocircuitar motor para freno rápido
      digitalWrite(RELE1, LOW);
      digitalWrite(RELE2, LOW);
      digitalWrite(RELE3, HIGH);
      digitalWrite(RELE4, HIGH);
      
      Serial.println("Estado: FRENO INMEDIATO");
      break;
      
    case 4:  // FRENO POR INERCIA (todos los relés apagados)
      digitalWrite(RELE1, LOW);
      digitalWrite(RELE2, LOW);
      digitalWrite(RELE3, LOW);
      digitalWrite(RELE4, LOW);
      
      Serial.println("Estado: FRENO POR INERCIA");
      break;
  }
}

