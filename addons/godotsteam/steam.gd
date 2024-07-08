extends Node

# Steam variables
onready var Steam = preload("res://addons/godotsteam/godotsteam.gdns").new()

var is_free_weekend: bool = false
var is_owned: bool = false
var is_online: bool = false
var is_on_steam_deck: bool = false
var steam_app_id: int = 480
var steam_id: int = 0
var steam_username: String = ""


# Set your game's Steam app IP
func _init() -> void:
	OS.set_environment("SteamAppId", str(steam_app_id))
	OS.set_environment("SteamGameId", str(steam_app_id))


func _ready() -> void:
	# Initialize Steam
	var initialize_response: Dictionary = Steam.steamInitEx(false)
	print("[STEAM] Did Steam initialize?: %s" % initialize_response)
	if initialize_response['status'] != 0:
		# If Steam fails to start up, shut down the app
		print("[STEAM] Failed to initialize Steam: %s" % initialize_response['verbal'])
#		get_tree().quit()

	# Some example functions to run after initializing.
	# These can be deleted or commented out if not needed.
	#############################################
	#Is the user online?
	is_online = Steam.loggedOn()

	# Get the user's Stean name and ID
	steam_id = Steam.getSteamID()
	steam_username = Steam.getPersonaName()

	# Is this app owned or is it a free weekend?
	is_owned = Steam.isSubscribed()
	is_free_weekend = Steam.isSubscribedFromFreeWeekend()

	# Is the game running on the Steam Deck
	is_on_steam_deck = Steam.isSteamRunningOnSteamDeck()


func _process(_delta: float) -> void:
	# Get callbacks
	Steam.run_callbacks()


###
# You can add more functionality here and just call it through GDScript like so: 
#	Steam._set_Achievements( achievement_name )
#
# The corresponding function in this file would be something like:
#	func _set_Achievement(this_achievement: String) -> void:
#		print("Setting Steam achievement: "+str(this_achievement))
#		var WAS_SET: bool = Steam.setAchievement(this_achievement)
#		print("Steam achievement "+str(this_achievement)+" set: "+str(WAS_SET))
#		Steam.storeStats()
#
# Due to how GDNative works, the functions must either reside here or be called with Steam.Steam. since we are calling this singleton script then the Steamworks class.
###