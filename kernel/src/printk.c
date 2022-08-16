
#include <kernel.h>

#include "serial.h"
#include "terminal.h"

// TODO: use common console
#define printk_putc serial_write

const char seq[] = "0123456789abcdef";

void print_num(uint64_t num, int base)
{
	if (base > 16)
		return;

	if (num / base != 0)
		print_num(num / base, base);

	printk_putc(seq[num % base]);
	return;
}

void print_str(char* str)
{
	while (*str) 
	{
		printk_putc(*str);
		str++;
	}
}

int printk(const char* fmt, ...) 
{
	va_list arg_list;
	va_start(arg_list, fmt);
	bool in_fmtseq = false;

	while (*fmt) 
	{
		switch (*fmt) 
		{
		case '%':
			in_fmtseq = true;
			fmt++;
			break;
		default:
			printk_putc(*fmt);
			break;
		}

		if (in_fmtseq) 
		{
			switch(*fmt) 
			{
			case 'd':
			{
				int32_t n;
				n = va_arg(arg_list, int64_t);
				print_num(n, 10);
				break;
			}
			case 'x':
			{
				uint32_t n;
				n = va_arg(arg_list, uint64_t);
				print_num(n, 16);
				break;
			}
			case 's':
			{
				char* s = va_arg(arg_list, char*);
				print_str(s);
				break;
			}
			default:
				in_fmtseq = false;
				break;
			}
		}

		fmt++;
	}
}
