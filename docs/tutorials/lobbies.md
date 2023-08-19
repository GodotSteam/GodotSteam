# Tutorials - Lobbies

One of the more requested tutorials is multiplayer lobbies and P2P networking through Steam; this tutorial specifically covers the lobby portion and [our P2P tutorial covers the other half](/tutorials/p2p).  Please note, only use this as a starting point.

I'd also like to suggest you [check out the Additional Resources section of this tutorial](/tutorials/lobbies/#additional-resources) before continuing on.

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

=== "Godot 2.x, 3.x"
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

		# Check for command line arguments
		_check_Command_Line()
	````
=== "Godot 4.x"
	````
	func _ready() -> void:
		Steam.lobby_created.connect(_on_Lobby_Created)
		Steam.lobby_match_list.connect(_on_Lobby_Match_List)
		Steam.lobby_joined.connect(_on_Lobby_Joined)
		Steam.lobby_chat_update.connect(_on_Lobby_Chat_Update)
		Steam.lobby_message.connect(_on_Lobby_Message)
		Steam.lobby_data_update.connect(_on_Lobby_Data_Update)
		Steam.lobby_invite.connect(_on_Lobby_Invite)
		Steam.join_requested.connect(_on_Lobby_Join_Requested)
		Steam.persona_state_change.connect(_on_Persona_Change)

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
		if ARGUMENTS[0] == "+connect_lobby":
		
			# Lobby invite exists so try to connect to it
			if int(ARGUMENTS[1]) > 0:

				# At this point, you'll probably want to change scenes
				# Something like a loading into lobby screen
				print("CMD Line Lobby ID: "+str(ARGUMENTS[1]))
				_join_Lobby(int(ARGUMENTS[1]))
````

This is important if the player is accepting a Steam invite or right-clicks a friend's name then selects 'Join Game' or 'Join Lobby' and doesn't have the game open. Doing either action will launch the game with the additional command ````+connect_lobby <Steam Lobby ID>````. Sadly Godot doesn't really understand this command argument so our ***_check_Command_Line()*** function has to be written to work within those constraints.

Additionally, you'll need to add the appropriate scene name to your Steamworks launch options on the Steamworks website. You'll want to add the full scene path (res://your-scene.tscn) on the **Arguments** line in your launch option. [You can read more about that, with details, in this link.](https://github.com/CoaguCo-Industries/GodotSteam/issues/100){ target="_blank" } Big thanks to **Antokolos** for answering this issue and providing a solid example.

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

- **0 - Private**<br />The only way to join the lobby is from an invite.
- **1 - Friends only**<br />Joinable by friends and invitees, but does not show up in the lobby list.
- **2 - Public**<br />Returned by search and visible to friends.
- **3 - Invisible**<br />Returned by search, but not visible to other friends.

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

Once this callback fires, you'll have your lobby ID which you can pass off to our **LOBBY_ID** variable for later use.  As the note says, the lobby should be set to joinable by default but, just in case, we add it here.  You can make the lobby unjoinable too.

You can also set some lobby data now; which can be whatever **key / value** pair you want.  I'm not aware of a maximum amount of pairs you can set.

And you'll notice I set ***allowP2PPacketRelay()*** to true at this point; this allows, as the note mentions, P2P connections to fallback to being relayed through Steam if needed.  This usually happens if you have NAT or firewall issues.

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

- **addRequestLobbyListStringFilter**<br />Allows you to look for specific works in the lobby metadata
- **addRequestLobbyListNumericalFilter**<br />Adds a numerical comparions filter (<=, <, =, >, >=, !=)
- **addRequestLobbyListNearValueFilter**<br />Gives results closes to the specified value you give
- **addRequestLobbyListFilterSlotsAvailable**<br />Only returns lobbies with a specified amount of open slots available
- **addRequestLobbyListResultCountFilter**<br/>Sets how many results you want returned
- **addRequestLobbyListDistanceFilter**<br/>Sets the distance to search for lobbies, like:
	- **0** - Close
	- **1** - Default
	- **2** - Far
	- **3** - Worldwide

Once you set all, some, or none of these, you can then call ***requestLobbyList()***. Once it pulls your lobby list it will fire a callback ***_on_Lobby_Match_List()***. You can then loop through the lobbies however you want.

In our example code, I do something like this to make buttons for each lobby:

=== "Godot 2.x, 3.x"
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
			LOBBY_BUTTON.set_text("Lobby "+str(LOBBY)+": "+str(LOBBY_NAME)+" ["+str(LOBBY_MODE)+"] - "+str(LOBBY_NUM_MEMBERS)+" Player(s)")
			LOBBY_BUTTON.set_size(Vector2(800, 50))
			LOBBY_BUTTON.set_name("lobby_"+str(LOBBY))
			LOBBY_BUTTON.connect("pressed", self, "_join_Lobby", [LOBBY])

			# Add the new lobby to the list
			$Lobbies/Scroll/List.add_child(LOBBY_BUTTON)
	````
=== "Godot 4.x"
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
			LOBBY_BUTTON.set_text("Lobby "+str(LOBBY)+": "+str(LOBBY_NAME)+" ["+str(LOBBY_MODE)+"] - "+str(LOBBY_NUM_MEMBERS)+" Player(s)")
			LOBBY_BUTTON.set_size(Vector2(800, 50))
			LOBBY_BUTTON.set_name("lobby_"+str(LOBBY))
			LOBBY_BUTTON.connect("pressed", Callable(self, "_join_Lobby").bind(LOBBY))

			# Add the new lobby to the list
			$Lobbies/Scroll/List.add_child(LOBBY_BUTTON)
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

- **1 (k_EChatRoomEnterResponseSuccess)**<br />Success.
- **2 (k_EChatRoomEnterResponseDoesntExist)**<br />Chat doesn't exist (probably closed).
- **3 (k_EChatRoomEnterResponseNotAllowed)**<br />General Denied - You don't have the permissions needed to join the chat.
- **4 (k_EChatRoomEnterResponseFull)**<br />Chat room has reached its maximum size.
- **5 (k_EChatRoomEnterResponseError)**<br />Unexpected Error.
- **6 (k_EChatRoomEnterResponseBanned)**<br />You are banned from this chat room and may not join.
- **7 (k_EChatRoomEnterResponseLimited)**<br />Joining this chat is not allowed because you are a limited user (no value on account).
- **8 (k_EChatRoomEnterResponseClanDisabled)**<br />Attempt to join a clan chat when the clan is locked or disabled.
- **9 (k_EChatRoomEnterResponseCommunityBan)**<br />Attempt to join a chat when the user has a community lock on their account.
- **10 (k_EChatRoomEnterResponseMemberBlockedYou)**<br />Join failed - a user that is in the chat has blocked you from joining.
- **11 (k_EChatRoomEnterResponseYouBlockedMember)**<br />Join failed - you have blocked a user that is already in the chat.
		
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
		LOBBY_MEMBERS.append({"steam_id":MEMBER_STEAM_ID, "steam_name":MEMBER_STEAM_NAME})
````

This will get the lobby members from Steam then loop through and get their names and Steam ID's then append them to our **LOBBY_MEMBERS** array for later use.  You can then display this list in your lobby room.

---

## Persona Changes / Avatars / Names
	
Sometimes you will see that a user's name and avatar, sometimes one or the other, won't immediately show up correctly. This is because our local user only really knows about friends and players they have played with; whatever is stored in local cache. A bit after a lobby is joined, this data will be sent by Steam which triggers a **persona_state_change** callback. You will want to update your player list to reflect this and get the correct name and avatar for unknown players.  Our connect ***_on_Persona_Change()*** function will do that:

````
# A user's information has changed
func _on_Persona_Change(steam_id: int, _flag: int) -> void:
	# Make sure you're in a lobby and this user is valid or Steam might spam your console log
	if LOBBY_ID > 0:
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

We won't get into what all this means just yet, but I wanted to show the code for the handshake function here since it is referenced; [more on that in the P2P tutorial](/tutorials/p2p). Your handshake messages can be anything and disregarded for the most part. Again, it is just to test our P2P session.

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

## Additional Resources

I'd like to point out some additional resources for you to check out:

First is this [video tutorial put together by **DawnsCrow Games**](https://youtu.be/si50G3S1XGU){ target="_blank" }.

Second is [**JDare**'s has a repo, **GodotSteamHL**](https://github.com/JDare/GodotSteamHL){ target="_blank" }, designed to streamline Steam's networking functionality (lobbies and P2P) which should serve as a handy guide and/or useful script.

I highly suggest you [check out the Example branch of the repo](https://github.com/CoaguCo-Industries/GodotSteam/tree/example){ target="_blank" }, specifically the **/src/examples/lobby.tscn** which will have the full working code and test interface for this tutorial.

---

That concludes the lobby tutorial.  At this point you may want to [check out the P2P tutorial which compliments this one](/tutorials/p2p).  Obviously this code should not be used for production and more for a very, very, very, simple guide on where to start.

[To see this tutorial in action, check out our GodotSteam 3 Example Project on Github.](https://github.com/CoaguCo-Industries/GodotSteam-3-Example-Project){ target="_blank" } There you can get a full view of the code used which can serve as a starting point for you to branch out from.