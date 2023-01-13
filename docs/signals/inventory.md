# Signals - Inventory

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

---

## inventory_definition_update

!!! function "inventory_definition_update"
	This callback is triggered whenever item definitions have been updated, which could be in response to [loadItemDefinitions](/functions/inventory/#loaditemdefinitions) or any time new item definitions are available (eg, from the dynamic addition of new item types while players are still in-game).

	**Returns:**

	* definitions (array)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamInventory#SteamInventoryDefinitionUpdate_t){ .md-button .md-button--store target="_blank" }

## inventory_eligible_promo_item

!!! function "inventory_eligible_promo_item"
	Returned when you have requested the list of "eligible" promo items that can be manually granted to the given user. These are promo items of type "manual" that won't be granted automatically.

	**Returns:**

	* result (int)
	* cached (bool)
	* definitions (array)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamInventory#SteamInventoryEligiblePromoItemDefIDs_t){ .md-button .md-button--store target="_blank" }

## inventory_full_update

!!! function "inventory_full_update"
	Triggered when [getAllItems](/functions/inventory/#getallitems) successfully returns a result which is newer / fresher than the last known result. (It will not trigger if the inventory hasn't changed, or if results from two overlapping calls are reversed in flight and the earlier result is already known to be stale/out-of-date.) The regular [inventory_result_ready](/signals/inventory/#inventory_result_ready) callback will still be triggered immediately afterwards; this is an additional notification for your convenience.

	**Returns:**

	* inventory_handle (int32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamInventory#SteamInventoryFullUpdate_t){ .md-button .md-button--store target="_blank" }

## inventory_request_prices_result

!!! function "inventory_request_prices_result"
	Returned after [requestPrices](/functions/inventory/#requestprices) is called.

	**Returns:**

	* result (int)
	* currency (string)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamInventory#SteamInventoryRequestPricesResult_t){ .md-button .md-button--store target="_blank" }

## inventory_result_ready

!!! function "inventory_result_ready"
	This is fired whenever an inventory result transitions from k_EResultPending to any other completed state, see [getResultStatus](/functions/inventory/#getresultstatus) for the complete list of states. There will always be exactly one callback per handle.

	**Returns:**

	* result (int)
	* inventory_handle (int32)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamInventory#SteamInventoryResultReady_t){ .md-button .md-button--store target="_blank" }

## inventory_start_purchase_result

!!! function "inventory_start_purchase_result"
	Returned after [startPurchase](/functions/inventory/#startpurchase) is called.

	**Returns:**

	* result (string)
	* order_id (uint64_t)
	* transaction_id (uint64_t)

	---
	[:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamInventory#SteamInventoryStartPurchaseResult_t){ .md-button .md-button--store target="_blank" }