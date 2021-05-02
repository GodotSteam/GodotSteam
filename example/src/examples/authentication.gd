extends Node2D
# Set up some variables
var TICKET: Dictionary		# Your auth ticket
var CLIENT_TICKETS: Array	# Array of clients tickets

# Set up some signals
signal back_to_main


func _ready():
	# Connect some Steam signals
	Steam.connect("get_auth_session_ticket_response", self, "_get_Auth_Session_Ticket_Response")
	Steam.connect("validate_auth_ticket_response", self, "_validate_Auth_Ticket_Response")


# Callback from getting the auth ticket from Steam
func _get_Auth_Session_Ticket_Response(auth_ticket: int, result: int) -> void:
	$Output.append_bbcode("Auth session result: "+str(result)+"\n")
	$Output.append_bbcode("Auth session ticket handle: "+str(auth_ticket)+"\n\n")


# Callback from attempting to validate the auth ticket
func _validate_Auth_Ticket_Response(authID: int, response: int, ownerID: int) -> void:
	$Output.append_bbcode("Ticket Owner: "+str(authID)+"\n")
	# Make the response more verbose
	var VERBOSE_RESPONSE: String
	match response:
		0: VERBOSE_RESPONSE = "Steam has verified the user is online, the ticket is valid and ticket has not been reused."
		1: VERBOSE_RESPONSE = "The user in question is not connected to Steam."
		2: VERBOSE_RESPONSE = "The user doesn't have a license for this App ID or the ticket has expired."
		3: VERBOSE_RESPONSE = "The user is VAC banned for this game."
		4: VERBOSE_RESPONSE = "The user account has logged in elsewhere and the session containing the game instance has been disconnected."
		5: VERBOSE_RESPONSE = "VAC has been unable to perform anti-cheat checks on this user."
		6: VERBOSE_RESPONSE = "The ticket has been canceled by the issuer."
		7: VERBOSE_RESPONSE = "This ticket has already been used, it is not valid."
		8: VERBOSE_RESPONSE = "This ticket is not from a user instance currently connected to steam."
		9: VERBOSE_RESPONSE = "The user is banned for this game. The ban came via the web api and not VAC."
	$Output.append_bbcode("Auth response: "+str(VERBOSE_RESPONSE)+"\n")
	$Output.append_bbcode("Game owner ID: "+str(ownerID)+"\n\n")


# Request an auth ticket from Steam
func _on_GetAuthTicket_pressed() -> void:
	TICKET = Steam.getAuthSessionTicket()
	$Output.append_bbcode("Auth Ticket: "+str(TICKET)+"\n\n")


# Send your auth ticket to the other client or server
func _send_Auth_Ticket() -> void:
	pass


func _on_BeginAuthSession_pressed() -> void:
	var RESPONSE: int = Steam.beginAuthSession(TICKET['buffer'], TICKET['size'], global.STEAM_ID)
	# Get a verbose response
	var VERBOSE_RESPONSE: String
	match RESPONSE:
		0: VERBOSE_RESPONSE = "Ticket is valid for this game and this Steam ID."
		1: VERBOSE_RESPONSE = "The ticket is invalid."
		2: VERBOSE_RESPONSE = "A ticket has already been submitted for this Steam ID."
		3: VERBOSE_RESPONSE = "Ticket is from an incompatible interface version."
		4: VERBOSE_RESPONSE = "Ticket is not for this game."
		5: VERBOSE_RESPONSE = "Ticket has expired."
	$Output.append_bbcode("Auth verifcation response: "+str(VERBOSE_RESPONSE)+"\n\n")
	
	# If successful, add this client to the list
	if RESPONSE == 0:
		CLIENT_TICKETS.append({"id": global.STEAM_ID, "ticket": TICKET['id']})
		# Allow the client to join the game


#################################################
# ENDING AUTH SESSION FUNCTIONS
#################################################
# Cancel the auth ticket
func _on_CancelAuthTicket_pressed() -> void:
	Steam.cancelAuthTicket(TICKET['id'])
	$Output.append_bbcode("Canceling your auth ticket...\n\n")


# Ends the auth session with, well, you
func _on_EndAuthSession_pressed() -> void:
	# Loop through all client tickets and close them out
	for C in CLIENT_TICKETS:
		$Output.append_bbcode("Ending auth session with "+str(C['id'])+"\n\n")
		Steam.endAuthSession(C['id'])
		# Remove this client from the list
		CLIENT_TICKETS.erase(C)


#################################################
# BACK BUTTON
#################################################
# Emit a signal to the main node letting it know the user is done
func _on_Back_pressed() -> void:
	emit_signal("back_to_main")
