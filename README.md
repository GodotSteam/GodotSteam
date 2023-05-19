# GodotSteam for Godot Engine
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 4.x). For the Windows, Linux, and Mac platforms.

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master)
- [GDExtension](https://github.com/Gramps/GodotSteam/tree/gdextension)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [Server](https://github.com/Gramps/GodotSteam/tree/server)

Documentation
---
[Documentation is available here](https://godotsteam.com/). You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v4.2.2)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

**Version 4.2.2 Changes**
- Added: new Input callback input_gamepad_slot_change
- Added: new User callback get_ticket_for_web_api
- Added: new User function getAuthTicketForWebApi
- Changed: getAuthSessionTicket argument is now optional, defaults to NULL
- Changed: some internal variable names to fix compiling warnings and conflicts

**Version 4.2.1 Changes**
- Added: new return values for _overlay_toggled_; this will break compatibility with this
- Added: new Input and Parental Settings enums
- Added: new UGC Content Descriptor ID enums
- Added: new UGC functions _removeContentDescriptor_, _addContentDescriptor_, and _getQueryUGCContentDescriptors_
- Added: new signal _filter_text_dictionary_changed_
- Changed: getAuthSessionTicket now uses networking identities
- Changed: gamepad_text_input_dismissed now passes back the app ID
- Changed: Steam Input max analog and digital actions values
- Removed: ERegisterActivationCodeResult due to removal in SDK

**Version 4.2 Changes**
- Added: various backports from Godot 3.x branch
- Fixed: options array size for new Networking classes and memory leaks, _thanks to profour_
- Fixed: need for godotsteam.sh file on some Linux systems, _thanks to mikix_

[You can read more change-logs here](https://godotsteam.com/changelog/godot4/).

Known Issues
---
- Steam overlay will not work when running your game from the editor if you are using Forward+ as the renderer.  It does work with Compatibility though.  Your exported project will work perfectly fine in the Steam client, however.
- **Using MinGW causes crashes.** I strongly recommend you **do not use MinGW** to compile at this time.

"Quick" How-To
---
For complete instructions on how to build the Godot 3.x version of GodotSteam, [please refer to our documentation's 'How-To Modules' section.](https://godotsteam.com/howto_modules/) It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/Gramps/GodotSteam/releases) and skip compiling it yourself!

Donate
---
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
