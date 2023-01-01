# Functions - Screenshots

------

## addScreenshotToLibrary

!!! function "addScreenshotToLibrary( ```string```filename, ```string```thumbnail_filename, ```int``` width, ```int``` height )"
    Adds a screenshot to the user's Steam screenshot library from disk. 

    **Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#AddScreenshotToLibrary){ .md-button .md-button--store target="_blank" }

## addVRScreenshotToLibrary

!!! function "addVRScreenshotToLibrary( ```int``` type, ```string``` filename, ```string``` vr_filename )"
    Adds a VR screenshot to the user's Steam screenshot library from disk in the supported type. 

    **Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#AddVRScreenshotToLibrary){ .md-button .md-button--store target="_blank" }

## hookScreenshots

!!! function "hookScreenshots( ```bool``` hook )"

    Toggles whether the overlay handles screenshots. 

    **Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#HookScreenshots){ .md-button .md-button--store target="_blank" }

## isScreenshotsHooked

!!! function "isScreenshotsHooked()"
    Checks if the app is hooking screenshots. 

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#IsScreenshotsHooked){ .md-button .md-button--store target="_blank" }

## setLocation

!!! function "setLocation( ```uint32_t``` screenshot, ```string``` location )"
    Sets optional metadata about a screenshot's location.

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#SetLocation){ .md-button .md-button--store target="_blank" }

## tagPublishedFile

!!! function "tagPublishedFile( ```uint32_t``` screenshot, ```uint64_t``` file_id )"
    Tags a published file as being visible in the screenshot. 

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#TagPublishedFile){ .md-button .md-button--store target="_blank" }

## tagUser

!!! function "tagUser( ```uint32_t``` screenshot, ```uint64_t``` steam_id )"

    Tags a Steam user as being visible in the screenshot. You can tag up to the value declared by k_nScreenshotMaxTaggedUsers in a single screenshot. Tagging more users than that will just be discarded. This function has a built in delay before saving the tag which allows you to call it repeatedly for each item. You can get the handle to tag the screenshot once it has been successfully saved from the [screenshot_ready](/signals/screenshots/#screenshot_ready) callback or via the [WriteScreenshot](/functions/screenshots/#writescreenshot), [AddScreenshotToLibrary](/functions/screenshots/#addscreenshottolibrary), [AddVRScreenshotToLibrary](/functions/screenshots/#addvrscreenshottolibrary) calls. 

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#TagUser){ .md-button .md-button--store target="_blank" }

## triggerScreenshot

!!! function "triggerScreenshot()"
    Causes Steam overlay to take a screenshot.

    **Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#TriggerScreenshot){ .md-button .md-button--store target="_blank" }

## writeScreenshot

!!! function "writeScreenshot( ```PoolByteArray``` rgb, ```int``` width, ```int``` height )"
    Writes a screenshot to the user's Steam screenshot library. 

    **Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#WriteScreenshot){ .md-button .md-button--store target="_blank" }
