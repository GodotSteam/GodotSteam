# How-To GDNative

## Please Note

If you are using the GDNative version of GodotSteam, make sure **not to download or use the module version** in tandem; it will cause a lot of problems.  The two are not compatible with each other.

This also means you cannot use the GodotSteam templates for exporting; you must use the normal Godot templates.

---
## 1a. Downloading

Just like the module version, the easiest way to use GodotSteam is downloading the plug-in and installing it into your project.

- [x] Download the [pre-compiled editor from the Release section](https://github.com/Gramps/GodotSteam/releases) and unpack it.
- [x] Alternatively you can download and install the [GDNative plug-in through Godot Asset Library](https://godotengine.org/asset-library/asset/1045).
- [x] Everything you need should be included.
    - Users on Linux may have issues with the libsteam_api.so, if so then [read our Linux Caveats doc](/tutorials/linux_caveats/).

At this point you can skip all the following steps and check our our tutorials to learn more about integrating Steamworks or just explore the SDK!

---
## 1b. Compile Yourself

For those of you who are comfortable compiling or want to give it a shot, here are some steps to follow.

- [x] Set your system up for [compiling based on Godot's recommendations / required tools.](https://docs.godotengine.org/en/stable/development/compiling/index.html)
- [x] Create a new folder and call it **GDNative** (or whatever you want).
- [x] Acquire the GodotSteam GDNative source either by downloading it or cloning the repo:
    - Download this repository then unpack it into the **GDNative** folder.
    - Alternatively, clone this repository into your **GDNative** folder:
        - Use ````git clone -b gdnative https://github.com/Gramps/GodotSteam.git````
- [x] Download the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- [x] Download the [Godot cpp](https://github.com/godotengine/godot-cpp), the 3.5 branch.
    - Unpack the **godot_cpp** into the **GDNative** folder.
    - **Alternatively**, you can just use these commands inside the **GDNative** folder to clone them:
  ````
    git clone --recursive -b 3.5 https://github.com/godotengine/godot-cpp
  ````
- [x] CD into the **godot-cpp** folder and compile the bindings (make sure your slashes are OS appropriate):
````
  scons platform=<your platform> generate_bindings=yes target=release
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
  --- godot-headers/*
  --- include/*
  --- misc/*
  --- src/*
  --- test/*
  --- binding_generator.py
  --- CMakeLists.txt
  --- LICENSE
  --- Makefile
  --- README.md
  --- SConstruct
  godotsteam/
  --- sdk/
  --- --- public/steam/*
  --- --- redistributable_bin/*
  --- godotsteam.h
  --- godotsteam.cpp
  --- init.cpp
  SConstruct
  README.md
  LICENSE.txt
````

---
## 4. Compiling Time

=== "Windows users using VS"
    Follow these steps for Visual Studio (big thanks to **willnationsdev**):

    - [x] Create a new Visual Studio project.
    - [x] Name it **GDNative** and make sure it DOES NOT create a directory.
        - Uncheck the box here.
    - [x] Select the **GDNative** folder we were working in.
    - [x] Choose Win32 Desktop Wizard template.
    - [x] Select options for both a dynamic library (.dll) and an empty project.
    - [x] Things should look like this:
      ````
        GDNative -godot-cpp -godot_headers -lib -GDNative --.vs --GDNative.sln --GDNative.vcxproj --GDNative.vcsproj.filters -src
      ````
    - [x] Make sure you have a debug, x64 configuration for the solution.
        - The options are located in the toolbar at the top left.
    - [x] Go to "Project > GDNative Properties" to open the project properties.
    - [x] Ensure you are on the x64 Debug configurations at the top and make these changes:
        - VC++ Directories > Include Directories. Add 'GDNative\godot-cpp\include', 'GDNative\godot-cpp\include\core', and 'GDNative\godot-cpp\godot-headers' to the list.
        - VC++ Directories > Library Directories. Add 'GDNative\godotsteam'.
        - VC++ Directories > Source Directories. Add 'GDNative\godotsteam'.
        - C/C++ > Linker > System. Subsystem = "Console (/SUBSYSTEM:CONSOLE)"
        - C/C++ > Linker > Input. Add "godot-cpp.windows.64.lib" (without quotes) to the Additional Dependencies parameter.
    - [x] Click on Apply and then Save.
    - [x] Now build the solution.
=== "Windows PowerShell"
    Just open the powershell or VS command prompt and run:

    ````
    scons platform=windows production=yes target=release
    ````
=== "Linux and Mac users"
    Just run terminal from within the **GDNative** folder and run:

    ````
    scons platform=<your platform> production=yes target=release
    ````

---
## 5. Altogether Now

Copy the **/win64**, **/linuxbsd**, or **/osx** folder from within the **GDNative/bin/** folder then place it into the **/addons/godotsteam/** folder inside your **game's project folder**. Then you will want to copy the matching Steam API file and put it in with the corresponding platform's folder.

Sound a little confusing? It should look a little something like this:
  
=== "For Windows"
    **Note:** If you compile for Windows 32-bit, get the steam_api.dll file instead.
    ````
    /addons/godotsteam/win64/godotsteam.dll
    /addons/godotsteam/win64/steam_api64.dll
    ````
=== "For Linux"
    **Note:** You will want to rename linuxbsd to x11.
    ````
    /addons/godotsteam/x11/libgodotsteam.so
    /addons/godotsteam/x11/libsteam_api.so
    ````
=== "For Mac"
    Nothing extra for Mac, just this.
    ````
    /addons/godotsteam/osx/libgodotsteam.dylib
    /addons/godotsteam/osx/libsteam_api.dylib
    ````

In a text editor, create a file called **godotsteam.gdnlib** (this may need to be renamed as such if using Windows). Place the following inside this file and save it in the **addons/godotsteam/** folder in your project:
  ````
  [general]
  singleton=false
  load_once=true
  symbol_prefix="godot_"
  reloadable=true

  [entry]
  (read below)

  [dependencies]
  (read below)
  ````

Replace (read below) with the following, based on platform. You can also use all three if you have the right files.
=== "Using Windows"
    ````
    [entry]
    Windows.64="res://addons/godotsteam/win64/godotsteam.dll"

    [dependencies]
    Windows.64=[ "res://addons/godotsteam/win64/steam_api64.dll" ]
    ````
=== "Using Linux"
    ````
    [entry]
    X11.64="res://addons/godotsteam/x11/libgodotsteam.so"

    [dependencies]
    X11.64=[ "res://addons/godotsteam/x11/libsteam_api.so" ]
    ````
=== "Using OSX"
    ````
    [entry]
    OSX.64="res://addons/godotsteam/osx/libgodotsteam.dylib"

    [dependencies]
    OSX.64=[ "res://addons/godotsteam/osx/libsteam_api.dylib" ]
    ````
 
To double-check this worked, in your Godot project, open the **.gdnlib** file in the **Inspector**. It will have the correct data in the GUI editor that pops up in the bottom panel.


In a text editor, create a file called **godotsteam.gdns** (this may need to be renamed as such if using Windows).  Place the following inside this file and save it in the **addons/godotsteam/** folder in your project:
  ````
  [gd_resource type="NativeScript" load_steps=2 format=2]

  [ext_resource path="res://addons/godotsteam/godotsteam.gdnlib" type="GDNativeLibrary" id=1]

  [resource]

  resource_name = "godotsteam"
  class_name = "Steam"
  library = ExtResource( 1 )
  script_class_name = "Steam"
  ````

Create a new scene in your game project and add a Node node with a GDScript as the script. Add the following code:
  ````
  extends Node

  onready var Steam = preload("res://addons/godotsteam/godotsteam.gdns").new()

  func _ready():
    Steam.steamInit()

  ````
Save the scene as **steam.tscn** and place it where ever you want.  Now navigate to **Project > Project Settings** in the editor and click on **Autoload**.  Add your **steam.tscn** as a singleton, with the node name of **Steam**.

Done!

---
## 6. Usage

Now you should be able to call functions from **Steam** like you would normally with the **GodotSteam module**.  They will, however, have to be added to your **steam.gd** script like this:
````
  name = Steam.getPersonaName()
  country = Steam.getIPCountry()
  running = Steam.isSteamRunning()

  func setAchievement(achieve):
    Steam.setAchievement(achieve)
````
These can then be called in any other script (since **steam.tscn** is a singleton) like this:
````
  print(steam.name)
  print(steam.country)

  if steam.running:
    runFunction()

  steam.setAchievement(achieve)
````

Make sure to create a file called **steam_appid.txt** and place it with your editor or at the root of your game's project folder.  You'll need this to run the game from the editor.

The documentation for GodotSteam should apply to GodotSteam GDNative as they are built from the same code and have all the same functions; generally speaking.

---
## 7. Exporting / Shipping Your Game

For a full explanation of exporting and shipping your game with GodotSteam, [please refer to our Export and Shipping tutorial.](/tutorials/exporting_shipping/)

That being said, you should be able to export your game with the normal Godot templates.  Also, here is a quick rundown of some things to remember. When uploading your game to Steam, you _**must**_ upload your game's executable and **Steam API .dll/.so/.dylb** (steam_api.dll, steam_api64.dll, libsteam_api.dylib, and/or libsteam_api.so).  *Do not* include the steam_appid.txt or any .lib files as they are unnecessary; however, they won't hurt anything.