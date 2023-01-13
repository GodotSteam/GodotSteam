# Signals - User Stats

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## current_stats_received

!!! function "current_stats_received"
	Called when the latest stats and achievements for the local user have been received from the server; in response to function [requestCurrentStats](/functions/user_stats/#requestcurrentstats).

	**Returns:**

	* game (uint64_t)
	* result (uint32_t)
	* user (uint64_t) as Steam ID

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsReceived_t){ .md-button .md-button--store target="_blank" }

## global_achievement_percentages_ready

!!! function "global_achievement_percentages_ready"
	Called when the global achievement percentages have been received from the server.

	**Returns:**

	* game (uint64_t)
	* result (uint32_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GlobalAchievementPercentagesReady_t){ .md-button .md-button--store target="_blank" }

## global_stats_received

!!! function "global_stats_received"
	Called when the global stats have been received from the server.

	**Returns:**

	* game_id (uint64_t)
	* result (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GlobalStatsReceived_t){ .md-button .md-button--store target="_blank" }

## leaderboard_find_result

!!! function "leaderboard_find_result"
	Result when finding a leaderboard.

	**Returns:**

	* leaderboard_handle (uint64_t)
	* found (uint8_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardFindResult_t){ .md-button .md-button--store target="_blank" }

## leaderboard_scores_downloaded

!!! function "leaderboard_scores_downloaded"
	Called when scores for a leaderboard have been downloaded and are ready to be retrieved.

	**Returns:**

	* message (string)
		* this_handle (uint64_t)
		* leaderboard_entries_array (array)
			* entry_dict (dictionary)
				* score (int)
				* steam_id (uint64_t)
				* global_rank (int)
				* ugc_handle (uint64_t)
				* details (array)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardScoresDownloaded_t){ .md-button .md-button--store target="_blank" }

## leaderboard_score_uploaded

!!! function "leaderboard_score_uploaded"
	Result indicating that a leaderboard score has been uploaded.

	**Returns:**

	* success (uint8)
	* this_handle (uint64_t)
	* score (int32)
	* score_changed (uint8)
	* global_rank_new (int)
	* global_rank_prev (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardScoreUploaded_t){ .md-button .md-button--store target="_blank" }

## leaderboard_ugc_set

!!! function "leaderboard_ugc_set"
	Result indicating that user generated content has been attached to one of the current user's leaderboard entries.

	**Returns:**

	* leaderboard_handle (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardUGCSet_t){ .md-button .md-button--store target="_blank" }

## number_of_current_players

!!! function "number_of_current_players"
	Gets the current number of players for the current app ID.

	**Returns:**

	* success (uint8)
	* players (int32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#NumberOfCurrentPlayers_t){ .md-button .md-button--store target="_blank" }

## user_achievement_stored

!!! function "user_achievement_stored"
	Result of a request to store the achievements on the server, or an "indicate progress" call. If both current progress and max progress are zero, that means the achievement has been fully unlocked.

	**Returns:**

	* game (uint64_t)
	* group_achieve (bool)
	* name (string)
	* current_progress (uint32_t)
	* max_progress (uint32_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserAchievementStored_t){ .md-button .md-button--store target="_blank" }

## user_stats_received

!!! function "user_stats_received"
	Called when the latest stats and achievements for a specific user (including the local user) have been received from the server.

	**Returns:**

	* game (uint64_t)
	* result (uint32_t)
	* user (uint64_t) as Steam ID

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsReceived_t){ .md-button .md-button--store target="_blank" }

## user_stats_stored

!!! function "user_stats_stored"
	Result of a request to store the user stats.
	
	**Returns:**

	* game (uint64_t)
	* result (uint32_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsStored_t){ .md-button .md-button--store target="_blank" }

## user_stats_unloaded

!!! function "user_stats_unloaded"
	Callback indicating that a user's stats have been unloaded. Call [requestUserStats](/functions/user_stats/#requestuserstats) again before accessing stats for this user.
	
	**Returns:**

	* user (uint64_t) as Steam ID

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsUnloaded_t){ .md-button .md-button--store target="_blank" }
