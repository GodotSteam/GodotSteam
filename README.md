# GodotSteam for Godot Engine
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

Additional flavors include:
- [Godot 2.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot2)
- [Godot 4.x](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot4)
- [GDExtension](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdextension)
- [GDNative](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdnative)
- [Server 3.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/server3)
- [Server 4.x](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/server4)
- [Server GDExtension](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdextension)
- [Server GDNative](https://github.com/CoaguCo-Industries/GodotSteam-Server/tree/gdnative)

Documentation
---
[Documentation is available here](https://godotsteam.com).  You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v3.21.2)_ of this repo here](https://github.com/CoaguCo-Industries/GodotSteam/releases).

**Version 3.21.3 Changes**
- Fixed: `requestEquippedProfileItems` was missing method bind, thanks to _BOTLANNER_
- Fixed: `get_ticket_for_web_api` callback for getting actual ticket buffer, thanks to _dicarne_
- Fixed: compiler complaining about comparison between Steam enum and GodotSteam enum for `steamInitEx`
- Changed: `createBrowser` now sends proper NULL when empty string passed
- Changed: `html_browser_ready` from callback to proper call result
- Changed: cast handle in `setSize` as Steam HHTMLBrowser
- Removed: unnecessary `steam_appid.txt` from zips in Github Actions

**Version 3.21.2 Changes**
- Fixed: missing descriptions for some in-editor functions/signals
- Fixed: `receiveMessagesOnChannel`, `receiveMessagesOnPollGroup`, and `receiveMessagesOnConnection` had overwriting dictionary keys

**Version 3.21.1 Changes**
- Changed: bump for Godot 3.5.3

**Version 3.21 Changes**
- Added: new enums and constant related to new Steam initialization function
- Added: new enums for NetworkingConfigValue
- Added: new intialization function `steamInitEx` that is more verbose
- Added: new UGC function `getUserContentDescriptorPreferences`
- Added: new Remote Play function `startRemotePlayTogether`
- Changed: UGC function`setItemTags` to have new argument for admin tags
- Changed: compatible with Steamworks SDK 1.58
- Changed: in-editor docs now reflect all changes
- Fixed: `gamepad_text_input_dismissed` signal passing back string instead of int for `app_id` 

[You can read more change-logs here](https://godotsteam.com/changelog/godot3/).

Known Issues
---
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.
- **As of Steamworks SDK 1.53,** you cannot compile with previous version of GodotSteam (3.11.1 or earlier) due to a code change in the SDK.
  - Using _**Steamworks SDK 1.53 or newer**_, you must use _**GodotSteam 3.12 or newer**_.
  - Using _**Steamworks SDK 1.53 or earlier**_, you must use _**GodotSteam 3.11.1 or earlier**_.

"Quick" How-To
---
For complete instructions on how to build the Godot 3.x version of GodotSteam from scratch, [please refer to our documentation's 'How-To Modules' section.](https://godotsteam.com/howto/modules/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/CoaguCo-Industries/GodotSteam/releases) and skip compiling it yourself!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
