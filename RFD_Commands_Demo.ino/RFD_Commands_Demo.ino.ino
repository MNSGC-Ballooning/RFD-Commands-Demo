#include <SoftwareSerial.h>
#include <ctype.h>

const uint8_t led = 13;

SoftwareSerial xBee(2, 3); // TX:3, RX:2

// Sends the argument string to the xbee with an "!" and a new line character appended to the end
void sendToXbee(String msg){
  if(msg != "LED\n"){
    xBee.print("LED;"+msg + "!" + "\n");
  }
  else{
    xBee.print(msg);
  }
  Serial.println("Sent " + msg + " to the xBee");
}

void serialFlush(){
  while(xBee.available() > 0) {
    char t = xBee.read();
  }
}  

void flashLED(int count){
  sendToXbee("Blinking "+(String)count + " times");
  for(int i = 0;i < count;i++){
    serialFlush();
    Serial.println(i);
    digitalWrite(led,HIGH);
    delay(500);
    digitalWrite(led,LOW);
    delay(500);
    serialFlush();
  }
  sendToXbee("Done blinking");
}

// Checks for and responds to any commands received via XBee //
void xBeeCommand() {
  if (xBee.available()) {

    //  Grab transmission and parse command  //
    int len, num;
    String transmission, id, command;
    transmission = xBee.readStringUntil('!');
    len = transmission.length();
    num = transmission.indexOf('?');
    id = transmission.substring(0, num);
    command = transmission.substring(num + 1, len);
    Serial.println(transmission);

    //  Verify id and perform valid command //
    if (id == "LED") {
      Serial.println("Command acknowledged");
      sendToXbee("LED\n");                                  //  Command acknowledged
      flashLED(command.toInt());
      }
    }
}

void setup () {
pinMode(led, OUTPUT); // initialize LED for testing

  //  Initialize Serials  //
  Serial.begin(9600);
  while (!Serial){;}      //  wait for serial port to connect
  xBee.begin(9600);
}

void loop (){
  xBeeCommand();                              //  respond to XBee commands if any
  serialFlush();
}

