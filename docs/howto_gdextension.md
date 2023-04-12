# How-To GDExtension

## Please Note

If you are using the GDExtension version of GodotSteam, make sure **not to download or use the module version** in tandem; it will cause a lot of problems.  The two are not compatible with each other.

This also means you cannot use the GodotSteam templates for exporting; you must use the normal Godot templates.

---
## 1a. Downloading

Just like the module version, the easiest way to use GodotSteam is downloading the plug-in and installing it into your project.

- [x] Download the [pre-compiled plug-in from the Release section](https://github.com/Gramps/GodotSteam/releases) and unpack it.
- [x] Alternatively you can download and install the [GDExtension plug-in through Godot Asset Library](https://godotengine.org/asset-library/asset/7469).
- [x] Everything you need should be included.
    - Users on Linux may have issues with the libsteam_api.so, if so then [read our Linux Caveats doc](/tutorials/linux_caveats/).

At this point you can skip all the following steps and check our our tutorials to learn more about integrating Steamworks or just explore the SDK!

---
## 1b. Compile Yourself

For those of you who are comfortable compiling or want to give it a shot, here are some steps to follow.

- [x] Set your system up for [compiling based on Godot's recommendations / required tools.](https://docs.godotengine.org/en/stable/development/compiling/index.html)
- [x] Create a new folder and call it **GDExtension** (or whatever you want).
- [x] Acquire the GodotSteam GDExtension source either by downloading it or cloning the repo:
    - Download this repository then unpack it into the **GDExtension** folder.
    - Alternatively, clone this repository into your **GDExtension** folder:
        - Use ````git clone -b gdextension https://github.com/Gramps/GodotSteam.git````
- [x] Download the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- [x] Download the [Godot cpp](https://github.com/godotengine/godot-cpp), the 4.0 branch.
    - Unpack the **godot_cpp** into the **GDExtension** folder.
    - **Alternatively**, you can just use these commands inside the **GDExtension** folder to clone them:
  ````
    git clone --recursive -b 4.0 https://github.com/godotengine/godot-cpp
  ````
- [x] CD into the **godot-cpp** folder and compile the bindings (make sure your slashes are OS appropriate):
````
  scons platform=<your platform> target=template_release
  scons platform=<your platform> target=template_debug
````

---
## 2. Setting Up the SDK

Move the following from the unzipped Steamworks SDK to the **/godotsteam/sdk** folder:
````
    sdk/public/
    sdk/redistributable_bin/
````

---
## 3. Double-Checking Folder / File Structure

The compiling directory contents should now look like this:
````
  godot-cpp/
  --- bin/*
  --- cmake/*
  --- gdextension/*
  --- gen/*
  --- include/*
  --- misc/*
  --- src/*
  --- test/*
  --- tools/*
  --- binding_generator.py
  --- CMakeLists.txt
  --- LICENSE
  --- Makefile
  --- README.md
  --- SConstruct
  bin/
  godotsteam/
  --- doc_classes/*
  --- sdk/
  --- --- public/steam/*
  --- --- redistributable_bin/*
  --- api.json
  --- godotsteam.h
  --- godotsteam.cpp
  --- register_types.cpp
  --- register_types.h
  godotsteam.gdextension
  SConstruct
  README.md
  LICENSE.txt
````

---
## 4. Compiling Time

Before compiling, create a `bin` directory in the GDExtension folder. This is where the compiled binaries will be placed.

The Visual Studio instructions carried over from GDNative probably do not work anymore so they were removed.  If anyone would like to contribute new ones, please feel free to do so!

=== "Windows PowerShell"
    Just open the powershell or VS command prompt and run:

    ````
    scons platform=windows target=template_release
    scons platform=windows target=template_debug
    ````
=== "Linux"
    Just run terminal from within the **GDExtension** folder and run:

    ````
    scons platform=linux target=template_release
    scons platform=linux target=template_debug
    ````

    You can also replace `platform=linux` with `platform=linuxbsd`.
=== "macOS"
    Run the following from a terminal within the **GDExtension** folder:
    ````
    scons platform=macos target=template_release
    scons platform=macos target=template_debug
    ````

---
## 5. Altogether Now

Copy the **/win64**, **/linuxbsd**, or **/osx** folder from within the **GDExtension/bin/** folder then place it into the **/addons/godotsteam/** folder inside your **game's project folder**. Then you will want to copy the matching Steam API file and put it in with the corresponding platform's folder.

Sound a little confusing? It should look a little something like this:
  
=== "For Windows"
    **Note:** If you compile for Windows 32-bit, get the steam_api.dll file instead.
    ````
    /addons/godotsteam/win64/godotsteam.windows.template_debug.x86_64.dll
    /addons/godotsteam/win64/godotsteam.windows.template_release.x86_64
    /addons/godotsteam/win64/steam_api64.dll
    ````
=== "For Linux"
    **Note:** You will want to rename linuxbsd to x11.
    ````
    /addons/godotsteam/linux/libgodotsteam.linux.template_debug.x86_64.so
    /addons/godotsteam/linux/libgodotsteam.linux.template_release.x86_64.so
    /addons/godotsteam/linux/libsteam_api.so
    ````
=== "For Mac"
    Nothing extra for Mac, just this.
    ````
    /addons/godotsteam/osx/libgodotsteam.macos.template_debug.universal.dylib
    /addons/godotsteam/osx/libgodotsteam.macos.template_release.universal.dylib
    /addons/godotsteam/osx/libsteam_api.dylib
    ````

Copy the **godotsteam.gdextension** file from the base of your compiling folder and place it in the **addons/godotsteam/** folder in your project.

That's it!  Done!

---
## 6. Usage

Now you should be able to call functions from **Steam** like you would normally with the **GodotSteam module**:
````
func _ready():
<<<<<<< HEAD
    Steam.steamInit()
    var isRunning = Steam.isSteamRunning()
    
    if (!isRunning):
        print("Steam is not running.")
        return
        
    print("Steam is running.")
    
    var id = Steam.getSteamID()
    var name = Steam.getFriendPersonaName(id)
    print("Your steam name: " + str(name))
=======
	Steam.steamInit()
	var isRunning = Steam.isSteamRunning()
	
	if (!isRunning):
		print("Steam is not running.")
		return
		
	print("Steam is running.")
	
	var id = Steam.getSteamID()
	var name = Steam.getFriendPersonaName(id)
	print("Your steam name: " + str(name))
>>>>>>> 0cac02ff7588f28c5554f133bae8b5902ae33d10
````

Make sure to create a file called **steam_appid.txt** and place it with your editor or at the root of your game's project folder.  You'll need this to run the game from the editor.

The documentation for GodotSteam modules should apply to GodotSteam GDExtension as they are built from the same code and have all the same functions; generally speaking.

---
## 7. Exporting / Shipping Your Game

For a full explanation of exporting and shipping your game with GodotSteam, [please refer to our Export and Shipping tutorial.](/tutorials/exporting_shipping/)

That being said, you should be able to export your game with the normal Godot templates.  Also, here is a quick rundown of some things to remember. When uploading your game to Steam, you _**must**_ upload your game's executable and **Steam API .dll/.so/.dylb** (steam_api.dll, steam_api64.dll, libsteam_api.dylib, and/or libsteam_api.so).  *Do not* include the steam_appid.txt or any .lib files as they are unnecessary; however, they won't hurt anything.