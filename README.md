# GodotSteam for GDNative
Steam API for the Godot game engine (version 3.3.3). For the Windows, Linux, and Mac platforms. 

**WARNING**: This GDNative branch is still in active development! While functional, I do not suggest using it for production code for your game; testing only.  Please use one of the GodotSteam modules instead.

Additional flavors include:
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master)
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Server](https://github.com/Gramps/GodotSteam/tree/server)

Documentation
----------
[Documentation is available here](https://gramps.github.io/GodotSteam/) and [there is the project's Wiki page here](https://github.com/Gramps/GodotSteam/wiki).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
----------
**Version 3.0.2 Changes**
- Added: for the add-on only, new production-ready API libs were made

**Version 3.0.1 Changes**
- Fixed: two issues with godotsteam.cpp that causes compiling error on Linux

**Version 3.0.0 Changes**
- Added: all missing functions to bring GDNative version in-line with module version
- Changed: rebuilt and restructured layout of project folder
- Removed: enums as they don't work in GDNative
- Removed: default arguments for functions as they don't work in GDNative

Known Issues
----------
- The GDNative version does not allow for default arguments in functions, thus some functions may have odd behaviors.  If you are using this version of GodotSteam you are required to pass any argument that has a default in the module version. Also, there are no enums in the GDNative version due to how it is structured.

Quick How-To
----------
Obtain the plugin through one of two ways:
- Visit the Godot Asset Library either through the website or in the editor and search for GodotSteam.
- Download this repo and unzip it into the base of your game project.

Go into **Project > Settings > Plugins** and activate the plugin.

Tinker with Steamworks!

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Patreon](https://patreon.com/coaguco) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
