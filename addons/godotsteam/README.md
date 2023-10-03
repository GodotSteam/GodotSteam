# GodotSteam for GDExtension
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 4.x). For the Windows 32/64-bit, Linux 32/64-bit, and Mac uinversal platforms. 

Additional flavors include:
- [Godot 2.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot2)
- [Godot 3.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot3)
- [Godot 4.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot4)
- [GDNative](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdnative)
- [Server 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/server3)
- [Server 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/server4)

Documentation
---
[Documentation is available here](https://godotsteam.com/).

Feel free to chat with us about Godotteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

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

[You can read more change-logs here](https://godotsteam.com/changelog/gdextension/).

Known Issues
---
- GDExtension for 4.1 is **not** compatible with 4.0.3 or lower. Please check the versions you are using.
- Overlay will not work in the editor but will work in export projects when uploaded to Steam.  This seems to a limitation with Vulkan currently.
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.

Quick How-To
---
Obtain the plugin through one of two ways:
- Visit the [Godot Asset Library](https://godotengine.org/asset-library/asset/7469) either through the website or in the editor and search for GodotSteam.
- Download this repo and unzip it into the base of your game project.

You will need to add the steam_appid.txt file with 480 or your game's app ID to where ever you have your Godot editor.  It should just work without having to do anything else.

Tinker with Steamworks!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia), or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
