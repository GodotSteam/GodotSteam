# Functions - User Stats

---

## attachLeaderboardUGC

!!! function "attachLeaderboardUGC( ```uint64_t``` ugc_handle, ```uint64_t``` this_leaderboard )"
	If no this_leaderboard is passed, then the function will use the last internally-stored handle.

	Attaches a piece of user generated content the current user's entry on a leaderboard.

	This content could be a replay of the user achieving the score or a ghost to race against. The attached handle will be available when the entry is retrieved and can be accessed by other users using [getDownloadedLeaderboardEntry](/functions/user_stats/#getdownloadedleaderboardentry). To create and download user generated content see the documentation for the Steam Workshop.

	Once attached, the content will be available even if the underlying Cloud file is changed or deleted by the user.

	You must call [findLeaderboard](/functions/user_stats/#findleaderboard) or [findOrCreateLeaderboard](/functions/user_stats/#findorcreateleaderboard) to get a leaderboard handle prior to calling this function.

	Triggers a [leaderboard_ugc_set](/signals/user_stats/#leaderboard_ugc_set) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#AttachLeaderboardUGC){ .md-button .md-button--store target="_blank" }

## clearAchievement

!!! function "clearAchievement( ```string``` name )"
	Resets the unlock status of an achievement.

	This is primarily only ever used for testing.
	
	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	This call only modifies Steam's in-memory state so it is quite cheap. To send the unlock status to the server and to trigger the Steam overlay notification you must call [storeStats](/functions/user_stats/#storestats).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#ClearAchievement){ .md-button .md-button--store target="_blank" }

## downloadLeaderboardEntries

!!! function "downloadLeaderboardEntries( ```int``` start, ```int``` end, ```int``` type, ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Fetches a series of leaderboard entries for a specified leaderboard.

	You can ask for more entries than exist, then this will return as many as do exist.

	If you want to download entries for an arbitrary set of users, such as all of the users on a server then you can use [downloadLeaderboardEntriesForUsers](/functions/user_stats/#downloadleaderboardentriesforusers) which takes an array of Steam IDs.

	You must call [findLeaderboard](/functions/user_stats/#findleaderboard) or [findOrCreateLeaderboard](/functions/user_stats/#findorcreateleaderboard) to get a leaderboard handle prior to calling this function.

	Triggers a [leaderboard_scores_downloaded](/signals/user_stats/#leaderboard_scores_downloaded) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#DownloadLeaderboardEntriesForUsers){ .md-button .md-button--store target="_blank" }
	

## downloadLeaderboardEntriesForUsers

!!! function "downloadLeaderboardEntriesForUsers( ```array``` users_id, ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Fetches leaderboard entries for an arbitrary set of users on a specified leaderboard.

	A maximum of 100 users can be downloaded at a time, with only one outstanding call at a time. If a user doesn't have an entry on the specified leaderboard, they won't be included in the result.

	If you want to download entries based on their ranking or friends of the current user then you should use [downloadLeaderboardEntries](/functions/user_stats/#downloadleaderboardentries).

	You must call [findLeaderboard](/functions/user_stats/#findleaderboard) or [findOrCreateLeaderboard](/functions/user_stats/#findorcreateleaderboard) to get a leaderboard handle prior to calling this function.

	Triggers a [leaderboard_scores_downloaded](/signals/user_stats/#leaderboard_scores_downloaded) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#DownloadLeaderboardEntries){ .md-button .md-button--store target="_blank" }

## findLeaderboard

!!! function "findLeaderboard( ```string``` name )"
	Gets a leaderboard by name.

	You must call either this or [findOrCreateLeaderboard](/functions/user_stats/#findorcreateleaderboard) to obtain the leaderboard handle which is valid for the game session for each leaderboard you wish to access prior to calling any other Leaderboard functions.
	
	Triggers a [leaderboard_find_result](/signals/user_stats/#leaderboard_find_result) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#FindLeaderboard){ .md-button .md-button--store target="_blank" }

## findOrCreateLeaderboard

!!! function "findOrCreateLeaderboard( ```string``` name, ```int``` sort_method, ```int``` display_type )"
	Gets a leaderboard by name, it will create it if it's not yet created.

	You must call either this or [findLeaderboard](/functions/user_stats/#findleaderboard) to obtain the leaderboard handle which is valid for the game session for each leaderboard you wish to access prior to calling any other Leaderboard functions.

	Leaderboards created with this function will not automatically show up in the Steam Community. You must manually set the Community Name field in the App Admin panel of the Steamworks website. As such it's generally recommended to prefer creating the leaderboards in the App Admin panel on the Steamworks website and using [findLeaderboard](/functions/user_stats/#findleaderboard) unless you're expected to have a large amount of dynamically created leaderboards.

	You should never pass 0 for sort_method or 0 for display_type as this is undefined behavior.
	
	Triggers a [leaderboard_find_result](/signals/user_stats/#leaderboard_find_result) callback.
	
	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#FindOrCreateLeaderboard){ .md-button .md-button--store target="_blank" }

## getAchievement

!!! function "getAchievement( ```string``` name )"
	Gets the unlock status of the Achievement.

	The equivalent function for other users is [getUserAchievement](/functions/user_stats/#getuserachievement).

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* achieved (bool)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievement){ .md-button .md-button--store target="_blank" }

## getAchievementAchievedPercent

!!! function "getAchievementAchievedPercent( ```string``` name )"
	Returns the percentage of users who have unlocked the specified achievement.

	You must have called [requestGlobalAchievementPercentages](/functions/user_stats/#requestglobalachievementpercentages) and it needs to return successfully via its callback prior to calling this.

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* percent (float)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementAchievedPercent){ .md-button .md-button--store target="_blank" }

## getAchievementAndUnlockTime

!!! function "getAchievementAndUnlockTime( ```string``` name )"
	Gets the achievement status, and the time it was unlocked if unlocked.

	If the return value is true, but the unlock time is zero, that means it was unlocked before Steam began tracking achievement unlock times (December 2009). The time is provided in Unix epoch format, seconds since January 1, 1970 UTC.

	The equivalent function for other users is [getUserAchievementAndUnlockTime](/functions/user_stats/#getuserachievementandunlocktime).

	**Returns:** dictionary

	Contains the following keys:

	* retrieve (bool)
	* achieved (bool)
	* unlocked (uint32)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementAndUnlockTime){ .md-button .md-button--store target="_blank" }

## getAchievementDisplayAttribute

!!! function "getAchievementDisplayAttribute( ```string``` name, ```string``` key )"
	Get general attributes for an achievement. Currently provides: Name, Description, and Hidden status.

	This receives the value from a dictionary/map keyvalue store, so you must provide one of the following keys:

	* name - to retrive the localized achievement name in UTF8
	* desc - to retrive the localized achievement description in UTF8
	* hidden - for retrieving if an achievement is hidden. Returns "0" when not hidden, "1" when hidden

	This localization is provided based on the games language if it's set, otherwise it checks if a localization is avilable for the users Steam UI Language. If that fails too, then it falls back to english.

	**Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementDisplayAttribute){ .md-button .md-button--store target="_blank" }

## getAchievementIcon

!!! function "getAchievementIcon( ```string``` name )"
	Gets the icon for an achievement.

	Triggers a [user_achievement_icon_fetched](/signals/user_stats/#user_achievement_icon_fetched) callback.

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementIcon){ .md-button .md-button--store target="_blank" }

## getAchievementName

!!! function "getAchievementName( ```uint32_t``` achievement )"
	Gets the 'API name' for an achievement index between 0 and [getNumAchievements](/functions/user_stats/#getnumachievements).

	This function must be used in cojunction with [getNumAchievements](/functions/user_stats/#getnumachievements) to loop over the list of achievements.

	In general games should not need these functions as they should have the list of achievements compiled into them.

	**Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementName){ .md-button .md-button--store target="_blank" }

## getAchievementProgressLimitsFloat

!!! function "getAchievementProgressLimitsFloat( ```string``` name )"
	For achievements that have related progress stats, use this to query what the bounds of that progress are. You may want this info to selectively call [indicateAchievementProgress](/functions/user_stats/#indicateachievementprogress) when appropriate milestones of progress have been made, to show a progress notification to the user.

	**Returns:** dictionary

	Contains the following keys:

	* name (string)
	* min (float)
	* max (float)

## getAchievementProgressLimitsInt

!!! function "getAchievementProgressLimitsInt( ```string``` name )"
	For achievements that have related progress stats, use this to query what the bounds of that progress are. You may want this info to selectively call [indicateAchievementProgress](/functions/user_stats/#indicateachievementprogress) when appropriate milestones of progress have been made, to show a progress notification to the user.

	**Returns:** dictionary

	Contains the following keys:

	* name (string)
	* min (int32)
	* max (int32)

## getGlobalStatFloat

!!! function "getGlobalStatFloat( ```string``` name )"
	Gets the lifetime totals for an aggregated stat.

	You must have called [requestGlobalStats](/functions/user_stats/#requestglobalstats) and it needs to return successfully via its callback prior to calling this.

	**Returns:** float

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStat){ .md-button .md-button--store target="_blank" }

## getGlobalStatInt

!!! function "getGlobalStatInt( ```string``` name )"
	Gets the lifetime totals for an aggregated stat.

	You must have called [requestGlobalStats](/functions/user_stats/#) and it needs to return successfully via its callback prior to calling this.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStat){ .md-button .md-button--store target="_blank" }

## getGlobalStatFloatHistory

!!! function "getGlobalStatFloatHistory( ```string``` name )"
	Gets the daily history for an aggregated stat.

	**Returns:** float

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStatHistory){ .md-button .md-button--store target="_blank" }

## getGlobalStatIntHistory

!!! function "getGlobalStatIntHistory( ```string``` name )"
	Gets the daily history for an aggregated stat.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStatHistory){ .md-button .md-button--store target="_blank" }	

## getLeaderboardDisplayType

!!! function "getLeaderboardDisplayType( ```uint64_t``` leaderboard_handle )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Get the display type of a leaderboard handle.

	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* verbal (string)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardDisplayType){ .md-button .md-button--store target="_blank" }

## getLeaderboardEntries

!!! function "getLeaderboardEntries()"
	Get the currently used leaderboard entries.

	**Returns:** array

	**Note:** This is a GodotSteam specific function.

## getLeaderboardEntryCount

!!! function "getLeaderboardEntryCount( ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Returns the total number of entries in a leaderboard.

	This is cached on a per leaderboard basis upon the first call to [findLeaderboard](/functions/user_stats/#findleaderboard) or [findOrCreateLeaderboard](/functions/user_stats/#findorcreateleaderboard) and is refreshed on each successful call to [downloadLeaderboardEntries](/functions/user_stats/#downloadleaderboardentries), [downloadLeaderboardEntriesForUsers](/functions/user_stats/#downloadleaderboardentriesforusers), and [uploadLeaderboardScore](/functions/user_stats/#uploadleaderboardscore).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardEntryCount){ .md-button .md-button--store target="_blank" }

## getLeaderboardName

!!! function "getLeaderboardName( ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Returns the name of a leaderboard handle.

	**Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardName){ .md-button .md-button--store target="_blank" }

## getLeaderboardSortMethod

!!! function "getLeaderboardSortMethod( ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Get the sort order of a leaderboard handle. If no thisLeaderboard handle is passed, then the function will use the last stored internal handle.

	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* verbal (string)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardSortMethod){ .md-button .md-button--store target="_blank" }

## getMostAchievedAchievementInfo

!!! function "getMostAchievedAchievementInfo()"
	Gets the info on the most achieved achievement for the game.

	You must have called [requestGlobalAchievementPercentages](/functions/user_stats/#requestglobalachievementpercentages) and it needs to return successfully via its callback prior to calling this.

	**Returns:** array

	* entry (dictionary)
		* rank (int)
		* name (string)
		* percent (float)
		* achieved (bool)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetMostAchievedAchievementInfo){ .md-button .md-button--store target="_blank" }

## getNextMostAchievedAchievementInfo

!!! function "getNextMostAchievedAchievementInfo( ```int``` iterator )"
	Gets the info on the next most achieved achievement for the game.

	You must have called [requestGlobalAchievementPercentages](/functions/user_stats/#requestglobalachievementpercentages) and it needs to return successfully via its callback prior to calling this.

	**Returns:** dictionary

	Contains the following keys:

	* rank (int)
	* name (string)
	* percent (float)
	* achieved (bool)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetNextMostAchievedAchievementInfo){ .md-button .md-button--store target="_blank" }

## getNumAchievements

!!! function "getNumAchievements()"
	Get the number of achievements defined in the App Admin panel of the Steamworks website.

	This is used for iterating through all of the achievements with [getAchievementName](/functions/user_stats/#getachievementname).

	In general games should not need these functions because they should have a list of existing achievements compiled into them.

	**Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetNumAchievements){ .md-button .md-button--store target="_blank" }

## getNumberOfCurrentPlayers

!!! function "getNumberOfCurrentPlayers()"
	Asynchronously retrieves the total number of players currently playing the current game. Both online and in offline mode.

	Triggers a [number_of_current_players](/signals/user_stats/#number_of_current_players) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetNumberOfCurrentPlayers){ .md-button .md-button--store target="_blank" }

## getStatFloat

!!! function "getStatFloat( ```string``` name )"
	Gets the current value of the a stat for the current user.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	To receive stats for other users use [getUserStat](/functions/user_stats/#getuserstat).

	**Returns:** float

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat){ .md-button .md-button--store target="_blank" }

## getStatInt

!!! function "getStatInt( ```string``` name )"
	Gets the current value of the a stat for the current user.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	To receive stats for other users use [getUserStat](/functions/user_stats/#getuserstat).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat){ .md-button .md-button--store target="_blank" }


## getUserAchievement

!!! function "getUserAchievement( ```uint64_t``` steam_id, ```string``` name )"
	Gets the unlock status of the Achievement.

	The equivalent function for the local user is [getAchievement](/functions/user_stats/#getachievement).

	**Returns:** dictionary

	Contains the following keys:

	* steam_id (uint64_t)
	* retrieved (bool)
	* name (string)
	* achieved (bool)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserAchievement){ .md-button .md-button--store target="_blank" }

## getUserAchievementAndUnlockTime

!!! function "getUserAchievementAndUnlockTime( ```uint64_t``` steam_id, ```string``` name )"
	Gets the achievement status, and the time it was unlocked if unlocked.

	If the return value is true, but the unlock time is zero, that means it was unlocked before Steam began tracking achievement unlock times (December 2009). The time is provided in Unix epoch format, seconds since January 1, 1970 UTC.

	The equivalent function for the local user is [getAchievementAndUnlockTime](/functions/user_stats/#getachievementandunlocktime).

	**Returns:** dictionary

	Contains the following keys:

	* retrieved (bool)
	* name (string)
	* function "achieved (bool)
	* unlocked (uint32)	

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserAchievementAndUnlockTime){ .md-button .md-button--store target="_blank" }

## getUserStatFloat

!!! function "getUserStatFloat( ```uint64_t``` steam_id, ```string``` name )"
	Gets the current value of the a stat for the specified user.

	You must have called [requestUserStats](/functions/user_stats/#requestuserstats) and it needs to return successfully via its callback prior to calling this.

	The equivalent function for the local user is [getStat](/functions/user_stats/#getstat).

	**Returns:** float.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserStat){ .md-button .md-button--store target="_blank" }

## getUserStatInt

!!! function "getUserStatInt( ```uint64_t``` steam_id, ```string``` name )"
	Gets the current value of the a stat for the specified user.

	You must have called [requestUserStats](/functions/user_stats/#) and it needs to return successfully via its callback prior to calling this.

	The equivalent function for the local user is [getStat](/functions/user_stats/#getstat).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserStat){ .md-button .md-button--store target="_blank" }

## indicateAchievementProgress

!!! function "indicateAchievementProgress( ```string``` name, ```int``` currentProgress, ```int``` maxProgress )"
	Shows the user a pop-up notification with the current progress of an achievement.

	Calling this function will not] set the progress or unlock the achievement, the game must do that manually by calling [setStat](/functions/user_stats/#setstats).

	Triggers a [user_stats_stored](/signals/user_stats/#user_stats_stored) callback.

	Triggers a [user_achievement_stored](/signals/user_stats/#user_achievement_stored) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#IndicateAchievementProgress){ .md-button .md-button--store target="_blank" }

## requestCurrentStats

!!! function "requestCurrentStats()"
	Asynchronously request the user's current stats and achievements from the server.

	You must always call this first to get the initial status of stats and achievements. Only after the resulting callback comes back can you start calling the rest of the stats and achievement functions for the current user.

	The equivalent function for other users is [requestUserStats](/functions/user_stats/#requestuserstats).

	Triggers a [current_stats_received](/signals/user_stats/#current_stats_received) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestCurrentStats){ .md-button .md-button--store target="_blank" }

## requestGlobalAchievementPercentages

!!! function "requestGlobalAchievementPercentages()"
	Asynchronously fetches global stats data, which is available for stats marked as "aggregated" in the App Admin panel of the Steamworks website.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	Triggers a [global_achievement_percentages_ready](/signals/user_stats/#global_achievement_percentage_ready) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestGlobalAchievementPercentages){ .md-button .md-button--store target="_blank" }

## requestGlobalStats

!!! function "requestGlobalStats( ```int``` history_days )"
	Asynchronously fetches global stats data, which is available for stats marked as "aggregated" in the App Admin panel of the Steamworks website.

	The limit is 60.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	Triggers a [global_stats_received](/signals/user_stats/#global_stats_received) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestGlobalStats){ .md-button .md-button--store target="_blank" }

## requestUserStats

!!! function "requestUserStats( ```uint64_t``` steam_id )"
	Asynchronously downloads stats and achievements for the specified user from the server.

	These stats are not automatically updated; you'll need to call this function again to refresh any data that may have change.
	To keep from using too much memory, an least recently used cache (LRU) is maintained and other user's stats will occasionally be unloaded. When this happens a [user_stats_unloaded](/signals/user_stats/#user_stats_unloaded) callback is sent. After receiving this callback the user's stats will be unavailable until this function is called again.

	The equivalent function for the local user is [requestCurrentStats](/functions/user_stats/#requestcurrentstats).

	Triggers a [user_stats_received](/signals/user_stats/#user_stats_received) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestUserStats){ .md-button .md-button--store target="_blank" }

## resetAllStats

!!! function "resetAllStats( ```bool``` achievements_too )"
	Resets the current users stats and, optionally achievements.

	This automatically calls [storeStats](/functions/user_stats/#storestats) to persist the changes to the server. This should typically only be used for testing purposes during development. Ensure that you sync up your stats with the new default values provided by Steam after calling this by calling [requestCurrentStats](/functions/user_stats/#requestcurrentstats).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#ResetAllStats){ .md-button .md-button--store target="_blank" }

## setAchievement

!!! function "setAchievement( ```string``` name )"
	Unlocks an achievement.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	You can unlock an achievement multiple times so you don't need to worry about only setting achievements that aren't already set. This call only modifies Steam's in-memory state so it is quite cheap. To send the unlock status to the server and to trigger the Steam overlay notification you must call [storeStats](/functions/user_stats/#storestats).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#SetAchievement){ .md-button .md-button--store target="_blank" }

## setLeaderboardDetailsMax

!!! function "setLeaderboardDetailsMax( ```int``` max )"
	Set the maximum number of details to return for leaderboard entries.

	**Returns:** int

	The current leaderboard details max value.

	**Note:** This is a GodotSteam specific function.

## setStatFloat

!!! function "setStatFloat( ```string``` name, ```float``` value )"
	Sets / updates the float value of a given stat for the current user.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call [storeStats](/functions/user_stats/#storestats).

	If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat){ .md-button .md-button--store target="_blank" }

## setStatInt

!!! function "setStatInt( ```string``` name, ```int32``` value )"
	Sets / updates the integer value of a given stat for the current user.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call [storeStats](/functions/user_stats/#storestats).

	If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat){ .md-button .md-button--store target="_blank" }

## storeStats

!!! function "storeStats()"
	Send the changed stats and achievements data to the server for permanent storage.

	If this fails then nothing is sent to the server. It's advisable to keep trying until the call is successful.

	This call can be rate limited. Call frequency should be on the order of minutes, rather than seconds. You should only be calling this during major state changes such as the end of a round, the map changing, or the user leaving a server. This call is required to display the achievement unlock notification dialog though, so if you have called [setAchievement](/functions/user_stats/#setachievement) then it's advisable to call this soon after that.

	If you have stats or achievements that you have saved locally but haven't uploaded with this function when your application process ends then this function will automatically be called.

	You can find additional debug information written to the ```%steam_install%\logs\stats_log.txt``` file.

	Triggers a [user_stats_stored](/signals/user_stats/#user_stats_stored) callback.

	Triggers a [user_achievement_stored](/signals/user_stats/#user_achievement_stored) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#StoreStats){ .md-button .md-button--store target="_blank" }

## updateAvgRateStat

!!! function "updateAvgRateStat( ```string``` name, ```float``` this_session, ```double``` session_length )"
	Updates an AVGRATE stat with new values.

	You must have called [requestCurrentStats](/functions/user_stats/#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call [storeStats](/functions/user_stats/#storestats).

	If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.

	**Returns:** bool
    
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UpdateAvgRateStat){ .md-button .md-button--store target="_blank" }

## uploadLeaderboardScore

!!! function "uploadLeaderboardScore( ```int32``` score, ```bool``` keep_best, ```array``` details, ```uint64_t``` this_leaderboard )"
	If this_leaderboard is not passed, then the function will use the last internally-stored handle.

	Uploads a user score to a specified leaderboard.
	
	Details are optional game-defined information which outlines how the user got that score. For example if it's a racing style time based leaderboard you could store the timestamps when the player hits each checkpoint. If you have collectibles along the way you could use bit fields as booleans to store the items the player picked up in the playthrough.

	Uploading scores to Steam is rate limited to 10 uploads per 10 minutes and you may only have one outstanding call to this function at a time.
	
	Triggers a [leaderboard_score_uploaded](/signals/user_stats/#leaderboard_score_uploaded) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore){ .md-button .md-button--store target="_blank" }