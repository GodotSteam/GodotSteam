# Functions - Matchmaking Servers

---

## cancelQuery

!!! function "cancelQuery( ```uint64_t``` server_list_request = 0 )"
	Cancel an outstanding server list request.

	You should call this to cancel any in-progress requests before destructing a callback object that may have been passed to one of the below request calls. Not doing so may result in a crash when a callback occurs on the destructed object. Canceling a query does not release the allocated request handle. The request handle must be released using [releaseRequest](/functions/matchmaking_servers/#releaserequest).

	You can pass a *server_list_request* handle or, if you do not, it will use the last internally stored one.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#CancelQuery){ .md-button .md-button--store target="_blank" }

## cancelServerQuery

!!! function "cancelServerQuery( ```int``` server_query )"
	Cancel an outstanding individual server query.

	The calls that create this type of query are: [pingServer](/functions/matchmaking_servers/#pingserver), [playerDetails](/functions/matchmaking_servers/#playerdetails), and [serverRules](/functions/matchmaking_servers/#serverrules). You should call this to cancel any in-progress requests before destructing a callback object that may have been passed to one of the above calls to avoid crashing when callbacks occur.

	**Returns:** void
	
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#CancelServerQuery){ .md-button .md-button--store target="_blank" }

## getServerCount

!!! function "getServerCount( ```uint64_t``` server_list_request = 0 )"
	Gets the number of servers in the given list. This is used for iterating with [getServerDetails](/functions/matchmaking_servers/#getserverdetails).

	You can pass a *server_list_request* handle or, if you do not, it will use the last internally stored one.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#GetServerCount){ .md-button .md-button--store target="_blank" }

## getServerDetails

!!! function "getServerDetails( ```int``` server, ```uint64_t``` server_list_request = 0 )"
	Get the details of a given server in the list.

	You can get the valid range of index values by calling [getServerCount](/functions/matchmaking_servers/#getservercount). You will also receive index values in [server_responded](/signals/matchmaking_servers/#server_responded) callbacks.

	You can pass a *server_list_request* handle or, if you do not, it will use the last internally stored one.

	**Returns:** dictionary

	Contains the following keys:

	* ping (int)
	* success_response (bool)
	* no_refresh (bool)
	* game_dir (string)
	* map (string)
	* description (string)
	* app_id (uint32)
	* players (int)
	* max_players (int)
	* bot_players (int)
	* password (bool)
	* secure (bool)
	* last_played (uint32)
	* server_version (int)	

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#GetServerDetails){ .md-button .md-button--store target="_blank" }

## isRefreshing

!!! function "isRefreshing( ```uint64_t``` server_list_request = 0 )"
	Checks if the server list request is currently refreshing.

	You can pass a *server_list_request* handle or, if you do not, it will use the last internally stored one.

	**Returns:** bool
	
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#IsRefreshing){ .md-button .md-button--store target="_blank" }

## pingServer

!!! function "pingServer( ```string``` ip, ```uint``` port )"
	Queries an individual game server directly via IP/Port to request an updated ping time and other details from the server.

	You must inherit from the ISteamMatchmakingPingResponse object to receive this callback.

	**Currently not enabled.**

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#PingServer){ .md-button .md-button--store target="_blank" }

## playerDetails

!!! function "playerDetails( ```string``` ip, ```uint16``` port )"
	Queries an individual game server directly via IP/Port to request the list of players currently playing on the server.

	You must inherit from the ISteamMatchmakingPlayersResponse object to receive this callback.

	**Currently not enabled.**

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#PlayerDetails){ .md-button .md-button--store target="_blank" }	

## refreshQuery

!!! function "refreshQuery( ```uint64_t``` server_list_request = 0 )"
	Ping every server in your list again but don't update the list of servers.

	The query callback installed when the server list was requested will be used again to post notifications and [refresh_complete](/signals/matchmaking_servers/#refresh_complete) will be called again, so the callback must remain valid until it completes or the request is released with [releaseRequest](/functions/matchmaking_servers/#releaserequest).

	You can pass a *server_list_request* handle or, if you do not, it will use the last internally stored one.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RefreshQuery){ .md-button .md-button--store target="_blank" }

## refreshServer

!!! function "refreshServer( ```int``` server, ```uint64_t``` server_list_request = 0 )"
	Refreshes a single server inside of a query.

	If you want to refresh all of the servers then you should use [refreshQuery](/functions/matchmaking_servers/#refreshquery).

	You can pass a *server_list_request* handle or, if you do not, it will use the last internally stored one.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RefreshServer){ .md-button .md-button--store target="_blank" }

## releaseRequest

!!! function "releaseRequest( ```uint64_t``` server_list_request = 0 )"
	Releases the asynchronous server list request object and cancels any pending query on it if there's a pending query in progress.

	You can pass a *server_list_request* handle or, if you do not, it will use the last internally stored one.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#ReleaseRequest){ .md-button .md-button--store target="_blank" }

## requestFavoritesServerList

!!! function "requestFavoritesServerList( ```uint32``` app_id, ```array``` filters )"
	Request a new list of game servers from the 'favorites' server list.

	[See MatchMakingKeyValuePair_t for more information.](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#MatchMakingKeyValuePair_t){ target="_blank" }

	**Returns:** uint64_t
	
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RequestFavoritesServerList){ .md-button .md-button--store target="_blank" }

## requestFriendsServerList

!!! function "requestFriendsServerList( ```uint32``` app_id, ```array``` filters )"
	Request a new list of game servers from the 'friends' server list.

	[See MatchMakingKeyValuePair_t for more information.](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#MatchMakingKeyValuePair_t){ target="_blank" }

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RequestFriendsServerList){ .md-button .md-button--store target="_blank" }
	
## requestHistoryServerList

!!! function "requestHistoryServerList( ```uint32``` app_id, ```array``` filters )"
	Request a new list of game servers from the 'history' server list.

	[See MatchMakingKeyValuePair_t for more information.](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#MatchMakingKeyValuePair_t){ target="_blank" }
	
	**Returns:** uint64_t
	
	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RequestHistoryServerList){ .md-button .md-button--store target="_blank" }
	
## requestInternetServerList

!!! function "requestInternetServerList( ```uint32``` app_id, ```array``` filters )"
	Request a new list of game servers from the 'internet' server list.

	[See MatchMakingKeyValuePair_t for more information.](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#MatchMakingKeyValuePair_t){ target="_blank" }

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RequestInternetServerList){ .md-button .md-button--store target="_blank" }

## requestLANServerList

!!! function "requestLANServerList( ```uint32``` app_id )"
	Request a new list of game servers from the 'LAN' server list.
	
	[See MatchMakingKeyValuePair_t for more information.](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#MatchMakingKeyValuePair_t){ target="_blank" }

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RequestLANServerList){ .md-button .md-button--store target="_blank" }

## requestSpectatorServerList

!!! function "requestSpectatorServerList( ```uint32``` app_id, ```array``` filters )"
	Request a new list of game servers from the 'spectator' server list.
	
	[See MatchMakingKeyValuePair_t for more information.](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#MatchMakingKeyValuePair_t){ target="_blank" }

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#RequestSpectatorServerList){ .md-button .md-button--store target="_blank" }
	
## serverRules

!!! function "serverRules( ```string``` ip, ```uint16``` port )"
	Queries an individual game server directly via IP/Port to request the list of rules that the server is running. [(See setKeyValue to set the rules on the server side.)](https://partner.steamgames.com/doc/api/ISteamGameServer#SetKeyValue){ target="_blank" }
	
	You must inherit from the ISteamMatchmakingRulesResponse object to receive this callback.
	
	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmakingServers#ServerRules){ .md-button .md-button--store target="_blank" }