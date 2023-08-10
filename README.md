# GodotSteam for GDExtension
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 4.x). For the Windows, Linux, and Mac platforms.

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/godot3)
- [GDExtension](https://github.com/Gramps/GodotSteam/tree/gdextension)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [Server 3.x](https://github.com/Gramps/GodotSteam/tree/server)

Documentation
---
[Documentation is available here](https://godotsteam.com/).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v4.3)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

**Version 4.3 Changes**
- Added: full GodotSteam documentation into the editor
- Added: steamShutdown to allow Steamworks to be manually shutdown
- Added: requestEquippedProfileItems function and equipped_profile_items callback
- Added: Steam Deck as Steam Input type
- Changed: all enums are now directly linked to their SDK counterparts
- Changed: getDigitalActionData returned keys are now state and active
- Changed: names of some Steam enums to be cleaner and leaner
- Changed: getAppInstallDir now returns dictionary with absolute path and install size
- Fixed: some missing enum binds
- Fixed: missing function argument binds
- Fixed: get_ticket_for_web_api sending back strings
- Removed: enums that are not in the SDK but Valve's docs

[You can read more change-logs here](https://godotsteam.com/changelog/gdextension/).

Known Issues
---
- GDExtension for 4.1 is **not** compatible with 4.0.3 or lower. Please check the versions you are using.
- Overlay will not work in the editor but will work in export projects when uploaded to Steam.  This seems to a limitation with Vulkan currently.
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.

Quick How-To
---
For complete instructions on how to build the GDExtension version of GodotSteam, [please refer to our documentation's 'How-To GDExtension' section.](https://godotsteam.com/howto_gdextension/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/Gramps/GodotSteam/releases) or [from the Godot Asset Library](https://godotengine.org/asset-library/asset/1768) and skip compiling it yourself!

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
