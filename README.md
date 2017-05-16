# Godot Steam for GDNative
Steam API for the Godot game engine. For the Windows, Linux, and Mac platforms. 

**WARNING**: GDNative is still in active development with Godot 3.0 as is this repo. I do not suggest using it for production code for your game; testing only.

Read more about GDNative here: https://godotengine.org/article/dlscript-here
The GDNative Github repo is here: https://github.com/GodotNativeTools

View the GodotSteam module here: https://github.com/Gramps/GodotSteam/tree/master

Documentation
----------
Documentation is available here: https://gramps.github.io/GodotSteam/

Current Build
----------
You can download pre-compiled versions of this repo here: https://github.com/Gramps/GodotSteam/releases

GodotSteam Version 1.1.0
- Added: getCurrentGameLanguage
- Added: Pre-compiled engines and templates for Linux, Mac, and Windows
- Added: change log to documentation
- Changed: minor things in godotsteam.cpp

GodotSteam GDNative Version 0.1
- Added: most pre-existing GodotSteam code over
- Added: GodotSteam GDNative documentation
- Changed: SConstruct file from GDNative to support architecture recognition or accept bit arguement in SCONS
- Bugs: Windows does not work for compiling yet, waiting for GDNative update

Quick How-To
----------
- Download this repository.
- Download the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- Download the [Godot 3.0 binary](https://github.com/GodotBuilder/godot-builds/releases).
- Download the [GDNative Starter kit](https://github.com/GodotNativeTools/cpp_bindings/releases/).

Follow these next steps in order (kind of important):
- Unpack the starter kit into your project's root folder
- Unpack the Steamworks SDK then copy the **public** and **redistributable_bin** to /include/sdk
- Unpack this repo and place it into your project's root folder
  - Allow the folders to merge
  - Nothing should overwrite unless you are updating to a new version of this repo
- You should end up with a project folder that looks something like this:
````
    include/godot/*
    include/godot_cpp/*
    include/godot_cpp/core/*
    include/sdk/public/steam/*
    include/sdk/redistributable_bin/*
    include/godot.h
    lib/godot.windows.opt.tools.64.lib
    lib/godot_cpp_bindings.lib
    lib/godot_cpp_core.lib
    lib/godotsteam.tres
    lib/libgodot_cpp_bindings.so (for Linux)
    lib/libgodot_cpp_core.so (for Linux)
    lib/godot_cpp_bindings.dll (for Windows)
    lib/godot_cpp_core.dll (for Windows)
    lib/steam.tscn
    src/godotsteam.cpp
    src/init.cpp
    src/SConstruct
````
- Navigate to the src directory in your project and run:
````
    scons platform=<your platform>
````
  - You can enter the bit architecture with **bits** or **b** arguments or not. There is a fallback to automatically find this. However, I recommend you use one.

**OPTIONAL**: if you choose not to use the included steam.tscn file, follow these steps. Otherwise, continue on to the next section.
- Create a new scene with one node as a, well, Node
  - Name it Steam
  - Save the scene as steam.tscn in your project
  - On Inspector tab for this node, select a new script
  - Name it Steam
  - Set language as Native
  - Set path: built-in script to On
  - Press create
- Under newly created script settings in Inspector tab
  - Click next to Library and select New GDNativeLibrary
  - Click the right arrow and it should show a list of platforms
  - Select the platforms you built for
    - You could dump multiple platform files (SO, DLL, Dylib) into your folder to select more than one
  - For each selected platform, link the right file by clicking the folder icon next to them
    - Example: x11 would link libgodotsteam.so, Windows would link godotsteam.dll, Mac would link libgodotsteam.dylib
  - Then press the save icon and save this as godotsteam.tres in your lib folder

The last step after doing the above optional steps or just using the supplied steam.tscn file in /lib.
- Add the steam.tscn to your project as a singelton in the AutoLoad section under Project Settings
- Done!

Now you should be able to call functions from GodotSteam like you would normally with the GodotSteam module:
````
    steam.steamInit()
    steam.getIPCountry()
    steam.isSteamRunning()
````

The documentation for GodotSteam should apply to GodotSteam GDNative as they are built from the same code. The only exeception is that GDNative is still in early development as is Godot 3.0 so things may change over time.  I will try to keep up and modify the code shortly after.

To-Do
-------------
- Additional testing
- Fix signals, leaderboards, and callbacks
- Add more Steamworks functionality

License
-------------
MIT license
