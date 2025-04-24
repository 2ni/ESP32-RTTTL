#include "RTTTL.h"
#include "FastLED.h"
#include "fx/1d/twinklefox.hpp"

#define BUZZER_PIN 23
#define BUTTON_PIN 16
#define VOLUME 30 // volume recommendation 5-30
#define SWITCHES 4
#define SONG_COUNT 9

#define NUM_LEDS      16
#define LED_TYPE   WS2812
#define COLOR_ORDER   GRB
#define DATA_PIN        13

#define DEBUG true

const int SONG_SWITCHER[SWITCHES] = {15, 4, 17, 5};

int song = 0;

bool playing = true;
volatile bool flag_songChange = false;

const char *silentnight = "SilentNight:d=4,o=5,b=112:g.,8a,g,2e.,g.,8a,g,2e.,2d6,d6,2b.,2c6,c6,2g.,2a,a,c6.,8b,a,g.,8a,g,2e.,2a,a,c6.,8b,a,g.,8a,g,2e.,2d6,d6,f6.,8d6,b,2c6.,2e6.,c6,g,e,g.,8f,d,2c.";
const char *jinglebells = "JingleBell:d=8,o=5,b=112:a,a,4a,a,a,4a,a,c6,f.,16g,2a,a#,a#,a#.,16a#,a#,a,a.,16a,a,g,g,a,4g,4c6,16p,a,a,4a,a,a,4a,a,c6,f.,16g,2a,a#,a#,a#.,16a#,a#,a,a.,16a,c6,c6,a#,g,2f";
const char *rudolph = "Rudolph:d=16,o=6,b=100:32p,g#5,8a#5,g#5,8f5,8c#,8a#5,4g#.5,g#5,a#5,g#5,a#5,8g#5,8c#,2c,f#5,8g#5,f#5,8d#5,8c,8a#5,4g#.5,g#5,a#5,g#5,a#5,8g#5,8a#5,2f5,g#5,8a#5,a#5,8f5,8c#,8a#5,4g#.5,g#5,a#5,g#5,a#5,8g#5,8c#,2c,f#5,8g#5,f#5,8d#5,8c,8a#5,4g#.5,g#5,a#5,g#5,a#5,8g#5,8d#,2c#";
const char *wewishyou = "WeWishYou:d=4,o=5,b=200:d,g,8g,8a,8g,8f#,e,e,e,a,8a,8b,8a,8g,f#,d,d,b,8b,8c6,8b,8a,g,e,d,e,a,f#,2g,d,g,8g,8a,8g,8f#,e,e,e,a,8a,8b,8a,8g,f#,d,d,b,8b,8c6,8b,8a,g,e,d,e,a,f#,1g,d,g,g,g,2f#,f#,g,f#,e,2d,a,b,8a,8a,8g,8g,d6,d,d,e,a,f#,2g";
const char *winterwonderland = "WinterWonderland:d=16,o=5,b=112:8a#.,a#,2a#.,8a#.,a#,4g,2a#,8a#.,a#,2a#.,8a#.,a#,4g#,2a#,8p,a#,8d.6,d6,8d.6,4c.6,8p,c6,8a#.,a#,8a#.,4g#.,8p,g#,8g.,g,8g.,g,8f.,f,8f.,f,2d#,4p,8a#.,a#,2a#.,8a#.,a#,4g,2a#,8a#.,a#,2a#.,8a#.,a#,4g#,2a#,8p,a#,8d.6,d6,8d.6,4c.6,8p,c6,8a#.,a#,8a#.,4g#.,8p,g#,8g.,g,8g.,g,8f.,f,8f.,f,2d#,4p,8d.,d,8b.,b,8e.,e,8c.6,c6,4b,2g,4p,8d.,d,8b.,b,8e.,e,8c.6,c6,2b.";
const char *ohtannenbaum = "OhTannenbaum:d=4,o=6,b=100:8c5,8f.5,16f5,f.5,8g5,8a.5,16a5,a5,8p,8a5,8g5,8a5,a_5,e5,g5,f.5,8c5,8f.5,16f5,f.5,8g5,8a.5,16a5,a5,8p,8a5,8g5,8a5,a_5,e5,g5,f.5,8c,8c,8a5,d.,8c,8c,8a_5,a_.5,8a_5,8a_5,8g5,c.,8a_5,8a_5,8a5,a.5,8c5,8f.5,16f5,f.5,8g5,8a.5,16a5,a5,8p,8a5,8g5,8a5,a_5,e5,g5,2f5";
const char *letitsnow = "LetItSnow:d=4,o=5,b=125:8c,8c,8c6,8c6,a#,a,g,f,2c,8c,16c,g.,8f,g.,8f,e,2c,d,8d6,8d6,c6,a#,a,2g.,8e.6,16d6,c6,8c.6,16a#,a,8a#.,16a,2f.,c,8c6,8c6,a#,a,g,f,2c,8c.,16c,g.,8f,g.,8f,e,2c,d,8d6,8d6,c6,a#,a,2g.,8e.6,16d6,c6,8c.6,16a#,a,8a.,16g,2f.";
const char *lastchristmas = "LastChristmas:d=4,o=6,b=112:g,16f5,16p,8g,16f5,16p,8f,16f5,16p,8c,8g,8g,8a,f,16f5,16p,16f5,16p,8c,8g,8g,8a,16d5,16p,f,16d5,16p,8f,8e,8f,8e,d,16d5,16p,16d5,16p,16d5,16p,a,16g5,16p,8a,16g5,16p,g,8d,8a,8a,8a_,g,16g5,16p,16g5,16p,8f,8e,8f,16c5,16p,8e,16c5,16p,8f,16c5,16p,8e,16c5,16p,c,16c5,16p,16c5,16p,16c5,16p,16c5,16p,16d5,16p,8f5";
const char *alliwant = "AllIWant:d=4,o=6,b=160:c5,e5,g5,8b5,c,b.5,8a5,g.5,d,c,8c,b5,c,b5,8a5,2g5,a5,c,8d,e,d,c,a.5,f5,8g#5,c.,8d,d#,d,a#5,g#.5,c,d,b5,8c,a5,b5,2g#5,c,d,b5,8c,a5,b5,2g#5,g5,a5,8c,g,f,8g,2f,e,d,c,a5,g#5,2d,e,8d.,2c.";

const char *christmassongs[SONG_COUNT] = {silentnight, jinglebells, rudolph, wewishyou, winterwonderland, ohtannenbaum, letitsnow, lastchristmas, alliwant};

RTTTL rtttl(BUZZER_PIN);

CRGBArray<NUM_LEDS> leds;
TwinkleFoxRef twinkleFox = TwinkleFoxRef::New(NUM_LEDS);

void ISR_setSong(void) 
{
  flag_songChange = true;
}


int SWICTHES2NUMBER(void)
{
  int songNumber = 0;
  int invertedBit;
  for(int i = 0; i < SWITCHES; i++)
  {
    invertedBit = digitalRead(SONG_SWITCHER[i]) == 1 ? 0 : 1;
    //songNumber += invertedBit * 2^i;
    songNumber += invertedBit * (1 << i);
    if(DEBUG) Serial.printf("\n\rPin: %i\tState: %i", SONG_SWITCHER[i], invertedBit);
  }
  Serial.printf("\n\rSong Nr.: %i", songNumber);
  songNumber = songNumber < SONG_COUNT ? songNumber : 0;
  return songNumber;
}

void setup() 
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("\n\rXMAS-Tree on ESP started...");
  for(int i = 0; i < SWITCHES; i++)
  {
    pinMode(SONG_SWITCHER[i], INPUT_PULLUP);
  }
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setRgbw();

  twinkleFox->lazyInit();

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_setSong, FALLING);
}

void loop() 
{
  if(flag_songChange)
  {
    song = SWICTHES2NUMBER();
    playing = true;
    flag_songChange = false;
  }
  
  if (rtttl.done() && playing)
  {
    Serial.printf("\n\rNow Playing: %i", song);
    rtttl.loadSong(christmassongs[song], VOLUME); 
    delay(2000);
    playing = false;
  } else rtttl.play();
  
  EVERY_N_SECONDS(SECONDS_PER_PALETTE) { 
    twinkleFox->chooseNextColorPalette(twinkleFox->targetPalette); 
  }
  twinkleFox->draw(Fx::DrawContext(millis(), leds));
  FastLED.show();
}