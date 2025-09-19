> ⚠️ **Warning:** This is a highly unstable version of the software. Use at your own risk. Features and behavior may change without notice.

# Homebrew Application launcher for Prolin devices

Tested on Pax S920

# Build instructions:


Following is application format:

```
/data/app/MAINAPP/apps/<APP_NAME>/
SDCARD_ROOT/apps/<APP_NAME>/
                        ├── <APP_NAME>         (Executable) 
                        ├── <APP_NAME>.so      (Library)  
                        ├── icon.png           (Required)  
                        ├── info.txt           (Required)  
                        └── [data/]            (Optional)
```

- Executable can be provided if your PAX can run unsigned binaries or binary is signed, otherwise library must be provided
- The library MUST expose an _init() function as the main entrypoint for execution.
- icon.jpg - 64x64 pixels max
- info.txt - Metadata 
- Optional: Applications may create and use a data/ subdirectory within their own folder for persistent storage.

Metadata (info.txt)
Provides metadata about the application

---
name: <Human-readable name>
version: <Semantic versioning, e.g., 1.0.0>
author: <Author or organization>
description: <Short description of the application>
license: <License identifier> (optional)
url: <Homepage or repository URL> (optional)
---
