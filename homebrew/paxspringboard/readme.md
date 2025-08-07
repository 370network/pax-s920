> ⚠️ **Warning:** This is a highly unstable version of the software. Use at your own risk. Features and behavior may change without notice.

# Homebrew Application launcher for Prolin devices

Tested on Pax S920

# Build instructions:


Following is application format:

/data/app/MAINAPP/apps/<APP_NAME>/
    ├── <APP_NAME>.s       (Required)  
    ├── icon.png           (Required)  
    ├── info.txt           (Required)  
    └── [data/]            (Optional)

- The binary MUST expose an _init() function as the main entrypoint for execution.
- icon.jpg - 32x32 pixels
- info.txt - metadata 
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