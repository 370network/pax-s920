# Tests
popisy testov + samostatné buildy, v inom prípade radšej použiť build.sh/make

## osal_test.c
testuvačka pre osal funkcie, mení jas od 0 do 10, vyžaduje loader aby nastavil `LD_PRELOAD=/usr/lib/libosal.so` alebo v telnete pustiť skrz `LD_PRELOAD=/usr/lib/libosal.so L;`

`arm-none-linux-gnueabi-gcc -fPIC -nostartfiles -shared -include prolin_printf.h osal_test.c -o osal_test.so`


## xui_test.c
testuvačka pre xui funkcie, vykreslí xui okno, vykreslí statusbar s 8 ikonami a vypíše text Xui testuvacka

`arm-none-linux-gnueabi-gcc -fPIC -nostartfiles -shared -include prolin_printf.h xui_test.c -o xui_test.so`

## magstripe-reader.c
testuvačka na načítaní magstripe karty a výpis informácií o trackoch (ktoré aj vytlačí)

`arm-none-linux-gnueabi-gcc -fPIC -nostartfiles -shared -include prolin_printf.h main.c -o osal_test.s` 