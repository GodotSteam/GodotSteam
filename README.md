# GodotSteam Server for Godot 3.x
Steam Server API for the Godot game engine (versions 3 to 3.2.2). For the Windows, Linux, and Mac platforms. 

Additional flavors include: [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2), [Godot 2.x Minimal](https://github.com/Gramps/GodotSteam/tree/godot2-min), [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master), [Godot 3.x Minimal](https://github.com/Gramps/GodotSteam/tree/godot3-min), and [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative).

Documentation
----------
Documentation is available here: https://gramps.github.io/GodotSteam/

Alternately, there is the project's Wiki page here: https://github.com/Gramps/GodotSteam/wiki

You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam Server.

Current Build
----------
**Version 1.1.0 Changes**
- Added: Apps, HTTP, Inventory, Networking, UGC, and Utils classes
- Added: related callbacks, call results, constants, and enums
- Added: headless server platform for linux
- Fixed: OSX compiling config rules

**Version 1.0.0 Changes**
- Added: missing server functions
- Fixed: missing argument in config.py
- Fixed: missing Godot include that allows more than 5 arguments per function
- Fixed: a lot of different functions
- Fixed: issue with include in register_types.cpp
- Removed: remaining depreciated functions
- Removed: functions not compatible with new networking functions

Known Issues
----------
- **Using MinGW causes crashes.** I strong recommend you **do not use MinGW** to compile at this time.

Quick How-To
----------
- Download this repository and unpack it.
- Download and unpack the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- Download and unpack the [Godot source](https://github.com/godotengine/godot); preferably 3.2.2.
- Move the following to godotsteam_server/sdk/:
````
    sdk/public/
    sdk/redistributable_bin/
````
- The repo's directory contents should now look like this:
````
    godotsteam_server/sdk/public/*
    godotsteam_server/sdk/redistributable_bin/*
    godotsteam_server/SCsub
    godotsteam_server/config.py
    godotsteam_server/godotsteam_server.cpp
    godotsteam_server/godotsteam_server.h
    godotsteam_server/register_types.cpp
    godotsteam_server/register_types.h
````
- Now move the "godotsteam_server" directory into the "modules" directory of the unpacked Godot Engine source.
- Recompile for your platform:
  - Windows ( http://docs.godotengine.org/en/stable/reference/compiling_for_windows.html )
  - Linux ( http://docs.godotengine.org/en/stable/reference/compiling_for_x11.html )
    - If using Ubuntu 16.10 or higher and having issues with PIE security in GCC, use LINKFLAGS='-no-pie' to get an executable instead of a shared library.
  - OSX ( http://docs.godotengine.org/en/stable/reference/compiling_for_osx.html )
    - When creating templates for this, please refer to this post for assistance as the documentation is a bit lacking ( http://steamcommunity.com/app/404790/discussions/0/364042703865087202/ ).
- When recompiling the engine is finished do the following before running it the first time:
  - Copy the shared library (steam_api), for your OS, from sdk/redistributable_bin/ folders to the Godot binary location (by default in the godot source /bin/ file but you can move them to a new folder).
  - Create a steam_appid.txt file with your game's app ID or 480 in this folder.  Necessary if the editor or game is run _outside_ of Steam.

- The finished hierarchy should look like this (if you downloaded the pre-compiles, the editor files go in place of these tools files, which are the same thing):
  - Linux 32/64-bit
  ```
  libsteam_api.so
  steam_appid.txt
  ./godot.linux.tools.32 or ./godot.linux.tools.64
  ```
  - OSX
  ```
  libsteam_api.dylib
  steam_appid.txt
  ./godot.osx.tools.32 or ./godot.osx.tools.64
  ```
  - Windows 32-bit
  ```
  steam_api.dll
  steam_appid.txt
  ./godot.windows.tools.32.exe
  ```
  - Windows 64-bit
  ```
  steam_api64.dll
  steam_appid.txt
  ./godot.windows.tools.64.exe
  ```
- Lack of the Steam API DLL/SO/DyLib (for your respective OS) or the steam_appid.txt will cause it fail and crash when testing or running the game outside of the Steam client.
  - **NOTE:** For OSX, the libsteam_api.dylib and steam_appid.txt must be in the Content/MacOS/ folder in your application zip or the game will crash.
  - **NOTE:** For Linux, you may have to load the overlay library for Steam overlay to work:
  ```
  export LD_PRELOAD=~/.local/share/Steam/ubuntu12_32/gameoverlayrenderer.so;~/.local/share/Steam/ubuntu12_64/gameoverlayrenderer.so
  
  or 
  
  export LD_PRELOAD=~/.local/share/Steam/ubuntu12_32/gameoverlayrenderer.so;
  export LD_PRELOAD=~/.local/share/Steam/ubuntu12_64/gameoverlayrenderer.so;
  ```
  This can be done in an .sh file that runs these before running your executable.  This issue may not arise for all users and can also just be done by the user in a terminal separately.

From here you should be able to call various functions of Steamworks. You should be able to look up the functions in Godot itself under the search section. In addition, you should be able to read the Steamworks API documentation to see what all is available and cross-reference with GodotSteam.

- When uploading your game to Steam, you *must* upload your game's executable and Steam API DLL/SO/DyLIB (steam_api.dll, steam_api64.dll, libsteam_api.dylib, and/or libsteam_api.so).  *Do not* include the steam_appid.txt or any .lib files as they are unnecessary.

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Patreon](https://patreon.com/coaguco) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
