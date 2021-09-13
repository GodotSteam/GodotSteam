extends Control
# Set up some variables
var JOYSTICK_NUM
var CURRENT_JOYSTICK: int = -1
var AXIS_VALUE
var STEAM_CONTROLLERS: Array
var GODOT_CONTROLLERS: Array

# Set up some signals
signal back_to_main


func _ready():
	Input.connect("joy_connection_changed", self, "_on_joy_connection_changed")


# Initialize the Inputs interface
func _on_Init_pressed() -> void:
	# Get Godot's input list
	GODOT_CONTROLLERS = Input.get_connected_joypads()
	
	# Print the list to output
	$Output.append_bbcode("Godot found "+str(GODOT_CONTROLLERS.size())+" connected controllers:\n")
	for CONTROLLER in GODOT_CONTROLLERS:
		$Output.append_bbcode(str(Input.get_joy_name(GODOT_CONTROLLERS[CONTROLLER]))+"\n")

	# Initialize Steam Inputs
	if Steam.inputInit():
		$Output.append_bbcode("\nSteam Inputs is running!\n\n")
	else:
		$Output.append_bbcode("\nSteam Inputs is not running... something went wrong.\n\n")

	# Start the frame run
	Steam.runFrame()


# Get a list of all connected controllers
func _on_GetControllers_pressed() -> void:
	Steam.runFrame()
	
	# Get Steam's input list
	STEAM_CONTROLLERS = Steam.getConnectedControllers()
	
	# Print the list to output
	$Output.append_bbcode("Steam found "+str(STEAM_CONTROLLERS.size())+" connected controllers:\n")
	for CONTROLLER in STEAM_CONTROLLERS.size():
		$Output.append_bbcode(str(STEAM_CONTROLLERS[CONTROLLER])+"\n")


# Shutdown the Inputs interface
func _on_Shutdown_pressed() -> void:
	if Steam.inputShutdown():
		$Output.append_bbcode("[Steam Inputs] Shutdown successfully.\n\n")
	else:
		$Output.append_bbcode("[Steam Inputs] Shutdown failed for some reason.\n\n")


# Called whenever a joypad has been connected or disconnected.
func _on_joy_connection_changed(device_id, connected):
	if connected:
		$Output.append_bbcode(str(Input.get_joy_name(device_id))+"\n\n")


# Vibrate all connected input handles
func _on_Vibrate_pressed() -> void:
	$Output.append_bbcode("[Steam Inputs] Vibrating all applicable and connected controllers...\n")

	for CONTROLLER in STEAM_CONTROLLERS.size():
		$Output.append_bbcode("[Steam Inputs] Vibrating controller "+str(STEAM_CONTROLLERS[CONTROLLER])+" with speeds 1000\n")
		Steam.triggerVibration(STEAM_CONTROLLERS[CONTROLLER], 5000, 5000)


# Create a haptic pulse
func _on_Haptic_pressed() -> void:
	$Output.append_bbcode("[Steam Inputs] Sending haptic pulse to all applicable and connected controllers...\n\n")

	for CONTROLLER in STEAM_CONTROLLERS.size():
		Steam.triggerHapticPulse(STEAM_CONTROLLERS[CONTROLLER], 0, 500000)


# Create a repeated haptic pulse
func _on_HapticRepeated_pressed() -> void:
	$Output.append_bbcode("[Steam Inputs] Sending repeated haptic pulse to all applicable and connected controllers...\n\n")

	for CONTROLLER in STEAM_CONTROLLERS.size():
		Steam.triggerRepeatedHapticPulse(STEAM_CONTROLLERS[CONTROLLER], 0, 50000, 500000, 10, 0)


# Get the input's type by handle
func _on_GetName_pressed() -> void:
	$Output.append_bbcode("[Steam Inputs] Get input type by it's handle...\n\n")

	for CONTROLLER in STEAM_CONTROLLERS.size():
		var TYPE: String = Steam.getInputTypeForHandle(STEAM_CONTROLLERS[CONTROLLER])

		# Print it to the output
		$Output.append_bbcode("For handle "+str(STEAM_CONTROLLERS[CONTROLLER])+": "+str(TYPE)+"\n")


#################################################
# BACK BUTTON
#################################################
# Emit a signal to the main node letting it know the user is done
func _on_Back_pressed() -> void:
	emit_signal("back_to_main")
