# Functions - Friends

------

## activateGameOverlay

!!! function "activateGameOverlay( ```string``` type )"
	Activates the overlay with optional dialog to open the following:

	* "Friends"
	* "Community"
	* "Players"
	* "Settings"
	* "OfficialGameGroup"
	* "Stats"
	* "Achievements"
	* "LobbyInvite". 

	**Returns:** void

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlay){ .md-button .md-button--store target="_blank" }

## activateGameOverlayInviteDialog

!!! function "activateGameOverlayInviteDialog( ```uint64_t``` steam_id )"
	Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby. 

	**Returns:** void

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayInviteDialog){ .md-button .md-button--store target="_blank" }

## activateGameOverlayInviteDialogConnectString

!!! function "activateGameOverlayInviteDialogConnectString( ```string``` connect_string )"
	Activates the game overlay to open an invite dialog that will send the provided Rich Presence connect string to selected friends. 

	**Returns:** void

## activateGameOverlayToStore

!!! function "activateGameOverlayToStore( ```uint32_t``` app_id =0 )"
	Activates the overlay with the application/game Steam store page. 

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToStore){ .md-button .md-button--store target="_blank" }

## activateGameOverlayToUser

!!! function "activateGameOverlayToUser( ```string``` type, ```uint64_t``` steam_id )"
	Activates the overlay to the following:

	* "steamid"
	* "chat"
	* "jointrade"
	* "stats"
	* "achievements"
	* "friendadd"
	* "friendremove"
	* "friendrequestaccept"
	* "friendrequestignore". 

	**Returns:** void

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToUser){ .md-button .md-button--store target="_blank" }

## activateGameOverlayToWebPage

!!! function "activateGameOverlayToWebPage( ```string``` url )"
	Activates the overlay with specified web address. 

	**Returns:** void

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToWebPage){ .md-button .md-button--store target="_blank" }

## clearRichPresence

!!! function "clearRichPresence()"
	Clear the game information in Steam; used in 'View Game Info'. 

	**Returns:** void

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ClearRichPresence){ .md-button .md-button--store target="_blank" }

## closeClanChatWindowInSteam

!!! function "closeClanChatWindowInSteam( ```uint64_t``` chat_id )"
	Closes the specified Steam group chat room in the Steam UI. 

	**Returns:** bool 

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#DownloadClanActivityCounts){ .md-button .md-button--store target="_blank" }

## downloadClanActivityCounts

!!! function "downloadClanActivityCounts( ```uint64_t``` clan_id, ```int``` clans_to_request )"
	Refresh the Steam Group activity data or get the data from groups other than one that the current user is a member. After receiving the callback you can then use [getClanActivityCounts](/functions/friends/#getclanactivitycounts) to get the up to date user counts.

	Triggers a [clan_activity_downloaded](/signals/friends/#clan_activity_downloaded) callback.

	**Returns:** void
 
 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#DownloadClanActivityCounts){ .md-button .md-button--store target="_blank" }

## enumerateFollowingList

!!! function "enumerateFollowingList( ```uint32``` start_index )"
	Gets the list of users that the current user is following. You can be following people that are not your friends. Following allows you to receive updates when the person does things like post a new piece of content to the Steam Workshop.

	Triggers a [enumerate_following_list](/signals/friends/#enumerate_following_list) callback.

	**Returns:** void

	**Note:** This returns up to k_cEnumerateFollowersMax users at once. If the current user is following more than that, you will need to call this repeatedly, with unStartIndex set to the total number of followers that you have received so far. I.E. If you have received 50 followers, and the user is following 105, you will need to call this again with unStartIndex = 50 to get the next 50, and then again with unStartIndex = 100 to get the remaining 5 users.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#EnumerateFollowingList){ .md-button .md-button--store target="_blank" }

## getChatMemberByIndex

!!! function "getChatMemberByIndex( ```uint64_t``` clan_id, ```int``` user )"
	Gets the Steam ID at the given index in a Steam group chat. 

	**Returns:** uint64_t

	 **Note:** You must call [getClanChatMemberCount](/functions/friends/#getclanchatmembercount) before calling this.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetChatMemberByIndex){ .md-button .md-button--store target="_blank" }

## getClanActivityCounts

!!! function "getClanActivityCounts( ```uint64_t``` clan_id )"
	Gets the most recent information we have about what the users in a Steam Group are doing.  This can only retrieve data that the local client knows about. To refresh the data or get data from a group other than one that the current user is a member of you must call [downloadClanActivityCounts](/functions/friends/#downloadclanactivitycounts).

	**Returns:** dictionary

	Contains the following keys:

	* clan (uint64_t)
	* online (int)
	* ingame (int)
	* chatting (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanActivityCounts){ .md-button .md-button--store target="_blank" }

## getClanByIndex

!!! function "getClanByIndex( ```int``` clan )"
	Gets the Steam group's Steam ID at the given index. 

	**Returns:** uint64_t

	**Note:**  You must call [getClanCount](/functions/friends/#) before calling this.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanByIndex){ .md-button .md-button--store target="_blank" }

## getClanChatMemberCount

!!! function "getClanChatMemberCount( ```uint64_t``` clan_id )"
	Get the number of users in a Steam group chat. 

	**Returns:** int

	**Note:** Large steam groups cannot be iterated by the local user.

	**Note:** The current user must be in a lobby to retrieve the Steam IDs of other users in that lobby. This is used for iteration, after calling this then [getChatMemberByIndex](/functions/friends/#getchatmemberbyindex) can be used to get the Steam ID of each person in the chat.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanChatMemberCount){ .md-button .md-button--store target="_blank" }

## getClanChatMessage

!!! function "getClanChatMessage( ```uint64_t``` chat_id, ```int``` message )"
	Gets the data from a Steam group chat room message.  This should only ever be called in response to a [connected_clan_chat_message](/signals/friends/#connected_clan_chat_message) callback.

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* text (string)
	* type (int)
	* chatter (uint64_t) as a Steam ID

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanChatMessage){ .md-button .md-button--store target="_blank" }

## getClanCount

!!! function "getClanCount()"
	Gets the number of Steam groups that the current user is a member of.  This is used for iteration, after calling this then [getClanByIndex](/functions/friends/#getclanbyindex) can be used to get the Steam ID of each Steam group.

	**Returns:** int

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanCount){ .md-button .md-button--store target="_blank" }

## getClanName

!!! function "getClanName( ```uint64_t``` clan_id )"
	Gets the display name for the specified Steam group; if the local client knows about it.

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanName){ .md-button .md-button--store target="_blank" }

## getClanOfficerByIndex

!!! function "getClanOfficerByIndex( ```uint64_t``` clan_id, ```int``` officer )"
	Gets the Steam ID of the officer at the given index in a Steam group.

	**Returns:** uint64_t

	**Note:** You must call [getClanOfficerCount](/functions/friends/#getclanofficercount) before calling this.
 
	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanOfficerByIndex){ .md-button .md-button--store target="_blank" }

## getClanOfficerCount

!!! function "getClanOfficerCount( ```uint64_t``` clan_id )"
	Gets the number of officers (administrators and moderators) in a specified Steam group. This also includes the owner of the Steam group. This is used for iteration, after calling this then [getClanOfficerByIndex](/functions/friends/#getclanofficerbyindex) can be used to get the Steam ID of each officer.

	**Returns:** int

	**Note:** You must call [requestClanOfficerList](/functions/friends/#requestclanofficerlist) before this to get the required data.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanOfficerCount){ .md-button .md-button--store target="_blank" }

## getClanOwner

!!! function "getClanOwner( ```uint64_t``` clan_id )"
	Get the steam ID of the clan owner.

	**Returns:** uint64_t

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanOwner){ .md-button .md-button--store target="_blank" }

## getClanTag

!!! function "getClanTag( ```uint64_t``` clan_id )"
	Gets the unique tag (abbreviation) for the specified Steam group; If the local client knows about it. The Steam group abbreviation is a unique way for people to identify the group and is limited to 12 characters. In some games this will appear next to the name of group members. 

	**Returns:** string

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetClanTag){ .md-button .md-button--store target="_blank" }

## getCoplayFriend

!!! function "getCoplayFriend( ```int``` friend_number )"
	Gets the Steam ID of the recently played with user at the given index.

	**Returns:** uint64_t

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetCoplayFriend){ .md-button .md-button--store target="_blank" }

## getCoplayFriendCount

!!! function "getCoplayFriendCount()"
	Gets the number of players that the current users has recently played with, across all games.  This is used for iteration, after calling this then [getCoplayFriend](/functions/friends/#getcoplayfriend) can be used to get the Steam ID of each player.  These players are have been set with previous calls to [setPlayedWith](/functions/friends/#setplayedwith).

	**Returns:** int

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetCoplayFriendCount){ .md-button .md-button--store target="_blank" }

## getFollowerCount

!!! function "getFollowerCount( ```uint64_t``` steam_id )"
	Gets the number of users following the specified user. 

	Triggers a [get_follower_count](/signals/friends/#get_follower_count) callback.

	**Returns:** void 

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFollowerCount){ .md-button .md-button--store target="_blank" }

## getFriendByIndex

!!! function "getFriendByIndex( ```int``` friend_number, ```int``` friend_flags )"
	Gets the Steam ID of the user at the given index.

	**Returns:** int

	**Note:** You must call [getFriendCount](/functions/friends/#getfriendcount) before calling this.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendByIndex){ .md-button .md-button--store target="_blank" }

## getFriendCoplayGame

!!! function "getFriendCoplayGame( ```uint64_t``` friend_id )"
	Gets the app ID of the game that user played with someone on their recently-played-with list. 

	**Returns:** uint32_t
 
 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendCoplayGame){ .md-button .md-button--store target="_blank" }
		

## getFriendCoplayTime

!!! function "getFriendCoplayTime( ```uint64_t``` friend_id )"
	Gets the timestamp of when the user played with someone on their recently-played-with list. The time is provided in Unix epoch format (seconds since Jan 1st 1970). 

	**Returns:** int

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendCoplayTime){ .md-button .md-button--store target="_blank" }

## getFriendCount

!!! function "getFriendCount()"
	Gets the number of users the client knows about who meet a specified criteria. (Friends, blocked, users on the same server, etc). This can be used to iterate over all of the users by calling [getFriendByIndex](/functions/friends/#getfriendbyindex) to get the Steam IDs of each user.

	**Returns:** int

	**Note:** Returns -1 if the current user is not logged on.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendCount){ .md-button .md-button--store target="_blank" }

## getFriendCountFromSource

!!! function "getFriendCountFromSource( ```uint64_t``` source_id )"
	Get the number of users in a source (Steam group, chat room, lobby, or game server). This is used for iteration, after calling this then [getFriendFromSourceByIndex](/functions/friends/#getfriendfromsourcebyindex) can be used to get the Steam ID of each person in the source.

	**Returns:** int

	**Note:** Large Steam groups cannot be iterated by the local user.

	**Note:** If you're getting the number of lobby members then you should use [getNumLobbyMembers](/functions/friends/#getnumlobbymembers) instead.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendCountFromSource){ .md-button .md-button--store target="_blank" }

## getFriendFromSourceByIndex

!!! function "getFriendFromSourceByIndex( ```uint64_t``` source_id, ```int``` friend_number )"
	Gets the Steam ID at the given index from a source (Steam group, chat room, lobby, or game server). 

	**Returns:** uint64_t

	**Note:** You must call [getFriendCountFromSource](/functions/friends/#getfriendcountfromsource) before calling this.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendFromSourceByIndex){ .md-button .md-button--store target="_blank" }

## getFriendGamePlayed

!!! function "getFriendGamePlayed( ```uint64_t``` steam_id )"
	Checks if the specified friend is in a game, and gets info about the game if they are.
	
	**Returns:** dictionary

	Contains the following keys:

	* id (int) as app ID
	* ip (string)
	* game_port (uint16)
	* query_port (uint16)
	* lobby (uint64_t)

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendGamePlayed){ .md-button .md-button--store target="_blank" }

## getFriendMessage

!!! function "getFriendMessage( ```uint64_t``` friend_id, ```int``` message )"
	Gets the data from a Steam friends message. This should only ever be called in response to a [connected_friend_chat_message](/signals/friends/#connected_friend_chat_message) callback.

	**Returns:** dictionary

	Contains the following keys:

	* ret (int)
	* text (string)

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendMessage){ .md-button .md-button--store target="_blank" }

## getFriendPersonaName

!!! function "getFriendPersonaName( ```uint64_t``` steam_id )"
	Gets the specified user's persona (display) name. This will only be known to the current user if the other user is in their friends list, on the same game server, in a chat room or lobby, or in a small Steam group with the local user. To get the persona name of the current user use [getPersonaName](/functions/friends/#getpersonaname).

	**Returns:** string

	**Note:** Upon on first joining a lobby, chat room, or game server the current user will not known the name of the other users automatically; that information will arrive asynchronously via [persona_state_change](/signals/friends/#persona_state_change) callbacks.

	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendPersonaName){ .md-button .md-button--store target="_blank" }

## getFriendPersonaNameHistory

!!! function "getFriendPersonaNameHistory( ```uint64_t``` steam_id, ```int``` name_history )"
	Gets one of the previous display names for the specified user. This only works for display names that the current user has seen on the local computer.

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendPersonaNameHistory){ .md-button .md-button--store target="_blank" }

## getFriendPersonaState

!!! function "getFriendPersonaState()"
	Gets the current status of the specified user. This will only be known to the current user if the other user is in their friends list, on the same game server, in a chat room or lobby, or in a small Steam group with the local user. To get the state of the current user use [getPersonaState](/functions/friends/#getpersonastate).

	**Returns:** int

	Should be one of the following:

	* 0 - Offline
	* 1 - Online
	* 2 - Busy
	* 3 - Away
	* 4 - Snooze
	* 5 - Looking For Trade
	* 6 - Looking for Play

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendPersonaState){ .md-button .md-button--store target="_blank" }

## getFriendRelationship

!!! function "getFriendRelationship( ```uint64_t``` steam_id )"
	Gets a relationship to a specified user.

	**Returns:** int

	Should be one of the following:

	* 0 - None
	* 1 - Blocked
	* 2 - Request Recipient
	* 3 - Friend
	* 4 - Request Initiator
	* 5 - Ignored
	* 6 - Ignored Friend
	* 7 - Suggested
	* 8 - Max

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendRelationship){ .md-button .md-button--store target="_blank" }

## getFriendRichPresence

!!! function "getFriendRichPresence( ```uint64_t``` friend_id, ```string``` key )"
	Get a Rich Presence value from a specified friend (typically only used for debugging). 

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendRichPresence){ .md-button .md-button--store target="_blank" }

## getFriendRichPresenceKeyCount

!!! function "getFriendRichPresenceKeyCount( ```uint64_t``` friend_id )"
	Gets the number of Rich Presence keys that are set on the specified user. This is used for iteration, after calling this then [getFriendRichPresenceKeyByIndex](/functions/friends/#getfriendrichpresencekeybyindex) to get the rich presence keys. This is typically only ever used for debugging purposes.

	**Returns:** int

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendRichPresenceKeyCount){ .md-button .md-button--store target="_blank" }

## getFriendRichPresenceKeyByIndex

!!! function "getFriendRichPresenceKeyByIndex( ```uint64_t``` friend_id, ```int``` key )"
	Returns an empty string ("") if the index is invalid or the specified user has no Rich Presence data available. 

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendRichPresenceKeyByIndex){ .md-button .md-button--store target="_blank" }

## getFriendsGroupCount

!!! function "getFriendsGroupCount()"
	Gets the number of friends groups (tags) the user has created. This is used for iteration, after calling this then [getFriendsGroupIDByIndex](/functions/friends/#getfriendsgroupidbyindex) can be used to get the ID of each friend group. This is not to be confused with Steam groups. Those can be obtained with [getClanCount](/functions/friends/#getclancount).

	**Returns:** int

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendsGroupMembersCount){ .md-button .md-button--store target="_blank" }

## getFriendsGroupIDByIndex

!!! function "```int16``` getFriendsGroupIDByIndex( ```int16``` friend_group )"
	Gets the friends group ID for the given index.

	**Returns:** int16

	**Note:** You must call [getFriendsGroupCount](/functions/friends/#getfriendsgroupcount) before calling this.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendsGroupIDByIndex){ .md-button .md-button--store target="_blank" }

## getFriendsGroupMembersCount

!!! function "getFriendsGroupMembersCount( ```int16``` friend_group )"
	Gets the number of friends in a given friends group.  This should be called before getting the list of friends with [getFriendsGroupMembersList](/functions/friends/#getfriendsgroupmemberslist).

	**Returns:** int

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendsGroupMembersCount){ .md-button .md-button--store target="_blank" }
		
## getFriendsGroupMembersList

!!! function "```array``` getFriendsGroupMembersList( ```int16``` friend_group, ```int``` member_count )"
	Gets the number of friends in the given friends group.  If fewer friends exist than requested those positions' Steam IDs will be invalid.  You must call [getFriendsGroupMembersCount](/functions/friends/#getfrendsgroupmemberscount) before calling this to set up the **member_count** argument with an appropriate size!

	**Returns:** array

	Contains a list of:

	* friend Steam IDs (uint64_t)
 
 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendsGroupMembersList){ .md-button .md-button--store target="_blank" }

## getFriendsGroupName

!!! function "getFriendsGroupName( ```int16``` friend_group )"
	Gets the name for the given friends group. 

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendsGroupName){ .md-button .md-button--store target="_blank" }

## getFriendSteamLevel

!!! function "getFriendSteamLevel( ```uint64_t``` steam_id )"
	Gets the Steam level of the specified user. You can use the local users Steam ID to get their level.

	**Returns:** int

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendSteamLevel){ .md-button .md-button--store target="_blank" }

## getLargeFriendAvatar

!!! function "getLargeFriendAvatar( ```uint64_t``` steam_id )"
	Gets a handle to the large (128 x 128px) avatar for the specified user. You can pass in [getSteamID](/functions/friends/#getsteamid) to get the current user's avatar.

	Triggers a [avatar_loaded](/functions/friends/#avatar_loaded) callback.

	**Returns:** int

	A Steam image handle which is used with [getImageSize](/functions/friends/#getimagesize) and [getImageRGBA](/functions/friends/#getimagergba).

	**Note:** This only works for users that the local user knows about. They will automatically know about their friends, people on leaderboards they've requested, or people in the same source as them (Steam group, chat room, lobby, or game server). If they don't know about them then you must call [requestUserInformation](/functions/friends/#) to cache the avatar locally.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetLargeFriendAvatar){ .md-button .md-button--store target="_blank" }

## getMediumFriendAvatar

!!! function "getMediumFriendAvatar( ```uint64_t``` steam_id )"
	Gets a handle to the medium (64 x 64px) avatar for the specified user. You can pass in [getSteamID](/functions/friends/#getsteamid) to get the current user's avatar.

	Triggers a [avatar_loaded](/functions/friends/#avatar_loaded) callback.

	**Returns:** int

	A Steam image handle which is used with [getImageSize](/functions/friends/#getimagesize) and [getImageRGBA](/functions/friends/#getimagergba).

	**Note:** This only works for users that the local user knows about. They will automatically know about their friends, people on leaderboards they've requested, or people in the same source as them (Steam group, chat room, lobby, or game server). If they don't know about them then you must call [requestUserInformation](/functions/friends/#requestuserinformation) to cache the avatar locally.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetMediumFriendAvatar){ .md-button .md-button--store target="_blank" }

## getPersonaName

!!! function "getPersonaName()"
	Gets the current user's persona (display) name. This is the same name that is displayed the users community profile page. To get the persona name of other users use [getFriendPersonaName](/functions/friends/#getfriendpersonaname).

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetPersonaName){ .md-button .md-button--store target="_blank" }

## getPersonaState

!!! function "getPersonaState()"
	Gets the friend status of the current user. To get the state of other users use [getFriendPersonaState](/functions/friends/#getfriendpersonastate).

	**Returns:** int

	Should be one of the following:

	* 0 - Offline
	* 1 - Online
	* 2 - Busy
	* 3 - Away
	* 4 - Snooze
	* 5 - Looking For Trade
	* 6 - Looking for Play

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetPersonaState){ .md-button .md-button--store target="_blank" }

## getPlayerAvatar

!!! function "getPlayerAvatar( ```int``` size = 2, ```uint64_t``` steam_id = 0 )"
	Get player's avatar. Short-cuts to the various avatar functions in Steamworks.

	Size is passed as follows:

	* 1 - small avatar
	* 2 - medium avatar
	* 3 - large avatar

	Triggers a [avatar_loaded](/signals/friends/#avatar_loaded) callback.

	**Returns:** void . 

	**Note:** This is a unique function to GodotSteam.

## getPlayerNickname

!!! function "getPlayerNickname( ```uint64_t``` steam_id )"
	Returns nickname the current user has set for the specified player. Returns NULL if the no nickname has been set for that player. 

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetPlayerNickname){ .md-button .md-button--store target="_blank" }

## getProfileItemPropertyString

!!! function "getProfileItemPropertyString( ```uint64_t``` steam_id, ```int``` item_type, ```int``` item_property )"
	Returns a string property for a user's equipped profile item.

	**Returns:** string

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetProfileItemPropertyString){ .md-button .md-button--store target="_blank" }

## getProfileItemPropertyInt

!!! function "getProfileItemPropertyInt( ```uint64_t``` steam_id, ```int``` item_type, ```int``` item_property )"
	Returns an unsigned integer property for a user's equipped profile item.

	**Returns:** uint32

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetProfileItemPropertyUint){ .md-button .md-button--store target="_blank" }

## getRecentPlayers

!!! function "getRecentPlayers()"
	Get list of players user has recently played game with.

	**Returns:** array

	Contains a list of:

	* player (dictionary)

	Contains the following keys:

	* id (uint64_t) as friend Steam ID.
	* name (string)
	* time (int)
	* status (int)

	**Note:** This is a unique function to GodotSteam.

## getSmallFriendAvatar

!!! function "getSmallFriendAvatar( ```uint64_t``` steam_id )"
	Gets a handle to the small (32 x 32px) avatar for the specified user. You can pass in [getSteamID](/functions/friends/#getsteamid) to get the current user's avatar.

	Triggers a [avatar_loaded](/functions/friends/#avatar_loaded) callback.

	**Returns:** int

	A Steam image handle which is used with [getImageSize](/functions/friends/#getimagesize) and [getImageRGBA](/functions/friends/#getimagergba).

	**Note:** This only works for users that the local user knows about. They will automatically know about their friends, people on leaderboards they've requested, or people in the same source as them (Steam group, chat room, lobby, or game server). If they don't know about them then you must call [requestUserInformation](/functions/friends/#requestuserinformation) to cache the avatar locally.

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetSmallFriendAvatar){ .md-button .md-button--store target="_blank" }

## getUserFriendsGroups

!!! function "getUserFriendsGroups()""
	Get list of friends groups (tags) the user has created. This is not to be confused with Steam groups.

	**Returns:** array

	Contains a list of:

	* tags (dictionary)

	Contains the following keys:

	* id (int16) as friend group ID
	* name (string)
	* members (int)

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](){ .md-button .md-button--store target="_blank" }
			getUserFriendsGroups()

## getUserRestrictions

!!! function "getUserRestrictions()"
	If current user is chat restricted, they can't send or receive any text or voice chat messages. The user can't see custom avatars. But the user can be online and send or receive game invites. A chat restricted user cannot add friends or join any groups. Restricted users can still be online and send/receive game invites.

	**Returns:** uint32

	It should be one of the following:

	* 0 - None
	* 1 - Unknown
	* 2 - Any Chat
	* 4 - Voice Chat
	* 8 - Group Chat
	* 16 - Rating
	* 32 - Game Invites
	* 64 - Trading

 	---
 	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GetUserRestrictions){ .md-button .md-button--store target="_blank" }

## getUserSteamFriends

!!! function "getUserSteamFriends()"
	Get a list of user's Steam friends; a mix of different Steamworks API friend functions.

	**Returns:** array

	Contains a list of:

	* friends (dictionary)

	Contains the following keys:

	* id (uint64_t) as friend Steam ID
	* name (string)
	* status (int)

	**Note:** This is a unique function to GodotSteam.

## getUserSteamGroups

!!! function "```array``` getUserSteamGroups()"
	Get list of user's Steam groups; a mix of different Steamworks API group functions.

	**Returns:** array

	Contains groups (dictionary) that contains the following keys:

	* id (uint64_t) as group ID
	*	name (string)
	*	tag (string)

	**Note:** This is a unique function to GodotSteam.

## hasEquippedProfileItem

!!! function "hasEquippedProfileItem( ```uint64_t``` steam_id, ```int``` item_type )"
	After calling RequestEquippedProfileItems, you can use this function to check if the user has a type of profile item equipped or not.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#BHasEquippedProfileItem){ .md-button .md-button--store target="_blank" }

## hasFriend

!!! function "hasFriend( ```uint64_t``` steam_id, ```int``` friend_flags )"
	Returns true if the specified user meets any of the criteria specified in **friend_flags**:

	* 0x00 - None
	* 0x01 - Blocked
	* 0x02 - Friendship Requested
	* 0x04 - Immediate
	* 0x08 - Clan Member
	* 0x10 - On Game Server
	* 0x80 - Requesting Friendship
	* 0x100 - Requesting Info
	* 0x200 - Ignored
	* 0x400 - Ignored Friend
	* 0x0000 - Chat Member
	* 0xFFFF - All

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#HasFriend){ .md-button .md-button--store target="_blank" }

## inviteUserToGame

!!! function "inviteUserToGame( ```uint64_t``` friend_id, ```string``` connect_string )"
	Invites a friend or clan member to the current game using a special invite string. If the target user accepts the invite then the pchConnectString gets added to the command-line when launching the game. If the game is already running for that user, then they will receive a [join_game_requested](/signals/friends/#join_game_requested) callback with the connect string.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#InviteUserToGame){ .md-button .md-button--store target="_blank" }

## isClanChatAdmin

!!! function "isClanChatAdmin( ```uint64_t``` chat_id, ```uint64_t``` steam_id )"
	Checks if a user in the Steam group chat room is an admin. 

	**Returns:** bool 

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#IsClanChatAdmin){ .md-button .md-button--store target="_blank" }

## isClanPublic

!!! function "isClanPublic( ```uint64_t``` clan_id )"
	Checks if the Steam group is public. 

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#IsClanPublic){ .md-button .md-button--store target="_blank" }

## isClanOfficialGameGroup

!!! function "isClanOfficialGameGroup( ```uint64_t``` clan_id )"
	Checks if the Steam group is an official game group/community hub. 

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#IsClanOfficialGameGroup){ .md-button .md-button--store target="_blank" }

## isClanChatWindowOpenInSteam

!!! function "isClanChatWindowOpenInSteam( ```uint64_t``` chat_id )"
	Checks if the Steam Group chat room is open in the Steam UI.

	**Returns:** bool 

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#IsClanChatWindowOpenInSteam){ .md-button .md-button--store target="_blank" }

## isFollowing

!!! function "isFollowing( ```uint64_t``` steam_id )"
	Checks if the current user is following the specified user.

	Triggers a [is_following](/signals/friends/#is_following) callback.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#IsFollowing){ .md-button .md-button--store target="_blank" }

## isUserInSource

!!! function "isUserInSource( ```uint64_t``` steam_id, ```uint64_t``` source_id )"
	Checks if a specified user is in a source (Steam group, chat room, lobby, or game server).

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#IsUserInSource){ .md-button .md-button--store target="_blank" }

## joinClanChatRoom

!!! function "joinClanChatRoom( ```uint64_t``` clan_id )"
	Allows the user to join Steam group (clan) chats right within the game. The behavior is somewhat complicated, because the user may or may not be already in the group chat from outside the game or in the overlay. You can use [activateGameOverlayToUser](/functions/friends/#activategameoverlaytouser) to open the in-game overlay version of the chat.

	Triggers a [join_clan_chat_complete](/signals/friends/#join_clan_chat_complete) callback.

	May additionally trigger the following callbacks:

	* [connect_chat_join](/signals/friends/#connect_chat_join)
	* [connect_chat_leave](/signals/friends/#connect_chat_leave)
	* [connect_clan_chat_message](/signals/friends/#connect_clan_chat_message)

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#JoinClanChatRoom){ .md-button .md-button--store target="_blank" }

## leaveClanChatRoom

!!! function "leaveClanChatRoom( ```uint64_t``` clan_id )"
	Leaves a Steam group chat that the user has previously entered with [joinClanChatRoom](/functions/friends/#joinclanchatroom).

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#LeaveClanChatRoom){ .md-button .md-button--store target="_blank" }

## openClanChatWindowInSteam

!!! function "openClanChatWindowInSteam( ```uint64_t``` chat_id )"
	Opens the specified Steam group chat room in the Steam UI. 

	**Returns:** bool 

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#OpenClanChatWindowInSteam){ .md-button .md-button--store target="_blank" }

## registerProtocolInOverlayBrowser

!!! function "registerProtocolInOverlayBrowser( ```string``` protocol )"
	Call this before calling [activateGameOverlayToWebPage](/functions/friends/#activategameoverlaytowebpage) to have the Steam Overlay Browser block navigations to your specified protocol (scheme) uris and instead dispatch a [overlay_browser_protocol](/signals/friends/#overlay_browser_protocol) callback to your game. [activateGameOverlayToWebPage](/functions/friends/#activategameoverlaytowebpage) must have been called with 1.

	**Returns:** bool

	**Note:** While this function is in the SDK, it is not listed in the Steamworks docs.

## replyToFriendMessage

!!! function "replyToFriendMessage( ```uint64_t``` steam_id, ```string``` message )"
	Sends a message to a Steam friend. 

	**Returns:** bool 

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ReplyToFriendMessage){ .md-button .md-button--store target="_blank" }

## requestClanOfficerList

!!! function "requestClanOfficerList( ```uint64_t``` clan_id )"
	Requests information about a Steam group officers (administrators and moderators).

	Triggers a [request_clan_officer_list](/signals/friends/#request_clan_officer_list) callback.

	**Returns:** void

	**Note:** You can only ask about Steam groups that a user is a member of.

	**Note:** This won't download avatars for the officers automatically. If no avatar image is available for an officer, then call [requestUserInformation](/functions/friends/#requestuserinformation) to download the avatar.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#RequestClanOfficerList){ .md-button .md-button--store target="_blank" }

## requestFriendRichPresence

!!! function "requestFriendRichPresence( ```uint64_t``` friend_id )"
	Requests Rich Presence data from a specific user. This is used to get the Rich Presence information from a user that is not a friend of the current user, like someone in the same lobby or game server. This function is rate limited, if you call this too frequently for a particular user then it will just immediately post a callback without requesting new data from the server.

	Triggers a [friend_rich_presence_update](/signals/friends/#friend_rich_presence_update) callback.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#RequestFriendRichPresence){ .md-button .md-button--store target="_blank" }

## requestUserInformation

!!! function "requestUserInformation( ```uint64_t``` steam_id, ```bool``` require_name_only )"
	Requests information about a user - persona name & avatar; if bRequireNameOnly is set, then the avatar of a user isn't downloaded. 

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#RequestUserInformation){ .md-button .md-button--store target="_blank" }

## sendClanChatMessage

!!! function "sendClanChatMessage( ```uint64_t``` chat_id, ```string``` text )"
	Sends a message to a Steam group chat room.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#SendClanChatMessage){ .md-button .md-button--store target="_blank" }

## setInGameVoiceSpeaking

!!! function "setInGameVoiceSpeaking( ```uint64_t``` steam_id, ```bool``` speaking )"
	User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI). 

	Let Steam know that the user is currently using voice chat in game. This will suppress the microphone for all voice communication in the Steam UI.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#SetInGameVoiceSpeaking){ .md-button .md-button--store target="_blank" }

## setListenForFriendsMessages

!!! function "setListenForFriendsMessages( ```bool``` intercept )"
	Listens for Steam friends chat messages. You can then show these chats inline in the game. For example with a Blizzard style chat message system or the chat system in Dota 2. After enabling this you will receive [connected_friend_chat_message](/signals/friends/#connected_friend_chat_message) callbacks when ever the user receives a chat message. You can get the actual message data from this callback with [getFriendMessage](/functions/friends/#getfriendmessage). You can send messages with [replyToFriendMessage](/functions/friends/#replytofriendmessage).

	Triggers a [connected_friend_chat_message](/signals/friends/#connected_friend_chat_message) callback.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#SetListenForFriendsMessages){ .md-button .md-button--store target="_blank" }

## setPersonaName

!!! function "setPersonaName( ```string``` name )"
	Sets the current user's persona name, stores it on the server and publishes the changes to all friends who are online. Changes take place locally immediately, and a [persona_state_change](/functions/friends/#persona_state_change) callback is posted, presuming success. If the name change fails to happen on the server, then an additional [persona_state_change](/functions/friends/#persona_state_change) callback will be posted to change the name back, in addition to the final result available in the call result.

	Triggers a [name_changed](/signals/friends/#name_changed) callback.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#SetPersonaName){ .md-button .md-button--store target="_blank" }

## setPlayedWith

!!! function "setPlayedWith( ```uint64_t``` steam_id )"
	Set player as 'Played With' for game.

	**Returns:** void

	**Note:** The current user must be in game with the other player for the association to work.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#SetPlayedWith){ .md-button .md-button--store target="_blank" }

## setRichPresence

!!! function "setRichPresence( ```string``` key, ```string``` value )"
	Sets a Rich Presence key/value for the current user that is automatically shared to all friends playing the same game. Each user can have up to 20 keys set as defined by k_cchMaxRichPresenceKeys. There are two special keys used for viewing/joining games:

	**status** - A UTF-8 string that will show up in the 'view game info' dialog in the Steam friends list.

	**connect** - A UTF-8 string that contains the command-line for how a friend can connect to a game. This enables the 'join game' button in the 'view game info' dialog, in the steam friends list right click menu, and on the players Steam community profile. Be sure your app implements [getLaunchCommandLine](/functions/apps/#getlaunchcommandline) so you can disable the popup warning when launched via a command line.

	There are three additional special keys used by the new Steam Chat:

	**steam_display** - Names a rich presence localization token that will be displayed in the viewing user's selected language in the Steam client UI. See Rich Presence Localization for more info, including a link to a page for testing this rich presence data. If steam_display is not set to a valid localization tag, then rich presence will not be displayed in the Steam client.

	**steam_player_group** - When set, indicates to the Steam client that the player is a member of a particular group. Players in the same group may be organized together in various places in the Steam UI. This string could identify a party, a server, or whatever grouping is relevant for your game. The string itself is not displayed to users.

	**steam_player_group_size** - When set, indicates the total number of players in the steam_player_group. The Steam client may use this number to display additional information about a group when all of the members are not part of a user's friends list. (For example, "Bob, Pete, and 4 more".)

	You can clear all of the keys for the current user with [clearRichPresence](/functions/friends/#clearrichpresence). To get rich presence keys for friends use [getFriendRichPresence](/functions/friends/#getfriendrichpresence).

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#SetRichPresence){ .md-button .md-button--store target="_blank" }