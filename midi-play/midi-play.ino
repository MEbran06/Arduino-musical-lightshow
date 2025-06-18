// Play a file from the SD card in looping mode, from the SD card.
// Example program to demonstrate the use of the MIDFile library
//
// Hardware required:
//  SD card interface - change SD_SELECT for SPI comms

#include <SdFat.h>
#include <MD_MIDIFile.h>
#include"notes_bits.h"
#include <Wire.h> 
#include <SoftwareSerial.h>

//Pin connected to latch pin (ST_CP) of 74HC595
const int LATCH_PIN  = 9;
//Pin connected to clock pin (SH_CP) of 74HC595
const int CLOCK_PIN = 8;
//Pin connected to Data in (DS) of 74HC595
const int DATA_PIN = 10;


// SD chip select pin for SPI comms.
// Default SD chip select is the SPI SS pin (10 on Uno, 53 on Mega).
const uint8_t SD_SELECT = 4;  

// The files in the tune list should be located on the SD card 
// or an error will occur opening the file and the next in the 
// list will be opened (skips errors).
const char *loopfile = "midi-file.mid";  // simple and short file
uint8_t light;
boolean state;  
byte lightData1 = 0, lightData2 = 0, lightData3 = 0;
int dataread = 0;

SDFAT  SD;
MD_MIDIFile SMF;
SoftwareSerial mySerial(6, 5); // RX, TX

void midiCallback(midi_event *pev)
// Called by the MIDIFile library when a file event needs to be processed
// thru the midi communications interface.
// This callback is set up in the setup() function.
{ 
  //the lights are set according to the data on MIDI ch.1
  if(pev->data[0]==0x90) state=0;               //144 ch1 noteON ; 0=relayON
  else if(pev->data[0]==0x80) state=1;        //128 ch1 noteOFF  ; 1=relayOFF
    
  // so that lights don't change while the new data is being shifted
  digitalWrite(LATCH_PIN, LOW);
  
  light = getBit(pev->data[1]);
  
  // set the corresponding bit in lightData
  if(light<8)
    bitWrite(lightData1, light, state);
  else if(light<16){
    light-=8;
    bitWrite(lightData2, light, state);
  }
  else {
    light-=16;
    bitWrite(lightData3, light, state);
  }
    

  // shift out the data
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lightData3);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lightData2);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lightData1);

  // turn on the lights according to the new data
  digitalWrite(LATCH_PIN, HIGH);
}
void setup(void)
{
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);  
  pinMode(CLOCK_PIN, OUTPUT);
  
  int  err;

//  Serial.begin(SERIAL_RATE);
  Serial.begin(9600);
  Wire.begin(); 
  mySerial.begin(9600);
  
  // Initialize SD
  if (!SD.begin(SD_SELECT, SPI_FULL_SPEED))
  {
    while (true) ;
  }

  //tell music to start
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(1);              // sends message
  Wire.endTransmission();    // stop transmitting
  
  // Initialize MIDIFile
  SMF.begin(&SD);
  SMF.setMidiHandler(midiCallback);
  SMF.setTempo(115);
//  SMF.looping(true);

  // use the next file name and play it
  err = SMF.load(loopfile);
  if (err != MD_MIDIFile::E_OK)
  {
    while (true);
  }

}

void loop(void)
{
  // play the file
  dataread = mySerial.read();
  if (!SMF.isEOF())
  {
    SMF.getNextEvent();
  } 
  else if (dataread == 5){
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(2);
    Wire.endTransmission();    // stop transmitting
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(1);
    Wire.endTransmission();    // stop transmitting
    SMF.restart();
  }
   else if (dataread != 5) {
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(2);
    Wire.endTransmission();    // stop transmitting
    digitalWrite(LATCH_PIN, LOW);
    for (int i = 0; i < 8; i++) {
      bitWrite(lightData1, i, 0);
    }
    for (int i = 8; i < 16; i++) {
      bitWrite(lightData2, i - 8, 0);
    }
    for (int i = 16; i < 24; i++) {
      bitWrite(lightData3, i - 16, 0);
    }
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lightData1);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lightData2);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lightData3);
    digitalWrite(LATCH_PIN, HIGH);
   }
//   Serial.println(mySerial.read());
}
