 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80); // 80 is the port number

const char* ssid="OmAr";
const char* password="01746160162";

const int sensor=A0; // Assigning analog pin A0 to variable 'sensor'
float tempc;  //variable to store temperature in degree Celsius
float tempf;  //variable to store temperature in Fahreinheit 
float vout;  //temporary variable to hold sensor reading

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
int distance;

String Website,data,temp,XML,Javascript;
int inc=0;

void javascriptContent(){
    Javascript ="<SCRIPT>\n";
    Javascript+="var xmlHttp=createXmlHttpObject();\n";
    Javascript+="function createXmlHttpObject(){\n";
    Javascript+="if(window.XMLHttpRequest){\n";
    Javascript+="xmlHttp=new XMLHttpRequest();\n";
    Javascript+="}else{\n";
    Javascript+="xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
    Javascript+="}\n";
    Javascript+="return xmlHttp;\n";
    Javascript+="}\n";
    Javascript+="\n";
    Javascript+="function response(){\n";
    Javascript+="xmlResponse=xmlHttp.responseXML;\n";
    Javascript+="xmldoc = xmlResponse.getElementsByTagName('data');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('div1').innerHTML=message;\n";
    Javascript+="}\n";

    Javascript+="function process(){\n";
    Javascript+="xmlHttp.open('PUT','xml',true);\n";
    Javascript+="xmlHttp.onreadystatechange=response;\n";
    Javascript+="xmlHttp.send(null);\n";
    Javascript+="setTimeout('process()',100);\n";
    Javascript+="}\n";
    
    Javascript+="</SCRIPT>\n";
    
  
  }

void WebsiteContent(){
    javascriptContent();
    
    Website="<html>\n";
    Website+="<style>\n";
    Website+="#div1{\n";
    Website+="width:900px;\n";
    Website+="margin:0 auto;\n";
    Website+="margin-top:130px;\n";
    Website+="font-size:700%;\n";
    Website+="color:powderblue;\n";
    Website+="}\n";
    Website+="</style>\n";
    Website+="<body onload='process()'>";
    Website+="<div id='div1'>"+data+"</div></body></html>";
    Website+=Javascript;
    server.send(200,"text/html",Website);
  }
void XMLcontent(){

  XML ="<?xml version='1.0'?>";
  XML+="<data>";
  XML+=data;
  XML+="</data>";

  server.send(200,"text/xml",XML);
  
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(sensor,INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.print(WiFi.localIP());
  server.on("/",WebsiteContent);
  server.on("/xml",XMLcontent);
  server.begin();


}

float sonar(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;

}

float lm_35(){
  vout=analogRead(sensor);
  vout=(vout*500)/1024;
  tempc=vout; // Storing value in Degree Celsius
  tempf=(vout*1.8)+32; // Converting to Fahrenheit 
  Serial.print("in DegreeC=");
  Serial.print("\t");
  Serial.print(tempc);
  return tempc;
}

void loop() {
  // put your main code here, to run repeatedly:
//  inc = inc + 1;
  data = "distence: "+(String)sonar();
  data += "\ntemperature: " + (String)lm_35();
  server.handleClient();

}
     
    
