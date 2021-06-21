#include <LedControl.h>
char bluetoothData = NULL;  //HC-05'den gelen açma kapama verisini almak için oluşturulan değişken
int sensorPin = A0; //Toprak nem ölçer sensörünün pini
int suseviyePin = A1; // Su seviye ölçerinin pini
bool isOpen = true; //ON OFF butonları ile sistemi açma kapama için kullanılan değişken
float suseviyeValue; //Su seviyesini nem ölçerden gelen değere göre 0-1023 arasında tutan değişken
float sensorValue;//Toprak nem seviyesini nem ölçerden gelen değere göre 0-1023 arasında tutan değişken
int kirmiziPin = 3; //RGB Kırmızı Ayak
int yesilPin = 9; //RGB Yeşil Ayak
int maviPin = 11; //RGB Mavi Ayak

enum Face {Smile, Sad, Neutral}; //8x8 Led için oluşturulmuş enum
Face previousFace; //Önceki yüz ifadesini tutup yazma fonksiyonunun gereksiz yere çalıştırılmasını engelleyen değişken
int DIN = 7; //8x8 Led DIN bacağı pini
int CS = 6; //8x8 Led CS bacağı pini
int CLK = 5;//8x8 Led CLK bacağı pini

byte smile[8] = { //Gülen yüz ifadesini oluşturmak için kullanılan array
  0x3C,
  0x42,
  0xA5,
  0x81,
  0xA5,
  0x99,
  0x42,
  0x3C
};
byte neutral[8] = { //Nötr yüz ifadesini oluşturmak için kullanılan array
  0x3C,
  0x42,
  0xA5,
  0x81,
  0xBD,
  0x81,
  0x42,
  0x3C
};
byte sad[8] = { //Mutsuz yüz ifadesini oluşturmak için kullanılan array
  0x3C,
  0x42,
  0xA5,
  0x81,
  0x99,
  0xA5,
  0x42,
  0x3C
};
LedControl lc = LedControl(DIN, CLK, CS, 0);
void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 0.5); //Led aydınlık seviyesi
  lc.clearDisplay(0);
  Serial.begin(9600);
  pinMode(10, OUTPUT);
}

void loop() {

  if (Serial.available()) {
    bluetoothData = Serial.read(); //Mobil uygulama ve processing üzerinden alınan değerin değişkene yazılması
  }
  if (bluetoothData == '3') { //Mobil uygulama ve Processing üzerindeki ON tuşunun yolladığı data
    isOpen = true;
  }
  else if (bluetoothData == '4') { //Mobil uygulama ve Processing üzerindeki OFF tuşunun yolladığı data
    isOpen = false;
    digitalWrite(10, LOW);
  }

  if (isOpen) {

    float kuruNem = 1023; //Toprak nem sensöründen alınan maksimum değer - Kuru değer
    float islakNem = 200; //Toprak nem sensöründen alınan minimum değer - Islak değer
    sensorValue = analogRead(sensorPin);
    delay(100);
    suseviyeValue = analogRead(suseviyePin);
    Serial.print((kuruNem - suseviyeValue) / (kuruNem - islakNem) * 100); //Su seviyesinin yüzdelik oranının bulunması için kullanılan denklem
    Serial.print("%");
    Serial.print("/"); //MIT App Inventor üzerinde ekranların bölünmüş gözükmesi için split fonksiyonundan ayırılıcak karakter
    Serial.print((kuruNem - sensorValue) / (kuruNem - islakNem) * 100); //Toprak nem oranının yüzdelik oranının bulunması için kullanılan denklem
    Serial.println("%");
    if (suseviyeValue > 980) { //Su seviyesi doluluk oranı hata payı ile birlikte
      rgb('r'); //Kırmızı ışık yakıcak fonksiyon çağırılıyor
    }
    else if (suseviyeValue <= 980) {
      rgb('g');//Yeşil ışık yakıcak fonksiyon çağırılıyor
    }
    if (sensorValue >= 600 || bluetoothData == '1') { //Pompanın çalışmasını sağlayan ve yüz ifadesinin mutsuz olduğu kısım
      printFace(Sad);
      digitalWrite(10, HIGH);
    } else if ((sensorValue < 600 && sensorValue > 300)) { //Pompanın durmasını sağlayan ve yüz ifadesinin Nötr olmasını sağlayacak kısım
      printFace(Neutral);
      digitalWrite(10, LOW);
    } else if (sensorValue < 300) { //Bitkinin mutlu olduğunu gösteren kısım
      printFace(Smile);
      digitalWrite(10, LOW);
    } else if (bluetoothData == '0') { //Manuel kapatma için kullanılan kısım
      digitalWrite(10, LOW);
    }

    delay(1000); //1 saniyelik döngü
  }
}

void printByte(byte character[]) { //8x8 Led üzerindeki ışıkların yanmasını sağlayan fonksiyon
  int i = 0;
  for (i = 0; i < 8; i++) {
    lc.setRow(0, i, character[i]);
  }
}
void printFace(Face face) { //Önceki yüz ifadesi ile güncel yüz ifadesini kıyaslayıp tekrardan ledlerin yanmasını engelleyen fonksiyon
  if (face == Smile && previousFace != Smile) {
    printByte(smile);
    previousFace = Smile;
  }
  if (face == Neutral && previousFace != Neutral) {
    printByte(neutral);
    previousFace = Neutral;
  }
  if (face == Sad && previousFace != Sad) {
    printByte(sad);
    previousFace = Sad;
  }

}
void rgb(char secim) { //RGB ile doluluk oranını gösteren fonksiyon
  if (secim == 'r') {
    analogWrite(kirmiziPin, 255);
    analogWrite(yesilPin, 0);
    analogWrite(maviPin, 0);
  }
  else if (secim == 'g') {
    analogWrite(kirmiziPin, 0);
    analogWrite(yesilPin, 255);
    analogWrite(maviPin, 0);
  }
}
