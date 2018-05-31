
#define BLYNK_PRINT Serial
float biaya, pajak, biaya_total, Rm ;
float  cost ;

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include <PZEM004T.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char auth[] = "5032192620e541d6bfeb9d375aaca819";
char ssid[] = "Aseplah";
char pass[] = "rt01rw01";

#define EspSerial Serial1
BlynkTimer timer;

PZEM004T pzem(10, 11);
IPAddress ip(192, 168, 1, 1);
#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);
LiquidCrystal_I2C lcd(0x3f, 20, 4);
const int buzzerPin = 8;
void notifs()
{
  if (biaya_total > 100000)
  {
    Blynk.notify("Anda sudah melebihi harga pemakaian energi bulanan");
  }
}

void myTimerEvent()
{
  float v = pzem.voltage(ip);
  float e = pzem.energy(ip);
  float i = pzem.current(ip);
  int p = pzem.power(ip);
  biaya = (e / 1000) * 1467.28;
  pajak = biaya * 0.03;
  biaya_total = biaya + pajak;
  Rm = 76298.56;
  cost ;

  if (v > 0)
  {
    Blynk.virtualWrite(V0, v);
    Blynk.virtualWrite(V3, e);
    Blynk.virtualWrite(V1, i);
    Blynk.virtualWrite(V2, p);

    if (e >= 52000) {
      cost = biaya_total;
    }
    else {
      cost = Rm;
    }

    if (biaya_total > 100000)
    {
      digitalWrite(buzzerPin, HIGH);
      //delay(1000);
      //digitalWrite(buzzerPin, LOW);
      //delay(3000);
      lcd.clear();
      lcd.setCursor(8, 0);
      lcd.print("V:");
      lcd.print(v);
      lcd.setCursor(0, 0);
      lcd.print("A:");
      lcd.print(i);
      lcd.setCursor(0, 1);
      lcd.print("W:");
      lcd.print(p);
      lcd.setCursor(8, 1);
      lcd.print("Wh:");
      lcd.print(e);
      lcd.setCursor(5, 2);
      lcd.print("Rp:");
      lcd.print(cost);
      Blynk.virtualWrite(V4, cost);
      lcd.setCursor(0, 3);
      lcd.print("SEGERA HEMAT ENERGI!");
 
    } else {
      digitalWrite(buzzerPin, LOW);
      lcd.setCursor(8, 0);
      lcd.print("V:");
      lcd.print(v);
      lcd.setCursor(0, 0);
      lcd.print("A:");
      lcd.print(i);
      lcd.setCursor(0, 1);
      lcd.print("W:");
      lcd.print(p);
      lcd.setCursor(8, 1);
      lcd.print("Wh:");
      lcd.print(e);
      lcd.setCursor(5, 2);
      lcd.print("Rp:");
      lcd.print(cost);
      Blynk.virtualWrite(V4, cost);
    }
  } else {
    lcd.clear();
    lcd.print("No Data !!!");
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(6, 1);
  lcd.print("AsepM.A");
  lcd.setCursor(5, 2);
  lcd.print("054114050");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("..|Energy|");
  lcd.setCursor(6, 2);
  lcd.print("| Meter|..");
  delay(3000);
  lcd.clear();
  lcd.print("Connecting...!!!");

  delay(10);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(3000L, myTimerEvent);
  timer.setInterval(300000L, notifs);
  lcd.clear();
  lcd.print("Connected !!!");
  lcd.clear();
}

void loop()
{

  Blynk.run();
  timer.run();
}

