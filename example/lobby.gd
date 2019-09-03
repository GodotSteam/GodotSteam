extends Node2D
# Set up some variables
var STEAM_ID = 0
var STEAM_USERNAME = ""
var STEAM_LOBBY_ID = 0
var LOBBY_MEMBERS = []
var DATA

# Called when the node enters the scene tree for the first time.
func _ready():
	# Initializing Steam
	_initialize_Steam()
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

func _process(delta):
	# Run Steam callbacks
	Steam.run_callbacks()
	# Get packets
	_read_P2P_Packet()

#################################################
# INITIALIZING STEAM
#################################################
func _initialize_Steam():
	var INIT = Steam.steamInit()
	print("Did Steam initialize?: "+str(INIT))
	STEAM_ID = Steam.getSteamID()
	print("Your Steam ID is: "+str(STEAM_ID))
	STEAM_USERNAME = Steam.getPersonaName()
	print("Your Steam username is: "+str(STEAM_USERNAME))

#################################################
# LOBBY FUNCTIONS
#################################################
# When the user starts a game with multiplayer enabled
func _create_Lobby():
	# Make sure a lobby is not already set
	if STEAM_LOBBY_ID == 0:
		# Set the lobby to public with two members max
		Steam.createLobby(2, 2)

# A lobby has been successfully created
func _on_Lobby_Created(connect, lobbyID):
	if connect == 0:
		# Set the lobby ID
		STEAM_LOBBY_ID = lobbyID
		print("Created a lobby: "+str(STEAM_LOBBY_ID))
		# Print the lobby ID to a label
		$Lobby.set_text("Lobby ID: "+str(STEAM_LOBBY_ID))
		# Print to the output display
		_append_Message("A lobby, with ID "+str(STEAM_LOBBY_ID)+" has been created.")
		# Set some lobby data
		Steam.setLobbyData(lobbyID, "name", "Gramps' Lobby")
		Steam.setLobbyData(lobbyID, "mode", "GodotSteam test")
		# Allow P2P connections to fallback to being relayed through Steam if needed
		var RELAY = Steam.allowP2PPacketRelay(true)
		print("Allowing Steam to be relay backup: "+str(RELAY))
		# Enable the leave lobby button and all testing buttons
		_change_Button_Controls(false)

# Getting a lobby match list
func _on_Lobby_Match_List(lobbies):
	# Show the list 
	for LOBBY in lobbies:
		# Pull lobby data from Steam
		var LOBBY_NAME = Steam.getLobbyData(LOBBY, "name")
		var LOBBY_MODE = Steam.getLobbyData(LOBBY, "mode")
		var LOBBY_MEMBERS = Steam.getNumLobbyMembers(LOBBY)
		# Create a button for the lobby
		var LOBBY_BUTTON = Button.new()
		LOBBY_BUTTON.set_text("Lobby "+str(LOBBY)+": "+str(LOBBY_NAME)+" ["+str(LOBBY_MODE)+"] - "+str(LOBBY_MEMBERS)+" Player(s)")
		LOBBY_BUTTON.set_size(Vector2(800, 50))
		LOBBY_BUTTON.set_name("lobby_"+str(LOBBY))
		LOBBY_BUTTON.connect("pressed", self, "_join_Lobby", [LOBBY])
		# Add the new lobby to the list
		$"Lobby Panel/Panel/Scroll/VBox".add_child(LOBBY_BUTTON)

# When the player is joining a lobby
func _join_Lobby(lobbyID):
	_append_Message("Attempting to join lobby "+str(lobbyID)+"...")
	# Close lobby panel if open
	_on_Close_Lobbies_pressed()
	# Clear any previous lobby lists
	LOBBY_MEMBERS.clear()
	# Make the lobby join request to Steam
	Steam.joinLobby(lobbyID)

# When a lobby is joined
func _on_Lobby_Joined(lobbyID, permissions, locked, response):
	# Set this lobby ID as your lobby ID
	STEAM_LOBBY_ID = lobbyID
	# Print the lobby ID to a label
	$Lobby.set_text("Lobby ID: "+str(STEAM_LOBBY_ID))
	# Append to output
	_append_Message("Joined lobby "+str(STEAM_LOBBY_ID)+".")
	# Get the lobby members
	_get_Lobby_Members()
	# Enable all necessary buttons
	_change_Button_Controls(false)
	# Make the initial handshake
	_make_P2P_Handshake()

# When accepting an invite
func _on_Lobby_Join_Requested(lobbyID, friendID):
	# Set the lobby owner's Steam ID
	LOBBY_MEMBERS['steam_id'] = friendID
	# Get the lobby owner's name
	LOBBY_MEMBERS['steam_name'] = Steam.getFriendPersonaName(friendID)
	# Make the intial handshake
	_make_P2P_Handshake()

# When a lobby message is received
func _on_Lobby_Message(result, user, message, type):
	# We are only concerned with who is sending the message and what the message is
	var SENDER = Steam.getFriendPersonaName(user)
	_append_Message(str(SENDER)+" says '"+str(message)+"'")

# When a lobby chat is updated
func _on_Lobby_Chat_Update(lobbyID, changedID, makingChangeID, chatState):
	# Note that chat state changes is: 1 - entered, 2 - left, 4 - user disconnected before leaving, 8 - user was kicked, 16 - user was banned
	print("Lobby ID: "+str(lobbyID)+", Changed ID: "+str(changedID)+", Making Change: "+str(makingChangeID)+", Chat State: "+str(chatState))
	# Get the user who has made the lobby change
	var CHANGER = Steam.getFriendPersonaName(makingChangeID)
	# If a player has joined the lobby
	if chatState == 1:
		_append_Message(str(CHANGER)+" has joined the lobby.")
	# Else if a player has left the lobby
	elif chatState == 2:
		_append_Message(str(CHANGER)+" has left the lobby.")
	# Else if a player has been kicked
	elif chatState == 8:
		_append_Message(str(CHANGER)+" has been kicked from the lobby.")
	# Else if a player has been banned
	elif chatState == 16:
		_append_Message(str(CHANGER)+" has been banned from the lobby.")
	# Else there was some unknown change
	else:
		_append_Message(str(CHANGER)+" did... something.")
	# Update the lobby now that a change has occurred
	_get_Lobby_Members()

# Whan lobby metadata has changed
func _on_Lobby_Data_Update(success, lobbyID, memberID, key):
	print("Success: "+str(success)+", Lobby ID: "+str(lobbyID)+", Member ID: "+str(memberID)+", Key: "+str(key))

# When the player leaves a lobby for whatever reason
func _leave_Lobby():
	# If in a lobby, leave it
	if STEAM_LOBBY_ID != 0:
		# Append a new message
		_append_Message("Leaving lobby "+str(STEAM_LOBBY_ID)+".")
		# Send leave request to Steam
		Steam.leaveLobby(STEAM_LOBBY_ID)
		# Wipe the Steam lobby ID then display the default lobby ID and player list title
		STEAM_LOBBY_ID = 0
		$Lobby.set_text("Lobby ID: "+str(STEAM_LOBBY_ID))
		$Players/Title.set_text("Player List (0)")
		# Close session with all users
		for MEMBERS in LOBBY_MEMBERS:
			Steam.closeP2PSessionWithUser(MEMBERS['steam_id'])
		# Clear the local lobby list
		LOBBY_MEMBERS.clear()
		# Disable the leave lobby button and all test buttons
		_change_Button_Controls(true)

#################################################
# P2P NETWORKING FUNCTIONS
#################################################
# Read a Steam P2P packet
func _read_P2P_Packet():
	var PACKET_SIZE = Steam.getAvailableP2PPacketSize(0)
	# There is a packet
	if PACKET_SIZE > 0:
		_append_Message("There is a packet available.")
		var PACKET = Steam.readP2PPacket(PACKET_SIZE, 0)
		if PACKET.empty():
			print("WARNING: read an empty packet with non-zero size!")
		# Get the remote user's ID
		var PACKET_ID = str(PACKET.steamIDRemote)
		var PACKET_CODE = str(PACKET.data[0])
		# Make the packet data readable
		var READABLE = bytes2var(PACKET.data.subarray(1, PACKET_SIZE - 1))
		# Print the packet to output
		_append_Message("Packet: "+str(READABLE))
		# Append logic here to deal with packet data

# Send a Steam P2P packet
func _send_P2P_Packet(data, send_type, channel):
	# If there is more than one user, send packets
	if LOBBY_MEMBERS.size() > 1:
		# Loop through all members that aren't you
		for MEMBER in LOBBY_MEMBERS:
			if MEMBER['steam_id'] != STEAM_ID:
				Steam.sendP2PPacket(MEMBER['steam_id'], data, send_type, channel)

# Make a Steam P2P handshake
func _make_P2P_Handshake():
	print("Sending P2P handshake to the lobby")
	var DATA = PoolByteArray()
	DATA.append(256)
	DATA.append_array(var2bytes({"message":"handshake", "from":STEAM_ID}))
	_send_P2P_Packet(DATA, 2, 0)

# Send test packet information
func _send_Test_Info():
	print("Sending test packet data")
	var DATA = PoolByteArray()
	DATA.append(256)
	DATA.append_array(var2bytes({"title":"This is a test packet", "player_id":STEAM_ID, "player_hp":"5", "player_coord":"56,40"}))
	_send_P2P_Packet(DATA, 2, 0)

# When receiving a P2P request from another user
func _on_P2P_Session_Request(remoteID):
	# Get the requester's name
	var REQUESTER = Steam.getFriendPersonaName(remoteID)
	# Print the debug message to output
	_append_Message("P2P session request from "+str(REQUESTER))
	# Accept the P2P session; can apply logic to deny this request if needed
	Steam.acceptP2PSessionWithUser(remoteID)
	# Make the initial handshake
	_make_P2P_Handshake()

# P2P connection failure
func _on_P2P_Session_Connect_Fail(lobbyID, session_error):
	# Note the session errors are: 0 - none, 1 - target user not running the same game, 2 - local user doesn't own app, 3 - target user isn't connected to Steam, 4 - connection timed out, 5 - unused
	# If no error was given
	if session_error == 0:
		_append_Message("WARNING: Session failure with "+str(lobbyID)+" [no error given].")
	# Else if target user was not running the same game
	elif session_error == 1:
		_append_Message("WARNING: Session failure with "+str(lobbyID)+" [target user not running the same game].")
	# Else if local user doesn't own app / game
	elif session_error == 2:
		_append_Message("WARNING: Session failure with "+str(lobbyID)+" [local user doesn't own app / game].")
	# Else if target user isn't connected to Steam
	elif session_error == 3:
		_append_Message("WARNING: Session failure with "+str(lobbyID)+" [target user isn't connected to Steam].")
	# Else if connection timed out
	elif session_error == 4:
		_append_Message("WARNING: Session failure with "+str(lobbyID)+" [connection timed out].")
	# Else if unused
	elif session_error == 5:
		_append_Message("WARNING: Session failure with "+str(lobbyID)+" [unused].")
	# Else no known error
	else:
		_append_Message("WARNING: Session failure with "+str(lobbyID)+" [unknown error "+str(session_error)+"].")

#################################################
# HELPER FUNCTIONS
#################################################
# Append a message to the output display
func _append_Message(message):
	$Output/Display.add_text("\n"+str(message))

# Get the lobby members from Steam
func _get_Lobby_Members():
	# Clear your previous lobby list
	LOBBY_MEMBERS.clear()
	# Get the number of members from this lobby from Steam
	var MEMBERS = Steam.getNumLobbyMembers(STEAM_LOBBY_ID)
	print("Number of members in this lobby: "+str(MEMBERS))
	# Update the player list title
	$Players/Title.set_text("Player List ("+str(MEMBERS)+")")
	# Get the data of these players from Steam
	for MEMBER in range(0, MEMBERS):
		# Get the member's Steam ID
		var MEMBER_STEAM_ID = Steam.getLobbyMemberByIndex(STEAM_LOBBY_ID, MEMBER)
		print("This member's Steam ID: "+str(MEMBER_STEAM_ID))
		# Get the member's Steam name
		var MEMBER_STEAM_NAME = Steam.getFriendPersonaName(MEMBER_STEAM_ID)
		print("This member's Steam username: "+str(MEMBER_STEAM_NAME))
		# Add them to the player list
		_add_Player_List(MEMBER_STEAM_ID, MEMBER_STEAM_NAME)

# Add a new Steam user to the connect users list
func _add_Player_List(steam_id, steam_name):
	# Add them to the list
	LOBBY_MEMBERS.append({"steam_id":steam_id, "steam_name":steam_name})
	# Clear the original player list
	$Players/Display.clear()
	# Update the player list
	for MEMBER in LOBBY_MEMBERS:
		$Players/Display.add_text(str(MEMBER['steam_name'])+" ("+str(MEMBER['steam_id'])+")\n")

# Enable or disable a gang of buttons
func _change_Button_Controls(toggle):
	$"Leave Lobby".set_disabled(toggle)
	$"Get Lobby Data".set_disabled(toggle)
	$"Send Packet".set_disabled(toggle)
	$"Send Chat".set_disabled(toggle)
	# Caveat for the lineedit
	if toggle:
		$Chat.set_editable(false)
	else:
		$Chat.set_editable(true)

#################################################
# BUTTON FUNCTIONS
#################################################
# Creating a lobby
func _on_Create_Lobby_pressed():
	_create_Lobby()

# Leaving the lobby
func _on_Leave_Lobby_pressed():
	_leave_Lobby()

# Getting associated metadata for the lobby
func _on_Get_Lobby_Data_pressed():
	DATA = Steam.getLobbyData(STEAM_LOBBY_ID, "name")
	_append_Message("Lobby data, name: "+str(DATA))
	DATA = Steam.getLobbyData(STEAM_LOBBY_ID, "mode")
	_append_Message("Lobby data, mode: "+str(DATA))

# Sending a test packet out to the players
func _on_Send_Packet_pressed():
	_send_Test_Info()

# Open the lobby list
func _on_Open_Lobby_List_pressed():
	$"Lobby Panel".popup()
	# Set distance to worldwide
	Steam.addRequestLobbyListDistanceFilter(3)
	print("Requesting a lobby list")
	Steam.requestLobbyList()

# Close the lobbies screen
func _on_Close_Lobbies_pressed():
	$"Lobby Panel".hide()

# Send a chat message
func _on_Send_Chat_pressed():
	# Get the entered chat message
	var MESSAGE = $Chat.get_text()
	# Pass the message to Steam
	var SENT = Steam.sendLobbyChatMsg(STEAM_LOBBY_ID, MESSAGE)
	# Was it sent successfully?
	if not SENT:
		_append_Message("ERROR: Chat message failed to send.")
	# Clear the chat input
	$Chat.clear()