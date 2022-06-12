# GodotSteam for GDNative
An open-source and fully functional Steamworks SDK / API module and plug-in for the Godot Game Engine (version 3.x). For the Windows, Linux, and Mac platforms. 

Additional flavors include:
- [Godot 2.x](https://github.com/Gramps/GodotSteam/tree/godot2)
- [Godot 3.x](https://github.com/Gramps/GodotSteam/tree/master)
- [Godot 4.x](https://github.com/Gramps/GodotSteam/tree/godot4)

Documentation
----------
[Documentation is available here](https://gramps.github.io/GodotSteam/) and [is mirrored on and exported from CoaguCo's site](https://coaguco.com/godotsteam).

Feel free to chat with us about GodotSteam on the [CoaguCo Discord server](https://discord.gg/SJRSq6K).

Current Build
----------
You can [download pre-compiled versions _(currently v3.3)_ of this repo here](https://github.com/Gramps/GodotSteam/releases).

**Version 3.3 Changes**
- Added: missing D_METHOD to all functions, should show the right argument names in-editor
- Added: Input origin enums for PS5 and Steam Deck
- Added: Input Types, Input Glyph Style, Input Glyph Size, and Input Configuration Enable Type enums
- Added: getConnectionRealTimeStatus, configureConnectionLanes, connectP2PCustomSignaling, receivedP2PCustomSignal, getCertificateRequest, setCertificate, resetIdentity, runNetworkingCallbacks, beginAsyncRequestFakeIP, getFakeIP, createListenScoketP2PFakeIP, getRemoveFakeIPForConnection, and createFakeUDPPort functions and callback to NetworkingSockets class
- Added: dismissFloatingGamepadTextInput function to Utils class
- Added: setTimeCreatedDateRange and setTimeUpdatedDateRange to UGC class
- Added: NetworkingeDebugOutputType enums for NetworkingUtils
- Added: missing constant binds for Server API, OverlayToWebPageMode
- Added: server branch merged in
- Fixed: minor compiler warnings
- Fixed: empty file hash being returned by file_details_result callback
- Fixed: a variety of small bugs and possible crashes, _thanks to qarmin_
- Fixed: missing binds for getFriendsGroupName, getFriendsGroupMembersList, getFriendsGroupIDByIndex, getFriendsGroupCount, getFriendMessage, getFriendCoplayTime, getFriendCoplayGame, getCoplayFriendCount, getCoplayFriend, getClanTag, getClanName, getClanCount, getClanChatMessage, getClanByIndex, getClanActivityCounts, fileWriteAsync, fileWriteStreamCancel, fileWriteStreamClose, fileWriteStreamOpen, fileWriteStreamWriteChunk, getCachedUGCCount, getUGCDownloadProgress, getUGCDetails, fileReadAsync, getOPFSettings, getOPFStringForApp, getVideoURL, isBroadcasting functions
- Fixed: setPNGIcon and updateCurrentEntryCoverArt in Music Remote class
- Fixed: missing getUGCDetails and getUGCDownloadProgress functions
- Changed: spacing in default arguments in godotsteam.h
- Changed: renamed STEAM_GAMESERVER_CALLBACK as STEAM_CALLBACK
- Changed: updated doc_class file for in-editor documentation
- Changed: updated to Steamworks 1.53
- Changed: lobby_data_update, removed lobby data queries as they should be done manually
- Changed: minor tweaks under-the-hood
- Changed: various generic 'int' to their actual types
- Changed: renamed servers and server stats to game server and game server stats respectively, to match SDK
- Changed: SteamNetworkingQuickConnectionStatus to SteamNetConnectionRealTimeStatus_t per Steamworks SDK 1.53, causes a break in previous GodotSteam versions
- Changed: getConfigValueInfo, removed name and next value from return dictionary as they are no longer passed by function in SDK 1.53
- Changed: rearranged functions in godotsteam.cpp class binds to match godotsteam.h order
- Changed: enum constant binds to match godotsteam.h enum order
- Removed: unused callback new_launch_query_parameters, broadcast_upload_start, broadcast_upload_stop
- Removed: allocateMessage as it shouldn't be used solo
- Removed: getQuickConnectionStatus and getFirstConfigValue as they were removed from SDK 1.53
- Removed: setDebugOutputFunction from Networking Utils
- Removed: unused structs
- Removed: SteamGameServer_RunCallbacks function

**Version 3.2 Changes**
- Added: new helper functions for newer networking classes, translations for steamnetworkingtypes
- Changed: now works in Godot 3.4
- Fixed: various compiler warnings

**Version 3.1.1 Changes**
- Removed: not logged in as a failure condition for steamInit

**Version 3.1 Changes**
- Added: various Steam Deck specific functions, _thanks to EIREXE_
- Added: new AppLists class of functions and callbacks
- Added: new or missing App functions, callbacks, and enums
- Added: OverlayToWebPageMode enum and unread_chat_messages_changed callback for Friends class
- Added: new Input functions and callbacks
- Added: new Parental Settings fuctions, callback, and enums
- Added: new Remote Storage functions, callback, and enums
- Added: new UGC functions, callbacks, and enum
- Added: memory allocation corrections
- Changed: updated various Input class functions
- Changed: lots of argument names internally, has no effect on usage
- Fixed: some enum names
- Fixed: various server list filter functions in Matchmaking Servers class
- Fixed: getGameCoordinatorServerLogin in Networking Sockets class
- Removed: receiveRelayAuthTicket, findRelayAuthTicketForServer, getHostedDedicatedServerAddress, and getGameCoordinatorServerLogin as they rely on datagram header that was removed from base SDK
- Removed: second call for steam_api.h in godotsteam.cpp

**Version 3.0.2 Changes**
- Added: for the add-on only, new production-ready API libs were made

**Version 3.0.1 Changes**
- Fixed: two issues with godotsteam.cpp that causes compiling error on Linux

**Version 3.0.0 Changes**
- Added: all missing functions to bring GDNative version in-line with module version
- Changed: rebuilt and restructured layout of project folder
- Removed: enums as they don't work in GDNative
- Removed: default arguments for functions as they don't work in GDNative

Known Issues
----------
- The GDNative version does not allow for default arguments in functions, thus some functions may have odd behaviors.  If you are using this version of GodotSteam you are required to pass any argument that has a default in the module version. Also, there are no enums in the GDNative version due to how it is structured.

Quick How-To
----------
Obtain the plugin through one of two ways:
- Visit the Godot Asset Library either through the website or in the editor and search for GodotSteam.
- Download this repo and unzip it into the base of your game project.

Go into **Project > Settings > Plugins** and activate the plugin.

Tinker with Steamworks!

Donate
-------------
Pull-requests are the best way to help the project out but you can also donate through [Patreon](https://patreon.com/coaguco) or [Paypal](https://www.paypal.me/sithlordkyle)!

License
-------------
MIT license
