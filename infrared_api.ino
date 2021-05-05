#include <SoftwareSerial.h>                                   //SoftwareSerial kütüphanimizi ekliyoruz.
  
String agAdi = "TTNET_ZyXEL_V79H";                 //Ağımızın adını buraya yazıyoruz.    
String agSifresi = "**********";           //Ağımızın şifresini buraya yazıyoruz.

int rxPin = 10;                                               //ESP8266 RX pini
int txPin = 11;                                               //ESP8266 TX pini

String ip = "184.106.153.149";                                //Ahmet bey in sayfasinin ip adresi
int irSensorPin = 3;
int ledPin = 8;                                               //led pinimiz
int count = 0;                                                //arac sayisi

int r0 = 0;                                                   //Ilk deger
SoftwareSerial esp(rxPin, txPin);                             //Seri haberleşme pin ayarlarını yapıyoruz.

void setup() { 
  pinMode(irSensorPin,INPUT); 
  pinMode(ledPin, OUTPUT);                            //ledi cikis pini olarak atadik
  attachInterrupt(1,arac_say, RISING);                 //pin digital 3
  Serial.begin(9600);  //Seri port ile haberleşmemizi başlatıyoruz.
  Serial.println("Started");
  esp.begin(115200);                                          //ESP8266 ile seri haberleşmeyi başlatıyoruz.
  esp.println("AT");                                          //AT komutu ile modül kontrolünü yapıyoruz.
  Serial.println("AT Yollandı");
  while(!esp.find("OK")){                                     //Modül hazır olana kadar bekliyoruz.
    esp.println("AT");
    Serial.println("ESP8266 Bulunamadı.");
  }
  Serial.println("OK Komutu Alındı");
  esp.println("AT+CWMODE=1");                                 //ESP8266 modülünü client olarak ayarlıyoruz.
  while(!esp.find("OK")){                                     //Ayar yapılana kadar bekliyoruz.
    esp.println("AT+CWMODE=1");
    Serial.println("Ayar Yapılıyor....");
  }
  Serial.println("Client olarak ayarlandı");
  Serial.println("Aga Baglaniliyor...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    //Ağımıza bağlanıyoruz.
  while(!esp.find("OK"));                                     //Ağa bağlanana kadar bekliyoruz.
  Serial.println("Aga Baglandi.");
  delay(1000);
}
void loop() {
  
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           //Thingspeak'e bağlanıyoruz.
  if(esp.find("Error")){                                      //Bağlantı hatası kontrolü yapıyoruz.
    Serial.println("AT+CIPSTART Error");
  }
  
    Serial.print("Arac sayisi=");
    Serial.println(count);
  String veri = "GET https://api.thingspeak.com/update?api_key=3R5RXJX7025XU85A";   //Thingspeak komutu. Key kısmına kendi api keyimizi yazıyoruz.                                   //Göndereceğimiz sıcaklık değişkeni
  veri += "&field1=";
  veri += String(count);                                        //Göndereceğimiz nem değişkeni
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   //ESP'ye göndereceğimiz veri uzunluğunu veriyoruz.
  esp.println(veri.length()+2);
  delay(2000);
  if(esp.find(">")){                                          //ESP8266 hazır olduğunda içindeki komutlar çalışıyor.
    esp.print(veri);                                          //Veriyi gönderiyoruz.
    Serial.println(veri);
    Serial.println("Veri gonderildi.");
    delay(1000);
  }
  Serial.println("Baglantı Kapatildi.");
  esp.println("AT+CIPCLOSE");                                //Bağlantıyı kapatıyoruz
  delay(1000);                                               //Yeni veri gönderimi için 1 dakika bekliyoruz.
}
int arac_say()
{ int r = digitalRead(irSensorPin) ;
  if (r == HIGH && r0 == LOW){
    digitalWrite(ledPin, HIGH);
    delay(10);
    digitalWrite(ledPin, LOW);
    count = count + 1;//cisim sayisini 1 arttiriyor.
    Serial.println("Algilanan Cisim Sayisi: ");//ve yazdiriyoruz.
    Serial.println(count);
    r0=r;
    }
 
  else { 
    
     Serial.println("Algilanan Cisim Sayisi: ");//cisim gecmemisse birsey yapmayacaktir..
     Serial.println(count);
       r0=r;
  }
  
}
