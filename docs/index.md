# GodotSteam Documentation

## Introduction

This is the documentation site for GodotSteam, a Steamworks module for [Godot Engine](https://godotengine.org){ target="_blank" } which includes a fully-functional version for all versions of Godot from 2.x to 4.x as well as a GDNative plug-in. Here you can find tutorials, lists of functions and signals, and even [games currently using GodotSteam](games.md).

Support for the project is provided through the [GodotSteam Github issues page](https://github.com/Gramps/GodotSteam/issues){ target="_blank" } by e-mail, or discussion in our Discord server.

<div class="link-grid" markdown>

[:fontawesome-brands-github: Check It Out On GitHub](https://github.com/Gramps/GodotSteam){ .md-button .md-button--store target="_blank" }
[:fontawesome-solid-paper-plane: Send A Support E-Mail](mailto:godotsteam@coaguco.com){ .md-button .md-button--store target="_blank" }
[:fontawesome-brands-discord: Chat With Us On Discord](https://discord.gg/SJRSq6K){ .md-button .md-button--store target="_blank" }

</div>

---
## The Branches

Roll up your sleeves and build your own versions of GodotSteam.  It does also require the source code for Godot.  More instructions are availale on each branch's readme page.

<div class="branch-grid" markdown>

!!! source "Godot Engine 3.x"
	Fully working module to compile into Godot Engine; versions 3.0 to 3.5.2

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/master){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/master){ .md-button .md-button--primary target="_blank" }

!!! source "GDNative"
	Want to alter the code for the GDNative plug-in or roll your own?

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/gdnative){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/gdnative){ .md-button .md-button--primary target="_blank" }

!!! source "Godot Engine 4.x"
	Fully working module to compile into Godot Engine; versions 4.0 to 4.0.2

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/godot4){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/godot4){ .md-button .md-button--primary target="_blank" }

!!! source "GDExtension"
	The successor of GDNative, fresh for Godot 4; versions 4.0 to 4.0.2

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/gdextension){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/gdextension){ .md-button .md-button--primary target="_blank" }

!!! source "Godot Engine 2.x" 
	Fully working module to compile into Godot Engine; versions 2.0 to 2.1.6

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/godot2){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/godot2){ .md-button .md-button--primary target="_blank" }

!!! server "Godot Steamworks Server"
	A Steamworks-enabled dedicated server with Godot 3.x

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/server){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/server){ .md-button .md-button--primary target="_blank" }

</div>

If you are compiling the module, GDNative, or GDExtension version yourself, there are a few things you'll need to start working regardless of which flavor you decide to try out:

<div class="start-grid" markdown>

!!! steam "Steamworks SDK"
	Preferably version 1.55; though older versions will work too.

	[Get The SDK](https://partner.steamgames.com/){ .md-button .md-button--primary target="_blank" }

!!! source Godot Engine Source Code
	Download the appropriate tag versions for the GodotSteam branch you are using.

	[Get The Source](https://github.com/godotengine/godot){ .md-button .md-button--primary target="_blank" }

</div>

---
## Pre-builds

Just download and go with either pre-compiled module-enable editors / templates or the GDNative plug-in.

<div class="branch-grid" markdown>

!!! compile "Module Pre-Compiles"
	Get integrating Steamworks into your game with pre-compiled editors and templates

	[Get Yours](https://github.com/Gramps/GodotSteam/releases){ .md-button .md-button--primary target="_blank" }

!!! compile "GDNative Plug-in - Godot Asset Library"
	Get integrating Steamworks into your game with the GDNative plug-in via Godot Asset Library

	[Get Yours](https://godotengine.org/asset-library/asset/1045){ .md-button .md-button--primary target="_blank" }

!!! source "GDNative Plug-In - GitHub Pre-compile" 
	Drops right into your game so you can start tinkering; versions 3.4.5 to 3.5.2

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/gdnative-plugin){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/gdnative-plugin){ .md-button .md-button--primary target="_blank" }

!!! compile "GDExtension Plug-in - Godot Asset Library"
	Get integrating Steamworks into your game with the GDExtension plug-in via Godot Asset Library

	[Get Yours](https://godotengine.org/asset-library/asset/1768){ .md-button .md-button--primary target="_blank" }

!!! source "GDExtension Plug-In - GitHub Pre-compile" 
	Drops right into your game so you can start tinkering; versions 4.0 to 4.0.2

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/gdextension-plugin){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/gdextension-plugin){ .md-button .md-button--primary target="_blank" }


</div>

---
## Quick Start

If you are downloading our pre-compiles or using the GDNative plugin, or have finished fresh compiling, you can move on to the how-to sections or just start experimenting.

<div class="start-grid" markdown>

!!! guide "How-To Guides"
	Based on what you want to work with, use one of these how-to guides to continue on.

	[Module How-To](howto_modules){ .md-button .md-button--primary }
	[GDNative How-To](howto_gdnative){ .md-button .md-button--primary }
	
	[GDExtension How-To](howto_gdextension){ .md-button .md-button--primary }

!!! example "GodotSteam Example Project"
	A working example of some GodotSteam features based on current tutorials

	[Get the tar](https://github.com/Gramps/GodotSteam/tarball/example){ .md-button .md-button--primary target="_blank" }
	[Get the zip](https://github.com/Gramps/GodotSteam/zipball/example){ .md-button .md-button--primary target="_blank" }

</div>

Last, but absolutely not least, make sure you are logged into your Steam client if testing your game or any scenes. Otherwise you will experience some pretty brutal crashing.

Have fun!

---
## Have A Game Using GodotSteam?

Finally got your Steam store page up? Whether you are about to release your game, already did, or are just tinkering away at it, you can send me the details and I will post it in [the Games Using GodotSteam section](games.md).

You can use the button below to submit your game to the list. Please remember to include your game's Steam store page URL. Additionally you can include another link like your Twitter account, development website, devlog, or anything relevant to your game or studio.

[List Your Game](mailto:games@godotsteam.com){ .md-button .md-button--primary target="_blank" }