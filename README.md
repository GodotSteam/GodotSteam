# GodotSteam Example Project
This is a basic example of how GodotSteam can work in your game. It is based on my [tutorials from the documentation](https://gramps.github.io/GodotSteam/).

Current Build
----------
This version covers:
- main.tscn
	- Initializing Steamworks
	- Loading achievements and statistics
		- Now utilizes both current and user statistics
	- Firing achievements
	- Loading avatars
- lobby.tscn
	- Creating a lobby
	- Lobby message chat
	- Getting lobby lists from Steam
	- P2P packet sending / reading
	- Joining a lobby

Updates:
- Many buttons are now renamed to the Steam functions they call.
- More comments have been added to the code.

Quick How-To
----------
- Download this repository and unpack it.
- Load it into the Godot editor that has been compiled with the GodotSteam module.
- Make sure the steam_appid.txt is in the same folder as your Godot editor executable.
- Make sure Steam is running.

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Patreon](https://patreon.com/coaguco) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
