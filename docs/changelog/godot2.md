# Godot 2.x Change-Log

### A history of all changes to the **godot2** branch.

------

**Version 1.10 Changes**

* Added: more verbose response to steamInit, now returns a dictionary
* Added: missing initialization constants
* Changed: steamInit to give actual response on Steamworks status (from bool to int)
* Changed: minor formatting to match Godot 3 version
* Fixed: currentAppID not utilized correctly
* Removed: unneeded gameInfo struct

**Version 1.9.3 Changes**

* Added: networking functions, big thanks to Antokolos
* Added: compiling flag for Linux and Mac, need for new Steamworks 1.44
* Changed: linked against Steamworks 1.44
* Fixed: issue with leaderboard upload returning false no matter what

**Version 1.9.2 Changes**

* Added: persona_state_change callback
* Changed: getFriendAvatar to getPlayerAvatar
* Changed: avatar_loaded now sends back Steam ID of avatar by avencherus
* Fixed: issue with avatar and Steam ID on Linux compile
* Fixed: join_requested signal by Fischer96 (requires further work)

**Version 1.9.1 Changes**

* Added: additional user statistics and achievement signals
* Changed: minor notations
* Fixed: Linux not compiling correctly with new Friends and Matchmaking updates
* Fixed: various Friends functions not providing correct data
* Fixed: missing bind methods for integers

**Version 1.9.0 Changes**

* Added: all remaining matchmaking functions
* Added: all remaining friend functions
* Changed: getRecentPlayers to include timestamp
* Changed: naming of leaderboard_handle and leaderboard_entries for consistency
* Changed: getAchievement to dictionary (courtesy of jandrewlong)
* Fixed: invite functions giving incorrect steam ids
* Fixed: getInstalledDepots, getDLCDownloadProgress, getItemUpdateProgress, getSubscribedItems
* Removed: setGameInfo, clearGameInfo, inviteFriend

**Version 1.8.0 Changes**

* Added: getAchievementDisplayAttribute, getAchievementName, getAchievementIcon, getImageRGBA, and getImageSize (courtesy of marcelofg55)
* Added: all missing SteamApps functions
* Changed: NULL statements for achievement functions
* Changed: cleaned up and organized signal functions in godotteam.h
* Fixed: issue with getAchievement failing to compile
* Removed: hasOtherApp function

**Version 1.7.0 Changes**

* Added: getCurrentBetaName, addScreenshotToLibrary, and setLocation (courtesy of marcelofg55)
* Added: Steam controller functionality (courtesy of marcelofg55)
* Added: more workshop functionality
* Changed: various small maintenance changes
* Fixed: compiling error on Linux

**Version 1.6.0 Changes**

* Added: getFileNameAndSize, getQuota, getSyncPlatforms functions
* Changed: small corrections with Steam ID variable
* Fixed: small things with getQuota

**Version 1.5.1 Changes**

* Added: getNumAchievements, getAchievementAchievedPercent, requestGlobalAchievementPercentages functions
* Added: related signals to new functions
* Added: minor notes
* Removed: is_valid for updateLeaderboardHandle
* Removed: depreciated function requestAppProofOfPurchaseKey
* Removed: callback for requestAppProofOfPurchaseKey
* Removed: commented out depreciated functions

**Version 1.5.0 Changes**

* Added: more Screenshot features
* Added: notes for callback
* Changed: documentation to reflect new features and signals
* Fixed: types in _validate_auth_ticket_response_

**Version 1.4.0 Changes**

* Added: Auth Session functions added to Godot 3 branch thanks to marcelofg55
* Added: Auth Session constants

**Version 1.3.0 Changes**

* Added: getNumberOfCurrentPlayers, thanks to marcelofg55
* Added: leaderboard_uploaded and number_of_current_players callbacks, thanks to marcelofg55
* Changed: signals in [godotsteam.h] with CCallback instead of SteamCallback

**Version 1.2.1 Changes**

* Changed: instances of int32 and int64 to int32_t and int64_t respectively; mostly for Linux compilation
* Changed: readme to reflect Godot 3 release

**Version 1.2.0 Changes**

* Added: Remote Storage functionality for Steam Cloud, thanks to marcelofg55
* Added: new functions to documentation
* Changed: SCsub file to include "no-pie" fix for Ubuntu 16.10 and higher

**Version 1.1.0 Changes**

* Added: getCurrentGameLanguage
* Added: Pre-compiled engines and templates for Windows
* Added: change log to documentation
* Changed: minor things in godotsteam.cpp 