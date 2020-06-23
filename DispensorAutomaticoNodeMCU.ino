#include <NewPing.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MAX_DISTANCE 30

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int respvar = 0; //valor retornado do sensor de água
int resping = A5; //porta do sensor de água

int motor = 8; //porta do motor
int numero;

void setup()
{
  pinMode(motor, OUTPUT);
  Serial.begin(9600);
  digitalWrite(motor,LOW);
}
 
void loop() {
  int distancia = sonar.ping_cm();
  
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println("cm");
  delay(100);
  if (distancia >= 3 && distancia <= 10) //Objeto detectado
  {
    Serial.println("***** ATIVOU *****"); 
    digitalWrite(motor,HIGH); //liga o motor
    delay(4000);
    
    Serial.println("***** DESATIVOU ***");
    digitalWrite(motor,LOW); //desliga o motor
    delay(1500);
    
    respvar = analogRead(resping); //lê a saída analógica do sensor de água
    
    if(respvar <= 100){
      Serial.println("Nivel: Vazio");
    }
    else if(respvar>100 && respvar <= 300){
      Serial.println("Nivel: Baixo"); 
    }
    else if(respvar>300 && respvar<=330){
      Serial.println("Nivel: Medio");
    }
    else if(respvar>330){
      Serial.println("Nivel: Alto");
    }
  }
  delay(500); 
}
