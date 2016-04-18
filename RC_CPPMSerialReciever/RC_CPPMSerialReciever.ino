#include <Servo.h>

String serialString;

unsigned long time0;
bool AutoPlt = false, RecieverErr = false, ERRORCHECKING = true, LED = true;

int servoData[] = {0, 0, 0, 0, 0, 0};
int AILE, ELEV, THRO, RUDD, GEAR, AUX1;
double TEMP = 0;
Servo aile, elev, thro, rudd, gear, aux1;

void setup() {
  Serial.begin(9600);
  aile.attach(11);
  elev.attach(10);
  thro.attach(9);
  rudd.attach(6);
  gear.attach(5);
  aux1.attach(3);
  delay(3000);// Wait for the reciever to boot and connecy
}

void loop() {
  while (Serial.available() > 0) {
    serialString = Serial.readStringUntil('\n');
    dataSplit(serialString, ',');
    for (int i = 0;i < 6;i++) {
      Serial.print(servoData[i]);
      Serial.print(i == 5 ? "" : ", ");
    }
    Serial.println();
  }
  
  
  Serial.print(">> ");
  AILE = servoData[0];
  ELEV = servoData[1];
  THRO = servoData[2];
  RUDD = servoData[3];
  GEAR = servoData[4];
  AUX1 = servoData[5];
  
  AutoPlt = (GEAR > 50);
  
  if (GEAR < 50 && GEAR > -50 && ERRORCHECKING) { //Something's wrong, reviever disconnect?
    AutoPlt = true;
    RecieverErr = true;
    Serial.println("RECIEVER ERROR!!");
  } else {
    RecieverErr = false;
    AILE = -AILE;
    ELEV = -ELEV;
  }
  
  if (AutoPlt) { //Autopilot engaged
    Serial.print(" Auto ON ");
    ELEV = 170.0;
    AILE = -170.0;
    
    thro.write(map(THRO, -256, 256, 0, 180));
  } else { //Normal RC Flight
    
  }
  aile.write(map(AILE, -256, 256, 0, 180));
  elev.write(map(ELEV, -256, 256, 0, 180));
  thro.write(map(THRO, -256, 256, 90, 180)); //Special Case for throttle
  rudd.write(map(RUDD, -256, 256, 0, 180));
  gear.write(map(GEAR, -256, 256, 0, 180));
  aux1.write(map(AUX1, -256, 256, 0, 180));
  
  if (RecieverErr && millis() - time0 > 100) { //Reciever Error LED State
    LED = !LED;
    time0 = millis();
    digitalWrite(13, (LED ? HIGH : LOW));
  } else if (AutoPlt && !RecieverErr && millis() - time0 > 500) { //Autopilot LED State
    LED = !LED;
    time0 = millis();
    digitalWrite(13, (LED ? HIGH : LOW));
  }
  
}

void dataSplit(String data, char delim) {  
  char temp[5];
  int tmp = 0;
  memset(temp, 0, sizeof(temp));
  int k = 0;
  
  for (int C = 0;C < data.length();C++) {
    if (data[C] == delim) {
      servoData[k] = atoi(temp);
      k++;
      memset(temp, 0, sizeof(temp));
      tmp = 0;
    } else {
      temp[tmp] = data[C];
      tmp++;
    }
  }
  servoData[k] = atoi(temp);
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
