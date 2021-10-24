//Libs
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_SSD1306.h>
#define OLED_Address 0x3C
Adafruit_SSD1306 oled(128, 64);
//Entradas
const int mais = 2;
const int menos = 3;
const int play = 4;
const int termistor = A0;
//Saidas
const int bomba = 6;
const int aquecedor = 8;
const int buzzer = 10;
const int led = 13;
//Variaveis
int tempo, tempoSalvo, tempSalvoLast, tempoAtualLast;
float tempSetado, vO, logR2, t, tc, r2, tempLast,  tempAtualLast, tempAgua = 20.00;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07, r1 = 10000;
unsigned char sB, sA, sP;
void setup() {
  Serial.begin(9600);
  pinMode(mais, INPUT);
  pinMode(menos, INPUT);
  pinMode(play, INPUT);
  pinMode(termistor, INPUT);
  pinMode(bomba, OUTPUT);
  pinMode(aquecedor, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
}
void loop() {
  tempSetado = readTemp(termistor);
  oledTemp(tempSetado);
  oledTempoSalvo(tempoSalvo);
  //Incrementa
  if (digitalRead(mais) == LOW) {
    tempo = tempo + 1;
    tempoSalvo = tempo;
    delay(200);
    Serial.println(tempo);
  }
  //Decrementa
  if (digitalRead(menos) == LOW && (tempo >= 1)) {
    tempo = tempo - 1;
    tempoSalvo = tempo;
    Serial.println(tempo);
    delay(200);
    if (tempo <= 1) {
      Serial.println("O tempo não pode ser menor que um!");
    }
  }
  oledTempAtual(tempAgua);
  //Aciona
  if (digitalRead(play) == LOW && tempo > 0) {
    Serial.println("");
    Serial.println("-------- Iniciado! ---------");
    do {
      delay(1000);
      digitalWrite(bomba, HIGH);
      sB = "Ligado";
      Serial.print("Bomba:");
      Serial.println(sB);
      oledTempAtual(tempAgua);
      tempo--;
      if (tempAgua >= tempSetado) {
        digitalWrite(aquecedor, LOW);
        sA = "Desligado - temperatura atingida";
        Serial.print("Aquecedor:");
        Serial.println(sA);
      } else {
        tempAgua++;
        digitalWrite(aquecedor, HIGH);
        sA = "Ligado";
        Serial.print("Aquecedor:");
        Serial.println(sA);
      }
      Serial.print(tempo);
      Serial.println(" segundos");
      Serial.print(tempAgua);
      Serial.println(" C");
      //Pausa
      if (digitalRead(play) == LOW) {
        sP = "Encerrado pelo usuario";
        Serial.print("Status Play/Pause:");
        Serial.print(sP);
        Serial.println("Encerrado pelo usuario!");
        digitalWrite(bomba, LOW);
        digitalWrite(aquecedor, LOW);
        sB = "Desligado";
        sA = "Desligado";
        Serial.print("Bomba:");
        Serial.println(sB);
        Serial.print("Aquecedor:");
        Serial.println(sA);
        tempo = 0;
      }
      oledTempoAtual(tempo);
    } while (tempo > 0 && digitalRead(play) == HIGH);
    oledTempoAtual(tempo);
    Serial.println("Sistema encerrado!");
    sP = "Encerrado";
    Serial.print(sP);
    digitalWrite(bomba, LOW);
    digitalWrite(aquecedor, LOW);
    if (tempo == 0) {
      digitalWrite(led, HIGH);
      //digitalWrite(buzzer, HIGH);
    }
  }
}
float readTemp(int termistor) {
  vO = analogRead(termistor);
  r2 = r1 * (1023.0 / (float)vO - 1.0);
  logR2 = log(r2);
  t = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  tc = t - 273.15;
  return tc;
}
void oledTemp(float temp) {
  oled.setCursor(0, 0);
  oled.setTextColor(BLACK);
  oled.print("Temp escolhida:");
  oled.print(tempLast);
  oled.setCursor(0, 0);
  oled.setTextColor(WHITE);
  oled.print("Temp escolhida:");
  oled.print(temp);
  oled.print("C");
  tempLast = temp;
  oled.display();
}
void oledTempAtual(float tempAtual) {
  oled.setCursor(0, 20);
  oled.setTextColor(BLACK);
  oled.print("Temp Atual:");
  oled.print(tempAtualLast);
  oled.setCursor(0, 20);
  oled.setTextColor(WHITE);
  oled.print("Temp Atual:");
  oled.print(tempAtual);
  oled.print("C");
  tempAtualLast = tempAtual;
  oled.display();
}
void oledTempoSalvo(int tempSalvo) {
  oled.setCursor(0, 35);
  oled.setTextColor(BLACK);
  oled.print("Tempo escolhido: ");
  oled.print(tempSalvoLast);
  oled.setCursor(0, 35);
  oled.setTextColor(WHITE);
  oled.print("Tempo escolhido: ");
  oled.print(tempSalvo);
  oled.print(" s");
  tempSalvoLast = tempSalvo;
  oled.display();
}
void oledTempoAtual(int tempoAtual) {
  oled.setCursor(0, 50);
  oled.setTextColor(BLACK);
  oled.print("Tempo Atual: ");
  oled.print(tempoAtualLast);
  oled.setCursor(0, 50);
  oled.setTextColor(WHITE);
  oled.print("Tempo atual: ");
  oled.print(tempoAtual);
  oled.print("s");
  tempoAtualLast = tempoAtual;
  oled.display();
}
