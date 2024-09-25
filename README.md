# GodotSteam for Godot Engine 4.x
An ecosystem of tools for [Godot Engine](https://godotengine.org) and [Valve's Steam](https://store.steampowered.com). For the Windows, Linux, and Mac platforms.

Additional Flavors
---
Pre-Compiles | Plug-ins | Server | Examples/Demos
--- | --- | --- | ---
[Godot 2.x](https://github.com/GodotSteam/GodotSteam/tree/godot2)| [GDNative](https://github.com/GodotSteam/GodotSteam/tree/gdnative) | [Server 3.x](https://github.com/GodotSteam/GodotSteam-Server/tree/godot3) | [Godot 3.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/godot3)
[Godot 3.x](https://github.com/GodotSteam/GodotSteam/tree/godot3) | [GDExtension](https://github.com/GodotSteam/GodotSteam/tree/gdextension) | [Server 4.x](https://github.com/GodotSteam/GodotSteam-Server/tree/godot4) |  [Godot 4.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/godot4)
[Godot 4.x](https://github.com/GodotSteam/GodotSteam/tree/godot4) | --- | [GDNative](https://github.com/GodotSteam/GodotSteam-Server/tree/gdnative) | [Server 3.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/server3)
[Multiplayer Peer](https://github.com/GodotSteam/GodotSteam/tree/multiplayer-peer)| --- | [GDExtension](https://github.com/GodotSteam/GodotSteam-Server/tree/gdextension) | [Server 4.x](https://github.com/GodotSteam/GodotSteam-Example-Project/tree/server4)

Documentation
---
[Documentation is available here](https://godotsteam.com/). You can also check out the Search Help section inside Godot Engine.

Feel free to chat with us about GodotSteam or ask for assistance on the [Discord server](https://discord.gg/SJRSq6K).

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps) or [Patreon](https://patreon.com/godotsteam)! Both types of contributions keep things rolling!

Current Build
---
You can [download pre-compiled versions of this repo here](https://github.com/GodotSteam/GodotSteam/releases).

**Version 4.11 Changes**
- Added: `getDecompressedVoice` which utilizes `getVoice`, `decompressVoice`, `getAvailableVoice`, and `getVoiceOptimalSampleRate` in C++
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
- Removed: duplicate call on setting embedded callbacks

[You can read more change-logs here](https://godotsteam.com/changelog/godot4/).

Compatibility
---
While rare, sometimes Steamworks SDK updates will break compatilibity with older GodotSteam versions. Any compatability breaks are noted below. Newer API files (dll, so, dylib) _should_ still work for older versions.

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
- Steam overlay will not work when running your game from the editor if you are using Forward+ as the renderer.  It does work with Compatibility though.  Your exported project will work perfectly fine in the Steam client, however.
- When self-compiling, using MinGW will cause crashes unless you are using GodotSteam 4.10 or newer.

"Quick" How-To
---
For complete instructions on how to build the Godot 4.x version of GodotSteam from scratch, [please refer to our documentation's 'How-To Modules' section.](https://godotsteam.com/howto/modules/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/GodotSteam/GodotSteam/releases) and skip compiling it yourself!

[To start, check out our tutorial on initializing Steam.](https://godotsteam.com/tutorials/initializing/)  There are additional tutorials with more in the works.  You can also [check out additional Godot and Steam related videos, text, additional tools, plug-ins, etc. here.](https://godotsteam.com/tutorials/external/)

License
---
MIT license
