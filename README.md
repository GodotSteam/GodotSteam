# GodotSteam for Godot Engine
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
---
[Documentation is available here](https://godotsteam.com).  You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v3.22)_ of this repo here](https://github.com/CoaguCo-Industries/GodotSteam/releases).

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

[You can read more change-logs here](https://godotsteam.com/changelog/godot3/).

Known Issues
---
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.

Compatibility
---
While rare, sometimes Steamworks SDK updates will break compatilibity with older GodotSteam versions. Any compatability breaks are noted below.

Steamworks SDK Version | GodotSteam Version
---|---
1.53 or newer | 3.12 or newer
1.53 or older | 3.11.1 or older

"Quick" How-To
---
For complete instructions on how to build the Godot 3.x version of GodotSteam, [please refer to our documentation's 'How-To Modules' section.](https://godotsteam.com/howto/modules/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/CoaguCo-Industries/GodotSteam/releases) and skip compiling it yourself!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
