# Functions - Networking Messages

---

## acceptSessionWithUser

!!! function "acceptSessionWithUser( ```string``` identity_reference )"
	Call this in response to a [network_messages_session_request](/signals/networking_messages/#network_messages_session_request) callback. [network_messages_session_request](/signals/networking_messages/#network_messages_session_request) callbacks are posted when a user tries to send you a message, and you haven't tried to talk to them first. If you don't want to talk to them, just ignore the request. If the user continues to send you messages, [network_messages_session_request](/signals/networking_messages/#network_messages_session_request) callbacks will continue to be posted periodically.

	Returns false if there is no session with the user pending or otherwise. If there is an existing active session, this function will return true, even if it is not pending.

	Calling [sendMessageToUser](/functions/networking_messages/#sendmessagetouser) will implicitly accepts any pending session request to that user.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingMessages#AcceptSessionWithUser){ .md-button .md-button--store target="_blank" }

## closeChannelWithUser

!!! function "closeChannelWithUser( ```string``` identity_reference, ```int``` channel )"
	Call this  when you're done talking to a user on a specific channel. Once all open channels to a user have been closed, the open session to the user will be closed, and any new data from this user will trigger a [network_messages_session_request](/signals/networking_messages/#network_messages_session_request) callback.

	**Returns:** bool

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingMessages#CloseChannelWithUser){ .md-button .md-button--store target="_blank" }

## closeSessionWithUser

!!! function "closeSessionWithUser( ```string``` identity_reference )"
	Call this when you're done talking to a user to immediately free up resources under-the-hood. If the remote user tries to send data to you again, another [network_messages_session_request](/signals/networking_messages/#network_messages_session_request) callback will be posted.

	Note that sessions that go unused for a few minutes are automatically timed out.

	**Returns:** bool.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingMessages#CloseSessionWithUser){ .md-button .md-button--store target="_blank" }

## getSessionConnectionInfo(

!!! function "getSessionConnectionInfo```string``` identity_reference, ```bool``` get_connection, ```bool``` get_status )"
	Returns information about the latest state of a connection, if any, with the given peer. Primarily intended for debugging purposes, but can also be used to get more detailed failure information.
	
	Returns the value of connection state or 0 if no connection exists with specified peer. You may pass _false_ for either **get_connection** or **get_status** if you do not need the corresponding details. Note that sessions time out after a while, so if a connection fails, or [sendMessageToUser](/functions/networking_messages/#sendmessagetouser) returns 3, you cannot wait indefinitely to obtain the reason for failure.

	**Returns:** dictionary

	Contains the following keys:

	* state (int) with values being:

		* 0 - none
		* 1 - connecting
		* 2 - finding route
		* 3 - connected
		* 4 - closed by peer
		* 5 - problem detected locally
		* -1 - wait
		* -2 - linger
		* -3 - dead

	If **get_connection** is true:

	* identity (string)
	* user_data (uint64_t)
	* listen_socket (uint32)
	* remote_address (string)
	* remote_pop (uint32)
	* pop_relay (uint32)
	* connection_state (int); same as above states
	* end_reason (int)
	* end_debug (string)
	* debug_description (string)

	If **get_status** is true:

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

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingMessages#GetSessionConnectionInfo){ .md-button .md-button--store target="_blank" }

## receiveMessagesOnChannel

!!! function "receiveMessagesOnChannel( ```int``` channel, ```int``` max_messages )"	
	Reads the next message that has been sent from another user via [sendMessageToUser](/functions/networking_messages/#sendmessagetouser) on the given channel. Returns number of messages returned into your list. (0 if no message are available on that channel.)

	**Returns:** array
	
	Contains a list of:

	* messages (dictionary)

	Contains the following keys:

	* payload (string)
	* size (int)
	* connection (uint32)
	* identity (string)
	* user_data (uint64_t)
	* time_received (uint64_t)
	* message_number (uint64_t)
	* channel (int)
	* flags (int)
	* user_data (uint64_t)
	
	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingMessages#ReceiveMessagesOnChannel){ .md-button .md-button--store target="_blank" }

## sendMessageToUser

!!! function "sendMessageToUser( ```string``` identity_reference, ```PoolByteArray``` data, ```int``` flags, ```int``` channel )"
	Sends a message to the specified host via **identity_reference**. If we don't already have a session with that user, a session is implicitly created. There might be some handshaking that needs to happen before we can actually begin sending message data. If this handshaking fails and we can't get through, an error will be posted via the callback [network_messages_session_failed](/signals/networking_messages/#network_messages_session_failed). There is no notification when the operation succeeds. (You should have the peer send a reply for this purpose.)
	
	Sending a message to a host will also implicitly accept any incoming connection from that host.
	
	**flags** is a bitmask of k_nSteamNetworkingSend_xxx options.
	
	**channel** is a routing number you can use to help route message to different systems. You'll have to call [receiveMessagesOnChannel](/functions/networking_messages/#receivemessagesonchannel) with the same channel number in order to retrieve the data on the other end.
	
	Using different channels to talk to the same user will still use the same underlying connection, saving on resources. If you don't need this feature, use 0. Otherwise, small integers are the most efficient.
	
	It is guaranteed that reliable messages to the same host on the same channel will be be received by the remote host (if they are received at all) exactly once, and in the same order that they were sent.
	
	No other order guarantees exist! In particular, unreliable messages may be dropped, received out of order with respect to each other and with respect to reliable data, or may be received multiple times. Messages on different channels are *not* guaranteed to be received in the order they were sent.
	
	A note for those familiar with TCP/IP ports, or converting an existing codebase that opened multiple sockets: You might notice that there is only one channel, and with TCP/IP each endpoint has a port number. You can think of the channel number as the destination port. If you need each message to also include a source port (so the recipient can route the reply), then just put that in your message. That is essentially how UDP works!
	
	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamNetworkingMessages#SendMessageToUser){ .md-button .md-button--store target="_blank" }