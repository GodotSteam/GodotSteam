# Tutorials - Avatars

You may want to get the current player's avatar, or someone else's, and display it in your game. This tutorial will walk you through the basics of avatar retrieval and passing it to a sprite so you can use it.


The relevant GodotSteam classes and functions for this tutorial are:

  * [Friends class](../classes/friends.md)
    * [getPlayerAvatar](../classes/friends.md#getplayeravatar)

---

## Request the Avatar

The default Steamworks functions to get player avatars requires multiple steps. However, GodotSteam has a unique function which makes getting the avatar data incredibly easy. Just use the following:

````
Steam.getPlayerAvatar()
````

This function has optional parameters. By default it will get medium-sized (64x64) avatar data for the current player. But you may pass it either **Steam.AVATAR_SMALL** (32x32), **Steam.AVATAR_MEDIUM** (64x64), or **Steam.AVATAR_LARGE** (128x128 or larger) to get different sizes; the numerical counterparts to these enums (1, 2, or 3) will also work. Additionally you can pass along a Steam ID64 to get avatar data for a specific user.

---

## Retrieve and Create the Image

To retrieve the avatar data buffer, you'll need to hook the signal for the callback:

=== "Godot 2.x, 3.x"
	````
	Steam.connect("avatar_loaded", self, "loaded_avatar")
	````
=== "Godot 4.x"
	````
	Steam.avatar_loaded.connect(loaded_avatar)
	````

This will return the user's Steam ID, the avatar's size, and the data buffer for rendering the image. If you have read the [Achievement Icons tutorial](achievement_icons.md), this should all look pretty familiar. Our **loaded_avatar** function will look something like this:

=== "Godot 2.x, 3.x"
	````
	func _loaded_avatar(id: int, size: int, buffer: PoolByteArray) -> void:
		print("Avatar for user: " + str(id))
		print("Size: " + str(size))

		# Create the image for loading
		AVATAR = Image.new()
		AVATAR.create_from_data(size, size, false, Image.FORMAT_RGBA8, buffer)
		
		# Optionally resize the image if it is too large
		if size > 128:
			AVATAR.resize(128, 128, Image.INTERPOLATE_LANCZOS)

		# Apply the image to a texture
		var AVATAR_TEXTURE: ImageTexture = ImageTexture.new()
		AVATAR_TEXTURE.create_from_image(AVATAR)

		# Set the texture to a Sprite, TextureRect, etc.
		$Sprite.set_texture(AVATAR_TEXTURE)
	````
=== "Godot 4.x"
	````
	func _loaded_Avatar(id: int, size: int, buffer: PackedByteArray) -> void:
		print("Avatar for user: "+str(id))
		print("Size: "+str(size))

		# Create the image and texture for loading
		var AVATAR: Image = Image.create_from_data(size, size, false, Image.FORMAT_RGBA8, buffer)

		# Optionally resize the image if it is too large
		if size > 128:
			AVATAR.resize(128, 128, Image.INTERPOLATE_LANCZOS)

		# Apply the image to a texture
		var AVATAR_TEXTURE: ImageTexture = ImageTexture.create_from_image(AVATAR)

		# Set the texture to a Sprite, TextureRect, etc.
		$Sprite.set_texture(AVATAR_TEXTURE)
	````

Naturally that texture could be applied elsewhere, depending on where you place this function.

---

That covers the basics of getting player avatars.

To see this tutorial in action, [check out our GodotSteam Example Project on GitHub](https://github.com/CoaguCo-Industries/GodotSteam-Example-Project){ target="_blank" }. There you can get a full view of the code used which can serve as a starting point for you to branch out from.
