# Functions - Matchmaking

---

## addFavoriteGame

!!! function "addFavoriteGame( ```uint32``` ip, ```uint16``` port, ```uint16``` queryPort, ```uint32``` flags, ```uint32``` lastPlayed )"
	Adds the game server to the local favorites list or updates the time played of the server if it already exists in the list.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](Ahttps://partner.steamgames.com/doc/api/ISteamMatchmaking#AddFavoriteGame){ .md-button .md-button--store target="_blank" }

## addRequestLobbyListDistanceFilter

!!! function "addRequestLobbyListDistanceFilter( ```int``` distance_filter )"
	Sets the physical distance for which we should search for lobbies, this is based on the users IP address and a IP location map on the Steam backed.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#AddRequestLobbyListDistanceFilter){ .md-button .md-button--store target="_blank" }

## addRequestLobbyListFilterSlotsAvailable

!!! function "addRequestLobbyListFilterSlotsAvailable( ```int``` slots_available )"
	Filters to only return lobbies with the specified number of open slots available.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#AddRequestLobbyListFilterSlotsAvailable){ .md-button .md-button--store target="_blank" }

## addRequestLobbyListNearValueFilter

!!! function "addRequestLobbyListNearValueFilter( ```string``` keyToMatch, ```int``` valueToBeCloseTo )"
	Sorts the results closest to the specified value.

	Near filters don't actually filter out values, they just influence how the results are sorted. You can specify multiple near filters, with the first near filter influencing the most, and the last near filter influencing the least.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#AddRequestLobbyListNearValueFilter){ .md-button .md-button--store target="_blank" }

## addRequestLobbyListNumericalFilter

!!! function "addRequestLobbyListNumericalFilter( ```string``` key_to_match, ```int``` value_to_match, ```int``` comparison_type )"
	Adds a numerical comparison filter to the next <strong>requestLobbyList</strong> call.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#AddRequestLobbyListNumericalFilter){ .md-button .md-button--store target="_blank" }

## addRequestLobbyListResultCountFilter

!!! function "addRequestLobbyListResultCountFilter( ```int``` max_results )"
	Sets the maximum number of lobbies to return. The lower the count the faster it is to download the lobby results & details to the client.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#AddRequestLobbyListResultCountFilter){ .md-button .md-button--store target="_blank" }

## addRequestLobbyListStringFilter

!!! function "addRequestLobbyListStringFilter( ```string``` key_to_match, ```string``` value_to_match, ```int``` comparison_type )"
	Adds a string comparison filter to the next <strong>requestLobbyList</strong> call.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#AddRequestLobbyListStringFilter){ .md-button .md-button--store target="_blank" }

## createLobby

!!! function "createLobby( ```int``` lobby_type, ```int``` max_members )"
	Create a new matchmaking lobby.

	Triggers all three callbacks: [lobby_created](/signals/matchmaking/#lobby_created), [lobby_joined](/signals/matchmaking/#lobby_joined), and [lobby_data_update](/signals/matchmaking/#lobby_data_update).

	**Returns:** void

	If the results returned via the [lobby_created](/signals/matchmaking/#lobby_created) call result indicate success then the lobby is joined & ready to use at this point.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#CreateLobby){ .md-button .md-button--store target="_blank" }

## deleteLobbyData

!!! function "deleteLobbyData( ```uint64_t``` steam_lobby_id, ```string``` key )"
	Removes a metadata key from the lobby.

	This can only be done by the owner of the lobby.

	This will only send the data if the key existed. There is a slight delay before sending the data so you can call this repeatedly to set all the data you need to and it will automatically be batched up and sent after the last sequential call.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#DeleteLobbyData){ .md-button .md-button--store target="_blank" }

## getAllLobbyData

!!! function "getAllLobbyData( ```uint64_t``` steam_lobby_id )"
	Get lobby data by the lobby's ID.

	**Returns:** dictionary

	Containing the following keys:

	* index (int)
	* key (string)
	* value (string)

	**Note:** This is a GodotSteam specific function.

## getFavoriteGames

!!! function "getFavoriteGames()"
	Gets the details of the favorite game server by index.

	**Returns:** array

	Containing a list of:

	favorite (dictionary)

	Contains the following keys:

	* ret (bool)
	* app (uint32)
	* ip (string)
	* game_port (uint16)
	* query_port (uint16)
	* flags (uint32)
	* last_played (uint32)

	**Note:** You must call [getFavoriteGameCount](/functions/matchmaking/#getfavoritegamecount) before calling this.  This is already used in GodotSteam's implementation, however.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetFavoriteGame){ .md-button .md-button--store target="_blank" }

## getLobbyData

!!! function "getLobbyData( ```uint64_t``` steam_lobby_id, ```string``` key )"
	Gets the metadata associated with the specified key from the specified lobby.

	**Returns:** string

	Returns an empty string if no value is set for this key, or if **steam_lobby_id** is invalid.

	**Note:** This can only get metadata from lobbies that the client knows about, either after receiving a list of lobbies from [lobby_match_list](/signals/matchmaking/#lobby_match_list), retrieving the data with [requestLobbyData](/functions/matchmaking/#requestlobbydata) or after joining a lobby.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetLobbyData){ .md-button .md-button--store target="_blank" }

## getLobbyGameServer

!!! function "getLobbyGameServer( ```uint64_t``` steam_lobby_id )"
	Gets the details of a game server set in a lobby. Either the IP/Port or the Steam ID of the game server has to be valid, depending on how you want the clients to be able to connect.

	**Returns:** dictionary

	* ret (bool) - true if the lobby is valid and has a valid game server set; otherwise, false.
	* ip (string)
	* port (uint32)
	* id (uint64_t)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetLobbyGameServer){ .md-button .md-button--store target="_blank" }

## getLobbyMemberByIndex

!!! function "getLobbyMemberByIndex( ```uint64_t``` steam_lobby_id, ```int``` member )"
	Gets the Steam ID of the lobby member at the given index.

	**Returns:** int

	**Note:** You must call [getNumLobbyMembers](/functions/matchmaking/#getnumlobbymembers) before calling this.

	**Note:** The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetLobbyMemberByIndex){ .md-button .md-button--store target="_blank" }

## getLobbyMemberData

!!! function "getLobbyMemberData( ```uint64_t``` steam_lobby_id, ```uint64_t``` steam_user_id, ```string``` key )"
	Gets per-user metadata from another player in the specified lobby.

	This can only be queried from members in lobbies that you are currently in.

	**Returns:** string

	Returns _NULL_ if **steam_lobby_id is invalid**, or **steam_user_id** is not in the lobby. Returns an empty string if **key** is not set for the player.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetLobbyMemberData){ .md-button .md-button--store target="_blank" }

## getLobbyMemberLimit

!!! function "getLobbyMemberLimit( ```uint64_t``` steam_lobby_id )"
	The current limit on the number of users who can join the lobby.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetLobbyMemberLimit){ .md-button .md-button--store target="_blank" }

## getLobbyOwner

!!! function "getLobbyOwner( ```uint64_t``` steam_lobby_id )"
	Returns the current lobby owner.

	**Returns:** uint64_t

	**Note:** You must be a member of the lobby to access this.
	There always one lobby owner - if the current owner leaves, another user in the lobby will become the owner automatically. It is possible (but rare) to join a lobby just as the owner is leaving, thus entering a lobby with self as the owner.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetLobbyOwner){ .md-button .md-button--store target="_blank" }

## getNumLobbyMembers

!!! function "getNumLobbyMembers( ```uint64_t``` steam_lobby_id )"
	Gets the number of users in a lobby.

	This is used for iteration, after calling this then [getLobbyMemberByIndex](/functions/matchmaking/#getlobbymemberbyindex) can be used to get the Steam ID of each person in the lobby. Persona information for other lobby members (name, avatar, etc.) is automatically received and accessible via the **SteamFriends** interface.

	**Returns:** int

	**Note:** The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#GetNumLobbyMembers){ .md-button .md-button--store target="_blank" }

## inviteUserToLobby

!!! function "inviteUserToLobby( ```uint64_t``` steam_lobby_id, ```uint64_t``` steam_id_invitee )"
	Invite another user to the lobby.

	If the specified user clicks the join link, a [join_requested](/signals/matchmaking/#join_requested) callback will be posted if the user is in-game, or if the game isn't running yet then the game will be automatically launched with the command line parameter ```+connect_lobby <64-bit lobby Steam ID>``` instead.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#InviteUserToLobby){ .md-button .md-button--store target="_blank" }

## joinLobby

!!! function "joinLobby( ```int``` steam_lobby_id )"
	Joins an existing lobby.

	The lobby Steam ID can be obtained either from a search with [requestLobbyList](/functions/matchmaking/#requestlobbylist) joining on a friend, or from an invite.

	Triggers a [lobby_joined](/signals/matchmaking/#lobby_joined) callback for other users.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#JoinLobby){ .md-button .md-button--store target="_blank" }

## leaveLobby

!!! function "leaveLobby( ```uint64_t``` steam_lobby_id )"
	Leave a lobby that the user is currently in; this will take effect immediately on the client side, other users in the lobby will be notified by a [lobby_chat_update](/signals/matchmaking/#lobby_chat_update) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LeaveLobby){ .md-button .md-button--store target="_blank" }

## removeFavoriteGame

!!! function "removeFavoriteGame( ```uint32``` app_id, ```uint32``` ip, ```uint16``` port, ```uint16``` query_port, ```uint32``` flags )"
	Removes the game server from the local favorites list.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#RemoveFavoriteGame){ .md-button .md-button--store target="_blank" }

## requestLobbyData

!!! function "requestLobbyData( ```uint64_t``` steam_lobby_id )"
	Refreshes all of the metadata for a lobby that you're not in right now.

	You will never do this for lobbies you're a member of, that data will always be up to date. You can use this to refresh lobbies that you have obtained from [requestLobbyList](/functions/matchmaking/#requestlobbylist) or that are available via friends.

	Triggers a [lobby_data_update](/signals/matchmaking/#lobby_data_update) callback.

	**Returns:** bool

	_True_ if the request was successfully sent to the server. _False_ if no connection to Steam could be made, or **steam_lobby_id** is invalid.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#RequestLobbyData){ .md-button .md-button--store target="_blank" }

## requestLobbyList

!!! function "requestLobbyList()"
	Get a filtered list of relevant lobbies.

	There can only be one active lobby search at a time. The old request will be canceled if a new one is started. Depending on the users connection to the Steam back-end, this call can take from 300ms to 5 seconds to complete, and has a timeout of 20 seconds.

	Triggers a [lobby_match_list](/signals/matchmaking/#lobby_match_list) callback.

	**Returns:** void

	**Note:** To filter the results you _must_ call the **addRequestLobbyList** functions before calling this. The filters are cleared on each call to this function.

	**Note:** If [addRequestLobbyListDistanceFilter](/functions/matchmaking/#addrequestlobbylistdistancefilter) is not called, **k_ELobbyDistanceFilterDefault** will be used, which will only find matches in the same or nearby regions.

	**Note:** This will only return lobbies that are not full, and only lobbies that are **public (2)** or **invisible (3)**, and are set to joinable with [setLobbyJoinable](/functions/matchmaking/#setlobbyjoinable).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#RequestLobbyList){ .md-button .md-button--store target="_blank" }

## sendLobbyChatMsg

!!! function "sendLobbyChatMsg( ```uint64_t``` steam_lobby_id, ```string``` message_body )"
	Broadcasts a chat (text or binary data) message to the all of the users in the lobby.
	All users in the lobby (including the local user) will receive a [lobby_message](/signals/matchmaking/#lobby_message) callback with the message.

	For communication that needs to be arbitrated (for example having a user pick from a set of characters, and making sure only one user has picked a character), you can use the lobby owner as the decision maker. [getLobbyOwner](/function/matchmaking/#getlobbyowner) returns the current lobby owner. There is guaranteed to always be one and only one lobby member who is the owner. So for the choose-a-character scenario, the user who is picking a character would send the binary message 'I want to be Zoe', the lobby owner would see that message, see if it was OK, and broadcast the appropriate result (user X is Zoe).

	These messages are sent via the Steam back-end, and so the bandwidth available is limited. For higher-volume traffic like voice or game data, you'll want to use the Steam Networking API.

	Triggers a [lobby_message](/signals/matchmaking/#lobby_message).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SendLobbyChatMsg){ .md-button .md-button--store target="_blank" }

## setLobbyData

!!! function "setLobbyData( ```uint64_t``` steam_lobby_id, ```string``` key, ```string``` value )"
	Sets a key/value pair in the lobby metadata. This can be used to set the the lobby name, current map, game mode, etc.

	This can only be set by the owner of the lobby. Lobby members should use [setLobbyMemberData](/functions/matchmaking/#setlobbymemberdata) instead.

	Each user in the lobby will be receive notification of the lobby data change via a [lobby_data_update](/signals/matchmaking/#lobby_data_update) callback, and any new users joining will receive any existing data.

	This will only send the data if it has changed. There is a slight delay before sending the data so you can call this repeatedly to set all the data you need to and it will automatically be batched up and sent after the last sequential call.
	
	**Returns:** bool

	_True_ if the data has been set successfully. _False_ if **steam_lobby_id** was invalid, or the key/value are too long.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SetLobbyData){ .md-button .md-button--store target="_blank" }

## setLobbyGameServer

!!! function "setLobbyGameServer( ```uint64_t``` steam_lobby_id, ```string``` server_ip, ```uint16``` server_port, ```uint64_t``` steam_id_game_server )"
	Sets the game server associated with the lobby.

	This can only be set by the owner of the lobby.

	Either the IP/Port or the Steam ID of the game server must be valid, depending on how you want the clients to be able to connect.

	A [lobby_game_created](/signals/matchmaking/#lobby_game_created) callback will be sent to all players in the lobby, usually at this point, the users will join the specified game server.

	Triggers a [lobby_game_created](/signals/matchmaking/#lobby_game_created) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SetLobbyGameServer){ .md-button .md-button--store target="_blank" }

## setLobbyJoinable

!!! function "setLobbyJoinable( ```uint64_t``` steam_lobby_id, ```bool``` joinable )"
	Sets whether or not a lobby is joinable by other players. This always defaults to enabled for a new lobby.

	If joining is disabled, then no players can join, even if they are a friend or have been invited.

	Lobbies with joining disabled will not be returned from a lobby search.

	**Returns:** bool

	_True_ upon success; otherwise, _false_ if you're not the owner of the lobby.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SetLobbyJoinable){ .md-button .md-button--store target="_blank" }

## setLobbyMemberData

!!! function "setLobbyMemberData( ```uint64_t``` steam_lobby_id, ```string``` key, ```string``` value )"
	Sets per-user metadata for the local user.

	Each user in the lobby will be receive notification of the lobby data change via a [lobby_data_update](/signals/matchmaking/#lobby_data_update) callback, and any new users joining will receive any existing data.

	There is a slight delay before sending the data so you can call this repeatedly to set all the data you need to and it will automatically be batched up and sent after the last sequential call.

	Triggers a [lobby_data_update](/signals/matchmaking/#lobby_data_update) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SetLobbyMemberData){ .md-button .md-button--store target="_blank" }

## setLobbyMemberLimit

!!! function "setLobbyMemberLimit( ```uint64_t``` steam_lobby_id, ```int``` max_members )"
	Set the maximum number of players that can join the lobby.

	This is also set when you create the lobby with [createLobby](/functions/matchmaking/#createlobby).

	This can only be set by the owner of the lobby.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SetLobbyMemberLimit){ .md-button .md-button--store target="_blank" }

## setLobbyOwner

!!! function "setLobbyOwner( ```uint64_t``` steam_lobby_id, ```uint64_t``` steam_id_new_owner )"
	Changes who the lobby owner is.

	This can only be set by the owner of the lobby. This will trigger a [lobby_data_update](/signals/matchmaking/#lobby_data_update) for all of the users in the lobby, each user should update their local state to reflect the new owner. This is typically accomplished by displaying a crown icon next to the owners name.

	Triggers a [lobby_data_update](/signals/matchmaking/#lobby_data_update) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SetLobbyOwner){ .md-button .md-button--store target="_blank" }

## setLobbyType

!!! function "setLobbyType( ```int``` steam_lobby_id, ```int``` eLobbyType )"
	Updates what type of lobby this is.

	This is also set when you create the lobby with [createLobby](/functions/matchmaking/#createlobby).

	This can only be set by the owner of the lobby.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#SetLobbyType){ .md-button .md-button--store target="_blank" }