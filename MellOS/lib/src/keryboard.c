#include <stdint.h>
#include <stmol.h>
#include <util.h>
#include <idt.h>
#include <pit.h>

#include <keyboard.h>
#include <buffer.h>
#include <shell.h>

#define scan_code_press 10

bool shift_pressed;
bool caps_lock;

bool altgr_pressed;


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
    'T', 'Y', 'U', 'I', 'O',  'P', 0xc2a8,  0xa3, '\n', 0,      //20-29
    'Q', 'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L', 'M',      //30-39
    '%', 'µ',  0,  '#', 'W', 'X', 'C', 'V', 'B', 'N',       //40-49
    '?', '.', '/', 0,   0,   '*', 0,  ' ', 0,   0,          //50-59
};

static const uint32_t azerty_altGr[128] = {
    0, '“', '´', '~', '#', '{', '[', '|', '`', 0x5c,        //0-9
    '^', '@', ']', '}', 'â', 'å', '€', 'ç',                 //10-19 
};

//change of keyboard layout
char* currlayout;
typedef struct {
    const uint32_t *lowercase;
    const uint32_t *uppercase;
} KeyboardLayout;

KeyboardLayout qwerty = { qwerty_lowercase, qwerty_uppercase };
KeyboardLayout azerty = { azerty_lowercase, azerty_uppercase };

KeyboardLayout *currentLayout = &azerty;

void changeLayout() {
    if (currentLayout == &qwerty) {
        currentLayout = &azerty;
        currlayout = "azerty";
    } else {
        currentLayout = &qwerty;
        currlayout = "qwerty";
    }
}


void keyboard_init()
{   
    shift_pressed = false;
    caps_lock = false;

    
    irq_install_handler(1, &keyboard_handler);
    terminal_writestring("keyboard initiated ");
}

void debug(bool enabled, uint8_t scan_code, uint8_t press)
{   
    if (enabled == true)
    {
        uint8_t terminal_color_temp = terminal_color;

        terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
        clear_row(scan_code_press);
        clear_row(scan_code_press);
        static_set_line_printf(scan_code_press,"Scan code: %d, Press: %d", scan_code, press);
        terminal_color = terminal_color_temp;

        clear_row(16);
        static_def_line_printf(16,-1,"%d : %d", terminal_row , terminal_column); 
    }
}

void keyboard_handler(struct InterruptRegisters* regs)
{   
    (void)regs;

    uint8_t raw = inPortB(0x60);
    uint8_t scan_code = raw & 0x7F;
    uint8_t press = !(raw & 0x80);


    // debug(true, scan_code, press);

    switch (scan_code)
    {
    case 1:                         //esc key

    case 14:                        //backtab key
        if (press == 1 && input_index > 0)
        {   
            terminal_column --;
            terminal_putchar(' '); input_buffer[input_index--] = ' ';
            terminal_column --;
        }
        break;

    case 29:

    case 56:                        // altGr
        if (press == 0)
        {
            altgr_pressed = false;
        }
        else
        {
            altgr_pressed = true;
        }
        break;
    
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
    case 87:
    case 88:
        break;

    case 42:                        //L shift key
        if (press == 0)
        {
            shift_pressed = false;
        }
        else
        {
            shift_pressed = true;
        }
        break;

    case 54:                        //R shift key
        if (press == 0)
        {
            shift_pressed = false;
        }
        else
        {
            shift_pressed = true;
        }
        break;
    
    case 58:                        //caps lock key
        if (!caps_lock && press == 1){
            caps_lock = true;
        }
        else if(caps_lock && press == 1){
            caps_lock = false;
        }
        break;

default:
    if (press == 1)
    {
        uint32_t c = 0;

        if (shift_pressed && caps_lock)
            c = currentLayout->lowercase[scan_code];
        else if (shift_pressed || caps_lock)
            c = currentLayout->uppercase[scan_code];
        else if (altgr_pressed)
        {
            c = azerty_altGr[scan_code];
        }
        else
            c = currentLayout->lowercase[scan_code];

        if (active_buffer == shell_buffer)
        {
            if (c != 0 && c != '\n' && input_index < VGA_MEMORY)
            {
                input_buffer[input_index++] = (char)c;
                terminal_putchar((char)c);
            }
            else if (c == '\n')
            {
                input_ready = true;
            }
        }else
        {
            if (c != 0)
            {
                input_buffer[input_index++] = (char)c;
                terminal_putchar((char)c); 
            }
        }
        
        
        
    }
    break;
    }
}