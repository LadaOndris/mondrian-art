
/*********************************************
* Description - Generates modrian art into and writes it to a bmp file.
* Author - Ladislav Ondris
* Date - led 24 2019
* *******************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "bitmap.c"

#define MIN_OFFSET 70
#define MAX_OFFSET 200

typedef struct {
	int left;
	int right;
	int top;
	int bottom;
} rectangle_t;

bool random_decision(float propability) {
	int rand_num = rand() % 100;
	return rand_num <= (propability * 100);
}

// Decides whether the given rect should be devided into two pieces.
bool should_devide(rectangle_t rect) {
	int hor_diff = rect.right - rect.left;
	int ver_diff = rect.bottom - rect.top;
	if (hor_diff < MIN_OFFSET || ver_diff < MIN_OFFSET) {
		return false;
	}
	if (hor_diff > MAX_OFFSET || ver_diff > MAX_OFFSET) 
		return true;
	return random_decision(.7f);
}

void draw_rectangle(bitmap_t *bmp, rectangle_t rect, color_t color) {
	for (int y = rect.top; y < rect.bottom; y++) {
		for (int x = rect.left; x < rect.right; x++) {
			bmp_set_pixel(bmp, y, x, color);
		}
	}
}

int rand_int(int min, int max) {
	return rand() % (max - min) + min;
}

// Make sure that the separator doesn't generate too thin rectangle.
int rand_sep(int min, int max, float offset) {
	int minimal_distance = offset * (max - min);

	int separator;
	do {
		separator = rand_int(min, max);
	}
	while (separator - min < minimal_distance || max - separator < minimal_distance);
	
	return separator;
}

void gen_modrian_art(bitmap_t *bmp, rectangle_t rect, bool last_division)
{	
	if (should_devide(rect)) {
		last_division = !last_division;

		if (last_division) {
			int y_separator = rand_sep(rect.left, rect.right, .3f);
			rectangle_t left_rect = { rect.left, y_separator - 1, rect.top, rect.bottom }; 
			rectangle_t right_rect = { y_separator, rect.right, rect.top, rect.bottom }; 
			gen_modrian_art(bmp, left_rect, last_division);
			gen_modrian_art(bmp, right_rect, last_division);
		}
		else {
			int x_separator = rand_sep(rect.top, rect.bottom, .3f);
			rectangle_t top_rect = { rect.left, rect.right, rect.top, x_separator - 1 };
			rectangle_t bottom_rect = { rect.left, rect.right, x_separator, rect.bottom };
			gen_modrian_art(bmp, top_rect, last_division);
			gen_modrian_art(bmp, bottom_rect, last_division);
		}
	}
	else {
		color_t color = { 255, rand() % 256, rand() % 256, rand() % 256 };
		draw_rectangle(bmp, rect, color);
	}
}

int main() {
	srand(time(NULL));

	int width = 1000, height = 1000;
	bitmap_t *bmp = bitmap_new(width, height);
	rectangle_t rect = { 0, bmp->infoheader.width, 0, bmp->infoheader.height };
	gen_modrian_art(bmp, rect, true);
	save_bmp_to_file(bmp, "modrian_art.bmp");
	bitmap_destroy(bmp);	
}
