# Tutorials - Removing Steam Easily

A lot of folks like to ship on other platforms like Playstation, XBox, Switch, Itch.io, etc.  Removing deeply embedded Steamworks stuff can be a pain and some have opted to keep separate repositories for their game's Steam version.  However, there is an alternative to this: programmatically ignoring the Steamworks bits. Below are some examples shared by users in our Discord.

---
## How We Do It

So the example I'll be using for this tutorial is based on solution #2 submitted by Rutger.  I am actively using this in my current project.

We will create two variable to hold our platform and the Steamworks object then modify the usual ```_initialize_Steam()``` function to look for the Steam singleton before trying to do anything:

```
var THIS_PLATFORM: String = "steam"
var STEAM_API: Object = null

# Initialize Steam
func _initialize_Steam() -> void:
  if Engine.has_singleton("Steam"):
    THIS_PLATFORM = "steam"
    STEAM_API = Engine.get_singleton("Steam")
    var INIT: Dictionary = STEAM_API.steamInit(false)
    logging.debug("[STEAM] Did Steam initialize?: "+str(INIT))
    if INIT['status'] != 1:
      logging.error("[STEAM] Failed to initialize Steam. "+str(INIT['verbal'])+" Disabling all Steamworks functionality...")
      STEAM_API = null
 
    # Is the user online?
    IS_ONLINE = STEAM_API.loggedOn()
    STEAM_ID = STEAM_API.getSteamID()
    IS_OWNED = STEAM_API.isSubscribed()
    IS_FREE_WEEKEND = STEAM_API.isSubscribedFromFreeWeekend()
    STEAM_NAME = STEAM_API.getPersonaName()
 
  else:
    THIS_PLATFORM = "itch"  # Could be anything else really like a console, etc.
    STEAM_ID = 0
    STEAM_NAME = "You"
```

Now we can use the ```STEAM_API``` object to call our Steam functions elsewhere in the game and, without Steamworks present, it won't crash the game due to the missing singleton call.

I also create a helper function to quickly check if anything Steam-wise can be used, like so:

```
func _is_Steam_Enabled() -> bool:
  if THIS_PLATFORM == "steam" and STEAM_API != null:
    return true
  return false
```

Since it is in my global script, it can be called anywhere I need to use Steam functions.  If this function doesn't return true, then I just have my code ignore related Steam portions like so:

```
func _fire_Steam_Achievement(value: int) -> void:
  if not stats.ACHIEVEMENTS[value]:
    stats.ACHIEVEMENTS[value] = true
    
    # Pass variable to Steam and fire it off
    var ACHIEVE: String = "ACHIEVE"+str(value)
    
    if _is_Steam_Enabled():
      var RESPONSE: bool = STEAM_API.setAchievement(ACHIEVE)
      logging.debug("[STEAM] Firing achievement "+str(ACHIEVE)+", success: "+str(RESPONSE))
      
      var STORED: bool = STEAM_API.storeStats()
      logging.debug("[STEAM] Statistics stored: "+str(STORED))
```

So when you need to export for non-Steam platforms, you can just use vanilla Godot templates and you don't have to worry about anything else!  No need for different versions of your game at all.

Below we have the two submitted user suggestions on how to pull this off; the example above, as mnetioned, was based on solution #2 but do also check out solution 1 if that makes more sense for you.

---
## Solution 1: Multiple Files

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
## Solution 2: Check For Singleton

[Rutger from Roost Games (maker of Cat Cafe Manager)](https://catcafemanager.com){ target="_blank" } shared a tidbit about it: "If anyone is wondering how to do that, since I had to find out through the Switch port, I have a **Platform** global as a wrapper for any platform specific stuff, it just does this in the **_ready:**".  His example code is as following:

````
if Engine.has_singleton("Steam"):
	self.platform = "steam"
	self.Steam = Engine.get_singleton("Steam")
````

---

Hopefully these examples give you some ideas on how to remove Steamworks features from your game without having to make a bunch of different builds and repositories.
