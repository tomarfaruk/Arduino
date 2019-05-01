
 
#include <ESP8266WiFi.h>  //wifi module header
#include <FirebaseArduino.h>  //firebase header you have to install those two librery

#define WIFI_SSID "omar"  //wifi name
#define WIFI_PASSWORD "12345678"  //wifi password
#define FIREBASE_HOST "arduino-f7537.firebaseio.com"  //firebase app database url  app name+".firebase.com"
#define FIREBASE_AUTH "dckvEukpVTwlsBxZ7I8MUGTtvsEZiDpl7mjP3aDo"  ///firebase app auth key

//declaration the pins
int device1 = D0;
int device2 = D1;
int device3 = D2;
int device4 = D3;
int device5 = D4;
int device6 = D5;
int echo = D6;
int triger = D7;
int pir = D8;
int lm = A0;
long duration;
int distance;



void setup(){
   ///initialize the pins 
    pinMode(device1, OUTPUT);
    pinMode(device2, OUTPUT);
    pinMode(pir, INPUT);
    pinMode(lm, INPUT);
    pinMode(device3, OUTPUT);
    pinMode(device4, OUTPUT);
    pinMode(device5, OUTPUT);
    pinMode(device6, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(triger, OUTPUT);
    

    Serial.begin(9600); //serial monitor initialize

  ///wifi connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
      }
      Serial.println();
      Serial.print("connected: ");
      Serial.println(WiFi.localIP());

      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //firebase connection
  }

///firebase app or databse connection if faile 
 void firebasereconnect()
  {
    Serial.println("Trying to reconnect");
    //delay(500);
    //ESP.wdtFeed();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    delay(5);
    ESP.wdtDisable();
  }

void loop(){

    //if firebase error
   if(Firebase.failed()){
       Serial.print("Firebase error ");
       firebasereconnect();       
     }

    //re-check the wifi connection if not connected re-try to connect
     while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.println("wifi probelm");
        delay(2000);
      }
     
     
    ///read data from firebase and store the value in a variable 
     boolean dev1 = Firebase.getBool("devices/device1/status");
     Serial.print("dev1 hello: ");
      Serial.println(dev1);
     boolean dev2 = Firebase.getBool("devices/device2/status");
     Serial.print("dev2: ");
      Serial.println(dev2);
     boolean dev3 = Firebase.getBool("devices/device3/status");
     Serial.print("dev3: ");
      Serial.println(dev3);
     boolean dev4 = Firebase.getBool("devices/device4/status");
     Serial.print("dev4: ");
      Serial.println(dev4);
     boolean dev5 = Firebase.getBool("devices/device5/status");
     Serial.print("dev5: ");
      Serial.println(dev5);
     boolean dev6 = Firebase.getBool("devices/device6/status");
     Serial.print("dev6: ");
      Serial.println(dev6);
     boolean dev7 = Firebase.getBool("devices/device7/status");
     Serial.print("dev7: ");
      Serial.println(dev7);
     boolean dev8 = Firebase.getBool("devices/device8/status");
     Serial.print("dev8: ");
      Serial.println(dev8);
     boolean pir = Firebase.getBool("condition/pir");
      Serial.print("pir: ");
      Serial.println(pir);
     float temp = Firebase.getFloat("condition/temp");
      Serial.print("temp: ");
      Serial.println(temp);

     //device1
     if(dev1){
      digitalWrite(device1, 1);
      Serial.println("device1 is on");
     }
     else{
      digitalWrite(device1, 0);
      Serial.println("device1 is off");
     }

      //device2
     if(dev2){
      digitalWrite(device2, 1);
      Serial.println("device2 is on");
     }
     else{
      digitalWrite(device2, 0);
      Serial.println("device2 is off");
     }

     //device3
     if(dev3){
      digitalWrite(device3, 1);
      Serial.println("device3 is on");
     }
     else{
      digitalWrite(device3, 0);
      Serial.println("device3 is off");
     }


     //device4
     if(dev4){
      digitalWrite(device4, 1);
      Serial.println("device4 is on");
     }
     else{
      digitalWrite(device4, 0);
      Serial.println("device4 is off");
     }


     //device5
     if(dev5){
      digitalWrite(device5, 1);
      Serial.println("device5 is on");
     }
     else{
      digitalWrite(device5, 0);
      Serial.println("device5 is off");
     }

     //device6
     if(dev6){
      digitalWrite(device6, 1);
      Serial.println("device6 is on");
     }
     else{
      digitalWrite(device6, 0);
      Serial.println("device6 is off");
     }

     
  /*sensors sensor it will take input from your invironment and send data to the database, if the temparatur is higher then 50 only then send a data .*/
    float temparature = analogRead(lm); //LM 35 temparature
    temparature = (temparature* 500)/1023.0;

    //for checking sonar sensor and send notice
    if(temparature > 50 ){
      
      digitalWrite(triger, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(triger, HIGH);
      delayMicroseconds(4);
      digitalWrite(triger, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echo, HIGH);
      // Calculating the distance
      distance = duration*0.034/2;

      if(distance > 10){
        Firebase.setBool("condition/notice", true);
      }
      else{
        Firebase.setBool("condition/notice", false);
      }
      // Prints the distance on the Serial Monitor
      Serial.print("Distance: ");
      Serial.println(distance);
      Firebase.setInt("condition/sonar", distance);
//      Firebase.remove("condition/soanr");
//      delay(1000);
    }
    Firebase.setFloat("condition/temp", temparature);

    //sonar sensor for map object distance 
     int val = digitalRead(pir);  
     Firebase.setBool("condition/pir", val);

     // for testing is database work or not 
     Firebase.setInt("temp", 34543);
     delay(200);
     Firebase.remove("temp");

}
