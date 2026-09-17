#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <idt.h>
#include <io.h>
#include <display.h>
#include <terminal.h>
#include <stmol.h>

#include <keyboard.h>

bool keyboard_enabled = true;

bool shift_pressed;
bool caps_lock;

kb_line_buffer keyboard_line_buffer;

bool kb_press;
char kb_input[256];
int kb_index = 0;

//QWERTY keyboard layout
//lowercase array
static const uint32_t qwerty_lowercase[128] = {
    0,    0,   '1', '2', '3', '4', '5', '6', '7', '8',      //0-9
    '9', '0', '-', '=',  '\b', '\t', 'q', 'w', 'e', 'r',    //10-19
    't', 'y', 'u', 'i', 'o',  'p', '[', ']', '\n', 0,       //20-29
    'a', 's', 'd', 'f', 'g',  'h', 'j', 'k', 'l', ';',      //30-39
    '\'', '`', 0,  '\\','z', 'x', 'c', 'v', 'b', 'n',       //40-49
    'm', ',', '.', '/',  0,   '*', 0,  ' ', 0,   0,         //50-49
};

//uppercase array
static const uint32_t qwerty_uppercase[128] = {
    0,    0,   '!', '@', '#', '$', '%', '^', '&', '*',      //0-9
    '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R',     //10-19
    'T', 'Y', 'U', 'I', 'O',  'P', '{', '}', '\n', 0,       //20-29
    'A', 'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L', ':',      //30-39
    '"', '~',  0,  '|', 'Z', 'X', 'C', 'V', 'B', 'N',       //40-49
    'M', '<', '>', '?',  0,   '*', 0,  ' ', 0,   0,         //50-49
};

//AZERTY keyboard layout
//lowercase array
static const uint32_t azerty_lowercase[128] = {
    0, 0, '&', 0x82, '"', '\'', '(', '-', 0x8A, '_',        //0-9
    0x87, 0xA0, ')', '=', '\b', '\t', 'a', 'z', 'e', 'r',   //10-19
    't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', 0,        //20-29
    'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',       //30-39
    0, '`',  0,  '*', 'w', 'x', 'c', 'v', 'b', 'n',         //40-49
    ',', ';', ':', '!',  0,   '*', 0,  ' ', 0,   0,         //50-59
};

//uppercase array
static const uint32_t azerty_uppercase[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',           //0-9
    '9', '0', '+', 0,  '\b', '\t', 'A', 'Z', 'E', 'R',      //10-19
    'T', 'Y', 'U', 'I', 'O',  'P', 0xF9,  0x9C, '\n', 0,      //20-29
    'Q', 'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L', 'M',      //30-39
    '%', 0xE6,  0,  '#', 'W', 'X', 'C', 'V', 'B', 'N',       //40-49
    '?', '.', '/', 0,   0,   '*', 0,  ' ', 0,   0,          //50-59
};

typedef struct
{
    const uint32_t *lowercase;
    const uint32_t *uppercase;
    const char *name; 
} keyboardlayout;

keyboardlayout qwerty = {qwerty_lowercase, qwerty_uppercase, "EN"};
keyboardlayout azerty = {azerty_lowercase, azerty_uppercase, "FR"};

keyboardlayout *currentLayout = &azerty;

void keyboard_init()
{
    shift_pressed = false; caps_lock = false;

    irq_install_handler(1, &keyboard_handler);

    write_string("Keyboard initiated ");
}

void keyboard_debug(uint8_t raw, uint8_t scancode, uint8_t press)
{
    terminal_color old_theme = current_theme;
    terminal_color debug_theme; debug_theme.fg = kernel_black; debug_theme.bg = kernel_light_gray; 

    terminal_set_theme(debug_theme);
    
    clear_row(TERMINAL_HEIGHT - 1);
    
    int TW = terminal_column; int TH = terminal_row;
    terminal_column = 0; terminal_row = TERMINAL_HEIGHT -1;

    printf("Keyboard Debug : %d %d", scancode, press);

    int max_len = 75;

    if (strlen(keyboard_line_buffer.buffer) > max_len)
    { 
        char buff[max_len + 1];
        
        for (size_t i = 0; i < max_len; i++)
        {
            buff[i] = keyboard_line_buffer.buffer[keyboard_line_buffer.len - max_len + i];
        }
        buff[max_len] = '\0';
        terminal_column = ((TERMINAL_WIDTH - strlen(buff)) / 2) - 6;
        printf("| %d | ..%s |", keyboard_line_buffer.len, buff);
    }
    else
    {        
        terminal_column = ((TERMINAL_WIDTH - strlen(keyboard_line_buffer.buffer)) / 2) - 4;
        printf("| %d | %s |", keyboard_line_buffer.len, keyboard_line_buffer.buffer);
    }

    terminal_column = TERMINAL_WIDTH - strlen("Lang : ") - strlen(currentLayout->name);
    printf("lang : %s", currentLayout->name);

    terminal_set_theme(old_theme);
    terminal_column = TW; terminal_row = TH;
}

void keyboard_handler(registers_t* regs)
{
    (void)regs;

    uint8_t raw = inb(0x60);
    uint8_t scancode = raw & 0x7F;
    uint8_t press = !(raw & 0x80);

    kb_press = press;

    keyboard_debug(raw, scancode, press);

    shell_update(); // will be changed to another file

    switch (scancode)
    {
        case 1:
            break;

        case 14:
            if (press == 1 &&  keyboard_line_buffer.len > 0 && keyboard_enabled == true)
            {
                terminal_column --;
                write_char('\0');
                keyboard_line_buffer.buffer[(keyboard_line_buffer.len--) - 1] = '\0';
                terminal_column --;
            }
            
            break;

        case 29:    // ctrl scancode
            break;
        case 56:    // alt scancode
            break;


        case 42:    // shift scancode
            if (press == 1)
            {
                shift_pressed = true;
            }
            else
            {
                shift_pressed = false;
            }

            break;
        case 58:    // MAJ scancode
            if (press == 1)
            {
                if (shift_pressed == false)
                {
                    shift_pressed = true;
                }
                else if (shift_pressed == true)
                {
                    shift_pressed = false;
                }
            }
            
            break;

        
        default:
            if (press == 1 && keyboard_enabled == true)
            {
                uint32_t c = 0;  

                if (shift_pressed == true)
                {
                    c = currentLayout->uppercase[scancode];
                }
                else
                {
                    c = currentLayout->lowercase[scancode];
                }
                
                if (c == '\n')
                {
                    keyboard_line_buffer.ready = true;
                    memcpy(keyboard_line_buffer.returned, keyboard_line_buffer.buffer, keyboard_line_buffer.len);

                    keyboard_line_buffer.len = 0;
                    memset(keyboard_line_buffer.buffer, '\0', sizeof(keyboard_line_buffer.buffer)/sizeof(keyboard_line_buffer.buffer[0]));
                }
                else
                {
                    
                    keyboard_line_buffer.buffer[keyboard_line_buffer.len++] = c;
                }

                // kbinput.raw = c

                write_char(c);
            }
            

            break;
    }
}