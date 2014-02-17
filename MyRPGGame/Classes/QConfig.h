#pragma once

#define SCREEN_WIDTH			960
#define SCREEN_HEIGHT			640
#define FPS_SHOW				true
#define FPS						60

struct Display
{
	float x;
	float y;
	float w;
	float h;
	float cx;
	float cy;
};

extern Display D_display;