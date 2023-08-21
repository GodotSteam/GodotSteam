# Functions - Input

---

## activateActionSet

!!! function "activateActionSet( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle )"
	Reconfigure the controller to use the specified action set (i.e. "Menu", "Walk", or "Drive").

	This is cheap, and can be safely called repeatedly. It's often easier to repeatedly call it in your state loops, instead of trying to place it in all of your state transitions.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#ActivateActionSet){ .md-button .md-button--store target="_blank" }

## activateActionSetLayer

!!! function "activateActionSetLayer( ```uint64_t``` input_handle, ```uint64_t``` action_set_layer_handle )"
	Reconfigure the controller to use the specified action set layer.

	See the [Action Set Layers](https://partner.steamgames.com/doc/features/steam_controller/action_set_layers){ target="_blank" } article for full details and an in-depth practical example.

	**Returns:** void
 
    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#ActivateActionSetLayer){ .md-button .md-button--store target="_blank" }

## deactivateActionSetLayer

!!! function "deactivateActionSetLayer( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle )"
	Reconfigure the controller to stop using the specified action set.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#DeactivateActionSetLayer){ .md-button .md-button--store target="_blank" }

 
## deactivateAllActionSetLayers

!!! function "deactivateAllActionSetLayers( ```uint64_t``` input_handle )"
	Reconfigure the controller to stop using all action set layers.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#DeactivateAllActionSetLayers){ .md-button .md-button--store target="_blank" }

## getActiveActionSetLayers

!!! function "getActiveActionSetLayers( ```uint64_t``` input_handle)"
	Fill an array with all of the currently active action set layers for a specified controller handle.

	**Returns:** array

	Contains a list of handles (uint64_t).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetActiveActionSetLayers){ .md-button .md-button--store target="_blank" }

## getActionSetHandle

!!! function "getActionSetHandle( ```string``` action_set_name )"
	Fill an array with all of the currently active action set layers for a specified controller handle.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetActiveActionSetLayers){ .md-button .md-button--store target="_blank" }
 
## getActionOriginFromXboxOrigin

!!! function "getActionOriginFromXboxOrigin( ```uint64_t``` input_handle, ```int``` origin )"
	Get an action origin that you can use in your glyph look up table or passed into [getGlyphForActionOrigin](/functions/input/#getglyphforactionorigin) or [getStringForActionOrigin](/functions/input/#getstringforactionorigin).

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetActionOriginFromXboxOrigin){ .md-button .md-button--store target="_blank" }

## getAnalogActionData

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

## getAnalogActionHandle

!!! function "getAnalogActionHandle( ```string``` action_name )"
	Get the handle of the specified analog action.

	**Returns:** uint64_t

	**Note:** This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two different action sets, this function will only ever return one of them and the other will be ignored.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetAnalogActionHandle){ .md-button .md-button--store target="_blank" }

## getAnalogActionOrigins

!!! function "getAnalogActionOrigins( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle, ```uint64_t``` analog_action_handle )"
	Get the origin(s) for an analog action within an action set by filling the return array with handles. Use this to display the appropriate on-screen prompt for the action.

	**Returns:** array

	Contains a list of handles (int).

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetAnalogActionOrigins){ .md-button .md-button--store target="_blank" }

## getConnectedControllers

!!! function "getConnectedControllers()"
	Enumerates currently connected controllers by filling returned array with controller handles.

	**Returns:** array

	Contains a list of controller handle (uint64_t).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetConnectedControllers){ .md-button .md-button--store target="_blank" }

## getControllerForGamepadIndex

!!! function "getControllerForGamepadIndex( ```int``` index )"
	Returns the associated controller handle for the specified emulated gamepad. Can be used with [getInputTypeForHandle](/functions/input/#getinputtypeforhandle) to determine the type of controller using Steam Input Gamepad Emulation.

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetControllerForGamepadIndex){ .md-button .md-button--store target="_blank" }
 
## getCurrentActionSet

!!! function "getCurrentActionSet( ```uint64_t``` input_handle )"
	Get the currently active action set for the specified controller.

	**Returns:** uint64_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetCurrentActionSet){ .md-button .md-button--store target="_blank" }

## getDeviceBindingRevision

!!! function "getDeviceBindingRevision( ```uint64_t``` input_handle )"
	Get's the major and minor device binding revisions for Steam Input API configurations. Minor revisions are for small changes such as adding a new option action or updating localization in the configuration. When updating a Minor revision only one new configuration needs to be update with the "Use Action Block" flag set. Major revisions are to be used when changing the number of action sets or otherwise reworking configurations to the degree that older configurations are no longer usable. When a user's binding disagree's with the major revision of the current official configuration Steam will forcibly update the user to the new configuration. New configurations will need to be made for every controller when updating the Major revision.

	**Returns:** array

	Contains a list of:

	* major (int)
	* minor (int)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDeviceBindingRevision){ .md-button .md-button--store target="_blank" }

## getDigitalActionData

!!! function "getDigitalActionData( ```uint64_t``` input_handle, ```uint64_t``` digital_action_handle )"
	Returns the current state of the supplied digital game action.

	**Returns:** dictionary 

	* state (bool)
	* active (bool)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDigitalActionData){ .md-button .md-button--store target="_blank" }

## getDigitalActionHandle

!!! function "getDigitalActionHandle( ```string``` action_name )"
	Get the handle of the specified digital action.

	**Returns:** uint64_t

	**Note:** This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two different action sets, this function will only ever return one of them and the other will be ignored.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDigitalActionHandle){ .md-button .md-button--store target="_blank" }
	
 
## getDigitalActionOrigins

!!! function "getDigitalActionOrigins( ```uint64_t``` input_handle, ```uint64_t``` action_set_handle, ```uint64_t``` digital_action_handle )"

	Get the origin(s) for a digital action within an action set by filling return array with input handles. Use this to display the appropriate on-screen prompt for the action.

	**Returns:** array

	Contains a list of handles (int).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetDigitalActionOrigins){ .md-button .md-button--store target="_blank" }

## getGamepadIndexForController

!!! function "getGamepadIndexForController( ```uint64_t``` input_handle )"
	Returns the associated gamepad index for the specified controller, if emulating a gamepad.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetGamepadIndexForController){ .md-button .md-button--store target="_blank" }

## getGlyphForActionOrigin

!!! function "getGlyphForActionOrigin( ```int``` origin )"
	Get a local path to art for on-screen glyph for a particular origin.

	**Returns:** string.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetGlyphForActionOrigin){ .md-button .md-button--store target="_blank" }

## getInputTypeForHandle

!!! function "getInputTypeForHandle( ```uint64_t``` input_handle )"
	Returns the input type (device model) for the specified controller. This tells you if a given controller is a Steam controller, Xbox 360 controller, PS4 controller, etc.

	**Returns:** string.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetInputTypeForHandle){ .md-button .md-button--store target="_blank" }
 
## getMotionData

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
 
## getRemotePlaySessionID

!!! function "getRemotePlaySessionID( ```uint64_t``` input_handle )"
	Get the Steam Remote Play session ID associated with a device, or 0 if there is no session associated with it. See isteamremoteplay.h for more information on Steam Remote Play sessions.

	**Returns:** int

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetRemotePlaySessionID){ .md-button .md-button--store target="_blank" }

## getSessionInputConfigurationSettings

!!! function "getSessionInputConfigurationSettings()"
	Get a bitmask of the Steam Input Configuration types opted in for the current session. Returns ESteamInputConfigurationEnableType values.	

	**Returns:** uint16

	**Note:** user can override the settings from the Steamworks Partner site so the returned values may not exactly match your default configuration.

## getStringforActionOrigin

!!! function "getStringforActionOrigin( ```int``` origin )"
	Returns a localized string (from Steam's language setting) for the specified origin.

	**Returns:** string.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#GetStringForActionOrigin){ .md-button .md-button--store target="_blank" }

## inputInit

!!! function "inputInit( ```bool``` explicitly_call_runframe )"
	[inputInit](/functions/input/#inputinit) and [inputShutdown](/functions/input/#inputshutdown) must be called when starting/ending use of this interface.

	If **explicitly_call_runframe**is called then you will need to manually call [runFrame](/functions/input/#runframe) each frame, otherwise Steam Input will updated when [run_callbacks](/functions/main/#run_callbacks) is called.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#Init){ .md-button .md-button--store target="_blank" }

## inputShutdown

!!! function "inputShutdown()"
	[inputInit](/functions/input/#inputinit) and [inputShutdown](/functions/input/#inputshutdown) must be called when starting/ending use of this interface.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#Shutdown){ .md-button .md-button--store target="_blank" }

## runFrame

!!! function "runFrame()"
	Synchronize API state with the latest Steam Controller inputs available. This is performed automatically by [run_callbacks](/functions/main/#run_callbacks), but for the absolute lowest possible latency, you can call this directly before reading controller state.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#RunFrame){ .md-button .md-button--store target="_blank" }
 
## setDualSenseTriggerEffect

!!! function "setDualSenseTriggerEffect( ```uint64_t``` input_handle, ```int``` parameter_index, ```int``` trigger_mask, ```int``` effect_mode, ```int``` position, ```int``` amplitude, ```int``` frequency )"
	Set the trigger effect for a DualSense controller.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#SetDualSenseTriggerEffect){ .md-button .md-button--store target="_blank" }

 
## setLEDColor

!!! function "setLEDColor( ```uint64_t``` input_handle, ```int``` color_r, ```int``` color_g, ```int``` color_b, ```int``` flags )"
	Set the controller LED color on supported controllers.

	**Returns:** void

	**Note:** The VSC does not support any color but white, and will interpret the RGB values as a greyscale value affecting the brightness of the Steam button LED. The DS4 responds to full color information and uses the values to set the color & brightness of the lightbar.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#SetLEDColor){ .md-button .md-button--store target="_blank" }
 
## showBindingPanel

!!! function "showBindingPanel( ```uint64_t``` input_handle )"
	Invokes the Steam overlay and brings up the binding screen.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#ShowBindingPanel){ .md-button .md-button--store target="_blank" }

## stopAnalogActionMomentum

!!! function "stopAnalogActionMomentum( ```uint64_t``` input_handle, ```uint64_t``` action )"
	Stops the momentum of an analog action (where applicable, ie a touchpad w/ virtual trackball settings).

	**Returns:** void

	**Note:** This will also stop all associated haptics. This is useful for situations where you want to indicate to the user that the limit of an action has been reached, such as spinning a carousel or scrolling a webpage.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#StopAnalogActionMomentum){ .md-button .md-button--store target="_blank" }

## translateActionOrigin

!!! function "translateActionOrigin( ```int``` destination_input, ```int``` source_origin )"
	Get the equivalent origin for a given controller type or the closest controller type that existed in the SDK you built into your game if **destination_input**is 0. This action origin can be used in your glyph look up table or passed into [getGlyphForActionOrigin](/functions/input/#getglyphforactionorigin) or [getStringForActionOrigin](/functions/input/#getstringforactionorigin).

	**Returns:** int

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TranslateActionOrigin){ .md-button .md-button--store target="_blank" }

## triggerHapticPulse

!!! function "triggerHapticPulse( ```uint64_t``` input_handle, ```int``` target_pad, ```int``` duration )"
	Triggers a (low-level) haptic pulse on supported controllers.

	Currently only the VSC supports haptic pulses. This API call will be ignored for all other controller models. The typical max value of an unsigned short is 65535, which means the longest haptic pulse you can trigger with this method has a duration of 0.065535 seconds (ie, less than 1/10th of a second). This function should be thought of as a low-level primitive meant to be repeatedly used in higher-level user functions to generate more sophisticated behavior.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerHapticPulse){ .md-button .md-button--store target="_blank" }
 
## triggerRepeatedHapticPulse

!!! function "triggerRepeatedHapticPulse( ```uint64_t``` input_handle, ```int``` target_pad, ```int``` duration, ```int``` offset, ```int``` repeat, ```int``` flags )"
	Triggers a repeated haptic pulse on supported controllers.

	**Returns:** void

	**Note:** Currently only the VSC supports haptic pulses. This API call will be ignored for incompatible controller models. This is a more user-friendly function to call than TriggerHapticPulse as it can generate pulse patterns long enough to be actually noticed by the user. Changing the usDurationMicroSec and usOffMicroSec parameters will change the "texture" of the haptic pulse.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerRepeatedHapticPulse){ .md-button .md-button--store target="_blank" }

## triggerVibration

!!! function "triggerVibration( ```uint64_t``` input_handle, ```uint16_t``` left_speed, ```uint16_t``` right_speed )"
	Trigger a vibration event on supported controllers.

	**Returns:** void

	**Note:** This API call will be ignored for incompatible controller models. This generates the traditional "rumble" vibration effect. The VSC will emulate traditional rumble using its haptics.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerVibration){ .md-button .md-button--store target="_blank" }

## triggerVibrationExtended

!!! function "triggerVibrationExtended( ```uint64_t``` input_handle, ```uint16_t``` left_speed, ```uint16_t``` right_speed, ```uint16_t``` left_trigger_speed, ```uint16_t``` right_trigger_speed )"
	Trigger a vibration event on supported controllers including Xbox trigger impulse rumble - Steam will translate these commands into haptic pulses for Steam Controllers.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteaminput#TriggerVibrationExtended){ .md-button .md-button--store target="_blank" }
