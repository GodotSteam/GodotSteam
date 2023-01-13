# Functions - Utilities

------

## filterText

!!! function "filterText( ```int``` context, ```uint64_t``` steam_id, ```string``` message )"
	Filters the provided input message and returns the filtered result. Legally required filtering is always applied. Additional filtering may occur, based on the context and user settings.

	**Returns:** string

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#FilterText){ .md-button .md-button--store target="_blank" }

## getAPICallFailureReason

!!! function "getAPICallFailureReason()"
	Used to get the failure reason of a call result.

	The primary usage for this function is debugging. The failure reasons are typically out of your control and tend to not be very important. Just keep retrying your API Call until it works.

	**Returns:** string

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetAPICallFailureReason){ .md-button .md-button--store target="_blank" }

## getAppID

!!! function "getAppID()"
	Gets the App ID of the current process.

	**Returns:** uint32_t

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetAppID){ .md-button .md-button--store target="_blank" }

## getCurrentBatteryPower

!!! function "getCurrentBatteryPower()"
	Gets the current amount of battery power on the computer.

	**Returns:** int

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetCurrentBatteryPower){ .md-button .md-button--store target="_blank" }

## getImageRGBA

!!! function "getImageRGBA( ```int``` image )"
	Gets the image bytes from an image handle.

	Prior to calling this you must get the size of the image by calling [getImageSize](/functions/utils/#getimagesize) so that you can create your buffer with an appropriate size. You can then allocate your buffer with the width and height as: width * height * 4. The image is provided in RGBA format. This call can be somewhat expensive as it converts from the compressed type (JPG, PNG, TGA) and provides no internal caching of returned buffer, thus it is highly recommended to only call this once per image handle and cache the result. This function is only used for Steam Avatars and Achievement images and those are not expected to change mid game.

	**Returns:** dictionary

	Contains the following keys:
	
	* buffer (PoolByteArray)
	* success (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetImageRGBA){ .md-button .md-button--store target="_blank" }

## getImageSize

!!! function "getImageSize( ```int``` image )"
	Gets the size of a Steam image handle.

	This must be called before calling [getImageRGBA](/functions/utils/#getimagesize) to create an appropriately sized buffer that will be filled with the raw image data.
	
	**Returns:** dictionary

	Contains the following keys:

	* width (uint32)
	* height (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetImageSize){ .md-button .md-button--store target="_blank" }

## getIPCCallCount

!!! function "getIPCCallCount()"
	Returns the number of IPC calls made since the last time this function was called.

	Used for perf debugging so you can determine how many IPC (Inter-Process Communication) calls your game makes per frame.
	
	Every IPC call is at minimum a thread context switch if not a process one so you want to rate control how often you do them.

	**Returns:** uint32

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetIPCCallCount){ .md-button .md-button--store target="_blank" }

## getIPCountry

!!! function "getIPCountry()"
	Returns the 2 digit ISO 3166-1-alpha-2 format country code which client is running in. e.g "US" or "UK".

	This is looked up via an IP-to-location database.

	**Returns:** string

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetIPCountry){ .md-button .md-button--store target="_blank" }

## getSecondsSinceAppActive

!!! function "getSecondsSinceAppActive()"
	Returns the number of seconds since the application was active.

	**Returns:** int

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetSecondsSinceAppActive){ .md-button .md-button--store target="_blank" }

## getSecondsSinceComputerActive

!!! function "getSecondsSinceComputerActive()"
	Returns the number of seconds since the user last moved the mouse.

	**Returns:** int

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetSecondsSinceComputerActive){ .md-button .md-button--store target="_blank" }

## getServerRealTime

!!! function "getServerRealTime()"
	Returns the Steam server time in Unix epoch format. (Number of seconds since Jan 1, 1970 UTC).

	**Returns:** int

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetServerRealTime){ .md-button .md-button--store target="_blank" }

## getSteamUILanguage

!!! function "getSteamUILanguage()"
	Returns the language the steam client is running in.

	You probably want [getCurrentGameLanguage](/functions/utils/#getcurrentgamelanguage) instead, this should only be used in very special cases.

	For a full list of languages see [Supported Languages](https://partner.steamgames.com/doc/store/localization#supported_languages){ target="_blank" }.

	**Returns:** string

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GetSteamUILanguage){ .md-button .md-button--store target="_blank" }

## initFilterText

!!! function "initFilterText()"
	Initializes text filtering, loading dictionaries for the language the game is running in.

	Users can customize the text filter behavior in their Steam Account preferences.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#InitFilterText){ .md-button .md-button--store target="_blank" }

## isAPICallCompleted

!!! function "isAPICallCompleted()"
	Checks if an API Call is completed. Provides the backend of the CallResult wrapper.

	It's generally not recommended that you use this yourself.
	
	**Returns:** dictionary

	Contains the following keys:

	* completed (bool)
	* failed (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsAPICallCompleted){ .md-button .md-button--store target="_blank" }

## isOverlayEnabled

!!! function "isOverlayEnabled()"
	Checks if the Steam Overlay is running & the user can access it.

	The overlay process could take a few seconds to start & hook the game process, so this function will initially return false while the overlay is loading.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsOverlayEnabled){ .md-button .md-button--store target="_blank" }

## isSteamChinaLauncher

!!! function "isSteamChinaLauncher()"
	Returns whether the current launcher is a Steam China launcher. You can cause the client to behave as the Steam China launcher by adding ```-dev -steamchina``` to the command line when running Steam.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsSteamChinaLauncher){ .md-button .md-button--store target="_blank" }

## isSteamInBigPictureMode

!!! function "isSteamInBigPictureMode()"
	Checks if Steam & the Steam Overlay are running in Big Picture mode.

	Games must be launched through the Steam client to enable the Big Picture overlay. During development, a game can be added as a non-steam game to the developers library to test this feature.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsSteamInBigPictureMode){ .md-button .md-button--store target="_blank" }

## isSteamRunningInVR

!!! function "isSteamRunningInVR()"
	Checks if Steam is running in VR mode.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsSteamRunningInVR){ .md-button .md-button--store target="_blank" }

## isSteamRunningOnSteamDeck

!!! function "isSteamRunningOnSteamDeck()"
	Checks if Steam is running on a Steam Deck device.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsSteamRunningOnSteamDeck){ .md-button .md-button--store target="_blank" }

## isVRHeadsetStreamingEnabled

!!! function "isVRHeadsetStreamingEnabled()"
	Checks if the HMD view will be streamed via Steam In-Home Streaming.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsVRHeadsetStreamingEnabled){ .md-button .md-button--store target="_blank" }

## overlayNeedsPresent

!!! function "overlayNeedsPresent()"
	Checks if the Overlay needs a present. Only required if using event driven render updates.

	Typically this call is unneeded if your game has a constantly running frame loop that calls the D3D Present API, or OGL SwapBuffers API every frame as is the case in most games. However, if you have a game that only refreshes the screen on an event driven basis then that can break the overlay, as it uses your Present/SwapBuffers calls to drive it's internal frame loop and it may also need to Present() to the screen any time a notification happens or when the overlay is brought up over the game by a user. You can use this API to ask the overlay if it currently need a present in that case, and then you can check for this periodically (roughly 33hz is desirable) and make sure you refresh the screen with Present or SwapBuffers to allow the overlay to do it's work.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#BOverlayNeedsPresent){ .md-button .md-button--store target="_blank" }

## setGameLauncherMode

!!! function "setGameLauncherMode( ```bool``` mode)"
	In game launchers that don't have controller support you can call this to have Steam Input translate the controller input into mouse/kb to navigate the launcher.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#SetGameLauncherMode){ .md-button .md-button--store target="_blank" }

## setOverlayNotificationInset

!!! function "setOverlayNotificationInset( ```int``` horizontal, ```int``` vertical )"
	Sets the inset of the overlay notification from the corner specified by [setOverlayNotificationPosition](/functions/utils/#setoverlaynotificationposition). Integer should be number of pixels.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationInset){ .md-button .md-button--store target="_blank" }

## setOverlayNotificationPosition

!!! function "setOverlayNotificationPosition( ```int``` pos )"
	Set the position where overlay shows notifications. Accepts the following integer inputs:

	* 0 - Top Left
	* 1 - Top Right
	* 2 - Bottom Left
	* 3 - Bottom Right
			
	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationPosition){ .md-button .md-button--store target="_blank" }

## setVRHeadsetStreamingEnabled

!!! function "setVRHeadsetStreamingEnabled()"
	Set whether the HMD content will be streamed via Steam In-Home Streaming.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IsVRHeadsetStreamingEnabled){ .md-button .md-button--store target="_blank" }

## showGamepadTextInput

!!! function "showGamepadTextInput( ```int``` input_mode, ```int``` line_input_mode, ```string``` description, ```uint32``` max_text, ```string``` preset_text )"
	Activates the Big Picture text input dialog which only supports gamepad input.

	**Returns:** bool

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#ShowGamepadTextInput){ .md-button .md-button--store target="_blank" }

## showFloatingGamepadTextInput

!!! function "showFloatingGamepadTextInput( ```int``` input_mode, ```int``` text_field_x_position, ```int``` text_field_y_position, ```int``` text_field_width, ```int``` text_field_height )"
	Opens a floating keyboard over the game content and sends OS keyboard keys directly to the game.

	The text field position is specified in pixels relative the origin of the game window and is used to position the floating keyboard in a way that doesn't cover the text field.

	**Returns:** ```bool```

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#ShowFloatingGamepadTextInput){ .md-button .md-button--store target="_blank" }

## startVRDashboard

!!! function "startVRDashboard()"
	Ask SteamUI to create and render its OpenVR dashboard.

	**Returns:** void

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#StartVRDashboard){ .md-button .md-button--store target="_blank" }