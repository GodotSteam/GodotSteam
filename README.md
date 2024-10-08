# GodotSteam for GDNative
An ecosystem of tools for [Godot Engine](https://godotengine.org) and [Valve's Steam](https://store.steampowered.com). For the Windows, Linux, and Mac platforms.

**Notice: GodotSteam GDNative is now retired. This branch still works fine but will not receive any further updates.**

Additional Flavors
---
Pre-Compiles | Plug-ins | Server | Examples/Demos
--- | --- | --- | ---
[Godot 2.x](https://github.com/GodotSteam/GodotSteam/tree/godot2)| [GDNative](https://github.com/GodotSteam/GodotSteam/tree/gdnative) | [Server 3.x](https://github.com/GodotSteam/GodotSteam-Server/tree/godot3) | [Godot 3.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/godot3)
[Godot 3.x](https://github.com/GodotSteam/GodotSteam/tree/godot3) | [GDExtension](https://github.com/GodotSteam/GodotSteam/tree/gdextension) | [Server 4.x](https://github.com/GodotSteam/GodotSteam-Server/tree/godot4) |  [Godot 4.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/godot4)
[Godot 4.x](https://github.com/GodotSteam/GodotSteam/tree/godot4) | --- | [GDNative](https://github.com/GodotSteam/GodotSteam-Server/tree/gdnative) | [Server 3.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/server3)
[Multiplayer Peer](https://github.com/GodotSteam/MultiplayerPeer)| --- | [GDExtension](https://github.com/GodotSteam/GodotSteam-Server/tree/gdextension) | [Server 4.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/server4)

Documentation
---
[Documentation is available here](https://godotsteam.com/).

Feel free to chat with us about GodotSteam or ask for assistance on the [Discord server](https://discord.gg/SJRSq6K).

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps) or [Patreon](https://patreon.com/godotsteam)! Both types of contributions keep things rolling!

Current Build
---
You can [download pre-compiled versions of this repo here](https://github.com/GodotSteam/GodotSteam/releases).

**Version 3.27 Changes**
- Added: `getDLCData` to get all DLC information and `getDLCDataByIndex` now acts as Steam intended with an index passed
- Added: properties for all variants GodotSteam stores
- Changed: buffer_size argument to `decompressVoice` with default of original value
- Changed: all options array parameters for all Sockets class functions changed to dictionaries, [check class docs](https://godotsteam.com/classes/networking_sockets/)
- Changed: deprecated `getAvailableVoice`, merged functionality into `getVoice`
- Changed: `setLeaderboardDetailsMax` changed to the set/get for leaderboard_details_max, now `set_leaderboard_details_max`
- Fixed: proper type for `network_connection_status_changed`, thanks to ***stickyShift***
- Fixed: `getResultItemProperty` now takes empty string to send all property list, thanks to ***Stralor***
- Fixed: missing return value hints from `lobby_data_update`
- Fixed: `get_app_dependencies_result` now passed back app_ids array
- Fixed: missing argument hints for `item_installed` callback
- Fixed: both global stat history functions - `getGlobalStatIntHistory` and `getGlobalStatFloatHistory`
- Fixed: broken returned variable in `network_connection_status_changed`
- Fixed: a variety of small tweaks

[You can read more change-logs here.](https://godotsteam.com/changelog/gdnative/)

Compatibility
---
While rare, sometimes Steamworks SDK updates will break compatilibity with older GodotSteam versions. Any compatability breaks are noted below. Newer API files (dll, so, dylib) _should_ still work for older versions.

Steamworks SDK Version | GodotSteam Version
---|---
1.59 or newer | 3.23 or newer
1.53 to 1.58a | 3.12 to 3.22.4
1.52 or older | 3.11.1 or older

Versions of GodotSteam that have compatibility breaks introduced.

GodotSteam Version | Broken Compatibility
---|---
3.25 | Networking identity system removed, replaced with Steam IDs
3.26 | sendMessages returns an Array
3.27 | setLeaderboardDetailsMax removed

Known Issues
---
- The GDNative version does not allow for default arguments in functions, thus some functions may have odd behaviors.  If you are using this version of GodotSteam you are required to pass any argument that has a default in the module version. Also, there are no enums in the GDNative version due to how it is structured.
- The function `setRichPresence` when used on Windows will occasionally send the key as the value. This behavior does not happen on Linux nor OSX; it also doesn't exist in any versions of the module nor GDExtension.  In which case, please check your rich presence is set correctly by reading the rich presence back when using Windows and GDNative.
- When self-compiling, using MinGW will cause crashes unless you are using GodotSteam 3.27 or newer.

"Quick" How-To
---
For complete instructions on how to build the GDNative version of GodotSteam, [please refer to our documentation's 'How-To GDNative' section.](https://godotsteam.com/howto/gdnative/)  It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/GodotSteam/GodotSteam/releases) or [from the Godot Asset Library](https://godotengine.org/asset-library/asset/1045) and skip compiling it yourself!

[To start, check out our tutorial on initializing Steam.](https://godotsteam.com/tutorials/initializing/)  There are additional tutorials with more in the works.  You can also [check out additional Godot and Steam related videos, text, additional tools, plug-ins, etc. here.](https://godotsteam.com/tutorials/external/)

Usage
---
Do not use the GDNative version of GodotSteam with any of the module versions whether it be our pre-compiled versions or ones you compile.  They are not compatible with each other.

When exporting with the GDNative version, please use the normal Godot Engine templates instead of our GodotSteam templates or you will have a lot of issues.

License
---
MIT license
