extends Node

# Steam variables
onready var Steam = preload("res://addons/godotsteam/godotsteam.gdns").new()

# Set your game's Steam app IP
OS.set_environment("SteamAppId", str(480))
OS.set_environment("SteamGameId", str(480))

var IS_OWNED: bool = false
var IS_ONLINE: bool = false
var IS_FREE_WEEKEND: bool = false
var STEAM_ID: int = 0
var STEAM_NAME: String = ""

func _ready() -> void:
	# Initialize Steam
	var INIT: Dictionary = Steam.steamInit(false)
	print("[STEAM] Did Steam initialize?: "+str(INIT))
	if INIT['status'] != 1:
		# If Steam fails to start up, shut down the app
		print("[STEAM] Failed to initialize Steam. "+str(INIT['verbal'])+" Shutting down...")
#		get_tree().quit()

	#Is the user online?
	IS_ONLINE = Steam.loggedOn()

	# Get the user's Stean name and ID
	STEAM_ID = Steam.getSteamID()
	STEAM_NAME = Steam.getPersonaName()

	# Is this app owned or is it a free weekend?
	IS_OWNED = Steam.isSubscribed()
	IS_FREE_WEEKEND = Steam.isSubscribedFromFreeWeekend()


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