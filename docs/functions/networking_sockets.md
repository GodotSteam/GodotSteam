# Functions - Networking Sockets

---

## acceptConnection

!!! function "acceptConnection( ```uint32``` connection_handle )"
	Accept an incoming connection that has been received on a listen socket.

	**Returns:** int

## beginAsyncRequestFakeIP

!!! function "beginAsyncRequestFakeIP( ```int``` num_ports )"
	Begin asynchronous process of allocating a fake IPv4 address that other peers can use to contact us via P2P. IP addresses returned by this function are globally unique for a given app ID.

	Triggers a [fake_ip_result](/signals/networking_messages/#fake_ip_result) callback.

	**Returns:** bool

	_False_ if a request was already in progress, _true_ if a new request was started.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#AcceptConnection){ .md-button .md-button--store target="_blank" }

## closeConnection

!!! function "closeConnection( ```uint32``` peer, ```int``` reason, ```string``` debug_message, ```bool``` linger )"
	Disconnects from the remote host and invalidates the connection handle. Any unread data on the connection is discarded.

	**reason** is an application defined code that will be received on the other end and recorded (when possible) in backend analytics. The value should come from a restricted range. ([See ESteamNetConnectionEnd.](https://partner.steamgames.com/doc/api/steamnetworkingtypes#ESteamNetConnectionEnd){ target="_blank" }) If you don't need to communicate any information to the remote host, and do not want analytics to be able to distinguish "normal" connection terminations from "exceptional" ones, you may pass zero, in which case the generic value of **CONNECTION_END_APP_MIN** will be used.

	**debug_message** is an optional human-readable diagnostic string that will be received by the remote host and recorded (when possible) in backend analytics.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CloseConnection){ .md-button .md-button--store target="_blank" }

## closeListenSocket

!!! function "closeListenSocket( ```uint32``` socket )"
	Destroy a listen socket. All the connections that were accepted on the listen socket are closed ungracefully.

	**Returns:** bool

## configureConnectionLanes

!!! function "configureConnectionLanes()"
	Configure multiple outbound messages streams ("lanes") on a connection, and control head-of-line blocking between them.  Messages within a given lane are always sent in the order they are queued, but messages from different lanes may be sent out of order.  Each lane has its own message number sequence.  The first message sent on each lane will be assigned the number 1.

	Each lane has a "priority".  Lower priority lanes will only be processed when all higher-priority lanes are empty.  The magnitudes of the priority values are not relevant, only their sort order.  Higher numeric values take priority over lower numeric values.

	Each lane also is assigned a weight, which controls the approximate proportion of the bandwidth that will be consumed by the lane, relative to other lanes of the same priority.  (This is assuming the lane stays busy.  An idle lane does not build up "credits" to be be spent once a message is queued.) This value is only meaningful as a proportion, relative to other lanes with the same priority.  For lanes with different priorities, the strict priority order will prevail, and their weights relative to each other are not relevant. Thus, if a lane has a unique priority value, the weight value for that lane is not relevant.

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CloseListenSocket){ .md-button .md-button--store target="_blank" }

## connectP2P

!!! function "connectP2P( ```string``` identity_reference, ```int``` port, ```array``` options )"
	Begin connecting to a server that is identified using a platform-specific identifier. This uses the default rendezvous service, which depends on the platform and library configuration. (E.g. on Steam, it goes through the steam backend.) The traffic is relayed over the Steam Datagram Relay network.

	If you use this, you probably want to call [initRelayNetworkAccess](/functions/networking_sockets/#initrelaynetworkaccess) when your app initializes.  If you need to set any initial config options, pass them here.

	[See SteamNetworkingConfigValue_t](https://partner.steamgames.com/doc/api/steamnetworkingtypes#SteamNetworkingConfigValue_t){ target="_blank" } for more about why this is preferable to setting the options "immediately" after creation.

	Pass your **options** as an array of arrays; each sub-array containing:

	* config value (int)
	* data type (int)
	* data value (int or string)

	Alternately you can pass an empty array.

	**Returns:** uint32 

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#ConnectP2P){ .md-button .md-button--store target="_blank" }

## connectToHostedDedicatedServer

!!! function "connectToHostedDedicatedServer( ```string``` identity_reference, ```int``` virtual_port, ```array``` options )"
	Client call to connect to a server hosted in a Valve data center, on the specified virtual port. You must have placed a ticket for this server into the cache, or else this connect attempt will fail!

	Pass your **options** as an array of arrays; each sub-array containing:

	* config value (int)
	* data type (int)
	* data value (int or string)

	Alternately you can pass an empty array.

	**Returns:** uint32 

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#ConnectToHostedDedicatedServer){ .md-button .md-button--store target="_blank" }

## createFakeUDPPort

!!! function "createFakeUDPPort( ```int``` fake_server_port)"
	Get an interface that can be used like a UDP port to send/receive datagrams to a FakeIP address.  This is intended to make it easy to port existing UDP-based code to take advantage of SDR.

	**fake_server_port** refers to the *index* of the port allocated using [beginAsyncRequestFakeIP](/functions/networking_sockets/#beginasyncrequestfakeip) and is used to create "server" ports.  You may call this before the allocation has completed.  However, any attempts to send packets will fail until the allocation has succeeded.  When the peer receives packets sent from this interface, the from address of the packet will be the globally-unique FakeIP.  If you call this function multiple times and pass the same (nonnegative) fake port index, the same object will be returned, and this object is not reference counted.

	To create a "client" port (e.g. the equivalent of an ephemeral UDP port) pass -1.  In this case, a distinct object will be returned for each call. When the peer receives packets sent from this interface, the peer will assign a FakeIP from its own locally-controlled namespace.

	**Returns:** void

## createHostedDedicatedServerListenSocket

!!! function "createHostedDedicatedServerListenSocket( ```int``` virtual_port, ```array``` options )"
	Create a listen socket on the specified virtual port. The physical UDP port to use will be determined by the SDR_LISTEN_PORT environment variable. If a UDP port is not configured, this call will fail.

	This function should be used when you are using the ticket generator library to issue your own tickets.  Clients connecting to the server on this virtual port will need a ticket, and they must connect using [connectToHostedDedicatedServer](/functions/networking_sockets/#connecttohosteddedicatedserver).

	Pass your **options** as an array of arrays; each sub-array containing:

	* config value (int)
	* data type (int)
	* data value (int or string)

	Alternately you can pass an empty array.

	**Returns:** uint32 

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateHostedDedicatedServerListenSocket){ .md-button .md-button--store target="_blank" }

## createListenSocketIP

!!! function "createListenSocketIP( ```string``` ip_reference, ```Array``` options )"
	Creates a "server" socket that listens for clients to connect to by calling [connectByIPAddress](/functions/networking_sockets/#connectbyipaddress), over ordinary UDP (IPv4 or IPv6)

	Pass your **options** as an array of arrays; each sub-array containing:

	* config value (int)
	* data type (int)
	* data value (int or string)

	Alternately you can pass an empty array.

	**Returns:** uint32 

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateListenSocketIP){ .md-button .md-button--store target="_blank" }

## createListenSocketP2P

!!! function "createListenSocketP2P( ```int``` virtual_port, ```array``` options )"
	Like [createListenSocketIP](/functions/networking_sockets/#createlistensocketip), but clients will connect using [connectP2P](/functions/networking_sockets/#connectp2p). The connection will be relayed through the Valve network.

	**virtual_port** specifies how clients can connect to this socket using [connectP2P](/functions/networking_sockets/#connectp2p). It's very common for applications to only have one listening socket; in that case, use zero. If you need to open multiple listen sockets and have clients be able to connect to one or the other, then virtual_port should be a small integer (<1000) unique to each listen socket you create.

	Pass your **options** as an array of arrays; each sub-array containing:

	* config value (int)
	* data type (int)
	* data value (int or string)

	Alternately you can pass an empty array.

	**Returns:** uint32 

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateListenSocketP2P){ .md-button .md-button--store target="_blank" }

## createListenSocketP2PFakeIP

!!! function "createListenSocketP2PFakeIP( ```int``` fake_port, ```array``` options )"
	Create a listen socket that will listen for P2P connections sent to our FakeIP.  A peer can initiate connections to this listen socket by calling [connectByIPAddress](/functions/networking_sockets/#connectbyipaddress).

	**fake_port** refers to the *index* of the fake port requested, not the actual port number.  For example, pass 0 to refer to the first port in the reservation.  You must call this only after calling [beginAsyncRequestFakeIP](/functions/networking_sockets/#beginasyncrequestfakeip).  However, you do not need to wait for the request to complete before creating the listen socket.

	Pass your **options** as an array of arrays; each sub-array containing:

	* config value (int)
	* data type (int)
	* data value (int or string)

	Alternately you can pass an empty array.

	**Returns:** uint32 

## createPollGroup

!!! function "createPollGroup()"
	Create a new poll group.

	You should destroy the poll group when you are done using [destroyPollGroup](/functions/networking_sockets/#destroypollgroup).

	**Returns:** uint32 

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreatePollGroup){ .md-button .md-button--store target="_blank" }

## createSocketPair

!!! function "createSocketPair( ```bool``` loopback, ```string``` identity_reference1, ```string``` identity_reference2 )"
	Create a pair of connections that are talking to each other, e.g. a loopback connection. This is very useful for testing, or so that your client/server code can work the same even when you are running a local "server".

	The two connections will immediately be placed into the connected state, and no callbacks will be posted immediately. After this, if you close either connection, the other connection will receive a callback, exactly as if they were communicating over the network. You must close *both* sides in order to fully clean up the resources!

	By default, internal buffers are used, completely bypassing the network, the chopping up of messages into packets, encryption, copying the payload, etc. This means that loopback packets, by default, will not simulate lag or loss. Passing true for **loopback** will cause the socket pair to send packets through the local network loopback device (127.0.0.1) on ephemeral ports. Fake lag and loss are supported in this case, and CPU time is expended to encrypt and decrypt.

	If you wish to assign a specific identity to either connection, you may pass a particular identity. Otherwise, if you pass nullptr, the respective connection will assume a generic "localhost" identity. If you use real network loopback, this might be translated to the actual bound loopback port. Otherwise, the port will be zero.

	**Returns:** dictionary

	Contains the following keys:

	* success (bool)
	* connection1 (int)
	* connection2 (int)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateSocketPair){ .md-button .md-button--store target="_blank" }

## destroyPollGroup

!!! function "destroyPollGroup( ```uint32``` poll_group )"
	Destroy a poll group created with [createPollGroup](/functions/networking_sockets/#createpollgroup).

	If there are any connections in the poll group, they are removed from the group, and left in a state where they are not part of any poll group. Returns false if passed an invalid poll group handle.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#DestroyPollGroup){ .md-button .md-button--store target="_blank" }

## findRelayAuthTicketForServer

!!! function "findRelayAuthTicketForServer( ```int``` gameServer, ```int``` port )"
	Search cache for a ticket to talk to the server on the specified virtual port. If found, returns the number of seconds until the ticket expires, and optionally the complete cracked ticket. Returns 0 if we don't have a ticket.

	**Currently not enabled; requires datagram header.**

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#FindRelayAuthTicketForServer){ .md-button .md-button--store target="_blank" }

## flushMessagesOnConnection

!!! function "flushMessagesOnConnection( ```uint32``` connection_handle )"
	Flush any messages waiting on the Nagle timer and send them at the next transmission opportunity (often that means right now).

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#FlushMessagesOnConnection){ .md-button .md-button--store target="_blank" }

## getAuthenticationStatus

!!! function "getAuthenticationStatus()"
	Query our readiness to participate in authenticated communications. A [network_authentication_status](/signals/networking_messages/#network_authentication_status) callback is posted any time this status changes, but you can use this function to query it at any time.

	**Returns:** int

## getCertificateRequest

!!! function "getCertificateRequest()"
	Certificate provision by the application.  On Steam, we normally handle all this automatically and you will not need to use these advanced functions.

	Get blob that describes a certificate request.  You can send this to your game coordinator. Pass this blob to your game coordinator and call SteamDatagram_CreateCert.

	**Returns:** dictionary

	Contains the following keys:

	* certificate (int)
	* cert_size (int)
	* error_message (string)

## getConnectionInfo

!!! function "getConnectionInfo( ```uint32``` connection_handle )"
	Returns basic information about the high-level state of the connection. Returns false if the connection handle is invalid.

	**Returns:** dictionary

	Contains the following keys:

	* identity (string)
	* user_data (uint64)
	* listen_socket (uint32)
	* remote_address (string)
	* remote_pop (uint32)
	* pop_relay (uint32)
	* connection_state (int)
	* end_reason (int)
	* end_debug (string)
	* debug_description (string)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetConnectionInfo){ .md-button .md-button--store target="_blank" }

## getConnectionName

!!! function "getConnectionName( ```int``` peer )"
	Fetch connection name into your buffer, which is at least nMaxLen bytes. Returns false if handle is invalid.

	**Returns:** string

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetConnectionName){ .md-button .md-button--store target="_blank" }

## GetConnectionRealTimeStatus

!!! function "GetConnectionRealTimeStatus( ```uint32``` connection_handle, ```int``` lanes, ```bool``` get_status )" 
	Returns a small set of information about the real-time state of the connection and the queue status of each lane.

	**Returns:** dictionary

	Contains the following keys:

	* connection_status (dictionary):
		* state (int)
		* ping (int)
		* local_quality (float)
		* remote_quality (float)
		* packets_out_per_second (float)
		* bytes_out_per_second (float)
		* packets_in_per_second (float)
		* bytes_in_per_second (float)
		* send_rate (int)
		* pending_unreliable (int)
		* pending_reliable (int)
		* send_unacknowledged_reliable (int)
		* queue_time (uint64_t)

	* lanes_status (array):
		* lane_status (dictionary):
			* pending_unreliable (int)
			* pending_reliable (int)
			* sent_unacknowledged_reliatble (int)
			* queue_time (uint64_t)

## getConnectionUserData

!!! function "getConnectionUserData( ```uint32``` peer )"
	Fetch connection user data. Returns -1 if handle is invalid or if you haven't set any userdata on the connection.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetConnectionUserData){ .md-button .md-button--store target="_blank" }

## getDetailedConnectionStatus

!!! function "getDetailedConnectionStatus( ```uint32``` connection_handle )"
	Returns very detailed connection stats in diagnostic text format. Useful for dumping to a log, etc. The format of this information is subject to change.

	**Returns:** dictionary

	Contains the following keys:

	* success (int)
	* buffer (string)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetDetailedConnectionStatus){ .md-button .md-button--store target="_blank" }

## getFakeIP

!!! function "getFakeIP( ```int``` first_port )"
	Return info about the FakeIP and port(s) that we have been assigned, if any.

	**first_port** is currently reserved and must be zero. Make sure and check **result**.

	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* identity_type (int)
	* ip (uint32)
	* ports (uint16)

## getGameCoordinatorServerLogin

!!! function "getGameCoordinatorServerLogin()"
	Generate an authentication blob that can be used to securely login with your backend, using SteamDatagram_ParseHostedServerLogin. (See steamdatagram_gamecoordinator.h)

	**Currently not enabled.**

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetGameCoordinatorServerLogin){ .md-button .md-button--store target="_blank" }	

## getHostedDedicatedServerAddress

!!! function "getHostedDedicatedServerAddress()"
	Return info about the hosted server. This contains the PoPID of the server, and opaque routing information that can be used by the relays to send traffic to your server.

	**Currently not enabled.**

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetHostedDedicatedServerAddress){ .md-button .md-button--store target="_blank" }

## getHostedDedicatedServerPOPId

!!! function "getHostedDedicatedServerPOPId()"
	Returns 0 if SDR_LISTEN_PORT is not set. Otherwise, returns the data center the server is running in. This will be k_SteamDatagramPOPID_dev in non-production envirionment.

	**Returns:** uint32

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetHostedDedicatedServerPOPID){ .md-button .md-button--store target="_blank" }

## getHostedDedicatedServerPort

!!! function "getHostedDedicatedServerPort()"
	Returns the value of the SDR_LISTEN_PORT environment variable. This is the UDP server your server will be listening on. This will configured automatically for you in production environments.

	**Returns:** uint16

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetHostedDedicatedServerPort){ .md-button .md-button--store target="_blank" }

## getListenSocketAddress

!!! function "getListenSocketAddress( ```uint32``` socket )"
	Returns local IP and port that a listen socket created using CreateListenSocketIP is bound to.

	**Returns:** bool

	**Note:** This is not how you find out your public IP that clients can connect to.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetListenSocketAddress){ .md-button .md-button--store target="_blank" }

## getIdentity

!!! function "getIdentity()"
	Get the identity assigned to this interface.

	E.g. on Steam, this is the user's Steam ID, or for the gameserver interface, the Steam ID assigned to the gameserver. Returns false and sets the result to an invalid identity if we don't know our identity yet. (E.g. GameServer has not logged in. On Steam, the user will know their SteamID even if they are not signed into Steam.)

	**Returns:** string

## getRemoteFakeIPForConnection

!!! function "getRemoteFakeIPForConnection( ```uint32``` connection_handle )" 
	If the connection was initiated using the "FakeIP" system, then we we can get an IP address for the remote host.  If the remote host had a global FakeIP at the time the connection was established, this function will return that global IP.

	Otherwise, a FakeIP that is unique locally will be allocated from the local FakeIP address space, and that will be returned.

	This should also add the returning struct to your **ip_addresses** vector as **fake_ip_address**.

	**Returns:** dictionary

	Contains the following keys:

	* result (int)
	* port (uint16)
	* ip_type (int)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#GetIdentity){ .md-button .md-button--store target="_blank" }		

## initAuthentication

!!! function "initAuthentication()"
	Indicate our desire to be ready participate in authenticated communications. If we are currently not ready, then steps will be taken to obtain the necessary certificates. (This includes a certificate for us, as well as any CA certificates needed to authenticate peers.)

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#InitAuthentication){ .md-button .md-button--store target="_blank" }

## receiveMessagesOnConnection

!!! function "receiveMessagesOnConnection( ```uint32``` connection_handle, ```int``` max_messages )"
	Fetch the next available message(s) from the connection, if any. Returns the number of messages returned into your array, up to **max_messages**. If the connection handle is invalid, -1 is returned. If no data is available, 0, is returned.

	**Returns:** array 

	Contains a list of:

	* messages (dictionary):

	Contains the following keys:

	* payload (string)
	* size (int)
	* connection (int)
	* identity (string)
	* user_data (uint64_t)
	* time_received (uint64_t)
	* message_number (uint64_t)
	* channel (int)
	* flags (int)
	* user_data (uint64_t)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#ReceiveMessagesOnConnection){ .md-button .md-button--store target="_blank" }

## receiveMessagesOnPollGroup

!!! function "receiveMessagesOnPollGroup( ```uint32``` pollGroup, ```int``` max_messages )"
	Same as [receiveMessagesOnConnection](/functions/networking_sockets/#receivemessagesonconnection), but will return the next messages available on any connection in the poll group. Examine **connection** to know which connection. **user_data** might also be useful.

	**Returns:** array 

	Contains a list of:

	* messages (dictionary):

	Contains the following keys:

	* payload (string)
	* size (int)
	* connection (int)
	* identity (string)
	* user_data (uint64_t)
	* time_received (uint64_t)
	* message_number (uint64_t)
	* channel (int)
	* flags (int)
	* user_data (uint64_t)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#ReceiveMessagesOnPollGroup){ .md-button .md-button--store target="_blank" }

## receivedRelayAuthTicket

!!! function "receivedRelayAuthTicket()"
	Call this when you receive a ticket from your backend / matchmaking system. Puts the ticket into a persistent cache, and optionally returns the parsed ticket.

	**Currently not enabled.**

	**Returns:** dictionary

	Contains the following keys:

	* game_server (string)
	* authorized_client (string)
	* public_ip (uint32)
	* expiry (int32)
	* routing (int)
	* app_id (uint32)
	* restrict_to_v_port (int)
	* number_of_extras (int)
	* extra_fields (vector)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#ReceivedRelayAuthTicket){ .md-button .md-button--store target="_blank" }

## resetIdentity

!!! function "resetIdentity( ```string``` identity_reference )"
	Reset the identity associated with this instance. Any open connections are closed.  Any previous certificates, etc are discarded.
	You can pass a specific identity that you want to use, or you can pass NULL, in which case the identity will be invalid until you set it using [setCertificate](/functions/networking_sockets/#setcertificate).

	**Note:** This function is not actually supported on Steam!  It is included for use on other platforms where the active user can sign out and a new user can sign in.

	**Returns:** void

## runNetworkingCallbacks

!!! function "runNetworkingCallbacks()"
	Invoke all callback functions queued for this interface. You don't need to call this if you are using Steam's callback dispatch mechanism [run_callbacks](/functions/main/#run_callbacks).

	**Returns:** void

## sendMessages

!!! function "sendMessages( ```int``` messages, ```PoolByteArray``` data, ```uint32``` connection_handle, ```int``` flags )"
	Send one or more messages without copying the message payload. This is the most efficient way to send messages.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#SendMessages){ .md-button .md-button--store target="_blank" }

## sendMessageToConnection

!!! function "sendMessageToConnection( ```int``` connection_handle, ```PoolByteArray``` data, ```int``` flags )"
	Send a message to the remote host on the specified connection.

	**Returns:** dictionary

	* result (int)
	* message_number (int64)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#SendMessageToConnection){ .md-button .md-button--store target="_blank" }

## setConnectionPollGroup

!!! function "setConnectionPollGroup( ```uint32``` connection_handle, ```uint32``` pollGroup )"
	Assign a connection to a poll group. Note that a connection may only belong to a single poll group. Adding a connection to a poll group implicitly removes it from any other poll group it is in.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#SetConnectionPollGroup){ .md-button .md-button--store target="_blank" }

## setConnectionName

!!! function "setConnectionName( ```uint32``` peer, ```string``` name )"
	Set a name for the connection, used mostly for debugging.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#SetConnectionName){ .md-button .md-button--store target="_blank" }