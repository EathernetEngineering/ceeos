// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <io/kprint.h>

#include <panic.h>
#include <io/serial.h>

#include <ctype.h>
#include <itoa.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

struct fmt_sink;

typedef void(*sink_putc)(struct fmt_sink *sink, char c);
typedef void(*sink_write)(struct fmt_sink *sink, const char *buf, size_t len);

struct fmt_sink {
	sink_putc putc;
	sink_write write;
	int ec;
	size_t pos;
	void *ctx;
};

static void serial_sink_putc(struct fmt_sink *sink, char c);
static void serial_sink_write(struct fmt_sink *sink, const char *buf, size_t len);
static void string_sink_putc(struct fmt_sink *sink, char c);
static void string_sink_write(struct fmt_sink *sink, const char *buf, size_t len);
static void sink_emit_buf(struct fmt_sink *sink, const char *bug, size_t len);
static void sink_emit_padding(struct fmt_sink *sink, char pad_char, int count);
static int vformat(struct fmt_sink *sink, const char *fmt, va_list args);

static struct fmt_sink g_serial_sink = {
	.putc = serial_sink_putc,
	.write = serial_sink_write,
	.ec = 0,
	.pos = 0,
	.ctx = NULL
};

int kvprintf(const char *fmt, va_list args)
{
	int ret;
	g_serial_sink.pos = 0;
	g_serial_sink.ec = 0;

	ret = vformat(&g_serial_sink, fmt, args);
	if (ret < 0)
		return -g_serial_sink.ec;
	return ret;
}

int kprintf(const char *fmt, ...)
{
	int ret;
	va_list args;
	va_start(args, fmt);
	ret = kvprintf(fmt, args);
	va_end(args);

	return ret;
}

int kvsprintf(char *restrict dst, const char *restrict fmt, va_list args)
{
	struct fmt_sink sink = { 
		.putc = string_sink_putc,
		.write = string_sink_write,
		.ec = 0,
		.pos = 0,
		.ctx = (void *)dst,
	};
	int ret = vformat(&sink, fmt, args);
	if (ret < 0)
		return -sink.ec;
	dst[sink.pos] = '\0';
	return ret;
}

int ksprintf(char *restrict dst, const char *restrict fmt, ...)
{
	int ret;
	va_list args;
	va_start(args, fmt);
	ret = kvsprintf(dst, fmt, args);
	va_end(args);

	return ret;
}

static void serial_sink_putc(struct fmt_sink *sink, char c)
{
	serial_out(c);
	sink->pos++;
}

static void serial_sink_write(struct fmt_sink *sink, const char *buf, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		serial_out(buf[i]);
	}
	sink->pos += len;
}

static void string_sink_putc(struct fmt_sink *sink, char c)
{
	char *str = (char *)sink->ctx;
	if (str)
		str[sink->pos] = c;
	sink->pos++;
}

static void string_sink_write(struct fmt_sink *sink, const char *buf, size_t len)
{
	char *str = (char *)sink->ctx;
	if (str)
		memcpy(&str[sink->pos], buf, len);
	sink->pos += len;
}

static void sink_emit_buf(struct fmt_sink *sink, const char *buf, size_t len)
{
	sink->write(sink, buf, len);
}

static void sink_emit_padding(struct fmt_sink *sink, char pad_char, int count)
{
	for (int i = 0; i < count; i++) {
		sink->putc(sink, pad_char);
	}
}

static int vformat(struct fmt_sink *sink, const char *fmt, va_list args)
{
	long val_i;
	unsigned long val_u;
	char *val_s;
	char val_c;
	char pad;
	size_t width;
	char spec;
	bool long_flag;

	char c;
	while ((c = *fmt) && sink->ec == 0) {
		const char *literal = fmt;
		while (c && c != '%')
			c = *fmt++;
		if (fmt != literal)
			sink_emit_buf(sink, literal, (size_t)(fmt - literal - 1));
		if ((c = *fmt) == '\0' || sink->ec != 0)
			break;

		// skip first '%'
		fmt++;

		pad = ' ';
		if (c == '0') {
			pad = '0';
			c = *fmt++;
		}

		width = 0;
		while (isdigit(c)) {
			width = width * 10 + (c - '0');
			c = *fmt++;
		}

		long_flag = false;
		if (c == 'l') {
			long_flag = true;
			spec = *fmt++;
		} else
			spec = c;

		if (spec == '\0')
			break;

		switch (spec) {
			case '%': {
				sink->putc(sink, '%');
				break;
			}
			case 'c': {
				val_c = (char)va_arg(args, int);
				sink->putc(sink, val_c);
				break;
			}
			case 's': {
				val_s = va_arg(args, char *);
				size_t len = strlen(val_s);
				if (width > 0 && (size_t)width > len) {
					sink_emit_padding(sink, pad, width - len);
				}
				sink_emit_buf(sink, val_s, len);
				break;
			}
			case 'd':
			case 'i': {
				if (long_flag) {
					val_i = va_arg(args, long);
				} else {
					val_i = va_arg(args, int);
				}
				char num_buf[32];
				int num_len = ltoa(val_i, num_buf, 10);
				if (width > 0 && (size_t)width > (size_t)num_len) {
					sink_emit_padding(sink, pad, width - num_len);
				}
				sink_emit_buf(sink, num_buf, num_len);
				break;
			}
			case 'u':
			case 'x':
			case 'X':
			case 'p':
			case 'b': {
				if (long_flag) {
					val_u = va_arg(args, unsigned long);
				} else {
					val_u = va_arg(args, unsigned int);
				}
				int radix = 10;
				if (spec == 'x' || spec == 'X' || spec == 'p') {
					radix = 16;
				} else if (spec == 'b') {
					radix = 2;
				}
				char num_buf[32];
				int num_len = ultoa(val_u, num_buf, radix);
				if (c == 'x') {
					// Convert to lowercase for 'x' format
					for (int i = 0; i < num_len; i++) {
						num_buf[i] = tolower(num_buf[i]);
					}
				}
				if (width > 0 && (size_t)width > (size_t)num_len) {
					sink_emit_padding(sink, pad, width - num_len);
				}
				sink_emit_buf(sink, num_buf, num_len);
				break;
			}
			default:
				// Unknown format specifier, emit it as-is
				sink->putc(sink, '%');
				sink->putc(sink, spec);
				break;
		}
	}

	return sink->pos;
}

