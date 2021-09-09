
#include <kernel.h>

#include <terminal.h>

int printk(const char* fmt, ...) {

	/* TODO: improve this function to be less wonky */

	va_list arg_list;
	va_start(arg_list, fmt);

	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			if (*fmt == 'd') {
				uint32_t _arg = va_arg(arg_list, uint32_t);

				int32_t i = 0;
				char _buf[10];

				while (_arg) {
					_buf[i] = (_arg % 10) + '0';
					_arg /= 10;
					i++;
				}

				while (i) {
					i--;
					term_putc(_buf[i]);
				}

				fmt++;
				continue;
			}
			else if (*fmt == 'x') {
				uint32_t _arg = va_arg(arg_list, uint32_t);

				int32_t i = 0;
				char _buf[10];

				while (_arg) {
					_buf[i] = (_arg % 16) + '0';
					if (_buf[i] > '9') {
						_buf[i] += 7;
					}
					_arg /= 16;
					i++;
				}

				while (i) {
					i--;
					term_putc(_buf[i]);
				}

				fmt++;
				continue;
			}
		}
		term_putc(*fmt);
		fmt++;
	}

}
