# How-To Modules

Here we provide a, hopefully, thorough explanation of how to set-up, build, and use GodotSteam.  You can, of course, skip all this and just download our pre-compiles or plug-in.

----------
## 1a. Downloading

By far the easiest way to use GodotSteam is to download our pre-compiled editors and templates; especially good for folks who don't want to set up the tools for compiling and just want to get going.

- [x] Download the [pre-compiled editor from the Release section](https://github.com/Gramps/GodotSteam/releases){ target="_blank" } and unpack it.
- [x] Alternatively you can download and install the [GDNative plug-in through Godot Asset Library](https://godotengine.org/asset-library/asset/1045){ target="_blank" }.
- [x] Everything you need should be included.

At this point you can skip all the following steps and check our our tutorials to learn more about integrating Steamworks or just explore the SDK!

Do not that OSX users will need to open the downloaded **GodotEditor.app** and navigate to **/Contents/MacOS** then open the **steam_appid.txt** and add your app ID here; or keep it as 480 if you don't have an app ID yet.

------
## 1b. Compile Yourself

For those of you who are comfortable compiling or want to give it a shot, here are some steps to follow.

- [x] Set your system up for [compiling based on Godot's recommendations / required tools.](https://docs.godotengine.org/en/stable/development/compiling/index.html){ target="_blank" }
- [x] Download and unpack the [Godot source 3.x](https://github.com/godotengine/godot){ target="_blank" }.
- [x] Acquire the GodotSteam source either by downloading it or cloning the repo:
    * [Download this repository](https://github.com/Gramps/GodotSteam){ target="_blank" } then unpack it into a folder called **godotsteam** inside your Godot Engine source code **/modules** folder.
        * You will have to create the **godotsteam** folder and it must be named exactly this.
    * Alternatively, clone this repository into your Godot Engine source code **/modules** folder
        * Use ````git clone https://github.com/Gramps/GodotSteam.git godotsteam````
- [x] Download and unpack the [Steamworks SDK 1.55](https://partner.steamgames.com){ target="_blank" }.
    * This requires a Steam developer account.

------
## 2. Setting Up the SDK

Move the following from the unzipped Steamworks SDK to the **/modules/godotsteam/sdk/** folder:
````
    sdk/public/
    sdk/redistributable_bin/
````

------
## 3. Double-Checking Folder / File Structure

The compiling directory contents should now look like this:
````
    godotsteam/sdk/public/*
    godotsteam/sdk/redistributable_bin/*
    godotsteam/SCsub
    godotsteam/config.py
    godotsteam/godotsteam.cpp
    godotsteam/godotsteam.h
    godotsteam/register_types.cpp
    godotsteam/register_types.h
````

You can also just put the godotsteam directory where ever you like and just apply the ````custom_modules=.../path/to/dir/godotsteam```` flag in SCONS when compiling.  Make sure the ````custom_modules=```` flag points to where the godotsteam folder is located.

------
## 4. Compiling Time

Recompile for your platform:

!!! godotsteam "For editors"
    === "Godot 2.x, 3.x"
        ````scons platform=<your platform> production=yes tools=yes target=release_debug````
    === "Godot 4.x"
        ````scons platform=<your platform> tools=yes target=editor````

!!! godotsteam "For debug templates"
    === "Godot 2.x, 3.x"
        ````scons platform=<your platform> production=yes tools=no target=release_debug````
    === "Godot 4.x"
        ````scons platform=<your platform> tools=no target=template_debug````

!!! godotsteam "For release templates"
    === "Godot 2.x, 3.x"
        ````scons platform=<your platform> production=yes tools=no target=release````
    === "Godot 4.x"
        ````scons platform=<your platform> tools=no target=template_release````

Some things to be aware of:

- If using Ubuntu 16.10 or higher and having issues with PIE security in GCC, use ````LINKFLAGS='-no-pie'```` to get an executable instead of a shared library.
- When creating templates for OSX, [please refer to this post for assistance as the documentation is a bit lacking.](http://steamcommunity.com/app/404790/discussions/0/364042703865087202/){ target="_blank" }

------
## 5. Altogether Now

When recompiling the engine is finished, do the following before running it the first time:

- [x] Copy the shared library (steam_api), for your OS, from sdk/redistributable_bin/ folders to the Godot binary location (by default in the godot source /bin/ file but you can move them to a new folder).
    - These files are called **steam_api.dll, steam_api64.dll, libsteam_api.so, or libsteam_api.dylib**; no other files are needed.
- [x] Create a **steam_appid.txt** file with your game's app ID or 480 in this folder.  Necessary if the editor or game is run _outside_ of Steam.

The finished hierarchy should look like this (if you downloaded the pre-compiles, the editor files go in place of these tools files, which are the same thing):

!!! godotsteam "Linux 32/64-bit"
    ```
    libsteam_api.so
    steam_appid.txt
    ./godot.linux.tools.32 or ./godot.linux.tools.64
    ```
 
!!! godotsteam "MacOS"
    ```
    libsteam_api.dylib
    steam_appid.txt
    ./godot.osx.tools.32 or ./godot.osx.tools.64
    ```
  
!!! godotsteam "Windows"
    === "Windows 32-bit"
        ```
        steam_api.dll
        steam_appid.txt
        ./godot.windows.tools.32.exe
        ```
    === "Windows 64-bit"
        ```
        steam_api64.dll
        steam_appid.txt
        ./godot.windows.tools.64.exe
        ```

Lack of the **Steam API .dll/.so/.dylib**, for your respective OS, or the **steam_appid.txt** will cause the editor or game fail and crash when testing or running the game _outside_ of the Steam client.

- **NOTE:** Some people report putting the Steam API file inside their project folder fixes some issues.
- **NOTE:** For MacOS, the libsteam_api.dylib and steam_appid.txt must be in the Content/MacOS/ folder in your application zip or the game will crash.
- **NOTE:** For Linux, you may have to load the overlay library for Steam overlay to work:
  ```
  export LD_PRELOAD=~/.local/share/Steam/ubuntu12_32/gameoverlayrenderer.so;~/.local/share/Steam/ubuntu12_64/gameoverlayrenderer.so
  
  or 
  
  export LD_PRELOAD=~/.local/share/Steam/ubuntu12_32/gameoverlayrenderer.so;
  export LD_PRELOAD=~/.local/share/Steam/ubuntu12_64/gameoverlayrenderer.so;
  ```
  This can be done in an .sh file that runs these before running your executable.  This issue may not arise for all users and can also just be done by the user in a terminal separately.  You can [read more about it in our Linux Caveats doc](/tutorials/linux_caveats/).

------
## 6. Good To Go

From here you should be able to call various functions of Steamworks. You should be able to look up the functions in Godot itself under the search section. In addition, you should be able to [read the Steamworks API documentation](https://partner.steamgames.com/doc/home){ target="_blank" } to see what all is available and cross-reference with GodotSteam's documentation.

------
## 7. Exporting / Shipping Your Game

For a full explanation of exporting and shipping your game with GodotSteam, [please refer to our Export and Shipping tutorial.](/tutorials/exporting_shipping/)

That being said, here is a quick run-down of things to remember. When uploading your game to Steam, you _**must**_ upload your game's executable and **Steam API .dll/.so/.dylb** (steam_api.dll, steam_api64.dll, libsteam_api.dylib, and/or libsteam_api.so).  *Do not* include the steam_appid.txt or any .lib files as they are unnecessary; however, they won't hurt anything.
