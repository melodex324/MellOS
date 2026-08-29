#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <limine.h>

#include <stmol.h>
#include <display.h>
#include <font.h>

#include <terminal.h>

size_t margins = 0;

void memset(void *dest, char val, uint32_t n)
{
    char *temp = (char*) dest;
    for (; n !=0; n --)
    {
        *temp++ = val;
    }
}

void *memcpy(void *dest, const void* src, size_t n)
{
	if (dest == NULL || src == NULL	)
	{
		return NULL;
	}

	char *char_dest = (char*)dest;
	char *char_src = (char*)src;
	for (size_t i = 0; i < n; i++)
	{
		char_dest[i] = char_src[i];
	}

	return dest;
}

int memcmp(const void *ptr1, const void *ptr2, size_t n)
{
	if (ptr1 == NULL || ptr2 == NULL	)
	{
		return 0;
	}

	const char *p1 = (const char*)ptr1;
	const char *p2 = (const char*)ptr2;

	for (size_t i = 0; i < n; i++)
	{
		if (p1[i] != p2[i])
		{
		    return p1[i] - p2[i];
		}
	}

	return 0;
}

void *memmove(void *dest, const void *src, size_t n)
{
	if (dest == NULL || src == NULL	)
	{
		return NULL;
	}

	char *char_dest = (char*)dest;
	char *char_src = (char*)src;

	if (char_dest == char_src)
	{
		return dest;
	}

	if (char_dest < char_src)
	{
		for (size_t i = 0; i < n; i++)
		{
			char_dest[i] = char_src[i];
		}
	}
	else
	{
		for (size_t i = n; i > 0; i--)
		{
			char_dest[i - 1] = char_src[i - 1];
		}
	}

	return dest;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

char *strcpy(char *dest, const char *src)
{
	size_t i;

	if (dest == NULL || src == NULL	)
	{
		return NULL;
	}

	char *char_dest = (char*)dest;
	char *char_src = (char*)src;
	
	for (i = 0; src[i] != '\0'; i++)
	{
		char_dest[i] = char_src[i];
	}
	char_dest[i] = '\0';

	return dest;
}

int strcmp(const char *str1, const char *str2)
{
	size_t i;

	if (str1 == NULL || str2 == NULL)
	{
		return 0;
	}

	const char *s1 = (const char*)str1;
	const char *s2 = (const char*)str2;

	for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
	{
	    if (s1[i] != s2[i])
	    {
	        return s1[i] - s2[i];
	    }
	}

	return s1[i] - s2[i];
}

char *strcat(char *dest, const char *src)
{
	size_t i;

	if (dest == NULL || src == NULL	)
	{
		return NULL;
	}

	char *char_dest = (char*)dest;
	char *char_src = (char*)src;

	size_t dest_len = strlen(char_dest);

	for (i = 0; src[i] != '\0'; i++)
	{
		char_dest[dest_len + i] = char_src[i];
	}
	char_dest[dest_len +  i] = '\0';
	
	return dest;
}

void halt(void) {
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

void swap(char *a, char *b)                                                                                                                                                                       
  {
       if(!a || !b)
           return;

       char tmp = *(a);
       *(a) = *(b);
       *(b) = tmp;
   }

void reverse(char *str, size_t len)
{
    size_t start = 0;
    size_t end = len - 1;
    while (start < end)
    {
		swap((str+start), (str+end)); 
        start++;
        end--;
    }
}

char *utoa(unsigned long long num, char* str, int base)
{
    int i = 0;

    if (num == 0)
	{
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    while (num != 0)
	{
        unsigned int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num /= base;
    }

    str[i] = '\0';
    reverse(str, i);
    return str;
}

char *itoa(long long num, char* str, int base)
{
    if (num < 0 && base == 10)
	{
		str[0] = '-';
        utoa((unsigned long long)(-(unsigned long long)num), str + 1, base);
		return str;
    }

	utoa((unsigned long long)num, str, base);
    return str;
}

void pprintf(uintptr_t ptr)
{
    if (ptr == 0)
	{
        write_string("(nil)");
        return;
    }

    write_string("0x");

    int len = sizeof(uintptr_t) * 2;
    char buff[17];
    buff[len] = '\0';

    for (int i = len - 1; i >= 0; i--)
	{
        buff[i] = "0123456789abcdef"[ptr & 0xF];
        ptr >>= 4;
    }
    write_string(buff);
}

void vprintf(const char *str, va_list args)
{
	if (str == NULL)
	{
		return;
	}
	
	char itoa_buffer[64];			// Buffer for the ITOA function set to 64 which is overkill

	for (size_t i = 0; str[i] != '\0'; i++)
	{
		if (str[i] != '%')			// this print a character at index i from the "str" string
        {
            write_char(str[i]);
            continue;
        }

        i++;

		if (str[i] == '\0')
        {
			write_char('%')
            break;
        }

		switch (str[i])
		{
			case 'c':						// print a char
				{
					char c = (char)va_arg(args, int);
					write_char(c);
					break;
				}

			case 's':						// print a string
				{
					char *strg = va_arg(args, char*);

					if (strg == NULL)
						write_string("(null)");
					else
						write_string(strg);

					break;
				}

			case 'd':						// print an int
			case 'i':
				{
					int val = va_arg(args, int);
					itoa(val, itoa_buffer, 10);
					write_string(itoa_buffer);
					break;
				}

			case 'u':						// print an unsigned int
				{
					unsigned int val = va_arg(args, unsigned int);
					utoa(val, itoa_buffer, 10);
					write_string(itoa_buffer);
					break;
				}

			case 'h':						// print a short
				i++;
				if (str[i] == 'd') 		// %hd -> short int
				{
					short val = (short)va_arg(args, int);
					itoa(val, itoa_buffer, 10);
					write_string(itoa_buffer);
					break;
				}
				else if (str[i] == 'u')	//%hu -> unsigned short
				{
					unsigned short val = (unsigned short)va_arg(args, unsigned int);
					utoa(val, itoa_buffer, 10);
					write_string(itoa_buffer);
					break;
				}
				else
				{
					i--;
					break;
				}
				
				break;

			case 'l':						// print a long
				i++;
				if (str[i] == 'd') 		// %ld -> long int
				{
					long val = va_arg(args, long);
					itoa(val, itoa_buffer, 10);
					write_string(itoa_buffer);
					break;
				}
				else if (str[i] == 'u') 	//%lu -> unisgned long
				{
					unsigned long val = va_arg(args, unsigned long);
					utoa(val, itoa_buffer, 16);
					write_string(itoa_buffer);
					break;
				}
				else if (str[i] == 'x') 	//%lx -> long hex
				{
					unsigned long val = va_arg(args, unsigned long);
					utoa(val, itoa_buffer, 16);
					write_string(itoa_buffer);
					break;
				}
				else if (str[i] == 'l')	// check for a long long "%ll_"
				{
					i++;
					if (str[i] == 'd')	//%lld -> long long int
					{
						long long val = va_arg(args, long long);
						itoa(val, itoa_buffer, 10);
						write_string(itoa_buffer);
						break;
					}
					else if (str[i] == 'u') 	//%llu -> unisgned long
					{
						unsigned long long val = va_arg(args, unsigned long long);
						utoa(val, itoa_buffer, 10);
						write_string(itoa_buffer);
						break;
					}
					else if (str[i] == 'x') //%llx -> long long hex
					{
						unsigned long long val = va_arg(args, unsigned long long);
						utoa(val, itoa_buffer, 16);
						write_string(itoa_buffer);
						break;
					}
					else
					{
						i--;
						break;
					}
					
				}

				break;

			case 'z':					// print a size_t
				i++;
				if (str[i] == 'x') 		// %zx -> size_t hex
				{
					size_t val = va_arg(args, size_t);
					utoa(val, itoa_buffer, 16);
					write_string(itoa_buffer);
					break;
				}
				else if (str[i] == 'u')	//%zu -> size_t
				{
					size_t val = va_arg(args, size_t);
					utoa(val, itoa_buffer, 10);
					write_string(itoa_buffer);
					break;
				}
				else
				{
					i--;
					break;
				}

				break;
			
			case 'b':						// print a value as a binary
				{
					unsigned int val = va_arg(args, int);
					itoa((int)val, itoa_buffer, 2);
					write_string(itoa_buffer);
					break;
				}

			case 'o':						// print in base 8
				{
					unsigned int val = va_arg(args, int);
					itoa((int)val, itoa_buffer, 8);
					write_string(itoa_buffer);
					break;
				}

			case 'x':						// print a value as an hex
				{
					unsigned int val = va_arg(args, int);
					itoa((int)val, itoa_buffer, 16);
					write_string(itoa_buffer);
					break;
				}
			
			case 'X':						// print a value as an hex in uppercase
				{
					unsigned int val = va_arg(args, int);
					itoa((int)val, itoa_buffer, 16);
					for (size_t x = 0; itoa_buffer[x] != '\0'; x++)
					{
						if (itoa_buffer[x] >= 'a' && itoa_buffer[x] <= 'f')
						{
							itoa_buffer[x] -= 32;
						}
					}
					
					write_string(itoa_buffer);
					break;
				}

			case 'p':						// print a pointer adress (i might rework this case in the future)
				{
					uintptr_t ptr = (uintptr_t)va_arg(args, void*);
					pprintf(ptr);
					break;
				}
				break;

			case '%':
				write_char('%');
				break;
			
			default:				// unknown specifier, just print it
				write_char('%');
				write_char(str[i]);
				break;
		}
	}
	
}

void printf(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	vprintf(str, args);
	va_end(args);
}