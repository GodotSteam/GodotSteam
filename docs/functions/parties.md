# Functions - Parties

---

## cancelReservation

!!! function "cancelReservation( ```uint64_t``` beacon_id, ```uint64_t``` steam_id )"
	To cancel a reservation (due to timeout or user input), call this. Steam will open a new reservation slot.

	**Returns:** void

	**Note:** The user may already be in-flight to your game, so it's possible they will still connect and try to join your party.

## changeNumOpenSlots

!!! function "changeNumOpenSlots( ```uint64_t``` beacon_id, ```uint32``` open_slots )"
	If a user joins your party through other matchmaking (perhaps a direct Steam friend, or your own matchmaking system), your game should reduce the number of open slots that Steam is managing through the party beacon. For example, if you created a beacon with five slots, and Steam sent you two [reservation_notification](/signals/parties/#reservation_notification) callbacks, and then a third user joined directly, you would want to call [changeNumOpenSlots](/functions/parties/#changenumopenslots) with a value of 2 for open_slots. That value represents the total number of new users that you would like Steam to send to your party.

	Triggers a [change_num_open_slots](/signals/parties/#change_num_open_slots) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#ChangeNumOpenSlots){ .md-button .md-button--store target="_blank" }

## createBeacon

!!! function "createBeacon( ```uint32``` open_slots, ```uint64_t``` location_id, ```int``` type, ```string``` connect_string, ```string``` metadata )"
	Create a beacon. You can only create one beacon at a time. Steam will display the beacon in the specified location, and let up to open_slots users "follow" the beacon to your party.
	
	If users join your party through other matchmaking, adjust the number of remaining open slots using [changeNumOpenSlots](/functions/parties/#changenumopenslots).
	
	Triggers a [create_beacon](/signals/parties/#create_beacon) call result.
	
	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#CreateBeacon){ .md-button .md-button--store target="_blank" }

## destroyBeacon

!!! function "destroyBeacon( ```uint6_t``` beacon_id )"
	Call this method to destroy the Steam party beacon. This will immediately cause Steam to stop showing the beacon in the target location. Note that any users currently in-flight may still arrive at your party expecting to join.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#DestroyBeacon){ .md-button .md-button--store target="_blank" }

## getAvailableBeaconLocations

!!! function "getAvailableBeaconLocations( ```uint32``` max )"
	Get the list of locations in which you can post a party beacon.

	**Returns:** array

	Contains a list of:

	* beacon_data (dictionary)
		* type (int)
		* location_id (uint64_t)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#GetAvailableBeaconLocations){ .md-button .md-button--store target="_blank" }

## getBeaconByIndex

!!! function "getBeaconByIndex( ```uint32``` index )"
	Use with [getNumActiveBeacons](/functions/parties#getnumactivebeacons) to iterate the active beacons visible to the current user. Argument [index is a zero-based index, so iterate over the range [0, getNumActiveBeacons() - 1]. The return is a beacon_id that can be used with [getBeaconDetails](/functions/parties/#getbeacondetails) to get information about the beacons suitable for display to the user.

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#GetBeaconByIndex){ .md-button .md-button--store target="_blank" }

## getBeaconDetails

!!! function "getBeaconDetails( ```uint64_t``` beacon_id )"
	Get details about the specified beacon. You can use the Friends API to get further details about owner_id, and [getBeaconLocationData](/functions/parties/#getbeaconlocationdata) to get further details about location_id. The metadata contents are specific to your game, and will be whatever was set (if anything) by the game process that created the beacon.

	**Returns:** dictionary

	Contains the following keys:

	* beacon_id (uint64_t)
	* owner_id (uint64_t)
	* type (int)
	* location_id (uint64_t)
	* metadata (string)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#GetBeaconDetails){ .md-button .md-button--store target="_blank" }

## getBeaconLocationData

!!! function "getBeaconLocationData( ```int``` location_id, ```int``` location_type, ```int``` location_data )"
	Query general metadata for the given beacon location. For instance the Name, or the URL for an icon if the location type supports icons (for example, the icon for a Steam Chat Room Group).
	
	**Returns:** string

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#GetBeaconLocationData){ .md-button .md-button--store target="_blank" }

## getNumActiveBeacons

!!! function "getNumActiveBeacons()"
	Get the number of active party beacons created by other users for your game, that are visible to the current user.

	**Returns:** uint32

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#GetNumActiveBeacons){ .md-button .md-button--store target="_blank" }

## joinParty

!!! function "joinParty( ```uint64_t``` beacon_id )"
	When the user indicates they wish to join the party advertised by a given beacon, call this method. On success, Steam will reserve a slot for this user in the party and return the necessary "join game" string to use to complete the connection.

	Triggers a [join_party](/signals/parties/#join_party) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#JoinParty){ .md-button .md-button--store target="_blank" }

## onReservationCompleted

!!! function "onReservationCompleted( ```uint64_t``` beacon_id, ```uint64_t``` steam_id )"
	When a user follows your beacon, Steam will reserve one of the open party slots for them, and send your game a [reservation_notification](/signals/parties/#reservation_notification) callback. When that user joins your party, call [onReservationCompleted](/functions/parties/#onreservationcompleted) to notify Steam that the user has joined successfully.

	Triggers a [reservation_notification](/signals/parties/#reservation_notification) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/isteamparties#OnReservationCompleted){ .md-button .md-button--store target="_blank" }