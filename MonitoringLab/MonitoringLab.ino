#include <ArduinoJson.h>


#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#include <CTBot.h>
CTBot myBot;

#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "oppodea";
const char *password = "dea101010";
const char *host = "http://192.168.43.137/Monitoring/kirim.php";
String token = "6595340801:AAHRMXlH_WCBfC0lQjUtuyw1of2HfKt13eI"; // token bot telegram yang telah dibuat



const int Mq2 = A0;
const int pinBuzzer = D6;

LiquidCrystal_I2C lcd(0x27, 16, 2);



void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  pinMode(Mq2, INPUT);
  pinMode(pinBuzzer, OUTPUT);

  dht.begin();

  // Initialize LCD display
  lcd.begin();
  lcd.backlight();

  // Membaca BootTelegram
  myBot.wifiConnect(ssid,password);
  myBot.setTelegramToken(token);

  if(myBot.testConnection()){
    Serial.println("Koneksi telegram berhasil");
  }
  else{

    Serial.println("Koneksi telegram gagal");
  }

}

void loop() {

  TBMessage msg;

   

  
  String temperature, humidity, postData, gas;

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int g = analogRead(Mq2);
  temperature = String(t);
  humidity = String(h);
  gas = String(g);

  // Print gas data on line 1, column 0
  lcd.setCursor(0, 0);
  lcd.print("ASAP : " + gas);

  // Print temperature data on line 2, column 0
  lcd.setCursor(0, 1);
  lcd.print("SUHU : " + temperature + " C");
  delay(1000);

   
    


  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, host);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "temperatur=" + temperature + "&kelembapan=" + humidity + "&gas=" + gas;
    int httpResponseCode = http.POST(httpRequestData);
  

    if (httpResponseCode == 200) {

    //  Serial.println("Kirim data Berhasil");

      if ((g > 150) || (t > 35)) {
        digitalWrite(pinBuzzer, HIGH);
        if(g > 150){
          if (CTBotMessageText == myBot.getNewMessage(msg)){
            Serial.print("Pesan Masuk: ");
            Serial.println(msg.text);
            myBot.sendMessage(msg.sender.id, "Gas atau asap terlalu tinggi !");
          } 
        }else if(t > 35){
          if (CTBotMessageText == myBot.getNewMessage(msg)){
            Serial.print("Pesan Masuk: ");
            Serial.println(msg.text);
            myBot.sendMessage(msg.sender.id, "Suhu ruangan terlalu tinggi !");
          }
        }
    
      } else {
        digitalWrite(pinBuzzer, LOW);
      }
    } else {
      Serial.println("Kirim data Gagal");
    }

    http.end();
  }

  delay(1000);
}