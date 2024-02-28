#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

#define NOTE_ON        0x09
#define NOTE_OFF       0x08
#define CONTROL_CHANGE 0x0b

#define CHANAL 0

#define SUSTAIN_PEDAL_PIN 1
#define SUSTAIN_PEDAL 64

int last_sustain_pedal_value;

void midi_send(const byte type, const byte chanal, const byte note, const byte velocity);

void setup() {
  // put your setup code here, to run once:
  last_sustain_pedal_value = map(analogRead(SUSTAIN_PEDAL_PIN), 0, 1023, 0, 127);
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

  /* 서스테인 페달 컨트롤러 */
  int sustain_pedal_value = map(analogRead(SUSTAIN_PEDAL_PIN), 0, 1023, 0, 127);
  if (sustain_pedal_value <= 3) {
    sustain_pedal_value = 0;
  }
  
  if (last_sustain_pedal_value != sustain_pedal_value) {
    last_sustain_pedal_value = sustain_pedal_value;
    midi_send(CONTROL_CHANGE, CHANAL, SUSTAIN_PEDAL, sustain_pedal_value);
  }
  /********************/
}

void midi_send(const byte type, const byte chanal, const byte note, const byte velocity) {
  midiEventPacket_t packet = {type, (type << 4) | chanal, note, velocity};
  MidiUSB.sendMIDI(packet);
  MidiUSB.flush();
  MidiUSB.flush();
}
