# Build záležitosti, headery a podobné srandy

## prolin-printf.h
header použitý ako redefinícia pre printf s flushom do stdoutu.

`-include prolin-printf.h` v cflagoch aby sa globálne natlačil na všetky zdrojáky v projekte

## osal_test.c
testuvačka pre osal funkcie, mení jas od 0 do 10, vyžaduje loader aby nastavil `LD_PRELOAD=/usr/lib/libosal.so` alebo v telnete pustiť skrz `LD_PRELOAD=/usr/lib/libosal.so L;`

`arm-none-linux-gnueabi-gcc -fPIC -nostartfiles -shared -include prolin_printf.h osal_test.c -o osal_test.so`

## osal.h
header pre šecky libosal.so relevantné funkcie, pre použití kukaj osal_test.c

## xui_test.c
testuvačka pre xui funkcie, vykreslí xui okno, vykreslí statusbar s 8 ikonami a vypíše text Xui testuvacka

`arm-none-linux-gnueabi-gcc -fPIC -nostartfiles -shared -include prolin_printf.h xui_test.c -o xui_test.so`

## xui.h
poloprepečený a ručne vyskladaný header pre skoro šecky libxui.so relevantné funkcie, pre čiastočné použití kukaj xui_test.c
