
#ifndef _BITMAP_C_
#define _BITMAP_C_

#include <stdint.h>

typedef struct { // total size = 14B
	int16_t type;
	int32_t size;
	int16_t reserved1;
	int16_t reserved2;
	int32_t offset;	
} bmp_header_t;

typedef struct { // total size = 40B
  int32_t size;
  int32_t width;
  int32_t height;
  int16_t planes;
  int16_t bit_count;
  int32_t compression;
  int32_t size_image;
  int32_t x_pels_per_meter;
  int32_t y_pels_per_meter;
  int32_t clr_used;
  int32_t clr_important;
} bmp_infoheader_t;

typedef struct {
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;

typedef struct {
	color_t *colors;
} bmp_pixelarray_t;

typedef struct {
	bmp_header_t header;
	bmp_infoheader_t infoheader;
	bmp_pixelarray_t pixelarray;
} bitmap_t;

bitmap_t *bitmap_new(int width, int height);

void bitmap_destroy(bitmap_t *bmp);

void bmp_set_pixel(bitmap_t *bmp, int r, int c, color_t color);

void bmp_write_pixels(bitmap_t *bmp, FILE *file);

int save_bmp_to_file(bitmap_t *bmp, char *filename);

#endif
