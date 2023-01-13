# Functions - Apps

------

## getAppBuildId

!!! function "getAppBuildId()"
    Gets the buildid of this app, may change at any time based on backend updates to the game. 

    **Return**: int

    The current build ID of this app. Defaults to 0 if you're not running a build downloaded from Steam.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsTimedTrial){ .md-button .md-button--store target="_blank" }


## getAppInstallDir

!!! function "getAppInstallDir( ```uint32_t``` app_id )"
    Gets the install folder for a specific app ID. This works even if the application is not installed, based on where the game would be installed with the default Steam library location. 

    **Return**: string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetAppInstallDir){ .md-button .md-button--store target="_blank" }

## getAppOwner

!!! function "getAppOwner()"
    Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.

    **Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetAppOwner){ .md-button .md-button--store target="_blank" }

## getAvailableGameLanguages

!!! function "getAvailableGameLanguages()"
    Gets a comma separated list of the languages the current app supports.

    **Return:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetAvailableGameLanguages){ .md-button .md-button--store target="_blank" }

## getCurrentBetaName

!!! function "getCurrentBetaName()"
    Checks if the user is running from a beta branch, and gets the name of the branch if they are.

    **Return:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetCurrentBetaName){ .md-button .md-button--store target="_blank" }

## getCurrentGameLanguage

!!! function "getCurrentGameLanguage()"
    Gets the current language that the user has set.

    **Return:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetCurrentGameLanguage){ .md-button .md-button--store target="_blank" }

## getDLCCount

!!! function "getDLCCount()"
    Get the number of DLC the user owns for a parent application/game. 

    **Return:** int32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetDLCCount){ .md-button .md-button--store target="_blank" }

## getDLCDataByIndex

!!! function "getDLCDataByIndex()"
    Returns metadata for a DLC by index. 

    **Return**: array

    Contains dictionaries (dlc) which contain the following keys:

    * app_id (int)
    * available (bool)
    * name (string)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BGetDLCDataByIndex){ .md-button .md-button--store target="_blank" }

## getDLCDownloadProgress

!!! function "getDLCDownloadProgress( ```uint32_t``` dlc_id )"
    Gets the download progress for optional DLC.

    **Returns:** dictionary

    Contains the following keys:

    * ret (bool)
    * downloaded (uint64_t)
    * total (uint64_t)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetDlcDownloadProgress){ .md-button .md-button--store target="_blank" }

## getEarliestPurchaseUnixTime

!!! function "getEarliestPurchaseUnixTime( ```uint32_t``` app_id )"
    Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).

    **Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetEarliestPurchaseUnixTime){ .md-button .md-button--store target="_blank" }

## getFileDetails

!!! function "getFileDetails( ```string``` filename )"
    Asynchronously retrieves metadata details about a specific file in the depot manifest. 

    **Return:** void

    **Notes:** Triggers a [file_details_result callback.](/signals/apps/#file_details_result)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetFileDetails){ .md-button .md-button--store target="_blank" }

## getInstalledDepots

!!! function "getInstalledDepots( ```uint32_t``` app_id )"
    Gets a list of all installed depots for a given App ID. 

    **Return**: array

    Contains the installed depots, returned in mount order. 

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetInstalledDepots){ .md-button .md-button--store target="_blank" }

## getLaunchCommandLine

!!! function "getLaunchCommandLine()"
    Gets the command line if the game was launched via Steam URL, e.g. ```steam://run/<appid>//<command line>/```. This method is preferable to launching with a command line via the operating system, which can be a security risk. In order for rich presence joins to go through this and not be placed on the OS command line, you must enable "Use launch command line" from the Installation > General page on your app.

    **Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetLaunchCommandLine){ .md-button .md-button--store target="_blank" }

## getLaunchQueryParam

!!! function "getLaunchQueryParam( ```string``` key )"
    Gets the associated launch parameter if the game is run via ```steam://run/<appid>/?param1=value1;param2=value2;param3=value3``` etc. 

    **Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#GetLaunchQueryParam){ .md-button .md-button--store target="_blank" }

## installDLC

!!! function "installDLC( ```uint32_t``` dlc_id )"
    Allows you to install an optional DLC.

    **Returns:** void

    **Note:** Triggers a [dlc_installed callback.](/signals/apps/#dlc_installed)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#InstallDLC){ .md-button .md-button--store target="_blank" }


## isAppInstalled

!!! function "isAppInstalled( ```uint32_t``` app_id )"
    Checks if a specific app is installed. The app may not actually be owned by the current user, they may have it left over from a free weekend, etc. This only works for base applications, not downloadable content (DLC). Use [isDLCInstalled](https://partner.steamgames.com/doc/api/ISteamApps#BIsAppInstalled){ target="_blank" } for DLC instead. 

    **Return**: bool

    True if the specified app ID is installed; otherwise, false.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsAppInstalled){ .md-button .md-button--store target="_blank" }

## isDLCInstalled

!!! function "isDLCInstalled( ```uint32_t``` dlc_id )"
    Checks if the user owns a specific DLC and if the DLC is installed. 

    **Return**: bool

    True if the user owns the DLC and it's currently installed, otherwise false.

    **Note**: Should only be used for simple client side checks - not intended for granting in-game items

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsDlcInstalled){ .md-button .md-button--store target="_blank" }

## isLowViolence

!!! function "isLowViolence()"
    Checks if the license owned by the user provides low violence depots. 

    **Return**: bool

    True if the user owns the DLC and it's currently installed, otherwise false.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsLowViolence){ .md-button .md-button--store target="_blank" }

## isSubscribed

!!! function "isSubscribed()"
    Checks if the active user is subscribed to the current app ID. 

    **Return**: bool

    True if the active user owns the current app ID, otherwise false.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsSubscribed){ .md-button .md-button--store target="_blank" }

## isSubscribedApp

!!! function "isSubscribedApp( ```uint32_t``` app_id )"
    Checks if the active user is subscribed to a specified app ID. Only use this if you need to check ownership of another game related to yours, a demo for example. 

    **Return**: bool

    True if the active user is subscribed to the specified app ID, otherwise false.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsSubscribedApp){ .md-button .md-button--store target="_blank" }

## isSubscribedFromFamilySharing

!!! function "isSubscribedFromFamilySharing()"
    Checks if the active user is accessing the current app ID via a temporary Family Shared license owned by another user. 

    **Return**: bool

    True if the active user is accessing the current appID via family sharing, otherwise false.

    If you need to determine the Steam ID of the permanent owner of the license, use [getAppOwner](/functions/apps/#getappowner).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsSubscribedFromFamilySharing){ .md-button .md-button--store target="_blank" }


## isSubscribedFromFreeWeekend

!!! function "isSubscribedFromFreeWeekend()"
    Checks if the user is subscribed to the current app through a free weekend. This function will return false for users who have a retail or other type of license. Suggested you contact Valve on how to package and secure your free weekend properly. 

    **Return**: bool

    True if the active user is subscribed to the current app ID via a free weekend otherwise false any other type of license.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsSubscribedFromFreeWeekend){ .md-button .md-button--store target="_blank" }


## isTimedTrial

!!! function "isTimedTrial()"
    Checks if the user is subscribed to the current app ID through a timed trial. If so, returns true and gives back the total time the timed trial is allowed to play, along with the current amount of time the user has played. 

    **Return**: dictionary

    Containing these keys:

    * seconds_allowed (uint32)
    * seconds_played (uint32)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsTimedTrial){ .md-button .md-button--store target="_blank" }

## isVACBanned

!!! function "isVACBanned()"
    Checks if the user has a VAC ban on their account. 

    **Return**: bool

    True if the user has a VAC ban on their account; otherwise, false.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#BIsVACBanned){ .md-button .md-button--store target="_blank" }


## markContentCorrupt

!!! function "markContentCorrupt( ```bool``` missing_files_only )"
    Allows you to force verify game content on next launch.

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#MarkContentCorrupt){ .md-button .md-button--store target="_blank" }

## setDLCContext

!!! function "setDLCContext( ```uint32_t``` app_id )"
    Set current DLC app ID being played (or 0 if none). Allows Steam to track usage of major DLC extensions.

    **Returns:** bool

## uninstallDLC

!!! function "uninstallDLC( ```uint32_t``` dlc_id )"
    Allows you to uninstall an optional DLC.

    **Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#UninstallDLC){ .md-button .md-button--store target="_blank" }