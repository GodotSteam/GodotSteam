# Signals - Game Search

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## end_game_result

!!! function "end_game_result"
	This callback confirms that the game is recorded as complete on the matchmaking service, the next call to [requestPlayersForGame](/functions/game_search/#requestplayersforgame) will generate a new unique game ID.

	**Returns:**

	* result (int)
	* game_id (uint64_t)

## request_players_for_game_final_result

!!! function "request_players_for_game_final_result"
	There are no notes about this in Valve's header files or documentation.

	**Returns:**

	* result (int)
	* search_id (uint64_t)
	* game_id (uint64_t)

## request_players_for_game_progress

!!! function "request_players_for_game_progress"
	Callback from [requestPlayersForGame](/functions/game_search/#requestplayersforgame) when the matchmaking service has started or ended search; callback will also follow a call from [cancelRequestPlayersForGame](/functions/game_search/#cancelrequestplayersforgame) - search_in_progress will be false.

	**Returns:**

	* result (int)
	* search_id (uint64_t)

## request_players_for_game_result

!!! function "request_players_for_game_result"
	Callback from [requestPlayersForGame](), one of these will be sent per player followed by additional callbacks when players accept or decline the game.

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

## search_for_game_progress

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

## search_for_game_result

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

## submit_player_result

!!! function "submit_player_result"
	This callback confirms that results were received by the matchmaking service for this player.

	**Returns:**

	* result (int)
	* game_id (uint64_t)
	* player_id (uint64_t)