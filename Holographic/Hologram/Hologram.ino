#include "MCUFRIEND_kbv.h"
MCUFRIEND_kbv tft;

#include "hologram.h"

#define MARILYN 
#undef TRACTOR10
#undef HARALD
#undef CLOUDS

#ifdef MARILYN
#include "marilyn_RGB.h"
#endif
#ifdef TRACTOR10
#include "tractor10_RGB.h"
#endif
#ifdef HARALD
#include "Harald_RGB.h"
#endif
#ifdef CLOUDS
#include "clouds_RGB.h"
#endif

void setup()
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();

    Serial.print(F("ID = 0x"));
    Serial.println(ID, HEX);

    tft.begin(ID);
}

// GFX drawBitmap() expects each row to be on 8-bit boundary.  i.e. pad = 7
// Mono BMP rows are on 32-bit boundary. i.e. pad = 31,  rows are in RVS order.
// FreeFont bitmaps have no padding.  i.e. pad = 0.   e.g. 5x4 bitmap is in 3 bytes
void showbgd(int x, int y, const uint8_t *bmp, int w, int h, uint16_t color, uint16_t bg, uint8_t pad = 7)
{
    int yy, xx;
    uint8_t first = 1, RVS = pad & 0x80;
    uint16_t pixel;
    pad &= 31;
    uint16_t wid = (w + pad) & ~pad;                    //bits per row
    tft.setAddrWindow(x, y, x + w - 1, y + h - 1); //
    for (yy = 0; yy < h; yy++) {
        uint32_t ofs = (RVS ? (h - yy - 1) : yy) * wid; //bit offset
        const uint8_t *p = bmp + (ofs >> 3);            //flash address
        uint8_t mask = 0x80 >> (ofs & 7);               //bit mask
        uint8_t c = pgm_read_byte(p++);
        for (xx = 0; xx < w; xx++) {
            if (mask == 0) {
                c = pgm_read_byte(p++);
                mask = 0x80;
            }
            pixel = (c & mask) ? color : bg;
            tft.pushColors(&pixel, 1, first);
            first = 0;
            mask >>= 1;
        }
    }
    tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1);
}

void loop(void)
{
#ifdef MARILYN
  tft.fillScreen(BLACK);
  hologram(marilyn_64x64, 64, 64);
#endif
 
#ifdef TRACTOR10
  tft.fillScreen(WHITE);
  hologram(tractor10_96x64, 96, 64);
#endif

#ifdef HARALD
  tft.fillScreen(BLACK);
  hologram(Harald_64x64, 64, 64);
#endif

#ifdef CLOUDS
  tft.fillScreen(0xBDF7); /* a color used in the image */
  hologram(clouds_64x64, 64, 64);
#endif

    
  delay(5000);

}
