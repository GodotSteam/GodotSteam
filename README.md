# GodotSteam for Godot Engine
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 4.x). For the Windows, Linux, and Mac platforms.

Additional flavors include:
- [Godot 2.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot2)
- [Godot 3.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot3)
- [GDExtension](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdextension)
- [GDNative](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdnative)
- [Server 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/server3)
- [Server 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/server4)
- [Server GDExtension](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdextension)
- [Server GDNative](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdnative)

Documentation
---
[Documentation is available here](https://godotsteam.com/). You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v4.4.1)_ of this repo here](https://github.com/CoaguCo-Industries/GodotSteam/releases).

**Version 4.4.1 Changes**
- Fixed: missing descriptions for some in-editor functions/signals
- Fixed: `receiveMessagesOnChannel`, `receiveMessagesOnPollGroup`, and `receiveMessagesOnConnection` had overwriting dictionary keys

**Version 4.4 Changes**

- Added: new enums and constant related to new Steam initialization function
- Added: new enums for NetworkingConfigValue
- Added: new intialization function `steamInitEx` that is more verbose
- Added: new UGC function `getUserContentDescriptorPreferences`
- Added: new Remote Play function `startRemotePlayTogether`
- Changed: UGC function`setItemTags` to have new argument for admin tags
- Changed: compatible with Steamworks SDK 1.58
- Changed: in-editor docs now reflect all changes

[You can read more change-logs here](https://godotsteam.com/changelog/godot4/).

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
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia), or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
