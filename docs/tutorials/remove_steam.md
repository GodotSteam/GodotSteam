# Tutorials - Removing Steam Easily

A lot of folks like to ship on other platforms like Playstation, XBox, Switch, Itch.io, etc.  Removing deeply embedded Steamworks stuff can be a pain and some have opted to keep separate repositories for their game's Steam version.  However, there is an alternative to this: programmatically ignoring the Steamworks bits. Below are some examples shared by users in our Discord.

---

Albey shared some scripts for his solution in GDScript and there are three separate files. The **SteamHandler.gd** file which swaps out which version of the game this is:

````
extends Node

var interface: SteamIntegrationBlank

func _ready() -> void:
  if OS.has_feature("Steam"):
    interface = load("res://Entities/Autoloads/Steam/SteamIntegration.gd").new()
  else:
    interface = load("res://Entities/Autoloads/Steam/SteamIntegrationBlank.gd").new()

  interface.initialise_steam()
  if interface.status != interface.STATUS_OK:
    get_tree().quit()
````

The **SteamIntegrationBlank.gd** file which handles what happens when Steam is not present:

````
extends Reference
class_name SteamIntegrationBlank

const STATUS_OK = 1
const STATUS_STEAM_NOT_RUNNING = 20
var status := STATUS_OK

func initialise_steam() -> void:
  pass
````

And finally the **SteamIntegration.gd** file for when Steam is present:

````
extends SteamIntegrationBlank

const APP_ID = ***

func initialise_steam() -> void:
  if Steam.restartAppIfNecessary(APP_ID):
    status = STATUS_STEAM_NOT_RUNNING
    return

  var init: Dictionary = Steam.steamInit()
  status = init['status']
````

---

[Rutger from Roost Games (maker of Cat Cafe Manager)](https://catcafemanager.com){ target="_blank" } shared a tidbit about it: "If anyone is wondering how to do that, since I had to find out through the Switch port, I have a **Platform** global as a wrapper for any platform specific stuff, it just does this in the **_ready:**".  His example code is as following:

````
if Engine.has_singleton("Steam"):
	self.platform = "steam"
	self.Steam = Engine.get_singleton("Steam")
````

---

Hopefully these examples give you some ideas on how to remove Steamworks features from your game without having to make a bunch of different builds and repositories.

We will be adding a longer tutorial for this later on too.