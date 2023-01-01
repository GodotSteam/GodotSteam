# Functions - Main

------

## isSteamRunning

!!! function "isSteamRunning()"
    Check if the Steam client is running. 

    **Return:** bool

    Naturally, returns true if the Steam client is running.

    **Note:** While this is not listed in the Steamworks docs, it is in the SDK. 

## restartAppIfNecessary

!!! function "restartAppIfNecessary( ```uint32_t``` app_id )"
    Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't.

    **Return:** bool

    If this returns true then it starts the Steam client if required and launches your game again through it, and you should quit your process as soon as possible. If it returns false, then your game was launched by the Steam client and no action needs to be taken.

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RestartAppIfNecessary){ .md-button .md-button--store target="_blank" }

## run_callbacks

!!! function "run_callbacks()"
    Enables your application to receive callbacks from Steamworks.  Must be placed in your \_process function.

    **Returns:** void

## steamInit

!!! function "steamInit( ```bool``` retrieve_stats = true )"
    Starts up the Steam API.

    **Return:** dictionary

    * status (int)
    * verbal (string) 
    
    You will receive one of three results:
    
    * 1 / "Steamworks active"
    * 2 / "Steamworks failed to initialize"
    * 20 / "Steam not running"
    * 79 / "Invalid app ID or app not installed"

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/steam_api#SteamAPI_Init){ .md-button .md-button--store target="_blank" }