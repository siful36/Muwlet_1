//format data = tujuan-asal-jenis-data-data-data-e-e
/*
A0 sensor arus
D0/16 indikator wifi
D2/4 output
D5/14 kontrol dekat = input
D6/12 sensor suhu
D12/SDD3/10 indikator kontrol dekat

channel kontrol : 116608
channel data    : 100192
*/
#include <ESP8266WiFi.h>
#include <OneWire.h>
OneWire ds(12);

const char* ssid      ="si botak";
const char* password  ="delapank";

const char* host      ="api.thingspeak.com";

const char* apikey    ="QIQC6QWCAVDYYJHM";
const char* streamId  ="/channels/116608/feeds.json";
const int httpPort = 80;

String  dt[80],dta[50];
float arus=2;
float suhu=25;

float sensor_suhu();
float sensor_arus();
void kondisi_led(String led);

void node_1(String asal, String kondisi);
void node_done(String asal, String kondisi);

void setup() 
{
  pinMode(10,OUTPUT); //indikator kontrol dekat
  pinMode(4,OUTPUT); //output
  pinMode(14,INPUT); //kontrol dekat
  
  Serial.begin(9600);
  delay(100);

  Serial.println("Connecting to ");
   Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status()  !=  WL_CONNECTED)
  {
    delay(400);
    Serial.print(".");
    if(digitalRead(14)==LOW)
      {
        digitalWrite(10,LOW);  
        //kondisi_led("2"); 
      }
    else
        digitalWrite(10,HIGH);
  }
  Serial.println();
  Serial.println("WIFI CONNECTED ");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  pinMode(16,OUTPUT); //indikator wifi

}
 void loop()
 {
    arus=sensor_arus();
    suhu=sensor_suhu(); 
    unsigned int j =0,i;
    String datot,asal,tujuan,jenis,on_off;
    if(digitalRead(14)==LOW)
    {
        digitalWrite(10,LOW);  
        //kondisi_led("2"); 
    }
    else
      digitalWrite(10,HIGH);
    
    WiFiClient client;    
    if(!client.connect(host,httpPort))
    {
      Serial.println("connecting failed");
      digitalWrite(16,HIGH);
      return;
    }
    String url2 ="";
    url2 += streamId;
    url2 += "?results=1";
    
    digitalWrite(16,LOW);
    
    Serial.print("Requesting URL : ");
    Serial.println(url2);
    
    client.println(String("GET ") + url2 );
    
    if(digitalRead(14)==LOW)
    {
        digitalWrite(10,LOW);  
       // kondisi_led("2"); 
    }
    else
      digitalWrite(10,HIGH);
    
    delay(500);
    
    while(client.available())
    {
      String line2 = "";
      line2 = client.readStringUntil('\r');
      Serial.print(line2);
      for(i=1;i<line2.length();i++)
      {
       
        if(line2[i] == '"')
        {
          j++;
          dt[j]="";
        } 
        else
        {
          dt[j]=dt[j]+line2[i];
        }
      }
      datot = dt[45];
      asal=datot.substring(0,3);
      tujuan=datot.substring(3,6);
      on_off=datot.substring(6,9);//data1
      
    }   // asal-tujuan-data1-data2-data3-ext-che
      Serial.println();
      Serial.print("data = ");
      Serial.println(dt[45]);
      Serial.print("datot = ");
      Serial.println(datot);
      Serial.print("asal = ");
      Serial.println(asal);
      Serial.print("tujuan = ");
      Serial.println(tujuan);
      Serial.print("on_off = ");
      Serial.println(on_off);
      
      if(asal=="001"&&tujuan=="001")
      {
        node_1(asal,on_off);
        node_done(asal,on_off);
      }
     
      delay(100);
      
 }

 void node_1(String asal, String kondisi)
 {
    bool k_kirim=false;
    kondisi_led(kondisi);
    WiFiClient client;
    do{
      
      if(digitalRead(14)==LOW)
      {
        digitalWrite(10,LOW);  
        //kondisi_led("2"); 
      }
      else
        digitalWrite(10,HIGH);
   
ulang_ngrim:
      if(!client.connect(host,httpPort))
      {
        Serial.println("connecting failed");
        digitalWrite(16,HIGH);
        goto ulang_ngrim;
      }
      
      digitalWrite(16,LOW);
      arus=sensor_arus();
      suhu=sensor_suhu();
      
      // asal-tujuan-data1-data2-data3-ext-che
       
      String url3 ="";
      url3 += "/update?api_key=QIQC6QWCAVDYYJHM";
      //url3 += apikey;
      url3 += "&field1=";
      url3 += asal;     //asal node1 
      url3 += "002";    //tujuan : 001 kontrol MUWLET ; 002 MUWLET  ; 003 simpan master
      url3 += kondisi;  //data1
      url3 += arus;     //data2
      url3 += suhu;     //data3
      
      // Serial.print("Kirim URL : ");
      // Serial.println(url3);
      delay(100);  
      client.println(String("GET ") + url3  );
       if(digitalRead(14)==LOW)
      {
        digitalWrite(10,LOW);  
        //kondisi_led("2"); 
      }
      else
        digitalWrite(10,HIGH);
        
      delay(250);
      
      while(client.available())
      {
        String line4 = client.readStringUntil('\r');
        Serial.print(line4);  
        if(line4 != "0" )   //3<data<6
        {
          k_kirim=true;
        }
      }
      Serial.println();
              
  }while(k_kirim==false);
  delay(100);
 }

//==================================================================================
void node_done(String asal, String kondisi)
 {
    bool k_kirim=false;
    WiFiClient client;
    do{
      
      if(digitalRead(14)==LOW)
      {
        digitalWrite(10,LOW);  
        //kondisi_led("2"); 
      }
      else
        digitalWrite(10,HIGH);
   
ulang_ngrim:
      if(!client.connect(host,httpPort))
      {
        Serial.println("connecting failed");
        digitalWrite(16,HIGH);
        goto ulang_ngrim;
      }
      
      digitalWrite(16,LOW);
      arus=sensor_arus();
      suhu=sensor_suhu();
      
      // asal-tujuan-data1-data2-data3-ext-che
      //tujuan 000=dikirim ; 001=diterima node
       
      String url3 ="";
      url3 += "/update?api_key=ODIDPCI4YIO7VS00";
      //url3 += apikey;
      url3 += "&field1=";
      url3 += asal;     //asal node1 
      url3 += "003";    //tujuan : 001 kontrol MUWLET ; 002 MUWLET  ; 003 simpan master
      url3 += kondisi;  //data1
      url3 += arus;     //data2
      url3 += suhu;     //data3
      
      // Serial.print("Kirim URL : ");
      // Serial.println(url3);
      delay(100);  
      client.println(String("GET ") + url3  );
       if(digitalRead(14)==LOW)
      {
        digitalWrite(10,LOW);  
        //kondisi_led("2"); 
      }
      else
        digitalWrite(10,HIGH);
        
      delay(250);
      
      while(client.available())
      {
        String line4 = client.readStringUntil('\r');
        Serial.print(line4);  
        if(line4 != "0" )   //3<data<6
        {
          k_kirim=true;
        }
      }
      Serial.println();
              
  }while(k_kirim==false);
  delay(100);
 }
//==================================================================================
int led_=0;
void kondisi_led(String led)
{
  
  if(led=="001")
  {
    digitalWrite(4,HIGH);
    Serial.println("Lampu nyala");
    led_=led_+1;
  }
  else if(led=="000")
  {
    digitalWrite(4,LOW);
    Serial.println("Lampu Mati");
    led_=led_+1;
  }
  else if(led=="2")
  {
    led_=led_+1;
    if(led_%2==0)
    {
      digitalWrite(4,LOW);
      Serial.println("Lampu Mati");
    }
    
    else if(led_%2!=0)
    {
      digitalWrite(4,HIGH);
      Serial.println("Lampu Nyala");
      led_=1;
    }
  }
  delay(200);  
}
//=====================================================================
float sensor_arus()
{
  float ampere;
  int ars=1000;
  Serial.println("Sensor Arus");
  //ars=analogRead(A0);
  ampere=ars*3/2;
  return(ampere);
}
//=====================================================================
float sensor_suhu()
{
  byte i;
  byte present =0;
  byte type_s;
  byte data[12];
  byte addr[8];
  int16_t celsius;
  Serial.println("Sensor Suhu");
  ulang_ssuhu:
  if(!ds.search(addr))
  {
   // Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(300);
    goto ulang_ssuhu;
  }

 /* Serial.print("ROM = ");
  for(i = 0 ; i < 8 ; i++)
  {
    Serial.write(' ');
    Serial.print(addr[i],HEX);
  }
  */
//  if (OneWire::crc8(addr, 7) != addr[7]) {
//      Serial.println("CRC is not valid!");
 //       return(celsius);
 // }
//  Serial.println();
  // the first ROM byte indicates which chip
  ulang_suhu:
  switch (addr[0]) 
  {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
     goto ulang_suhu;
  } 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

 // Serial.print("  Data = ");
 // Serial.print(present, HEX);
 // Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  //  Serial.print(data[i], HEX);
  // Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(data, 8), HEX);
  //Serial.println();
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = raw/1.9;  //(float)raw; // 16.0;
 // data[5]=String(celsius,DEC);
  //fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.println(celsius);
  //Serial.print(" Celsius, ");
  //Serial.print(fahrenheit);
  //Serial.println(" Fahrenheit");
  return(celsius);
}

