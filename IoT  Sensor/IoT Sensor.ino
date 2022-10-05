#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 

#include <Wire.h>
#include "BlueDot_BME280.h"

BlueDot_BME280 bme1;                                     //Object for Sensor 1
BlueDot_BME280 bme2;                                     //Object for Sensor 2

int bme1Detected = 0;                                    //Checks if Sensor 1 is available
int bme2Detected = 0;                                    //Checks if Sensor 2 is available

#include <ESP8266WiFi.h>

static WiFiClient espClient;
#include <ArduinoJson.h>

// 引入阿里云 IoT SDK
#include <AliyunIoTSDK.h>
#define PRODUCT_KEY "a17HV9XJkYo"
#define DEVICE_NAME "SENSOR_SERVE"
#define DEVICE_SECRET "e589f8f54edded43f11f27a3b523844a"
#define REGION_ID "cn-shanghai"

// 设置 wifi 信息

#define WIFI_SSID       "HORNOR"//替换自己的WIFI
#define WIFI_PASSWD     "yoyo123ttt"//替换自己的WIFI



void wifiInit(const char *ssid, const char *passphrase)
{
    u8g2.clearBuffer();					// clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2.drawStr(0,10,"wifiInit");	// write something to the internal memory
    u8g2.sendBuffer();					// transfer internal memory to the display
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passphrase);
    WiFi.setAutoConnect (true);
    WiFi.setAutoReconnect (true);
    while (WiFi.status() != WL_CONNECTED)
    {   
      u8g2.clearBuffer();					// clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
      u8g2.drawStr(0,10,"not Connect");	// write something to the internal memory
      u8g2.sendBuffer();					// transfer internal memory to the display

        delay(1000);
        Serial.println("WiFi not Connect");
    }
    Serial.println("Connected to AP");
     u8g2.clearBuffer();					// clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
      u8g2.drawStr(0,10,"Connected to AP");	// write something to the internal memory
      u8g2.setCursor(5,20);
      u8g2.print(WIFI_SSID);
      u8g2.sendBuffer();					// transfer internal memory to the display
      delay(1000);
}

void setup()
{ 
    u8g2.begin();  

    Serial.begin(115200);

    wifiInit(WIFI_SSID, WIFI_PASSWD);

    intiBME280();

    AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);


/*
    下面为操作其他各类topic方法，注意替换自己的参数
    AliyunIoTSDK::subscribe("/sys/${ProductKey}/${deviceName}/thing/service/${tsl.service.identifier}", callback);
    AliyunIoTSDK::publish("/sys/${ProductKey}/${deviceName}/thing/service/${tsl.service.identifier}_reply", callback);
*/
}

unsigned long lastMsMain = 0;
void loop()
{
    AliyunIoTSDK::loop();
    if (millis() - lastMsMain >= 5000)
    {  
    u8g2.clearBuffer();
    u8g2.setCursor(60,60);
    u8g2.print("wifi online");
    u8g2.setCursor(60,50);
    u8g2.print("iot    online");
    sendBME280();
    sendGas();
    u8g2.sendBuffer();
    }
}			// transfer internal memory to the display

void sendGas(){
  float GasValue = 0-(analogRead(A0)*3.3-2079)/10+23.76;
  AliyunIoTSDK::send("GasConcentration",GasValue);
   u8g2.drawStr(64,25,"GasSensor");	
   u8g2.setCursor(64,35);
  u8g2.print(GasValue);
  u8g2.print("mv");	

}

void sendBME280(){
  
					// clear the internal memory
	
  AliyunIoTSDK::send("temperature",bme2.readTempC());
   u8g2.drawStr(5,10,"Temp");	
   u8g2.setCursor(5,20);
  u8g2.print(bme2.readTempC());
  u8g2.print("°C");	
  AliyunIoTSDK::send("Humidity",bme2.readHumidity());
   u8g2.drawStr(5,30,"Hum");	
   u8g2.setCursor(5,40);
  u8g2.print(bme2.readHumidity());
  u8g2.print("%");	
  AliyunIoTSDK::send("Atmosphere",bme2.readPressure());
   u8g2.drawStr(5,50,"Pre");	
   u8g2.setCursor(5,60);
  u8g2.print(bme2.readPressure());
  u8g2.print("kpa");	
  delay(1000);
}
void intiBME280(){//BME280初始化

  Serial.println(F("Basic Weather Station"));

  //*********************************************************************
  //*************BASIC SETUP - SAFE TO IGNORE**************************** 
  
  //This program is set for the I2C mode

    bme1.parameter.communication = 0;                    //I2C communication for Sensor 1 (bme1)
    bme2.parameter.communication = 0;                    //I2C communication for Sensor 2 (bme2)
    

  
  //*********************************************************************
  //*************BASIC SETUP - SAFE TO IGNORE****************************
    
  //Set the I2C address of your breakout board  

    bme1.parameter.I2CAddress = 0x77;                    //I2C Address for Sensor 1 (bme1)
    bme2.parameter.I2CAddress = 0x76;                    //I2C Address for Sensor 2 (bme2)

    
  
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
    
  //Now choose on which mode your device will run
  //On doubt, just leave on normal mode, that's the default value

  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)
  
    bme1.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 1
    bme2.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 2 


                  
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Great! Now set up the internal IIR Filter
  //The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  //In short, a high factor value means less noise, but measurements are also less responsive
  //You can play with these values and check the results!
  //In doubt just leave on default

  //0b000:      factor 0 (filter off)
  //0b001:      factor 2
  //0b010:      factor 4
  //0b011:      factor 8
  //0b100:      factor 16 (default value)

    bme1.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 1
    bme2.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 2

    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************

    //Next you'll define the oversampling factor for the humidity measurements
  //Again, higher values mean less noise, but slower responses
  //If you don't want to measure humidity, set the oversampling to zero

  //0b000:      factor 0 (Disable humidity measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

    bme1.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 1
    bme2.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 2

    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Now define the oversampling factor for the temperature measurements
  //You know now, higher values lead to less noise but slower measurements
  
  //0b000:      factor 0 (Disable temperature measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

    bme1.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 1
    bme2.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 2

    

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //Finally, define the oversampling factor for the pressure measurements
  //For altitude measurements a higher factor provides more stable values
  //On doubt, just leave it on default
  
  //0b000:      factor 0 (Disable pressure measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)  

    bme1.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 1
    bme2.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 2 

     
  
  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE*************************
  
  //For precise altitude measurements please put in the current pressure corrected for the sea level
  //On doubt, just leave the standard pressure as default (1013.25 hPa);
  
    bme1.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 1)
    bme2.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 2)

  //Also put in the current average temperature outside (yes, really outside!)
  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C and 59°F);
  
    bme1.parameter.tempOutsideCelsius = 15;               //default value of 15°C
    bme2.parameter.tempOutsideCelsius = 15;               //default value of 15°C
  
    bme1.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F
    bme2.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F

  
    
  //*********************************************************************
  //*************ADVANCED SETUP IS OVER - LET'S CHECK THE CHIP ID!*******

  if (bme1.init() != 0x60)
  {    
    Serial.println(F("Ops! First BME280 Sensor not found!"));
    bme1Detected = 0;
  }

  else
  {
    Serial.println(F("First BME280 Sensor detected!"));
    bme1Detected = 1;
  }

  if (bme2.init() != 0x60)
  {    
    Serial.println(F("Ops! Second BME280 Sensor not found!"));
    bme2Detected = 0;
  }

  else
  {
    Serial.println(F("Second BME280 Sensor detected!"));
    bme2Detected = 1;
  }

  if ((bme1Detected == 0)&(bme2Detected == 0))
  {
    Serial.println();
    Serial.println();
    Serial.println(F("Troubleshooting Guide"));
    Serial.println(F("*************************************************************"));
    Serial.println(F("1. Let's check the basics: Are the VCC and GND pins connected correctly? If the BME280 is getting really hot, then the wires are crossed."));
    Serial.println();
    Serial.println(F("2. Did you connect the SDI pin from your BME280 to the SDA line from the Arduino?"));
    Serial.println();
    Serial.println(F("3. And did you connect the SCK pin from the BME280 to the SCL line from your Arduino?"));
    Serial.println();
    Serial.println(F("4. One of your sensors should be using the alternative I2C Address(0x76). Did you remember to connect the SDO pin to GND?"));
    Serial.println();
    Serial.println(F("5. The other sensor should be using the default I2C Address (0x77. Did you remember to leave the SDO pin unconnected?"));

    Serial.println();
    
    while(1);
  }
    
  Serial.println();
  Serial.println();


}
