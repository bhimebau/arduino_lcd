/*

 TFT Pong

 This example for the Arduino screen reads the values
 of 2 potentiometers to move a rectangular platform
 on the x and y axes. The platform can intersect
 with a ball causing it to bounce.

 This example code is in the public domain.

 Created by Tom Igoe December 2012
 Modified 15 April 2013 by Scott Fitzgerald

 http://www.arduino.cc/en/Tutorial/TFTPong

 */

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

// TFT TFTscreen = TFT(cs, dc, rst);

// variables for the position of the ball and paddle
int paddleX = 0;
int paddleY = 0;
int oldPaddleX = 0;
int oldPaddleY = 0;
int ballDirectionX = 1;
int ballDirectionY = 1;
int ballSpeed = 10; // lower numbers are faster
int ballX, ballY, oldBallX, oldBallY;

void setup() {
  Serial.begin(9600);
  TFT_BL_ON;                                          //turn on the background  light 
  Tft.TFTinit();                                      //init TFT library     
  Tft.fillScreen(0,239,0,319,BLACK);                  // Fill the screen with all Black
                                                      // Lower left coordinate is (0,0) upper right coordinate is (239,319)
}

void loop() {

  // save the width and height of the screen
  int myWidth = 320;
  int myHeight = 240;

  // map the paddle's location to the position of the potentiometers
  // paddleX = map(analogRead(A0), 512, -512, 0, myWidth) - 20 / 2;
  // paddleY = map(analogRead(A1), 512, -512, 0, myHeight) - 5 / 2;

  paddleX = 50;
  paddleY = 10;

  // set the fill color to black and erase the previous
  // position of the paddle if different from present
  if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
    Tft.fillRectangle(oldPaddleX, oldPaddleY, 20, 5, BLACK);  // Erase the previous paddle position
  }
  
  // draw the paddle on screen, save the current position
  // as the previous.
  Tft.fillRectangle(oldPaddleX, oldPaddleY, 20, 5, WHITE);  // Draw the new paddle location
  oldPaddleX = paddleX;
  oldPaddleY = paddleY;

  // update the ball's position and draw it on screen
  if (millis() % ballSpeed < 2) {
     moveBall();
  }
}

// this function determines the ball's position on screen
void moveBall() {
  // if the ball goes offscreen, reverse the direction:
  if (ballX >= 234 || ballX <= 0) {
    ballDirectionX = -ballDirectionX;
  }
  if (ballY >= 314 || ballY <= 0) {
    ballDirectionY = -ballDirectionY;
  }
  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddleX, paddleY, 20, 5)) {
    ballDirectionX = -ballDirectionX;
    ballDirectionY = -ballDirectionY;
  }
  // update the ball's position
  ballX += ballDirectionX;
  ballY += ballDirectionY;

  // erase the ball's previous position
  if (oldBallX != ballX || oldBallY != ballY) {
    Tft.fillRectangle(oldBallX, oldBallY, 5, 5, BLACK); 
  }
  // draw the ball's current position
  if (ballX<0) ballX=0;
  if (ballY<0) ballY=0;
  Tft.fillRectangle(ballX, ballY, 5, 5, WHITE); 
  oldBallX = ballX;
  oldBallY = ballY;
}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;
  if ((x >= rectX && x <= (rectX + rectWidth)) &&
      (y >= rectY && y <= (rectY + rectHeight))) {
    result = true;
  }
  return result;
}


