//Rahul 18BCE0018
//Connections:
//Connect Ultra Sonic - VCC to Vin , GND to GND near Vin , Echo to D2, Trig to D1
//Connect Infrared - VCC to Vin , GND to GND near Vin , Out to D4
//Connect MQ2 Gas Sensor - VCC to Vin , GND to GND near Vin , A0 to A0;
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define TRIGGERPIN D1
#define ECHOPIN    D2

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
BlynkTimer timer;
int flag=0;
char auth[] = "XH8FggY90xQFzRQkYm005LW-tR_D-ggx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PSCHAHAR";//your wifi name
char pass[] = "dkcpp9109n"; //your wifi password
int eggs=0;
int noti=0;
int mq2 = A0; // smoke sensor is connected with the analog pin A0 
int data = 0; 
WidgetLCD lcd(V1);
WidgetLCD lcd1(V2);


void notifyOnThings()
{
  int isButtonPressed = digitalRead(D4);
  if (isButtonPressed==1 && flag==0) {
    Serial.println("Milk quantity Low!");
    Blynk.notify("Alert : Milk Quantity Low, Buy Milk!");
    Blynk.email("rahulxdagar@gmail.com", "Subject: Buy Milk", "Milk is about to get over. Please buy Milk!");
    flag=1;
  }
  else if (isButtonPressed==0)
  { 
    flag=0;
  }
}

int calceggs()
{
   // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  long duration, distance;
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12); 
  digitalWrite(TRIGGERPIN, LOW);
  
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  if (distance <= 5 && distance>=2) {
  eggs=3;
  Serial.println("No. of Eggs: 3");
  }
  else if (distance >=7 && distance<=10) {
    eggs=2;
  Serial.println("No. of Eggs: 2");
  }
  else {
    Serial.println("No. of Eggs: 1"); 
    eggs=1;

  }
  
  return eggs;
}

void getSendData()
{
  data = analogRead(mq2); 
  Blynk.virtualWrite(V4, data);
 
  if (data > 500 )
  {
    Blynk.notify("Fumes Detected : Possible Rotten Food!");
    Blynk.email("rahulxdagar@gmail.com", "Subject: Food has Rotten", "Your Vegeables and Fruits in the Vegetable compartment has gone bad. Buy fresh vegetables."); 
    
  }
}
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  
  pinMode(D4,INPUT_PULLUP);   //For IR Sensor
  timer.setInterval(1000L,notifyOnThings);
   
  timer.setInterval(1000L, getSendData); //For Gas Sensor
  
  pinMode(TRIGGERPIN, OUTPUT);  //For Ultra sensoor
  pinMode(ECHOPIN, INPUT);

  
 
  lcd.clear(); //Use it to clear the LCD Widget
  lcd1.clear(); //Use it to clear the LCD Widget
  lcd.print(0, 0, "No of Eggs:"); 
  lcd1.print(0, 0, "Milk Quantity:"); 

}

void loop()
{ int num=0;
  int milk;

  //Display and Alert for Milk
  lcd1.clear();
  milk=digitalRead(D4);
  lcd1.print(0, 0, "Milk Quantity:"); 
  if(milk==0)
  {
    lcd1.print(7, 1, "High"); 
  }
  else
  {
    lcd1.print(7, 1, "Low"); 
  }

  //Display and Alert for eggs
  lcd.clear();
  lcd.print(2, 0, "Eggs in tray");
  num=calceggs();
  lcd.print(7, 1, num);
  if(num>1){
    noti=0;
  }
  if(num<2 && noti==0){
    Blynk.notify("Alert : Only 1 Egg left! Buy Eggs");
    Blynk.email("rahulxdagar@gmail.com", "Subject: Buy Eggs", "Only 1 Eggs is left. Buy More Eggs!");
    noti=1;
  }
  
  Blynk.run();
  timer.run();
  delay(1000);

}
