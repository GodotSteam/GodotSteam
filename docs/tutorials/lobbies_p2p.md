# Tutorials - Lobbies And P2P Networking


One of the more requested tutorials is multiplayer lobbies and P2P networking through Steam. I highly suggest you [check out the Example branch of the repo](https://github.com/Gramps/GodotSteam/tree/example){ target="_blank" }, specifically the **/src/examples/lobby.tscn** which will have the full working code and test interface for this tutorial.

Additionally, you can check out this video tutorial put together by DawnsCrow Games. Also JDare has a repo, GodotSteamHL, designed to streamline Steam's networking functionality (lobbies and P2P) which should serve as a handy guide and/or useful script.

- [Video Tutorial by DawnsCrow Games](https://youtu.be/si50G3S1XGU"){ target="_blank" }
- [GodotSteamHL by JDare](https://github.com/JDare/GodotSteamHL"){ target="_blank" }

I highly suggest you read through [Valve's networking documentations](https://partner.steamgames.com/doc/features/multiplayer/networking){ target="_blank" }; it also contains some links to other articles about networking which should prove helpful.  Here are some additional networking article resources to check out:

- [https://github.com/MFatihMAR/Game-Networking-Resources](https://github.com/MFatihMAR/Game-Networking-Resources){ target="_blank" }
- [https://gamedev.stackexchange.com/questions/249/how-to-write-a-network-game](https://gamedev.stackexchange.com/questions/249/how-to-write-a-network-game){ target="_blank" }
- [https://web.archive.org/web/20180823014743/https://gafferongames.com/tags/networking](https://web.archive.org/web/20180823014743/https://gafferongames.com/tags/networking){ target="_blank" }
- [https://www.gabrielgambetta.com/client-server-game-architecture.html](https://www.gabrielgambetta.com/client-server-game-architecture.html){ target="_blank" }

One last note, this is for a basic, turn-based lobby / P2P set-up. Only use this as a starting point.

---

## Set Up

First let's set up some variables to fill in later:

````
const PACKET_READ_LIMIT: int = 32
var STEAM_ID: int = 0
var STEAM_USERNAME: String = ""
var LOBBY_ID: int = 0
var LOBBY_MEMBERS: Array = []
var DATA
var LOBBY_VOTE_KICK: bool = false
var LOBBY_MAX_MEMBERS: int = 10
enum LOBBY_AVAILABILITY {PRIVATE, FRIENDS, PUBLIC, INVISIBLE}
````
Your Steam ID and username may actually be in a different GDScript, especially if you use the **global.gd** the way I do [mentioned in the initialization tutorial](/tutorials/initializing/). The most important will be the **LOBBY_ID**, which obviously houses the lobby's ID, and **LOBBY_MEMBERS**, which will be an array of dictionaries of lobby members and their Steam ID 64's. **LOBBY_INVITE_ARG** is mostly used when the game is started by accepting a Steam invite or joining a game from your friends list if the game isn't running.

---

## The ***_ready()*** Function

Next we'll want to set up the signal connections for Steamworks and a command line checker like so:

````
func _ready() -> void:
	Steam.connect("lobby_created", self, "_on_Lobby_Created")
	Steam.connect("lobby_match_list", self, "_on_Lobby_Match_List")
	Steam.connect("lobby_joined", self, "_on_Lobby_Joined")
	Steam.connect("lobby_chat_update", self, "_on_Lobby_Chat_Update")
	Steam.connect("lobby_message", self, "_on_Lobby_Message")
	Steam.connect("lobby_data_update", self, "_on_Lobby_Data_Update")
	Steam.connect("lobby_invite", self, "_on_Lobby_Invite")
	Steam.connect("join_requested", self, "_on_Lobby_Join_Requested")
	Steam.connect("persona_state_change", self, "_on_Persona_Change")
	Steam.connect("p2p_session_request", self, "_on_P2P_Session_Request")
	Steam.connect("p2p_session_connect_fail", self, "_on_P2P_Session_Connect_Fail")

	# Check for command line arguments
	_check_Command_Line()
````

We will get into each of these below. You noticed we added a check for command line arguments. Here is how our basic function will look:

````
func _check_Command_Line() -> void:
	var ARGUMENTS: Array = OS.get_cmdline_args()

	# There are arguments to process
	if ARGUMENTS.size() > 0:

		# A Steam connection argument exists
		if ARGUMENT[0] == "+connect_lobby":
		
			# Lobby invite exists so try to connect to it
			if int(ARGUMENTS[1]) > 0:

				# At this point, you'll probably want to change scenes
				# Something like a loading into lobby screen
				print("CMD Line Lobby ID: "+str(ARGUMENTS[1]))
				_join_Lobby(int(ARGUMENTS[1]))
````

This is important if the player is accepting a Steam invite or right-clicks a friend's name then selects 'Join Game' or 'Join Lobby' and doesn't have the game open. Doing either action will launch the game with the additional command ````+connect_lobby <Steam Lobby ID>````. Sadly Godot doesn't really understand this command argument so our ***_check_Command_Line()*** function has to be written to work within those constraints.

Additionally, you'll need to add the appropriate scene name to your Steamworks launch options on the Steamworks website. You'll want to add the full scene path (res://your-scene.tscn) on the **Arguments** line in your launch option. [You can read more about that, with details, in this link.](https://github.com/Gramps/GodotSteam/issues/100){ target="_blank" } Big thanks to **Antokolos** for answering this issue and providing a solid example.

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

## Creating Lobbies

Next we'll set up our lobby creation functions.  You'll probably want to connect this function to a button somewhere in your game:

````
func _create_Lobby() -> void:
	# Make sure a lobby is not already set
	if LOBBY_ID == 0:
		Steam.createLobby(LOBBY_AVAILABILITY.PUBLIC, LOBBY_MAX_MEMBERS)
````

In this example we have ***createLobby()*** using our variables and enum.  The first variables covers the type of lobby; we are using a public lobby open to all.  There are, of course, four settings in total you can use:

- **0 - Private** - The only way to join the lobby is from an invite.
- **1 - Friends only** - Joinable by friends and invitees, but does not show up in the lobby list.
- **2 - Public** - Returned by search and visible to friends.
- **3 - Invisible** - Returned by search, but not visible to other friends.

The second variable is the maximum number of players allowed to join the lobby. This cannot be set higher than 250.
Next we'll cover the callback from Steam saying the lobby has been created:

````
func _on_Lobby_Created(connect: int, lobby_id: int) -> void:
	if connect == 1:
		# Set the lobby ID
		LOBBY_ID = lobby_id
		print("Created a lobby: "+str(LOBBY_ID))

		# Set this lobby as joinable, just in case, though this should be done by default
		Steam.setLobbyJoinable(LOBBY_ID, true)

		# Set some lobby data
		Steam.setLobbyData(lobby_id, "name", "Gramps' Lobby")
		Steam.setLobbyData(lobby_id, "mode", "GodotSteam test")

		# Allow P2P connections to fallback to being relayed through Steam if needed
		var RELAY: bool = Steam.allowP2PPacketRelay(true)
		print("Allowing Steam to be relay backup: "+str(RELAY))
````

Once this callback fires, you'll have your lobby ID which you can pass off to our **LOBBY_ID** variable for later use.  As the note says, the lobby should be set to joinable by default but, just in case, we add it here.  You can make the lobby unjoinable too. You can also se some lobby data now; which can be whatever **key / value** pair you want.  I'm not aware of a maximum amount of pairs you can set. And you'll notice I set ***allowP2PPacketRelay()*** to true at this point; this allows, as the note mentions, P2P connections to fallback to being relayed through Steam if needed.  This usually happens if you have NAT or firewall issues.

---

## Get Lobby Lists

Now that we can create lobbies, let's query and pull a list of lobbies.  I usually have a button that will open a lobby interface which is a list of buttons, one per lobby:

````
func _on_Open_Lobby_List_pressed() -> void:
	# Set distance to worldwide
	Steam.addRequestLobbyListDistanceFilter(3)

	print("Requesting a lobby list")
	Steam.requestLobbyList()
````

Before requesting the lobby list with ***requestLobbyList()*** you can add more search queries like:

- **addRequestLobbyListStringFilter** - which allows you to look for specific works in the lobby metadata
- **addRequestLobbyListNumericalFilter** - which adds a numerical comparions filter (<=, <, =, >, >=, !=)
- **addRequestLobbyListNearValueFilter** - which gives results closes to the specified value you give
- **addRequestLobbyListFilterSlotsAvailable** - which only returns lobbies with a specified amount of open slots available
- **addRequestLobbyListResultCountFilter** - which sets how many results you want returned
- **addRequestLobbyListDistanceFilter** - which sets the distance to search for lobbies, like:
	- **0** - Close
	- **1** - Default
	- **2** - Far
	- **3** - Worldwide

Once you set all or none of these, you can then call ***requestLobbyList()***. Once it pulls your lobby list it will fire a callback ***_on_Lobby_Match_List()***. You can then loop through the lobbies however you want. In our example code, I do something like this to make buttons for each lobby:

````
func _on_Lobby_Match_List(lobbies: Array) -> void:
	for LOBBY in lobbies:
		# Pull lobby data from Steam, these are specific to our example
		var LOBBY_NAME: String = Steam.getLobbyData(LOBBY, "name")
		var LOBBY_MODE: String = Steam.getLobbyData(LOBBY, "mode")

		# Get the current number of members
		var LOBBY_NUM_MEMBERS: int = Steam.getNumLobbyMembers(LOBBY)

		# Create a button for the lobby
		var LOBBY_BUTTON: Button = Button.new()
		LOBBY_BUTTON.set_text("Lobby ", LOBBY, ": ", LOBBY_NAME, " [", LOBBY_MODE, "] - ", LOBBY_NUM_MEMBERS, " Player(s)")
		LOBBY_BUTTON.set_size(Vector2(800, 50))
		LOBBY_BUTTON.set_name("lobby_"+str(LOBBY))
		LOBBY_BUTTON.connect("pressed", self, "_join_Lobby", [LOBBY])

		# Add the new lobby to the list
		$"Lobby Panel/Panel/Scroll/VBox".add_child(LOBBY_BUTTON)
````

You should now have a way to call lobby lists and display them.

---

## Joining Lobbies
	
Next we'll tackle joining a lobby. Clicking one of the lobby buttons we created in the last step will fire this function:

````
func _join_Lobby(lobby_id: int) -> void:
	print("Attempting to join lobby "+str(lobby_id)+"...")

	# Clear any previous lobby members lists, if you were in a previous lobby
	LOBBY_MEMBERS.clear()

	# Make the lobby join request to Steam
	Steam.joinLobby(lobby_id)
````

This will attempt to join the lobby you click on and, when it succeeds, it will fire the ***_on_Lobby_Joined()*** callback:

````
func _on_Lobby_Joined(lobby_id: int, _permissions: int, _locked: bool, response: int) -> void:
	# If joining was successful
	if response == 1:
		# Set this lobby ID as your lobby ID
		LOBBY_ID = lobby_id

		# Get the lobby members
		_get_Lobby_Members()

		# Make the initial handshake
		_make_P2P_Handshake()

	# Else it failed for some reason
	else:
		# Get the failure reason
		var FAIL_REASON: String
	
		match response:
			2:	FAIL_REASON = "This lobby no longer exists."
			3:	FAIL_REASON = "You don't have permission to join this lobby."
			4:	FAIL_REASON = "The lobby is now full."
			5:	FAIL_REASON = "Uh... something unexpected happened!"
			6:	FAIL_REASON = "You are banned from this lobby."
			7:	FAIL_REASON = "You cannot join due to having a limited account."
			8:	FAIL_REASON = "This lobby is locked or disabled."
			9:	FAIL_REASON = "This lobby is community locked."
			10:	FAIL_REASON = "A user in the lobby has blocked you from joining."
			11:	FAIL_REASON = "A user you have blocked is in the lobby."

		#Reopen the lobby list
		_on_Open_Lobby_List_pressed()
````

For a more clear explanation of these join responses:

- **1 (k_EChatRoomEnterResponseSuccess)** - Success.
- **2 (k_EChatRoomEnterResponseDoesntExist)** - Chat doesn't exist (probably closed).
- **3 (k_EChatRoomEnterResponseNotAllowed)** - General Denied - You don't have the permissions needed to join the chat.
- **4 (k_EChatRoomEnterResponseFull)** - Chat room has reached its maximum size.
- **5 (k_EChatRoomEnterResponseError)** - Unexpected Error.
- **6 (k_EChatRoomEnterResponseBanned)** - You are banned from this chat room and may not join.
- **7 (k_EChatRoomEnterResponseLimited)** - Joining this chat is not allowed because you are a limited user (no value on account).
- **8 (k_EChatRoomEnterResponseClanDisabled)** - Attempt to join a clan chat when the clan is locked or disabled.
- **9 (k_EChatRoomEnterResponseCommunityBan)** - Attempt to join a chat when the user has a community lock on their account.
- **10 (k_EChatRoomEnterResponseMemberBlockedYou)** - Join failed - a user that is in the chat has blocked you from joining.
- **11 (k_EChatRoomEnterResponseYouBlockedMember)** - Join failed - you have blocked a user that is already in the chat.
		
If the player is already in-game and accepts a Steam invite or clicks on a friend in their friend list then selects 'Join Game' from there, it will trigger the **join_requested** callback. This function will handle that:

````
func _on_Lobby_Join_Requested(lobby_id: int, friendID: int) -> void:
	# Get the lobby owner's name
	var OWNER_NAME: String = Steam.getFriendPersonaName(friendID)

	print("Joining "+str(OWNER_NAME)+"'s lobby...")

	# Attempt to join the lobby
	_join_Lobby(lobby_id)
````

It will then follow the normal ***_join_Lobby()*** process of setting up all lobby members, handshakes, etc.  Not to sound repetitive, but note again that if the player is not in-game and accepts a Steam invite or joins a game through the friends list then we are back to the command line situation talked about earlier.

---

## Getting Lobby Members
	
Depending on how you set up your lobby interface, you'll probably want the player to see some kind of chat window with a player list. Our ***_get_Lobby_Members()*** will help with finding out who all is in this lobby:

````
func _get_Lobby_Members() -> void:
	# Clear your previous lobby list
	LOBBY_MEMBERS.clear()

	# Get the number of members from this lobby from Steam
	var MEMBERS: int = Steam.getNumLobbyMembers(LOBBY_ID)

	# Get the data of these players from Steam
	for MEMBER in range(0, MEMBERS):
		# Get the member's Steam ID
		var MEMBER_STEAM_ID: int = Steam.getLobbyMemberByIndex(LOBBY_ID, MEMBER)

		# Get the member's Steam name
		var MEMBER_STEAM_NAME: String = Steam.getFriendPersonaName(MEMBER_STEAM_ID)

		# Add them to the list
		LOBBY_MEMBERS.append({"steam_id":steam_id, "steam_name":steam_name})
````

This will get the lobby members from Steam then loop through and get their names and Steam ID's then append them to our **LOBBY_MEMBERS** array for later use.  You can then display this list in your lobby room.

---

## Persona Changes / Avatars / Names
	
Sometimes you will see that a user's name and avatar, sometimes one or the other, won't immediately show up correctly. This is because our local user only really knows about friends and players they have played with; whatever is stored in local cache. A bit after a lobby is joined, this data will be sent by Steam which triggers a **persona_state_change** callback. You will want to update your player list to reflect this and get the correct name and avatar for unknown players.  Our connect ***_on_Persona_Change()*** function will do that:

````
# A user's information has changed
func _on_Persona_Change(steam_id: int, _flag: int) -> void:
	print("[STEAM] A user ("+str(steam_id)+") had information change, update the lobby list")

	# Update the player list
	_get_Lobby_Members()
````

All this really does is refresh our lobby list information to get the avatar and name right by re-calling ***_get_Lobby_Members()*** again.

---

## P2P Handshakes

You'll also note in the joining lobbies part we fire the initial P2P handshake; this just opens and checks our P2P session:

````
func _make_P2P_Handshake() -> void:
	print("Sending P2P handshake to the lobby")
	
	_send_P2P_Packet(0, {"message":"handshake", "from":STEAM_ID})
````

We won't get into what all this means just yet, but I wanted to show the code for the handshake function here since it is referenced. Your handshake messages can be anything and disregarded for the most part. Again, it is just to test our P2P session.

---

## Lobby Updates / Changes

Now that a player has joined the lobby, everyone in the lobby will receive a callback notifying of the change.  We will handle it like this:

````
func _on_Lobby_Chat_Update(lobby_id: int, change_id: int, making_change_id: int, chat_state: int) -> void:
	# Get the user who has made the lobby change
	var CHANGER: String = Steam.getFriendPersonaName(change_id)

	# If a player has joined the lobby
	if chat_state == 1:
		print(str(CHANGER)+" has joined the lobby.")

	# Else if a player has left the lobby
	elif chat_state == 2:
		print(str(CHANGER)+" has left the lobby.")

	# Else if a player has been kicked
	elif chat_state == 8:
		print(str(CHANGER)+" has been kicked from the lobby.")

	# Else if a player has been banned
	elif chat_state == 16:
		print(str(CHANGER)+" has been banned from the lobby.")

	# Else there was some unknown change
	else:
		print(str(CHANGER)+" did... something.")

	# Update the lobby now that a change has occurred
	_get_Lobby_Members()
````

For the most part this will update when players join or leave the lobby. However, if you add functionality to kick or ban players, it will show that too.  At the end of this function, I always update the player list so we can show the correct list of players in the lobby.

---

## Lobby Chat / Messages

You may also want players to be able to chat while in the lobby and waiting for a game to start. If you have a **LineEdit** node for messaging, clicking a "send" button should trigger something like this:

````
func _on_Send_Chat_pressed() -> void:
	# Get the entered chat message
	var MESSAGE: String = $Chat.get_text()

	# If there is even a message
	if MESSAGE.length() > 0:
		# Pass the message to Steam
		var SENT: bool = Steam.sendLobbyChatMsg(LOBBY_ID, MESSAGE)

		# Was it sent successfully?
		if not SENT:
			print("ERROR: Chat message failed to send.")

	# Clear the chat input
	$Chat.clear()
````

The $Chat is your **LineEdit** and will probably be different in your project. Most importantly is your get the text and send it to ***sendLobbyChatMsg()***.

---

## Leaving A Lobby

Next we'll handle leaving a lobby. If you have a button do to so, have it connect to this function:

````
func _leave_Lobby() -> void:
	# If in a lobby, leave it
	if LOBBY_ID != 0:
		# Send leave request to Steam
		Steam.leaveLobby(LOBBY_ID)

		# Wipe the Steam lobby ID then display the default lobby ID and player list title
		LOBBY_ID = 0

		# Close session with all users
		for MEMBERS in LOBBY_MEMBERS:
			# Make sure this isn't your Steam ID
			if MEMBERS['steam_id'] != STEAM_ID:

			# Close the P2P session
			Steam.closeP2PSessionWithUser(MEMBERS['steam_id'])

		# Clear the local lobby list
		LOBBY_MEMBERS.clear()
````

This will inform Steam you have left the lobby then clear your **LOBBY_ID** variable, as well as your **LOBBY_MEMBERS** array after it closes your P2P sessions with all players in the lobby. You'll notice at this point we don't have any functions to handle invites through Steam. This will be added in the second half of the lobby tutorial at a later time.

---

## P2P Networking - Session Request
	
Next we'll check out the P2P networking functionality. Earlier when we did the P2P handshake when someone joins the lobby, it would trigger a **p2p_session_request** callback which would in turn trigger this function:

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

If the packet size is greater than zero then it will get the sender's Steam ID and the data they sent. The line **bytes2var** is very important as it decodes the data back into something you can read and use. After it is decoded you can pass that data to any number of functions for your game.

---

## Sending P2P Packets

Beyond the handshake, you will probably want to pass a lot of different pieces of data back and forth between players. I have mine set up with two arguments: the first is the recipient as a string and the second is a dictionary. I think the dictionary is best for sending data so you can have a key / value pair to reference and make things less confusing on the receiving end. Each packet will go through the following function:

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

The **send_type** variable will corresponed to these integers:

- **0** - Send unreliable
- **1** - Send unreliable with no delay
- **2** - Send reliable
- **3** - Send reliable with buffering

The channel used should match for both read and send functions. You may want to use multiple channels so this should obviously be adjusted.

As your game increases in complexity, you may find the amount of data you're sending increases significantly. One of the core tenets of responsive, effective networking is reducing the amount of data you're sending, to reduce the chance of some part becoming corrupted or requiring players of your game to have a very fast internet connection to even play your game. Luckily, we can introduce **compression** to our send function to shrink the size of the data without needing to change our whole dictionary. The concept is straightforward enough; when we call the **var2bytes** function, we turn our dictionary (or some other variable) into a **PoolByteArray** and send it over the internet. We can compress the PoolByteArray to be smaller with a single line of code:

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

Of course, we've now sent a **compressed** PoolByteArray to someone else over the internet, so when they receive the packet, they will need to first **decompress** the PoolByteArray before they can decode it.
		To accomplish this, we add a single line of code to our **\_read_P2P_Packet** function like so:
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

That concludes the first part of the lobby / P2P networking tutorial.  Obviously this code should not be used for production and more for a very, very, very, simple guide on where to start.

[To see this tutorial in action, check out the example branch on the GodotSteam Github repo.](https://github.com/Gramps/GodotSteam/tree/example){ { target="_blank" } } There you can get a full view of the code used which can serve as a starting point for you to branch out from.