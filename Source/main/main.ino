//COM4

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SD.h>

#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8
#define pulse          2
#define data           3
#define POWER         A3
const int returnSwitchPin = 6;
#define DEBUG true

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SoftwareSerial esp8266(5,4);  

const char *characters[] = {"a", "b", "c", "d", "e", "f", "g", "h",
       "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
       "u", "v", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G",
       "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S",
       "T", "U", "V", "X", "Y", "Z", "0", "1", "2", "3", "4", "5"
       "6", "7", "8", "9"};

char *pwd[25];
int selectedChar = 0;

int currMenu = 1;
int selected = 1;
int lastSelectedMenu = 0;
int currSettings = 1;
int selectedSettings = 1;
int lastSelectedSettings = 0;
int sceneChanged = 1;
int oldKeyStroke = 0;
int menuChanged = 1;
int lastSelectedBlink = 0;
int currentMillis = 0;
int selectedBlink = 0;

String serialData = String(6);
int newSerialData = 0;
unsigned long lastPoweredOn = 0;
unsigned long lastTimeMillis = 0;

//variables for control
char i2cResponse = '0';
char box[4];
int workActive = 0;
int x1 = 0;
int x1done = 0;
int z1 = 0;
int z1done = 0;
int s1 = 0;
int s1done = 0;
int g1 = 0;
int g1done = 0;
int z2 = 0;
int z2done = 0;
int s2 = 0;
int s2done = 0;
int x2 = 0;
int x2done = 0;
int z3 = 0;
int z3done = 0;
int g2 = 0;
int g2done = 0;
int g3 = 0;
int g3done = 0;
int g4 = 0;
int g4done = 0;
int z4 = 0;
int z4done = 0;
int x3 = 0;
int x3done = 0;
int s3 = 0;
int s3done = 0;
int z5 = 0;
int z5done = 0;
int g5 = 0;
int g5done = 0;
int s4 = 0;
int s4done = 0;
int z6 = 0;
int z6done = 0;
int x4 = 0;
int x4done = 0;
int z7 = 0;
int z7done = 0;
int z8 = 0;
int z8done = 0;

void setup() {
  pinMode(POWER, OUTPUT); 
  digitalWrite(POWER, HIGH);
  Wire.begin();
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Everything ok");
  esp8266.begin(9600);
  if (esp8266.isListening()) { 
        Serial.println("esp8266 is listening!");
    }
  Serial.println("Init WIFI");
  InitWifiModule();
  Serial.println("Init tft");
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  pinMode(pulse, OUTPUT);  
  pinMode(data, INPUT); 
  lastPoweredOn = millis();
  pinMode(returnSwitchPin, INPUT_PULLUP); 
}

void loop() {
  if (workActive == 0)
  {
    esp8266data();
  }
  if (workActive == 1)
  {
    workDo();
    workDoing();
    workDone();
  }
  checkScene();
  if (menuChanged == 1)
  {
    menu();
    menuChanged = 0;
    if (currMenu == 4)
    {
      settingsMenu();
    }
  }
//  int Key = 0;
//  Key = getkey();
//  keyPressed(Key);

  blinkControl();

  if (newSerialData == 1)
  {
    sendData();
    newSerialData = 0;
  }

  if ((millis()-lastPoweredOn) > 60000)
  {
    digitalWrite(POWER, LOW);
    lastPoweredOn = millis();
  }
}

void tftPrintText(int x, int y, uint16_t color, int size, char *text){
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextWrap(false);
  
  tft.setTextSize(size);
  tft.print(text);
}

void tftTickBox(int x, int y){
  tft.drawLine(x-1, y, x+3, y+7, ST77XX_BLACK);
  tft.drawLine(x+3, y+7, x+12, y-4, ST77XX_BLACK);
  tft.drawLine(x-2, y, x+2, y+7, ST77XX_BLACK);
  tft.drawLine(x+2, y+7, x+13, y-4, ST77XX_BLACK);
}

void menu(){
  if (selected == 1 & selectedBlink == 1)
  {
    tft.drawRect(0, 104, 32, 22, 33808);
    tft.fillRect(2, 106, 28, 18, 33808);
    tftPrintText(3, 111, ST77XX_WHITE, 1, "INFO");
  }
  else if (currMenu == 1)
  {
    tft.drawRect(0, 104, 32, 22, ST77XX_BLACK);
    tft.fillRect(2, 106, 28, 18, ST77XX_BLACK);
    tftPrintText(3, 111, ST77XX_WHITE, 1, "INFO");
  }
  else
  {
    tft.drawRect(0, 104, 32, 22, ST77XX_WHITE);
    tft.drawRect(1, 105, 30, 20, ST77XX_BLACK);
    tft.fillRect(2, 106, 28, 18, ST77XX_WHITE);
    tftPrintText(3, 111, ST77XX_BLACK, 1, "INFO");
  }
  if (selected == 2 & selectedBlink == 1)
  {
    tft.drawRect(31, 104, 38, 22, 33808);
    tft.fillRect(33, 106, 34, 18, 33808);
    tftPrintText(35, 111, ST77XX_WHITE, 1, "STATS");
  }
  else if (currMenu == 2)
  {
    tft.drawRect(31, 104, 38, 22, ST77XX_BLACK);
    tft.fillRect(33, 106, 34, 18, ST77XX_BLACK);
    tftPrintText(35, 111, ST77XX_WHITE, 1, "STATS");
  }
  else
  {
    tft.drawRect(31, 104, 38, 22, ST77XX_WHITE);
    tft.drawRect(32, 105, 36, 20, ST77XX_BLACK);
    tft.fillRect(33, 106, 34, 18, ST77XX_WHITE);
    tftPrintText(35, 111, ST77XX_BLACK, 1, "STATS");
  }
  if (selected == 3 & selectedBlink == 1)
  {
    tft.drawRect(69, 104, 32, 22, 33808);
    tft.fillRect(71, 106, 28, 18, 33808);
    tftPrintText(73, 111, ST77XX_WHITE, 1, "WIFI");    
  }
  else if (currMenu == 3)
  {
    tft.drawRect(69, 104, 32, 22, ST77XX_BLACK);
    tft.fillRect(71, 106, 28, 18, ST77XX_BLACK);
    tftPrintText(73, 111, ST77XX_WHITE, 1, "WIFI");    
  }
  else
  {
    tft.drawRect(69, 104, 32, 22, ST77XX_WHITE);
    tft.drawRect(70, 105, 30, 20, ST77XX_BLACK);
    tft.fillRect(71, 106, 28, 18, ST77XX_WHITE);
    tftPrintText(73, 111, ST77XX_BLACK, 1, "WIFI");
  }
  if (selected == 4 & selectedBlink == 1)
  {
    tft.drawRect(101, 104, 58, 22, 33808);
    tft.fillRect(103, 106, 54, 18, 33808);
    tftPrintText(105, 111, ST77XX_WHITE, 1, "SETTINGS");
  }
  else if (currMenu == 4)
  {
    tft.drawRect(101, 104, 58, 22, ST77XX_BLACK);
    tft.fillRect(103, 106, 54, 18, ST77XX_BLACK);
    tftPrintText(105, 111, ST77XX_WHITE, 1, "SETTINGS");
  }
  else
  {
    tft.drawRect(101, 104, 58, 22, ST77XX_WHITE);
    tft.drawRect(102, 105, 56, 20, ST77XX_BLACK);
    tft.fillRect(103, 106, 54, 18, ST77XX_WHITE);
    tftPrintText(105, 111, ST77XX_BLACK, 1, "SETTINGS");
  }
  
}

void tftSceneInfo(){
    
  tft.fillScreen(ST77XX_WHITE);

  tftPrintText(10, 10, ST77XX_BLACK, 1, "HANDLING");
  tftPrintText(18, 25, ST77XX_BLACK, 2, "BOX");
  tftPrintText(10, 60, ST77XX_BLACK, 3, box);
  tft.drawRect(2, 2, 64, 100, ST77XX_BLACK);

  tftPrintText(70, 5, ST77XX_BLACK, 1, "extrac");
  tft.drawRect(106, 3, 10, 10, ST77XX_BLACK);
  
  tftPrintText(120, 5, ST77XX_BLACK, 1, "retr");
  tft.drawRect(145, 3, 10, 10, ST77XX_BLACK);
  
  tftPrintText(70, 20, ST77XX_BLACK, 1, "x");
  tft.drawRect(80, 18, 10, 10, ST77XX_BLACK);
  
  tftPrintText(100, 20, ST77XX_BLACK, 1, "y");
  tft.drawRect(110, 18, 10, 10, ST77XX_BLACK);
  
  tftPrintText(135, 20, ST77XX_BLACK, 1, "z");
  tft.drawRect(145, 18, 10, 10, ST77XX_BLACK);
  
  tftPrintText(70, 35, ST77XX_BLACK, 1, "grab");
  tft.drawRect(100, 33, 10, 10, ST77XX_BLACK);
  
  tftPrintText(115, 35, ST77XX_BLACK, 1, "lift");
  tft.drawRect(145, 33, 10, 10, ST77XX_BLACK);
  
  tftPrintText(70, 50, ST77XX_BLACK, 1, "x");
  tft.drawRect(80, 48, 10, 10, ST77XX_BLACK);
  
  tftPrintText(100, 50, ST77XX_BLACK, 1, "y");
  tft.drawRect(110, 48, 10, 10, ST77XX_BLACK);
  
  tftPrintText(135, 50, ST77XX_BLACK, 1, "z");
  tft.drawRect(145, 48, 10, 10, ST77XX_BLACK);
  
  tftPrintText(70, 65, ST77XX_BLACK, 1, "drop");
  tft.drawRect(100, 63, 10, 10, ST77XX_BLACK);
  
  tftPrintText(115, 65, ST77XX_BLACK, 1, "pick");
  tft.drawRect(145, 63, 10, 10, ST77XX_BLACK);

  tftPrintText(70, 80, ST77XX_BLACK, 1, "x");
  tft.drawRect(80, 78, 10, 10, ST77XX_BLACK);
  
  tftPrintText(100, 80, ST77XX_BLACK, 1, "y");
  tft.drawRect(110, 78, 10, 10, ST77XX_BLACK);
  
  tftPrintText(135, 80, ST77XX_BLACK, 1, "z");
  tft.drawRect(145, 78, 10, 10, ST77XX_BLACK);
  
  tftPrintText(70, 95, ST77XX_BLACK, 1, "shelf");
  tft.drawRect(100, 93, 10, 10, ST77XX_BLACK);
  
  tftPrintText(115, 95, ST77XX_BLACK, 1, "reset");
  tft.drawRect(145, 93, 10, 10, ST77XX_BLACK);

//  tftTickBox(110, 20);
//
//  tftTickBox(110, 50);
//
//  tftTickBox(110, 80);
}

void tftSceneSettingsNetwork(){
  tft.fillScreen(ST77XX_WHITE);
  int num = 0;
  int stop = 0;
  while (stop = 0)
  {
    if (pwd[num] != "")
    {
      tftPrintText(3 + (num * 6), 18, ST77XX_BLACK, 1, pwd[num]);
    }
    else
    {
      stop = 1;
    }
    num++;
  }
  tftPrintText(3 + (num * 6), 18, ST77XX_BLACK, 1, characters[selectedChar]);
  magnifier(num, characters[selectedChar]);
}

void tftSceneStats(){
  tft.fillScreen(ST77XX_WHITE);
}

void tftSceneWifi(){
  tft.fillScreen(ST77XX_WHITE);
}

byte getkey(void){
  byte cnt;
  byte num = 0;
  for(cnt = 1; cnt < 17; cnt++)
  {
    digitalWrite(pulse, LOW); 
    if (digitalRead(data) == 0)
      num = cnt; 
    digitalWrite(pulse, HIGH);
  }  
  return num; 
}

void keyPressed(byte Key){
  if (Key!=0&Key!=oldKeyStroke)
  {
    Serial.print("PUSH--->");
    Serial.println(Key);
    if (Key == 5)
    {
      lastSelectedMenu = millis();
      selectedBlink = 0;
      if (selected > 1)
      {
        selected--;
      }
    }
    else if (Key == 6)
    {
      lastSelectedMenu = millis();
      selectedBlink = 0;
      sceneChanged = 1;
      currMenu = selected;
    }
    else if (Key == 7)
    {
      lastSelectedMenu = millis();
      selectedBlink = 0;
      if (selected < 4)
      {
        selected++;
      }
    }
    else if (Key == 2 & currMenu == 4)
    {
      lastSelectedMenu = millis();
      selectedBlink = 0;
      selectedChar++;
      sceneChanged = 1;
    }
     else if (Key == 10 & currMenu == 4)
    {
      lastSelectedMenu = millis();
      selectedBlink = 0;
      selectedChar--;
      sceneChanged = 1;
    }
  }
  oldKeyStroke = Key;
}

void checkScene(){
  
  if (currMenu == 1 & sceneChanged == 1)
  {
    tftSceneInfo();
    sceneChanged = 0;
  }
  if (currMenu == 2 & sceneChanged == 1)
  {
    tftSceneStats();
    sceneChanged = 0;
  }
  if (currMenu == 3 & sceneChanged == 1)
  {
    tftSceneWifi();
    sceneChanged = 0;
  }
  else if (currMenu == 4 & currSettings == 1 & sceneChanged == 1)
  {
    tftSceneSettingsNetwork();
    sceneChanged = 0;
  }
}

void blinkControl(){
  currentMillis = millis();
  if (currentMillis - lastSelectedMenu < 20000)
  {
    if (currentMillis - lastSelectedBlink > 500)
    {
      lastSelectedBlink = currentMillis;
      if (selectedBlink == 0)
      {
        selectedBlink = 1;
      }
      else
      {
        selectedBlink = 0;
      }
      menuChanged = 1;
    }
  }
}

void settingsMenu(){
  
  if (selected == 5 & selectedBlink == 1)
  {
    tft.drawRect(111, 80, 48, 22, 33808);
    tft.fillRect(113, 82, 44, 18, 33808);
    tftPrintText(115, 87, ST77XX_WHITE, 1, "Network");
  }
  else if (currSettings == 1)
  {
    tft.drawRect(111, 80, 48, 22, ST77XX_BLACK);
    tft.fillRect(113, 82, 44, 18, ST77XX_BLACK);
    tftPrintText(115, 87, ST77XX_WHITE, 1, "Network");
  }
  else
  {
    tft.drawRect(111, 80, 48, 22, ST77XX_WHITE);
    tft.drawRect(112, 81, 46, 20, ST77XX_BLACK);
    tft.fillRect(113, 82, 44, 18, ST77XX_WHITE);
    tftPrintText(115, 87, ST77XX_BLACK, 1, "Network");
  }
}

void magnifier(int num, char *character){
  tft.drawCircle(6+((num-1)*6), 22, 5, ST77XX_BLACK);
  tft.drawLine(3+((num-1)*6), 22, 30, 70, ST77XX_BLACK);
  tft.drawLine(9+((num-1)*6), 22, 70, 70, ST77XX_BLACK);
  tft.drawCircle(50, 70, 20, ST77XX_BLACK);
  tftPrintText(40, 54, ST77XX_BLACK, 4, character);
}

void serialEvent(){
  serialData = Serial.readStringUntil('-');
  newSerialData = 1;
  digitalWrite(POWER, HIGH);
  lastPoweredOn = millis();
  Serial.println("ser");
}

void sendData(){
  Serial.println("send");
  char hylla = serialData[3];
  char col = serialData[4];
  char row = serialData[5];
  
  if (hylla == '1')
  {
//    Serial.println("b");
//    Wire.beginTransmission(10);
//    Wire.write("b");
//    Wire.endTransmission();
//
    Serial.println("500");
    Wire.beginTransmission(12);
    Wire.write("100");
    Wire.endTransmission();
  }
  Wire.beginTransmission(11);
  Wire.write(col);
  Wire.endTransmission();
  delay(120000);
  delay(120000);
  if (hylla == '1')
  {
//    Serial.println("5");
//    Wire.beginTransmission(10);
//    Wire.write("5");
//    Wire.endTransmission();
    Serial.println("999");
    Wire.beginTransmission(12);
    Wire.write("999");
    Wire.endTransmission();
  }
  Wire.beginTransmission(11);
  Wire.write("0");
  Wire.endTransmission();
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
    sceneChanged = 1;
    workActive = 1;
    Serial.print(box);
  }

}
void workDo(){
  char info[3];
  if (x1 == 0)
  {
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'X';
    info[1] = box[1];
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    x1 = 1;
  }
  if (x1done == 1 && z1 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    info[1] = box[0];
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z1 = 1;
  }
  if (z1done == 1 && s1 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    if (box[0] == '1'){
      info[0] = 'A';
    }
    if (box[0] == '2'){
      info[0] = 'B';
    }
    if (box[0] == '3'){
      info[0] = 'C';
    }
    if (box[0] == '4'){
      info[0] = 'D';
    }
    info[1] = box[2];
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    s1 = 1;
  }
  if (s1done == 1 && z2 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    info[1] = 'N';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z2 = 1;
  }
  if (z2done == 1 && g1 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'G';
    info[1] = 'G';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    g1 = 1;
  }
  if (g1done == 1 && z3 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    info[1] = 'L';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z3 = 1;
  }
  if (z3done == 1 && s2 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    if (box[0] == '1'){
      info[0] = 'A';
    }
    if (box[0] == '2'){
      info[0] = 'B';
    }
    if (box[0] == '3'){
      info[0] = 'C';
    }
    if (box[0] == '4'){
      info[0] = 'D';
    }
    info[1] = 'R';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    s2 = 1;
  }
  if (s2done == 1 && x2 == 0)
  {
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'X';
    info[1] = 'S';
    info[2] = '\0';
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    x2 = 1;
  }
  if (x2done == 1 && z4 == 0)
  {
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    info[1] = 'S';
    info[2] = '\0';
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z4 = 1;
  }
  if (z4done == 1 && g2 == 0)
  {
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'G';
    info[1] = 'R';
    info[2] = '\0';
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    g2 = 1;
  }
  if (g2done == 1 && g3 == 0)
  {
    while (digitalRead(returnSwitchPin)){
      
    }
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'G';
    info[1] = 'R';
    info[2] = '\0';
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    g3 = 1;
  }
  if (g3done == 1 && g4 == 0)
  {
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'G';
    info[1] = 'G';
    info[2] = '\0';
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    g4 = 1;
  }
  if (g4done == 1 && z5 == 0)
  {
    Serial.print("x3done");
    Serial.print(x3done);
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    if (box[0] == '1'){
      info[1] = 'a';
    }
    if (box[0] == '2'){
      info[1] = 'b';
    }
    if (box[0] == '3'){
      info[1] = 'c';
    }
    if (box[0] == '4'){
      info[1] = 'd';
    }
    info[2] = '\0';
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z5 = 1;
  }
  if (z5done == 1 && x3 == 0)
  {
    Serial.print("x3done");
    Serial.print(x3done);
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'X';
    if (box[1] == 'A'){
      info[1] = '1';
    }
    if (box[1] == 'B'){
      info[1] = '2';
    }
    if (box[1] == 'C'){
      info[1] = '3';
    }
    info[2] = '\0';
    Serial.print(info);
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    x3 = 1;
  }
  if (x3done == 1 && s3 == 0){
    Serial.print("x3done");
    Serial.print(x3done);
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    if (box[0] == '1'){
      info[0] = 'A';
    }
    if (box[0] == '2'){
      info[0] = 'B';
    }
    if (box[0] == '3'){
      info[0] = 'C';
    }
    if (box[0] == '4'){
      info[0] = 'D';
    }
    info[1] = box[2];
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    s3 = 1;
  }
  if (s3done == 1 && z6 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    info[1] = 'M';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z6 = 1;
  }
  if (z6done == 1 && g5 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'G';
    info[1] = 'R';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    g5 = 1;
  }
  if (g5done == 1 && z7 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    info[1] = 'O';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z7 = 1;
  }
  if (z7done == 1 && s4 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    if (box[0] == '1'){
      info[0] = 'A';
    }
    if (box[0] == '2'){
      info[0] = 'B';
    }
    if (box[0] == '3'){
      info[0] = 'C';
    }
    if (box[0] == '4'){
      info[0] = 'D';
    }
    info[1] = 'R';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    s4 = 1;
  }
  if (s4done == 1 && z8 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'Z';
    info[1] = 'R';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(11);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    z8 = 1;
  }
  if (z8done == 1 && x4 == 0){
    digitalWrite(POWER, HIGH);
    lastPoweredOn = millis();
    info[0] = 'X';
    info[1] = 'R';
    info[2] = '\0';
    Serial.println(info);
    Wire.beginTransmission(10);
    Wire.write(info);
    Wire.endTransmission();
    Wire.end();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    x4 = 1;
  }
}
void workDoing(){
  if (x1 == 1 && i2cResponse == '1' && x1done == 0){
    x1done = 1;
    i2cResponse = '0';
  }
  if (z1 == 1 && i2cResponse == '1' && z1done == 0){
    z1done = 1;
    i2cResponse = '0';
    Serial.print("z1done");
  }
  if (s1 == 1 && i2cResponse == '1' && s1done == 0){
    s1done = 1;
    i2cResponse = '0';
    Serial.print("s1done");
  }
  if (g1 == 1 && i2cResponse == '1' && g1done == 0){
    g1done = 1;
    i2cResponse = '0';
    Serial.print("g1done");
  }
  if (z2 == 1 && i2cResponse == '1' && z2done == 0){
    z2done = 1;
    i2cResponse = '0';
    Serial.print("z2done");
  }
  if (s2 == 1 && i2cResponse == '1' && s2done == 0){
    s2done = 1;
    i2cResponse = '0';
    Serial.print("s2done");
  }
  if (x2 == 1 && i2cResponse == '1' && x2done == 0){
    x2done = 1;
    i2cResponse = '0';
  }
  if (z3 == 1 && i2cResponse == '1' && z3done == 0){
    z3done = 1;
    i2cResponse = '0';
  }
  if (g2 == 1 && i2cResponse == '1' && g2done == 0){
    g2done = 1;
    i2cResponse = '0';
  }
  if (g3 == 1 && i2cResponse == '1' && g3done == 0){
    g3done = 1;
    i2cResponse = '0';
  }
  if (g4 == 1 && i2cResponse == '1' && g4done == 0){
    g4done = 1;
    i2cResponse = '0';
  }
  if (z4 == 1 && i2cResponse == '1' && z4done == 0){
    z4done = 1;
    i2cResponse = '0';
  }
  if (x3 == 1 && i2cResponse == '1' && x3done == 0){
    x3done = 1;
    i2cResponse = '0';
  }
  if (s3 == 1 && i2cResponse == '1' && s3done == 0){
    s3done = 1;
    i2cResponse = '0';
  }
  if (z5 == 1 && i2cResponse == '1' && z5done == 0){
    z5done = 1;
    i2cResponse = '0';
  }
  if (g5 == 1 && i2cResponse == '1' && g5done == 0){
    g5done = 1;
    i2cResponse = '0';
  }
  if (s4 == 1 && i2cResponse == '1' && s4done == 0){
    s4done = 1;
    i2cResponse = '0';
  }
  if (z6 == 1 && i2cResponse == '1' && z6done == 0){
    z6done = 1;
    i2cResponse = '0';
  }
  if (x4 == 1 && i2cResponse == '1' && x4done == 0){
    x4done = 1;
    i2cResponse = '0';
  }
  if (z7 == 1 && i2cResponse == '1' && z7done == 0){
    z7done = 1;
    i2cResponse = '0';
  }
  if (z8 == 1 && i2cResponse == '1' && z8done == 0){
    z8done = 1;
    i2cResponse = '0';
  }
}
void workDone(){
  if (x1done == 1){
    tftTickBox(80, 20);
  }
  
  if (z1done == 1){
    tftTickBox(145, 20);
  }
  
  if (s1done == 1){
    tftTickBox(106, 5);
  }
  
  if (g1done == 1){
    tftTickBox(100, 35);
  }
  
  if (z2done == 1){
    tftTickBox(145, 35);
  }
  if (s2done == 1){
    tftTickBox(145, 5);
  }
  
  if (x2done == 1){
    tftTickBox(80, 50);
  }
  
  if (z3done == 1){
    tftTickBox(145, 50);
  }
  
  if (g2done == 1){
    tftTickBox(100, 65);
  }
  
  if (g4done == 1){
    tftTickBox(145, 65);
  }
  
  if (z4done == 1){
    tftTickBox(145, 80);
  }
  
  if (x3done == 1){
    tftTickBox(80, 80);
  }
  
  if (s3done == 1){
    tftTickBox(100, 95);
  }
  
  if (x4done == 1){
    tftTickBox(145, 95);
  }
}
void receiveEvent(){
  while (0 < Wire.available())
  {
    i2cResponse = Wire.read();
  }
  Serial.print(i2cResponse);
}
