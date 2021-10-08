#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht11(DHTPIN, DHTTYPE);

// Isi sesuai pada Firebase
#define FIREBASE_HOST "monitoringsuhudht11-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "z7O93C6w6IYi4DHuIiuPwbauLJj1H5noAU4kmPJa"

// Nama Wifi
#define WIFI_SSID "V2040"
#define WIFI_PASSWORD "regzaa24"

FirebaseData firebaseData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  dht11.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while(WiFi.status()!= WL_CONNECTED){
    Serial.print(".");
    delay(400);
  }
  Serial.println();
  Serial.print("Connected with IP : ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // put your main code here, to run repeatedly:
  float t = dht11.readTemperature();
  float h = dht11.readHumidity();

  if (isnan(t) || isnan(h)){
    Serial.println("Gagal membaca sensor DHT11");
    delay(1000);
    return;
  }

  Serial.print("Suhu : ");
  Serial.print(t);
  Serial.println();
  Serial.print("Kelembaban : ");
  Serial.print(h);
  Serial.println();

  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/suhu", t)){
    Serial.println("Suhu terkirim");
  }else{
    Serial.println("Suhu tidak terkirim");
    Serial.println("Karena : "+firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/kelembaban", h)){
    Serial.println("Kelembaban terkirim");
  }else{
    Serial.println("Kelembaban tidak terkirim");
    Serial.println("Karena : "+firebaseData.errorReason());
  }

  delay(1000);
}
