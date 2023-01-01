# Tutorials - Linux Caveats

There are issues some users might run into on Linux when using Godot and GodotSteam. This tutorial page is here to cover them.

---

## GLES 2 / Mesa / Overlay Crash

There is apparently an issue with games using GLES 2 and systems running Mesa 20.3.5 through 21.2.5 that causes said games to crash when Steam overlay is enabled.  As of now there is no solution to fix this.  However, you can employ the launcher sh file mentioned below to prevent the crash but the overlay will still not work. Other solutions include using GLES 3 instead or a Mesa version outside that small window.

---

## Libsteam_api.so Issues

Sometimes on various Linux distributions you will run into the following error:

````
error while loading shared libraries: libsteam_api.so: cannot open shared object file: No such file or directory
````

As far as I can tell, this has something to do with how Steam was installed on your system and it not installing the libsteam_api.so in the right places.  This issue affects Linux Steam users in general and other games on Steam not using Godot or GodotSteam.  And while this is not a GodotSteam-specific issue, or very common, it is one worth mentioning to save you and your players some headaches.

To resolve this you need to place the libsteam_api.so in the proper places or set the LD_LIBRARY_PATH with a launcher .sh file.  We'll look at each method below.

### Solution 1 - Copy The libsteam_api.so

This method puts the solution squarely in the hands of your affected users.  As Github user **hadber** points out, you can use this command to find out where your editor or game is looking for the library:

````
ldd < bin path for your game >
````

Once you figure out where it is looking you can run the following code from your project's directory (or where ever you have a copy of the libsteam_api.so file), also courtesy of **hadber**:

=== "Manjaro and Ubuntu"
    ````sudo cp libsteam_api.so /usr/lib/````

=== "Fedora 32"
    ````sudo cp libsteam_api.so /lib64/````

Please note that the directories you are copying the libsteam_api.so file to may differ on your installation; make sure to copy it to the right folder. Also make sure to copy the right version of the libsteam_api.so since the naming convention Steam users for them is not denoted by 32 or 64 bits.

### Solution 2 - Create A Launcher .sh File

This solution is preferred as it will work for all users and they won't have to do anything on their end.  As Github user **nuggs** showed, by way of an adapted script from the Godot documentation, you can just place this code into a blank text file:

````
#!/bin/bash

HELP_STRING="-v - Run Godot with the --verbose option."
verbose=""

while getopts hv flag
do
        case "${flag}" in
		h) help=
			echo $HELP_STRING
			exit
		;;
                v) verbose=--verbose;;
        esac
done

export LD_LIBRARY_PATH="$PWD/"
./godot.x11.opt.tools.64 $verbose
````

Obviously, you will want to replace ***godot.x11.opt.tools.64*** with your game's executable and path (if used). Ideally this script will be in the same folder as your game's executable so you only really need to have the executable's name.

You can save this file as (your game).sh or whatever you'd like to name it.  Just make sure the extension is .sh and that you have edited the permissions to make this executable.  I am fairly confident that Steam will make said script executable during installation, but do it yourself just in case.

After all that, list it in [your Steamworks back-end Launch Options](https://partner.steamgames.com/apps/config/){ target="_blank" } to run when the game is launched in Steam:

![Launch Options](/assets/images/steamworks-launch-options.png){ loading=lazy }

Obviously, in the screenshot, this is a Windows example, but fill it in for Linux.  Make sure the operating system is also set to Linux and then save it.  At this point, Steam should use this .sh file to run your game and prevent any possible issues with it being unable to find the libsteam_api.so file.