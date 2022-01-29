
#include <kernel.h>
#include <serial.h>
#include <terminal.h>

// TODO: use common console
#ifndef SERIAL_CONSOLE
	#define PUTC term_putc
#else
	#define PUTC serial_write
#endif

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

				if (_arg == 0) {
					_buf[i] = '0';
					i++;
				}

				while (_arg) {
					_buf[i] = (_arg % 10) + '0';
					_arg /= 10;
					i++;
				}

				while (i) {
					i--;
					PUTC(_buf[i]);
				}

				fmt++;
				continue;
			}
			else if (*fmt == 'x') {
				uint32_t _arg = va_arg(arg_list, uint32_t);

				int32_t i = 0;
				char _buf[10];

				if (_arg == 0) {
					_buf[i] = '0';
					i++;
				}

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
					PUTC(_buf[i]);
				}

				fmt++;
				continue;
			}
		}
		PUTC(*fmt);
		fmt++;
	}

}
