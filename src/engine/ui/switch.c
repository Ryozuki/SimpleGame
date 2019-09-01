#include "switch.h"
#include <stdlib.h>
#include <string.h>
#include <engine/util.h>
#include <engine/logger.h>

Switch *switch_create(int w, int h, Color bg, Color offColor, Color onColor) {
	Switch *s = malloc(sizeof(Switch));
	s->rect = util_rect(0, 0, w, h);
	s->bg = bg;
	s->off_color = offColor;
	s->on_color = onColor;
	s->value = 0;
	s->clicked_tick = -1000;
	s->padding = 4;
	s->animation_time = 350;
	return s;
}

void switch_update(Switch *s) {
	if(util_mouse_in_rect(&s->rect) && util_is_mouse_click(BUTTON_LEFT) && util_tick_passed(s->clicked_tick)) {
		s->clicked_tick = util_tick();
		s->value = !s->value;
	}
}

void switch_free(Switch *s) {
	free(s);
}

void render_switch(Switch *s) {
	render_color_s(s->bg);
	render_rect_s(&s->rect, 1);

	if(util_tick_passed(s->clicked_tick + s->animation_time)) {
		if(!s->value) {
			Rect r = util_rect(s->rect.x + s->padding, s->rect.y + s->padding,
					s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2);
			render_color_s(s->off_color);
			render_rect_s(&r, 1);
		}
		else {
			Rect r = util_rect(s->rect.x + s->rect.w / 2, s->rect.y + s->padding,
					s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2);
			render_color_s(s->on_color);
			render_rect_s(&r, 1);
		}
	}
	else {
		float percent = (util_tick() - s->clicked_tick) / (float)(s->animation_time);
		int diff = s->rect.w / 2;
		int current = round(diff * percent);
		if(s->value) {
			Color c;

			c.r = s->off_color.r + round((s->on_color.r - s->off_color.r) * percent);
			c.g = s->off_color.g + round((s->on_color.g - s->off_color.g) * percent);
			c.b = s->off_color.b + round((s->on_color.b - s->off_color.b) * percent);
			c.a = 255;

			Rect r = util_rect(s->rect.x + s->padding + current, s->rect.y + s->padding,
					s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2);
			render_color_s(c);
			render_rect_s(&r, 1);
		}
		else {
			Color c;

			c.r = s->on_color.r + round((s->off_color.r - s->on_color.r) * percent);
			c.g = s->on_color.g + round((s->off_color.g - s->on_color.g) * percent);
			c.b = s->on_color.b + round((s->off_color.b - s->on_color.b) * percent);
			c.a = 255;

			Rect r = util_rect(s->rect.x + s->rect.w / 2 - current, s->rect.y + s->padding,
					s->rect.w / 2 - s->padding, s->rect.h - s->padding * 2);
			render_color_s(c);
			render_rect_s(&r, 1);
		}
	}
}