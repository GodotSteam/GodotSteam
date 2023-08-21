# Signals - Networking Utilities

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## relay_network_status

!!! function "relay_network_status"
	A struct used to describe our readiness to use the relay network.

	**Returns:**

	* available (int)
	* ping_measurement (int)
	* available_config (int)
	* available_relay (int)
	* debug_message (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/steamnetworkingtypes#SteamRelayNetworkStatus_t){ .md-button .md-button--store target="_blank" }
