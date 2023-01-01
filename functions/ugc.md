# Functions - UGC

---

## addAppDependency

!!! function "addAppDependency( ```uint64_t``` published_file_id, ```uint32_t``` app_id )"
	Adds a dependency between the given item and the appid. This list of dependencies can be retrieved by calling [getAppDependencies](/functions/ugc/#getappdependencies). This is a soft-dependency that is displayed on the web. It is up to the application to determine whether the item can actually be used or not.

	Triggers a [add_app_dependency_result](/signals/ugc/#add_app_dependency_result) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddAppDependency){ .md-button .md-button--store target="_blank" }

## addDependency

!!! function "addDependency( ```int``` published_file_id, ```int``` child_published_file_id )"
	Adds a workshop item as a dependency to the specified item. If the published_file_id item is of type k_EWorkshopFileTypeCollection (2), than the child_published_file_id is simply added to that collection. Otherwise, the dependency is a soft one that is displayed on the web and can be retrieved via the ISteamUGC API using a combination of the numChildren key returned from [getQueryUGCResult](/functions/ugc/#getqueryugcresult) and [getQueryUGCChildren](/functions/ugc/#getqueryugcchildren).

	Triggers a [add_ugc_dependency_result](/signals/ugc/#add_ugc_dependency_result) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddDependency){ .md-button .md-button--store target="_blank" }

## addExcludedTag

!!! function "addExcludedTag( ```uint64_t``` query_handle, ```string``` tag_name )"
	Adds a excluded tag to a pending UGC Query. This will only return UGC without the specified tag.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddExcludedTag){ .md-button .md-button--store target="_blank" }

## addItemKeyValueTag

!!! function "addItemKeyValueTag( ```uint64_t``` update_handle, ```string``` key, ```string``` value )"
	Adds a key-value tag pair to an item. Keys can map to multiple different values (1-to-many relationship).

	Key names are restricted to alpha-numeric characters and the '_' character.

	Both keys and values cannot exceed 255 characters in length.

	Key-value tags are searchable by exact match only.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddItemKeyValueTag){ .md-button .md-button--store target="_blank" }

## addItemPreviewFile

!!! function "addItemPreviewFile( ```uint64_t``` query_handle, ```string``` preview_file, ```int``` type )"
	Adds an additional preview file for the item.

	Then the format of the image should be one that both the web and the application (if necessary) can render, and must be under 1MB. Suggested formats include JPG, PNG and GIF.

	**Returns:** bool

	**Note:** Using 1 or 2 in type are not currently supported with this API. For YouTube videos you should use [addItemPreviewVideo](/functions/ugc/#).

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddItemPreviewFile){ .md-button .md-button--store target="_blank" }

## addItemPreviewVideo

!!! function "addItemPreviewVideo( ```uint64_t``` query_handle, ```string``` video_id )"
	Adds an additional video preview from YouTube for the item.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddItemPreviewVideo){ .md-button .md-button--store target="_blank" }

## addItemToFavorites

!!! function "addItemToFavorites( ```uint32_t``` app_id, ```uint64_t``` published_file_id )"
	Adds a workshop item to the users favorites list.

	Triggers a [user_favorite_items_list_changed](/signals/ugc/#user_favorite_items_list_changed) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddItemToFavorites){ .md-button .md-button--store target="_blank" }

## addRequiredKeyValueTag

!!! function "addRequiredKeyValueTag( ```uint64_t``` query_handle, ```string``` key, ```string``` value )"
	Adds a required key-value tag to a pending UGC Query. This will only return workshop items that have a key 
	and a value.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddRequiredKeyValueTag){ .md-button .md-button--store target="_blank" }

## addRequiredTag

!!! function "addRequiredTag( ```uint64_t``` query_handle, ```string``` tag_name )"
	Adds a required tag to a pending UGC Query. This will only return UGC with the specified tag.

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddRequiredTag){ .md-button .md-button--store target="_blank" }

## addRequiredTagGroup

!!! function "addRequiredTagGroup( ```uint64_t``` query_handle, ```array``` tag_array )"
	Adds the requirement that the returned items from the pending UGC Query have at least one of the tags in the given set (logical "or"). For each tag group that is added, at least one tag from each group is required to be on the matching items.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#AddRequiredTagGroup){ .md-button .md-button--store target="_blank" }

## initWorkshopForGameServer

!!! function "initWorkshopForGameServer( ```uint32_t``` workshop_depot_id )"
	Lets game servers set a specific workshop folder before issuing any UGC commands.

	This is helpful if you want to support multiple game servers running out of the same install folder.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#BInitWorkshopForGameServer){ .md-button .md-button--store target="_blank" }

## createItem

!!! function "createItem( ```uint32``` app_id, ```int``` file_type )"
	Creates a new workshop item with no content attached yet.

	Triggers a [item_created](/signals/ugc/#item_created) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#CreateItem){ .md-button .md-button--store target="_blank" }

## createQueryAllUGCRequest

!!! function "createQueryAllUGCRequest( ```int``` query_type, ```int``` matching_type, ```uint32_t``` creator_id, ```uint32_t``` consumer_id, ```int``` page )"
	Query for all matching UGC. You can use this to list all of the available UGC for your app.

	This will return up to 50 results. You can make subsequent calls to this function, increasing the page each time to get the next set of results.

	To query for the UGC associated with a single user you can use [createQueryUserUGCRequest](/functions/ugc/#createqueryuserugcrequest) instead.

	**Returns:** uint64_t

	**Note:** Either consumer_id or creator_id must have a valid app ID.

	**Note:** You must release the handle returned by this function by calling [releaseQueryUGCRequest](/functions/ugc/#releasequeryugcrequest) when you are done with it.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#CreateQueryAllUGCRequest){ .md-button .md-button--store target="_blank" }

## createQueryUGCDetailsRequest

!!! function "createQueryUGCDetailsRequest( ```array``` published_file_ids )"
	Query for the details of specific workshop items.

	This will return up to 50 results.

	To query all the UGC for your app you can use [createQueryAllUGCRequest](/functions/ugc/#createqueryallugcrequest) instead.

	**Returns:** uint64_t

	**Note:** Either consumer_id or creator_id must have a valid app ID.

	**Note:** You must release the handle returned by this function by calling [releaseQueryUGCRequest](/functions/ugc/#releasequeryugcrequest) when you are done with it.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#CreateQueryUGCDetailsRequest){ .md-button .md-button--store target="_blank" }
		

## createQueryUserUGCRequest

!!! function "createQueryUserUGCRequest( ```uint64_t``` steam_id, ```int``` list_type, ```int``` matching_ugc_type, ```int``` sort_order, ```uint32_t``` creator_id, ```uint32_t``` consumer_id, ```int``` page )"
	Query UGC associated with a user. You can use this to list the UGC the user is subscribed to amongst other things.

	This will return up to 50 results.

	To query all the UGC for your app you can use [createQueryAllUGCRequest](/functions/ugc/#createqueryallugcrequest) instead.

	**Returns:** uint64_t

	**Note:** Either consumer_id or creator_id must have a valid app ID.

	**Note:** You must release the handle returned by this function by calling [releaseQueryUGCRequest](/functions/ugc/#releasequeryugcrequest) when you are done with it.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#CreateQueryUserUGCRequest){ .md-button .md-button--store target="_blank" }
		

## deleteItem

!!! function "deleteItem( ```uint64_t``` published_file_id )"
	Deletes the item without prompting the user.

	Triggers a [item_deleted](/signals/ugc/#item_deleted) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#DeleteItem){ .md-button .md-button--store target="_blank" }

## downloadItem

!!! function "downloadItem( ```uint64_t``` published_file_id, ```bool``` high_priority )"
	Download new or update already installed item.

	If the return value is true then register and wait for the callback [item_downloaded](/signals/ugc/#item_downloaded) before calling [getItemInstallInfo](/functions/ugc/#) or accessing the workshop item on disk.

	If the user is not subscribed to the item (e.g. a Game Server using anonymous login), the workshop item will be downloaded and cached temporarily.

	If the workshop item has an item state of 8, then this function can be called to initiate the update. Do not access the workshop item on disk until the callback [item_downloaded](/signals/ugc/#item_downloaded) is called.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#DownloadItem){ .md-button .md-button--store target="_blank" }

## getAppDependencies

!!! function "getAppDependencies( ```uint64_t``` published_file_id )"
	Get the app dependencies associated with the given published_file_id. These are "soft" dependencies that are shown on the web. It is up to the application to determine whether an item can be used or not.

	Triggers a [get_app_dependencies_result](/signals/ugc/#get_app_dependencies_result) callback.

	**Returns:** void

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetAppDependencies){ .md-button .md-button--store target="_blank" }

## getItemDownloadInfo

!!! function "getItemDownloadInfo( ```uint64_t``` file_id )"
	Get info about a pending download of a workshop item that has 8 set.

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* downloaded (uint64_t)
	* total (uint64_t)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetItemDownloadInfo){ .md-button .md-button--store target="_blank" }

## getItemInstallInfo

!!! function "getItemInstallInfo( ```uint64_t``` file_id )"
	Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled (4) set.

	Calling this sets the "used" flag on the workshop item for the current player and adds it to their k_EUserUGCList_UsedOrPlayed (7) list.

	If k_EItemStateLegacyItem (2) is set then folder contains the path to the legacy file itself, not a folder.

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* size (int)
	* folder (string)
	* folder_size (uint32)
	* timestamp (uint32)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetItemInstallInfo){ .md-button .md-button--store target="_blank" }

## getItemState

!!! function "getItemState( ```uint64_t``` published_file_id )"
	Gets the current state of a workshop item on this client.

	**Returns:** uint32

	It should be one of the following:

	* 0 - None
	* 1 - Subscribed
	* 2 - Legacy
	* 4 - Installed
	* 8 - Update
	* 16 - Downloading
	* 32 - Pending

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetItemState){ .md-button .md-button--store target="_blank" }

## getItemUpdateProgress

!!! function "getItemUpdateProgress( ```uint64_t``` update_handle )"
	Gets the progress of an item update.

	**Returns:** dictionary

	Contains the following keys:

	* status (int)
	* processed (uint64)
	* total (uint64)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetItemUpdateProgress){ .md-button .md-button--store target="_blank" }

## getNumSubscribedItems

!!! function "getNumSubscribedItems()"
	Gets the total number of items the current user is subscribed to for the game or application.

	**Returns:** uint32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetNumSubscribedItems){ .md-button .md-button--store target="_blank" }

## getQueryUGCAdditionalPreview

!!! function "getQueryUGCAdditionalPreview( ```uint64_t``` query_handle, ```uint32``` index, ```uint32``` preview_index )"
	Retrieve the details of an additional preview associated with an individual workshop item after receiving a querying UGC call result.

	You should call this in a loop to get the details of all the workshop items returned.

	Before calling this you should call [getQueryUGCNumAdditionalPreviews](/functions/ugc/#getqueryugcnumadditionalpreviews) to get number of additional previews.

	**Returns:** dictionary

	Contains the following keys:

	* success (bool)
	* handle (uint64_t)
	* index (uint32)
	* preview (uint32)
	* url_or_video (string)
	* filename (string)
	* type (int)

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCAdditionalPreview){ .md-button .md-button--store target="_blank" }

## getQueryUGCChildren

!!! function "getQueryUGCChildren( ```uint64_t``` query_handle, ```uint32``` index, ```int32_t``` child_count )"
	Retrieve the IDs of any child items of an individual workshop item after receiving a querying UGC call result. These items can either be a part of a collection or some other dependency (see [addDependency](/functions/ugc/#adddependency)).

	You should create published_file_id with num_children provided in returned dictionary after getting the UGC details with [getQueryUGCResult](/functions/ugc/#getqueryugcresult).

	**Returns:** dictionary

	Contains the following keys:

	* success (bool)
	* handle (uint64_t)
	* index (uint32)
	* children (uint64_t)

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCChildren){ .md-button .md-button--store target="_blank" }

## getQueryUGCKeyValueTag

!!! function "getQueryUGCKeyValueTag( ```uint64_t``` query_handle, ```uint32``` index, ```uint32``` key_value_tag_index )"
	Retrieve the details of a key-value tag associated with an individual workshop item after receiving a querying UGC call result.

	You should call this in a loop to get the details of all the workshop items returned.

	Before calling this you should call [getQueryUGCNumKeyValueTags](/functions/ugc/#getqueryugcnumkeyvaluetags) to get number of tags.

	**Returns:** dictionary

	Contains the following keys:

	* success (bool)
	* handle (uint64_t)
	* index (uint32)
	* tag (uint32)
	* key (string)
	* value (string)

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCKeyValueTag){ .md-button .md-button--store target="_blank" }

## getQueryUGCMetadata

!!! function "getQueryUGCMetadata( ```uint64_t``` query_handle, ```uint32``` index )"
	Retrieve the developer set metadata of an individual workshop item after receiving a querying UGC call result.
	You should call this in a loop to get the details of all the workshop items returned.

	**Returns:** string

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCMetadata){ .md-button .md-button--store target="_blank" }

## getQueryUGCNumAdditionalPreviews

!!! function "getQueryUGCNumAdditionalPreviews( ```uint64_t``` query_handle, ```uint32``` index )"
	Retrieve the number of additional previews of an individual workshop item after receiving a querying UGC call result.

	You can then call [getQueryUGCAdditionalPreview](/functions/ugc/#getqueryugcadditionalpreview) to get the details of each additional preview.

	**Returns:** uint32

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCNumAdditionalPreviews){ .md-button .md-button--store target="_blank" }		

## getQueryUGCNumKeyValueTags

!!! function "getQueryUGCNumKeyValueTags( ```uint64_t``` query_handle, ```uint32``` index )"
	Retrieve the number of key-value tags of an individual workshop item after receiving a querying UGC call result.

	You can then call [getQueryUGCKeyValueTag](/functions/ugc/#getqueryugckeyvaluetag) to get the details of each tag.

	**Returns:** uint32

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCNumKeyValueTags){ .md-button .md-button--store target="_blank" }		

## getQueryUGCNumTags

!!! function "getQueryUGCNumTags( ```uint64_t``` query_handle, ```uint32``` index )"
	Available in SDK v1.51.

	Retrieve the number of tags for an individual workshop item after receiving a querying UGC call result.

	You can then call [getQueryUGCTag](/functions/ugc/#) to get the tag name or [getQueryUGCTagDisplayName](/functions/ugc/#) to get the localized tag string (if any).

	**Returns:** uint32

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCNumTags){ .md-button .md-button--store target="_blank" }

## getQueryUGCPreviewURL

!!! function "getQueryUGCPreviewURL( ```uint64_t``` query_handle, ```uint32``` index )"
	Retrieve the URL to the preview image of an individual workshop item after receiving a querying UGC call result.

	You can use this URL to download and display the preview image instead of having to download it using the [handle_preview_file key in the return dictionary from [getQueryUGCResult](/functions/ugc/#getqueryugcresult).

	**Returns:** string

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugcquerycompleted) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCPreviewURL){ .md-button .md-button--store target="_blank" }

## getQueryUGCResult

!!! function "getQueryUGCResult( ```uint64_t``` query_handle, ```uint32``` index )"
	Retrieve the details of an individual workshop item after receiving a querying UGC call result.

	**Returns:** dictionary

	Contains the following keys:

	* result (uint64_t)
	* file_id (uint64_t)
	* file_type (uint64_t)
	* creator_app_id (uint32_t)
	* consumer_app_id (uint32_t)
	* title (string)
	* description (string)
	* steam_id_owner (uint64_t)
	* time_created (uint32)
	* time_updated (uint32)
	* time_added_to_user_list (uint32)
	* visibility (int)
	* banned (bool)
	* accepted_for_use (bool)
	* tags_truncated (bool)
	* tags (string)
	* handle_file (uint64_t)
	* handle_preview-file (uint64_t)
	* file_name (string)
	* file_size (int32)
	* preview_file_size (int32)
	* url (string)
	* votes_up (uint32)
	* votes_down (uint32)
	* score (float)
	* num_children (uint32)

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCResult){ .md-button .md-button--store target="_blank" }

## getQueryUGCStatistic

!!! function "getQueryUGCStatistic( ```uint64_t``` query_handle, ```uint32``` index, ```int``` stat_type )"
	Retrieve various statistics of an individual workshop item after receiving a querying UGC call result.

	**Returns:** dictionary

	Contains the following keys:

	* success (bool)
	* handle (uint64_t)
	* index (uint32)
	* type (int)
	* value (uint64_t)

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCStatistic){ .md-button .md-button--store target="_blank" }


## getQueryUGCTag

!!! function "getQueryUGCTag( ```uint64_t``` query_handle, ```uint32``` index, ```uint32``` tag_index )"
	Available in SDK v1.51.

	Retrieve the "nth" tag associated with an individual workshop item after receiving a querying UGC call result.

	Before calling this you should call [getQueryUGCNumTags](/functions/ugc/#getqueryugcnumtags) to get number of tags.

	**Returns:** string

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCTag){ .md-button .md-button--store target="_blank" }

## getQueryUGCTagDisplayName

!!! function "getQueryUGCTagDisplayName( ```uint64_t``` query_handle, ```uint32``` index, ```uint32``` tag_index )"
	Available in SDK v1.51.

	Retrieve the "nth" display string (usually localized) for a tag, which is associated with an individual workshop item after receiving a querying UGC call result.

	Before calling this you should call [getQueryUGCNumTags](/functions/ugc/#getqueryugcnumtags) to get number of tags.

	**Returns:** string

	**Note:** This must only be called with the handle obtained from a successful [ugc_query_completed](/signals/ugc/#ugc_query_completed) call result.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetQueryUGCTagDisplayName){ .md-button .md-button--store target="_blank" }

## getSubscribedItems

!!! function "getSubscribedItems()"
	Gets a list of all of the items the current user is subscribed to for the current game.

	**Returns:** array of published_file_ids

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetSubscribedItems){ .md-button .md-button--store target="_blank" }

## getWorkshopEULAStatus

!!! function "getWorkshopEULAStatus()"
	Asynchronously retrieves data about whether the user accepted the Workshop EULA for the current app.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetWorkshopEULAStatus){ .md-button .md-button--store target="_blank" }

## getUserItemVote

!!! function "getUserItemVote( ```uint64_t``` published_file_id )"
	Gets the users vote status on a workshop item.

	Triggers a [get_item_vote_result](/signals/ugc/#get_item_vote_result) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#GetUserItemVote){ .md-button .md-button--store target="_blank" }

## releaseQueryUGCRequest

!!! function "releaseQueryUGCRequest( ```uint64_t``` query_handle )"
	Releases a UGC query handle when you are done with it to free up memory.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#ReleaseQueryUGCRequest){ .md-button .md-button--store target="_blank" }

## removeAppDependency

!!! function "removeAppDependency( ```uint64_t``` published_file_id, ```uint32_t``` app_id )"
	Removes the dependency between the given item and the appid. This list of dependencies can be retrieved by calling [getAppDependencies](/functions/ugc/#getappdependencies).

	Triggers a [remove_app_dependency_result](/signals/ugc/#remove_app_dependency_result) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#RemoveAppDependency){ .md-button .md-button--store target="_blank" }

## removeDependency

!!! function "removeDependency( ```uint64_t``` published_file_id, ```uint64_t``` child_published_file_id )"
	Removes a workshop item as a dependency from the specified item.

	Triggers a [remove_ugc_dependency_result](/signals/ugc/#remove_ugc_dependency_result) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#RemoveDependency){ .md-button .md-button--store target="_blank" }

## removeItemFromFavorites

!!! function "removeItemFromFavorites( ```uint32_t``` app_id, ```uint64_t``` published_file_id )"
	Removes a workshop item from the users favorites list.

	Triggers a [user_favorite_items_list_changed](/signals/ugc/#user_favorite_items_list_changed) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#RemoveItemFromFavorites){ .md-button .md-button--store target="_blank" }

## removeItemKeyValueTags

!!! function "removeItemKeyValueTags( ```uint64_t``` update_handle, ```string``` key )"
	Removes an existing key value tag from an item.

	You can only call this up to 100 times per item update. If you need remove more tags than that you'll need to make subsequent item updates.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#RemoveItemKeyValueTags){ .md-button .md-button--store target="_blank" }

## removeItemPreview

!!! function "removeItemPreview( ```uint64_t``` update_handle, ```uint32``` index )"
	Removes an existing preview from an item.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#RemoveItemPreview){ .md-button .md-button--store target="_blank" }

## sendQueryUGCRequest

!!! function "sendQueryUGCRequest( ```uint64_t``` update_handle )"
	Send a UGC query to Steam.

	This must be called with a handle obtained from [createQueryUserUGCRequest](/functions/ugc/#createqueryuserugcrequest), [createQueryAllUGCRequest](/functions/ugc/#createqueryallugcrequest), or [createQueryUGCDetailsRequest](/functions/ugc/#createqueryugcdetailsrequest) to actually send the request to Steam.

	Before calling this you should use one or more of the following APIs to customize your query: [addRequiredTag](/functions/ugc/#addrequiredtag), [addExcludedTag](/functions/ugc/#addexcludedtag), [setReturnOnlyIDs](/functions/ugc/#setreturnonlyids), [setReturnKeyValueTags](/functions/ugc/#setreturnkeyvaluetags), [setReturnLongDescription](/functions/ugc/#setreturnlongdescription), [setReturnMetadata](/functions/ugc/#setreturnmetadata), [setReturnChildren](/functions/ugc/#setreturnchildren), [setReturnAdditionalPreviews](/functions/ugc/#setreturnadditionalpreviews), [setReturnTotalOnly](/functions/ugc/#setreturntotalonly), [setLanguage](/functions/ugc/#setlanguage), [setAllowCachedResponse](/functions/ugc/#setallowcachedresponse), [setCloudFileNameFilter](/functions/ugc/#setcloudfilenamefilter), [setMatchAnyTag](/functions/ugc/#setmatchanytag), [setSearchText](/functions/ugc/#setsearchtext), [setRankedByTrendDays](/functions/ugc/#setrankedbytrenddays), or [addRequiredKeyValueTag](/functions/ugc/#addrequiredkeyvaluetag).

	Triggers a [ugc_query_completed](/signals/ugc/#ugc_query_completed) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SendQueryUGCRequest){ .md-button .md-button--store target="_blank" }

## setAllowCachedResponse

!!! function "setAllowCachedResponse( ```uint64_t``` update_handle, ```uint32``` max_age_seconds )"
	Sets whether results will be returned from the cache for the specific period of time on a pending UGC Query.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetAllowCachedResponse){ .md-button .md-button--store target="_blank" }

## setCloudFileNameFilter

!!! function "setCloudFileNameFilter( ```uint64_t``` update_handle, ```string``` match_cloud_filename )"
	Sets to only return items that have a specific filename on a pending UGC Query.

	**Returns:** bool

	**Note:** This can only be used with [createQueryUserUGCRequest](/functions/ugc/#createqueryuserugcrequest).

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetCloudFileNameFilter){ .md-button .md-button--store target="_blank" }

## setItemContent

!!! function "setItemContent( ```uint64_t``` update_handle, ```string``` content_folder )"
	Sets the folder that will be stored as the content for an item.

	For efficient upload and download, files should not be merged or compressed into single files (e.g. zip files).

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemContent){ .md-button .md-button--store target="_blank" }

## setItemDescription

!!! function "setItemDescription( ```uint64_t``` update_handle, ```string``` description )"
	Sets a new description for an item.

	The description must be limited to the length defined by k_cchPublishedDocumentDescriptionMax (8000).

	You can set what language this is for by using [setItemUpdateLanguage](/functions/ugc/#setitemupdatelanguage), if no language is set then "english" is assumed.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemDescription){ .md-button .md-button--store target="_blank" }

## setItemMetadata

!!! function "setItemMetadata( ```uint64_t``` update_handle, ```string``` metadata )"
	Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.

	The metadata must be limited to the size defined by k_cchDeveloperMetadataMax (5000).

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemMetadata){ .md-button .md-button--store target="_blank" }

## setItemPreview

!!! function "setItemPreview( ```uint64_t``` update_handle, ```string``` preview_file )"
	Sets the primary preview image for the item.

	The format should be one that both the web and the application (if necessary) can render. Suggested formats include JPG, PNG and GIF.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemPreview){ .md-button .md-button--store target="_blank" }

## setItemTags

!!! function "setItemTags( ```uint64_t``` update_handle, ```array``` tag_array)"
	Sets arbitrary developer specified tags on an item.

	Each tag must be limited to 255 characters. Tag names can only include printable characters, excluding ','. For reference on what characters are allowed, refer to [http://en.cppreference.com/w/c/string/byte/isprint](http://en.cppreference.com/w/c/string/byte/isprint){ target="_blank" }

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemTags){ .md-button .md-button--store target="_blank" }

## setItemTitle

!!! function "setItemTitle( ```uint64_t``` update_handle, ```string``` title )"
	Sets a new title for an item.

	The title must be limited to the size defined by k_cchPublishedDocumentTitleMax (128).

	You can set what language this is for by using [setItemUpdateLanguage](/functions/ugc/#setitemupdatelanguage), if no language is set then "english" is assumed.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemTitle){ .md-button .md-button--store target="_blank" }

## setItemUpdateLanguage

!!! function "setItemUpdateLanguage( ```uint64_t``` update_handle, ```string``` language )"
	Sets the language of the title and description that will be set in this item update.

	This must be in the format of the [API language code](https://partner.steamgames.com/doc/store/localization#supported_languages){ target="_blank" }.

	If this is not set then "english" is assumed.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemUpdateLanguage){ .md-button .md-button--store target="_blank" }

## setItemVisibility

!!! function "setItemVisibility( ```uint64_t``` update_handle, ```int``` visibility )"
	Sets the visibility of an item.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetItemVisibility){ .md-button .md-button--store target="_blank" }

## setLanguage

!!! function "setLanguage( ```uint64_t``` query_handle, ```string``` language )"
	Sets the language to return the title and description in for the items on a pending UGC Query.

	This must be in the format of the [API language code](https://partner.steamgames.com/doc/store/localization#supported_languages){ target="_blank" }.

	If this is not set then "english" is assumed.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetLanguage){ .md-button .md-button--store target="_blank" }

## setMatchAnyTag

!!! function "setMatchAnyTag( ```uint64_t``` query_handle, ```bool``` match_any_tag )"
	Sets whether workshop items will be returned if they have one or more matching tag, or if all tags need to match on a pending UGC Query.

	**Returns:** bool

	**Note:** This can only be used with [createQueryAllUGCRequest](/functions/ugc/#createqueryallugcrequest).

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetMatchAnyTag){ .md-button .md-button--store target="_blank" }

## setRankedByTrendDays

!!! function "setRankedByTrendDays( ```uint64_t``` query_handle, ```uint32``` days )"
	Sets whether the order of the results will be updated based on the rank of items over a number of days on a pending UGC Query.

	**Returns:** bool

	**Note:** This can only be used with [createQueryAllUGCRequest](/functions/ugc/#createqueryallugcrequest).

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetRankedByTrendDays){ .md-button .md-button--store target="_blank" }

## setReturnAdditionalPreviews

!!! function "setReturnAdditionalPreviews( ```uint64_t``` query_handle, ```bool``` return_additional_previews )"
	Sets whether to return any additional images/videos attached to the items on a pending UGC Query.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnAdditionalPreviews){ .md-button .md-button--store target="_blank" }

## setReturnChildren

!!! function "setReturnChildren( ```uint64_t``` query_handle, ```bool``` return_children )"
	Sets whether to return the IDs of the child items of the items on a pending UGC Query.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnChildren){ .md-button .md-button--store target="_blank" }

## setReturnKeyValueTags

!!! function "setReturnKeyValueTags( ```uint64_t``` query_handle, ```bool``` return_key_value_tags )"
	Sets whether to return any key-value tags for the items on a pending UGC Query.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnKeyValueTags){ .md-button .md-button--store target="_blank" }

## setReturnLongDescription

!!! function "setReturnLongDescription( ```uint64_t``` query_handle, ```bool``` return_long_description )"
	Sets whether to return the full description for the items on a pending UGC Query.

	If you don't set this then you only receive the summary which is the description truncated at 255 bytes.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnLongDescription){ .md-button .md-button--store target="_blank" }

## setReturnMetadata

!!! function "setReturnMetadata( ```uint64_t``` query_handle, ```bool``` return_metadata )"
	Sets whether to return the developer specified metadata for the items on a pending UGC Query.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnMetadata){ .md-button .md-button--store target="_blank" }

## setReturnOnlyIDs

!!! function "setReturnOnlyIDs( ```uint64_t``` query_handle, ```bool``` return_only_ids )"
	Sets whether to only return IDs instead of all the details on a pending UGC Query.

	This is useful for when you don't need all the information (e.g. you just want to get the IDs of the items a user has in their favorites list.)

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnOnlyIDs){ .md-button .md-button--store target="_blank" }

## setReturnPlaytimeStats

!!! function "setReturnPlaytimeStats( ```uint64_t``` query_handle, ```uint32``` days )"
	Sets whether to return the the playtime stats on a pending UGC Query.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnPlaytimeStats){ .md-button .md-button--store target="_blank" }

## setReturnTotalOnly

!!! function "setReturnTotalOnly( ```uint64_t``` query_handle, ```bool``` return_total_only )"
	Sets whether to only return the the total number of matching items on a pending UGC Query.

	The actual items will not be returned when [ugc_query_completed](/signals/ugc/#ugc_query_completed) is called.

	**Returns:** bool

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetReturnTotalOnly){ .md-button .md-button--store target="_blank" }

## setSearchText

!!! function "setSearchText( ```uint64_t``` query_handle, ```string``` search_text )"
	Sets a string to that items need to match in either the title or the description on a pending UGC Query.

	**Returns:** bool

	**Note:** This can only be used with [createQueryAllUGCRequest](/functions/ugc/#createqueryallugcrequest).

	**Note:** This must be set before you send a UGC Query handle using [sendQueryUGCRequest](/functions/ugc/#sendqueryugcrequest).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetSearchText){ .md-button .md-button--store target="_blank" }

## setUserItemVote

!!! function "setUserItemVote( ```uint64_t``` published_file_id, ```bool``` vote_up )"
	Allows the user to rate a workshop item up or down.

	Triggers a [set_user_item_vote](/signals/ugc/#set_user_item_vote) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SetUserItemVote){ .md-button .md-button--store target="_blank" }

## showWorkshopEULA

!!! function "showWorkshopEULA()"
	Show the app's latest Workshop EULA to the user in an overlay window, where they can accept it or not.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#ShowWorkshopEULA){ .md-button .md-button--store target="_blank" }

## startItemUpdate

!!! function "startItemUpdate( ```uint32_t``` app_id, ```uint64_t``` published_file_id )"
	Starts the item update process.

	This gets you a handle that you can use to modify the item before finally sending off the update to the server with [submitItemUpdate](/functions/ugc/#submititemupdate).

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#StartItemUpdate){ .md-button .md-button--store target="_blank" }

## startPlaytimeTracking

!!! function "startPlaytimeTracking( ```array``` published_file_ids )"
	Start tracking playtime on a set of workshop items.

	When your app shuts down, playtime tracking will automatically stop.

	Triggers a [start_playtime_tracking](/signals/ugc/#start_playtime_tracking) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#StartPlaytimeTracking){ .md-button .md-button--store target="_blank" }

## stopPlaytimeTracking

!!! function "stopPlaytimeTracking( ```array``` published_file_ids )"
	Stop tracking playtime on a set of workshop items.

	When your app shuts down, playtime tracking will automatically stop.

	Triggers a [stop_playtime_tracking](/signals/ugc/#stop_playtime_tracking) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#StopPlaytimeTracking){ .md-button .md-button--store target="_blank" }

## stopPlaytimeTrackingForAllItems

!!! function "stopPlaytimeTrackingForAllItems()"
	Stop tracking playtime of all workshop items.

	When your app shuts down, playtime tracking will automatically stop.

	Triggers a [stop_playtime_tracking](/signals/ugc/#stop_playtime_tracking) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#StopPlaytimeTrackingForAllItems){ .md-button .md-button--store target="_blank" }

## submitItemUpdate

!!! function "submitItemUpdate( ```uint64_t``` update_handle, ```string``` change_note )"
	Uploads the changes made to an item to the Steam Workshop.

	You can track the progress of an item update with [getItemUpdateProgress](/functions/ugc/#getitemupdateprogress).

	Triggers a [item_updated](/signals/ugc/#item_updated) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SubmitItemUpdate){ .md-button .md-button--store target="_blank" }

## subscribeItem

!!! function "subscribeItem( ```uint64_t``` published_file_id )"
	Subscribe to a workshop item. It will be downloaded and installed as soon as possible.

	Triggers a [subscribe_item](/signals/ugc/#subscribe_item) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SubscribeItem){ .md-button .md-button--store target="_blank" }

## suspendDownloads

!!! function "suspendDownloads( ```bool``` suspend )"
	Suspends and resumes all workshop downloads.

	If you call this with suspend set to true then downloads will be suspended until you resume them by setting suspend to false or when the game ends.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#SuspendDownloads){ .md-button .md-button--store target="_blank" }

## unsubscribeItem

!!! function "unsubscribeItem( ```uint64_t``` published_file_id )"
	Unsubscribe from a workshop item. This will result in the item being removed after the game quits.

	Triggers a [unsubscribe_item](/signals/ugc/#unsubscribe_item) callback.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#UnsubscribeItem){ .md-button .md-button--store target="_blank" }

## updateItemPreviewFile

!!! function "updateItemPreviewFile( ```uint64_t``` update_handle, ```uint32``` index, ```string``` preview_file )"
	Updates an additional video preview from YouTube for the item.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#UpdateItemPreviewFile){ .md-button .md-button--store target="_blank" }

## updateItemPreviewVideo

!!! function "updateItemPreviewVideo( ```uint64_t``` update_handle, ```uint32``` index, ```string``` video_id )"
	Updates an additional video preview from YouTube for the item.

	**Returns:** bool

	**Note:** This must be set before you submit the UGC update handle using [submitItemUpdate](/functions/ugc/#submititemupdate).

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamUGC#UpdateItemPreviewVideo){ .md-button .md-button--store target="_blank" }
