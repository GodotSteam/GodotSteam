# GodotSteam for GDNative
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

Additional Flavors
---
Pre-Compiles | Plug-ins | Server | Examples/Demos
--- | --- | --- | ---
[Godot 2.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot2)| [GDNative](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdnative) | [Server 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/godot3) | [Godot 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/godot3)
[Godot 3.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot3) | [GDExtension](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdextension) | [Server 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/godot4) |  [Godot 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/godot4)
[Godot 4.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot4) | --- | [GDNative](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdnative) | [Server 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/server3)
--- | --- | [GDExtension](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdextension) | [Server 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/server4)

Documentation
----------
[Documentation is available here](https://godotsteam.com/).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
----------
You can [download pre-compiled versions _(currently v3.22.2)_ of this repo here](https://github.com/CoaguCo-Industries/GodotSteam/releases).

**Version 3.22.2 Changes**
- Changed: reverted `steamInit` and `steamInitEx` as new methods won't work with GDNative
- Removed: all enums as they cannot be used in GDNative

**Version 3.22.1 Changes**
- Added: two new arguments to `steamInit` and `steamInitEx` to set your app ID and run_callbacks interally, thanks to _GreenFox_
- Fixed: issue with callback that caused compiling failure in Linux

**Version 3.22 Changes**
- Added: two Music class callbacks
- Changed: `generateItems`, `exchangeItems`, `getItemsByID`, and `startPurchase` all list-based arguments are now PoolIntArrays
- Changed: `getItemsByID` now takes one argument, counts the elements in the passed array instead
- Changed: `getItemsWithPrices` no longer requires any arguments passed to it
- Changed: in-editor docs have been updated
- Fixed: `getResultItems` now returns all item data
- Fixed: missing DEFVAL for `steamInitEx`
- Fixed: Joy Con name in `getInputTypeForHandle`
- Removed: `getNumItemsWithPrices` as it was unnecessary

[You can read more change-logs here.](https://godotsteam.com/changelog/gdnative/)

Known Issues
----------
- The GDNative version does not allow for default arguments in functions, thus some functions may have odd behaviors.  If you are using this version of GodotSteam you are required to pass any argument that has a default in the module version. Also, there are no enums in the GDNative version due to how it is structured.
- The function `setRichPresence` when used on Windows will occasionally send the key as the value. This behavior does not happen on Linux nor OSX; it also doesn't exist in any versions of the module nor GDExtension.  In which case, please check your rich presence is set correctly by reading the rich presence back when using Windows and GDNative.

Quick How-To
----------
For complete instructions on how to build the GDNative version of GodotSteam, [please refer to our documentation's 'How-To GDNative' section.](https://godotsteam.com/howto/gdnative/)  It will have the most up-to-date information.


Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/CoaguCo-Industries/GodotSteam/releases) or [from the Godot Asset Library](https://godotengine.org/asset-library/asset/1045) and skip compiling it yourself!

Usage
----------
Do not use the GDNative version of GodotSteam with any of the module versions whether it be our pre-compiled versions or ones you compile.  They are not compatible with each other.

When exporting with the GDNative version, please use the normal Godot Engine templates instead of our GodotSteam templates or you will have a lot of issues.

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia), or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
