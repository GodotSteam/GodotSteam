extends Control
# Preload the button them
onready var BUTTON_THEME = preload("res://data/themes/button-theme.tres")
onready var LOBBY_MEMBER = preload("res://src/examples/lobby-member.tscn")

# Set up some variables
var LOBBY_ID: int = 0
var LOBBY_MEMBERS: Array = []
var DATA
var LOBBY_VOTE_KICK: bool = false
var LOBBY_MAX_MEMBERS: int = 10
enum LOBBY_AVAILABILITY {PRIVATE, FRIENDS, PUBLIC, INVISIBLE}

# Set up some signals
signal back_to_main


func _ready() -> void:
	# Set up some signals
	_connect_Steam_Signals("lobby_created", "_on_Lobby_Created")
	_connect_Steam_Signals("lobby_match_list", "_on_Lobby_Match_List")
	_connect_Steam_Signals("lobby_joined", "_on_Lobby_Joined")
	_connect_Steam_Signals("lobby_chat_update", "_on_Lobby_Chat_Update")
	_connect_Steam_Signals("lobby_message", "_on_Lobby_Message")
	_connect_Steam_Signals("lobby_data_update", "_on_Lobby_Data_Update")
	_connect_Steam_Signals("lobby_invite", "_on_Lobby_Invite")
	_connect_Steam_Signals("join_requested", "_on_Lobby_Join_Requested")
	_connect_Steam_Signals("persona_state_change", "_on_Persona_Change")
	_connect_Steam_Signals("p2p_session_request", "_on_P2P_Session_Request")
	_connect_Steam_Signals("p2p_session_connect_fail", "_on_P2P_Session_Connect_Fail")
	# Check for command line arguments
	_check_Command_Line()


func _process(_delta: float) -> void:
	# Get packets if lobby is joined
	if LOBBY_ID > 0:
		_read_P2P_Packet()


#################################################
# LOBBY FUNCTIONS
#################################################
# When the user starts a game with multiplayer enabled
func _create_Lobby() -> void:
	# Make sure a lobby is not already set
	if LOBBY_ID == 0:
		# Set the lobby to public with ten members max
		Steam.createLobby(LOBBY_AVAILABILITY.PUBLIC, LOBBY_MAX_MEMBERS)


# A lobby has been successfully created
func _on_Lobby_Created(connect: int, lobby_id: int) -> void:
	if connect == 1:
		$Output.append_bbcode("[STEAM] Created a lobby: "+str(LOBBY_ID)+"\n")
		# Set lobby joinable as a test
		var SET_JOINABLE: bool = Steam.setLobbyJoinable(LOBBY_ID, true)
		print("[STEAM] The lobby has been set joinable: "+str(SET_JOINABLE))
		# Print the lobby ID to a label
		$Lobby.set_text("Lobby ID: "+str(LOBBY_ID))
		# Set some lobby data
		var SET_LOBBY_DATA: bool = false
		SET_LOBBY_DATA = Steam.setLobbyData(lobby_id, "name", str(global.STEAM_USERNAME)+"'s Lobby")
		print("[STEAM] Setting lobby name data successful: "+str(SET_LOBBY_DATA))
		SET_LOBBY_DATA = Steam.setLobbyData(lobby_id, "mode", "GodotSteam test")
		print("[STEAM] Setting lobby mode data successful: "+str(SET_LOBBY_DATA))
		# Allow P2P connections to fallback to being relayed through Steam if needed
		var IS_RELAY: bool = Steam.allowP2PPacketRelay(true)
		$Output.append_bbcode("[STEAM] Allowing Steam to be relay backup: "+str(IS_RELAY)+"\n")
		# Enable the leave lobby button and all testing buttons
		_change_Button_Controls(false)
	else:
		$Output.append_bbcode("[STEAM] Failed dto create lobby\n")


# When getting a lobby invitation
func _on_Lobby_Invite(inviter: int, lobby_id: int, game_id: int) -> void:
	$Output.append_bbcode("[STEAM] You have received an invite from "+str(Steam.getFriendPersonaName(inviter))+" to join lobby "+str(lobby_id)+" / game "+str(game_id)+"\n")


# When the player is joining a lobby
func _join_Lobby(lobby_id: int) -> void:
	$Output.append_bbcode("[STEAM] Attempting to join lobby "+str(lobby_id)+"...\n")
	# Close lobby panel if open
	_on_Close_Lobbies_pressed()
	# Clear any previous lobby lists
	LOBBY_MEMBERS.clear()
	# Make the lobby join request to Steam
	Steam.joinLobby(lobby_id)


# When a lobby is joined
func _on_Lobby_Joined(lobby_id: int, _permissions: int, _locked: bool, response: int) -> void:
	# If joining succeed, this will be 1
	if response == 1:
		# Set this lobby ID as your lobby ID
		LOBBY_ID = lobby_id
		# Print the lobby ID to a label
		$Lobby.set_text("Lobby ID: "+str(LOBBY_ID))
		# Append to output
		$Output.append_bbcode("[STEAM] Joined lobby "+str(LOBBY_ID)+".\n")
		# Get the lobby members
		_get_Lobby_Members()
		# Enable all necessary buttons
		_change_Button_Controls(false)
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
		$Output.append_bbcode("[STEAM] Failed joining lobby "+str(lobby_id)+": "+str(FAIL_REASON)+"\n")
		# Reopen the server list
		_on_Open_Lobby_List_pressed()


# When accepting an invite
func _on_Lobby_Join_Requested(lobby_id: int, friend_id: int) -> void:
	# Get the lobby owner's name
	var OWNER_NAME = Steam.getFriendPersonaName(friend_id)
	$Output.append_bbcode("[STEAM] Joining "+str(OWNER_NAME)+"'s lobby...\n")
	# Attempt to join the lobby
	_join_Lobby(lobby_id)


# Whan lobby metadata has changed
func _on_Lobby_Data_Update(success: bool, lobby_id: int, memberID: int, key: int) -> void:
	print("[STEAM] Success: "+str(success)+", Lobby ID: "+str(lobby_id)+", Member ID: "+str(memberID)+", Key: "+str(key)+"\n\n")


# When the player leaves a lobby for whatever reason
func _leave_Lobby() -> void:
	# If in a lobby, leave it
	if LOBBY_ID != 0:
		# Append a new message
		$Output.append_bbcode("[STEAM] Leaving lobby "+str(LOBBY_ID)+".\n")
		# Send leave request to Steam
		Steam.leaveLobby(LOBBY_ID)
		# Wipe the Steam lobby ID then display the default lobby ID and player list title
		LOBBY_ID = 0
		$Lobby.set_text("Lobby ID: "+str(LOBBY_ID))
		$"Players Title".set_text("Player List (0)")
		# Close session with all users
		for MEMBERS in LOBBY_MEMBERS:
			var SESSION_CLOSED: bool = Steam.closeP2PSessionWithUser(MEMBERS['steam_id'])
			print("[STEAM] P2P session closed with "+str(MEMBERS['steam_id'])+": "+str(SESSION_CLOSED))
		# Clear the local lobby list
		LOBBY_MEMBERS.clear()
		for MEMBER in $Players.get_children():
			MEMBER.hide()
			MEMBER.queue_free()
		# Enable the create lobby button
		$"Panel/Options/Create Lobby".set_disabled(false)
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
		print("[STEAM] There is a packet available.")
		# Get the packet
		var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)
		# If it is empty, set a warning
		if PACKET.empty():
			print("[WARNING] Read an empty packet with non-zero size!")
		# Get the remote user's ID
		var PACKET_SENDER: String = str(PACKET['steam_id_remote'])
		var PACKET_CODE: PoolByteArray = PACKET['data']
		# Make the packet data readable
		var READABLE: Dictionary = bytes2var(PACKET_CODE)
		# Print the packet to output
		$Output.append_bbcode("[STEAM] Packet from "+str(PACKET_SENDER)+": "+str(READABLE)+"\n")
		# Append logic here to deal with packet data
		if READABLE['message'] == "start":
			$Output.append_bbcode("[STEAM] Starting P2P game...\n")


# Send a Steam P2P packet
func _send_P2P_Packet(target: int, packet_data: Dictionary) -> void:
	# Set the send_type and channel
	var SEND_TYPE: int = Steam.P2P_SEND_RELIABLE
	var CHANNEL: int = 0
	# Create a data array to send the data through
	var PACKET_DATA: PoolByteArray = []
	PACKET_DATA.append_array(var2bytes(packet_data))
	# If sending a packet to everyone
	var SEND_RESPONSE: bool
	if target == 0:
		# If there is more than one user, send packets
		if LOBBY_MEMBERS.size() > 1:
			# Loop through all members that aren't you
			for MEMBER in LOBBY_MEMBERS:
				if MEMBER['steam_id'] != global.STEAM_ID:
					SEND_RESPONSE = Steam.sendP2PPacket(MEMBER['steam_id'], PACKET_DATA, SEND_TYPE, CHANNEL)
	# Else send the packet to a particular user
	else:
		# Send this packet
		SEND_RESPONSE = Steam.sendP2PPacket(target, PACKET_DATA, SEND_TYPE, CHANNEL)
	# The packets send response is...?
	$Output.append_bbcode("[STEAM] P2P packet sent successfully? "+str(SEND_RESPONSE)+"\n")


# Make a Steam P2P handshake
func _make_P2P_Handshake() -> void:
	$Output.append_bbcode("[STEAM] Sending P2P handshake to the lobby...\n")
	_send_P2P_Packet(0, {"message":"handshake", "from":global.STEAM_ID})


# Send test packet information
func _send_Test_Info() -> void:
	$Output.append_bbcode("[STEAM] Sending test packet data...\n")
	var TEST_DATA: Dictionary = {"title":"This is a test packet", "player_id":global.STEAM_ID, "player_hp":"5", "player_coord":"56,40"}
	_send_P2P_Packet(0, TEST_DATA)


# When receiving a P2P request from another user
func _on_P2P_Session_Request(remoteID: int) -> void:
	# Get the requester's name
	var REQUESTER: String = Steam.getFriendPersonaName(remoteID)
	# Print the debug message to output
	$Output.append_bbcode("[STEAM] P2P session request from "+str(REQUESTER)+"\n")
	# Accept the P2P session; can apply logic to deny this request if needed
	var SESSION_ACCEPTED: bool = Steam.acceptP2PSessionWithUser(remoteID)
	$Output.append_bbcode("[STEAM] P2P session was connected: "+str(SESSION_ACCEPTED)+"\n")
	# Make the initial handshake
	_make_P2P_Handshake()


# P2P connection failure
func _on_P2P_Session_Connect_Fail(lobby_id: int, session_error: int) -> void:
	# Note the session errors are: 0 - none, 1 - target user not running the same game, 2 - local user doesn't own app, 3 - target user isn't connected to Steam, 4 - connection timed out, 5 - unused
	# If no error was given
	if session_error == 0:
		print("[WARNING] Session failure with "+str(lobby_id)+" [no error given].")
	# Else if target user was not running the same game
	elif session_error == 1:
		print("[WARNING] Session failure with "+str(lobby_id)+" [target user not running the same game].")
	# Else if local user doesn't own app / game
	elif session_error == 2:
		print("[WARNING] Session failure with "+str(lobby_id)+" [local user doesn't own app / game].")
	# Else if target user isn't connected to Steam
	elif session_error == 3:
		print("[WARNING] Session failure with "+str(lobby_id)+" [target user isn't connected to Steam].")
	# Else if connection timed out
	elif session_error == 4:
		print("[WARNING] Session failure with "+str(lobby_id)+" [connection timed out].")
	# Else if unused
	elif session_error == 5:
		print("[WARNING] Session failure with "+str(lobby_id)+" [unused].")
	# Else no known error
	else:
		print("[WARNING] Session failure with "+str(lobby_id)+" [unknown error "+str(session_error)+"].")


#################################################
# HELPER FUNCTIONS
#################################################
# Get the lobby members from Steam
func _get_Lobby_Members() -> void:
	# Clear your previous lobby list
	LOBBY_MEMBERS.clear()
	# Clear the original player list
	for MEMBER in $Players.get_children():
		MEMBER.hide()
		MEMBER.queue_free()
	# Get the number of members from this lobby from Steam
	var MEMBERS: int = Steam.getNumLobbyMembers(LOBBY_ID)
	# Update the player list title
	$"Players Title".set_text("Player List ("+str(MEMBERS)+")")
	# Get the data of these players from Steam
	for MEMBER in range(0, MEMBERS):
		print(MEMBER)
		# Get the member's Steam ID
		var MEMBER_STEAM_ID: int = Steam.getLobbyMemberByIndex(LOBBY_ID, MEMBER)
		# Get the member's Steam name
		var MEMBER_STEAM_NAME: String = Steam.getFriendPersonaName(MEMBER_STEAM_ID)
		# Add them to the player list
		_add_Player_List(MEMBER_STEAM_ID, MEMBER_STEAM_NAME)


# Add a new Steam user to the connect users list
func _add_Player_List(steam_id: int, steam_name: String) -> void:
	print("Adding new player to the list: "+str(steam_id)+" / "+str(steam_name))
	# Add them to the list
	LOBBY_MEMBERS.append({"steam_id":steam_id, "steam_name":steam_name})
	# Instance the lobby member object
	var THIS_MEMBER: Object = LOBBY_MEMBER.instance()
	# Add their Steam name and ID
	THIS_MEMBER.name = str(steam_id)
	THIS_MEMBER._set_Member(steam_id, steam_name)
	# Connect the kick signal
	var THIS_SIGNAL: int = THIS_MEMBER.connect("kick_player", self, "_on_Lobby_Kick")
	print("[STEAM] Connecting kick_player signal to _on_Lobby_Kick for "+str(steam_name)+" ["+str(steam_id)+"]: "+str(THIS_SIGNAL))
	# Add the child node
	$Players.add_child(THIS_MEMBER)
	# If you are the host, enable the kick button
	if global.STEAM_ID == Steam.getLobbyOwner(LOBBY_ID):
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


# A user's information has changed
func _on_Persona_Change(steam_id: int, _flag: int) -> void:
	print("[STEAM] A user ("+str(steam_id)+") had information change, update the lobby list")
	# Update the player list
	_get_Lobby_Members()


# Connect a Steam signal and show the success code
func _connect_Steam_Signals(this_signal: String, this_function: String) -> void:
	var SIGNAL_CONNECT: int = Steam.connect(this_signal, self, this_function)
	print("[STEAM] Connecting "+str(this_signal)+" to "+str(this_function)+" successful: "+str(SIGNAL_CONNECT))


#################################################
# BUTTON FUNCTIONS
#################################################
# Creating a lobby
func _on_Create_Lobby_pressed() -> void:
	# Attempt to create a lobby
	_create_Lobby()
	$Output.append_bbcode("[STEAM] Attempt to create a new lobby...\n")
	# Disable the create lobby button
	$"Panel/Options/Create Lobby".set_disabled(true)


# Leaving the lobby
func _on_Leave_Lobby_pressed() -> void:
	_leave_Lobby()


# Getting associated metadata for the lobby
func _on_Get_Lobby_Data_pressed() -> void:
	DATA = Steam.getLobbyData(LOBBY_ID, "name")
	$Output.append_bbcode("[STEAM] Lobby data, name: "+str(DATA)+"\n")
	DATA = Steam.getLobbyData(LOBBY_ID, "mode")
	$Output.append_bbcode("[STEAM] Lobby data, mode: "+str(DATA)+"\n")


# Sending a test packet out to the players
func _on_Send_Packet_pressed() -> void:
	_send_Test_Info()


#################################################
# LOBBY BROWSER FUNCTIONS
#################################################
# Open the lobby list
func _on_Open_Lobby_List_pressed() -> void:
	$Lobbies.show()
	# Set distance to worldwide
	Steam.addRequestLobbyListDistanceFilter(3)
	# Request the list
	$Output.append_bbcode("[STEAM] Requesting a lobby list...\n")
	Steam.requestLobbyList()


# Getting a lobby match list
func _on_Lobby_Match_List(lobbies: Array) -> void:
	# Show the list 
	for LOBBY in lobbies:
		# Pull lobby data from Steam
		var LOBBY_NAME: String = Steam.getLobbyData(LOBBY, "name")
		var LOBBY_MODE: String = Steam.getLobbyData(LOBBY, "mode")
		var LOBBY_NUMS: int = Steam.getNumLobbyMembers(LOBBY)
		# Create a button for the lobby
		var LOBBY_BUTTON: Button = Button.new()
		LOBBY_BUTTON.set_text("Lobby "+str(LOBBY)+": "+str(LOBBY_NAME)+" ["+str(LOBBY_MODE)+"] - "+str(LOBBY_NUMS)+" Player(s)")
		LOBBY_BUTTON.set_size(Vector2(800, 50))
		LOBBY_BUTTON.set_name("lobby_"+str(LOBBY))
		LOBBY_BUTTON.set_text_align(0)
		LOBBY_BUTTON.set_theme(BUTTON_THEME)
		var LOBBY_SIGNAL: int = LOBBY_BUTTON.connect("pressed", self, "_join_Lobby", [LOBBY])
		print("[STEAM] Connecting pressed to function _join_Lobby for "+str(LOBBY)+" successfully: "+str(LOBBY_SIGNAL))
		# Add the new lobby to the list
		$Lobbies/Scroll/List.add_child(LOBBY_BUTTON)
	# Enable the refresh button
	$Lobbies/Refresh.set_disabled(false)


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
	# If there is even a message
	if MESSAGE.length() > 0:
		# Pass the message to Steam
		var IS_SENT: bool = Steam.sendLobbyChatMsg(LOBBY_ID, MESSAGE)
		# Was it sent successfully?
		if not IS_SENT:
			$Output.append_bbcode("[ERROR] Chat message '"+str(MESSAGE)+"' failed to send.\n")
		# Clear the chat input
		$Chat.clear()


# When a lobby message is received
# Using / delimiter for host commands like kick
func _on_Lobby_Message(_result: int, user: int, message: String, type: int) -> void:
	# We are only concerned with who is sending the message and what the message is
	var SENDER = Steam.getFriendPersonaName(user)
	# If this is a message or host command
	if type == 1:
		# If the lobby owner and the sender are the same, check for commands
		if user == Steam.getLobbyOwner(LOBBY_ID) and message.begins_with("/"):
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
	# Else this is a different type of message
	else:
		match type:
			2: $Output.append_bbcode(str(SENDER)+" is typing...\n")
			3: $Output.append_bbcode(str(SENDER)+" sent an invite that won't work in this chat!\n")
			4: $Output.append_bbcode(str(SENDER)+" sent a text emote that is depreciated.\n")
			6: $Output.append_bbcode(str(SENDER)+" has left the chat.\n")
			7: $Output.append_bbcode(str(SENDER)+" has entered the chat.\n")
			8: $Output.append_bbcode(str(SENDER)+" was kicked!\n")
			9: $Output.append_bbcode(str(SENDER)+" was banned!\n")
			10: $Output.append_bbcode(str(SENDER)+" disconnected.\n")
			11: $Output.append_bbcode(str(SENDER)+" sent an old, offline message.\n")
			12: $Output.append_bbcode(str(SENDER)+" sent a link that was removed by the chat filter.\n")


# When a lobby chat is updated
func _on_Lobby_Chat_Update(lobby_id: int, changed_id: int, making_change_id: int, chat_state: int) -> void:
	# Note that chat state changes is: 1 - entered, 2 - left, 4 - user disconnected before leaving, 8 - user was kicked, 16 - user was banned
	print("[STEAM] Lobby ID: "+str(lobby_id)+", Changed ID: "+str(changed_id)+", Making Change: "+str(making_change_id)+", Chat State: "+str(chat_state))
	# Get the user who has made the lobby change
	var CHANGER = Steam.getFriendPersonaName(changed_id)
	# If a player has joined the lobby
	if chat_state == 1:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has joined the lobby.\n")
	# Else if a player has left the lobby
	elif chat_state == 2:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has left the lobby.\n")
	# Else if a player has been kicked
	elif chat_state == 8:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has been kicked from the lobby.\n")
	# Else if a player has been banned
	elif chat_state == 16:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" has been banned from the lobby.\n")
	# Else there was some unknown change
	else:
		$Output.append_bbcode("[STEAM] "+str(CHANGER)+" did... something.\n")
	# Update the lobby now that a change has occurred
	_get_Lobby_Members()


func _on_Lobby_Kick(kick_id: int) -> void:
	# Pass the kick message to Steam
	var IS_SENT: bool = Steam.sendLobbyChatMsg(LOBBY_ID, "/kick:"+str(kick_id))
	# Was it send successfully?
	if not IS_SENT:
		print("[ERROR] Kick command failed to send.\n")


# Send the message by pressing enter
func _input(ev: InputEvent) -> void:
	if ev.is_pressed() and !ev.is_echo() and ev.is_action("chat_send"):
		_on_Send_Chat_pressed()


#################################################
# COMMAND LINE ARGUMENTS
#################################################
# Check the command line for arguments
# Used primarily if a player accepts an invite and does not have the game opened
func _check_Command_Line():
	var ARGUMENTS = OS.get_cmdline_args()
	# There are arguments to process
	if ARGUMENTS.size() > 0:
		# There is a connect lobby argument
		if ARGUMENTS[0] == "+connect_lobby":
			if int(ARGUMENTS[1]) > 0:
				print("CMD Line Lobby ID: "+str(ARGUMENTS[1]))
				_join_Lobby(int(ARGUMENTS[1]))


#################################################
# BACK BUTTON
#################################################
# Emit a signal to the main node letting it know the user is done
func _on_Back_pressed() -> void:
	# Leave the lobby if in one
	if LOBBY_ID > 0:
		_leave_Lobby()
	# Head back to the main menu
	emit_signal("back_to_main")
