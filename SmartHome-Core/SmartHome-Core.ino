/*******************************************/
/* SmartHome Core                          */
/* Version 0.1                             */
/* by wi3                                  */
/* Arduino IDE 1.6.11                      */
/*******************************************/


/*Variablen definieren */
byte mqttsrv[] = {192, 168, 88, 101}; // MQTT Server IP Address
byte mac[] = {0xDE, 0xED, 0xBA, 0xAA, 0xFE, 0xED}; 
char macString[13];
char ipString[16];
String clientName;

char buff[43]; //Universal Buffer 

/******************************************/
/*           MQTT Topic Naming            */
/******************************************/
//MQTT Topic zusammen setzung z.B.:
// core01/digital01/


#define rootvec "core01"

//Funktions Vektor
#define commandvec "command"
#define infovec "info"      
#define setupvec "megasetup"     

//EndPunkt Vektoren
#define digital01vec "digital01" //Digitale Output Pins
#define digital02vec "digital02"
#define digital03vec "digital03"
#define digital04vec "digital04"
#define digital05vec "digital05"
#define digital06vec "digital06"
#define digital07vec "digital07"
#define digital08vec "digital08"


/*Include Libaries */
#include <ClickButton.h>            //https://code.google.com/archive/p/clickbutton/downloads
#include <Ethernet.h>
#include <PubSubClient.h>

/* Pin's */
#define btn1Pin         54  // illuminated tact switch - Switch part
#define btn2Pin         55

#define digitalOut01    22
#define digitalOut02    24
#define digitalOut03    26
#define digitalOut04    28
#define digitalOut05    30
#define digitalOut06    32
#define digitalOut07    34
#define digitalOut08    36

// Delay between requesting each setting / value over MQTT to allow OH to process
// On this front, Arduino is really reliable with fast processing of MQTT, OpenHAB is not so!
#define mqttsettingsreqdelay 400
#define mqttvalreqdelay 400

/******************************************/
/*       Define library functions         */
/******************************************/
//Clickbutton
ClickButton btn1(btn1Pin, LOW, CLICKBTN_PULLUP);
ClickButton btn2(btn2Pin, LOW, CLICKBTN_PULLUP);




//Ethernet
EthernetClient ethClient;
//PubSub
PubSubClient client(mqttsrv, 1883,ethClient);


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

 // Click library
btn1.debounceTime    = 20;
btn1.multiclickTime  = 0;
btn1.longClickTime = 1000;
btn2.debounceTime    = 20;
btn2.multiclickTime  = 0;
btn2.longClickTime = 1000;

//  Ethernet.begin(mac, ip);
Ethernet.begin(mac);

delay(150); // this was 720 and stable - before then it was 150 and fine!
client.connect("arduinoClient");
delay(50); // change back to 50

// MQTT Subscribe
sprintf (buff, "%s/%s/#", rootvec, setupvec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, infovec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital01vec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital02vec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital03vec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital04vec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital05vec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital06vec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital07vec);
client.subscribe(buff); // subscribing
sprintf (buff, "%s/%s/#", rootvec, digital08vec);
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
  Serial.print("Message arrived: topic: " + String(topic) + " - Payload: " );
  // Controller Setup / paging functionality

   for (i = 0; i < length; i++) {
    msg[i] = payload[i];
  }
  msg[i] = '\0';
  String msgString = String(msg);
  Serial.println(msgString);
  
  //Filter Messages & Execute Commands

  //Digital Output Port 1
  sprintf (buff, "%s/%s/%s", rootvec, digital01vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut01, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital01vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut01, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital01vec, state);
    client.publish(buff2, "off");
  }
  //Digital Output Port 2
  sprintf (buff, "%s/%s/%s", rootvec, digital02vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut02, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital02vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut02, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital02vec, state);
    client.publish(buff2, "off");
  }
   //Digital Output Port 3
  sprintf (buff, "%s/%s/%s", rootvec, digital03vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut03, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital03vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut03, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital03vec, state);
    client.publish(buff2, "off");
  }
   //Digital Output Port 4
  sprintf (buff, "%s/%s/%s", rootvec, digital04vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut04, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital04vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut04, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital04vec, state);
    client.publish(buff2, "off");
  }
     //Digital Output Port 5
  sprintf (buff, "%s/%s/%s", rootvec, digital05vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut05, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital05vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut05, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital05vec, state);
    client.publish(buff2, "off");
  }
     //Digital Output Port 6
  sprintf (buff, "%s/%s/%s", rootvec, digital06vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut06, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital06vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut06, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital06vec, state);
    client.publish(buff2, "off");
  }
     //Digital Output Port 7
  sprintf (buff, "%s/%s/%s", rootvec, digital07vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut07, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital07vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut07, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital07vec, state);
    client.publish(buff2, "off");
  }
     //Digital Output Port 8
  sprintf (buff, "%s/%s/%s", rootvec, digital08vec, set);
  if (String(topic) == buff && msgString == "on") {
    digitalWrite(digitalOut08, HIGH);
    sprintf(buff2, "%s/%s/%s", rootvec, digital08vec, state);
    client.publish(buff2, "on");
  }
   else if (String(topic) == buff && msgString == "off") {
    digitalWrite(digitalOut08, LOW);
    sprintf(buff2, "%s/%s/%s", rootvec, digital08vec, state);
    client.publish(buff2, "off");
  }


}

/******************************************/
/*       MQTT Reconnect                   */
/******************************************/

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    //Serial.println("MQTT Broker");
    //Serial.print(mqttsrv);
    // Generate client name based on MAC address
    String clientName;
    clientName += rootvec;
    clientName += macString;



    if (client.connect((char*) clientName.c_str())) {
      Serial.println("connected with client name ");
      Serial.println(clientName);
      client.publish("DB_Fenster_8266/boot", "reconnect");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 5 Sekunden warten bis zu einen erneuten Verbinden
      delay(5000);
    }
  }
}


