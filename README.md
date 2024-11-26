# RTTTL player for ESP32

> Any rtttl library is based on the arduino tone() functionality, which is not (yet) available for ESP32.
> This library plays rtttl files on a ESP32. [NonBlockingRTTTL](https://github.com/end2endzone/NonBlockingRTTTL) was used as a base but a lot was changed also. For pwm output generation [ESP32-hal-ledc](https://github.com/espressif/arduino-esp32/blob/a4305284d085caeddd1190d141710fb6f1c6cbe1/cores/esp32/esp32-hal-ledc.h) is used.

The library has been updated to work with the new API of esp32-hal-ledc.h, and it should function flawlessly again.

# Usage
```cpp
#include "RTTTL.h"

#define BUZZER_PIN 23

const char *silentnight = "SilentNight:d=4,o=5,b=112:g.,8a,g,2e.,g.,8a,g,2e.,2d6,d6,2b.,2c6,c6,2g.,2a,a,c6.,8b,a,g.,8a,g,2e.,2a,a,c6.,8b,a,g.,8a,g,2e.,2d6,d6,f6.,8d6,b,2c6.,2e6.,c6,g,e,g.,8f,d,2c.";

RTTTL rtttl(BUZZER_PIN);

void setup() {
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  if (!rtttl.isPlaying()) {
    rtttl.loadSong(silentnight, 15); // 2nd value is the volume.  Try values between ~5-30.
    delay(2000);
  } else rtttl.play();
}
```
