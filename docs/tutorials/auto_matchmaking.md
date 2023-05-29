# Tutorials - Lobby Auto-Matchmaking

A quick, requested tutorial is auto-matchmaking lobbies. While this example does not show how to match specific criteria, it will be noted where you can place such matches. This tutorial is basically an extention of the [lobbies / P2P tutorial.](/tutorials/lobbies_p2p/) Because of that we'll focus only on what's different; please refer to the lobby tutorial for additional information and layouts.

---

## Set Up

First let's set up some variables to fill in later:

````
var STEAM_LOBBY_ID: int = 0
var LOBBY_MEMBERS: Array = []
var MATCHMAKING_PHASE: int = 0
var LOBBY_MAX_PLAYERS: int = 2
````

Carried over from the lobby tutorial is **STEAM_LOBBY_ID**, which obviously houses the lobby's ID, and **LOBBY_MEMBERS**, which will be an array of dictionaries of lobby members and their Steam ID 64's.

New to this tutorial is **MATCHMAKING_PHASE** which keeps track of which iteration of the matchmaking search our code does.  Also new is **LOBBY_MAX_PLAYERS** which is used to check if the lobby has space for our player.

All of the ***_ready()*** function callback connections will be the same.

---

## Finding A Lobby
For our purposes, we will create a button named "Auto Matchmake" and connect a **on_pressed** signal to it called ***_on_Auto_Matchmake_pressed()***.  Here is the function that triggers when that button is pressed:

````
# Start the auto matchmaking process.
func _on_Auto_Matchmake_pressed() -> void:
	# Set the matchmaking process over
	MATCHMAKE_PHASE = 0

	# Start the loop!
	_matchmaking_Loop()
````			

This, obviously, starts the main loop that looks for a matching lobby for your player to join:

````
# Iteration for trying different distances
func _matchmaking_Loop() -> void:
	# If this matchmake_phase is 3 or less, keep going
	if MATCHMAKE_PHASE < 4:
		###
		# Add other filters for things like game modes, etc.
		# Since this is an example, we cannot set game mode or text match features.
		# However you could use addRequestLobbyListStringFilter to look for specific
		# text in lobby metadata to match different criteria.

		###
		# Set the distance filter
		Steam.addRequestLobbyListDistanceFilter(MATCHMAKE_PHASE)

		# Request a list
		Steam.requestLobbyList()

	else:
		print("[STEAM] Failed to automatically match you with a lobby. Please try again.\n")
````

As it notes in the code above, you could have a list of different filters the player can pick from before searching for a lobby. These can be applied to terms that **addRequestLobbyListStringFilter()** looks for ahead of time. Things like game modes, maps, difficulties, etc.

Very important is our **addRequestLobbyListDistanceFilter()** and **MATCHMAKE_PHASE** variable. We are starting with "close" / 0 and ending with "worldwide" 3; hence at 4 it fails to find anything and prompts the user to try again.

This loop function will trigger a callback once it finds some lobbies to check through. Sorting through our matches should look like this:

````
# A lobby list was created, find a possible lobby
func _on_Lobby_Match_List(lobbies: Array) -> void:
	# Set attempting_join to false
	var ATTEMPTING_JOIN: bool = false

	# Show the list 
	for LOBBY in lobbies:
		# Pull lobby data from Steam
		var LOBBY_NAME: String = Steam.getLobbyData(LOBBY, "name")
		var LOBBY_NUMS: int = Steam.getNumLobbyMembers(LOBBY)

		###
		# Add other filters for things like game modes, etc.
		# Since this is an example, we cannot set game mode or text match features.
		# However, much like lobby_name, you can use Steam.getLobbyData to get other
		# preset lobby defining data to append to the next if statement.
		###

		# Attempt to join the first lobby that fits the criteria
		if LOBBY_NUMS < LOBBY_MAX_PLAYERS and not ATTEMPTING_JOIN:
			# Turn on attempting_join
			ATTEMPTING_JOIN = true
			print("Attempting to join lobby...\n")
			Steam.joinLobby(LOBBY)

	# No lobbies that matched were found, go onto the next phase
	if not ATTEMPTING_JOIN:
		# Increment the matchmake_phase
		MATCHMAKE_PHASE += 1
		_matchmaking_Loop()
````

This will go through every lobby returned and, if none of them match, it will iterate the **MATCHMAKE_PHASE** variable and start the loop again but moving one step further in the distance filter.

Much like the previous additional filters, you can sort by other lobby data in the manner the **LOBBY_NAME** data is acquired. These bits of information can be added to a lobby when it is created, making this process easier.

The first lobby that matches your criteria and has space for the user, triggers the **joinLobby()** function to fire and the player should soon join their automatically found lobby.

---

[To see this tutorial in action, check out the example branch on the GodotSteam Github repo.](https://github.com/CoaguCo-Industries/GodotSteam/tree/example){ target="_blank" } There you can get a full view of the code used which can serve as a starting point for you to branch out from.