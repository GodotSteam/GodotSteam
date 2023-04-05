# Signals - Matchmaking

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

------

## favorites_list_accounts_updated

!!! function "favorites_list_accounts_updated"
	Called when an account on your favorites list is updated.

	**Returns:**

	* result (int)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#FavoritesListAccountsUpdated_t){ .md-button .md-button--store target="_blank" }

## favorites_list_changed

!!! function "favorites_list_changed"
	A server was added/removed from the favorites list, you should refresh now.

	**Returns:**

	* favorite (dictionary):
		* ip (string)
		* query_port (uint32)
		* connection_port (uint32)
		* app_id (uint32_t)
		* flags (uint32)
		* add (bool)
		* account_id (uint32)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#FavoritesListChanged_t){ .md-button .md-button--store target="_blank" }

## lobby_chat_update

!!! function "lobby_chat_update"
	A lobby chat room state has changed, this is usually sent when a user has joined or left the lobby.

	**Returns:**

	* lobby_id (uint64_t)
	* changed_id (uint64_t)
	* making_change_id (uint64_t)
	* chat_state (uint32)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyChatUpdate_t){ .md-button .md-button--store target="_blank" }

## lobby_created

!!! function "lobby_created"
	Result of our request to create a lobby. At this point, the lobby has been joined and is ready for use.

	**Returns:**

	* connect (int)
	* lobby (uint64_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyCreated_t){ .md-button .md-button--store target="_blank" }

## lobby_data_update

!!! function "lobby_data_update"
	The lobby metadata has changed.

	**Returns:**

	* success (uint8)
	* lobby_id (uint64_t)
	* member_id (uint64_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyDataUpdate_t){ .md-button .md-button--store target="_blank" }

## lobby_game_created

!!! function "lobby_game_created"
	A game server has been set via [setLobbyGameServer](/functions/matchmaking/#setlobbygameserver) for all of the members of the lobby to join. It's up to the individual clients to take action on this; the typical game behavior is to leave the lobby and connect to the specified game server; but the lobby may stay open throughout the session if desired.

	**Returns:**

	* lobby_id (uint64_t)
	* server_id (uint64_t)
	* server_ip (uint32)
	* port (uint16)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyGameCreated_t){ .md-button .md-button--store target="_blank" }

## lobby_invite

!!! function "lobby_invite"
	Someone has invited you to join a lobby. Normally you don't need to do anything with this, as the Steam UI will also display a 'user has invited you to the lobby, join?' notification and message.

	**Returns:**

	* inviter (uint64_t)
	* lobby (uint64_t)
	* game (uint64_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyGameCreated_t){ .md-button .md-button--store target="_blank" }

## lobby_joined

!!! function "lobby_joined"
	Recieved upon attempting to enter a lobby. Lobby metadata is available to use immediately after receiving this.

	**Returns:**

	* lobby_id (uint64_t)
	* permissions (uint32_t)
	* locked (bool)
	* response (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyEnter_t){ .md-button .md-button--store target="_blank" }

## lobby_kicked

!!! function "lobby_kicked"
	Posted if a user is forcefully removed from a lobby; can occur if a user loses connection to Steam.

	**Returns:**

	* lobby_id (uint64_t)
	* admin_id (uint64_t)
	* due_to_disconnect (uint8)
    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyKicked_t){ .md-button .md-button--store target="_blank" }

## lobby_match_list

!!! function "lobby_match_list"
	Result when requesting the lobby list.

	Emits signal in response to function [requestLobbyList](/functions/matchmaking/#requestlobbylist).

	**Returns:**

	* lobbies (array)

	**Notes:** With the GDNative plug-in, this callback will also send back the lobby count as an integer; this fixes a very strange issue with GDNative mangling the lobbies array.

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyMatchList_t){ .md-button .md-button--store target="_blank" }

## lobby_message

!!! function "lobby_message"
	A chat (text or binary) message for this lobby has been received.

	**Returns:**

	* lobby_id (uint64_t)
	* user (uint64_t)
	* buffer (string)
	* chat_type (int)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyChatMsg_t){ .md-button .md-button--store target="_blank" }
