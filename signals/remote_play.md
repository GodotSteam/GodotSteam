# Signals - Remote Play

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## remote_play_session_connected

!!! function "remote_play_session_connected"
	Called when a player connects to a remote play session.

	**Returns:**

	* session_id (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#SteamRemotePlaySessionConnected_t){ .md-button .md-button--store target="_blank" }

## remote_play_session_disconnected

!!! function "remote_play_session_disconnected"
	Called when a player disconnects from a remote play session.

	**Returns:**

	* session_id (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemotePlay#SteamRemotePlaySessionDisconnected_t){ .md-button .md-button--store target="_blank" }
