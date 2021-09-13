extends Control
# Preload the button them
onready var BUTTON_THEME = preload("res://data/themes/button-theme.tres")
onready var LOBBY_MEMBER = preload("res://src/examples/lobby-member.tscn")

# Set up some variables
var STEAM_LOBBY_ID: int = 0
var LOBBY_MEMBERS: Array = []
var DATA
var LOBBY_INVITE_ARG: bool = false
var LOBBY_VOTE_KICK: bool = false

# Set up some signals
signal back_to_main


func _ready() -> void:
	# Set up some signals
	Steam.connect("lobby_created", self, "_on_Lobby_Created")
	Steam.connect("lobby_match_list", self, "_on_Lobby_Match_List")
	Steam.connect("lobby_joined", self, "_on_Lobby_Joined")
	Steam.connect("lobby_chat_update", self, "_on_Lobby_Chat_Update")
	Steam.connect("lobby_message", self, "_on_Lobby_Message")
	Steam.connect("lobby_data_update", self, "_on_Lobby_Data_Update")
	Steam.connect("lobby_invite", self, "_on_Lobby_Invite")
	Steam.connect("join_requested", self, "_on_Lobby_Join_Requested")
	Steam.connect("p2p_session_request", self, "_on_P2P_Session_Request")
	Steam.connect("p2p_session_connect_fail", self, "_on_P2P_Session_Connect_Fail")
	# Check for command line arguments
	_check_Command_Line()


func _process(_delta) -> void:
	# Get packets
	_read_P2P_Packet()


#################################################
# LOBBY FUNCTIONS
#################################################
# When the user starts a game with multiplayer enabled
func _create_Lobby() -> void:
	# Make sure a lobby is not already set
	if STEAM_LOBBY_ID == 0:
		# Set the lobby to public with two members max
		Steam.createLobby(2, 10)


# A lobby has been successfully created
func _on_Lobby_Created(connect: int, lobbyID: int) -> void:
	if connect == 1:
		# Set the lobby ID
		STEAM_LOBBY_ID = lobbyID
		$Output.append_bbcode("[STEAM] Created a lobby: "+str(STEAM_LOBBY_ID)+"\n")

		# Set lobby joinable as a test
		Steam.setLobbyJoinable(STEAM_LOBBY_ID, true)

		# Print the lobby ID to a label
		$Lobby.set_text("Lobby ID: "+str(STEAM_LOBBY_ID))

		# Set some lobby data
		Steam.setLobbyData(lobbyID, "name", str(global.STEAM_USERNAME)+"'s Lobby")
		Steam.setLobbyData(lobbyID, "mode", "GodotSteam test")

		# Allow P2P connections to fallback to being relayed through Steam if needed
		var IS_RELAY: bool = Steam.allowP2PPacketRelay(true)
		$Output.append_bbcode("[STEAM] Allowing Steam to be relay backup: "+str(IS_RELAY)+"\n\n")
		
		# Enable the leave lobby button and all testing buttons
		_change_Button_Controls(false)


# Getting a lobby match list
func _on_Lobby_Match_List(lobbies: Array) -> void:
	# Show the list 
	for LOBBY in lobbies:
		# Pull lobby data from Steam
		var LOBBY_NAME: String = Steam.getLobbyData(LOBBY, "name")
		var LOBBY_MODE: String = Steam.getLobbyData(LOBBY, "mode")
		var LOBBY_MEMBERS: int = Steam.getNumLobbyMembers(LOBBY)

		# Create a button for the lobby
		var LOBBY_BUTTON: Button = Button.new()
		LOBBY_BUTTON.set_text("Lobby "+str(LOBBY)+": "+str(LOBBY_NAME)+" ["+str(LOBBY_MODE)+"] - "+str(LOBBY_MEMBERS)+" Player(s)")
		LOBBY_BUTTON.set_size(Vector2(800, 50))
		LOBBY_BUTTON.set_name("lobby_"+str(LOBBY))
		LOBBY_BUTTON.set_text_align(0)
		LOBBY_BUTTON.set_theme(BUTTON_THEME)
		LOBBY_BUTTON.connect("pressed", self, "_join_Lobby", [LOBBY])

		# Add the new lobby to the list
		$Lobbies/Scroll/List.add_child(LOBBY_BUTTON)
	
	# Enable the refresh button
	$Lobbies/Refresh.set_disabled(false)


# When the player is joining a lobby
func _join_Lobby(lobbyID: int) -> void:
	$Output.append_bbcode("[STEAM] Attempting to join lobby "+str(lobbyID)+"...\n")

	# Close lobby panel if open
	_on_Close_Lobbies_pressed()

	# Clear any previous lobby lists
	LOBBY_MEMBERS.clear()

	# Make the lobby join request to Steam
	Steam.joinLobby(lobbyID)


# When a lobby is joined
func _on_Lobby_Joined(lobbyID: int, permissions: int, locked: bool, response: int) -> void:
	# Set this lobby ID as your lobby ID
	STEAM_LOBBY_ID = lobbyID

	# Print the lobby ID to a label
	$Lobby.set_text("Lobby ID: "+str(STEAM_LOBBY_ID))

	# Append to output
	$Output.append_bbcode("[STEAM] Joined lobby "+str(STEAM_LOBBY_ID)+".\n\n")

	# Get the lobby members
	_get_Lobby_Members()

	# Enable all necessary buttons
	_change_Button_Controls(false)

	# Make the initial handshake
	_make_P2P_Handshake()


# When accepting an invite
func _on_Lobby_Join_Requested(lobbyID: int, friendID: int) -> void:
	# Get the lobby owner's name
	var OWNER_NAME = Steam.getFriendPersonaName(friendID)
	$Output.append_bbcode("[STEAM] Joining "+str(OWNER_NAME)+"'s lobby...\n\n")
	
	# Attempt to join the lobby
	_join_Lobby(lobbyID)


# When a lobby message is received
# Using / delimiter for host commands like kick
func _on_Lobby_Message(result: int, user: int, message: String, type: int) -> void:
	# We are only concerned with who is sending the message and what the message is
	var SENDER = Steam.getFriendPersonaName(user)
	
	# If the lobby owner and the sender are the same, check for commands
	if user == Steam.getLobbyOwner(STEAM_LOBBY_ID) and message.begins_with("/"):
		print("Message sender is the lobby owner.")
		# Get any commands
		if message.begins_with("/kick"):
			# Get the user ID for kicking
			var COMMANDS: PoolStringArray = message.split(":", true)

			# If this is your ID, leave the lobby
			if global.STEAM_ID == int(COMMANDS[1]):
				_leave_Lobby()
	# Else this is just chat message
	else:
		# Print the outpubt before showing the message
		print(str(SENDER)+" says: "+str(message))
		$Output.append_bbcode(str(SENDER)+" says '"+str(message)+"'\n")


# When a lobby chat is updated
func _on_Lobby_Chat_Update(lobbyID: int, changedID: int, makingChangeID: int, chatState: int) -> void:
	# Note that chat state changes is: 1 - entered, 2 - left, 4 - user disconnected before leaving, 8 - user was kicked, 16 - user was banned
	$Output.append_bbcode("[STEAM] Lobby ID: "+str(lobbyID)+", Changed ID: "+str(changedID)+", Making Change: "+str(makingChangeID)+", Chat State: "+str(chatState)+"\n")

	# Get the user who has made the lobby change
	var CHANGER = Steam.getFriendPersonaName(makingChangeID)

	# If a player has joined the lobby
	if chatState == 1:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has joined the lobby.\n\n")
	# Else if a player has left the lobby
	elif chatState == 2:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has left the lobby.\n\n")
	# Else if a player has been kicked
	elif chatState == 8:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has been kicked from the lobby.\n\n")
	# Else if a player has been banned
	elif chatState == 16:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has been banned from the lobby.\n\n")
	# Else there was some unknown change
	else:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" did... something.\n\n")

	# Update the lobby now that a change has occurred
	_get_Lobby_Members()


# Whan lobby metadata has changed
func _on_Lobby_Data_Update(success: bool, lobbyID: int, memberID: int, key: int) -> void:
	$Output.append_bbcode("[STEAM] Success: "+str(success)+", Lobby ID: "+str(lobbyID)+", Member ID: "+str(memberID)+", Key: "+str(key)+"\n\n")


# When the player leaves a lobby for whatever reason
func _leave_Lobby() -> void:
	# If in a lobby, leave it
	if STEAM_LOBBY_ID != 0:
		# Append a new message
		$Output.append_bbcode("[STEAM] Leaving lobby "+str(STEAM_LOBBY_ID)+".\n\n")

		# Send leave request to Steam
		Steam.leaveLobby(STEAM_LOBBY_ID)

		# Wipe the Steam lobby ID then display the default lobby ID and player list title
		STEAM_LOBBY_ID = 0
		$Lobby.set_text("Lobby ID: "+str(STEAM_LOBBY_ID))
		$"Players Title".set_text("Player List (0)")

		# Close session with all users
		for MEMBERS in LOBBY_MEMBERS:
			Steam.closeP2PSessionWithUser(MEMBERS['steam_id'])

		# Clear the local lobby list
		LOBBY_MEMBERS.clear()
		for MEMBER in $Players.get_children():
			MEMBER.hide()
			MEMBER.queue_free()


		# Disable the leave lobby button and all test buttons
		_change_Button_Controls(true)


#################################################
# P2P NETWORKING FUNCTIONS
#################################################
# Read a Steam P2P packet
func _read_P2P_Packet() -> void:
	var PACKET_SIZE: int = Steam.getAvailableP2PPacketSize(0)
	# There is a packet
	if PACKET_SIZE > 0:
		$Output.append_bbcode("[STEAM] There is a packet available.\n")

		# Get the packet
		var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)
		# If it is empty, set a warning
		if PACKET.empty():
			$Output.append_bbcode("[WARNING] Read an empty packet with non-zero size!\n")

		# Get the remote user's ID
		var PACKET_ID: String = str(PACKET.steamIDRemote)
		var PACKET_CODE: String = str(PACKET.data[0])

		# Make the packet data readable
		var READABLE: PoolByteArray = bytes2var(PACKET.data.subarray(1, PACKET_SIZE - 1))

		# Print the packet to output
		$Output.append_bbcode("[STEAM] Packet: "+str(READABLE)+"\n\n")
		# Append logic here to deal with packet data


# Send a Steam P2P packet
func _send_P2P_Packet(target: String, packet_data: Dictionary) -> void:
	# Set the send_type and channel
	var SEND_TYPE: int = 2
	var CHANNEL: int = 0
	# Create a data array to send the data through
	var DATA: PoolByteArray
	DATA.append(256)
	DATA.append_array(var2bytes(packet_data))
	# If sending a packet to everyone
	if target == "all":
		# If there is more than one user, send packets
		if LOBBY_MEMBERS.size() > 1:
			# Loop through all members that aren't you
			for MEMBER in LOBBY_MEMBERS:
				if MEMBER['steam_id'] != global.STEAM_ID:
					Steam.sendP2PPacket(MEMBER['steam_id'], DATA, SEND_TYPE, CHANNEL)
	# Else send the packet to a particular user
	else:
		# Send this packet
		Steam.sendP2PPacket(int(target), DATA, SEND_TYPE, CHANNEL)


# Make a Steam P2P handshake
func _make_P2P_Handshake() -> void:
	$Output.append_bbcode("[STEAM] Sending P2P handshake to the lobby...\n\n")
	_send_P2P_Packet("all", {"message":"handshake", "from":global.STEAM_ID})


# Send test packet information
func _send_Test_Info() -> void:
	$Output.append_bbcode("[STEAM] Sending test packet data...\n\n")
	var DATA: Dictionary = {"title":"This is a test packet", "player_id":global.STEAM_ID, "player_hp":"5", "player_coord":"56,40"}
	_send_P2P_Packet("all", DATA)


# When receiving a P2P request from another user
func _on_P2P_Session_Request(remoteID: int) -> void:
	# Get the requester's name
	var REQUESTER: String = Steam.getFriendPersonaName(remoteID)

	# Print the debug message to output
	$Output.append_bbcode("[STEAM] P2P session request from "+str(REQUESTER)+"\n\n")

	# Accept the P2P session; can apply logic to deny this request if needed
	Steam.acceptP2PSessionWithUser(remoteID)

	# Make the initial handshake
	_make_P2P_Handshake()


# P2P connection failure
func _on_P2P_Session_Connect_Fail(lobbyID: int, session_error: int) -> void:
	# Note the session errors are: 0 - none, 1 - target user not running the same game, 2 - local user doesn't own app, 3 - target user isn't connected to Steam, 4 - connection timed out, 5 - unused
	# If no error was given
	if session_error == 0:
		$Output.append_bbcode("[WARNING] Session failure with "+str(lobbyID)+" [no error given].\n\n")
	# Else if target user was not running the same game
	elif session_error == 1:
		$Output.append_bbcode("[WARNING] Session failure with "+str(lobbyID)+" [target user not running the same game].\n\n")
	# Else if local user doesn't own app / game
	elif session_error == 2:
		$Output.append_bbcode("[WARNING] Session failure with "+str(lobbyID)+" [local user doesn't own app / game].\n\n")
	# Else if target user isn't connected to Steam
	elif session_error == 3:
		$Output.append_bbcode("[WARNING] Session failure with "+str(lobbyID)+" [target user isn't connected to Steam].\n\n")
	# Else if connection timed out
	elif session_error == 4:
		$Output.append_bbcode("[WARNING] Session failure with "+str(lobbyID)+" [connection timed out].\n\n")
	# Else if unused
	elif session_error == 5:
		$Output.append_bbcode("[WARNING] Session failure with "+str(lobbyID)+" [unused].\n\n")
	# Else no known error
	else:
		$Output.append_bbcode("[WARNING] Session failure with "+str(lobbyID)+" [unknown error "+str(session_error)+"].\n\n")


#################################################
# HELPER FUNCTIONS
#################################################
# Get the lobby members from Steam
func _get_Lobby_Members():
	# Clear your previous lobby list
	LOBBY_MEMBERS.clear()

	# Get the number of members from this lobby from Steam
	var MEMBERS: int = Steam.getNumLobbyMembers(STEAM_LOBBY_ID)

	# Update the player list title
	$"Players Title".set_text("Player List ("+str(MEMBERS)+")")

	# Get the data of these players from Steam
	for MEMBER in range(0, MEMBERS):
		# Get the member's Steam ID
		var MEMBER_STEAM_ID: int = Steam.getLobbyMemberByIndex(STEAM_LOBBY_ID, MEMBER)

		# Get the member's Steam name
		var MEMBER_STEAM_NAME: String = Steam.getFriendPersonaName(MEMBER_STEAM_ID)

		# Add them to the player list
		_add_Player_List(MEMBER_STEAM_ID, MEMBER_STEAM_NAME)


# Add a new Steam user to the connect users list
func _add_Player_List(steam_id: int, steam_name: String) -> void:
	print("Adding new player to the list: "+str(steam_id)+" / "+str(steam_name))

	# Add them to the list
	LOBBY_MEMBERS.append({"steam_id":steam_id, "steam_name":steam_name})

	# Clear the original player list
	for MEMBER in $Players.get_children():
		MEMBER.hide()
		MEMBER.queue_free()

	# Update the player list
	for MEMBER in LOBBY_MEMBERS:
		# Instance the lobby member object
		var THIS_MEMBER: Object = LOBBY_MEMBER.instance()

		# Add their Steam name and ID
		THIS_MEMBER.name = str(steam_id)
		THIS_MEMBER._set_Member(steam_id, steam_name)

		# Connect the kick signal
		THIS_MEMBER.connect("kick_player", self, "_on_Lobby_Kick")
		
		# Add the child node
		$Players.add_child(THIS_MEMBER)
		
		# If you are the host, enable the kick button
		if global.STEAM_ID == Steam.getLobbyOwner(STEAM_LOBBY_ID):
			get_node("Players/"+str(THIS_MEMBER.name)+"/Member/Stuff/Controls/Kick").set_disabled(false)


# Enable or disable a gang of buttons
func _change_Button_Controls(toggle: bool) -> void:
	$Panel/Options/Leave.set_disabled(toggle)
	$Panel/Options/GetLobbyData.set_disabled(toggle)
	$Panel/Options/SendPacket.set_disabled(toggle)
	$Send.set_disabled(toggle)
	# Caveat for the lineedit
	if toggle:
		$Chat.set_editable(false)
	else:
		$Chat.set_editable(true)


#################################################
# BUTTON FUNCTIONS
#################################################
# Creating a lobby
func _on_Create_Lobby_pressed() -> void:
	_create_Lobby()


# Leaving the lobby
func _on_Leave_Lobby_pressed() -> void:
	_leave_Lobby()


# Getting associated metadata for the lobby
func _on_Get_Lobby_Data_pressed() -> void:
	DATA = Steam.getLobbyData(STEAM_LOBBY_ID, "name")
	$Output.append_bbcode("[STEAM] Lobby data, name: "+str(DATA)+"\n")
	DATA = Steam.getLobbyData(STEAM_LOBBY_ID, "mode")
	$Output.append_bbcode("[STEAM] Lobby data, mode: "+str(DATA)+"\n\n")


# Sending a test packet out to the players
func _on_Send_Packet_pressed() -> void:
	_send_Test_Info()


# Open the lobby list
func _on_Open_Lobby_List_pressed() -> void:
	$Lobbies.show()

	# Set distance to worldwide
	Steam.addRequestLobbyListDistanceFilter(3)

	# Request the list
	$Output.append_bbcode("[STEAM] Requesting a lobby list...\n\n")
	Steam.requestLobbyList()


# Close the lobbies screen
func _on_Close_Lobbies_pressed() -> void:
	$Lobbies.hide()


# Refresh the lobby list
func _on_Refresh_pressed() -> void:
	# Clear all previous server entries
	for SERVER in $Lobbies/Scroll/List.get_children():
		SERVER.free()
	
	# Disable the refresh button
	$Lobbies/Refresh.set_disabled(true)
	
	# Set distance to world (or maybe change this option)
	Steam.addRequestLobbyListDistanceFilter(3)
	
	# Request a new server list
	Steam.requestLobbyList()



#################################################
# LOBBY CHAT FUNCTIONS
#################################################
# Send a chat message
func _on_Send_Chat_pressed() -> void:
	# Get the entered chat message
	var MESSAGE: String = $Chat.get_text()

	# Pass the message to Steam
	var IS_SENT: bool = Steam.sendLobbyChatMsg(STEAM_LOBBY_ID, MESSAGE)

	# Was it sent successfully?
	if not IS_SENT:
		$Output.append_bbcode("[ERROR] Chat message failed to send.\n\n")

	# Clear the chat input
	$Chat.clear()


func _on_Lobby_Kick(kick_id: int) -> void:
	# Pass the kick message to Steam
	var IS_SENT: bool = Steam.sendLobbyChatMsg(STEAM_LOBBY_ID, "/kick:"+str(kick_id))
	
	# Was it send successfully?
	if not IS_SENT:
		$Output.append_bbcode("[ERROR] Kick command failed to send.\n\n")


#################################################
# COMMAND LINE ARGUMENTS
#################################################
# Check the command line for arguments
# Used primarily if a player accepts an invite and does not have the game opened
func _check_Command_Line():
	var ARGUMENTS = OS.get_cmdline_args()
	# There are arguments to process
	if ARGUMENTS.size() > 0:
		# Loop through them and get the userful ones
		for ARGUMENT in ARGUMENTS:
			print("Command line: "+str(ARGUMENT))
			# An invite argument was passed
			if LOBBY_INVITE_ARG:
				_join_Lobby(int(ARGUMENT))
			# A Steam connection argument exists
			if ARGUMENT == "+connect_lobby":
				LOBBY_INVITE_ARG = true


#################################################
# BACK BUTTON
#################################################
# Emit a signal to the main node letting it know the user is done
func _on_Back_pressed() -> void:
	emit_signal("back_to_main")
