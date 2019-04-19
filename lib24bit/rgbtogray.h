#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "../maxmin.h"
#include "../lib8bit/histogram.h"

/*/
  void gray_by_color(image_t *img, image_t *new_img, enum COLOR color)
    image_t *img - obraz z którego będą pobierane dane
    image_t *new_img - obraz do którego będą zapisywane dane
    enum COLOR color - kolor według którego ma być generowana skala szarości

/*/

#ifndef __RGBGRAY__
#define __RGBGRAY__
enum COLOR{
  BLUE,
  GREEN, 
  RED
};

void convert_gray_mem(image_t *dest, image_t *src){
  /*/Alokacja pamięci na nowy obraz/*/
  *dest = (image_t){
    (uint8_t*)malloc(src->head_size+1024),
    (uint8_t*)malloc(src->size / 3),
    src->head_size+1024, 
    src->size / 3,
    src->width,
    src->height
  };
  /*/Tworzenie nowego nagłówka/*/
  fullhead_t head = {
    (infomin_t){ 0x4D42, src->size/3+1078, 0, 0, 1078 },
    (header_t){ 40, src->width, src->height, 1, 8, 0, src->size/3, 0, 0, 256, 0 }
  };
  /*/Ustawianie kolorów w tablicy rgb/*/
  for(uint32_t i = 0; i < 256; ++i)
    { head.rgb[i] = (rgb_t){(uint8_t)i, (uint8_t)i, (uint8_t)i, 0}; }

  memcpy(dest->header, &head, sizeof(fullhead_t));
  memset(dest->pixels, 255, dest->size);
}


void rgb_to_gray(image_t *img, image_t *new_img){
  uint32_t x, avg, p = 0;
  /*/Przygotowanie pustego pliku/*/
  convert_gray_mem(new_img, img);
  for(x = 0; x < img->size; x+=3){
    avg = (img->pixels[x] + img->pixels[x+1] + img->pixels[x+2])/3;
    new_img->pixels[p++] = avg;
  }
}
void gray_by_color(image_t *img, image_t *new_img, enum COLOR color){
  uint32_t x, p = 0;
  /*/Przygotowanie pustego pliku/*/
  convert_gray_mem(new_img, img);
  for(x = 0; x < img->size; x+=3)
    { new_img->pixels[p++] = img->pixels[x + color]; }
  draw_histogram(new_img);
}

/*******Tylko jeden kolor na obrazie********/
void color_mask(image_t *img, void(*mask)(uint8_t*, uint8_t*, uint8_t*)){
  for(uint32_t x = 0; x < img->size; x+=3)
    { mask(&img->pixels[x+2], &img->pixels[x+1], &img->pixels[x]); }
}
void red_mask(uint8_t* r, uint8_t* g, uint8_t* b){
  if( *r < *g || *r < *b ){
    uint16_t avg = (*r + *g + *b) / 3;
    *r = avg; *g = avg; *b = avg;
  }
}
void green_mask(uint8_t* r, uint8_t* g, uint8_t* b){
  if( *g < *r || *g < *b ){
    uint16_t avg = (*r + *g + *b) / 3;
    *r = avg; *g = avg; *b = avg;
  }
}
void blue_mask(uint8_t* r, uint8_t* g, uint8_t* b){
  if( *b < *g || *b < *r ){
    uint16_t avg = (*r + *g + *b) / 3;
    *r = avg; *g = avg; *b = avg;
  }
}
/****************************************/

#endif
