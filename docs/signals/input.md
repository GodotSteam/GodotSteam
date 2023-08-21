# Signals - Input

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## input_configuration_loaded

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

## input_device_connected

!!! function "input_device_connected"
	Called when a new controller has been connected, will fire once per controller if multiple new controllers connect in the same frame.

	**Returns:**

	* input_handle (uint64_t)

## input_device_disconnected

!!! function "input_device_disconnected"
	Called when a new controller has been connected, will fire once per controller if multiple new controllers connect in the same frame.

	**Returns:**

	* input_handle (uint64_t)

## input_gamepad_slot_change

!!! function "input_gamepad_slot_change"
	Called when controller gamepad slots change. On Linux / macOS these slots are shared for all running apps.

	**Returns:**

	* app_id (uint32_t)
	* device_handle (uint64_t)
	* device_type (int)
	* old_gamepad_slot (int)
	* new_gamepad_slot (int)
