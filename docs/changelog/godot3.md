# Godot 3.x Change-Log

### A history of all changes to the **master** branch.

**Note:** when Godot 4.x goes stable, this branch will become the godot3 branch and godot4 will become master.

------

**Version 3.19 Changes**

* Changed: various backports from godot4 branch
* Fixed: various lobby list functions now return the server_list_request handle
* Fixed: need for godotsteam.sh file on some Linux systems, _thanks to mikix_
* Fixed: typo in returned dictionary for createSocketPair
* Fixed: requestInternetServerList causing crashes

**Version 3.18.5 Changes**

* Fixed: createListenSocketP2P
* Fixed: other issues with reading array size

**Version 3.18.4 Changes**

* Changed: documentation updates for Doxygen, thanks to _Ralian_
* Removed: isCyberCafe function

**Version 3.18.3 Changes**

* Added: networking type message constants
* Added: more descriptions and tutorial links to in-editor docs
* Fixed: getLobbyData not returning UTF-8 encoded string
* Fixed: sendLobbyChatMsg truncating non-English strings
* Removed: MarketingMessageFlags as they don't exist in the header files

**Version 3.18.2 Changes**

* Changed: minor internal variable names
* Fixed: filterText truncating input; thanks to _tcoxon_

**Version 3.18.1 Changes**

* Added: link to SDK placeholder
* Added: more descriptions to in-editor docs
* Fixed: some regressions that made their way into 3.18

**Version 3.18 Changes**

* Added: avatar_image_loaded callback to get raw response from Steamworks
* Changed: project layout to be Git clone friendly
* Fixed: incorrectly named io_failure for steamworks_error signal, thanks to raulsntos
* Fixed: getSessionConnectionInfo using old networking struct
* Removed: unused networking stricts

**Version 3.17.5 Changes**

* Changed: submitItemUpdate to use null if no notes are passed, thanks to mashumafi
* Removed: unused server signals

**Version 3.17.4 Changes**

* Changed: leaderboard_scores_downloaded and leaderboard_score_updated now pass back their handles; this is incompatible with earlier versions
* Fixed: issue where leaderboard_score_uploaded would not fire if passed leaderboard_handle was not internally stored

**Version 3.17.3 Changes**

* Fixed: getVoice and getAvailableVoice functions
* Removed: all server functionality, put back into server branch

**Version 3.17.2 Changes**

* Fixed: location data structs for new Networking classes, thanks to Kryx-Ikyr

**Version 3.17.1 Changes**

* Fixed: missing comma in getVideoURL argument function
* Fixed: argument name mismatch with file_details_result
* Fixed: wrong int type for inventory update handle
* Fixed: not casting app ID for addFavoriteGame
* Fixed: wrong int type for server ID in getLobbyGameServer
* Fixed: not casting account ID for createQueryUserUGCRequest

**Version 3.17 Changes**

* Added: new functions, enums for Steamworks SDK 1.55
* Fixed: getPublicIP in Game Servers class

**Version 3.16.1 Changes**

* Fixed: issues with getPSNID and getStadiaID functions when compiling on Linux

**Version 3.16 Changes**

* Added: new enums for Community Profile item types and properties in Friends class
* Added: new functions hasEquippedProfileItem, getProfileItemPropertyString, and getProfileItemPropertyInt in Friends class
* Added: new callbacks/signals equipped_profile_items_changed and equipped_profile_items in Friends class
* Added: new networking identity types
* Added: new functions setXboxPairwiseID, getXboxPairwiseID, setPSNID, getPSNID, setStadiaID, and getStadiaID to Networking Types class
* Changed: minor correction to createListenSocketP2P in attempt to fix possible crash

**Version 3.15 Changes**

* Changed: sendMessageToConnection and sendMessages now take PoolByteArrays to send any data
* Fixed: issue with receiving messages, now allows more than one at a time; thanks to Frostings
* Fixed: getQueryUGCChildren not working correctly; thanks to EIREXE

**Version 3.14 Changes**

* Added: inventory handle argument to various Inventory class functions, defaults to 0 to use internally store argument
* Changed: various Inventory class functions to send back the new inventory handle as well as storing it internally
* Fixed: various string issues; thanks to Green Fox
* Fixed: file_read_async_complete call result not sending back the file buffer
* Fixed: missing variant type for avatar_loaded signal
* Fixed: enumerate_following_list calling the wrong signal name
* Fixed: print of Steamworks error didn't contain signal name
* Fixed: some variable and argument names
* Fixed: deserializeResult to accept incoming PoolByteArray buffer
* Fixed: various message functions in new networking classes; thanks to Avantir-Chaosfire

**Version 3.13.3 Changes**

* Fixed: get correct size of lobby message in sendLobbyChatMsg; thanks to Green Fox

**Version 3.13.2 Changes**

* Fixed: various functions and callbacks that sent back scrambled IP addresses

**Version 3.13.1 Changes**

* Changed: all HTML Surface functions can now have the handle passed to them or not; will use internal handle if not passed
* Changed: all HTML Surface callbacks now send back their browser handles, if applicable
* Changed: fileWrite and fileWriteAsync now allow you to pass size or not; will determine if not passed
* Fixed: fileWrite and fileWriteAsync passing wrong byte array size

**Version 3.13 Changes**

* Added: missing function getPlaybackStatus to Music class
* Added: missing function setDurationControlOnlineState to Users class
* Added: missing signals for Matchmaking Servers
* Added: missing PropertyInfo data for signals
* Changed: serverInit now takes the individual arguments and no longer a dictionary of arguments
* Changed: getAppName, getAppListInstallDir, and getAppListBuildId in App Lists to use uint32_t instead of uint32
* Changed: initGameServer to use correct arguments
* Changed: all signal / callback names for Game Server class to lower-case to match the all others
* Changed: server_connect_failure, policy_response, client_group_status callback to match function names
* Changed: various variables in Game Server class callbacks to match the others
* Changed: setMaxPlayerCount argument to players_max from max to be more clear
* Changed: setPasswordProtected argument to password_protected from password to be more clear
* Changed: call result / signal stat_received to stats_received
* Changed: createCookieContainer now sends back the cookie_handle
* Changed: checkResultSteamID changed argument name to match
* Changed: getItemsWithPrices return dictionary name
* Changed: getAppID now returns uint32_t
* Changed: getFavoriteGames to have more distinct port names in return dictionary
* Changed: some returned types and argument types to better match their Steamworks counterparts
* Changed: names of some keys and some integer types in getQueryUGCResult return dictionary
* Changed: keys in getBeaconDetails return dictionary to be more clear
* Changed: removed data_size argument from various Remote Storage functions and get size internally
* Changed: playerDetails and serverRules IP argument to a string
* Changed: various Networking Messages, Networking Sockets, and Networking Utils functions to use internal struct system with Networking Type functions
* Changed: a variety of miscellaneous small changes and corrections
* Fixed: some missing function binds
* Fixed: lobby_message callback data, thanks to kongo555
* Fixed: missing default value for getAvailableP2PPacketSize, readP2PPacket, sendP2PPacket
* Fixed: getAnalogActionData so the return dictionary has the right keys
* Fixed: getUserSteamFriends, getUserSteamGroups to give the correct Steam ID back
* Fixed: getFriendGamePlayed using wrong key name in return dictionary
* Fixed: toIdentityString to provide the correct string data
* Fixed: parseIdentityString to properly parse back the string data
* Fixed: getSesssionConnectionInfo now passes back all data
* Fixed: getLocalPingLocation should return both the ping and location ID in a dictionary
* Fixed: getPingToDataCenter, getPOPList, parsePingLocationString, closeConnection, getAuthenticationStatus, getConnectionInfo, createSocketPair functions
* Removed: requestAllProofOfPurchaseKeys and requestAppProofOfPurchaseKey as they are depreciated
* Removed: gameplay_stats signal from Game Server class as it wasn't connected to anything
* Removed: getUserDataFolder as it is depreciated
* Removed: leading _ in front of callbacks and call results internally
* Removed: initGameServer as it is unnecessary
* Removed: connectByIPAddress, isPingMeasurementInProgress, setLinkedLobby as they are not in the SDK

**Version 3.12.1 Changes**

* Fixed: incorrect case on app_installed and app_uninstalled, thanks to craftablescience

**Version 3.12.1 Changes**

* Fixed: incorrect case on app_installed and app_uninstalled, thanks to craftablescience

**Version 3.12 Changes**

* Added: missing D_METHOD to all functions, should show the right argument names in-editor
* Added: Input origin enums for PS5 and Steam Deck
* Added: Input Types, Input Glyph Style, Input Glyph Size, and Input Configuration Enable Type enums
* Added: getConnectionRealTimeStatus, configureConnectionLanes, connectP2PCustomSignaling, receivedP2PCustomSignal, getCertificateRequest, setCertificate, resetIdentity, runNetworkingCallbacks, beginAsyncRequestFakeIP, getFakeIP, createListenScoketP2PFakeIP, getRemoveFakeIPForConnection, and createFakeUDPPort functions and callback to NetworkingSockets class
* Added: dismissFloatingGamepadTextInput function to Utils class
* Added: setTimeCreatedDateRange and setTimeUpdatedDateRange to UGC class
* Added: NetworkingeDebugOutputType enums for NetworkingUtils
* Added: missing constant binds for Server API, OverlayToWebPageMode
* Fixed: minor compiler warnings
* Fixed: empty file hash being returned by file_details_result callback
* Fixed: a variety of small bugs and possible crashes, thanks to qarmin
* Fixed: missing binds for getFriendsGroupName, getFriendsGroupMembersList, getFriendsGroupIDByIndex, getFriendsGroupCount, getFriendMessage, getFriendCoplayTime, getFriendCoplayGame, getCoplayFriendCount, getCoplayFriend, getClanTag, getClanName, getClanCount, getClanChatMessage, getClanByIndex, getClanActivityCounts, fileWriteAsync, fileWriteStreamCancel, fileWriteStreamClose, fileWriteStreamOpen, fileWriteStreamWriteChunk, getCachedUGCCount, getUGCDownloadProgress, getUGCDetails, fileReadAsync, getOPFSettings, getOPFStringForApp, getVideoURL, isBroadcasting functions
* Fixed: setPNGIcon and updateCurrentEntryCoverArt in Music Remote class
* Fixed: missing getUGCDetails and getUGCDownloadProgress functions
* Changed: updated doc_class file for in-editor documentation
* Changed: updated to Steamworks 1.53
* Changed: lobby_data_update, removed lobby data queries as they should be done manually
* Changed: minor tweaks under-the-hood
* Changed: various generic 'int' to their actual types
* Changed: renamed servers and server stats to game server and game server stats respectively, to match SDK
* Changed: SteamNetworkingQuickConnectionStatus to SteamNetConnectionRealTimeStatus_t per Steamworks SDK 1.53, causes a break in previous GodotSteam versions
* Changed: getConfigValueInfo, removed name and next value from return dictionary as they are no longer passed by function in SDK 1.53
* Changed: rearranged functions in godotsteam.cpp class binds to match [godotsteam.h] order
* Changed: enum constant binds to match [godotsteam.h] enum order
* Removed: unused callback new_launch_query_parameters, broadcast_upload_start, broadcast_upload_stop
* Removed: allocateMessage as it shouldn't be used solo
* Removed: getQuickConnectionStatus and getFirstConfigValue as they were removed from SDK 1.53
* Removed: setDebugOutputFunction from Networking Utils

**Version 3.11.1 Changes**

* Removed: unused structs

**Version 3.11 Changes**

* Added: server branch merged into master
* Changed: spacing in default arguments in [godotsteam.h]
* Changed: renamed STEAM_GAMESERVER_CALLBACK as STEAM_CALLBACK
* Removed: SteamGameServer_RunCallbacks function

**Version 3.10.5 Changes**

* Added: more helper functions for newer networking classes, translations for steamnetworkingtypes
* Fixed: lots of compiler warnings on Linux, thanks to gregcsokas

**Version 3.10.4 Changes**

* Added: new helper functions for newer networking classes, translations for steamnetworkingtypes
* Fixed: default argument inputInit function, thanks to hhyyrylainen

**Version 3.10.3 Changes**

* Changed: various internal variable / arguments names for clarity, will affect signal-returned dictionaries

**Version 3.10.2 Changes**

* Removed: not logged in as error condition in steamInit function

**Version 3.10.1 Changes**

* Changed: various compilation errors for OSX, thanks to SapphireMH
* Removed: receiveRelayAuthTicket, findRelayAuthTicketForServer, getHostedDedicatedServerAddress, and getGameCoordinatorServerLogin as they rely on datagram header that was removed from base SDK

**Version 3.10 Changes**

* Added: various Steam Deck specific functions, thanks to EIREXE
* Added: new AppLists class of functions and callbacks
* Added: new or missing App functions, callbacks, and enums
* Added: OverlayToWebPageMode enum and unread_chat_messages_changed callback for Friends class
* Added: new Input functions and callbacks
* Added: new Parental Settings fuctions, callback, and enums
* Added: new Remote Storage functions, callback, and enums
* Added: new UGC functions, callbacks, and enum
* Added: memory allocation corrections
* Changed: updated various Input class functions
* Changed: lots of argument names internally, has no effect on usage
* Fixed: some enum names
* Fixed: various server list filter functions in Matchmaking Servers class
* Fixed: receivedRelayAuthTicket, getGameCoordinatorServerLogin, FindRelayAuthTicketForServer in Networking Sockets class
* Removed: second call for steam_api.h in godotsteam.cpp

**Version 3.9.7 Changes**

* Added: two Matchmaking Server call results
* Added: requestHandle to various HTTP functions so handle can be passed
* Added: new internal variables for Matchmaking Servers
* Added: setSyncPlatforms in Remote Storage, actual function was missing
* Changed: serverRequest is now serverListRequest
* Changed: various HTTP callbacks now return cookieHandle as well
* Fixed: issue where lobby chat messages were truncated for every accented character used
* Fixed: pingServer, playerDetails, serverRules functions in Matchmaking Servers
* Fixed: receiveMessagesOnChannel, receiveMessagesOnPollGroup, receiveMessagesOnConnection in Networking Sockets, should now return an array of messages
* Fixed: connectByIPAddress, createSocketPair in Networking Sockets
* Fixed: network_messages_session_request callback, now provides identity of remote host
* Fixed: network_connection_status_changed callback, now provides the full connection information
* Removed: unnecessary bool from setLeaderboardDetailsMax

**Version 3.9.6 Changes**

* Added: ability to provide different locations for custom modules, thanks to dsnopek
* Changed: gamepad_text_input_dismissed to return submitted boolean and use UTF8 string, thanks to EIREXE

**Version 3.9.5 Changes**

* Added: setLeaderboardDetailsMax function back in to set the internal details variable
* Fixed: leaderboard_scores_downloaded sigal to provide the actual details for leaderboard results

**Version 3.9.4 Changes**

* Fixed: conversion issue in getGlobalStatInt and getGlobalStatIntHistory that caused compiling failure on Linux

**Version 3.9.3 Changes**

* Changed: restored getGlobalStatInt and getGlobalStatIntHistory functions

**Version 3.9.2 Changes**

* Added: getNextMostAchievedAchievementInfo function, moved out of getMostAchievedAchievementInfo
* Fixed: getMostAchievedAchievementInfo causing a crash

**Version 3.9.1 Changes**

* Added: documentation to P2P functions, constants, and signals; thanks to blaze-the-star
* Fixed: destroyResult and getResultItemProperty being bound to the wrong functions
* Fixed: incorrect function bind from pull request
* Removed: unused C++ line from config.py for Mac, which caused compiling issues
* Removed: storeStats from setAchievement, resetAllStats as it should be called manually after them
* Removed: requestCurrentStats from storeStats as it should be called manually

**Version 3.9 Changes**

* Added: new UGC functions addRequiredTagGroup, getQueryUGCNumTags, getQueryUGCTag, getQueryUGCTagDisplayName
* Added: new Friends function activateGameOverlayInviteDialogConnectString
* Added: default values to leaderboard functions, you can now pass handles for specific leaderboards or use the internally-stored, last-called handle
* Added: multiple controller types from Input function getInputTypeForHandle
* Changed: minor readability changes to function arguments and defaults
* Changed: additional spacing and readability to overall module
* Changed: replaced leaderboardDetailsMax with k_cLeaderboardDetailsMax
* Changed: applied EIREXE's UTF-16 fix module-wide
* Changed: minor corrections to comments and added missing comments
* Changed: some additional code to some call results and callbacks
* Changed: metadata length for UGC to 5000 from 255, thanks to EIREXE
* Changed: beginAuthSession to use new auth function arguments
* Changed: cancelAuthTicket to actually use the Steamworks function
* Fixed: renamed addItemToFavorite to addItemToFavorites to match SDK
* Fixed: incorrect class check in some UGC functions
* Fixed: minor corrections to various functions
* Fixed: destroyResult and getResultItemProperty being bound to the wrong functions
* Removed: setLeaderboardDetailsMax as it is unnecessary
* Removed: getAuthSessionTicketID as it is no longer useful due to auth function changes

**Version 3.8.2 Changes**

* Added: different avatar constants
* Changed: array deletions for Clang, _thanks to thisismike97_
* Changed: initializing char text, _thanks to thisismike97_
* Fixed: createQueryUserUGCRequest being commented out accidentally
* Fixed: logic check for setOverlayNotificationPosition, _thanks to thisismike97_
* Fixed: UTF8 not being handled correctly in some UGC functions, _thanks to EIREXE_

**Version 3.8.1 Changes**

* Added: extra newline beween each class section for readability
* Added: new signal, steamworks_error, currently used for call results failures
* Changed: cleared most items from to-do list
* Changed: getSyncPlatforms now returns a dictionary with the bitwise and full name version of the platform
* Changed: separated callbacks and call results in the godotsteam.cpp into two categories

**Version 3.8 Changes**

* Added: default argument to steamInit to pull all current stats or not, defaults to true so no one has to change anything
* Added: new SteamNetworkingMessages class; with functions, callbacks, constants, and enums
* Added: all missing functions due to 5 argument limit in Godot
* Changed: filterText updated to match new SDK 1.50 function
* Changed: HTTP class setCookie to setHTTPCookie to prevent confusion with HTML setCookie
* Changed: moved fileLoadDialogReponse into html_file_open_dialog callback as it must follow the call anyway
* Fixed: retrieveConnectionDetails and getAllLobbyData functions
* Fixed: (probably) various NetworkingSockets and NetworkingUtils functions

**Version 3.7 Changes**

* Added: Networking Sockets class - all functions, enums, structs, and callbacks (still beta in Steamworks)
* Added: Networking Utils class - all functions, enums, structs, and callbacks (still beta in Steamworks)
* Added: Game Search callbacks, enums, and functions
* Added: missing Steam Parties functions
* Changed: bIOFailure argument naming in godotsteam.cpp to ioFailure
* Fixed: issue where Cyrillic characters did not display correctly or at all
* Fixed: call result for JoinParty, was previously callback
* Removed: mingw_patch.py since it fixes one issue but creates additional issues

**Version 3.6.1 Changes**

* Changed: function getLobbyDataByIndex to getAllLobbyData
* Changed: commented out getAllLobbyData until crash is fixed
* Fixed: issue where not having the game installed or owning the game caused a crash

**Version 3.6 Changes**

* Added: newest functions for Apps, Friends, and UserStats
* Added: all functions and callbacks for Videos
* Added: MinGW patch file for people using MinGW, thanks to MichaelBelousov
* Added: all remaining Remote Storage, Utils, and Users functions and callbacks
* Changed: some User callbacks were actually call results
* Changed: moved callback code block to end of function block in godotsteam.cpp
* Fixed: incorrect signal link for unsubscribe_item and subscribe_item callbacks

**Version 3.5 Changes**

* Added: all Music Remote functions, callbacks, enums, and constants
* Added: all Parties functions, callbacks, enums, and constants
* Added: placeholders for function classes not added yet
* Changed: minor tweaks to layout, comments, etc.
* Changed: swapped getAuthSessionTicketID and getAuthSessionTicket to make more sense
* Changed: moved pragma into Windows if
* Fixed: getAuthSessionTicket to properly give buffer, thanks to _EIREXE_

**Version 3.4.1 Changes**

* Added: organization separators for all binds
* Changed: enums are now callable as constants in Godot
* Changed: case on enums
* Fixed: some mis-spelling in enums

**Version 3.4 Changes**

* Added: getAuthSessionTicketID to aquire additional ticket data
* Added: additional pragma to silence offset warnings in Steamworks SDK itself
* Changed: steamInit status results to use internal enums
* Changed: getLeaderboardSortMethod now returns a dictionary with result and verbal response
* Changed: getLeaderboardDisplayType now returns a dictionary with result and verbal response
* Changed: getLeaderboardEntries to have a default failure response
* Changed: leaderboard_scores_downloaded callback now incorporates getDownloadLeaderboardEntry to streamline process, callback returns the result array now
* Changed: complete overhaul of enums and constants
* Changed: leaderboardDetailsMax default from 0 to 10
* Fixed: various void functions
* Fixed: casting for addRequestLobbyListNumericalFilter, addRequestLobbyListStringFilter, addRequestLobbyListDistanceFilter
* Fixed: setItemTags thanks to _EIREXE_
* Fixed: missing publishedFileID in return from GetQueryUGCResult
* Fixed: getGlobalStatInt and getGlobalStatIntHistory
* Removed: getLeaderboardHandle as redundant
* Removed: getDownloadedLeaderboardEntry as it should not be called manually, has been added to leaderboard_scores_download callback

**Version 3.3.2 Changes**

* Added: all Inventory functions, callbacks, and enums
* Added: rule to suppress MSVC-only warning about strcpy
* Fixed: minor corrections to Inputs, especially those copied over from Controllers (depreciated)
* Fixed: tons of warnings for callbacks in Unix compiling
* Fixed: printf warnings for int
* Removed: unnecessary browserHandle argument from HTML functions
* Removed: unnecessary browserHandle returns from HTML callbacks
* Removed: unnecessary cookieHandle argument from HTTP functions
* Removed: unnecessary cookieHandle returns from HTTP callbacks

**Version 3.3.1 Changes**

* Added: all HTML Surface functions, callbacks, and enums
* Added: all HTTP functions, callbacks, and enums
* Changed: sendRemotePlayTogetherInvite now works since it was added back to the SDK
* Fixed: (probably) output for getLaunchCommandLine

**Version 3.3 Changes**

* Added: all Steam Input functions; used to be Steam Controller
* Added: all Steam Input constants
* Added: new Apps functions
* Added: missing Friends functions
* Added: missing Screenshots functions
* Added: all missing Screenshot constants
* Changed: removed Steam Controller as it is now depreciated
* Changed: split up call results and callbacks in [godotsteam.h] for editing ease
* Changed: user_stats_received to current_stats_received for requestCurrentStats callback / signal
* Changed: sorted Apps and Friends functions alphabetically like Steamworks Docs to find new functions easier
* Changed: getAchievementIcon; now returns the handle
* Changed: getInputTypeForHandle to output verbose controller type
* Changed: SteamInput function init to inputInit
* Changed: SteamInput function shutdown to inputShutdown
* Removed: user_achievement_icon_fetched signal / callback as it is never called

**Version 3.2.1 Changes**

* Added: back some needed UGC constants
* Changed: int to uint32 in some for loops
* Fixed: compiling issues on Linux

**Version 3.2.0 Changes**

* Added: all remaining UGC functions and callbacks
* Added: all new Remote Play functions and callbacks
* Added: remaining UGC constants and enums
* Added: relevant Remote Storage callbacks for UGC
* Added: back some needed UGC constants
* Changed: renamed some UGC enums for consistency
* Changed: getItemDownloadInfo to give proper default return
* Fixed: a few missing default returns
* Removed: non-listed UGC enums

**Version 3.1 Changes**

* Added: all remaining User Stats functions
* Added: missing User Stats constants, mostly leaderboard stuff
* Added: missing default return values in some functions
* Changed: getAchievementAndUnlockTime to return actual data
* Changed: user_achievement_icon_fetched callback to return icon data for parsing in-game
* Changed: value in D_METHOD for setLeaderboardDetailsMax to match function
* Changed: getDownloadedLeaderboardEntry to use handle correctly
* Fixed: delete used memory in getInstalledDepots

**Version 3.0.2 Changes**

* Added: more verbose response to steamInit, now returns a dictionary
* Added: missing initialization constants
* Changed: steamInit to give actual response on Steamworks status (from bool to int)
* Fixed: currentAppID not utilized correctly

**Version 3.0.1 Changes**

* Added: MacOS C++ rule back in for compiling
* Added: all missing Steam Utils functions (except depreciated or non-relevant functions)
* Added: additional ENUMS for Steam Utils
* Added: missing failure conditions for some Steam Utils functions
* Changed: output for getFriendGamePlayed to show game information even if no valid lobby
* Changed: order of previous Steam Utils functions to be alphabetical with new ones
* Changed: gamepad_text_input_dismissed callback
* Fixed: lobby_message bug, thanks to pull request from Frostings

**Version 3.0 Changes**

* Added: missing Matchmaking signals-callbacks
* Added: missing User signals-callbacks
* Added: missing Utility signals-callbacks
* Added: join_requested, screenshot_requested callback
* Changed: merged Godot 3.0.6 into Master branch
* Changed: callback descriptions updated
* Changed: organization of cpp and h files for better readability
* Changed: signal lobby_message_received to lobby_message
* Changed: server_connect and server_disconnected renamed to steam_server_connect and steam_server_disconnected respectively
* Changed: leaderboard_loaded, leaderboard_uploaded, and \_leaderboard_entries_loaded renamed to leaderboard_find_result, leaderboard_score_uploaded, and leaderboard_scores_downloaded respectively
* Changed: workshop_item_created, workshop_item_installed, and item_updated renamed to item_created, item_installed, and workshop_item_updated respectively
* Changed: renamed workshop to UGC to match Steamworks
* Fixed: addFavoriteGame and getItemInstallInfo functions
* Removed: connection_changed signal

**Version 2.8.5 Changes**

* Added: Networking functions; courtesy of Antokolos
* Changed: linked against Steamworks 1.44
* Fixed: leaderboard_uploaded always returning false even when successful

**Version 2.8.4 Changes**

* Added: persona_state_change callback
* Added: additional user statistics and achievement signals
* Added: join_game_requested signal by Fischer96
* Changed: dictionary term 'ret' to 'success' in getImageRGBA and getImageSize
* Changed: dictionary term 'buf' to 'buffer' in getImageRGBA
* Changed: getFriendAvatar to getPlayerAvatar
* Changed: avatar_loaded now sends back Steam ID of avatar by avencherus
* Fixed: lots of fixes by marcelofg55
* Fixed: issue with avatar and Steam ID on Linux compile
* Fixed: join_requested signal by Fischer96 (requires further work)
* Fixed: getImageRGBA
* Fixed: getDownloadedLeaderboardEntry returning wrong SteamID by marcelofg55
* Removed: drawAvatar

**Version 2.8.3 Changes**

* Added: additional user statistics and achievement signals
* Changed: minor notations

**Version 2.8.2 Changes**

* Fixed: Linux not compiling correctly with new Friends and Matchmaking updates
* Fixed: various Friends functions not providing correct data

**Version 2.8.1 Changes**

* Added: additional missing lobby signals
* Fixed: various lobby functions due to incorrect Steam lobby ID
* Fixed: some bind methods
* Fixed: config.py still using env
* Fixed: location of core Godot includes

**Version 2.8.0 Changes**

* Added: all remaining matchmaking functions
* Added: all remaining friend functions
* Changed: getRecentPlayers to include timestamp
* Changed: naming of leaderboard_handle and leaderboard_entries for consistency
* Changed: getAchievement to dictionary (courtesy of jandrewlong)
* Fixed: invite functions giving incorrect steam ids
* Fixed: getInstalledDepots, getDLCDownloadProgress, getItemUpdateProgress, getSubscribedItems
* Removed: setGameInfo, clearGameInfo, inviteFriend

**Version 2.7.0 Changes**

* Added: getAchievementDisplayAttribute, getAchievementName, getAchievementIcon, getImageRGBA, and getImageSize (courtesy of marcelofg55)
* Added: all missing SteamApps functions
* Changed: NULL statements for achievement functions
* Changed: cleaned up and organized signal functions in godotteam.h
* Fixed: issue with dictionary formatting in a function
* Fixed: missing NULL statement
* Fixed: issue with getAchievement failing to compile
* Removed: hasOtherApp function

**Version 2.6.0 Changes**

* Added: getCurrentBetaName, addScreenshotToLibrary, and setLocation (courtesy of marcelofg55)
* Added: Steam controller functionality (courtesy of marcelofg55)
* Added: more workshop functionality
* Changed: various small maintenance changes

**Version 2.5.0 Changes**

* Added: getFileNameAndSize, getQuota, getSyncPlatforms functions
* Changed: small corrections with Steam ID variable
* Fixed: small things with getQuota

**Version 2.4.1 Changes**

* Added: getNumAchievements, getAchievementAchievedPercent, requestGlobalAchievementPercentages functions
* Added: related signals to new functions
* Added: some notes
* Fixed: leaderboard_update signal
* Removed: depreciated function requestAppProofOfPurchaseKey
* Removed: related callback to requestAppProofOfPurchaseKey
* Removed: commented out depreciated functions

**Version 2.4.0 Changes**

* Added: more Screenshot features
* Added: notes for callback
* Fixed: types in \_validate_auth_ticket_response

**Version 2.3.0 Changes**

* Added: implemented Auth Session functions, thanks to marcelofg55

**Version 2.2.1 Changes**

* Fixed: getFileTimestamp and getSteamID return types, thanks to marcelofg55

**Version 2.2.0 Changes**

* Added: getNumberOfCurrentPlayers, thanks to marcelofg55
* Added: leaderboard_uploaded and number_of_current_players callbacks, thanks to marcelofg55

**Version 2.1.1 Changes**

* Fixed: fileRead and fileWrite, thanks to marcelofg55

**Version 2.1.0 Changes**

* Added: two more functions for Remote Storage
* Changed: instances of int32 and int64 to int32_t and int64_t respectively; mostly for Linux compilation
* Removed: -no-pie from SCsub; now suggested for Ubuntu 16.10 and higher

**Version 2.0.0 Changes**

* Added: Remote Storage functionality for Steam Cloud, thanks to marcelofg55
* Added: new functions to documentation
* Changed: SCsub file to include "no-pie" fix for Ubuntu 16.10 and higher