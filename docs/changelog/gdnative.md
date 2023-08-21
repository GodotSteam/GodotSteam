# GDNative Change-Log

### A history of all changes to the **gdnative** and **gdnative-plugin** branches.

------

**Version 3.20.1 Changes**

- Fixed: wrong variant type for join_requested

**Version 3.20 Changes**

- Added: steamShutdown to allow Steamworks to be manually shutdown
- Added: requestEquippedProfileItems function and equipped_profile_items callback
- Added: Steam Deck as Steam Input type
- Changed: all enums are now directly linked to their SDK counterparts
- Changed: getDigitalActionData returned keys are now state and active
- Changed: names of some Steam enums to be cleaner and leaner
- Changed: getAppInstallDir now returns dictionary with absolute path and install size
- Fixed: missing function argument binds
- Removed: enums that are not in the SDK but Valve's docs

**Version 3.6.2 Changes**

- Added: new Input callback _input_gamepad_slot_change_
- Added: new User callback _get_ticket_for_web_api_
- Added: new User function _getAuthTicketForWebApi_
- Fixed: lobby_match_list callback, but no sends the lobby count along with the lobby list array (only in GDNative due to weird GDNative bug)
- Changed: getAuthSessionTicket argument is now optional, defaults to NULL

**Version 3.6.1 Changes**

- Added: new return values for _overlay_toggled_; this will break compatibility with this
- Added: new UGC functions _removeContentDescriptor_, _addContentDescriptor_, and _getQueryUGCContentDescriptors_
- Added: new signal _filter_text_dictionary_changed_
- Changed: getAuthSessionTicket now uses networking identities
- Changed: gamepad_text_input_dismissed now passes back the app ID
- Changed: Steam Input max analog and digital actions values
- Fixed: lobby_match_list callback, but it now passes back the lobby count too
- Removed: ERegisterActivationCodeResult due to removal in SDK

**Version 3.6 Changes**

* Changed: brought the plug-in version up to speed with the module version

**Version 3.5.1 Changes**

* Changed: createSteamID function to use proper 64-bit int
* Changed: brought the plug-in version up to speed with the module version
* Changed: output from subscribe_item, unsubscribe_item file_id to uint64_t
* Fixed: some new compiling quirks
* Removed: setIdentitySteamID and getIdentitySteamID

**Version 3.5 Changes**

* Fixed: incorrect type for getAppListBuildId

**Version 3.4.1 Changes**

* Fixed: OSX naming of DyLib files

**Version 3.4 Changes**

* Added: setDLCContext, getProfileItemPropertyString, getProfileItemPropertyInt, hasEquippedProfileItem, getPublicIP, getPSNID, getStadiaID, getXboxPairwiseID functions
* Added: handle arguments to HTML Surface class functions
* Added: inventory handle arguments to Inventory class functions
* Added: networking identity references into new Networking class functions
* Added: equipped_profile_items_changed, equipped_profile_items callbacks
* Added: additional data types to signals
* Changed: many functions in Networking Sockets and Networking Types classes
* Changed: newtorking identity maps
* Changed: various argument names to fall in line with module version
* Changed: renamed some signals
* Fixed: serverInit, advertiseGame functions
* Fixed: getSessionID using wrong type in Remote Play class
* Fixed: various incorrect types in Screenshots class
* Fixed: wrong int type for inventory update handle
* Fixed: not casting app ID for addFavoriteGame
* Fixed: wrong int type for server ID in getLobbyGameServer
* Removed: requestAllProofOfPurchaseKeys, requestAppProofOfPurchaseKey functions and related callbacks
* Removed: leading dash from signal / callback function names

**Version 3.3 Changes**

* Added: missing D_METHOD to all functions, should show the right argument names in-editor
* Added: Input origin enums for PS5 and Steam Deck
* Added: Input Types, Input Glyph Style, Input Glyph Size, and Input Configuration Enable Type enums
* Added: getConnectionRealTimeStatus, configureConnectionLanes, connectP2PCustomSignaling, receivedP2PCustomSignal, getCertificateRequest, setCertificate, resetIdentity, runNetworkingCallbacks, beginAsyncRequestFakeIP, getFakeIP, createListenScoketP2PFakeIP, getRemoveFakeIPForConnection, and createFakeUDPPort functions and callback to NetworkingSockets class
* Added: dismissFloatingGamepadTextInput function to Utils class
* Added: setTimeCreatedDateRange and setTimeUpdatedDateRange to UGC class
* Added: NetworkingeDebugOutputType enums for NetworkingUtils
* Added: missing constant binds for Server API, OverlayToWebPageMode
* Added: server branch merged in
* Fixed: minor compiler warnings
* Fixed: empty file hash being returned by file_details_result callback
* Fixed: a variety of small bugs and possible crashes, thanks to qarmin
* Fixed: missing binds for getFriendsGroupName, getFriendsGroupMembersList, getFriendsGroupIDByIndex, getFriendsGroupCount, getFriendMessage, getFriendCoplayTime, getFriendCoplayGame, getCoplayFriendCount, getCoplayFriend, getClanTag, getClanName, getClanCount, getClanChatMessage, getClanByIndex, getClanActivityCounts, fileWriteAsync, fileWriteStreamCancel, fileWriteStreamClose, fileWriteStreamOpen, fileWriteStreamWriteChunk, getCachedUGCCount, getUGCDownloadProgress, getUGCDetails, fileReadAsync, getOPFSettings, getOPFStringForApp, getVideoURL, isBroadcasting functions
* Fixed: setPNGIcon and updateCurrentEntryCoverArt in Music Remote class
* Fixed: missing getUGCDetails and getUGCDownloadProgress functions
* Changed: spacing in default arguments in [godotsteam.h]
* Changed: renamed STEAM_GAMESERVER_CALLBACK as STEAM_CALLBACK
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
* Removed: unused structs
* Removed: SteamGameServer_RunCallbacks function

**Version 3.2 Changes**

* Added: new helper functions for newer networking classes, translations for steamnetworkingtypes
* Changed: now works in Godot 3.4
* Fixed: various compiler warnings

**Version 3.1.1 Changes**

* Removed: not logged in as a failure condition for steamInit

**Version 3.1**

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
* Fixed: getGameCoordinatorServerLogin in Networking Sockets class
* Removed: receiveRelayAuthTicket, findRelayAuthTicketForServer, getHostedDedicatedServerAddress, and getGameCoordinatorServerLogin as they rely on datagram header that was removed from base SDK
* Removed: second call for steam_api.h in godotsteam.cpp

**Version 3.0.1**

* Fixed: two issues with godotsteam.cpp that causes compiling error on Linux

**Version 3.0.0**

* Added: all missing functions to bring GDNative version in-line with module version
* Changed: rebuilt and restructured layout of project folder
* Removed: enums as they don't work in GDNative
* Removed: default arguments for functions as they don't work in GDNative

**Version 2.1.0**

* Added: Steamworks P2P functions thanks to Antokolos

**Version 2.0.0**

* Added: all current GodotSteam functions and signals
* Added: [godotsteam.h] file
* Changed: compile process; big thanks to willnationsdev
* Changed: various parts of the CPP files
* Removed: pre-compiled TSCN and TRES files

**Version 1.1.0**

* Added: [godotsteam.h] file
* Added: getCurrentGameLanguage
* Added: Pre-compiled engines and templates for Linux, Mac, and Windows
* Added: change log to documentation
* Changed: various parts of the CPP files
* Changed: minor things in godotsteam.cpp
* Removed: pre-compiled TSCN and TRES files

**Version 1.0.0**

* Added: most pre-existing GodotSteam code over
* Added: GodotSteam GDNative documentation
* Changed: SConstruct file from GDNative to support architecture recognition or accept bit arguement in SCONS
* Bugs: Windows does not work for compiling yet, waiting for GDNative update
