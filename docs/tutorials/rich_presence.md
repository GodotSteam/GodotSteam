# Tutorials - Rich Presence

This short tutorial is all about rich presence for your game; specifically the game's enhanced rich presence.  You have probably seen a friend in-game, in your friend list, that has a secondary text string with some information about the game.  Usually something about the level they are on, a lobby, number of players, etc.  Well, that's what this tutorial is all about.  [You can read more about enhanced rich presence in the Steamworks documentation](https://partner.steamgames.com/doc/features/enhancedrichpresence){ target="_blank" }.

---

## Setting It Up

First you will need to set up your localization file in the Steamworks back-end.  Obviously without this step the rich presence text does not really work as it has nothing to reference.  You will need to set up your text file like this:

````
"lang" {
	"language" {
		"english" {
			"tokens" {
				"#something1"	"Rich presence string"
				"#something2"	"Another string"
				"#something_with_input"	"{something: %input%"
			}
		}
	}
}
````

Make sure you tab separate the strings in the tokens bracket. Also make sure you publish the changes after your file is uploaded _before_ you try to test it.  The changes must be live in Steamworks for it to appear.

You can put various languages in their own nests so these show up for users how have their Steam client language set specifically. The first string in each token is what you'll end up using in your game's code.

---

## Suggested Code

I have a global function, in my **global.gd** that can be called anywhere in my project and handles this.  It is written something like this:

````
func _set_Rich_Presence(token: String) -> void:
# Set the token
var SETTING_PRESENCE = Steam.setRichPresence("steam_display", token)

# Debug it
print("Setting rich presence to "+str(token)+": "+str(SETTING_PRESENCE))
````

And in whatever scene you want to set the token:

````
global._set_Rich_Presence("#something1")
````

And now Steam will set the text in your friend's list to what you have set it as in the token list in the Steamworks back-end. You can also hover over your own profile picture to see the in-game text; for testing purposes.

There are additional tags you can set which are [covered in detail in the SDK documentation here.](https://partner.steamgames.com/doc/api/ISteamFriends#SetRichPresence){ target="_blank" }

---

## Weird Bug In GDNative

A weird bug exists in the **Windows version of GDNative**, both in GodotSteam and Samsfacee's plug-in.  Sometimes the `setRichPresence` function will send the key as the value.  It isn't consistent but happens enough to be noticeable and a pain.

Please note this bug *does not exist* in the pre-compiled module versions nor GDExtension version or the GDNative versions for Linux or OSX.

This behavior definitely seems to be a GDNative problem so we can't really fix it on our and an issue has been submitted to the Godot Github page.

---

And that's a quick run-down on how to set your rich presence for your game!

[To see this tutorial in action, check out our GodotSteam 3 Example Project on Github.](https://github.com/CoaguCo-Industries/GodotSteam-3-Example-Project){ target="_blank" } There you can get a full view of the code used which can serve as a starting point for you to branch out from.
