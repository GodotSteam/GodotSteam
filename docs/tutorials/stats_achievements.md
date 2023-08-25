# Tutorials - Stats And Achievements

At some point you may want to save statistics to Steam's database and / or use their achievement system. You will want to [read up about these in Steam's documentation](https://partner.steamgames.com/doc/features/achievements){ target="_blank" } as I won't be covering the basics on how to set it up in the Steamworks back-end.

---

## Preparations

First thing, you'll want to set up your achievements and statistics in the Steamworks back-end. Most importantly, you want to publish these changes live. If not, they will not be exposed to the Steamworks API and you will get errors when trying to retrieve or set them. Once they have been published you can continue on with this tutorial.

---

## Getting Steam Statistics And Achievements

When starting up your game, in most cases, you'll want to pull all related achievements and statistics from Steam's servers for the local user. There are a few ways to handle connecting the callback for statistics / achievements data received from Steam's servers.
Unless you choose to turn it off by passing **false** to the ***steamInit()*** function; it will automatically request the local user's current statistics and achievements. I generally pass false to prevent this.

To retrieve the data from the resulting callback, you need to connect the **current_stats_received**callback to a function like so:

=== "Godot 2.x, 3.x"
	````
	Steam.connect("current_stats_received", self, "_steam_Stats_Ready", [], CONNECT_ONESHOT)
	````
=== "Godot 4.x"
	````
	Steam.current_stats_received.connect(_steam_Stats_Ready)
	````

You'll notice that **CONNECT_ONESHOT** is passed along to prevent this from firing more than once. This is because that signal is sent any time stats are updated or received and will run the ***_steam_Stats_Ready()*** again. In my use-cases this is not desirable, but it may be in yours. If you don't mind the ***_steam_Stats_Ready()*** firing each time, depending on your function's logic, feel free to omit that part like so:

=== "Godot 2.x, 3.x"
	````
	Steam.connect("current_stats_received", self, "_steam_Stats_Ready")
	````
=== "Godot 4.x"
	````
	Steam.current_stats_received.connect(_steam_Stats_Ready)
	````

If you do keep the **CONNECT_ONESHOT**as I do, I suggest calling for Steam stat updates with ***requestUserStats()***and pass with it the user's Steam ID. This function will work with any user: local or remote. You'll also want to connect it's signal in a similar manner:

=== "Godot 2.x, 3.x"
	````
	Steam.connect("user_stats_received", self, "_steam_Stats_Ready")
	````
=== "Godot 4.x"
	````
	Steam.user_stats_received.connect(_steam_Stats_Ready)
	````

It can be connected to the same function as ***requestCurrentStats()*** as they send the same data back. For our example, here is the ***_steam_Stats_Ready()*** function we listed in the connected signals:

````
func _steam_Stats_Ready(game: int, result: int, user: int) -> void:
	print("This game's ID: "+str(game))
	print("Call result: "+str(result))
	print("This user's Steam ID: "+str(user))
````

---

## Working With The Data

In this function you can check if the result is what you expect (ideally it is 1), see if the given stats are for the current player, and check that the game's ID matches. Also you can now pass the achievements and stats to local variables or functions. I will often pass the achievement to a function to parse them correctly as they send back a BOOL for retrieval and a BOOL for earned or not.

````
var ACHIEVEMENTS: Dictionary = {"achieve1":false, "achieve2":false, "achieve3":false}

func _steam_Stats_Ready(game: int, result: int, user: int) -> void:
	print("This game's ID: "+str(game))
	print("Call result: "+str(result))
	print("This user's Steam ID: "+str(user))

	# Get achievements and pass them to variables
	_get_Achievement("acheive1")
	_get_Achievement("acheive2")
	_get_Achievement("acheive3")

	# Get statistics (int) and pass them to variables
	var HIGHSCORE: int = Steam.getStatInt("HighScore")
	var HEALTH: int = Steam.getStatInt("Health")
	var MONEY: int = Steam.getStatInt("Money")

# Process achievements
func _get_Achievement(value: String) -> void:
	var ACHIEVEMENT: Dictionary = Steam.getAchievement(value)
	# Achievement exists
	if ACHIEVEMENT['ret']:
		# Achievement is unlocked
		if ACHIEVEMENT['achieved']:
			ACHIEVEMENTS[value] = true
		# Achievement is locked
		else:
			ACHIEVEMENTS[value] = false
	# Achievement does not exist
	else:
		ACHIEVEMENTS[value] = false
````

---

## Setting Achievements

Setting the achievements and statistics is pretty simple too. We'll start with achievements. You need to tell Steam the achievement is unlocked and then store it so 'pops':

````
Steam.setAchievement("achieve1")
Steam.storeStats()
````

If you don't call ***Steam.storeStats()*** the achievement pop-up won't trigger but the achievement should be recorded. However, you will still have to call ***Steam.storeStats()*** at some point to upload them. I generally make a generic function to house this process then call it when needed:

````
func _fire_Steam_Achievement(value: String) -> void:
	# Set the achievement to an in-game variable
	player.ACHIEVEMENTS[value] = true

	# Pass the value to Steam then fire it
	Steam.setAchievement(value)
	Steam.storeStats()
````

When that last ***Steam.storeStats()*** is called the achievement will "pop" visually for the user and update their Steam client.

---

## Setting Statistics

Statistics follow a pretty similar process; both int and float based ones. Set them like so:

````
Steam.setStatInt("stat1", value)
Steam.setStatFloat("stat2", value)
Steam.storeStats()
````

When that last ***Steam.storeStats()*** is called the stats will update on Steam's servers.

---

That covers the basics of statistics and achievements.

[To see this tutorial in action, check out our GodotSteam Example Project on GitHub.](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project){ target="_blank" } There you can get a full view of the code used which can serve as a starting point for you to branch out from.
