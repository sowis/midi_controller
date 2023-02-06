#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

#define NOTE_ON        0x09
#define NOTE_OFF       0x08
#define CONTROL_CHANGE 0x0b

#define CHANAL 0

#define SUSTAIN_PEDAL 64

void midi_send(const byte type, const byte chanal, const byte note, const byte velocity) {
  midiEventPacket_t packet = {type, (type << 4) | chanal, note, velocity};
  MidiUSB.sendMIDI(packet);
  MidiUSB.flush();
  MidiUSB.flush();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
}

void loop() {
  //midi_send(CONTROL_CHANGE, CHANAL, SUSTAIN_PEDAL, 127);
  //delay(1000);
  
  //midi_send(NOTE_ON, CHANAL, 50, 64);
  //delay(100);
  
  //midi_send(NOTE_OFF, CHANAL, 50, 64);
  //delay(2000);

  //midi_send(CONTROL_CHANGE, CHANAL, SUSTAIN_PEDAL, 0);
  //delay(3000);

  int value = analogRead(0);
  
  Serial.println(value);
  midi_send(CONTROL_CHANGE, CHANAL, SUSTAIN_PEDAL, map(value, 0, 1023, 0, 127));
}
