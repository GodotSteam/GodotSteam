# Tutorials - Linux Caveats

There are issues some users might run into on Linux when using Godot and GodotSteam. This tutorial page is here to cover them.

---

## GLES 2 / Mesa / Overlay Crash

There is apparently an issue with games using GLES 2 and systems running Mesa 20.3.5 through 21.2.5 that causes the Steam overlay to not work or the game to crash. As of now there is no solution to fix this, except to use GLES 3 instead or a Mesa version outside that small window.

---

## Libsteam_api.so Issues

You may run into the following error:

````
error while loading shared libraries: libsteam_api.so: cannot open shared object file: No such file or directory
````

This means you forgot to place `libsteam_api.so` next to your executable.
Remember to include it along with your shipped game, as described in the [Export and Shipping tutorial](exporting_shipping.md).
