# Tutorials - Achievement Icons

This quick tutorial will cover to how get achievement icons from Steam's servers.  It was made because there are some extra steps folks need to use to render the images and it may not be very clear otherwise.

---

## Getting The Handle And Buffer

First you will need to request the icon handle by using the achievement's name you set in the Steamworks back-end:

````
var HANDLE: int = Steam.getAchievementIcon("achievement name")
````

This handle is used to get the image information.  This would be passed to a buffer variable like so:

````
var BUFFER: Dictionary = Steam.getImageRGBA(HANDLE)
````

---

## Creating The Image

This buffer contains the actual image data for our icon. However, as this buffer is simply binary data, we need to load it into an Image so that Godot can use it as a Texture:

=== "Godot 2.x, 3.x"
	````
	var SIZE: int = 64
	var ICON: Image = Image.new()
	var ICON_TEXTURE: ImageTexture = ImageTexture.new()

	````
=== "Godot 4.x"
	````
	var SIZE: int = 64
	````

The size variable can be whatever you want; for our example we will use 64 (64 pixels by 64 pixels) or else Gododt will complain when attempting to create the image and cause a failure.

Now that we've made our Image and Texture objects, we can load the buffer data into the Image. Note the format we use (RGBA8); this is the format the data in the buffer takes when we receive it from Steam, and so in order for Godot to know how to make sense of it, we need to ensure it is correct:

=== "Godot 2.x, 3.x"
	````
	ICON.create_from_data(SIZE, SIZE, false, Image.FORMAT_RGBA8, buffer)
	````
=== "Godot 4.x"
	````
	var ICON: Image = Image.create_from_data(SIZE, SIZE, false, Image.FORMAT_RGBA8, buffer)
	````

Now that all the pixels are set in the right places, we can create the texture:

=== "Godot 2.x, 3.x"
	````
	ICON_TEXTURE.create_from_image(ICON)
	````
=== "Godot 4.x"
	````
	var ICON_TEXTURE: ImageTexture = ImageTexture.create_from_image(ICON)
	````

Finally we can display the icon.  Using the previous ICON_TEXTURE, we can place this icon on our waiting sprite:

````
get_node("Sprite").set_texture(ICON_TEXTURE)
````

---

## All-In-One

Our complete example should look something like this:

=== "Godot 2.x, 3.x"
	````
	# Set up some icon variables
	var SIZE: int = 64

	# Get the image's handle
	var HANDLE: int = Steam.getAchievementIcon("ACH_WIN_ONE_GAME")

	# Get the image data
	var BUFFER: Dictionary = Steam.getImageRGBA(HANDLE)

	#Create the image and texture for loading
	var ICON: Image = Image.new()
	var ICON_TEXTURE: ImageTexture = ImageTexture.new()
	ICON.create_from_data(SIZE, SIZE, false, Image.FORMAT_RGBA8, buffer)

	# Apply it to the texture
	ICON_TEXTURE.create_from_image(ICON)

	# Display it on a sprite node
	get_node("Sprite").set_texture(ICON_TEXTURE)
	````
=== "Godot 4.x"
	````
	# Set up some icon variables
	var SIZE: int = 64

	# Get the image's handle
	var HANDLE: int = Steam.getAchievementIcon("ACH_WIN_ONE_GAME")

	# Get the image data
	var BUFFER: Dictionary = Steam.getImageRGBA(HANDLE)

	#Create the image and texture for loading
	var ICON: Image = Image.create_from_data(SIZE, SIZE, false, Image.FORMAT_RGBA8, buffer)

	# Apply it to the texture
	var ICON_TEXTURE: ImageTexture = ImageTexture.create_from_image(ICON)

	get_node("Sprite").set_texture(ICON_TEXTURE)
	````

---

And that's how you display an achievement icon.

[To see this tutorial in action, check out our GodotSteam 3 Example Project on Github.](https://github.com/CoaguCo-Industries/GodotSteam-3-Example-Project){ target="_blank" } There you can get a full view of the code used which can serve as a starting point for you to branch out from.