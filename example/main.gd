extends Node
# Steam variables
var OWNED = false
var ONLINE = false
var STEAM_ID = 0
var ACHIEVEMENTS = {
	"ACH_WIN_ONE_GAME":false,
	"ACH_WIN_100_GAMES":false,
	"ACH_TRAVEL_FAR_ACCUM":false,
	"ACH_TRAVEL_FAR_SINGLE":false
	}
var STATS = {
	"NumGames":0,
	"NumWins":0,
	"NumLosses":0,
	"FeetTraveled":0.0,
	"MaxFeetTraveled":0.0
	}
var INIT_LOAD = false

func _ready():
	# Initialize Steam
	_initialize_Steam()
	# Connect some signals
	Steam.connect("user_stats_received", self, "_steam_Stats_Ready")
	Steam.connect("avatar_loaded", self, "loaded_avatar")

# Process all Steamworks callbacks
func _process(delta):
	Steam.run_callbacks()

# Initializing Steamworks
func _initialize_Steam():
	var INIT = Steam.steamInit()
	if INIT['status'] != 0:
		print("Failed to initialize Steam. "+str(INIT['verbal'])+" Shutting down...")
		get_tree().quit()

	# Is the user online?
	ONLINE = Steam.loggedOn()
	STEAM_ID = Steam.getSteamID()
	OWNED = Steam.isSubscribed()

	# Check if account owns the game
	if OWNED == false:
		print("User does not own this game")
		# Uncomment this line to close the game if the user does not own the game
#		get_tree().quit()
	$Online.set_text("Steam online?: "+str(ONLINE))
	$Owns.set_text("Owns the game?: "+str(OWNED))
	$ID.set_text("Steam ID: "+str(STEAM_ID))

# Fire a Steam achievement
func _fire_Steam_Achievement(value):
	# Set the achievement to an in-game variable
	ACHIEVEMENTS[value] = true

	# Pass the value to Steam then fire it
	Steam.setAchievement(value)
	Steam.storeStats()
	
	# Update our list to see
	_update_Achievement_List()

# Handle callback from requesting user data
func _steam_Stats_Ready(game, result, user):
	print("This game's ID: "+str(game))
	print("Call result: "+str(result))
	print("This user's Steam ID: "+str(user))

	# Prevent it from loading again
	if INIT_LOAD == false:
		INIT_LOAD = true
	
		# Get achievements and pass them to variables
		_get_Achievement("ACH_WIN_ONE_GAME")
		_get_Achievement("ACH_WIN_100_GAMES")
		_get_Achievement("ACH_TRAVEL_FAR_ACCUM")
		_get_Achievement("ACH_TRAVEL_FAR_SINGLE")

		# Update our achievement list to see
		_update_Achievement_List()

		# Get statistics (int) and pass them to variables
		STATS["NumGames"] = Steam.getStatInt("NumGames")
		STATS["NumWins"] = Steam.getStatInt("NumWins")
		STATS["NumLosses"] = Steam.getStatInt("NumLosses")
		STATS["FeetTraveled"] = Steam.getStatFloat("FeetTraveled")
		STATS["MaxFeetTraveled"] = Steam.getStatFloat("MaxFeetTraveled")

		# Update our stat list to see
		_update_Stat_List()

# Process achievements
func _get_Achievement(value):
	var ACHIEVE = Steam.getAchievement(value)
	# Achievement exists
	if ACHIEVE['ret']:
		# Achievement is unlocked
		if ACHIEVE['achieved']:
			ACHIEVEMENTS[value] = true
		# Achievement is locked
		else:
			ACHIEVEMENTS[value] = false
	# Achievement does not exist
	else:
		ACHIEVEMENTS[value] = false

#################################################
# EXTRA FUNCTIONS FOR THE EXAMPLE
#################################################
# Show our achievement list
func _update_Achievement_List():
	var LIST = "Achievements Earned:\n"
	for A in ACHIEVEMENTS.keys():
		if ACHIEVEMENTS[A]:
			LIST += "Achievement "+str(A)+": true\n"
		else:
			LIST += "Achievement "+str(A)+": false\n"
	# Print it to the label
	$Achievements.set_text(LIST)

# Show our statistics list
func _update_Stat_List():
	var LIST = "Statistics:\n"
	for S in STATS.keys():
		LIST += str(S)+": "+str(STATS[S])+"\n"
	# Print it to the label
	$Stats.set_text(LIST)

func _on_Get_Stats_pressed():
	# Request user's data
	Steam.requestCurrentStats()

# Load avatars buttons
func _on_Load_Small_Avatar_pressed():
	Steam.getPlayerAvatar(Steam.AVATAR_SMALL)

func _on_Load_Medium_Avatar_pressed():
	Steam.getPlayerAvatar(Steam.AVATAR_MEDIUM)

func _on_Load_Large_Avatar_pressed():
	Steam.getPlayerAvatar(Steam.AVATAR_LARGE)

# Avatar is ready for display
func loaded_avatar(id, size, buffer):
	print("Avatar for user: "+str(id))
	
	#Create the image and texture for loading
	var AVATAR = Image.new()
	var AVATAR_TEXTURE = ImageTexture.new()
	AVATAR.create(size, size, false, Image.FORMAT_RGBAF)
	
	# Lock and draw the image
	AVATAR.lock()
	for y in range(0, size):
		for x in range(0, size):
			var pixel = 4 * (x + y * size)
#			print("Pixel: "+str(pixel))
			var r = float(buffer[pixel]) / 255
			var g = float(buffer[pixel+1]) / 255
			var b = float(buffer[pixel+2]) / 255
			var a = float(buffer[pixel+3]) / 255
			AVATAR.set_pixel(x, y, Color(r, g, b, a))
	AVATAR.unlock()
	
	# Apply it to the texture
	AVATAR_TEXTURE.create_from_image(AVATAR)
	
	# Display it
	if size == 32:
		get_node("Avatar Small").set_texture(AVATAR_TEXTURE)
	elif size == 64:
		get_node("Avatar Medium").set_texture(AVATAR_TEXTURE)
	else:
		get_node("Avatar Large").set_texture(AVATAR_TEXTURE)