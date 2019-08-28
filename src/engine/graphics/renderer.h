#ifndef RENDERER_H
#define RENDERER_H

enum {
	STYLE_LIGHT,
	STYLE_LIGHT_ITALIC,
	STYLE_REGULAR,
	STYLE_ITALIC,
	STYLE_BOLD,
	STYLE_BOLD_ITALIC,
	STYLE_EXTRABOLD,
	STYLE_EXTRABOLD_ITALIC,
	STYLE_SEMIBOLD,
	STYLE_SEMIBOLD_ITALIC
};

int render_init(int width, int height, const char *title);
void render_quit();

void render_clear();
void render_present();
void render_color(int r, int g, int b, int a);
void render_rect(float x, float y, float width, float height, int filled);
void render_texture2D(float x, float y, float width, float height, unsigned int tex);
void render_line(float x1, float y1, float x2, float y2);
void render_text_color(int r, int g, int b, int a);
void render_text_size(const char* text, int pt, int style, int *w, int *h);
void render_text(int pt, int style, const char *text, float x, float y);

#endif
