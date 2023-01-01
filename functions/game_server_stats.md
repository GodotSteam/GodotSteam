# Functions - Game Server Stats

---

## clearUserAchievement

!!! function "clearUserAchievement( ```uint64_t``` steam_id, ```string``` name )"
	Resets the unlock status of an achievement for the specified user. This is primarily only ever used for testing. You must have called [requestUserStats](/functions/game-server-stats/#requestuserstats) and it needs to return successfully via its callback prior to calling this. This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call [storeUserStats](/functions/game-server-stats/#storeuserstats).

	**Returns:** bool

	**Note:** This will work only on achievements that game servers are allowed to set. If the "Set By" field for this achievement is "Official GS" then only game servers that have been declared as officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#ClearUserAchievement){ .md-button .md-button--store target="_blank" }

## serverGetUserAchievement

!!! function "serverGetUserAchievement( ```uint64_t``` steam_id, ```string``` name )"
	Gets the unlock status of the Achievement.

	**Returns:** dictionary

	Contains the following keys:

	* result (bool)
	* user (int) as Steam ID
	* name (string)
	* unlocked (bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#GetUserAchievement){ .md-button .md-button--store target="_blank" }		

## serverGetUserStatFloat

!!! function "serverGetUserStatFloat( ```uint64_t``` steam_id, ```string``` name )"
	Gets the current value of the a stat for the specified user.

	**Returns:** float

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#GetUserStat){ .md-button .md-button--store target="_blank" }

## serverGetUserStatInt

!!! function "serverGetUserStatInt( ```uint64_t``` steam_id, ```string``` name )"
	Gets the current value of the a stat for the specified user.

	**Returns:** uint32

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#GetUserStat){ .md-button .md-button--store target="_blank" }

## serverRequestUserStats

!!! function "serverRequestUserStats( ```uint64_t``` steam_id )"
	Asynchronously downloads stats and achievements for the specified user from the server. These stats will only be auto-updated for clients currently playing on the server. For other users you'll need to call this function again to refresh any data.

	Triggers a [stats_received](/signals/game-server-stats/#stats_received) call result.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#RequestUserStats){ .md-button .md-button--store target="_blank" }

## setUserAchievement

!!! function "setUserAchievement( ```uint64_t``` steam_id, ```string``` name )"
	Unlocks an achievement for the specified user.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#SetUserAchievement){ .md-button .md-button--store target="_blank" }

## setUserStatFloat

!!! function "setUserStatFloat( ```uint64_t``` steam_id, ```string``` name, ```float``` stat )"
	Sets / updates the value of a given stat for the specified user. You must have called [requestUserStats](/functions/game-server-stats/#requestuserstats) and it needs to return successfully via its callback prior to calling this. This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call [storeUserStats](/functions/game-server-stats/#storeuserstats).

	**Returns:** bool

	**Note:** These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#SetUserStat){ .md-button .md-button--store target="_blank" }
			setUserStatInt( ```int``` steam_id, ```string``` name, ```int``` stat )"

## setUserStatInt()

!!! function "setUserStatInt( ```uint64_t``` steam_id, ```string``` name, ```int``` stat )"
	Sets / updates the value of a given stat for the specified user. You must have called [requestUserStats](/functions/game-server-stats/#requestuserstats) and it needs to return successfully via its callback prior to calling this. This call only modifies Steam's in-memory state and is very cheap. To submit the stats to the server you must call [storeUserStats](/functions/game-server-stats/#storeuserstats).

	**Returns:** bool

	**Note:** These updates will work only on stats that game servers are allowed to edit. If the "Set By" field for this stat is "Official GS" then only game servers that have been declared as officially controlled by you will be able to set it. To do this you must set the IP range of your official servers in the Dedicated Servers section of App Admin.

   ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#SetUserStat){ .md-button .md-button--store target="_blank" }

## storeUserStats

!!! function "storeUserStats( ```uint64_t``` steam_id )"
	Send the changed stats and achievements data to the server for permanent storage for the specified user. If this fails then nothing is sent to the server. It's advisable to keep trying until the call is successful. This call can be rate limited. Call frequency should be on the order of minutes, rather than seconds. You should only be calling this during major state changes such as the end of a round, the map changing, or the user leaving a server. If you have stats or achievements that you have saved locally but haven't uploaded with this function when your application process ends then this function will automatically be called. You can find additional debug information written to the ```%steam_install%\logs\stats_log.txt``` file.

	Triggers a [stats_stored](/signals/game-server-stats/#stats_stored) callback.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#StoreUserStats){ .md-button .md-button--store target="_blank" }

## updateUserAvgRateStat

!!! function "updateUserAvgRateStat( ```uint64_t``` steam_id, ```string``` name, ```float``` this_session, ```double``` session_length )"
	Updates an AVGRATE stat with new values for the specified user.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamGameServerStats#UpdateUserAvgRateStat){ .md-button .md-button--store target="_blank" }