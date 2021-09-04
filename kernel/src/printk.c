
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
					tty_putc(_buf[i]);
				}

				fmt++;
				continue;
			}
		}
		tty_putc(*fmt);
		fmt++;
	}

}
