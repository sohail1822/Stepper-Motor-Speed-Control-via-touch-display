#include <bits/stdc++.h>
using namespace std;

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "URTouch.h"
#include "SPI.h"

byte dirr = A5;
byte step = A4;
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_SCK 13

#define t_SCK 3 // t_CLK
#define t_CS 4
#define t_MOSI 5 // t_DIN
#define t_MISO 6 // t_DO
#define t_IRQ 7

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// for slider
//  #define SLIDER_X 50
//  #define SLIDER_Y 180
//  #define SLIDER_WIDTH 280
//  #define SLIDER_HEIGHT 20
//  #define MIN_SPEED   1
//  #define MAX_SPEED   100

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

URTouch ts(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

void drawClockwiseArc()
{
    int x0 = 107.5;
    int y0 = 167.5;
    int r = 10;
    int start_angle = 0; // Start angle in degrees
    int end_angle = 270; // End angle in degrees
    int delta = 1;       // Angle step

    for (int angle = start_angle; angle <= end_angle; angle += delta)
    {
        int x = x0 + r * cos(angle * PI / 180);
        int y = y0 + r * sin(angle * PI / 180);
        tft.drawPixel(x, y, RED);
    }
}

void drawAntiClockwiseArc()
{
    int x0 = 157.5;
    int y0 = 167.5;
    int r = 10;
    int start_angle = 0; // Start angle in degrees
    int end_angle = 270; // End angle in degrees
    int delta = 1;       // Angle step

    for (int angle = start_angle; angle <= end_angle; angle += delta)
    {
        int x = x0 - r * cos(angle * PI / 180);
        int y = y0 + r * sin(angle * PI / 180);
        tft.drawPixel(x, y, RED);
    }
}

void movesteps(int st, bool d, float dt)
{
    pinMode(dirr, OUTPUT);
    pinMode(step, OUTPUT);

    digitalWrite(dirr, LOW);
    digitalWrite(step, LOW);

    delay(500);
    if (d)
    {
        digitalWrite(dirr, HIGH);
    }
    else
    {
        digitalWrite(dirr, LOW);
    }
    for (int i = 0; i < st; i++)
    {

        digitalWrite(step, HIGH);
        delay(dt);
        digitalWrite(step, LOW);
        delay(dt);
    }
}

void movesteps_reset(int st, bool d, int angle)
{
    // pinMode(dirr,OUTPUT);
    // pinMode(step,OUTPUT);

    digitalWrite(dirr, LOW);
    digitalWrite(step, LOW);

    delay(500);
    if (d)
    {
        digitalWrite(dirr, HIGH);
    }
    else
    {
        digitalWrite(dirr, LOW);
    }

    tft.fillRect(10, 180, 80, 40, BLACK);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(1);
    tft.setCursor(20, 190);
    tft.print("RESET!");

    for (int i = 0; i < st; i++)
    {

        digitalWrite(step, HIGH);
        delay(20);
        digitalWrite(step, LOW);
        delay(20);
    }
}

void setup()
{

    // movesteps(360, 0);

    tft.begin();
    tft.setRotation(3);
    ts.InitTouch();
    ts.setPrecision(PREC_EXTREME);
    tft.fillScreen(ILI9341_BLACK);

    // Angle(TEXT=====================================================================)
    //  tft.fillRect(20, 60, 40, 40, WHITE);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(12, 38);
    tft.print("Angle");

    //=========================================================================

    // increase button
    tft.fillRect(20, 60, 40, 40, WHITE); // increase
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(4);
    tft.setCursor(30, 65);
    tft.print("+");

    // decrease button
    tft.fillRect(20, 110, 40, 40, WHITE); // decrease
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(4);
    tft.setCursor(30, 115);
    tft.print("-");

    // angle box
    //  tft.fillRect(10, 180, 60, 40, WHITE);
    //  tft.setTextColor(ILI9341_WHITE);
    //  tft.setTextSize(2);
    //  tft.setCursor(10,162);
    //  tft.print("Angle:");

    //=========================================================================

    // resolution
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1); // Resoltuion -1
    tft.setCursor(75, 80);
    tft.print("Resolution:");

    tft.fillRect(75, 90, 25, 25, WHITE);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1); // Resoltuion -10
    tft.setCursor(86, 100);
    tft.print("1");

    tft.fillRect(110, 90, 25, 25, WHITE);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1); // Resoltuion -10
    tft.setCursor(115, 100);
    tft.print("10");

    tft.fillRect(145, 90, 25, 25, WHITE);
    tft.setTextColor(ILI9341_BLACK); // Resoltuion -100
    tft.setTextSize(1);
    tft.setCursor(149, 100);
    tft.print("100");
    //===================================================================

    // Direction====================================================

    tft.setTextColor(ILI9341_WHITE); // Resoltuion -100
    tft.setTextSize(1);
    tft.setCursor(105, 135);
    tft.print("Direction");

    drawClockwiseArc(); // clockwise
    tft.drawLine(107, 157, 103, 152, YELLOW);
    tft.drawLine(107, 157, 103, 161, YELLOW);
    tft.drawRect(90, 149, 35, 35, CYAN);
    drawAntiClockwiseArc(); // anticlockwise
    tft.drawLine(157, 157, 161, 153, YELLOW);
    tft.drawLine(157, 157, 161, 161, YELLOW);
    tft.drawRect(140, 149, 35, 35, CYAN);

    tft.setTextColor(ILI9341_WHITE); ////reset
    tft.setTextSize(1);
    tft.setCursor(190, 70);
    tft.print("RESET");
    tft.fillRect(190, 80, 30, 30, CYAN);

    tft.fillRect(250, 50, 61, 30, GREEN); // START
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.setCursor(252, 56);
    tft.print("START");

    // tft.fillRect(250, 110, 61, 30, RED);        //STOP
    // tft.setTextColor(BLACK);
    // tft.setTextSize(2);
    // tft.setCursor(257,118);
    // tft.print("STOP");

    tft.fillRect(210, 165, 40, 35, YELLOW); // TOGGLE ANgle
    tft.setTextColor(BLACK);
    tft.setTextSize(1);
    tft.setCursor(212, 171);
    tft.print("TOGGLE");
    tft.println();
    tft.setCursor(215, 181);
    tft.print("ANGLE");

    tft.fillRect(261, 165, 40, 35, YELLOW); // TOGGLE speed
    tft.setTextColor(BLACK);
    tft.setTextSize(1);
    tft.setCursor(264, 171);
    tft.print("TOGGLE");
    tft.println();
    tft.setCursor(267, 181);
    tft.print("SPEED");

    // drawSlider(50);                             //slider
}
int angle = 0, resolution = 1, speed = 0;
float delay_time = 1;
bool dir = 1, start = 0, r_dir = 0, stop_ = 0, reset, toggle = 1;

void loop()
{

    int x = 0, y = 0;

    //------------------------------------------------------------------
    while (ts.dataAvailable())
    {
        ts.read();
        x = ts.getX();
        y = ts.getY();

        // toggle angle======================================
        if ((x >= 210 && x <= 250) && (y >= 165 && y <= 200))
        {
            toggle = 1;
            tft.fillRect(11, 38, 90, 20, BLACK);
            tft.setTextColor(WHITE);
            tft.setTextSize(2);
            tft.setCursor(12, 38);
            tft.print("ANGLE");

            tft.fillRect(10, 180, 60, 40, BLACK);
            tft.setTextColor(ILI9341_RED);
            tft.setTextSize(2);
            tft.setCursor(20, 190);
            tft.print(angle);

            delay(200);
        }

        // toggle speed======================================
        if ((x >= 261 && x <= 301) && (y >= 165 && y <= 200))
        {
            toggle = 0;
            tft.fillRect(11, 38, 90, 20, BLACK);
            tft.setTextColor(WHITE);
            tft.setTextSize(2);
            tft.setCursor(12, 38);
            tft.print("SPEED");

            tft.fillRect(10, 180, 60, 40, BLACK);
            tft.setTextColor(ILI9341_RED);
            tft.setTextSize(2);
            tft.setCursor(20, 190);
            tft.print(speed);

            delay(200);
        }

        // increase angle
        else if ((x >= 20 && x <= 60) && (y >= 60 && y <= 100) && toggle == 1)
        {

            angle += resolution;

            tft.fillRect(10, 180, 60, 40, BLACK);
            tft.setTextColor(ILI9341_RED);
            tft.setTextSize(2);
            tft.setCursor(20, 190);
            tft.print(angle);
            // tft.print("°");
            delay(200);
        }
        // decrease angle======================================
        else if ((x >= 20 && x <= 60) && (y >= 110 && y <= 150) && toggle == 1)
        {
            angle -= resolution;
            tft.fillRect(10, 180, 60, 40, BLACK);
            tft.setTextColor(ILI9341_RED);
            tft.setTextSize(2);
            tft.setCursor(20, 190);
            tft.print(angle);
            delay(200);
        }

        // increase speed

        else if ((x >= 20 && x <= 60) && (y >= 60 && y <= 100) && toggle == 0)
        {

            speed += resolution;

            tft.fillRect(10, 180, 60, 40, BLACK);
            tft.setTextColor(ILI9341_RED);
            tft.setTextSize(2);
            tft.setCursor(20, 190);
            tft.print(speed);
            // tft.print("°");
            delay(200);
            //  toggle=0;
        }
        // decrease speed

        else if ((x >= 20 && x <= 60) && (y >= 110 && y <= 150) && toggle == 0)
        {
            // tft.fillRect(11, 38, 90, 20, BLACK);

            speed -= resolution;

            tft.fillRect(10, 180, 60, 40, BLACK);
            tft.setTextColor(ILI9341_RED);
            tft.setTextSize(2);
            tft.setCursor(20, 190);
            tft.print(speed);
            // tft.print("°");
            delay(200);
        }

        // resolution buttons=============================================================================
        else if ((x >= 75 && x <= 100) && (y >= 90 && y <= 115))
        {
            resolution = 1;
            tft.fillRect(75, 90, 25, 25, GREEN);

            tft.setTextColor(ILI9341_BLACK);
            tft.setTextSize(1); // Resoltuion -1
            tft.setCursor(86, 100);
            tft.print("1");

            tft.fillRect(110, 90, 25, 25, WHITE);
            tft.fillRect(145, 90, 25, 25, WHITE);

            tft.setTextColor(ILI9341_BLACK);
            tft.setTextSize(1); // Resoltuion -10
            tft.setCursor(115, 100);
            tft.print("10");

            tft.fillRect(145, 90, 25, 25, WHITE);
            tft.setTextColor(ILI9341_BLACK); // Resoltuion -100
            tft.setTextSize(1);
            tft.setCursor(149, 100);
            tft.print("100");
        }

        else if ((x >= 110 && x <= 135) && (y >= 90 && y <= 115))
        {
            resolution = 10;

            tft.fillRect(110, 90, 25, 25, GREEN);

            tft.setTextColor(ILI9341_BLACK);
            tft.setTextSize(1); // Resoltuion -10
            tft.setCursor(115, 100);
            tft.print("10");

            tft.fillRect(75, 90, 25, 25, WHITE);
            tft.fillRect(145, 90, 25, 25, WHITE);

            tft.setTextColor(ILI9341_BLACK);
            tft.setTextSize(1); // Resoltuion -1
            tft.setCursor(86, 100);
            tft.print("1");

            tft.fillRect(145, 90, 25, 25, WHITE);
            tft.setTextColor(ILI9341_BLACK); // Resoltuion -100
            tft.setTextSize(1);
            tft.setCursor(149, 100);
            tft.print("100");
        }

        else if ((x >= 145 && x <= 170) && (y >= 90 && y <= 115))
        {
            resolution = 100;

            tft.fillRect(145, 90, 25, 25, GREEN);
            tft.setTextColor(ILI9341_BLACK); // Resoltuion -100
            tft.setTextSize(1);
            tft.setCursor(149, 100);
            tft.print("100");

            tft.fillRect(75, 90, 25, 25, WHITE);
            tft.fillRect(110, 90, 25, 25, WHITE);

            tft.setTextColor(ILI9341_BLACK);
            tft.setTextSize(1); // Resoltuion -10
            tft.setCursor(115, 100);
            tft.print("10");

            tft.setTextColor(ILI9341_BLACK);
            tft.setTextSize(1); // Resoltuion -1
            tft.setCursor(86, 100);
            tft.print("1");
        }
        //============================================================================================

        // direction buttons================================================================================================
        else if ((x >= 90 && x <= 125) && (y >= 149 && y <= 184))
        {
            dir = 1;
            tft.fillRect(94, 189, 25, 15, BLACK);
            tft.fillRect(141, 189, 35, 15, BLACK);
            tft.setTextColor(RED);
            tft.setTextSize(2);
            tft.setCursor(95, 190);
            tft.print("CW");
        }
        else if ((x >= 140 && x <= 175) && (y >= 149 && y <= 184))
        {
            dir = 0;
            tft.fillRect(94, 189, 25, 15, BLACK);
            tft.fillRect(141, 189, 35, 15, BLACK);
            tft.setTextColor(RED);
            tft.setTextSize(2);
            tft.setCursor(142, 190);
            tft.print("ACW");
        }

        // start-stop=====================================
        else if ((x >= 250 && x <= 311) && (y >= 50 && y <= 80))
        {
            start = 1; // start
        }
        else if ((x >= 250 && x <= 311) && (y >= 110 && y <= 140))
        {
            start = 0; // stop
        }

        // reset button=============================================
        else if ((x >= 190 && x <= 220) && (y >= 80 && y <= 110))
        {
            reset = 1;
        }
    }

    float fsteps = angle / 1.8;
    int steps = floor(fsteps);
    delay_time = 300 / ((float)speed);

    // directionPIN-driver part====================================================

    // stepsPIN-driver part=========================================================

    if (start)
    {
        tft.setTextColor(WHITE);
        tft.setTextSize(1);
        tft.setCursor(252, 97);
        tft.print("started");

        movesteps(steps, dir, delay_time);

        start = 0;
        tft.setTextColor(BLACK);
        tft.setTextSize(1);
        tft.setCursor(252, 97);
        tft.print("started");
    }

    if (reset)
    {
        movesteps_reset(steps, !dir, angle);
        angle = 0;
        reset = 0;
    }
}

// void drawSlider(int value) {
//   tft.fillRect(SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT, ILI9341_BLACK); // Clear previous slider
//   int sliderWidth = map(value, MIN_SPEED, MAX_SPEED, 0, SLIDER_WIDTH);
//   tft.fillRect(SLIDER_X, SLIDER_Y, sliderWidth, SLIDER_HEIGHT, GREEN); // Draw slider
//  }