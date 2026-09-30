#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define rxPin 20
#define txPin 21

EspSoftwareSerial::UART FPSerial;
DFRobotDFPlayerMini myDFPlayer;

void setup(){
    Serial.begin(9600);
    FPSerial.begin(9600,SWSERIAL_8N1,rxPin,txPin,false);
    delay(10000);

    if (!myDFPlayer.begin(FPSerial, true,true)) {  //Use serial to communicate with mp3.
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card or USB drive!"));
        while(true){
            delay(1000);
            Serial.println("Serial fail");          
        }
    }
    else{
        Serial.println("Serial ok");
    }
}

void loop(){
    //Serial.println("loop");
    static unsigned long timer = millis();
    //Serial.println(timer);
    if (millis() - timer > 3000) {
        timer = millis();
        //Serial.println(timer);
        int value;
        //value = myDFPlayer.readState(); //read mp3 state
        //value = myDFPlayer.readVolume(); //read current volume
        //value = myDFPlayer.readEQ(); //read EQ setting
        value = myDFPlayer.readFileCounts(); //read all file counts in SD card
        //value = myDFPlayer.readCurrentFileNumber(); //read current play file number
        //value = myDFPlayer.readFileCountsInFolder(3); //read file counts in folder SD:/03
        //Serial.println(value);
        if (value == -1) {  //Error while Reading.
            Serial.println("value = -1");
            //printDetail(myDFPlayer.readType(), myDFPlayer.read());
        }
        else{ //Successfully get the result.
            Serial.println(value);
            //myDFPlayer.play(1);
        }
    }
    delay(500);
}