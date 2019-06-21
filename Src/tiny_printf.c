#include <stdarg.h>
#include <stdint-gcc.h>
#include "tiny_printf.h"

extern char __io_putchar(char ch);

static const uint32_t dv[] = {
//  4294967296        // 32 bit unsigned max
    1000000000,      // +0
    100000000,      // +1
    10000000,      // +2
    1000000,      // +3
    100000,      // +4
//  65535       // 16 bit unsigned max
    10000,     // +5
    1000,     // +6
    100,     // +7
    10,     // +8
    1,     // +9
};

static void xtoa(uint32_t x, const uint32_t *dp) {
  char c;
  uint32_t d;
  if(x) {
    while(x < *dp) ++dp;
    do {
      d = *dp++;
      c = '0';
      while(x >= d) ++c, x -= d;
      __io_putchar(c);
    } while(!(d & 1));
  } else
    __io_putchar('0');
}

static inline void __io_putstr(char *s) { while(*s) __io_putchar(*s++); }

static inline void __io_puthex(char n) {
  static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  __io_putchar(hex[n & 15]);
}

void tiny_printf(const char* format, ...) {
  char c;
  int i;

  va_list a;
  va_start(a, format);
  while((c = *format++)) {
    if(c == '%') {
      switch(c = *format++) {
        case 's':                       // String
          __io_putstr(va_arg(a, char*));
          break;
        case 'c':                       // Char
          __io_putchar(va_arg(a, int));
          break;
        case 'u':                       // 32 bit unsigned
          xtoa(va_arg(a, uint32_t), dv);
          break;
        case 'i':                       // 32 bit signed
          i = va_arg(a, int);
          if (i < 0) {
            i = -i;
            __io_putchar('-');
          }
          xtoa((uint32_t)i, dv);
          break;
        case 'x':                       // 32 bit hexadecimal
          i = va_arg(a, int);
          __io_putchar('0');
          __io_putchar('x');
          __io_puthex(i >> 28);
          __io_puthex(i >> 24);
          __io_puthex(i >> 20);
          __io_puthex(i >> 16);
          __io_puthex(i >> 12);
          __io_puthex(i >> 8);
          __io_puthex(i >> 4);
          __io_puthex(i);
          break;
        case 0:
          return;
        default:
          goto bad_fmt;
      }
    } else
      bad_fmt:    __io_putchar(c);
  }
  va_end(a);
}
