#include <TVout.h>
#include "font4x6.h"

// int light = 13;

bool data[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

TVout tv;

// ShiftRegister74HC595<numberOfShiftRegisters> sr(serialDataPin, clockPin, latchPin);

char comm[60];

class commandClass {
  public:
    void execute() {
      int i = 2;
      bool others = 0;
      byte x = (byte)comm[0] - 17;
      byte y = (byte)comm[1] - 17;
      if (x > 116) {
        x = 116;
      }
      if (x < 0) {
        x = 0;
      }
      if (y > 90) {
        y = 90;
      } if (y < 0) {
        y = 0;
      }
      // Serial.println("");
      // Serial.print(x);
      // Serial.print(" ;;; ");
      // Serial.print(y);
      tv.set_cursor(x, y);
      bool f = 0;
      // Serial.println(comm);
      if (comm[0] != '>') {
        while (f != 1) {
          if (comm[i] != '>') {
            if (comm[i] == '|') {
              tv.print(' ');
            } else if (comm[i] == '^') {
              tv.clear_screen();
            } else {
              if (comm[i] < ' ') {
                tv.print(' ');
              } else if (comm[i] > '}') {
                tv.print('}');
              } else {
                tv.print(comm[i]);
              }
            }
          }
          i = i + 1;
          if ((i > 59) or (comm[i - 1] == '>')) {
            f = 1;
          }
        }
      }
      memset(comm, 0, 60);
    }
};
commandClass command;

class toolsClass {
  public:
    char boolsToChar() {
      unsigned long c = (data[0] * pow(2, 7)) + (data[1] * pow(2, 6)) + (data[2] * pow(2, 5)) + (data[3] * pow(2, 4)) + (data[4] * pow(2, 3)) + (data[5] * pow(2, 2)) + (data[6] * pow(2, 1)) +  + (data[7] * pow(2, 0));
      // int i = 0;
      // while (i < 8) {
      //   c = c + (bools[i] * (pow(2, (7 - i))));
      //   i = i + 1;
      // }
      return (char)(c);
    }

    char * charToBools(char charf) {
      // int f[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      float chari = float((int)charf);
      // int i = 7;
      String r = "";
      // while (chari >= 1) {
      //   chari = chari / 2;
      //   if (chari != floor(chari)) {
      //     f[i] = 1;
      //     chari = chari - 0.5;
      //   } else {
      //     f[i] = 0;
      //   }
      //   i = i - 1;
      // }

      char * returnf[16] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
      float floatf = (chari / 16);
      r = String(returnf[(int)(16 * (floatf - floor(floatf)))]) + r;
      floatf = floor(floatf);
      floatf = (floatf / 16);
      r = String(returnf[(int)(16 * (floatf - floor(floatf)))]) + r;
      // return String(f[0]) + String(f[1]) + String(f[2]) + String(f[3]) + String(f[4]) + String(f[5]) + String(f[6]) + String(f[7]);
      return r.c_str();
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
    int pins[9] = {12, 2, 3, 4, 5, 6, -1, 8, 10};
    int clocky = 10;
    int transN = 0;
    int transI = 0;
    int bufferX = 0;
    int bufferY = 0;
    int receiveClock = A0;
    unsigned long transmitTic = 0;
    bool readN = 0;
    bool typef = 0;
    unsigned long speedfn = 0;
    bool clockMode = 0;

  public:
    int commf = 0;
    // type: 0 = Master, 1 = Slave
    void setupP(bool type, unsigned long speedf) {
      typef = type;
      speedfn = speedf;
      int i = 0;
      while (i < 8) {
        if (type == 1) {
          pinMode(pins[i], INPUT);
        }
        i = i + 1;
      }
      if (type == 1) {
        pinMode(clocky, INPUT);
        pinMode(receiveClock, OUTPUT);
      } else {
        pinMode(clocky, OUTPUT);
        pinMode(receiveClock, INPUT);
      }
    }

    bool readPin(int i) {
      if (pins[i] != -1) {
        return digitalRead(pins[i]);
      } else {
        if (analogRead(A2) > 500) {
          return 1;
        } else {
          return 0;
        }
      }
    }

    void receiveHandler() {
      // digitalWrite(light, clocky);
      if (digitalRead(clocky) == HIGH) {
        if (readN != 1) {
          // int i = 0;
          // while (i < 8) {
          // Serial.println(data[i]);
          //   if (pins[i] != -1) {
          //     data[i] = digitalRead(pins[i]);
          //   } else {
          //     if (analogRead(A2) > 500) {
          //       data[i] = 1;
          //     } else {
          //       data[i] = 0;
          //     }
          //   }
          //   i = i + 1;
          // }
          data[0] = 0;
          data[1] = 0;
          data[2] = 0;
          data[3] = 0;
          data[4] = 0;
          data[5] = 0;
          data[6] = 0;
          data[7] = 0;
          int il = 0;
          while (il < 1) {
            data[0] = data[0] or readPin(0);
            data[1] = data[1] or readPin(1);
            data[2] = data[2] or readPin(2);
            data[3] = data[3] or readPin(3);
            data[4] = data[4] or readPin(4);
            data[5] = data[5] or readPin(5);
            data[6] = data[6] or readPin(6);
            data[7] = data[7] or readPin(7);
            il = il + 1;
          }
          // Serial.begin(115200);
          // Serial.println("");
          // Serial.print(data[0]);
          // Serial.print(data[1]);
          // Serial.print(data[2]);
          // Serial.print(data[3]);
          // Serial.print(data[4]);
          // Serial.print(data[5]);
          // Serial.print(data[6]);
          // Serial.print(data[7]);
          // Serial.println("");
          // Serial.println("returned.");
          // Serial.end();
          digitalWrite(receiveClock, HIGH);
          digitalWrite(13, HIGH);
          // Serial.println(tools.boolsToChar(data));
          readN = 1;
        }
      } else {
        if (readN != 0) {
          comm[commf] = tools.boolsToChar();
          commf = commf + 1;
          if ((commf > 59) or (comm[commf - 1] == '>')) {
            command.execute();
            commf = 0;
          }
          readN = 0;
          digitalWrite(13, LOW);
          digitalWrite(receiveClock, LOW);
        }
      }
    }

    void handler() {
      if (typef == 1) {
        receiveHandler();
      }
    }
};
octalParClass parralel;

void setup() {
  parralel.setupP(1, 5000);
  Serial.begin(115200);
  tv.begin(NTSC, 120, 96);
  tv.select_font(font4x6);
  pinMode(13, OUTPUT);
}

unsigned long latency = 0;
unsigned long m = 0;

int f13 = 0;

void loop() {
  // Serial.println(analogRead(A2));
  // latency = millis() - m;
  // m = millis();
  if (f13 == 0) {
    // digitalWrite(13, HIGH);
    f13 = 1;
  } else {
    // digitalWrite(13, LOW);
    f13 = 0;
  }
  // delay(300);
  parralel.handler();
}
