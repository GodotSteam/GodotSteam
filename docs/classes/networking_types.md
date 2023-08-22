# Networking Types

Miscellaneous types and functions used by networking APIs.

---

## Functions

These Networking Type functions are all unique to GodotSteam since we cannot work with C++ structs directly in GDscript.  These will create networking identities to use with [Networking Messages](../networking_messages/), [Networking Sockets](../networking_sockets/), and [Networking Utils](../networking_utils/) classes.  Much like how it works in a C++ implementation, the struct must be created (with either [addIdentity](#addidentity) or [addIPAddress](#addipaddress)) then it must be populated with data (Steam ID, IP address, etc.).

---

### addIdentity

!!! function "addIdentity( ```string``` reference_name )"
	Create a new network identity struct and store it for use.  When this network identity is used in other functions, you will always use the reference_name to use this struct.
	
	You will have to set the IP, Steam ID, string, or bytes with other functions below otherwise the identity is invalid.

	**Returns:** bool

### addIPAddress

!!! function "addIPAddress( ```string``` refrence_name )"
	Add a new IP address struct and store it for use.  When this networking IP address is used in other functions, you will always use the ```reference_name``` to use this struct.

	**Returns:** bool

### clearIPAddress

!!! function "clearIPAddress( ```string``` reference_name )"
	IP Address - Set everything to zero. E.g. [::]:0.

	**Returns:** void

### clearIdentity

!!! function "clearIdentity( ```string``` reference_name )"
	Clear a network identity's data.

	**Returns:** void

### getGenericBytes

!!! function "getGenericBytes( ```string``` reference_name )"
	Returns null if not generic bytes type.

	**Returns:** uint8

### getGenericString

!!! function "getGenericString( ```string``` reference_name )"
	Returns null if not generic string type.

	**Returns:** string

### getIdentities

!!! function "getIdentities()"
	Get a list of all known network identities.

	**Returns:** array

	Contains a list of:

	* this_identity (dictionary)
		* reference_name (string)
		* steam_id (uint64_t)
		* type (int)

### getIdentityIPAddr

!!! function "getIdentityIPAddr( ```string``` reference_name )"
	Returns null if we are not an IP address.

	**Returns:** uint32

### getIdentitySteamID

!!! function "getIdentitySteamID( ```string``` reference_name )"
	Return back a Steam ID32 or 0 if identity is not a Steam ID.

	**Returns:** uint32	

### getIdentitySteamID64

!!! function "getIdentitySteamID64( ```string``` reference_name )"
	Returns 0 if identity is not a Steam ID.

	**Returns:** uint64_t

### getIPAddresses

!!! function "getIPAddresses()"
	Get a list of all IP address structs and their names.

	**Returns:** array

	Contains a list of:

	* this_address (dictionary)
		* reference_name (string)
		* localhost (bool)
		* ip_address (uint32)

### getIPv4

!!! function "getIPv4( ```string``` reference_name )"
	Returns IP in host byte order (e.g. aa.bb.cc.dd as 0xaabbccdd). Returns 0 if IP is not mapped IPv4.

	**Returns:** uint32

### getPSNID

!!! function "getPSNID( ```string``` reference_name )"
	Retrieve this identity's Playstation Network ID.

	**Returns:** uint64	

### getStadiaID

!!! function "getStadiaID( ```string``` reference_name )"
	Retrieve this identity's Google Stadia ID.

	**Returns:** uint64

### getXboxPairwiseID

!!! function "getXboxPairwiseID( ```string``` reference_name )"
	Retrieve this identity's XBox pair ID.

	**Returns:** string

### isAddressLocalHost

!!! function "isAddressLocalHost( ```string``` reference_name )"
	Return true if this identity is localhost. (Either IPv6 ::1, or IPv4 127.0.0.1).

	**Returns:** bool

### isIdentityInvalid

!!! function "isIdentityInvalid( ```string``` reference_name )"
	Return true if we are the invalid type.  Does not make any other validity checks (e.g. is SteamID actually valid).

	**Returns:** bool

### isIdentityLocalHost

!!! function "isIdentityLocalHost( ```string``` reference_name )"
	Return true if this identity is localhost.

	**Returns:** bool

### isIPv4

!!! function "isIPv4( ```string``` reference_name )"
	Return true if IP is mapped IPv4.

	**Returns:** bool

### isIPv6AllZeros

!!! function "isIPv6AllZeros( ```string``` reference_name )"
	Return true if the IP is ::0. (Doesn't check port.).

	**Returns:** bool

### parseIPAddressString

!!! function "parseIPAddressString( ```string``` reference_name )"
	Parse an IP address and optional port.  If a port is not present, it is set to 0. (This means that you cannot tell if a zero port was explicitly specified.).

	**Returns:** string

### parseIdentityString

!!! function "parseIdentityString( ```string``` reference_name )"
	Parse back a string that was generated using ToString. If we don't understand the string, but it looks "reasonable" (it matches the pattern ```type:<type-data>``` and doesn't have any funky characters, etc), then we will return true, and the type is set to k_ESteamNetworkingIdentityType_UnknownType (0). False will only be returned if the string looks invalid.

	**Returns:** string

### setGenericBytes

!!! function "setGenericBytes( ```string``` reference_name, ```uint8``` data )"
	Returns false if invalid size.

	**Returns:** bool

### setIdentityIPAddr

!!! function "setIdentityIPAddr( ```string``` reference_name, ```string``` ip_address_name )"
	Set to specified IP:port.

	**Returns:** bool

### setIdentityLocalHost

!!! function "setIdentityLocalHost( ```string``` reference_name )"
	Set to localhost. (We always use IPv6 ::1 for this, not 127.0.0.1).

	**Returns:** void

### setIdentitySteamID

!!! function "setIdentitySteamID( ```string``` reference_name, ```uint32``` steam_id )"
	Set a 32-bit Steam ID.

	**Returns:** void

### setIdentitySteamID64

!!! function "setIdentitySteamID64( ```string``` reference_name, ```uint64_t``` steam_id )"
	Takes SteamID as raw 64-bit number.
	
	**Returns:** void

### setGenericString

!!! function "setGenericString( ```string``` reference_name, ```string``` this_string )"
	Returns false if invalid length.

	**Returns:** bool

### setPSNID

!!! function "setPSNID( ```string``` reference_name, ```uint64_t``` psn_id )"
	Set the Playstation Network ID for this identity.

	**Returns:** void

### setStadiaID

!!! function "setStadiaID( ```string``` reference_name, ```uint64_t``` stadia_id )"
	Set the Google Stadia ID for this identity.

	**Returns:** void

### setXboxPairwiseID

!!! function "setXboxPairwiseID( ```string``` reference_name, ```string``` xbox_id )"
	Set the Xbox Pairwise ID for this identity.

	**Returns:** bool

### toIdentityString

!!! function "toIdentityString( ```string``` reference_name )"
	Print to a human-readable string.  This is suitable for debug messages or any other time you need to encode the identity as a string. It has a URL-like format ```type:<type-data>```. Your buffer should be at least k_cchMaxString (128) bytes big to avoid truncation.

	**Returns:** void

### toIPAddressString

!!! function "toIPAddressString( ```string``` reference_name, ```bool``` with_port )"
	Print to a string, with or without the port. Mapped IPv4 addresses are printed as dotted decimal (12.34.56.78), otherwise this will print the canonical form according to RFC5952. If you include the port, IPv6 will be surrounded by brackets, e.g. [::1:2]:80. Your buffer should be at least k_cchMaxString (128) bytes to avoid truncation.

	**Returns:** string

### setIPv4

!!! function "setIPv4( ```string``` reference_name, ```uint32``` ip, ```uint16``` port )"
	Sets to IPv4 mapped address. IP and port are in host byte order.

	**Returns:** void

### setIPv6

!!! function "setIPv6( ```string``` reference_name, ```uint8``` ipv6, ```uint16``` port )"
	Set IPv6 address. IP is interpreted as bytes, so there are no endian issues. (Same as inaddr_in6.) The IP can be a mapped IPv4 address.

	**Returns:** void

### setIPv6LocalHost

!!! function "setIPv6LocalHost( ```string``` reference_name, ```uint16``` port )"
	Set to the IPv6 localhost address ::1, and the specified port.

	**Returns:** void

