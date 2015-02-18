#include "fontCommon.h"

// depends on letters in font->Index table being arranged in ascending order
// save location of last lookup to speed up repeated lookups of the same letter
// TODO: use successive approximation to located index faster
int getBitmapFontLocation(unsigned char letter, const bitmap_font *font) {
    static int location = 0;

    if(location < 0)
        location = 0;

    if(font->Index[location] == letter)
        return location;

    if(font->Index[location] < letter) {
        for (; location < font->Chars; location++) {
            if (font->Index[location] == letter)
                return location;
        }
    } else {
        for (; location >= 0; location--) {
            if (font->Index[location] == letter)
                return location;
        }
    }

    return -1;
}

bool getBitmapFontPixelAtXY(unsigned char letter, unsigned char x, unsigned char y, const bitmap_font *font)
{
    int location;
    if (y >= font->Height)
        return false;

    location = getBitmapFontLocation(letter, font);

    if (location < 0)
        return false;

    if (font->Bitmap[(location * font->Height) + y] & (0x80 >> x))
        return true;
    else
        return false;
}

uint16_t getBitmapFontRowAtXY(unsigned char letter, unsigned char y, const bitmap_font *font) {
    int location;
    if (y >= font->Height)
        return 0x0000;

    location = getBitmapFontLocation(letter, font);

    if (location < 0)
        return 0x0000;

    return(font->Bitmap[(location * font->Height) + y]);
}

void drawChar(int16_t x, int16_t y, const CRGB& charColor, char character, const bitmap_font *font) {
    int xcnt, ycnt;

    for (ycnt = 0; ycnt < font->Height; ycnt++) {
        for (xcnt = 0; xcnt < font->Width; xcnt++) {
            if (getBitmapFontPixelAtXY(character, xcnt, ycnt, font)) {
                leds[XY(x + xcnt,y + ycnt)] = charColor;
            }
        }
    }
}

void drawString(int16_t x, int16_t y, const CRGB& charColor, const char text[], const bitmap_font *font) {
    int xcnt, ycnt, i = 0, offset = 0;
    char character;

    // limit text to 10 chars, why?
    for (i = 0; i < 10; i++) {
        character = text[offset++];
        if (character == '\0')
            return;

        for (ycnt = 0; ycnt < font->Height; ycnt++) {
            for (xcnt = 0; xcnt < font->Width; xcnt++) {
                if (getBitmapFontPixelAtXY(character, xcnt, ycnt, font)) {
                    leds[XY(x + xcnt,y + ycnt)] = charColor;
                }
            }
        }
        x += font->Width;
    }
}

// draw string while clearing background
void drawString(int16_t x, int16_t y, const CRGB& charColor, const CRGB& backColor, const char text[], const bitmap_font *font) {
    int xcnt, ycnt, i = 0, offset = 0;
    char character;

    // limit text to 10 chars, why?
    for (i = 0; i < 10; i++) {
        character = text[offset++];
        if (character == '\0')
            return;

        for (ycnt = 0; ycnt < font->Height; ycnt++) {
            for (xcnt = 0; xcnt < font->Width; xcnt++) {
                if (getBitmapFontPixelAtXY(character, xcnt, ycnt, font)) {
                    leds[XY(x + xcnt,y + ycnt)] = charColor;
                } else {
                    leds[XY(x + xcnt,y + ycnt)] = backColor;
                }
            }
        }
        x += font->Width;
    }
}
