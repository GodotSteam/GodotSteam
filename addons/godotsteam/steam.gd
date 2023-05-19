extends Node

# Steam variables
onready var Steam = preload("res://addons/godotsteam/godotsteam.gdns").new()
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
