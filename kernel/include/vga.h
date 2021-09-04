
#pragma once

#define VGA_BUFFER_ADDR 0xb8000

#define VGA_CTRL 0x3d4          // register indexed for io over port
#define VGA_DATA 0x3d5          // data in indexed register

#define VGA_REG_CURSOR_START 10
#define VGA_REG_CURSOR_END 11
#define VGA_REG_CURSOR_HI 14
#define VGA_REG_CURSOR_LO 15

#define VGA_ROWS 25
#define VGA_COLS 80

#pragma pack(2)
typedef struct {
    char data;
    char attr;
} vgach_t;

typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vgacolor_t;

void vga_clear();

void vga_putc(char, int);			// Put char at index

void vga_setbg(vgacolor_t, int);  	// Set background color
void vga_setfg(vgacolor_t, int);  	// Set foreground color

void vga_initcursor();

void vga_enablecursor();
void vga_disablecursor();

void vga_getcursor(int*);         	// Get cursor position
void vga_setcursor(int);      		// Set cursor position
