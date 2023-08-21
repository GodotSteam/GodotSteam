# Signals - Screenshots

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## screenshot_ready

!!! function "screenshot_ready"
	A screenshot successfully written or otherwise added to the library and can now be tagged.
	
	**Returns:**

	* handle (uint32_t)
	* result (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#ScreenshotReady_t){ .md-button .md-button--store target="_blank" }

## screenshot_requested

!!! function "screenshot_requested"
	A screenshot has been requested by the user from the Steam screenshot hotkey. This will only be called if [hookScreenshots](/functions/screenshots/#hookscreenshots) has been enabled, in which case Steam will not take the screenshot itself.

	**Returns:** nothing

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamScreenshots#ScreenshotRequested_t){ .md-button .md-button--store target="_blank" }
