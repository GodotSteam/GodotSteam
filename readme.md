# GodotSteam for Godot Engine
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

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
[Documentation is available here](https://godotsteam.com).  You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions of this repo here](https://github.com/GodotSteam/GodotSteam/releases).

**Version 3.26 Changes**
- Added: missing `user_achievement_icon_fetched` callback
- Added: new functions and enums to Apps class
- Added: new Steam Timeline class functions, enums, and constants
- Added: new functions to UGC class
- Added: new enum BetaBranchFlags
- Added: new value NETWORKING_CONFIG_IP_LOCAL_HOST_ALLOW_WITHOUT_AUTH for NetworkingConfigValue enum
- Changed: updated for Steamworks SDK 1.60
- Changed: `network_messages_session_failed` callback now returns the Steam ID associated with the user whose session failed
- Changed: `global_stats_received` had call result name change under-the-hood, does not affect anything
- Changed: `sendMessages()` now returns the message result
- Changed: `getQueryUGCResult()` now passes back additional value total_files_size
- Changed: enum RemoteStoragePlatform now cast as uint32_t, fixes Rust compatibility, ***thanks to GreenFox***
- Changed: `item_installed` signal now returns additional data - legacy_content and manifest_id
- Fixed: incorrect signal name for `inventory_definition_update`, ***thanks to Foxushka***

[You can read more change-logs here](https://godotsteam.com/changelog/godot3/).

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

Known Issues
---
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.

"Quick" How-To
---
For complete instructions on how to build the Godot 3.x version of GodotSteam, [please refer to our documentation's 'How-To Modules' section.](https://godotsteam.com/howto/modules/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/GodotSteam/GodotSteam/releases) and skip compiling it yourself!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps)!

License
---
MIT license
