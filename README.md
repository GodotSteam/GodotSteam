# GodotSteam for GDExtension
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 4.x). For the Windows, Linux, and (soon) Mac platforms. 

Additional flavors include:
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [Server](https://github.com/Gramps/GodotSteam/tree/server)

Documentation
---
[Documentation is available here](https://godotsteam.com/).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v4.2.3)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

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
- Overlay will not work in the editor but will work in export projects when uploaded to Steam.  This seems to a limitation with Vulkan currently.

Quick How-To
---
Obtain the plugin through one of two ways:
- Visit the [Godot Asset Library](https://godotengine.org/asset-library/asset/7469) either through the website or in the editor and search for GodotSteam.
- Download this repo and unzip it into the base of your game project.

You will need to add the steam_appid.txt file with 480 or your game's app ID to where ever you have your Godot editor.  It should just work without having to do anything else.

Tinker with Steamworks!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
