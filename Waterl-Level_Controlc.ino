#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
#define CANAL_DAC0 25

int ledVerde = 19;
int ledVermelho = 15;
int vEntrada = 2;
int vSaida = 4;
int trigPin = 18;
int echoPin = 35;
long duration;
float distanceCm;


#define SOUND_SPEED 0.034

void sensorDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = (18.4) - (duration * SOUND_SPEED / 2);
  Serial.print("Distancia (cm):");
  Serial.println(distanceCm);
}

void acelerometroMedi() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.println("");
  delay(1000);
}

void setup(void) {
  Serial.begin(115200);
  // Inisialisasi ledPin
  pinMode (ledVerde, OUTPUT);
  pinMode (ledVermelho, OUTPUT);
  pinMode (vEntrada, OUTPUT);
  pinMode(vSaida, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  while (!Serial)
    delay(10);
  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  int controle = 0;

  while (controle == 0) {
    int aux = 0;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    sensorDist();
    acelerometroMedi();

    if (a.acceleration.y >= 5.5) {
      Serial.println("PERIGO!!! NIVEL DE AGUA ABAIXO DO NORMAL");
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(vSaida, HIGH);
      digitalWrite(vEntrada, HIGH);
      Serial.println("AUMENTANDO VAZÃO DA AGUA....");
      dacWrite(CANAL_DAC0, 255);
      delay(2000);
    }
    sensor_t();

    if (a.acceleration.y <= -4.5) {
      Serial.println("AVISO : TANQUE ESTA EM UM LIMITE ACIMA DO PERMITIDO!!");
      Serial.println("DESLIGANDO VALVULA DE ENTRADA");
      dacWrite(CANAL_DAC0, 0);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(vSaida, HIGH);
      digitalWrite(vEntrada, LOW);
      delay(2000);
    }
    sensor_t();

    if (a.acceleration.y < 5.5 && a.acceleration.y > 1.6) {
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(vEntrada, HIGH);
      digitalWrite(vSaida, HIGH);
      Serial.println("enchendo");
      dacWrite(CANAL_DAC0, 255);
    }
    sensor_t();

    if (a.acceleration.y <= 1.6 && a.acceleration.y > -2.65) {
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledVermelho, LOW);
      digitalWrite(vSaida, HIGH);
      digitalWrite(vEntrada, HIGH);
      Serial.println("Enchendo");
      dacWrite(CANAL_DAC0, 255);
    }
    sensor_t();

    if (a.acceleration.y <= -2.65) {
      while (aux == 0) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        acelerometroMedi();
        sensorDist();
        if (a.acceleration.y >= -2.65) {
          digitalWrite(ledVermelho, LOW);
          digitalWrite(ledVerde, HIGH);
          digitalWrite(vSaida, HIGH);
          digitalWrite(vEntrada, LOW);
          Serial.println("desliga e acima de 12");
        } else {
          digitalWrite(ledVermelho, HIGH);
          digitalWrite(ledVerde, LOW);
          digitalWrite(vSaida, HIGH);
          digitalWrite(vEntrada, LOW);
          Serial.println("desliga e acima de 12");
        }
        if (a.acceleration.y <= -4.5) {
          Serial.println("AVISO : TANQUE ESTA EM UM LIMITE ACIMA DO PERMITIDO!!");
          Serial.println("DESLIGANDO VALVULA DE ENTRADA");
          dacWrite(CANAL_DAC0, 0);
          digitalWrite(ledVerde, LOW);
          digitalWrite(ledVermelho, HIGH);
          digitalWrite(vSaida, HIGH);
          digitalWrite(vEntrada, LOW);
        }
        if (a.acceleration.y >= 1.6) {
          aux = 1;
        }
        dacWrite(CANAL_DAC0, 0);
      }
    }
    if (distanceCm >= 15.5) {
      controle = 1;
    }
  }

  while (controle == 1) {
    int aux2 = 0;
    sensorDist();
    acelerometroMedi();

    if (distanceCm < 3) {
      Serial.println("PERIGO!!! NIVEL DE AGUA ABAIXO DO NORMAL");
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(vSaida, LOW);
      digitalWrite(vEntrada, HIGH);
      Serial.println("AUMENTANDO VAZÃO DA AGUA....");
      dacWrite(CANAL_DAC0, 255);
    } else if (distanceCm >= 3 && distanceCm < 8) {
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(vEntrada, HIGH);
      digitalWrite(vSaida, HIGH);
      Serial.println("enchendo");
      dacWrite(CANAL_DAC0, 255);
    }
    if (distanceCm >= 8 && distanceCm < 12) {
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledVermelho, LOW);
      digitalWrite(vEntrada, HIGH);
      digitalWrite(vSaida, HIGH);
      Serial.println("enchendo");
      dacWrite(CANAL_DAC0, 255);
    }

    if (distanceCm >= 12) {
      while (aux2 == 0) {
        sensorDist() ;
        acelerometroMedi();
        dacWrite(CANAL_DAC0, 0);
        if (distanceCm <= 12) {
          digitalWrite(ledVermelho, LOW);
          digitalWrite(ledVerde, HIGH);
          digitalWrite(vSaida, HIGH);
          digitalWrite(vEntrada, LOW);
          Serial.println("desliga e acima de 12");
        } else {
          digitalWrite(ledVermelho, HIGH);
          digitalWrite(ledVerde, LOW);
          digitalWrite(vSaida, HIGH);
          digitalWrite(vEntrada, LOW);
          Serial.println("desliga e acima de 12");
        }
        if (distanceCm >= 15) {
          Serial.println("AVISO : TANQUE ESTA EM UM LIMITE ACIMA DO PERMITIDO!!");
          Serial.println("DESLIGANDO VALVULA DE ENTRADA");
          dacWrite(CANAL_DAC0, 0);
          digitalWrite(ledVerde, LOW);
          digitalWrite(ledVermelho, HIGH);
          digitalWrite(vSaida, HIGH);
          digitalWrite(vEntrada, LOW);
          delay(2000);
        }
        if (distanceCm <= 8) {
          aux2 = 1;
        }
      }
    }
  }
}
