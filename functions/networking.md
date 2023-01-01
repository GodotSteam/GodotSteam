# Functions - Networking

---

## acceptP2PSessionWithUser

!!! function "acceptP2PSessionWithUser( ```uint64_t``` steam_id_remote )"
	This allows the game to specify accept an incoming packet. This needs to be called before a real connection is established to a remote host, the game will get a chance to say whether or not the remote user is allowed to talk to them.

	When a remote user that you haven't sent a packet to recently, tries to first send you a packet, your game will receive a callback [p2p_session_request](/signals/networking/#). This callback contains the Steam ID of the user who wants to send you a packet. In response to this callback, you'll want to see if it's someone you want to talk to (for example, if they're in a lobby with you), and if so, accept the connection; otherwise if you don't want to talk to the user, just ignore the request. If the user continues to send you packets, another [p2p_session_request](/signals/networking/#) will be posted periodically. If you've called <strong>sendP2PPacket</strong> on the other user, this implicitly accepts the session request.

	**Returns:** bool

	**Note:** This call should only be made in response to a [p2p_session_request](/signals/networking/#) callback.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#AcceptP2PSessionWithUser){ .md-button .md-button--store target="_blank" }

## allowP2PPacketRelay

!!! function "allowP2PPacketRelay( ```bool``` allow )"
	Allow or disallow P2P connections to fall back to being relayed through the Steam servers if a direct connection or NAT-traversal cannot be established.

	This only applies to connections created after setting this value, or to existing connections that need to automatically reconnect after this value is set.

	P2P packet relay is allowed by default.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#AllowP2PPacketRelay){ .md-button .md-button--store target="_blank" }

## closeP2PChannelWithUser

!!! function "closeP2PChannelWithUser( ```uint64_t``` steam_id_remote, ```int``` channel )"
	Closes a P2P channel when you're done talking to a user on the specific channel.

	Once all channels to a user have been closed, the open session to the user will be closed and new data from this user will trigger a new [p2p_session_request](/signals/networking/#p2p_session_request) callback.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#CloseP2PChannelWithUser){ .md-button .md-button--store target="_blank" }

## closeP2PSessionWithUser

!!! function "closeP2PSessionWithUser( ```uint64_t``` steam_id_remote )"
	This should be called when you're done communicating with a user, as this will free up all of the resources allocated for the connection under-the-hood.

	If the remote user tries to send data to you again, a new [p2p_session_request](/signals/networking/#p2p_session_request) callback will be posted.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#CloseP2PSessionWithUser){ .md-button .md-button--store target="_blank" }
 
## getP2PSessionState

!!! function "getP2PSessionState( ```uint64_t``` steam_id_remote )"
	Fills out a dictionary with details about the connection like whether or not there is an active connection; number of bytes queued on the connection; the last error code, if any; whether or not a relay server is being used; and the IP and Port of the remote user, if known.

	This should only needed for debugging purposes.

	**Returns:** dictionary

	Contains the following keys:

	* connection_active (bool)
	* connecting (bool)
	* session_error (uint8)
	* using_relay (bool)
	* bytes_queued_for_send (int32)
	* packets_queued_for_send (int32)
	* remote_ip (uint32)
	* remote_port (uint16)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#GetP2PSessionState){ .md-button .md-button--store target="_blank" }

## getAvailableP2PPacketSize

!!! function "getAvailableP2PPacketSize( ```int``` channel )"
	Calls [IsP2PPacketAvailable](https://partner.steamgames.com/doc/api/ISteamNetworking#IsP2PPacketAvailable){ target="_blank" } under the hood, returns the size of the available packet or zero if there is no such packet.

	**Returns:** uint32_t

## readP2PPacket

!!! function "readP2PPacket( ```uint32_t``` packet, ```int``` channel )"
	Reads in a packet that has been sent from another user via SendP2PPacket.

	This call is not blocking, and will return false if no data is available.

	Before calling this you should have called [getAvailableP2PPacketSize](/functions/networking/#getavailablep2ppacketsize) / [isP2PPacketAvailable](/functions/networking/#isp2ppacketavailable).

	**Returns:** dictionary

	* data (PoolByteArray)
	* steam_id_remote (uint64_t)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#ReadP2PPacket){ .md-button .md-button--store target="_blank" }

## sendP2PPacket

!!! function "sendP2PPacket( ```uint64_t``` steam_id_remote, ```array``` data, ```int``` send_type, ```int``` channel )"
	Sends a P2P packet to the specified user.

	This is a session-less API which automatically establishes NAT-traversing or Steam relay server connections.

	See [EP2PSend](https://partner.steamgames.com/doc/api/ISteamNetworking#EP2PSend){ target="_blank" } for descriptions of the different ways of sending packets. The integers passed in GodotSteam are:

	* 0 - send ureliable
	* 1 - send unreliable, no delay
	* 2 - send reliable
	* 3 - send reliable with buffering

	The type of data you send is arbitrary, you can use an off the shelf system like [Protocol Buffers](https://developers.google.com/protocol-buffers/){ target="_blank" } or [Cap'n Proto](https://capnproto.org/){ target="_blank" } to encode your packets in an efficient way, or you can create your own messaging system.

	Sends a P2P packet to the specified user.

	Triggers a [p2p_session_request](/signals/networking/#p2p_session_request) callback.

	**Returns:** bool

	_True_ if the packet was successfully sent. Note that this does not mean successfully received, if we can't get through to the user after a timeout of 20 seconds, then an error will be posted via the [p2p_session_connect_fail](/signals/networking/#p2p_session_connect_fail) callback.

	**Note:** The first packet send may be delayed as the NAT-traversal code runs.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworking#SendP2PPacket){ .md-button .md-button--store target="_blank" }