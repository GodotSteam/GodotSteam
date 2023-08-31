# Networking Utils

Miscellaneous networking utilities for checking the local networking environment and estimating pings.

---

## Functions

---

### checkPingDataUpToDate

!!! function "checkPingDataUpToDate( ```float``` max_age_in_seconds )"
	Check if the ping data of sufficient recency is available, and if it's too old, start refreshing it.

	Please only call this function when you really do need to force an immediate refresh of the data. (For example, in response to a specific user input to refresh this information.) Don't call it "just in case", before every connection, etc. That will cause extra traffic to be sent for no benefit. The library will automatically refresh the information as needed.

	**Returns:** bool

	True if sufficiently recent data is already available. False if sufficiently recent data is not available. In this case, ping measurement is initiated, if it is not already active. (You cannot restart a measurement already in progress.)
 
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#CheckPingDataUpToDate){ .md-button .md-button--store target="_blank" }

### convertPingLocationToString

!!! function "convertPingLocationToString( ```PoolByteArray``` location )"
	Convert a ping location into a text format suitable for sending over the wire. The format is a compact and human readable. However, it is subject to change so please do not parse it yourself.

	**Returns:** string

	   ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#ConvertPingLocationToString){ .md-button .md-button--store target="_blank" }

### estimatePingTimeBetweenTwoLocations

!!! function "estimatePingTimeBetweenTwoLocations( ```PoolByteArray``` location1, ```PoolByteArray``` location2 )"
	Estimate the round-trip latency between two arbitrary locations, in milliseconds. This is a conservative estimate, based on routing through the relay network. For most basic relayed connections, this ping time will be pretty accurate, since it will be based on the route likely to be actually used.
	
	If a direct IP route is used (perhaps via NAT traversal), then the route will be different, and the ping time might be better. Or it might actually be a bit worse! Standard IP routing is frequently suboptimal! But even in this case, the estimate obtained using this method is a reasonable upper bound on the ping time. (Also it has the advantage of returning immediately and not sending any packets.)
	
	In a few cases we might not able to estimate the route. In this case a negative value is returned. k_nSteamNetworkingPing_Failed means the reason was because of some networking difficulty. (Failure to ping, etc) k_nSteamNetworkingPing_Unknown is returned if we cannot currently answer the question for some other reason.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#EstimatePingTimeBetweenTwoLocations){ .md-button .md-button--store target="_blank" }

### estimatePingTimeFromLocalHost

!!! function "estimatePingTimeFromLocalHost( ```PoolByteArray``` location )"
	Same as [estimatePingTimeBetweenTwoLocations](#estimatepingtimebetweentwolocations), but assumes that one location is the local host. This is a bit faster, especially if you need to calculate a bunch of these in a loop to find the fastest one.
	In rare cases this might return a slightly different estimate than combining [getLocalPingLocation](#getlocalpinglocation) with [estimatePingTimeBetweenTwoLocations](#estimatepingtimebetweentwolocations). That's because this function uses a slightly more complete set of information about what route would be taken.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#EstimatePingTimeFromLocalHost){ .md-button .md-button--store target="_blank" }

### getConfigValue

!!! function "	getConfigValue( ```int``` config_value, ```int``` scope_type, ```uint32_t``` connection_handle )"
	Get a configuration value.

	For values to pass to config_value, [check the SDK's listing.](https://partner.steamgames.com/doc/api/steamnetworkingtypes#ESteamNetworkingConfigValue){ target="_blank" }

	For values to pass to scope_type, [check the SDK's listing.](https://partner.steamgames.com/doc/api/steamnetworkingtypes#ESteamNetworkingConfigScope){ target="_blank" }

	**Returns:** dictionary

	Contains the following keys: 

	* result (int), which may be:
		* -3: buffer too small
		* -2: bad connection handle
		* -1: bad value
		* 1: OK
		* 2: OK, inherited
	* type (int)
	* value (PoolByteArray)
	* buffer (int)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetConfigValue){ .md-button .md-button--store target="_blank" }

### getConfigValueInfo

!!! function "getConfigValueInfo( ```int``` config_value )"
	Returns info about a configuration value.
	For values to pass to config_value, [check the SDK's listing.](https://partner.steamgames.com/doc/api/steamnetworkingtypes#ESteamNetworkingConfigValue){ target="_blank" }
	next_value can be used to iterate through all of the known configuration values.

	**Returns:** dictionary

	Contains the following keys:

	* type (int)
	* scope (int)
	* next_value (int)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetConfigValueInfo){ .md-button .md-button--store target="_blank" }

### getDirectPingToPOP

!!! function "getDirectPingToPOP( ```uint32``` pop_id )"
	Get *direct* ping time to the relays at the point of presence.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetDirectPingToPOP){ .md-button .md-button--store target="_blank" }

### getLocalPingLocation

!!! function "getLocalPingLocation()"
	Return location info for the current host. Returns the approximate age of the data, in seconds, or -1 if no data is available. You can use this value in [checkPingDataUpToDate](#checkpinguptodate).

	It takes a few seconds to initialize access to the relay network. If you call this very soon after calling [initializeRelayNetworkAccess](#initializerelaynetworkaccess), the data may not be available yet.

	This always return the most up-to-date information we have available right now, even if we are in the middle of re-calculating ping times.

	**Returns:** dictionary

	Contains the following keys:

	* location (PoolByteArray)
	* age (float)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetLocalPingLocation){ .md-button .md-button--store target="_blank" }

### getLocalTimestamp

!!! function "getLocalTimestamp()"
	A general purpose high resolution local timer with the following properties:

	* Monotonicity is guaranteed.
	* The initial value will be at least 24*3600*30*1e6, i.e. about 30 days worth of microseconds. In this way, the timestamp value of 0 will always be at least "30 days ago". Also, negative numbers will never be returned.
	* Wraparound / overflow is not a practical concern.

	If you are running under the debugger and stop the process, the clock might not advance the full wall clock time that has elapsed between calls. If the process is not blocked from normal operation, the timestamp values will track wall clock time, even if you don't call the function frequently.

	The value is only meaningful for this run of the process. Don't compare it to values obtained on another computer, or other runs of the same process.

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetLocalTimestamp){ .md-button .md-button--store target="_blank" }

### getPingToDataCenter

!!! function "	getPingToDataCenter( ```uint32``` pop_id )"
	Fetch ping time of best available relayed route from this host to the specified data center.

	**Returns:** dictionary

	Contains the following keys:

	* pop_relay (uint32)
	* ping (int)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetPingToDataCenter){ .md-button .md-button--store target="_blank" }

### getPOPCount

!!! function "getPOPCount()"
	Get number of network points of presence in the config.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetPOPCount){ .md-button .md-button--store target="_blank" }

### getPOPList

!!! function "getPOPList()"
	Get list of all POP IDs. Returns the number of entries that were filled into your list.

	**Returns:** array of POP IDs.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetPOPList){ .md-button .md-button--store target="_blank" }

### getRelayNetworkStatus

!!! function "getRelayNetworkStatus()"
	Fetch current status of the relay network.
	
	[relay_network_status](#relay_network_status) is also a callback. It will be triggered on both the user and gameserver interfaces any time the status changes, or ping measurement starts or stops.
	
	**Returns:** int which can be:

	* -102 - cannot try
	* -101 - failed
 	* -100 - previously worked, now there is a problem
	* -10 - retrying
	* 0 - unknown
	* 1 - never tried
	* 2 - waiting
	* 3 - attempting
	* 100 - current

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#GetRelayNetworkStatus){ .md-button .md-button--store target="_blank" }

### initRelayNetworkAccess

!!! function "initRelayNetworkAccess()"
	If you know that you are going to be using the relay network (for example, because you anticipate making P2P connections), call this to initialize the relay network. If you do not call this, the initialization will be delayed until the first time you use a feature that requires access to the relay network, which will delay that first access.
	
	You can also call this to force a retry if the previous attempt has failed. Performing any action that requires access to the relay network will also trigger a retry, and so calling this function is never strictly necessary, but it can be useful to call it a program launch time, if access to the relay network is anticipated. Use [getRelayNetworkStatus](#getrelaynetworkstatus) or listen for [relay_network_status](#relay_network_status) callbacks to know when initialization has completed. Typically initialization completes in a few seconds.
		
	**Returns:** void

	**Note:** dedicated servers hosted in known data centers do <strong>not</strong> need to call this, since they do not make routing decisions. However, if the dedicated server will be using P2P functionality, it will act as a "client" and this should be called.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#InitRelayNetworkAccess){ .md-button .md-button--store target="_blank" }

### parsePingLocationString

!!! function "parsePingLocationString( ```string``` location_string )"
	Parse back location_string string. Returns false if we couldn't understand the string.

	**Returns:** dictionary

	Contains the following keys:

	* success (bool)
	* ping_location (PoolByeArray)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#ParsePingLocationString){ .md-button .md-button--store target="_blank" }

### setConnectionConfigValueFloat

!!! function "	setConnectionConfigValueFloat( ```uint32``` connection, ```int``` config, ```float``` value )"
	Set a configuration value.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#SetConfigValue){ .md-button .md-button--store target="_blank" }

### setConnectionConfigValueInt32

!!! function "setConnectionConfigValueInt32( ```uint32``` connection, ```int``` config, ```int32``` value )"
	Set a configuration value.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#SetConfigValue){ .md-button .md-button--store target="_blank" }

### setConnectionConfigValueString

!!! function "setConnectionConfigValueString( ```uint32``` connection, ```int``` config, ```string``` value )"
	Set a configuration value.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#SetConfigValue){ .md-button .md-button--store target="_blank" }

### setConfigValue

!!! function "setConfigValue( ```int``` setting, ```int``` scope_type, ```int``` data_type )"
	Set a configuration value.

	Currently not enabled. Use [setConnectionConfigValue](#setconnectionconfigvalue) or [setGlobalConfigValue](#setglobalconfigvalue) functions.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#SetConfigValue){ .md-button .md-button--store target="_blank" }

### setGlobalConfigValueFloat

!!! function "setGlobalConfigValueFloat( ```int``` config, ```float``` value )"
	Set a configuration value.

	**Returns:** bool

   ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#shortcuts){ .md-button .md-button--store target="_blank" }

### setGlobalConfigValueInt32

!!! function "setGlobalConfigValueInt32( ```int``` config, ```int32``` value )"
	Set a configuration value.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#shortcuts){ .md-button .md-button--store target="_blank" }

### setGlobalConfigValueString

!!! function "setGlobalConfigValueString( ```int``` config, ```string``` value )"
	Set a configuration value.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingUtils#shortcuts){ .md-button .md-button--store target="_blank" }

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### relay_network_status

!!! function "relay_network_status"
	A struct used to describe our readiness to use the relay network.

	**Returns:**

	* available (int)
	* ping_measurement (int)
	* available_config (int)
	* available_relay (int)
	* debug_message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/steamnetworkingtypes#SteamRelayNetworkStatus_t){ .md-button .md-button--store target="_blank" }

---

## Enums

---

### NetworkingAvailability

Enumerator | Value
---------- | -----
NETWORKING_AVAILABILITY_CANNOT_TRY | -102
NETWORKING_AVAILABILITY_FAILED | -101
NETWORKING_AVAILABILITY_PREVIOUSLY | -100
NETWORKING_AVAILABILITY_UNKNOWN | 0
NETWORKING_AVAILABILITY_NEVER_TRIED | 1
NETWORKING_AVAILABILITY_WAITING | 2
NETWORKING_AVAILABILITY_ATTEMPTING | 3
NETWORKING_AVAILABILITY_CURRENT | 100
NETWORKING_AVAILABILITY_FORCE_32BIT | 0x7fffffff

### NetworkingConfigScope

Enumerator | Value
---------- | -----
NETWORKING_CONFIG_SCOPE_GLOBAL | 1
NETWORKING_CONFIG_SCOPE_SOCKETS_INTERFACE | 2
NETWORKING_CONFIG_SCOPE_LISTEN_SOCKET | 3
NETWORKING_CONFIG_SCOPE_CONNECTION | 4
NETWORKING_CONFIG_SCOPE_FORCE_32BIT | 0x7fffffff

### NetworkingConfigDataType

Enumerator | Value
---------- | -----
NETWORKING_CONFIG_TYPE_INT32 | 1
NETWORKING_CONFIG_TYPE_INT64 | 2
NETWORKING_CONFIG_TYPE_FLOAT | 3
NETWORKING_CONFIG_TYPE_STRING | 4
NETWORKING_CONFIG_TYPE_FUNCTION_PTR | 5
NETWORKING_CONFIG_TYPE_FORCE_32BIT | 0x7fffffff
