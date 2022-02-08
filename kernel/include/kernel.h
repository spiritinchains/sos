
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

#define inb(port, value)    asm("in %%dx, %%al" : "=a" (value) : "d" (port) );
#define outb(port, value)   asm("out %%al, %%dx" : : "a" (value), "d" (port) );

#define inw(port, value)    asm("in %%dx, %%ax" : "=a" (value) : "d" (port) );
#define outw(port, value)   asm("out %%ax, %%dx" : : "a" (value), "d" (port) );

int printk(const char* format, ...);

// only included for sanity checking cause I screwed up vga once
#define debug_vga_print		*((uint64_t*)0xb8000)=0x0720072107690748
