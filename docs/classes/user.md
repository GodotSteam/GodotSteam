# User

Functions for accessing and manipulating Steam user information.

This is also where the APIs for [Steam Voice](https://partner.steamgames.com/doc/features/voice){ target="_blank" } are exposed.

---

## Functions

---

### advertiseGame

!!! function "advertiseGame( ```string``` server_ip, ```uint16``` port )"
	Set the rich presence data for an unsecured game server that the user is playing on. This allows friends to be able to view the game info and join your game.

	When you are using Steam authentication system this call is never required, the auth system automatically sets the appropriate rich presence.

	**Returns:** void

	**Note:** This is a legacy function.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#AdvertiseGame){ .md-button .md-button--store target="_blank" }

### beginAuthSession

!!! function "beginAuthSession( ```PoolByteArray / PackedByteArray``` auth_ticket, ```int``` ticket_size, ```uint64_t``` steam_id )"
	Authenticate the ticket from the entity's Steam ID to be sure it is valid and isn't reused. The ticket is first created by the entity with [getAuthSessionTicket](#getauthsessionticket) and then needs to be provided over the network for the other end to validate.

	When the multiplayer session terminates you must call [endAuthSession](#endauthsession).

	Triggers a [validate_auth_ticket_response](#validate_auth_ticket_response) callback. Beginning an auth session also registers for these callbacks if the entity goes offline or cancels the ticket.

	**Returns:** int ([BeginAuthSessionResult enum](main.md#beginauthsessionresult))
	
	See [EBeginAuthSessionResult](https://partner.steamgames.com/doc/api/steam_api#EBeginAuthSessionResult){ target="_blank" } for more details.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BeginAuthSession){ .md-button .md-button--store target="_blank" }

### cancelAuthTicket

!!! function "cancelAuthTicket( ```uint32_t``` auth_ticket )"
	Cancels an auth ticket received from [getAuthSessionTicket](#getauthsessionticket). This should be called when no longer playing with the specified entity.

	**Returns:** void
	
	***Note***: This will trigger a [validate_auth_ticket_response](#validate_auth_ticket_response) callback for any other server or client who called [beginAuthSession](#beginauthsession) using this ticket to let them know it is no longer valid.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#CancelAuthTicket){ .md-button .md-button--store target="_blank" }

### decompressVoice

!!! function "decompressVoice( ```PoolByteArray / PackedByteArray``` voice, ```uint32``` voice_size, ```uint32``` sample_rate )"
	Decodes the compressed voice data returned by [getVoice](#getvoice).
	
	The output data is raw single-channel 16-bit PCM audio. The decoder supports any sample rate from 11025 to 48000. See [getVoiceOptimalSampleRate](#getvoiceoptimalsamplerate) for more information.

	It is recommended that you start with a 20KiB buffer and then reallocate as necessary.

	See [Steam Voice](https://partner.steamgames.com/doc/features/voice){ target="_blank" } for more information.

	**Returns:** dictionary

	Contains the following keys:

	* uncompressed (PoolByteArray / PackedByteArray)
	* size (uint32)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#DecompressVoice){ .md-button .md-button--store target="_blank" }

### endAuthSession

!!! function "endAuthSession( ```uint64_t``` steam_id )"
	Ends an auth session that was started with [beginAuthSession](#beginauthsession). This should be called when no longer playing with the specified entity.
	
	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#EndAuthSession){ .md-button .md-button--store target="_blank" }

### getAuthSessionTicket

!!! function "getAuthSessionTicket( ```string``` network_identity )"
	Retrieve an authentication ticket to be sent to the entity who wishes to authenticate you.

	After calling this you can send the ticket to the entity where they can then call [beginAuthSession](#beginauthsession) to verify this entity's integrity.

	**NOTE:** This API can not be used to create a ticket for use by the [AuthenticateUserTicket Web API](https://partner.steamgames.com/doc/webapi/ISteamUserAuth){ target="_blank" }. Use [getAuthTicketForWebApi](#getauthticketforwebapi) for that instead.

	Triggers a [get_auth_session_ticket_response](#get_auth_session_ticket_response) callback.

	**Returns:** dictionary

	Contains the following keys:

	* id (uint32_t)
	* buffer (PoolByteArray / PackedByteArray)
	* size (uint32_t)

	**Note:** As of Steamworks SDK 1.57, you may pass a network identity that was created with our [Networking Types](networking_types.md) class. However, this is optional and defaults to NULL.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetAuthSessionTicket){ .md-button .md-button--store target="_blank" }

### getAuthTicketForWebApi

!!! function "getAuthTicketForWebApi( ```string``` service_identity )"
	Request a ticket which will be used for the [AuthenticateUserTicket Web API](https://partner.steamgames.com/doc/webapi/ISteamUserAuth){ target="_blank" }.

	Triggers a [get_ticket_for_web_api](#get_ticket_for_web_api) callback which will include the actual ticket.

	**Returns:** auth_ticket_handle (uint32)

	**Note:** The ```service_identity``` is **not** a network identity used by or created with GodotSteam's Networking Types class. It is an optional input parameter to identify the service the ticket will be sent to.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetAuthTicketForWebApi){ .md-button .md-button--store target="_blank" }

### getAvailableVoice

!!! function "getAvailableVoice()"
	Checks to see if there is captured audio data available from [getVoice](#getvoice), and gets the size of the data.

	Most applications will only use compressed data and should ignore the other parameters, which exist primarily for backwards compatibility. See [getVoice](#getVoice) for further explanation of "uncompressed" data.

	See [Steam Voice](https://partner.steamgames.com/doc/features/voice){ target="_blank" } for more information.
	
	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* buffer (uint32)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetAvailableVoice){ .md-button .md-button--store target="_blank" }

### getDurationControl

!!! function "getDurationControl()"
	Retrieves anti indulgence / duration control for current user / game combination.

	Triggers a [duration_control](#duration_control) call result.
	
	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetDurationControl){ .md-button .md-button--store target="_blank" }

### getEncryptedAppTicket

!!! function "getEncryptedAppTicket()"
	Retrieve an encrypted ticket.

	This should be called after requesting an encrypted app ticket with [requestEncryptedAppTicket](#requestencryptedappticket) and receiving the [encrypted_app_ticket_response](#encrypted_app_ticket_response) call result.

	You should then pass this encrypted ticket to your secure servers to be decrypted using your secret key using [decryptTicket](#decryptticket)

	**Returns:** dictionary

	Contains the following keys:

	* buffer (PoolByteArray / PackedByteArray)
	* size (uint32_t)

	**Note:**  If you call this without calling [requestEncryptedAppTicket](#requestencryptedappticket), the call may succeed but you will likely get a stale ticket.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetEncryptedAppTicket){ .md-button .md-button--store target="_blank" }

### getGameBadgeLevel

!!! function "getGameBadgeLevel( ```int``` series, ```bool``` foil )"
	Gets the level of the users Steam badge for your game.

	The user can have two different badges for a series; the regular badge (max level 5) and the foil badge (max level 1).
	
	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetGameBadgeLevel){ .md-button .md-button--store target="_blank" }

### getPlayerSteamLevel

!!! function "getPlayerSteamLevel()"
	Gets the Steam level of the user, as shown on their Steam community profile.

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetPlayerSteamLevel){ .md-button .md-button--store target="_blank" }

### getSteamID

!!! function "getSteamID()"
	Gets the Steam ID (ID64) of the account currently logged into the Steam client. This is commonly called the 'current user', or 'local user'.

	A Steam ID is a unique identifier for a Steam accounts, Steam groups, Lobbies and Chat rooms, and used to differentiate users in all parts of the Steamworks API.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetSteamID){ .md-button .md-button--store target="_blank" }

### getVoice

!!! function "getVoice()"
	Read captured audio data from the microphone buffer.

	The compressed data can be transmitted by your application and decoded back into raw audio data using DecompressVoice on the other side. The compressed data provided is in an arbitrary format and is not meant to be played directly.

	This should be called once per frame, and at worst no more than four times a second to keep the microphone input delay as low as possible. Calling this any less may result in gaps in the returned stream.

	It is recommended that you pass in an 8 kilobytes or larger destination buffer for compressed audio. Static buffers are recommended for performance reasons. However, if you would like to allocate precisely the right amount of space for a buffer before each call you may use [getAvailableVoice](#getavailablevoice) to find out how much data is available to be read.

	See [Steam Voice](https://partner.steamgames.com/doc/features/voice){ target="_blank" } for more information.

	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* written (uint32)
	* buffer (uint8)

	**Note:**  "Uncompressed" audio is a deprecated feature and should not be used by most applications. It is raw single-channel 16-bit PCM wave data which may have been run through preprocessing filters and/or had silence removed, so the uncompressed audio could have a shorter duration than you expect. There may be no data at all during long periods of silence. Also, fetching uncompressed audio will cause [getVoice](#getvoice) to discard any leftover compressed audio, so you must fetch both types at once. Finally, [getAvailableVoice](#getavailablevoice) is not precisely accurate when the uncompressed size is requested. So if you really need to use uncompressed audio, you should call [getVoice](#getvoice) frequently with two very large (20KiB+) output buffers instead of trying to allocate perfectly-sized buffers. But most applications should ignore all of these details and simply leave the "uncompressed" parameters as NULL/0.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetVoice){ .md-button .md-button--store target="_blank" }	

### getVoiceOptimalSampleRate

!!! function "getVoiceOptimalSampleRate()"
	Gets the native sample rate of the Steam voice decoder.

	Using this sample rate for [decompressVoice](#decompressVoice) will perform the least CPU processing. However, the final audio quality will depend on how well the audio device (and/or your application's audio output SDK) deals with lower sample rates. You may find that you get the best audio output quality when you ignore this function and use the native sample rate of your audio output device, which is usually 48000 or 44100.

	See [Steam Voice](https://partner.steamgames.com/doc/features/voice){ target="_blank" } for more information.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetVoiceOptimalSampleRate){ .md-button .md-button--store target="_blank" }

### initiateGameConnection

!!! function "initiateGameConnection( ```uint64_t``` server_id, ```uint32``` server_ip, ```uint16``` server_port, ```bool``` secure )"
	This starts the state machine for authenticating the game client with the game server.

	It is the client portion of a three-way handshake between the client, the game server, and the steam servers.

	**Returns:** dictionary

	Contains the following keys:

	* auth_blob (PoolByteArray / PackedByteArray)
	* server_id (uint64_t)
	* server_ip (uint32)
	* server_port (uint16)

	**Note:**  When you're done with the connection you must call [terminateGameConnection](#terminategameconnection)

	**Note:**  This is part of the old user authentication API and should not be mixed with the new API.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#InitiateGameConnection){ .md-button .md-button--store target="_blank" }

### isBehindNAT

!!! function "isBehindNAT()"
	Checks if the current user looks like they are behind a NAT device.

	This is only valid if the user is connected to the Steam servers and may not catch all forms of NAT.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BIsBehindNAT){ .md-button .md-button--store target="_blank" }

### isPhoneIdentifying

!!! function "isPhoneIdentifying()"
	Checks whether the user's phone number is used to uniquely identify them.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BIsPhoneIdentifying){ .md-button .md-button--store target="_blank" }

### isPhoneRequiringVerification

!!! function "isPhoneRequiringVerification()"
	Checks whether the current user's phone number is awaiting (re)verification.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BIsPhoneRequiringVerification){ .md-button .md-button--store target="_blank" }

### isPhoneVerified

!!! function "isPhoneVerified()"
	Checks whether the current user has verified their phone number.

	See the [Steam Guard Mobile Authenticator](https://support.steampowered.com/kb_article.php?ref=8625-wrah-9030){ target="_blank" } page on the customer facing Steam Support site for more information.
	
	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BIsPhoneVerified){ .md-button .md-button--store target="_blank" }

### isTwoFactorEnabled

!!! function "isTwoFactorEnabled()"
	Checks whether the current user has Steam Guard two factor authentication enabled on their account.

	See the [Steam Guard Mobile Authenticator](https://support.steampowered.com/kb_article.php?ref=8625-wrah-9030){ target="_blank" } page on the customer facing Steam Support site for more information.
	
	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BIsTwoFactorEnabled){ .md-button .md-button--store target="_blank" }

### loggedOn

!!! function "loggedOn()"
	Checks if the current user's Steam client is connected to the Steam servers.

	If it's not then no real-time services provided by the Steamworks API will be enabled. The Steam client will automatically be trying to recreate the connection as often as possible. When the connection is restored a [steam_server_connected](#steam_server_connected) callback will be posted.

	You usually don't need to check for this yourself. All of the API calls that rely on this will check internally. Forcefully disabling stuff when the player loses access is usually not a very good experience for the player and you could be preventing them from accessing APIs that do not need a live connection to Steam.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BLoggedOn){ .md-button .md-button--store target="_blank" }

### requestEncryptedAppTicket

!!! function "requestEncryptedAppTicket( ```string``` secret )"
	Requests an application ticket encrypted with the secret "encrypted app ticket key".

	The encryption key can be obtained from the [Encrypted App Ticket Key](https://partner.steamgames.com/apps/sdkauth/){ target="_blank" } page on the App Admin for your app.

	There can only be one [encrypted_app_ticket_response](#encrypted_app_ticket_response) pending, and this call is subject to a 60 second rate limit.

	After receiving the response you should call [getEncryptedAppTicket](#getencryptedappticket) to get the ticket data, and then you need to send it to a secure server to be decrypted with the [SteamEncryptedAppTicket](https://partner.steamgames.com/doc/api/SteamEncryptedAppTicket){ target="_blank" } functions.

	Triggers a [encrypted_app_ticket_response](#encrypted_app_ticket_response) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#RequestEncryptedAppTicket){ .md-button .md-button--store target="_blank" }

### requestStoreAuthURL

!!! function "requestStoreAuthURL( ```string``` redirect )"
	Requests a URL which authenticates an in-game browser for store check-out, and then redirects to the specified URL.

	As long as the in-game browser accepts and handles session cookies, Steam microtransaction checkout pages will automatically recognize the user instead of presenting a login page.

	Triggers a [store_auth_url_response](#store_auth_url_response) call result.
	
	**Returns:** void

	**Note:** The URL has a very short lifetime to prevent history-snooping attacks, so you should only call this API when you are about to launch the browser, or else immediately navigate to the result URL using a hidden browser window.

	**Note:** The resulting authorization cookie has an expiration time of one day, so it would be a good idea to request and visit a new auth URL every 12 hours.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#RequestStoreAuthURL){ .md-button .md-button--store target="_blank" }

### setDurationControlOnlineState

!!! function "setDurationControlOnlineState( ```int``` new_state )"
	Allows the game to specify the offline/online gameplay state for Steam China duration control.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#BSetDurationControlOnlineState){ .md-button .md-button--store target="_blank" }

### startVoiceRecording

!!! function "startVoiceRecording()"
	Starts voice recording.

	Once started, use [getAvailableVoice](#getavailablevoice) and [getVoice](#getvoice) to get the data, and then call [stopVoiceRecording](#stopvoicerecording) when the user has released their push-to-talk hotkey or the game session has completed.

	See [Steam Voice](https://partner.steamgames.com/doc/features/voice){ target="_blank" } for more information.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#StartVoiceRecording){ .md-button .md-button--store target="_blank" }

### stopVoiceRecording

!!! function "stopVoiceRecording()"
	Stops voice recording.

	Because people often release push-to-talk keys early, the system will keep recording for a little bit after this function is called. As such, [getVoice](#getvoice) should continue to be called until it returns 2, only then will voice recording be stopped.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#StopVoiceRecording){ .md-button .md-button--store target="_blank" }

### terminateGameConnection

!!! function "terminateGameConnection( ```uint32``` server_ip, ```uint32``` server_port )"
	Notify the game server that we are disconnecting.

	This needs to occur when the game client leaves the specified game server, needs to match with the [initiateGameConnection](#initiategameconnection) call.

	**Returns:** void

	**Note:**  This is part of the old user authentication API and should not be mixed with the new API.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#TerminateGameConnection){ .md-button .md-button--store target="_blank" }

### userHasLicenseForApp

!!! function "userHasLicenseForApp( ```uint64_t``` steam_id, ```uint32_t``` app_id )"
	Checks if the user owns a specific piece of Downloadable Content (DLC).

	This can only be called after sending the users auth ticket to [beginAuthSession](#beginauthsession).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#UserHasLicenseForApp){ .md-button .md-button--store target="_blank" }

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### duration_control

!!! function "duration_control"
	Sent for games with enabled anti indulgence / duration control, for enabled users. Lets the game know whether persistent rewards or XP should be granted at normal rate, half rate, or zero rate.

	**Returns:**
	### 
	* result (int)
	* duration (dictionary)
		* app_id (uint32)
		* applicable (bool)
		* seconds_last_5hrs (int32)
		* progress (int)
		* notification (int)
		* notification_verbal (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#DurationControl_t){ .md-button .md-button--store target="_blank" }

### client_game_server_deny

!!! function "client_game_server_deny"
	Sent by the Steam server to the client telling it to disconnect from the specified game server, which it may be in the process of or already connected to. The game client should immediately disconnect upon receiving this message. This can usually occur if the user doesn't have rights to play on the game server.

	**Returns:**
			
	* app_id (uint32)
	* ip (string)
	* server_port (uint16)
	* secure (uint16)
	* reason (uint32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#ClientGameServerDeny_t){ .md-button .md-button--store target="_blank" }

### encrypted_app_ticket_response

!!! function "encrypted_app_ticket_response"
	Called when an encrypted application ticket has been received.

	**Returns:**

	* result (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#EncryptedAppTicketResponse_t){ .md-button .md-button--store target="_blank" }

### game_web_callback

!!! function "game_web_callback"
	Sent to your game in response to a ```steam://gamewebcallback/``` command from a user clicking a link in the Steam overlay browser. You can use this to add support for external site signups where you want to pop back into the browser after some web page signup sequence, and optionally get back some detail about that.

	**Returns:**

	* url (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GameWebCallback_t){ .md-button .md-button--store target="_blank" }

### get_auth_session_ticket_response

!!! function "get_auth_session_ticket_response"
	Result when creating an auth session ticket with [getAuthSessionTicket](#getauthsessionticket).

	**Returns:**
		
	* auth_ticket (uint32)
	* result (int) ([Result enum](main.md#result))

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetAuthSessionTicketResponse_t){ .md-button .md-button--store target="_blank" }

### get_ticket_for_web_api

!!! function "get_ticket_for_web_api"
	Result when creating a Web API ticket with [getAuthTicketForWebApi](#getauthticketforwebapi).

	**Returns:**

	* auth_ticket (uint32)
	* result (int) ([Result enum](main.md#result))
	* ticket_size (int)
	* ticket_buffer (uint8)
	
	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#GetTicketForWebApiResponse_t){ .md-button .md-button--store target="_blank" }	

### ipc_failure

!!! function "ipc_failure"
	Called when the callback system for this client is in an error state (and has flushed pending callbacks). When getting this message the client should disconnect from Steam, reset any stored Steam state and reconnect. This usually occurs in the rare event the Steam client has some kind of fatal error.

	**Returns:**

	* type (uint8)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#IPCFailure_t){ .md-button .md-button--store target="_blank" }

### licenses_updated

!!! function "licenses_updated"
	Called whenever the users licenses (owned packages) changes.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#LicensesUpdated_t){ .md-button .md-button--store target="_blank" }

### microtransaction_auth_response

!!! function "microtransaction_auth_response"
	Called when a user has responded to a microtransaction authorization request.

	**Returns:**

	* app_id (uint32)
	* order_id (uint64_t)
	* authorized (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#MicroTxnAuthorizationResponse_t){ .md-button .md-button--store target="_blank" }

### steam_server_connect_failed

!!! function "steam_server_connect_failed"
	Called when a connection attempt has failed. This will occur periodically if the Steam client is not connected, and has failed when retrying to establish a connection.

	**Returns:**

	* result (int)
	* retrying (bool)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServerConnectFailure_t){ .md-button .md-button--store target="_blank" }

### steam_server_connected

!!! function "steam_server_connected"
	Called when a connections to the Steam back-end has been established. This means the Steam client now has a working connection to the Steam servers. Usually this will have occurred before the game has launched, and should only be seen if the user has dropped connection due to a networking issue or a Steam server update.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServersConnected_t){ .md-button .md-button--store target="_blank" }

### steam_server_disconnected

!!! function "steam_server_disconnected"
	Called if the client has lost connection to the Steam servers. Real-time services will be disabled until a matching [steam_server_connected](#steam_server_connected) has been posted.

	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#SteamServersDisconnected_t){ .md-button .md-button--store target="_blank" }

### store_auth_url_response

!!! function "store_auth_url_response"
	Response when we have recieved the authentication URL after a call to [requestStoreAuthURL](#requeststoreauthurl).

	**Returns:**

	* url (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#StoreAuthURLResponse_t){ .md-button .md-button--store target="_blank" }

### validate_auth_ticket_response

!!! function "validate_auth_ticket_response"
	Called when an auth ticket has been validated.
	
	Emits signal in response to function [beginAuthSession](#beginauthsession). Also received when a user you have an active auth session with cancels their auth ticket so that you can respond appropriately by calling [endAuthSession](#endauthsession), etc.

	**Returns:**

	* auth_id (uint64_t)
	* response (uint32_t) ([AuthSessionResponse enum](main.md#authsessionresponse))
	* owner_id (uint64_t)
	
	See [EAuthSessionResponse](https://partner.steamgames.com/doc/api/steam_api#EAuthSessionResponse){ target="_blank" } for more details about possible responses.

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUser#ValidateAuthTicketResponse_t){ .md-button .md-button--store target="_blank" }

---

## Enums

---

### FailureType

Enumerator | Value
---------- | -----
FAILURE_FLUSHED_CALLBACK_QUEUE | 0
FAILURE_PIPE_FAIL | 1

### DurationControlProgress

Enumerator | Value
---------- | -----
DURATION_CONTROL_PROGRESS_FULL | 0
DURATION_CONTROL_PROGRESS_HALF | 1
DURATION_CONTROL_PROGRESS_NONE | 2

### DurationControlNotification

Enumerator | Value
---------- | -----
DURATION_CONTROL_NOTIFICATION_NONE | 0
DURATION_CONTROL_NOTIFICATION_1_HOUR | 1
DURATION_CONTROL_NOTIFICATION_3_HOURS | 3
DURATION_CONTROL_NOTIFICATION_HALF_PROGRESS | 3
DURATION_CONTROL_NOTIFICATION_NO_PROGRESS | 4
