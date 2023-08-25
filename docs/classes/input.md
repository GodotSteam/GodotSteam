# Input

Steam Input API is a flexible action-based API that supports all major controller types - Xbox, PlayStation, Nintendo Switch Pro, and Steam Controllers.
  
See the [Steam Input](https://partner.steamgames.com/doc/features/steam_controller){ target="_blank" } documentation for more information.

---

## Functions

---

### activateActionSet

!!! function "activateActionSet( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle )"
	Reconfigure the controller to use the specified action set (i.e. "Menu", "Walk", or "Drive").

	This is cheap, and can be safely called repeatedly. It's often easier to repeatedly call it in your state loops, instead of trying to place it in all of your state transitions.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#ActivateActionSet){ .md-button .md-button--store target="_blank" }

### activateActionSetLayer

!!! function "activateActionSetLayer( ```uint64_t``` input_handle, ```uint64_t``` action_set_layer_handle )"
	Reconfigure the controller to use the specified action set layer.

	See the [Action Set Layers](https://partner.steamgames.com/doc/features/steam_controller/action_set_layers){ target="_blank" } article for full details and an in-depth practical example.

	**Returns:** void
 
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#ActivateActionSetLayer){ .md-button .md-button--store target="_blank" }

### deactivateActionSetLayer

!!! function "deactivateActionSetLayer( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle )"
	Reconfigure the controller to stop using the specified action set.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#DeactivateActionSetLayer){ .md-button .md-button--store target="_blank" }

 
### deactivateAllActionSetLayers

!!! function "deactivateAllActionSetLayers( ```uint64_t``` input_handle )"
	Reconfigure the controller to stop using all action set layers.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#DeactivateAllActionSetLayers){ .md-button .md-button--store target="_blank" }

### getActiveActionSetLayers

!!! function "getActiveActionSetLayers( ```uint64_t``` input_handle)"
	Fill an array with all of the currently active action set layers for a specified controller handle.

	**Returns:** array

	Contains a list of handles (uint64_t).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetActiveActionSetLayers){ .md-button .md-button--store target="_blank" }

### getActionSetHandle

!!! function "getActionSetHandle( ```string``` action_set_name )"
	Fill an array with all of the currently active action set layers for a specified controller handle.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetActiveActionSetLayers){ .md-button .md-button--store target="_blank" }
 
### getActionOriginFromXboxOrigin

!!! function "getActionOriginFromXboxOrigin( ```uint64_t``` input_handle, ```int``` origin )"
	Get an action origin that you can use in your glyph look up table or passed into [getGlyphForActionOrigin](#getglyphforactionorigin) or [getStringForActionOrigin](#getstringforactionorigin).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetActionOriginFromXboxOrigin){ .md-button .md-button--store target="_blank" }

### getAnalogActionData

!!! function "getAnalogActionData( ```uint64_t``` input_handle, ```uint64_t``` analog_action_handle )"
	Gets the current state of the supplied analog game action.

	**Returns:** dictionary 

	Contains the following keys:

	* mode (int)
	* x (float)
	* y (float)
	* active (bool)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetAnalogActionData){ .md-button .md-button--store target="_blank" }

### getAnalogActionHandle

!!! function "getAnalogActionHandle( ```string``` action_name )"
	Get the handle of the specified analog action.

	**Returns:** uint64_t

	**Note:** This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two different action sets, this function will only ever return one of them and the other will be ignored.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetAnalogActionHandle){ .md-button .md-button--store target="_blank" }

### getAnalogActionOrigins

!!! function "getAnalogActionOrigins( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle, ```uint64_t``` analog_action_handle )"
	Get the origin(s) for an analog action within an action set by filling the return array with handles. Use this to display the appropriate on-screen prompt for the action.

	**Returns:** array

	Contains a list of handles (int).

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetAnalogActionOrigins){ .md-button .md-button--store target="_blank" }

### getConnectedControllers

!!! function "getConnectedControllers()"
	Enumerates currently connected controllers by filling returned array with controller handles.

	**Returns:** array

	Contains a list of controller handle (uint64_t).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetConnectedControllers){ .md-button .md-button--store target="_blank" }

### getControllerForGamepadIndex

!!! function "getControllerForGamepadIndex( ```int``` index )"
	Returns the associated controller handle for the specified emulated gamepad. Can be used with [getInputTypeForHandle](#getinputtypeforhandle) to determine the type of controller using Steam Input Gamepad Emulation.

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetControllerForGamepadIndex){ .md-button .md-button--store target="_blank" }
 
### getCurrentActionSet

!!! function "getCurrentActionSet( ```uint64_t``` input_handle )"
	Get the currently active action set for the specified controller.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetCurrentActionSet){ .md-button .md-button--store target="_blank" }

### getDeviceBindingRevision

!!! function "getDeviceBindingRevision( ```uint64_t``` input_handle )"
	Get's the major and minor device binding revisions for Steam Input API configurations. Minor revisions are for small changes such as adding a new option action or updating localization in the configuration. When updating a Minor revision only one new configuration needs to be update with the "Use Action Block" flag set. Major revisions are to be used when changing the number of action sets or otherwise reworking configurations to the degree that older configurations are no longer usable. When a user's binding disagree's with the major revision of the current official configuration Steam will forcibly update the user to the new configuration. New configurations will need to be made for every controller when updating the Major revision.

	**Returns:** array

	Contains a list of:

	* major (int)
	* minor (int)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDeviceBindingRevision){ .md-button .md-button--store target="_blank" }

### getDigitalActionData

!!! function "getDigitalActionData( ```uint64_t``` input_handle, ```uint64_t``` digital_action_handle )"
	Returns the current state of the supplied digital game action.

	**Returns:** dictionary 

	* state (bool)
	* active (bool)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDigitalActionData){ .md-button .md-button--store target="_blank" }

### getDigitalActionHandle

!!! function "getDigitalActionHandle( ```string``` action_name )"
	Get the handle of the specified digital action.

	**Returns:** uint64_t

	**Note:** This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two different action sets, this function will only ever return one of them and the other will be ignored.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDigitalActionHandle){ .md-button .md-button--store target="_blank" }
	
 
### getDigitalActionOrigins

!!! function "getDigitalActionOrigins( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle, ```uint64_t``` digital_action_handle )"

	Get the origin(s) for a digital action within an action set by filling return array with input handles. Use this to display the appropriate on-screen prompt for the action.

	**Returns:** array

	Contains a list of handles (int).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDigitalActionOrigins){ .md-button .md-button--store target="_blank" }

### getGamepadIndexForController

!!! function "getGamepadIndexForController( ```uint64_t``` input_handle )"
	Returns the associated gamepad index for the specified controller, if emulating a gamepad.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetGamepadIndexForController){ .md-button .md-button--store target="_blank" }

### getGlyphForActionOrigin

!!! function "getGlyphForActionOrigin( ```int``` origin )"
	Get a local path to art for on-screen glyph for a particular origin.

	**Returns:** string.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetGlyphForActionOrigin){ .md-button .md-button--store target="_blank" }

### getInputTypeForHandle

!!! function "getInputTypeForHandle( ```uint64_t``` input_handle )"
	Returns the input type (device model) for the specified controller. This tells you if a given controller is a Steam controller, Xbox 360 controller, PS4 controller, etc.

	**Returns:** string.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetInputTypeForHandle){ .md-button .md-button--store target="_blank" }
 
### getMotionData

!!! function "getMotionData( ```uint64_t``` input_handle )"
	Returns raw motion data for the specified controller.

	**Returns:** dictionary 

	* rot_quat_x (float)
	* rot_quat_y (float)
	* rot_quat_z (float)
	* rot_quat_w (float)
	* pos_accel_x (float)
	* pos_accel_y (float)
	* pos_accel_z (float)
	* rot_vel_x (float)
	* rot_vel_y (float)
	* rot_vel_z (float)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetMotionData){ .md-button .md-button--store target="_blank" }
 
### getRemotePlaySessionID

!!! function "getRemotePlaySessionID( ```uint64_t``` input_handle )"
	Get the Steam Remote Play session ID associated with a device, or 0 if there is no session associated with it. See isteamremoteplay.h for more information on Steam Remote Play sessions.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetRemotePlaySessionID){ .md-button .md-button--store target="_blank" }

### getSessionInputConfigurationSettings

!!! function "getSessionInputConfigurationSettings()"
	Get a bitmask of the Steam Input Configuration types opted in for the current session. Returns ESteamInputConfigurationEnableType values.	

	**Returns:** uint16

	**Note:** user can override the settings from the Steamworks Partner site so the returned values may not exactly match your default configuration.

### getStringforActionOrigin

!!! function "getStringforActionOrigin( ```int``` origin )"
	Returns a localized string (from Steam's language setting) for the specified origin.

	**Returns:** string.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetStringForActionOrigin){ .md-button .md-button--store target="_blank" }

### inputInit

!!! function "inputInit( ```bool``` explicitly_call_runframe )"
	[inputInit](#inputinit) and [inputShutdown](#inputshutdown) must be called when starting/ending use of this interface.

	If **explicitly_call_runframe** is called then you will need to manually call [runFrame](#runframe) each frame, otherwise Steam Input will updated when [run_callbacks](main.md#run_callbacks) is called.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#Init){ .md-button .md-button--store target="_blank" }

### inputShutdown

!!! function "inputShutdown()"
	[inputInit](#inputinit) and [inputShutdown](#inputshutdown) must be called when starting/ending use of this interface.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#Shutdown){ .md-button .md-button--store target="_blank" }

### runFrame

!!! function "runFrame()"
	Synchronize API state with the latest Steam Controller inputs available. This is performed automatically by [run_callbacks](main.md#run_callbacks), but for the absolute lowest possible latency, you can call this directly before reading controller state.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#RunFrame){ .md-button .md-button--store target="_blank" }
 
### setDualSenseTriggerEffect

!!! function "setDualSenseTriggerEffect( ```uint64_t``` input_handle, ```int``` parameter_index, ```int``` trigger_mask, ```int``` effect_mode, ```int``` position, ```int``` amplitude, ```int``` frequency )"
	Set the trigger effect for a DualSense controller.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#SetDualSenseTriggerEffect){ .md-button .md-button--store target="_blank" }

 
### setLEDColor

!!! function "setLEDColor( ```uint64_t``` input_handle, ```int``` color_r, ```int``` color_g, ```int``` color_b, ```int``` flags )"
	Set the controller LED color on supported controllers.

	**Returns:** void

	**Note:** The VSC does not support any color but white, and will interpret the RGB values as a greyscale value affecting the brightness of the Steam button LED. The DS4 responds to full color information and uses the values to set the color & brightness of the lightbar.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#SetLEDColor){ .md-button .md-button--store target="_blank" }
 
### showBindingPanel

!!! function "showBindingPanel( ```uint64_t``` input_handle )"
	Invokes the Steam overlay and brings up the binding screen.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#ShowBindingPanel){ .md-button .md-button--store target="_blank" }

### stopAnalogActionMomentum

!!! function "stopAnalogActionMomentum( ```uint64_t``` input_handle, ```uint64_t``` action )"
	Stops the momentum of an analog action (where applicable, ie a touchpad w/ virtual trackball settings).

	**Returns:** void

	**Note:** This will also stop all associated haptics. This is useful for situations where you want to indicate to the user that the limit of an action has been reached, such as spinning a carousel or scrolling a webpage.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#StopAnalogActionMomentum){ .md-button .md-button--store target="_blank" }

### translateActionOrigin

!!! function "translateActionOrigin( ```int``` destination_input, ```int``` source_origin )"
	Get the equivalent origin for a given controller type or the closest controller type that existed in the SDK you built into your game if **destination_input**is 0. This action origin can be used in your glyph look up table or passed into [getGlyphForActionOrigin](#getglyphforactionorigin) or [getStringForActionOrigin](#getstringforactionorigin).

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TranslateActionOrigin){ .md-button .md-button--store target="_blank" }

### triggerHapticPulse

!!! function "triggerHapticPulse( ```uint64_t``` input_handle, ```int``` target_pad, ```int``` duration )"
	Triggers a (low-level) haptic pulse on supported controllers.

	Currently only the VSC supports haptic pulses. This API call will be ignored for all other controller models. The typical max value of an unsigned short is 65535, which means the longest haptic pulse you can trigger with this method has a duration of 0.065535 seconds (ie, less than 1/10th of a second). This function should be thought of as a low-level primitive meant to be repeatedly used in higher-level user functions to generate more sophisticated behavior.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerHapticPulse){ .md-button .md-button--store target="_blank" }
 
### triggerRepeatedHapticPulse

!!! function "triggerRepeatedHapticPulse( ```uint64_t``` input_handle, ```int``` target_pad, ```int``` duration, ```int``` offset, ```int``` repeat, ```int``` flags )"
	Triggers a repeated haptic pulse on supported controllers.

	**Returns:** void

	**Note:** Currently only the VSC supports haptic pulses. This API call will be ignored for incompatible controller models. This is a more user-friendly function to call than TriggerHapticPulse as it can generate pulse patterns long enough to be actually noticed by the user. Changing the usDurationMicroSec and usOffMicroSec parameters will change the "texture" of the haptic pulse.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerRepeatedHapticPulse){ .md-button .md-button--store target="_blank" }

### triggerVibration

!!! function "triggerVibration( ```uint64_t``` input_handle, ```uint16_t``` left_speed, ```uint16_t``` right_speed )"
	Trigger a vibration event on supported controllers.

	**Returns:** void

	**Note:** This API call will be ignored for incompatible controller models. This generates the traditional "rumble" vibration effect. The VSC will emulate traditional rumble using its haptics.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerVibration){ .md-button .md-button--store target="_blank" }

### triggerVibrationExtended

!!! function "triggerVibrationExtended( ```uint64_t``` input_handle, ```uint16_t``` left_speed, ```uint16_t``` right_speed, ```uint16_t``` left_trigger_speed, ```uint16_t``` right_trigger_speed )"
	Trigger a vibration event on supported controllers including Xbox trigger impulse rumble - Steam will translate these commands into haptic pulses for Steam Controllers.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerVibrationExtended){ .md-button .md-button--store target="_blank" }

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### input_configuration_loaded

!!! function "input_configuration_loaded"
	Called when a controller configuration has been loaded, will fire once per controller per focus change for Steam Input enabled controllers.

	**Returns:**

	* app_id (uint32_t)
	* device_handle (uint64_t)
	* config_data (dictionary)
		* mapping_creator (uint64_t)
		* major_revision (uint32)
		* minor_revision (uint32)
		* uses_steam_input_api (bool)
		* uses_gamepad_api (bool)

### input_device_connected

!!! function "input_device_connected"
	Called when a new controller has been connected, will fire once per controller if multiple new controllers connect in the same frame.

	**Returns:**

	* input_handle (uint64_t)

### input_device_disconnected

!!! function "input_device_disconnected"
	Called when a new controller has been connected, will fire once per controller if multiple new controllers connect in the same frame.

	**Returns:**

	* input_handle (uint64_t)

### input_gamepad_slot_change

!!! function "input_gamepad_slot_change"
	Called when controller gamepad slots change. On Linux / macOS these slots are shared for all running apps.

	**Returns:**

	* app_id (uint32_t)
	* device_handle (uint64_t)
	* device_type (int)
	* old_gamepad_slot (int)
	* new_gamepad_slot (int)

---

## Enums

---

### InputActionOrigin

Enumerator | Value
---------- | -----
INPUT_ACTION_ORIGIN_NONE | 0
INPUT_ACTION_ORIGIN_A | 1
INPUT_ACTION_ORIGIN_B | 2
INPUT_ACTION_ORIGIN_X | 3
INPUT_ACTION_ORIGIN_Y | 4
INPUT_ACTION_ORIGIN_LEFT_BUMPER | 5
INPUT_ACTION_ORIGIN_RIGHT_BUMPER | 6
INPUT_ACTION_ORIGIN_LEFTGRIP | 7
INPUT_ACTION_ORIGIN_RIGHTGRIP | 8
INPUT_ACTION_ORIGIN_START | 9
INPUT_ACTION_ORIGIN_BACK | 10
INPUT_ACTION_ORIGIN_LEFT_PAD_TOUCH | 11
INPUT_ACTION_ORIGIN_LEFT_PAD_SWIPE | 12
INPUT_ACTION_ORIGIN_LEFT_PAD_CLICK | 13
INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_NORTH | 14
INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_SOUTH | 15
INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_WEST | 16
INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_EAST | 17
INPUT_ACTION_ORIGIN_RIGHT_PAD_TOUCH | 18
INPUT_ACTION_ORIGIN_RIGHT_PAD_SWIPE | 19
INPUT_ACTION_ORIGIN_RIGHT_PAD_CLICK | 20
INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_NORTH | 21
INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_SOUTH | 22
INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_WEST | 23
INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_EAST | 24
INPUT_ACTION_ORIGIN_LEFT_TRIGGER_PULL | 25
INPUT_ACTION_ORIGIN_LEFT_TRIGGER_CLICK | 26
INPUT_ACTION_ORIGIN_RIGHT_TRIGGER_PULL | 27
INPUT_ACTION_ORIGIN_RIGHT_TRIGGER_CLICK | 28
INPUT_ACTION_ORIGIN_LEFT_STICK_MOVE | 29
INPUT_ACTION_ORIGIN_LEFT_STICK_CLICK | 30
INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_NORTH | 31
INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_SOUTH | 32
INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_WEST | 33
INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_EAST | 34
INPUT_ACTION_ORIGIN_GYRO_MOVE | 35
INPUT_ACTION_ORIGIN_GYRO_PITCH | 36
INPUT_ACTION_ORIGIN_GYRO_YAW | 37
INPUT_ACTION_ORIGIN_GYRO_ROLL | 38
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED0 | 39
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED1 | 40
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED2 | 41
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED3 | 42
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED4 | 43
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED5 | 44
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED6 | 45
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED7 | 46
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED8 | 47
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED9 | 48
INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED10 | 49
INPUT_ACTION_ORIGIN_PS4_X | 50
INPUT_ACTION_ORIGIN_PS4_CIRCLE | 51
INPUT_ACTION_ORIGIN_PS4_TRIANGLE | 52
INPUT_ACTION_ORIGIN_PS4_SQUARE | 53
INPUT_ACTION_ORIGIN_PS4_LEFT_BUMPER | 54
INPUT_ACTION_ORIGIN_PS4_RIGHT_BUMPER | 55
INPUT_ACTION_ORIGIN_PS4_OPTIONS | 56
INPUT_ACTION_ORIGIN_PS4_SHARE | 57
INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_TOUCH | 58
INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_SWIPE | 59
INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_CLICK | 60
INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_NORTH | 61
INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_SOUTH | 62
INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_WEST | 63
INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_EAST | 64
INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_TOUCH | 65
INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_SWIPE | 66
INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_CLICK | 67
INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_NORTH | 68
INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_SOUTH | 69
INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_WEST | 70
INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_EAST | 71
INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_TOUCH | 72
INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_SWIPE | 73
INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_CLICK | 74
INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_NORTH | 75
INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_SOUTH | 76
INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_WEST | 77
INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_EAST | 78
INPUT_ACTION_ORIGIN_PS4_LEFT_TRIGGER_PULL | 79
INPUT_ACTION_ORIGIN_PS4_LEFT_TRIGGER_CLICK | 80
INPUT_ACTION_ORIGIN_PS4_RIGHT_TRIGGER_PULL | 81
INPUT_ACTION_ORIGIN_PS4_RIGHT_TRIGGER_CLICK | 82
INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_MOVE | 83
INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_CLICK | 84
INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_NORTH | 85
INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_SOUTH | 86
INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_WEST | 87
INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_EAST | 88
INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_MOVE | 89
INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_CLICK | 90
INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_NORTH | 91
INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_SOUTH | 92
INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_WEST | 93
INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_EAST | 94
INPUT_ACTION_ORIGIN_PS4_DPAD_NORTH | 95
INPUT_ACTION_ORIGIN_PS4_DPAD_SOUTH | 96
INPUT_ACTION_ORIGIN_PS4_DPAD_WEST | 97
INPUT_ACTION_ORIGIN_PS4_DPAD_EAST | 98
INPUT_ACTION_ORIGIN_PS4_GYRO_MOVE | 99
INPUT_ACTION_ORIGIN_PS4_GYRO_PITCH | 100
INPUT_ACTION_ORIGIN_PS4_GYRO_YAW | 101
INPUT_ACTION_ORIGIN_PS4_GYRO_ROLL | 102
INPUT_ACTION_ORIGIN_PS4_RESERVED0 | 103
INPUT_ACTION_ORIGIN_PS4_RESERVED1 | 104
INPUT_ACTION_ORIGIN_PS4_RESERVED2 | 105
INPUT_ACTION_ORIGIN_PS4_RESERVED3 | 106
INPUT_ACTION_ORIGIN_PS4_RESERVED4 | 107
INPUT_ACTION_ORIGIN_PS4_RESERVED5 | 108
INPUT_ACTION_ORIGIN_PS4_RESERVED6 | 109
INPUT_ACTION_ORIGIN_PS4_RESERVED7 | 110
INPUT_ACTION_ORIGIN_PS4_RESERVED8 | 111
INPUT_ACTION_ORIGIN_PS4_RESERVED9 | 112
INPUT_ACTION_ORIGIN_PS4_RESERVED10 | 113
INPUT_ACTION_ORIGIN_XBOX_ONE_A | 114
INPUT_ACTION_ORIGIN_XBOX_ONE_B | 115
INPUT_ACTION_ORIGIN_XBOX_ONE_X | 116
INPUT_ACTION_ORIGIN_XBOX_ONE_Y | 117
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_BUMPER | 118
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_BUMPER | 119
INPUT_ACTION_ORIGIN_XBOX_ONE_MENU | 120
INPUT_ACTION_ORIGIN_XBOX_ONE_VIEW | 121
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_TRIGGER_PULL | 122
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_TRIGGER_CLICK | 123
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_TRIGGER_PULL | 124
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_TRIGGER_CLICK | 125
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_MOVE | 126
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_CLICK | 127
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_NORTH | 128
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_SOUTH | 129
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_WEST | 130
INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_EAST | 131
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_MOVE | 132
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_CLICK | 133
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_NORTH | 134
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_SOUTH | 135
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_WEST | 136
INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_EAST | 137
INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_NORTH | 138
INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_SOUTH | 139
INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_WEST | 140
INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_EAST | 141
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED0 | 142
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED1 | 143
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED2 | 144
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED3 | 145
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED4 | 146
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED5 | 147
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED6 | 148
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED7 | 149
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED8 | 150
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED9 | 151
INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED10 | 152
INPUT_ACTION_ORIGIN_XBOX_360_A | 153
INPUT_ACTION_ORIGIN_XBOX_360_B | 154
INPUT_ACTION_ORIGIN_XBOX_360_X | 155
INPUT_ACTION_ORIGIN_XBOX_360_Y | 156
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_BUMPER | 157
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_BUMPER | 158
INPUT_ACTION_ORIGIN_XBOX_360_START | 159
INPUT_ACTION_ORIGIN_XBOX_360_BACK | 160
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_TRIGGER_PULL | 161
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_TRIGGER_CLICK | 162
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_TRIGGER_PULL | 163
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_TRIGGER_CLICK | 164
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_MOVE | 165
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_CLICK | 166
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_NORTH | 167
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_SOUTH | 168
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_WEST | 169
INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_EAST | 170
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_MOVE | 171
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_CLICK | 172
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_NORTH | 173
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_SOUTH | 174
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_WEST | 175
INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_EAST | 176
INPUT_ACTION_ORIGIN_XBOX_360_DPAD_NORTH | 177
INPUT_ACTION_ORIGIN_XBOX_360_DPAD_SOUTH | 178
INPUT_ACTION_ORIGIN_XBOX_360_DPAD_WEST | 179
INPUT_ACTION_ORIGIN_XBOX_360_DPAD_EAST | 180
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED0 | 181
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED1 | 182
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED2 | 183
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED3 | 184
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED4 | 185
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED5 | 186
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED6 | 187
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED7 | 188
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED8 | 189
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED9 | 190
INPUT_ACTION_ORIGIN_XBOX_360_RESERVED10 | 191
INPUT_ACTION_ORIGIN_SWITCH_A | 192
INPUT_ACTION_ORIGIN_SWITCH_B | 193
INPUT_ACTION_ORIGIN_SWITCH_X | 194
INPUT_ACTION_ORIGIN_SWITCH_Y | 195
INPUT_ACTION_ORIGIN_SWITCH_LEFT_BUMPER | 196
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_BUMPER | 197
INPUT_ACTION_ORIGIN_SWITCH_PLUS | 198
INPUT_ACTION_ORIGIN_SWITCH_MINUS | 199
INPUT_ACTION_ORIGIN_SWITCH_CAPTURE | 200
INPUT_ACTION_ORIGIN_SWITCH_LEFT_TRIGGER_PULL | 201
INPUT_ACTION_ORIGIN_SWITCH_LEFT_TRIGGER_CLICK | 202
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_TRIGGER_PULL | 203
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_TRIGGER_CLICK | 204
INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_MOVE | 205
INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_CLICK | 206
INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_NORTH | 207
INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_SOUTH | 208
INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_WEST | 209
INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_EAST | 210
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_MOVE | 211
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_CLICK | 212
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_NORTH | 213
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_SOUTH | 214
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_WEST | 215
INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_EAST | 216
INPUT_ACTION_ORIGIN_SWITCH_DPAD_NORTH | 217
INPUT_ACTION_ORIGIN_SWITCH_DPAD_SOUTH | 218
INPUT_ACTION_ORIGIN_SWITCH_DPAD_WEST | 219
INPUT_ACTION_ORIGIN_SWITCH_DPAD_EAST | 220
INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_MOVE | 221
INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_PITCH | 222
INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_YAW | 223
INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_ROLL | 224
INPUT_ACTION_ORIGIN_SWITCH_RESERVED0 | 225
INPUT_ACTION_ORIGIN_SWITCH_RESERVED1 | 226
INPUT_ACTION_ORIGIN_SWITCH_RESERVED2 | 227
INPUT_ACTION_ORIGIN_SWITCH_RESERVED3 | 228
INPUT_ACTION_ORIGIN_SWITCH_RESERVED4 | 229
INPUT_ACTION_ORIGIN_SWITCH_RESERVED5 | 230
INPUT_ACTION_ORIGIN_SWITCH_RESERVED6 | 231
INPUT_ACTION_ORIGIN_SWITCH_RESERVED7 | 232
INPUT_ACTION_ORIGIN_SWITCH_RESERVED8 | 233
INPUT_ACTION_ORIGIN_SWITCH_RESERVED9 | 234
INPUT_ACTION_ORIGIN_SWITCH_RESERVED10 | 235
INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_MOVE | 236
INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_PITCH | 237
INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_YAW | 238
INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_ROLL | 239
INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_MOVE | 240
INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_PITCH | 241
INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_YAW | 242
INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_ROLL | 243
INPUT_ACTION_ORIGIN_SWITCH_LEFTGRIP_LOWER | 244
INPUT_ACTION_ORIGIN_SWITCH_LEFTGRIP_UPPER | 245
INPUT_ACTION_ORIGIN_SWITCH_RIGHTGRIP_LOWER | 246
INPUT_ACTION_ORIGIN_SWITCH_RIGHTGRIP_UPPER | 247
INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_N | 248
INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_E | 249
INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_S | 250
INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_W | 251
INPUT_ACTION_ORIGIN_SWITCH_RESERVED15 | 252
INPUT_ACTION_ORIGIN_SWITCH_RESERVED16 | 253
INPUT_ACTION_ORIGIN_SWITCH_RESERVED17 | 254
INPUT_ACTION_ORIGIN_SWITCH_RESERVED18 | 255
INPUT_ACTION_ORIGIN_SWITCH_RESERVED19 | 256
INPUT_ACTION_ORIGIN_SWITCH_RESERVED20 | 257
INPUT_ACTION_ORIGIN_PS5_X | 258
INPUT_ACTION_ORIGIN_PS5_CIRCLE | 259
INPUT_ACTION_ORIGIN_PS5_TRIANGLE | 260
INPUT_ACTION_ORIGIN_PS5_SQUARE | 261
INPUT_ACTION_ORIGIN_PS5_LEFTBUMPER | 262
INPUT_ACTION_ORIGIN_PS5_RIGHTBUMPER | 263
INPUT_ACTION_ORIGIN_PS5_OPTION | 264
INPUT_ACTION_ORIGIN_PS5_CREATE | 265
INPUT_ACTION_ORIGIN_PS5_MUTE | 266
INPUT_ACTION_ORIGIN_PS5_LEFTPAD_TOUCH | 267
INPUT_ACTION_ORIGIN_PS5_LEFTPAD_SWIPE | 268
INPUT_ACTION_ORIGIN_PS5_LEFTPAD_CLICK | 269
INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADNORTH | 270
INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADSOUTH | 271
INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADWEST | 272
INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADEAST | 273
INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_TOUCH | 274
INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_SWIPE | 275
INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_CLICK | 276
INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADNORTH | 277
INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADSOUTH | 278
INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADWEST | 279
INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADEAST | 280
INPUT_ACTION_ORIGIN_PS5_CENTERPAD_TOUCH | 281
INPUT_ACTION_ORIGIN_PS5_CENTERPAD_SWIPE | 282
INPUT_ACTION_ORIGIN_PS5_CENTERPAD_CLICK | 283
INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADNORTH | 284
INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADSOUTH | 285
INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADWEST | 286
INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADEAST | 287
INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_PULL | 288
INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_CLICK | 289
INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_PULL | 290
INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_CLICK | 291
INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_MOVE | 292
INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_CLICK | 293
INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADNORTH | 294
INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADSOUTH | 295
INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADWEST | 296
INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADEAST | 297
INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_MOVE | 298
INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_CLICK | 299
INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADNORTH | 300
INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADSOUTH | 301
INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADWEST | 302
INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADEAST | 303
INPUT_ACTION_ORIGIN_PS5_DPAD_NORTH | 304
INPUT_ACTION_ORIGIN_PS5_DPAD_SOUTH | 305
INPUT_ACTION_ORIGIN_PS5_DPAD_WEST | 306
INPUT_ACTION_ORIGIN_PS5_DPAD_EAST | 307
INPUT_ACTION_ORIGIN_PS5_GYRO_MOVE | 308
INPUT_ACTION_ORIGIN_PS5_GYRO_PITCH | 309
INPUT_ACTION_ORIGIN_PS5_GYRO_YAW | 310
INPUT_ACTION_ORIGIN_PS5_GYRO_ROLL | 311
INPUT_ACTION_ORIGIN_PS5_DPAD_MOVE | 312
INPUT_ACTION_ORIGIN_PS5_LEFTGRIP | 313
INPUT_ACTION_ORIGIN_PS5_RIGHTGRIP | 314
INPUT_ACTION_ORIGIN_PS5_LEFTFN | 315
INPUT_ACTION_ORIGIN_PS5_RIGHTFN | 316
INPUT_ACTION_ORIGIN_PS5_RESERVED5 | 317
INPUT_ACTION_ORIGIN_PS5_RESERVED6 | 318
INPUT_ACTION_ORIGIN_PS5_RESERVED7 | 319
INPUT_ACTION_ORIGIN_PS5_RESERVED8 | 320
INPUT_ACTION_ORIGIN_PS5_RESERVED9 | 321
INPUT_ACTION_ORIGIN_PS5_RESERVED10 | 322
INPUT_ACTION_ORIGIN_PS5_RESERVED11 | 323
INPUT_ACTION_ORIGIN_PS5_RESERVED12 | 324
INPUT_ACTION_ORIGIN_PS5_RESERVED13 | 325
INPUT_ACTION_ORIGIN_PS5_RESERVED14 | 326
INPUT_ACTION_ORIGIN_PS5_RESERVED15 | 327
INPUT_ACTION_ORIGIN_PS5_RESERVED16 | 328
INPUT_ACTION_ORIGIN_PS5_RESERVED17 | 329
INPUT_ACTION_ORIGIN_PS5_RESERVED18 | 330
INPUT_ACTION_ORIGIN_PS5_RESERVED19 | 331
INPUT_ACTION_ORIGIN_PS5_RESERVED20 | 332
INPUT_ACTION_ORIGIN_STEAMDECK_A | 333
INPUT_ACTION_ORIGIN_STEAMDECK_B | 334
INPUT_ACTION_ORIGIN_STEAMDECK_X | 335
INPUT_ACTION_ORIGIN_STEAMDECK_Y | 336
INPUT_ACTION_ORIGIN_STEAMDECK_L1 | 337
INPUT_ACTION_ORIGIN_STEAMDECK_R1 | 338
INPUT_ACTION_ORIGIN_STEAMDECK_MENU | 339
INPUT_ACTION_ORIGIN_STEAMDECK_VIEW | 340
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_TOUCH | 341
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_SWIPE | 342
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_CLICK | 343
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADNORTH | 344
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADSOUTH | 345
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADWEST | 346
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADEAST | 347
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_TOUCH | 348
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_SWIPE | 349
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_CLICK | 350
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADNORTH | 351
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADSOUTH | 352
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADWEST | 353
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADEAST | 354
INPUT_ACTION_ORIGIN_STEAMDECK_L2_SOFTPULL | 355
INPUT_ACTION_ORIGIN_STEAMDECK_L2 | 356
INPUT_ACTION_ORIGIN_STEAMDECK_R2_SOFTPULL | 357
INPUT_ACTION_ORIGIN_STEAMDECK_R2 | 358
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_MOVE | 359
INPUT_ACTION_ORIGIN_STEAMDECK_L3 | 360
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADNORTH | 361
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADSOUTH | 362
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADWEST | 363
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADEAST | 364
INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_TOUCH | 365
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_MOVE | 366
INPUT_ACTION_ORIGIN_STEAMDECK_R3 | 367
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADNORTH | 368
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADSOUTH | 369
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADWEST | 370
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADEAST | 371
INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_TOUCH | 372
INPUT_ACTION_ORIGIN_STEAMDECK_L4 | 373
INPUT_ACTION_ORIGIN_STEAMDECK_R4 | 374
INPUT_ACTION_ORIGIN_STEAMDECK_L5 | 375
INPUT_ACTION_ORIGIN_STEAMDECK_R5 | 376
INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_MOVE | 377
INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_NORTH | 378
INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_SOUTH | 379
INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_WEST | 380
INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_EAST | 381
INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_MOVE | 382
INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_PITCH | 383
INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_YAW | 384
INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_ROLL | 385
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED1 | 386
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED2 | 387
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED3 | 388
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED4 | 389
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED5 | 390
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED6 | 391
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED7 | 392
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED8 | 393
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED9 | 394
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED10 | 395
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED11 | 396
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED12 | 397
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED13 | 398
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED14 | 399
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED15 | 400
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED16 | 401
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED17 | 402
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED18 | 403
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED19 | 404
INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED20 | 405
INPUT_ACTION_ORIGIN_COUNT | 406
INPUT_ACTION_ORIGIN_MAXIMUMPOSSIBLEVALUE | 32767

### SteamInputType

Enumerator | Value
---------- | -----
INPUT_TYPE_UNKNOWN | 0
INPUT_TYPE_STEAM_CONTROLLER | 1
INPUT_TYPE_XBOX360_CONTROLLER | 2
INPUT_TYPE_XBOXONE_CONTROLLER | 3
INPUT_TYPE_GENERIC_XINPUT | 4
INPUT_TYPE_PS4_CONTROLLER | 5
INPUT_TYPE_APPLE_MFI_CONTROLLER | 6
INPUT_TYPE_ANDROID_CONTROLLER | 7
INPUT_TYPE_SWITCH_JOYCON_PAIR | 8
INPUT_TYPE_SWITCH_JOYCON_SINGLE | 9
INPUT_TYPE_SWITCH_PRO_CONTROLLER | 10
INPUT_TYPE_MOBILE_TOUCH | 11
INPUT_TYPE_PS3_CONTROLLER | 12
INPUT_TYPE_PS5_CONTROLLER | 13
INPUT_TYPE_STEAM_DECK_CONTROLLER | 14
INPUT_TYPE_COUNT | 15
INPUT_TYPE_MAXIMUM_POSSIBLE_VALUE | 255

### ConfigurationEnableType

Enumerator | Value
---------- | -----
INPUT_CONFIGURATION_ENABLE_TYPE_NONE | 0x0000
INPUT_CONFIGURATION_ENABLE_TYPE_PLAYSTATION | 0x0001
INPUT_CONFIGURATION_ENABLE_TYPE_XBOX | 0x0002
INPUT_CONFIGURATION_ENABLE_TYPE_GENERIC | 0x0004
INPUT_CONFIGURATION_ENABLE_TYPE_SWITCH | 0x0008

### GlyphSize

Enumerator | Value
---------- | -----
INPUT_GLYPH_SIZE_SMALL | 0
INPUT_GLYPH_SIZE_MEDIUM | 1
INPUT_GLYPH_SIZE_LARGE | 2
INPUT_GLYPH_SIZE_COUNT | 3

### GlyphStyle

Enumerator | Value
---------- | -----
INPUT_GLYPH_STYLE_KNOCKOUT | 0x0
INPUT_GLYPH_STYLE_LIGHT | 0x1
INPUT_GLYPH_STYLE_DARK | 0x2
INPUT_GLYPH_STYLE_NEUTRAL_COLOR_ABXY | 0x10
INPUT_GLYPH_STYLE_SOLID_ABXY | 0x20

### SCEPadTriggerEffectMode

Enumerator | Value
---------- | -----
PAD_TRIGGER_EFFECT_MODE_OFF | 0
PAD_TRIGGER_EFFECT_MODE_FEEDBACK | 1
PAD_TRIGGER_EFFECT_MODE_WEAPON | 2
PAD_TRIGGER_EFFECT_MODE_VIBRATION | 3
PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_FEEDBACK | 4
PAD_TRIGGER_EFFECT_MODE_SLOPE_FEEDBACK | 5
PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_VIBRATION | 6
