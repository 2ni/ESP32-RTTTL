#include "RTTTL.h"

#define BUZZER_PIN 23

const char *silentnight = "SilentNight:d=4,o=5,b=112:g.,8a,g,2e.,g.,8a,g,2e.,2d6,d6,2b.,2c6,c6,2g.,2a,a,c6.,8b,a,g.,8a,g,2e.,2a,a,c6.,8b,a,g.,8a,g,2e.,2d6,d6,f6.,8d6,b,2c6.,2e6.,c6,g,e,g.,8f,d,2c.";

RTTTL rtttl(BUZZER_PIN);

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
}

void loop() 
{
  if (!rtttl.isPlaying()) {
    rtttl.loadSong(silentnight, 15); // 2nd value is the volume.  Try values between ~5-30.
    delay(2000);
  } else rtttl.play();
}