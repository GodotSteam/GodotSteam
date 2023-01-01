# Functions - Music

---

## getPlaybackStatus

!!! function "getPlaybackStatus()"
	Gets the current status of the Steam Music player.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#GetPlaybackStatus){ .md-button .md-button--store target="_blank" }

## musicIsEnabled

!!! function "musicIsEnabled()"
	Checks if Steam Music is enabled.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#BIsEnabled){ .md-button .md-button--store target="_blank" }

## musicIsPlaying

!!! function "musicIsPlaying()"
	Checks if Steam Music is active. This does not necessarily a song is currently playing, it may be paused.
	For finer grain control use [getPlaybackStatus](/functions/music/#getplaybackstatus).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#BIsPlaying){ .md-button .md-button--store target="_blank" }

## musicGetVolume

!!! function "musicGetVolume()"
	Gets the current volume of the Steam Music player.

	**Returns:** float

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#GetVolume){ .md-button .md-button--store target="_blank" }

## musicPause

!!! function "musicPause()"
	Pause the Steam Music player.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#Pause){ .md-button .md-button--store target="_blank" }

## musicPlay

!!! function "musicPlay()"
	Have the Steam Music player resume playing.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#Play){ .md-button .md-button--store target="_blank" }

## musicPlayNext

!!! function "musicPlayNext()"
	Have the Steam Music player skip to the next song.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#PlayNext){ .md-button .md-button--store target="_blank" }

## musicPlayPrev

!!! function "musicPlayPrev()"
	Have the Steam Music player play the previous song.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#PlayPrevious){ .md-button .md-button--store target="_blank" }

## musicSetVolume

!!! function "musicSetVolume( ```float``` value )"
	Sets the volume of the Steam Music player.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusic#SetVolume){ .md-button .md-button--store target="_blank" }
