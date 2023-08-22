# Parental Settings

---

## Functions

---

### isAppBlocked

!!! function "isAppBlocked( ```uint32``` app_id )"
	Check if the given app is blocked by parental settings.

	**Returns:** bool

### isAppInBlockList

!!! function "isAppInBlockList( ```uint32``` app_id )"
	Check if this app is in the block list.

	**Returns:** bool

### isFeatureBlocked

!!! function "isFeatureBlocked( ```int``` feature )"
	Check if the given feature is blocked by parental settings.

	**Returns:** bool

### isFeatureInBlockList

!!! function "isFeatureInBlockList( ```int``` feature )"
	Check if the given feature is in the block list.

	**Returns:** bool

### isParentalLockEnabled

!!! function "isParentalLockEnabled()"
	Check if the parental lock is enabled.

	**Returns:** bool

### isParentalLockLocked

!!! function "isParentalLockLocked()"
	Check if the parental lock is actually locked.

	**Returns:** bool

---

## Signals

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

### parental_setting_changed

!!! function "parental_setting_changed"
	Callback for querying UGC.
	
	**Returns:** nothing

---

## Enums

---

### ParentalFeature

Enumerator | Value
---------- | -----
FEATURE_INVALID | 0
FEATURE_STORE | 1
FEATURE_COMMUNITY | 2
FEATURE_PROFILE | 3
FEATURE_FRIENDS | 4
FEATURE_NEWS | 5
FEATURE_TRADING | 6
FEATURE_SETTINGS | 7
FEATURE_CONSOLE | 8
FEATURE_BROWSER | 9
FEATURE_PARENTAL_SETUP | 10
FEATURE_LIBRARY | 11
FEATURE_TEST | 12
FEATURE_SITE_LICENSE | 13
FEATURE_KIOSK_MODE | 14
FEATURE_MAX | 
