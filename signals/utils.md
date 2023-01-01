# Signals - Utilities

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## app_resuming_from_suspend

!!! function "app_resuming_from_suspend"
	Sent after the device returns from sleep/suspend mode.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#AppResumingFromSuspend_t){ .md-button .md-button--store target="_blank" }

## check_file_signature

!!! function "check_file_signature"
	Call result for [checkFileSignature](/functions/utils/#checkfilesignature).
	
	**Returns:**

	* signature (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#CheckFileSignature_t){ .md-button .md-button--store target="_blank" }

## floating_gamepad_text_input_dismissed

!!! function "floating_gamepad_text_input_dismissed"
	Sent after the device returns from sleep/suspend mode.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#FloatingGamepadTextInputDismissed_t){ .md-button .md-button--store target="_blank" }

## gamepad_text_input_dismissed

!!! function "gamepad_text_input_dismissed"
	Called when the big picture gamepad text input has been closed.

	**Returns:**

	* submitted (bool)
	* text (string)
	* length (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#GamepadTextInputDismissed_t){ .md-button .md-button--store target="_blank" }

## ip_country

!!! function "ip_country"
	Called when the country of the user changed. The country should be updated with getIPCountry.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#IPCountry_t){ .md-button .md-button--store target="_blank" }

## low_power

!!! function "low_power"
	Called when running on a laptop and less than 10 minutes of battery is left, and then fires then every minute afterwards.

	**Returns:**
	* power (uint8)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#LowBatteryPower_t){ .md-button .md-button--store target="_blank" }


## steam_api_call_completed

!!! function "steam_api_call_completed"
	Called when a SteamAPICall_t has completed (or failed)

	**Returns:**
	* async_call (uint64_t)
	* callback (int)
	* parameter (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#SteamAPICallCompleted_t){ .md-button .md-button--store target="_blank" }

## steam_shutdown

!!! function "steam_shutdown"
	Called when Steam wants to shut down.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUtils#SteamShutdown_t){ .md-button .md-button--store target="_blank" }