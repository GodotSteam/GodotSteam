# Functions - Game Search

These functions are not listed in the official Steamworks SDK documentation but do exist in the SDK header files.

---

## acceptGame

!!! function "acceptGame()"
	After receiving [search_for_game_result](/signals/game_search/#search_for_game_result), accept or decline the game. Multiple [search_for_game_result](/signals/game_search/#search_for_game_result) will follow as players accept game until the host starts or cancels the game.

	**Note:** The [search_for_game_progress](/signals/game_search/#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

## addGameSearchParams

!!! function "addGameSearchParams( ```string``` key, ```string``` value )"
	A keyname and a list of comma separated values: one of which is must be found in order for the match to qualify; fails if a search is currently in progress.
	**Returns:** int

## cancelRequestPlayersForGame

!!! function "cancelRequestPlayersForGame()"
	Cancel request and leave the pool of game hosts looking for players.

	**Returns:** int

## declineGame

!!! function "declineGame()"
	After receiving [search_for_game_result](/signals/game_search/#search_for_game_result), accept or decline the game. Multiple [search_for_game_result](/signals/game_search/#search_for_game_result) will follow as players accept game until the host starts or cancels the game.

	**Note:** The [search_for_game_progress](/signals/game_search/#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

## endGame

!!! function "endGame( ```uint64_t``` game_id )"
	Ends the game. No further [submitPlayerResult](/functions/game_search/#submitplayerresult) for **game_id** will be accepted.

	**Returns:** int

## endGameSearch

!!! function "endGameSearch()"
	Leaves queue if still waiting.

	**Returns:** int

## hostConfirmGameStart

!!! function "hostConfirmGameStart( ```uint64_t``` game_id )"
	Accept the player list and release connection details to players.

	**Returns:** int

## requestPlayersForGame

!!! function "requestPlayersForGame( ```int``` player_min, ```int``` player_max, ```int``` max_team_size )"
	Mark server as available for more players with **player_min**, **player_max** desired. Accept no lobbies with player-count greater than **max_team_size**.

	**Returns:** int

## retrieveConnectionDetails

!!! function "retrieveConnectionDetails( ```uint64_t``` host_id )"
	After receiving **GameStartedByHostCallback_t** get connection details to server.

	**Note:** The **GameStartedByHostCallback_t** callback does not seem to exist in the SDK anywhere.

	**Returns:** string

## searchForGameSolo

!!! function "searchForGameSolo( ```int``` player_min, ```int``` player_max)"
	User enter the queue and await a [search_for_game_progress](/signals/game_search/#search_for_game_progress) callback. fails if another search is currently in progress. Periodic callbacks will be sent as queue time estimates change.

	**Note:** The [search_for_game_progress](/signals/game_search/#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

## searchForGameWithLobby

!!! function "searchForGameWithLobby( ```uint64_t``` lobby_id, ```int``` player_min, ```int``` player_max)"

	All players in lobby enter the queue and await a [search_for_game_progress](/signals/game_search/#search_for_game_progress) callback. Fails if another search is currently in progress. If not the owner of the lobby or search already in progress this call fails. Periodic callbacks will be sent as queue time estimates change.

	**Note:** The [search_for_game_progress](/signals/game_search/#search_for_game_progress) callback does not seem to exist in the SDK anywhere.

	**Returns:** int

## setConnectionDetails

!!! function "setConnectionDetails( ```string``` details, ```int``` connection_details )"
	Set connection details for players once game is found so they can connect to this server.

	**Returns:** int

## setGameHostParams

!!! function "setGameHostParams( ```string``` key, ```string``` value )"
	A keyname and a list of comma separated values: all the values you allow.

	**Returns:** int

## submitPlayerResult

!!! function "submitPlayerResult( ```uint64_t``` game_id, ```uint64_t``` player_id, ```int``` player_result )"

	Submit a result for one player; does not end the game. **game_id** continues to describe this game.

	**Returns:** int
