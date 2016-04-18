#include <CPPM.h>

unsigned long time0;
bool LED = true;

void cppm_cycle(void) {
  if (CPPM.synchronized()) {
    int aile = map(CPPM.read_us(CPPM_AILE) - 1500, -430, 430, -256, 256); // aile
    int elev = map(CPPM.read_us(CPPM_ELEV) - 1500, -430, 430, -256, 256); // elevator
    int thro = map(CPPM.read_us(CPPM_THRO) - 1500, -430, 430, -256, 256); // throttle
    int rudd = map(CPPM.read_us(CPPM_RUDD) - 1500, -430, 430, -256, 256); // rudder
    int gear = map(CPPM.read_us(CPPM_GEAR) - 1500, -430, 430, -256, 256); // gear
    int aux1 = map(CPPM.read_us(CPPM_AUX1) - 1500, -430, 430, -256, 256); // flap
    Serial.print(aile); Serial.print(",");
    Serial.print(elev); Serial.print(",");
    Serial.print(thro); Serial.print(",");
    Serial.print(rudd); Serial.print(",");
    Serial.print(gear); Serial.print(",");
    Serial.print(aux1); Serial.print("\n");
    Serial.flush();
  }
}

void setup() {
  Serial.begin(9600);
  CPPM.begin();
  pinMode(13, OUTPUT);
  time0 = millis();
}

void loop() {
  cppm_cycle();
  delay(20);
  if (millis() - time0 > 500) {
    LED = !LED;
    time0 = millis();
  }
  digitalWrite(13, (LED ? HIGH : LOW));
    
}
