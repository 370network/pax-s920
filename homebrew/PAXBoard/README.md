# PAXBoard
A homegrown homebrew launcher as a replacement to the original `"hodiny.so"` program loader.

## Controls:
Usually controlled using the touchscreen + with additional keys for more control
* **1 to 9 + FUNC, 0 and ALPHA** - runs the apps in the visual order from 1 to 12 
* **Red X key** - closes PAXBoard
* **Yellow back key** - puts PAX into sleep
* **Green circle key** - switches pages
### Key combos:
* **Green + Yellow** - deep sleep
* **Green + Red** - opens Terminal Manager

## Build instructions:
Just run `make`, the output is going to be called Launcher.so, feel free to paxpush it to `/data/app/MAINAPP/paxboard/lib/libosal.so` since that's the default place for PAXBoard on deployed terminals

## Application format:
All apps packaged for use with PAXBoard use one of either paths + a following folder structure:
```
/data/app/MAINAPP/apps/<APP_NAME>/
/mnt/sdcard/apps/<APP_NAME>/
                        ├── <APP_NAME>         (Executable) 
                        ├── <APP_NAME>.so      (Library)  
                        ├── icon.png           (Required)  
                        └── info.txt           (Required)  
```
* Executable can be provided if your PAX can run unsigned binaries *(true for deployed terminals)*, otherwise .so library must be provided
* The library MUST expose an _init() function as the main entrypoint for execution.
* icon.jpg - 64x64 pixels max.
* info.txt - Metadata information
* Apps are being launched with the folder as the working directory, please store all additional data in this folder.

### Metadata (info.txt)
Provides metadata about the application:
```
name: <Human-readable name>
version: <Semantic versioning, e.g., 1.0.0>
author: <Author or organization>
description: <Short description of the application>
license: <License identifier> (optional)
url: <Homepage or repository URL> (optional)
```
