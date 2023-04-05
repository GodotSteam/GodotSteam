# Signals - Friends

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## avatar_loaded
			
!!! function "avatar_loaded"
	Called when a large avatar is loaded if you have tried requesting it when it was unavailable.
	
	Emits signal in response to function [getLargeFriendAvatar](/functions/friends/#getlargefriendavatar), [getMediumFriendAvatar](/functions/friends/#getmediumfriendavatar), or [getSmallFriendAvatar](/functions/friends/#getsmallfriendavatar).

	**Returns:**

	* avatar_id (uint64_t)
	* width (int)
	* data (PoolByteArray)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#AvatarImageLoaded_t){ .md-button .md-button--store target="_blank" }

## avatar_image_loaded

!!! function "avatar_image_loaded"
	Called when an avatar is requested; sends back the raw Steamworks callback data compared to [avatar_loaded](/signals/friends/#avatar_loaded).
		
	Emits signal in response to function [getLargeFriendAvatar](/functions/friends/#getlargefriendavatar), [getMediumFriendAvatar](/functions/friends/#getmediumfriendavatar), or [getSmallFriendAvatar](/functions/friends/#getsmallfriendavatar).

	**Returns:**

	* avatar_id (uint64_t)
	* avatar_handle (uint32_t)
	* width (uint32_t)
	* height (uint32_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#AvatarImageLoaded_t){ .md-button .md-button--store target="_blank" }

## change_server_requested

!!! function "change_server_requested"	
	This callback is made when joining a game. If the user is attempting to join a lobby, then the callback [join_requested] will be made.

	**Returns:**

	* server (string)
	* password (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameServerChangeRequested_t){ .md-button .md-button--store target="_blank" }

## clan_activity_downloaded

!!! function "clan_activity_downloaded"
	Called when a Steam group activity has received.

	Emits signal in response to function [downloadClanActivityCounts](/functions/friends/#downloadclanactivitycounts).

	**Returns:**

	* online (int)
	* in_game (int)
	* chatting (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#DownloadClanActivityCountsResult_t){ .md-button .md-button--store target="_blank" }

## connected_chat_join

!!! function "connected_chat_join"
	Called when a user has joined a Steam group chat that the we are in.

	**Returns:**

	* chat_id (uint64_t)
	* steam_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameConnectedChatJoin_t){ .md-button .md-button--store target="_blank" }

## connected_chat_leave

!!! function "connected_chat_leave"
	Called when a user has left a Steam group chat that the we are in.

	**Returns:**

	* chat_id (uint64_t)
	* steam_id (uint64_t)
	* kicked (bool)
	* dropped (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameConnectedChatLeave_t){ .md-button .md-button--store target="_blank" }

## connected_clan_chat_message

!!! function "connected_clan_chat_message"
	Called when a chat message has been received in a Steam group chat that we are in.

	**Returns:**

	* chat (dictionary):
		* ret (int)
		* text (string)
		* type (string)
		* chatter (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameConnectedClanChatMsg_t){ .md-button .md-button--store target="_blank" }

## connected_friend_chat_message

!!! function "connected_friend_chat_message"
	Called when chat message has been received from a friend.

	**Returns:**

	* chat (dictionary):
		* ret (int)
		* text (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameConnectedFriendChatMsg_t){ .md-button .md-button--store target="_blank" }

## enumerate_following_list

!!! function "enumerate_following_list"
	Returns the result of [enumerateFollowingList](/functions/friends/#enumeratefollowinglist).

	**Returns:**

	* message (string)
	* following (array)
		* follow (dictionary)
			* num (int)
			* id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#FriendsEnumerateFollowingList_t){ .md-button .md-button--store target="_blank" }

## equipped_profile_items

!!! function "equipped_profile_items"
	Call result from [requestEquippedProfileItems](/functions/friends/#requestequippedprofileitems). Also sent as a callback.

	**Returns:**

	* result (int)
	* steam_id (uint64_t)
		* profile_data (dictionary)
			* has_avatar_animated (bool)
			* has_avatar_frame (bool)
			* has_profile_modifier (bool)
			* has_profile_background (bool)
			* has_profile_mini_background (bool)

	**Note:** GodotSteam is using the callback version.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#EquippedProfileItems_t){ .md-button .md-button--store target="_blank" }

## equipped_profile_items_changed

!!! function "equipped_profile_items_changed"
	Callback for when a user's equipped Steam Commuity profile items have changed. This can be for the current user or their friends.

	**Returns:**

	* steam_id (uint64_t)

	**Note:** GodotSteam is using the callback version.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#EquippedProfileItemsChanged_t){ .md-button .md-button--store target="_blank" }



## friend_rich_presence_update

!!! function "friend_rich_presence_update"
	Called when rich presence data has been updated for a user, this can happen automatically when friends in the same game update their rich presence, or after a call to [requestFriendRichPresence](/functions/friends/#requestfriendrichpresence).

	**Returns:**

	* steam_id (uint64_t)
	* app_id (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#FriendRichPresenceUpdate_t){ .md-button .md-button--store target="_blank" }

## get_follower_count

!!! function "get_follower_count"
	Returns the result of [getFollowerCount](/functions/friends/#getfollowercount).

	**Returns:**

	* result (int)
	* steam_id (uint64_t)
	* count (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#FriendsGetFollowerCount_t){ .md-button .md-button--store target="_blank" }

## is_following

!!! function "is_following"
	Returns the result of [isFollowing](/functions/friends/#isfollowing).

	**Returns:**
			
	* result (int)
	* steam_id (uint64_t)
	* following (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#FriendsIsFollowing_t){ .md-button .md-button--store target="_blank" }

## join_clan_chat_complete

!!! function "join_clan_chat_complete"
	Posted when the user has attempted to join a Steam group chat via [joinClanChatRoom](/functions/friends/#joinclanchatroom).

	**Returns:**

	* chat_id (uint64_t)
	* response (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#JoinClanChatRoomCompletionResult_t){ .md-button .md-button--store target="_blank" }

## join_game_requested

!!! function "join_game_requested"
	Called when the user tries to join a game from their friends list or after a user accepts an invite by a friend with [inviteUserToGame](/functions/friends/#inviteusertogame).

	Emits signal in response to receiving a Steam invite.

	**Returns:**

	* user (uint64_t)
	* connect (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameRichPresenceJoinRequested_t){ .md-button .md-button--store target="_blank" }

## join_requested

!!! function "join_requested"
	Called when the user tries to join a lobby from their friends list or after a user accepts an invite by a friend with [inviteUserToGame](/functions/friends/#inviteusertogame).

	Emits signal in response to receiving a Steam invite.

	**Returns:**

	* lobby (uint64_t)
	* steam_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameLobbyJoinRequested_t){ .md-button .md-button--store target="_blank" }

## name_changed
	
!!! function "name_changed"
	Reports the result of an attempt to change the current user's persona name.

	Emits signal in response to function [setPersonaName](/functions/friends/#setpersonaname).

	**Returns:**

	* success (bool)
	* local_success (bool)
	* result (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#SetPersonaNameResponse_t){ .md-button .md-button--store target="_blank" }

## overlay_browser_protocol

!!! function "overlay_browser_protocol"
	Dispatched when an overlay browser instance is navigated to a protocol/scheme registered by [registerProtocolInOverlayBrowser](/functions/friends/#registerprotocolinoverlaybrowser).

	**Returns:**

	* uri (string)

## overlay_toggled

!!! function "overlay_toggled"
	Posted when the Steam Overlay activates or deactivates. The game can use this to be pause or resume single player games.

	**Returns:**

	* toggled (bool)
	* user_initiated (bool)
	* app_id (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#GameOverlayActivated_t){ .md-button .md-button--store target="_blank" }

## persona_state_change

!!! function "persona_state_change"
	This is called when a user has some kind of change.

	**Returns:**

	* steam_id (uint64_t)
	* flags (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#PersonaStateChange_t){ .md-button .md-button--store target="_blank" }

## request_clan_officer_list

!!! function "request_clan_officer_list"
	Marks the return of a request officer list call.
	
	Emits signal in response to function [requestClanOfficerList](/functions/friends/#requestclanoffierlist).

	**Returns:**

	* message (string)
	* officers_list (array)
		* officer (dictionary)
			* id (uint64_t)
			* name (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamFriends#ClanOfficerListResponse_t){ .md-button .md-button--store target="_blank" }

## unread_chat_messages_changed
	
!!! function "unread_chat_messages_changed"
	Invoked when the status of unread messages changes

	**Returns:** nothing