#include <LedControl.h>

const int X = 0;
const int Y = 1;
int x_position; //Joysticks positon in the x axis
int y_position;

int DIN = 10;
int CS = 12;
int CLK = 11;
int x_head; // The head of the snake
int y_head;

const int button = 50;

LedControl lc = LedControl(DIN, CLK, CS, 0);

int rounds = 0;

int lenght = 2;
int direction = 1;
//                   x, y
int snake[64][2] = {{1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
// The cordinates of the pieces of the snake

int x_apple = random(8); // the position of the apple
int y_apple = random(8);

void setup() {

  pinMode(button, INPUT);


  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);

  x_head = 1;
  y_head = 0;

  //lc.setLed(0, y_head, x_head, true);

}

void loop() {
  x_position = analogRead(X);
  y_position = analogRead(Y);


  /*
  Sets the direction the joystick is turned

      0
    3   1
      2
  */
  if (x_position < 200 && direction != 1) {
    direction = 3;
  }
  else if (x_position > 823 && direction != 3) {
    direction = 1;
  }
  else if (y_position < 200 && direction != 0) {
    direction = 2;
  }
  else if (y_position > 823 && direction != 2) {
    direction = 0;
  }

  // Moves the snakekes head to the direction
  switch (direction) {
    case 0:
      if (y_head >= 7) {
        y_head = 0;
      }
      else {
        y_head += 1;
      }
      break;

    case 1:
      if (x_head >= 7) {
        x_head = 0;
      }
      else {
        x_head += 1;
      }
      break;

    case 2:
      if (y_head <= 0) {
        y_head = 7;
      }
      else {
        y_head -= 1;
      }
      break;

    case 3:
      if (x_head <= 0) {
        x_head = 7;
      }
      else {
        x_head -= 1;
      }
      break;
  }

  // checks if the sanke touches the apple 
  if (x_head == x_apple && y_head == y_apple) {
    lc.setLed(0, y_apple, x_apple, false);

    // Its generates cordinates until the apple is not where the snake is
    bool valid = false;
    while (valid == false) {
      x_apple = random(8);
      y_apple = random(8);

      //checks every piece of the snake if its where the apple is
      for (int piece = 0; piece < lenght; piece++) {
        if (snake[piece][0] == x_apple && snake[piece][1] == y_apple) {
          break;
        }
        // if the for loop is over then it is a valid location for the apple
        if (piece == lenght-1) {
          valid = true;
          lenght += 1;
          break;
        }
      }
    }
  }
  // The snake grows
  else{
    lc.setLed(0, snake[lenght-1][1], snake[lenght-1][0], false);
  }

  // moves forvard the snake
  for (int piece = 1; piece < lenght; piece++) {
    snake[lenght-piece][0] = snake[lenght-piece-1][0];
    snake[lenght-piece][1] = snake[lenght-piece-1][1];
  }
  snake[0][0] = x_head;
  snake[0][1] = y_head;

  // checks if the snake collided with itself
  for (int piece = 1; piece < lenght; piece++) {
    if (snake[0][0] == snake[piece][0] && snake[0][1] == snake[piece][1] && rounds != 0) {
      lc.clearDisplay(0);
      game_over();

      while (digitalRead(button) == LOW){
        delay(50);
      }
      lenght = 2;
      rounds = 0;
      direction = 1;
      snake[0][0] = 1;
      snake[0][1] = 0;
      x_head = 1;
      y_head = 0;
      snake[1][0] = 0;
      snake[1][1] = 0;
      lc.clearDisplay(0);
      break;
    }
  }

  // turns every piece of the snake on
  for (int piece = 0; piece < lenght; piece++) {
    lc.setLed(0, snake[piece][1], snake[piece][0], true);
  }

  // Turns the apple led on-of every round to distinguishes the snake from the apple
  if (rounds % 2 == 0) {
    lc.setLed(0, y_apple, x_apple, true);
  }
  else {
    lc.setLed(0, y_apple, x_apple, false);
  }

  rounds +=1;
  //delay(175);
  delay(300);
}

// this animation is played when the game is over
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
