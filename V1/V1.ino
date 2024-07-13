#include <LedControl.h>

const int X = 0;
const int Y = 1;
int x_pozicio;
int y_pozicio;

int DIN = 10;
int CS = 12;
int CLK = 11;
int x_fej;
int y_fej;

const int gomb = 50;

LedControl lc = LedControl(DIN, CLK, CS, 0);

int kor = 0;

int hossz = 2;
int irany = 1;
//                   x, y
int kigyo[64][2] = {{1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

int x_alma = random(8);
int y_alma = random(8);

void setup() {
  // put your setup code here, to run once:
  pinMode(gomb, INPUT);


  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);

  x_fej = 1;
  y_fej = 0;

  //lc.setLed(0, y_fej, x_fej, true);

}

void loop() {
  x_pozicio = analogRead(X);
  y_pozicio = analogRead(Y);


  /*
      0
    3   1
      2
  */

  if (x_pozicio < 200 && irany != 1) {
    //x_fej -= 1;
    irany = 3;
  }
  else if (x_pozicio > 823 && irany != 3) {
    //x_fej += 1;
    irany = 1;
  }
  else if (y_pozicio < 200 && irany != 0) {
    //y_fej -= 1;
    irany = 2;
  }
  else if (y_pozicio > 823 && irany != 2) {
    //y_fej += 1;
    irany = 0;
  }

  switch (irany) {
    case 0:
      if (y_fej >= 7) {
        y_fej = 0;
      }
      else {
        y_fej += 1;
      }
      break;

    case 1:
      if (x_fej >= 7) {
        x_fej = 0;
      }
      else {
        x_fej += 1;
      }
      break;

    case 2:
      if (y_fej <= 0) {
        y_fej = 7;
      }
      else {
        y_fej -= 1;
      }
      break;

    case 3:
      if (x_fej <= 0) {
        x_fej = 7;
      }
      else {
        x_fej -= 1;
      }
      break;

  }

  if (x_fej == x_alma && y_fej == y_alma) {
    lc.setLed(0, y_alma, x_alma, false);
    //hossz+=1;

    bool valid = false;
    while (valid == false) {
      x_alma = random(8);
      y_alma = random(8);

      for (int resz = 0; resz < hossz; resz++) {
        if (kigyo[resz][0] == x_alma && kigyo[resz][1] == y_alma) {
          break;
        }
        if (resz == hossz-1) {
          valid = true;
          hossz += 1;
          break;
        }
      }
    }
  }
  else{
    lc.setLed(0, kigyo[hossz-1][1], kigyo[hossz-1][0], false);
  }

  for (int resz = 1; resz < hossz; resz++) {
    kigyo[hossz-resz][0] = kigyo[hossz-resz-1][0];
    kigyo[hossz-resz][1] = kigyo[hossz-resz-1][1];
  }
  kigyo[0][0] = x_fej;
  kigyo[0][1] = y_fej;

  for (int resz = 1; resz < hossz; resz++) {
    if (kigyo[0][0] == kigyo[resz][0] && kigyo[0][1] == kigyo[resz][1] && kor != 0) {

      Serial.println(resz);

      Serial.print(kigyo[0][0]);
      Serial.print("   X   ");
      Serial.println(kigyo[resz][0]);
      Serial.print(kigyo[0][1]);
      Serial.print("   Y   ");
      Serial.println(kigyo[resz][1]);

      
      lc.clearDisplay(0);
      game_over();

      while (digitalRead(gomb) == LOW){
        delay(50);
      }
      hossz = 2;
      kor = 0;
      irany = 1;
      kigyo[0][0] = 1;
      kigyo[0][1] = 0;
      x_fej = 1;
      y_fej = 0;
      kigyo[1][0] = 0;
      kigyo[1][1] = 0;
      lc.clearDisplay(0);
      break;
    }
  }

  for (int resz = 0; resz < hossz; resz++) {
    lc.setLed(0, kigyo[resz][1], kigyo[resz][0], true);
  }

  if (kor % 2 == 0) {
    lc.setLed(0, y_alma, x_alma, true);
  }
  else {
    lc.setLed(0, y_alma, x_alma, false);
  }

  kor +=1;
  //delay(175);
  delay(300);
}


void game_over() {
  lc.clearDisplay(9);

  for (int y = 0; y<=7; y++) {
    for (int x = 0; x<=7; x++) {
        lc.setLed(0, y, x, true);
        delay(25);
    }
    tone(30, 150, 50);
    delay(50);
  }
  tone(30, 150, 150);
  delay(150);
  tone(30, 175, 250);
  delay(250);
  tone(30, 200, 200);
  delay(200);
  delay(300);
  tone(30, 75, 400);
  delay(400);
  delay(200);
  tone(30, 50, 600);
  delay(600);
  tone(0, 50, 600);   
}