# GodotSteam for GDExtension
An ecosystem of tools for [Godot Engine](https://godotengine.org) and [Valve's Steam](https://store.steampowered.com). For the Windows, Linux, and Mac platforms.

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

**Version 4.11 Changes**
- Added: `getDLCData` to get all DLC information and `getDLCDataByIndex` now acts as Steam intended with an index passed
- Added: properties for all variants GodotSteam stores
- Changed: using Godot's macros for error reporting back to the editor
- Changed: buffer_size argument to `decompressVoice` with default of original value
- Changed: `steamworksError` replaced with prints to editor
- Changed: all options array parameters for all Sockets class functions changed to dictionaries, [check class docs](https://godotsteam.com/classes/networking_sockets/)
- Changed: deprecated `getAvailableVoice`, merged functionality into `getVoice`
- Changed: `setLeaderboardDetailsMax` changed to the set/get for leaderboard_details_max, now `set_leaderboard_details_max`
- Changed: Steam singleton now removed during uninitialization
- Fixed: proper type for `network_connection_status_changed`, thanks to ***stickyShift***
- Fixed: `getResultItemProperty` now takes empty string to send all property list, thanks to ***Stralor***
- Fixed: missing return value hints from `lobby_data_update`
- Fixed: `get_app_dependencies_result` now passed back app_ids array
- Fixed: missing argument hints for `item_installed` callback
- Fixed: both global stat history functions - `getGlobalStatIntHistory` and `getGlobalStatFloatHistory`
- Fixed: broken returned variable in `network_connection_status_changed`
- Fixed: a variety of small tweaks

[You can read more change-logs here](https://godotsteam.com/changelog/gdextension/).

Compatibility
---
While rare, sometimes Steamworks SDK updates will break compatilibity with older GodotSteam versions. Any compatability breaks are noted below.  Newer API files (dll, so, dylib) _should_ still work for older versions.

Steamworks SDK Version | GodotSteam Version
---|---
1.59 or newer | 4.6 or newer
1.58a or older | 4.5.4 or older

Versions of GodotSteam that have compatibility breaks introduced.

GodotSteam Version | Broken Compatibility
---|---
4.8 | Networking identity system removed, replaced with Steam IDs
4.9 | sendMessages returns an Array
4.11 | setLeaderboardDetailsMax removed

Known Issues
---
- GDExtension for 4.1 is **not** compatible with 4.0.3 or lower. Please check the versions you are using.
- Overlay will not work in the editor but will work in export projects when uploaded to Steam.  This seems to a limitation with Vulkan currently.
- When self-compiling, using MinGW will cause crashes unless you are using GodotSteam 4.10 or newer.

Quick How-To
---
For complete instructions on how to build the GDExtension version of GodotSteam, [please refer to our documentation's 'How-To GDExtension' section.](https://godotsteam.com/howto/gdextension/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/GodotSteam/GodotSteam/releases) or [from the Godot Asset Library](https://godotengine.org/asset-library/asset/2445) and skip compiling it yourself!

[To start, check out our tutorial on initializing Steam.](https://godotsteam.com/tutorials/initializing/)  There are additional tutorials with more in the works.  You can also [check out additional Godot and Steam related videos, text, additional tools, plug-ins, etc. here.](https://godotsteam.com/tutorials/external/)

Usage
---
Do not use the GDExtension version of GodotSteam with any of the module versions whether it be our pre-compiled versions or ones you compile.  They are not compatible with each other.

When exporting with the GDExtension version, please use the normal Godot Engine templates instead of our GodotSteam templates or you will have a lot of issues.

License
---
MIT license
