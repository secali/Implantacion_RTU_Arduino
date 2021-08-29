#include <Servo.h>


/*
La idea que he tenido es un un sensor de gas para una nave industrial, donde puede haber acumulacion de gas 
de algun tipo. A traves del led se indica el nivel de gas, verde seria nada, azul nivel medio y rojo nivel 
de seguridad sobrepasado. Con nivel medio se abriria una tampilla(representado por el servo) o 
puerta/ventana 45º para ventilar, si se llega a nivel alto se abriria esa trampilla 90º y se activaria 
un ventilador para ayudar a desalojar el gas(estaria representado por el motor CC). Por ultimo hay un 
sensor de proximidad que seria un sistema de seguridad por si alguien se acerca a la zona "contaminada por el gas",
y dependiendo de su nivel sonara de una forma u otra.
*/


//SE HA INCLUIDO UN POTENCIOMETRO PARA SIMULAR LOS CAMBIOS DE NIVEL EN EL GAS
//PINES DEL SISTEMA: 
const byte PIN_LED_VERDE = 7;
const byte PIN_LED_AZUL = 6;
const byte PIN_LED_ROJO = 5;
const byte PIN_OUT_SGAS = A1;
const byte PIN_OUT_SERVO = A0;
const byte PIN_OUT_VENTILA = 4;
const byte PIN_SONAR_TRIG = 2;
const byte PIN_SONAR_ECHO = 3;
const byte PIN_BUZZER = 8;

Servo servo;


//NIVELES DE GAS (ARROJA VALORES ENTRE 300 Y 650 APROX)
const unsigned GAS_BAJO = 400;
const unsigned GAS_ALTO = 500;

//ROTACIONES DEL SERVO
const unsigned APERTURA_MEDIA = 45;
const unsigned APERTURA_COMPLETA = 90;
const unsigned CIERRE_SERVO = 0;

//CONSTANTES DE TIEMPO
const unsigned long REFRESCO = 250;
const unsigned long TIEMPO_ALARMA_ADVERTENCIA = 2000;
const unsigned long TIEMPO_ALARMA_PELIGRO = 500;

//CONTROL DE TIEMPO
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

//DISTANCIAS DE SONAR (EL SONAR DA DISTANCIAS ENTRE 2-400CM APROX)
const unsigned DISTANCIA_CERCA = 100;
const unsigned DISTANCIA_LEJOS = 250;

//TONO DE BUZZER
const unsigned TONO_ALARMA_ADVERTENCIA = 244;
const unsigned TONO_ALARMA_PELIGRO = 400;


//Cálculo para la distancia - FUNCION EXTRAIDA DE https://www.hwlibre.com/hc-sr04/#Pinout_y_datasheets
int obtenerDistanciaSonar(byte TriggerPin, byte EchoPin) {
   long duration, distanceCm;
    
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
    
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
    
   distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
   return distanceCm;
}


void setup()
{
 pinMode(PIN_LED_VERDE, OUTPUT);
 pinMode(PIN_LED_AZUL, OUTPUT);
 pinMode(PIN_LED_ROJO, OUTPUT);
 pinMode(PIN_OUT_SGAS, INPUT);
  
 servo.attach(PIN_OUT_SERVO);
  
 pinMode(PIN_OUT_VENTILA, OUTPUT);
  
 pinMode(PIN_SONAR_TRIG, OUTPUT);
 pinMode(PIN_SONAR_ECHO, INPUT);
  
 Serial.begin(9600);
 tiempo1 = millis();
}


void loop()
{
 tiempo2 = millis(); // control de tiempo sin usar delay: https://hetpro-store.com/TUTORIALES/arduino-millis/
  
  //DEBUG CONSOLA
  unsigned long lectGas = analogRead(PIN_OUT_SGAS);
  Serial.print("Lectura gas: ");
  Serial.println(lectGas);
  Serial.print("Lectura tiempo 1 y 2: ");
  Serial.print(tiempo1);
   Serial.print("   ");
  Serial.println(tiempo2);
  
  //CONTROL LEDS SEGUN NIVELES DE GAS ENTRE 0 Y 99
  //SE USA UN POTENCIOMETRO PARA SIMULAR LOS CAMBIOS DE NIVEL DE GAS
  digitalWrite(PIN_LED_VERDE, lectGas <= GAS_BAJO ? HIGH : LOW);
  digitalWrite(PIN_LED_ROJO, lectGas > GAS_ALTO ? HIGH : LOW);
  
  if(lectGas > GAS_BAJO && lectGas<=GAS_ALTO) {  
  	digitalWrite(PIN_LED_AZUL,LOW);     
    digitalWrite(PIN_LED_ROJO,HIGH);
    digitalWrite(PIN_LED_VERDE,HIGH);
  	
  }
  
  unsigned distancia_sonar = obtenerDistanciaSonar(PIN_SONAR_TRIG, PIN_SONAR_ECHO);
  Serial.print("Lectura distancia: ");
  Serial.println(distancia_sonar);
  
  //ABRIR TRAMPILLA SI HAY NIVELES DE GAS MEDIO
  if(lectGas > GAS_BAJO && lectGas<=GAS_ALTO){
  	servo.write(APERTURA_MEDIA);	//45º
    if(distancia_sonar<=DISTANCIA_CERCA){ 				//SI HAY UNA PERSONA PROXIMA 
      													//A LA FUENTE DE EMISION DE GAS (DISTANCIA CERCA)
        if(tiempo2>tiempo1+TIEMPO_ALARMA_ADVERTENCIA){	//SONAR ALARMA COMO ADVERTENCIA
          tiempo1 = millis();
          tone(PIN_BUZZER, TONO_ALARMA_ADVERTENCIA);
        } else noTone(PIN_BUZZER);
    }

  }else if (lectGas > GAS_ALTO){
    servo.write(APERTURA_COMPLETA);    //90º
    digitalWrite(PIN_OUT_VENTILA,HIGH); //ENCENDER VENTILACION
    
      if(distancia_sonar<=DISTANCIA_LEJOS){ 		//SI HAY UNA PERSONA EN LA SALA (DISTANCIA LEJOS)
        if(tiempo2>tiempo1+TIEMPO_ALARMA_PELIGRO){	//DEBIDO A LOS ALTOS NIVELES DE GAS, SONAR ALARMA DE PELIGRO!
          tiempo1 = millis();
          tone(PIN_BUZZER, TONO_ALARMA_PELIGRO);
        } else noTone(PIN_BUZZER);
    }
    
    
    if(distancia_sonar<=DISTANCIA_LEJOS){
      
    }
    
  }else {
    noTone(PIN_BUZZER);
    servo.write(CIERRE_SERVO); //CERRAR

  }
  delay(REFRESCO);
}