#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <stmol.h>
#include <vga.h>
#include <util.h>
#include <pit.h>
#include <buffer.h>

size_t margins = 0;


uint16_t *saved_buffer = (uint16_t*)VGA_MEMORY;

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

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len + 1] = '\0';
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

char *itoa(int value, char *str, int base)
{
    if (str == NULL)
        return NULL;

    if (base < 2 || base > 16)
    {
        str[0] = '\0';
        return str;
    }

    if (value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    size_t i = 0;
    bool is_negative = false;

    if (value < 0 && base == 10)
    {
        is_negative = true;
        value = -value;
    }

    while (value != 0)
    {
        int rem = value % base;
        str[i++] = (rem < 10) ? rem + '0' : rem - 10 + 'a';
        value /= base;
    }

    if (is_negative)
        str[i++] = '-';

    str[i] = '\0';

    reverse(str, i);

    return str;
}

void make_line(int lines)
{
	for (int i = 0; i < lines; i++)
	{
		terminal_row++;
		terminal_column = 0;

		if (terminal_row >= VGA_HEIGHT) 
        {
    		terminal_scroll();
    		terminal_row = VGA_HEIGHT - 1;
    	}
	}
}

void new_line()
{
	make_line(1);
}

void vprintf(const char *format, va_list args)
{
	if (format == NULL)
    return;

	char buf[32];  // buffer for itoa

    for (size_t i = 0; format[i] != '\0'; i++)
    {
        // if not a format specifier just print the character
        if (format[i] != '%')
        {
            terminal_putchar(format[i]);
            continue;
        }

        // skip the '%' and look at next character
        i++;

        switch (format[i])
        {
            // character
            case 'c':
            {
                char c = (char)va_arg(args, int);
                terminal_putchar(c);
                break;
            }
            // string
            case 's':
            {
                char *str = va_arg(args, char*);
                if (str == NULL)
                    terminal_writestring("(null)");
                else
                    terminal_writestring(str);
                break;
            }
            // decimal integer
            case 'd':
            case 'i':
            {
                int val = va_arg(args, int);
                itoa(val, buf, 10);
                terminal_writestring(buf);
                break;
            }
            // unsigned integer
            case 'u':
            {
                unsigned int val = va_arg(args, unsigned int);
                itoa((int)val, buf, 10);
                terminal_writestring(buf);
                break;
            }
            // hex lowercase
            case 'x':
            {
                int val = va_arg(args, int);
                itoa(val, buf, 16);
                terminal_writestring(buf);
                break;
            }
            // hex uppercase
            case 'X':
            {
                int val = va_arg(args, int);
                itoa(val, buf, 16);
                // convert to uppercase
                for (size_t j = 0; buf[j] != '\0'; j++)
                {
                    if (buf[j] >= 'a' && buf[j] <= 'f')
                        buf[j] -= 32;
                }
                terminal_writestring(buf);
                break;
            }
            // octal
            case 'o':
            {
                int val = va_arg(args, int);
                itoa(val, buf, 8);
                terminal_writestring(buf);
                break;
            }
            // literal %
            case '%':
            {
                terminal_putchar('%');
                break;
            }
            // unknown specifier, just print it
            default:
            {
                terminal_putchar('%');
                terminal_putchar(format[i]);
                break;
            }
        }
    }
}

void printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
	vprintf(format, args);
    va_end(args);
}

//code directly related to the terminal

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
// uint16_t *terminal_buffer;

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outPortB(0x3D4, 0x0F);
	outPortB(0x3D5, (uint8_t) (pos & 0xFF));
	outPortB(0x3D4, 0x0E);
	outPortB(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			active_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	
	buffer_dirty = true;
}


void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	if (c == '\n')
	{
		terminal_column = -1;
		terminal_row++;
	}
	else
	{
		const size_t index = y * VGA_WIDTH + x;
		active_buffer[index] = vga_entry(c, color);
	}

	buffer_dirty = true;
	
}

void terminal_putchar(char c)
{
	if (terminal_row == 0 && terminal_column == -1)
	{
		terminal_row = 0;
		terminal_column = 0;
	}

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
		terminal_row = 0;
	}
	
	if (terminal_row == VGA_HEIGHT- margins)
	{
		terminal_row = VGA_HEIGHT -1 - margins;
		terminal_scroll();
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void terminal_scroll()
{
    for (size_t row = 1; row < VGA_HEIGHT - margins; row++) {
        copy_row(row - margins, row - 1 - margins);
    }
    clear_row(VGA_HEIGHT - 1 - margins);
}


void copy_row(size_t src, size_t dest) {
    for (size_t col = 0; col < VGA_WIDTH; col++) {
        active_buffer[dest * VGA_WIDTH + col] = active_buffer[src * VGA_WIDTH + col];
    }
}

void clear_row(size_t row) {
    for (size_t col = 0; col < VGA_WIDTH; col++) {
        active_buffer[row * VGA_WIDTH + col] = (uint16_t)' ' | ((uint16_t)terminal_color << 8);
    }
}

void copy_current_row(size_t dest) 
{
	size_t src = terminal_row;
    for (size_t col = 0; col < terminal_row; col++) {
        active_buffer[dest * terminal_row + col] = active_buffer[src * terminal_row + col];
    }
}

void clear_current_row()
{
	size_t row = terminal_row;
	terminal_column = 0;
	clear_row(row);
}


//Test & experimental function needs works

void _static_set_line_printf(int row, const char *format, va_list args)
{

	if (row > 24)
	{
		row = 24;
	}

    size_t saved_row = terminal_row;
    size_t saved_col = terminal_column;

    terminal_row = row;
    terminal_column = 0;

    vprintf(format, args);
    terminal_row = saved_row;
    terminal_column = saved_col;
}

void _static_def_line_printf(int row, int col, const char *format, va_list args)
{

	if (row > 24)
	{
		row = 24;
	}

	if (col > 80){
		col = 80;
	} else if (col < -3){
		col = -1;
	} 
	
	//col set at 0 will on center
	//col set at -1 will on left
	//col set at -2 will on right
	if (col == -1)
	{
		col = (VGA_WIDTH  - strlen(format)) / 2;
	} else if (col == -2)
	{
		col = 0;
	} else if (col == -3)
	{
		col = VGA_WIDTH - strlen(format);
	}
	
    size_t saved_row = terminal_row;
    size_t saved_col = terminal_column;

    terminal_row = row;
    terminal_column = col;

    vprintf(format, args);
    terminal_row = saved_row;
    terminal_column = saved_col;
}

//public function for printing at a specific line in a specific col
//col set at 0 will on center
//col set at -1 will on left
//col set at -2 will on right
void static_def_line_printf(int row, int col, const char *format, ...)
{
    va_list args;
	//va_list pos;
    // va_start(args, row);
	va_start(args, format);
    _static_def_line_printf(row, col, format, args);
    va_end(args);
}

//public function for printing at a specific line
void static_set_line_printf(int row, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _static_set_line_printf(row, format, args);
    va_end(args);
}

//public function for printing at current line
void static_line_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _static_set_line_printf(terminal_row, format, args);
    va_end(args);
}