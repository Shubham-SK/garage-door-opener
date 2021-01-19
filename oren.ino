/////////////
// IMPORTS //
/////////////
#include <Adafruit_mfGFX.h>
#include "Adafruit_ILI9341.h"

/////////////////
// PHOTON PINS //
/////////////////

// relay
int relay = D7;

// display screen
int spiReset = D6;
int spiSerialClock = A3;
int spiMasterInSlaveOut = A4;
int spiMasterOutSlaveIn = A5;
int spiChipSelect = D4;
int spiDigitalCommand = D5;

// touch screen
int ypos = A2;
int yneg = D3;
int xpos = D2;
int xneg = A1;

////////////////////
// GLOBAL OBJECTS //
////////////////////

// tft object
Adafruit_ILI9341 tft = Adafruit_ILI9341(spiChipSelect, spiDigitalCommand, spiReset);

//////////////////////
// GLOBAL VARIABLES //
//////////////////////
String state = "closed";
int cycles = 0;

///////////
// SETUP // 
///////////
void setup() {
    // cloud things
    Particle.function("garage", garage);
    Particle.variable("state", state);
    Particle.variable("cycles", cycles);
    
    // setting up relay
    pinMode(relay, OUTPUT);
    digitalWrite(relay, LOW);
    
    // screen initialization stuff
    Serial.begin(9600);
    tft.begin();
    tft.setRotation(90);
    tft.fillScreen(ILI9341_BLACK);
  
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_RED);  
    tft.setTextSize(3);
    tft.println("Control");
    tft.println("System");
    
    tft.setCursor(0, 60);
    tft.setTextColor(ILI9341_GREEN);  
    tft.setTextSize(1);
    tft.println("All Resources Active");
}

//////////
// LOOP //
//////////
void loop() {
    
}


///////////////////
// GARAGE TOGGLE //
///////////////////
int garage(String command){
    if(command == "push"){
        // updating cloud things
        if(state == "open"){
            state = "closed";
        } else if(state == "closed"){
            state = "open";
        }
        cycles++;
        
        // sending signal
        digitalWrite(relay, HIGH);
        delay(500);
        digitalWrite(relay, LOW);
        
        // success
        return 1;
    }
    // failure
    return 0;
}

