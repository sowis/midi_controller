#include <HX710B.h>
#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

#define NOTE_ON        0x09
#define NOTE_OFF       0x08
#define CONTROL_CHANGE 0x0b

#define CC_BREATH_CONTROLLER 2
#define CC_SUSTAIN_PEDAL 64

#define PIN_SUSTAIN_PEDAL 1
#define PIN_BREATH_CONTROLLER 2

#define CHANAL 0

int sustain_pedal_value;

void midi_send(const byte type, const byte chanal, const byte note, const byte velocity);

void setup() {
  // put your setup code here, to run once:
  sustain_pedal_value = map(analogRead(PIN_SUSTAIN_PEDAL), 0, 1023, 0, 127);
}

void loop() {
  /* 사용 예제 */
  //midi_send(CONTROL_CHANGE, CHANAL, CC_SUSTAIN_PEDAL, 127);
  //delay(1000);
  
  //midi_send(NOTE_ON, CHANAL, 50, 64);
  //delay(100);
  
  //midi_send(NOTE_OFF, CHANAL, 50, 64);
  //delay(2000);

  //midi_send(CONTROL_CHANGE, CHANAL, CC_SUSTAIN_PEDAL, 0);
  //delay(3000);
  /************/

  /* 서스테인 페달 컨트롤러 */
  int tmp_sustain_pedal_value = map(analogRead(PIN_SUSTAIN_PEDAL), 0, 1023, 0, 127);
  if (tmp_sustain_pedal_value <= 3) {
    tmp_sustain_pedal_value = 0;
  }
  
  if (abs(sustain_pedal_value - tmp_sustain_pedal_value) > 1) { // 값이 달라질때만 신호 보냄
    sustain_pedal_value = tmp_sustain_pedal_value;
    midi_send(CONTROL_CHANGE, CHANAL, CC_SUSTAIN_PEDAL, tmp_sustain_pedal_value);
  }
  /********************/
}

void midi_send(const byte type, const byte chanal, const byte note, const byte velocity) {
  midiEventPacket_t packet = {type, (type << 4) | chanal, note, velocity};
  MidiUSB.sendMIDI(packet);
  MidiUSB.flush();
}
