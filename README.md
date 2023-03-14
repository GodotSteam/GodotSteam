# GodotSteam for GDNative
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)
- [Server](https://github.com/Gramps/GodotSteam/tree/server)
- [GDNative](https://github.com/Gramps/GodotSteam/tree/gdnative)
- [GDExtension](https://github.com/Gramps/GodotSteam/tree/gdextension)

Documentation
----------
[Documentation is available here](https://gramps.github.io/GodotSteam/) and [is mirrored on and exported from CoaguCo's site](https://coaguco.com/godotsteam).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
----------
You can [download pre-compiled versions _(currently v3.6)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

**Version 3.6 Changes**
- Changed: brought the plug-in version up to speed with the module version

Known Issues
----------
- The GDNative version does not allow for default arguments in functions, thus some functions may have odd behaviors.  If you are using this version of GodotSteam you are required to pass any argument that has a default in the module version. Also, there are no enums in the GDNative version due to how it is structured.
- Getting lobby lists returns a non-array response despite being coded for an array.

Quick How-To
----------
Obtain the plugin through one of two ways:
- Visit the Godot Asset Library either through the website or in the editor and search for GodotSteam.
- Download this repo and unzip it into the base of your game project.

Go into **Project > Settings > Plugins** and activate the plugin.

Tinker with Steamworks!

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Github Sponsors](https://github.com/sponsors/Gramps) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
