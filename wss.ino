#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// GSM module setup
SoftwareSerial gsmSerial(12, 11);  // RX, TX pins

String phoneNumber = "+91XXXXXXXXXX";  // Replace with the desired phone number

// GPS module setup
SoftwareSerial gpsSerial(3, 4);  // RX, TX pins

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;
TinyGPSPlus gps;

// SOS button setup
const int sosButtonPin = A5;

void setup() 
{
  Serial.begin(9600);
  gsmSerial.begin(9600);
  gpsSerial.begin(9600);
  pinMode(sosButtonPin, INPUT_PULLUP);
}

void loop() 
{
  while (gpsSerial.available() > 0) 
  {
    if (gps.encode(gpsSerial.read())) 
    {
      handleGPS();
      Serial.print("Inside loop...");
    }
  }

  delay(200);
}

void handleGPS() 
{
  if (gps.location.isValid()) 
  {  Serial.print("Valid Location---");
     float latitude = gps.location.lat();
     float longitude = gps.location.lng();
     Serial.print("Latitude= ");
       Serial.println(latitude, 6);
       Serial.print(", Longitude=");
       Serial.println(longitude, 6);

    if (digitalRead(sosButtonPin) == LOW) 
    {
      String message = "Emergency Alert!\r\n";
      message += "Latitude: " + String(latitude, 6) + "\r\n";
      message += "Longitude: " + String(longitude, 6) + "\r\n";
      message += "Location URL: http://maps.google.com/maps?q=loc:" + String(latitude, 6) + "," + String(longitude, 6);

      sendSMS(phoneNumber, message);
    }
  } 
  
  else
   {
      Serial.println("Invalid GPS Data");

      Serial.println("Location: Not Available");

    if (digitalRead(sosButtonPin) == LOW)
    { 
       float latitude=28.6095;
       float longitude=77.0386;
       Serial.print("SOS PRESSED!!!");
       Serial.print("Latitude= ");
       Serial.println(latitude, 6);
       Serial.print(", Longitude=");
       Serial.println(longitude, 6);

       String message = "Emergency Alert!\r\n";
       message += "Latitude: " + String(latitude, 6) + "\r\n";
       message += "Longitude: " + String(longitude, 6) + "\r\n";
       message += "Location URL: http://maps.google.com/maps?q=loc:" + String(latitude, 6) + "," + String(longitude, 6);

       sendSMS(phoneNumber, message);
    }
  }
}

void sendSMS(String number, String message) 
{
  gsmSerial.print("AT+CMGF=1\r");
  delay(1000);
  gsmSerial.print("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  gsmSerial.print(message);
}
