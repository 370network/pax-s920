#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

//XUI_FONT_GREY still seems to return the wrong flags
//we have to patch four bytes from offset 0x27bbcU changing the ft flags to 0x24 (from 0x03)
#define XUI_FT_FLAG_OFFSET 0x27bbcU
static const unsigned char antialias_freetype_flags[4] = {0x24, 0x90, 0xa0, 0xe3};

__attribute__((constructor))
static void patch(void)
{
	//find XuiCreateFont, this CAN'T fail
	void *symbol = dlsym(RTLD_DEFAULT, "XuiCreateFont");
	
	//fetch base address of the createfont symbol
	Dl_info symbol_info;
	memset(&symbol_info, 0, sizeof(symbol_info));
	if (dladdr(symbol, &symbol_info) == 0 || symbol_info.dli_fbase == NULL)
		return;
	
	//find instructions at offset
	unsigned char *instruction = (unsigned char *)symbol_info.dli_fbase + XUI_FT_FLAG_OFFSET;
	
	//find the start of memory page, needed for mprotect as it requires a whole page
	uintptr_t page_start = (uintptr_t)instruction & ~((uintptr_t)4096 - 1U);
	
	//switch page to READ + WRITE
	if (mprotect((void *)page_start, (size_t)4096, PROT_READ | PROT_WRITE) != 0)
		return;
	
	//copy patched bytes to the offset
	memcpy(instruction, antialias_freetype_flags, sizeof(antialias_freetype_flags));
	
	//flush instruction cache (if potentially cached in the SoC instruction cache?)
	__builtin___clear_cache((char *)instruction, (char *)instruction + sizeof(antialias_freetype_flags));
	
	//revert page to READ + EXEC like it was before
	if (mprotect((void *)page_start, (size_t)4096, PROT_READ | PROT_EXEC) != 0)
		return;
}