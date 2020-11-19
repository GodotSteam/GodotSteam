extends Node
# Stat and achievement variables
var ACHIEVEMENTS: Dictionary = {
	"ACH_WIN_ONE_GAME":false,
	"ACH_WIN_100_GAMES":false,
	"ACH_TRAVEL_FAR_ACCUM":false,
	"ACH_TRAVEL_FAR_SINGLE":false
	}
var STATISTICS: Dictionary = {
	"NumGames":0,
	"NumWins":0,
	"NumLosses":0,
	"FeetTraveled":0.0,
	"MaxFeetTraveled":0.0
	}

# Set up some signals
signal back_to_main


func _ready() -> void:
	# Connect some signals
	Steam.connect("current_stats_received", self, "_steam_Stats_Ready", ["current"])
	Steam.connect("user_stats_received", self, "_steam_Stats_Ready", ["user"])
	
	# Add options to the drop-downs
	for ACHIEVEMENT in ACHIEVEMENTS.keys():
		$Selections/Grid/Achievements.add_item(ACHIEVEMENT)
	for STATISTIC in STATISTICS.keys():
		$Selections/Grid/Statistics/Select.add_item(STATISTIC)


#################################################
# EXTRA FUNCTIONS FOR THE EXAMPLE
#################################################
# Show our achievement list
func _update_Achievement_List() -> void:
	# Loop through keys and set the display
	for A in ACHIEVEMENTS.keys():
		match A:
			"ACH_WIN_ONE_GAME": $Readout/Achievements/List/Grid/Value1.set_text(str(ACHIEVEMENTS[A]))
			"ACH_WIN_100_GAMES": $Readout/Achievements/List/Grid/Value2.set_text(str(ACHIEVEMENTS[A]))
			"ACH_TRAVEL_FAR_ACCUM": $Readout/Achievements/List/Grid/Value3.set_text(str(ACHIEVEMENTS[A]))
			"ACH_TRAVEL_FAR_SINGLE": $Readout/Achievements/List/Grid/Value4.set_text(str(ACHIEVEMENTS[A]))

# Show our statistics list
func _update_Stat_List() -> void:
	# Loop through keys and set the display
	for S in STATISTICS.keys():
		match S:
			"NumGames": $Readout/Statistics/List/Grid/Value1.set_text(str(STATISTICS[S]))
			"NumWins": $Readout/Statistics/List/Grid/Value2.set_text(str(STATISTICS[S]))
			"NumLosses": $Readout/Statistics/List/Grid/Value3.set_text(str(STATISTICS[S]))
			"FeetTraveled": $Readout/Statistics/List/Grid/Value4.set_text(str(STATISTICS[S]))
			"MaxFeetTraveled": $Readout/Statistics/List/Grid/Value5.set_text(str(STATISTICS[S]))


#################################################
# STEAM STATISTIC RETRIEVAL
#################################################
# Request the current, local user's achievements and stats
# This will fire by default once Steamworks is initialized
func _on_requestCurrentStats_pressed() -> void:
	Steam.requestCurrentStats()


# Request the given user's achievements and stats
func _on_requestUserStats_pressed() -> void:
	Steam.requestUserStats(global.STEAM_ID)


# Handle callback from requesting user data
func _steam_Stats_Ready(game: int, result: bool, user: int, source: String) -> void:
	# Minor debug information
	$Output.append_bbcode("[STEAM] Stats pulled from: "+str(source)+"\n")
	$Output.append_bbcode("[STEAM] This game's ID: "+str(game)+"\n")
	$Output.append_bbcode("[STEAM] Call result: "+str(result)+"\n")
	$Output.append_bbcode("[STEAM] This user's Steam ID: "+str(user)+"\n")

	# Get achievements and pass them to variables
	_get_Achievement("ACH_WIN_ONE_GAME")
	_get_Achievement("ACH_WIN_100_GAMES")
	_get_Achievement("ACH_TRAVEL_FAR_ACCUM")
	_get_Achievement("ACH_TRAVEL_FAR_SINGLE")

	# Update our achievement list to see
	_update_Achievement_List()

	# Get statistics (int) and pass them to variables
	STATISTICS["NumGames"] = Steam.getStatInt("NumGames")
	STATISTICS["NumWins"] = Steam.getStatInt("NumWins")
	STATISTICS["NumLosses"] = Steam.getStatInt("NumLosses")
	STATISTICS["FeetTraveled"] = Steam.getStatFloat("FeetTraveled")
	STATISTICS["MaxFeetTraveled"] = Steam.getStatFloat("MaxFeetTraveled")

	# Update our stat list to see
	_update_Stat_List()


##################################################
# STATISTIC FUNCTIONS
##################################################
# Setting a statistic
func _on_setStats_pressed():
	# Acquire the statistic name from the drop-down
	# Awful way to do this, but only necessary for the example (NEVER DO THIS)
	var THIS_ID: int = $Selections/Grid/Statistics/Select.get_selected_id()
	var THIS_STAT: String = $Selections/Grid/Statistics/Select.get_item_text(THIS_ID)

	# Acquire the new value
	var THIS_VALUE: String = $Selections/Grid/Statistics/Amount.get_text()

	# If this statistic is 1-3 then it is an INT
	var IS_SET: bool = false
	if THIS_ID <= 3:
		IS_SET = Steam.setStatInt(THIS_STAT, int(THIS_VALUE))
	# Else this is a float-based statistic
	else:
		IS_SET = Steam.setStatFloat(THIS_STAT, float(THIS_VALUE))
	$Output.append_bbcode("[STEAM] Statistic for "+str(THIS_STAT)+" stored: "+str(IS_SET)+"\n")
	
	# The stats must be pushed to Steam to register
	var STORE_STATS: bool = Steam.storeStats()
	$Output.append_bbcode("[STEAM] Stats and achievements stored correctly: "+str(STORE_STATS)+"\n")

##################################################
# ACHIEVEMENT FUNCTIONS
##################################################
# Fire a Steam achievement
# Must contain the same name as what is listed in your Steamworks back-end
func _on_setAchievement_pressed() -> void:
	# Acquire the achievement name from the drop-down
	# Awful way to do this, but only necessary for the example (NEVER DO THIS)
	var THIS_ID: int = $Selections/Grid/Achievements.get_selected_id()
	var THIS_ACHIEVE: String = $Selections/Grid/Achievements.get_item_text(THIS_ID)

	# Set the achievement value locally
	ACHIEVEMENTS[THIS_ACHIEVE] = true

	# Pass the value to Steam
	var SET_ACHIEVE: bool = Steam.setAchievement(THIS_ACHIEVE)
	$Output.append_bbcode("[STEAM] Achievement "+str(THIS_ACHIEVE)+" set correctly: "+str(SET_ACHIEVE)+"\n")

	# Now fire it so it appears to the player
	var STORE_STATS: bool = Steam.storeStats()
	$Output.append_bbcode("[STEAM] Stats and achievements stored correctly: "+str(STORE_STATS)+"\n")

	# Update our list to see
	_update_Achievement_List()


# Process achievements
func _get_Achievement(achievement_name: String) -> void:
	var ACHIEVE: Dictionary = Steam.getAchievement(achievement_name)
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


func _on_getAchievementIcon_pressed() -> void:
	# Acquire the achievement name from the drop-down
	# Awful way to do this, but only necessary for the example (NEVER DO THIS)
	var THIS_ID: int = $Selections/Grid/Achievements.get_selected_id()
	var THIS_ACHIEVE: String = $Selections/Grid/Achievements.get_item_text(THIS_ID)

	# Set up some icon variables
	var SIZE: int = 32
	
	# Get the image's handle
	var HANDLE: int = Steam.getAchievementIcon(THIS_ACHIEVE)

	# Get the image data
	var BUFFER: Dictionary = Steam.getImageRGBA(HANDLE)
	
	#Create the image and texture for loading
	var ICON: Image = Image.new()
	var ICON_TEXTURE: ImageTexture = ImageTexture.new()
	ICON.create(SIZE, SIZE, false, Image.FORMAT_RGBAF)
	
	# Lock and draw the image
	ICON.lock()
	for y in range(0, SIZE):
		for x in range(0, SIZE):
			var pixel: int = 4 * (x + y * SIZE)
			var r: float = float(BUFFER['buffer'][pixel]) / 255
			var g: float = float(BUFFER['buffer'][pixel+1]) / 255
			var b: float = float(BUFFER['buffer'][pixel+2]) / 255
			var a: float = float(BUFFER['buffer'][pixel+3]) / 255
			ICON.set_pixel(x, y, Color(r, g, b, a))
	ICON.unlock()
	
	# Apply it to the texture
	ICON_TEXTURE.create_from_image(ICON)
	
	# Display it
	$Readout/Achievements/List/Icon/Icon.set_texture(ICON_TEXTURE)


#################################################
# STATISTICS RESETTING
#################################################
# This will reset all statistics the user has on Steam
# Setting the variable to true will also reset their achievements
func _on_resetAllStats_pressed() -> void:
	var ACHIEVEMENTS_TOO: bool = true
	var IS_RESET: bool = Steam.resetAllStats(ACHIEVEMENTS_TOO)
	$Output.append_bbcode("[STEAM] Statistics and achievements reset: "+str(IS_RESET)+"\n")
	
	# Make sure to request the updated stats and achievements
	_on_requestUserStats_pressed()


#################################################
# BACK BUTTON
#################################################
# Emit a signal to the main node letting it know the user is done
func _on_Back_pressed() -> void:
	emit_signal("back_to_main")
