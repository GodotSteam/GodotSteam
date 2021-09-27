extends Node
#################################################
# GODOTSTEAM EXAMPLE PROJECT v2.1

# Set up some global Steam variables
var IS_OWNED: bool = false
var IS_ONLINE: bool = false
var STEAM_ID: int = 0
var STEAM_USERNAME: String


func _ready() -> void:
	# Start Steamworks
	_initialize_Steam()


# Initializing Steamworks
func _initialize_Steam() -> void:
	# Get the initialization dictionary from Steam
	var INIT: Dictionary = Steam.steamInit()
	# If the status isn't one, print out the possible error and quit the program
	if INIT['status'] != 1:
		print("[STEAM] Failed to initialize: "+str(INIT['verbal'])+" Shutting down...")
		get_tree().quit()

	# Acquire information about the user
	IS_ONLINE = Steam.loggedOn()
	STEAM_ID = Steam.getSteamID()
	IS_OWNED = Steam.isSubscribed()
	STEAM_USERNAME = Steam.getPersonaName()

	# Check if account owns the game
	if IS_OWNED == false:
		print("[STEAM] User does not own this game")
		# Uncomment this line to close the game if the user does not own the game
#		get_tree().quit()


# Process all Steamworks callbacks
func _process(_delta: float) -> void:
	Steam.run_callbacks()
