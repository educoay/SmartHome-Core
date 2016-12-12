/*******************************************/
/* SmartHome Core                          */
/* Version 1.0                             */
/* by wi3                                  */
/* Arduino IDE 1.6.12                      */
/*******************************************/


/*Variablen definieren */
byte mqttsrv[] = {192, 168, 0, 10}; // MQTT Server IP Address
byte mac[] = {0xDE, 0xED, 0xBA, 0xAA, 0xFE, 0xED};
char macString[13];
char ipString[16];
String clientName;

char buff[43]; //Universal Buffer

/******************************************/
/*           MQTT Topic Naming            */
/******************************************/
//MQTT Topic zusammen setzung z.B.:
// core01/dIN01/clicks


#define rootvec "core01"

//Funktions Vektor
#define commandvec "command"
#define infovec "info"
#define setupvec "megasetup"

//EndPunkt Vektoren
#define dOUT01vec "dOUT01"  //Digitale Output Pins
#define dOUT02vec "dOUT02"
#define dOUT03vec "dOUT03"
#define dOUT04vec "dOUT04"
#define dOUT05vec "dOUT05"
#define dOUT06vec "dOUT06"
#define dOUT07vec "dOUT07"
#define dOUT08vec "dOUT08"

#define dIN01vec "dIN01"    //Digital Input Pins
#define dIN02vec "dIN02"
#define dIN03vec "dIN03"
#define dIN04vec "dIN04"
#define dIN05vec "dIN05"
#define dIN06vec "dIN06"
#define dIN07vec "dIN07"
#define dIN08vec "dIN08"


/*Include Libaries */
#include <ClickButton.h>            //https://code.google.com/archive/p/clickbutton/downloads
#include <Ethernet.h>
#include <PubSubClient.h>           //https://github.com/knolleary/pubsubclient

/* Pin's */
#define btn1Pin         3  // Switch Pin's
#define btn2Pin         5
#define btn3Pin         6
#define btn4Pin         25 
#define btn5Pin         26
#define btn6Pin         27
#define btn7Pin         28
#define btn8Pin         6

//DUE/Mega
/*
#define digitalOut01    22
#define digitalOut02    24
#define digitalOut03    26
#define digitalOut04    28
#define digitalOut05    30
#define digitalOut06    32
#define digitalOut07    34
#define digitalOut08    36
*/

//UNO

#define digitalOut01    7
#define digitalOut02    8
#define digitalOut03    9
#define digitalOut04    2
#define digitalOut05    16
#define digitalOut06    17
#define digitalOut07    18
#define digitalOut08    19


// Do not use PIN D4 A0 A1 D13 D12 D11 D10 (Ethernet Shield)
// Mega 50 51 52 53

/******************************************/
/*       Define library functions         */
/******************************************/
//Clickbutton
ClickButton btn1(btn1Pin, LOW, CLICKBTN_PULLUP);
ClickButton btn2(btn2Pin, LOW, CLICKBTN_PULLUP);
ClickButton btn3(btn3Pin, LOW, CLICKBTN_PULLUP);

//Ethernet
EthernetClient ethClient;
//PubSub
PubSubClient client(mqttsrv, 1883, ethClient);


/******************************************/
/*               SETUP                    */
/******************************************/
void setup() {
  Serial.begin(9600);
  Serial.println("Starting.....");
  client.setCallback(callback);

  // MAC string used for MQTT setup
  sprintf (macString, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  //Output's
  pinMode(digitalOut01, OUTPUT);     //Output PIN 1
  pinMode(digitalOut02, OUTPUT);     //Output PIN 2
  pinMode(digitalOut03, OUTPUT);     //Output PIN 3
  pinMode(digitalOut04, OUTPUT);     //Output PIN 4
  pinMode(digitalOut05, OUTPUT);     //Output PIN 5
  pinMode(digitalOut06, OUTPUT);     //Output PIN 6
  pinMode(digitalOut07, OUTPUT);     //Output PIN 7
  pinMode(digitalOut08, OUTPUT);     //Output PIN 8



  // ButtonClick library
  btn1.debounceTime    = 20;
  btn1.multiclickTime  = 250;
  btn1.longClickTime = 1000;
  btn2.debounceTime    = 20;
  btn2.multiclickTime  = 250;
  btn2.longClickTime = 1000;
  btn3.debounceTime    = 20;
  btn3.multiclickTime  = 250;
  btn3.longClickTime = 1000;

  Ethernet.begin(mac);

  delay(150);   
  reconnect(); //connect to MQTT Broker
  delay(50);  


  // MQTT Subscribe
  sprintf (buff, "%s/%s/#", rootvec, setupvec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, infovec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT01vec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT02vec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT03vec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT04vec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT05vec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT06vec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT07vec);
  client.subscribe(buff); // subscribing
  sprintf (buff, "%s/%s/#", rootvec, dOUT08vec);
  client.subscribe(buff); // subscribing

  sprintf (buff, "%s/%s", rootvec, infovec);
  client.publish(buff, "up_and_running");
  Serial.println("Start completed");
}

/******************************************/
/*       LOOOOP                           */
/******************************************/

void loop() {
  if (!client.connected()) { //Check MQTT Connection
    reconnect();
  }

  client.loop();

  /*       Respond to button inputs         */
  btn1.Update();
  btn2.Update();
  btn3.Update();
  if (btn1.clicks != 0) btnFunc("btn1", (char*)btn1.clicks); 
  if (btn2.clicks != 0) btnFunc("btn2", (char*)btn2.clicks);
  if (btn3.clicks != 0) btnFunc("btn3", (char*)btn3.clicks);
}


/******************************************/
/*       Button Functions                 */
/******************************************/

void btnFunc(char* btnName, char* clickcount) {
char clicks[10] = "clicks";

  if (btnName == "btn1"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN01vec, clicks);
     
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
  }
  else if (btnName == "btn2"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN02vec, clicks);
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
  }
  else if (btnName == "btn3"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN03vec, clicks);
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
  }
  else if (btnName == "btn4"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN04vec, clicks);
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
  }
  else if (btnName == "btn5"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN05vec, clicks);
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
  }
    else if (btnName == "btn6"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN06vec, clicks);
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
  }
    else if (btnName == "btn7"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN08vec, clicks);
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
    }
       else if (btnName == "btn8"){
     sprintf (buff, "%s/%s/%s", rootvec, dIN08vec, clicks);
     if (clickcount == 1){
      client.publish(buff, "1");
     }
     else if (clickcount == 2){
      client.publish(buff, "2");
     }
     else if (clickcount == 3){
      client.publish(buff, "3");
     }
     else if (clickcount == 4){
      client.publish(buff, "4");
     }
     else if (clickcount == -1){
      client.publish(buff, "L");
     }
  }
}

/******************************************/
/*       MQTT Callback                    */
/******************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  char msg[100];
  char set[10] = "set";
  char state[10] = "state";
  char buff2[43];
  payload[length] = '\0';
  Serial.print("Message arrived: Topic: " + String(topic) + " - Payload: " );
  // Controller Setup / paging functionality

  for (i = 0; i < length; i++) {
    msg[i] = payload[i];
  }
  msg[i] = '\0';
  String msgString = String(msg);
  Serial.println(msgString);

  //Filter Messages & Execute Commands

  //Digital Output Port 1
  sprintf (buff, "%s/%s/%s", rootvec, dOUT01vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut01, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT01vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut01, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT01vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 2
  sprintf (buff, "%s/%s/%s", rootvec, dOUT02vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut02, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT02vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut02, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT02vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 3
  sprintf (buff, "%s/%s/%s", rootvec, dOUT03vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut03, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT03vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut03, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT03vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 4
  sprintf (buff, "%s/%s/%s", rootvec, dOUT04vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut04, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT04vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut04, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT04vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 5
  sprintf (buff, "%s/%s/%s", rootvec, dOUT05vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut05, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT05vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut05, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT05vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 6
  sprintf (buff, "%s/%s/%s", rootvec, dOUT06vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut06, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT06vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut06, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT06vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 7
  sprintf (buff, "%s/%s/%s", rootvec, dOUT07vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut07, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT07vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut07, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT07vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 8
  sprintf (buff, "%s/%s/%s", rootvec, dOUT08vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut08, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT08vec, state);
    client.publish(buff2, "on");
  }
  else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut08, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, dOUT08vec, state);
    client.publish(buff2, "off");
  }


}

/******************************************/
/*       MQTT Reconnect                   */
/******************************************/
void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Generate client name based on MAC address
    String clientName;
    clientName += rootvec;
    clientName += macString;

    if (client.connect((char*) clientName.c_str())) {
      Serial.print("Connected with Client-Name: ");
      Serial.println(clientName);
      sprintf (buff, "%s/%s/%s", rootvec, infovec, "mqtt");
      client.publish(buff, "reconnect");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 5 Sekunden warten bis zu einen erneuten Verbinden
      delay(5000);
    }
  }
}
