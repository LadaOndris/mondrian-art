
#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"

static bmp_header_t get_default_header(int width, int height) {
	bmp_header_t header;
	header.type = 0x4D42;
	header.size = 14 + 40 + 4 * (width * height);
	header.reserved1 = 0x0;
	header.reserved2 = 0x0;
	header.offset = 14 + 40;
	return header;	
}

static bmp_infoheader_t get_default_infoheader(int width, int height) {
	bmp_infoheader_t ih;
	ih.size = 40;
	ih.width = width;
	ih.height = height;
	ih.planes = 1;
	ih.bit_count = 32; // Number of bits per pixel
	ih.compression = 0; // None, no compression used
	ih.size_image = 4 * (width * height);
	ih.x_pels_per_meter = 2835; // Print resolution of the image, 72 DPI × 39.3701 inches per metre yields 2834.6472
	ih.y_pels_per_meter = 2835;
	ih.clr_used = 0;
	ih.clr_important = 0;
	return ih;
}

static bmp_pixelarray_t get_new_pixelarray(int width, int height) {
	bmp_pixelarray_t pa;
	pa.colors = calloc(width * height, sizeof(color_t));
	return pa;
}

void bitmap_init(bitmap_t *bmp, int width, int height) {
	bmp->header = get_default_header(width, height);
	bmp->infoheader = get_default_infoheader(width, height);
	bmp->pixelarray = get_new_pixelarray(width, height);	
}

void bitmap_destroy(bitmap_t *bmp) {
	if (!bmp)
		return;
	if (bmp->pixelarray.colors)
		free(bmp->pixelarray.colors);
	free(bmp);
}

bitmap_t *bitmap_new(int width, int height) {
	bitmap_t *bmp = malloc(sizeof(bitmap_t));
	if (!bmp)
		return NULL;
	bitmap_init(bmp, width, height);
	if (!bmp->pixelarray.colors) {
		bitmap_destroy(bmp);
		return NULL;
	}
	return bmp;
}

void bmp_write_pixels(bitmap_t *bmp, FILE *file) {
	color_t *colors = bmp->pixelarray.colors;
	for (int r = 0; r < bmp->infoheader.height; r++) {
		for (int c = 0; c < bmp->infoheader.width; c++) {
			fwrite(&colors[r * bmp->infoheader.width + c].r, sizeof(char), 1, file);
			fwrite(&colors[r * bmp->infoheader.width + c].g, sizeof(char), 1, file);
			fwrite(&colors[r * bmp->infoheader.width + c].b, sizeof(char), 1, file);
			fwrite(&colors[r * bmp->infoheader.width + c].a, sizeof(char), 1, file);
		}
	}
}

void bmp_set_pixel(bitmap_t *bmp, int r, int c, color_t color) {
	int index = r * bmp->infoheader.width + c; 
	bmp->pixelarray.colors[index] = color;
}

int save_bmp_to_file(bitmap_t *bmp, char *filename) {
	FILE *file;
	if ((file = fopen(filename, "wb")) == NULL) {
		return 1;
	}

	fwrite(&bmp->header.type, 2, 1, file);
	fwrite(&bmp->header.size, 4, 1, file);
	fwrite(&bmp->header.reserved1, 2, 1, file);
	fwrite(&bmp->header.reserved2, 2, 1, file);
	fwrite(&bmp->header.offset, 4, 1, file);
	
	fwrite(&bmp->infoheader.size, 4, 1, file);
	fwrite(&bmp->infoheader.width, 4, 1, file);
	fwrite(&bmp->infoheader.height, 4, 1, file);
	fwrite(&bmp->infoheader.planes, 2, 1, file);
	fwrite(&bmp->infoheader.bit_count, 2, 1, file);
	fwrite(&bmp->infoheader.compression, 4, 1, file);
	fwrite(&bmp->infoheader.size_image, 4, 1, file);
	fwrite(&bmp->infoheader.x_pels_per_meter, 4, 1, file);
	fwrite(&bmp->infoheader.y_pels_per_meter, 4, 1, file);
	fwrite(&bmp->infoheader.clr_used, 4, 1, file);
	fwrite(&bmp->infoheader.clr_important, 4, 1, file);

	bmp_write_pixels(bmp, file);

	fclose(file);
	return 0;
}

