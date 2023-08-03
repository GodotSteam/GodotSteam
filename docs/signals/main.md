# Signals - Main

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## steamworks_error

!!! function "steamworks_error"
	Intended to serve as generic error messaging for failed call results.
	
	**Returns:**

	* failed_signal (string)
	* message (string)

	**Note:** this callback is unique to GodotSteam.