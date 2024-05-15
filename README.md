# GodotSteam for Godot Engine
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 4.x). For the Windows, Linux, and Mac platforms.

Additional Flavors
---
Pre-Compiles | Plug-ins | Server | Examples/Demos
--- | --- | --- | ---
[Godot 2.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot2)| [GDNative](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdnative) | [Server 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/godot3) | [Godot 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/godot3)
[Godot 3.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot3) | [GDExtension](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdextension) | [Server 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/godot4) |  [Godot 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/godot4)
[Godot 4.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot4) | --- | [GDNative](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdnative) | [Server 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/server3)
[Multiplayer Peer](https://github.com/CoaguCo-Industries/GodotSteam/tree/multiplayer-peer)| --- | [GDExtension](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdextension) | [Server 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project/tree/server4)

Documentation
---
[Documentation is available here](https://godotsteam.com/). You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions of this repo here](https://github.com/CoaguCo-Industries/GodotSteam/releases).

**Version 4.8 Changes**
- Added: Steam Matchmaking response handlers, ***thanks to jeremybeier***
- Added: all missing Messages and Sockets constants
- Changed: Networking Messages, Sockets, and Utils now use Steam IDs instead of identity system
- Changed: UserUGCListSortOrder enums for readability
- Changed: UGCContentDescriptorID enums for readability
- Changed: `getResultStatus()` now returns the integer / enum
- Changed: cleaned up `addItemPreviewFile()`, `check_file_signature`, and `showGamepadTextInput()`
- Changed: various bits and pieces
- Changed: IP logic for all related functions
- Changed: `addFavoriteGame()`, `initiateGameConnection()`, `terminateGameConnection()`, and `removeFavoriteGame()` now take strings for IP
- Changed: `getAuthSessionTicket()` now defaults to 0 for Steam ID
- Changed: IP address now accepted instead of IP references
- Fixed: `getFriendCount()` has correct bit-wise value
- Fixed: server browser functionality, ***thanks to jeremybeier***
- Fixed: wrong string IP conversions, ***thanks to jeremybeier***
- Fixed: server list request filters, ***thanks to jeremybeier***
- Fixed: typo with UGC_MATCHING_UGC_TYPE_ITEMS enum
- Fixed: minor case issue with Workshop enums
- Fixed: `playerDetails()`, `requestFavoritesServerList()`, `requestInternetServerList()`, `requestSpectatorServerList()`, `requestFriendsServerList()`, `requestHistoryServerList()`, and `pingServer()`, ***thanks to jeremybeier***
- Fixed: regressions caused by minor update
- Fixed: typo with NETWORKING_CONFIG_TYPE_STRING enum
- Fixed: typo with LOBBY_COMPARISON_EQUAL_TO_GREATER_THAN
- Fixed: in-editor docs
- Removed: Networking Types identity system and related bits
- Removed: P2P Networking constants as they are duplicates of the P2PSend enum
- Removed: previous, non-functioning Matchmaking Server call results
- Removed: `getIdentity()` as it is redundant now

[You can read more change-logs here](https://godotsteam.com/changelog/godot4/).

Compatibility
---
While rare, sometimes Steamworks SDK updates will break compatilibity with older GodotSteam versions. Any compatability breaks are noted below. Newer API files (dll, so, dylib) _should_ still work for older versions.

Steamworks SDK Version | GodotSteam Version
---|---
1.59 or newer | 4.6 or newer
1.58a or older | 4.5.4 or older

GodotSteam Version | Broken Compatibility
---|---
4.8 | Networking identity system removed, replaced with Steam IDs

Known Issues
---
- Steam overlay will not work when running your game from the editor if you are using Forward+ as the renderer.  It does work with Compatibility though.  Your exported project will work perfectly fine in the Steam client, however.
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.

"Quick" How-To
---
For complete instructions on how to build the Godot 4.x version of GodotSteam scratch, [please refer to our documentation's 'How-To Modules' section.](https://godotsteam.com/howto/modules/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/CoaguCo-Industries/GodotSteam/releases) and skip compiling it yourself!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps)!

License
---
MIT license
