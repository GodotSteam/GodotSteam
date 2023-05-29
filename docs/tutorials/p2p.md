# Tutorials - P2P Networking

One of the more requested tutorials is multiplayer lobbies and P2P networking through Steam; this tutorial specifically covers the P2P Networking portion and [our lobbies tutorial covers the other half](/tutorials/lobbies).  Please note this tutorial uses the **older Steamworks Networking class** and this is for a basic, turn-based lobby / P2P set-up.  Only use this as a starting point.

I'd also like to suggest you [check out the Additional Resources section of this tutorial](/tutorials/p2p/#additional-resources) before continuing on.

---

## The ***_ready()*** Function

Next we'll want to set up the signal connections for Steamworks and a command line checker like so:

=== "Godot 2.x, 3.x"
	````
	func _ready() -> void:
		Steam.connect("p2p_session_request", self, "_on_P2P_Session_Request")
		Steam.connect("p2p_session_connect_fail", self, "_on_P2P_Session_Connect_Fail")

		# Check for command line arguments
		_check_Command_Line()
	````
=== "Godot 4.x"
	````
	func _ready() -> void:
		Steam.p2p_session_request.connect(_on_P2P_Session_Request)
		Steam.p2p_session_connect_fail.connect(_on_P2P_Session_Connect_Fail)

		# Check for command line arguments
		_check_Command_Line()
	````

We will get into each of these below.

---

## The ***_process()*** Function

We'll also need to set ***_read_P2P_Packet()*** in our ***_process()*** function so it is always looking for new packets:

````
func _process(_delta) -> void:
	Steam.run_callbacks()

	# If the player is connected, read packets
	if LOBBY_ID > 0:
		_read_P2P_Packet()
````

If you are using the **global.gd** autoload singleton then you can omit the ***Steam.run_callbacks()*** command as they'll be running already.

Here is a nice bit of code from **tehsquidge** for handling packet reading:

````
func _process(delta):
	Steam.run_callbacks()

	if LOBBY_ID > 0:
		_read_All_P2P_Packets()


func _read_All_P2P_Packets(read_count: int = 0):
	if read_count >= PACKET_READ_LIMIT:
		return
	if Steam.getAvailableP2PPacketSize(0) > 0:
		_read_P2P_Packet()
		_read_All_P2P_Packets(read_count + 1)
````

---

## P2P Networking - Session Request

Next we'll check out the P2P networking functionality. [Over in the lobby tutorial](/tutorials/lobbies), we did a P2P handshake when someone joins the lobby, it would trigger a **p2p_session_request** callback which would in turn trigger this function:

````
func _on_P2P_Session_Request(remote_id: int) -> void:
	# Get the requester's name
	var REQUESTER: String = Steam.getFriendPersonaName(remote_id)

	# Accept the P2P session; can apply logic to deny this request if needed
	Steam.acceptP2PSessionWithUser(remote_id)

	# Make the initial handshake
	_make_P2P_Handshake()
````

It pretty simply acknowledges the session request, accepts it, then sends a handshake back.


---

## Reading P2P Packets

Inside that handshake there was a call to the ***_read_P2P_Packet()*** function which does this:

=== "Godot 2.x, 3.x"
	````
	func _read_P2P_Packet() -> void:
		var PACKET_SIZE: int = Steam.getAvailableP2PPacketSize(0)

		# There is a packet
		if PACKET_SIZE > 0:
			var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)

			if PACKET.empty() or PACKET == null:
				print("WARNING: read an empty packet with non-zero size!")

			# Get the remote user's ID
			var PACKET_SENDER: int = PACKET['steam_id_remote']

			# Make the packet data readable
			var PACKET_CODE: PoolByteArray = PACKET['data']
			var READABLE: Dictionary = bytes2var(PACKET_CODE)

			# Print the packet to output
			print("Packet: "+str(READABLE))

			# Append logic here to deal with packet data
	````
=== "Godot 4.x"
	````
	func _read_P2P_Packet() -> void:
		var PACKET_SIZE: int = Steam.getAvailableP2PPacketSize(0)

		# There is a packet
		if PACKET_SIZE > 0:
			var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)

			if PACKET.empty() or PACKET == null:
				print("WARNING: read an empty packet with non-zero size!")

			# Get the remote user's ID
			var PACKET_SENDER: int = PACKET['steam_id_remote']

			# Make the packet data readable
			var PACKET_CODE: PackedByteArray = PACKET['data']
			var READABLE: Dictionary = bytes_to_var(PACKET_CODE)

			# Print the packet to output
			print("Packet: "+str(READABLE))

			# Append logic here to deal with packet data
	````

If the packet size is greater than zero then it will get the sender's Steam ID and the data they sent. The line **bytes2var** (Godot 2x., 3.x) or **bytes_to_var** (Godot 4.x) is very important as it decodes the data back into something you can read and use. After it is decoded you can pass that data to any number of functions for your game.

---

## Sending P2P Packets

Beyond the handshake, you will probably want to pass a lot of different pieces of data back and forth between players. I have mine set up with two arguments: the first is the recipient as a string and the second is a dictionary. I think the dictionary is best for sending data so you can have a key / value pair to reference and make things less confusing on the receiving end. Each packet will go through the following function:

=== "Godot 2.x, 3.x"
	````
	func _send_P2P_Packet(target: int, packet_data: Dictionary) -> void:
		# Set the send_type and channel
		var SEND_TYPE: int = Steam.P2P_SEND_RELIABLE
		var CHANNEL: int = 0

		# Create a data array to send the data through
		var DATA: PoolByteArray
		DATA.append_array(var2bytes(packet_data))

		# If sending a packet to everyone
		if target == 0:
			# If there is more than one user, send packets
			if LOBBY_MEMBERS.size() > 1:
				# Loop through all members that aren't you
				for MEMBER in LOBBY_MEMBERS:
					if MEMBER['steam_id'] != STEAM_ID:
						Steam.sendP2PPacket(MEMBER['steam_id'], DATA, SEND_TYPE, CHANNEL)
		# Else send it to someone specific
		else:
			Steam.sendP2PPacket(target, DATA, SEND_TYPE, CHANNEL)
	````
=== "Godot 4.x"
	````
	func _send_P2P_Packet(target: int, packet_data: Dictionary) -> void:
		# Set the send_type and channel
		var SEND_TYPE: int = Steam.P2P_SEND_RELIABLE
		var CHANNEL: int = 0

		# Create a data array to send the data through
		var DATA: PackedByteArray
		DATA.append_array(var_to_bytes(packet_data))

		# If sending a packet to everyone
		if target == 0:
			# If there is more than one user, send packets
			if LOBBY_MEMBERS.size() > 1:
				# Loop through all members that aren't you
				for MEMBER in LOBBY_MEMBERS:
					if MEMBER['steam_id'] != STEAM_ID:
						Steam.sendP2PPacket(MEMBER['steam_id'], DATA, SEND_TYPE, CHANNEL)
		# Else send it to someone specific
		else:
			Steam.sendP2PPacket(target, DATA, SEND_TYPE, CHANNEL)
	````

The **send_type** variable will corresponed to these integers:

- **0** - Send unreliable
- **1** - Send unreliable with no delay
- **2** - Send reliable
- **3** - Send reliable with buffering

The channel used should match for both read and send functions. You may want to use multiple channels so this should obviously be adjusted.

As your game increases in complexity, you may find the amount of data you're sending increases significantly. One of the core tenets of responsive, effective networking is reducing the amount of data you're sending, to reduce the chance of some part becoming corrupted or requiring players of your game to have a very fast internet connection to even play your game.

Luckily, we can introduce **compression** to our send function to shrink the size of the data without needing to change our whole dictionary. The concept is straightforward enough; when we call the **var2bytes** (Godot 2.x, 3.x) or **var_to_bytes** (Godot 4.x) function, we turn our dictionary (or some other variable) into a **PoolByteArray** (Godot 2.x, 3.x) or **PackedByteArray** (Godot 4.x) and send it over the internet. 

We can compress the PoolByteArray/PackedByteArray to be smaller with a single line of code:

=== "Godot 2.x, 3.x"
	````
	func _send_P2P_Packet(target: int, packet_data: Dictionary) -> void:
		# Set the send_type and channel
		var SEND_TYPE: int = Steam.P2P_SEND_RELIABLE
		var CHANNEL: int = 0

		# Create a data array to send the data through
		var DATA: PoolByteArray
		# Compress the PoolByteArray we create from our dictionary  using the GZIP compression method
		var COMPRESSED_DATA: PoolByteArray = var2bytes(packet_data).compress(File.COMPRESSION_GZIP)
		DATA.append_array(COMPRESSED_DATA)

		# If sending a packet to everyone
		if target == 0:
			# If there is more than one user, send packets
			if LOBBY_MEMBERS.size() > 1:
				# Loop through all members that aren't you
				for MEMBER in LOBBY_MEMBERS:
					if MEMBER['steam_id'] != STEAM_ID:
						Steam.sendP2PPacket(MEMBER['steam_id'], DATA, SEND_TYPE, CHANNEL)
		# Else send it to someone specific
		else:
			Steam.sendP2PPacket(target, DATA, SEND_TYPE, CHANNEL)
	````
=== "Godot 4.x"
	````
	func _send_P2P_Packet(target: int, packet_data: Dictionary) -> void:
		# Set the send_type and channel
		var SEND_TYPE: int = Steam.P2P_SEND_RELIABLE
		var CHANNEL: int = 0

		# Create a data array to send the data through
		var DATA: PackedByteArray
		# Compress the PackedByteArray we create from our dictionary  using the GZIP compression method
		var COMPRESSED_DATA: PackedByteArray = var_to_bytes(packet_data).compress(File.COMPRESSION_GZIP)
		DATA.append_array(COMPRESSED_DATA)

		# If sending a packet to everyone
		if target == 0:
			# If there is more than one user, send packets
			if LOBBY_MEMBERS.size() > 1:
				# Loop through all members that aren't you
				for MEMBER in LOBBY_MEMBERS:
					if MEMBER['steam_id'] != STEAM_ID:
						Steam.sendP2PPacket(MEMBER['steam_id'], DATA, SEND_TYPE, CHANNEL)
		# Else send it to someone specific
		else:
			Steam.sendP2PPacket(target, DATA, SEND_TYPE, CHANNEL)
	````

Of course, we've now sent a **compressed** PoolByteArray / PackedByteArray to someone else over the internet, so when they receive the packet, they will need to first **decompress** the PoolByteArray / PackedByteArray before they can decode it.
To accomplish this, we add a single line of code to our **\_read_P2P_Packet** function like so:

=== "Godot 2.x, 3.x"
	````
	func _read_P2P_Packet() -> void:
		var PACKET_SIZE: int = Steam.getAvailableP2PPacketSize(0)

		# There is a packet
		if PACKET_SIZE > 0:
			var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)

			if PACKET.empty() or PACKET == null:
				print("WARNING: read an empty packet with non-zero size!")

			# Get the remote user's ID
			var PACKET_SENDER: int = PACKET['steam_id_remote']

			# Make the packet data readable
			var PACKET_CODE: PoolByteArray = PACKET['data']
			# Decompress the array before turning it into a useable dictionary
			var READABLE: Dictionary = bytes2var(PACKET_CODE.decompress_dynamic(-1, File.COMPRESSION_GZIP))

			# Print the packet to output
			print("Packet: "+str(READABLE))

			# Append logic here to deal with packet data
	````
=== "Godot 4.x"
	````
	func _read_P2P_Packet() -> void:
		var PACKET_SIZE: int = Steam.getAvailableP2PPacketSize(0)

		# There is a packet
		if PACKET_SIZE > 0:
			var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)

			if PACKET.empty() or PACKET == null:
				print("WARNING: read an empty packet with non-zero size!")

			# Get the remote user's ID
			var PACKET_SENDER: int = PACKET['steam_id_remote']

			# Make the packet data readable
			var PACKET_CODE: PackedByteArray = PACKET['data']
			# Decompress the array before turning it into a useable dictionary
			var READABLE: Dictionary = bytes_to_var(PACKET_CODE.decompress_dynamic(-1, File.COMPRESSION_GZIP))

			# Print the packet to output
			print("Packet: "+str(READABLE))

			# Append logic here to deal with packet data
	````

The key point to note here is the format **must be the same for sending and receiving**. There's a whole lot to read about compression in Godot, far beyond this tutorial; to learn more, [read all about it here.](https://docs.godotengine.org/en/stable/classes/class_poolbytearray.html#class-poolbytearray-method-compress)

---

## P2P Session Failures
	
For the last part of this tutorial we'll handle P2P failures with the following function which is triggered by the **p2p_session_connect_fail** callback:

````
func _on_P2P_Session_Connect_Fail(steamID: int, session_error: int) -> void:
	# If no error was given
	if session_error == 0:
		print("WARNING: Session failure with "+str(steamID)+" [no error given].")

	# Else if target user was not running the same game
	elif session_error == 1:
		print("WARNING: Session failure with "+str(steamID)+" [target user not running the same game].")

	# Else if local user doesn't own app / game
	elif session_error == 2:
		print("WARNING: Session failure with "+str(steamID)+" [local user doesn't own app / game].")

	# Else if target user isn't connected to Steam
	elif session_error == 3:
		print("WARNING: Session failure with "+str(steamID)+" [target user isn't connected to Steam].")

	# Else if connection timed out
	elif session_error == 4:
		print("WARNING: Session failure with "+str(steamID)+" [connection timed out].")

	# Else if unused
	elif session_error == 5:
		print("WARNING: Session failure with "+str(steamID)+" [unused].")

	# Else no known error
	else:
		print("WARNING: Session failure with "+str(steamID)+" [unknown error "+str(session_error)+"].")
````

This will print a warning message so you know why the session connection failed. From here you can add any additional functionality you want like retrying the connection or something else.



---

## Additional Resources

Before we get going, I'd like to point out some additional resources for you to check out:

First is this [video tutorial put together by **DawnsCrow Games**](https://youtu.be/si50G3S1XGU){ target="_blank" }.

Second is [**JDare**'s has a repo, **GodotSteamHL**](https://github.com/JDare/GodotSteamHL){ target="_blank" }, designed to streamline Steam's networking functionality (lobbies and P2P) which should serve as a handy guide and/or useful script.

I highly suggest you [check out the Example branch of the repo](https://github.com/CoaguCo-Industries/GodotSteam/tree/example){ target="_blank" }, specifically the **/src/examples/lobby.tscn** which will have the full working code and test interface for this tutorial.

I extremely and highly suggest you read through [Valve's networking documentations](https://partner.steamgames.com/doc/features/multiplayer/networking){ target="_blank" }; it also contains some links to other articles about networking which should prove helpful.

Here are some additional networking article resources to check out:

- [https://github.com/MFatihMAR/Game-Networking-Resources](https://github.com/MFatihMAR/Game-Networking-Resources){ target="_blank" }
- [https://gamedev.stackexchange.com/questions/249/how-to-write-a-network-game](https://gamedev.stackexchange.com/questions/249/how-to-write-a-network-game){ target="_blank" }
- [https://web.archive.org/web/20180823014743/https://gafferongames.com/tags/networking](https://web.archive.org/web/20180823014743/https://gafferongames.com/tags/networking){ target="_blank" }
- [https://www.gabrielgambetta.com/client-server-game-architecture.html](https://www.gabrielgambetta.com/client-server-game-architecture.html){ target="_blank" }

---

That concludes the P2P tutorial.  At this point you may want to [check out the lobbies tutorial (if you haven't yet) which compliments this one](/tutorials/lobbies).  Obviously this code should not be used for production and more for a very, very, very, simple guide on where to start.

[To see this tutorial in action, check out the example branch on the GodotSteam Github repo.](https://github.com/CoaguCo-Industries/GodotSteam/tree/example){ { target="_blank" } } There you can get a full view of the code used which can serve as a starting point for you to branch out from.
