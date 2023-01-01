# Signals - UGC

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

------

## add_app_dependency_result

!!! function "add_app_dependency_result"
	The result of a call to [addAppDependency](/functions/ugc/#addappdependency).

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* app_id (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddAppDependencyResult_t){ .md-button .md-button--store target="_blank" }	

## add_ugc_dependency_result

!!! function "add_ugc_dependency_result"
	The result of a call to AddDependency.

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* child_id (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddUGCDependencyResult_t){ .md-button .md-button--store target="_blank" }

## get_app_dependencies_result

!!! function "get_app_dependencies_result"
	Called when getting the app dependencies for an item.

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* app_dependencies (uint32)
	* total_app_dependencies (uint32)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetAppDependenciesResult_t){ .md-button .md-button--store target="_blank" }

## get_item_vote_result

!!! function "get_item_vote_result"
	Called when getting the users vote status on an item.

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* vote_up (bool)
	* vote_down (bool)
	* vote_skipped (bool)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetUserItemVoteResult_t){ .md-button .md-button--store target="_blank" }

## item_created

!!! function "item_created"
	Called when a new workshop item has been created.

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* accept_tos (bool)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#CreateItemResult_t){ .md-button .md-button--store target="_blank" }

## item_deleted

!!! function "item_deleted"
	Called when an attempt at deleting an item completes.

	**Returns:**

	* result (int)
	* file_id (uint64_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#DeleteItemResult_t){ .md-button .md-button--store target="_blank" }

## item_downloaded

!!! function "item_downloaded"
	Called when a workshop item has been downloaded.

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* app_id (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#DownloadItemResult_t){ .md-button .md-button--store target="_blank" }

## item_installed

!!! function "item_installed"
	Called when a workshop item has been installed or updated.

	**Returns:**

	* app_id (uint32_t)
	* file_id (uint64_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#ItemInstalled_t){ .md-button .md-button--store target="_blank" }

## item_updated

!!! function "item_updated"
	Called when an item update has completed.
	Emits signal in response to function [submitItemUpdate](/functions/ugc/#submititemupdate).

	**Returns:**

	* result (int)
	* accept_tos (bool)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SubmitItemUpdateResult_t){ .md-button .md-button--store target="_blank" }

## remove_app_dependency_result

!!! function "remove_app_dependency_result"
	Purpose: The result of a call to [removeAppDependency](/functions/ugc/#removeappdependency).

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* app_id (uint32_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#RemoveAppDependencyResult_t){ .md-button .md-button--store target="_blank" }

## remove_ugc_dependency_result

!!! function "remove_ugc_dependency_result"
	Purpose: The result of a call to [removeDependency](/functions/ugc/#removedependency).

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* child_id (uint64_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#RemoveUGCDependencyResult_t){ .md-button .md-button--store target="_blank" }

## set_user_item_vote

!!! function "set_user_item_vote"
	Called when the user has voted on an item.

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* vote_up (bool)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetUserItemVoteResult_t){ .md-button .md-button--store target="_blank" }

## start_playtime_tracking

!!! function "start_playtime_tracking"
	Called when workshop item playtime tracking has started.

	**Returns:**

	* result (int)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#StartPlaytimeTrackingResult_t){ .md-button .md-button--store target="_blank" }

## stop_playtime_tracking

!!! function "stop_playtime_tracking"
	Called when workshop item playtime tracking has stopped.
	
	**Returns:**

	* result (int)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#StopPlaytimeTrackingResult_t){ .md-button .md-button--store target="_blank" }

## ugc_query_completed

!!! function "ugc_query_completed"
	Called when a UGC query request completes.

	**Returns:**

	* handle (uint64_t)
	* result (int)
	* results_returned (uint32)
	* total_matching (uint32)
	* cached (bool)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SteamUGCQueryCompleted_t){ .md-button .md-button--store target="_blank" }

## user_favorite_items_list_changed

!!! function "user_favorite_items_list_changed"
	Called when the user has added or removed an item to/from their favorites.

	**Returns:**

	* result (int)
	* file_id (uint64_t)
	* was_add_request (bool)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#UserFavoriteItemsListChanged_t){ .md-button .md-button--store target="_blank" }

## user_subscribed_items_list_changed

!!! function "user_subscribed_items_list_changed"
	Signal that the list of subscribed items changed.

	**Returns:**

	* app_id (uint32)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#UserSubscribedItemsListChanged_t){ .md-button .md-button--store target="_blank" }

## workshop_eula_status

!!! function "workshop_eula_status"
	Status of the user's acceptable/rejection of the app's specific Workshop EULA.

	**Returns:**

	* result (int)
	* app_id (uint32)
	* eula_data (dictionary)
		* version (uint32)
		* action (uint32)
		* accepted (bool)
		* needs_action (bool)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#WorkshopEULAStatus_t){ .md-button .md-button--store target="_blank" }