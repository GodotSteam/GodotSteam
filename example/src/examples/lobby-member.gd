extends Control
#################################################
var STEAM_ID: int = 0

signal kick_player


# Is it ready? Do stuff!
func _ready():
	# connect some signals
	Steam.connect("avatar_loaded", self, "_loaded_Avatar")


# Set this player up
func _set_Member(steam_id: int, steam_name: String) -> void:
	# Set the ID and username
	STEAM_ID = steam_id
	$Member/Stuff/Username.set_text(steam_name)
	
	# Get the avatar and show it
	Steam.getPlayerAvatar(2, STEAM_ID)


# Kick this player
# Should only work if you are the host or vote-kicking is enabled
func _on_Kick_pressed():
	emit_signal("kick_player", STEAM_ID)


# View this player's profile on Steam
func _on_View_pressed() -> void:
	Steam.activateGameOverlayToUser("steamid", STEAM_ID)


#################################################
# HELPER FUNCTIONS
#################################################
# Load an avatar
func _loaded_Avatar(id: int, size: int, buffer: PoolByteArray) -> void:
	if id == STEAM_ID:
		print("Loading avatar for user: "+str(id))
		# Create the image and texture for loading
		var AVATAR: Image = Image.new()
		var AVATAR_TEXTURE: ImageTexture = ImageTexture.new()
		AVATAR.create(size, size, false, Image.FORMAT_RGBAF)
	
		# Lock and draw the image
		AVATAR.lock()
		for y in range(0, size):
			for x in range(0, size):
				var pixel: int = 4 * (x + y * size)
				var r: float = float(buffer[pixel]) / 255
				var g: float = float(buffer[pixel+1]) / 255
				var b: float = float(buffer[pixel+2]) / 255
				var a: float = float(buffer[pixel+3]) / 255
				AVATAR.set_pixel(x, y, Color(r, g, b, a))
		AVATAR.unlock()
	
		# Apply it to the texture
		AVATAR_TEXTURE.create_from_image(AVATAR)
	
		# Set it
		$Member/Avatar.set_texture(AVATAR_TEXTURE)
