#include <stmol.h>
#include <display.h>
#include <font.h>

#include <terminal.h>

size_t terminal_row;
size_t terminal_column;

terminal_color current_theme;
terminal_color default_theme;

terminal_offset default_offset;
terminal_offset current_offset;

void terminal_init()
{
    default_offset.height = 0; default_offset.width = 0;
    default_theme.bg = kernel_black; default_theme.fg = kernel_white;
    
    terminal_set_theme(default_theme);
    current_offset = default_offset;
}

void terminal_set_color(enum kernel_color fg, enum kernel_color bg)
{
    current_theme.fg = fg;
    current_theme.bg = bg;
}

void terminal_set_theme(terminal_color theme)
{
    current_theme = theme;
}

void make_lines(size_t lines)
{
	for (size_t i = 0; i < lines; i++)
	{
		terminal_row ++;
		terminal_column = 0;
	}
}

void new_line(void)
{
	make_lines(1);
}

void copy_row(size_t src, size_t dest)
{
    copy_buffer_region(fb, 0, TEXT_HEIGHT * src, 0, TEXT_HEIGHT * dest, fb->width, TEXT_HEIGHT);
}

void clear_row(size_t row)
{
    clear_buffer_region(fb, 0, TEXT_HEIGHT * row, fb->width, TEXT_HEIGHT, current_theme.bg);
}

void terminal_scroll(void)
{
    terminal_row --;
    terminal_column = 0;

    for (size_t i = 1; i < TERMINAL_HEIGHT; i++)
    {
        copy_row(i, i - 1);
    }

    clear_row(TERMINAL_HEIGHT - 1);
}


void write_char(char c)
{
	if (c == '\n')
	{
		terminal_row++;
		terminal_column = 0;
	}
	else
	{	
        if (terminal_column == TERMINAL_WIDTH)
		{
            terminal_row ++;
			terminal_column = 0;
		}

        if (terminal_row == TERMINAL_HEIGHT)
        {
            terminal_scroll();
        }

        draw_font(fb, c, (current_offset.width + terminal_column++) * TEXT_WIDTH, terminal_row * TEXT_HEIGHT, current_theme.fg, current_theme.bg);
    }
}

void write_string(const char* string)
{
    size_t len = strlen(string);

    for (size_t i = 0; i < len; i++)
    {
        write_char(string[i]);
    }
}

void terminal_clear()
{
    terminal_row = 0; terminal_column = 0;

    for (size_t i = 0; i < TERMINAL_HEIGHT * TERMINAL_WIDTH; i++)
    {
        write_char('\0');
    }
    terminal_row = 0; terminal_column = 0;
}