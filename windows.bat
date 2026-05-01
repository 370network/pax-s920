@echo off
echo ==================
echo 370network paxdevs
echo =   toolchain    =
echo ==================
echo.
echo [*] System info:
ver
echo %PROCESSOR_ARCHITECTURE%
echo.
echo [*] Cache setup!
mkdir cache

echo.
echo [*] MSYS2 Setup!

IF EXIST cache\msys64.exe (
    echo MSYS2 cache already exists, continuing
) ELSE (
    echo MSYS2 cache download
    curl -o cache/msys64.exe https://repo.msys2.org/distrib/msys2-x86_64-latest.sfx.exe
)

IF NOT EXIST "msys64\" (
    echo MSYS2 unpack...
    cache\msys64.exe
) ELSE (
    echo MSYS2 already exists, continuing
)

echo.
IF NOT EXIST "toolchain\bin\" (
    echo [*] Strapping to MSYS2 to prepare toolchain
    msys64\mingw32.exe %CD%\setup_toolchain.sh
    echo You should be now continuing in the newly opened window.
    pause
) ELSE (
    echo [*] Strapping to MSYS2 to open a new session
    set CHERE_INVOKING=1
    msys64\usr\bin\env MSYSTEM=MINGW32 /usr/bin/bash -li -c "echo -n cd $PWD > ~/.bashrc;echo -n ';source env.sh' >> ~/.bashrc;echo ';rm ~/.bashrc' >> ~/.bashrc;exec bash"
)
