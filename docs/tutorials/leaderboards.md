# Tutorials - Leaderboards

This tutorial will cover setting up leaderboards for your games. [You can check out the example project on GitHub to see a somewhat working version.](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project){ target="_blank" }

---

## Set Up

First, let's get our signals set up.

=== "Godot 2.x, 3.x"
	````
	Steam.connect("leaderboard_find_result", self, "_leaderboard_Find_Result")
	Steam.connect("leaderboard_score_uploaded", self, "_leaderboard_Score_Uploaded")
	Steam.connect("leaderboard_scores_downloaded", self, "_leaderboard_Scores_Downloaded")
	````
=== "Godot 4.x"
	````
	Steam.leaderboard_find_result.connect(_leaderboard_Find_Result)
	Steam.leaderboard_score_uploaded.connect(_leaderboard_Score_Uploaded)
	Steam.leaderboard_scores_downloaded.connect(_leaderboard_Scores_Downloaded)
	````

We'll go over each signal and related function in order. First, you'll need to pass your leaderboard's Steamworks back-end name to the ***findLeaderboard()*** function like so:

````
Steam.findLeaderboard( your_leaderboard_name )
````

Once Steam finds your leaderboard it will pass back the handle to the **leaderboard_find_result** callback. The ***_leaderboard_Find_Result()*** function that it is connected to it should look something like this:

````
func _leaderboard_Find_Result(handle: int, found: int) -> void:
	if found == 1:
		LEADERBOARD_HANDLE = handle
		print("Leaderboard handle found: "+str(LEADERBOARD_HANDLE))
	else:
	print("No handle was found")
````

Once you have this handle you can use all the additional functions. **Please note** you do not need to save the leaderboard handle since it is stored internally. However, you will only be able to work with one leaderboard at a time unless you store them locally in a variable. I would keep a dictionary of handles locally like:

````
var LEADERBOARD_HANDLES: Dictionary = {
	"top_score": handle1,
	"most_kills": handle2,
	"most_games": handlde3
}
````

This way you can call whatever handle you need when updating leaderboards quickly. Otherwise you have to query each leaderboard again with ***findLeaderboard()*** then wait for the callback then upload the new score. If you aren't updating leaderboards frequently or that many of them, then using the internally stored handle will probably work fine.

---

## Uploading Scores

Before we can download scores, we need to upload them. The function itself is pretty simple:

````
Steam.uploadLeaderboardScore( score, keep_best, details, handle )
````

The first argument is, obviously, the score. The second is if you want the score to update regardless of whether it is better than the current score for the user. The third are details which must be integers; they essentially can be anything but [here is what Valve says about it.](https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore){ target="_blank" }  The fourth is the leaderboard handle we are updating. You do not have to pass the handle though, if you want to use the internally stored one.


Once you pass a score to Steam, you should receive a callback from **leaderboard_score_uploaded**. This will trigger our ***_leaderboard_Score_Uploaded()*** function:

````
func _leaderboard_Score_Uploaded(success: int, this_handle: int, this_score: Dictionary) -> void:
	if success == 1:
		print("Successfully uploaded scores!")
		# Add additional logic to use other variables passed back
	else:
		print("Failed to upload scores!")
````		

For the most part you are just looking for a success of 1 to tell that it worked. However, you may with to use the additional variables passed back by the signal for logic in your game. They are contained in the dictionary called **this_score** which contains these keys:

- **score:** the new score as it stands
- **score_changed:** if the score was changed (0 if false, 1 if true)
- **new_rank:** the new global rank of this player
- **prev_rank:** the previous rank of this player
	
---

## Downloading Scores

Naturally you'll want to display leaderboard scores to the player. But before we pull any leaderboard entries, we need to set the maximum amount of details each one contains by setting the ***setLeaderboardDetailsMax()*** function up:

````
var DETAILS_MAX: int = Steam.setLeaderboardDetailsMax( value )
print("Max details: "+str(DETAILS_MAX)
````

By default the value is set to 0 but you will want to change it to match the number of details you upload with scores from the previous section. If you do not save any details with the scores you can safely ignore this part and move on to just requesting leaderboard entries.

In most cases you'll want to use ***downloadLeaderboardEntries()***, but you can also use ***downloadLeaderboardEntriesForUsers()*** by passing an array of users' Steam IDs to it. Both will respond with the same callback, but ***downloadLeaderboardEntriesForUsers()*** does not allow as much control over what you can request:

````
Steam.downloadLeaderboardEntries( 1, 10, 0, leaderboard_handle )

Steam.downloadLeaderboardEntriesForUsers( user_array, leaderboard_handle )
````

Just like uploading, downloading scores does not require a leaderboard handle to be included if you are using the internally stored one. You will noticed, as I mentioned above, that ***downloadLeaderboardEntriesForUsers()*** only takes an array of users' Steam IDs as it's other argument whereas ***downloadLeaderboardEntries()*** has quite a few others; which we will cover right now.

The first argument the index of entries you are starting with; this is usually 1 for your first request. The second argument is the last index to retrieve; there isn't a listed cap to this number but make it something you can display easily like 10 or so. The third argument is the type of data request; [you can read more details about it in the SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#ELeaderboardDataRequest){ target="_blank" }. For a quick overview:

- 0 - Used for a sequential range by leaderboard rank.
- 1 - Used to get entries relative to the user's entry. You may want to use negatives for the start to get entries before the user's.
- 2 - Used to get all entries for friends for the current user. Start and end arguments are ignored.
- 3 - Used internally by Steam, **do not use this**.

After you request leaderboard entries, you should receive a **leaderboard_scores_downloaded** callback which will trigger our ***_leaderboard_Scores_Downloaded()*** function. That function should look similar to this:

````
func _leaderboard_Scores_Downloaded(message: string, leaderboard_handle: int, result: Array) -> void:
	print("Scores downloaded message: "+str(message)

	# Save this for later leaderboard interactions, if you want
	var LEADERBOARD_HANDLE: int = leaderboard_handle

	# Add logic to display results
	for R in result:
		# Use each entry that is returned
````

The message is just a basic message to inform you to the status the download; whether successful or not and why. The second piece sent back are the results as an array. Each entry in the array is actually a dictionary like so:

- **score:** this user's score
- **steamID:** this user's Steam ID; you can use this to get their avatar, name, etc.
- **global_rank:** obviously their global ranking for this leaderboard
- **ugc_handle:** handle for any UGC that is attached to this entry
- **details:** any details you stored with this entry for later use

---

I will add more to this tutorial at a later date with other functions but this should cover all the basics for working with leaderboards.

[To see this tutorial in action, check out our GodotSteam Example Project on GitHub.](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project){ target="_blank" } There you can get a full view of the code used which can serve as a starting point for you to branch out from.
