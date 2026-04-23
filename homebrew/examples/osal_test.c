#include <dlfcn.h>
#include <fcntl.h>

// lib symbol loader
typedef void (*generic_func)();
generic_func load_symbol(void *lib_handle, const char *symbol_name) {
    generic_func symbol = (generic_func)dlsym(lib_handle, symbol_name);
    return symbol;
}

static unsigned short *lcd_fb;

int _init()
{
		printf("OSAL Test!\n");
		
		int fb_fd = open ("/dev/fb", O_RDWR);
		lcd_fb = mmap (0, 320 * 240 * 2, 3, 1, fb_fd, 0);
		
		int i, j;
		for (i = 0; i < 240; i++)
			for (j = 0; j < 320; j++)
			{
				int r = (j * 31) / 319;
				int g = (i * 63) / 239;
				int b = 15;
				
				lcd_fb[j + i * 320] = (r << 11) | (g << 5) | (b);
			}
		
		void *libosal = dlopen("/usr/lib/libosal.so", RTLD_LAZY);
		
		void (*OsScrBrightness)() = load_symbol(libosal, "OsScrBrightness");
		
		i = 0;
		for(i = 0; i <= 10; i++)
		{
			printf("Setting screen brightness to: %d\n", i);
			OsScrBrightness(i);
			sleep(1);
		}
		
		return(0);
}


