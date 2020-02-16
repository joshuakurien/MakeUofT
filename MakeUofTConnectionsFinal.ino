const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

const int buzzerPin = 13;

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
Servo servo;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

uint32_t chipId = ESP.getChipId();

boolean serialPortDebug = true;

typedef struct {
  char* ssid;
  char* password;
} wifiCreds_t;

const char* ssid = "OnePlus 6T";
const char* password =  "HELLO1234";
const char* mqttServer = "mr2j0vvhki1l0v.messaging.solace.cloud";
const int mqttPort = 20358;
const char* mqttUser = "solace-cloud-client";
const char* mqttPassword = "h9opao4l88b69t3nvqo2svv0b0";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMqttReconnectAttempt = 0;

unsigned long loopCounter = 0;

void setup() {
  randomSeed(analogRead(0));

  Serial.begin(115200);
  Serial.println("Booting");
  servo.attach(0);
  servo.write(0);
  pinMode(2, OUTPUT); //red
  pinMode(14,OUTPUT); //blue
  pinMode(12,OUTPUT);// green
  pinMode(15,OUTPUT); // motor
  pinMode(buzzerPin, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000); 
  ///
  ///Setup Pins
  ///

  Serial.println("Connecting to wifi");
  
  WiFi.mode(WIFI_STA);

  WiFi.setAutoConnect (true);
  WiFi.setAutoReconnect (true);
 
  WiFi.begin(ssid, password); 

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("Connected to Wifi");
  Serial.println(mqttServer);
  Serial.println(mqttPort);
  Serial.println(mqttUser);
  Serial.println(mqttPassword);
  
  client.setServer(mqttServer, mqttPort);
}

void testWrite(String toSend, int delaY) {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println((toSend));
  display.display();
  delay(delaY);
}

void drive(int delaY){
  digitalWrite(15,HIGH);
  delay(delaY);
  digitalWrite(15,LOW);
}


void turnOnFan(int delaY){
  testWrite("Fan Turned On",1000);
  drive(delaY);
  delay(50);
}

void openDoor(){
  testWrite("Open Door",1000);
  servo.write(90);
  delay(50);
}

void closeDoor(){
  testWrite("Closing Door",1000);
  servo.write(0);
  delay(50);
}

void lightDisplay(){
  testWrite("Light Show", 1000);
  for(int i = 0; i < 9; i++){
    multiLED(i);
    delay(1000); 
  }
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note);
  delay(duration);
  noTone(buzzerPin);
  delay(50);
}

void firstSection()
{
  testWrite("Playing Starwars", 1000);
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}

void multiLED(int number){
  if(number < 1){
   analogWrite(2,255);
   analogWrite(14,0);
   analogWrite(12,0);
  }
  else if(number == 2){
    analogWrite(2,255);
    analogWrite(14,208);
    analogWrite(12,96);
  }
  else if(number == 3){
    analogWrite(2,160);
    analogWrite(14,255);
    analogWrite(12,32);
  }
  else if(number == 4){
    analogWrite(2,80);
    analogWrite(14,255);
    analogWrite(12,208);
  }
  else if(number == 5){
    analogWrite(2,0);
    analogWrite(14,255);
    analogWrite(12,32);
  }
  else if(number == 6){
    analogWrite(2,96);
    analogWrite(14,128);
    analogWrite(12,255);
  }
  else if (number == 7)
  {
    analogWrite(2,0);
    analogWrite(14,0);
    analogWrite(12,192);
  }
  else if (number == 8)
  {
    analogWrite(2,255);
    analogWrite(14,32);
    analogWrite(12,224);
  }
  else
  {
    analogWrite(2,255);
    analogWrite(14,16);
    analogWrite(12,160);
  }
}

void alarmMode(){
  testWrite("ALARM ACTIVATED", 1000);
  for(int i = 0; i < 15; i++)
  {
    if(i % 2 == 0){
      multiLED(1);
      beep(125,200);
    }
    else{
      multiLED(5);
      beep(250,200);
    }
    delay(200);
   }
}

void partyMode(){
  testWrite("PARTY MODE",1000);
  for(int j = 0; j < 5; j++){
       testWrite("MODE PARTY",100);
       servo.write(90);
       delay(50);
       drive(350);
       delay(50);
       servo.write(90);
       testWrite("PARTY MODE",100);
      for(int i = 0; i < 9; i++){
        multiLED(i);
        delay(100); 
    }
  }
}

void testTransistion(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.drawCircle(display.width()/2, display.height()/2, i+2, SSD1306_WHITE);
    display.drawCircle(display.width()/2, display.height()/2, i+4, SSD1306_WHITE);

    display.display();
    delay(1);
    display.clearDisplay();
  }
}

boolean mqttReconnect() {
  char mqttClientId[40];
  sprintf(mqttClientId, "ESP8266Client%lu", chipId);
  Serial.println(mqttClientId);
  Serial.println("Connecting to MQTT...");
  
  if (client.connect(mqttClientId, mqttUser, mqttPassword, NULL, NULL, NULL, NULL, false )) {
    Serial.println("connected");
    client.setCallback(callback);
  
    client.subscribe("light");
    client.subscribe("fan");
    client.subscribe("music");
    client.subscribe("openDoor");
    client.subscribe("closeDoor");
    client.subscribe("alarm");
    client.subscribe("party");
    
    
  } else {
    Serial.println(" not connected");
  }
  return client.connected();
}

//MQTT Callback
void callback(char* topic, byte* payload, unsigned int length) {
  
  // Make a copy of the payload
  byte message[length + 10];
  memcpy(message, payload, length);
  message[length] = '\0';

  // Make a copy of the topic
  char t[sizeof(topic) * 4];
  strncpy(t, topic, sizeof(topic) * 4);

  String topicString(t);
  if (topicString.equals("light")) {
    lightDisplay();
  }
  else if (topicString.equals("fan")) {
    turnOnFan(1000);
  }
  else if (topicString.equals("music")) {
    firstSection();
  }
  else if (topicString.equals("openDoor")) {
    openDoor();
  }
  else if (topicString.equals("closeDoor")) {
    closeDoor();
  }
  else if (topicString.equals("alarm")) {
    alarmMode();
  }
  else if (topicString.equals("party")) {
    partyMode();
  }
  else {
  }
}

boolean mqtConnected = false;

// the loop function runs over and over again forever
void loop() {
   if (!mqtConnected) {
    mqttReconnect();
    mqtConnected = true;
   }
   client.loop();
   
   if ((loopCounter % 100000 == 0) && !client.connected()) {
    long now = millis();
    if (now - lastMqttReconnectAttempt > 5000) {
      lastMqttReconnectAttempt = now;
      // Attempt to reconnect
      if (mqttReconnect()) {
        lastMqttReconnectAttempt = 0;
      }
    }
  }

  if (loopCounter >= 1000) {
    loopCounter = 0;
  }

}
