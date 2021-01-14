extends Control
# Set up some variables
var ENTRY_COUNT: int = 0
var LEADERBOARD_HANDLE: int = 0

# Set up some signals
signal back_to_main


func _ready() -> void:
	# Set up some signals
	Steam.connect("leaderboard_ugc_set", self, "_leaderboard_UGC_Set")
	Steam.connect("leaderboard_find_result", self, "_leaderboard_Find_Result")
	Steam.connect("leaderboard_scores_downloaded", self, "_leaderboard_Scores_Downloaded")


#################################################
# LEADERBOARD FUNCTIONS
#################################################
# Used to attach UGC to a leaderboard score
func _on_AttachLeaderboardUGC_pressed() -> void:
	var UGC_HANDLE: int = 0
	Steam.attachLeaderboardUGC(UGC_HANDLE)
	$Output.append_bbcode("Attaching UGC "+str(UGC_HANDLE)+" to leaderboard.")


# Find a leaderboard with a given name
func _on_FindLeaderboard_pressed() -> void:
	# Pull leaderboard name from the name field
	var LEADERBOARD: String = $Name.get_text()
	$Output.append_bbcode("Finding leaderboard handle for name: "+str(LEADERBOARD)+"\n\n")
	# Find the leaderboard
	Steam.findLeaderboard(LEADERBOARD)


# Find the given leaderboard or, if it doesn not exist, create it
func _on_FindOrCreateLeaderboard_pressed() -> void:
	# Set the name, sort method (1 or 2), and display type (1 - 3)
	var LEADERBOARD: String = $Name.get_text()
	var LEADERBOARD_SORT_METHOD = 1
	var LEADERBOARD_DISPLAY_TYPE = 1
	Steam.findOrCreateLeaderboard(LEADERBOARD, LEADERBOARD_SORT_METHOD, LEADERBOARD_DISPLAY_TYPE)


# A leaderboard result was found
func _leaderboard_Find_Result(handle, found) -> void:
	# The handle was found
	if found == 1:
		# Handle is actually stored internally until it is changed or update, no need to store it locally
		LEADERBOARD_HANDLE = handle
		$Output.append_bbcode("Leaderboard handle: "+str(LEADERBOARD_HANDLE)+" (stored internally)\n\n")
	else:
		$Output.append_bbcode("No handle was found for the given leaderboard\n\n")


# Get the name of the leaderboard associated with the current handle
func _on_GetLeaderboardName_pressed() -> void:
	var LEADERBOARD_NAME = Steam.getLeaderboardName()
	# If no name was returned, might be missing or faulty leaderboard handle
	if LEADERBOARD_NAME.empty():
		$Output.append_bbcode("No leaderboard name found, handle is missing or faulty\n\n")
	else:
		$Output.append_bbcode("Leaderboard name for handle "+str(LEADERBOARD_HANDLE)+": "+str(LEADERBOARD_NAME)+"\n\n")


# Get the sort method of the current handle
func _on_GetLeaderboardSortMethod_pressed() -> void:
	var SORT_METHOD: Dictionary = Steam.getLeaderboardSortMethod()
	$Output.append_bbcode("Leaderboard "+str(LEADERBOARD_HANDLE)+" sort method: "+str(SORT_METHOD['verbal'])+" ["+str(SORT_METHOD['result'])+"]\n\n")


# Get the display type of the current handle
func _on_GetLeaderboardDisplayType_pressed() -> void:
	var DISPLAY_TYPE: Dictionary = Steam.getLeaderboardDisplayType()
	$Output.append_bbcode("Leaderboard "+str(LEADERBOARD_HANDLE)+" display type: "+str(DISPLAY_TYPE['verbal'])+" ["+str(DISPLAY_TYPE['result'])+"]\n\n")


# UGC has been attached to the leaderboard score
func _leaderboard_UGC_Set(handle, result) -> void:
	$Output.append_bbcode("UGC set to handle: "+str(handle)+" ["+str(result)+"]\n\n")


# Request all rows for friends of the current user
func _on_DownloadLeaderboardEntries_pressed() -> void:
	$Output.append_bbcode("Downloading entries for leaderboard "+str(LEADERBOARD_HANDLE)+"...\n\n")
	Steam.downloadLeaderboardEntries(1, 10, 0)


# Leaderboard entries are ready to be retrieved
func _leaderboard_Scores_Downloaded(message: String, result: Array) -> void:
	$Output.append_bbcode("Leaderboard entries for handle "+str(LEADERBOARD_HANDLE)+": "+str(message)+"\nResults are as such: "+str(result)+"\n\n")


# Request all rows for a maximum of 100 users
func _on_DownloadLeaderboardEntriesForUsers_pressed() -> void:
	# Set an array of users, in this case just the current user
	var USER_ARRAY: Array = [global.STEAM_ID]

	# Request them and write to output
	$Output.append_bbcode("Downloading leaderboard entries for handle "+str(LEADERBOARD_HANDLE)+" and for users: "+str(USER_ARRAY)+"...\n\n")
	Steam.downloadLeaderboardEntriesForUsers(USER_ARRAY)


# Get the number of leaderboard entries
func _on_GetLeaderboardEntryCount_pressed() -> void:
	ENTRY_COUNT = Steam.getLeaderboardEntryCount()
	$Output.append_bbcode("Entry count for leaderboard handle "+str(LEADERBOARD_HANDLE)+": "+str(ENTRY_COUNT)+"\n\n")


#################################################
# BACK BUTTON
#################################################
# Emit a signal to the main node letting it know the user is done
func _on_Back_pressed() -> void:
	emit_signal("back_to_main")
