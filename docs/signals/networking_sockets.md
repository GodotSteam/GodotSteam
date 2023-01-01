# Signals - Networking Sockets

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## fake_ip_result

!!! function "fake_ip_result"
	A struct used to describe a "fake IP" we have been assigned to use as an identifier. This callback is posted when [beginAsyncRequestFakeIP](/functions/networking_sockets/#beginasyncrequestfakeip) completes.

	**Returns:**

	* result (int)
	* ip_reference (string) as "fake_ip_identity"
	* ip (uint32)
	* ports (uint16)

## network_authentication_status

!!! function "network_authentication_status"
	This callback is posted whenever the state of our readiness changes.

	**Returns:**

	* available (int)
	* debug_message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#SteamNetAuthenticationStatus_t){ .md-button .md-button--store target="_blank" }

## network_connection_status_changed

!!! function "network_connection_status_changed"
	This callback is posted whenever a connection is created, destroyed, or changes state. The m_info field will contain a complete description of the connection at the time the change occurred and the callback was posted. In particular, m_info.m_eState will have the new connection state.

	**Returns:**

	* connect_handle (uint64_t)
	* connection (dictionary)
		* identity (string)
		* user_data (uint64_t)
		* listen_socket (uint32)
		* remote_address (string)
		* remote_pop (uint32)
		* pop_relay (uint32)
		* connection_state (int)
		* end_reason (string)
		* end_debug (string)
		* debug_description (string)
	* old_state (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#SteamNetConnectionStatusChangedCallback_t){ .md-button .md-button--store target="_blank" }
