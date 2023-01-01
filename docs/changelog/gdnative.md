# GDNative Change-Log

### A history of all changes to the **gdnative** and **gdnative-plugin** branches.

------

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
* Added: Input origin enums for PS5 and [Steam](/GodotSteam/Classes/classSteam/) Deck
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

* Added: various [Steam](/GodotSteam/Classes/classSteam/) Deck specific functions, thanks to EIREXE
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

</section> [__](#)

 <nav id="menu"> <header class="major"> 
## Menu

 </header> 

* 
[Introduction](index.html)

* How-To's 
    * [How-To Godot 2, 3, 4](howto-module.html)
    * [How-To GDNative](howto-gdnative.html)
* Functions 
    * [Main](functions-main.html)
    * [Apps](functions-apps.html)
    * [App Lists](functions-apps-list.html)
    * [Friends](functions-friends.html)
    * [Game Search](functions-game-search.html)
    * [Game Server](functions-game-server.html)
    * [Game Server Stats](functions-game-server-stats.html)
    * [HTML Surface](functions-html-surface.html)
    * [HTTP](functions-http.html)
    * [Input](functions-input.html)
    * [Inventory](functions-inventory.html)
    * [Matchmaking](functions-matchmaking.html)
    * [Matchmaking Servers](functions-matchmaking-servers.html)
    * [Music](functions-music.html)
    * [Music Remote](functions-music-remote.html)
    * [Networking](functions-networking.html)
    * [Networking Messages](functions-networking-messages.html)
    * [Networking Sockets](functions-networking-sockets.html)
    * [Networking Types](functions-networking-types.html)
    * [Networking Utils](functions-networking-utils.html)
    * [Parental Settings](functions-parental-settings.html)
    * [Parties](functions-parties.html)
    * [Remote Play](functions-remote-play.html)
    * [Remote Storage](functions-remote-storage.html)
    * [Screenshots](functions-screenshots.html)
    * [UGC](functions-ugc.html)
    * [User](functions-user.html)
    * [User Stats](functions-user-stats.html)
    * [Utils](functions-utils.html)
    * [Video](functions-video.html)
* Signals 
    * [Main](signals-main.html)
    * [Apps](signals-apps.html)
    * [Apps List](signals-apps-list.html)
    * [Friends](signals-friends.html)
    * [Game Search](signals-game-search.html)
    * [Game Server](signals-game-server.html)
    * [Game Server Stats](signals-game-server-stats.html)
    * [HTML Surface](signals-html-surface.html)
    * [HTTP](signals-http.html)
    * [Input](signals-input.html)
    * [Inventory](signals-inventory.html)
    * [Matchmaking](signals-matchmaking.html)
    * [Matchmaking Servers](signals-matchmaking-servers.html)
    * [Music Remote](signals-music-remote.html)
    * [Networking](signals-networking.html)
    * [Networking Sockets](signals-networking-sockets.html)
    * [Networking Utils](signals-networking-utils.html)
    * [Parental Settings](signals-parental-settings.html)
    * [Parties](signals-parties.html)
    * [Remote Play](signals-remote-play.html)
    * [Remote Storage](signals-remote-storage.html)
    * [Screenshots](signals-screenshots.html)
    * [UGC](signals-ugc.html)
    * [User](signals-user.html)
    * [User Stats](signals-user-stats.html)
    * [Utils](signals-utils.html)
    * [Video](signals-video.html)
* Godot 3.x Tutorials 
    * [Initializing Steam](tutorials-initializing.html)
    * [Stats and Achievements](tutorials-stats-achievements.html)
    * [Leaderboards](tutorials-leaderboards.html)
    * [Avatars](tutorials-avatars.html)
    * [Lobbies / P2P Networking](tutorials-lobbies-p2p.html)
    * [Authentication](tutorials-authentication.html)
    * [Mac Exporting](tutorials-mac-export.html)
    * [Achievement Icons](tutorials-achievement-icons.html)
    * [Auto-Matchmaking](tutorials-auto-matchmaking.html)
    * [Rich Presence](tutorials-rich-presence.html)
    * [Linux Caveats](tutorials-linux-caveats.html)
    * [Exporting and Shipping](tutorials-exporting-shipping.html)
    * [Common Issues](tutorials-common-issues.html)
    * [Remove Steam](tutorials-remove-steam.html)
* [Games Using GodotSteam](games.html)
* [To-Do](https://github.com/Gramps/GodotSteam/projects/1.html)
* Change Log 
    * [Godot 4.x](changelog-godot4.html)
    * [Godot 3.x](changelog-godot3.html)
    * [Godot 2.x](changelog-godot2.html)
    * [GDNative](changelog-gdnative.html)
    * [Server](changelog-server.html)
* [Contributors](contributors.html)

 </nav> <section> <header class="major"> 
## Stuck? Need Help?

 </header> 

Navigating Steamworks can be confusing sometimes. If you run across any problems, there's a few ways to get it ironed out.



* 
[__&nbsp;&nbsp;Send A Support E-Mail](mailto:godotsteam@coaguco.com)

* [__&nbsp;&nbsp;Chat With Us On Discord](https://discord.gg/SJRSq6K)

 </section> <section> <header class="major"> 
## Donate

 </header> 

If C++ is not your bag but you're feeling generous, you can support the project with your pocket change using either button below!



* 
[__&nbsp;&nbsp;with Github Sponsors](https://github.com/sponsors/Gramps)

* [__&nbsp;&nbsp;with Patreon](https://patreon.com/coaguco)
* [__&nbsp;&nbsp;with Paypal](https://www.paypal.me/sithlordkyle)

 </section> <footer> 

* 

Godot Engine by [Juan Linietsky, Ariel Manzur and contributors](https://godotengine.org)

* Steamworks API by [Valve Corporation](https://valvesoftware.com)
* GodotSteam by [GP Garcia](https://coaguco.com) and contributors
* Site Template by [HTML5UP](https://html5up.net)

 </footer> <footer id="footer" class="copyright"> 2022  GP Garcia & CoaguCo Industries </footer>   

<script src="js/jquery.min.js"></script> <script src="js/browser.min.js"></script> <script src="js/breakpoints.min.js"></script> <script src="js/util.js"></script> <script src="js/main.js"></script> <script src="js/godotsteam.js"></script> </body> </html> 

-------------------------------

Updated on 2022-11-24 at 12:17:42 -0600
