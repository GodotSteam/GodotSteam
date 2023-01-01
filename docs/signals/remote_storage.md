# Signals - Remote Storage

These callbacks require you to run ```Steam.run_callbacks()``` in your ```_process()``` function to receive them.

------

## download_ugc_result

!!! function "download_ugc_result"
	Response when downloading UGC.

	**Returns:**

	* result (int)

	* download_data (dictionary)
		* handle (uint64_t)
		* app_id (uint32_t)
		* size (int32)
		* filename (string)
		* owner_id (uint64_t)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#RemoteStorageDownloadUGCResult_t){ .md-button .md-button--store target="_blank" }

## file_read_async_complete

!!! function "file_read_async_complete"
	Response when reading a file with [fileReadAsync](/functions/remote_storage/#filereadasync).

	**Returns:**

	* handle (uint64_t)
	* result (int)
	* offset (iunt32)
	* read (uint32)
	* complete (bool)

	------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#RemoteStorageFileReadAsyncComplete_t){ .md-button .md-button--store target="_blank" }

## file_share_result

!!! function "file_share_result"
	Response to a file being shared.

	**Returns:**
	
	* result (int)
	* handle (uint64_t)
	* name (string)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#RemoteStorageFileShareResult_t){ .md-button .md-button--store target="_blank" }

## file_write_async_complete

!!! function "file_write_async_complete"
	Response when writing a file asyncrounously with [fileWriteAsync](/functions/remote_storage/#file_write_async_complete).

	**Returns:**

	* result (int)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#RemoteStorageFileWriteAsyncComplete_t){ .md-button .md-button--store target="_blank" }

## local_file_changed

!!! function "local_file_changed"
	One or more files for this app have changed locally after syncing to remote session changes.

	**Note:** only posted if this happens _during_ the local app session.

	**Returns:** nothing

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#RemoteStorageLocalFileChange_t){ .md-button .md-button--store target="_blank" }

## subscribe_item

!!! function "subscribe_item"
	Called when a player attempts to subscribe to a Workshop item.

	**Deprecated:** only used with the deprecated Remote Storage based Workshop API.

	**Returns:**

	* *result (int)
	* file_id (int)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#RemoteStorageSubscribePublishedFileResult_t){ .md-button .md-button--store target="_blank" }

## unsubscribe_item

!!! function "unsubscribe_item"
	Called when a player attempts to unsubscribe from a Workshop item.

	**Deprecated:** only used with the deprecated Remote Storage based Workshop API.

	**Returns:**
	
	* result (int)
	* file_id (int)

    ------
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#RemoteStorageUnsubscribePublishedFileResult_t){ .md-button .md-button--store target="_blank" }
