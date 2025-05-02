#include <Arduino.h>
#include "ST7567-13064.h"

#define MOSI_PIN 14
#define SCLK_PIN 12
#define DC_PIN 13
#define RST_PIN 11
#define CS_PIN 10

ST7567 display(MOSI_PIN, SCLK_PIN, DC_PIN, RST_PIN, CS_PIN);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16

void testdrawline();
void testdrawrect();
void testfillrect();
void testdrawcircle();
void testdrawroundrect();
void testfillroundrect();
void testdrawtriangle();
void testfilltriangle();
void testdrawchar();
void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h);

static const unsigned char PROGMEM logo16_glcd_bmp[] =
    {0B00000000, 0B11000000,
     0B00000001, 0B11000000,
     0B00000001, 0B11000000,
     0B00000011, 0B11100000,
     0B11110011, 0B11100000,
     0B11111110, 0B11111000,
     0B01111110, 0B11111111,
     0B00110011, 0B10011111,
     0B00011111, 0B11111100,
     0B00001101, 0B01110000,
     0B00011011, 0B10100000,
     0B00111111, 0B11100000,
     0B00111111, 0B11110000,
     0B01111100, 0B11110000,
     0B01110000, 0B01110000,
     0B00000000, 0B00110000};

void setup()
{

    display.begin();
    display.setContrast(45);
    display.setCursor(20,10);
    display.setTextSize(6);
    display.setTextColor(BLACK);
    display.print("HI!");
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw many lines
    testdrawline();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw rectangles
    testdrawrect();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw multiple rectangles
    testfillrect();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw mulitple circles
    testdrawcircle();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw a white circle, 10 pixel radius
    display.fillCircle(display.width() / 2, display.height() / 2, 10, BLACK);
    display.display();
    delay(2000);
    display.clearDisplay();

    testdrawroundrect();
    delay(2000);
    display.clearDisplay();

    testfillroundrect();
    delay(2000);
    display.clearDisplay();

    testdrawtriangle();
    delay(2000);
    display.clearDisplay();

    testfilltriangle();
    delay(2000);
    display.clearDisplay();

    // draw the first ~12 characters in the font
    testdrawchar();
    display.display();
    delay(2000);
    display.clearDisplay();

    for (uint8_t rot = 0; rot < 4; rot++)
    {
        display.setRotation(rot);
        display.clearDisplay();
        // text display tests
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(0, 0);
        display.println("Hello, world!");
        display.setTextColor(WHITE, BLACK); // 'inverted' text
        display.println(3.141592);
        display.setTextSize(2);
        display.setTextColor(BLACK);
        display.print("0x");
        display.println(0xDEADBEEF, HEX);
        display.display();
        delay(2000);
    }

    display.setRotation(0);

    // miniature bitmap display
    display.clearDisplay();
    display.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, BLACK);
    display.display();

    // invert the display
    display.invertDisplay(true);
    delay(2000);
    display.invertDisplay(false);
    delay(2000);

    // draw a bitmap icon and 'animate' movement
    testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

void loop()
{
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h)
{
    uint8_t icons[NUMFLAKES][3];
    randomSeed(666); // whatever seed

    // initialize
    for (uint8_t f = 0; f < NUMFLAKES; f++)
    {
        icons[f][XPOS] = random(display.width() - w);
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
    }

    while (1)
    {
        // draw each icon
        for (uint8_t f = 0; f < NUMFLAKES; f++)
        {
            display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
        }
        display.display();
        delay(100);

        // then erase it + move it
        for (uint8_t f = 0; f < NUMFLAKES; f++)
        {
            display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
            // move it
            icons[f][YPOS] += icons[f][DELTAY];
            // if its gone, reinit
            if (icons[f][YPOS] > display.height())
            {
                icons[f][XPOS] = random(display.width());
                icons[f][YPOS] = 0;
                icons[f][DELTAY] = random(5) + 1;
            }
        }
    }
}

void testdrawchar(void)
{
    display.setTextSize(1);
    display.setTextWrap(false);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);

    for (uint8_t i = 0; i < 168; i++)
    {
        if (i == '\n')
            continue;
        display.write(i);
        if ((i > 0) && (i % 21 == 0))
            display.println();
    }
    display.display();
}

void testdrawcircle(void)
{
    for (uint8_t i = 0; i < display.height(); i += 2)
    {
        display.drawCircle(display.width() / 2, display.height() / 2, i, BLACK);
        display.display();
    }
}

void testfillrect(void)
{
    uint8_t color = 1;
    for (uint8_t i = 0; i < display.height() / 2; i += 3)
    {
        // alternate colors
        display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
        display.display();
        color++;
    }
}

void testdrawtriangle(void)
{
    for (uint16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5)
    {
        display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                             display.width() / 2 - i, display.height() / 2 + i,
                             display.width() / 2 + i, display.height() / 2 + i, BLACK);
        display.display();
    }
}

void testfilltriangle(void)
{
    uint8_t color = WHITE;
    for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5)
    {
        display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                             display.width() / 2 - i, display.height() / 2 + i,
                             display.width() / 2 + i, display.height() / 2 + i, BLACK);
        if (color == BLACK)
            color = WHITE;
        else
            color = BLACK;
        display.display();
    }
}

void testdrawroundrect(void)
{
    for (uint8_t i = 0; i < display.height() / 3 - 2; i += 2)
    {
        display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, BLACK);
        display.display();
    }
}

void testfillroundrect(void)
{
    uint8_t color = BLACK;
    for (uint8_t i = 0; i < display.height() / 3 - 2; i += 2)
    {
        display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
        if (color == BLACK)
            color = WHITE;
        else
            color = BLACK;
        display.display();
    }
}

void testdrawrect(void)
{
    for (uint8_t i = 0; i < display.height() / 2; i += 2)
    {
        display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, BLACK);
        display.display();
    }
}

void testdrawline()
{
    for (uint8_t i = 0; i < display.width(); i += 4)
    {
        display.drawLine(0, 0, i, display.height() - 1, BLACK);
        display.display();
    }
    for (uint8_t i = 0; i < display.height(); i += 4)
    {
        display.drawLine(0, 0, display.width() - 1, i, BLACK);
        display.display();
    }
    delay(2500);

    display.clearDisplay();
    for (uint8_t i = 0; i < display.width(); i += 4)
    {
        display.drawLine(0, display.height() - 1, i, 0, BLACK);
        display.display();
    }
    for (int16_t i = display.height() - 1; i >= 0; i -= 4)
    {
        display.drawLine(0, display.height() - 1, display.width() - 1, i, BLACK);
        display.display();
    }
    delay(2500);

    display.clearDisplay();
    for (int16_t i = display.width() - 1; i >= 0; i -= 4)
    {
        display.drawLine(display.width() - 1, display.height() - 1, i, 0, BLACK);
        display.display();
    }
    for (int16_t i = display.height() - 1; i >= 0; i -= 4)
    {
        display.drawLine(display.width() - 1, display.height() - 1, 0, i, BLACK);
        display.display();
    }
    delay(2500);

    display.clearDisplay();
    for (uint8_t i = 0; i < display.height(); i += 4)
    {
        display.drawLine(display.width() - 1, 0, 0, i, BLACK);
        display.display();
    }
    for (uint8_t i = 0; i < display.width(); i += 4)
    {
        display.drawLine(display.width() - 1, 0, i, display.height() - 1, BLACK);
        display.display();
    }
    delay(2500);
}
