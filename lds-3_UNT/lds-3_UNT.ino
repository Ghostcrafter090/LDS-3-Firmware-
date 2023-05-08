#include <ShiftRegister74HC595.h>
#include <Wire.h>
#include <ds3231.h>
#include <string.h>
#include <MemoryFree.h>;

using namespace std;

const int numberOfShiftRegisters = 2;
int serialDataPin = 10;
int clockPin = 9;
int latchPin = 11;

int redLed = 3;
int Bri = 5;
int fan = 12;

unsigned long loopTic = 0;

ShiftRegister74HC595<numberOfShiftRegisters> sr(serialDataPin, clockPin, latchPin);

class toolsClass {
  public:
    char boolsToChar(bool* bools) {
      int c = (bools[0] * pow(2, 7)) + (bools[1] * pow(2, 6)) + (bools[2] * pow(2, 5)) + (bools[3] * pow(2, 4)) + (bools[4] * pow(2, 3)) + (bools[5] * pow(2, 2)) + (bools[6] * pow(2, 1)) +  + (bools[7] * pow(2, 0));
      // int i = 0;
      // while (i < 8) {
      //   c = c + (bools[i] * (pow(2, (7 - i))));
      //   i = i + 1;
      // }
      return (char)(c);
    }

    char * strAdd (char* a, char* b) {
      char* name_with_extension;
      name_with_extension = malloc(strlen(a) + 1 + 4); /* make space for the new string (should check the return value ...) */
      strcpy(name_with_extension, a); /* copy name into the new var */
      strcat(name_with_extension, b); /* add the extension */
    }

    char * boolOut = "00000000";

    char * addStrBool(char * a, char * b) {
      boolOut[0] = b[0];
      boolOut[1] = b[1];
      boolOut[2] = b[2];
      boolOut[3] = b[3];
      boolOut[4] = a[0];
      boolOut[5] = a[1];
      boolOut[6] = a[2];
      boolOut[7] = a[3];
    }

    char * charToBools(char charf) {
      float chari = float((int)charf);
      char * returnf[16] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
      float floatf = (chari / 16);
      char * r;
      char * s;
      // r = String(returnf[(int)(16 * (floatf - floor(floatf)))]) + r;
      r = returnf[(int)(16 * (floatf - floor(floatf)))];
      floatf = floor(floatf);
      floatf = (floatf / 16);
      // s = returnf[(int)(16 * (floatf - floor(floatf)))];
      s = addStrBool(r, returnf[(int)(16 * (floatf - floor(floatf)))]);
      // r = strAdd(r, returnf[(int)(16 * (floatf - floor(floatf)))]);
      // strAdd(r, s);
      // return String(f[0]) + String(f[1]) + String(f[2]) + String(f[3]) + String(f[4]) + String(f[5]) + String(f[6]) + String(f[7]);
      // return r.c_str();
      return s;
    }

    bool stringToBool(char f) {
      if (f == '0') {
        return 0;
      } else {
        return 1;
      }
    }

};
toolsClass tools;

class octalParClass {
  private:
    int pins[9] = {8, 9, 10, 11, 12, 13, 14, 15, 7};
    bool data[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int clocky = 7;
    int transN = 0;
    int transI = 0;
    int receiveClock = 2;
    int transF = 0;
    char transValue[300];
    int transJ = 300;
    int bufferX = 0;
    int bufferY = 0;
    unsigned long transmitTic = 0;
    bool readN = 0;
    bool typef = 0;
    unsigned long speedfn = 0;
    bool clockMode = 0;

  public:
    String comm = "";
    // type: 0 = Master, 1 = Slave
    void setupP(bool type, unsigned long speedf) {
      typef = type;
      speedfn = speedf;
      int i = 0;
      while (i < 9) {
        if (type == 1) {
          pinMode(pins[i], INPUT);
        }
        i = i + 1;
      }
      if (type == 0) {
        pinMode(pins[8], OUTPUT);
      }
      if (type == 1) {
        pinMode(clocky, INPUT);
        pinMode(receiveClock, OUTPUT);
      } else {
        pinMode(clocky, OUTPUT);
        pinMode(receiveClock, INPUT);
      }
    }

    void transmit(byte x, byte y, const __FlashStringHelper *gf, bool h = 0) {
      int i = 0;
      int g = 0;
      if (transI > transN) {
        transF = 0;
        memset(transValue, 0, 300);
      }
      byte xf = (x * 4) + 16;
      byte yf = (y * 6) + 16;
      
      if (xf == 16) {
        xf = 15;
      } else if (xf == 32) {
        xf = 31;
      } else if (xf == 64) {
        xf = 63;
      } 
      if (yf == 16) {
        yf = 15;
      } else if (yf == 32) {
        yf == 31;
      } else if (yf == 64) {
        yf = 63;
      }
      if (transF < (299)) {
        if (h != 1) {
          transValue[transF] = xf;
          transValue[transF + 1] = yf;
          transF = transF + 2;
        }

        while (i < strlen((const char*)gf)) {
          if (((const char*)gf)[i] == ' ') {
            transValue[transF] = '|';
          } else {
            transValue[transF] = ((const char*)gf)[i];
          }
          transF = transF + 1;
          i = i + 1;
        }
      }
      // Serial.begin(115200);
      // Serial.println(transI);
      // Serial.end();
      if (transI > transN) {
        transI = 0;
      }
      transN = strlen(transValue);
    }

    void transmit(byte x, byte y, char * gf, bool h = 0) {
      int i = 0;
      int g = 0;
      if (transI > transN) {
        transF = 0;
        memset(transValue, 0, 300);
      }
      byte xf = (x * 4) + 16;
      byte yf = (y * 6) + 16;
      
      if (xf == 16) {
        xf = 15;
      } else if (xf == 32) {
        xf = 31;
      } else if (xf == 64) {
        xf = 63;
      } 
      if (yf == 16) {
        yf = 15;
      } else if (yf == 32) {
        yf == 31;
      } else if (yf == 64) {
        yf = 63;
      }
      if (transF < (299)) {
        if (h != 1) {
          transValue[transF] = xf;
          transValue[transF + 1] = yf;
          transF = transF + 2;
        }

        while (i < strlen(gf)) {
          if (gf[i] == ' ') {
            transValue[transF] = '|';
          } else {
            transValue[transF] = gf[i];
          }
          transF = transF + 1;
          i = i + 1;
        }
      }
      // Serial.begin(115200);
      // Serial.println(transI);
      // Serial.end();
      if (transI > transN) {
        transI = 0;
      }
      transN = strlen(transValue);
    }

    char yStr[10] = "O123456789";

    void transmitInt(byte x, byte y, int num, bool m = 0) {
      if (num < 0) {
        num = 0;
      }
      char r[20];
      byte i = 0;
      int f = 0;
      int numr = num;
      int numf = num;
      while (numf > 9) {
        numr = numf;
        f = 0;
        while (numr > 9) {
          numr = numr / 10;
          f = f + 1;
        }
        numf = numf - (numr * pow(10, f));
        r[i] = yStr[numr];
        i = i + 1;
      }
      r[i] = yStr[numf];
      r[i + 1] = ' ';
      r[i + 2] = '>';
      r[i + 3] = '\0';
      if (i == 0) {
        i = 1;
      }
      if (m == 0) {
        transmit(x - i, y, r);
      } else {
        transmit(x, y, r);
      }
    }

    void updateState(int pin, bool mode) {
      if (data[pin] != mode) {
        int modef = LOW;
        if (mode == 1) {
          modef = HIGH;
        }
        if (pin == 8) {
          digitalWrite(pins[pin], modef);
        } else {
          sr.set(pins[pin], modef);
        }
        data[pin] = mode;
      }
    }

    void transmitChar() {
      transN = strlen(transValue);
      // String n = "";
      if (transI < transN) {
        tools.charToBools(transValue[transI]);
        char * valuef = tools.boolOut;
        // Serial.begin(115200);
        // Serial.println("");
        // Serial.println(valuef);
        // Serial.println(tools.stringToBool(valuef[1]));
        // Serial.end();
        // int i = 0;
        // while (i < 8) {
        //   n = valuef[i];
        //   if (n == "0") {
        //     digitalWrite(pins[i], LOW);
        //   } else {
        //     digitalWrite(pins[i], HIGH);
        //   }
        //   i = i + 1;
        // }

        updateState(0, tools.stringToBool(valuef[0]));
        updateState(1, tools.stringToBool(valuef[1]));
        updateState(2, tools.stringToBool(valuef[2]));
        updateState(3, tools.stringToBool(valuef[3]));
        updateState(4, tools.stringToBool(valuef[4]));
        updateState(5, tools.stringToBool(valuef[5]));
        updateState(6, tools.stringToBool(valuef[6]));
        updateState(7, tools.stringToBool(valuef[7]));
        // transI = transI + 1;
        free(valuef);
      }
    }

    void receiveHandler() {
      if (digitalRead(clocky) == HIGH) {
        if (readN != 1) {
          int i = 0;
          while (i < 8) {
            data[i] = digitalRead(pins[i]);
            i = i + 1;
          }
          // comm = comm + tools.boolsToChar(data);
          readN = 1;
        }
      } else {
        readN = 0;
      }
    }

    bool updatef = 0;

    void transmitHandler() {
      transN = strlen(transValue);
      if (digitalRead(receiveClock) != 1) {
        if (transI <= transN) {
          transmitChar();
          updateState(8, 1);
          updatef = 0;
        }
      } else {
        if (updatef != 1) {
          updateState(8, 0);
          transI = transI + 1;
          updatef = 1;
        }
      }
      if (transI > (transJ - 1)) {
        transF = 0;
        transI = 0;
        memset(transValue, 0, 300);
      }
      // if (transmitTic < millis()) {
      //   transmitTic = millis() + speedfn;
      //   transmitChar();
      // }
      // if ((transmitTic - (speedfn / 2)) < millis()) {
      //   if (transI <= transN) {
      //     updateState(8, 1);
      //   }
      //   if (transI == transN) {
      //     transI = transI + 1;
      //   }
      // } else {
      //   updateState(8, 0);
      // }
    }

    void handler() {
      if (typef == 1) {
        receiveHandler();
      } else {
        if (transI <= transN) {
          transmitHandler();
        }
      }
    }
};
octalParClass parralel;

class timeClass {
  private:
    struct systemTime {
      int year;
      int month;
      int day;
      int hour;
      int minute;
      int second;
    };
    unsigned long lastUpdated = 0;

  public:
    struct ts t;

    systemTime getTime() {
      systemTime out;
      if (lastUpdated != millis()) {
        lastUpdated = millis();
        DS3231_get(&t);
      }
      out.year = (int)t.year;
      out.month = (int)t.mon;
      out.day = (int)t.mday;
      out.hour = (int)t.hour;
      out.minute = (int)t.min;
      out.second = (int)t.sec;
      return out;
    }

    void setTime(int year, int month, int day, int hour, int minute, int second) {
      t.hour = hour;
      t.min = minute;
      t.sec = second;
      t.mday = day;
      t.mon = month;
      t.year = year;
      DS3231_set(t);
    }
};
timeClass timef;

class tickersClass {
  public:
    unsigned long calibTic = 0;
    unsigned long beepTic = 0;
    unsigned long dispTic = 0;
    unsigned long emfTic = 0;
};
tickersClass tickers;

class latencyClass {
  private:
    unsigned long mill = 0;
  public:
    unsigned long loopTime = 0;

    void getLatency() {
      loopTime = ((loopTime * 100) + (micros() - mill)) / 101;
      mill = micros();
    }
};
latencyClass latencyClass;

class soundClass {
  private:
    unsigned long flashTic = 0;
    unsigned long tic = 0;
    int freql = 750;
    unsigned long alarmTic = 0;
    bool flash = 0;
  public:
    void playSound(int tonef, int ticf) {
      if (millis() < tic) {
        noTone(redLed);
      }
      tone(redLed, tonef, ticf);
      tic = millis() + ticf;
    }

    bool getFlashState() {
      return flash;
    }

    void alarm() {
      if (alarmTic < millis()) {
        alarmTic = millis() + 50;
        playSound(freql, 50);
        freql = freql + 25;
        if (freql >= 1000) {
          freql = 750;
        }
        if (flashTic < millis()) {
          flashTic = millis() + 50;
          if (flash == 0) {
            flash = 1;
          } else {
            flash = 0;
          }
        }
      }
    }

    void handler() {
      if (millis() > tic) {
        noTone(redLed);
      }
    }
};
soundClass sounds;

class dangerController {
  private:
    int strikeCount = 0;
    unsigned long danger = 0;
    unsigned long det = 0;
    unsigned long millisf = 0;
    unsigned long soundTic = 0;
    int dangerl = 0;
    int dangerlf = 0;

  public:
    void recordStrike() {
      sounds.playSound(1000, 1000);
      strikeCount = strikeCount + 1;
      danger = danger + 1;
      millisf = millis();
    }

    int getDanger() {
      return (int)danger;
    }

    void handler() {
      det = millisf + ((60 - ((danger - 5) * 20)) * 1000);
      if (millis() > det) {
        if (danger > 0) {
          danger = danger - 1;
          sounds.playSound(440, 100);
        }

        if (danger > 5) {
          danger = 5;
        }
        millisf = millis();
      }
      if (dangerl > 0) {
        if (dangerlf != (getDanger() + 1)) {
          dangerl = getDanger() + 1;
          dangerlf = dangerl;
        }
        if (soundTic < millis()) {
          if (getDanger() != 0) {
            sounds.playSound(550, 125);
          }
          soundTic = millis() + 250;
          dangerl = dangerl - 1;
        }
      } else {
        dangerl = getDanger() + 1;
        dangerlf = dangerl;
        soundTic = millis() + (((60 - ((danger - 5) * 20)) * 1000) / 2);
      }
    }
};
dangerController dangerClass;

class comClass {

  public:

    int strikeReg[3] = {0, 0, 0};
  
    void messageGeneral(int emfHighA, int emfLowA, int emfHighB, int emfLowB) {
      Serial.begin(115200);
      Serial.println(F(""));
      Serial.print(F("{\"strike\":\"False\",\"Calibration\":{\"sensorA\":{\"emfHigh\":"));
      Serial.print(emfHighA);
      Serial.print(F(",\"emfLow\":"));
      Serial.print(emfLowA);
      Serial.print(F("},\"sensorB\":{\"emfHigh\":"));
      Serial.print(emfHighB);
      Serial.print(F(",\"emfLow\":"));
      Serial.print(emfLowB);
      Serial.print(F("}},\"latency\":"));
      Serial.print(latencyClass.loopTime);
      Serial.print(F(",\"freeMemmory\":"));
      Serial.print(freeMemory(), DEC);
      Serial.print(F(",\"millis\":"));
      Serial.print(millis());
      Serial.print(F(",\"dangerLevel\":"));
      Serial.print(dangerClass.getDanger());
      Serial.print(F(",\"time\":["));
      Serial.print(timef.getTime().year);
      Serial.print(",");
      Serial.print(timef.getTime().month);
      Serial.print(",");
      Serial.print(timef.getTime().day);
      Serial.print(",");
      Serial.print(timef.getTime().hour);
      Serial.print(",");
      Serial.print(timef.getTime().minute);
      Serial.print(",");
      Serial.print(timef.getTime().second);
      Serial.print("]}");
      Serial.end();
    }

    void messageStrike(int emfHighA, int emfLowA, int emfHighB, int emfLowB, int emf, int sensor) {
      Serial.begin(115200);
      Serial.println("");
      Serial.print(F("{\"strike\":\"True\",\"Calibration\":{\"sensorA\":{\"emfHigh\":"));
      Serial.print(emfHighA);
      Serial.print(F(",\"emfLow\":"));
      Serial.print(emfLowA);
      Serial.print(F("},\"sensorB\":{\"emfHigh\":"));
      Serial.print(emfHighB);
      Serial.print(F(",\"emfLow\":"));
      Serial.print(emfLowB);
      Serial.print(F("},\"sensor\":"));
      if (sensor == A2) {
        Serial.print(F("\"A\""));
      } else {
        Serial.print(F("\"B\""));
      }
      Serial.print(F("},\"latency\":"));
      Serial.print(latencyClass.loopTime);
      Serial.print(F(",\"millis\":"));
      Serial.print(millis());
      Serial.print(F(",\"strikeMax\":"));
      Serial.print(emf);
      Serial.print(F(",\"dangerLevel\":"));
      Serial.print(dangerClass.getDanger());
      Serial.print(F(",\"time\":["));
      Serial.print(timef.getTime().year);
      Serial.print(",");
      Serial.print(timef.getTime().month);
      Serial.print(",");
      Serial.print(timef.getTime().day);
      Serial.print(",");
      Serial.print(timef.getTime().hour);
      Serial.print(",");
      Serial.print(timef.getTime().minute);
      Serial.print(",");
      Serial.print(timef.getTime().second);
      Serial.print("]}");
      Serial.end();
    }

    void registerStrike(int emf, int sensor) {
      strikeReg[0] = emf;
      strikeReg[1] = sensor;
      strikeReg[2] = 1;
      
    }

    void sendMessage(String message) {
      Serial.begin(115200);
      Serial.println(message);
      Serial.end();
    }
};
comClass com;

class sensorClass {
  private:
    double tempf = 0;
    int mod = 20;
  
  public:
    double getLightLevel() {
      int out = (100 - (analogRead(A3)) + 61) * 4;
      if (out < 50) {
        out = 50;
      }
      if (out > 255) {
        out = 255;
      }
      return out; // replace with out variable, unit is %
    }

    double getTemperature() {
      double a[2] = {136.8288, 24.4};
      double b[2] = {137.6537, 21.9};
      double temp = (((b[1] - a[1]) / (b[0] - a[0])) * analogRead(A1)) + 439.08299187781074;
      tempf = ((tempf * mod) + temp) / (mod + 1);
      // add thermister code here.
      // Serial.begin(115200);
      // Serial.println(tempf);
      // Serial.end();
      return tempf; // replace with out variable here, unit is Celcius
    }
};
sensorClass sensors;

class fanClass {
  private:
    unsigned long fanTic = 0;
    bool fanMode = 0;
    unsigned long lastEvent = 0;

    void setFan(bool mode) {
      if (fanMode != mode) {
        fanMode = mode;
        digitalWrite(fan, mode);
        lastEvent = millis();
      }
    }

  public:
    int returnLastFanEvent() {
      return lastEvent;
    }

    void handler() {
      // double templ = sensors.getTemperature();
      if (fanTic < millis()) {
        if (sensors.getTemperature() < 20) {
          setFan(LOW);
        } else {
          setFan(HIGH);
        }
        fanTic = millis() + 10000;
      }
    }
};
fanClass thermals;

class emfClass {
  public:
    int emf = 0;
    int emfHigh = 0;
    int emfLow = 0;
    int emfHighn = 0;
    int emfLown = 0;
    unsigned long calibTic = 0;

    double refToVolt(int ref) {
      switch(ref) {
        case 1:
          return 5;
          break;
        case 2:
          return 1.1;
          break;
        case 3:
          return 2.56;
          break;
      }
      return 5;
    }

    int analogRef = 1;

    int analogPin = A0;

    emfClass(int analogPinf, int reference) {
      analogPin = analogPinf;
      analogRef = reference;
    }

    void calibrate() {
      if (millis() > tickers.beepTic) {
        if (emf > emfHigh) {
          emfHigh = emf;
          sounds.playSound(2000, 10);
        }
        if (emf < emfLow) {
          emfLow = emf;
          sounds.playSound(200, 10);
        }
        if (millis() > calibTic) {
          emfHigh = emfHigh - 1;
          emfLow = emfLow + 1;
          calibTic = millis() + 5000;
          sounds.playSound(100, 10);
        }
        emfHighn = emfHigh + (int)(8 * (5 / refToVolt(analogRef)));
        emfLown = emfLow - (int)(8 * (5 / refToVolt(analogRef)));
      }
    }

    // analogReference(0); EXTERNAL
    // analogReference(1); DEFAULT
    // analogReference(2); INTERNAL1V1
    // analogReference(3); INTERNAL2V56

    void detect() {
      analogReference(analogRef);
      emf = analogRead(analogPin);
      analogReference(1);
      if ((emf > emfHighn) or (emf < emfLown)) {
        if (millis() > tickers.beepTic) {
          if ((thermals.returnLastFanEvent() + 2000) < millis()) {
            if (millis() > 10000) {
              dangerClass.recordStrike();
              tickers.beepTic = millis() + 2000;
            }
          }
        }
        com.registerStrike(emf, analogPin);
      }
    }
};
emfClass sensorA(A2, 2);
emfClass sensorB(A0, 2);

class dispClass {
  private:
    int white5Led0 = 1;
    int green5Led0 = 2;
    int yellow5Led0 = 3;
    int green3Led0 = 4;
    int yellow3Led0 = 5;
    int yellow3Led1 = 6;
    int red3Led0 = 7;
    bool states[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // {green5Led0, yellow5Led0, red5Led0, green3Led0, yellow3Led0, yellow3Led1, red3Led0, white5Led0}
    unsigned long blinker = 0;
    bool blinkState = 0;
    int emfHighOld = 0;
    float change = 0;

  public:
    bool alarm = 0;
    void ledState(int led, bool mode) {
      if (states[led] != mode) {
        int modef = LOW;
        if (mode == 1) {
          modef = HIGH;
        }
        if (led == 2) {
          digitalWrite(redLed, modef);
        } else if (led < 2) {
          sr.set(led + 2, modef);
        } else if (led < 7) {
          sr.set(led + 1, modef);
        } else {
          sr.set(1, modef);
        }
        states[led] = mode;
      }
    }

    void dangerIndicator() {
      analogWrite(Bri, (sensors.getLightLevel()));
      if (dangerClass.getDanger() > 1) {
        if (blinker < millis()) {
          if (blinkState == 0) {
            blinkState = 1;
          } else {
            blinkState = 0;
          }
          blinker = millis() + 500;
        }
      }
      if (dangerClass.getDanger() == 0) {
        ledState(0, 1);
        ledState(1, 0);
        ledState(2, 0);
      } else if (dangerClass.getDanger() == 1) {
        ledState(0, 1);
        ledState(1, 1);
        ledState(2, 0);
      } else if (dangerClass.getDanger() == 2) {
        ledState(0, blinkState);
        ledState(1, 1);
        ledState(2, 0);
      } else if (dangerClass.getDanger() == 3) {
        ledState(0, 0);
        ledState(1, 1);
        ledState(2, 0);
      } else if (dangerClass.getDanger() == 4) {
        ledState(0, 0);
        ledState(1, blinkState);
        ledState(2, 0);
      }
      if (dangerClass.getDanger() >= 5) {
        ledState(0, 0);
        ledState(1, blinkState);
        // ledState(2, 1);
        alarm = 1;
      } else {
        alarm = 0;
      }
    }

    void flash() {
      if (alarm == 1) {
        ledState(7, sounds.getFlashState());
      } else {
        ledState(7, 0);
      }
    }

    void emfMeter() {
      float changef = (sensorB.emf - (sensorB.emfHigh - (sensorB.emfHigh / 3)));
      float changen = sensorB.emfHigh - emfHighOld;
      if (changef < -0.1) {
        changef = -0.1;
      }
      if (changen < 0) {
        changen = 0;
      }
      change = ((((change) * 5) + changef) / 6) + changen;
      if (change < -0.5) {
        change = -0.5;
      }
      if (change > 8) {
        change = 8;
      }
      if (change >= 0) {
        ledState(3, 1);
        if (change > 2) {
          ledState(4, 1);
          if (change > 4) {
            ledState(5, 1);
            if (change > 6) {
              ledState(6, 1);
            } else {
              ledState(6, 0);
            }
          } else {
            ledState(5, 0);
          }
        } else {
          ledState(4, 0);
        }
      } else {
        ledState(3, 0);
      }
      emfHighOld = sensorB.emfHigh;
    }

    void testLights() {
      sounds.playSound(500, 800);
      analogWrite(Bri, 255);
      int out = 0;
      sr.set(white5Led0, HIGH);
      delay(100);
      sr.set(white5Led0, LOW);
      digitalWrite(redLed, HIGH);
      delay(100);
      digitalWrite(redLed, LOW);
      sr.set(yellow5Led0, HIGH);
      delay(100);
      sr.set(yellow5Led0, LOW);
      sr.set(green5Led0, HIGH);
      delay(100);
      sr.set(green5Led0, LOW);
      sr.set(green3Led0, HIGH);
      delay(100);
      sr.set(green3Led0, LOW);
      sr.set(yellow3Led0, HIGH);
      delay(100);
      sr.set(yellow3Led0, LOW);
      sr.set(yellow3Led1, HIGH);
      delay(100);
      sr.set(yellow3Led1, LOW);
      sr.set(red3Led0, HIGH);
      delay(100);
      digitalWrite(redLed, HIGH);
      sr.set(yellow5Led0, HIGH);
      sr.set(green5Led0, HIGH);
      sr.set(green3Led0, HIGH);
      sr.set(yellow3Led0, HIGH);
      sr.set(yellow3Led1, HIGH);
      while (out < 255) {
        analogWrite(Bri, out);
        out = out + 1;
        if (((float)out / 50) == floor(out / 50)) {
          sounds.playSound(out * 4, 50);
        }
        delay(2);
      }
      delay(50);
      out = 255;
      while (out >= 0) {
        analogWrite(Bri, out);
        if (((float)out / 50) == floor(out / 50)) {
          sounds.playSound(out * 4, 50);
        }
        out = out - 1;
        delay(2);
      }
      digitalWrite(redLed, LOW);
      sr.set(yellow5Led0, LOW);
      sr.set(green5Led0, LOW);
      sr.set(green3Led0, LOW);
      sr.set(yellow3Led0, LOW);
      sr.set(yellow3Led1, LOW);
      sr.set(red3Led0, LOW);
    }
};
dispClass dispClass;

class screenClass {
  private:
    unsigned long updateTic = 0;
    char * strings[1] = {"LDS-3 Lightning Detector>"};
    unsigned int coords[1][2] = {{(char)1, (char)1}};
    int n = 0;
    int r = 40;
    int j = 5;

    int hourOld = -1;
    int minuteOld = -1;

    int dangerOld = 0;
    int emfHighOld[2] = {0, 0};
    int emfLowOld[2] = {0, 0};

  public:
    void handler() {
      if (updateTic < millis()) {
        updateTic = millis() + 500;
        r = r + 1;
        if (r > 40) {
          parralel.transmit(0, 0, F("^>"));
          parralel.transmit(1, 1, F("LDS-3>"));
          parralel.transmit(7, 1, F("Lightning Detector>"));
          parralel.transmit(1, 2, F("------------------------->"));
          parralel.transmit(3, 5, F("Danger Level: >"));
          parralel.transmit(3, 6, F("EMF High SA : >"));
          parralel.transmit(3, 7, F("EMF Low  SA : >"));
          parralel.transmit(3, 9, F("EMF High SB : >"));
          parralel.transmit(3, 10, F("EMF Low  SB : >"));
          parralel.transmitInt(17, 5, dangerClass.getDanger(), 1);
          parralel.transmitInt(17, 6, sensorA.emfHigh, 1);
          parralel.transmitInt(17, 7, sensorA.emfLow, 1);
          parralel.transmitInt(17, 9, sensorB.emfHigh, 1);
          parralel.transmitInt(17, 10, sensorB.emfLow, 1);
          dangerOld =  dangerClass.getDanger();
          emfHighOld[0] = sensorA.emfHigh;
          emfLowOld[0] = sensorA.emfLow;
          emfHighOld[1] = sensorB.emfHigh;
          emfLowOld[1] = sensorB.emfLow;
          r = 0;
        }
        
        int secondf = timef.getTime().second;
        int minutef = timef.getTime().minute;
        int hourf = timef.getTime().hour;
        if (hourf > 12) {
          hourf = hourf - 12;
        }
        if (hourf == 0) {
          hourf = 12;
        }

        if (dangerClass.getDanger() != dangerOld) {
          dangerOld =  dangerClass.getDanger();
          parralel.transmitInt(17, 5, dangerClass.getDanger(), 1);
        }
        if (emfHighOld[0] != sensorA.emfHigh) {
          emfHighOld[0] = sensorA.emfHigh;
          parralel.transmitInt(17, 6, sensorA.emfHigh, 1);
        }
        if (emfLowOld[0] != sensorA.emfLow) {
          emfLowOld[0] = sensorA.emfLow;
          parralel.transmitInt(17, 7, sensorA.emfLow, 1);
        }
        if (emfHighOld[1] != sensorB.emfHigh) {
          emfHighOld[1] = sensorB.emfHigh;
          parralel.transmitInt(17, 9, sensorB.emfHigh, 1);
        }
        if (emfLowOld[1] != sensorB.emfLow) {
          emfLowOld[1] = sensorB.emfLow;
          parralel.transmitInt(17, 10, sensorB.emfLow, 1);
        }
        
        if (hourOld != hourf) {
          if (hourf < 10) {
            parralel.transmit(1, 15, F("O>"));
            parralel.transmitInt(2, 15, hourf, 1);
          } else {
            parralel.transmitInt(1, 15, hourf, 1);
          }
          parralel.transmit(4, 15, F(":>"));
        }

        if (minuteOld != minutef) {
          if (minutef < 10) {
            parralel.transmit(6, 15, F("O>"));
            parralel.transmitInt(7, 15, minutef, 1);
          } else {
            parralel.transmitInt(6, 15, minutef, 1);
          }
          parralel.transmit(9, 15, F(":>"));
        }
        j = j + 1;
        if (j > 5) {
          if (secondf < 10) {
            parralel.transmit(11, 15, F("O>"));
            parralel.transmitInt(12, 15, secondf, 1);
          } else {
            parralel.transmitInt(11, 15, secondf, 1);
          }
        }

        // parralel.transmit(25, 11,     "X>");
        // parralel.transmit(22, 12,  "  X X>");
        // parralel.transmit(22, 13,  " X   X>");
        // parralel.transmit(22, 14,  "X     X>");
        // parralel.transmit(21, 15, "XXXXXXXXX>");

        // parralel.transmit(17, 4, tools.intToChar(dangerClass.getDanger()));
        // Serial.begin(115200);
        // char * danger;
        // Serial.println(tools.intToChar(danger, 10));
        // Serial.end();
        // free(danger);
        n = n + 1;
        if (n > sizeof(strings)) {
          n = 0;
        }
      }
    }
};
screenClass screen;

void setup() {
  // put your setup code here, to run once:
  pinMode(redLed, OUTPUT);
  pinMode(Bri, OUTPUT);
  pinMode(fan, OUTPUT);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("{\"message\":\"System Started.\"}");
  Serial.end();
  Wire.begin();
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  DS3231_init(DS3231_CONTROL_INTCN);
  dispClass.testLights();
  parralel.setupP(0, 80);
  delay(1000);
  digitalWrite(13, LOW);
}

bool runCalibrate = true;
bool runScreen = false;
bool slowTic = true;
bool runEmf = true;
int sendGeneral = 0;

void loop() {
  runCalibrate = ((loopTic % 2) == 0); // 2
  runScreen = (loopTic % 4) == 0; // 2
  runEmf = (loopTic % 10) == 0; // 10
  slowTic = (loopTic % 100) == 0; // 100
  sensorA.detect();
  if (com.strikeReg[2] == 1) {
    com.messageStrike(sensorA.emfHigh, sensorA.emfLow, sensorB.emfHigh, sensorB.emfLow, com.strikeReg[0], com.strikeReg[1]);
    com.strikeReg[2] = 0;
  }
  sensorB.detect();
  if (com.strikeReg[2] == 1) {
    com.messageStrike(sensorA.emfHigh, sensorA.emfLow, sensorB.emfHigh, sensorB.emfLow, com.strikeReg[0], com.strikeReg[1]);
    com.strikeReg[2] = 0;
  }
  if (runCalibrate) {
    sensorA.calibrate();
    sensorB.calibrate();
  }
  if (tickers.dispTic < millis()) {
    dispClass.dangerIndicator();
    tickers.dispTic = millis() + 500;
    sendGeneral = sendGeneral + 1;
  }
  if (sendGeneral > 9) {
    com.messageGeneral(sensorA.emfHigh, sensorA.emfLow, sensorB.emfHigh, sensorB.emfLow);
    sendGeneral = 0;
  }
  dispClass.flash();
  if (loopTic > 2) {
    if (runEmf) {
      dispClass.emfMeter();
    }
  }
  if (dispClass.alarm == 1) {
    sounds.alarm();
  }
  if (slowTic) {
    dangerClass.handler();
    sounds.handler();
    thermals.handler();
  }
  if (runScreen) {
    parralel.handler();
    screen.handler();
  }
  latencyClass.getLatency();
  loopTic = loopTic + 1;
  if (loopTic >= 100000) {
    loopTic = 2;
  }
}
