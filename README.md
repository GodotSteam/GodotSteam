# Godot Steam for Godot 3
Steam API for the Godot game engine (version 3 or higher). For the Windows, Linux, and Mac platforms. 

- View the GodotSteam for Godot 2 here: https://github.com/Gramps/GodotSteam/tree/master
- View the GodotSteam for GDNative here: https://github.com/Gramps/GodotSteam/tree/gdnative
- View the GodotSteam Server for Godot 3 here: https://github.com/Gramps/GodotSteam/tree/server

Documentation
----------
Documentation is available here: https://gramps.github.io/GodotSteam/

Alternately, there is the project's Wiki page here: https://github.com/Gramps/GodotSteam/wiki

You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Current Build
----------
You can download pre-compiled versions _(currently v2.6.0)_ of this repo here: https://github.com/Gramps/GodotSteam/releases

**Version 2.7.0 Changes**
- Added: getAchievementDisplayAttribute, getAchievementName, getAchievementIcon, getImageRGBA, and getImageSize (courtesy of marcelofg55)
- Added: all missing SteamApps functions
- Changed: NULL statements for achievement functions
- Changed: cleaned up and organized signal functions in godotteam.h
- Fixed: issue with dictionary formatting in a function
- Fixed: issue with getAchievement failing to compile
- Fixed: missing NULL statement
- Removed: hasOtherApp function

Known Issues
----------
- User avatar functions do not work due to changes in Godot's image class

Quick How-To
----------
- Download this repository and unpack it.
- Download and unpack the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- Download and unpack the [Godot source](https://github.com/godotengine/godot); preferably 3.0 or higher.
- Move the following to godotsteam/sdk/:
````
    sdk/public/
    sdk/redistributable_bin/
````
- The repo's directory contents should now look like this:
````
    godotsteam/sdk/public/*
    godotsteam/sdk/redistributable_bin/*
    godotsteam/SCsub
    godotsteam/config.py
    godotsteam/godotsteam.cpp
    godotsteam/godotsteam.h
    godotsteam/register_types.cpp
    godotsteam/register_types.h
````
- Now move the "godotsteam" directory into the "modules" directory of the unpacked Godot Engine source.
- Recompile for your platform:
  - Windows ( http://docs.godotengine.org/en/stable/reference/compiling_for_windows.html )
  - Linux ( http://docs.godotengine.org/en/stable/reference/compiling_for_x11.html )
    - If using Ubuntu 16.10 or higher and having issues with PIE security in GCC, use LINKFLAGS='-no-pie' to get an executable instead of a shared library.
  - OSX ( http://docs.godotengine.org/en/stable/reference/compiling_for_osx.html )
    - When creating templates for this, please refer to this post for assistance as the documentation is a bit lacking ( http://steamcommunity.com/app/404790/discussions/0/364042703865087202/ ).
- When recompiling the engine is finished, copy the shared library (steam_api) from sdk/redistributable_bin/ folders to the Godot binary location (by default in the godot source /bin/ file but you can move them to a new folder). It should look like this:
  - Linux 32/64-bit
  ```
  libsteam_api.so
  ./godot.linux.tools.32 or ./godot.linux.tools.64
  ```
  - OSX
  ```
  libsteam_api.dylib
  ./godot.osx.tools.32 or ./godot.osx.tools.64
  ```
  - Windows 32-bit
  ```
  steam_api.dll
  ./godot.windows.tools.32.exe
  ```
  - Windows 64-bit
  ```
  steam_api64.dll
  ./godot.windows.tools.64.exe
  ```
- Your game must ship with the executable, Steam API DLL/SO/DyLIB, and steam_appid.txt to function. Lack of the Steam API DLL/SO/DyLib (for your respective OS) or the steam_appid.txt will cause it fail and crash.
  - **NOTE:** For OSX, the libsteam_api.dylib and steam_appid.txt must be in the Content/MacOS/ folder in your application zip or the game will crash.

From here you should be able to call various functions of Steamworks. You should be able to look up the functions in Godot itself under the search section. In addition, you should be able to read the Steamworks API documentation to see what all is available and cross-reference with GodotSteam.

Donate
-------------
If you feel like buying me a beer, feel free to do so here: https://www.paypal.me/sithlordkyle

License
-------------
MIT license
