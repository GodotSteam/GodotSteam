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
	Steam.connect("current_stats_received", self, "_steam_Stats_Ready", ["current"])
	Steam.connect("user_stats_received", self, "_steam_Stats_Ready", ["user"])
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
	$Readout/Online.set_text("Steam online?: "+str(ONLINE))
	$Readout/Owns.set_text("Owns the game?: "+str(OWNED))
	$Readout/ID.set_text("Steam ID: "+str(STEAM_ID))

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
	$Readout/Achievements.set_text(LIST)

# Show our statistics list
func _update_Stat_List():
	var LIST = "Statistics:\n"
	for S in STATS.keys():
		LIST += str(S)+": "+str(STATS[S])+"\n"
	# Print it to the label
	$Readout/Stats.set_text(LIST)

#################################################
# STEAM STATISTIC RETRIEVAL
#################################################
# Request the current, local user's achievements and stats
# This will fire by default once Steamworks is initialized
func _on_requestCurrentStats_pressed():
	Steam.requestCurrentStats()

# Request the given user's achievements and stats
func _on_requestUserStats_pressed():
	Steam.requestUserStats(STEAM_ID)

# Handle callback from requesting user data
func _steam_Stats_Ready(game, result, user, source):
	print("Stats pulled from: "+str(source))
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

##################################################
# ACHIEVEMENT FUNCTIONS
##################################################
# Fire a Steam achievement
# Must contain the same name as what is listed in your Steamworks back-end
func _on_setAchievement_pressed(achievement_name):
	# Set the achievement to an in-game variable
	ACHIEVEMENTS[achievement_name] = true

	# Pass the value to Steam
	Steam.setAchievement(achievement_name)
	
	# Now fire it so it appears to the player
	Steam.storeStats()
	
	# Update our list to see
	_update_Achievement_List()

# Process achievements
func _get_Achievement(achievement_name):
	var ACHIEVE = Steam.getAchievement(achievement_name)
	# Achievement exists
	if ACHIEVE['ret']:
		# Achievement is unlocked
		if ACHIEVE['achieved']:
			ACHIEVEMENTS[achievement_name] = true
		# Achievement is locked
		else:
			ACHIEVEMENTS[achievement_name] = false
	# Achievement does not exist
	else:
		ACHIEVEMENTS[achievement_name] = false

func _on_getAchievementIcon_pressed():
	# Set up some icon variables
	var SIZE = 32
	
	# Get the image's handle
	var HANDLE = Steam.getAchievementIcon("ACH_WIN_ONE_GAME")
	
	# Get the image data
	var BUFFER = Steam.getImageRGBA(HANDLE)
	
	#Create the image and texture for loading
	var ICON = Image.new()
	var ICON_TEXTURE = ImageTexture.new()
	ICON.create(SIZE, SIZE, false, Image.FORMAT_RGBAF)
	
	# Lock and draw the image
	ICON.lock()
	for y in range(0, SIZE):
		for x in range(0, SIZE):
			var pixel = 4 * (x + y * SIZE)
			var r = float(BUFFER['buffer'][pixel]) / 255
			var g = float(BUFFER['buffer'][pixel+1]) / 255
			var b = float(BUFFER['buffer'][pixel+2]) / 255
			var a = float(BUFFER['buffer'][pixel+3]) / 255
			ICON.set_pixel(x, y, Color(r, g, b, a))
	ICON.unlock()
	
	# Apply it to the texture
	ICON_TEXTURE.create_from_image(ICON)
	
	# Display it
	get_node("Readout/Achievement Icon/Icon").set_texture(ICON_TEXTURE)

#################################################
# STATISTICS RESETTING
#################################################
# This will reset all statistics the user has on Steam
# Setting the variable to true will also reset their achievements
func _on_resetAllStats_pressed():
	var ACHIEVEMENTS_TOO = true
	Steam.resetAllStats(ACHIEVEMENTS_TOO)

#################################################
# AVATAR FUNCTIONS
#################################################
# Load avatars buttons
func _on_getPlayerAvatar_Small_pressed():
	Steam.getPlayerAvatar(Steam.AVATAR_SMALL)

func _on_getPlayerAvatar_Medium_pressed():
	Steam.getPlayerAvatar(Steam.AVATAR_MEDIUM)

func _on_getPlayerAvatar_Large_pressed():
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
		get_node("Readout/Avatars/Avatar Small").set_texture(AVATAR_TEXTURE)
	elif size == 64:
		get_node("Readout/Avatars/Avatar Medium").set_texture(AVATAR_TEXTURE)
	else:
		get_node("Readout/Avatars/Avatar Large").set_texture(AVATAR_TEXTURE)