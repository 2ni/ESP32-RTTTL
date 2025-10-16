/*
 * RTTTL library to run on ESP32
 * ported from https://github.com/end2endzone/NonBlockingRTTTL
 *
 * using ledc functionality of Arduino ESP32
 *
 * Creator: https://github.com/2ni
 * 
 * Edited: 20.11.2024 by Erik Tóth (https://github.com/erik-toth)
 * Edited: 10.10.2025 by Ryan (https://github.com/ryanhz)
 */

#include "Arduino.h"
#include "RTTTL.h"

RTTTL::RTTTL(const byte pin) {
  this->pin = pin;

  // Attach LEDC channel automatically (Core 3.x doesn't use channel number)
  ledcAttach(pin, 1000, 10);
}

void RTTTL::loadSong(const char *song) {
  loadSong(song, 10);
}

void RTTTL::loadSong(const char *song, const int volume) {
  buffer = song;
  defaultDur = 4;
  defaultOct = 6;
  bpm = 63;
  playing = true;
  noteDelay = 0;
  this->volume = volume;

  // stop current note
  noTone();

  int num;

  // Skip name
  while (*buffer != ':' && *buffer != '\0') buffer++;
  if (*buffer == ':') buffer++;

  // default duration
  if (*buffer == 'd') {
    buffer += 2; // skip "d="
    num = 0;
    while (isdigit(*buffer)) num = (num * 10) + (*buffer++ - '0');
    if (num > 0) defaultDur = num;
    if (*buffer == ',') buffer++;
  }

  // default octave
  if (*buffer == 'o') {
    buffer += 2; // skip "o="
    num = *buffer++ - '0';
    if (num >= 3 && num <= 7) defaultOct = num;
    if (*buffer == ',') buffer++;
  }

  // bpm
  if (*buffer == 'b') {
    buffer += 2; // skip "b="
    num = 0;
    while (isdigit(*buffer)) num = (num * 10) + (*buffer++ - '0');
    bpm = num;
    if (*buffer == ':') buffer++;
  }

  wholenote = (60 * 1000L / bpm) * 4;
}

void RTTTL::noTone() {
  ledcWrite(pin, 0);
}

void RTTTL::tone(int frq, int duration) {
  // Start the tone
  ledcWriteTone(pin, frq);
  ledcWrite(pin, volume);

  // Schedule note end time (no delay!)
  noteDelay = millis() + duration;
}

void RTTTL::nextNote() {
  long duration;
  byte note;
  byte scale;

  // stop previous tone if needed
  noTone();

  // end of song
  if (*buffer == '\0') {
    stop();
    return;
  }

  // duration
  int num = 0;
  while (isdigit(*buffer)) num = (num * 10) + (*buffer++ - '0');
  if (num) duration = wholenote / num;
  else duration = wholenote / defaultDur;

  // note letter
  note = 0;
  switch (*buffer) {
    case 'c': note = 1; break;
    case 'd': note = 3; break;
    case 'e': note = 5; break;
    case 'f': note = 6; break;
    case 'g': note = 8; break;
    case 'a': note = 10; break;
    case 'b': note = 12; break;
    case 'p':
    default: note = 0;
  }
  buffer++;

  // sharp
  if (*buffer == '#') {
    note++;
    buffer++;
  }

  // dotted note
  if (*buffer == '.') {
    duration += duration / 2;
    buffer++;
  }

  // scale
  if (isdigit(*buffer)) {
    scale = *buffer - '0';
    buffer++;
  } else {
    scale = defaultOct;
  }
  scale += OCTAVE_OFFSET;

  // now, get optional '.' dotted note again.
  // A dot /after/ the octave is allowed too, depending on which
  // RTTTL specification you read.
  // See https://github.com/end2endzone/NonBlockingRTTTL/issues/10
  if (*buffer == '.') {
    duration += duration / 2;
    buffer++;
  }

  if (*buffer == ',') buffer++;

  // rest note
  if (note == 0) {
    noTone();
    noteDelay = millis() + duration;
    return;
  }

  // play tone asynchronously
  int freq = notes[(scale - 4) * 12 + note];
  tone(freq, duration);
}

void RTTTL::play() {
  if (!playing) return;

  unsigned long now = millis();

  // If still within note duration, just continue
  if (now < noteDelay) return;

  // If reached end of buffer, stop
  if (*buffer == '\0') {
    stop();
    return;
  }

  // Otherwise, advance to next note
  nextNote();
}

void RTTTL::stop() {
  if (playing) {
    noTone();
    playing = false;
  }
}

bool RTTTL::done() {
  return !playing;
}

bool RTTTL::isPlaying() {
  return playing;
}

