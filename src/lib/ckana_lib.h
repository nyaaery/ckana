#pragma once

#include <curses.h>

#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))

float randf();
void trim_end_in_place(char* str);
WINDOW* new_win_center(int h, int w);
