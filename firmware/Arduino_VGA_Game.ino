/******************************************************************* 
*                                                                  * 
*  Arduino VGA Game is a game consolle for VGA monitors,           * 
*  written by Roberto Melzi - December the 24th, 2016              * 
*  Arduino software version: 1.6.4                                 * 
*  ArduinoVGAgame Version 01                                       * 
*                                                                  * 
*  VGAx Library written By Sandro Maffiodo aka Smaffer.            * 
*  Version downloaded on August the 17th 2015) here:               * 
*  https://github.com/smaffer/vgax                                 * 
*  See also the Arduino forum:                                     * 
*  http://forum.arduino.cc/index.php?topic=328631.0                * 
*  and on Instructables:                                           *
*  http://www.instructables.com/member/Rob%20Cai/instructables/    *
*                                                                  * 
*******************************************************************/ 

#include <VGAX.h>
#include <math.h>
#include <VGAXUtils.h> 

#define FNT_NANOFONT_HEIGHT 6
#define FNT_NANOFONT_SYMBOLS_COUNT 95
#define PADDLE_HEIGHT 8
#define PADDLE_WIDTH 2
#define RIGHT_PADDLE_X (VGAX_WIDTH-4)
#define LEFT_PADDLE_X 2
#define MAX_Y_VELOCITY 0.1
#define WHEEL_ONE_PIN 3   //analog
#define WHEEL_TWO_PIN 1   //analog
#define BUTTON_ONE_PIN A4 //digital 
#define BUTTON_TWO_PIN A2 //digital 
// NB: pin A0 is used for the sound 

VGAX vga;
VGAXUtils vgaU;

//data size=570 bytes
const unsigned char fnt_nanofont_data[FNT_NANOFONT_SYMBOLS_COUNT][1+FNT_NANOFONT_HEIGHT] PROGMEM={
{ 1, 128, 128, 128, 0, 128, 0, }, //glyph '!' code=0
{ 3, 160, 160, 0, 0, 0, 0, }, //glyph '"' code=1
//{ 5, 80, 248, 80, 248, 80, 0, },  //glyph '#' code=2
//{ 5, 248, 248, 248, 248, 248, 0, },  //glyph '#' code=2 - full rectangle
{ 5, 240, 240, 240, 240, 240, 240, },  //glyph '#' = 'full rectangle' code=2
//{ 5, 32, 160, 240, 120, 32, 32, },  //glyph '#' = 'planeR' code=2
{ 3, 80, 32, 112, 112, 32, 0, },  //glyph '$' = 'bomb' code=3
{ 5, 32, 40, 120, 240, 32, 32, },  //glyph '%' = 'planeL' code=4
{ 5, 96, 144, 104, 144, 104, 0, },  //glyph '&' code=5
{ 5, 248, 248, 248, 248, 248, 248, },  //glyph ''' = 'rectangle 5 x 6' code=6
//{ 5, 120, 160, 112, 40, 240, 0, },  //glyph '$' code=3
//{ 5, 136, 16, 32, 64, 136, 0, },  //glyph '%' code=4
//{ 5, 96, 144, 104, 144, 104, 0, },  //glyph '&' code=5
//{ 2, 128, 64, 0, 0, 0, 0, },  //glyph ''' code=6
{ 2, 64, 128, 128, 128, 64, 0, }, //glyph '(' code=7
{ 2, 128, 64, 64, 64, 128, 0, },  //glyph ')' code=8
{ 3, 0, 160, 64, 160, 0, 0, },  //glyph '*' code=9
{ 3, 0, 64, 224, 64, 0, 0, }, //glyph '+' code=10
{ 2, 0, 0, 0, 0, 128, 64, },  //glyph ',' code=11
{ 3, 0, 0, 224, 0, 0, 0, }, //glyph '-' code=12
{ 1, 0, 0, 0, 0, 128, 0, }, //glyph '.' code=13
{ 5, 8, 16, 32, 64, 128, 0, },  //glyph '/' code=14
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph '0' code=15
{ 3, 64, 192, 64, 64, 224, 0, },  //glyph '1' code=16
{ 4, 224, 16, 96, 128, 240, 0, }, //glyph '2' code=17
{ 4, 224, 16, 96, 16, 224, 0, },  //glyph '3' code=18
{ 4, 144, 144, 240, 16, 16, 0, }, //glyph '4' code=19
{ 4, 240, 128, 224, 16, 224, 0, },  //glyph '5' code=20
{ 4, 96, 128, 224, 144, 96, 0, }, //glyph '6' code=21
{ 4, 240, 16, 32, 64, 64, 0, }, //glyph '7' code=22
{ 4, 96, 144, 96, 144, 96, 0, },  //glyph '8' code=23
{ 4, 96, 144, 112, 16, 96, 0, },  //glyph '9' code=24
{ 1, 0, 128, 0, 128, 0, 0, }, //glyph ':' code=25
{ 2, 0, 128, 0, 0, 128, 64, },  //glyph ';' code=26
{ 3, 32, 64, 128, 64, 32, 0, }, //glyph '<' code=27
{ 3, 0, 224, 0, 224, 0, 0, }, //glyph '=' code=28
{ 3, 128, 64, 32, 64, 128, 0, },  //glyph '>' code=29
{ 4, 224, 16, 96, 0, 64, 0, },  //glyph '?' code=30
{ 4, 96, 144, 176, 128, 112, 0, },  //glyph '@' code=31
{ 4, 96, 144, 240, 144, 144, 0, },  //glyph 'A' code=32
{ 4, 224, 144, 224, 144, 224, 0, }, //glyph 'B' code=33
{ 4, 112, 128, 128, 128, 112, 0, }, //glyph 'C' code=34
{ 4, 224, 144, 144, 144, 224, 0, }, //glyph 'D' code=35
{ 4, 240, 128, 224, 128, 240, 0, }, //glyph 'E' code=36
{ 4, 240, 128, 224, 128, 128, 0, }, //glyph 'F' code=37
{ 4, 112, 128, 176, 144, 112, 0, }, //glyph 'G' code=38
{ 4, 144, 144, 240, 144, 144, 0, }, //glyph 'H' code=39
{ 3, 224, 64, 64, 64, 224, 0, },  //glyph 'I' code=40
{ 4, 240, 16, 16, 144, 96, 0, },  //glyph 'J' code=41
{ 4, 144, 160, 192, 160, 144, 0, }, //glyph 'K' code=42
{ 4, 128, 128, 128, 128, 240, 0, }, //glyph 'L' code=43
{ 5, 136, 216, 168, 136, 136, 0, }, //glyph 'M' code=44
{ 4, 144, 208, 176, 144, 144, 0, }, //glyph 'N' code=45
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph 'O' code=46
{ 4, 224, 144, 224, 128, 128, 0, }, //glyph 'P' code=47
{ 4, 96, 144, 144, 144, 96, 16, },  //glyph 'Q' code=48
{ 4, 224, 144, 224, 160, 144, 0, }, //glyph 'R' code=49
{ 4, 112, 128, 96, 16, 224, 0, }, //glyph 'S' code=50
{ 3, 224, 64, 64, 64, 64, 0, }, //glyph 'T' code=51
{ 4, 144, 144, 144, 144, 96, 0, },  //glyph 'U' code=52
{ 3, 160, 160, 160, 160, 64, 0, },  //glyph 'V' code=53
{ 5, 136, 168, 168, 168, 80, 0, },  //glyph 'W' code=54
{ 4, 144, 144, 96, 144, 144, 0, },  //glyph 'X' code=55
{ 3, 160, 160, 64, 64, 64, 0, },  //glyph 'Y' code=56
{ 4, 240, 16, 96, 128, 240, 0, }, //glyph 'Z' code=57
{ 2, 192, 128, 128, 128, 192, 0, }, //glyph '[' code=58
{ 5, 128, 64, 32, 16, 8, 0, },  //glyph '\' code=59
{ 2, 192, 64, 64, 64, 192, 0, },  //glyph ']' code=60
{ 5, 32, 80, 136, 0, 0, 0, }, //glyph '^' code=61
{ 4, 0, 0, 0, 0, 240, 0, }, //glyph '_' code=62
{ 2, 128, 64, 0, 0, 0, 0, },  //glyph '`' code=63
{ 3, 0, 224, 32, 224, 224, 0, },  //glyph 'a' code=64
{ 3, 128, 224, 160, 160, 224, 0, }, //glyph 'b' code=65
{ 3, 0, 224, 128, 128, 224, 0, }, //glyph 'c' code=66
{ 3, 32, 224, 160, 160, 224, 0, },  //glyph 'd' code=67
{ 3, 0, 224, 224, 128, 224, 0, }, //glyph 'e' code=68
{ 2, 64, 128, 192, 128, 128, 0, },  //glyph 'f' code=69
{ 3, 0, 224, 160, 224, 32, 224, },  //glyph 'g' code=70
{ 3, 128, 224, 160, 160, 160, 0, }, //glyph 'h' code=71
{ 1, 128, 0, 128, 128, 128, 0, }, //glyph 'i' code=72
{ 2, 0, 192, 64, 64, 64, 128, },  //glyph 'j' code=73
{ 3, 128, 160, 192, 160, 160, 0, }, //glyph 'k' code=74
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph 'l' code=75
{ 5, 0, 248, 168, 168, 168, 0, }, //glyph 'm' code=76
{ 3, 0, 224, 160, 160, 160, 0, }, //glyph 'n' code=77
{ 3, 0, 224, 160, 160, 224, 0, }, //glyph 'o' code=78
{ 3, 0, 224, 160, 160, 224, 128, }, //glyph 'p' code=79
{ 3, 0, 224, 160, 160, 224, 32, },  //glyph 'q' code=80
{ 3, 0, 224, 128, 128, 128, 0, }, //glyph 'r' code=81
{ 2, 0, 192, 128, 64, 192, 0, },  //glyph 's' code=82
{ 3, 64, 224, 64, 64, 64, 0, }, //glyph 't' code=83
{ 3, 0, 160, 160, 160, 224, 0, }, //glyph 'u' code=84
{ 3, 0, 160, 160, 160, 64, 0, },  //glyph 'v' code=85
{ 5, 0, 168, 168, 168, 80, 0, },  //glyph 'w' code=86
{ 3, 0, 160, 64, 160, 160, 0, },  //glyph 'x' code=87
{ 3, 0, 160, 160, 224, 32, 224, },  //glyph 'y' code=88
{ 2, 0, 192, 64, 128, 192, 0, },  //glyph 'z' code=89
{ 3, 96, 64, 192, 64, 96, 0, }, //glyph '{' code=90
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph '|' code=91
{ 3, 192, 64, 96, 64, 192, 0, },  //glyph '}' code=92
//{ 3, 96, 192, 0, 0, 0, 0, },  //glyph '~' code=93
{ 5, 32, 160, 240, 120, 32, 32, }, //= 'planeR' code=93
{ 4, 48, 64, 224, 64, 240, 0, },  //glyph '£' code=94
};

static const char str0[] PROGMEM="0"; 
static const char str1[] PROGMEM="1"; 
static const char str2[] PROGMEM="2"; 
static const char str3[] PROGMEM="3"; 
static const char str4[] PROGMEM="4"; 
static const char str5[] PROGMEM="5"; 
static const char str6[] PROGMEM="6"; 
static const char str7[] PROGMEM="7"; 
static const char str8[] PROGMEM="8"; 
static const char str9[] PROGMEM="9"; 
static const char str10[] PROGMEM="#"; 
static const char str15[] PROGMEM="Red wins!"; 
static const char str16[] PROGMEM="Green wins!"; 
static const char str18[] PROGMEM="Game Over!!!"; 
static const char str21[] PROGMEM="########";
static const char str30[] PROGMEM="Choose Arduino Game"; 
static const char str31[] PROGMEM="Pong"; 
static const char str32[] PROGMEM="Breakout"; 
static const char str33[] PROGMEM="Bomber"; 
static const char str34[] PROGMEM="Etch-a-Sketch"; 
static const char str35[] PROGMEM="Tetris"; 
static const char str40[] PROGMEM="X"; 
static const char str51[] PROGMEM="~"; //planeR
static const char str52[] PROGMEM="%"; //planeL
static const char str53[] PROGMEM="$"; //bomb
static const char str54[] PROGMEM="'"; //rectangle 5 x 6
static const char str59[] PROGMEM="'''''''"; //clear 'Hit' banner 
static const char str510[] PROGMEM="Hit!!!"; //'Hit' banner
static const char str511[] PROGMEM="Arduino VGA Bomber"; 
static const char str512[] PROGMEM="by Roberto Melzi"; 
static const char str515[] PROGMEM="Game Over!!!"; 

void setup() {
  //Serial.begin(9600); 
  vga.begin();
  randomSeed(1);  
}

// ************* Variable definition Pong ***************************************
unsigned char x,y;
boolean buttonStatus = 0;
boolean buttonOneStatus = 0;
boolean buttonTwoStatus = 0;
int wheelOnePosition; 
int wheelTwoPosition;
byte state = 1;
int rightPaddleY;
int leftPaddleY;
int rightPaddleY0 = 30;
int leftPaddleY0 = 30;
byte ballX = 4;
byte ballY = 20;
byte ballX0 = 21;
byte ballY0 = 21; 
float ballPX = 20;
float ballPY = 20; 
float ballVx;
float ballVy;
byte scoreR = 0;
byte scoreL = 0; 
int PaddleHit = 0; 
byte ticPosition = 12; 
//int xDrawing; --> ballX
//int yDrawing; --> ballY

// ************* Variable definition Breakout ***************************************
int padPosition; 
int padPositionOld; 
//byte PadLenght = 6; 
byte ballXold;
byte ballYold; 
float ballFX = 70.;
float ballFY = 30.;
float speedX; 
float speedY; 
float speedT; 
float angle; 
float angleDeg = 45.; 
byte gameStep = 0; 
boolean cancelSafe = 0;
byte hitScore = 0; 
byte lives = 9; 
int distanceX;
int distanceY; 
int iDel;
int jDel; 
//byte rowBricks = 7; 
int ballXhit; 
int ballYhit; 
static const float pi PROGMEM = 3.1415296;
int nBricks = 0;
int color = 1; 
static const float speedIncrement PROGMEM = 1.25992105; 
int beginning = 0;

// ************* Variable definition Bomber ***************************************
int shot = 12; // ------------ number of frame before next shot is possible ---------------------
int bombX; 
// float bombY; --> float ballFY; 
// float bombV0; --> float angle;
// float bombV0y; --> float angleDeg;
int bombX0;  
// float ballFY0;  --> float ballVx;  
// float ballFY_0; --> float ballVy; 
// float aereoX; --> float speedX; 
// float aereoY; --> float speedY;
// float Vaereo; --> float speedT;
// int ricarica = 0; --> int beginning = 0; 
//int t = 0; --> int iDel; 
int score = 40; 
int frame = 0;
int bombX_0; 
byte planeX;
byte planeY;
byte planeX0;
byte planeY0; 
//int BomberX --> int padPosition; 
//int BomberX0 --> int padPositionOld;


void processInputs() {
  buttonOneStatus = digitalRead(BUTTON_ONE_PIN); 
  buttonTwoStatus = digitalRead(BUTTON_TWO_PIN);
  wheelOnePosition = analogRead(WHEEL_ONE_PIN);
  wheelTwoPosition = analogRead(WHEEL_TWO_PIN);
  buttonStatus = buttonOneStatus || buttonTwoStatus;  // use this in case you connect two buttons ------------------------------
}

void parameterPongIni() { // Pong
  if (rightPaddleY < 30) {
     rightPaddleY0 = rightPaddleY + 1;
  }
  else {
     rightPaddleY0 = rightPaddleY - 1;
  }
  if (leftPaddleY < 30) {
     leftPaddleY0 = leftPaddleY + 1;
  }
  else {
     leftPaddleY0 = leftPaddleY - 1;
  }
  ballVx = random(50., 80.)/500. + (scoreL + scoreR)/35;  
  ballVy = random(40.)/1000. - 0.02;
}

void parameterIniBomber() { // Bomber 
  speedX = 10 + random(VGAX_WIDTH - 20);
  speedY = 10 + random(VGAX_HEIGHT/4);
  bombX0 = speedX;
  ballVx = speedY + 4;
  speedT = 0.1 + random(20)/100.; 
  angle = speedT - random(-10,10)/100.; 
  angleDeg = random(10)/200.; 
  score = 40; 
  lives = 0;
  buttonOneStatus = 0; 
}

void gameIniBreakout(){ // Breakout
  lives = 9; 
  speedT = .06; 
  gameStep = 0; 
  beginning = 0; 
  angleDeg = 180/4; 
  parameterUpdate(); 
  drawLives(); 
  drawBorderBreakout();
  cancelPad(); 
  drawPadBreakout(); 
  drawBricksGrid(gameStep); 
}

void waitForStart(){ // Pong
   buttonStatus = 0; 
   if(scoreR == 8 || scoreR == 0){
      processInputs(); 
      ballY = ((wheelTwoPosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 4;
      drawGameScreen(); 
   }
   if(scoreL == 8){
      processInputs(); 
      ballY = ((wheelOnePosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 4;
      drawGameScreen(); 
    }
    if (buttonStatus == 1){ 
       ballPX = ballX; 
       ballPY = ballY;
       parameterPongIni(); 
       scoreL = 0; 
       scoreR = 0; 
       drawScore(); 
       buttonStatus = 0;
       state = 3; 
    }
}

void drawPaddles(){ // Pong
    // ----------------- draw right paddle -----------------------------------------------
    rightPaddleY = ((wheelOnePosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 1;
    x = RIGHT_PADDLE_X; 
    if (rightPaddleY != rightPaddleY0 || PaddleHit > 0){
      vgaU.draw_column(x,rightPaddleY0,rightPaddleY0+PADDLE_HEIGHT,0);
      vgaU.draw_column(x+1,rightPaddleY0,rightPaddleY0+PADDLE_HEIGHT,0);
      vgaU.draw_column(x,rightPaddleY,rightPaddleY+PADDLE_HEIGHT,2);
      vgaU.draw_column(x+1,rightPaddleY,rightPaddleY+PADDLE_HEIGHT,2);
    }
    // ----------------- draw left paddle -------------------------------------------------
    leftPaddleY = ((wheelTwoPosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 1;
    x = LEFT_PADDLE_X;
    if (leftPaddleY != leftPaddleY0 || PaddleHit > 0){
       vgaU.draw_column(x,leftPaddleY0,leftPaddleY0+PADDLE_HEIGHT,0);
       vgaU.draw_column(x+1,leftPaddleY0,leftPaddleY0+PADDLE_HEIGHT,0);
       vgaU.draw_column(x,leftPaddleY,leftPaddleY+PADDLE_HEIGHT,1);
       vgaU.draw_column(x+1,leftPaddleY,leftPaddleY+PADDLE_HEIGHT,1);
    }
}

void drawBall(){ // Pong
   if ((ballX != ballX0) || (ballY != ballY0)){
      vga.putpixel(ballX0, ballY0, 0);
      vga.putpixel(ballX0, ballY0 + 1, 0);
      vga.putpixel(ballX0 + 1, ballY0, 0);
      vga.putpixel(ballX0 + 1, ballY0 + 1, 0);
      vga.putpixel(ballX, ballY, 3);
      vga.putpixel(ballX, ballY + 1, 3);
      vga.putpixel(ballX + 1, ballY, 3);
      vga.putpixel(ballX + 1, ballY + 1, 3);
   }
}

void drawNet(){ // Pong 
   for(int i=1; i<VGAX_HEIGHT - 4; i += 6) {
      vgaU.draw_column(VGAX_WIDTH/2, i, i + 3, 3);
   }
}

void drawPlaneBomber() { // --------------- draw plane ----------------------------------------------  
   if(speedT > 0 && planeX != planeX0) {
      vgaPrint(str54, planeX0, planeY0, 0); 
      vgaPrint(str51, planeX, planeY, 1);
   }
   if(speedT < 0 && planeX != planeX0) {
      vgaPrint(str54, planeX0, planeY0, 0); 
      vgaPrint(str52, planeX, planeY, 1);
   }
}

void drawBombBomber() { // -------------------- draw bomb -----------------------------------------------
   vgaPrint(str53, bombX_0, ballVy, 0);
   vgaPrint(str53, bombX, ballFY, 2);
}

void drawBombBomberard() { // ----------------- draw bombard --------------------------------------------
    if (padPosition != padPositionOld) {
      vgaU.draw_column(padPositionOld, VGAX_HEIGHT-4, VGAX_HEIGHT-7, 0); 
      vgaU.draw_row(VGAX_HEIGHT-4, padPositionOld-2, padPositionOld+3, 0); 
      vgaU.draw_row(VGAX_HEIGHT-3, padPositionOld-5, padPositionOld+6, 0); 
      vgaU.draw_row(VGAX_HEIGHT-2, padPositionOld-5, padPositionOld+6, 0); 
     
      vgaU.draw_column(padPosition, VGAX_HEIGHT-4, VGAX_HEIGHT-7, 3); 
      vgaU.draw_row(VGAX_HEIGHT-4, padPosition-2, padPosition+3, 3); 
      vgaU.draw_row(VGAX_HEIGHT-3, padPosition-5, padPosition+6, 3); 
      vgaU.draw_row(VGAX_HEIGHT-2, padPosition-5, padPosition+6, 3); 
   }
}

void drawShotBomber() { // --------------------- draw shot ----------------------------------------------
   if ( beginning == shot - 1) {
      vgaU.draw_column(padPositionOld, 2, VGAX_HEIGHT - 7, 0);
   }
   if ( beginning == shot) {
      vgaU.draw_column(padPosition, 2, VGAX_HEIGHT - 7, 1);
      vga.delay(5); 
   }
}

void drawScoreBomber() { // Bomber
   vgaU.draw_line(VGAX_WIDTH - (116 - 1), 1, VGAX_WIDTH-score-1, 1, 0);  
   vga.putpixel(VGAX_WIDTH - 116, 1, (lives + 2)%3 + 1);
   vgaU.draw_line(VGAX_WIDTH - 1 , 1, VGAX_WIDTH-score, 1, 3);  
}

void drawGameScreenBomber() { // ----------------- main function to draw Bomber game screen ---------------------------
   drawPlaneBomber();
   drawBombBomber();
   drawScoreBomber(); 
   vgaU.draw_row(VGAX_HEIGHT-1, VGAX_WIDTH, 1, 2); 
   drawBombBomberard(); 
   drawShotBomber(); 
}

void parameterUpdate() { // Breakout
  angle = angleDeg/180* pi; 
  if (speedX > 0) {speedX = sin(angle)*speedT;} else {speedX = -sin(angle)*speedT;} 
  if (speedY > 0) {speedY = cos(angle)*speedT;} else {speedY = -cos(angle)*speedT;} 
}

void drawLives() { // Breakout
    vgaPrint(str10, 110, 10, 0);
    if(lives == 9) {vgaPrint(str9, 110, 10, 3);}
    if(lives == 8) {vgaPrint(str8, 110, 10, 3);}
    if(lives == 7) {vgaPrint(str7, 110, 10, 3);}
    if(lives == 6) {vgaPrint(str6, 110, 10, 3);}
    if(lives == 5) {vgaPrint(str5, 110, 10, 3);}
    if(lives == 4) {vgaPrint(str4, 110, 10, 3);}
    if(lives == 3) {vgaPrint(str3, 110, 10, 3);}
    if(lives == 2) {vgaPrint(str2, 110, 10, 3);}
    if(lives == 1) {vgaPrint(str1, 110, 10, 3);}
    if(lives == 0) {gameOverBreakout();} 
}

void gameOverBreakout() { // Breakout
  vgaTone(660, 200); 
  vgaTone(330, 200);
  vgaTone(165, 200); 
  vgaTone(82, 200);
  vgaPrint(str18, 25, 40, 1);
  ballStart(); 
  //if (state != 1){ 
  if (state == 4){ 
     vga.clear(0); 
     gameIniBreakout(); 
  }
  vga.delay(300);
  buttonStatus = 0;
  buttonOneStatus = 0; 
  buttonTwoStatus = 0; 
}

void ballStart(){ // Breakout
   ballFX = padPosition; 
   ballFY = 55; 
   ballCoordinatesBreakout(); 
   ballY = 55; 
   drawBallBreakout(ballX, ballY, 2); 
   //while (buttonStatus == 0 && state != 1){
   while (buttonStatus == 0 && state == 4){
      padPositionOld = padPosition; 
      processInputs();
      padPosition = map(wheelOnePosition, 1023, 0, 7, 93); 
      if (padPosition != padPositionOld){
         cancelPad(); 
         drawPadBreakout();
         drawBallBreakout(ballX, ballY, 0); 
         ballFX = padPosition; 
         ballFY = 55; 
         ballCoordinatesBreakout(); 
         ballY = 55; 
         drawBallBreakout(ballX, ballY, 2); 
      }
      if (buttonTwoStatus == 1){
         state = 0; //*********************************************
         buttonTwoStatus = 0; 
         buttonOneStatus = 0; 
         buttonStatus = 0; 
         vga.clear(0);
         vga.delay(300);
         drawStartMenu();
      }
   }
}

void drawBorderBreakout() { // Breakout 
   vgaU.draw_line(0, 0, 0, 59, 3);
   vgaU.draw_line(0, 0, 99, 0, 3);
   vgaU.draw_line(0, 59, 99, 59, 3);
   vgaU.draw_line(99, 0, 99, 60, 3);
}

void drawPadBreakout() { // Breakout
   vgaU.draw_line(padPosition - 6, 58, padPosition + 6, 58, 1);
   vgaU.draw_line(padPosition - 6, 57, padPosition + 6, 57, 1);
}

void cancelPad() { // Breakout
   vgaU.draw_line(padPositionOld - 6, 58, padPositionOld + 6, 58, 0);
   vgaU.draw_line(padPositionOld - 6, 57, padPositionOld + 6, 57, 0);
}

void brick(int x, int y, int col) { // Breakout
   vgaU.draw_line(x - 5, y + 1, x + 5, y + 1 , col);
   vgaU.draw_line(x - 5, y - 1, x + 5, y - 1 , col);
   vga.putpixel(x - 5, y, col); 
   vga.putpixel(x + 4, y, col); 
}

void drawBricksGrid(int n) { // Breakout
  nBricks = 0; 
  if (n%3 == 0)
  {   
    for (int i = 1; i <= int(100/13 - 1); i++) {
      for (int j = 1; j < 5; j++) {
         color = (i + j)%3 + 1; 
         brick(8 + i*12, 3 + j*4, color); 
         if (color |= 0) {nBricks++;}
      }
    }
  }
  if (n%3 == 1)
  {   
    for (int i = 0; i <= int(100/13); i++) {
      for (int j = 0; j < 7; j++) {
         color = (i + j)%4 + 0; 
         brick(8 + i*12, 3 + j*4, color); 
         if (color |= 0) {nBricks++;}
      }
    }
  }
  if (n%3 == 2)
  {   
    for (int i = 0; i <= int(100/13); i++) {
      for (int j = 0; j < 7; j++) {
         color = (i + j)%3 + 1; 
         brick(8 + i*12, 3 + j*4, color); 
         if (color |= 0) {nBricks++;}
      }
    }
  }
}

void searchHitBrick(int bX, int bY) { // Breakout
   distanceX = 120;
   distanceY = 60;
   for (int i = 0; i <= int(100/(13)); i++) {
      for (int j = 0; j < 7; j++) {
         if (abs(8 + i*12 - bX) < distanceX){
            distanceX = abs(8 + i*12 - bX); 
            iDel = i;
         }
         if (abs(1 + 2 + j*4 - bY) < distanceY){
            distanceY = abs(1 + 2 + j*4 - bY); 
            jDel = j;
         }
      }
   }
   brick(8 + iDel*12, 3 + jDel*4, 0); 
   nBricks--; 
   if (nBricks == 0){nextlivesBreakout();}
}

void drawBallBreakout(int x, int y, int col) { // Breakout
   vga.putpixel(x, y, col); 
   vga.putpixel(x + 1, y, col); 
   vga.putpixel(x, y + 1, col); 
   vga.putpixel(x + 1, y + 1, col);
}

void ballCoordinatesBreakout() { // Breakout
  ballXold = ballX; 
  ballYold = ballY; 
  ballFX += speedX;
  ballFY += speedY;
  ballX = int(ballFX); 
  ballY = int(ballFY); 
}

void nextlivesBreakout() { // Breakout
  gameStep++; 
  speedT = speedIncrement*speedT; 
  drawBricksGrid(gameStep); 
}

void drawBorder() { // Pong
    vgaU.draw_line(0,0,VGAX_WIDTH-1,0,3);
    vgaU.draw_line(0,VGAX_HEIGHT-1,VGAX_WIDTH-1,VGAX_HEIGHT-1,3);
    vgaU.draw_line(0,0,0,VGAX_HEIGHT-1,3);
    vgaU.draw_line(VGAX_WIDTH-1,0,VGAX_WIDTH-1,VGAX_HEIGHT,3);
}

void drawScore() { // Pong
    vgaPrint(str10, 52, 4, 0);
    vgaPrint(str10, 64, 4, 0);
    if(scoreL == 0) {vgaPrint(str0, 52, 4, 1);} 
    if(scoreR == 0) {vgaPrint(str0, 64, 4, 2);}
    if(scoreL == 1) {vgaPrint(str1, 52, 4, 1);} 
    if(scoreR == 1) {vgaPrint(str1, 64, 4, 2);}
    if(scoreL == 2) {vgaPrint(str2, 52, 4, 1);} 
    if(scoreR == 2) {vgaPrint(str2, 64, 4, 2);}
    if(scoreL == 3) {vgaPrint(str3, 52, 4, 1);} 
    if(scoreR == 3) {vgaPrint(str3, 64, 4, 2);}
    if(scoreL == 4) {vgaPrint(str4, 52, 4, 1);} 
    if(scoreR == 4) {vgaPrint(str4, 64, 4, 2);}
    if(scoreL == 5) {vgaPrint(str5, 52, 4, 1);} 
    if(scoreR == 5) {vgaPrint(str5, 64, 4, 2);}
    if(scoreL == 6) {vgaPrint(str6, 52, 4, 1);} 
    if(scoreR == 6) {vgaPrint(str6, 64, 4, 2);}
    if(scoreL == 7) {vgaPrint(str7, 52, 4, 1);} 
    if(scoreR == 7) {vgaPrint(str7, 64, 4, 2);}
    if(scoreL == 8) {
       vgaPrint(str8, 52, 4, 1);
       vgaPrint(str15, 12, 24, 1);
       ballX = VGAX_WIDTH - 6; 
       buttonStatus = 0; 
       while(buttonStatus == 0){
          processInputs(); 
          ballY = ((wheelOnePosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 4;
          drawGameScreen(); 
       }
    }
    if(scoreR == 8) {
       vgaPrint(str8, 64, 4, 2);
       vgaPrint(str16, 66, 24, 2);
       ballX = 4; 
       buttonStatus = 0; 
       while(buttonStatus == 0){
          processInputs(); 
          ballY = ((wheelTwoPosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 4;
          drawGameScreen(); 
       }
    }
    if(scoreL == 8 || scoreR == 8){ //******************************************** Pong end *********************************************************
       buttonStatus = 0; 
       do{ 
          processInputs(); 
       } while (buttonStatus == 0);  
       if (buttonOneStatus == 1){ 
          state = 0;
          vga.delay(300);
       }
       else{
          vga.delay(300);
          parameterPongIni();
          drawPongStartScreen(); 
          state = 2; 
       }
    }
}

void drawPongStartScreen() { // Pong 
   vgaPrint(str10, 52, 4, 0);
   vgaPrint(str10, 64, 4, 0);
   vgaPrint(str0, 52, 4, 1);
   vgaPrint(str0, 64, 4, 2);
   vgaPrint(str16, 66, 24, 0);
   vgaPrint(str15, 12, 24, 0);
   vgaPrint(str21, 11, 24, 0);
   vgaPrint(str21, 65, 24, 0);
   drawBorder(); 
   drawGameScreen(); 
   buttonStatus = 0;
   vga.delay(200);
} 

void drawGameScreen() { // Pong 
    drawBall(); 
    drawPaddles(); 
    drawNet(); 
    ballX0 = ballX;
    ballY0 = ballY; 
    leftPaddleY0 = leftPaddleY;
    rightPaddleY0 = rightPaddleY; 
}

void gameOverSoundBomber() { // Bomber
   vga.delay(300);
   vgaTone(660, 200); 
   vgaTone(330, 200);
   vgaTone(165, 200); 
   vgaTone(82, 200);
}

void bomberHitSoundBomber() { //Bomber
   vgaTone(880, 200); 
   vgaTone(440, 200); 
   vgaTone(220, 200);
   vgaTone(110, 200); 
}

void nextlivesSoundBomber() { //Bomber
   vgaTone(440, 100);
   vgaTone(880, 100);
}

void vgaTone(int freq, int time) {
   vga.tone(freq);
   vga.delay(time); 
   vga.noTone(); 
}

void vgaPrint(const char* str, byte x, byte y, byte color){
   vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str, x, y, color);
}

// -------------------------------------- This is the main loop of the game ------------------------------------
// -------------------------------------------------------------------------------------------------------------
void loop() {
  processInputs(); 
  //if(state == 8) { vga.clear(0); state = 1;}
  if(state == 1 || state == 0) { drawStartMenu(); } 
  if(state == 2) { waitForStart(); }  // Pong
  if(state == 3) { pong(); }
  if(state == 4) { breakout(); }
  if(state == 5) { tetris(); }
  if(state == 6) { bomber(); }
  if(state == 7) { drawingToy(); }
} 
// ------------------------------------------- end of main loop ------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void drawStartMenu(){
   if (state == 0) {
      state = 1; 
      ballX = 4;
      ballY = 20;
      ballX0 = 21;
      ballY0 = 21; 
      vga.clear(0);
   }
   vgaPrint(str30, 20, 2, 3); 
   vgaPrint(str31, 38, 12, 2); 
   vgaPrint(str32, 38, 20, 2); 
   vgaPrint(str33, 38, 28, 2); 
   vgaPrint(str34, 38, 36, 2); 
   //vgaPrint(str10, 70, 52, 3); 
   //vgaPrint(str35, 38, 44, 2); 
   vgaPrint(str40, 26, ticPosition, 1); 
   vga.delay(50);
   processInputs(); 
   if (buttonTwoStatus == 1){
      buttonOneStatus = 0;
      vgaPrint(str40, 26, ticPosition, 0); 
      ticPosition += 8; 
      if (ticPosition > 36) {ticPosition = 12;}
      vgaPrint(str40, 26, ticPosition, 1);
      vga.delay(200);
   }
   if (buttonOneStatus == 1){ 
      buttonOneStatus = 0;
      buttonTwoStatus = 0;
      buttonStatus = 0; 
      vga.clear(0);
      if (ticPosition == 12) { // Pong
         state = 2;
         ballY = ((wheelTwoPosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 4;
         drawPongStartScreen(); 
      } 
      if (ticPosition == 20) { // Breakout 
         state = 4;
         buttonTwoStatus = 0; 
         buttonOneStatus = 0; 
         buttonStatus = 0; 
         gameIniBreakout(); 
         vga.delay(200); 
      } 
      if (ticPosition == 28) { // Bomber
         parameterIniBomber(); 
         state = 6;
      } 
      if (ticPosition == 36) {state = 7;} // drawingToy
      //if (ticPosition == 44) {state = 5;} // Tetris
   }
}

// ----------------------------------------------- pong -----------------------------------
void pong(){ 
    
     ballPX += ballVx;
     ballPY += ballVy;
     ballX = int(ballPX);
     ballY = int(ballPY);
    
     if(ballPY <= 1 || ballPY >= VGAX_HEIGHT - 2 ) {
        ballVy = -ballVy;
        ballPY += ballVy;
        ballY = int(ballPY);
        vgaTone(880,30); 
     }; 
     
     // ---------------------------------------- left paddle hit -------------------------------------------------
     if(ballVx < 0 && ballX == LEFT_PADDLE_X+PADDLE_WIDTH -1 && ballY >= leftPaddleY - 1 && ballY <= leftPaddleY + PADDLE_HEIGHT) { 
        ballVx = -ballVx;
        ballVy += 0.2 * ((ballY - leftPaddleY + 1) - (PADDLE_HEIGHT / 2)) / (PADDLE_HEIGHT / 2);
        vgaTone(660, 30);
        drawScore();
        PaddleHit = 2; 
        drawPaddles(); 
      }
     //----------------------------------------- right paddle hit --------------------------------------------------
     if(ballVx > 0 && ballX == RIGHT_PADDLE_X - 1 && ballY >= rightPaddleY - 1 && ballY <= rightPaddleY + PADDLE_HEIGHT) { 
        ballVx = -ballVx;
        ballVy += 0.2 * ((ballY - rightPaddleY + 1) - (PADDLE_HEIGHT / 2)) / (PADDLE_HEIGHT / 2);
        vgaTone(660, 30);
        drawScore();
        PaddleHit = 2; 
        drawPaddles(); 
     }
     
     //limit vertical speed
     if(ballVy > MAX_Y_VELOCITY) ballVy = MAX_Y_VELOCITY;
     if(ballVy < -MAX_Y_VELOCITY) ballVy = -MAX_Y_VELOCITY;
     
     if(ballX <= 0) { // ------------------------- ball out from left ------------------------------------------
        vgaTone(220, 200);
        scoreR++; 
        drawScore();  
        ballX = 4;   
        PaddleHit = 1;  
        while(buttonTwoStatus == 0){
           processInputs(); 
           ballY = ((wheelTwoPosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 4;
           drawGameScreen(); 
           PaddleHit = 0; 
        }
        ballPX = ballX; 
        ballPY = ballY;
        parameterPongIni(); 
        drawBorder();
     }
     if(ballX >= VGAX_WIDTH - 2) { // ------------ ball out from right ------------------------------------------
        vgaTone(220, 200); 
        scoreL++; 
        drawScore();      
        ballX = VGAX_WIDTH - 6;   
        PaddleHit = 1; 
        while(buttonOneStatus == 0){
           processInputs(); 
           ballY = ((wheelOnePosition / 8) * (VGAX_HEIGHT-PADDLE_HEIGHT-1))/ 128 + 4;
           drawGameScreen(); 
           PaddleHit = 0; 
        }
        ballPX = ballX; 
        ballPY = ballY;
        parameterPongIni(); 
        drawBorder(); 
        ballVx = -ballVx;
     }
     //if (state == 1) {
     //if (state != 2) {
     if (state == 1 || state == 0) {
        vga.clear(0); 
        vga.delay(300);
     }
     else {drawGameScreen();} // ************************************* Pong end check ******************************
     PaddleHit -= 1; 
     PaddleHit = constrain(PaddleHit, 0, 10); 
  }
// ---------------------------------------- void pong() end ---------------------------------------------


// ----------------------------------------------- breakout -----------------------------------
void breakout(){
  if (beginning == 0){ 
     padPositionOld = padPosition; 
     processInputs();
     padPosition = map(wheelOnePosition, 1023, 0, 7, 93); 
     gameIniBreakout(); 
     ballStart(); 
     beginning = 1; 
  }
  padPositionOld = padPosition; 
  processInputs();
  padPosition = map(wheelOnePosition, 1023, 0, 7, 93); 

  if (padPosition != padPositionOld) {cancelPad(); drawPadBreakout();}
  
  ballCoordinatesBreakout(); 
  
  if ( ballX != ballXold | ballY != ballYold) { 
     if (cancelSafe == 0) {
        drawBallBreakout(ballXold, ballYold, 0); 
        cancelSafe = 1; 
     }
     if (vga.getpixel(ballX, ballY) != 0) {hitScore += 1;}
     if (vga.getpixel(ballX + 1, ballY) != 0) {hitScore += 2;}
     if (vga.getpixel(ballX, ballY + 1) != 0) {hitScore += 4;}
     if (vga.getpixel(ballX + 1, ballY + 1) != 0) {hitScore += 8;}
     if (hitScore == 0) // *************************************** ball did not hit anything and can go ahead ************
     {
        drawBallBreakout(ballX, ballY, 2); 
        cancelSafe = 0; 
     }
     else // ******************************* ball has hit something  ***********************
     {
        if (hitScore == 3 | hitScore == 12) 
        {
           speedY = -speedY;
        }
        else
        {
           if (hitScore == 5 | hitScore == 10) 
           {
              speedX = -speedX;
           }
           else 
           {
              speedX = -speedX;
              speedY = -speedY;
           }
        }
        if (ballY > 55) 
        {
            if (vga.getpixel(ballX, ballY + 1) == 1 | vga.getpixel(ballX + 1, ballY + 1) == 1) // ******* ball hits the paddle *************
           {
              angleDeg = angleDeg - 2*speedX/abs(speedX)*(padPosition - ballX); 
              if (angleDeg < 20) {angleDeg = 20;}
              if (angleDeg > 70) {angleDeg = 70;}
              parameterUpdate(); 
           } 
           else // *************** ball hits the bottom *************************
           {
              
              lives = lives - 1; 
              if (lives |= 0){vgaTone(110,100);}
              drawLives(); 
              //if (state != 1 && state != 0) {ballStart();}
              if (state == 4 ) {ballStart();} 
              cancelSafe = 0; 
              speedY = - abs(speedY); 
           }
        }
        else if (ballX > 1 && ballX + 1 < 99 && ballY > 1) // *********** ball hits a brick *******************
        {
           vgaTone(440,30);
           if (hitScore == 1 | hitScore == 3 ) {ballXhit = ballX; ballYhit = ballY;}
           if (hitScore == 2 | hitScore == 10) {ballXhit = ballX + 1; ballYhit = ballY;}
           if (hitScore == 8 | hitScore == 12) {ballXhit = ballX + 1; ballYhit = ballY + 1;}
           if (hitScore == 4 | hitScore == 5) {ballXhit = ballX; ballYhit = ballY + 1;}
           searchHitBrick(ballXhit, ballYhit); 
        }
        hitScore = 0; 
      }
   }
}
// ---------------------------------------- void breakout() end ---------------------------------------------

// ----------------------------------------------- tetris -----------------------------------
void tetris(){
}
// ---------------------------------------- void tetris() end ---------------------------------------------

void drawingToy() {
  
  processInputs(); 
  ballX = map(wheelOnePosition, 0, 1024, VGAX_WIDTH - 1, 1);
  ballY = map(wheelTwoPosition, 0, 1024, VGAX_HEIGHT - 1, 1);
  vga.putpixel(ballX, ballY, color);
  if (buttonOneStatus == 1) {
     vga.clear(0);
     vgaTone(262, 80);
     vgaTone(330, 80);
     vgaTone(392, 80);
     vgaTone(524, 80);
     //buttonOneStatus = 0; 
  }
  if (buttonTwoStatus == 1) {
    if (color == 3) 
       {color = 0;} 
    else 
       {if (color == 2) 
           {color = 3;}
       else 
           {if (color == 1) 
               {color = 2;}
            else
               {if (color == 0) {color = 1;}}
           }
       }
    //buttonTwoStatus = 0;
    vgaTone(440, 60);  
    vga.delay(200); 
  }
  if(buttonOneStatus == 1 && buttonTwoStatus == 1) {
     state = 0;
     //ballX = 4;
     //ballY = 20;
  }
} 
// ---------------------------------------- void drawingToy() end ---------------------------------------------

// ---------------------------------------- void bomber() -------------------------------------------
void bomber() { 
     if (beginning == shot) {  
        if (padPosition > speedX - 2 - lives && padPosition < speedX + 2 + lives) { // plane is hit ------------------
           speedX = -10; 
           score += 8;
           vga.tone(110);
           vga.delay(60); 
           vga.noTone();
         }
         if (padPosition > (bombX - 2 ) && padPosition < (bombX + 2)) { // bomb is hit ----------------
            ballVx = speedY + 4;
            bombX0 = speedX;
            angle = speedT - 0.1*lives + random(-100,100)/1000.; 
            angleDeg = 0.1*lives + random(100)/1000.; 
            iDel = 0;
            score += 8; 
            vga.tone(165);
            vga.delay(60); 
            vga.noTone(); 
          }
     }     
            
     if(speedX > VGAX_WIDTH || speedX < 0) {  // -------------- plane has escaped -----------------------------
        score += -3 - lives; 
        speedT = random(200)/1000.0 + 0.2 + 0.2*lives; 
        speedY = random(VGAX_HEIGHT*2/3*100)/100.0 + 2; 
        if(random(1000) > 500)
           {speedX = 10;} 
        else {
           speedX = VGAX_WIDTH-10; 
           speedT = -speedT; 
        }
     }
     
     // plane position -------------------------------------------------------
     speedX += speedT;
        
     // bomb position and trajectory -----------------------------------------
        bombX = bombX0 + angle*iDel;            
        ballFY = ballVx + 0.6*angleDeg*iDel + 0.08*iDel*iDel/(60 - 20*lives);
        if(ballFY > VGAX_HEIGHT - 5 || bombX < 0 || bombX > VGAX_WIDTH){
          if(bombX >= padPosition - 6 && bombX < padPosition + 6){ // ---------------- Bomber has been hit -------------------------------------------
             vgaPrint(str510, 50, 10, 1); 
             bomberHitSoundBomber(); 
             vgaPrint(str59, 50, 10, 0); 
             vgaPrint(str59, 51, 10, 0); 
             score += -30; 
          }
          ballVx = speedY + 4;
          bombX0 = speedX;
          angle = speedT - 0.1*lives + random(-100,100)/1000.; 
          angleDeg = 0.1*lives + random(100)/1000.; 
          iDel = 0;
       }
       else
       {iDel = iDel + 1;}
     
       // check the shot ---------------------------------------------------------
       if(beginning != 0 ) {
          beginning += -1; 
       }
       if ( buttonOneStatus == 1 && beginning == 0) {
          score += -.5; 
          beginning = shot; 
          padPositionOld = padPosition;
          buttonOneStatus = 0; 
        }
         
        // check score state and Game Over --------------------------------------
        if(score < 0) { // --------------- Game Over !!! ------------------------ 
           buttonOneStatus == 0; 
           gameOverSoundBomber(); 
           vgaPrint(str515, 40, 20, 1); 
           parameterIniBomber(); 
           padPositionOld = padPosition + 1;   
           do
           {
              processInputs(); 
           } while (buttonStatus == 0);  
           vga.clear(0); 
           if (buttonOneStatus == 0){state = 0;}
           vga.delay(300);
        }
        if (score > 116) {
          score = 58;
          nextlivesSoundBomber(); 
          lives++; 
     }
     planeX = byte(speedX);
     planeY = byte(speedY); 
     padPosition = int((((1024 - wheelOnePosition) / 8) * (VGAX_WIDTH - 12))/ 128 + 6);
     if(state == 1) {
        vga.clear(0); 
     }
     else{
        drawGameScreenBomber(); 
     }
     bombX_0 = bombX;
     ballVy = ballFY; 
     planeX0 = planeX;
     planeY0 = planeY;
     padPositionOld = padPosition; 
}
// ---------------------------------------- void bomber() end ---------------------------------------------
