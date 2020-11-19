extends Node2D
# Set up some signals
signal back_to_main


func _ready():
	# Connect the avatar_loaded signal
	Steam.connect("avatar_loaded", self, "loaded_avatar")


#################################################
# AVATAR FUNCTIONS
#################################################
# Load avatars buttons, pass the size you want and the player's Steam ID
func _on_Small_pressed() -> void:
	Steam.getPlayerAvatar(1, global.STEAM_ID)


func _on_Medium_pressed() -> void:
	Steam.getPlayerAvatar(2, global.STEAM_ID)


func _on_Large_pressed() -> void:
	Steam.getPlayerAvatar(3, global.STEAM_ID)


# Avatar is ready for display
func loaded_avatar(id: int, size: int, buffer: PoolByteArray) -> void:
	print("Avatar for user: "+str(id)+", size: "+str(size))
	
	#Create the image and texture for loading
	var AVATAR: Image = Image.new()
	var AVATAR_TEXTURE: ImageTexture = ImageTexture.new()
	AVATAR.create(size, size, false, Image.FORMAT_RGBAF)
	
	# Lock and draw the image
	AVATAR.lock()
	for y in range(0, size):
		for x in range(0, size):
			var pixel: int = 4 * (x + y * size)
#			print("Pixel: "+str(pixel))
			var r: float = float(buffer[pixel]) / 255
			var g: float = float(buffer[pixel+1]) / 255
			var b: float = float(buffer[pixel+2]) / 255
			var a: float = float(buffer[pixel+3]) / 255
			AVATAR.set_pixel(x, y, Color(r, g, b, a))
	AVATAR.unlock()
	
	# Apply it to the texture
	AVATAR_TEXTURE.create_from_image(AVATAR)
	
	# Display it
	if size == 32:
		$Images/Small/Avatar.set_texture(AVATAR_TEXTURE)
	elif size == 64:
		$Images/Medium/Avatar.set_texture(AVATAR_TEXTURE)
	else:
		$Images/Large/Label.set_text("Large Avatar - 128 x 128 pixels (Retrieved as "+str(size)+" pixels)")
		$Images/Large/Avatar.set_texture(AVATAR_TEXTURE)


#################################################
# BACK BUTTON
#################################################
# Emit a signal to the main node letting it know the user is done
func _on_Back_pressed() -> void:
	emit_signal("back_to_main")
