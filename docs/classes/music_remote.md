# Music Remote

Allows direct interaction with the Steam Music player.

---

## Functions

---

### activationSuccess

!!! function "activationSuccess( ```bool``` value )"
	If remote access was successfully activated.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#BActivationSuccess){ .md-button .md-button--store target="_blank" }

### isCurrentMusicRemote

!!! function "isCurrentMusicRemote()"
	Is a remote music client / host connected?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#BIsCurrentMusicRemote){ .md-button .md-button--store target="_blank" }

### currentEntryDidChange

!!! function "currentEntryDidChange()"
	Did the currenty music entry just change?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#CurrentEntryDidChange){ .md-button .md-button--store target="_blank" }

### currentEntryIsAvailable

!!! function "currentEntryIsAvailable( ```bool``` available )"
	Is the current music entry available?

	**Returns:** bool
 
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#CurrentEntryIsAvailable){ .md-button .md-button--store target="_blank" }

### currentEntryWillChange

!!! function "currentEntryWillChange()"
	Will the current music entry change?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#CurrentEntryWillChange){ .md-button .md-button--store target="_blank" }

### deregisterSteamMusicRemote

!!! function "deregisterSteamMusicRemote()"
	Disconnect from remote music client / host.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#DeregisterSteamMusicRemote){ .md-button .md-button--store target="_blank" }

### enableLooped

!!! function "enableLooped( ```bool``` value )"
	Enable track loop on client.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#EnableLooped){ .md-button .md-button--store target="_blank" }

### enablePlaylists

!!! function "enablePlaylists( ```bool``` value )"
	Enable playlists on client.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#EnablePlaylists){ .md-button .md-button--store target="_blank" }

### enablePlayNext

!!! function "enablePlayNext( ```bool``` value )"
	Play the next track on client.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#EnablePlayNext){ .md-button .md-button--store target="_blank" }

### enablePlayPrevious

!!! function "enablePlayPrevious( ```bool``` value )"
	Play previous track on client.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#EnablePlayPrevious){ .md-button .md-button--store target="_blank" }

### enableQueue

!!! function "enableQueue( ```bool``` value )"
	Enable the music queue on the client.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#EnableQueue){ .md-button .md-button--store target="_blank" }

### enableShuffled

!!! function "enableShuffled( ```bool``` value )"
	Enable shuffle on the client.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#EnableShuffled){ .md-button .md-button--store target="_blank" }

### playlistDidChange

!!! function "playlistDidChange()"
	Has the playlist changed?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#PlaylistDidChange){ .md-button .md-button--store target="_blank" }

### playlistWillChange

!!! function "playlistWillChange()"
	Will the playlist change?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#PlaylistWillChange){ .md-button .md-button--store target="_blank" }

### queueDidChange

!!! function "queueDidChange()"
	Did the song queue change?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#QueueDidChange){ .md-button .md-button--store target="_blank" }

### queueWillChange

!!! function "queueWillChange()"
	Will the song queue change?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#QueueWillChange){ .md-button .md-button--store target="_blank" }

### registerSteamMusicRemote

!!! function "registerSteamMusicRemote( ```string``` name )"
	Connect to a music remote client / host?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#RegisterSteamMusicRemote){ .md-button .md-button--store target="_blank" }

### resetPlaylistEntries

!!! function "resetPlaylistEntries()"
	Reset the playlist entries.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#ResetPlaylistEntries){ .md-button .md-button--store target="_blank" }

### resetQueueEntries

!!! function "resetQueueEntries()"
	Reset the song queue entries.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#ResetQueueEntries){ .md-button .md-button--store target="_blank" }

### setCurrentPlaylistEntry

!!! function "setCurrentPlaylistEntry( ```int``` id )"
	Set a new current playlist.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#SetCurrentPlaylistEntry){ .md-button .md-button--store target="_blank" }

### setCurrentQueueEntry

!!! function "setCurrentQueueEntry( ```int``` id )"
	Set a new current song queue.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#SetCurrentQueueEntry){ .md-button .md-button--store target="_blank" }

### setDisplayName

!!! function "setDisplayName( ```string``` name )"
	Set a new display name.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#SetDisplayName){ .md-button .md-button--store target="_blank" }

### setPlaylistEntry

!!! function "setPlaylistEntry( ```int``` id, ```int``` position, ```string``` entryText )"
	Set a new playlist entry.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#SetPlaylistEntry){ .md-button .md-button--store target="_blank" }

### setPNGIcon64x64

!!! function "setPNGIcon64x64( ```PoolByteArray``` icon )"
	Set a PNG icon for a song? A playlist?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#SetPNGIcon_64x64){ .md-button .md-button--store target="_blank" }

### setQueueEntry

!!! function "setQueueEntry( ```int``` id, ```int``` position, ```string``` entryText )"
	Set a new queue entry.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#SetQueueEntry){ .md-button .md-button--store target="_blank" }

### updateCurrentEntryCoverArt

!!! function "updateCurrentEntryCoverArt( ```PoolByteArray``` art )"
	Update the current song entry's cover art.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#UpdateCurrentEntryCoverArt){ .md-button .md-button--store target="_blank" }

### updateCurrentEntryElapsedSeconds

!!! function "updateCurrentEntryElapsedSeconds( ```int``` value )"
	Update the current seconds that have elapsed for an entry.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#UpdateCurrentEntryElapsedSeconds){ .md-button .md-button--store target="_blank" }

### updateCurrentEntryText

!!! function "updateCurrentEntryText( ```string``` text )"
	Update the current song entry's text?

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#UpdateCurrentEntryText){ .md-button .md-button--store target="_blank" }

### updateLooped

!!! function "updateLooped( ```bool``` value )"
	Update looped or not.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#UpdateLooped){ .md-button .md-button--store target="_blank" }

### updatePlaybackStatus

!!! function "updatePlaybackStatus( ```int``` status )"
	Update the current playback status:

	* 0 - undefined
	* 1 - playing
	* 2 - paused
	* 3 - idle.

	**Returns:** bool
 
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#UpdatePlaybackStatus){ .md-button .md-button--store target="_blank" }

### updateShuffled

!!! function "updateShuffled( ```bool``` value )"
	Update whether to shuffle or not.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#UpdateShuffled){ .md-button .md-button--store target="_blank" }

### updateVolume

!!! function "updateVolume( ```float``` value )"
	Volume is between 0.0 and 1.0.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#UpdateVolume){ .md-button .md-button--store target="_blank" }

---

## Signals

---

### music_player_remote_to_front

!!! function "music_player_remote_to_front"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing
	
	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerRemoteToFront_t){ .md-button .md-button--store target="_blank" }

### music_player_remote_will_activate
!!! function "music_player_remote_will_activate"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing
	
	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerRemoteWillActivate_t){ .md-button .md-button--store target="_blank" }
	
### music_player_remote_will_deactivate

!!! function "music_player_remote_will_deactivate"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerRemoteWillDeactivate_t){ .md-button .md-button--store target="_blank" }

### music_player_selects_playlist_entry

!!! function "music_player_selects_playlist_entry"
	There are no notes in the Steamworks documentation.

	**Returns:**

	* entry (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerSelectsPlaylistEntry_t){ .md-button .md-button--store target="_blank" }

### music_player_selects_queue_entry

!!! function "music_player_selects_queue_entry"
	There are no notes in the Steamworks documentation.

	**Returns:**

	*entry (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerSelectsQueueEntry_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_looped

!!! function "music_player_wants_looped"
	There are no notes in the Steamworks documentation.

	**Returns:**
	
	* looped (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsLooped_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_pause

!!! function "music_player_wants_pause"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsPause_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_play

!!! function "music_player_wants_play"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing
	
	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsPlay_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_play_next

!!! function "music_player_wants_play_next"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsPlayNext_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_play_previous

!!! function "music_player_wants_play_previous"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsPlay_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_playing_repeat_status

!!! function "music_player_wants_playing_repeat_status"
	There are no notes in the Steamworks documentation.

	**Returns:**

	* status (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsPlayNext_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_shuffled

!!! function "music_player_wants_shuffled"
	There are no notes in the Steamworks documentation.

	**Returns:**

	* shuffled (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsShuffled_t){ .md-button .md-button--store target="_blank" }

### music_player_wants_volume

!!! function "music_player_wants_volume"
	There are no notes in the Steamworks documentation.

	**Returns:**
	
	* volume (float)
	
	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWantsVolume_t){ .md-button .md-button--store target="_blank" }

### music_player_will_quit

!!! function "music_player_will_quit"
	There are no notes in the Steamworks documentation.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamMusicRemote#MusicPlayerWillQuit_t){ .md-button .md-button--store target="_blank" }
