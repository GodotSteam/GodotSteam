# GodotSteam Documentation

## Introduction

This is the documentation site for GodotSteam, a Steamworks module for [Godot Engine](https://godotengine.org){ target="_blank" } which includes a fully-functional version for all versions of Godot from 2.x to 4.x as well as a GDNative plug-in. Here you can find tutorials, lists of functions and signals, and even [games currently using GodotSteam](games.md).

Support for the project is provided through the [GodotSteam Github issues page](https://github.com/CoaguCo-Industries/GodotSteam/issues){ target="_blank" } by e-mail, or discussion in our Discord server.

<div class="link-grid" markdown>

[:fontawesome-brands-github: Check It Out On GitHub](https://github.com/CoaguCo-Industries/GodotSteam){ .md-button .md-button--store target="_blank" }
[:fontawesome-solid-paper-plane: Send A Support E-Mail](mailto:godotsteam@coaguco.com){ .md-button .md-button--store target="_blank" }
[:fontawesome-brands-discord: Chat With Us On Discord](https://discord.gg/SJRSq6K){ .md-button .md-button--store target="_blank" }

</div>

---

## Pre-builds

Most people just want to download something and go.  For that, we have a few different options: either pre-compiled module-enabled editors and templates, the GDNative (Godot 3.x) plug-in, or the GDExtension (Godot 4.x) plug-in.

Make sure you **don't mix and match** the pre-compiled editors with the plug-ins or you'll have a really bad time.

<div class="branch-grid" markdown>

!!! compile "Module Pre-Compiles"
	Get integrating Steamworks into your game with pre-compiled editors and templates

	[:fontawesome-brands-github: For All Versions](https://github.com/CoaguCo-Industries/GodotSteam/releases){ .md-button .md-button--primary target="_blank" }

!!! compile "GDNative Plug-in"
	Get integrating Steamworks into your game with the GDNative plug-in for Godot Engine 3.x

	[:simple-godotengine: Godot Asset Library](https://godotengine.org/asset-library/asset/1045){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Github Drop-in TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/gdnative-plugin){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Github Drop-in ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/gdnative-plugin){ .md-button .md-button--primary target="_blank" }

!!! compile "GDExtension Plug-in"
	Get integrating Steamworks into your game with the GDExtension plug-in via Godot Engine 4.x

	[:simple-godotengine: Godot Asset Library](https://godotengine.org/asset-library/asset/1768){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Github Drop-in TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/gdextension-plugin){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Github Drop-in ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/gdextension-plugin){ .md-button .md-button--primary target="_blank" }


</div>

---

## Compiling Yourself

Roll up your sleeves and build your own versions of GodotSteam.  It does also require the source code for Godot.  More instructions are availale on each branch's readme page.

### Godot 4.x Branches

<div class="branch-grid" markdown>


!!! source "Godot Engine 4.x"
	Fully working module to compile into any Godot Engine 4.x versions

	[:fontawesome-solid-code-branch: See The Source](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot4){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/godot4){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/godot4){ .md-button .md-button--primary target="_blank" }

!!! source "GDExtension"
	The successor of GDNative, fresh for any Godot Engine 4.x versions

	[:fontawesome-solid-code-branch: See The Source](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdextension){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/gdextension){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/gdextension){ .md-button .md-button--primary target="_blank" }

</div>

### Godot 3.x Branches

<div class="branch-grid" markdown>

!!! source "Godot Engine 3.x"
	Fully working module to compile into any Godot Engine 3.x versions

	[:fontawesome-solid-code-branch: See The Source](https://github.com/CoaguCo-Industries/GodotSteam/tree/master){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/master){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/master){ .md-button .md-button--primary target="_blank" }

!!! source "GDNative"
	Want to alter the code for the GDNative plug-in or roll your own?

	[:fontawesome-solid-code-branch: See The Source](https://github.com/CoaguCo-Industries/GodotSteam/tree/gdnative){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/gdnative){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/gdnative){ .md-button .md-button--primary target="_blank" }

!!! server "Godot Steamworks Server"
	A Steamworks-enabled dedicated server with any Godot Engine 3.x versions

	[:fontawesome-solid-code-branch: See The Source](https://github.com/CoaguCo-Industries/GodotSteam/tree/server){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/server){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/server){ .md-button .md-button--primary target="_blank" }

</div>

### Godot 2.x Branches

<div class="branch-grid" markdown>

!!! source "Godot Engine 2.x" 
	Fully working module to compile into any Godot Engine 2.x versions

	[:fontawesome-solid-code-branch: See The Source](https://github.com/CoaguCo-Industries/GodotSteam/tree/godot2){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The TAR](https://github.com/CoaguCo-Industries/GodotSteam/tarball/godot2){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Get The ZIP](https://github.com/CoaguCo-Industries/GodotSteam/zipball/godot2){ .md-button .md-button--primary target="_blank" }

</div>

### Requirements

If you are compiling the module, GDNative, or GDExtension version yourself, there are a few things you'll need to start working regardless of which flavor you decide to try out.

<div class="start-grid" markdown>

!!! steam "Steamworks SDK"
	Preferably version 1.55; though older versions will work too.

	[:fontawesome-solid-code-branch: Get The SDK](https://partner.steamgames.com/){ .md-button .md-button--primary target="_blank" }

!!! source Godot Engine Source Code
	Download the appropriate tag versions for the GodotSteam branch you are using.

	[:fontawesome-solid-code-branch: Get The Source](https://github.com/godotengine/godot){ .md-button .md-button--primary target="_blank" }

</div>

---

## Quick Start

If you are downloading our pre-compiles or using the GDNative plugin, or have finished fresh compiling, you can move on to the how-to sections or just start experimenting.

<div class="start-grid" markdown>

!!! guide "How-To Guides"
	Based on what you want to work with, use one of these how-to guides to continue on.

	[:fontawesome-solid-book: Module How-To](howto/modules){ .md-button .md-button--primary }
	[:fontawesome-solid-book: GDNative How-To](howto/gdnative){ .md-button .md-button--primary }
	[:fontawesome-solid-book: GDExtension How-To](howto/gdextension){ .md-button .md-button--primary }
	
	[:fontawesome-solid-book: Server How-To](howto/server){ .md-button .md-button--primary }

!!! example "GodotSteam Example Project"
	A working example of some GodotSteam features based on current tutorials; currently only available for Godot 3.x

	[:fontawesome-solid-code-branch: Get The Source](https://github.com/CoaguCo-Industries/GodotSteam-3-Example-Project/){ .md-button .md-button--primary target="_blank" }
	[:simple-godotengine: Godot Asset Library](https://godotengine.org/asset-library/asset/1956){ .md-button .md-button--primary target="_blank" }
	[:fontawesome-solid-file-zipper: Github Drop-in ZIP](https://github.com/CoaguCo-Industries/GodotSteam-3-Example-Project/zipball/master){ .md-button .md-button--primary target="_blank" }


</div>

Last, but absolutely not least, make sure you are logged into your Steam client if testing your game or any scenes. Otherwise you will experience some pretty brutal crashing.

Have fun!

---

## Have A Game Using GodotSteam?

Finally got your Steam store page up? Whether you are about to release your game, already did, or are just tinkering away at it, you can send me the details and I will post it in [the Games Using GodotSteam section](games.md).

You can use the button below to submit your game to the list. Please remember to include your game's Steam store page URL. Additionally you can include up to four additional links like your Mastodon or Twitter account, Discord invite link, development website, devlog, or anything relevant to your game or studio.

[:fontawesome-solid-gamepad: List Your Game](mailto:games@godotsteam.com){ .md-button .md-button--primary target="_blank" }

---

## Donating And Contributing

Want to help out? The best way is to contribute fixes or additions through pull-requests on Github. We also accept donations through these places:

<div class="link-grid" markdown>

[:fontawesome-brands-github: Github Sponsors](https://github.com/sponsors/Gramps){ .md-button .md-button--primary target="_blank" }
[:simple-kofi: Ko-Fi](https://ko-fi.com/grampsgarcia){ .md-button .md-button--primary target="_blank" }
[:fontawesome-brands-paypal: Paypal](https://www.paypal.me/sithlordkyle){ .md-button .md-button--primary target="_blank" }

</div>