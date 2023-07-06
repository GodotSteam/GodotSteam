# GodotSteam for GDExtension
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 4.x). For the Windows, Linux, and Mac platforms.

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [Server](https://github.com/Gramps/GodotSteam/tree/server)

Documentation
---
[Documentation is available here](https://godotsteam.com/).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v4.2.4)_ of this repo here.](https://github.com/Gramps/GodotSteam/releases)

**Version 4.2.4 Changes**
- Added: new Input callback input_gamepad_slot_change
- Added: new User callback get_ticket_for_web_api
- Added: new User function getAuthTicketForWebApi
- Changed: getAuthSessionTicket argument is now optional, defaults to NULL
- Changed: some internal variable names to fix compiling warnings and conflicts

**Version 4.2.3 Changes**
- Added: new return values for _overlay_toggled_; this will break compatibility with this
- Added: new Input and Parental Settings enums
- Added: new UGC Content Descriptor ID enums
- Added: new UGC functions _removeContentDescriptor_, _addContentDescriptor_, and _getQueryUGCContentDescriptors_
- Added: new signal _filter_text_dictionary_changed_
- Changed: getAuthSessionTicket now uses networking identities
- Changed: gamepad_text_input_dismissed now passes back the app ID
- Changed: Steam Input max analog and digital actions values
- Removed: ERegisterActivationCodeResult due to removal in SDK

**Version 4.2.2 Changes**
- Changed: names of all godotsteam dll/so/dylib files to be less verbose

**Version 4.2.1 Changes**
- Added: compiled dylib files for OSX, _thanks to definitelyokay_
- Fixed: names of OSX files in the godotsteam.gdextension file

**Version 4.2 Changes**
- Changed: brought the plug-in version up to speed with the module version

Known Issues
---
- GDExtension for 4.1 is **not** compatible with 4.0.3 or lower. Please check the versions you are using.
- Overlay will not work in the editor but will work in export projects when uploaded to Steam.  This seems to a limitation with Vulkan currently.

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
