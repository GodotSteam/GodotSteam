# Signals | Video

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## get_opf_settings_result

!!! function "get_opf_settings_result"
	Triggered when the OPF Details for 360 video playback are retrieved. After receiving this you can use [getOPFStringForApp](/functions/video/#getopfstringforapp) to access the OPF details.

	**Returns:**

	* result (int)
	* app_id (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamVideo#GetOPFSettingsResult_t){ .md-button .md-button--store target="_blank" }

## get_video_result

!!! function "get_video_result"
	Provides the result of a call to [getVideoURL](/functions/video/#getvideourl).

	**Returns:**

	* result (int)
	* app_id (uint32)
	* url (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamVideo#GetVideoURLResult_t){ .md-button .md-button--store target="_blank" }
