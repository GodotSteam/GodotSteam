# Tutorials - Avatars

You may want to get the current player's avatar, or someone else's, and display it in your game.  This tutorial will walk you through the basics of avatar retrieval and passing it to a sprite so you can use it.

---

## Request The Avatar

Getting the avatar data is incredibly easy. Just use the following line:

````
Steam.getPlayerAvatar(Steam.AVATAR_MEDIUM)
````

You may pass it either **Steam.AVATAR_SMALL**, **Steam.AVATAR_MEDIUM**, or **Steam.AVATAR_LARGE**; their numerical counterparts (1, 2, or 3) will also work.  Additionally you can pass along a Steam ID64 to get a specific user or, if no ID is provided, it will get the current user by default.

---

## Retrieve And Create The Image

To retrieve the avatar data buffer, you'll need to hook the signal for the callback:

````
Steam.connect("avatar_loaded", self, "loaded_avatar")
````

This will return the user's Steam ID, the avatar's size, and the data buffer for rendering the image.  If you have read the [Achievement Icons tutorial](/tutorials/achievement_icons/), this should all look pretty familiar.  Our **loaded_avatar** function will look something like this:

````
func _loaded_Avatar(id: int, size: int, buffer: PoolByteArray) -> void:
	print("Avatar for user: "+str(id))
	print("Size: "+str(size))
	# Create the image and texture for loading
	AVATAR = Image.new()
	var AVATAR_TEXTURE: ImageTexture = ImageTexture.new()
	AVATAR.create_from_data(size, size, false, Image.FORMAT_RGBA8, buffer)
	# Apply it to the texture
	AVATAR_TEXTURE.create_from_image(AVATAR)
	# Set it
	$Sprite.set_texture(AVATAR_TEXTURE)
````

Naturally that texture could be applied elsewhere, depending on where you place this function.

---

## Another Example

A longer, but similar way to get the end result could look like this:

````
# Request one or all sizes
var HANDLE1: int = Steam.getSmallFriendAvatar(steamID)
var HANDLE2: int = Steam.getMediumFriendAvatar(steamID)
var HANDLE3: int = Steam.getLargeFriendAvatar(steamID)

# Using the small avatar handle
var IMAGE: Dictionary = Steam.getImageSize(HANDLE1)
var IMAGE_DATA: Dictionary = Steam.getImageRGBA(HANDLE1)

# Now create the avatar like before
var AVATAR: Image = Image.new()
var AVATAR_TEXTURE: ImageTexture = ImageTexture.new()
AVATAR.create(IMAGE['width'], IMAGE['height'], false, Image.FORMAT_RGBAF)

# Lock the image and fill the pixels from the data buffer
AVATAR.lock()
for y in range(0, size):
	for x in range(0, size):
		var pixel: int = 4 * (x + y * size)
		var r: float = float(IMAGE_DATA['buffer'][pixel]) / 255
		var g: float = float(IMAGE_DATA['buffer'][pixel+1]) / 255
		var b: float = float(IMAGE_DATA['buffer'][pixel+2]) / 255
		var a: float = float(IMAGE_DATA['buffer'][pixel+3]) / 255
		AVATAR.set_pixel(x, y, Color(r, g, b, a))
AVATAR.unlock()

# Now apply the texture
AVATAR_TEXTURE.create_from_image(AVATAR)

# For our purposes, set a sprite with the avatar texture
$Sprite.set_texture(AVATAR_TEXTURE)
````

---

That covers the basics of getting player avatars.

[To see this tutorial in action, check out the example branch on the GodotSteam Github repo.](https://github.com/Gramps/GodotSteam/tree/example){ target="_blank" } There you can get a full view of the code used which can serve as a starting point for you to branch out from.