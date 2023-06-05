# GodotSteam for Godot 2.x
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 2.x). For the Windows, Linux, and Mac platforms. 

Additional flavors include:
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [GDExtension](https://github.com/Gramps/GodotSteam/tree/gdextension)

**Notice:** GodotSteam for Godot 2.x is now retired. Please use the Godot 3.x version and update your game. This branch still works fine but will not receive any further updates.

Documentation
----------
[Documentation is available here](https://godotsteam.com/).

You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
----------
You can download pre-compiled versions _(currently v1.10)_ of this repo here: https://github.com/Gramps/GodotSteam/releases

**Version 1.10 Changes**
- Added: more verbose response to steamInit, now returns a dictionary
- Added: missing initialization constants
- Changed: steamInit to give actual response on Steamworks status (from bool to int)
- Changed: minor formatting to match Godot 3 version
- Fixed: currentAppID not utilized correctly
- Removed: unneeded gameInfo struct

Known Issues
----------
- **Using MinGW causes crashes.** I strong recommend you **do not use MinGW** to compile at this time.

Quick How-To
----------
[Full instructions can be found in our documentation here.](https://godotsteam.com/howto_modules/)

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia), or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
