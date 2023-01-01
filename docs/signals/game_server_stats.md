# Signals - Game Server Stats

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## stat_received

!!! function "stat_received"
	Result when getting the latests stats and achievements for a user from the server.

	Emits signal in response to function [serverRequestUserStats](/functions/game_server_stats/#serverrequestuserstats).

	**Returns:**

	* result (int)
	* steam_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#GSStatsReceived_t){ .md-button .md-button--store target="_blank" }

## stats_stored

!!! function "stats_stored"
	Result of a request to store the user stats.

	Emits signal in response to function [storeUserStats](/functions/game_server_stats/#storeuserstats).

	**Returns:**

	* result (int)
	* steam_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#GSStatsStored_t){ .md-button .md-button--store target="_blank" }

## stats_unloaded

!!! function "stats_unloaded"
	Callback indicating that a user's stats have been unloaded.

	**Returns:**

	* steam_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#GSStatsUnloaded_t){ .md-button .md-button--store target="_blank" }