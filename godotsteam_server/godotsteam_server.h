#ifndef GODOTSTEAM_SERVER_H
#define GODOTSTEAM_SERVER_H
// Turn off MSVC-only warning about strcpy
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#pragma warning(disable:4828)
#endif
// Include INT types header
#include <inttypes.h>
// Include Steamworks Server API header
#include <steam/steam_gameserver.h>
// Include some Godot headers
#include "core/object.h"
#include "scene/resources/texture.h"
#include "core/reference.h"
#include "core/dictionary.h"

class SteamServer: public Object {
	GDCLASS(SteamServer, Object);
	
	public:
		// Steam API enums
		enum AccountType {
			ACCOUNT_TYPE_INVALID = 0, ACCOUNT_TYPE_INDIVIDUAL = 1, ACCOUNT_TYPE_MULTISEAT = 2, ACCOUNT_TYPE_GAME_SERVER = 3, ACCOUNT_TYPE_ANON_GAME_SERVER = 4, ACCOUNT_TYPE_PENDING = 5, ACCOUNT_TYPE_CONTENT_SERVER = 6, ACCOUNT_TYPE_CLAN = 7, ACCOUNT_TYPE_CHAT = 8, ACCOUNT_TYPE_CONSOLE_USER = 9, ACCOUNT_TYPE_ANON_USER = 10, ACCOUNT_TYPE_MAX = 11
		};
		enum AppOwnershipFlags {
			APP_OWNERSHIP_FLAGS_NONE = 0x0000, APP_OWNERSHIP_FLAGS_OWNS_LICENSE = 0x0001, APP_OWNERSHIP_FLAGS_FREE_LICENSE = 0x0002, APP_OWNERSHIP_FLAGS_REGION_RESTRICTED = 0x0004, APP_OWNERSHIP_FLAGS_LOW_VIOLENCE = 0x0008, APP_OWNERSHIP_FLAGS_INVALID_PLATFORM = 0x0010, APP_OWNERSHIP_FLAGS_SHARED_LICENSE = 0x0020,
			APP_OWNERSHIP_FLAGS_FREE_WEEKEND = 0x0040, APP_OWNERSHIP_FLAGS_RETAIL_LICENSE = 0x0080, APP_OWNERSHIP_FLAGS_LICENSE_LOCKED = 0x0100, APP_OWNERSHIP_FLAGS_LICENSE_PENDING = 0x0200, APP_OWNERSHIP_FLAGS_LICENSE_EXPIRED = 0x0400, APP_OWNERSHIP_FLAGS_LICENSE_PERMANENT = 0x0800, APP_OWNERSHIP_FLAGS_LICENSE_RECURRING = 0x1000,
			APP_OWNERSHIP_FLAGS_LICENSE_CANCELED = 0x2000, APP_OWNERSHIP_FLAGS_AUTO_GRANT = 0x4000, APP_OWNERSHIP_FLAGS_PENDING_GIFT = 0x8000, APP_OWNERSHIP_FLAGS_RENTAL_NOT_ACTIVATED = 0x10000, APP_OWNERSHIP_FLAGS_RENTAL = 0x20000, APP_OWNERSHIP_FLAGS_SITE_LICENSE = 0x40000
		};
		enum AppReleaseState {
			APP_RELEASE_STATE_UNKNOWN = 0, APP_RELEASE_STATE_UNAVAILABLE = 1, APP_RELEASE_STATE_PRERELEASE = 2, APP_RELEASE_STATE_PRELOAD_ONLY = 3, APP_RELEASE_STATE_RELEASED = 4
		};
		enum AppType {
			APP_TYPE_INVALID = 0x000, APP_TYPE_GAME = 0x001, APP_TYPE_APPLICATION = 0x002, APP_TYPE_TOOL = 0x004, APP_TYPE_DEMO = 0x008, APP_TYPE_MEDIA_DEPRECATED = 0x010, APP_TYPE_DLC = 0x020, APP_TYPE_GUIDE = 0x040, APP_TYPE_DRIVER = 0x080, APP_TYPE_CONFIG = 0x100, APP_TYPE_HARDWARE = 0x200, APP_TYPE_FRANCHISE = 0x400, APP_TYPE_VIDEO = 0x800,
			APP_TYPE_PLUGIN = 0x1000, APP_TYPE_MUSIC = 0x2000, APP_TYPE_SERIES = 0x4000, APP_TYPE_SHORTCUT = 0x40000000, APP_TYPE_DEPOT_ONLY = 0X80000000
		};
		enum AuthSessionResponse {
			AUTH_SESSION_RESPONSE_OK = 0, AUTH_SESSION_RESPONSE_USER_NOT_CONNECTED_TO_STEAM = 1, AUTH_SESSION_RESPONSE_NO_LICENSE_OR_EXPIRED = 2, AUTH_SESSION_RESPONSE_VAC_BANNED = 3, AUTH_SESSION_RESPONSE_LOGGED_IN_ELSEWHERE = 4, AUTH_SESSION_RESPONSE_VAC_CHECK_TIMEDOUT = 5, AUTH_SESSION_RESPONSE_AUTH_TICKET_CANCELED = 6,
			AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID_ALREADY_USED = 7, AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID = 8, AUTH_SESSION_RESPONSE_PUBLISHER_ISSUED_BAN = 9
		};
		enum BeginAuthSessionResult {
			BEGIN_AUTH_SESSION_RESULT_OK = 0, BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET = 1, BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST = 2, BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION = 3, BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH = 4, BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET = 5
		};
		enum BroadcastUploadResult {
			BROADCAST_UPLOAD_RESULT_NONE = 0, BROADCAST_UPLOAD_RESULT_OK = 1, BROADCAST_UPLOAD_RESULT_INIT_FAILED = 2, BROADCAST_UPLOAD_RESULT_FRAME_FAILED = 3, BROADCAST_UPLOAD_RESULT_TIME_OUT = 4, BROADCAST_UPLOAD_RESULT_BANDWIDTH_EXCEEDED = 5, BROADCAST_UPLOAD_RESULT_LOW_FPS = 6, BROADCAST_UPLOAD_RESULT_MISSING_KEYFRAMES = 7,
			BROADCAST_UPLOAD_RESULT_NO_CONNECTION = 8, BROADCAST_UPLOAD_RESULT_RELAY_FAILED = 9, BROADCAST_UPLOAD_RESULT_SETTINGS_CHANGED = 10, BROADCAST_UPLOAD_RESULT_MISSING_AUDIO = 11, BROADCAST_UPLOAD_RESULT_TOO_FAR_BEHIND = 12, BROADCAST_UPLOAD_RESULT_TRANSCODE_BEHIND = 13
		};
		enum ChatEntryType {
			CHAT_ENTRY_TYPE_INVALID = 0, CHAT_ENTRY_TYPE_CHAT_MSG = 1, CHAT_ENTRY_TYPE_TYPING = 2, CHAT_ENTRY_TYPE_INVITE_GAME = 3, CHAT_ENTRY_TYPE_EMOTE = 4, CHAT_ENTRY_TYPE_LEFT_CONVERSATION = 6, CHAT_ENTRY_TYPE_ENTERED = 7, CHAT_ENTRY_TYPE_WAS_KICKED = 8, CHAT_ENTRY_TYPE_WAS_BANNED = 9, CHAT_ENTRY_TYPE_DISCONNECTED = 10, CHAT_ENTRY_TYPE_HISTORICAL_CHAT = 11,
			CHAT_ENTRY_TYPE_LINK_BLOCKED = 14
		};
		enum ChatRoomEnterResponse {
			CHAT_ROOM_ENTER_RESPONSE_SUCCESS = 1, CHAT_ROOM_ENTER_RESPONSE_DOESNT_EXIST = 2, CHAT_ROOM_ENTER_RESPONSE_NOT_ALLOWED = 3, CHAT_ROOM_ENTER_RESPONSE_FULL = 4, CHAT_ROOM_ENTER_RESPONSE_ERROR = 5, CHAT_ROOM_ENTER_RESPONSE_BANNED = 6, CHAT_ROOM_ENTER_RESPONSE_LIMITED = 7, CHAT_ROOM_ENTER_RESPONSE_CLAN_DISABLED = 8, CHAT_ROOM_ENTER_RESPONSE_COMMUNITY_BAN = 9,
			CHAT_ROOM_ENTER_RESPONSE_MEMBER_BLOCKED_YOU = 10, CHAT_ROOM_ENTER_RESPONSE_YOU_BLOCKED_MEMBER = 11
		};
		enum ChatSteamIDInstanceFlags {
			CHAT_ACCOUNT_INSTANCE_MASK = 0X00000FFF, CHAT_INSTANCE_FLAG_CLAN = ((k_unSteamAccountInstanceMask + 1) >> 1), CHAT_INSTANCE_FLAG_LOBBY = ((k_unSteamAccountInstanceMask + 1) >> 2), CHAT_INSTANCE_FLAG_MMS_LOBBY = ((k_unSteamAccountInstanceMask + 1) >> 3)
		};
		enum DenyReason {
			DENY_INVALID = 0, DENY_INVALID_VERSION = 1, DENY_GENERIC = 2, DENY_NOT_LOGGED_ON = 3, DENY_NO_LICENSE = 4, DENY_CHEATER = 5, DENY_LOGGED_IN_ELSEWHERE = 6, DENY_UNKNOWN_TEXT = 7, DENY_INCOMPATIBLE_ANTI_CHEAT = 8, DENY_MEMORY_CORRUPTION = 9, DENY_INCOMPATIBLE_SOFTWARE = 10, DENY_STEAM_CONNECTION_LOST = 11, DENY_STEAM_CONNECTION_ERROR = 12,
			DENY_STEAM_RESPONSE_TIMED_OUT = 13, DENY_STEAM_VALIDATION_STALLED = 14, DENY_STEAM_OWNER_LEFT_GUEST_USER = 15
		};
		enum GameIDType {
			GAME_TYPE_APP = 0, GAME_TYPE_GAME_MOD = 1, GAME_TYPE_SHORTCUT = 2, GAME_TYPE_P2P = 3
		};
		enum LAUNCH_OPTION_Type {
			LAUNCH_OPTION_TYPE_NONE = 0, LAUNCH_OPTION_TYPE_DEFAULT = 1, LAUNCH_OPTION_TYPE_SAFE_MODE = 2, LAUNCH_OPTION_TYPE_MULTIPLAYER = 3, LAUNCH_OPTION_TYPE_CONFIG = 4, LAUNCH_OPTION_TYPE_OPEN_VR = 5, LAUNCH_OPTION_TYPE_SERVER = 6, LAUNCH_OPTION_TYPE_EDITOR = 7, LAUNCH_OPTION_TYPE_MANUAL = 8, LAUNCH_OPTION_TYPE_BENCHMARK = 9, LAUNCH_OPTION_TYPE_OPTION1 = 10,
			LAUNCH_OPTION_TYPE_OPTION2 = 11, LAUNCH_OPTION_TYPE_OPTION3 = 12, LAUNCH_OPTION_TYPE_OCULUS_VR = 13, LAUNCH_OPTION_TYPE_OPEN_VR_OVERLAY = 14, LAUNCH_OPTION_TYPE_OS_VR = 15, LAUNCH_OPTION_TYPE_DIALOG = 1000
		};
		enum MarketingMessageFlags {
			MARKETING_MESSAGE_FLAGS_NONE = 0, MARKETING_MESSAGE_FLAGS_HIGH_PRIORITY = (1<<0), MARKETING_MESSAGE_FLAGS_PLATFORM_WINDOWS = (1<<1), MARKETING_MESSAGE_FLAGS_PLATFORM_MAC = (1<<2), MARKETING_MESSAGE_FLAGS_PLATFORM_LINUX = (1<<3), MARKETING_MESSAGE_FLAGS_PLATFORM_RESTRICTIONS
		};
		enum NotificationPosition {
			POSITION_TOP_LEFT = 0, POSITION_TOP_RIGHT = 1, POSITION_BOTTOM_LEFT = 2, POSITION_BOTTOM_RIGHT = 3
		};
		enum Result {
			RESULT_OK = 1, RESULT_FAIL = 2, RESULT_NO_CONNECTION = 3, RESULT_INVALID_PASSWORD = 5, RESULT_LOGGED_IN_ELSEWHERE = 6, RESULT_INVALID_PROTOCOL_VER = 7, RESULT_INVALID_PARAM = 8, RESULT_FILE_NOT_FOUND = 9, RESULT_BUSY = 10, RESULT_INVALID_STATE = 11, RESULT_INVALID_NAME = 12, RESULT_INVALID_EMAIL = 13, RESULT_DUPLICATE_NAME = 14, RESULT_ACCESS_DENIED = 15,
			RESULT_TIMEOUT = 16, RESULT_BANNED = 17, RESULT_ACCOUNT_NOT_FOUND = 18, RESULT_INVALID_STEAM_ID = 19, RESULT_SERVICE_UNAVAILABLE = 20, RESULT_NOT_LOGGED_ON = 21, RESULT_PENDING = 22, RESULT_ENCRYPTION_FAILURE = 23, RESULT_INSUFFICIENT_PRIVILEGE = 24, RESULT_LIMIT_EXCEEDED = 25, RESULT_REVOKED = 26, RESULT_EXPIRED = 27, RESULT_ALREADY_REDEEMED = 28,
			RESULT_DUPLICATE_REQUEST = 29, RESULT_ALREADY_OWNED = 30, RESULT_IP_NOT_FOUND = 31, RESULT_PERSIST_FAILED = 32, RESULT_LOCKING_FAILED = 33, RESULT_LOG_ON_SESSION_REPLACED = 34, RESULT_CONNECT_FAILED = 35, RESULT_HANDSHAKE_FAILED = 36, RESULT_IO_FAILURE = 37, RESULT_REMOTE_DISCONNECT = 38, RESULT_SHOPPING_CART_NOT_FOUND = 39, RESULT_BLOCKED = 40,
			RESULT_IGNORED = 41, RESULT_NO_MATCH = 42, RESULT_ACCOUNT_DISABLED = 43, RESULT_SERVICE_READ_ONLY = 44, RESULT_ACCOUNT_NOT_FEATURED = 45, RESULT_ADMINISTRATOR_OK = 46, RESULT_CONTENT_VERSION = 47, RESULT_TRY_ANOTHER_CM = 48, RESULT_PASSWORD_REQUIRED_TO_KICK_SESSION = 49, RESULT_ALREADY_LOGGED_IN_ELSEWHERE = 50, RESULT_SUSPENDED = 51, RESULT_CANCELLED = 52,
			RESULT_DATA_CORRUPTION = 53, RESULT_DISK_FULL = 54, RESULT_REMOTE_CALL_FAILED = 55, RESULT_PASSWORD_UNSET = 56, RESULT_EXTERNAL_ACCOUNT_UNLINKED = 57, RESULT_PSN_TICKET_INVALID = 58, RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED = 59, RESULT_REMOTE_FILE_CONFLICT = 60, RESULT_ILLEGAL_PASSWORD = 61, RESULT_SAME_AS_PREVIOUS_VALUE = 62,
			RESULT_ACCOUNT_LOG_ON_DENIED = 63, RESULT_CANNOT_USE_OLD_PASSWORD = 64, RESULT_INVALID_LOGIN_AUTH_CODE = 65, RESULT_ACCOUNT_LOG_ON_DENIED_NO_MAIL = 66, RESULT_HARDWARE_NOT_CAPABLE_OF_IPT = 67, RESULT_IPT_INIT_ERROR = 68, RESULT_PARENTAL_CONTROL_RESTRICTED = 69, RESULT_FACEBOOK_QUERY_ERROR = 70, RESULT_EXPIRED_LOGIN_AUTH_CODE = 71,
			RESULT_IP_LOGIN_RESTRICTION_FAILED = 72, RESULT_ACCOUNT_LOCKED_DOWN = 73, RESULT_ACCOUNT_LOG_ON_DENIED_VERIFIED_EMAIL_REQUIRED = 74, RESULT_NO_MATCHING_URL = 75, RESULT_BAD_RESPONSE = 76, RESULT_REQUIRE_PASSWORD_REENTRY = 77, RESULT_VALUE_OUT_OF_RANGE = 78, RESULT_UNEXPECTED_ERROR = 79, RESULT_DISABLED = 80, RESULT_INVALID_CEG_SUBMISSION = 81,
			RESULT_RESTRICTED_DEVICE = 82, RESULT_REGION_LOCKED = 83, RESULT_RATE_LIMIT_EXCEEDED = 84, RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR = 85, RESULT_ITEM_DELETED = 86, RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE = 87, RESULT_TWO_FACTOR_CODE_MISMATCH = 88, RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH = 89, RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS = 90,
			RESULT_NOT_MODIFIED = 91, RESULT_NO_MOBILE_DEVICE = 92, RESULT_TIME_NOT_SYNCED = 93, RESULT_SMS_CODE_FAILED = 94, RESULT_ACCOUNT_LIMIT_EXCEEDED = 95, RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED = 96, RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED = 97, RESULT_REFUND_TO_WALLET = 98, RESULT_EMAIL_SEND_FAILURE = 99, RESULT_NOT_SETTLED = 100, RESULT_NEED_CAPTCHA = 101,
			RESULT_GSLT_DENIED = 102, RESULT_GS_OWNER_DENIED = 103, RESULT_INVALID_ITEM_TYPE = 104, RESULT_IP_BANNED = 105, RESULT_GSLT_EXPIRED = 106, RESULT_INSUFFICIENT_FUNDS = 107, RESULT_TOO_MANY_PENDING = 108
		};
		enum SteamUserStatType {
			STEAM_USER_STAT_TYPE_INVALID = 0, STEAM_USER_STAT_TYPE_INT = 1, STEAM_USER_STAT_TYPE_FLOAT = 2, STEAM_USER_STAT_TYPE_AVGRATE = 3, STEAM_USER_STAT_TYPE_ACHIEVEMENTS = 4, STEAM_USER_STAT_TYPE_GROUPACHIEVEMENTS = 5, STEAM_USER_STAT_TYPE_MAX = 6
		};
		enum Universe {
			UNIVERSE_INVALID = 0, UNIVERSE_PUBLIC = 1, UNIVERSE_BETA = 2, UNIVERSE_INTERNAL = 3, UNIVERSE_DEV = 4, UNIVERSE_MAX = 5
		};
		enum UserHasLicenseForAppResult {
			USER_HAS_LICENSE_RESULT_HAS_LICENSE = 0, USER_HAS_LICENSE_RESULT_DOES_NOT_HAVE_LICENSE = 1, USER_HAS_LICENSE_RESULT_NO_AUTH = 2
		};
		enum VoiceResult {
			VOICE_RESULT_OK = 0, VOICE_RESULT_NOT_INITIALIZED = 1, VOICE_RESULT_NOT_RECORDING = 2, VOICE_RESULT_NO_DATE = 3, VOICE_RESULT_BUFFER_TOO_SMALL = 4, VOICE_RESULT_DATA_CORRUPTED = 5, VOICE_RESULT_RESTRICTED = 6
		};
		enum VRHMDType {
			VR_HMD_TYPE_NONE = -1, VR_HMD_TYPE_UNKNOWN = 0, VR_HMD_TYPE_HTC_DEV = 1, VR_HMD_TYPE_HTC_VIVEPRE = 2, VR_HMD_TYPE_HTC_VIVE = 3, VR_HMD_TYPE_HTC_UNKNOWN = 20, VR_HMD_TYPE_OCULUS_DK1 = 21, VR_HMD_TYPE_OCULUS_DK2 = 22, VR_HMD_TYPE_OCULUS_RIFT = 23, VR_HMD_TYPE_OCULUS_UNKNOWN = 40
		};
		// HTTP enums
		enum HTTPMethod {
			HTTP_METHOD_INVALID = 0, HTTP_METHOD_GET = 1, HTTP_METHOD_HEAD = 2, HTTP_METHOD_POST = 3, HTTP_METHOD_PUT = 4, HTTP_METHOD_DELETE = 5, HTTP_METHOD_OPTIONS = 6, HTTP_METHOD_PATCH = 7
		};
		enum HTTPStatusCode {
			HTTP_STATUS_CODE_INVALID = 0, HTTP_STATUS_CODE_100_CONTINUE = 100, HTTP_STATUS_CODE_101_SWITCHING_PROTOCOLS = 101, HTTP_STATUS_CODE_200_OK = 200, HTTP_STATUS_CODE_201_CREATED = 201, HTTP_STATUS_CODE_202_ACCEPTED = 202, HTTP_STATUS_CODE_203_NON_AUTHORITATIVE = 203, HTTP_STATUS_CODE_204_NO_CONTENT = 204, HTTP_STATUS_CODE_205_RESET_CONTENT = 205,
			HTTP_STATUS_CODE_206_PARTIAL_CONTENT = 206, HTTP_STATUS_CODE_300_MULTIPLE_CHOICES = 300, HTTP_STATUS_CODE_301_MOVED_PERMANENTLY = 301, HTTP_STATUS_CODE_302_FOUND = 302, HTTP_STATUS_CODE_303_SEE_OTHER = 303, HTTP_STATUS_CODE_304_NOT_MODIFIED = 304, HTTP_STATUS_CODE_305_USE_PROXY = 305, HTTP_STATUS_CODE_307_TEMPORARY_REDIRECT = 307,
			HTTP_STATUS_CODE_400_BAD_REQUEST = 400, HTTP_STATUS_CODE_401_UNAUTHORIZED = 401, HTTP_STATUS_CODE_402_PAYMENT_REQUIRED = 402, HTTP_STATUS_CODE_403_FORBIDDEN = 403, HTTP_STATUS_CODE_404_NOT_FOUND = 404, HTTP_STATUS_CODE_405_METHOD_NOT_ALLOWED = 405, HTTP_STATUS_CODE_406_NOT_ACCEPTABLE = 406, HTTP_STATUS_CODE_407_PROXY_AUTH_REQUIRED = 407,
			HTTP_STATUS_CODE_408_REQUEST_TIMEOUT = 408, HTTP_STATUS_CODE_409_CONFLICT = 409, HTTP_STATUS_CODE_410_GONE = 410, HTTP_STATUS_CODE_411_LENGTH_REQUIRED = 411, HTTP_STATUS_CODE_412_PRECONDITION_FAILED = 412, HTTP_STATUS_CODE_413_REQUEST_ENTITY_TOO_LARGE = 413, HTTP_STATUS_CODE_414_REQUEST_URI_TOO_LONG = 414, HTTP_STATUS_CODE_415_UNSUPPORTED_MEDIA_TYPE = 415,
			HTTP_STATUS_CODE_416_REQUESTED_RANGE_NOT_SATISFIABLE = 416, HTTP_STATUS_CODE_417_EXPECTATION_FAILED = 417, HTTP_STATUS_CODE_4XX_UNKNOWN = 418, HTTP_STATUS_CODE_429_TOO_MANY_REQUESTS = 429, HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR = 500, HTTP_STATUS_CODE_501_NOT_IMPLEMENTED = 501, HTTP_STATUS_CODE_502_BAD_GATEWAY = 502,
			HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE = 503, HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT = 504, HTTP_STATUS_CODE_505_HTTP_VERSION_NOT_SUPPORTED = 505, HTTP_STATUS_CODE_5XX_UNKNOWN = 599
		};
		// Inventory enums
		enum SteamItemFlags {
			STEAM_ITEM_NO_TRADE = (1<<0), STEAM_ITEM_REMOVED = (1<<8), STEAM_ITEM_CONSUMED = (1<<9)
		};
		// Networking enums
		enum P2PSend {
			P2P_SEND_UNRELIABLE = 0, P2P_SEND_UNRELIABLE_NO_DELAY = 1, P2P_SEND_RELIABLE = 2, P2P_SEND_RELIABLE_WITH_BUFFERING = 3
		};
		enum P2PSessionError {
			P2P_SESSION_ERROR_NONE = 0, P2P_SESSION_ERROR_NOT_RUNNING_APP = 1, P2P_SESSION_ERROR_NO_RIGHTS_TO_APP = 2, P2P_SESSION_ERROR_DESTINATION_NOT_LOGGED_ON = 3, P2P_SESSION_ERROR_TIMEOUT = 4, P2P_SESSION_ERROR_MAX = 5
		};
		enum SNetSocketConnectionType {
			NET_SOCKET_CONNECTION_TYPE_NOT_CONNECTED = 0, NET_SOCKET_CONNECTION_TYPE_UDP = 1, NET_SOCKET_CONNECTION_TYPE_UDP_RELAY = 2
		};
		enum SNetSocketState {
			NET_SOCKET_STATE_INVALID = 0, NET_SOCKET_STATE_CONNECTED = 1, NET_SOCKET_STATE_INITIATED = 10, NET_SOCKET_STATE_LOCAL_CANDIDATE_FOUND = 11, NET_SOCKET_STATE_RECEIVED_REMOTE_CANDIDATES = 12, NET_SOCKET_STATE_CHALLENGE_HANDSHAKE = 15, NET_SOCKET_STATE_DISCONNECTING = 21, NET_SOCKET_STATE_LOCAL_DISCONNECT = 22, NET_SOCKET_STATE_TIMEOUT_DURING_CONNECT = 23,
			NET_SOCKET_STATE_REMOTE_END_DISCONNECTED = 24, NET_SOCKET_STATE_BROKEN = 25
		};
		// UGC enums
		enum ItemPreviewType {
			ITEM_PREVIEW_TYPE_IMAGE = 0, ITEM_PREVIEW_TYPE_YOUTUBE_VIDEO = 1, ITEM_PREVIEW_TYPE_SKETCHFAB = 2, ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_HORIZONTAL_CROSS = 3, ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_LAT_LONG = 4, ITEM_PREVIEW_TYPE_RESERVED_MAX = 255
		};
		enum ItemState {
			ITEM_STATE_NONE = 0, ITEM_STATE_SUBSCRIBED = 1, ITEM_STATE_LEGACY_ITEM = 2, ITEM_STATE_INSTALLED = 4, ITEM_STATE_NEEDS_UPDATE = 8, ITEM_STATE_DOWNLOADING = 16, ITEM_STATE_DOWNLOAD_PENDING = 32
		};
		enum ItemStatistic {
			ITEM_STATISTIC_NUM_SUBSCRIPTIONS = 0, ITEM_STATISTIC_NUM_FAVORITES = 1, ITEM_STATISTIC_NUM_FOLLOWERS = 2, ITEM_STATISTIC_NUM_UNIQUE_SUBSCRIPTIONS = 3, ITEM_STATISTIC_NUM_UNIQUE_FAVORITES = 4, ITEM_STATISTIC_NUM_UNIQUE_FOLLOWERS = 5, ITEM_STATISTIC_NUM_UNIQUE_WEBSITE_VIEWS = 6, ITEM_STATISTIC_REPORT_SCORE = 7, ITEM_STATISTIC_NUM_SECONDS_PLAYED = 8,
			ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS = 9, ITEM_STATISTIC_NUM_COMMENTS = 10, ITEM_STATISTIC_NUM_SECONDS_PLAYED_DURING_TIME_PERIOD = 11, ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS_DURING_TIME_PERIOD = 12
		};
		enum ItemUpdateStatus {
			ITEM_UPDATE_STATUS_INVALID = 0, ITEM_UPDATE_STATUS_PREPARING_CONFIG = 1, ITEM_UPDATE_STATUS_PREPARING_CONTENT = 2, ITEM_UPDATE_STATUS_UPLOADING_CONTENT = 3, ITEM_UPDATE_STATUS_UPLOADING_PREVIEW_FILE = 4, ITEM_UPDATE_STATUS_COMMITTING_CHANGES = 5
		};
		enum UGCMatchingUGCType {
			UGCMATCHINGUGCTYPE_ITEMS = 0, UGC_MATCHING_UGC_TYPE_ITEMS_MTX = 1, UGC_MATCHING_UGC_TYPE_ITEMS_READY_TO_USE = 2, UGC_MATCHING_UGC_TYPE_COLLECTIONS = 3, UGC_MATCHING_UGC_TYPE_ARTWORK = 4, UGC_MATCHING_UGC_TYPE_VIDEOS = 5, UGC_MATCHING_UGC_TYPE_SCREENSHOTS = 6, UGC_MATCHING_UGC_TYPE_ALL_GUIDES = 7, UGC_MATCHING_UGC_TYPE_WEB_GUIDES = 8,
			UGC_MATCHING_UGC_TYPE_INTEGRATED_GUIDES = 9, UGC_MATCHING_UGC_TYPE_USABLE_IN_GAME = 10, UGC_MATCHING_UGC_TYPE_CONTROLLER_BINDINGS = 11, UGC_MATCHING_UGC_TYPE_GAME_MANAGED_ITEMS = 12, UGC_MATCHING_UGC_TYPE_ALL = ~0
		};
		enum UGCQuery {
			UGCQUERY_RANKED_BY_VOTE = 0, UGC_QUERY_RANKED_BY_PUBLICATION_DATE = 1, UGC_QUERY_ACCEPTED_FOR_GAME_RANKED_BY_ACCEPTANCE_DATE = 2, UGC_QUERY_RANKED_BY_TREND = 3, UGC_QUERY_FAVORITED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE = 4, UGC_QUERY_CREATED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE = 5, UGC_QUERY_RANKED_BY_NUM_TIMES_REPORTED = 6,
			UGC_QUERY_CREATED_BY_FOLLOWED_USERS_RANKED_BY_PUBLICATION_DATE = 7, UGC_QUERY_NOT_YET_RATED = 8, UGC_QUERY_RANKED_BY_TOTAL_VOTES_ASC = 9, UGC_QUERY_RANKED_BY_VOTES_UP = 10, UGC_QUERY_RANKED_BY_TEXT_SEARCH = 11, UGC_QUERY_RANKED_BY_TOTAL_UNIQUE_SUBSCRIPTIONS = 12, UGC_QUERY_RANKED_BY_PLAYTIME_TREND = 13, UGC_QUERY_RANKED_BY_TOTAL_PLAYTIME = 14,
			UGC_QUERY_RANKED_BY_AVERAGE_PLAYTIME_TREND = 15, UGC_QUERY_RANKED_BY_LIFETIME_AVERAGE_PLAYTIME = 16, UGC_QUERY_RANKED_BY_PLAYTIME_SESSIONS_TREND = 17, UGCQUERY_RANKED_BY_LIFETIME_PLAYTIME_SESSIONS = 18
		};
		enum UserUGCList {
			USER_UGC_LIST_PUBLISHED = 0, USER_UGC_LIST_VOTED_ON = 1, USER_UGC_LIST_VOTED_UP = 2, USER_UGC_LIST_VOTED_DOWN = 3, USER_UGC_LIST_FAVORITED = 5, USER_UGC_LIST_SUBSCRIBED = 6, USER_UGC_LIST_USED_OR_PLAYED = 7, USER_UGC_LIST_FOLLOWED = 8
		};
		enum UserUGCListSortOrder {
			USERUGCLISTSORTORDER_CREATIONORDERDESC = 0, USERUGCLISTSORTORDER_CREATIONORDERASC = 1, USERUGCLISTSORTORDER_TITLEASC = 2, USERUGCLISTSORTORDER_LASTUPDATEDDESC = 3, USERUGCLISTSORTORDER_SUBSCRIPTIONDATEDESC = 4, USERUGCLISTSORTORDER_VOTESCOREDESC = 5, USERUGCLISTSORTORDER_FORMODERATION = 6
		};
		// Utils enums
		enum CheckFileSignature {
			CHECK_FILE_SIGNATURE_INVALID_SIGNATURE = 0, CHECK_FILE_SIGNATURE_VALID_SIGNATURE = 1, CHECK_FILE_SIGNATURE_FILE_NOT_FOUND = 2, CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_APP = 3, CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_FILE = 4
		};
		enum GamepadTextInputLineMode {
			GAMEPAD_TEXT_INPUT_LINE_MODE_SINGLE_LINE = 0, GAMEPAD_TEXT_INPUT_LINE_MODE_MULTIPLE_LINES = 1
		};
		enum GamepadTextInputMode {
			GAMEPAD_TEXT_INPUT_MODE_NORMAL = 0, GAMEPAD_TEXT_INPUT_MODE_PASSWORD = 1
		};
		enum SteamAPICallFailure {
			STEAM_API_CALL_FAILURE_NONE = -1, STEAM_API_CALL_FAILURE_STEAM_GONE = 0, STEAM_API_CALL_FAILURE_NETWORK_FAILURE = 1, STEAM_API_CALL_FAILURE_INVALID_HANDLE = 2, STEAM_API_CALL_FAILURE_MISMATCHED_CALLBACK = 3
		};

		static SteamServer* get_singleton();
		SteamServer();
		~SteamServer();

		CSteamID createSteamID(uint64_t steamID, int accountType=-1);

		// Main /////////////////////////////////
		bool serverInit(Dictionary connectData, int serverMode, const String& versionString);
		void serverReleaseCurrentThreadMemory();
		void serverShutdown();

		// Apps /////////////////////////////////
		Array getDLCDataByIndex();
		bool isAppInstalled(int value);
		bool isCybercafe();
		bool isDLCInstalled(int value);
		bool isLowViolence();
		bool isSubscribed();
		bool isSubscribedApp(int value);
		bool isSubscribedFromFamilySharing();
		bool isSubscribedFromFreeWeekend();
		Dictionary isTimedTrial();
		bool isVACBanned();
		int getAppBuildId();
		String getAppInstallDir(AppId_t appID);
		uint64_t getAppOwner();
		String getAvailableGameLanguages();
		String getCurrentBetaName();
		String getCurrentGameLanguage();
		int getDLCCount();
		Dictionary getDLCDownloadProgress(uint32_t appID);
		int getEarliestPurchaseUnixTime(int value);
		void getFileDetails(const String& filename);
		Array getInstalledDepots(uint32_t appID);
		String getLaunchCommandLine();
		String getLaunchQueryParam(const String& key);
		void installDLC(int value);
		bool markContentCorrupt(bool missingFilesOnly);
		void uninstallDLC(int value);

		// HTTP /////////////////////////////////
		void createCookieContainer( bool allowResponsesToModify);
		void createHTTPRequest(int requestMethod, const String& absoluteURL);
		bool deferHTTPRequest(uint32 request);
		float getHTTPDownloadProgressPct(uint32 request);
		bool getHTTPRequestWasTimedOut(uint32 request);
		uint8 getHTTPResponseBodyData(uint32 request, uint32 bufferSize);
		uint32 getHTTPResponseBodySize(uint32 request);
		uint32 getHTTPResponseHeaderSize(uint32 request, const String& headerName);
		uint8 getHTTPResponseHeaderValue(uint32 request, const String& headerName, uint32 bufferSize);
		uint8 getHTTPStreamingResponseBodyData(uint32 request, uint32 offset, uint32 bufferSize);
		bool prioritizeHTTPRequest(uint32 request);
		bool releaseCookieContainer();
		bool releaseHTTPRequest(uint32 request);
		bool sendHTTPRequest(uint32 request);
		bool sendHTTPRequestAndStreamResponse(uint32 request);
		bool setCookie(const String& host, const String& url, const String& cookie);
		bool setHTTPRequestAbsoluteTimeoutMS(uint32 request, uint32 milliseconds);
		bool setHTTPRequestContextValue(uint32 request, uint64_t contextValue);
		bool setHTTPRequestCookieContainer(uint32 request);
		bool setHTTPRequestGetOrPostParameter(uint32 request, const String& name, const String& value);
		bool setHTTPRequestHeaderValue(uint32 request, const String& headerName, const String& headerValue);
		bool setHTTPRequestNetworkActivityTimeout(uint32 request, uint32 timeoutSeconds);
		uint8 setHTTPRequestRawPostBody(uint32 request, const String& contentType, uint32 bodyLen);
		bool setHTTPRequestRequiresVerifiedCertificate(uint32 request, bool requireVerifiedCertificate);
		bool setHTTPRequestUserAgentInfo(uint32 request, const String& userAgentInfo);

		// Inventory ////////////////////////////
		bool addPromoItem(uint32 item);
		bool addPromoItems(const PoolIntArray items);
		bool checkResultSteamID(uint64_t steamIDExpected);
		bool consumeItem(uint64_t itemConsume, uint32 quantity);
		bool deserializeResult();
		void destroyResult();
		bool exchangeItems(const PoolIntArray outputItems, const uint32 outputQuantity, const uint64_t inputItems, const uint32 inputQuantity);
		bool generateItems(const PoolIntArray items, const uint32 quantity);
		bool getAllItems();
		String getItemDefinitionProperty(uint32 definition, const String& name);
		bool getItemsByID(const uint64_t idArray, uint32 count);
		uint64_t getItemPrice(uint32 definition);
		Array getItemsWithPrices(uint32 length);
		uint32 getNumItemsWithPrices();
		String getResultItemProperty(uint32 index, const String& name);
		Array getResultItems();
		String getResultStatus();
		uint32 getResultTimestamp();
		bool grantPromoItems();
		bool loadItemDefinitions();
		void requestEligiblePromoItemDefinitionsIDs(uint64_t steamID);
		void requestPrices();
		bool serializeResult();
		void startPurchase(const PoolIntArray items, const uint32 quantity);
		bool transferItemQuantity(uint64_t itemID, uint32 quantity, uint64_t itemDestination, bool split);
		bool triggerItemDrop(uint32 definition);
		void startUpdateProperties();
		bool submitUpdateProperties();
		bool removeProperty(uint64_t itemID, const String& name);
		bool setPropertyString(uint64_t itemID, const String& name, const String& value);
		bool setPropertyBool(uint64_t itemID, const String& name, bool value);
		bool setPropertyInt(uint64_t itemID, const String& name, uint64_t value);
		bool setPropertyFloat(uint64_t itemID, const String& name, float value);

		// Networking ///////////////////////////
		bool acceptP2PSessionWithUser(uint64_t steamIDRemote);
		bool allowP2PPacketRelay(bool allow);
		bool closeP2PChannelWithUser(uint64_t steamIDRemote, int channel);
		bool closeP2PSessionWithUser(uint64_t steamIDRemote);
		Dictionary getP2PSessionState(uint64_t steamIDRemote);
		uint32_t getAvailableP2PPacketSize(int channel = 0);
		Dictionary readP2PPacket(uint32_t packet, int channel = 0);
		bool sendP2PPacket(uint64_t steamIDRemote, const PoolByteArray data, int eP2PSendType, int channel = 0);

		// Server ///////////////////////////////
		bool initGameServer(Dictionary connectData, int serverMode, const String& versionString);
		void setProduct(const String& product);
		void setGameDescription(const String& description);
		void setModDir(const String& modDir);
		void setDedicatedServer(bool dedicated);
		void logOn(const String& token);
		void logOnAnonymous();
		void logOff();
		bool loggedOn();
		bool secure();
		uint64_t getSteamID();
		bool wasRestartRequested();
		void setMaxPlayerCount(int max);
		void setBotPlayerCount(int bots);
		void setServerName(const String& name);
		void setMapName(const String& map);
		void setPasswordProtected(bool password);
		void setSpectatorPort(uint16 port);
		void setSpectatorServerName(const String& name);
		void clearAllKeyValues();
		void setKeyValue(const String& key, const String& value);
		void setGameTags(const String& tags);
		void setGameData(const String& data);
		void setRegion(const String& region);
		uint32_t getAuthSessionTicket();
		int beginAuthSession(uint32_t authTicket, uint64_t steamID);
		void endAuthSession(uint64_t steamID);
		void cancelAuthTicket(int authTicket);
		int userHasLicenceForApp(uint64_t steamID, AppId_t appID);
		bool requestUserGroupStatus(uint64_t steamID, int groupID);
		Dictionary handleIncomingPacket(int packet, const String& ip, uint16 port);
		Dictionary getNextOutgoingPacket();
		void enableHeartbeats(bool active);
		void setHeartbeatInterval(int interval);
		void forceHeartbeat();
		void associateWithClan(uint64_t clanID);
		void computeNewPlayerCompatibility(uint64_t steamID);

		// Server Stats /////////////////////////
		bool clearUserAchievement(uint64_t steamID, const String& name);
		Dictionary getUserAchievement(uint64_t steamID, const String& name);
		uint32_t getUserStatInt(uint64_t steamID, const String& name);
		float getUserStatFloat(uint64_t steamID, const String& name);
		void requestUserStats(uint64_t steamID);
		bool setUserAchievement(uint64_t steamID, const String& name);
		bool setUserStatInt(uint64_t steamID, const String& name, int32 stat);
		bool setUserStatFloat(uint64_t steamID, const String& name, float stat);
		void storeUserStats(uint64_t steamID);
		bool updateUserAvgRateStat(uint64_t steamID, const String& name, float thisSession, double sessionLength);

		// UGC //////////////////////////////////
		void addAppDependency(uint64_t publishedFileID, uint32_t appID);
		void addDependency(uint64_t publishedFileID, uint64_t childPublishedFileID);
		bool addExcludedTag(uint64_t queryHandle, const String& tagName);
		bool addItemKeyValueTag(uint64_t queryHandle, const String& key, const String& value);
		bool addItemPreviewFile(uint64_t queryHandle, const String& previewFile, int type);
		bool addItemPreviewVideo(uint64_t queryHandle, const String& videoID);
		void addItemToFavorite(uint32_t appID, uint64_t publishedFileID);
		bool addRequiredKeyValueTag(uint64_t queryHandle, const String& key, const String& value);
		bool addRequiredTag(uint64_t queryHandle, const String& tagName);
		bool initWorkshopForGameServer(uint32_t workshopDepotID);
		void createItem(AppId_t appID, int fileType);
		uint64_t createQueryAllUGCRequest(int queryType, int matchingType, uint32_t creatorID, uint32_t consumerID, uint32 page);
		uint64_t createQueryUGCDetailsRequest(Array publishedFileID);
//		uint64_t createQueryUserUGCRequest(int accountID, int listType, int matchingUGCType, int sortOrder, int creatorID, int consumerID, uint32 page);
		void deleteItem(uint64_t publishedFileID);
		bool downloadItem(uint64_t publishedFileID, bool highPriority);
		Dictionary getItemDownloadInfo(uint64_t publishedFileID);
		Dictionary getItemInstallInfo(uint64_t publishedFileID);
		int getItemState(uint64_t publishedFileID);
		Dictionary getItemUpdateProgress(uint64_t updateHandle);
		uint32 getNumSubscribedItems();
		Dictionary getQueryUGCAdditionalPreview(uint64_t queryHandle, uint32 index, uint32 previewIndex);
		Dictionary getQueryUGCChildren(uint64_t queryHandle, uint32 index);
		Dictionary getQueryUGCKeyValueTag(uint64_t queryHandle, uint32 index, uint32 keyValueTagIndex);
		String getQueryUGCMetadata(uint64_t queryHandle, uint32 index);
		uint32 getQueryUGCNumAdditionalPreviews(uint64_t queryHandle, uint32 index);
		uint32 getQueryUGCNumKeyValueTags(uint64_t queryHandle, uint32 index);
		String getQueryUGCPreviewURL(uint64_t queryHandle, uint32 index);
		Dictionary getQueryUGCResult(uint64_t queryHandle, uint32 index);
		Dictionary getQueryUGCStatistic(uint64_t queryHandle, uint32 index, int statType);
		Array getSubscribedItems();
		void getUserItemVote(uint64_t publishedFileID);
		bool releaseQueryUGCRequest(uint64_t queryHandle);
		void removeAppDependency(uint64_t publishedFileID, uint32_t appID);
		void removeDependency(uint64_t publishedFileID, uint64_t childPublishedFileID);
		void removeItemFromFavorites(uint32_t appID, uint64_t publishedFileID);
		bool removeItemKeyValueTags(uint64_t updateHandle, const String& key);
		bool removeItemPreview(uint64_t updateHandle, uint32 index);
		void sendQueryUGCRequest(uint64_t updateHandle);
		bool setAllowCachedResponse(uint64_t updateHandle, uint32 maxAgeSeconds);
		bool setCloudFileNameFilter(uint64_t updateHandle, const String& matchCloudFilename);
		bool setItemContent(uint64_t updateHandle, const String& contentFolder);
		bool setItemDescription(uint64_t updateHandle, const String& description);
		bool setItemMetadata(uint64_t updateHandle, const String& metadata);
		bool setItemPreview(uint64_t updateHandle, const String& previewFile);
		bool setItemTags(uint64_t updateHandle, Array tagArray);
		bool setItemTitle(uint64_t updateHandle, const String& title);
		bool setItemUpdateLanguage(uint64_t updateHandle, const String& language);
		bool setItemVisibility(uint64_t updateHandle, int visibility);
		bool setLanguage(uint64_t queryHandle, const String& language);
		bool setMatchAnyTag(uint64_t queryHandle, bool matchAnyTag);
		bool setRankedByTrendDays(uint64_t queryHandle, uint32 days);
		bool setReturnAdditionalPreviews(uint64_t queryHandle, bool returnAdditionalPreviews);
		bool setReturnChildren(uint64_t queryHandle, bool returnChildren);
		bool setReturnKeyValueTags(uint64_t queryHandle, bool returnKeyValueTags);
		bool setReturnLongDescription(uint64_t queryHandle, bool returnLongDescription);
		bool setReturnMetadata(uint64_t queryHandle, bool returnMetadata);
		bool setReturnOnlyIDs(uint64_t queryHandle, bool returnOnlyIDs);
		bool setReturnPlaytimeStats(uint64_t queryHandle, uint32 days);
		bool setReturnTotalOnly(uint64_t queryHandle, bool returnTotalOnly);
		bool setSearchText(uint64_t queryHandle, const String& searchText);
		void setUserItemVote(uint64_t publishedFileID, bool voteUp);
		uint64_t startItemUpdate(uint32_t appID, uint64_t fileId);
		void startPlaytimeTracking(Array publishedFileIDs);
		void stopPlaytimeTracking(Array publishedFileIDs);
		void stopPlaytimeTrackingForAllItems();
		void getAppDependencies(uint64_t publishedFileID);
		void submitItemUpdate(uint64_t updateHandle, const String& changeNote);
		void subscribeItem(uint64_t publishedFileID);
		void suspendDownloads(bool suspend);
		void unsubscribeItem(uint64_t publishedFileID);
		bool updateItemPreviewFile(uint64_t updateHandle, uint32 index, const String& previewFile);
		bool updateItemPreviewVideo(uint64_t updateHandle, uint32 index, const String& videoID);

		// Utils ////////////////////////////////
		String filterText(const String& message, bool legalOnly);
		String getAPICallFailureReason();
		int getAppID();
		int getCurrentBatteryPower();
		Dictionary getImageRGBA(int iImage);
		Dictionary getImageSize(int iImage);
		uint32 getIPCCallCount();
		String getIPCountry();
		int getSecondsSinceAppActive();
		int getSecondsSinceComputerActive();
		int getServerRealTime();
		String getSteamUILanguage();
		bool initFilterText();
		Dictionary isAPICallCompleted();
		bool isOverlayEnabled();
		bool isSteamChinaLauncher();
		bool isSteamInBigPictureMode();
		bool isSteamRunningInVR();
		bool isVRHeadsetStreamingEnabled();
		bool overlayNeedsPresent();
		void setOverlayNotificationInset(int horizontal, int vertical);
		void setOverlayNotificationPosition(int pos);
		void setVRHeadsetStreamingEnabled(bool enabled);
		bool showGamepadTextInput(int inputMode, int lineInputMode, const String& description, uint32 maxText, const String& presetText);
		void startVRDashboard();

	protected:
		static void _bind_methods();
		static SteamServer* singleton;

	private:
		bool isInitSuccess;

		// Apps
		uint64 currentAppID;

		// HTTP
		uint32 cookieHandle;
		uint32 requestHandle;

		// Inventory
		SteamInventoryUpdateHandle_t inventoryUpdateHandle;
		SteamInventoryResult_t inventoryHandle;
		SteamItemDetails_t inventoryDetails;

		// Utils
		uint64_t apiHandle = 0;

		/////////////////////////////////////////
		// STRUCTS //////////////////////////////
		/////////////////////////////////////////
		//
		// Authentication ///////////////////////
		struct TicketData {
			uint32_t id;
			uint32_t *buffer;
			uint32_t size;
		};
		Vector<TicketData> tickets;

		// UGC item details /////////////////////
		struct UGCDetails {
			uint64_t publishedFileID;
			int result;
			int fileType;
			int creatorAppID;
			int consumerAppID;
			char title[128];
			char description[256];
			uint64_t steamIDOwner;
			uint32 timeCreated;
			uint32 timeUpdated;
			uint32 timeAddedToList;
			int visibility;
			bool banned;
			bool acceptedForUse;
			bool tagsTruncated;
			char tags[256];
			int file;
			int previewFile;
			char filename[128];
			int32 fileSize;
			int32 previewFileSize;
			char url[256];
			uint32 votesUp;
			uint32 votesDown;
			float score;
			uint32 numChildren;
		};
		Vector<UGCDetails> ugcDetails;

		// Item Details /////////////////////////
		struct SteamItemDetails {
			SteamItemInstanceID_t itemID;
			SteamItemDef_t definition;
			uint16 quantity;
			uint16 flags;
		};
		Vector<SteamItemDetails> itemDetails;

		// Run the Steamworks server API callbacks
		void run_callbacks(){
			SteamGameServer_RunCallbacks();
		}

		/////////////////////////////////////////
		// STEAM SERVER CALLBACKS ///////////////
		/////////////////////////////////////////
		//
		// Apps callbacks ///////////////////////
		STEAM_GAMESERVER_CALLBACK(SteamServer, _dlc_installed, DlcInstalled_t, callbackDLCInstalled);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _file_details_result, FileDetailsResult_t, callbackFileDetailsResult);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _new_launch_url_parameters, NewUrlLaunchParameters_t, callbackNewLaunchURLParameters);
//		STEAM_GAMESERVER_CALLBACK(Steam, _new_launch_query_parameters, NewLaunchQueryParameters_t, callbackNewLaunchQueryParameters);  Seems not to be found?
		
		// HTTP callbacks ///////////////////////
		STEAM_GAMESERVER_CALLBACK(SteamServer, _http_request_completed, HTTPRequestCompleted_t, callbackHTTPRequestCompleted);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _http_request_data_received, HTTPRequestDataReceived_t, callbackHTTPRequestDataReceived);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _http_request_headers_received, HTTPRequestHeadersReceived_t, callbackHTTPRequestHeadersReceived);

		// Inventory callbacks //////////////////
		STEAM_GAMESERVER_CALLBACK(SteamServer, _inventory_definition_update, SteamInventoryDefinitionUpdate_t, callbackInventoryDefinitionUpdate);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _inventory_full_update, SteamInventoryFullUpdate_t, callbackInventoryFullUpdate);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _inventory_result_ready, SteamInventoryResultReady_t, callbackInventoryResultReady);

		// Networking callbacks /////////////////
		STEAM_GAMESERVER_CALLBACK(SteamServer, _p2p_session_connect_fail, P2PSessionConnectFail_t, callbackP2PSessionConnectFail);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _p2p_session_request, P2PSessionRequest_t, callbackP2PSessionRequest);

		// Server callbacks
		STEAM_GAMESERVER_CALLBACK(SteamServer, _server_Connect_Failure, SteamServerConnectFailure_t, callbackServerConnectFailure);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _server_Connected, SteamServersConnected_t, callbackServerConnected);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _server_Disconnected, SteamServersDisconnected_t, callbackServerDisconnected);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Approved, GSClientApprove_t, callbackClientApproved);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Denied, GSClientDeny_t, callbackClientDenied);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Kick, GSClientKick_t, callbackClientKicked);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _policy_Response, GSPolicyResponse_t, callbackPolicyResponse);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Group_Status, GSClientGroupStatus_t, callbackClientGroupStatus);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _associate_Clan, AssociateWithClanResult_t, callbackAssociateClan);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _player_Compat, ComputeNewPlayerCompatibilityResult_t, callbackPlayerCompat);

		// Server Stat callbacks
		STEAM_GAMESERVER_CALLBACK(SteamServer, _stats_stored, GSStatsStored_t, callbackStatsStored);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _stats_unloaded, GSStatsUnloaded_t, callbackStatsUnloaded);

		// UGC callbacks ////////////////////////
		STEAM_GAMESERVER_CALLBACK(SteamServer, _item_downloaded, DownloadItemResult_t, callbackItemDownloaded);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _item_installed, ItemInstalled_t, callbackItemInstalled);

		// Utility callbacks ////////////////////
		STEAM_GAMESERVER_CALLBACK(SteamServer, _gamepad_text_input_dismissed, GamepadTextInputDismissed_t, callbackGamepadTextInputDismissed);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _ip_country, IPCountry_t, callbackIPCountry);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _low_power, LowBatteryPower_t, callbackLowPower);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _steam_api_call_completed, SteamAPICallCompleted_t, callbackSteamAPICallCompleted);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _steam_shutdown, SteamShutdown_t, callbackSteamShutdown);

		/////////////////////////////////////////
		// STEAM CALL RESULTS ///////////////////
		/////////////////////////////////////////
		//
		// Inventory call results ///////////////
		CCallResult<SteamServer, SteamInventoryEligiblePromoItemDefIDs_t> callResultEligiblePromoItemDefIDs;
		void _inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *callData, bool ioFailure);
		CCallResult<SteamServer, SteamInventoryRequestPricesResult_t> callResultRequestPrices;
		void _inventory_request_prices_result(SteamInventoryRequestPricesResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, SteamInventoryStartPurchaseResult_t> callResultStartPurchase;
		void _inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *callData, bool ioFailure);

		// Remote Storage call results
		CCallResult<SteamServer, RemoteStorageUnsubscribePublishedFileResult_t> callResultUnsubscribeItem;
		void _unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, RemoteStorageSubscribePublishedFileResult_t> callResultSubscribeItem;
		void _subscribe_item(RemoteStorageSubscribePublishedFileResult_t *callData, bool ioFailure);

		// UGC call results /////////////////////
		CCallResult<SteamServer, AddAppDependencyResult_t> callResultAddAppDependency;
		void _add_app_dependency_result(AddAppDependencyResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, AddUGCDependencyResult_t> callResultAddUGCDependency;
		void _add_ugc_dependency_result(AddUGCDependencyResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, CreateItemResult_t> callResultItemCreate;
		void _item_created(CreateItemResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, GetAppDependenciesResult_t> callResultGetAppDependencies;
		void _get_app_dependencies_result(GetAppDependenciesResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, DeleteItemResult_t> callResultDeleteItem;
		void _item_deleted(DeleteItemResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, GetUserItemVoteResult_t> callResultGetUserItemVote;
		void _get_item_vote_result(GetUserItemVoteResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, RemoveAppDependencyResult_t> callResultRemoveAppDependency;
		void _remove_app_dependency_result(RemoveAppDependencyResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, RemoveUGCDependencyResult_t> callResultRemoveUGCDependency;
		void _remove_ugc_dependency_result(RemoveUGCDependencyResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, SetUserItemVoteResult_t> callResultSetUserItemVote;
		void _set_user_item_vote(SetUserItemVoteResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, StartPlaytimeTrackingResult_t> callResultStartPlaytimeTracking;
		void _start_playtime_tracking(StartPlaytimeTrackingResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, SteamUGCQueryCompleted_t> callResultUGCQueryCompleted;
		void _ugc_query_completed(SteamUGCQueryCompleted_t *callData, bool ioFailure);
		CCallResult<SteamServer, StopPlaytimeTrackingResult_t> callResultStopPlaytimeTracking;
		void _stop_playtime_tracking(StopPlaytimeTrackingResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, SubmitItemUpdateResult_t> callResultItemUpdate;
		void _item_updated(SubmitItemUpdateResult_t *callData, bool ioFailure);
		CCallResult<SteamServer, UserFavoriteItemsListChanged_t> callResultFavoriteItemListChanged;
		void _user_favorite_items_list_changed(UserFavoriteItemsListChanged_t *callData, bool ioFailure);

		// Server Stats call results ////////////
		CCallResult<SteamServer, GSStatsReceived_t> callResultStatReceived;
		void _stat_received(GSStatsReceived_t *callData, bool ioFailure);

		// Utility call results /////////////////
		CCallResult<SteamServer, CheckFileSignature_t> callResultCheckFileSignature;
		void _check_file_signature(CheckFileSignature_t *callData, bool ioFailure);
};
#endif // GODOTSTEAM_SERVER_H