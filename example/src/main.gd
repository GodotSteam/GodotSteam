extends Node

func _ready() -> void:
	# Set the default Steamworks information into the output section
	if global.IS_ONLINE:
		$Output/Status/Title.set_text("Steamworks Status (Online)")
	else:
		$Output/Status/Title.set_text("Steamworks Status (Offline)")
	$Output/Status/ID.set_text("Steam ID: "+str(global.STEAM_ID))
	$Output/Status/Username.set_text("Username: "+str(global.STEAM_USERNAME))
	$Output/Status/Owns.set_text("Owns App: "+str(global.IS_OWNED))


#################################################
# EXAMPLE LOADING
#################################################
# Load up an example
func _start_Example(which: String) -> void:
	# Hide the main menu
	_toggle_Main_Menu(false)

	# Hide the welcome message
	_toggle_Welcome_Message(false)

	# Change the title based on the button pressed
	match which:
		"avatars": $Title.set_text("Steamworks Player Avatars")
		"inputs": $Title.set_text("Steamworks Controller Inputs")
		"leaderboards": $Title.set_text("Steamworks Leaderboards")
		"lobby": $Title.set_text("Steamworks Lobby")
		"stats-achieves": $Title.set_text("Steamworks Statistics And Achievements")
		"authentication": $Title.set_text("Steamworks Authentication")

	# Instance the example
	var EXAMPLE: Object = load("res://src/examples/"+str(which)+".tscn").instance()
	# Load it
	$Example.add_child(EXAMPLE)

	# Connect the back button
	EXAMPLE.connect("back_to_main", self, "_show_Main")


#################################################
# COMPONENT TOGGLING
#################################################
# The back button was pressed, show the main menu again
func _show_Main() -> void:
	# Resore the main menu title
	$Title.set_text("GodotSteam Example Project")

	# Show the main menu
	_toggle_Main_Menu(true)

	# Show the welcome message
	_toggle_Welcome_Message(true)

	# Discard the child nodes
	for EXAMPLE in $Example.get_children():
		EXAMPLE.hide()
		EXAMPLE.queue_free()


# Toggle whether the main menu is hidden or shown
func _toggle_Main_Menu(is_shown: bool) -> void:
	if is_shown:
		$Examples.show()
	else:
		$Examples.hide()


# Toggle the welcome message
func _toggle_Welcome_Message(is_shown: bool) -> void:
	if is_shown:
		$Message.show()
	else:
		$Message.hide()


# Exit the application
func _on_Exit_pressed():
	get_tree().quit()
