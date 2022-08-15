# GodotSteam for GDNative
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)

Documentation
----------
[Documentation is available here](https://gramps.github.io/GodotSteam/) and [is mirrored on and exported from CoaguCo's site](https://coaguco.com/godotsteam).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
----------
You can [download pre-compiled versions _(currently v3.4.1)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

**Version 3.4.1 Changes:**
- Fixed: OSX file name for DyLib

**Version 3.4 Changes:**
- Added: setDLCContext, getProfileItemPropertyString, getProfileItemPropertyInt, hasEquippedProfileItem, getPublicIP, getPSNID, getStadiaID, getXboxPairwiseID functions
- Added: handle arguments to HTML Surface class functions
- Added: inventory handle arguments to Inventory class functions
- Added: networking identity references into new Networking class functions
- Added: equipped_profile_items_changed, equipped_profile_items callbacks
- Added: additional data types to signals
- Changed: many functions in Networking Sockets and Networking Types classes
- Changed: newtorking identity maps
- Changed: various argument names to fall in line with module version
- Changed: renamed some signals
- Fixed: serverInit, advertiseGame functions
- Fixed: getSessionID using wrong type in Remote Play class
- Fixed: various incorrect types in Screenshots class
- Fixed: wrong int type for inventory update handle
- Fixed: not casting app ID for addFavoriteGame
- Fixed: wrong int type for server ID in getLobbyGameServer
- Removed: requestAllProofOfPurchaseKeys, requestAppProofOfPurchaseKey functions and related callbacks
- Removed: leading dash from signal / callback function names

Known Issues
----------
- The GDNative version does not allow for default arguments in functions, thus some functions may have odd behaviors.  If you are using this version of GodotSteam you are required to pass any argument that has a default in the module version. Also, there are no enums in the GDNative version due to how it is structured.

Quick How-To
----------
Obtain the plugin through one of two ways:
- Visit the Godot Asset Library either through the website or in the editor and search for GodotSteam.
- Download this repo and unzip it into the base of your game project.

Go into **Project > Settings > Plugins** and activate the plugin.

Tinker with Steamworks!

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Patreon](https://patreon.com/coaguco) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
