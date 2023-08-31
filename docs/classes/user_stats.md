# User Stats

Provides functions for accessing and submitting stats, achievements, and leaderboards.

---

## Functions

---

### attachLeaderboardUGC

!!! function "attachLeaderboardUGC( ```uint64_t``` ugc_handle, ```uint64_t``` this_leaderboard )"
	If no this_leaderboard is passed, then the function will use the last internally-stored handle.

	Attaches a piece of user generated content the current user's entry on a leaderboard.

	This content could be a replay of the user achieving the score or a ghost to race against. The attached handle will be available when the entry is retrieved and can be accessed by other users using [getDownloadedLeaderboardEntry](#getdownloadedleaderboardentry). To create and download user generated content see the documentation for the Steam Workshop.

	Once attached, the content will be available even if the underlying Cloud file is changed or deleted by the user.

	You must call [findLeaderboard](#findleaderboard) or [findOrCreateLeaderboard](#findorcreateleaderboard) to get a leaderboard handle prior to calling this function.

	Triggers a [leaderboard_ugc_set](#leaderboard_ugc_set) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#AttachLeaderboardUGC){ .md-button .md-button--store target="_blank" }

### clearAchievement

!!! function "clearAchievement( ```string``` name )"
	Resets the unlock status of an achievement.

	This is primarily only ever used for testing.
	
	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	This call only modifies Steam's in-memory state so it is quite cheap. To send the unlock status to the server and to trigger the Steam overlay notification you must call [storeStats](#storestats).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#ClearAchievement){ .md-button .md-button--store target="_blank" }

### downloadLeaderboardEntries

!!! function "downloadLeaderboardEntries( ```int``` start, ```int``` end, ```int``` type, ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Fetches a series of leaderboard entries for a specified leaderboard.

	You can ask for more entries than exist, then this will return as many as do exist.

	If you want to download entries for an arbitrary set of users, such as all of the users on a server then you can use [downloadLeaderboardEntriesForUsers](#downloadleaderboardentriesforusers) which takes an array of Steam IDs.

	You must call [findLeaderboard](#findleaderboard) or [findOrCreateLeaderboard](#findorcreateleaderboard) to get a leaderboard handle prior to calling this function.

	Triggers a [leaderboard_scores_downloaded](#leaderboard_scores_downloaded) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#DownloadLeaderboardEntriesForUsers){ .md-button .md-button--store target="_blank" }
	

### downloadLeaderboardEntriesForUsers

!!! function "downloadLeaderboardEntriesForUsers( ```array``` users_id, ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Fetches leaderboard entries for an arbitrary set of users on a specified leaderboard.

	A maximum of 100 users can be downloaded at a time, with only one outstanding call at a time. If a user doesn't have an entry on the specified leaderboard, they won't be included in the result.

	If you want to download entries based on their ranking or friends of the current user then you should use [downloadLeaderboardEntries](#downloadleaderboardentries).

	You must call [findLeaderboard](#findleaderboard) or [findOrCreateLeaderboard](#findorcreateleaderboard) to get a leaderboard handle prior to calling this function.

	Triggers a [leaderboard_scores_downloaded](#leaderboard_scores_downloaded) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#DownloadLeaderboardEntries){ .md-button .md-button--store target="_blank" }

### findLeaderboard

!!! function "findLeaderboard( ```string``` name )"
	Gets a leaderboard by name.

	You must call either this or [findOrCreateLeaderboard](#findorcreateleaderboard) to obtain the leaderboard handle which is valid for the game session for each leaderboard you wish to access prior to calling any other Leaderboard functions.
	
	Triggers a [leaderboard_find_result](#leaderboard_find_result) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#FindLeaderboard){ .md-button .md-button--store target="_blank" }

### findOrCreateLeaderboard

!!! function "findOrCreateLeaderboard( ```string``` name, ```int``` sort_method, ```int``` display_type )"
	Gets a leaderboard by name, it will create it if it's not yet created.

	You must call either this or [findLeaderboard](#findleaderboard) to obtain the leaderboard handle which is valid for the game session for each leaderboard you wish to access prior to calling any other Leaderboard functions.

	Leaderboards created with this function will not automatically show up in the Steam Community. You must manually set the Community Name field in the App Admin panel of the Steamworks website. As such it's generally recommended to prefer creating the leaderboards in the App Admin panel on the Steamworks website and using [findLeaderboard](#findleaderboard) unless you're expected to have a large amount of dynamically created leaderboards.

	You should never pass 0 for sort_method or 0 for display_type as this is undefined behavior.
	
	Triggers a [leaderboard_find_result](#leaderboard_find_result) callback.
	
	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#FindOrCreateLeaderboard){ .md-button .md-button--store target="_blank" }

### getAchievement

!!! function "getAchievement( ```string``` name )"
	Gets the unlock status of the Achievement.

	The equivalent function for other users is [getUserAchievement](#getuserachievement).

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* achieved (bool)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievement){ .md-button .md-button--store target="_blank" }

### getAchievementAchievedPercent

!!! function "getAchievementAchievedPercent( ```string``` name )"
	Returns the percentage of users who have unlocked the specified achievement.

	You must have called [requestGlobalAchievementPercentages](#requestglobalachievementpercentages) and it needs to return successfully via its callback prior to calling this.

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* percent (float)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementAchievedPercent){ .md-button .md-button--store target="_blank" }

### getAchievementAndUnlockTime

!!! function "getAchievementAndUnlockTime( ```string``` name )"
	Gets the achievement status, and the time it was unlocked if unlocked.

	If the return value is true, but the unlock time is zero, that means it was unlocked before Steam began tracking achievement unlock times (December 2009). The time is provided in Unix epoch format, seconds since January 1, 1970 UTC.

	The equivalent function for other users is [getUserAchievementAndUnlockTime](#getuserachievementandunlocktime).

	**Returns:** dictionary

	Contains the following keys:

	* retrieve (bool)
	* achieved (bool)
	* unlocked (uint32)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementAndUnlockTime){ .md-button .md-button--store target="_blank" }

### getAchievementDisplayAttribute

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

### getAchievementIcon

!!! function "getAchievementIcon( ```string``` name )"
	Gets the icon for an achievement.

	The image is returned as a handle to be used with [getImageRGBA](utils.md#getimagergba) to get the actual image data.

	**Returns:** int

	See the [Achievement Icons tutorial](../tutorials/achievement_icons.md) for an example.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementIcon){ .md-button .md-button--store target="_blank" }

### getAchievementName

!!! function "getAchievementName( ```uint32_t``` achievement )"
	Gets the 'API name' for an achievement index between 0 and [getNumAchievements](#getnumachievements).

	This function must be used in conjunction with [getNumAchievements](#getnumachievements) to loop over the list of achievements.

	In general games should not need these functions as they should have the list of achievements compiled into them.

	**Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementName){ .md-button .md-button--store target="_blank" }

### getAchievementProgressLimitsFloat

!!! function "getAchievementProgressLimitsFloat( ```string``` name )"
	For achievements that have related progress stats, use this to query what the bounds of that progress are. You may want this info to selectively call [indicateAchievementProgress](#indicateachievementprogress) when appropriate milestones of progress have been made, to show a progress notification to the user.

	**Returns:** dictionary

	Contains the following keys:

	* name (string)
	* min (float)
	* max (float)

### getAchievementProgressLimitsInt

!!! function "getAchievementProgressLimitsInt( ```string``` name )"
	For achievements that have related progress stats, use this to query what the bounds of that progress are. You may want this info to selectively call [indicateAchievementProgress](#indicateachievementprogress) when appropriate milestones of progress have been made, to show a progress notification to the user.

	**Returns:** dictionary

	Contains the following keys:

	* name (string)
	* min (int32)
	* max (int32)

### getGlobalStatFloat

!!! function "getGlobalStatFloat( ```string``` name )"
	Gets the lifetime totals for an aggregated stat.

	You must have called [requestGlobalStats](#requestglobalstats) and it needs to return successfully via its callback prior to calling this.

	**Returns:** float

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStat){ .md-button .md-button--store target="_blank" }

### getGlobalStatInt

!!! function "getGlobalStatInt( ```string``` name )"
	Gets the lifetime totals for an aggregated stat.

	You must have called [requestGlobalStats](#) and it needs to return successfully via its callback prior to calling this.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStat){ .md-button .md-button--store target="_blank" }

### getGlobalStatFloatHistory

!!! function "getGlobalStatFloatHistory( ```string``` name )"
	Gets the daily history for an aggregated stat.

	**Returns:** float

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStatHistory){ .md-button .md-button--store target="_blank" }

### getGlobalStatIntHistory

!!! function "getGlobalStatIntHistory( ```string``` name )"
	Gets the daily history for an aggregated stat.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetGlobalStatHistory){ .md-button .md-button--store target="_blank" }	

### getLeaderboardDisplayType

!!! function "getLeaderboardDisplayType( ```uint64_t``` leaderboard_handle )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Get the display type of a leaderboard handle.

	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* verbal (string)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardDisplayType){ .md-button .md-button--store target="_blank" }

### getLeaderboardEntries

!!! function "getLeaderboardEntries()"
	Get the currently used leaderboard entries.

	**Returns:** array

	**Note:** This is a GodotSteam specific function.

### getLeaderboardEntryCount

!!! function "getLeaderboardEntryCount( ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Returns the total number of entries in a leaderboard.

	This is cached on a per leaderboard basis upon the first call to [findLeaderboard](#findleaderboard) or [findOrCreateLeaderboard](#findorcreateleaderboard) and is refreshed on each successful call to [downloadLeaderboardEntries](#downloadleaderboardentries), [downloadLeaderboardEntriesForUsers](#downloadleaderboardentriesforusers), and [uploadLeaderboardScore](#uploadleaderboardscore).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardEntryCount){ .md-button .md-button--store target="_blank" }

### getLeaderboardName

!!! function "getLeaderboardName( ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Returns the name of a leaderboard handle.

	**Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardName){ .md-button .md-button--store target="_blank" }

### getLeaderboardSortMethod

!!! function "getLeaderboardSortMethod( ```uint64_t``` this_leaderboard )"
	If no leaderboard_handle is passed, then the function will use the last internally-stored handle.

	Get the sort order of a leaderboard handle. If no thisLeaderboard handle is passed, then the function will use the last stored internal handle.

	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* verbal (string)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardSortMethod){ .md-button .md-button--store target="_blank" }

### getMostAchievedAchievementInfo

!!! function "getMostAchievedAchievementInfo()"
	Gets the info on the most achieved achievement for the game.

	You must have called [requestGlobalAchievementPercentages](#requestglobalachievementpercentages) and it needs to return successfully via its callback prior to calling this.

	**Returns:** dictionary

	Contains the following keys:
	* rank (int)
	* name (string)
	* percent (float)
	* achieved (bool)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetMostAchievedAchievementInfo){ .md-button .md-button--store target="_blank" }

### getNextMostAchievedAchievementInfo

!!! function "getNextMostAchievedAchievementInfo( ```int``` iterator )"
	Gets the info on the next most achieved achievement for the game.

	You must have called [requestGlobalAchievementPercentages](#requestglobalachievementpercentages) and it needs to return successfully via its callback prior to calling this.

	**Returns:** dictionary

	Contains the following keys:

	* rank (int)
	* name (string)
	* percent (float)
	* achieved (bool)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetNextMostAchievedAchievementInfo){ .md-button .md-button--store target="_blank" }

### getNumAchievements

!!! function "getNumAchievements()"
	Get the number of achievements defined in the App Admin panel of the Steamworks website.

	This is used for iterating through all of the achievements with [getAchievementName](#getachievementname).

	In general games should not need these functions because they should have a list of existing achievements compiled into them.

	**Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetNumAchievements){ .md-button .md-button--store target="_blank" }

### getNumberOfCurrentPlayers

!!! function "getNumberOfCurrentPlayers()"
	Asynchronously retrieves the total number of players currently playing the current game. Both online and in offline mode.

	Triggers a [number_of_current_players](#number_of_current_players) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetNumberOfCurrentPlayers){ .md-button .md-button--store target="_blank" }

### getStatFloat

!!! function "getStatFloat( ```string``` name )"
	Gets the current value of the a stat for the current user.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	To receive stats for other users use [getUserStat](#getuserstat).

	**Returns:** float

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat){ .md-button .md-button--store target="_blank" }

### getStatInt

!!! function "getStatInt( ```string``` name )"
	Gets the current value of the a stat for the current user.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	To receive stats for other users use [getUserStat](#getuserstat).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat){ .md-button .md-button--store target="_blank" }


### getUserAchievement

!!! function "getUserAchievement( ```uint64_t``` steam_id, ```string``` name )"
	Gets the unlock status of the Achievement.

	The equivalent function for the local user is [getAchievement](#getachievement).

	**Returns:** dictionary

	Contains the following keys:

	* steam_id (uint64_t)
	* retrieved (bool)
	* name (string)
	* achieved (bool)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserAchievement){ .md-button .md-button--store target="_blank" }

### getUserAchievementAndUnlockTime

!!! function "getUserAchievementAndUnlockTime( ```uint64_t``` steam_id, ```string``` name )"
	Gets the achievement status, and the time it was unlocked if unlocked.

	If the return value is true, but the unlock time is zero, that means it was unlocked before Steam began tracking achievement unlock times (December 2009). The time is provided in Unix epoch format, seconds since January 1, 1970 UTC.

	The equivalent function for the local user is [getAchievementAndUnlockTime](#getachievementandunlocktime).

	**Returns:** dictionary

	Contains the following keys:

	* retrieved (bool)
	* name (string)
	* achieved (bool)
	* unlocked (uint32)	

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserAchievementAndUnlockTime){ .md-button .md-button--store target="_blank" }

### getUserStatFloat

!!! function "getUserStatFloat( ```uint64_t``` steam_id, ```string``` name )"
	Gets the current value of the a stat for the specified user.

	You must have called [requestUserStats](#requestuserstats) and it needs to return successfully via its callback prior to calling this.

	The equivalent function for the local user is [getStat](#getstat).

	**Returns:** float.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserStat){ .md-button .md-button--store target="_blank" }

### getUserStatInt

!!! function "getUserStatInt( ```uint64_t``` steam_id, ```string``` name )"
	Gets the current value of the a stat for the specified user.

	You must have called [requestUserStats](#requestuserstats) and it needs to return successfully via its callback prior to calling this.

	The equivalent function for the local user is [getStat](#getstat).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GetUserStat){ .md-button .md-button--store target="_blank" }

### indicateAchievementProgress

!!! function "indicateAchievementProgress( ```string``` name, ```int``` currentProgress, ```int``` maxProgress )"
	Shows the user a pop-up notification with the current progress of an achievement.

	Calling this function will not set the progress or unlock the achievement, the game must do that manually by calling [setStat](#setstats).

	Triggers a [user_stats_stored](#user_stats_stored) callback.

	Triggers a [user_achievement_stored](#user_achievement_stored) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#IndicateAchievementProgress){ .md-button .md-button--store target="_blank" }

### requestCurrentStats

!!! function "requestCurrentStats()"
	Asynchronously request the user's current stats and achievements from the server.

	You must always call this first to get the initial status of stats and achievements. Only after the resulting callback comes back can you start calling the rest of the stats and achievement functions for the current user.

	The equivalent function for other users is [requestUserStats](#requestuserstats).

	Triggers a [current_stats_received](#current_stats_received) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestCurrentStats){ .md-button .md-button--store target="_blank" }

### requestGlobalAchievementPercentages

!!! function "requestGlobalAchievementPercentages()"
	Asynchronously fetches global stats data, which is available for stats marked as "aggregated" in the App Admin panel of the Steamworks website.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	Triggers a [global_achievement_percentages_ready](#global_achievement_percentage_ready) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestGlobalAchievementPercentages){ .md-button .md-button--store target="_blank" }

### requestGlobalStats

!!! function "requestGlobalStats( ```int``` history_days )"
	Asynchronously fetches global stats data, which is available for stats marked as "aggregated" in the App Admin panel of the Steamworks website.

	The limit is 60.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this.

	Triggers a [global_stats_received](#global_stats_received) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestGlobalStats){ .md-button .md-button--store target="_blank" }

### requestUserStats

!!! function "requestUserStats( ```uint64_t``` steam_id )"
	Asynchronously downloads stats and achievements for the specified user from the server.

	These stats are not automatically updated; you'll need to call this function again to refresh any data that may have change.
	To keep from using too much memory, an least recently used cache (LRU) is maintained and other user's stats will occasionally be unloaded. When this happens a [user_stats_unloaded](#user_stats_unloaded) callback is sent. After receiving this callback the user's stats will be unavailable until this function is called again.

	The equivalent function for the local user is [requestCurrentStats](#requestcurrentstats).

	Triggers a [user_stats_received](#user_stats_received) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#RequestUserStats){ .md-button .md-button--store target="_blank" }

### resetAllStats

!!! function "resetAllStats( ```bool``` achievements_too )"
	Resets the current users stats and, optionally achievements.

	This automatically calls [storeStats](#storestats) to persist the changes to the server. This should typically only be used for testing purposes during development. Ensure that you sync up your stats with the new default values provided by Steam after calling this by calling [requestCurrentStats](#requestcurrentstats).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#ResetAllStats){ .md-button .md-button--store target="_blank" }

### setAchievement

!!! function "setAchievement( ```string``` name )"
	Unlocks an achievement.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	You can unlock an achievement multiple times so you don't need to worry about only setting achievements that aren't already set. This call only modifies Steam's in-memory state so it is quite cheap. To send the unlock status to the server and to trigger the Steam overlay notification you must call [storeStats](#storestats).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#SetAchievement){ .md-button .md-button--store target="_blank" }

### setLeaderboardDetailsMax

!!! function "setLeaderboardDetailsMax( ```int``` max )"
	Set the maximum number of details to return for leaderboard entries.

	**Returns:** int

	The current leaderboard details max value.

	**Note:** This is a GodotSteam specific function.

### setStatFloat

!!! function "setStatFloat( ```string``` name, ```float``` value )"
	Sets / updates the float value of a given stat for the current user.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call [storeStats](#storestats).

	If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat){ .md-button .md-button--store target="_blank" }

### setStatInt

!!! function "setStatInt( ```string``` name, ```int32``` value )"
	Sets / updates the integer value of a given stat for the current user.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call [storeStats](#storestats).

	If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat){ .md-button .md-button--store target="_blank" }

### storeStats

!!! function "storeStats()"
	Send the changed stats and achievements data to the server for permanent storage.

	If this fails then nothing is sent to the server. It's advisable to keep trying until the call is successful.

	This call can be rate limited. Call frequency should be on the order of minutes, rather than seconds. You should only be calling this during major state changes such as the end of a round, the map changing, or the user leaving a server. This call is required to display the achievement unlock notification dialog though, so if you have called [setAchievement](#setachievement) then it's advisable to call this soon after that.

	If you have stats or achievements that you have saved locally but haven't uploaded with this function when your application process ends then this function will automatically be called.

	You can find additional debug information written to the ```%steam_install%\logs\stats_log.txt``` file.

	Triggers a [user_stats_stored](#user_stats_stored) callback.

	Triggers a [user_achievement_stored](#user_achievement_stored) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#StoreStats){ .md-button .md-button--store target="_blank" }

### updateAvgRateStat

!!! function "updateAvgRateStat( ```string``` name, ```float``` this_session, ```double``` session_length )"
	Updates an AVGRATE stat with new values.

	You must have called [requestCurrentStats](#requestcurrentstats) and it needs to return successfully via its callback prior to calling this!

	This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call [storeStats](#storestats).

	If this is returning false and everything appears correct, then check to ensure that your changes in the App Admin panel of the Steamworks website are published.

	**Returns:** bool
    
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UpdateAvgRateStat){ .md-button .md-button--store target="_blank" }

### uploadLeaderboardScore

!!! function "uploadLeaderboardScore( ```int32``` score, ```bool``` keep_best, ```array``` details, ```uint64_t``` this_leaderboard )"
	If this_leaderboard is not passed, then the function will use the last internally-stored handle.

	Uploads a user score to a specified leaderboard.
	
	Details are optional game-defined information which outlines how the user got that score. For example if it's a racing style time based leaderboard you could store the timestamps when the player hits each checkpoint. If you have collectibles along the way you could use bit fields as booleans to store the items the player picked up in the playthrough.

	Uploading scores to Steam is rate limited to 10 uploads per 10 minutes and you may only have one outstanding call to this function at a time.
	
	Triggers a [leaderboard_score_uploaded](#leaderboard_score_uploaded) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore){ .md-button .md-button--store target="_blank" }

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### current_stats_received

!!! function "current_stats_received"
	Called when the latest stats and achievements for the local user have been received from the server; in response to function [requestCurrentStats](#requestcurrentstats).

	**Returns:**

	* game (uint64_t)
	* result (uint32_t)
	* user (uint64_t) as Steam ID

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsReceived_t){ .md-button .md-button--store target="_blank" }

### global_achievement_percentages_ready

!!! function "global_achievement_percentages_ready"
	Called when the global achievement percentages have been received from the server.

	**Returns:**

	* game (uint64_t)
	* result (uint32_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GlobalAchievementPercentagesReady_t){ .md-button .md-button--store target="_blank" }

### global_stats_received

!!! function "global_stats_received"
	Called when the global stats have been received from the server.

	**Returns:**

	* game_id (uint64_t)
	* result (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#GlobalStatsReceived_t){ .md-button .md-button--store target="_blank" }

### leaderboard_find_result

!!! function "leaderboard_find_result"
	Result when finding a leaderboard.

	**Returns:**

	* leaderboard_handle (uint64_t)
	* found (uint8_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardFindResult_t){ .md-button .md-button--store target="_blank" }

### leaderboard_scores_downloaded

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

### leaderboard_score_uploaded

!!! function "leaderboard_score_uploaded"
	Result indicating that a leaderboard score has been uploaded.

	**Returns:**

	* success (uint8)
	* this_handle (uint64_t)
	* this_score (dictionary)
		* score (int32)
		* score_changed (uint8)
		* global_rank_new (int)
		* global_rank_prev (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardScoreUploaded_t){ .md-button .md-button--store target="_blank" }

### leaderboard_ugc_set

!!! function "leaderboard_ugc_set"
	Result indicating that user generated content has been attached to one of the current user's leaderboard entries.

	**Returns:**

	* leaderboard_handle (uint64_t)
	* result (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#LeaderboardUGCSet_t){ .md-button .md-button--store target="_blank" }

### number_of_current_players

!!! function "number_of_current_players"
	Gets the current number of players for the current app ID.

	**Returns:**

	* success (uint8)
	* players (int32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#NumberOfCurrentPlayers_t){ .md-button .md-button--store target="_blank" }

### user_achievement_stored

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

### user_stats_received

!!! function "user_stats_received"
	Called when the latest stats and achievements for a specific user (including the local user) have been received from the server.

	**Returns:**

	* game (uint64_t)
	* result (uint32_t)
	* user (uint64_t) as Steam ID

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsReceived_t){ .md-button .md-button--store target="_blank" }

### user_stats_stored

!!! function "user_stats_stored"
	Result of a request to store the user stats.
	
	**Returns:**

	* game (uint64_t)
	* result (uint32_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsStored_t){ .md-button .md-button--store target="_blank" }

### user_stats_unloaded

!!! function "user_stats_unloaded"
	Callback indicating that a user's stats have been unloaded. Call [requestUserStats](#requestuserstats) again before accessing stats for this user.
	
	**Returns:**

	* user (uint64_t) as Steam ID

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsUnloaded_t){ .md-button .md-button--store target="_blank" }

---

## Enums

---

### LeaderboardDataRequest

Enumerator | Value
---------- | -----
LEADERBOARD_DATA_REQUEST_GLOBAL | 0
LEADERBOARD_DATA_REQUEST_GLOBAL_AROUND_USER | 1
LEADERBOARD_DATA_REQUEST_FRIENDS | 2
LEADERBOARD_DATA_REQUEST_USERS | 3

### LeaderboardDisplayType

Enumerator | Value
---------- | -----
LEADERBOARD_DISPLAY_TYPE_NONE | 0
LEADERBOARD_DISPLAY_TYPE_NUMERIC | 1
LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS | 2
LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS | 3

### LeaderboardSortMethod

Enumerator | Value
---------- | -----
LEADERBOARD_SORT_METHOD_NONE | 0
LEADERBOARD_SORT_METHOD_ASCENDING | 1
LEADERBOARD_SORT_METHOD_DESCENDING | 2

### LeaderboardUploadScoreMethod

Enumerator | Value
---------- | -----
LEADERBOARD_UPLOAD_SCORE_METHOD | 0
LEADERBOARD_UPLOAD_SCORE_METHOD_KEEP_BEST | 1
LEADERBOARD_UPLOAD_SCORE_METHOD_FORCE_UPDATE | 2

### SteamUserStatType

Enumerator | Value
---------- | -----
STEAM_USER_STAT_TYPE_INVALID | 0
STEAM_USER_STAT_TYPE_INT | 1
STEAM_USER_STAT_TYPE_FLOAT | 2
STEAM_USER_STAT_TYPE_AVGRATE | 3
STEAM_USER_STAT_TYPE_ACHIEVEMENTS | 4
STEAM_USER_STAT_TYPE_GROUPACHIEVEMENTS | 5
STEAM_USER_STAT_TYPE_MAX | 6
