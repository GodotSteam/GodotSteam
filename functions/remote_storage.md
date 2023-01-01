# Functions - Remote Storage

---

## beginFileWriteBatch

!!! function "beginFileWriteBatch()"
	Use this along with [endFileWriteBatch](/functions/remote_storage/#endfilewritebatch) to wrap a set of local file writes/deletes that should be considered part of one single state change. For example, if saving game progress requires updating both savegame1.dat and maxprogress.dat, wrap those operations with calls to [beginFileWriteBatch](/functions/remote_storage/#beginfilewritebatch) and [endFileWriteBatch](/functions/remote_storage/#endfilewritebatch).

	These functions provide a hint to Steam which will help it manage the app's Cloud files. Using these functions is optional, however it will provide better reliability.

	Note that the functions may be used whether the writes are done using the Remote Storage API, or done directly to local disk (where AutoCloud is used).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#BeginFileWriteBatch){ .md-button .md-button--store target="_blank" }


## endFileWriteBatch

!!! function "endFileWriteBatch()"
	Use this along with [beginFileWriteBatch](/functions/remote_storage/#beginfilewritebatch) - see that documentation for more details.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#EndFileWriteBatch){ .md-button .md-button--store target="_blank" }

## fileDelete

!!! function "fileDelete( ```string``` file )"
	Deletes a file from the local disk, and propagates that delete to the cloud.

	This is meant to be used when a user actively deletes a file. Use [fileForget](/functions/remote_storage/#fileforget) if you want to remove a file from the Steam Cloud but retain it on the users local disk.

	When a file has been deleted it can be re-written with [fileWrite](/functions/remote_storage/#filewrite) to reupload it to the Steam Cloud.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileDelete){ .md-button .md-button--store target="_blank" }

## fileExists

!!! function "fileExists( ```string``` file )"
	Check if a remote file exists.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileExists){ .md-button .md-button--store target="_blank" }

## fileForget

!!! function "fileForget( ```string``` file )"
	Deletes the file from remote storage, but leaves it on the local disk and remains accessible from the API.

	When you are out of Cloud space, this can be used to allow calls to [fileWrite](/functions/remote_storage/#filewrite) to keep working without needing to make the user delete files.

	How you decide which files to forget are up to you. It could be a simple Least Recently Used (LRU) queue or something more complicated.

	Requiring the user to manage their Cloud-ized files for a game, while is possible to do, it is never recommended. For instance, "Which file would you like to delete so that you may store this new one?" removes a significant advantage of using the Cloud in the first place: its transparency.
	
	Once a file has been deleted or forgotten, calling [fileWrite](/functions/remote_storage/#filewrite) will resynchronize it in the Cloud. Rewriting a forgotten file is the only way to make it persisted again.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileForget){ .md-button .md-button--store target="_blank" }

## filePersisted

!!! function "filePersisted( ```string``` file )"
	Checks if a specific file is persisted in the Steam Cloud.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FilePersisted){ .md-button .md-button--store target="_blank" }

## fileRead

!!! function "fileRead( ```string``` file, ```uint32_t``` data_to_read )"
	Opens a binary file, reads the contents of the file into a byte array, and then closes the file.

	**Returns:** dictionary

	Contains the following keys:

	* ret (bool)
	* buf (PoolByteArray)

	**Note:**  This is a synchronous call and as such is a will block your calling thread on the disk IO, and will also block the SteamAPI, which can cause other threads in your application to block. To avoid "hitching" due to a busy disk on the client machine using [fileReadAsync](/functions/remote_storage/#filereadasync), the asynchronous version of this API is recommended.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileRead){ .md-button .md-button--store target="_blank" }

## fileReadAsync

!!! function "fileReadAsync( ```string``` file, ```uint32``` offset, ```uint32_t``` data_to_read )"
	Starts an asynchronous read from a file. The offset and amount to read should be valid for the size of the file, as indicated by [getFileSize](/functions/remote_storage/#getfilesize) or [getFileTimestamp](/functions/remote_storage/#getfiletimestamp).

	Triggers a [file_read_async_complete](/signals/parties/#file_read_async_complete) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileReadAsync){ .md-button .md-button--store target="_blank" }

## fileShare

!!! function "fileShare( ```string``` file )"
	Share a file.

	Triggers a [file_share_result](/signals/parties/#file_share_result) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileShare){ .md-button .md-button--store target="_blank" }

## fileWrite

!!! function "fileWrite( ```string``` file, ```PoolByteArray``` data )"
	Creates a new file, writes the bytes to the file, and then closes the file. If the target file already exists, it is overwritten.

	**Returns:** bool

	**Note:**  This is a synchronous call and as such is a will block your calling thread on the disk IO, and will also block the SteamAPI, which can cause other threads in your application to block. To avoid "hitching" due to a busy disk on the client machine using [fileWriteAsync](/functions/remote_storage/#filewriteasync), the asynchronous version of this API is recommended.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileWrite){ .md-button .md-button--store target="_blank" }

## fileWriteAsync

!!! function "fileWriteAsync( ```string``` file, ```PoolByteArray``` data )"
	Creates a new file and asynchronously writes the raw byte data to the Steam Cloud, and then closes the file. If the target file already exists, it is overwritten.

	Triggers a [file_write_async_complete](/signals/parties/#_file_write_async_complete) call result.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileWriteAsync){ .md-button .md-button--store target="_blank" }

## fileWriteStreamCancel

!!! function "fileWriteStreamCancel( ```uint64_t``` write_handle )"
	Cancels a file write stream that was started by [fileWriteStreamOpen](/functions/remote_storage/#filewritestreamopen).

	This trashes all of the data written and closes the write stream, but if there was an existing file with this name, it remains untouched.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileWriteStreamCancel){ .md-button .md-button--store target="_blank" }

## fileWriteStreamClose

!!! function "fileWriteStreamClose( ```uint64_t``` write_handle )"
	Closes a file write stream that was started by [fileWriteStreamOpen](/functions/remote_storage/#filewritestreamopen).

	This flushes the stream to the disk, overwriting the existing file if there was one.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileWriteStreamClose){ .md-button .md-button--store target="_blank" }

## fileWriteStreamOpen

!!! function "fileWriteStreamOpen( ```string``` file )"
	Creates a new file output stream allowing you to stream out data to the Steam Cloud file in chunks. If the target file already exists, it is not overwritten until [fileWriteStreamClose](/functions/remote_storage/#filewritestreamclose) has been called.

	To write data out to this stream you can use [fileWriteStreamWriteChunk](/functions/remote_storage/#filewritestreamwritechunk), and then to close or cancel you use [fileWriteStreamClose](/functions/remote_storage/#filewritestreamclose) and [fileWriteStreamCancel](/functions/remote_storage/#filewritestreamcancel) respectively.

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileWriteStreamOpen){ .md-button .md-button--store target="_blank" }

## fileWriteStreamWriteChunk

!!! function "fileWriteStreamWriteChunk( ```uint64_t``` write_handle, ```PoolByteArray``` data )"
	Writes a blob of data to the file write stream.

	**Returns:** bool

   ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#FileWriteStreamWriteChunk){ .md-button .md-button--store target="_blank" }

## getCachedUGCCount

!!! function "getCachedUGCCount()"
	Gets the number of cached UGC.

	**Returns:** int32

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetCachedUGCCount){ .md-button .md-button--store target="_blank" }

## getCachedUGCHandle

!!! function "getCachedUGCHandle( ```int32``` content )"
	Gets the cached UGC's handle.

	**Returns:** uint64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetCachedUGCHandle){ .md-button .md-button--store target="_blank" }

## getFileCount

!!! function "getFileCount()"
	Gets the total number of local files synchronized by Steam Cloud.

	Used for enumeration with [getFileNameAndSize](/functions/remote_storage/#getfilenameandsize).

	**Returns:** uint32_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetFileCount){ .md-button .md-button--store target="_blank" }

## getFileNameAndSize

!!! function "getFileNameAndSize( ```int``` file )"
	Gets the file name and size of a file from the index.

	**Returns:** dictionary

	Contains the following keys:

	* name (string)
	* size (int32_t)

	**Note:**  You must call [getFileCount](/functions/remote_storage/#getfilecount) first to get the number of files.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetFileNameAndSize){ .md-button .md-button--store target="_blank" }

## getFileSize

!!! function "getFileSize( ```string``` file )"
	Get the size of the current file.

	**Returns:** int32_t

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetFileSize){ .md-button .md-button--store target="_blank" }

## getFileTimestamp

!!! function "getFileTimestamp( ```string``` file )"
	Get the timestamp of when the file was uploaded.

	**Returns:** int64_t

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetFileTimestamp){ .md-button .md-button--store target="_blank" }

## getLocalFileChange

!!! function "getLocalFileChange( ```int``` file )"
	After calling [getLocalFileChangeCount](/functions/remote_storage/#getlocalfilechangecount), use this method to iterate over the changes. The changes described have already been made to local files. Your application should take appropriate action to reload state from disk, and possibly notify the user.
	
	For example: The local system had been suspended, during which time the user played elsewhere and uploaded changes to the Steam Cloud. On resume, Steam downloads those changes to the local system before resuming the application. The application receives an [local_file_changed](/signals/parties/#local_file_changed), and uses [getLocalFileChangeCount](/functions/remote_storage/#getlocalfilechangecount) and [getLocalFileChange](/functions/remote_storage/#getlocalfilechange) to iterate those changes. Depending on the application structure and the nature of the changes, the application could:
	
	* Re-load game progress to resume at exactly the point where the user was when they exited the game on the other device
	* Notify the user of any synchronized changes that don't require reloading
	* Etc.
		
	**Returns:** dictionary

	Contains the following keys:

	* file (string)
	* change_type (int)
	* path_type (int)

	**Note:** only applies to applications flagged as supporting dynamic Steam Cloud sync.

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetLocalFileChange){ .md-button .md-button--store target="_blank" }

## getLocalFileChangeCount

!!! function "getLocalFileChangeCount()"
	When your application receives a [local_file_changed](/signals/parties/#local_file_changed), use this method to get the number of changes (file updates and file deletes) that have been made. You can then iterate the changes using [getLocalFileChange](/functions/remote_storage/#getlocalfilechange).

	**Returns:** uint32_t

	**Note:** only applies to applications flagged as supporting dynamic Steam Cloud sync.

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetLocalFileChangeCount){ .md-button .md-button--store target="_blank" }

## getQuota

!!! function "getQuota()"
	Gets the number of bytes available, and used on the users Steam Cloud storage.

	**Returns:** dictionary

	Contains the following keys:

	* total_bytes (uint64_t)
	* available_bytes (uint64_t)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetQuota){ .md-button .md-button--store target="_blank" }

## getSyncPlatforms

!!! function "getSyncPlatforms( ```string``` file )"
	Obtains the platforms that the specified file will synchronize to.

	**Returns:** dictionary

	Contains the following keys:

	* bitwise (int)
		* 0 - None
		* 1<<0 - Windows
		* 1<<1 - OSX
		* 1<<2 - PS3
		* 1<<3 - Linux
		* 1<<4 - Reserved
		* 0xffffffff - All
	* verbose (string)

	---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetSyncPlatforms){ .md-button .md-button--store target="_blank" }

## getUGCDetails

!!! function "getUGCDetails( ```uint64_t``` content )"
	Gets metadata for a file after it has been downloaded. This is the same metadata given in the [download_ugc_result](/signals/parties/#download_ugc_result) call result.

	**Returns:** dictionary

	Contains the following keys:

	* handle (uint64_t)
	* app_id (uint32_t)
	* size (int32)
	* filename (string)
	* owner_id (uint64_t)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetUGCDetails){ .md-button .md-button--store target="_blank" }

## getUGCDownloadProgress

!!! function "getUGCDownloadProgress( ```uint64_t``` content )"
	Gets the amount of data downloaded so far for a piece of content. bytes_expected can be 0 if function returns false or if the transfer hasn't started yet, so be careful to check for that before dividing to get a percentage.
	
	**Returns:** dictionary

	Contains the following keys:

	* bytes_downloaded (int32)
	* bytes_expected (int32)

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#GetUGCDownloadProgress){ .md-button .md-button--store target="_blank" }

## isCloudEnabledForAccount

!!! function "isCloudEnabledForAccount()"
	Checks if the account wide Steam Cloud setting is enabled for this user; or if they disabled it in the Settings->Cloud dialog.

	Ensure that you are also checking [isCloudEnabledForApp](/functions/remote_storage/#iscloudenabledforapp), as these two options are mutually exclusive.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#IsCloudEnabledForAccount){ .md-button .md-button--store target="_blank" }

## isCloudEnabledForApp

!!! function "isCloudEnabledForApp()"
	Checks if the per game Steam Cloud setting is enabled for this user; or if they disabled it in the Game Properties->Update dialog.

	Ensure that you are also checking [isCloudEnabledForAccount](/functions/remote_storage/#iscloudenabledforaccount), as these two options are mutually exclusive.

	It's generally recommended that you allow the user to toggle this setting within your in-game options, you can toggle it with [setCloudEnabledForApp](/functions/remote_storage/#setcloudenabledforapp).

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#IsCloudEnabledForApp){ .md-button .md-button--store target="_blank" }


## setCloudEnabledForApp

!!! function "setCloudEnabledForApp( ```bool``` enabled )"
	Enable or disable Steam Cloud for this application. This must only ever be called as the direct result of the user explicitly requesting that it's enabled or not. This is typically accomplished with a checkbox within your in-game options.

	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#SetCloudEnabledForApp){ .md-button .md-button--store target="_blank" }

## setSyncPlatforms

!!! function "setSyncPlatforms( ```string``` file, ```int``` platform )"
	Allows you to specify which operating systems a file will be synchronized to.
	
	Use this if you have a multiplatform game but have data which is incompatible between platforms.
	
	Files default to k_ERemoteStoragePlatformAll (0xffffffff) when they are first created. You can use the bitwise OR operator, "|" to specify multiple platforms.
	
	**Returns:** bool

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#SetSyncPlatforms){ .md-button .md-button--store target="_blank" }

## ugcDownload

!!! function "ugcDownload( ```uint64_t``` content, ```uint32``` priority )"
	Downloads a UGC file.

	A priority value of 0 will download the file immediately, otherwise it will wait to download the file until all downloads with a lower priority value are completed.  Downloads with equal priority will occur simultaneously.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#SetSyncPlatforms){ .md-button .md-button--store target="_blank" }

## ugcDownloadToLocation

!!! function "ugcDownloadToLocation( ```uint64_t``` content, ```string``` location, ```uint32``` priority )"
	Downloads a UGC file to a specific location.

	**Returns:** void

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#SetSyncPlatforms){ .md-button .md-button--store target="_blank" }

## ugcRead

!!! function "ugcRead( ```uint64_t``` content, ```int32``` data_size, ```uint32``` offset, ```int``` action )"
	After download, gets the content of the file.

	**Returns:** PoolByteArray

    ---
    [:fontawesome-brands-steam: Read more in the official Steamworks SDK documentation](https://partner.steamgames.com/doc/api/ISteamRemoteStorage#SetSyncPlatforms){ .md-button .md-button--store target="_blank" }