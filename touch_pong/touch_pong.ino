/*
 Modified version of TFT pong for the TFTV2 display from seed studios. This version uses the touch display to determine the position of the paddle. 
 */

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
#include <SeeedTouchScreen.h> 

#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3 

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2

// Initialize the touch screen
TouchScreen ts = TouchScreen(XP, YP, XM, YM);

// variables for the position of the ball and paddle
int paddleX = 120;
int paddleY = 170;
int oldPaddleX = 120;
int oldPaddleY = 170;
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
  Point p = ts.getPoint();  
  if (p.z > __PRESURE){
    paddleX = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    paddleY = map(p.y, TS_MINY, TS_MAXY, 0, 320);
  }
  else {
    paddleX = oldPaddleX;
    paddleY = oldPaddleY; 
  }
  // set the fill color to black and erase the previous
  // position of the paddle if different from present
  if ((oldPaddleX != paddleX) || (oldPaddleY != paddleY)) {
    Tft.fillRectangle(oldPaddleX, oldPaddleY, 20, 5, BLACK);  // Erase the previous paddle position
    Tft.fillRectangle(paddleX, paddleY, 20, 5, WHITE);  // Draw the new paddle location
    oldPaddleX = paddleX;
    oldPaddleY = paddleY;
  }
  Tft.fillRectangle(paddleX, paddleY, 20, 5, WHITE);  // Draw the new paddle location 

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
    ballDirectionX = ballDirectionX;
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


