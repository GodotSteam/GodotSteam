# Signals - Users

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## duration_control

!!! function "duration_control"
	Sent for games with enabled anti indulgence / duration control, for enabled users. Lets the game know whether persistent rewards or XP should be granted at normal rate, half rate, or zero rate.

	**Returns:**
	## 
	* result (int)
	* duration (dictionary)
		* app_id (uint32)
		* applicable (bool)
		* seconds_last_5hrs (int32)
		* progress (int)
		* notification (int)
		* notification_verbal (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#DurationControl_t){ .md-button .md-button--store target="_blank" }

## client_game_server_deny

!!! function "client_game_server_deny"
	Sent by the Steam server to the client telling it to disconnect from the specified game server, which it may be in the process of or already connected to. The game client should immediately disconnect upon receiving this message. This can usually occur if the user doesn't have rights to play on the game server.

	**Returns:**
			
	* app_id (uint32)
	* ip (string)
	* server_port (uint16)
	* secure (uint16)
	* reason (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#ClientGameServerDeny_t){ .md-button .md-button--store target="_blank" }

## encrypted_app_ticket_response

!!! function "encrypted_app_ticket_response"
	Called when an encrypted application ticket has been received.

	**Returns:**

	* result (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#EncryptedAppTicketResponse_t){ .md-button .md-button--store target="_blank" }

## game_web_callback

!!! function "game_web_callback"
	Sent to your game in response to a ```steam://gamewebcallback/``` command from a user clicking a link in the Steam overlay browser. You can use this to add support for external site signups where you want to pop back into the browser after some web page signup sequence, and optionally get back some detail about that.

	**Returns:**

	* url (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GameWebCallback_t){ .md-button .md-button--store target="_blank" }

## get_auth_session_ticket_response

!!! function "get_auth_session_ticket_response"
	Result when creating an auth session ticket.

	**Returns:**
		
	* auth_ticket (uint32)
	* result (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetAuthSessionTicketResponse_t){ .md-button .md-button--store target="_blank" }

## get_ticket_for_web_api

!!! function "get_ticket_for_web_api"
	Result when creating an web API ticket from GetAuthTicketForWebApi.

	**Returns:**

	* auth_ticket (uint32)
	* result (int)
	* ticket_size (int)
	* ticket_buffer (uint8)

## ipc_failure

!!! function "ipc_failure"
	Called when the callback system for this client is in an error state (and has flushed pending callbacks). When getting this message the client should disconnect from Steam, reset any stored Steam state and reconnect. This usually occurs in the rare event the Steam client has some kind of fatal error.

	**Returns:**

	* type (uint8)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#IPCFailure_t){ .md-button .md-button--store target="_blank" }

## licenses_updated

!!! function "licenses_updated"
	Called whenever the users licenses (owned packages) changes.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#LicensesUpdated_t){ .md-button .md-button--store target="_blank" }

## microtransaction_auth_response

!!! function "microtransaction_auth_response"
	Called when a user has responded to a microtransaction authorization request.

	**Returns:**

	* app_id (uint32)
	* order_id (uint64_t)
	* authorized (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#MicroTxnAuthorizationResponse_t){ .md-button .md-button--store target="_blank" }

## steam_server_connect_failed

!!! function "steam_server_connect_failed"
	Called when a connection attempt has failed. This will occur periodically if the Steam client is not connected, and has failed when retrying to establish a connection.

	**Returns:**

	* result (int)
	* retrying (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServerConnectFailure_t){ .md-button .md-button--store target="_blank" }

## steam_server_connected

!!! function "steam_server_connected"
	Called when a connections to the Steam back-end has been established. This means the Steam client now has a working connection to the Steam servers. Usually this will have occurred before the game has launched, and should only be seen if the user has dropped connection due to a networking issue or a Steam server update.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServersConnected_t){ .md-button .md-button--store target="_blank" }

## steam_server_disconnected

!!! function "steam_server_disconnected"
	Called if the client has lost connection to the Steam servers. Real-time services will be disabled until a matching [steam_server_connected](/signals/users/#steam_server_connected) has been posted.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServersDisconnected_t){ .md-button .md-button--store target="_blank" }

## store_auth_url_response

!!! function "store_auth_url_response"
	Response when we have recieved the authentication URL after a call to [requestStoreAuthURL](/functions/user/#requeststoreauthurl).

	**Returns:**

	* url (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#StoreAuthURLResponse_t){ .md-button .md-button--store target="_blank" }

## validate_auth_ticket_response

!!! function "validate_auth_ticket_response"
	Called when an auth ticket has been validated.
	
	Emits signal in response to function [beginAuthSession](/functions/user/#beginauthsession).

	**Returns:**

	* auth_id (uint64_t)
	* response (uint32_t)
	* owner_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#ValidateAuthTicketResponse_t){ .md-button .md-button--store target="_blank" }
