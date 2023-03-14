# Signals - Networking

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## p2p_session_connect_fail

!!! function "p2p_session_connect_fail"
	Called when a user sends a packet and it fails.

	**Returns:**

	* steam_id_remote (uint64_t)
	* session_error (uint8_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#P2PSessionConnectFail_t){ .md-button .md-button--store target="_blank" }

## p2p_session_request

!!! function "p2p_session_request"
	Called when a user sends a packet.
	
	**Returns:**

	* steam_id_remote (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#P2PSessionRequest_t){ .md-button .md-button--store target="_blank" }