# Signals - Apps

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

------

## dlc_installed

!!! function "dlc_installed"
    Triggered after the current user gains ownership of DLC and that DLC is installed.

    **Returns:**

    * app_id (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#DlcInstalled_t){ .md-button .md-button--store target="_blank" }

## file_details_result

!!! function "file_details_result"
    Called after requesting the details of a specific file.

    **Returns:**
    
    * result (uint32_t)
    * file_size (uint64_t)
    * file_hash (uint8)
    * flags (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#FileDetailsResult_t){ .md-button .md-button--store target="_blank" }

## new_launch_url_parameters

!!! function "new_launch_url_parameters"
    Triggered after the user executes a steam url with command line or query parameters such as ```steam://run/(app_id)//?param1=value1;param2=value2;param3=value3;``` while the game is already running. The new params can be queried with [getLaunchCommandLine](/functions/apps/#getlaunchcommandline) and [getLaunchQueryParam](/functions/apps/#getlaunchqueryparam).
    
    **Returns:** nothing

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#NewUrlLaunchParameters_t){ .md-button .md-button--store target="_blank" }

## timed_trial_status

!!! function "timed_trials_status"
    Called for games in Timed Trial mode.
    
    **Returns:**

    * app_id (uint32_t)
    * is_offline (bool)
    * seconds_allowed (uint32)
    * seconds_played (uint32)
    
    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamApps#TimedTrialStatus_t){ .md-button .md-button--store target="_blank" }
