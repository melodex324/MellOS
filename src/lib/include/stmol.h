#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#ifndef STMOL_H
#define STMOL_H

void memset(void *dest, char val, uint32_t count);
void *memcpy(void *dest, const void* src, size_t n);
int memcmp(const void *ptr1, const void *ptr2, size_t n);
void *memmove(void *dest, const void *src, size_t count);

extern size_t strlen(const char* str);
char *strcpy(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);
char *strcat(char *dest, const char *src);
void halt(void);

void reverse(char *str, size_t len);

char *itoa(long long num, char *str, int base);
char *utoa(unsigned long long num, char* str, int base);

void pprintf(uintptr_t ptr);
void vprintf(const char *str, va_list args);
void printf(const char *str, ...);

#endif