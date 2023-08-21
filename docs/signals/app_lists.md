# Signals - App Lists

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

------

## app_installed

!!! function "app_installed"
	Sent when a new app is installed.
	
	**Returns:**

	* app_id (uint32_t)
	* install_folder_index (uint32_t)

## app_uninstalled

!!! function "app_uninstalled"
	Sent when an app is uninstalled.

	**Returns:**
	
	* app_id (uint32_t)
	* install_folder_index (uint32_t)
