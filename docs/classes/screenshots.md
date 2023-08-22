# Screenshots

Functions for adding screenshots to the user's screenshot library.  
  
See [Steam Screenshots](https://partner.steamgames.com/doc/features/screenshots){ target="_blank" } for more information.

---

## Functions

------

### addScreenshotToLibrary

!!! function "addScreenshotToLibrary( ```string``` filename, ```string``` thumbnail_filename, ```int``` width, ```int``` height )"
    Adds a screenshot to the user's Steam screenshot library from disk.

	Triggers a [screenshot_ready](#screenshot_ready) callback.

    **Returns:** uint32_t

	- Returns `0` if the screenshot could not be saved.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#AddScreenshotToLibrary){ .md-button .md-button--store target="_blank" }

### addVRScreenshotToLibrary

!!! function "addVRScreenshotToLibrary( ```int``` type, ```string``` filename, ```string``` vr_filename )"
    Adds a VR screenshot to the user's Steam screenshot library from disk in the supported type. 

	Triggers a [screenshot_ready](#screenshot_ready) callback.

    **Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#AddVRScreenshotToLibrary){ .md-button .md-button--store target="_blank" }

### hookScreenshots

!!! function "hookScreenshots( ```bool``` hook )"

    Toggles whether the overlay handles screenshots when the user presses the screenshot hotkey, or if the game handles them.  
  
	Hooking is disabled by default, and only ever enabled if you do so with this function.  
  
	If the hooking is enabled, then the [screenshot_requested](#screenshot_requested) callback will be sent if the user presses the hotkey or when [triggerScreenshot](#triggerscreenshot) is called. Then the game is expected to call [writeScreenshot](#writescreenshot) or [addScreenshotToLibrary](#addscreenshottolibrary) in response.  
 
	You can check if hooking is enabled with [isScreenshotsHooked](#isscreenshotshooked).

    **Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#HookScreenshots){ .md-button .md-button--store target="_blank" }

### isScreenshotsHooked

!!! function "isScreenshotsHooked()"
    Checks if the app is hooking screenshots. 

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#IsScreenshotsHooked){ .md-button .md-button--store target="_blank" }

### setLocation

!!! function "setLocation( ```uint32_t``` screenshot, ```string``` location )"
    Sets optional metadata about a screenshot's location.

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#SetLocation){ .md-button .md-button--store target="_blank" }

### tagPublishedFile

!!! function "tagPublishedFile( ```uint32_t``` screenshot, ```uint64_t``` file_id )"
    Tags a published file as being visible in the screenshot. 

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#TagPublishedFile){ .md-button .md-button--store target="_blank" }

### tagUser

!!! function "tagUser( ```uint32_t``` screenshot, ```uint64_t``` steam_id )"

    Tags a Steam user as being visible in the screenshot. You can tag up to the value declared by k_nScreenshotMaxTaggedUsers in a single screenshot. Tagging more users than that will just be discarded. This function has a built in delay before saving the tag which allows you to call it repeatedly for each item. You can get the handle to tag the screenshot once it has been successfully saved from the [screenshot_ready](#screenshot_ready) callback or via the [WriteScreenshot](#writescreenshot), [AddScreenshotToLibrary](#addscreenshottolibrary), [AddVRScreenshotToLibrary](#addvrscreenshottolibrary) calls. 

    **Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#TagUser){ .md-button .md-button--store target="_blank" }

### triggerScreenshot

!!! function "triggerScreenshot()"
    Either causes the Steam Overlay to take a screenshot, or tells your screenshot manager that a screenshot needs to be taken. Depending on the value of [isScreenshotsHooked](#isscreenshotshooked).

	Triggers a [screenshot_requested](#screenshot_requested) callback if hooking has been enabled with [hookScreenshots](#hookscreenshots).

	Triggers a [screenshot_ready](#screenshot_ready) callback.

    **Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#TriggerScreenshot){ .md-button .md-button--store target="_blank" }

### writeScreenshot

!!! function "writeScreenshot( ```PoolByteArray``` rgb, ```int``` width, ```int``` height )"
    Writes a screenshot to the user's Steam screenshot library.

	Triggers a [screenshot_ready](#screenshot_ready) callback.

    **Returns:** uint32_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#WriteScreenshot){ .md-button .md-button--store target="_blank" }

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### screenshot_ready

!!! function "screenshot_ready"
	A screenshot successfully written or otherwise added to the library and can now be tagged.
	
	**Returns:**

	* handle (uint32_t)
	* result (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#ScreenshotReady_t){ .md-button .md-button--store target="_blank" }

### screenshot_requested

!!! function "screenshot_requested"
	A screenshot has been requested by the user from the Steam screenshot hotkey. This will only be called if [hookScreenshots](#hookscreenshots) has been enabled, in which case Steam will not take the screenshot itself.

	**Returns:** nothing

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#ScreenshotRequested_t){ .md-button .md-button--store target="_blank" }

---

## Enums

---

### VRScreenshotType

Enumerator | Value
---------- | -----
VR_SCREENSHOT_TYPE_NONE | 0
VR_SCREENSHOT_TYPE_MONO | 1
VR_SCREENSHOT_TYPE_STEREO | 2
VR_SCREENSHOT_TYPE_MONO_CUBE_MAP | 3
VR_SCREENSHOT_TYPE_MONO_PANORAMA | 4
VR_SCREENSHOT_TYPE_STEREO_PANORAMA | 5
