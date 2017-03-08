# Godot Steam
Steam API for the Godot game engine. For the Windows, Linux, and Mac platforms. Mac is currently experimental.

How to use
----------
- Download [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- Download [Godot source](https://github.com/godotengine/godot); preferably 2.0.3 or higher. 
- Copy SDK files:

  ```
  sdk/public
  sdk/redistributable_bin
  ```
  To
  ```
  godotsteam/sdk
  ```
  The directory contents should look like this:
  ```
  godotsteam/sdk/public/*
  godotsteam/sdk/redistributable_bin/*
  godotsteam/SCsub
  godotsteam/config.py
  godotsteam/godotsteam.cpp
  godotsteam/godotsteam.h
  godotsteam/register_types.cpp
  godotsteam/register_types.h
  ```
- Move the "godotsteam" directory inside the "modules" directory on the Godot source. Recompile for your platform.

  For Linux, if not using Godot 2.0.3 or higher, you must add ```openssl=no``` when compile because it has problem with libcrypto (class StreamPeerSSL can't use).
  
  For Windows it's highly advised to use Visual Studio. With some extra steps MinGW will work, but most likely with limited functionality.

- Copy shared library (steam_api) from /sdk/redistributable_bin/ folders to Godot binary location, should look like this (depending on target OS):

  Linux 32/64-Bit
  ```
  ./libsteam_api.so
  ./godot_binary
  ```
  Mac 32-Bit
  ```
  ./libsteam_api.dylib
  ./godot_binary
  ```
  Windows 32-Bit
  ```
  ./steam_api.dll
  ./godot_binary.exe
  ```
  Windows 64-Bit
  ```
  ./steam_api64.dll
  ./godot_binary.exe
  ```
- Game must ship with exe, Steam API DLL, SO, or DyLIB, and steam_appid.txt to function. Lack of Steam API DLL, SO, DyLIB (for respective OS), or steam_appid.txt will cause it to fail.

Steamworks API Reference
-------------
I rewrote the project and pulled, what I thought, the most useful and common features of the Steamworks API.  Below they are broken up by group and listed as they can be called in your code.
```
Steam.steamInit()
	Needs to be called in order for this module to work properly. Returns true if success (though the script seems to say it'll return something else).

Steam.isSteamRunning()
	Checks that Steam is indeed running. Returns true if Steam is on and running.
```
Apps
```
Steam.hasOtherApp( AppID )
	Checks if user owns a game or application by checking the App ID. Returns true if user owns game or application given.

Steam.getDLCCount()
	Should check how many DLC user has for running game. Returns number of DLC.

Steam.isDLCInstalled( AppID )
	Checks if given DLC is installed; must pass App ID of DLC. Returns true if DLC is indeed installed.

Steam.isSubscribedApp( AppID )
	Checks if the game is registered to the user. Return true if the game is owned by the player.

Steam.requestAppProofOfPurchaseKey( AppID )
	Should send back key for given App ID. Requires callback and may not function.
```
Friends
```
Steam.getFriendCount()
	Gives the number of friends the user has. Returns an integer.

Steam.getPersonaName()
	Gets the user's Steam name.

Steam.getFriendPersonaName( Steam ID )
	Get a user's current Steam username.

Steam.setGameInfo( key, value )
	Rich presence data is automatically shared beween friends in the same game. Each user has a set of key/value pairs, up to 20 can be set. Two magic keys (status, connect):
	"status" - a UTF-8 string that will show up in the 'view game info' dialog in the Steam friends list
	"connect" - a UTF-8 string that contains the command-line for how a friend can connect to a game
	When setGameInfo() is called with an empty string, it deletes the key.

Steam.clearGameInfo()
	Should clear rich presence data

Steam.inviteFriend ( Steam ID )
	Send invite request to specific Steam friend.

Steam.setPlayedWith ( Steam ID )
	Sets specific friend to "played with" list of users for specific game.

Steam.getRecentPlayers()
	Show list of friends and users you have played with.
```
Users
```
Steam.getSteamID()
	Returns the Steam ID 3 integer.

Steam.loggedOn()
	Checks if the user is logged onto Steam or not. Returns true or false.

Steam.getPlayerSteamLevel()
	Returns the user's Steam level as an integer.

Steam.setServerInfo()
	Resolves IP address and converts it for joining game through Steam.
```
User Stats
```
Steam.clearAchievement( name )
	Will clear the given achievement from the user's account. Should return true if successful, but may return nothing.

Steam.getAchievement( name )
	Check if user has given achievement. Returns true if they indeed have it.

Steam.getStatFloat( name )
	Grabs the value of the statistic given; must use the stat's name in Steamworks back-end. Returns the value of the statistic as a float.

Steam.getStatInt( name )
	Grabs the value of the statistic given; must use the stat's name in Steamworks back-end. Returns the value of the statistic as an integer.

Steam.resetAllStats( 0 or 1 )
	Will wipe out all statistics, and achievements if set to 1, for the game for the user. Returns true if successful.

Steam.requestCurrentStats()
	Should request all current stats for the game running. May need to be used before getStatFloat or getStatInt. Returns true if successful.

Steam.setAchievement( name )
	Sets given achievement in user's account. Must be followed by Steam.storeStats() if you want the pop-up to fire. Returns true if successful.

Steam.setStatFloat( name, value )
	Sets the given float statistic; must use the stat's name in Steamworks back-end. Depending on how you set it there, depends on how the given value affects the record. Returns true if successful.

Steam.setStatInt( name, value )
	Sets the given integer statistic; must use the stat's name in Steamworks back-end. Depending on how you set it there, depends on how the given value affects the record. Returns true if successful.

Steam.storeStats()
	Stores all statistics and achievements to Steam's servers. This call should not be done often but at certain points like when an achievement is earned or saving a game session. Returns true if successful.
```
Utilities
```
Steam.getIPCountry()
	Get the two-letter country code of the user. May be affected by VPN.

Steam.isOverlayEnabled()
	Check if the Steam overlay is currently active. Useful if game should pause when overlay is up. Returns true is overlay is open.

Steam.getSteamUILanguage()
	Gets the native language the Steam UI is using. May be useful for localization? Though this is usually handled in Steamworks back-end.

Steam.getAppID()
	Gets the running game or application's Steam ID. Not particularly useful unless paired with another function as the steam_appid.txt has this value already. Returns the integer.
```

To-Do
-------------
- Mac version needs additional testing (I do not own a Mac).
- Add additional functionality, ideally all of the Steamworks API.
- Rewrite the examples section to fit this new layout.

License
-------------
MIT license
