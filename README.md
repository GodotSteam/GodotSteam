# GodotSteam for Godot Engine
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)
- [GDExtension](https://github.com/Gramps/GodotSteam/tree/gdextension)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [Server 3.x](https://github.com/Gramps/GodotSteam/tree/server)

Documentation
---
[Documentation is available here](https://godotsteam.com).  You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v3.20)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

**Version 3.20.1 Changes**
- Fixed: wrong variant type for join_requested

**Version 3.20 Changes**
- Added: full GodotSteam documentation into the editor
- Added: steamShutdown to allow Steamworks to be manually shutdown
- Added: requestEquippedProfileItems function and equipped_profile_items callback
- Added: Steam Deck as Steam Input typ
- Changed: all enums are now directly linked to their SDK counterparts
- Changed: getDigitalActionData returned keys are now state and active
- Changed: names of some Steam enums to be cleaner and leaner
- Changed: getAppInstallDir now returns dictionary with absolute path and install size
- Fixed: some missing enum binds
- Fixed: missing function argument binds
- Removed: enums that are not in the SDK but Valve's docs

[You can read more change-logs here](https://godotsteam.com/changelog/godot3/).

Known Issues
---
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.
- **As of Steamworks SDK 1.53,** you cannot compile with previous version of GodotSteam (3.11.1 or earlier) due to a code change in the SDK.
  - Using _**Steamworks SDK 1.53 or newer**_, you must use _**GodotSteam 3.12 or newer**_.
  - Using _**Steamworks SDK 1.53 or earlier**_, you must use _**GodotSteam 3.11.1 or earlier**_.

"Quick" How-To
---
For complete instructions on how to build the Godot 3.x version of GodotSteam, [please refer to our documentation's 'How-To Modules' section.](https://godotsteam.com/howto_modules/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/Gramps/GodotSteam/releases) and skip compiling it yourself!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps), [Ko-Fi](https://ko-fi.com/grampsgarcia) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
