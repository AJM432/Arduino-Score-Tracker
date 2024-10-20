#include <LiquidCrystal.h>

#define IR_USE_AVR_TIMER1
#include "IRremote.h"

#include "pitches.h"
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int Contrast = 110;

const int gLEDPin = 4;
const int buzzerPin = 2;
const int contrastPin = 6;
const int receiver = 5;  // Signal Pin of IR receiver to Arduino Digital Pin 11
IRrecv irrecv(receiver); // create instance of 'irrecv'

// vairable uses to store the last decodedRawData
uint32_t last_decodedRawData = 0;

const String players[]{
    "Alexander",
    "Perdiccas",
    "Ptolemy",
    "Seleucus",
    "Antipater",
    "Antigonus",
    "Craterus",
    "Cassander",
    "Darius"};


// Global Variables
int t1 = 0;
int t2 = 0;
int numPlayers = 0; // singles=2, doubles=4
int playersArray[] = {-1, -1, -1, -1};
unsigned long greenTimer = 0;
unsigned long irTimer = 0;
int buzzEnabled = 0;
int ledEnabled = 1;
int helpEnabled = 0;
int state = 0; // 0: play, 1: finished, 2: help

int melody[] = {
    NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

int duration = 50;

int helpLocation = 0;

const String enable_disable[] = {"Disabled", "Enabled"};


void setup()
{
  pinMode(gLEDPin, OUTPUT);
  analogWrite(contrastPin, Contrast);
  lcd.begin(16, 2);

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  power_cycle();
  displayStart();
}

void loop()
{
  if ((greenTimer != 0) && (millis() - greenTimer >= 500))
  {
    greenTimer = 0;
    digitalWrite(gLEDPin, LOW);
  }

  if (irrecv.decode()) // have we received an IR signal?
  {
    String ret = translateIR();
    if (helpEnabled && (ret == "9" || ret == "UP" || ret == "DOWN"))
    {
      displayHelp();
    }
    else if (ret == "POWER")
    {
      power_cycle();
      displayStart();
    }
    else if (ret == "7")
    {
      Contrast--;
      showLCDMsg("Contrast--: " + String(Contrast), "", 500);
      analogWrite(contrastPin, Contrast);
      return_to_previous_screen();
    }
    else if (ret == "8")
    {
      Contrast++;
      showLCDMsg("Contrast++: " + String(Contrast), "", 500);
      analogWrite(contrastPin, Contrast);
      return_to_previous_screen();
    }
    else if (ret == "EQ")
    {
      buzzEnabled ^= 1;
      showLCDMsg("Buzzer " + enable_disable[buzzEnabled], "", 500);
      return_to_previous_screen();
    }
    else if (ret == "ST/REPT")
    {
      ledEnabled ^= 1;
      showLCDMsg("LED " + enable_disable[ledEnabled], "", 500);
      return_to_previous_screen();
    }
    else if (ret == "VOL-")
    {
      for (int i = 7; buzzEnabled && (i >= 0); i--)
      {
        tone(buzzerPin, melody[i], duration);
        delay(duration);
      }
      complete_game();
    }
    else if (ret == "FAST BACK")
    {
      play();
    }
    else if (ret == "FUNC/STOP")
    {
      update_t2(t2 + 1);
      play();
    }
    else if (ret == "VOL+")
    {
      update_t1(t1 + 1);
      play();
    }
    else if (ret == "PAUSE")
    {
      update_t1(t1 - 1);
      play();
    }
    else if (ret == "FAST FORWARD")
    {
      update_t2(t2 - 1);
      play();
    }
    else if (ret != "")
    {
      showLCDMsg("Invalid Press", "", 250);
      return_to_previous_screen();
    }
    irrecv.resume(); // receive the next value
  }
}