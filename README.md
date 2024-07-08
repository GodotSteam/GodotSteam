# GodotSteam Sponsors - GDNative
A series of projects for GodotSteam sponsors to check out before release.  You can also guide how upcoming development by [weighing in over in the Discussion section](https://github.com/GodotSteam/GodotSteam-Sponsors/discussions), submitting pull requests, or discussing in our [Discord server](https://discord.gg/SJRSq6K).

Additional Flavors
---
Pre-Compiles | Plug-ins
--- | ---
[Godot 3.x](https://github.com/GodotSteam/GodotSteam-Sponsors/tree/godot3) | [GDNative](https://github.com/GodotSteam/GodotSteam-Sponsors/tree/gdnative)
[Godot 4.x](https://github.com/GodotSteam/GodotSteam-Sponsors/tree/godot4) | [GDExtension](https://github.com/GodotSteam/GodotSteam-Sponsors/tree/gdextension)
[Multiplayer Peer](https://github.com/GodotSteam/GodotSteam-Sponsors/tree/multiplayer-peer)

Upcoming Branches
---
- GodotSteam Components : A collection of custom nodes and editor tools for speeding up Steamworks development
- Skillet : Early beta key to the open-source, free-to-play game showcasing GodotSteam's functionality.

Documentation
---
[Documentation is available here](https://godotsteam.com/). You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Feel free to chat with us about GodotSteam on the [Discord server](https://discord.gg/SJRSq6K).

Current Build
----------
You can [download pre-compiled versions of this repo here](https://github.com/CoaguCo-Industries/GodotSteam/releases).

**Version 3.26 Changes**
- Added: missing `user_achievement_icon_fetched` callback
- Added: new functions to Apps class
- Added: new Steam Timeline class functions
- Added: new functions to UGC class
- Changed: updated for Steamworks SDK 1.60
- Changed: `network_messages_session_failed` callback now returns the Steam ID associated with the user whose session failed
- Changed: `global_stats_received` had call result name change under-the-hood, does not affect anything
- Changed: `sendMessages()` now returns the message result
- Changed: `getQueryUGCResult()` now passes back additional value total_files_size
- Changed: `item_installed` signal now returns additional data - legacy_content and manifest_id
- Fixed: incorrect signal name for `inventory_definition_update`, ***thanks to Foxushka***

[You can read more change-logs here.](https://godotsteam.com/changelog/gdnative/)

Compatibility
---
While rare, sometimes Steamworks SDK updates will break compatilibity with older GodotSteam versions. Any compatability breaks are noted below. Newer API files (dll, so, dylib) _should_ still work for older versions.

Steamworks SDK Version | GodotSteam Version
---|---
1.59 or newer | 3.23 or newer
1.53 to 1.58a | 3.12 to 3.22.4
1.52 or older | 3.11.1 or older

Versions of GodotSteam that have compatibility breaks introduced.

GodotSteam Version | Broken Compatibility
---|---
3.25 | Networking identity system removed, replaced with Steam IDs
3.26 | sendMessages returns an Array

Known Issues
----------
- The GDNative version does not allow for default arguments in functions, thus some functions may have odd behaviors.  If you are using this version of GodotSteam you are required to pass any argument that has a default in the module version. Also, there are no enums in the GDNative version due to how it is structured.
- The function `setRichPresence` when used on Windows will occasionally send the key as the value. This behavior does not happen on Linux nor OSX; it also doesn't exist in any versions of the module nor GDExtension.  In which case, please check your rich presence is set correctly by reading the rich presence back when using Windows and GDNative.

Quick How-To
----------
For complete instructions on how to build the GDNative version of GodotSteam, [please refer to our documentation's 'How-To GDNative' section.](https://godotsteam.com/howto/gdnative/)  It will have the most up-to-date information.

Alternatively, you can just [download the pre-compiled versions in our Releases section](https://github.com/GodotSteam/GodotSteam/releases) or [from the Godot Asset Library](https://godotengine.org/asset-library/asset/1045) and skip compiling it yourself!

Need Help?
---
As a sponsor, you can reach out to me [directly by e-mail at gp@godotsteam.com](mailto:gp@godotsteam.com) or on Discord at grampsgarcia.

Thank You!
---
Thank you so much for supporting this project!

I am always looking for some additional perks to provide to sponsors to show appreciation for you all supporting the project.  If you have any ideas, please feel free to hit me up at either of the places above!

License
---
MIT license
