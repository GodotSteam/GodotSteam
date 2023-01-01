# Signals - Parties

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## active_beacons_updated

!!! function "active_beacons_updated"
	Notification that the list of active beacons visible to the current user has changed.
	
	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#ActiveBeaconsUpdated_t){ .md-button .md-button--store target="_blank" }

## available_beacon_locations_updated

!!! function "available_beacon_locations_updated"
	Notification that the list of available locations for posting a beacon has been updated. 
	
	**Returns:** nothing

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#AvailableBeaconLocationsUpdated_t){ .md-button .md-button--store target="_blank" }

## change_num_open_slots

!!! function "change_num_open_slots"
	Call result for [changeNumOpenSlots](/functions/parties/#changenumopenslots).

	**Returns:**

	* result (int)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#ChangeNumOpenSlotsCallback_t){ .md-button .md-button--store target="_blank" }

## create_beacon

!!! function "create_beacon"
	This callback is used as a call response for [createBeacon](/functions/parties/#createbeacon). If successful, your beacon has been posted in the desired location and you may start receiving [reservation_notification](/signals/parties/#reservation_notification) callbacks for users following the beacon.

	**Returns:**

	* result (int)
	* beacon_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#CreateBeaconCallback_t){ .md-button .md-button--store target="_blank" }

## join_party

!!! function "join_party"
	This callback is used as a call response for [joinParty](/functions/parties/#joinparty). On success, you will have reserved a slot in the beacon-owner's party, and should use connect_string to connect to their game and complete the process.

	**Returns:**

	* result (int)
	* beacon_id (uint64_t)
	* steam_id (uint64_t)
	* connect_string (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#JoinPartyCallback_t){ .md-button .md-button--store target="_blank" }

## reservation_notification

!!! function "reservation_notification"
	After creating a beacon, when a user "follows" that beacon Steam will send you this callback to know that you should be prepared for the user to join your game. When they do join, be sure to call [onReservationCompleted](/functions/parties/#onreservationcompleted) to let Steam know.

	**Returns:**

	* beacon_id (uint64_t)
	* steam_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#ReservationNotificationCallback_t){ .md-button .md-button--store target="_blank" }