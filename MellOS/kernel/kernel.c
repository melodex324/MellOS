#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <vga.h>
#include <stmol.h>
#include <util.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <keyboard.h>
#include <shell.h>
#include <buffer.h>

#include <kernel_util.h>


int test = 5;
void stmol_test()
{
	#define wait_time 0.025
	#define WT wait(wait_time);

	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	terminal_writestring("Checking stmol libraries\n");
	WT;

	//memcpy test
	char src[] = "Hello";
	char dst[6];
	
	memcpy(dst, src, 6);
	if (dst[0] == 'H' && dst[4] == 'o')
	    terminal_writestring("memcpy: OK\n");
	else
	    terminal_writestring("memcpy: FAIL\n");

	WT;

	//memcmp test
	char a[] = "Hello";
	char b[] = "Hello";
	char c[] = "World";

	if (memcmp(a, b, 5) == 0)
	    terminal_writestring("memcmp equal: OK\n");
	else
	    terminal_writestring("memcmp equal: FAIL\n"); 
		
	WT;

	if (memcmp(a, c, 5) != 0)
	    terminal_writestring("memcmp not equal: OK\n");
	else
	    terminal_writestring("memcmp not equal: FAIL\n"); 
		
	WT;

	//memmove test
	char buf[] = "AABBCC";

	// overlapping move: shift right by 2
	memmove(buf + 2, buf, 4);
	if (buf[2] == 'A' && buf[3] == 'A')
	    terminal_writestring("memmove overlap: OK\n");
	else
	    terminal_writestring("memmove overlap: FAIL\n"); 
		
	WT;

	// non overlapping move
	char mov_src[] = "Hello";
	char mov_dst[6];

	memmove(mov_dst, mov_src, 6);
	if (mov_dst[0] == 'H' && mov_dst[4] == 'o')
	    terminal_writestring("memmove normal: OK\n");
	else
	    terminal_writestring("memmove normal: FAIL\n"); 
		
	WT;

	//memset test
	char setbuf[5];

	memset(setbuf, 'X', 5);
	if (setbuf[0] == 'X' && setbuf[4] == 'X')
	    terminal_writestring("memset: OK\n");
	else
	    terminal_writestring("memset: FAIL\n"); 
	
	WT;


	//strlen test
	if (strlen("Hello") == 5)
	    terminal_writestring("strlen: OK\n");
	else
	    terminal_writestring("strlen: FAIL\n"); 
	
	WT;

	//strcmp test
	char cmp1[] = "Hello";
	char cmp2[] = "Hello";
	char cmp3[] = "World";

	if (strcmp(cmp1, cmp2) == 0)
	    terminal_writestring("strcmp equal: OK\n");
	else
	    terminal_writestring("strcmp eterminal_color = vga_entry_color(VGA_COLOR_RED, VGA_COLOR_WHITE);qual: FAIL\n");

	WT;

	if (strcmp(cmp1, cmp3) != 0)
	    terminal_writestring("strcmp not equal: OK\n");
	else
	    terminal_writestring("strcmp not equal: FAIL\n");

	WT;

	//length difference test
	if (strcmp("Hell", "Hello") != 0)
	    terminal_writestring("strcmp length diff: OK\n");
	else
	    terminal_writestring("strcmp length diff: FAIL\n");

	WT;

	//strcpy test	
	char apy[7] = "Hello";
	char bpy[7];

	strcpy(bpy, apy);
	if (memcmp(bpy, apy, 3) == 0)
	{
	    terminal_writestring("strcpy: OK\n");
	} else {
	    terminal_writestring("strcpy: FAIL\n");
	} WT;

	//strcat test
	char cat_dst[20] = "Hello";

	strcat(cat_dst, " World");
	if (strcmp(cat_dst, "Hello World") == 0)
	    terminal_writestring("strcat: OK\n");
	else
	    terminal_writestring("strcat: FAIL\n");

	WT;


	//swap
		//TODO

	//reverse
		//TODO
	
	//itoa
	char bufI[20];

	itoa(1234, bufI, 10);
	if (strcmp(bufI, "1234") == 0)
	    terminal_writestring("itoa decimal: OK\n");
	else
	    terminal_writestring("itoa decimal: FAIL\n");

	WT;

	itoa(255, bufI, 16);
	if (strcmp(bufI, "ff") == 0)
	    terminal_writestring("itoa hex: OK\n");
	else
	    terminal_writestring("itoa hex: FAIL\n");

	WT;

	itoa(-42, bufI, 10);
	if (strcmp(bufI, "-42") == 0)
	    terminal_writestring("itoa negative: OK\n");
	else
	    terminal_writestring("itoa negative: FAIL\n");

	WT;

	//printf
	printf("printf string: %s\n", "Hello"); WT;
	printf("printf decimal: %d\n", 1234); WT;
	printf("printf negative: %d\n", -42); WT;
	printf("printf hex: %x\n", 255); WT;
	printf("printf hex upper: %X\n", 255); WT;
	printf("printf char: %c\n", 'A'); WT;
	printf("printf percent: 100%%\n"); WT;
	printf("printf multiple: %s is %d years old\n", "MDXOS", 1); WT;

	//print
		//TODO

	terminal_color = vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
	terminal_writestring("\ntest finnished");
	wait(0.5);
}

void disable_cursor()
{
	outPortB(0x3D4, 0x0A);
	outPortB(0x3D5, 0x20);
}

void kernel_main(void) 
{
	// disable_cursor();
	/* Initialize terminal interface */
	terminal_initialize();
	gdt_init();
	idt_init();
	pit_init();		//volatile int crash = 1 / 0; 			//debug or test only keep as comment
	keyboard_init();
	kernel_util_init();
	shell_init();

	wait(0.5); new_line();

	stmol_test(); terminal_initialize();
	uptime_init();

	terminal_writestring("Hi World, this is MDXOS or MDXMOS!\nAn Operating System made by Melodex\n");
	new_line();
    terminal_writestring("My second OS\n");

	make_line(2);
	terminal_writestring("Im testing my new line and make line function\n");
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	terminal_writestring("Cheking text color\n");
	terminal_color = vga_entry_color(VGA_COLOR_RED, VGA_COLOR_WHITE);
	terminal_writestring("Cheking backrgound color\n");
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	static_line_printf("a\ntest");
	static_line_printf("b : %d", test);
	static_def_line_printf(13, -1,"This text is going to be centered no matter the lenght of the string %d", test);
	static_def_line_printf(14,-2,"This will be in left");
	static_def_line_printf(14,-3,"This will be in right");

	static_def_line_printf(16, -1,"This text is going to be centered no matter the lenght of the string %d", test);
	
	// memcpy(saved_buffer, terminal_buffer, 80*25);
	buffer_save(terminal_saved_buffer);
	wait(0.5);
	shell_loader();

	for(;;)
	{
		// buffer_update();
		shell_update();
	}
	
}
