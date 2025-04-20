![Built for ESP32](https://img.shields.io/badge/Built%20for-ESP32-blue?logo=espressif&logoColor=white)
![Made with :heart:](https://img.shields.io/badge/Made%20with-%E2%9D%A4-red)
[![Forked from 2ni/ESP32-RTTTL](https://img.shields.io/badge/Forked%20from-2ni%2FESP32--RTTTL-181717?logo=github&logoColor=white)](https://github.com/2ni/ESP32-RTTTL)

> [!NOTE]
> This repo has been forked from [2ni/ESP32-RTTTL](https://github.com/2ni/ESP32-RTTTL). Unfortunately, the original creator no longer maintains the repo, and as of now, it doesn't work. This is due to a change in the API usage of [esp32-hal-ledc.h](https://github.com/espressif/arduino-esp32/blob/a4305284d085caeddd1190d141710fb6f1c6cbe1/cores/esp32/esp32-hal-ledc.h).

# RTTTL player for ESP32
## What is RTTTL?
RTTTL stands for [**Ring Tone Text Transfer Language**](https://en.wikipedia.org/wiki/Ring_Tone_Text_Transfer_Language). It was originally used to define ringtones for old phones, using a simple text-based format to describe musical notes, durations, and tempos.

With RTTTL, you can easily encode short melodies into strings and play them back on hardware like the ESP32 using a piezo buzzer or speaker. It’s a fun and lightweight way to bring sound into your microcontroller projects without needing external audio libraries or complex waveforms.

## Why you should use it!
- :musical_note:**Simple & Readable:** RTTTL strings are human-readable and easy to edit, perfect for tinkering.
- :zap:**Lightweight:** No fancy libraries or large files—just text and timing.
- :loud_sound:**Instant Audio:** Add sound to your ESP32 project with minimal code and no external dependencies.
- :jigsaw:**Customizable:** Adjust tempo, pitch, and volume on the fly for different musical effects.
- :rocket:**Plug and Play:** Just drop a melody string in, and you’re ready to go!

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
> [!TIP]
> There are many songs already available in RTTTL format! Just Google something like "song name RTTTL" to find them.