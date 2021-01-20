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
int yp = A2;
int ym = D3;
int xp = D2;
int xm = A1;

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
    
    // screen display initialization stuff
    Serial.begin(9600);
    tft.begin();
    tft.setRotation(90);
    tft.fillScreen(ILI9341_BLACK);
    
    // time stuff
    Time.zone(-8);
}

//////////
// LOOP //
//////////
void loop() {
    // formatting time
    String timeString = Time.format(Time.now(), "%I:%M %p");
    String dateString = Time.format(Time.now(), "%A, %b %d");
    
    // init
    tft.setCursor(0, 0);
    
    // writing time
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(4);
    tft.println(timeString);
    
    // writing date
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(dateString);
    
    // spacing
    tft.setTextSize(1);
    tft.println();
    tft.println();
    
    // wifi status
    WiFiSignal sig = WiFi.RSSI();
    int strength = sig.getStrength();
    int quality = sig.getQuality();
    
    tft.setTextSize(1);
    if(WiFi.ready()){
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);  
        tft.println("Resources Active");
    } else {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);  
        tft.println("Resources Inactive");
    }
    tft.println("WiFi strength: " + String(strength) + "%");
    tft.println("WiFi quality: " + String(quality) +  "%");
    
    // spacing
    tft.setTextSize(1);
    tft.println();
    
    // weather data
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);  
    tft.println("Cloudy");
    tft.println("Temperature: 70 DEG");
    tft.println("Precipitation: 0%");
    tft.println("Wind Speed: 25 MPH");
    
    // spacing
    tft.setTextSize(1);
    tft.println();
    tft.println();
    
    // status
    tft.setTextSize(2);
    String stateCopy = state;
    stateCopy.toUpperCase();
    if(stateCopy == "OPEN"){
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    } else {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    }
    tft.println("STATUS:"+stateCopy);
    
    // spacing
    tft.setTextSize(1);
    tft.println();
    
    // prompt
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.println("Push Below To Toggle:");
    
    // spacing
    // tft.setTextSize(1);
    // tft.println();
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
