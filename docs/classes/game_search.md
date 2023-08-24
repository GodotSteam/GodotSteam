# Game Search

---

## Functions

These functions are not listed in the official Steamworks SDK documentation but do exist in the SDK header files.

---

### acceptGame

!!! function "acceptGame()"
	After receiving [search_for_game_result](#search_for_game_result), accept or decline the game. Multiple [search_for_game_result](#search_for_game_result) will follow as players accept game until the host starts or cancels the game.

	**Note:** The [search_for_game_progress](#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

### addGameSearchParams

!!! function "addGameSearchParams( ```string``` key, ```string``` value )"
	A keyname and a list of comma separated values: one of which is must be found in order for the match to qualify; fails if a search is currently in progress.
	**Returns:** int

### cancelRequestPlayersForGame

!!! function "cancelRequestPlayersForGame()"
	Cancel request and leave the pool of game hosts looking for players.

	**Returns:** int

### declineGame

!!! function "declineGame()"
	After receiving [search_for_game_result](#search_for_game_result), accept or decline the game. Multiple [search_for_game_result](#search_for_game_result) will follow as players accept game until the host starts or cancels the game.

	**Note:** The [search_for_game_progress](#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

### endGame

!!! function "endGame( ```uint64_t``` game_id )"
	Ends the game. No further [submitPlayerResult](#submitplayerresult) for **game_id** will be accepted.

	**Returns:** int

### endGameSearch

!!! function "endGameSearch()"
	Leaves queue if still waiting.

	**Returns:** int

### hostConfirmGameStart

!!! function "hostConfirmGameStart( ```uint64_t``` game_id )"
	Accept the player list and release connection details to players.

	**Returns:** int

### requestPlayersForGame

!!! function "requestPlayersForGame( ```int``` player_min, ```int``` player_max, ```int``` max_team_size )"
	Mark server as available for more players with **player_min**, **player_max** desired. Accept no lobbies with player-count greater than **max_team_size**.

	**Returns:** int

### retrieveConnectionDetails

!!! function "retrieveConnectionDetails( ```uint64_t``` host_id )"
	After receiving **GameStartedByHostCallback_t** get connection details to server.

	**Note:** The **GameStartedByHostCallback_t** callback does not seem to exist in the SDK anywhere.

	**Returns:** string

### searchForGameSolo

!!! function "searchForGameSolo( ```int``` player_min, ```int``` player_max)"
	User enter the queue and await a [search_for_game_progress](#search_for_game_progress) callback. fails if another search is currently in progress. Periodic callbacks will be sent as queue time estimates change.

	**Note:** The [search_for_game_progress](#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

### searchForGameWithLobby

!!! function "searchForGameWithLobby( ```uint64_t``` lobby_id, ```int``` player_min, ```int``` player_max)"

	All players in lobby enter the queue and await a [search_for_game_progress](#search_for_game_progress) callback. Fails if another search is currently in progress. If not the owner of the lobby or search already in progress this call fails. Periodic callbacks will be sent as queue time estimates change.

	**Note:** The [search_for_game_progress](#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

### setConnectionDetails

!!! function "setConnectionDetails( ```string``` details, ```int``` connection_details )"
	Set connection details for players once game is found so they can connect to this server.

	**Returns:** int

### setGameHostParams

!!! function "setGameHostParams( ```string``` key, ```string``` value )"
	A keyname and a list of comma separated values: all the values you allow.

	**Returns:** int

### submitPlayerResult

!!! function "submitPlayerResult( ```uint64_t``` game_id, ```uint64_t``` player_id, ```int``` player_result )"

	Submit a result for one player; does not end the game. **game_id** continues to describe this game.

	**Returns:** int

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### end_game_result

!!! function "end_game_result"
	This callback confirms that the game is recorded as complete on the matchmaking service, the next call to [requestPlayersForGame](#requestplayersforgame) will generate a new unique game ID.

	**Returns:**

	* result (int)
	* game_id (uint64_t)

### request_players_for_game_final_result

!!! function "request_players_for_game_final_result"
	There are no notes about this in Valve's header files or documentation.

	**Returns:**

	* result (int)
	* search_id (uint64_t)
	* game_id (uint64_t)

### request_players_for_game_progress

!!! function "request_players_for_game_progress"
	Callback from [requestPlayersForGame](#requestplayersforgame) when the matchmaking service has started or ended search; callback will also follow a call from [cancelRequestPlayersForGame](#cancelrequestplayersforgame) - search_in_progress will be false.

	**Returns:**

	* result (int)
	* search_id (uint64_t)

### request_players_for_game_result

!!! function "request_players_for_game_result"
	Callback from [requestPlayersForGame](#requestplayersforgame), one of these will be sent per player followed by additional callbacks when players accept or decline the game.

	**Returns:**

	* result (int)
	* search_id (uint64_t)
	* player_data (dictionary)
		* player_id (uint64_t)
		* lobby_id (uint64_t)
		* player_accept_state (int)
		* player_index (int32)
		* total_players (int32)
		* total_players_accepted_game (int32)
		* suggested_team_index (int32)
		* unique_game_id (uint64_t)

### search_for_game_progress

!!! function "search_for_game_progress"
	There are no notes about this in Valve's header files or documentation.

	**Returns:**

	* result (int)
	* search_id (uint64_t)
	* search_progress (dictionary):
		* lobby_id (uint64_t)
		* ended_search_id (uint64_t)
		* seconds_remaining_estimate (int32)
		* players_searching (int32)

### search_for_game_result

!!! function "search_for_game_result"
	Notification to all players searching that a game has been found.

	**Returns:**

	* result (int)
	* search_id (uint64_t)
	* search_result (dictionary):
		* count_players_ingame (int32)
		* count_accepted_game (int32)
		* host_id (uint64_t)
		* final_callback (bool)

### submit_player_result

!!! function "submit_player_result"
	This callback confirms that results were received by the matchmaking service for this player.

	**Returns:**

	* result (int)
	* game_id (uint64_t)
	* player_id (uint64_t)

---

## Enums

---

### GameSearchErrorCode

Enumerator | Value
---------- | -----
GAME_SEARCH_ERROR_CODE_OK | 1
GAME_SEARCH_ERROR_CODE_SEARCH_AREADY_IN_PROGRESS | 2
GAME_SEARCH_ERROR_CODE_NO_SEARCH_IN_PROGRESS | 3
GAME_SEARCH_ERROR_CODE_NOT_LOBBY_LEADER | 4
GAME_SEARCH_ERROR_CODE_NO_HOST_AVAILABLE | 5
GAME_SEARCH_ERROR_CODE_SEARCH_PARAMS_INVALID | 6
GAME_SEARCH_ERROR_CODE_OFFLINE | 7
GAME_SEARCH_ERROR_CODE_NOT_AUTHORIZED | 8
GAME_SEARCH_ERROR_CODE_UNKNOWN_ERROR | 9

### PlayerResult

Enumerator | Value
---------- | -----
PLAYER_RESULT_FAILED_TO_CONNECT | 1
PLAYER_RESULT_ABANDONED | 2
PLAYER_RESULT_KICKED | 3
PLAYER_RESULT_INCOMPLETE | 4
PLAYER_RESULT_COMPLETED | 5
