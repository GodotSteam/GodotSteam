# Godot Steam for GDNative
Steam API for the Godot game engine. For the Windows, Linux, and Mac platforms. 

**WARNING**: This GDNative branch is still in active development! I do not suggest using it for production code for your game; testing only.  Please use on of the GodotSteam modules instead.

- View the GodotSteam module for Godot 2 here: https://github.com/Gramps/GodotSteam/tree/master
- View the GodotSteam module for Godot 3 here: https://github.com/Gramps/GodotSteam/tree/godot3

Read more about GDNative here: https://godotengine.org/article/dlscript-here
The GDNative Github repo is here: https://github.com/GodotNativeTools

Documentation
----------
Documentation is available here: https://gramps.github.io/GodotSteam/

Current Build
----------
**Version 2.0.0**
- Added: all current GodotSteam functions and signals
- Added: godotsteam.h file
- Changed: various parts of the CPP files
- Removed: pre-compiled TSCN and TRES files

Quick How-To
----------
**Some of these steps may not work correctly as the branch is still under development.**

- Download this repository.
  - Unpack this repo and place it somewhere
- Download the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
  - Unpack the Steamworks SDK then copy the **public** and **redistributable_bin** to /include/sdk.
- Download the [Godot 3.0 binary](https://github.com/GodotBuilder/godot-builds/releases) if you do not have it already.
  - Unpack it and place it outside the GodotSteam GDNative folder but in the same parent folder.
- Download the [CPP bindings](https://github.com/GodotNativeTools/cpp_bindings) and [Godot headers](https://github.com/GodotNativeTools/godot_headers).
  - Unpack both cpp_bindings and godot_header into the GodotSteam GDNative folder.
      - You can follow their instructions and place them outside the folder, but it'll require changing the SConstruct file.
- CD into the cpp_bindings folder and compile the core:
````
    scons platform=<your platform> target=core
````
  - Then compile the bindings:
````
    scons platform=<your platform> target=bindings generate_bindings=yes
````
- You should end up with a project folder that looks something like this:
````
    cpp_bindings
    godot_headers
    include/sdk/public/steam/*
    include/sdk/redistributable_bin/*
    include/godotsteam.h
    lib
    src/godotsteam.cpp
    src/init.cpp
    SConstruct
    README.md
    LICENSE.txt
````
- In the base GodotSteam GDNative folder, run:
````
    scons platform=<your platform>
````
  - You can enter the bit architecture with **bits** or **b** arguments or not. There is a fallback to automatically find this. However, I recommend you use one.
- Create a new scene with a Node.
  - Save the scene as godotsteam.tscn in your project.
- On Inspector tab for this node, select a new script.
  - Create a new resource as NativeScript.
  - Check singlton.
  - In library list as the bottom, find your platform and add the created SO/DLL/DyLib in Dynamic Library.
    - Example: x11 would link libgodotsteam.so, Windows would link godotsteam.dll, Mac would link libgodotsteam.dylib
  - Add the libsteam_api.so, steam_api.dll, or libsteam_api.dylib in Dependencies.
  - Then save this as either godotsteam.tres or godotsteam.gdns or both.
- Back on your node, attach a new script.
  - Set language as NativeScript.
  - Set path: built-in script to On.
  - Set class name as godotsteam.
  - Press create.
- Add the steam.tscn to your project as a singelton in the AutoLoad section under Project Settings
- Done!

Now you should be able to call functions from GodotSteam like you would normally with the GodotSteam module:
````
    steam.steamInit()
    steam.getIPCountry()
    steam.isSteamRunning()
````

The documentation for GodotSteam should apply to GodotSteam GDNative as they are built from the same code. The only exeception is that GDNative is still in early development.

License
-------------
MIT license
