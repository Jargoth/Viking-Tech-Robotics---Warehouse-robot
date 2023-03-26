//COM3

#include <SoftwareSerial.h>
SoftwareSerial esp8266(5,4);


#define DEBUG true


char box[4];
unsigned long lastTimeMillis = 0;
int workActive = 0;

void setup() {
  Serial.begin(9600);
  while (! Serial);
  esp8266.begin(9600);
  if (esp8266.isListening()) { 
        Serial.println("esp8266 is listening!");
    }
  Serial.println("Init WIFI");
  InitWifiModule();

}

void loop() {
  esp8266data();

}
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";                                             
    esp8266.print(command);  
    long int time = millis();                                      
    while( (time+timeout) > millis())                                 
    {      
      while(esp8266.available())                                      
      {
        char c = esp8266.read(); 
        response+=c;                                                  
      }  
    }    
    if(debug)                                                        
    {
      Serial.print(response);
    }    
    return response;                                                  
}
void InitWifiModule()
{
  sendData("AT+RST\r\n", 2000, DEBUG); 
  sendData("AT+CWJAP=\"NOSnet\",\"Kalle Anka123\"\r\n", 2000, DEBUG);        
  delay (3000);
  sendData("AT+CWMODE=1\r\n", 1500, DEBUG);                                             
  delay (1500);
  sendData("AT+CIFSR\r\n", 1500, DEBUG);                                   

}

void printResponse() {
  while (esp8266.available()) {
    Serial.println(esp8266.readStringUntil('\n')); 
  }
}
void printResponse2() {
  int i = 0;
  while (esp8266.available()) {
    i++;
    String s = esp8266.readStringUntil('\n');
    
    if (i==4)
    {
//      Serial.println(s); 
      box[0] = s[9];
      box[1] = s[10];
      box[2] = s[11];
      box[3] = '\n';
    }
  }
  Serial.print(i);
  Serial.println(box);
}
void esp8266data() {
  int i = 0;
  String ans;
  String ans2;

  if (millis() - lastTimeMillis > 30000) {
    lastTimeMillis = millis();

    esp8266.println("AT+CIPMUX=1");
    delay(1000);
    printResponse();

    esp8266.println("AT+CIPSTART=4,\"TCP\",\"192.168.1.138\",80");
    delay(1000);
    printResponse();

    String cmd = "GET http://192.168.1.138/vikingtech/robot.php";
    Serial.print("AT+CIPSEND=4," + String(cmd.length() + 4));
    esp8266.println("AT+CIPSEND=4," + String(cmd.length() + 4));
    delay(2000);
    printResponse();
    Serial.print(cmd);

    esp8266.println(cmd);
    delay(2000);
    
    printResponse();
    esp8266.println(""); 
  }

  if (esp8266.available()) {
    ans = esp8266.readString();
    ans2 = ans.substring(39,42);
    box[0] = ans2.charAt(0);
    box[1] = ans2.charAt(1);
    box[2] = ans2.charAt(2);
    box[3] = '\0';
    workActive = 1;
    Serial.print(box);
  }

}
