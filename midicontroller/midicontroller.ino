#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

#define NOTE_ON        0x09
#define NOTE_OFF       0x08
#define CONTROL_CHANGE 0x0b

#define CC_BREATH_CONTROLLER 2
#define PIN_BREATH_CONTROLLER 2
#define PIN_BREATH_CONTROLLER_CLOCK 3
#define PRESSURE_MIN 8789000
#define PRESSURE_MAX 11000000

#define CC_SUSTAIN_PEDAL 64
#define PIN_SUSTAIN_PEDAL 1

#define CHANNAL 0

void midi_send(const byte type, const byte chanal, const byte note, const byte velocity);

void sustain_pedal_controller();

void breath_controller_init();

void breath_controller_controller();

void setup() { 
  breath_controller_init();
}

void loop() {
  /* 사용 예제 */
  //midi_send(CONTROL_CHANGE, CHANNAL, CC_SUSTAIN_PEDAL, 127);
  //delay(1000);
  
  //midi_send(NOTE_ON, CHANNAL, 50, 64);
  //delay(100);
  
  //midi_send(NOTE_OFF, CHANNAL, 50, 64);
  //delay(2000);

  //midi_send(CONTROL_CHANGE, CHANNAL, CC_SUSTAIN_PEDAL, 0);
  //delay(3000);
  /************/

  sustain_pedal_controller();
  // breath_controller_controller();
}

void midi_send(const byte type, const byte chanal, const byte note, const byte velocity) {
  midiEventPacket_t packet = {type, (type << 4) | chanal, note, velocity};
  MidiUSB.sendMIDI(packet);
  MidiUSB.flush();
}

void sustain_pedal_controller() {
  static int sustain_pedal_value = 0;
  
  int tmp_sustain_pedal_value = map(analogRead(PIN_SUSTAIN_PEDAL), 0, 1023, 0, 127);
  if (tmp_sustain_pedal_value <= 3) {
    tmp_sustain_pedal_value = 0;
  }
  
  if (abs(sustain_pedal_value - tmp_sustain_pedal_value) > 1) { // 값이 달라질때만 신호 보냄
    sustain_pedal_value = tmp_sustain_pedal_value;
    midi_send(CONTROL_CHANGE, CHANNAL, CC_SUSTAIN_PEDAL, sustain_pedal_value);
  }
}

void breath_controller_init() {
  pinMode(PIN_BREATH_CONTROLLER, INPUT);
  pinMode(PIN_BREATH_CONTROLLER_CLOCK, OUTPUT);
}

void breath_controller_controller() {
  /* 아날로그 압력 측정 */
  // wait for the current reading to finish
  while (digitalRead(2)) {}

  // read 24 bits
  long analog_pressure = 0;
  for (int i = 0; i < 24; i++) {
    digitalWrite(PIN_BREATH_CONTROLLER_CLOCK, HIGH);
    digitalWrite(PIN_BREATH_CONTROLLER_CLOCK, LOW);
    analog_pressure = analog_pressure << 1;
    if (digitalRead(PIN_BREATH_CONTROLLER)) {
      analog_pressure++;
    }
  }

  // get the 2s compliment
  analog_pressure = analog_pressure ^ 0x800000;

  // pulse the clock line 3 times to start the next pressure reading
  for (char i = 0; i < 3; i++) {
    digitalWrite(PIN_BREATH_CONTROLLER_CLOCK, HIGH);
    digitalWrite(PIN_BREATH_CONTROLLER_CLOCK, LOW);
  }
  /******************/

  /* 아날로그 값을 0~127 사이의 값으로 변환 */
  static int breath_controller_value = 0;

  int tmp_breath_controller_value = map(analog_pressure, PRESSURE_MIN, PRESSURE_MAX, 0, 127);
  tmp_breath_controller_value = max(0, tmp_breath_controller_value);
  tmp_breath_controller_value = min(127, tmp_breath_controller_value);
  /*************************************/

  if (breath_controller_value != tmp_breath_controller_value){ // 값이 달라지면 신호 보냄
    breath_controller_value = tmp_breath_controller_value;
    midi_send(CONTROL_CHANGE, CHANNAL, CC_BREATH_CONTROLLER, breath_controller_value);
  }
}
