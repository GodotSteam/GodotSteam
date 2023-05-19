# GodotSteam for Godot Engine
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)
- [GDExtension](https://github.com/Gramps/GodotSteam/tree/gdextension)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [Server](https://github.com/Gramps/GodotSteam/tree/server)

Documentation
---
[Documentation is available here](https://godotsteam.com).  You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
---
You can [download pre-compiled versions _(currently v3.19.3)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

**Version 3.19.3 Changes**
- Added: new Input callback _input_gamepad_slot_change_
- Added: new User callback _get_ticket_for_web_api_
- Added: new User function _getAuthTicketForWebApi_
- Changed: getAuthSessionTicket argument is now optional, defaults to NULL

**Version 3.19.2 Changes**
- Added: new return values for _overlay_toggled_; this will break compatibility with this
- Added: new Input and Parental Settings enums
- Added: new UGC Content Descriptor ID enums
- Added: new UGC functions _removeContentDescriptor_, _addContentDescriptor_, and _getQueryUGCContentDescriptors_
- Added: new signal _filter_text_dictionary_changed_
- Changed: getAuthSessionTicket now uses networking identities
- Changed: gamepad_text_input_dismissed now passes back the app ID
- Changed: Steam Input max analog and digital actions values
- Removed: ERegisterActivationCodeResult due to removal in SDK

**Version 3.19.1 Changes**
- Fixed: issue with UGC tags occasionally getting lost upon update, _thanks to EIREXE_

**Version 3.19 Changes**
- Changed: various backports from godot4 branch
- Fixed: various lobby list functions now return the server_list_request handle
- Fixed: need for godotsteam.sh file on some Linux systems, _thanks to mikix_
- Fixed: typo in returned dictionary for createSocketPair
- Fixed: requestInternetServerList causing crashes

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
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
---
MIT license
