# GodotSteam MultiplayerPeer
An ecosystem of tools for Godot Engine (version 4.x) and Steam. MultiplayerPeer bridges Godot's multiplayer nodes with Steam networking. For the Windows, Linux, and Mac platforms.


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
[Documentation is available here](https://godotsteam.com/). You can also check out the Search Help section inside Godot Engine.

Feel free to chat with us or ask questions on the [GodotSteam Discord server](https://discord.gg/SJRSq6K).


Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps) or [Patreon](https://patreon.com/godotsteam)!


Current Build
---
You can [download pre-compiled versions of this repo here](https://github.com/GodotSteam/MultiplayerPeer/releases).

**Version 4.11 Changes**
- Changed: in-editor docs
- Changed: reorganized code-base
- Changed: more readable error for `sendPending`
- Changed: removed \_scb from callback names are they are unnecessary due to class call
- Changed: `lobby_chat_update` to just add or remove players
- Changed: chat contants names changed
- Fixed: missing argument hints for `lobby_data_update`
- Fixed: `get_peer_info` overwriting various dictionary keys
- Removed: string response to packet send failure, now returns actual error code
- Removed: matchmaking enums, were not used; can be taken from GodotSteam directly if needed
- Removed: `lobby_invite`, `lobby_match_list`, `lobby_kicked` as they are bound but not used in MP

[You can read more change-logs here](https://godotsteam.com/changelog/multiplayer_peer/).

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
4.11 | MP chat state constants changed

Known Issues
---
- Steam overlay will not work when running your game from the editor if you are using Forward+ as the renderer.  It does work with Compatibility though.  Your exported project will work perfectly fine in the Steam client, however.
- When self-compiling, using MinGW will cause crashes unless you are using GodotSteam 4.10 or newer.

"Quick" How-To
---
For complete instructions on how to build the GodotSteam MultiplayerPeer from scratch, [please refer to our documentation's 'How-To Multiplayer Peer' section.](https://godotsteam.com/howto/multiplayer_peer/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/GodotSteam/MultiplayerPeer/releases) and skip compiling it yourself!

[To start, check out our tutorial on initializing Steam.](https://godotsteam.com/tutorials/initializing/)  There are additional tutorials with more in the works.  You can also [check out additional Godot and Steam related videos, text, additional tools, plug-ins, etc. here.](https://godotsteam.com/tutorials/external/)

License
---
MIT license