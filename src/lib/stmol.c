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

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    if (num == 0)
	{
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10)
	{
        isNegative = true;
        num = -num;
    }

    while (num != 0)
	{
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
	{
        str[i++] = '-';
	}

    str[i] = '\0';
    reverse(str, i);
    return str;
}
