# Remote Play

Functions that provide information about Steam Remote Play sessions, streaming your game content to another computer or to a Steam Link app or hardware.  
  
See [Steam Remote Play](https://partner.steamgames.com/doc/features/remoteplay){ target="_blank" } for more information.

---

## Functions

---

### getSessionClientFormFactor

!!! function "getSessionClientFormFactor( ```uint32``` session_id )"
	Get the form factor of the session client device.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#GetSessionClientFormFactor){ .md-button .md-button--store target="_blank" }

### getSessionClientName

!!! function "getSessionClientName( ```uint64_t``` session_id )"
	Get the name of the session client device.
    
    **Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#GetSessionClientName){ .md-button .md-button--store target="_blank" }

### getSessionClientResolution

!!! function "getSessionClientResolution( ```uint32``` session_id )"
	Get the resolution, in pixels, of the session client device. This is set to 0x0 if the resolution is not available.

	**Returns:** dictionary

	Contains the following keys:
	
	* success (bool)
	* x (int)
	* y (int)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#BGetSessionClientResolution){ .md-button .md-button--store target="_blank" }

### getSessionCount

!!! function "getSessionCount()"
	Get the number of currently connected Steam Remote Play sessions.

	**Returns:** uint32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#GetSessionCount){ .md-button .md-button--store target="_blank" }

### getSessionID

!!! function "getSessionID( ```uint32``` index )"
	Get the currently connected Steam Remote Play session ID at the specified index.

	**Returns:** uint32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#GetSessionID){ .md-button .md-button--store target="_blank" }

### getSessionSteamID

!!! function "getSessionSteamID( ```uint32``` session_id )"
	Get the Steam ID of the connected user.

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#GetSessionSteamID){ .md-button .md-button--store target="_blank" }

### sendRemotePlayTogetherInvite

!!! function "sendRemotePlayTogetherInvite( ```uint64_t``` friend_id )"
	Invite a friend to join the game using Remote Play Together.
	
	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#BSendRemotePlayTogetherInvite){ .md-button .md-button--store target="_blank" }

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### remote_play_session_connected

!!! function "remote_play_session_connected"
	Called when a player connects to a remote play session.

	**Returns:**

	* session_id (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#SteamRemotePlaySessionConnected_t){ .md-button .md-button--store target="_blank" }

### remote_play_session_disconnected

!!! function "remote_play_session_disconnected"
	Called when a player disconnects from a remote play session.

	**Returns:**

	* session_id (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#SteamRemotePlaySessionDisconnected_t){ .md-button .md-button--store target="_blank" }
