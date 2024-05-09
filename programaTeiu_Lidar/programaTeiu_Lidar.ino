SET_LOOP_TASK_STACK_SIZE(16 * 1024);

#include "LidarX2.h"
#include <HardwareSerial.h>
#include <BluetoothSerial.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <SD.h>
#include <array>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
BluetoothSerial SerialBT;

#define SENSOR_PIN1 14
#define SENSOR_PIN2 27
#define RXD2 16
#define TXD2 17
#define MOTOR_PIN 26
#define SERVO_PIN 13
byte command;

int motor1Pin1A = 33;
int motor1Pin2A = 32;
int enablePinA = 25;
int motor1Pin1B = 15;
int motor1Pin2B = 4;
int enablePinB = 12;
const int PINO_USUARIO = 2;
File myFile;
Servo servoMotor;

const int CS = 5;

LidarX2 lidar(Serial2);
int measureCount = 0;
int measureCapacity = 100;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to and enable it
#endif

void inicializa() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(PINO_USUARIO, HIGH);
    delay(1000);
    digitalWrite(PINO_USUARIO, LOW);
    delay(1000);
  }
}

void executa() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(PINO_USUARIO, HIGH);
    delay(1000);
    digitalWrite(PINO_USUARIO, LOW);
    delay(1000);
  }
}

void WriteFile(const char * path, const char * message) {
  myFile = SD.open(path, FILE_APPEND);
  if (myFile) {

    myFile.println(message);
    myFile.close();

  }
  else {
  }
}

void ReadFile(const char * path) {
  myFile  = SD.open(path);
  if (myFile) {

    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  }
  else {

  }
}

void setup() {

  servoMotor.attach(SERVO_PIN);
  
  File myFile;

  Serial.begin(57600);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
  analogWrite(MOTOR_PIN, 0);
  if (!lidar.open()) {
    Serial.println("Cannot open lidar");
    while (1); // Halt the program
  }
  pinMode(motor1Pin1A, OUTPUT);
  pinMode(motor1Pin2A, OUTPUT);
  ledcSetup(0, 500, 8);
  ledcAttachPin(enablePinA, 0);

  //pinMode(enablePinA,OUTPUT);
  pinMode(motor1Pin1B, OUTPUT);
  pinMode(motor1Pin2B, OUTPUT);
  ledcSetup(1, 500, 8);
  ledcAttachPin(enablePinB, 1);

  //pinMode(enablePinB,OUTPUT);
  pinMode(SENSOR_PIN1, INPUT);
  pinMode(SENSOR_PIN2, INPUT);

  pinMode(PINO_USUARIO, OUTPUT);

  SerialBT.begin("ESP32");
  SD.begin(CS);
  inicializa();
  delay(15000);
  executa();
}

void serialFlush() {
  while (Serial2.available() > 0) {
    char t = Serial2.read();
  }
}

void para() {
  ledcWrite(0, 127);
  //analogWrite(enablePinA, 127);
  digitalWrite(motor1Pin1A, LOW);
  digitalWrite(motor1Pin2A, LOW);
  ledcWrite(1, 127);
  //analogWrite(enablePinB, 127);
  digitalWrite(motor1Pin1B, LOW);
  digitalWrite(motor1Pin2B, LOW);
}

void frente() {
  ledcWrite(0, 127);
  //analogWrite(enablePinA, 127);
  digitalWrite(motor1Pin1A, LOW);
  digitalWrite(motor1Pin2A, HIGH);
  ledcWrite(1, 127);
  //analogWrite(enablePinB, 127;
  digitalWrite(motor1Pin1B, HIGH);
  digitalWrite(motor1Pin2B, LOW);
}

void tras() {
  ledcWrite(0, 127);
  //analogWrite(enablePinA, 127);
  digitalWrite(motor1Pin1A, HIGH);
  digitalWrite(motor1Pin2A, LOW);
  ledcWrite(1, 127);
  //analogWrite(enablePinB, 127);
  digitalWrite(motor1Pin1B, LOW);
  digitalWrite(motor1Pin2B, HIGH);
}

void direita() {
  ledcWrite(0, 255);
  //analogWrite(enablePinA, 255);
  digitalWrite(motor1Pin1A, LOW);
  digitalWrite(motor1Pin2A, HIGH);
  ledcWrite(1, 255);
  //analogWrite(enablePinB, 255);
  digitalWrite(motor1Pin1B, LOW);
  digitalWrite(motor1Pin2B, LOW);
}

void esquerda() {
  ledcWrite(0, 255);
  //analogWrite(enablePinA, 255);
  digitalWrite(motor1Pin1A, LOW);
  digitalWrite(motor1Pin2A, LOW);
  ledcWrite(1, 255);
  //analogWrite(enablePinB, 255);
  digitalWrite(motor1Pin1B, HIGH);
  digitalWrite(motor1Pin2B, LOW);
}

float** verificaLidar() {
  
  float** dados = new float*[4];
    for(int i = 0; i < 4; ++i) {
      dados[i] = new float[2];
    }
  //Inicializa o verificador do estado da matriz dados
  bool estd;
  //Inicializa, ou limpa, o array dados

  do {
  
    LidarMeasure* measures = lidar.readMeasures(measureCount, measureCapacity);

    if (measures) {
      for (int i = 0; i < sizeof(measures); ++i) {
        if (measures[i].angle != 0) {
          float angle = measures[i].angle;
          float distance = measures[i].distance;

          if ((angle >= 0 && angle < 45) || (angle >= 315 && angle < - 360)) {
            dados[0][0] = angle;
            dados[0][1] = distance;
          } else if (angle >= 45 && angle < 135) {
            dados[1][0] = angle;
            dados[1][1] = distance;
          } else if (angle >= 135 && angle < 225) {
            dados[2][0] = angle;
            dados[2][1] = distance;
          } else if (angle >= 225 && angle < 315) {
            dados[3][0] = angle;
            dados[3][1] = distance;
          }

          int dados_raw = measures[i].sumSilsiM;
          std::string s = std::to_string(dados_raw);
          const char *dados = s.c_str();
          WriteFile("/dados_map.txt", dados);
        }
      }
    }

    delete[] measures;
    estd = true;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 2; ++j) {
        if (dados[i][j] == 0) {
          estd = false;
        }
      }
    }
    
    delay(10);
  } while (!estd);
  
  return dados;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 2; ++j) {
      Serial.println(dados[i][j]);
    }
  }
}

int verificaIR() {

  int state1 = digitalRead(SENSOR_PIN1);
  int state2 = digitalRead(SENSOR_PIN2);

  if (state1 == HIGH) {
    return 1;
  }
  else if (state2 == HIGH) {
    return 2;
  }
  else if (state1 ==  LOW && state2 == LOW) {
    return 3;
  }
  else {
    return 4;
  }
}

void loop() {

  int estado;
  estado = verificaIR();

  while (estado == 3) {

    if (SerialBT.available()) {
      command = SerialBT.read();
      Serial.write(command);
      if (command == 'F') {
        frente();
        estado = verificaIR();

      }

      if (command == 'B') {
        tras();
        estado = verificaIR();

      }
      if (command == 'L') {
        esquerda();
        estado = verificaIR();
      }
      if (command == 'R') {
        direita();
        estado = verificaIR();

      }
      if (command == 'S') {
        para();
        estado = verificaIR();

      }
    } else {

      float** dados;

      do {
        frente();
        delay(200);
        estado = verificaIR();
        dados = verificaLidar();

      } while (dados[2][1] > 200);

      if (dados[2][1] < 200) {
        //Captura a distância que Teiu tem logo a sua frente
        float d_frente = dados[2][1];
        float d_direita = dados[3][1];
        float d_esquerda = dados[1][1];
        Serial.println("Parou");
        para();
        delay(200);
        Serial.println("Recua");
        tras();
        delay(200);
        if (d_esquerda < d_direita) {
          //Se a distância a esquerda for menor que a direita
          if (d_esquerda < 200) {
            //E se a distância à esquerda for menor que 200
            //Irá para a direita
            Serial.println("Foi para a direita");
            direita();
            delay(300);
            para();
            delay(200);
            estado = verificaIR();
          }else {
            esquerda();
            delay(300);
            estado = verificaIR();
          }

        } else if (d_direita < d_esquerda) {
          if (d_direita < 200) {
            Serial.println("Foi para a esquerda");
            esquerda();
            delay(300);
            para();
            delay(200);
            estado = verificaIR();
            
          } else {
            Serial.println("Foi para a direita");
            direita();
            delay(300);
          }
        } else if(d_direita <= 200 && d_frente <200){
          para();
          delay(300);
          tras();
          delay(200);
          esquerda();
          delay(300);
                     
        } else if(d_esquerda <= 200 && d_frente <=200) {
          para();
          delay(300);
          tras();
          delay(200);
          direita();
          delay(300);
        } 
        
     }else if((dados[1][1] < 200) && (dados[2][1] < 200) && (dados[3][1] < 200)) {
        do {
          tras();
          delay(200);
          dados = verificaLidar();
        } while((dados[1][1] < 200) && (dados[2][1] < 200) && (dados[3][1] < 200));

     }    
       delete [] dados;
     }
       
     }

  if (estado == 1) {

    para();
    delay(200);
    tras();
    delay(200);

  }

  else if (estado == 2) {

    para();
    delay(200);
    frente();
    delay(200);

  }

  else if (estado == 3) {
    para();
  }
  delay(100);

}
