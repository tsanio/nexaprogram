#include <Nexa.h>

/*
  Demo program to control nexa sockets using serial commands.

  To communicate.
    Separate commands with #-sign ie. to test communcication send "hello#"

  To introduce new remote socket. (re-plug nexa socket after this immediately)
    introduce(dev) (dev being number 0..n)

  To set state of device
    stateOn(dev)
    stateOff(dev)

  To clear completely memory of receiver. (re-plug nexa socket after this immediately)
    clear(dev)

  To switch state of all devices.
    allOn()
    allOff()

   @author Sanio 2016
*/


Nexa nexa;


void setup() {                
  
  // Connect to correct pin in Arduino for broadcasting singal.
  nexa.setRadioPin(2);    
    
  // Set 10 seconds broadcasting for time consuming processes ie. connecting new device or clearing devices.
  nexa.setLongConfigurationWaitTimeSeconds(10); 
  
  Serial.begin(9600);
  Serial.print("Nexa controller is awake!");
  
}


// Main loop.
void loop() {
   
  // Read from serial..
  String msg = getMessage();
  
  if (!msg.equals("NA")) {
    
    handleIncomeMessage(msg);
    
  }
  
  // Just a bit delay.
  delay(1000);    

}


void handleIncomeMessage(String msg) {
  
    Serial.println("Received: " + msg);
    
    // # Hello test
    if (msg.indexOf("hello") >= 0) {    
          Serial.println("Hey! How can I help you? ^__^");
    }
    
    // # Introduce new device
    if (msg.indexOf("introduce") >= 0) {
      String devIStr = msg.substring(msg.indexOf("(")+1,msg.indexOf(")"));
      int dev = getSafeNumeric(devIStr);
      introcudeReceiver(dev);      
    }
    
    // # Set state of device
    if (msg.indexOf("state") >= 0) {
        String devIStr = msg.substring(msg.indexOf("(")+1,msg.indexOf(")"));
        int dev = getSafeNumeric(devIStr);
        if (msg.indexOf("On") >= 0)
          switchReceiverState(dev, true);
        if (msg.indexOf("Off") >= 0)
          switchReceiverState(dev, false);
    }

    // # Clear socket's memory of receivers
    if (msg.indexOf("clear") >= 0) {
      String devIStr = msg.substring(msg.indexOf("(")+1,msg.indexOf(")"));
        int dev = getSafeNumeric(devIStr);
        clearReceiver(dev);
    }

    // # Global switch on/off    
    if (msg.indexOf("all") >= 0) {
        if (msg.indexOf("On") >= 0)
          switchGlobalState(true);
        if (msg.indexOf("Off") >= 0)
          switchGlobalState(false);
    }
}


void introcudeReceiver(int dev) {
  nexa.introduceReceiver(dev);
  Serial.println("New receiver ("+String(dev)+") introduced."); 
}


void switchGlobalState(boolean state) {  
  if (state)
    Serial.println("Setting global state on."); 
  else
    Serial.println("Setting global state off.");     
  nexa.setGlobalState(state);
}


void switchReceiverState(int dev, boolean state) {  
  if (state)
    Serial.println("Setting receiver " + String(dev) + " on."); 
  else
    Serial.println("Setting receiver " + String(dev) + " off.");     
  nexa.setReceiverState(dev, state);
}


void clearReceiver(int dev) {
  nexa.clearReceiverCompletely(dev);
  Serial.println("Receiver ("+String(dev)+") cleared completely."); 
}



/** Additional functions */

// Get message from Serial.
String getMessage()
{
  String msg=""; //the message starts empty
  byte ch; // the character that you use to construct the Message 
  byte d='#';// the separating symbol 
  if(Serial.available())// checks if there is a new message;
  {
    while(Serial.available() && Serial.peek()!=d)// while the message did not finish
    {
      ch=Serial.read();// get the character
      msg+=(char)ch;//add the character to the message
      delay(1);//wait for the next character
    }
  ch=Serial.read();// pop the '#' from the buffer
  if(ch==d) // id finished
    return msg;
  else
    return "NA";
  }
  else
    return "NA"; // return "NA" if no message;
}

// TODO
int getSafeNumeric(String str) {  
  return str.toInt();
}

