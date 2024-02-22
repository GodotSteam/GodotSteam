#include "steam/steam_api.h"

// Define Steam API constants
// Constants with 'deprecated/': these were listed in the SDK docs but do not exist in the header files; safe to remove probably
// Possibly deprecated or never existed?
#define ACCOUNT_ID_INVALID k_uAccountIdInvalid
#define API_CALL_INVALID k_uAPICallInvalid
#define APP_ID_INVALID k_uAppIdInvalid
#define AUTH_TICKET_INVALID k_HAuthTicketInvalid
#define DEPOT_ID_INVALID k_uDepotIdInvalid
#define GAME_EXTRA_INFO_MAX k_cchGameExtraInfoMax
#define INVALID_BREAKPAD_HANDLE 0 //deprecated?
#define QUERY_PORT_ERROR 0xFFFE //deprecated?
#define QUERY_PORT_NOT_INITIALIZED 0xFFFF //deprecated?
#define STEAM_ACCOUNT_ID_MASK k_unSteamAccountIDMask
#define STEAM_ACCOUNT_INSTANCE_MASK k_unSteamAccountInstanceMask
#define STEAM_BUFFER_SIZE 255 //deprecated?
#define STEAM_LARGE_BUFFER_SIZE 8160 //deprecated?
#define STEAM_MAX_ERROR_MESSAGE 1024
#define STEAM_USER_CONSOLE_INSTANCE 2 //deprecated?
#define STEAM_USER_DESKTOP_INSTANCE k_unSteamUserDefaultInstance
#define STEAM_USER_WEB_INSTANCE 4 //deprecated?

// Define Friends constants
#define CHAT_METADATA_MAX k_cubChatMetadataMax
#define ENUMERATED_FOLLOWERS_MAX k_cEnumerateFollowersMax
#define FRIEND_GAME_INFO_QUERY_PORT_ERROR k_usFriendGameInfoQueryPort_Error
#define FRIEND_GAME_INFO_QUERY_PORT_NOT_INITIALIZED k_usFriendGameInfoQueryPort_NotInitialized
#define FRIENDS_GROUP_LIMIT k_cFriendsGroupLimit
#define INVALID_FRIEND_GROUP_ID k_FriendsGroupID_Invalid
#define MAX_FRIENDS_GROUP_NAME k_cchMaxFriendsGroupName
#define MAX_RICH_PRESENCE_KEY_LENGTH k_cchMaxRichPresenceKeyLength
#define MAX_RICH_PRESENCE_KEYS k_cchMaxRichPresenceKeys
#define MAX_RICH_PRESENCE_VALUE_LENTH k_cchMaxRichPresenceValueLength
#define PERSONA_NAME_MAX_UTF8 k_cchPersonaNameMax
#define PERSONA_NAME_MAX_UTF16 k_cwchPersonaNameMax

// Define HTML Surface constants
#define INVALID_HTMLBROWSER 0

// Define HTTP constants
#define HTTPCOOKIE_INVALID_HANDLE INVALID_HTTPCOOKIE_HANDLE
#define HTTPREQUEST_INVALID_HANDLE INVALID_HTTPREQUEST_HANDLE

// Define Input constants
#define INPUT_HANDLE_ALL_CONTROLLERS STEAM_INPUT_HANDLE_ALL_CONTROLLERS
#define INPUT_MAX_ACTIVE_LAYERS STEAM_INPUT_MAX_ACTIVE_LAYERS
#define INPUT_MAX_ANALOG_ACTIONS STEAM_INPUT_MAX_ANALOG_ACTIONS
#define INPUT_MAX_ANALOG_ACTION_DATA STEAM_INPUT_MAX_ANALOG_ACTION_DATA
#define INPUT_MAX_COUNT STEAM_INPUT_MAX_COUNT
#define INPUT_MAX_DIGITAL_ACTIONS STEAM_INPUT_MAX_DIGITAL_ACTIONS
#define INPUT_MAX_ORIGINS STEAM_INPUT_MAX_ORIGINS
#define INPUT_MIN_ANALOG_ACTION_DATA STEAM_INPUT_MIN_ANALOG_ACTION_DATA

// Define Inventory constants
#define INVENTORY_RESULT_INVALID k_SteamInventoryResultInvalid
#define ITEM_INSTANCE_ID_INVALID k_SteamItemInstanceIDInvalid

// Define Matchmaking constants
#define FAVORITE_FLAG_FAVORITE k_unFavoriteFlagFavorite
#define FAVORITE_FLAG_HISTORY k_unFavoriteFlagHistory
#define FAVORITE_FLAG_NONE k_unFavoriteFlagNone
#define MAX_LOBBY_KEY_LENGTH k_nMaxLobbyKeyLength
#define SERVER_QUERY_INVALID HSERVERQUERY_INVALID

// Define Matchmaking Servers constants
#define MAX_GAME_SERVER_GAME_DATA k_cbMaxGameServerGameData
#define MAX_GAME_SERVER_GAME_DESCRIPTION k_cbMaxGameServerGameDescription
#define MAX_GAME_SERVER_GAME_DIR k_cbMaxGameServerGameDir
#define MAX_GAME_SERVER_MAP_NAME k_cbMaxGameServerMapName
#define MAX_GAME_SERVER_NAME k_cbMaxGameServerName
#define MAX_GAME_SERVER_TAGS k_cbMaxGameServerTags

// Define Music Remote constants
#define MUSIC_NAME_MAX_LENGTH k_SteamMusicNameMaxLength
#define MUSIC_PNG_MAX_LENGTH k_SteamMusicPNGMaxLength

// Define Networking Message constants
#define NETWORKING_SEND_AUTO_RESTART_BROKEN_SESSION k_nSteamNetworkingSend_AutoRestartBrokenSession
#define NETWORKING_SEND_NO_DELAY k_EP2PSendReliable
#define NETWORKING_SEND_NO_NAGLE k_EP2PSendUnreliableNoDelay
#define NETWORKING_SEND_RELIABLE k_EP2PSendReliableWithBuffering
#define NETWORKING_SEND_UNRELIABLE k_EP2PSendUnreliable

// Define Networking Socket constants
#define MAX_STEAM_PACKET_SIZE k_cbMaxSteamNetworkingSocketsMessageSizeSend

// Define Remote Storage constants
#define ENUMERATE_PUBLISHED_FILES_MAX_RESULTS k_unEnumeratePublishedFilesMaxResults
#define FILE_NAME_MAX k_cchFilenameMax
#define MAX_CLOUD_FILE_CHUNK_SIZE k_unMaxCloudFileChunkSize
#define PUBLISHED_DOCUMENT_CHANGE_DESCRIPTION_MAX k_cchPublishedDocumentChangeDescriptionMax
#define PUBLISHED_DOCUMENT_DESCRIPTION_MAX k_cchPublishedDocumentDescriptionMax
#define PUBLISHED_DOCUMENT_TITLE_MAX k_cchPublishedDocumentTitleMax
#define PUBLISHED_FILE_ID_INVALID k_PublishedFileIdInvalid
#define PUBLISHED_FILE_UPDATE_HANDLE_INVALID k_PublishedFileIdInvalid
#define PUBLISHED_FILE_URL_MAX k_cchPublishedFileURLMax
#define TAG_LIST_MAX k_cchTagListMax
#define UGC_FILE_STREAM_HANDLE_INVALID k_UGCFileStreamHandleInvalid
#define UGC_HANDLE_INVALID k_UGCHandleInvalid

// Define Screenshot constants
#define MAX_TAGGED_PUBLISHED_FILES k_nScreenshotMaxTaggedPublishedFiles
#define MAX_TAGGED_USERS k_nScreenshotMaxTaggedUsers
#define SCREENSHOT_INVALID_HANDLE INVALID_SCREENSHOT_HANDLE
#define SCREENSHOT_THUMB_WIDTH k_ScreenshotThumbWidth
#define UFS_TAG_TYPE_MAX k_cubUFSTagTypeMax
#define UFS_TAG_VALUE_MAX k_cubUFSTagValueMax

// Define UGC constants
#define DEVELOPER_METADATA_MAX k_cchDeveloperMetadataMax
#define NUM_UGC_RESULTS_PER_PAGE kNumUGCResultsPerPage
#define UGC_QUERY_HANDLE_INVALID k_UGCQueryHandleInvalid
#define UGC_UPDATE_HANDLE_INVALID k_UGCUpdateHandleInvalid

// Define User Stats constants
#define LEADERBOARD_DETAIL_MAX k_cLeaderboardDetailsMax
#define LEADERBOARD_NAME_MAX k_cchLeaderboardNameMax
#define STAT_NAME_MAX k_cchStatNameMax