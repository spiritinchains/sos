
#include <kernel.h>
#include <serial.h>
#include <terminal.h>

// TODO: use common console
#define PUTC term_putc

const char seq[] = "0123456789abcdef";

bool in_fmtseq = false;

void print_num(uint64_t num, int base)
{
	if (base > 16)
		return;

	if (num / base != 0)
		print_num(num / base, base);

	PUTC(seq[num % base]);
	return;
}

void print_str(char* str)
{
	while (*str) 
	{
		PUTC(*str);
		str++;
	}
}

int printk(const char* fmt, ...) 
{
	va_list arg_list;
	va_start(arg_list, fmt);

	while (*fmt) 
	{
		switch (*fmt) 
		{
		case '%':
			in_fmtseq = true;
			fmt++;
			break;
		default:
			PUTC(*fmt);
			break;
		}

		if (in_fmtseq) 
		{
			int n;
			switch(*fmt) 
			{
			case 'd':
				n = va_arg(arg_list, int32_t);
				print_num(n, 10);
				break;
			case 'x':
				n = va_arg(arg_list, int32_t);
				print_num(n, 16);
				break;
			case 's':
				char* s = va_arg(arg_list, char*);
				print_str(s);
				break;
			default:
				in_fmtseq = false;
				break;
			}
		}

		fmt++;
	}
}
