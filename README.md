# SmartHome-Core :house_with_garden:

Is an Arduino based MQTT Smarthome controller.
I use it to communicate with OpenHAB.

Digital inputs and outputs are basics in an smarthome environment so it makes sense to put it on an 
standalone microcontroller.

Hardware is an Arduino Due with Ethernet-Shield
it also works with a Mega, UNO, etc...


**Overview:**
- Communicates over Ethernet (Arduino Ethernet Shield)
  - uses DHCP (I recommend a DHCP reservation)
- MQTT is the communication protocol
  - sends information on start-up and reconnect to broker
  - sends information about digital input (single click, multi-clicks or long click)
  - receive commands to turn digital PIN's high or low
  - reconnects automatically
 
 
**What you have to do:**
- Set MQTT Broker IP
- Set unique MAC address


**What you can do:**
- reduce the code with some cool functions :stuck_out_tongue_winking_eye:
