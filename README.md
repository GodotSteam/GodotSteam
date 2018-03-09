# Godot Steam for GDNative
Steam API for the Godot game engine. For the Windows, Linux, and Mac platforms. 

**WARNING**: This GDNative branch is still in active development! While functional, I do not suggest using it for production code for your game; testing only.  Please use one of the GodotSteam modules instead.

- View the GodotSteam module for Godot 2 here: https://github.com/Gramps/GodotSteam/tree/master
- View the GodotSteam module for Godot 3 here: https://github.com/Gramps/GodotSteam/tree/godot3

Documentation
----------
Documentation is available here: https://gramps.github.io/GodotSteam/

Alternately, there is the project's Wiki page here: https://github.com/Gramps/GodotSteam/wiki

Current Build
----------
You can download pre-compiled versions (currently v2.0.0) of this repo here: https://github.com/Gramps/GodotSteam/releases

**Version 2.0.0**
- Added: all current GodotSteam functions and signals
- Added: godotsteam.h file
- Changed: compile process; big thanks to **willnationsdev**
- Changed: various parts of the CPP files
- Removed: pre-compiled TSCN and TRES files

"Quick" How-To
----------
- Create a new folder and call it GDNative (or whatever you want).
- Download this repository.
	- Unpack this repo and place it inside the GDNative folder.
		- _I usually rename it GodotSteam for the sake of ease._
- Download the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
	- Unpack the Steamworks SDK then copy the **public** and **redistributable_bin** to the /include/sdk folder in the unpacked repo folder.
- Download the [Godot 3.0 binary](https://github.com/GodotBuilder/godot-builds/releases) if you do not have it already.
	- _Preferably you use the Godot 3.0.3 binary, at this time of writing._
	- Unpack it and place it GDNative folder.
- Download the [CPP bindings](https://github.com/GodotNativeTools/cpp_bindings) and [Godot headers](https://github.com/GodotNativeTools/godot_headers).
	- Unpack both cpp_bindings and godot_header into the GDNative folder.
	- **Alternatively**, you can just use these commands inside the GDNative folder to clone them:
	````
		git clone https://github.com/GodotNativeTools/cpp_bindings
		git clone https://github.com/GodotNativeTools/godot_headers
	````
- CD into the cpp_bindings folder and compile the bindings (make sure your slashes are OS appropriate):
````
	scons godotbinpath="../<your binary>" headers="../godot_headers" platform=<your platform> generate_bindings=yes
````
- Once this is finished, copy the resulting file from the cpp_bindings/bin folder to the cpp_bindings/lib folder.
- You should end up with a GDNative folder that looks something like this:
````
	Godot_v3.0.2-stable_x11.64 (or whatever your binary is for your platform)
	cpp_bindings/
	--- bin/*
	--- lib/* (this should contain your compiled cpp_bindings)
	--- src/*
	--- binding_generator.py
	--- binding_generator.pyc
	--- godot_api.json
	--- LICENSE
	--- Makefile
	--- README.md
	--- SConstruct
	godot_headers/
	--- arvr/*
	--- gdnative/*
	--- images/*
	--- nativescript/*
	--- pluginscript/*
	--- gdnative_api.json
	--- gdnative_ap_struct.gen.h
	--- LICENSE
	--- README.md
	GodotSteam/
	--- include/
	--- --- sdk/
	--- --- --- public/steam/*
	--- --- --- redistributable_bin/*
	--- --- godotsteam.h
	--- lib/
	--- src/
	--- --- godotsteam.cpp
	--- --- init.cpp
	--- SConstruct
	--- README.md
	--- LICENSE.txt
````
- **Windows users**, follow these steps for Visual Studio (big thanks to **willnationsdev**):
	- Create a new Visual Studio project.
	- Name it GDNative and make sure it DOES NOT create a directory.
		- Uncheck the box here.
	- Select the GDNative folder we were working in.
	- Choose Win32 Desktop Wizard template.
	- Select options for both a dynamic library (.dll) and an empty project.
	- Things should look like this:
	````
		GDNative -cpp_bindings -godot_headers -lib -GDNative --.vs --GDNative.sln --GDNative.vcxproj --GDNative.vcsproj.filters -src
	````
	- Make sure you have a debug, x64 configuration for the solution.
		- The options are located in the toolbar at the top left.
	- Go to "Project > GDNative Properties" to open the project properties.
	- Ensure you are on the x64 Debug configurations at the top and make these changes:
		- VC++ Directories > Include Directories. Add 'GDNative\cpp_bindings\include', 'GDNative\cpp_bindings\include\core', and 'GDNative\godot_headers' to the list.
		- VC++ Directories > Library Directories. Add 'GDNative\GodotSteam\lib'.
		- VC++ Directories > Source Directories. Add 'GDNative\GodotSteam\src'.
		- C/C++ > Linker > System. Subsystem = "Console (/SUBSYSTEM:CONSOLE)"
		- C/C++ > Linker > Input. Add "cpp_bindings.windows.64.lib" (without quotes) to the Additional Dependencies parameter.
	- Click on Apply and then Save.
	- Now build the solution.
	- **Alternatively,** you can also run the following in the VS Command Prompt:
	````
		scons platform=windows
	````
- **Linux and Mac users**, follow these steps:
	- Simply CD into the GodotSteam folder and run:
	````
		scons platform=<your platform>
	````
		- You can enter the bit architecture with **bits** or **b** arguments or not. There is a fallback to automatically find this. However, I recommend you use one.
- Copy the resulting file into your game project folder and matching Steam API file; preferably the root folder.
	- For Windows, you want godotsteam.windows.<your arch>.dll and steam_api.dll or steam_api64.dll.
	- For Linux, you want libgodotsteam.linux.<your arch>.so and libsteam_api.so.
	- For Mac, you want libgodotsteam.osx.<your arch>.dylib and libsteam_api.dylib.
- In a text editor, create a file called godotsteam.gdnlib (this may need to be renamed as such if using Windows).
	- Place the following inside this file and save it in the root of your project (game) folder:
	````
	[general]
	singleton=false
	load_once=true
	symbol_prefix="godot_"
	reloadable=true

	[entry]
	_(read below)_

	[dependencies]
	_(read below)_
	````
	- If using Windows enter either of these pairs (do not put both 32 and 64 lines unless including all files):
	````
		[entry]
		Windows.32="res://godotsteam.windows.32.dll"
		(or)
		Windows.64="res://godotsteam.windows.64.dll"

		[dependencies]
		Windows.32=["res://steam_api.dll"]
		(or)
		Windows.64=["res://steam_api64.dll"]
	````
	- If using Linux enter either of these pairs (do not put both 32 and 64 lines as Steam does not use separate naming conventions):
	````
		[entry]
		X11.32="res://libgodotsteam.linux.32.so"
		(or)
		X11.64="res://libgodotsteam.linux.64.so"

		[dependencies]
		X11.32=["res://libsteam_api.so"]
		(or)
		X11.64=["res://libsteam_api.so"]
	````
	- To double-check this worked, in your Godot project, open the .gdnlib file in the Inspector. It will have the correct data in the GUI editor that pops up in the bottom panel.
- In the Script Editor, cerate a new script.
	- Set the type as NativeScript.
	- Set the Inherits as Reference.
	- Set the class as Steam.
	- Set the filename as godotsteam.gdns.
	- Hit create to save it.
- Open the new godotsteam.gdns file in the Inspector.
	- It will have a null GDNativeLibrary field.
	- Click the arrow and select Load to add the godotsteam.gdnlib file.
	- Click the save icon in the Inspector.
- Create a new scene in your game project.
	- Add a Node node with a built-in GDScript as the script.
	- Add the following code:
	````
	extends Node

	onready var Steam = preload("godotsteam.gdns").new()

	func _ready():
		Steam.steamInit()

	````
- Save the scene as godotsteam.tscn and place it where ever you want.
- Go to "Project > Project Settings" then click on Autoload.
- Add your godotsteam.tscn as a singleton, with the node name of godotsteam.
- Done!

Usage
----------
Now you should be able to call functions from GodotSteam like you would normally with the GodotSteam module.  They will, however, have to be added to your script in the godotsteam.tscn like this:
````
	name = Steam.getPersonaName()
	country = Steam.getIPCountry()
	running = Steam.isSteamRunning()

	func setAchievement(achieve):
		Steam.setAchievement(achieve)
````
These can then be called in any other script (since godotsteam.tscn is a singleton) like this:
````
	print(godosteam.name)
	print(godotsteam.country)

	if godotsteam.running:
		runFunction()

	godotsteam.setAchievement(achieve)
````

The documentation for GodotSteam should apply to GodotSteam GDNative as they are built from the same code and have all the same functions; generally speaking.

License
-------------
MIT license
