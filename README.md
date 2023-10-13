# GodotSteam for GDExtension
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
[Documentation is available here](https://godotsteam.com/).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v4.4.2)_ of this repo here](https://github.com/CoaguCo-Industries/GodotSteam/releases).

**Version 4.4.3 Changes**
- Fixed: `requestEquippedProfileItems` was missing method bind, thanks to _BOTLANNER_
- Fixed: `get_ticket_for_web_api` callback for getting actual ticket buffer, thanks to _dicarne_
- Fixed: compiler complaining about comparison between Steam enum and GodotSteam enum for `steamInitEx`
- Changed: different defaults in Github Actions files
- Changed: `createBrowser` now sends proper NULL when empty string passed
- Changed: `html_browser_ready` from callback to proper call result
- Changed: cast handle in `setSize` as Steam HHTMLBrowser
- Removed: unnecessary `steam_appid.txt` from zips in Github Actions

**Version 4.4.2 Changes**
- Changed: bump for Godot 4.1.2

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
For complete instructions on how to build the GDExtension version of GodotSteam, [please refer to our documentation's 'How-To GDExtension' section.](https://godotsteam.com/howto/gdextension/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/CoaguCo-Industries/GodotSteam/releases) or [from the Godot Asset Library](https://godotengine.org/asset-library/asset/1972) and skip compiling it yourself!

Usage
----------
Do not use the GDExtension version of GodotSteam with any of the module versions whether it be our pre-compiled versions or ones you compile.  They are not compatible with each other.

When exporting with the GDExtension version, please use the normal Godot Engine templates instead of our GodotSteam templates or you will have a lot of issues.

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
