# Functions - App Lists

------

**Note:** This is a restricted interface that can only be used by previously approved apps, contact your Steam Account Manager if you believe you need access to this API. It is not listed in the official Steamworks SDK documentation either.

## getAppListBuildId

!!! function "getAppListBuildId( ```uint32_t``` app_id )"
    Get a given app ID's build. 

    **Returns:** int

## getAppListInstallDir

!!! function "getAppListInstallDir( ```uint32_t``` app_id, ```int``` name_max )"
    Get a given app ID's install directory.

    **Returns:** string

## getAppName

!!! function "getAppName( ```uint32_t``` app_id, ```int``` name_max )"
    Get a given app ID's name. 

    **Returns:** string

## getInstalledApps

!!! function "getInstalledApps( ```uint32``` max_app_ids )"
    Get a list of app IDs for installed apps for this player. 

    **Return:** array

    Contains a list of (int) app IDs.

## getNumInstalledApps

!!! function "getNumInstalledApps()"
    Get the number of installed apps for this player. 

    **Return:** int

    **Note:** This is a restricted interface that can only be used by previously approved apps, contact your Steam Account Manager if you believe you need access to this API.