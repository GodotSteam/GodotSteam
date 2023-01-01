# Signals - Game Server

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## associate_clan

!!! function "associate_clan"
	Sent as a reply to [associateWithClan](/functions/game_server/#associatewithclan).
	
	**Returns:**

	* result (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServer#AssociateWithClanResult_t){ .md-button .md-button--store target="_blank" }

## client_approved

!!! function "client_approved"
	Client has been approved to connect to this game server.

	Emits signal in response to function [sendUserConnectAndAuthenticate](/functions/game_server/#senduserconnectandauthenticate).

	**Returns:**

	* steam_id (uint64_t)
	* owner_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServer#GSClientApprove_t){ .md-button .md-button--store target="_blank" }

## client_denied

!!! function "client_denied"
	Client has been denied to connection to this game server.

	Emits signal in response to function [sendUserConnectAndAuthenticate](/functions/game_server/#senduserconnectandauthenticate).

	**Returns:**

	* steam_id (uint64_t)
	* reason (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServer#GSClientDeny_t){ .md-button .md-button--store target="_blank" }

## client_group_status

!!! function "client_group_status"
	Sent as a reply to [requestUserGroupStatus](/functions/game_server/#requestusergroupstatus).

	**Returns:**

	* steam_id (uint64_t)
	* group_id (uint64_t)
	* member (bool)
	* officer (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServer#GSClientGroupStatus_t){ .md-button .md-button--store target="_blank" }

## client_kick

!!! function "client_kick"
	Request the game server should kick the user.
	
	**Returns:**

	* steam_id (uint64_t)
	* reason (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServer#GSClientKick_t){ .md-button .md-button--store target="_blank" }

## player_compat

!!! function "player_compat"
	Sent as a reply to [computeNewPlayerCompatibility](/functions/game_server/#computenewplayercompatibility).
	
	**Returns:**

	* result (int)
	* players_dont_like_candidate (int)
	* players_candidate_doesnt_like (int)
	* clan_players_dont_like_candidate (int)
	* steam_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServer#ComputeNewPlayerCompatibilityResult_t){ .md-button .md-button--store target="_blank" }

## policy_response

!!! function "policy_response"
	Received when the game server requests to be displayed as secure (VAC protected).
	
	**Returns:**

	* secure (uint8)

	Secure is true if the game server should display itself as secure to users, false otherwise.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServer#GSPolicyResponse_t){ .md-button .md-button--store target="_blank" }

## server_connect_failure

!!! function "server_connect_failure"
	Logging the game server onto Steam.

	Emits signal in response to functions [logOff](/functions/game_server/#logoff), [logOn](/functions/game_server/#logon), or [logOnAnonymous](/functions/game_server/#logonanonymous).

	**Returns:**

	* result (int)
	* retrying (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServerConnectFailure_t){ .md-button .md-button--store target="_blank" }

## server_connected

!!! function "server_connected"
	Server has connected to the Steam back-end.

	Emits signal in response to functions [logOff](/functions/game_server/#logoff), [logOn](/functions/game_server/#logon), or [logOnAnonymous](/functions/game_server/#logonanonymous).

	**Results:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServersConnected_t){ .md-button .md-button--store target="_blank" }

## server_disconnected

!!! function "server_disconnected"
	Called if the client has lost connection to the Steam servers. Real-time services will be disabled until a matching [server_connected](/signals/game_server/#server_connected) has been posted.

	Emits signal in response to functions [logOff](/functions/game_server/#logoff), [logOn](/functions/game_server/#logon), or [logOnAnonymous](/functions/game_server/#logonanonymous).

	**Returns:**

	* result (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServersDisconnected_t){ .md-button .md-button--store target="_blank" }
