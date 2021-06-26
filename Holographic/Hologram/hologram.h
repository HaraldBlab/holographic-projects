#ifndef HOLOGRAM_H
#define HOLOGRAM_H

#include <Arduino.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410
#define ORANGE  0xE880

/*
 * Gets the mirrored pixel(xp,xp) of the image at (xi,yi) for the top image. 
 */
uint16_t top_pixel(const int xi, const int yi, const uint16_t *bmp, const int w, const int h) {
  int xp = w-1-xi;
  int yp = yi;
  const uint16_t *p = bmp + yp * w + xp;
  uint16_t pixel = pgm_read_word(p);
  return pixel;
}

/*
 * Gets the mirrored pixel(xp,xp) of the image at (xi,yi) for the left image. 
 */
uint16_t left_pixel(const int xi, const int yi, const uint16_t *bmp, const int w, const int h) {
  int xp = yi;
  int yp = xi;
  const uint16_t *p = bmp + yp * h + xp;
  uint16_t pixel = pgm_read_word(p);
  return pixel;
}

/*
 * Gets the mirrored pixel(xp,xp) of the image at (xi,yi) for the right image. 
 */
uint16_t right_pixel(const int xi, const int yi, const uint16_t *bmp, const int w, const int h) {
  int xp = h-1-yi;
  int yp = w-1-xi;
  const uint16_t *p = bmp + yp * h + xp;
  uint16_t pixel = pgm_read_word(p);
  return pixel;
}
/*
 * Gets the mirrored pixel(xp,xp) of the image at (xi,yi) for the bottom image. 
 */
uint16_t bottom_pixel(const int xi, const int yi, const uint16_t *bmp, const int w, const int h) {
  int xp = xi;
  int yp = h-1-yi;
  const uint16_t *p = bmp + yp * w + xp;
  uint16_t pixel = pgm_read_word(p);
  return pixel;
}

/*
 * draws the bitmap (bmp) of size(w,h) at (x,y) to screen. Pixel positions are translated with t function.
 */
void drawRGBBitmap(int x, int y, const uint16_t *bmp, int w, int h, uint16_t (*t)(const int, const int, const uint16_t*, const int, const int )) {
    int yy, xx;
    uint8_t first = 1;
    tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
    for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
            uint16_t pixel = t(xx, yy, bmp, w, h);
            tft.pushColors(&pixel, 1, first);
            first = 0;
        }
    }
    tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1);
}

/*
 * Draws a hologram of the bitmap (bmp) with size (w,h).
 * Draws four mirrored copies of the image (top, left, right, buttom.
 * view the hologram with a projection pyramide.
 */
void hologram(const uint16_t *bmp, int w, int h) {
    int top_x = (tft.width() - w) / 2;
    int top_y = (tft.height() - tft.width()) / 2;
    drawRGBBitmap(top_x, top_y, bmp, w, h, &top_pixel);

    int left_x = 0;
    int left_y = (tft.height() - w) / 2;
    drawRGBBitmap(left_x, left_y, bmp, h, w, &left_pixel);

    int right_x = tft.width()-h;
    int right_y = (tft.height() - w) / 2;
    drawRGBBitmap(right_x, right_y, bmp, h, w, &right_pixel);
    
    int bottom_x = (tft.width() - w) / 2;
    int bottom_y = tft.height() - (tft.height() - tft.width()) / 2 - h;
    drawRGBBitmap(bottom_x, bottom_y, bmp, w, h, &bottom_pixel);
}

#endif
