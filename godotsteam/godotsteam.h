#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H


// SILENCE STEAMWORKS WARNINGS
/////////////////////////////////////////////////
//
// Turn off MSVC-only warning about strcpy
#include <cstdint>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable : 4996)
#pragma warning(disable : 4828)
#endif


// INCLUDE HEADERS
/////////////////////////////////////////////////
//
// Include INT types header
#include <inttypes.h>

// Include Steamworks API headers
#include "steam/steam_api.h"
#include "steam/steamnetworkingfakeip.h"
#include "steam/isteamdualsense.h"

// Include Godot headers
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/dictionary.hpp>

// Include GodotSteam headers
#include "godotsteam_constants.h"

// Include some system headers
#include "map"

using namespace godot;


class Steam: public Object {
	GDCLASS(Steam, Object);


public:
	static Steam *get_singleton();
	Steam();
	~Steam();


	// STEAMWORKS API ENUMS
	/////////////////////////////////////////
	//
	enum AccountType {
		// Found in steamclientpublic.h
		ACCOUNT_TYPE_INVALID = k_EAccountTypeInvalid,
		ACCOUNT_TYPE_INDIVIDUAL = k_EAccountTypeIndividual,
		ACCOUNT_TYPE_MULTISEAT = k_EAccountTypeMultiseat,
		ACCOUNT_TYPE_GAME_SERVER = k_EAccountTypeGameServer,
		ACCOUNT_TYPE_ANON_GAME_SERVER = k_EAccountTypeAnonGameServer,
		ACCOUNT_TYPE_PENDING = k_EAccountTypePending,
		ACCOUNT_TYPE_CONTENT_SERVER = k_EAccountTypeContentServer,
		ACCOUNT_TYPE_CLAN = k_EAccountTypeClan,
		ACCOUNT_TYPE_CHAT = k_EAccountTypeChat,
		ACCOUNT_TYPE_CONSOLE_USER = k_EAccountTypeConsoleUser,
		ACCOUNT_TYPE_ANON_USER = k_EAccountTypeAnonUser,
		ACCOUNT_TYPE_MAX = k_EAccountTypeMax
	};
	enum AuthSessionResponse {
		// Found in steamclientpublic.h
		AUTH_SESSION_RESPONSE_OK = k_EAuthSessionResponseOK,
		AUTH_SESSION_RESPONSE_USER_NOT_CONNECTED_TO_STEAM = k_EAuthSessionResponseUserNotConnectedToSteam,
		AUTH_SESSION_RESPONSE_NO_LICENSE_OR_EXPIRED = k_EAuthSessionResponseNoLicenseOrExpired,
		AUTH_SESSION_RESPONSE_VAC_BANNED = k_EAuthSessionResponseVACBanned,
		AUTH_SESSION_RESPONSE_LOGGED_IN_ELSEWHERE = k_EAuthSessionResponseLoggedInElseWhere,
		AUTH_SESSION_RESPONSE_VAC_CHECK_TIMED_OUT = k_EAuthSessionResponseVACCheckTimedOut,
		AUTH_SESSION_RESPONSE_AUTH_TICKET_CANCELED = k_EAuthSessionResponseAuthTicketCanceled,
		AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID_ALREADY_USED = k_EAuthSessionResponseAuthTicketInvalidAlreadyUsed,
		AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID = k_EAuthSessionResponseAuthTicketInvalid,
		AUTH_SESSION_RESPONSE_PUBLISHER_ISSUED_BAN = k_EAuthSessionResponsePublisherIssuedBan,
		AUTH_SESSION_RESPONSE_AUTH_TICKET_NETWORK_IDENTITY_FAILURE = k_EAuthSessionResponseAuthTicketNetworkIdentityFailure
	};
	enum BeginAuthSessionResult {
		// Found in steamclientpublic.h
		BEGIN_AUTH_SESSION_RESULT_OK = k_EBeginAuthSessionResultOK,
		BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET = k_EBeginAuthSessionResultInvalidTicket,
		BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST = k_EBeginAuthSessionResultDuplicateRequest,
		BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION = k_EBeginAuthSessionResultInvalidVersion,
		BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH = k_EBeginAuthSessionResultGameMismatch,
		BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET = k_EBeginAuthSessionResultExpiredTicket
	};
	enum BroadcastUploadResult {
		 // Found in steamclientpublic.h
		BROADCAST_UPLOAD_RESULT_NONE = k_EBroadcastUploadResultNone,
		BROADCAST_UPLOAD_RESULT_OK = k_EBroadcastUploadResultOK,
		BROADCAST_UPLOAD_RESULT_INIT_FAILED = k_EBroadcastUploadResultInitFailed,
		BROADCAST_UPLOAD_RESULT_FRAME_FAILED = k_EBroadcastUploadResultFrameFailed,
		BROADCAST_UPLOAD_RESULT_TIME_OUT = k_EBroadcastUploadResultTimeout,
		BROADCAST_UPLOAD_RESULT_BANDWIDTH_EXCEEDED = k_EBroadcastUploadResultBandwidthExceeded,
		BROADCAST_UPLOAD_RESULT_LOW_FPS = k_EBroadcastUploadResultLowFPS,
		BROADCAST_UPLOAD_RESULT_MISSING_KEYFRAMES = k_EBroadcastUploadResultMissingKeyFrames,
		BROADCAST_UPLOAD_RESULT_NO_CONNECTION = k_EBroadcastUploadResultNoConnection,
		BROADCAST_UPLOAD_RESULT_RELAY_FAILED = k_EBroadcastUploadResultRelayFailed,
		BROADCAST_UPLOAD_RESULT_SETTINGS_CHANGED = k_EBroadcastUploadResultSettingsChanged,
		BROADCAST_UPLOAD_RESULT_MISSING_AUDIO = k_EBroadcastUploadResultMissingAudio,
		BROADCAST_UPLOAD_RESULT_TOO_FAR_BEHIND = k_EBroadcastUploadResultTooFarBehind,
		BROADCAST_UPLOAD_RESULT_TRANSCODE_BEHIND = k_EBroadcastUploadResultTranscodeBehind,
		BROADCAST_UPLOAD_RESULT_NOT_ALLOWED_TO_PLAY = k_EBroadcastUploadResultNotAllowedToPlay,
		BROADCAST_UPLOAD_RESULT_BUSY = k_EBroadcastUploadResultBusy,
		BROADCAST_UPLOAD_RESULT_BANNED = k_EBroadcastUploadResultBanned,
		BROADCAST_UPLOAD_RESULT_ALREADY_ACTIVE = k_EBroadcastUploadResultAlreadyActive,
		BROADCAST_UPLOAD_RESULT_FORCED_OFF = k_EBroadcastUploadResultForcedOff,
		BROADCAST_UPLOAD_RESULT_AUDIO_BEHIND = k_EBroadcastUploadResultAudioBehind,
		BROADCAST_UPLOAD_RESULT_SHUTDOWN = k_EBroadcastUploadResultShutdown,
		BROADCAST_UPLOAD_RESULT_DISCONNECT = k_EBroadcastUploadResultDisconnect,
		BROADCAST_UPLOAD_RESULT_VIDEO_INIT_FAILED = k_EBroadcastUploadResultVideoInitFailed,
		BROADCAST_UPLOAD_RESULT_AUDIO_INIT_FAILED = k_EBroadcastUploadResultAudioInitFailed
	};
	enum ChatEntryType {
		// Found in steamclientpublic.h
		CHAT_ENTRY_TYPE_INVALID = k_EChatEntryTypeInvalid,
		CHAT_ENTRY_TYPE_CHAT_MSG = k_EChatEntryTypeChatMsg,
		CHAT_ENTRY_TYPE_TYPING = k_EChatEntryTypeTyping,
		CHAT_ENTRY_TYPE_INVITE_GAME = k_EChatEntryTypeInviteGame,
		CHAT_ENTRY_TYPE_EMOTE = k_EChatEntryTypeEmote,
		//			CHAT_ENTRY_TYPE_LOBBY_GAME_START = k_EChatEntryTypeLobbyGameStart,
		CHAT_ENTRY_TYPE_LEFT_CONVERSATION = k_EChatEntryTypeLeftConversation,
		CHAT_ENTRY_TYPE_ENTERED = k_EChatEntryTypeEntered,
		CHAT_ENTRY_TYPE_WAS_KICKED = k_EChatEntryTypeWasKicked,
		CHAT_ENTRY_TYPE_WAS_BANNED = k_EChatEntryTypeWasBanned,
		CHAT_ENTRY_TYPE_DISCONNECTED = k_EChatEntryTypeDisconnected,
		CHAT_ENTRY_TYPE_HISTORICAL_CHAT = k_EChatEntryTypeHistoricalChat,
		CHAT_ENTRY_TYPE_LINK_BLOCKED = k_EChatEntryTypeLinkBlocked
	};
	enum ChatRoomEnterResponse {
		// Found in steamclientpublic.h
		CHAT_ROOM_ENTER_RESPONSE_SUCCESS = k_EChatRoomEnterResponseSuccess,
		CHAT_ROOM_ENTER_RESPONSE_DOESNT_EXIST = k_EChatRoomEnterResponseDoesntExist,
		CHAT_ROOM_ENTER_RESPONSE_NOT_ALLOWED = k_EChatRoomEnterResponseNotAllowed,
		CHAT_ROOM_ENTER_RESPONSE_FULL = k_EChatRoomEnterResponseFull,
		CHAT_ROOM_ENTER_RESPONSE_ERROR = k_EChatRoomEnterResponseError,
		CHAT_ROOM_ENTER_RESPONSE_BANNED = k_EChatRoomEnterResponseBanned,
		CHAT_ROOM_ENTER_RESPONSE_LIMITED = k_EChatRoomEnterResponseLimited,
		CHAT_ROOM_ENTER_RESPONSE_CLAN_DISABLED = k_EChatRoomEnterResponseClanDisabled,
		CHAT_ROOM_ENTER_RESPONSE_COMMUNITY_BAN = k_EChatRoomEnterResponseCommunityBan,
		CHAT_ROOM_ENTER_RESPONSE_MEMBER_BLOCKED_YOU = k_EChatRoomEnterResponseMemberBlockedYou,
		CHAT_ROOM_ENTER_RESPONSE_YOU_BLOCKED_MEMBER = k_EChatRoomEnterResponseYouBlockedMember,
		CHAT_ROOM_ENTER_RESPONSE_RATE_LIMIT_EXCEEDED = k_EChatRoomEnterResponseRatelimitExceeded
	};
	enum ChatSteamIDInstanceFlags {
		// Found in steamclientpublic.h
		CHAT_ACCOUNT_INSTANCE_MASK = k_EChatAccountInstanceMask,
		CHAT_INSTANCE_FLAG_CLAN = k_EChatInstanceFlagClan,
		CHAT_INSTANCE_FLAG_LOBBY = k_EChatInstanceFlagLobby,
		CHAT_INSTANCE_FLAG_MMS_LOBBY = k_EChatInstanceFlagMMSLobby
	};
	enum DenyReason {
		// Found in steamclientpublic.h
		DENY_INVALID = k_EDenyInvalid,
		DENY_INVALID_VERSION = k_EDenyInvalidVersion,
		DENY_GENERIC = k_EDenyGeneric,
		DENY_NOT_LOGGED_ON = k_EDenyNotLoggedOn,
		DENY_NO_LICENSE = k_EDenyNoLicense,
		DENY_CHEATER = k_EDenyCheater,
		DENY_LOGGED_IN_ELSEWHERE = k_EDenyLoggedInElseWhere,
		DENY_UNKNOWN_TEXT = k_EDenyUnknownText,
		DENY_INCOMPATIBLE_ANTI_CHEAT = k_EDenyIncompatibleAnticheat,
		DENY_MEMORY_CORRUPTION = k_EDenyMemoryCorruption,
		DENY_INCOMPATIBLE_SOFTWARE = k_EDenyIncompatibleSoftware,
		DENY_STEAM_CONNECTION_LOST = k_EDenySteamConnectionLost,
		DENY_STEAM_CONNECTION_ERROR = k_EDenySteamConnectionError,
		DENY_STEAM_RESPONSE_TIMED_OUT = k_EDenySteamResponseTimedOut,
		DENY_STEAM_VALIDATION_STALLED = k_EDenySteamValidationStalled,
		DENY_STEAM_OWNER_LEFT_GUEST_USER = k_EDenySteamOwnerLeftGuestUser
	};
	enum GameIDType {
		GAME_TYPE_APP = CGameID::k_EGameIDTypeApp,
		GAME_TYPE_GAME_MOD = CGameID::k_EGameIDTypeGameMod,
		GAME_TYPE_SHORTCUT = CGameID::k_EGameIDTypeShortcut,
		GAME_TYPE_P2P = CGameID::k_EGameIDTypeP2P
	};
	enum IPType {
		IP_TYPE_IPV4 = k_ESteamIPTypeIPv4,
		IP_TYPE_IPV6 = k_ESteamIPTypeIPv6
	};
	enum IPv6ConnectivityProtocol {
		// Found in steamclientpublic.h
		IPV6_CONNECTIVITY_PROTOCOL_INVALID = k_ESteamIPv6ConnectivityProtocol_Invalid,
		IPV6_CONNECTIVITY_PROTOCOL_HTTP = k_ESteamIPv6ConnectivityProtocol_HTTP,
		IPV6_CONNECTIVITY_PROTOCOL_UDP = k_ESteamIPv6ConnectivityProtocol_UDP
	};
	enum IPv6ConnectivityState {
		// Found in steamclientpublic.h
		IPV6_CONNECTIVITY_STATE_UNKNOWN = k_ESteamIPv6ConnectivityState_Unknown,
		IPV6_CONNECTIVITY_STATE_GOOD = k_ESteamIPv6ConnectivityState_Good,
		IPV6_CONNECTIVITY_STATE_BAD = k_ESteamIPv6ConnectivityState_Bad
	};
	enum MarketNotAllowedReasonFlags {
		// Found in steamclientpublic.h
		MARKET_NOT_ALLOWED_REASON_NONE = k_EMarketNotAllowedReason_None,
		MARKET_NOT_ALLOWED_REASON_TEMPORARY_FAILURE = k_EMarketNotAllowedReason_TemporaryFailure,
		MARKET_NOT_ALLOWED_REASON_ACCOUNT_DISABLED = k_EMarketNotAllowedReason_AccountDisabled,
		MARKET_NOT_ALLOWED_REASON_ACCOUNT_LOCKED_DOWN = k_EMarketNotAllowedReason_AccountLockedDown,
		MARKET_NOT_ALLOWED_REASON_ACCOUNT_LIMITED = k_EMarketNotAllowedReason_AccountLimited,
		MARKET_NOT_ALLOWED_REASON_TRADE_BANNED = k_EMarketNotAllowedReason_TradeBanned,
		MARKET_NOT_ALLOWED_REASON_ACCOUNT_NOT_TRUSTED = k_EMarketNotAllowedReason_AccountNotTrusted,
		MARKET_NOT_ALLOWED_REASON_STEAM_GUARD_NOT_ENABLED = k_EMarketNotAllowedReason_SteamGuardNotEnabled,
		MARKET_NOT_ALLOWED_REASON_STEAM_GAURD_ONLY_RECENTLY_ENABLED = k_EMarketNotAllowedReason_SteamGuardOnlyRecentlyEnabled,
		MARKET_NOT_ALLOWED_REASON_RECENT_PASSWORD_RESET = k_EMarketNotAllowedReason_RecentPasswordReset,
		MARKET_NOT_ALLOWED_REASON_NEW_PAYMENT_METHOD = k_EMarketNotAllowedReason_NewPaymentMethod,
		MARKET_NOT_ALLOWED_REASON_INVALID_COOKIE = k_EMarketNotAllowedReason_InvalidCookie,
		MARKET_NOT_ALLOWED_REASON_USING_NEW_DEVICE = k_EMarketNotAllowedReason_UsingNewDevice,
		MARKET_NOT_ALLOWED_REASON_RECENT_SELF_REFUND = k_EMarketNotAllowedReason_RecentSelfRefund,
		MARKET_NOT_ALLOWED_REASON_NEW_PAYMENT_METHOD_CANNOT_BE_VERIFIED = k_EMarketNotAllowedReason_NewPaymentMethodCannotBeVerified,
		MARKET_NOT_ALLOWED_REASON_NO_RECENT_PURCHASES = k_EMarketNotAllowedReason_NoRecentPurchases,
		MARKET_NOT_ALLOWED_REASON_ACCEPTED_WALLET_GIFT = k_EMarketNotAllowedReason_AcceptedWalletGift
	};
	enum NotificationPosition {
		// Found in steamclientpublic.h
		POSITION_INVALID = k_EPositionInvalid,
		POSITION_TOP_LEFT = k_EPositionTopLeft,
		POSITION_TOP_RIGHT = k_EPositionTopRight,
		POSITION_BOTTOM_LEFT = k_EPositionBottomLeft,
		POSITION_BOTTOM_RIGHT = k_EPositionBottomRight
	};
	enum Result {
		// Found in steamclientpublic.h
		RESULT_NONE = k_EResultNone,
		RESULT_OK = k_EResultOK,
		RESULT_FAIL = k_EResultFail,
		RESULT_NO_CONNECTION = k_EResultNoConnection,
		RESULT_INVALID_PASSWORD = k_EResultInvalidPassword,
		RESULT_LOGGED_IN_ELSEWHERE = k_EResultLoggedInElsewhere,
		RESULT_INVALID_PROTOCOL_VER = k_EResultInvalidProtocolVer,
		RESULT_INVALID_PARAM = k_EResultInvalidParam,
		RESULT_FILE_NOT_FOUND = k_EResultFileNotFound,
		RESULT_BUSY = k_EResultBusy,
		RESULT_INVALID_STATE = k_EResultInvalidState,
		RESULT_INVALID_NAME = k_EResultInvalidName,
		RESULT_INVALID_EMAIL = k_EResultInvalidEmail,
		RESULT_DUPLICATE_NAME = k_EResultDuplicateName,
		RESULT_ACCESS_DENIED = k_EResultAccessDenied,
		RESULT_TIMEOUT = k_EResultTimeout,
		RESULT_BANNED = k_EResultBanned,
		RESULT_ACCOUNT_NOT_FOUND = k_EResultAccountNotFound,
		RESULT_INVALID_STEAMID = k_EResultInvalidSteamID,
		RESULT_SERVICE_UNAVAILABLE = k_EResultServiceUnavailable,
		RESULT_NOT_LOGGED_ON = k_EResultNotLoggedOn,
		RESULT_PENDING = k_EResultPending,
		RESULT_ENCRYPTION_FAILURE = k_EResultEncryptionFailure,
		RESULT_INSUFFICIENT_PRIVILEGE = k_EResultInsufficientPrivilege,
		RESULT_LIMIT_EXCEEDED = k_EResultLimitExceeded,
		RESULT_REVOKED = k_EResultRevoked,
		RESULT_EXPIRED = k_EResultExpired,
		RESULT_ALREADY_REDEEMED = k_EResultAlreadyRedeemed,
		RESULT_DUPLICATE_REQUEST = k_EResultDuplicateRequest,
		RESULT_ALREADY_OWNED = k_EResultAlreadyOwned,
		RESULT_IP_NOT_FOUND = k_EResultIPNotFound,
		RESULT_PERSIST_FAILED = k_EResultPersistFailed,
		RESULT_LOCKING_FAILED = k_EResultLockingFailed,
		RESULT_LOG_ON_SESSION_REPLACED = k_EResultLogonSessionReplaced,
		RESULT_CONNECT_FAILED = k_EResultConnectFailed,
		RESULT_HANDSHAKE_FAILED = k_EResultHandshakeFailed,
		RESULT_IO_FAILURE = k_EResultIOFailure,
		RESULT_REMOTE_DISCONNECT = k_EResultRemoteDisconnect,
		RESULT_SHOPPING_CART_NOT_FOUND = k_EResultShoppingCartNotFound,
		RESULT_BLOCKED = k_EResultBlocked,
		RESULT_IGNORED = k_EResultIgnored,
		RESULT_NO_MATCH = k_EResultNoMatch,
		RESULT_ACCOUNT_DISABLED = k_EResultAccountDisabled,
		RESULT_SERVICE_READ_ONLY = k_EResultServiceReadOnly,
		RESULT_ACCOUNT_NOT_FEATURED = k_EResultAccountNotFeatured,
		RESULT_ADMINISTRATO_ROK = k_EResultAdministratorOK,
		RESULT_CONTENT_VERSION = k_EResultContentVersion,
		RESULT_TRY_ANOTHER_CM = k_EResultTryAnotherCM,
		RESULT_PASSWORD_REQUIRED_TO_KICK_SESSION = k_EResultPasswordRequiredToKickSession,
		RESULT_ALREADY_LOGGED_IN_ELSEWHERE = k_EResultAlreadyLoggedInElsewhere,
		RESULT_SUSPENDED = k_EResultSuspended,
		RESULT_CANCELLED = k_EResultCancelled,
		RESULT_DATA_CORRUPTION = k_EResultDataCorruption,
		RESULT_DISK_FULL = k_EResultDiskFull,
		RESULT_REMOTE_CALL_FAILED = k_EResultRemoteCallFailed,
		RESULT_PASSWORD_UNSET = k_EResultPasswordUnset,
		RESULT_EXTERNAL_ACCOUNT_UNLINKED = k_EResultExternalAccountUnlinked,
		RESULT_PSN_TICKET_INVALID = k_EResultPSNTicketInvalid,
		RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED = k_EResultExternalAccountAlreadyLinked,
		RESULT_REMOTE_FILE_CONFLICT = k_EResultRemoteFileConflict,
		RESULT_ILLEGAL_PASSWORD = k_EResultIllegalPassword,
		RESULT_SAME_AS_PREVIOUS_VALUE = k_EResultSameAsPreviousValue,
		RESULT_ACCOUNT_LOG_ON_DENIED = k_EResultAccountLogonDenied,
		RESULT_CANNOT_USE_OLD_PASSWORD = k_EResultCannotUseOldPassword,
		RESULT_INVALID_LOG_IN_AUTH_CODE = k_EResultInvalidLoginAuthCode,
		RESULT_ACCOUNT_LOG_ON_DENIED_NO_MAIL = k_EResultAccountLogonDeniedNoMail,
		RESULT_HARDWARE_NOT_CAPABLE_OF_IPT = k_EResultHardwareNotCapableOfIPT,
		RESULT_IPT_INIT_ERROR = k_EResultIPTInitError,
		RESULT_PARENTAL_CONTROL_RESTRICTED = k_EResultParentalControlRestricted,
		RESULT_FACEBOOK_QUERY_ERROR = k_EResultFacebookQueryError,
		RESULT_EXPIRED_LOGIN_AUTH_CODE = k_EResultExpiredLoginAuthCode,
		RESULT_IP_LOGIN_RESTRICTION_FAILED = k_EResultIPLoginRestrictionFailed,
		RESULT_ACCOUNT_LOCKED_DOWN = k_EResultAccountLockedDown,
		RESULT_ACCOUNT_LOG_ON_DENIED_VERIFIED_EMAIL_REQUIRED = k_EResultAccountLogonDeniedVerifiedEmailRequired,
		RESULT_NO_MATCHING_URL = k_EResultNoMatchingURL,
		RESULT_BAD_RESPONSE = k_EResultBadResponse,
		RESULT_REQUIRE_PASSWORD_REENTRY = k_EResultRequirePasswordReEntry,
		RESULT_VALUE_OUT_OF_RANGE = k_EResultValueOutOfRange,
		RESULT_UNEXPECTED_ERROR = k_EResultUnexpectedError,
		RESULT_DISABLED = k_EResultDisabled,
		RESULT_INVALID_CEG_SUBMISSION = k_EResultInvalidCEGSubmission,
		RESULT_RESTRICTED_DEVICE = k_EResultRestrictedDevice,
		RESULT_REGION_LOCKED = k_EResultRegionLocked,
		RESULT_RATE_LIMIT_EXCEEDED = k_EResultRateLimitExceeded,
		RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR = k_EResultAccountLoginDeniedNeedTwoFactor,
		RESULT_ITEM_DELETED = k_EResultItemDeleted,
		RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE = k_EResultAccountLoginDeniedThrottle,
		RESULT_TWO_FACTOR_CODE_MISMATCH = k_EResultTwoFactorCodeMismatch,
		RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH = k_EResultTwoFactorActivationCodeMismatch,
		RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS = k_EResultAccountAssociatedToMultiplePartners,
		RESULT_NOT_MODIFIED = k_EResultNotModified,
		RESULT_NO_MOBILE_DEVICE = k_EResultNoMobileDevice,
		RESULT_TIME_NOT_SYNCED = k_EResultTimeNotSynced,
		RESULT_SMS_CODE_FAILED = k_EResultSmsCodeFailed,
		RESULT_ACCOUNT_LIMIT_EXCEEDED = k_EResultAccountLimitExceeded,
		RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED = k_EResultAccountActivityLimitExceeded,
		RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED = k_EResultPhoneActivityLimitExceeded,
		RESULT_REFUND_TO_WALLET = k_EResultRefundToWallet,
		RESULT_EMAIL_SEND_FAILURE = k_EResultEmailSendFailure,
		RESULT_NOT_SETTLED = k_EResultNotSettled,
		RESULT_NEED_CAPTCHA = k_EResultNeedCaptcha,
		RESULT_GSLT_DENIED = k_EResultGSLTDenied,
		RESULT_GS_OWNER_DENIED = k_EResultGSOwnerDenied,
		RESULT_INVALID_ITEM_TYPE = k_EResultInvalidItemType,
		RESULT_IP_BANNED = k_EResultIPBanned,
		RESULT_GSLT_EXPIRED = k_EResultGSLTExpired,
		RESULT_INSUFFICIENT_FUNDS = k_EResultInsufficientFunds,
		RESULT_TOO_MANY_PENDING = k_EResultTooManyPending,
		RESULT_NO_SITE_LICENSES_FOUND = k_EResultNoSiteLicensesFound,
		RESULT_WG_NETWORK_SEND_EXCEEDED = k_EResultWGNetworkSendExceeded,
		RESULT_ACCOUNT_NOT_FRIENDS = k_EResultAccountNotFriends,
		RESULT_LIMITED_USER_ACCOUNT = k_EResultLimitedUserAccount,
		RESULT_CANT_REMOVE_ITEM = k_EResultCantRemoveItem,
		RESULT_ACCOUNT_DELETED = k_EResultAccountDeleted,
		RESULT_EXISTING_USER_CANCELLED_LICENSE = k_EResultExistingUserCancelledLicense,
		RESULT_COMMUNITY_COOLDOWN = k_EResultCommunityCooldown,
		RESULT_NO_LAUNCHER_SPECIFIED = k_EResultNoLauncherSpecified,
		RESULT_MUST_AGREE_TO_SSA = k_EResultMustAgreeToSSA,
		RESULT_LAUNCHER_MIGRATED = k_EResultLauncherMigrated,
		RESULT_STEAM_REALM_MISMATCH = k_EResultSteamRealmMismatch,
		RESULT_INVALID_SIGNATURE = k_EResultInvalidSignature,
		RESULT_PARSE_FAILURE = k_EResultParseFailure,
		RESULT_NO_VERIFIED_PHONE = k_EResultNoVerifiedPhone,
		RESULT_INSUFFICIENT_BATTERY = k_EResultInsufficientBattery,
		RESULT_CHARGER_REQUIRED = k_EResultChargerRequired,
		RESULT_CACHED_CREDENTIAL_INVALID = k_EResultCachedCredentialInvalid,
		RESULT_PHONE_NUMBER_IS_VOIP = K_EResultPhoneNumberIsVOIP,
		RESULT_NOT_SUPPORTED = k_EResultNotSupported,
		RESULT_FAMILY_SIZE_LIMIT_EXCEEDED = k_EResultFamilySizeLimitExceeded
	};
	enum SteamAPIInitResult {
		STEAM_API_INIT_RESULT_OK = k_ESteamAPIInitResult_OK,
		STEAM_API_INIT_RESULT_FAILED_GENERIC = k_ESteamAPIInitResult_FailedGeneric,
		STEAM_API_INIT_RESULT_NO_STEAM_CLIENT = k_ESteamAPIInitResult_NoSteamClient,
		STEAM_API_INIT_RESULT_VERSION_MISMATCH = k_ESteamAPIInitResult_VersionMismatch
	};
	enum Universe {
		// Found in steamuniverse.h
		UNIVERSE_INVALID = k_EUniverseInvalid,
		UNIVERSE_PUBLIC = k_EUniversePublic,
		UNIVERSE_BETA = k_EUniverseBeta,
		UNIVERSE_INTERNAL = k_EUniverseInternal,
		UNIVERSE_DEV = k_EUniverseDev,
		UNIVERSE_MAX = k_EUniverseMax
	};
	enum UserHasLicenseForAppResult {
		// Found in steamclientpublic.h
		USER_HAS_LICENSE_RESULT_HAS_LICENSE = k_EUserHasLicenseResultHasLicense,
		USER_HAS_LICENSE_RESULT_DOES_NOT_HAVE_LICENSE = k_EUserHasLicenseResultDoesNotHaveLicense,
		USER_HAS_LICENSE_RESULT_NO_AUTH = k_EUserHasLicenseResultNoAuth
	};
	enum VoiceResult {
		// Found in steamclientpublic.h
		VOICE_RESULT_OK = k_EVoiceResultOK,
		VOICE_RESULT_NOT_INITIALIZED = k_EVoiceResultNotInitialized,
		VOICE_RESULT_NOT_RECORDING = k_EVoiceResultNotRecording,
		VOICE_RESULT_NO_DATE = k_EVoiceResultNoData,
		VOICE_RESULT_BUFFER_TOO_SMALL = k_EVoiceResultBufferTooSmall,
		VOICE_RESULT_DATA_CORRUPTED = k_EVoiceResultDataCorrupted,
		VOICE_RESULT_RESTRICTED = k_EVoiceResultRestricted,
		VOICE_RESULT_UNSUPPORTED_CODEC = k_EVoiceResultUnsupportedCodec,
		VOICE_RESULT_RECEIVER_OUT_OF_DATE = k_EVoiceResultReceiverOutOfDate,
		VOICE_RESULT_RECEIVER_DID_NOT_ANSWER = k_EVoiceResultReceiverDidNotAnswer
	};

	// Friends enums
	enum AvatarSizes {
		AVATAR_SMALL = 1,
		AVATAR_MEDIUM = 2,
		AVATAR_LARGE = 3
	};
	enum CommunityProfileItemProperty {
		PROFILE_ITEM_PROPERTY_IMAGE_SMALL = k_ECommunityProfileItemProperty_ImageSmall,
		PROFILE_ITEM_PROPERTY_IMAGE_LARGE = k_ECommunityProfileItemProperty_ImageLarge,
		PROFILE_ITEM_PROPERTY_INTERNAL_NAME = k_ECommunityProfileItemProperty_InternalName,
		PROFILE_ITEM_PROPERTY_TITLE = k_ECommunityProfileItemProperty_Title,
		PROFILE_ITEM_PROPERTY_DESCRIPTION = k_ECommunityProfileItemProperty_Description,
		PROFILE_ITEM_PROPERTY_APP_ID = k_ECommunityProfileItemProperty_AppID,
		PROFILE_ITEM_PROPERTY_TYPE_ID = k_ECommunityProfileItemProperty_TypeID,
		PROFILE_ITEM_PROPERTY_CLASS = k_ECommunityProfileItemProperty_Class,
		PROFILE_ITEM_PROPERTY_MOVIE_WEBM = k_ECommunityProfileItemProperty_MovieWebM,
		PROFILE_ITEM_PROPERTY_MOVIE_MP4 = k_ECommunityProfileItemProperty_MovieMP4,
		PROFILE_ITEM_PROPERTY_MOVIE_WEBM_SMALL = k_ECommunityProfileItemProperty_MovieWebMSmall,
		PROFILE_ITEM_PROPERTY_MOVIE_MP4_SMALL = k_ECommunityProfileItemProperty_MovieMP4Small
	};
	enum CommunityProfileItemType {
		PROFILE_ITEM_TYPE_ANIMATED_AVATAR = k_ECommunityProfileItemType_AnimatedAvatar,
		PROFILE_ITEM_TYPE_AVATAR_FRAME = k_ECommunityProfileItemType_AvatarFrame,
		PROFILE_ITEM_TYPE_PROFILE_MODIFIER = k_ECommunityProfileItemType_ProfileModifier,
		PROFILE_ITEM_TYPE_PROFILE_BACKGROUND = k_ECommunityProfileItemType_ProfileBackground,
		PROFILE_ITEM_TYPE_MINI_PROFILE_BACKGROUND = k_ECommunityProfileItemType_MiniProfileBackground
	};
	enum FriendFlags {
		FRIEND_FLAG_NONE = k_EFriendFlagNone,
		FRIEND_FLAG_BLOCKED = k_EFriendFlagBlocked,
		FRIEND_FLAG_FRIENDSHIP_REQUESTED = k_EFriendFlagFriendshipRequested,
		FRIEND_FLAG_IMMEDIATE = k_EFriendFlagImmediate,
		FRIEND_FLAG_CLAN_MEMBER = k_EFriendFlagClanMember,
		FRIEND_FLAG_ON_GAME_SERVER = k_EFriendFlagOnGameServer,
		//			FRIEND_FLAG_HAS_PLAYED_WITH = k_EFriendFlagHasPlayedWith,
		//			FRIEND_FLAG_FRIEND_OF_FRIEND = k_EFriendFlagFriendOfFriend,
		FRIEND_FLAG_REQUESTING_FRIENDSHIP = k_EFriendFlagRequestingFriendship,
		FRIEND_FLAG_REQUESTING_INFO = k_EFriendFlagRequestingInfo,
		FRIEND_FLAG_IGNORED = k_EFriendFlagIgnored,
		FRIEND_FLAG_IGNORED_FRIEND = k_EFriendFlagIgnoredFriend,
		//			FRIEND_FLAG_SUGGESTED = k_EFriendFlagSuggested,
		FRIEND_FLAG_CHAT_MEMBER = k_EFriendFlagChatMember,
		FRIEND_FLAG_ALL = k_EFriendFlagAll
	};
	enum FriendRelationship {
		FRIEND_RELATION_NONE = k_EFriendRelationshipNone,
		FRIEND_RELATION_BLOCKED = k_EFriendRelationshipBlocked,
		FRIEND_RELATION_REQUEST_RECIPIENT = k_EFriendRelationshipRequestRecipient,
		FRIEND_RELATION_FRIEND = k_EFriendRelationshipFriend,
		FRIEND_RELATION_REQUEST_INITIATOR = k_EFriendRelationshipRequestInitiator,
		FRIEND_RELATION_IGNORED = k_EFriendRelationshipIgnored,
		FRIEND_RELATION_IGNORED_FRIEND = k_EFriendRelationshipIgnoredFriend,
		FRIEND_RELATION_SUGGESTED = k_EFriendRelationshipSuggested_DEPRECATED,
		FRIEND_RELATION_MAX = k_EFriendRelationshipMax
	};
	enum OverlayToStoreFlag {
		OVERLAY_TO_STORE_FLAG_NONE = k_EOverlayToStoreFlag_None,
		OVERLAY_TO_STORE_FLAG_ADD_TO_CART = k_EOverlayToStoreFlag_AddToCart,
		OVERLAY_TO_STORE_FLAG_AND_TO_CART_AND_SHOW = k_EOverlayToStoreFlag_AddToCartAndShow
	};
	enum OverlayToWebPageMode {
		OVERLAY_TO_WEB_PAGE_MODE_DEFAULT = k_EActivateGameOverlayToWebPageMode_Default,
		OVERLAY_TO_WEB_PAGE_MODE_MODAL = k_EActivateGameOverlayToWebPageMode_Modal
	};
	enum PersonaChange {
		PERSONA_CHANGE_NAME = k_EPersonaChangeName,
		PERSONA_CHANGE_STATUS = k_EPersonaChangeStatus,
		PERSONA_CHANGE_COME_ONLINE = k_EPersonaChangeComeOnline,
		PERSONA_CHANGE_GONE_OFFLINE = k_EPersonaChangeGoneOffline,
		PERSONA_CHANGE_GAME_PLAYED = k_EPersonaChangeGamePlayed,
		PERSONA_CHANGE_GAME_SERVER = k_EPersonaChangeGameServer,
		PERSONA_CHANGE_AVATAR = k_EPersonaChangeAvatar,
		PERSONA_CHANGE_JOINED_SOURCE = k_EPersonaChangeJoinedSource,
		PERSONA_CHANGE_LEFT_SOURCE = k_EPersonaChangeLeftSource,
		PERSONA_CHANGE_RELATIONSHIP_CHANGED = k_EPersonaChangeRelationshipChanged,
		PERSONA_CHANGE_NAME_FIRST_SET = k_EPersonaChangeNameFirstSet,
		PERSONA_CHANGE_FACEBOOK_INFO = k_EPersonaChangeBroadcast,
		PERSONA_CHANGE_NICKNAME = k_EPersonaChangeNickname,
		PERSONA_CHANGE_STEAM_LEVEL = k_EPersonaChangeSteamLevel,
		PERSONA_CHANGE_RICH_PRESENCE = k_EPersonaChangeRichPresence
	};
	enum PersonaState {
		PERSONA_STATE_OFFLINE = k_EPersonaStateOffline,
		PERSONA_STATE_ONLINE = k_EPersonaStateOnline,
		PERSONA_STATE_BUSY = k_EPersonaStateBusy,
		PERSONA_STATE_AWAY = k_EPersonaStateAway,
		PERSONA_STATE_SNOOZE = k_EPersonaStateSnooze,
		PERSONA_STATE_LOOKING_TO_TRADE = k_EPersonaStateLookingToTrade,
		PERSONA_STATE_LOOKING_TO_PLAY = k_EPersonaStateLookingToPlay,
		PERSONA_STATE_INVISIBLE = k_EPersonaStateInvisible,
		PERSONA_STATE_MAX = k_EPersonaStateMax
	};
	enum UserRestriction {
		USER_RESTRICTION_NONE = k_nUserRestrictionNone,
		USER_RESTRICTION_UNKNOWN = k_nUserRestrictionUnknown,
		USER_RESTRICTION_ANY_CHAT = k_nUserRestrictionAnyChat,
		USER_RESTRICTION_VOICE_CHAT = k_nUserRestrictionVoiceChat,
		USER_RESTRICTION_GROUP_CHAT = k_nUserRestrictionGroupChat,
		USER_RESTRICTION_RATING = k_nUserRestrictionRating,
		USER_RESTRICTION_GAME_INVITES = k_nUserRestrictionGameInvites,
		USER_RESTRICTION_TRADING = k_nUserRestrictionTrading
	};

	// Game Search enums
	enum GameSearchErrorCode {
		// Found in steamclientpublic.h
		GAME_SEARCH_ERROR_CODE_OK = k_EGameSearchErrorCode_OK,
		GAME_SEARCH_ERROR_CODE_SEARCH_AREADY_IN_PROGRESS = k_EGameSearchErrorCode_Failed_Search_Already_In_Progress,
		GAME_SEARCH_ERROR_CODE_NO_SEARCH_IN_PROGRESS = k_EGameSearchErrorCode_Failed_No_Search_In_Progress,
		GAME_SEARCH_ERROR_CODE_NOT_LOBBY_LEADER = k_EGameSearchErrorCode_Failed_Not_Lobby_Leader,
		GAME_SEARCH_ERROR_CODE_NO_HOST_AVAILABLE = k_EGameSearchErrorCode_Failed_No_Host_Available,
		GAME_SEARCH_ERROR_CODE_SEARCH_PARAMS_INVALID = k_EGameSearchErrorCode_Failed_Search_Params_Invalid,
		GAME_SEARCH_ERROR_CODE_OFFLINE = k_EGameSearchErrorCode_Failed_Offline,
		GAME_SEARCH_ERROR_CODE_NOT_AUTHORIZED = k_EGameSearchErrorCode_Failed_NotAuthorized,
		GAME_SEARCH_ERROR_CODE_UNKNOWN_ERROR = k_EGameSearchErrorCode_Failed_Unknown_Error
	};
	enum PlayerResult {
		// Found in steamclientpublic.h
		PLAYER_RESULT_FAILED_TO_CONNECT = k_EPlayerResultFailedToConnect,
		PLAYER_RESULT_ABANDONED = k_EPlayerResultAbandoned,
		PLAYER_RESULT_KICKED = k_EPlayerResultKicked,
		PLAYER_RESULT_INCOMPLETE = k_EPlayerResultIncomplete,
		PLAYER_RESULT_COMPLETED = k_EPlayerResultCompleted
	};

	// HTMLSurface enums
	enum HTMLKeyModifiers {
		HTML_KEY_MODIFIER_NONE = ISteamHTMLSurface::k_eHTMLKeyModifier_None,
		HTML_KEY_MODIFIER_ALT_DOWN = ISteamHTMLSurface::k_eHTMLKeyModifier_AltDown,
		HTML_KEY_MODIFIER_CTRL_DOWN = ISteamHTMLSurface::k_eHTMLKeyModifier_CtrlDown,
		HTML_KEY_MODIFIER_SHIFT_DOWN = ISteamHTMLSurface::k_eHTMLKeyModifier_ShiftDown
	};
	enum HTMLMouseButton {
		HTML_MOUSE_BUTTON_LEFT = ISteamHTMLSurface::eHTMLMouseButton_Left,
		HTML_MOUSE_BUTTON_RIGHT = ISteamHTMLSurface::eHTMLMouseButton_Right,
		HTML_MOUSE_BUTTON_MIDDLE = ISteamHTMLSurface::eHTMLMouseButton_Middle
	};
	enum MouseCursor {
		DC_USER = ISteamHTMLSurface::dc_user,
		DC_NONE = ISteamHTMLSurface::dc_none,
		DC_ARROW = ISteamHTMLSurface::dc_arrow,
		DC_IBEAM = ISteamHTMLSurface::dc_ibeam,
		DC_HOUR_GLASS = ISteamHTMLSurface::dc_hourglass,
		DC_WAIT_ARROW = ISteamHTMLSurface::dc_waitarrow,
		DC_CROSSHAIR = ISteamHTMLSurface::dc_crosshair,
		DC_UP = ISteamHTMLSurface::dc_up,
		DC_SIZE_NW = ISteamHTMLSurface::dc_sizenw,
		DC_SIZE_SE = ISteamHTMLSurface::dc_sizese,
		DC_SIZE_NE = ISteamHTMLSurface::dc_sizene,
		DC_SIZE_SW = ISteamHTMLSurface::dc_sizesw,
		DC_SIZE_W = ISteamHTMLSurface::dc_sizew,
		DC_SIZE_E = ISteamHTMLSurface::dc_sizee,
		DC_SIZE_N = ISteamHTMLSurface::dc_sizen,
		DC_SIZE_S = ISteamHTMLSurface::dc_sizes,
		DC_SIZE_WE = ISteamHTMLSurface::dc_sizewe,
		DC_SIZE_NS = ISteamHTMLSurface::dc_sizens,
		DC_SIZE_ALL = ISteamHTMLSurface::dc_sizeall,
		DC_NO = ISteamHTMLSurface::dc_no,
		DC_HAND = ISteamHTMLSurface::dc_hand,
		DC_BLANK = ISteamHTMLSurface::dc_blank,
		DC_MIDDLE_PAN = ISteamHTMLSurface::dc_middle_pan,
		DC_NORTH_PAN = ISteamHTMLSurface::dc_north_pan,
		DC_NORTH_EAST_PAN = ISteamHTMLSurface::dc_north_east_pan,
		DC_EAST_PAN = ISteamHTMLSurface::dc_east_pan,
		DC_SOUTH_EAST_PAN = ISteamHTMLSurface::dc_south_east_pan,
		DC_SOUTH_PAN = ISteamHTMLSurface::dc_south_pan,
		DC_SOUTH_WEST_PAN = ISteamHTMLSurface::dc_south_west_pan,
		DC_WEST_PAN = ISteamHTMLSurface::dc_west_pan,
		DC_NORTH_WEST_PAN = ISteamHTMLSurface::dc_north_west_pan,
		DC_ALIAS = ISteamHTMLSurface::dc_alias,
		DC_CELL = ISteamHTMLSurface::dc_cell,
		DC_COL_RESIZE = ISteamHTMLSurface::dc_colresize,
		DC_COPY_CUR = ISteamHTMLSurface::dc_copycur,
		DC_VERTICAL_TEXT = ISteamHTMLSurface::dc_verticaltext,
		DC_ROW_RESIZE = ISteamHTMLSurface::dc_rowresize,
		DC_ZOOM_IN = ISteamHTMLSurface::dc_zoomin,
		DC_ZOOM_OUT = ISteamHTMLSurface::dc_zoomout,
		DC_HELP = ISteamHTMLSurface::dc_help,
		DC_CUSTOM = ISteamHTMLSurface::dc_custom,
		DC_LAST = ISteamHTMLSurface::dc_last
	};

	// HTTP enums
	enum HTTPMethod {
		HTTP_METHOD_INVALID = k_EHTTPMethodInvalid,
		HTTP_METHOD_GET = k_EHTTPMethodGET,
		HTTP_METHOD_HEAD = k_EHTTPMethodHEAD,
		HTTP_METHOD_POST = k_EHTTPMethodPOST,
		HTTP_METHOD_PUT = k_EHTTPMethodPUT,
		HTTP_METHOD_DELETE = k_EHTTPMethodDELETE,
		HTTP_METHOD_OPTIONS = k_EHTTPMethodOPTIONS,
		HTTP_METHOD_PATCH = k_EHTTPMethodPATCH
	};
	enum HTTPStatusCode {
		HTTP_STATUS_CODE_INVALID = k_EHTTPStatusCodeInvalid,
		HTTP_STATUS_CODE_100_CONTINUE = k_EHTTPStatusCode100Continue,
		HTTP_STATUS_CODE_101_SWITCHING_PROTOCOLS = k_EHTTPStatusCode101SwitchingProtocols,
		HTTP_STATUS_CODE_200_OK = k_EHTTPStatusCode200OK,
		HTTP_STATUS_CODE_201_CREATED = k_EHTTPStatusCode201Created,
		HTTP_STATUS_CODE_202_ACCEPTED = k_EHTTPStatusCode202Accepted,
		HTTP_STATUS_CODE_203_NON_AUTHORITATIVE = k_EHTTPStatusCode203NonAuthoritative,
		HTTP_STATUS_CODE_204_NO_CONTENT = k_EHTTPStatusCode204NoContent,
		HTTP_STATUS_CODE_205_RESET_CONTENT = k_EHTTPStatusCode205ResetContent,
		HTTP_STATUS_CODE_206_PARTIAL_CONTENT = k_EHTTPStatusCode206PartialContent,
		HTTP_STATUS_CODE_300_MULTIPLE_CHOICES = k_EHTTPStatusCode300MultipleChoices,
		HTTP_STATUS_CODE_301_MOVED_PERMANENTLY = k_EHTTPStatusCode301MovedPermanently,
		HTTP_STATUS_CODE_302_FOUND = k_EHTTPStatusCode302Found,
		HTTP_STATUS_CODE_303_SEE_OTHER = k_EHTTPStatusCode303SeeOther,
		HTTP_STATUS_CODE_304_NOT_MODIFIED = k_EHTTPStatusCode304NotModified,
		HTTP_STATUS_CODE_305_USE_PROXY = k_EHTTPStatusCode305UseProxy,
		HTTP_STATUS_CODE_307_TEMPORARY_REDIRECT = k_EHTTPStatusCode307TemporaryRedirect,
		HTTP_STATUS_CODE_308_PERMANENT_REDIRECT = k_EHTTPStatusCode308PermanentRedirect,
		HTTP_STATUS_CODE_400_BAD_REQUEST = k_EHTTPStatusCode400BadRequest,
		HTTP_STATUS_CODE_401_UNAUTHORIZED = k_EHTTPStatusCode401Unauthorized,
		HTTP_STATUS_CODE_402_PAYMENT_REQUIRED = k_EHTTPStatusCode402PaymentRequired,
		HTTP_STATUS_CODE_403_FORBIDDEN = k_EHTTPStatusCode403Forbidden,
		HTTP_STATUS_CODE_404_NOT_FOUND = k_EHTTPStatusCode404NotFound,
		HTTP_STATUS_CODE_405_METHOD_NOT_ALLOWED = k_EHTTPStatusCode405MethodNotAllowed,
		HTTP_STATUS_CODE_406_NOT_ACCEPTABLE = k_EHTTPStatusCode406NotAcceptable,
		HTTP_STATUS_CODE_407_PROXY_AUTH_REQUIRED = k_EHTTPStatusCode407ProxyAuthRequired,
		HTTP_STATUS_CODE_408_REQUEST_TIMEOUT = k_EHTTPStatusCode408RequestTimeout,
		HTTP_STATUS_CODE_409_CONFLICT = k_EHTTPStatusCode409Conflict,
		HTTP_STATUS_CODE_410_GONE = k_EHTTPStatusCode410Gone,
		HTTP_STATUS_CODE_411_LENGTH_REQUIRED = k_EHTTPStatusCode411LengthRequired,
		HTTP_STATUS_CODE_412_PRECONDITION_FAILED = k_EHTTPStatusCode412PreconditionFailed,
		HTTP_STATUS_CODE_413_REQUEST_ENTITY_TOO_LARGE = k_EHTTPStatusCode413RequestEntityTooLarge,
		HTTP_STATUS_CODE_414_REQUEST_URI_TOO_LONG = k_EHTTPStatusCode414RequestURITooLong,
		HTTP_STATUS_CODE_415_UNSUPPORTED_MEDIA_TYPE = k_EHTTPStatusCode415UnsupportedMediaType,
		HTTP_STATUS_CODE_416_REQUESTED_RANGE_NOT_SATISFIABLE = k_EHTTPStatusCode416RequestedRangeNotSatisfiable,
		HTTP_STATUS_CODE_417_EXPECTATION_FAILED = k_EHTTPStatusCode417ExpectationFailed,
		HTTP_STATUS_CODE_4XX_UNKNOWN = k_EHTTPStatusCode4xxUnknown,
		HTTP_STATUS_CODE_429_TOO_MANY_REQUESTS = k_EHTTPStatusCode429TooManyRequests,
		HTTP_STATUS_CODE_444_CONNECTION_CLOSED = k_EHTTPStatusCode444ConnectionClosed,
		HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR = k_EHTTPStatusCode500InternalServerError,
		HTTP_STATUS_CODE_501_NOT_IMPLEMENTED = k_EHTTPStatusCode501NotImplemented,
		HTTP_STATUS_CODE_502_BAD_GATEWAY = k_EHTTPStatusCode502BadGateway,
		HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE = k_EHTTPStatusCode503ServiceUnavailable,
		HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT = k_EHTTPStatusCode504GatewayTimeout,
		HTTP_STATUS_CODE_505_HTTP_VERSION_NOT_SUPPORTED = k_EHTTPStatusCode505HTTPVersionNotSupported,
		HTTP_STATUS_CODE_5XX_UNKNOWN = k_EHTTPStatusCode5xxUnknown
	};

	// Input enums
	enum ControllerHapticLocation {
		CONTROLLER_HAPTIC_LOCATION_LEFT = k_EControllerHapticLocation_Left,
		CONTROLLER_HAPTIC_LOCATION_RIGHT = k_EControllerHapticLocation_Right,
		CONTROLLER_HAPTIC_LOCATION_BOTH = k_EControllerHapticLocation_Both
	};
	enum ControllerHapticType {
		CONTROLLER_HAPTIC_TYPE_OFF = k_EControllerHapticType_Off,
		CONTROLLER_HAPTIC_TYPE_TICK = k_EControllerHapticType_Tick,
		CONTROLLER_HAPTIC_TYPE_CLICK = k_EControllerHapticType_Click
	};
	enum ControllerPad {
		STEAM_CONTROLLER_PAD_LEFT = k_ESteamControllerPad_Left,
		STEAM_CONTROLLER_PAD_RIGHT = k_ESteamControllerPad_Right
	};
	enum InputActionEventType {
		INPUT_ACTION_EVENT_TYPE_DIGITAL_ACTION = ESteamInputActionEventType_DigitalAction,
		INPUT_ACTION_EVENT_TYPE_ANALOG_ACTION = ESteamInputActionEventType_AnalogAction
	};
	enum InputActionOrigin {
		INPUT_ACTION_ORIGIN_NONE = k_EInputActionOrigin_None,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_A = k_EInputActionOrigin_SteamController_A,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_B = k_EInputActionOrigin_SteamController_B,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_X = k_EInputActionOrigin_SteamController_X,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_Y = k_EInputActionOrigin_SteamController_Y,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTBUMPER = k_EInputActionOrigin_SteamController_LeftBumper,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTBUMPER = k_EInputActionOrigin_SteamController_RightBumper,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTGRIP = k_EInputActionOrigin_SteamController_LeftGrip,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTGRIP = k_EInputActionOrigin_SteamController_RightGrip,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_START = k_EInputActionOrigin_SteamController_Start,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_BACK = k_EInputActionOrigin_SteamController_Back,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTPAD_TOUCH = k_EInputActionOrigin_SteamController_LeftPad_Touch,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTPAD_SWIPE = k_EInputActionOrigin_SteamController_LeftPad_Swipe,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTPAD_CLICK = k_EInputActionOrigin_SteamController_LeftPad_Click,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTPAD_DPADNORTH = k_EInputActionOrigin_SteamController_LeftPad_DPadNorth,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTPAD_DPADSOUTH = k_EInputActionOrigin_SteamController_LeftPad_DPadSouth,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTPAD_DPADWEST = k_EInputActionOrigin_SteamController_LeftPad_DPadWest,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTPAD_DPADEAST = k_EInputActionOrigin_SteamController_LeftPad_DPadEast,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTPAD_TOUCH = k_EInputActionOrigin_SteamController_RightPad_Touch,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTPAD_SWIPE = k_EInputActionOrigin_SteamController_RightPad_Swipe,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTPAD_CLICK = k_EInputActionOrigin_SteamController_RightPad_Click,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTPAD_DPADNORTH = k_EInputActionOrigin_SteamController_RightPad_DPadNorth,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTPAD_DPADSOUTH = k_EInputActionOrigin_SteamController_RightPad_DPadSouth,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTPAD_DPADWEST = k_EInputActionOrigin_SteamController_RightPad_DPadWest,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTPAD_DPADEAST = k_EInputActionOrigin_SteamController_RightPad_DPadEast,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTTRIGGER_PULL = k_EInputActionOrigin_SteamController_LeftTrigger_Pull,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTTRIGGER_CLICK = k_EInputActionOrigin_SteamController_LeftTrigger_Click,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTTRIGGER_PULL = k_EInputActionOrigin_SteamController_RightTrigger_Pull,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RIGHTTRIGGER_CLICK = k_EInputActionOrigin_SteamController_RightTrigger_Click,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTSTICK_MOVE = k_EInputActionOrigin_SteamController_LeftStick_Move,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTSTICK_CLICK = k_EInputActionOrigin_SteamController_LeftStick_Click,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTSTICK_DPADNORTH = k_EInputActionOrigin_SteamController_LeftStick_DPadNorth,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTSTICK_DPADSOUTH = k_EInputActionOrigin_SteamController_LeftStick_DPadSouth,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTSTICK_DPADWEST = k_EInputActionOrigin_SteamController_LeftStick_DPadWest,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_LEFTSTICK_DPADEAST = k_EInputActionOrigin_SteamController_LeftStick_DPadEast,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_GYRO_MOVE = k_EInputActionOrigin_SteamController_Gyro_Move,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_GYRO_PITCH = k_EInputActionOrigin_SteamController_Gyro_Pitch,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_GYRO_YAW = k_EInputActionOrigin_SteamController_Gyro_Yaw,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_GYRO_ROLL = k_EInputActionOrigin_SteamController_Gyro_Roll,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED0 = k_EInputActionOrigin_SteamController_Reserved0,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED1 = k_EInputActionOrigin_SteamController_Reserved1,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED2 = k_EInputActionOrigin_SteamController_Reserved2,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED3 = k_EInputActionOrigin_SteamController_Reserved3,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED4 = k_EInputActionOrigin_SteamController_Reserved4,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED5 = k_EInputActionOrigin_SteamController_Reserved5,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED6 = k_EInputActionOrigin_SteamController_Reserved6,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED7 = k_EInputActionOrigin_SteamController_Reserved7,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED8 = k_EInputActionOrigin_SteamController_Reserved8,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED9 = k_EInputActionOrigin_SteamController_Reserved9,
		INPUT_ACTION_ORIGIN_STEAMCONTROLLER_RESERVED10 = k_EInputActionOrigin_SteamController_Reserved10,
		INPUT_ACTION_ORIGIN_PS4_X = k_EInputActionOrigin_PS4_X,
		INPUT_ACTION_ORIGIN_PS4_CIRCLE = k_EInputActionOrigin_PS4_Circle,
		INPUT_ACTION_ORIGIN_PS4_TRIANGLE = k_EInputActionOrigin_PS4_Triangle,
		INPUT_ACTION_ORIGIN_PS4_SQUARE = k_EInputActionOrigin_PS4_Square,
		INPUT_ACTION_ORIGIN_PS4_LEFTBUMPER = k_EInputActionOrigin_PS4_LeftBumper,
		INPUT_ACTION_ORIGIN_PS4_RIGHTBUMPER = k_EInputActionOrigin_PS4_RightBumper,
		INPUT_ACTION_ORIGIN_PS4_OPTIONS = k_EInputActionOrigin_PS4_Options,
		INPUT_ACTION_ORIGIN_PS4_SHARE = k_EInputActionOrigin_PS4_Share,
		INPUT_ACTION_ORIGIN_PS4_LEFTPAD_TOUCH = k_EInputActionOrigin_PS4_LeftPad_Touch,
		INPUT_ACTION_ORIGIN_PS4_LEFTPAD_SWIPE = k_EInputActionOrigin_PS4_LeftPad_Swipe,
		INPUT_ACTION_ORIGIN_PS4_LEFTPAD_CLICK = k_EInputActionOrigin_PS4_LeftPad_Click,
		INPUT_ACTION_ORIGIN_PS4_LEFTPAD_DPADNORTH = k_EInputActionOrigin_PS4_LeftPad_DPadNorth,
		INPUT_ACTION_ORIGIN_PS4_LEFTPAD_DPADSOUTH = k_EInputActionOrigin_PS4_LeftPad_DPadSouth,
		INPUT_ACTION_ORIGIN_PS4_LEFTPAD_DPADWEST = k_EInputActionOrigin_PS4_LeftPad_DPadWest,
		INPUT_ACTION_ORIGIN_PS4_LEFTPAD_DPADEAST = k_EInputActionOrigin_PS4_LeftPad_DPadEast,
		INPUT_ACTION_ORIGIN_PS4_RIGHTPAD_TOUCH = k_EInputActionOrigin_PS4_RightPad_Touch,
		INPUT_ACTION_ORIGIN_PS4_RIGHTPAD_SWIPE = k_EInputActionOrigin_PS4_RightPad_Swipe,
		INPUT_ACTION_ORIGIN_PS4_RIGHTPAD_CLICK = k_EInputActionOrigin_PS4_RightPad_Click,
		INPUT_ACTION_ORIGIN_PS4_RIGHTPAD_DPADNORTH = k_EInputActionOrigin_PS4_RightPad_DPadNorth,
		INPUT_ACTION_ORIGIN_PS4_RIGHTPAD_DPADSOUTH = k_EInputActionOrigin_PS4_RightPad_DPadSouth,
		INPUT_ACTION_ORIGIN_PS4_RIGHTPAD_DPADWEST = k_EInputActionOrigin_PS4_RightPad_DPadWest,
		INPUT_ACTION_ORIGIN_PS4_RIGHTPAD_DPADEAST = k_EInputActionOrigin_PS4_RightPad_DPadEast,
		INPUT_ACTION_ORIGIN_PS4_CENTERPAD_TOUCH = k_EInputActionOrigin_PS4_CenterPad_Touch,
		INPUT_ACTION_ORIGIN_PS4_CENTERPAD_SWIPE = k_EInputActionOrigin_PS4_CenterPad_Swipe,
		INPUT_ACTION_ORIGIN_PS4_CENTERPAD_CLICK = k_EInputActionOrigin_PS4_CenterPad_Click,
		INPUT_ACTION_ORIGIN_PS4_CENTERPAD_DPADNORTH = k_EInputActionOrigin_PS4_CenterPad_DPadNorth,
		INPUT_ACTION_ORIGIN_PS4_CENTERPAD_DPADSOUTH = k_EInputActionOrigin_PS4_CenterPad_DPadSouth,
		INPUT_ACTION_ORIGIN_PS4_CENTERPAD_DPADWEST = k_EInputActionOrigin_PS4_CenterPad_DPadWest,
		INPUT_ACTION_ORIGIN_PS4_CENTERPAD_DPADEAST = k_EInputActionOrigin_PS4_CenterPad_DPadEast,
		INPUT_ACTION_ORIGIN_PS4_LEFTTRIGGER_PULL = k_EInputActionOrigin_PS4_LeftTrigger_Pull,
		INPUT_ACTION_ORIGIN_PS4_LEFTTRIGGER_CLICK = k_EInputActionOrigin_PS4_LeftTrigger_Click,
		INPUT_ACTION_ORIGIN_PS4_RIGHTTRIGGER_PULL = k_EInputActionOrigin_PS4_RightTrigger_Pull,
		INPUT_ACTION_ORIGIN_PS4_RIGHTTRIGGER_CLICK = k_EInputActionOrigin_PS4_RightTrigger_Click,
		INPUT_ACTION_ORIGIN_PS4_LEFTSTICK_MOVE = k_EInputActionOrigin_PS4_LeftStick_Move,
		INPUT_ACTION_ORIGIN_PS4_LEFTSTICK_CLICK = k_EInputActionOrigin_PS4_LeftStick_Click,
		INPUT_ACTION_ORIGIN_PS4_LEFTSTICK_DPADNORTH = k_EInputActionOrigin_PS4_LeftStick_DPadNorth,
		INPUT_ACTION_ORIGIN_PS4_LEFTSTICK_DPADSOUTH = k_EInputActionOrigin_PS4_LeftStick_DPadSouth,
		INPUT_ACTION_ORIGIN_PS4_LEFTSTICK_DPADWEST = k_EInputActionOrigin_PS4_LeftStick_DPadWest,
		INPUT_ACTION_ORIGIN_PS4_LEFTSTICK_DPADEAST = k_EInputActionOrigin_PS4_LeftStick_DPadEast,
		INPUT_ACTION_ORIGIN_PS4_RIGHTSTICK_MOVE = k_EInputActionOrigin_PS4_RightStick_Move,
		INPUT_ACTION_ORIGIN_PS4_RIGHTSTICK_CLICK = k_EInputActionOrigin_PS4_RightStick_Click,
		INPUT_ACTION_ORIGIN_PS4_RIGHTSTICK_DPADNORTH = k_EInputActionOrigin_PS4_RightStick_DPadNorth,
		INPUT_ACTION_ORIGIN_PS4_RIGHTSTICK_DPADSOUTH = k_EInputActionOrigin_PS4_RightStick_DPadSouth,
		INPUT_ACTION_ORIGIN_PS4_RIGHTSTICK_DPADWEST = k_EInputActionOrigin_PS4_RightStick_DPadWest,
		INPUT_ACTION_ORIGIN_PS4_RIGHTSTICK_DPADEAST = k_EInputActionOrigin_PS4_RightStick_DPadEast,
		INPUT_ACTION_ORIGIN_PS4_DPAD_NORTH = k_EInputActionOrigin_PS4_DPad_North,
		INPUT_ACTION_ORIGIN_PS4_DPAD_SOUTH = k_EInputActionOrigin_PS4_DPad_South,
		INPUT_ACTION_ORIGIN_PS4_DPAD_WEST = k_EInputActionOrigin_PS4_DPad_West,
		INPUT_ACTION_ORIGIN_PS4_DPAD_EAST = k_EInputActionOrigin_PS4_DPad_East,
		INPUT_ACTION_ORIGIN_PS4_GYRO_MOVE = k_EInputActionOrigin_PS4_Gyro_Move,
		INPUT_ACTION_ORIGIN_PS4_GYRO_PITCH = k_EInputActionOrigin_PS4_Gyro_Pitch,
		INPUT_ACTION_ORIGIN_PS4_GYRO_YAW = k_EInputActionOrigin_PS4_Gyro_Yaw,
		INPUT_ACTION_ORIGIN_PS4_GYRO_ROLL = k_EInputActionOrigin_PS4_Gyro_Roll,
		INPUT_ACTION_ORIGIN_PS4_DPAD_MOVE = k_EInputActionOrigin_PS4_DPad_Move,
		INPUT_ACTION_ORIGIN_PS4_RESERVED1 = k_EInputActionOrigin_PS4_Reserved1,
		INPUT_ACTION_ORIGIN_PS4_RESERVED2 = k_EInputActionOrigin_PS4_Reserved2,
		INPUT_ACTION_ORIGIN_PS4_RESERVED3 = k_EInputActionOrigin_PS4_Reserved3,
		INPUT_ACTION_ORIGIN_PS4_RESERVED4 = k_EInputActionOrigin_PS4_Reserved4,
		INPUT_ACTION_ORIGIN_PS4_RESERVED5 = k_EInputActionOrigin_PS4_Reserved5,
		INPUT_ACTION_ORIGIN_PS4_RESERVED6 = k_EInputActionOrigin_PS4_Reserved6,
		INPUT_ACTION_ORIGIN_PS4_RESERVED7 = k_EInputActionOrigin_PS4_Reserved7,
		INPUT_ACTION_ORIGIN_PS4_RESERVED8 = k_EInputActionOrigin_PS4_Reserved8,
		INPUT_ACTION_ORIGIN_PS4_RESERVED9 = k_EInputActionOrigin_PS4_Reserved9,
		INPUT_ACTION_ORIGIN_PS4_RESERVED10 = k_EInputActionOrigin_PS4_Reserved10,
		INPUT_ACTION_ORIGIN_XBOXONE_A = k_EInputActionOrigin_XBoxOne_A,
		INPUT_ACTION_ORIGIN_XBOXONE_B = k_EInputActionOrigin_XBoxOne_B,
		INPUT_ACTION_ORIGIN_XBOXONE_X = k_EInputActionOrigin_XBoxOne_X,
		INPUT_ACTION_ORIGIN_XBOXONE_Y = k_EInputActionOrigin_XBoxOne_Y,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTBUMPER = k_EInputActionOrigin_XBoxOne_LeftBumper,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTBUMPER = k_EInputActionOrigin_XBoxOne_RightBumper,
		INPUT_ACTION_ORIGIN_XBOXONE_MENU = k_EInputActionOrigin_XBoxOne_Menu,
		INPUT_ACTION_ORIGIN_XBOXONE_VIEW = k_EInputActionOrigin_XBoxOne_View,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTTRIGGER_PULL = k_EInputActionOrigin_XBoxOne_LeftTrigger_Pull,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTTRIGGER_CLICK = k_EInputActionOrigin_XBoxOne_LeftTrigger_Click,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTTRIGGER_PULL = k_EInputActionOrigin_XBoxOne_RightTrigger_Pull,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTTRIGGER_CLICK = k_EInputActionOrigin_XBoxOne_RightTrigger_Click,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTSTICK_MOVE = k_EInputActionOrigin_XBoxOne_LeftStick_Move,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTSTICK_CLICK = k_EInputActionOrigin_XBoxOne_LeftStick_Click,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTSTICK_DPADNORTH = k_EInputActionOrigin_XBoxOne_LeftStick_DPadNorth,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTSTICK_DPADSOUTH = k_EInputActionOrigin_XBoxOne_LeftStick_DPadSouth,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTSTICK_DPADWEST = k_EInputActionOrigin_XBoxOne_LeftStick_DPadWest,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTSTICK_DPADEAST = k_EInputActionOrigin_XBoxOne_LeftStick_DPadEast,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTSTICK_MOVE = k_EInputActionOrigin_XBoxOne_RightStick_Move,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTSTICK_CLICK = k_EInputActionOrigin_XBoxOne_RightStick_Click,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTSTICK_DPADNORTH = k_EInputActionOrigin_XBoxOne_RightStick_DPadNorth,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTSTICK_DPADSOUTH = k_EInputActionOrigin_XBoxOne_RightStick_DPadSouth,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTSTICK_DPADWEST = k_EInputActionOrigin_XBoxOne_RightStick_DPadWest,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTSTICK_DPADEAST = k_EInputActionOrigin_XBoxOne_RightStick_DPadEast,
		INPUT_ACTION_ORIGIN_XBOXONE_DPAD_NORTH = k_EInputActionOrigin_XBoxOne_DPad_North,
		INPUT_ACTION_ORIGIN_XBOXONE_DPAD_SOUTH = k_EInputActionOrigin_XBoxOne_DPad_South,
		INPUT_ACTION_ORIGIN_XBOXONE_DPAD_WEST = k_EInputActionOrigin_XBoxOne_DPad_West,
		INPUT_ACTION_ORIGIN_XBOXONE_DPAD_EAST = k_EInputActionOrigin_XBoxOne_DPad_East,
		INPUT_ACTION_ORIGIN_XBOXONE_DPAD_MOVE = k_EInputActionOrigin_XBoxOne_DPad_Move,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTGRIP_LOWER = k_EInputActionOrigin_XBoxOne_LeftGrip_Lower,
		INPUT_ACTION_ORIGIN_XBOXONE_LEFTGRIP_UPPER = k_EInputActionOrigin_XBoxOne_LeftGrip_Upper,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTGRIP_LOWER = k_EInputActionOrigin_XBoxOne_RightGrip_Lower,
		INPUT_ACTION_ORIGIN_XBOXONE_RIGHTGRIP_UPPER = k_EInputActionOrigin_XBoxOne_RightGrip_Upper,
		INPUT_ACTION_ORIGIN_XBOXONE_SHARE = k_EInputActionOrigin_XBoxOne_Share,
		INPUT_ACTION_ORIGIN_XBOXONE_RESERVED6 = k_EInputActionOrigin_XBoxOne_Reserved6,
		INPUT_ACTION_ORIGIN_XBOXONE_RESERVED7 = k_EInputActionOrigin_XBoxOne_Reserved7,
		INPUT_ACTION_ORIGIN_XBOXONE_RESERVED8 = k_EInputActionOrigin_XBoxOne_Reserved8,
		INPUT_ACTION_ORIGIN_XBOXONE_RESERVED9 = k_EInputActionOrigin_XBoxOne_Reserved9,
		INPUT_ACTION_ORIGIN_XBOXONE_RESERVED10 = k_EInputActionOrigin_XBoxOne_Reserved10,
		INPUT_ACTION_ORIGIN_XBOX360_A = k_EInputActionOrigin_XBox360_A,
		INPUT_ACTION_ORIGIN_XBOX360_B = k_EInputActionOrigin_XBox360_B,
		INPUT_ACTION_ORIGIN_XBOX360_X = k_EInputActionOrigin_XBox360_X,
		INPUT_ACTION_ORIGIN_XBOX360_Y = k_EInputActionOrigin_XBox360_Y,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTBUMPER = k_EInputActionOrigin_XBox360_LeftBumper,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTBUMPER = k_EInputActionOrigin_XBox360_RightBumper,
		INPUT_ACTION_ORIGIN_XBOX360_START = k_EInputActionOrigin_XBox360_Start,
		INPUT_ACTION_ORIGIN_XBOX360_BACK = k_EInputActionOrigin_XBox360_Back,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTTRIGGER_PULL = k_EInputActionOrigin_XBox360_LeftTrigger_Pull,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTTRIGGER_CLICK = k_EInputActionOrigin_XBox360_LeftTrigger_Click,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTTRIGGER_PULL = k_EInputActionOrigin_XBox360_RightTrigger_Pull,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTTRIGGER_CLICK = k_EInputActionOrigin_XBox360_RightTrigger_Click,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTSTICK_MOVE = k_EInputActionOrigin_XBox360_LeftStick_Move,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTSTICK_CLICK = k_EInputActionOrigin_XBox360_LeftStick_Click,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTSTICK_DPADNORTH = k_EInputActionOrigin_XBox360_LeftStick_DPadNorth,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTSTICK_DPADSOUTH = k_EInputActionOrigin_XBox360_LeftStick_DPadSouth,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTSTICK_DPADWEST = k_EInputActionOrigin_XBox360_LeftStick_DPadWest,
		INPUT_ACTION_ORIGIN_XBOX360_LEFTSTICK_DPADEAST = k_EInputActionOrigin_XBox360_LeftStick_DPadEast,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTSTICK_MOVE = k_EInputActionOrigin_XBox360_RightStick_Move,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTSTICK_CLICK = k_EInputActionOrigin_XBox360_RightStick_Click,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTSTICK_DPADNORTH = k_EInputActionOrigin_XBox360_RightStick_DPadNorth,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTSTICK_DPADSOUTH = k_EInputActionOrigin_XBox360_RightStick_DPadSouth,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTSTICK_DPADWEST = k_EInputActionOrigin_XBox360_RightStick_DPadWest,
		INPUT_ACTION_ORIGIN_XBOX360_RIGHTSTICK_DPADEAST = k_EInputActionOrigin_XBox360_RightStick_DPadEast,
		INPUT_ACTION_ORIGIN_XBOX360_DPAD_NORTH = k_EInputActionOrigin_XBox360_DPad_North,
		INPUT_ACTION_ORIGIN_XBOX360_DPAD_SOUTH = k_EInputActionOrigin_XBox360_DPad_South,
		INPUT_ACTION_ORIGIN_XBOX360_DPAD_WEST = k_EInputActionOrigin_XBox360_DPad_West,
		INPUT_ACTION_ORIGIN_XBOX360_DPAD_EAST = k_EInputActionOrigin_XBox360_DPad_East,
		INPUT_ACTION_ORIGIN_XBOX360_DPAD_MOVE = k_EInputActionOrigin_XBox360_DPad_Move,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED1 = k_EInputActionOrigin_XBox360_Reserved1,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED2 = k_EInputActionOrigin_XBox360_Reserved2,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED3 = k_EInputActionOrigin_XBox360_Reserved3,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED4 = k_EInputActionOrigin_XBox360_Reserved4,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED5 = k_EInputActionOrigin_XBox360_Reserved5,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED6 = k_EInputActionOrigin_XBox360_Reserved6,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED7 = k_EInputActionOrigin_XBox360_Reserved7,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED8 = k_EInputActionOrigin_XBox360_Reserved8,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED9 = k_EInputActionOrigin_XBox360_Reserved9,
		INPUT_ACTION_ORIGIN_XBOX360_RESERVED10 = k_EInputActionOrigin_XBox360_Reserved10,
		INPUT_ACTION_ORIGIN_SWITCH_A = k_EInputActionOrigin_Switch_A,
		INPUT_ACTION_ORIGIN_SWITCH_B = k_EInputActionOrigin_Switch_B,
		INPUT_ACTION_ORIGIN_SWITCH_X = k_EInputActionOrigin_Switch_X,
		INPUT_ACTION_ORIGIN_SWITCH_Y = k_EInputActionOrigin_Switch_Y,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTBUMPER = k_EInputActionOrigin_Switch_LeftBumper,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTBUMPER = k_EInputActionOrigin_Switch_RightBumper,
		INPUT_ACTION_ORIGIN_SWITCH_PLUS = k_EInputActionOrigin_Switch_Plus,
		INPUT_ACTION_ORIGIN_SWITCH_MINUS = k_EInputActionOrigin_Switch_Minus,
		INPUT_ACTION_ORIGIN_SWITCH_CAPTURE = k_EInputActionOrigin_Switch_Capture,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTTRIGGER_PULL = k_EInputActionOrigin_Switch_LeftTrigger_Pull,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTTRIGGER_CLICK = k_EInputActionOrigin_Switch_LeftTrigger_Click,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTTRIGGER_PULL = k_EInputActionOrigin_Switch_RightTrigger_Pull,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTTRIGGER_CLICK = k_EInputActionOrigin_Switch_RightTrigger_Click,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTSTICK_MOVE = k_EInputActionOrigin_Switch_LeftStick_Move,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTSTICK_CLICK = k_EInputActionOrigin_Switch_LeftStick_Click,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTSTICK_DPADNORTH = k_EInputActionOrigin_Switch_LeftStick_DPadNorth,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTSTICK_DPADSOUTH = k_EInputActionOrigin_Switch_LeftStick_DPadSouth,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTSTICK_DPADWEST = k_EInputActionOrigin_Switch_LeftStick_DPadWest,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTSTICK_DPADEAST = k_EInputActionOrigin_Switch_LeftStick_DPadEast,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTSTICK_MOVE = k_EInputActionOrigin_Switch_RightStick_Move,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTSTICK_CLICK = k_EInputActionOrigin_Switch_RightStick_Click,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTSTICK_DPADNORTH = k_EInputActionOrigin_Switch_RightStick_DPadNorth,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTSTICK_DPADSOUTH = k_EInputActionOrigin_Switch_RightStick_DPadSouth,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTSTICK_DPADWEST = k_EInputActionOrigin_Switch_RightStick_DPadWest,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTSTICK_DPADEAST = k_EInputActionOrigin_Switch_RightStick_DPadEast,
		INPUT_ACTION_ORIGIN_SWITCH_DPAD_NORTH = k_EInputActionOrigin_Switch_DPad_North,
		INPUT_ACTION_ORIGIN_SWITCH_DPAD_SOUTH = k_EInputActionOrigin_Switch_DPad_South,
		INPUT_ACTION_ORIGIN_SWITCH_DPAD_WEST = k_EInputActionOrigin_Switch_DPad_West,
		INPUT_ACTION_ORIGIN_SWITCH_DPAD_EAST = k_EInputActionOrigin_Switch_DPad_East,
		INPUT_ACTION_ORIGIN_SWITCH_PROGYRO_MOVE = k_EInputActionOrigin_Switch_ProGyro_Move,
		INPUT_ACTION_ORIGIN_SWITCH_PROGYRO_PITCH = k_EInputActionOrigin_Switch_ProGyro_Pitch,
		INPUT_ACTION_ORIGIN_SWITCH_PROGYRO_YAW = k_EInputActionOrigin_Switch_ProGyro_Yaw,
		INPUT_ACTION_ORIGIN_SWITCH_PROGYRO_ROLL = k_EInputActionOrigin_Switch_ProGyro_Roll,
		INPUT_ACTION_ORIGIN_SWITCH_DPAD_MOVE = k_EInputActionOrigin_Switch_DPad_Move,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED1 = k_EInputActionOrigin_Switch_Reserved1,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED2 = k_EInputActionOrigin_Switch_Reserved2,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED3 = k_EInputActionOrigin_Switch_Reserved3,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED4 = k_EInputActionOrigin_Switch_Reserved4,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED5 = k_EInputActionOrigin_Switch_Reserved5,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED6 = k_EInputActionOrigin_Switch_Reserved6,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED7 = k_EInputActionOrigin_Switch_Reserved7,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED8 = k_EInputActionOrigin_Switch_Reserved8,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED9 = k_EInputActionOrigin_Switch_Reserved9,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED10 = k_EInputActionOrigin_Switch_Reserved10,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_MOVE = k_EInputActionOrigin_Switch_RightGyro_Move,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_PITCH = k_EInputActionOrigin_Switch_RightGyro_Pitch,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_YAW = k_EInputActionOrigin_Switch_RightGyro_Yaw,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_ROLL = k_EInputActionOrigin_Switch_RightGyro_Roll,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_MOVE = k_EInputActionOrigin_Switch_LeftGyro_Move,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_PITCH = k_EInputActionOrigin_Switch_LeftGyro_Pitch,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_YAW = k_EInputActionOrigin_Switch_LeftGyro_Yaw,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_ROLL = k_EInputActionOrigin_Switch_LeftGyro_Roll,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTGRIP_LOWER = k_EInputActionOrigin_Switch_LeftGrip_Lower,
		INPUT_ACTION_ORIGIN_SWITCH_LEFTGRIP_UPPER = k_EInputActionOrigin_Switch_LeftGrip_Upper,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTGRIP_LOWER = k_EInputActionOrigin_Switch_RightGrip_Lower,
		INPUT_ACTION_ORIGIN_SWITCH_RIGHTGRIP_UPPER = k_EInputActionOrigin_Switch_RightGrip_Upper,
		INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_N = k_EInputActionOrigin_Switch_JoyConButton_N,
		INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_E = k_EInputActionOrigin_Switch_JoyConButton_E,
		INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_S = k_EInputActionOrigin_Switch_JoyConButton_S,
		INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_W = k_EInputActionOrigin_Switch_JoyConButton_W,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED15 = k_EInputActionOrigin_Switch_Reserved15,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED16 = k_EInputActionOrigin_Switch_Reserved16,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED17 = k_EInputActionOrigin_Switch_Reserved17,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED18 = k_EInputActionOrigin_Switch_Reserved18,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED19 = k_EInputActionOrigin_Switch_Reserved19,
		INPUT_ACTION_ORIGIN_SWITCH_RESERVED20 = k_EInputActionOrigin_Switch_Reserved20,
		INPUT_ACTION_ORIGIN_PS5_X = k_EInputActionOrigin_PS5_X,
		INPUT_ACTION_ORIGIN_PS5_CIRCLE = k_EInputActionOrigin_PS5_Circle,
		INPUT_ACTION_ORIGIN_PS5_TRIANGLE = k_EInputActionOrigin_PS5_Triangle,
		INPUT_ACTION_ORIGIN_PS5_SQUARE = k_EInputActionOrigin_PS5_Square,
		INPUT_ACTION_ORIGIN_PS5_LEFTBUMPER = k_EInputActionOrigin_PS5_LeftBumper,
		INPUT_ACTION_ORIGIN_PS5_RIGHTBUMPER = k_EInputActionOrigin_PS5_RightBumper,
		INPUT_ACTION_ORIGIN_PS5_OPTION = k_EInputActionOrigin_PS5_Option,
		INPUT_ACTION_ORIGIN_PS5_CREATE = k_EInputActionOrigin_PS5_Create,
		INPUT_ACTION_ORIGIN_PS5_MUTE = k_EInputActionOrigin_PS5_Mute,
		INPUT_ACTION_ORIGIN_PS5_LEFTPAD_TOUCH = k_EInputActionOrigin_PS5_LeftPad_Touch,
		INPUT_ACTION_ORIGIN_PS5_LEFTPAD_SWIPE = k_EInputActionOrigin_PS5_LeftPad_Swipe,
		INPUT_ACTION_ORIGIN_PS5_LEFTPAD_CLICK = k_EInputActionOrigin_PS5_LeftPad_Click,
		INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADNORTH = k_EInputActionOrigin_PS5_LeftPad_DPadNorth,
		INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADSOUTH = k_EInputActionOrigin_PS5_LeftPad_DPadSouth,
		INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADWEST = k_EInputActionOrigin_PS5_LeftPad_DPadWest,
		INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADEAST = k_EInputActionOrigin_PS5_LeftPad_DPadEast,
		INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_TOUCH = k_EInputActionOrigin_PS5_RightPad_Touch,
		INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_SWIPE = k_EInputActionOrigin_PS5_RightPad_Swipe,
		INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_CLICK = k_EInputActionOrigin_PS5_RightPad_Click,
		INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADNORTH = k_EInputActionOrigin_PS5_RightPad_DPadNorth,
		INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADSOUTH = k_EInputActionOrigin_PS5_RightPad_DPadSouth,
		INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADWEST = k_EInputActionOrigin_PS5_RightPad_DPadWest,
		INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADEAST = k_EInputActionOrigin_PS5_RightPad_DPadEast,
		INPUT_ACTION_ORIGIN_PS5_CENTERPAD_TOUCH = k_EInputActionOrigin_PS5_CenterPad_Touch,
		INPUT_ACTION_ORIGIN_PS5_CENTERPAD_SWIPE = k_EInputActionOrigin_PS5_CenterPad_Swipe,
		INPUT_ACTION_ORIGIN_PS5_CENTERPAD_CLICK = k_EInputActionOrigin_PS5_CenterPad_Click,
		INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADNORTH = k_EInputActionOrigin_PS5_CenterPad_DPadNorth,
		INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADSOUTH = k_EInputActionOrigin_PS5_CenterPad_DPadSouth,
		INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADWEST = k_EInputActionOrigin_PS5_CenterPad_DPadWest,
		INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADEAST = k_EInputActionOrigin_PS5_CenterPad_DPadEast,
		INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_PULL = k_EInputActionOrigin_PS5_LeftTrigger_Pull,
		INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_CLICK = k_EInputActionOrigin_PS5_LeftTrigger_Click,
		INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_PULL = k_EInputActionOrigin_PS5_RightTrigger_Pull,
		INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_CLICK = k_EInputActionOrigin_PS5_RightTrigger_Click,
		INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_MOVE = k_EInputActionOrigin_PS5_LeftStick_Move,
		INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_CLICK = k_EInputActionOrigin_PS5_LeftStick_Click,
		INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADNORTH = k_EInputActionOrigin_PS5_LeftStick_DPadNorth,
		INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADSOUTH = k_EInputActionOrigin_PS5_LeftStick_DPadSouth,
		INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADWEST = k_EInputActionOrigin_PS5_LeftStick_DPadWest,
		INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADEAST = k_EInputActionOrigin_PS5_LeftStick_DPadEast,
		INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_MOVE = k_EInputActionOrigin_PS5_RightStick_Move,
		INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_CLICK = k_EInputActionOrigin_PS5_RightStick_Click,
		INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADNORTH = k_EInputActionOrigin_PS5_RightStick_DPadNorth,
		INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADSOUTH = k_EInputActionOrigin_PS5_RightStick_DPadSouth,
		INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADWEST = k_EInputActionOrigin_PS5_RightStick_DPadWest,
		INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADEAST = k_EInputActionOrigin_PS5_RightStick_DPadEast,
		INPUT_ACTION_ORIGIN_PS5_DPAD_NORTH = k_EInputActionOrigin_PS5_DPad_North,
		INPUT_ACTION_ORIGIN_PS5_DPAD_SOUTH = k_EInputActionOrigin_PS5_DPad_South,
		INPUT_ACTION_ORIGIN_PS5_DPAD_WEST = k_EInputActionOrigin_PS5_DPad_West,
		INPUT_ACTION_ORIGIN_PS5_DPAD_EAST = k_EInputActionOrigin_PS5_DPad_East,
		INPUT_ACTION_ORIGIN_PS5_GYRO_MOVE = k_EInputActionOrigin_PS5_Gyro_Move,
		INPUT_ACTION_ORIGIN_PS5_GYRO_PITCH = k_EInputActionOrigin_PS5_Gyro_Pitch,
		INPUT_ACTION_ORIGIN_PS5_GYRO_YAW = k_EInputActionOrigin_PS5_Gyro_Yaw,
		INPUT_ACTION_ORIGIN_PS5_GYRO_ROLL = k_EInputActionOrigin_PS5_Gyro_Roll,
		INPUT_ACTION_ORIGIN_PS5_DPAD_MOVE = k_EInputActionOrigin_PS5_DPad_Move,
		INPUT_ACTION_ORIGIN_PS5_LEFTGRIP = k_EInputActionOrigin_PS5_LeftGrip,
		INPUT_ACTION_ORIGIN_PS5_RIGHTGRIP = k_EInputActionOrigin_PS5_RightGrip,
		INPUT_ACTION_ORIGIN_PS5_LEFTFN = k_EInputActionOrigin_PS5_LeftFn,
		INPUT_ACTION_ORIGIN_PS5_RIGHTFN = k_EInputActionOrigin_PS5_RightFn,
		INPUT_ACTION_ORIGIN_PS5_RESERVED5 = k_EInputActionOrigin_PS5_Reserved5,
		INPUT_ACTION_ORIGIN_PS5_RESERVED6 = k_EInputActionOrigin_PS5_Reserved6,
		INPUT_ACTION_ORIGIN_PS5_RESERVED7 = k_EInputActionOrigin_PS5_Reserved7,
		INPUT_ACTION_ORIGIN_PS5_RESERVED8 = k_EInputActionOrigin_PS5_Reserved8,
		INPUT_ACTION_ORIGIN_PS5_RESERVED9 = k_EInputActionOrigin_PS5_Reserved9,
		INPUT_ACTION_ORIGIN_PS5_RESERVED10 = k_EInputActionOrigin_PS5_Reserved10,
		INPUT_ACTION_ORIGIN_PS5_RESERVED11 = k_EInputActionOrigin_PS5_Reserved11,
		INPUT_ACTION_ORIGIN_PS5_RESERVED12 = k_EInputActionOrigin_PS5_Reserved12,
		INPUT_ACTION_ORIGIN_PS5_RESERVED13 = k_EInputActionOrigin_PS5_Reserved13,
		INPUT_ACTION_ORIGIN_PS5_RESERVED14 = k_EInputActionOrigin_PS5_Reserved14,
		INPUT_ACTION_ORIGIN_PS5_RESERVED15 = k_EInputActionOrigin_PS5_Reserved15,
		INPUT_ACTION_ORIGIN_PS5_RESERVED16 = k_EInputActionOrigin_PS5_Reserved16,
		INPUT_ACTION_ORIGIN_PS5_RESERVED17 = k_EInputActionOrigin_PS5_Reserved17,
		INPUT_ACTION_ORIGIN_PS5_RESERVED18 = k_EInputActionOrigin_PS5_Reserved18,
		INPUT_ACTION_ORIGIN_PS5_RESERVED19 = k_EInputActionOrigin_PS5_Reserved19,
		INPUT_ACTION_ORIGIN_PS5_RESERVED20 = k_EInputActionOrigin_PS5_Reserved20,
		INPUT_ACTION_ORIGIN_STEAMDECK_A = k_EInputActionOrigin_SteamDeck_A,
		INPUT_ACTION_ORIGIN_STEAMDECK_B = k_EInputActionOrigin_SteamDeck_B,
		INPUT_ACTION_ORIGIN_STEAMDECK_X = k_EInputActionOrigin_SteamDeck_X,
		INPUT_ACTION_ORIGIN_STEAMDECK_Y = k_EInputActionOrigin_SteamDeck_Y,
		INPUT_ACTION_ORIGIN_STEAMDECK_L1 = k_EInputActionOrigin_SteamDeck_L1,
		INPUT_ACTION_ORIGIN_STEAMDECK_R1 = k_EInputActionOrigin_SteamDeck_R1,
		INPUT_ACTION_ORIGIN_STEAMDECK_MENU = k_EInputActionOrigin_SteamDeck_Menu,
		INPUT_ACTION_ORIGIN_STEAMDECK_VIEW = k_EInputActionOrigin_SteamDeck_View,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_TOUCH = k_EInputActionOrigin_SteamDeck_LeftPad_Touch,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_SWIPE = k_EInputActionOrigin_SteamDeck_LeftPad_Swipe,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_CLICK = k_EInputActionOrigin_SteamDeck_LeftPad_Click,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADNORTH = k_EInputActionOrigin_SteamDeck_LeftPad_DPadNorth,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADSOUTH = k_EInputActionOrigin_SteamDeck_LeftPad_DPadSouth,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADWEST = k_EInputActionOrigin_SteamDeck_LeftPad_DPadWest,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADEAST = k_EInputActionOrigin_SteamDeck_LeftPad_DPadEast,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_TOUCH = k_EInputActionOrigin_SteamDeck_RightPad_Touch,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_SWIPE = k_EInputActionOrigin_SteamDeck_RightPad_Swipe,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_CLICK = k_EInputActionOrigin_SteamDeck_RightPad_Click,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADNORTH = k_EInputActionOrigin_SteamDeck_RightPad_DPadNorth,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADSOUTH = k_EInputActionOrigin_SteamDeck_RightPad_DPadSouth,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADWEST = k_EInputActionOrigin_SteamDeck_RightPad_DPadWest,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADEAST = k_EInputActionOrigin_SteamDeck_RightPad_DPadEast,
		INPUT_ACTION_ORIGIN_STEAMDECK_L2_SOFTPULL = k_EInputActionOrigin_SteamDeck_L2_SoftPull,
		INPUT_ACTION_ORIGIN_STEAMDECK_L2 = k_EInputActionOrigin_SteamDeck_L2,
		INPUT_ACTION_ORIGIN_STEAMDECK_R2_SOFTPULL = k_EInputActionOrigin_SteamDeck_R2_SoftPull,
		INPUT_ACTION_ORIGIN_STEAMDECK_R2 = k_EInputActionOrigin_SteamDeck_R2,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_MOVE = k_EInputActionOrigin_SteamDeck_LeftStick_Move,
		INPUT_ACTION_ORIGIN_STEAMDECK_L3 = k_EInputActionOrigin_SteamDeck_L3,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADNORTH = k_EInputActionOrigin_SteamDeck_LeftStick_DPadNorth,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADSOUTH = k_EInputActionOrigin_SteamDeck_LeftStick_DPadSouth,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADWEST = k_EInputActionOrigin_SteamDeck_LeftStick_DPadWest,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADEAST = k_EInputActionOrigin_SteamDeck_LeftStick_DPadEast,
		INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_TOUCH = k_EInputActionOrigin_SteamDeck_LeftStick_Touch,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_MOVE = k_EInputActionOrigin_SteamDeck_RightStick_Move,
		INPUT_ACTION_ORIGIN_STEAMDECK_R3 = k_EInputActionOrigin_SteamDeck_R3,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADNORTH = k_EInputActionOrigin_SteamDeck_RightStick_DPadNorth,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADSOUTH = k_EInputActionOrigin_SteamDeck_RightStick_DPadSouth,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADWEST = k_EInputActionOrigin_SteamDeck_RightStick_DPadWest,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADEAST = k_EInputActionOrigin_SteamDeck_RightStick_DPadEast,
		INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_TOUCH = k_EInputActionOrigin_SteamDeck_RightStick_Touch,
		INPUT_ACTION_ORIGIN_STEAMDECK_L4 = k_EInputActionOrigin_SteamDeck_L4,
		INPUT_ACTION_ORIGIN_STEAMDECK_R4 = k_EInputActionOrigin_SteamDeck_R4,
		INPUT_ACTION_ORIGIN_STEAMDECK_L5 = k_EInputActionOrigin_SteamDeck_L5,
		INPUT_ACTION_ORIGIN_STEAMDECK_R5 = k_EInputActionOrigin_SteamDeck_R5,
		INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_MOVE = k_EInputActionOrigin_SteamDeck_DPad_Move,
		INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_NORTH = k_EInputActionOrigin_SteamDeck_DPad_North,
		INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_SOUTH = k_EInputActionOrigin_SteamDeck_DPad_South,
		INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_WEST = k_EInputActionOrigin_SteamDeck_DPad_West,
		INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_EAST = k_EInputActionOrigin_SteamDeck_DPad_East,
		INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_MOVE = k_EInputActionOrigin_SteamDeck_Gyro_Move,
		INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_PITCH = k_EInputActionOrigin_SteamDeck_Gyro_Pitch,
		INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_YAW = k_EInputActionOrigin_SteamDeck_Gyro_Yaw,
		INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_ROLL = k_EInputActionOrigin_SteamDeck_Gyro_Roll,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED1 = k_EInputActionOrigin_SteamDeck_Reserved1,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED2 = k_EInputActionOrigin_SteamDeck_Reserved2,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED3 = k_EInputActionOrigin_SteamDeck_Reserved3,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED4 = k_EInputActionOrigin_SteamDeck_Reserved4,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED5 = k_EInputActionOrigin_SteamDeck_Reserved5,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED6 = k_EInputActionOrigin_SteamDeck_Reserved6,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED7 = k_EInputActionOrigin_SteamDeck_Reserved7,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED8 = k_EInputActionOrigin_SteamDeck_Reserved8,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED9 = k_EInputActionOrigin_SteamDeck_Reserved9,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED10 = k_EInputActionOrigin_SteamDeck_Reserved10,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED11 = k_EInputActionOrigin_SteamDeck_Reserved11,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED12 = k_EInputActionOrigin_SteamDeck_Reserved12,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED13 = k_EInputActionOrigin_SteamDeck_Reserved13,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED14 = k_EInputActionOrigin_SteamDeck_Reserved14,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED15 = k_EInputActionOrigin_SteamDeck_Reserved15,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED16 = k_EInputActionOrigin_SteamDeck_Reserved16,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED17 = k_EInputActionOrigin_SteamDeck_Reserved17,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED18 = k_EInputActionOrigin_SteamDeck_Reserved18,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED19 = k_EInputActionOrigin_SteamDeck_Reserved19,
		INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED20 = k_EInputActionOrigin_SteamDeck_Reserved20,
		INPUT_ACTION_ORIGIN_COUNT = k_EInputActionOrigin_Count,
		INPUT_ACTION_ORIGIN_MAXIMUM_POSSIBLE_VALUE = k_EInputActionOrigin_MaximumPossibleValue
	};
	enum InputConfigurationEnableType {
		INPUT_CONFIGURATION_ENABLE_TYPE_NONE = k_ESteamInputConfigurationEnableType_None,
		INPUT_CONFIGURATION_ENABLE_TYPE_PLAYSTATION = k_ESteamInputConfigurationEnableType_Playstation,
		INPUT_CONFIGURATION_ENABLE_TYPE_XBOX = k_ESteamInputConfigurationEnableType_Xbox,
		INPUT_CONFIGURATION_ENABLE_TYPE_GENERIC = k_ESteamInputConfigurationEnableType_Generic,
		INPUT_CONFIGURATION_ENABLE_TYPE_SWITCH = k_ESteamInputConfigurationEnableType_Switch
	};
	enum InputGlyphSize {
		INPUT_GLYPH_SIZE_SMALL = k_ESteamInputGlyphSize_Small,
		INPUT_GLYPH_SIZE_MEDIUM = k_ESteamInputGlyphSize_Medium,
		INPUT_GLYPH_SIZE_LARGE = k_ESteamInputGlyphSize_Large,
		INPUT_GLYPH_SIZE_COUNT = k_ESteamInputGlyphSize_Count
	};
	enum InputGlyphStyle {
		INPUT_GLYPH_STYLE_KNOCKOUT = ESteamInputGlyphStyle_Knockout,
		INPUT_GLYPH_STYLE_LIGHT = ESteamInputGlyphStyle_Light,
		INPUT_GLYPH_STYLE_DARK = ESteamInputGlyphStyle_Dark,
		INPUT_GLYPH_STYLE_NEUTRAL_COLOR_ABXY = ESteamInputGlyphStyle_NeutralColorABXY,
		INPUT_GLYPH_STYLE_SOLID_ABXY = ESteamInputGlyphStyle_SolidABXY
	};
	enum InputLEDFlag {
		INPUT_LED_FLAG_SET_COLOR = k_ESteamInputLEDFlag_SetColor,
		INPUT_LED_FLAG_RESTORE_USER_DEFAULT = k_ESteamInputLEDFlag_RestoreUserDefault
	};
	enum InputSourceMode {
		INPUT_SOURCE_MODE_NONE = k_EInputSourceMode_None,
		INPUT_SOURCE_MODE_DPAD = k_EInputSourceMode_Dpad,
		INPUT_SOURCE_MODE_BUTTONS = k_EInputSourceMode_Buttons,
		INPUT_SOURCE_MODE_FOUR_BUTTONS = k_EInputSourceMode_FourButtons,
		INPUT_SOURCE_MODE_ABSOLUTE_MOUSE = k_EInputSourceMode_AbsoluteMouse,
		INPUT_SOURCE_MODE_RELATIVE_MOUSE = k_EInputSourceMode_RelativeMouse,
		INPUT_SOURCE_MODE_JOYSTICK_MOVE = k_EInputSourceMode_JoystickMove,
		INPUT_SOURCE_MODE_JOYSTICK_MOUSE = k_EInputSourceMode_JoystickMouse,
		INPUT_SOURCE_MODE_JOYSTICK_CAMERA = k_EInputSourceMode_JoystickCamera,
		INPUT_SOURCE_MODE_SCROLL_WHEEL = k_EInputSourceMode_ScrollWheel,
		INPUT_SOURCE_MODE_TRIGGER = k_EInputSourceMode_Trigger,
		INPUT_SOURCE_MODE_TOUCH_MENU = k_EInputSourceMode_TouchMenu,
		INPUT_SOURCE_MODE_MOUSE_JOYSTICK = k_EInputSourceMode_MouseJoystick,
		INPUT_SOURCE_MODE_MOUSE_REGION = k_EInputSourceMode_MouseRegion,
		INPUT_SOURCE_MODE_RADIAL_MENU = k_EInputSourceMode_RadialMenu,
		INPUT_SOURCE_MODE_SINGLE_BUTTON = k_EInputSourceMode_SingleButton,
		INPUT_SOURCE_MODE_SWITCH = k_EInputSourceMode_Switches
	};
	enum InputType {
		INPUT_TYPE_UNKNOWN = k_ESteamInputType_Unknown,
		INPUT_TYPE_STEAM_CONTROLLER = k_ESteamInputType_SteamController,
		INPUT_TYPE_XBOX360_CONTROLLER = k_ESteamInputType_XBox360Controller,
		INPUT_TYPE_XBOXONE_CONTROLLER = k_ESteamInputType_XBoxOneController,
		INPUT_TYPE_GENERIC_XINPUT = k_ESteamInputType_GenericGamepad,
		INPUT_TYPE_PS4_CONTROLLER = k_ESteamInputType_PS4Controller,
		INPUT_TYPE_APPLE_MFI_CONTROLLER = k_ESteamInputType_AppleMFiController,
		INPUT_TYPE_ANDROID_CONTROLLER = k_ESteamInputType_AndroidController,
		INPUT_TYPE_SWITCH_JOYCON_PAIR = k_ESteamInputType_SwitchJoyConPair,
		INPUT_TYPE_SWITCH_JOYCON_SINGLE = k_ESteamInputType_SwitchJoyConSingle,
		INPUT_TYPE_SWITCH_PRO_CONTROLLER = k_ESteamInputType_SwitchProController,
		INPUT_TYPE_MOBILE_TOUCH = k_ESteamInputType_MobileTouch,
		INPUT_TYPE_PS3_CONTROLLER = k_ESteamInputType_PS3Controller,
		INPUT_TYPE_PS5_CONTROLLER = k_ESteamInputType_PS5Controller,
		INPUT_TYPE_STEAM_DECK_CONTROLLER = k_ESteamInputType_SteamDeckController,
		INPUT_TYPE_COUNT = k_ESteamInputType_Count,
		INPUT_TYPE_MAXIMUM_POSSIBLE_VALUE = k_ESteamInputType_MaximumPossibleValue
	};
	enum SCEPadTriggerEffectMode {
		PAD_TRIGGER_EFFECT_MODE_OFF = SCE_PAD_TRIGGER_EFFECT_MODE_OFF,
		PAD_TRIGGER_EFFECT_MODE_FEEDBACK = SCE_PAD_TRIGGER_EFFECT_MODE_FEEDBACK,
		PAD_TRIGGER_EFFECT_MODE_WEAPON = SCE_PAD_TRIGGER_EFFECT_MODE_WEAPON,
		PAD_TRIGGER_EFFECT_MODE_VIBRATION = SCE_PAD_TRIGGER_EFFECT_MODE_VIBRATION,
		PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_FEEDBACK = SCE_PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_FEEDBACK,
		PAD_TRIGGER_EFFECT_MODE_SLOPE_FEEDBACK = SCE_PAD_TRIGGER_EFFECT_MODE_SLOPE_FEEDBACK,
		PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_VIBRATION = SCE_PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_VIBRATION
	};
	enum XboxOrigin {
		XBOX_ORIGIN_A = k_EXboxOrigin_A,
		XBOX_ORIGIN_B = k_EXboxOrigin_B,
		XBOX_ORIGIN_X = k_EXboxOrigin_X,
		XBOX_ORIGIN_Y = k_EXboxOrigin_Y,
		XBOX_ORIGIN_LEFT_BUMPER = k_EXboxOrigin_LeftBumper,
		XBOX_ORIGIN_RIGHT_BUMPER = k_EXboxOrigin_RightBumper,
		XBOX_ORIGIN_MENU = k_EXboxOrigin_Menu,
		XBOX_ORIGIN_VIEW = k_EXboxOrigin_View,
		XBOX_ORIGIN_LEFT_TRIGGER_PULL = k_EXboxOrigin_LeftTrigger_Pull,
		XBOX_ORIGIN_LEFT_TRIGGER_CLICK = k_EXboxOrigin_LeftTrigger_Click,
		XBOX_ORIGIN_RIGHT_TRIGGER_PULL = k_EXboxOrigin_RightTrigger_Pull,
		XBOX_ORIGIN_RIGHT_TRIGGER_CLICK = k_EXboxOrigin_RightTrigger_Click,
		XBOX_ORIGIN_LEFT_STICK_MOVE = k_EXboxOrigin_LeftStick_Move,
		XBOX_ORIGIN_LEFT_STICK_CLICK = k_EXboxOrigin_LeftStick_Click,
		XBOX_ORIGIN_LEFT_STICK_DPAD_NORTH = k_EXboxOrigin_LeftStick_DPadNorth,
		XBOX_ORIGIN_LEFT_STICK_DPAD_SOUTH = k_EXboxOrigin_LeftStick_DPadSouth,
		XBOX_ORIGIN_LEFT_STICK_DPAD_WEST = k_EXboxOrigin_LeftStick_DPadWest,
		XBOX_ORIGIN_LEFT_STICK_DPAD_EAT = k_EXboxOrigin_LeftStick_DPadEast,
		XBOX_ORIGIN_RIGHT_STICK_MOVE = k_EXboxOrigin_RightStick_Move,
		XBOX_ORIGIN_RIGHT_STICK_CLICK = k_EXboxOrigin_RightStick_Click,
		XBOX_ORIGIN_RIGHT_STICK_DPAD_NORTH = k_EXboxOrigin_RightStick_DPadNorth,
		XBOX_ORIGIN_RIGHT_STICK_DPAD_SOUTH = k_EXboxOrigin_RightStick_DPadSouth,
		XBOX_ORIGIN_RIGHT_STICK_DPAD_WEST = k_EXboxOrigin_RightStick_DPadWest,
		XBOX_ORIGIN_RIGHT_STICK_DPAD_EAST = k_EXboxOrigin_RightStick_DPadEast,
		XBOX_ORIGIN_DPAD_NORTH = k_EXboxOrigin_DPad_North,
		XBOX_ORIGIN_DPAD_SOUTH = k_EXboxOrigin_DPad_South,
		XBOX_ORIGIN_DPAD_WEST = k_EXboxOrigin_DPad_West,
		XBOX_ORIGIN_DPAD_EAST = k_EXboxOrigin_DPad_East,
		XBOX_ORIGIN_COUNT = k_EXboxOrigin_Count
	};

	// Inventory enums
	enum ItemFlags {
		STEAM_ITEM_NO_TRADE = k_ESteamItemNoTrade,
		STEAM_ITEM_REMOVED = k_ESteamItemRemoved,
		STEAM_ITEM_CONSUMED = k_ESteamItemConsumed
	};

	// Matchmaking enums
	enum ChatMemberStateChange {
		CHAT_MEMBER_STATE_CHANGE_ENTERED = k_EChatMemberStateChangeEntered,
		CHAT_MEMBER_STATE_CHANGE_LEFT = k_EChatMemberStateChangeLeft,
		CHAT_MEMBER_STATE_CHANGE_DISCONNECTED = k_EChatMemberStateChangeDisconnected,
		CHAT_MEMBER_STATE_CHANGE_KICKED = k_EChatMemberStateChangeKicked,
		CHAT_MEMBER_STATE_CHANGE_BANNED = k_EChatMemberStateChangeBanned
	};
	enum LobbyComparison {
		LOBBY_COMPARISON_EQUAL_TO_OR_LESS_THAN = k_ELobbyComparisonEqualToOrLessThan,
		LOBBY_COMPARISON_LESS_THAN = k_ELobbyComparisonLessThan,
		LOBBY_COMPARISON_EQUAL = k_ELobbyComparisonEqual,
		LOBBY_COMPARISON_GREATER_THAN = k_ELobbyComparisonGreaterThan,
		OBBY_COMPARISON_EQUAL_TO_GREATER_THAN = k_ELobbyComparisonEqualToOrGreaterThan,
		LOBBY_COMPARISON_NOT_EQUAL = k_ELobbyComparisonNotEqual
	};
	enum LobbyDistanceFilter {
		LOBBY_DISTANCE_FILTER_CLOSE = k_ELobbyDistanceFilterClose,
		LOBBY_DISTANCE_FILTER_DEFAULT = k_ELobbyDistanceFilterDefault,
		LOBBY_DISTANCE_FILTER_FAR = k_ELobbyDistanceFilterFar,
		LOBBY_DISTANCE_FILTER_WORLDWIDE = k_ELobbyDistanceFilterWorldwide
	};
	enum LobbyType {
		LOBBY_TYPE_PRIVATE = k_ELobbyTypePrivate,
		LOBBY_TYPE_FRIENDS_ONLY = k_ELobbyTypeFriendsOnly,
		LOBBY_TYPE_PUBLIC = k_ELobbyTypePublic,
		LOBBY_TYPE_INVISIBLE = k_ELobbyTypeInvisible,
		LOBBY_TYPE_PRIVATE_UNIQUE = k_ELobbyTypePrivateUnique

	};

	// Matchmaking Servers enums
	enum MatchMakingServerResponse {
		SERVER_RESPONDED = eServerResponded,
		SERVER_FAILED_TO_RESPOND = eServerFailedToRespond,
		NO_SERVERS_LISTED_ON_MASTER_SERVER = eNoServersListedOnMasterServer
	};

	// Music enums
	enum AudioPlaybackStatus {
		AUDIO_PLAYBACK_UNDEFINED = AudioPlayback_Undefined,
		AUDIO_PLAYBACK_PLAYING = AudioPlayback_Playing,
		AUDIO_PLAYBACK_PAUSED = AudioPlayback_Paused,
		AUDIO_PLAYBACK_IDLE = AudioPlayback_Idle
	};

	// Networking enums
	enum P2PSend {
		P2P_SEND_UNRELIABLE = k_EP2PSendUnreliable,
		P2P_SEND_UNRELIABLE_NO_DELAY = k_EP2PSendUnreliableNoDelay,
		P2P_SEND_RELIABLE = k_EP2PSendReliable,
		P2P_SEND_RELIABLE_WITH_BUFFERING = k_EP2PSendReliableWithBuffering
	};
	enum P2PSessionError {
		P2P_SESSION_ERROR_NONE = k_EP2PSessionErrorNone,
		P2P_SESSION_ERROR_NOT_RUNNING_APP = k_EP2PSessionErrorNotRunningApp_DELETED,
		P2P_SESSION_ERROR_NO_RIGHTS_TO_APP = k_EP2PSessionErrorNoRightsToApp,
		P2P_SESSION_ERROR_DESTINATION_NOT_LOGGED_ON = k_EP2PSessionErrorDestinationNotLoggedIn_DELETED,
		P2P_SESSION_ERROR_TIMEOUT = k_EP2PSessionErrorTimeout,
		P2P_SESSION_ERROR_MAX = k_EP2PSessionErrorMax
	};
	enum SocketConnectionType {
		NET_SOCKET_CONNECTION_TYPE_NOT_CONNECTED = k_ESNetSocketConnectionTypeNotConnected,
		NET_SOCKET_CONNECTION_TYPE_UDP = k_ESNetSocketConnectionTypeUDP,
		NET_SOCKET_CONNECTION_TYPE_UDP_RELAY = k_ESNetSocketConnectionTypeUDPRelay
	};
	enum SocketState {
		NET_SOCKET_STATE_INVALID = k_ESNetSocketStateInvalid,
		NET_SOCKET_STATE_CONNECTED = k_ESNetSocketStateConnected,
		NET_SOCKET_STATE_INITIATED = k_ESNetSocketStateInitiated,
		NET_SOCKET_STATE_LOCAL_CANDIDATE_FOUND = k_ESNetSocketStateLocalCandidatesFound,
		NET_SOCKET_STATE_RECEIVED_REMOTE_CANDIDATES = k_ESNetSocketStateReceivedRemoteCandidates,
		NET_SOCKET_STATE_CHALLENGE_HANDSHAKE = k_ESNetSocketStateChallengeHandshake,
		NET_SOCKET_STATE_DISCONNECTING = k_ESNetSocketStateDisconnecting,
		NET_SOCKET_STATE_LOCAL_DISCONNECT = k_ESNetSocketStateLocalDisconnect,
		NET_SOCKET_STATE_TIMEOUT_DURING_CONNECT = k_ESNetSocketStateTimeoutDuringConnect,
		NET_SOCKET_STATE_REMOTE_END_DISCONNECTED = k_ESNetSocketStateRemoteEndDisconnected,
		NET_SOCKET_STATE_BROKEN = k_ESNetSocketStateConnectionBroken
	};

	// Networking Sockets enums
	enum NetworkingConfigValue {
		NETWORKING_CONFIG_INVALID = k_ESteamNetworkingConfig_Invalid,
		NETWORKING_CONFIG_FAKE_PACKET_LOSS_SEND = k_ESteamNetworkingConfig_FakePacketLoss_Send,
		NETWORKING_CONFIG_FAKE_PACKET_LOSS_RECV = k_ESteamNetworkingConfig_FakePacketLoss_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_LAG_SEND = k_ESteamNetworkingConfig_FakePacketLag_Send,
		NETWORKING_CONFIG_FAKE_PACKET_LAG_RECV = k_ESteamNetworkingConfig_FakePacketLag_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_REORDER_SEND = k_ESteamNetworkingConfig_FakePacketReorder_Send,
		NETWORKING_CONFIG_FAKE_PACKET_REORDER_RECV = k_ESteamNetworkingConfig_FakePacketReorder_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_REORDER_TIME = k_ESteamNetworkingConfig_FakePacketReorder_Time,
		NETWORKING_CONFIG_FAKE_PACKET_DUP_SEND = k_ESteamNetworkingConfig_FakePacketDup_Send,
		NETWORKING_CONFIG_FAKE_PACKET_DUP_REVC = k_ESteamNetworkingConfig_FakePacketDup_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_DUP_TIME_MAX = k_ESteamNetworkingConfig_FakePacketDup_TimeMax,
		NETWORKING_CONFIG_PACKET_TRACE_MAX_BYTES = k_ESteamNetworkingConfig_PacketTraceMaxBytes,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_SEND_RATE = k_ESteamNetworkingConfig_FakeRateLimit_Send_Rate,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_SEND_BURST = k_ESteamNetworkingConfig_FakeRateLimit_Send_Burst,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_RECV_RATE = k_ESteamNetworkingConfig_FakeRateLimit_Recv_Rate,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_RECV_BURST = k_ESteamNetworkingConfig_FakeRateLimit_Recv_Burst,
		NETWORKING_CONFIG_OUT_OF_ORDER_CORRECTION_WINDOW_MICROSECONDS = k_ESteamNetworkingConfig_OutOfOrderCorrectionWindowMicroseconds,
		NETWORKING_CONFIG_CONNECTION_USER_DATA = k_ESteamNetworkingConfig_ConnectionUserData,
		NETWORKING_CONFIG_TIMEOUT_INITIAL = k_ESteamNetworkingConfig_TimeoutInitial,
		NETWORKING_CONFIG_TIMEOUT_CONNECTED = k_ESteamNetworkingConfig_TimeoutConnected,
		NETWORKING_CONFIG_SEND_BUFFER_SIZE = k_ESteamNetworkingConfig_SendBufferSize,
		NETWORKING_CONFIG_RECV_BUFFER_SIZE = k_ESteamNetworkingConfig_RecvBufferSize,
		NETWORKING_CONFIG_RECV_BUFFER_MESSAGES = k_ESteamNetworkingConfig_RecvBufferMessages,
		NETWORKING_CONFIG_RECV_MAX_MESSAGE_SIZE = k_ESteamNetworkingConfig_RecvMaxMessageSize,
		NETWORKING_CONFIG_RECV_MAX_SEGMENTS_PER_PACKET = k_ESteamNetworkingConfig_RecvMaxSegmentsPerPacket,
		NETWORKING_CONFIG_SEND_RATE_MIN = k_ESteamNetworkingConfig_SendRateMin,
		NETWORKING_CONFIG_SEND_RATE_MAX = k_ESteamNetworkingConfig_SendRateMax,
		NETWORKING_CONFIG_NAGLE_TIME = k_ESteamNetworkingConfig_NagleTime,
		NETWORKING_CONFIG_IP_ALLOW_WITHOUT_AUTH = k_ESteamNetworkingConfig_IP_AllowWithoutAuth,
		NETWORKING_CONFIG_MTU_PACKET_SIZE = k_ESteamNetworkingConfig_MTU_PacketSize,
		NETWORKING_CONFIG_MTU_DATA_SIZE = k_ESteamNetworkingConfig_MTU_DataSize,
		NETWORKING_CONFIG_UNENCRYPTED = k_ESteamNetworkingConfig_Unencrypted,
		NETWORKING_CONFIG_SYMMETRIC_CONNECT = k_ESteamNetworkingConfig_SymmetricConnect,
		NETWORKING_CONFIG_LOCAL_VIRTUAL_PORT = k_ESteamNetworkingConfig_LocalVirtualPort,
		NETWORKING_CONFIG_DUAL_WIFI_ENABLE = k_ESteamNetworkingConfig_DualWifi_Enable,
		NETWORKING_CONFIG_ENABLE_DIAGNOSTICS_UI = k_ESteamNetworkingConfig_EnableDiagnosticsUI,
		NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL_INITIAL = k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFailInitial,
		NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL = k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFail,
		NETWORKING_CONFIG_SDR_CLIENT_MIN_PINGS_BEFORE_PING_ACCURATE = k_ESteamNetworkingConfig_SDRClient_MinPingsBeforePingAccurate,
		NETWORKING_CONFIG_SDR_CLIENT_SINGLE_SOCKET = k_ESteamNetworkingConfig_SDRClient_SingleSocket,
		NETWORKING_CONFIG_SDR_CLIENT_FORCE_RELAY_CLUSTER = k_ESteamNetworkingConfig_SDRClient_ForceRelayCluster,
		NETWORKING_CONFIG_SDR_CLIENT_DEV_TICKET = k_ESteamNetworkingConfig_SDRClient_DevTicket,
		NETWORKING_CONFIG_SDR_CLIENT_FORCE_PROXY_ADDR = k_ESteamNetworkingConfig_SDRClient_ForceProxyAddr,
		NETWORKING_CONFIG_SDR_CLIENT_FAKE_CLUSTER_PING = k_ESteamNetworkingConfig_SDRClient_FakeClusterPing,
		NETWORKING_CONFIG_SDR_CLIENT_LIMIT_PING_PROBES_TO_NEAREST_N = k_ESteamNetworkingConfig_SDRClient_LimitPingProbesToNearestN,
		NETWORKING_CONFIG_LOG_LEVEL_ACK_RTT = k_ESteamNetworkingConfig_LogLevel_AckRTT,
		NETWORKING_CONFIG_LOG_LEVEL_PACKET_DECODE = k_ESteamNetworkingConfig_LogLevel_PacketDecode,
		NETWORKING_CONFIG_LOG_LEVEL_MESSAGE = k_ESteamNetworkingConfig_LogLevel_Message,
		NETWORKING_CONFIG_LOG_LEVEL_PACKET_GAPS = k_ESteamNetworkingConfig_LogLevel_PacketGaps,
		NETWORKING_CONFIG_LOG_LEVEL_P2P_RENDEZVOUS = k_ESteamNetworkingConfig_LogLevel_P2PRendezvous,
		NETWORKING_CONFIG_LOG_LEVEL_SRD_RELAY_PINGS = k_ESteamNetworkingConfig_LogLevel_SDRRelayPings,
		NETWORKING_CONFIG_CALLBACK_CONNECTION_STATUS_CHANGED = k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
		NETWORKING_CONFIG_CALLBACK_AUTH_STATUS_CHANGED = k_ESteamNetworkingConfig_Callback_AuthStatusChanged,
		NETWORKING_CONFIG_CALLBACK_RELAY_NETWORK_STATUS_CHANGED = k_ESteamNetworkingConfig_Callback_RelayNetworkStatusChanged,
		NETWORKING_CONFIG_CALLBACK_MESSAGE_SESSION_REQUEST = k_ESteamNetworkingConfig_Callback_MessagesSessionRequest,
		NETWORKING_CONFIG_CALLBACK_MESSAGES_SESSION_FAILED = k_ESteamNetworkingConfig_Callback_MessagesSessionFailed,
		NETWORKING_CONFIG_CALLBACK_CREATE_CONNECTION_SIGNALING = k_ESteamNetworkingConfig_Callback_CreateConnectionSignaling,
		NETWORKING_CONFIG_CALLBACK_FAKE_IP_RESULT = k_ESteamNetworkingConfig_Callback_FakeIPResult,
		NETWORKING_CONFIG_P2P_STUN_SERVER_LIST = k_ESteamNetworkingConfig_P2P_STUN_ServerList,
		NETWORKING_CONFIG_P2P_TRANSPORT_ICE_ENABLE = k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable,
		NETWORKING_CONFIG_P2P_TRANSPORT_ICE_PENALTY = k_ESteamNetworkingConfig_P2P_Transport_ICE_Penalty,
		NETWORKING_CONFIG_P2P_TRANSPORT_SDR_PENALTY = k_ESteamNetworkingConfig_P2P_Transport_SDR_Penalty,
		NETWORKING_CONFIG_P2P_TURN_SERVER_LIST = k_ESteamNetworkingConfig_P2P_TURN_ServerList,
		NETWORKING_CONFIG_P2P_TURN_uSER_LIST = k_ESteamNetworkingConfig_P2P_TURN_UserList,
		NETWORKING_CONFIG_P2P_TURN_PASS_LIST = k_ESteamNetworkingConfig_P2P_TURN_PassList,
		//			NETWORKING_CONFIG_P2P_TRANSPORT_LAN_BEACON_PENALTY = k_ESteamNetworkingConfig_P2P_Transport_LANBeacon_Penalty,
		NETWORKING_CONFIG_P2P_TRANSPORT_ICE_IMPLEMENTATION = k_ESteamNetworkingConfig_P2P_Transport_ICE_Implementation,
		NETWORKING_CONFIG_ECN = k_ESteamNetworkingConfig_ECN,
		NETWORKING_CONFIG_VALUE_FORCE32BIT = k_ESteamNetworkingConfigValue__Force32Bit
	};
	enum NetworkingConnectionEnd {
		CONNECTION_END_INVALID = k_ESteamNetConnectionEnd_Invalid,
		CONNECTION_END_APP_MIN = k_ESteamNetConnectionEnd_App_Min,
		CONNECTION_END_APP_GENERIC = k_ESteamNetConnectionEnd_App_Generic,
		CONNECTION_END_APP_MAX = k_ESteamNetConnectionEnd_App_Max,
		CONNECTION_END_APP_EXCEPTION_MIN = k_ESteamNetConnectionEnd_AppException_Min,
		CONNECTION_END_APP_EXCEPTION_GENERIC = k_ESteamNetConnectionEnd_AppException_Generic,
		CONNECTION_END_APP_EXCEPTION_MAX = k_ESteamNetConnectionEnd_AppException_Max,
		CONNECTION_END_LOCAL_MIN = k_ESteamNetConnectionEnd_Local_Min,
		CONNECTION_END_LOCAL_OFFLINE_MODE = k_ESteamNetConnectionEnd_Local_OfflineMode,
		CONNECTION_END_LOCAL_MANY_RELAY_CONNECTIVITY = k_ESteamNetConnectionEnd_Local_ManyRelayConnectivity,
		CONNECTION_END_LOCAL_HOSTED_SERVER_PRIMARY_RELAY = k_ESteamNetConnectionEnd_Local_HostedServerPrimaryRelay,
		CONNECTION_END_LOCAL_NETWORK_CONFIG = k_ESteamNetConnectionEnd_Local_NetworkConfig,
		CONNECTION_END_LOCAL_RIGHTS = k_ESteamNetConnectionEnd_Local_Rights,
		CONNECTION_END_NO_PUBLIC_ADDRESS = k_ESteamNetConnectionEnd_Local_P2P_ICE_NoPublicAddresses,
		CONNECTION_END_LOCAL_MAX = k_ESteamNetConnectionEnd_Local_Max,
		CONNECTION_END_REMOVE_MIN = k_ESteamNetConnectionEnd_Remote_Min,
		CONNECTION_END_REMOTE_TIMEOUT = k_ESteamNetConnectionEnd_Remote_Timeout,
		CONNECTION_END_REMOTE_BAD_CRYPT = k_ESteamNetConnectionEnd_Remote_BadCrypt,
		CONNECTION_END_REMOTE_BAD_CERT = k_ESteamNetConnectionEnd_Remote_BadCert,
		CONNECTION_END_BAD_PROTOCOL_VERSION = k_ESteamNetConnectionEnd_Remote_BadProtocolVersion,
		CONNECTION_END_REMOTE_P2P_ICE_NO_PUBLIC_ADDRESSES = k_ESteamNetConnectionEnd_Remote_P2P_ICE_NoPublicAddresses,
		CONNECTION_END_REMOTE_MAX = k_ESteamNetConnectionEnd_Remote_Max,
		CONNECTION_END_MISC_MIN = k_ESteamNetConnectionEnd_Misc_Min,
		CONNECTION_END_MISC_GENERIC = k_ESteamNetConnectionEnd_Misc_Generic,
		CONNECTION_END_MISC_INTERNAL_ERROR = k_ESteamNetConnectionEnd_Misc_InternalError,
		CONNECTION_END_MISC_TIMEOUT = k_ESteamNetConnectionEnd_Misc_Timeout,
		CONNECTION_END_MISC_STEAM_CONNECTIVITY = k_ESteamNetConnectionEnd_Misc_SteamConnectivity,
		CONNECTION_END_MISC_NO_RELAY_SESSIONS_TO_CLIENT = k_ESteamNetConnectionEnd_Misc_NoRelaySessionsToClient,
		CONNECTION_END_MISC_P2P_RENDEZVOUS = k_ESteamNetConnectionEnd_Misc_P2P_Rendezvous,
		CONNECTION_END_MISC_P2P_NAT_FIREWALL = k_ESteamNetConnectionEnd_Misc_P2P_NAT_Firewall,
		CONNECTION_END_MISC_PEER_SENT_NO_CONNECTION = k_ESteamNetConnectionEnd_Misc_PeerSentNoConnection,
		CONNECTION_END_MISC_MAX = k_ESteamNetConnectionEnd_Misc_Max,
		CONNECTION_END_FORCE32BIT = k_ESteamNetConnectionEnd__Force32Bit
	};
	enum NetworkingConnectionState {
		CONNECTION_STATE_NONE = k_ESteamNetworkingConnectionState_None,
		CONNECTION_STATE_CONNECTING = k_ESteamNetworkingConnectionState_Connecting,
		CONNECTION_STATE_FINDING_ROUTE = k_ESteamNetworkingConnectionState_FindingRoute,
		CONNECTION_STATE_CONNECTED = k_ESteamNetworkingConnectionState_Connected,
		CONNECTION_STATE_CLOSED_BY_PEER = k_ESteamNetworkingConnectionState_ClosedByPeer,
		CONNECTION_STATE_PROBLEM_DETECTED_LOCALLY = k_ESteamNetworkingConnectionState_ProblemDetectedLocally,
		CONNECTION_STATE_FIN_WAIT = k_ESteamNetworkingConnectionState_FinWait,
		CONNECTION_STATE_LINGER = k_ESteamNetworkingConnectionState_Linger,
		CONNECTION_STATE_DEAD = k_ESteamNetworkingConnectionState_Dead,
		CONNECTION_STATE_FORCE_32BIT = k_ESteamNetworkingConnectionState__Force32Bit
	};
	enum NetworkingFakeIPType {
		FAKE_IP_TYPE_INVALID = k_ESteamNetworkingFakeIPType_Invalid,
		FAKE_IP_TYPE_NOT_FAKE = k_ESteamNetworkingFakeIPType_NotFake,
		FAKE_IP_TYPE_GLOBAL_IPV4 = k_ESteamNetworkingFakeIPType_GlobalIPv4,
		FAKE_IP_TYPE_LOCAL_IPV4 = k_ESteamNetworkingFakeIPType_LocalIPv4,
		FAKE_IP_TYPE_FORCE32BIT = k_ESteamNetworkingFakeIPType__Force32Bit
	};
	enum NetworkingGetConfigValueResult {
		NETWORKING_GET_CONFIG_VALUE_BAD_VALUE = k_ESteamNetworkingGetConfigValue_BadValue,
		NETWORKING_GET_CONFIG_VALUE_BAD_SCOPE_OBJ = k_ESteamNetworkingGetConfigValue_BadScopeObj,
		NETWORKING_GET_CONFIG_VALUE_BUFFER_TOO_SMALL = k_ESteamNetworkingGetConfigValue_BufferTooSmall,
		NETWORKING_GET_CONFIG_VALUE_OK = k_ESteamNetworkingGetConfigValue_OK,
		NETWORKING_GET_CONFIG_VALUE_OK_INHERITED = k_ESteamNetworkingGetConfigValue_OKInherited,
		NETWORKING_GET_CONFIG_VALUE_FORCE_32BIT = k_ESteamNetworkingGetConfigValueResult__Force32Bit
	};
	enum NetworkingIdentityType {
		IDENTITY_TYPE_INVALID = k_ESteamNetworkingIdentityType_Invalid,
		IDENTITY_TYPE_STEAMID = k_ESteamNetworkingIdentityType_SteamID,
		IDENTITY_TYPE_IP_ADDRESS = k_ESteamNetworkingIdentityType_IPAddress,
		IDENTITY_TYPE_GENERIC_STRING = k_ESteamNetworkingIdentityType_GenericString,
		IDENTITY_TYPE_GENERIC_BYTES = k_ESteamNetworkingIdentityType_GenericBytes,
		IDENTITY_TYPE_UNKNOWN_TYPE = k_ESteamNetworkingIdentityType_UnknownType,
		IDENTITY_TYPE_XBOX_PAIRWISE = k_ESteamNetworkingIdentityType_XboxPairwiseID,
		IDENTITY_TYPE_SONY_PSN = k_ESteamNetworkingIdentityType_SonyPSN,
		IDENTITY_TYPE_GOOGLE_STADIA = k_ESteamNetworkingIdentityType_GoogleStadia,
		//			IDENTITY_TYPE_NINTENDO = k_ESteamNetworkingIdentityType_NintendoNetworkServiceAccount,
		//			IDENTITY_TYPE_EPIC_GS = k_ESteamNetworkingIdentityType_EpicGameStore,
		//			IDENTITY_TYPE_WEGAME = k_ESteamNetworkingIdentityType_WeGame,
		IDENTITY_TYPE_FORCE_32BIT = k_ESteamNetworkingIdentityType__Force32bit
	};
	enum NetworkingSocketsDebugOutputType {
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_NONE = k_ESteamNetworkingSocketsDebugOutputType_None,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_BUG = k_ESteamNetworkingSocketsDebugOutputType_Bug,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_ERROR = k_ESteamNetworkingSocketsDebugOutputType_Error,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_IMPORTANT = k_ESteamNetworkingSocketsDebugOutputType_Important,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_WARNING = k_ESteamNetworkingSocketsDebugOutputType_Warning,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_MSG = k_ESteamNetworkingSocketsDebugOutputType_Msg,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_VERBOSE = k_ESteamNetworkingSocketsDebugOutputType_Verbose,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_DEBUG = k_ESteamNetworkingSocketsDebugOutputType_Debug,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_EVERYTHING = k_ESteamNetworkingSocketsDebugOutputType_Everything,
		NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_FORCE_32BIT = k_ESteamNetworkingSocketsDebugOutputType__Force32Bit
	};

	// Networking Utils enums {
	enum NetworkingAvailability {
		NETWORKING_AVAILABILITY_CANNOT_TRY = k_ESteamNetworkingAvailability_CannotTry,
		NETWORKING_AVAILABILITY_FAILED = k_ESteamNetworkingAvailability_Failed,
		NETWORKING_AVAILABILITY_PREVIOUSLY = k_ESteamNetworkingAvailability_Previously,
		NETWORKING_AVAILABILITY_RETRYING = k_ESteamNetworkingAvailability_Retrying,
		NETWORKING_AVAILABILITY_NEVER_TRIED = k_ESteamNetworkingAvailability_NeverTried,
		NETWORKING_AVAILABILITY_WAITING = k_ESteamNetworkingAvailability_Waiting,
		NETWORKING_AVAILABILITY_ATTEMPTING = k_ESteamNetworkingAvailability_Attempting,
		NETWORKING_AVAILABILITY_CURRENT = k_ESteamNetworkingAvailability_Current,
		NETWORKING_AVAILABILITY_UNKNOWN = k_ESteamNetworkingAvailability_Unknown,
		NETWORKING_AVAILABILITY_FORCE_32BIT = k_ESteamNetworkingAvailability__Force32bit
	};
	enum NetworkingConfigDataType {
		NETWORKING_CONFIG_TYPE_INT32 = k_ESteamNetworkingConfig_Int32,
		NETWORKING_CONFIG_TYPE_INT64 = k_ESteamNetworkingConfig_Int64,
		NETWORKING_CONFIG_TYPE_FLOAT = k_ESteamNetworkingConfig_Float,
		ETWORKING_CONFIG_TYPE_STRING = k_ESteamNetworkingConfig_String,
		NETWORKING_CONFIG_TYPE_FUNCTION_PTR = k_ESteamNetworkingConfig_Ptr,
		NETWORKING_CONFIG_TYPE_FORCE_32BIT = k_ESteamNetworkingConfigDataType__Force32Bit
	};
	enum NetworkingConfigScope {
		NETWORKING_CONFIG_SCOPE_GLOBAL = k_ESteamNetworkingConfig_Global,
		NETWORKING_CONFIG_SCOPE_SOCKETS_INTERFACE = k_ESteamNetworkingConfig_SocketsInterface,
		NETWORKING_CONFIG_SCOPE_LISTEN_SOCKET = k_ESteamNetworkingConfig_ListenSocket,
		NETWORKING_CONFIG_SCOPE_CONNECTION = k_ESteamNetworkingConfig_Connection,
		NETWORKING_CONFIG_SCOPE_FORCE_32BIT = k_ESteamNetworkingConfigScope__Force32Bit
	};

	// Parental Settings enums
	enum ParentalFeature {
		FEATURE_INVALID = k_EFeatureInvalid,
		FEATURE_STORE = k_EFeatureStore,
		FEATURE_COMMUNITY = k_EFeatureCommunity,
		FEATURE_PROFILE = k_EFeatureProfile,
		FEATURE_FRIENDS = k_EFeatureFriends,
		FEATURE_NEWS = k_EFeatureNews,
		FEATURE_TRADING = k_EFeatureTrading,
		FEATURE_SETTINGS = k_EFeatureSettings,
		FEATURE_CONSOLE = k_EFeatureConsole,
		FEATURE_BROWSER = k_EFeatureBrowser,
		FEATURE_PARENTAL_SETUP = k_EFeatureParentalSetup,
		FEATURE_LIBRARY = k_EFeatureLibrary,
		FEATURE_TEST = k_EFeatureTest,
		FEATURE_SITE_LICENSE = k_EFeatureSiteLicense,
		FEATURE_KIOSK_MODE = k_EFeatureKioskMode_Deprecated,
		FEATURE_MAX = k_EFeatureMax
	};

	// Steam Parties enums
	enum PartyBeaconLocationData {
		STEAM_PARTY_BEACON_LOCATION_DATA = k_ESteamPartyBeaconLocationDataInvalid,
		STEAM_PARTY_BEACON_LOCATION_DATA_NAME = k_ESteamPartyBeaconLocationDataName,
		STEAM_PARTY_BEACON_LOCATION_DATA_URL_SMALL = k_ESteamPartyBeaconLocationDataIconURLSmall,
		STEAM_PARTY_BEACON_LOCATION_DATA_URL_MEDIUM = k_ESteamPartyBeaconLocationDataIconURLMedium,
		STEAM_PARTY_BEACON_LOCATION_DATA_URL_LARGE = k_ESteamPartyBeaconLocationDataIconURLLarge
	};
	enum PartyBeaconLocationType {
		STEAM_PARTY_BEACON_LOCATIONTYPE_INVALID = k_ESteamPartyBeaconLocationType_Invalid,
		STEAM_PARTY_BEACON_LOCATIONTYPE_CHAT_GROUP = k_ESteamPartyBeaconLocationType_ChatGroup,
		STEAM_PARTY_BEACON_LOCATION_TYPE_MAX = k_ESteamPartyBeaconLocationType_Max
	};

	// Remote Play enums
	enum DeviceFormFactor {
		FORM_FACTOR_UNKNOWN = k_ESteamDeviceFormFactorUnknown,
		FORM_FACTOR_PHONE = k_ESteamDeviceFormFactorPhone,
		FORM_FACTOR_TABLET = k_ESteamDeviceFormFactorTablet,
		FORM_FACTOR_COMPUTER = k_ESteamDeviceFormFactorComputer,
		FORM_FACTOR_TV = k_ESteamDeviceFormFactorTV,
		FORM_FACTOR_VR_HEADSET = k_ESteamDeviceFormFactorVRHeadset
	};

	// Remote Storage enums
	enum FilePathType {
		FILE_PATH_TYPE_INVALID = k_ERemoteStorageFilePathType_Invalid,
		FILE_PATH_TYPE_ABSOLUTE = k_ERemoteStorageFilePathType_Absolute,
		FILE_PATH_TYPE_API_FILENAME = k_ERemoteStorageFilePathType_APIFilename
	};
	enum LocalFileChange {
		LOCAL_FILE_CHANGE_INVALID = k_ERemoteStorageLocalFileChange_Invalid,
		LOCAL_FILE_CHANGE_FILE_UPDATED = k_ERemoteStorageLocalFileChange_FileUpdated,
		LOCAL_FILE_CHANGE_FILE_DELETED = k_ERemoteStorageLocalFileChange_FileDeleted
	};
	enum RemoteStoragePlatform {
		REMOTE_STORAGE_PLATFORM_NONE = k_ERemoteStoragePlatformNone,
		REMOTE_STORAGE_PLATFORM_WINDOWS = k_ERemoteStoragePlatformWindows,
		REMOTE_STORAGE_PLATFORM_OSX = k_ERemoteStoragePlatformOSX,
		REMOTE_STORAGE_PLATFORM_PS3 = k_ERemoteStoragePlatformPS3,
		REMOTE_STORAGE_PLATFORM_LINUX = k_ERemoteStoragePlatformLinux,
		REMOTE_STORAGE_PLATFORM_SWITCH = k_ERemoteStoragePlatformSwitch,
		REMOTE_STORAGE_PLATFORM_ANDROID = k_ERemoteStoragePlatformAndroid,
		REMOTE_STORAGE_PLATFORM_IOS = k_ERemoteStoragePlatformIOS,
		REMOTE_STORAGE_PLATFORM_ALL = k_ERemoteStoragePlatformAll
	};
	enum RemoteStoragePublishedFileVisibility {
		REMOTE_STORAGE_PUBLISHED_VISIBILITY_PUBLIC = k_ERemoteStoragePublishedFileVisibilityPublic,
		REMOTE_STORAGE_PUBLISHED_VISIBILITY_FRIENDS_ONLY = k_ERemoteStoragePublishedFileVisibilityFriendsOnly,
		REMOTE_STORAGE_PUBLISHED_VISIBILITY_PRIVATE = k_ERemoteStoragePublishedFileVisibilityPrivate,
		REMOTE_STORAGE_PUBLISHED_VISIBILITY_UNLISTED = k_ERemoteStoragePublishedFileVisibilityUnlisted
	};
	enum UGCReadAction {
		UGC_READ_CONTINUE_READING_UNTIL_FINISHED = k_EUGCRead_ContinueReadingUntilFinished,
		UGC_READ_CONTINUE_READING = k_EUGCRead_ContinueReading,
		UGC_READ_CLOSE = k_EUGCRead_Close
	};
	enum WorkshopEnumerationType {
		WORKSHOP_ENUMERATION_TYPE_RANKED_BY_VOTE = k_EWorkshopEnumerationTypeRankedByVote,
		WORKSHOP_ENUMERATION_TYPE_RECENT = k_EWorkshopEnumerationTypeRecent,
		WORKSHOP_ENUMERATION_TYPE_TRENDING = k_EWorkshopEnumerationTypeTrending,
		WORKSHOP_ENUMERATION_TYPE_FAVORITES_OF_FRIENDS = k_EWorkshopEnumerationTypeFavoritesOfFriends,
		WORKSHOP_ENUMERATION_TYPE_VOTED_BY_FRIENDS = k_EWorkshopEnumerationTypeVotedByFriends,
		WORKSHOP_ENUMERATION_TYPE_CONTENT_BY_FRIENDS = k_EWorkshopEnumerationTypeContentByFriends,
		WORKSHOP_ENUMERATION_TYPE_RECENT_FROM_FOLLOWED_USERS = k_EWorkshopEnumerationTypeRecentFromFollowedUsers
	};
	enum WorkshopFileAction {
		WORKSHOP_FILE_ACTION_PLAYED = k_EWorkshopFileActionPlayed,
		WORKSHOP_FILE_ACTION_COMPLETED = k_EWorkshopFileActionCompleted
	};
	enum WorkshopFileType {
		WORKSHOP_FILE_TYPE_FIRST = k_EWorkshopFileTypeFirst,
		WORKSHOP_FILE_TYPE_COMMUNITY = k_EWorkshopFileTypeCommunity,
		WORKSHOP_FILE_TYPE_MICROTRANSACTION = k_EWorkshopFileTypeMicrotransaction,
		WORKSHOP_FILE_TYPE_COLLECTION = k_EWorkshopFileTypeCollection,
		WORKSHOP_FILE_TYPE_ART = k_EWorkshopFileTypeArt,
		wORKSHOP_FILE_TYPE_VIDEO = k_EWorkshopFileTypeVideo,
		WORKSHOP_FILE_TYPE_SCREENSHOT = k_EWorkshopFileTypeScreenshot,
		WORKSHOP_FILE_TYPE_GAME = k_EWorkshopFileTypeGame,
		WORKSHOP_FILE_TYPE_SOFTWARE = k_EWorkshopFileTypeSoftware,
		WORKSHOP_FILE_TYPE_CONCEPT = k_EWorkshopFileTypeConcept,
		WORKSHOP_FILE_TYPE_WEB_GUIDE = k_EWorkshopFileTypeWebGuide,
		WORKSHOP_FILE_TYPE_INTEGRATED_GUIDE = k_EWorkshopFileTypeIntegratedGuide,
		WORKSHOP_FILE_TYPE_MERCH = k_EWorkshopFileTypeMerch,
		WORKSHOP_FILE_TYPE_CONTROLLER_BINDING = k_EWorkshopFileTypeControllerBinding,
		wORKSHOP_FILE_TYPE_STEAMWORKS_ACCESS_INVITE = k_EWorkshopFileTypeSteamworksAccessInvite,
		WORKSHOP_FILE_TYPE_STEAM_VIDEO = k_EWorkshopFileTypeSteamVideo,
		WORKSHOP_FILE_TYPE_GAME_MANAGED_ITEM = k_EWorkshopFileTypeGameManagedItem,
		WORKSHOP_FILE_TYPE_CLIP = k_EWorkshopFileTypeClip,
		WORKSHOP_FILE_TYPE_MAX = k_EWorkshopFileTypeMax
	};
	enum WorkshopVideoProvider {
		WORKSHOP_VIDEO_PROVIDER_NONE = k_EWorkshopVideoProviderNone,
		WORKSHOP_VIDEO_PROVIDER_YOUTUBE = k_EWorkshopVideoProviderYoutube
	};
	enum WorkshopVote {
		WORKSHOP_VOTE_UNVOTED = k_EWorkshopVoteUnvoted,
		WORKSHOP_VOTE_FOR = k_EWorkshopVoteFor,
		WORKSHOP_VOTE_AGAINST = k_EWorkshopVoteAgainst,
		WORKSHOP_VOTE_LATER = k_EWorkshopVoteLater
	};

	// Screenshot enums
	enum VRScreenshotType {
		VR_SCREENSHOT_TYPE_NONE = k_EVRScreenshotType_None,
		VR_SCREENSHOT_TYPE_MONO = k_EVRScreenshotType_Mono,
		VR_SCREENSHOT_TYPE_STEREO = k_EVRScreenshotType_Stereo,
		VR_SCREENSHOT_TYPE_MONO_CUBE_MAP = k_EVRScreenshotType_MonoCubemap,
		VR_SCREENSHOT_TYPE_MONO_PANORAMA = k_EVRScreenshotType_MonoPanorama,
		VR_SCREENSHOT_TYPE_STEREO_PANORAMA = k_EVRScreenshotType_StereoPanorama
	};

	// UGC enums
	enum ItemPreviewType {
		ITEM_PREVIEW_TYPE_IMAGE = k_EItemPreviewType_Image,
		ITEM_PREVIEW_TYPE_YOUTUBE_VIDEO = k_EItemPreviewType_YouTubeVideo,
		ITEM_PREVIEW_TYPE_SKETCHFAB = k_EItemPreviewType_Sketchfab,
		ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_HORIZONTAL_CROSS = k_EItemPreviewType_EnvironmentMap_HorizontalCross,
		ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_LAT_LONG = k_EItemPreviewType_EnvironmentMap_LatLong,
		ITEM_PREVIEW_TYPE_CLIP = k_EItemPreviewType_Clip,
		ITEM_PREVIEW_TYPE_RESERVED_MAX = k_EItemPreviewType_ReservedMax
	};
	enum ItemState {
		ITEM_STATE_NONE = k_EItemStateNone,
		ITEM_STATE_SUBSCRIBED = k_EItemStateSubscribed,
		ITEM_STATE_LEGACY_ITEM = k_EItemStateLegacyItem,
		ITEM_STATE_INSTALLED = k_EItemStateInstalled,
		ITEM_STATE_NEEDS_UPDATE = k_EItemStateNeedsUpdate,
		ITEM_STATE_DOWNLOADING = k_EItemStateDownloading,
		ITEM_STATE_DOWNLOAD_PENDING = k_EItemStateDownloadPending,
		ITEM_STATE_DISABLED_LOCALLY = k_EItemStateDisabledLocally
	};
	enum ItemStatistic {
		ITEM_STATISTIC_NUM_SUBSCRIPTIONS = k_EItemStatistic_NumSubscriptions,
		ITEM_STATISTIC_NUM_FAVORITES = k_EItemStatistic_NumFavorites,
		ITEM_STATISTIC_NUM_FOLLOWERS = k_EItemStatistic_NumFollowers,
		ITEM_STATISTIC_NUM_UNIQUE_SUBSCRIPTIONS = k_EItemStatistic_NumUniqueSubscriptions,
		ITEM_STATISTIC_NUM_UNIQUE_FAVORITES = k_EItemStatistic_NumUniqueFavorites,
		ITEM_STATISTIC_NUM_UNIQUE_FOLLOWERS = k_EItemStatistic_NumUniqueFollowers,
		ITEM_STATISTIC_NUM_UNIQUE_WEBSITE_VIEWS = k_EItemStatistic_NumUniqueWebsiteViews,
		ITEM_STATISTIC_REPORT_SCORE = k_EItemStatistic_ReportScore,
		ITEM_STATISTIC_NUM_SECONDS_PLAYED = k_EItemStatistic_NumSecondsPlayed,
		ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS = k_EItemStatistic_NumPlaytimeSessions,
		ITEM_STATISTIC_NUM_COMMENTS = k_EItemStatistic_NumComments,
		ITEM_STATISTIC_NUM_SECONDS_PLAYED_DURING_TIME_PERIOD = k_EItemStatistic_NumSecondsPlayedDuringTimePeriod,
		ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS_DURING_TIME_PERIOD = k_EItemStatistic_NumPlaytimeSessionsDuringTimePeriod
	};
	enum ItemUpdateStatus {
		ITEM_UPDATE_STATUS_INVALID = k_EItemUpdateStatusInvalid,
		ITEM_UPDATE_STATUS_PREPARING_CONFIG = k_EItemUpdateStatusPreparingConfig,
		ITEM_UPDATE_STATUS_PREPARING_CONTENT = k_EItemUpdateStatusPreparingContent,
		ITEM_UPDATE_STATUS_UPLOADING_CONTENT = k_EItemUpdateStatusUploadingContent,
		ITEM_UPDATE_STATUS_UPLOADING_PREVIEW_FILE = k_EItemUpdateStatusUploadingPreviewFile,
		ITEM_UPDATE_STATUS_COMMITTING_CHANGES = k_EItemUpdateStatusCommittingChanges
	};
	enum UGCContentDescriptorID {
		UGCCONTENTDESCRIPTOR_NUDITY_OR_SEXUAL_CONTENT = k_EUGCContentDescriptor_NudityOrSexualContent,
		UGCCONTENTDESCRIPTOR_FREQUENT_VIOLENCE_OR_GORE = k_EUGCContentDescriptor_FrequentViolenceOrGore,
		UGCCONTENTDESCRIPTOR_ADULT_ONLY_SEXUAL_CONTENT = k_EUGCContentDescriptor_AdultOnlySexualContent,
		UGCCONTENTDESCRIPTOR_GRATUITOUS_SEXUAL_CONTENT = k_EUGCContentDescriptor_GratuitousSexualContent,
		UGCCONTENTDESCRIPTOR_ANY_MATURE_CONTENT = k_EUGCContentDescriptor_AnyMatureContent
	};
	enum UGCMatchingUGCType {
		UGC_MATCHINGUGCTYPE_ITEMS = k_EUGCMatchingUGCType_Items,
		UGC_MATCHING_UGC_TYPE_ITEMS_MTX = k_EUGCMatchingUGCType_Items_Mtx,
		UGC_MATCHING_UGC_TYPE_ITEMS_READY_TO_USE = k_EUGCMatchingUGCType_Items_ReadyToUse,
		UGC_MATCHING_UGC_TYPE_COLLECTIONS = k_EUGCMatchingUGCType_Collections,
		UGC_MATCHING_UGC_TYPE_ARTWORK = k_EUGCMatchingUGCType_Artwork,
		UGC_MATCHING_UGC_TYPE_VIDEOS = k_EUGCMatchingUGCType_Videos,
		UGC_MATCHING_UGC_TYPE_SCREENSHOTS = k_EUGCMatchingUGCType_Screenshots,
		UGC_MATCHING_UGC_TYPE_ALL_GUIDES = k_EUGCMatchingUGCType_AllGuides,
		UGC_MATCHING_UGC_TYPE_WEB_GUIDES = k_EUGCMatchingUGCType_WebGuides,
		UGC_MATCHING_UGC_TYPE_INTEGRATED_GUIDES = k_EUGCMatchingUGCType_IntegratedGuides,
		UGC_MATCHING_UGC_TYPE_USABLE_IN_GAME = k_EUGCMatchingUGCType_UsableInGame,
		UGC_MATCHING_UGC_TYPE_CONTROLLER_BINDINGS = k_EUGCMatchingUGCType_ControllerBindings,
		UGC_MATCHING_UGC_TYPE_GAME_MANAGED_ITEMS = k_EUGCMatchingUGCType_GameManagedItems,
		UGC_MATCHING_UGC_TYPE_ALL = k_EUGCMatchingUGCType_All
	};
	enum UGCQuery {
		UGC_QUERY_RANKED_BY_VOTE = k_EUGCQuery_RankedByVote,
		UGC_QUERY_RANKED_BY_PUBLICATION_DATE = k_EUGCQuery_RankedByPublicationDate,
		UGC_QUERY_ACCEPTED_FOR_GAME_RANKED_BY_ACCEPTANCE_DATE = k_EUGCQuery_AcceptedForGameRankedByAcceptanceDate,
		UGC_QUERY_RANKED_BY_TREND = k_EUGCQuery_RankedByTrend,
		UGC_QUERY_FAVORITED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE = k_EUGCQuery_FavoritedByFriendsRankedByPublicationDate,
		UGC_QUERY_CREATED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE = k_EUGCQuery_CreatedByFriendsRankedByPublicationDate,
		UGC_QUERY_RANKED_BY_NUM_TIMES_REPORTED = k_EUGCQuery_RankedByNumTimesReported,
		UGC_QUERY_CREATED_BY_FOLLOWED_USERS_RANKED_BY_PUBLICATION_DATE = k_EUGCQuery_CreatedByFollowedUsersRankedByPublicationDate,
		UGC_QUERY_NOT_YET_RATED = k_EUGCQuery_NotYetRated,
		UGC_QUERY_RANKED_BY_TOTAL_VOTES_ASC = k_EUGCQuery_RankedByTotalVotesAsc,
		UGC_QUERY_RANKED_BY_VOTES_UP = k_EUGCQuery_RankedByVotesUp,
		UGC_QUERY_RANKED_BY_TEXT_SEARCH = k_EUGCQuery_RankedByTextSearch,
		UGC_QUERY_RANKED_BY_TOTAL_UNIQUE_SUBSCRIPTIONS = k_EUGCQuery_RankedByTotalUniqueSubscriptions,
		UGC_QUERY_RANKED_BY_PLAYTIME_TREND = k_EUGCQuery_RankedByPlaytimeTrend,
		UGC_QUERY_RANKED_BY_TOTAL_PLAYTIME = k_EUGCQuery_RankedByTotalPlaytime,
		UGC_QUERY_RANKED_BY_AVERAGE_PLAYTIME_TREND = k_EUGCQuery_RankedByAveragePlaytimeTrend,
		UGC_QUERY_RANKED_BY_LIFETIME_AVERAGE_PLAYTIME = k_EUGCQuery_RankedByLifetimeAveragePlaytime,
		UGC_QUERY_RANKED_BY_PLAYTIME_SESSIONS_TREND = k_EUGCQuery_RankedByPlaytimeSessionsTrend,
		UGC_QUERY_RANKED_BY_LIFETIME_PLAYTIME_SESSIONS = k_EUGCQuery_RankedByLifetimePlaytimeSessions,
		UGC_QUERY_RANKED_BY_LAST_UPDATED_DATE = k_EUGCQuery_RankedByLastUpdatedDate
	};
	enum UserUGCList {
		USER_UGC_LIST_PUBLISHED = k_EUserUGCList_Published,
		USER_UGC_LIST_VOTED_ON = k_EUserUGCList_VotedOn,
		USER_UGC_LIST_VOTED_UP = k_EUserUGCList_VotedUp,
		USER_UGC_LIST_VOTED_DOWN = k_EUserUGCList_VotedDown,
		USER_UGC_LIST_WILL_VOTE_LATER = k_EUserUGCList_WillVoteLater,
		USER_UGC_LIST_FAVORITED = k_EUserUGCList_Favorited,
		USER_UGC_LIST_SUBSCRIBED = k_EUserUGCList_Subscribed,
		USER_UGC_LIST_USED_OR_PLAYED = k_EUserUGCList_UsedOrPlayed,
		USER_UGC_LIST_FOLLOWED = k_EUserUGCList_Followed
	};
	enum UserUGCListSortOrder {
		USERUGCLISTSORTORDER_CREATIONORDERDESC = k_EUserUGCListSortOrder_CreationOrderDesc,
		USERUGCLISTSORTORDER_CREATIONORDERASC = k_EUserUGCListSortOrder_CreationOrderAsc,
		USERUGCLISTSORTORDER_TITLEASC = k_EUserUGCListSortOrder_TitleAsc,
		USERUGCLISTSORTORDER_LASTUPDATEDDESC = k_EUserUGCListSortOrder_LastUpdatedDesc,
		USERUGCLISTSORTORDER_SUBSCRIPTIONDATEDESC = k_EUserUGCListSortOrder_SubscriptionDateDesc,
		USERUGCLISTSORTORDER_VOTESCOREDESC = k_EUserUGCListSortOrder_VoteScoreDesc,
		SERUGCLISTSORTORDER_FORMODERATION = k_EUserUGCListSortOrder_ForModeration
	};

	// User enums
	enum DurationControlNotification {
		// Found in steamclientpublic.h
		DURATION_CONTROL_NOTIFICATION_NONE = k_EDurationControlNotification_None,
		DURATION_CONTROL_NOTIFICATION_1_HOUR = k_EDurationControlNotification_1Hour,
		DURATION_CONTROL_NOTIFICATION_3_HOURS = k_EDurationControlNotification_3Hours,
		DURATION_CONTROL_NOTIFICATION_HALF_PROGRESS = k_EDurationControlNotification_HalfProgress,
		DURATION_CONTROL_NOTIFICATION_NO_PROGRESS = k_EDurationControlNotification_NoProgress,
		DURATION_CONTROL_NOTIFICATION_EXIT_SOON_3H = k_EDurationControlNotification_ExitSoon_3h,
		DURATION_CONTROL_NOTIFICATION_EXIT_SOON_5H = k_EDurationControlNotification_ExitSoon_5h,
		DURATION_CONTROL_NOTIFICATION_EXIT_SOON_NIGHT = k_EDurationControlNotification_ExitSoon_Night
	};
	enum DurationControlOnlineState {
		// Found in steamclientpublic.h
		DURATION_CONTROL_ONLINE_STATE_INVALID = k_EDurationControlOnlineState_Invalid,
		DURATION_CONTROL_ONLINE_STATE_OFFLINE = k_EDurationControlOnlineState_Offline,
		DURATION_CONTROL_ONLINE_STATE_ONLINE = k_EDurationControlOnlineState_Online,
		DURATION_CONTROL_ONLINE_STATE_ONLINE_HIGH_PRIORITY = k_EDurationControlOnlineState_OnlineHighPri
	};
	enum DurationControlProgress {
		// Found in steamclientpublic.h
		DURATION_CONTROL_PROGRESS_FULL = k_EDurationControlProgress_Full,
		DURATION_CONTROL_PROGRESS_HALF = k_EDurationControlProgress_Half,
		DURATION_CONTROL_PROGRESS_NONE = k_EDurationControlProgress_None,
		DURATION_CONTROL_EXIT_SOON_3H = k_EDurationControl_ExitSoon_3h,
		DURATION_CONTROL_EXIT_SOON_5H = k_EDurationControl_ExitSoon_5h,
		DURATION_CONTROL_EXIT_SOON_NIGHT = k_EDurationControl_ExitSoon_Night
	};

	// User Stats enums
	enum LeaderboardDataRequest {
		LEADERBOARD_DATA_REQUEST_GLOBAL = k_ELeaderboardDataRequestGlobal,
		LEADERBOARD_DATA_REQUEST_GLOBAL_AROUND_USER = k_ELeaderboardDataRequestGlobalAroundUser,
		LEADERBOARD_DATA_REQUEST_FRIENDS = k_ELeaderboardDataRequestFriends,
		LEADERBOARD_DATA_REQUEST_USERS = k_ELeaderboardDataRequestUsers
	};
	enum LeaderboardDisplayType {
		LEADERBOARD_DISPLAY_TYPE_NONE = k_ELeaderboardDisplayTypeNone,
		LEADERBOARD_DISPLAY_TYPE_NUMERIC = k_ELeaderboardDisplayTypeNumeric,
		LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS = k_ELeaderboardDisplayTypeTimeSeconds,
		LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS = k_ELeaderboardDisplayTypeTimeMilliSeconds
	};
	enum LeaderboardSortMethod {
		LEADERBOARD_SORT_METHOD_NONE = k_ELeaderboardSortMethodNone,
		LEADERBOARD_SORT_METHOD_ASCENDING = k_ELeaderboardSortMethodAscending,
		LEADERBOARD_SORT_METHOD_DESCENDING = k_ELeaderboardSortMethodDescending
	};
	enum LeaderboardUploadScoreMethod {
		LEADERBOARD_UPLOAD_SCORE_METHOD_NONE = k_ELeaderboardUploadScoreMethodNone,
		LEADERBOARD_UPLOAD_SCORE_METHOD_KEEP_BEST = k_ELeaderboardUploadScoreMethodKeepBest,
		LEADERBOARD_UPLOAD_SCORE_METHOD_FORCE_UPDATE = k_ELeaderboardUploadScoreMethodForceUpdate
	};

	// Utils enums
	enum CheckFileSignature {
		CHECK_FILE_SIGNATURE_INVALID_SIGNATURE = k_ECheckFileSignatureInvalidSignature,
		CHECK_FILE_SIGNATURE_VALID_SIGNATURE = k_ECheckFileSignatureValidSignature,
		CHECK_FILE_SIGNATURE_FILE_NOT_FOUND = k_ECheckFileSignatureFileNotFound,
		CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_APP = k_ECheckFileSignatureNoSignaturesFoundForThisApp,
		CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_FILE = k_ECheckFileSignatureNoSignaturesFoundForThisFile
	};
	enum GamepadTextInputLineMode {
		GAMEPAD_TEXT_INPUT_LINE_MODE_SINGLE_LINE = k_EGamepadTextInputLineModeSingleLine,
		GAMEPAD_TEXT_INPUT_LINE_MODE_MULTIPLE_LINES = k_EGamepadTextInputLineModeMultipleLines
	};
	enum GamepadTextInputMode {
		GAMEPAD_TEXT_INPUT_MODE_NORMAL = k_EGamepadTextInputModeNormal,
		GAMEPAD_TEXT_INPUT_MODE_PASSWORD = k_EGamepadTextInputModePassword
	};
	enum FloatingGamepadTextInputMode {
		FLOATING_GAMEPAD_TEXT_INPUT_MODE_SINGLE_LINE = k_EFloatingGamepadTextInputModeModeSingleLine,
		FLOATING_GAMEPAD_TEXT_INPUT_MODE_MULTIPLE_LINES = k_EFloatingGamepadTextInputModeModeMultipleLines,
		FLOATING_GAMEPAD_TEXT_INPUT_MODE_EMAIL = k_EFloatingGamepadTextInputModeModeEmail,
		FLOATING_GAMEPAD_TEXT_INPUT_MODE_NUMERIC = k_EFloatingGamepadTextInputModeModeNumeric
	};
	enum APICallFailure {
		STEAM_API_CALL_FAILURE_NONE = k_ESteamAPICallFailureNone,
		STEAM_API_CALL_FAILURE_STEAM_GONE = k_ESteamAPICallFailureSteamGone,
		STEAM_API_CALL_FAILURE_NETWORK_FAILURE = k_ESteamAPICallFailureNetworkFailure,
		STEAM_API_CALL_FAILURE_INVALID_HANDLE = k_ESteamAPICallFailureInvalidHandle,
		STEAM_API_CALL_FAILURE_MISMATCHED_CALLBACK = k_ESteamAPICallFailureMismatchedCallback
	};
	enum TextFilteringContext {
		TEXT_FILTERING_CONTEXT_UNKNOWN = k_ETextFilteringContextUnknown,
		TEXT_FILTERING_CONTEXT_GAME_CONTENT = k_ETextFilteringContextGameContent,
		TEXT_FILTERING_CONTEXT_CHAT = k_ETextFilteringContextChat,
		TEXT_FILTERING_CONTEXT_NAME = k_ETextFilteringContextName
	};



	// STEAMWORKS FUNCTIONS
	/////////////////////////////////////////
	//
	CSteamID createSteamID(uint64_t steam_id, AccountType account_type = AccountType(-1));

	// Main /////////////////////////////////
	uint32_t getSteamID32(uint64_t steam_id);
	bool isAnonAccount(uint64_t steam_id);
	bool isAnonUserAccount(uint64_t steam_id);
	bool isChatAccount(uint64_t steam_id);
	bool isClanAccount(uint64_t steam_id);
	bool isConsoleUserAccount(uint64_t steam_id);
	bool isIndividualAccount(uint64_t steam_id);
	bool isLobby(uint64_t steam_id);
	bool isSteamRunning();
	bool restartAppIfNecessary(uint32 app_id);
	void steamworksError(const String &failed_signal);
	Dictionary steamInit(bool retrieve_stats = true, uint32_t app_id = 0);
	Dictionary steamInitEx(bool retrieve_stats = true, uint32_t app_id = 0);
	void steamShutdown();

	// Apps /////////////////////////////////
	Array getDLCDataByIndex();
	bool isAppInstalled(uint32_t app_id);
	bool isCybercafe();
	bool isDLCInstalled(uint32_t dlc_id);
	bool isLowViolence();
	bool isSubscribed();
	bool isSubscribedApp(uint32_t app_id);
	bool isSubscribedFromFamilySharing();
	bool isSubscribedFromFreeWeekend();
	Dictionary isTimedTrial();
	bool isVACBanned();
	int getAppBuildId();
	Dictionary getAppInstallDir(uint32_t app_id);
	uint64_t getAppOwner();
	String getAvailableGameLanguages();
	String getCurrentBetaName();
	String getCurrentGameLanguage();
	int32 getDLCCount();
	Dictionary getDLCDownloadProgress(uint32_t dlc_id);
	uint32_t getEarliestPurchaseUnixTime(uint32_t app_id);
	void getFileDetails(const String &filename);
	Array getInstalledDepots(uint32_t app_id);
	String getLaunchCommandLine();
	String getLaunchQueryParam(const String &key);
	void installDLC(uint32_t dlc_id);
	bool markContentCorrupt(bool missing_files_only);
	bool setDLCContext(uint32_t app_id);
	void uninstallDLC(uint32_t dlc_id);

	// Friends //////////////////////////////
	void activateGameOverlay(const String &type);
	void activateGameOverlayInviteDialog(uint64_t steam_id);
	void activateGameOverlayInviteDialogConnectString(const String &connect_string);
	void activateGameOverlayToStore(uint32_t app_id = 0);
	void activateGameOverlayToUser(const String &type, uint64_t steam_id);
	void activateGameOverlayToWebPage(const String &url);
	void clearRichPresence();
	bool closeClanChatWindowInSteam(uint64_t chat_id);
	void downloadClanActivityCounts(uint64_t clan_id, int clans_to_request);
	void enumerateFollowingList(uint32 start_index);
	uint64_t getChatMemberByIndex(uint64_t clan_id, int user);
	Dictionary getClanActivityCounts(uint64_t clan_id);
	uint64_t getClanByIndex(int clan);
	int getClanChatMemberCount(uint64_t clan_id);
	Dictionary getClanChatMessage(uint64_t chat_id, int message);
	int getClanCount();
	String getClanName(uint64_t clan_id);
	uint64_t getClanOfficerByIndex(uint64_t clan_id, int officer);
	int getClanOfficerCount(uint64_t clan_id);
	uint64_t getClanOwner(uint64_t clan_id);
	String getClanTag(uint64_t clan_id);
	uint64_t getCoplayFriend(int friend_number);
	int getCoplayFriendCount();
	void getFollowerCount(uint64_t steam_id);
	uint64_t getFriendByIndex(int friend_number, BitField<FriendFlags> friend_flags);
	uint32 getFriendCoplayGame(uint64_t friend_id);
	int getFriendCoplayTime(uint64_t friend_id);
	int getFriendCount(int friend_flags = 0x04);
	int getFriendCountFromSource(uint64_t source_id);
	uint64_t getFriendFromSourceByIndex(uint64_t source_id, int friend_number);
	Dictionary getFriendGamePlayed(uint64_t steam_id);
	Dictionary getFriendMessage(uint64_t friend_id, int message);
	String getFriendPersonaName(uint64_t steam_id);
	String getFriendPersonaNameHistory(uint64_t steam_id, int name_history);
	PersonaState getFriendPersonaState(uint64_t steam_id);
	FriendRelationship getFriendRelationship(uint64_t steam_id);
	String getFriendRichPresence(uint64_t friend_id, const String &key);
	int getFriendRichPresenceKeyCount(uint64_t friend_id);
	String getFriendRichPresenceKeyByIndex(uint64_t friend_id, int key);		
	int getFriendsGroupCount();
	int16 getFriendsGroupIDByIndex(int friend_group);
	int getFriendsGroupMembersCount(int friend_group);
	Array getFriendsGroupMembersList(int friend_group, int member_count);
	String getFriendsGroupName(int friend_group);
	int getFriendSteamLevel(uint64_t steam_id);
	int getLargeFriendAvatar(uint64_t steam_id);
	int getMediumFriendAvatar(uint64_t steam_id);
	String getPersonaName();
	PersonaState getPersonaState();
	void getPlayerAvatar(int size = 2, uint64_t steam_id = 0);
	String getPlayerNickname(uint64_t steam_id);
	String getProfileItemPropertyString(uint64_t steam_id, CommunityProfileItemType item_type, CommunityProfileItemProperty item_property);
	uint32 getProfileItemPropertyInt(uint64_t steam_id, CommunityProfileItemType item_type, CommunityProfileItemProperty item_property);
	Array getRecentPlayers();
	int getSmallFriendAvatar(uint64_t steam_id);
	Array getUserFriendsGroups();
	uint32 getUserRestrictions();
	Array getUserSteamFriends();
	Array getUserSteamGroups();
	bool hasEquippedProfileItem(uint64_t steam_id, CommunityProfileItemType item_type);
	bool hasFriend(uint64_t steam_id, BitField<FriendFlags> friend_flags);
	bool inviteUserToGame(uint64_t friend_id, const String &connect_string);
	bool isClanChatAdmin(uint64_t chat_id, uint64_t steam_id);
	bool isClanPublic(uint64_t clan_id);
	bool isClanOfficialGameGroup(uint64_t clan_id);
	bool isClanChatWindowOpenInSteam(uint64_t chat_id);
	void isFollowing(uint64_t steam_id);
	bool isUserInSource(uint64_t steam_id, uint64_t source_id);
	void joinClanChatRoom(uint64_t clan_id);
	bool leaveClanChatRoom(uint64_t clan_id);
	bool openClanChatWindowInSteam(uint64_t chat_id);
	bool registerProtocolInOverlayBrowser(const String &protocol);
	bool replyToFriendMessage(uint64_t steam_id, const String &message);
	void requestClanOfficerList(uint64_t clan_id);
	void requestEquippedProfileItems(uint64_t steam_id);
	void requestFriendRichPresence(uint64_t friend_id);
	bool requestUserInformation(uint64_t steam_id, bool require_name_only);
	bool sendClanChatMessage(uint64_t chat_id, const String &text);
	void setInGameVoiceSpeaking(uint64_t steam_id, bool speaking);
	bool setListenForFriendsMessages(bool intercept);
	void setPersonaName(const String &name);
	void setPlayedWith(uint64_t steam_id);
	bool setRichPresence(const String &key, const String &value);

	// Game Search //////////////////////////
	int addGameSearchParams(const String &key, const String &values);
	int searchForGameWithLobby(uint64_t lobby_id, int player_min, int player_max);
	int searchForGameSolo(int player_min, int player_max);
	int acceptGame();
	int declineGame();
	String retrieveConnectionDetails(uint64_t host_id);
	int endGameSearch();
	int setGameHostParams(const String &key, const String &value);
	int setConnectionDetails(const String &details, int connection_details);
	int requestPlayersForGame(int player_min, int player_max, int max_team_size);
	int hostConfirmGameStart(uint64_t game_id);
	int cancelRequestPlayersForGame();
	int submitPlayerResult(uint64_t game_id, uint64_t player_id, PlayerResult player_result);
	int endGame(uint64_t game_id);

	// HTML Surface /////////////////////////
	void addHeader(const String &key, const String &value, uint32 this_handle = 0);
	void allowStartRequest(bool allowed, uint32 this_handle = 0);
	void copyToClipboard(uint32 this_handle = 0);
	void createBrowser(const String &user_agent = "", const String &user_css = "");
	void executeJavascript(const String &javascript, uint32 this_handle = 0);
	void find(const String &search, bool currently_in_find, bool reverse, uint32 this_handle = 0);
	void getLinkAtPosition(int x, int y, uint32 this_handle = 0);
	void goBack(uint32 this_handle = 0);
	void goForward(uint32 this_handle = 0);
	void htmlInit();
	void jsDialogResponse(bool result, uint32 this_handle = 0);
	void keyChar(uint32 unicode_char, BitField<HTMLKeyModifiers> key_modifiers, uint32 this_handle = 0);
	void keyDown(uint32 native_key_code, BitField<HTMLKeyModifiers> key_modifiers, uint32 this_handle = 0);
	void keyUp(uint32 native_key_code, BitField<HTMLKeyModifiers> key_modifiers, uint32 this_handle = 0);
	void loadURL(const String &url, const String &post_data, uint32 this_handle = 0);
	void mouseDoubleClick(HTMLMouseButton mouse_button, uint32 this_handle = 0);
	void mouseDown(HTMLMouseButton mouse_button, uint32 this_handle = 0);
	void mouseMove(int x, int y, uint32 this_handle = 0);
	void mouseUp(HTMLMouseButton mouse_button, uint32 this_handle = 0);
	void mouseWheel(int32 delta, uint32 this_handle = 0);
	void pasteFromClipboard(uint32 this_handle = 0);
	void reload(uint32 this_handle = 0);
	void removeBrowser(uint32 this_handle = 0);
	void setBackgroundMode(bool background_mode, uint32 this_handle = 0);
	void setCookie(const String &hostname, const String &key, const String &value, const String &path, uint32 expires, bool secure, bool http_only);
	void setHorizontalScroll(uint32 absolute_pixel_scroll, uint32 this_handle = 0);
	void setKeyFocus(bool has_key_focus, uint32 this_handle = 0);
	void setPageScaleFactor(float zoom, int point_x, int point_y, uint32 this_handle = 0);
	void setSize(uint32 width, uint32 height, uint32 this_handle = 0);
	void setVerticalScroll(uint32 absolute_pixel_scroll, uint32 this_handle = 0);
	bool htmlShutdown();
	void stopFind(uint32 this_handle = 0);
	void stopLoad(uint32 this_handle = 0);
	void viewSource(uint32 this_handle = 0);

	// HTTP /////////////////////////////////
	uint32_t createCookieContainer(bool allow_responses_to_modify);
	uint32_t createHTTPRequest(HTTPMethod request_method, const String &absolute_url);
	bool deferHTTPRequest(uint32 request_handle);
	float getHTTPDownloadProgressPct(uint32 request_handle);
	bool getHTTPRequestWasTimedOut(uint32 request_handle);
	PackedByteArray getHTTPResponseBodyData(uint32 request_handle, uint32 buffer_size);
	uint32 getHTTPResponseBodySize(uint32 request_handle);
	uint32 getHTTPResponseHeaderSize(uint32 request_handle, const String &header_name);
	uint8 getHTTPResponseHeaderValue(uint32 request_handle, const String &header_name, uint32 buffer_size);
	uint8 getHTTPStreamingResponseBodyData(uint32 request_handle, uint32 offset, uint32 buffer_size);
	bool prioritizeHTTPRequest(uint32 request_handle);
	bool releaseCookieContainer(uint32 cookie_handle);
	bool releaseHTTPRequest(uint32 request_handle);
	bool sendHTTPRequest(uint32 request_handle);
	bool sendHTTPRequestAndStreamResponse(uint32 request_handle);
	bool setHTTPCookie(uint32 cookie_handle, const String &host, const String &url, const String &cookie);
	bool setHTTPRequestAbsoluteTimeoutMS(uint32 request_handle, uint32 milliseconds);
	bool setHTTPRequestContextValue(uint32 request_handle, uint64_t context_value);
	bool setHTTPRequestCookieContainer(uint32 request_handle, uint32 cookie_handle);
	bool setHTTPRequestGetOrPostParameter(uint32 request_handle, const String &name, const String &value);
	bool setHTTPRequestHeaderValue(uint32 request_handle, const String &header_name, const String &header_value);
	bool setHTTPRequestNetworkActivityTimeout(uint32 request_handle, uint32 timeout_seconds);
	uint8 setHTTPRequestRawPostBody(uint32 request_handle, const String &content_type, uint32 body_length);
	bool setHTTPRequestRequiresVerifiedCertificate(uint32 request_handle, bool require_verified_certificate);
	bool setHTTPRequestUserAgentInfo(uint32 request_handle, const String &user_agent_info);

	// Input ////////////////////////////////
	void activateActionSet(uint64_t input_handle, uint64_t action_set_handle);
	void activateActionSetLayer(uint64_t input_handle, uint64_t action_set_layer_handle);
	void deactivateActionSetLayer(uint64_t input_handle, uint64_t action_set_handle);
	void deactivateAllActionSetLayers(uint64_t input_handle);
	void enableDeviceCallbacks();
//		void enableActionEventCallbacks();
	uint64_t getActionSetHandle(const String &action_set_name);
	InputActionOrigin getActionOriginFromXboxOrigin(uint64_t input_handle, int origin);
	Array getActiveActionSetLayers(uint64_t input_handle);
	Dictionary getAnalogActionData(uint64_t input_handle, uint64_t analog_action_handle);
	uint64_t getAnalogActionHandle(const String &action_name);
	Array getAnalogActionOrigins(uint64_t input_handle, uint64_t action_set_handle, uint64_t analog_action_handle);
	Array getConnectedControllers();
	uint64_t getControllerForGamepadIndex(int index);
	uint64_t getCurrentActionSet(uint64_t input_handle);
	Array getDeviceBindingRevision(uint64_t input_handle);
	Dictionary getDigitalActionData(uint64_t input_handle, uint64_t digital_action_handle);
	uint64_t getDigitalActionHandle(const String &action_name);
	Array getDigitalActionOrigins(uint64_t input_handle, uint64_t action_set_handle, uint64_t digital_action_handle);
	int getGamepadIndexForController(uint64_t input_handle);
	String getGlyphForActionOrigin(InputActionOrigin origin);
	String getGlyphForXboxOrigin(int origin);
	String getGlyphPNGForActionOrigin(InputActionOrigin origin, InputGlyphSize size, uint32 flags);
	String getGlyphSVGForActionOrigin(InputActionOrigin origin, uint32 flags);
	InputType getInputTypeForHandle(uint64_t input_handle);
	Dictionary getMotionData(uint64_t input_handle);
	int getRemotePlaySessionID(uint64_t input_handle);
	uint16 getSessionInputConfigurationSettings();
	String getStringForActionOrigin(InputActionOrigin origin);
	String getStringForAnalogActionName(uint64_t action_handle);
	String getStringForDigitalActionName(uint64_t action_handle);
	String getStringForXboxOrigin(int origin);
	bool inputInit(bool explicitly_call_runframe = false);
	bool inputShutdown();
	void inputActionEventCallback(SteamInputActionEvent_t *call_data);
	bool newDataAvailable();
	void runFrame(bool reserved_value = true);
	void setLEDColor(uint64_t input_handle, int color_r, int color_g, int color_b, int flags);
	bool showBindingPanel(uint64_t input_handle);
	void stopAnalogActionMomentum(uint64_t input_handle, uint64_t action);
	int translateActionOrigin(InputType destination_input, InputActionOrigin source_origin);
	void triggerHapticPulse(uint64_t input_handle, int target_pad, int duration);
	void triggerRepeatedHapticPulse(uint64_t input_handle, int target_pad, int duration, int offset, int repeat, int flags);
	void triggerSimpleHapticEvent(uint64_t input_handle, int haptic_location, int intensity, const String &gain_db, int other_intensity, const String &other_gain_db);
	void triggerVibration(uint64_t input_handle, int left_speed, int right_speed);
	void triggerVibrationExtended(uint64_t input_handle, int left_speed, int right_speed, int left_trigger_speed, int right_trigger_speed);
	bool setInputActionManifestFilePath(const String &manifest_path);
	void setDualSenseTriggerEffect(uint64_t input_handle, int parameter_index, int trigger_mask, SCEPadTriggerEffectMode effect_mode, int position, int amplitude, int frequency);
	bool waitForData(bool wait_forever, uint32 timeout);

	// Inventory ////////////////////////////
	int32 addPromoItem(uint32 item);
	int32 addPromoItems(PackedInt64Array items);
	bool checkResultSteamID(uint64_t steam_id_expected, int32 this_inventory_handle = 0);
	int32 consumeItem(uint64_t item_consume, uint32 quantity);
	int32 deserializeResult(PackedByteArray buffer);
	void destroyResult(int32 this_inventory_handle = 0);
	int32 exchangeItems(const PackedInt64Array output_items, const PackedInt32Array output_quantity, const PackedInt64Array input_items, const PackedInt32Array input_quantity);
	int32 generateItems(const PackedInt64Array items, const PackedInt32Array quantity);
	int32 getAllItems();
	String getItemDefinitionProperty(uint32 definition, const String &name);
	int32 getItemsByID(const PackedInt64Array id_array);
	uint64_t getItemPrice(uint32 definition);
	Array getItemsWithPrices();
	String getResultItemProperty(uint32 index, const String &name, int32 this_inventory_handle = 0);
	Array getResultItems(int32 this_inventory_handle = 0);
	String getResultStatus(int32 this_inventory_handle = 0);
	uint32 getResultTimestamp(int32 this_inventory_handle = 0);
	int32 grantPromoItems();
	bool loadItemDefinitions();
	void requestEligiblePromoItemDefinitionsIDs(uint64_t steam_id);
	void requestPrices();
	String serializeResult(int32 this_inventory_handle = 0);
	void startPurchase(const PackedInt64Array items, const PackedInt32Array quantity);
	int32 transferItemQuantity(uint64_t item_id, uint32 quantity, uint64_t item_destination, bool split);
	int32 triggerItemDrop(uint32 definition);
	void startUpdateProperties();
	int32 submitUpdateProperties(uint64_t this_inventory_update_handle = 0);
	bool removeProperty(uint64_t item_id, const String &name, uint64_t this_inventory_update_handle = 0);
	bool setPropertyString(uint64_t item_id, const String &name, const String &value, uint64_t this_inventory_update_handle = 0);
	bool setPropertyBool(uint64_t item_id, const String &name, bool value, uint64_t this_inventory_update_handle = 0);
	bool setPropertyInt(uint64_t item_id, const String &name, uint64_t value, uint64_t this_inventory_update_handle = 0);
	bool setPropertyFloat(uint64_t item_id, const String &name, float value, uint64_t this_inventory_update_handle = 0);

	// Matchmaking //////////////////////////
	Array getFavoriteGames();
	int addFavoriteGame(uint32 ip, int port, int query_port, uint32 flags, uint32 last_played);
	bool removeFavoriteGame(uint32 app_id, uint32 ip, int port, int query_port, uint32 flags);
	void requestLobbyList();
	void addRequestLobbyListStringFilter(const String &key_to_match, const String &value_to_match, LobbyComparison comparison_type);
	void addRequestLobbyListNumericalFilter(const String &key_to_match, int value_to_match, LobbyComparison comparison_type);
	void addRequestLobbyListNearValueFilter(const String &key_to_match, int value_to_be_close_to);
	void addRequestLobbyListFilterSlotsAvailable(int slots_available);
	void addRequestLobbyListDistanceFilter(LobbyDistanceFilter distance_filter);
	void addRequestLobbyListResultCountFilter(int max_results);
	void createLobby(LobbyType lobby_type, int max_members);
	void joinLobby(uint64_t steam_lobby_id);
	void leaveLobby(uint64_t steam_lobby_id);
	bool inviteUserToLobby(uint64_t steam_lobby_id, uint64_t steam_id_invitee);
	int getNumLobbyMembers(uint64_t steam_lobby_id);
	uint64_t getLobbyMemberByIndex(uint64_t steam_lobby_id, int member);
	String getLobbyData(uint64_t steam_lobby_id, const String &key);
	bool setLobbyData(uint64_t steam_lobby_id, const String &key, const String &value);
	Dictionary getAllLobbyData(uint64_t steam_lobby_id);
	bool deleteLobbyData(uint64_t steam_lobby_id, const String &key);
	String getLobbyMemberData(uint64_t steam_lobby_id, uint64_t steam_id_user, const String &key);
	void setLobbyMemberData(uint64_t steam_lobby_id, const String &key, const String &value);
	bool sendLobbyChatMsg(uint64_t steam_lobby_id, const String &message_body);
	bool requestLobbyData(uint64_t steam_lobby_id);
	void setLobbyGameServer(uint64_t steam_lobby_id, const String &server_ip, int server_port, uint64_t steam_id_game_server);
	Dictionary getLobbyGameServer(uint64_t steam_lobby_id);
	bool setLobbyMemberLimit(uint64_t steam_lobby_id, int max_members);
	int getLobbyMemberLimit(uint64_t steam_lobby_id);
	bool setLobbyType(uint64_t steam_lobby_id, LobbyType lobby_type);
	bool setLobbyJoinable(uint64_t steam_lobby_id, bool joinable);
	uint64_t getLobbyOwner(uint64_t steam_lobby_id);
	bool setLobbyOwner(uint64_t steam_lobby_id, uint64_t steam_id_new_owner);

	// Matchmaking Servers //////////////////
	void cancelQuery(uint64_t server_list_request = 0);
	void cancelServerQuery(int server_query);
	int getServerCount(uint64_t server_list_request = 0);
	Dictionary getServerDetails(int server, uint64_t server_list_request = 0);
	bool isRefreshing(uint64_t server_list_request = 0);
	int pingServer(const String &ip, int port);
	int playerDetails(const String &ip, int port);
	void refreshQuery(uint64_t server_list_request = 0);
	void refreshServer(int server, uint64_t server_list_request = 0);
	void releaseRequest(uint64_t server_list_request = 0);
	uint64_t requestFavoritesServerList(uint32 app_id, Array filters);
	uint64_t requestFriendsServerList(uint32 app_id, Array filters);
	uint64_t requestHistoryServerList(uint32 app_id, Array filters);
	uint64_t requestInternetServerList(uint32 app_id, Array filters);
	uint64_t requestLANServerList(uint32 app_id);
	uint64_t requestSpectatorServerList(uint32 app_id, Array filters);
	int serverRules(const String &ip, int port);

	// Music ////////////////////////////////
	bool musicIsEnabled();
	bool musicIsPlaying();
	AudioPlaybackStatus getPlaybackStatus();
	float musicGetVolume();
	void musicPause();
	void musicPlay();
	void musicPlayNext();
	void musicPlayPrev();
	void musicSetVolume(float volume);

	// Music Remote /////////////////////////
	bool activationSuccess(bool activate);
	bool isCurrentMusicRemote();
	bool currentEntryDidChange();
	bool currentEntryIsAvailable(bool available);
	bool currentEntryWillChange();
	bool deregisterSteamMusicRemote();
	bool enableLooped(bool loop);
	bool enablePlaylists(bool playlists);
	bool enablePlayNext(bool next);
	bool enablePlayPrevious(bool previous);
	bool enableQueue(bool queue);
	bool enableShuffled(bool shuffle);
	bool playlistDidChange();
	bool playlistWillChange();
	bool queueDidChange();
	bool queueWillChange();
	bool registerSteamMusicRemote(const String &name);
	bool resetPlaylistEntries();
	bool resetQueueEntries();
	bool setCurrentPlaylistEntry(int id);
	bool setCurrentQueueEntry(int id);
	bool setDisplayName(const String &name);
	bool setPlaylistEntry(int id, int position, const String &entry_text);
	bool setPNGIcon64x64(PackedByteArray icon);
	bool setQueueEntry(int id, int position, const String &entry_text);
	bool updateCurrentEntryCoverArt(PackedByteArray art);
	bool updateCurrentEntryElapsedSeconds(int seconds);
	bool updateCurrentEntryText(const String &text);
	bool updateLooped(bool looped);
	bool updatePlaybackStatus(AudioPlaybackStatus status);
	bool updateShuffled(bool shuffle);
	bool updateVolume(float volume);

	// Networking ///////////////////////////
	bool acceptP2PSessionWithUser(uint64_t steam_id_remote);
	bool allowP2PPacketRelay(bool allow);
	bool closeP2PChannelWithUser(uint64_t steam_id_remote, int channel);
	bool closeP2PSessionWithUser(uint64_t steam_id_remote);
	Dictionary getP2PSessionState(uint64_t steam_id_remote);
	uint32_t getAvailableP2PPacketSize(int channel = 0);
	Dictionary readP2PPacket(uint32_t packet, int channel = 0);
	bool sendP2PPacket(uint64_t steam_id_remote, const PackedByteArray data, P2PSend send_type, int channel = 0);

	// Networking Messages //////////////////
	bool acceptSessionWithUser(const String &identity_reference);
	bool closeChannelWithUser(const String &identity_reference, int channel);
	bool closeSessionWithUser(const String &identity_reference);
	Dictionary getSessionConnectionInfo(const String &identity_reference, bool get_connection, bool get_status);
	Array receiveMessagesOnChannel(int channel, int max_messages);
	int sendMessageToUser(const String &identity_reference, const PackedByteArray data, int flags, int channel);

	// Networking Sockets ///////////////////
	int acceptConnection(uint32 connection_handle);
	bool beginAsyncRequestFakeIP(int num_ports);
	bool closeConnection(uint32 peer, int reason, const String &debug_message, bool linger);
	bool closeListenSocket(uint32 socket);
	int configureConnectionLanes(uint32 connection, int lanes, Array priorities, Array weights);
	uint32 connectP2P(const String &identity_reference, int virtual_port, Array options);
	uint32 connectByIPAddress(const String &ip_address_with_port, Array options);
	uint32 connectToHostedDedicatedServer(const String &identity_reference, int virtual_port, Array options);
	void createFakeUDPPort(int fake_server_port);
	uint32 createHostedDedicatedServerListenSocket(int virtual_port, Array options);
	uint32 createListenSocketIP(const String &ip_reference, Array options);
	uint32 createListenSocketP2P(int virtual_port, Array options);
	uint32 createListenSocketP2PFakeIP(int fake_port, Array options);
	uint32 createPollGroup();
	Dictionary createSocketPair(bool loopback, const String &identity_reference1, const String &identity_reference2);
	bool destroyPollGroup(uint32 poll_group);
//		int findRelayAuthTicketForServer(int port);	<------ Uses datagram relay structs which were removed from base SDK
	int flushMessagesOnConnection(uint32 connection_handle);
	NetworkingAvailability getAuthenticationStatus();
	Dictionary getCertificateRequest();
	Dictionary getConnectionInfo(uint32 connection_handle);
	String getConnectionName(uint32 peer);
	Dictionary getConnectionRealTimeStatus(uint32 connection_handle, int lanes, bool get_status = true);
	uint64_t getConnectionUserData(uint32 peer);
	Dictionary getDetailedConnectionStatus(uint32 connection_handle);
	Dictionary getFakeIP(int first_port = 0);
//		int getGameCoordinatorServerLogin(const String &app_data);	<------ Uses datagram relay structs which were removed from base SDK
//		int getHostedDedicatedServerAddress();	<------ Uses datagram relay structs which were removed from base SDK
	uint32 getHostedDedicatedServerPOPId();
	uint16 getHostedDedicatedServerPort();
	String getListenSocketAddress(uint32 socket, bool with_port = true);
	String getIdentity();
	Dictionary getRemoteFakeIPForConnection(uint32 connection);
	NetworkingAvailability initAuthentication();
	Array receiveMessagesOnConnection(uint32 connection, int max_messages);
	Array receiveMessagesOnPollGroup(uint32 poll_group, int max_messages);
//		Dictionary receivedRelayAuthTicket();	<------ Uses datagram relay structs which were removed from base SDK
	void resetIdentity(const String &this_identity);
	void runNetworkingCallbacks();
	void sendMessages(int messages, const PackedByteArray data, uint32 connection_handle, int flags);
	Dictionary sendMessageToConnection(uint32 connection_handle, const PackedByteArray data, int flags);
	Dictionary setCertificate(const PackedByteArray &certificate);
	bool setConnectionPollGroup(uint32 connection_handle, uint32 poll_group);
	void setConnectionName(uint32 peer, const String &name);

	// Networking Types /////////////////////
	bool addIdentity(const String &reference_name);
	bool addIPAddress(const String &reference_name);
	void clearIdentity(const String &reference_name);
	void clearIPAddress(const String &reference_name);
	uint8 getGenericBytes(const String &reference_name);
	String getGenericString(const String &reference_name);
	Array getIdentities();
	uint32 getIdentityIPAddr(const String &reference_name);
	uint32 getIdentitySteamID(const String &reference_name);
	uint64_t getIdentitySteamID64(const String &reference_name);
	Array getIPAddresses();
	uint32 getIPv4(const String &reference_name);
	uint64_t getPSNID(const String &reference_name);
	uint64_t getStadiaID(const String &reference_name);
	String getXboxPairwiseID(const String &reference_name);
	bool isAddressLocalHost(const String &reference_name);
	bool isIdentityInvalid(const String &reference_name);
	bool isIdentityLocalHost(const String &reference_name);
	bool isIPv4(const String &reference_name);
	bool isIPv6AllZeros(const String &reference_name);
	bool parseIdentityString(const String &reference_name, const String &string_to_parse);
	bool parseIPAddressString(const String &reference_name, const String &string_to_parse);
	bool setGenericBytes(const String &reference_name, int data);
	bool setGenericString(const String &reference_name, const String &this_string);
	bool setIdentityIPAddr(const String &reference_name, const String &ip_address_name);
	void setIdentityLocalHost(const String &reference_name);
	void setIdentitySteamID(const String &reference_name, uint32 steam_id);
	void setIdentitySteamID64(const String &reference_name, uint64_t steam_id);
	void setIPv4(const String &reference_name, uint32 ip, int port);
	void setIPv6(const String &reference_name, int ipv6, int port);
	void setIPv6LocalHost(const String &reference_name, int port = 0);
	void setPSNID(const String &reference_name, uint64_t psn_id);
	void setStadiaID(const String &reference_name, uint64_t stadia_id);
	bool setXboxPairwiseID(const String &reference_name, const String &xbox_id);
	String toIdentityString(const String &reference_name);
	String toIPAddressString(const String &reference_name, bool with_port);
	const SteamNetworkingConfigValue_t *convertOptionsArray(Array options);
	
	// Networking Utils /////////////////////
	bool checkPingDataUpToDate(float max_age_in_seconds);
	String convertPingLocationToString(PackedByteArray location);
	int estimatePingTimeBetweenTwoLocations(PackedByteArray location1, PackedByteArray location2);
	int estimatePingTimeFromLocalHost(PackedByteArray location);
	Dictionary getConfigValue(NetworkingConfigValue config_value, NetworkingConfigScope scope_type, uint32_t connection_handle);
	Dictionary getConfigValueInfo(NetworkingConfigValue config_value);
	int getDirectPingToPOP(uint32 pop_id);
	Dictionary getLocalPingLocation();
	uint64_t getLocalTimestamp();
	Dictionary getPingToDataCenter(uint32 pop_id);
	int getPOPCount();
	Array getPOPList();
	NetworkingAvailability getRelayNetworkStatus();
	void initRelayNetworkAccess();
	Dictionary parsePingLocationString(const String &location_string);
	bool setConnectionConfigValueFloat(uint32 connection, NetworkingConfigValue config, float value);
	bool setConnectionConfigValueInt32(uint32 connection, NetworkingConfigValue config, int32 value);
	bool setConnectionConfigValueString(uint32 connection, NetworkingConfigValue config, const String &value);
//		bool setConfigValue(NetworkingConfigValue setting, NetworkingConfigScope scope_type, uint32_t connection_handle, NetworkingConfigDataType data_type, auto value);
	bool setGlobalConfigValueFloat(NetworkingConfigValue config, float value);
	bool setGlobalConfigValueInt32(NetworkingConfigValue config, int32 value);
	bool setGlobalConfigValueString(NetworkingConfigValue config, const String &value);

	// Parental Settings ////////////////////
	bool isParentalLockEnabled();
	bool isParentalLockLocked();
	bool isAppBlocked(uint32 app_id);
	bool isAppInBlockList(uint32 app_id);
	bool isFeatureBlocked(ParentalFeature feature);
	bool isFeatureInBlockList(ParentalFeature feature);

	// Parties //////////////////////////////
	void cancelReservation(uint64_t beacon_id, uint64_t steam_id);
	void changeNumOpenSlots(uint64_t beacon_id, uint32 open_slots);
	void createBeacon(uint32 open_slots, uint64_t location_id, PartyBeaconLocationType type, const String &connect_string, const String &beacon_metadata);
	bool destroyBeacon(uint64_t beacon_id);
	Array getAvailableBeaconLocations(uint32 max);
	uint64_t getBeaconByIndex(uint32 index);
	Dictionary getBeaconDetails(uint64_t beacon_id);
	String getBeaconLocationData(uint64_t location_id, PartyBeaconLocationType location_type, PartyBeaconLocationData location_data);
	uint32 getNumActiveBeacons();
	void joinParty(uint64_t beacon_id);
	void onReservationCompleted(uint64_t beacon_id, uint64_t steam_id);

	// Remote Play //////////////////////////
	uint32 getSessionCount();
	uint32 getSessionID(uint32 index);
	uint64_t getSessionSteamID(uint32 session_id);
	String getSessionClientName(uint32 session_id);
	int getSessionClientFormFactor(uint32 session_id);
	Dictionary getSessionClientResolution(uint32 session_id);
	bool sendRemotePlayTogetherInvite(uint64_t friend_id);
	bool startRemotePlayTogether(bool show_overlay = true);

	// Remote Storage ///////////////////////
	bool beginFileWriteBatch();
	bool endFileWriteBatch();
	bool fileDelete(const String &file);
	bool fileExists(const String &file);
	bool fileForget(const String &file);
	bool filePersisted(const String &file);
	Dictionary fileRead(const String &file, int32_t data_to_read);
	void fileReadAsync(const String &file, uint32 offset, uint32_t data_to_read);
	void fileShare(const String &file);
	bool fileWrite(const String &file, PackedByteArray data, int32 size = 0);
	void fileWriteAsync(const String &file, PackedByteArray data, int32 size = 0);
	bool fileWriteStreamCancel(uint64_t write_handle);
	bool fileWriteStreamClose(uint64_t write_handle);
	uint64_t fileWriteStreamOpen(const String &file);
	bool fileWriteStreamWriteChunk(uint64_t write_handle, PackedByteArray data);
	int32 getCachedUGCCount();
	uint64_t getCachedUGCHandle(int32 content);
	int32_t getFileCount();
	Dictionary getFileNameAndSize(int file);
	int32_t getFileSize(const String &file);
	int64_t getFileTimestamp(const String &file);
	Dictionary getLocalFileChange(int file);
	uint32_t getLocalFileChangeCount();
	Dictionary getQuota();
	Dictionary getSyncPlatforms(const String &file);
	Dictionary getUGCDetails(uint64_t content);
	Dictionary getUGCDownloadProgress(uint64_t content);
	bool isCloudEnabledForAccount();
	bool isCloudEnabledForApp();
	void setCloudEnabledForApp(bool enabled);
	bool setSyncPlatforms(const String &file, int platform);
	void ugcDownload(uint64_t content, uint32 priority);
	void ugcDownloadToLocation(uint64_t content, const String &location, uint32 priority);
	PackedByteArray ugcRead(uint64_t content, int32 data_size, uint32 offset, UGCReadAction action);

	// Screenshots //////////////////////////
	uint32_t addScreenshotToLibrary(const String &filename, const String &thumbnail_filename, int width, int height);
	uint32_t addVRScreenshotToLibrary(VRScreenshotType type, const String &filename, const String &vr_filename);
	void hookScreenshots(bool hook);
	bool isScreenshotsHooked();
	bool setLocation(uint32_t screenshot, const String &location);
	bool tagPublishedFile(uint32_t screenshot, uint64_t file_id);
	bool tagUser(uint32_t screenshot, uint64_t steam_id);
	void triggerScreenshot();
	uint32_t writeScreenshot(const PackedByteArray &rgb, int width, int height);

	// UGC //////////////////////////////////
	void addAppDependency(uint64_t published_file_id, uint32_t app_id);
	bool addContentDescriptor(uint64_t update_handle, int descriptor_id);
	void addDependency(uint64_t published_file_id, uint64_t child_published_file_id);
	bool addExcludedTag(uint64_t query_handle, const String &tag_name);
	bool addItemKeyValueTag(uint64_t query_handle, const String &key, const String &value);
	bool addItemPreviewFile(uint64_t query_handle, const String &preview_file, ItemPreviewType type);
	bool addItemPreviewVideo(uint64_t query_handle, const String &video_id);
	void addItemToFavorites(uint32_t app_id, uint64_t published_file_id);
	bool addRequiredKeyValueTag(uint64_t query_handle, const String &key, const String &value);
	bool addRequiredTag(uint64_t query_handle, const String &tag_name);
	bool addRequiredTagGroup(uint64_t query_handle, Array tag_array);
	bool initWorkshopForGameServer(uint32_t workshop_depot_id);
	void createItem(uint32 app_id, WorkshopFileType file_type);
	uint64_t createQueryAllUGCRequest(UGCQuery query_type, UGCMatchingUGCType matching_type, uint32_t creator_id, uint32_t consumer_id, uint32 page);
	uint64_t createQueryUGCDetailsRequest(Array published_file_id);
	uint64_t createQueryUserUGCRequest(uint64_t steam_id, UserUGCList list_type, UGCMatchingUGCType matching_ugc_type, UserUGCListSortOrder sort_order, uint32_t creator_id, uint32_t consumer_id, uint32 page);
	void deleteItem(uint64_t published_file_id);
	bool downloadItem(uint64_t published_file_id, bool high_priority);
	Dictionary getItemDownloadInfo(uint64_t published_file_id);
	Dictionary getItemInstallInfo(uint64_t published_file_id);
	uint32 getItemState(uint64_t published_file_id);
	Dictionary getItemUpdateProgress(uint64_t update_handle);
	uint32 getNumSubscribedItems();
	Dictionary getQueryUGCAdditionalPreview(uint64_t query_handle, uint32 index, uint32 preview_index);
	Dictionary getQueryUGCChildren(uint64_t query_handle, uint32 index, uint32_t child_count);
	Dictionary getQueryUGCContentDescriptors(uint64_t query_handle, uint32 index, uint32_t max_entries);
	Dictionary getQueryUGCKeyValueTag(uint64_t query_handle, uint32 index, uint32 key_value_tag_index);
	String getQueryUGCMetadata(uint64_t query_handle, uint32 index);
	uint32 getQueryUGCNumAdditionalPreviews(uint64_t query_handle, uint32 index);
	uint32 getQueryUGCNumKeyValueTags(uint64_t query_handle, uint32 index);
	uint32 getQueryUGCNumTags(uint64_t query_handle, uint32 index);
	String getQueryUGCPreviewURL(uint64_t query_handle, uint32 index);
	Dictionary getQueryUGCResult(uint64_t query_handle, uint32 index);
	Dictionary getQueryUGCStatistic(uint64_t query_handle, uint32 index, ItemStatistic stat_type);
	String getQueryUGCTag(uint64_t query_handle, uint32 index, uint32 tag_index);
	String getQueryUGCTagDisplayName(uint64_t query_handle, uint32 index, uint32 tag_index);
	Array getSubscribedItems();
	Array getUserContentDescriptorPreferences(uint32 max_entries);
	void getUserItemVote(uint64_t published_file_id);
	bool releaseQueryUGCRequest(uint64_t query_handle);
	void removeAppDependency(uint64_t published_file_id, uint32_t app_id);
	bool removeContentDescriptor(uint64_t update_handle, int descriptor_id);
	void removeDependency(uint64_t published_file_id, uint64_t child_published_file_id);
	void removeItemFromFavorites(uint32_t app_id, uint64_t published_file_id);
	bool removeItemKeyValueTags(uint64_t update_handle, const String &key);
	bool removeItemPreview(uint64_t update_handle, uint32 index);
	void sendQueryUGCRequest(uint64_t update_handle);
	bool setAllowCachedResponse(uint64_t update_handle, uint32 max_age_seconds);
	bool setCloudFileNameFilter(uint64_t update_handle, const String &match_cloud_filename);
	bool setItemContent(uint64_t update_handle, const String &content_folder);
	bool setItemDescription(uint64_t update_handle, const String &description);
	bool setItemMetadata(uint64_t update_handle, const String &ugc_metadata);
	bool setItemPreview(uint64_t update_handle, const String &preview_file);
	bool setItemTags(uint64_t update_handle, Array tag_array, bool allow_admin_tags = false);
	bool setItemTitle(uint64_t update_handle, const String &title);
	bool setItemUpdateLanguage(uint64_t update_handle, const String &language);
	bool setItemVisibility(uint64_t update_handle, RemoteStoragePublishedFileVisibility visibility);
	bool setLanguage(uint64_t query_handle, const String &language);
	bool setMatchAnyTag(uint64_t query_handle, bool match_any_tag);
	bool setRankedByTrendDays(uint64_t query_handle, uint32 days);
	bool setReturnAdditionalPreviews(uint64_t query_handle, bool return_additional_previews);
	bool setReturnChildren(uint64_t query_handle, bool return_children);
	bool setReturnKeyValueTags(uint64_t query_handle, bool return_key_value_tags);
	bool setReturnLongDescription(uint64_t query_handle, bool return_long_description);
	bool setReturnMetadata(uint64_t query_handle, bool return_metadata);
	bool setReturnOnlyIDs(uint64_t query_handle, bool return_only_ids);
	bool setReturnPlaytimeStats(uint64_t query_handle, uint32 days);
	bool setReturnTotalOnly(uint64_t query_handle, bool return_total_only);
	bool setSearchText(uint64_t query_handle, const String &search_text);
	void setUserItemVote(uint64_t published_file_id, bool vote_up);
	uint64_t startItemUpdate(uint32_t app_id, uint64_t file_id);
	void startPlaytimeTracking(Array published_file_ids);
	void stopPlaytimeTracking(Array published_file_ids);
	void stopPlaytimeTrackingForAllItems();
	void getAppDependencies(uint64_t published_file_id);
	void submitItemUpdate(uint64_t update_handle, const String &change_note);
	void subscribeItem(uint64_t published_file_id);
	void suspendDownloads(bool suspend);
	void unsubscribeItem(uint64_t published_file_id);
	bool updateItemPreviewFile(uint64_t update_handle, uint32 index, const String &preview_file);
	bool updateItemPreviewVideo(uint64_t update_handle, uint32 index, const String &video_id);
	bool showWorkshopEULA();
	void getWorkshopEULAStatus();
	bool setTimeCreatedDateRange(uint64_t update_handle, uint32 start, uint32 end);
	bool setTimeUpdatedDateRange(uint64_t update_handle, uint32 start, uint32 end);

	// Users ////////////////////////////////
	void advertiseGame(const String &server_ip, int port);
	BeginAuthSessionResult beginAuthSession(PackedByteArray ticket, int ticket_size, uint64_t steam_id);
	void cancelAuthTicket(uint32_t auth_ticket);
	Dictionary decompressVoice(const PackedByteArray &voice, uint32 voice_size, uint32 sample_rate);
	void endAuthSession(uint64_t steam_id);
	Dictionary getAuthSessionTicket(const String &identity_reference = "");
	uint32 getAuthTicketForWebApi(const String &service_identity = "");
	Dictionary getAvailableVoice();
	void getDurationControl();
	Dictionary getEncryptedAppTicket();
	int getGameBadgeLevel(int series, bool foil);
	int getPlayerSteamLevel();
	uint64_t getSteamID();
	Dictionary getVoice();
	uint32 getVoiceOptimalSampleRate();
	Dictionary initiateGameConnection(uint64_t server_id, uint32 server_ip, int server_port, bool secure);
	bool isBehindNAT();
	bool isPhoneIdentifying();
	bool isPhoneRequiringVerification();
	bool isPhoneVerified();
	bool isTwoFactorEnabled();
	bool loggedOn();
	void requestEncryptedAppTicket(const String &secret);
	void requestStoreAuthURL(const String &redirect);
	void startVoiceRecording();
	bool setDurationControlOnlineState(int new_state);
	void stopVoiceRecording();
	void terminateGameConnection(uint32 server_ip, int server_port);
	int userHasLicenseForApp(uint64_t steam_id, uint32_t app_id);

	// User Stats ///////////////////////////
	void attachLeaderboardUGC(uint64_t ugc_handle, uint64_t this_leaderboard = 0);
	bool clearAchievement(const String &achievement_name);
	void downloadLeaderboardEntries(int start, int end, LeaderboardDataRequest type = LeaderboardDataRequest(k_ELeaderboardDataRequestGlobal), uint64_t this_leaderboard = 0);
	void downloadLeaderboardEntriesForUsers(Array users_id, uint64_t this_leaderboard = 0);
	void findLeaderboard(const String &leaderboard_name);
	void findOrCreateLeaderboard(const String &leaderboard_name, LeaderboardSortMethod sort_method, LeaderboardDisplayType display_type);
	Dictionary getAchievement(const String &achievement_name);
	Dictionary getAchievementAchievedPercent(const String &achievement_name);
	Dictionary getAchievementAndUnlockTime(const String &achievement_name);
	String getAchievementDisplayAttribute(const String &achievement_name, const String &key);
	int getAchievementIcon(const String &achievement_name);
	String getAchievementName(uint32_t achievement);
	Dictionary getAchievementProgressLimitsInt(const String &achievement_name);
	Dictionary getAchievementProgressLimitsFloat(const String &achievement_name);
	uint64_t getGlobalStatInt(const String &stat_name);
	double getGlobalStatFloat(const String &stat_name);
	uint64_t getGlobalStatIntHistory(const String &stat_name);
	double getGlobalStatFloatHistory(const String &stat_name);
	Dictionary getLeaderboardDisplayType(uint64_t this_leaderboard = 0);
	Array getLeaderboardEntries();
	int getLeaderboardEntryCount(uint64_t this_leaderboard = 0);
	String getLeaderboardName(uint64_t this_leaderboard = 0);
	Dictionary getLeaderboardSortMethod(uint64_t this_leaderboard = 0);
	Dictionary getMostAchievedAchievementInfo();
	Dictionary getNextMostAchievedAchievementInfo(int iterator);
	uint32_t getNumAchievements();
	void getNumberOfCurrentPlayers();
	float getStatFloat(const String &stat_name);
	int getStatInt(const String &stat_name);
	Dictionary getUserAchievement(uint64_t steam_id, const String &name);
	Dictionary getUserAchievementAndUnlockTime(uint64_t steam_id, const String &name);
	float getUserStatFloat(uint64_t steam_id, const String &name);
	int getUserStatInt(uint64_t steam_id, const String &name);
	bool indicateAchievementProgress(const String &name, int current_progress, int max_progress);
	bool requestCurrentStats();
	void requestGlobalAchievementPercentages();
	void requestGlobalStats(int history_days);
	void requestUserStats(uint64_t steam_id);
	bool resetAllStats(bool achievements_too = true);
	bool setAchievement(const String &name);
	int setLeaderboardDetailsMax(int max);
	bool setStatFloat(const String &name, float value);
	bool setStatInt(const String &name, int value);
	bool storeStats();
	bool updateAvgRateStat(const String &name, float this_session, double session_length);
	void uploadLeaderboardScore(int score, bool keep_best = false, PackedInt32Array details = PackedInt32Array(), uint64_t this_leaderboard = 0);

	// Utils ////////////////////////////////
	bool dismissFloatingGamepadTextInput();
	bool dismissGamepadTextInput();
	String filterText(TextFilteringContext context, uint64_t steam_id, const String &message);
	String getAPICallFailureReason();
	uint32_t getAppID();
	int getCurrentBatteryPower();
	Dictionary getImageRGBA(int image);
	Dictionary getImageSize(int image);
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
	bool isSteamRunningOnSteamDeck();
	bool isVRHeadsetStreamingEnabled();
	bool overlayNeedsPresent();
	void setGameLauncherMode(bool mode);
	void setOverlayNotificationInset(int horizontal, int vertical);
	void setOverlayNotificationPosition(int pos);
	void setVRHeadsetStreamingEnabled(bool enabled);
	bool showGamepadTextInput(GamepadTextInputMode input_mode, GamepadTextInputLineMode line_input_mode, const String &description, uint32 max_text, const String &preset_text);
	bool showFloatingGamepadTextInput(FloatingGamepadTextInputMode input_mode, int text_field_x_position, int text_field_y_position, int text_field_width, int text_field_height);
	void startVRDashboard();
	
	// Video ////////////////////////////////
	void getOPFSettings(uint32_t app_id);
	String getOPFStringForApp(uint32_t app_id);
	void getVideoURL(uint32_t app_id);
	Dictionary isBroadcasting();


protected:
	static void _bind_methods();
	static Steam *singleton;


private:
	// Main
	bool is_init_success;

	// Apps
	uint64_t current_app_id = 0;

	// Friends
	CSteamID clan_activity;

	// HTML Surface
	uint32 browser_handle;

	// Inventory
	SteamInventoryUpdateHandle_t inventory_update_handle;
	SteamInventoryResult_t inventory_handle;
	SteamItemDetails_t inventory_details;

	// Leaderboards
	SteamLeaderboard_t leaderboard_handle;
	Array leaderboard_entries_array;
	int leaderboard_details_max = LEADERBOARD_DETAIL_MAX;

	// Matchmaking Server
	HServerListRequest server_list_request;
	ISteamMatchmakingServerListResponse *server_list_response;
	ISteamMatchmakingPingResponse *ping_response;
	ISteamMatchmakingPlayersResponse *player_response;
	ISteamMatchmakingRulesResponse *rules_response;

	// Networking Messages
//		std::map<int, SteamNetworkingMessage_t> network_messages;

	// Networking Sockets
	uint32 network_connection;
	uint32 network_poll_group;
	uint64_t networking_microseconds = 0;
	SteamNetworkingIdentity networking_identity;
//		SteamDatagramHostedAddress hosted_address;
	PackedByteArray routing_blob;
//		SteamDatagramRelayAuthTicket relay_auth_ticket;
	std::map<String, SteamNetworkingIdentity> networking_identities;
	std::map<String, SteamNetworkingIPAddr> ip_addresses;

	// Parties
	uint64 party_beacon_id;

	// Remote Storage
	uint64_t write_stream_handle = 0;

	// User stats
	int number_achievements = 0;
	bool stats_initialized = false;

	// Utils
	uint64_t api_handle = 0;

	// Run the Steamworks API callbacks /////
	void run_callbacks() {
		SteamAPI_RunCallbacks();
	}


	// STEAM CALLBACKS
	/////////////////////////////////////////
	//
	// Apps callbacks ///////////////////////
	STEAM_CALLBACK(Steam, dlc_installed, DlcInstalled_t, callbackDLCInstalled);
	STEAM_CALLBACK(Steam, file_details_result, FileDetailsResult_t, callbackFileDetailsResult);
	STEAM_CALLBACK(Steam, new_launch_url_parameters, NewUrlLaunchParameters_t, callbackNewLaunchURLParameters);
	STEAM_CALLBACK(Steam, timed_trial_status, TimedTrialStatus_t, callbackTimedTrialStatus);

	// Friends callbacks ////////////////////
	STEAM_CALLBACK(Steam, avatar_loaded, AvatarImageLoaded_t, callbackAvatarLoaded);
	STEAM_CALLBACK(Steam, avatar_image_loaded, AvatarImageLoaded_t, callbackAvatarImageLoaded);
	STEAM_CALLBACK(Steam, clan_activity_downloaded, DownloadClanActivityCountsResult_t, callbackClanActivityDownloaded);
	STEAM_CALLBACK(Steam, friend_rich_presence_update, FriendRichPresenceUpdate_t, callbackFriendRichPresenceUpdate);
	STEAM_CALLBACK(Steam, connected_chat_join, GameConnectedChatJoin_t, callbackConnectedChatJoin);
	STEAM_CALLBACK(Steam, connected_chat_leave, GameConnectedChatLeave_t, callbackConnectedChatLeave);
	STEAM_CALLBACK(Steam, connected_clan_chat_message, GameConnectedClanChatMsg_t, callbackConnectedClanChatMessage);
	STEAM_CALLBACK(Steam, connected_friend_chat_message, GameConnectedFriendChatMsg_t, callbackConnectedFriendChatMessage);
	STEAM_CALLBACK(Steam, join_requested, GameLobbyJoinRequested_t, callbackJoinRequested);
	STEAM_CALLBACK(Steam, overlay_toggled, GameOverlayActivated_t, callbackOverlayToggled);
	STEAM_CALLBACK(Steam, join_game_requested, GameRichPresenceJoinRequested_t, callbackJoinGameRequested);
	STEAM_CALLBACK(Steam, change_server_requested, GameServerChangeRequested_t, callbackChangeServerRequested);
	STEAM_CALLBACK(Steam, join_clan_chat_complete, JoinClanChatRoomCompletionResult_t, callbackJoinClanChatComplete);
	STEAM_CALLBACK(Steam, persona_state_change, PersonaStateChange_t, callbackPersonaStateChange);
	STEAM_CALLBACK(Steam, name_changed, SetPersonaNameResponse_t, callbackNameChanged);
	STEAM_CALLBACK(Steam, overlay_browser_protocol, OverlayBrowserProtocolNavigation_t, callbackOverlayBrowserProtocol);
	STEAM_CALLBACK(Steam, unread_chat_messages_changed, UnreadChatMessagesChanged_t, callbackUnreadChatMessagesChanged);
	STEAM_CALLBACK(Steam, equipped_profile_items_changed, EquippedProfileItemsChanged_t, callbackEquippedProfileItemsChanged);

	// Game Search callbacks ////////////////
	STEAM_CALLBACK(Steam, search_for_game_progress, SearchForGameProgressCallback_t, callbackSearchForGameProgress);
	STEAM_CALLBACK(Steam, search_for_game_result, SearchForGameResultCallback_t, callbackSearchForGameResult);
	STEAM_CALLBACK(Steam, request_players_for_game_progress, RequestPlayersForGameProgressCallback_t, callbackRequestPlayersForGameProgress);
	STEAM_CALLBACK(Steam, request_players_for_game_result, RequestPlayersForGameResultCallback_t, callbackRequestPlayersForGameResult);
	STEAM_CALLBACK(Steam, request_players_for_game_final_result, RequestPlayersForGameFinalResultCallback_t, callbackRequestPlayersForGameFinalResult);
	STEAM_CALLBACK(Steam, submit_player_result, SubmitPlayerResultResultCallback_t, callbackSubmitPlayerResult);
	STEAM_CALLBACK(Steam, end_game_result, EndGameResultCallback_t, callbackEndGameResult);

	// HTML Surface callbacks ///////////////
	STEAM_CALLBACK(Steam, html_can_go_backandforward, HTML_CanGoBackAndForward_t, callbackHTMLCanGoBackandforward);
	STEAM_CALLBACK(Steam, html_changed_title, HTML_ChangedTitle_t, callbackHTMLChangedTitle);
	STEAM_CALLBACK(Steam, html_close_browser, HTML_CloseBrowser_t, callbackHTMLCloseBrowser);
	STEAM_CALLBACK(Steam, html_file_open_dialog, HTML_FileOpenDialog_t, callbackHTMLFileOpenDialog);
	STEAM_CALLBACK(Steam, html_finished_request, HTML_FinishedRequest_t, callbackHTMLFinishedRequest);
	STEAM_CALLBACK(Steam, html_hide_tooltip, HTML_HideToolTip_t, callbackHTMLHideTooltip);
	STEAM_CALLBACK(Steam, html_horizontal_scroll, HTML_HorizontalScroll_t, callbackHTMLHorizontalScroll);
	STEAM_CALLBACK(Steam, html_js_alert, HTML_JSAlert_t, callbackHTMLJSAlert);
	STEAM_CALLBACK(Steam, html_js_confirm, HTML_JSConfirm_t, callbackHTMLJSConfirm);
	STEAM_CALLBACK(Steam, html_link_at_position, HTML_LinkAtPosition_t, callbackHTMLLinkAtPosition);
	STEAM_CALLBACK(Steam, html_needs_paint, HTML_NeedsPaint_t, callbackHTMLNeedsPaint);
	STEAM_CALLBACK(Steam, html_new_window, HTML_NewWindow_t, callbackHTMLNewWindow);
	STEAM_CALLBACK(Steam, html_open_link_in_new_tab, HTML_OpenLinkInNewTab_t, callbackHTMLOpenLinkInNewTab);
	STEAM_CALLBACK(Steam, html_search_results, HTML_SearchResults_t, callbackHTMLSearchResults);
	STEAM_CALLBACK(Steam, html_set_cursor, HTML_SetCursor_t, callbackHTMLSetCursor);
	STEAM_CALLBACK(Steam, html_show_tooltip, HTML_ShowToolTip_t, callbackHTMLShowTooltip);
	STEAM_CALLBACK(Steam, html_start_request, HTML_StartRequest_t, callbackHTMLStartRequest);
	STEAM_CALLBACK(Steam, html_status_text, HTML_StatusText_t, callbackHTMLStatusText);
	STEAM_CALLBACK(Steam, html_update_tooltip, HTML_UpdateToolTip_t, callbackHTMLUpdateTooltip);
	STEAM_CALLBACK(Steam, html_url_changed, HTML_URLChanged_t, callbackHTMLURLChanged);
	STEAM_CALLBACK(Steam, html_vertical_scroll, HTML_VerticalScroll_t, callbackHTMLVerticalScroll);

	// HTTP callbacks ///////////////////////
	STEAM_CALLBACK(Steam, http_request_completed, HTTPRequestCompleted_t, callbackHTTPRequestCompleted);
	STEAM_CALLBACK(Steam, http_request_data_received, HTTPRequestDataReceived_t, callbackHTTPRequestDataReceived);
	STEAM_CALLBACK(Steam, http_request_headers_received, HTTPRequestHeadersReceived_t, callbackHTTPRequestHeadersReceived);

	// Input callbacks //////////////////////
	STEAM_CALLBACK(Steam, input_device_connected, SteamInputDeviceConnected_t, callbackInputDeviceConnected);
	STEAM_CALLBACK(Steam, input_device_disconnected, SteamInputDeviceDisconnected_t, callbackInputDeviceDisconnected);
	STEAM_CALLBACK(Steam, input_configuration_loaded, SteamInputConfigurationLoaded_t, callbackInputConfigurationLoaded);
	STEAM_CALLBACK(Steam, input_gamepad_slot_change, SteamInputGamepadSlotChange_t, callbackInputGamePadSlotChange);

	// Inventory callbacks //////////////////
	STEAM_CALLBACK(Steam, inventory_definition_update, SteamInventoryDefinitionUpdate_t, callbackInventoryDefinitionUpdate);
	STEAM_CALLBACK(Steam, inventory_full_update, SteamInventoryFullUpdate_t, callbackInventoryFullUpdate);
	STEAM_CALLBACK(Steam, inventory_result_ready, SteamInventoryResultReady_t, callbackInventoryResultReady);

	// Matchmaking callbacks ////////////////
	STEAM_CALLBACK(Steam, favorites_list_accounts_updated, FavoritesListAccountsUpdated_t, callbackFavoritesListAccountsUpdated);
	STEAM_CALLBACK(Steam, favorites_list_changed, FavoritesListChanged_t, callbackFavoritesListChanged);
	STEAM_CALLBACK(Steam, lobby_message, LobbyChatMsg_t, callbackLobbyMessage);
	STEAM_CALLBACK(Steam, lobby_chat_update, LobbyChatUpdate_t, callbackLobbyChatUpdate);
	STEAM_CALLBACK(Steam, lobby_data_update, LobbyDataUpdate_t, callbackLobbyDataUpdate);
	STEAM_CALLBACK(Steam, lobby_joined, LobbyEnter_t, callbackLobbyJoined);
	STEAM_CALLBACK(Steam, lobby_game_created, LobbyGameCreated_t, callbackLobbyGameCreated);
	STEAM_CALLBACK(Steam, lobby_invite, LobbyInvite_t, callbackLobbyInvite);
	STEAM_CALLBACK(Steam, lobby_kicked, LobbyKicked_t, callbackLobbyKicked);

	// Music callbacks //////////////////////
	STEAM_CALLBACK(Steam, music_playback_status_has_changed, PlaybackStatusHasChanged_t, callbackMusicPlaybackStatusHasChanged);
	STEAM_CALLBACK(Steam, music_volume_has_changed, VolumeHasChanged_t, callbackMusicVolumeHasChanged);

	// Music Remote callbacks ///////////////
	STEAM_CALLBACK(Steam, music_player_remote_to_front, MusicPlayerRemoteToFront_t, callbackMusicPlayerRemoteToFront);
	STEAM_CALLBACK(Steam, music_player_remote_will_activate, MusicPlayerRemoteWillActivate_t, callbackMusicPlayerRemoteWillActivate);
	STEAM_CALLBACK(Steam, music_player_remote_will_deactivate, MusicPlayerRemoteWillDeactivate_t, callbackMusicPlayerRemoteWillDeactivate);
	STEAM_CALLBACK(Steam, music_player_selects_playlist_entry, MusicPlayerSelectsPlaylistEntry_t, callbackMusicPlayerSelectsPlaylistEntry);
	STEAM_CALLBACK(Steam, music_player_selects_queue_entry, MusicPlayerSelectsQueueEntry_t, callbackMusicPlayerSelectsQueueEntry);
	STEAM_CALLBACK(Steam, music_player_wants_looped, MusicPlayerWantsLooped_t, callbackMusicPlayerWantsLooped);
	STEAM_CALLBACK(Steam, music_player_wants_pause, MusicPlayerWantsPause_t, callbackMusicPlayerWantsPause);
	STEAM_CALLBACK(Steam, music_player_wants_playing_repeat_status, MusicPlayerWantsPlayingRepeatStatus_t, callbackMusicPlayerWantsPlayingRepeatStatus);
	STEAM_CALLBACK(Steam, music_player_wants_play_next, MusicPlayerWantsPlayNext_t, callbackMusicPlayerWantsPlayNext);
	STEAM_CALLBACK(Steam, music_player_wants_play_previous, MusicPlayerWantsPlayPrevious_t, callbackMusicPlayerWantsPlayPrevious);
	STEAM_CALLBACK(Steam, music_player_wants_play, MusicPlayerWantsPlay_t, callbackMusicPlayerWantsPlay);
	STEAM_CALLBACK(Steam, music_player_wants_shuffled, MusicPlayerWantsShuffled_t, callbackMusicPlayerWantsShuffled);
	STEAM_CALLBACK(Steam, music_player_wants_volume, MusicPlayerWantsVolume_t, callbackMusicPlayerWantsVolume);
	STEAM_CALLBACK(Steam, music_player_will_quit, MusicPlayerWillQuit_t, callbackMusicPlayerWillQuit);

	// Networking callbacks /////////////////
	STEAM_CALLBACK(Steam, p2p_session_connect_fail, P2PSessionConnectFail_t, callbackP2PSessionConnectFail);
	STEAM_CALLBACK(Steam, p2p_session_request, P2PSessionRequest_t, callbackP2PSessionRequest);

	// Networking Messages callbacks ////////
	STEAM_CALLBACK(Steam, network_messages_session_request, SteamNetworkingMessagesSessionRequest_t, callbackNetworkMessagesSessionRequest);
	STEAM_CALLBACK(Steam, network_messages_session_failed, SteamNetworkingMessagesSessionFailed_t, callbackNetworkMessagesSessionFailed);

	// Networking Sockets callbacks /////////
	STEAM_CALLBACK(Steam, network_connection_status_changed, SteamNetConnectionStatusChangedCallback_t, callbackNetworkConnectionStatusChanged);
	STEAM_CALLBACK(Steam, network_authentication_status, SteamNetAuthenticationStatus_t, callbackNetworkAuthenticationStatus);
	STEAM_CALLBACK(Steam, fake_ip_result, SteamNetworkingFakeIPResult_t, callbackNetworkingFakeIPResult);

	// Networking Utils callbacks ///////////
	STEAM_CALLBACK(Steam, relay_network_status, SteamRelayNetworkStatus_t, callbackRelayNetworkStatus);

	// Parental Settings callbacks //////////
	STEAM_CALLBACK(Steam, parental_setting_changed, SteamParentalSettingsChanged_t, callbackParentlSettingChanged);

	// Parties callbacks ////////////////////
	STEAM_CALLBACK(Steam, reservation_notification, ReservationNotificationCallback_t, callbackReserveNotification);
	STEAM_CALLBACK(Steam, available_beacon_locations_updated, AvailableBeaconLocationsUpdated_t, callbackAvailableBeaconLocationsUpdated);
	STEAM_CALLBACK(Steam, active_beacons_updated, ActiveBeaconsUpdated_t, callbackActiveBeaconsUpdated);

	// Remote Play callbacks ////////////////
	STEAM_CALLBACK(Steam, remote_play_session_connected, SteamRemotePlaySessionConnected_t, callbackRemotePlaySessionConnected);
	STEAM_CALLBACK(Steam, remote_play_session_disconnected, SteamRemotePlaySessionDisconnected_t, callbackRemotePlaySessionDisconnected);

	// Remote Storage callbacks /////////////
	STEAM_CALLBACK(Steam, local_file_changed, RemoteStorageLocalFileChange_t, callbackLocalFileChanged);

	// Screenshot callbacks /////////////////
	STEAM_CALLBACK(Steam, screenshot_ready, ScreenshotReady_t, callbackScreenshotReady);
	STEAM_CALLBACK(Steam, screenshot_requested, ScreenshotRequested_t, callbackScreenshotRequested);

	// UGC callbacks ////////////////////////
	STEAM_CALLBACK(Steam, item_downloaded, DownloadItemResult_t, callbackItemDownloaded);
	STEAM_CALLBACK(Steam, item_installed, ItemInstalled_t, callbackItemInstalled);
	STEAM_CALLBACK(Steam, user_subscribed_items_list_changed, UserSubscribedItemsListChanged_t, callbackUserSubscribedItemsListChanged);

	// User callbacks ///////////////////////
	STEAM_CALLBACK(Steam, client_game_server_deny, ClientGameServerDeny_t, callbackClientGameServerDeny);
	STEAM_CALLBACK(Steam, game_web_callback, GameWebCallback_t, callbackGameWebCallback);
	STEAM_CALLBACK(Steam, get_auth_session_ticket_response, GetAuthSessionTicketResponse_t, callbackGetAuthSessionTicketResponse);
	STEAM_CALLBACK(Steam, get_ticket_for_web_api, GetTicketForWebApiResponse_t, callbackGetTicketForWebApiResponse);
	STEAM_CALLBACK(Steam, ipc_failure, IPCFailure_t, callbackIPCFailure);
	STEAM_CALLBACK(Steam, licenses_updated, LicensesUpdated_t, callbackLicensesUpdated);
	STEAM_CALLBACK(Steam, microtransaction_auth_response, MicroTxnAuthorizationResponse_t, callbackMicrotransactionAuthResponse);
	STEAM_CALLBACK(Steam, steam_server_connected, SteamServersConnected_t, callbackSteamServerConnected);
	STEAM_CALLBACK(Steam, steam_server_disconnected, SteamServersDisconnected_t, callbackSteamServerDisconnected);
	STEAM_CALLBACK(Steam, validate_auth_ticket_response, ValidateAuthTicketResponse_t, callbackValidateAuthTicketResponse);

	// User stat callbacks //////////////////
	STEAM_CALLBACK(Steam, user_achievement_stored, UserAchievementStored_t, callbackUserAchievementStored);
	STEAM_CALLBACK(Steam, current_stats_received, UserStatsReceived_t, callbackCurrentStatsReceived);
	STEAM_CALLBACK(Steam, user_stats_stored, UserStatsStored_t, callbackUserStatsStored);
	STEAM_CALLBACK(Steam, user_stats_unloaded, UserStatsUnloaded_t, callbackUserStatsUnloaded);

	// Utility callbacks ////////////////////
	STEAM_CALLBACK(Steam, gamepad_text_input_dismissed, GamepadTextInputDismissed_t, callbackGamepadTextInputDismissed);
	STEAM_CALLBACK(Steam, ip_country, IPCountry_t, callbackIPCountry);
	STEAM_CALLBACK(Steam, low_power, LowBatteryPower_t, callbackLowPower);
	STEAM_CALLBACK(Steam, steam_api_call_completed, SteamAPICallCompleted_t, callbackSteamAPICallCompleted);
	STEAM_CALLBACK(Steam, steam_shutdown, SteamShutdown_t, callbackSteamShutdown);
	STEAM_CALLBACK(Steam, app_resuming_from_suspend, AppResumingFromSuspend_t, callbackAppResumingFromSuspend);
	STEAM_CALLBACK(Steam, floating_gamepad_text_input_dismissed, FloatingGamepadTextInputDismissed_t, callbackFloatingGamepadTextInputDismissed);
	STEAM_CALLBACK(Steam, filter_text_dictionary_changed, FilterTextDictionaryChanged_t, callbackFilterTextDictionaryChanged);

	// Video callbacks //////////////////////
	STEAM_CALLBACK(Steam, get_opf_settings_result, GetOPFSettingsResult_t, callbackGetOPFSettingsResult);
	STEAM_CALLBACK(Steam, get_video_result, GetVideoURLResult_t, callbackGetVideoResult);


	// STEAM CALL RESULTS
	/////////////////////////////////////////
	//
	// Friends call results /////////////////
	CCallResult<Steam, ClanOfficerListResponse_t> callResultClanOfficerList;
	void request_clan_officer_list(ClanOfficerListResponse_t *call_data, bool io_failure);
	CCallResult<Steam, FriendsEnumerateFollowingList_t> callResultEnumerateFollowingList;
	void enumerate_following_list(FriendsEnumerateFollowingList_t *call_data, bool io_failure);
	CCallResult<Steam, EquippedProfileItems_t> callResultEquippedProfileItems;
	void equipped_profile_items(EquippedProfileItems_t *call_data, bool io_failure);
	CCallResult<Steam, FriendsGetFollowerCount_t> callResultFollowerCount;
	void get_follower_count(FriendsGetFollowerCount_t *call_data, bool io_failure);
	CCallResult<Steam, FriendsIsFollowing_t> callResultIsFollowing;
	void is_following(FriendsIsFollowing_t *call_data, bool io_failure);

	// HTML Surface call results ////////////
	CCallResult<Steam, HTML_BrowserReady_t> callResultHTMLBrowserReady;
	void html_browser_ready(HTML_BrowserReady_t *call_data, bool io_failure);

	// Inventory call results ///////////////
	CCallResult<Steam, SteamInventoryEligiblePromoItemDefIDs_t> callResultEligiblePromoItemDefIDs;
	void inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *call_data, bool io_failure);
	CCallResult<Steam, SteamInventoryRequestPricesResult_t> callResultRequestPrices;
	void inventory_request_prices_result(SteamInventoryRequestPricesResult_t *call_data, bool io_failure);
	CCallResult<Steam, SteamInventoryStartPurchaseResult_t> callResultStartPurchase;
	void inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *call_data, bool io_failure);

	// Matchmaking call results /////////////
	CCallResult<Steam, LobbyCreated_t> callResultCreateLobby;
	void lobby_created(LobbyCreated_t *call_data, bool io_failure);
	CCallResult<Steam, LobbyMatchList_t> callResultLobbyList;
	void lobby_match_list(LobbyMatchList_t *call_data, bool io_failure);

	// Matchmaking Server call results //////
	void server_Responded(gameserveritem_t server);
	void server_Failed_To_Respond();

	// Parties call results /////////////////
	CCallResult<Steam, JoinPartyCallback_t> callResultJoinParty;
	void join_party(JoinPartyCallback_t *call_data, bool io_failure);
	CCallResult<Steam, CreateBeaconCallback_t> callResultCreateBeacon;
	void create_beacon(CreateBeaconCallback_t *call_data, bool io_failure);
	CCallResult<Steam, ChangeNumOpenSlotsCallback_t> callResultChangeNumOpenSlots;
	void change_num_open_slots(ChangeNumOpenSlotsCallback_t *call_data, bool io_failure);

	// Remote Storage call results //////////
	CCallResult<Steam, RemoteStorageFileReadAsyncComplete_t> callResultFileReadAsyncComplete;
	void file_read_async_complete(RemoteStorageFileReadAsyncComplete_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageFileShareResult_t> callResultFileShareResult;
	void file_share_result(RemoteStorageFileShareResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageFileWriteAsyncComplete_t> callResultFileWriteAsyncComplete;
	void file_write_async_complete(RemoteStorageFileWriteAsyncComplete_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageDownloadUGCResult_t> callResultDownloadUGCResult;
	void download_ugc_result(RemoteStorageDownloadUGCResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageUnsubscribePublishedFileResult_t> callResultUnsubscribeItem;
	void unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageSubscribePublishedFileResult_t> callResultSubscribeItem;
	void subscribe_item(RemoteStorageSubscribePublishedFileResult_t *call_data, bool io_failure);

	// UGC call results /////////////////////
	CCallResult<Steam, AddAppDependencyResult_t> callResultAddAppDependency;
	void add_app_dependency_result(AddAppDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, AddUGCDependencyResult_t> callResultAddUGCDependency;
	void add_ugc_dependency_result(AddUGCDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, CreateItemResult_t> callResultItemCreate;
	void item_created(CreateItemResult_t *call_data, bool io_failure);
	CCallResult<Steam, GetAppDependenciesResult_t> callResultGetAppDependencies;
	void get_app_dependencies_result(GetAppDependenciesResult_t *call_data, bool io_failure);
	CCallResult<Steam, DeleteItemResult_t> callResultDeleteItem;
	void item_deleted(DeleteItemResult_t *call_data, bool io_failure);
	CCallResult<Steam, GetUserItemVoteResult_t> callResultGetUserItemVote;
	void get_item_vote_result(GetUserItemVoteResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoveAppDependencyResult_t> callResultRemoveAppDependency;
	void remove_app_dependency_result(RemoveAppDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoveUGCDependencyResult_t> callResultRemoveUGCDependency;
	void remove_ugc_dependency_result(RemoveUGCDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, SetUserItemVoteResult_t> callResultSetUserItemVote;
	void set_user_item_vote(SetUserItemVoteResult_t *call_data, bool io_failure);
	CCallResult<Steam, StartPlaytimeTrackingResult_t> callResultStartPlaytimeTracking;
	void start_playtime_tracking(StartPlaytimeTrackingResult_t *call_data, bool io_failure);
	CCallResult<Steam, SteamUGCQueryCompleted_t> callResultUGCQueryCompleted;
	void ugc_query_completed(SteamUGCQueryCompleted_t *call_data, bool io_failure);
	CCallResult<Steam, StopPlaytimeTrackingResult_t> callResultStopPlaytimeTracking;
	void stop_playtime_tracking(StopPlaytimeTrackingResult_t *call_data, bool io_failure);
	CCallResult<Steam, SubmitItemUpdateResult_t> callResultItemUpdate;
	void item_updated(SubmitItemUpdateResult_t *call_data, bool io_failure);
	CCallResult<Steam, UserFavoriteItemsListChanged_t> callResultFavoriteItemListChanged;
	void user_favorite_items_list_changed(UserFavoriteItemsListChanged_t *call_data, bool io_failure);
	CCallResult<Steam, WorkshopEULAStatus_t> callResultWorkshopEULAStatus;
	void workshop_eula_status(WorkshopEULAStatus_t *call_data, bool io_failure);

	// User call results ////////////////////
	CCallResult<Steam, DurationControl_t> callResultDurationControl;
	void duration_control(DurationControl_t *call_data, bool io_failure);
	CCallResult<Steam, EncryptedAppTicketResponse_t> callResultEncryptedAppTicketResponse;
	void encrypted_app_ticket_response(EncryptedAppTicketResponse_t *call_data, bool io_failure);
	CCallResult<Steam, SteamServerConnectFailure_t> callResultSteamServerConnectFailure;
	void steam_server_connect_failed(SteamServerConnectFailure_t *call_data, bool io_failure);
	CCallResult<Steam, StoreAuthURLResponse_t> callResultStoreAuthURLResponse;
	void store_auth_url_response(StoreAuthURLResponse_t *call_data, bool io_failure);

	// User stat call results ///////////////
	CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
	void global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *call_data, bool io_failure);
	CCallResult<Steam, GlobalStatsReceived_t> callResultGetGlobalStatsReceived;
	void global_stats_received(GlobalStatsReceived_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
	void leaderboard_find_result(LeaderboardFindResult_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
	void leaderboard_scores_downloaded(LeaderboardScoresDownloaded_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
	void leaderboard_score_uploaded(LeaderboardScoreUploaded_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardUGCSet_t> callResultLeaderboardUGCSet;
	void leaderboard_ugc_set(LeaderboardUGCSet_t *call_data, bool io_failure);
	CCallResult<Steam, NumberOfCurrentPlayers_t> callResultNumberOfCurrentPlayers;
	void number_of_current_players(NumberOfCurrentPlayers_t *call_data, bool io_failure);
	CCallResult<Steam, UserStatsReceived_t> callResultUserStatsReceived;
	void user_stats_received(UserStatsReceived_t *call_data, bool io_failure);

	// Utility call results /////////////////
	CCallResult<Steam, CheckFileSignature_t> callResultCheckFileSignature;
	void check_file_signature(CheckFileSignature_t *call_data, bool io_failure);
};


VARIANT_ENUM_CAST(Steam::AccountType);
VARIANT_ENUM_CAST(Steam::APICallFailure);
VARIANT_ENUM_CAST(Steam::AudioPlaybackStatus);
VARIANT_ENUM_CAST(Steam::AuthSessionResponse);
VARIANT_ENUM_CAST(Steam::AvatarSizes);

VARIANT_ENUM_CAST(Steam::BeginAuthSessionResult);
VARIANT_ENUM_CAST(Steam::BroadcastUploadResult);

VARIANT_ENUM_CAST(Steam::ChatEntryType);
VARIANT_BITFIELD_CAST(Steam::ChatMemberStateChange);
VARIANT_ENUM_CAST(Steam::ChatRoomEnterResponse);
VARIANT_BITFIELD_CAST(Steam::ChatSteamIDInstanceFlags);
VARIANT_ENUM_CAST(Steam::CheckFileSignature);
VARIANT_ENUM_CAST(Steam::CommunityProfileItemType);
VARIANT_ENUM_CAST(Steam::CommunityProfileItemProperty);
VARIANT_ENUM_CAST(Steam::ControllerHapticLocation);
VARIANT_ENUM_CAST(Steam::ControllerHapticType);
VARIANT_ENUM_CAST(Steam::ControllerPad);

VARIANT_ENUM_CAST(Steam::DenyReason);
VARIANT_ENUM_CAST(Steam::DeviceFormFactor);
VARIANT_ENUM_CAST(Steam::DurationControlNotification);
VARIANT_ENUM_CAST(Steam::DurationControlOnlineState);
VARIANT_ENUM_CAST(Steam::DurationControlProgress);

VARIANT_ENUM_CAST(Steam::FilePathType);
VARIANT_ENUM_CAST(Steam::FloatingGamepadTextInputMode);
VARIANT_BITFIELD_CAST(Steam::FriendFlags);
VARIANT_ENUM_CAST(Steam::FriendRelationship);

VARIANT_ENUM_CAST(Steam::GameIDType);
VARIANT_ENUM_CAST(Steam::GamepadTextInputLineMode);
VARIANT_ENUM_CAST(Steam::GamepadTextInputMode);
VARIANT_ENUM_CAST(Steam::GameSearchErrorCode);

VARIANT_BITFIELD_CAST(Steam::HTMLKeyModifiers);
VARIANT_ENUM_CAST(Steam::HTMLMouseButton);
VARIANT_ENUM_CAST(Steam::HTTPMethod);
VARIANT_ENUM_CAST(Steam::HTTPStatusCode);

VARIANT_ENUM_CAST(Steam::InputActionEventType);
VARIANT_ENUM_CAST(Steam::InputActionOrigin);
VARIANT_BITFIELD_CAST(Steam::InputConfigurationEnableType);
VARIANT_ENUM_CAST(Steam::InputGlyphSize);
VARIANT_BITFIELD_CAST(Steam::InputGlyphStyle);
VARIANT_ENUM_CAST(Steam::InputLEDFlag);
VARIANT_ENUM_CAST(Steam::InputSourceMode);
VARIANT_ENUM_CAST(Steam::InputType);
VARIANT_ENUM_CAST(Steam::IPType);
VARIANT_ENUM_CAST(Steam::IPv6ConnectivityProtocol);
VARIANT_ENUM_CAST(Steam::IPv6ConnectivityState);
VARIANT_BITFIELD_CAST(Steam::ItemFlags);
VARIANT_ENUM_CAST(Steam::ItemPreviewType);
VARIANT_BITFIELD_CAST(Steam::ItemState);
VARIANT_ENUM_CAST(Steam::ItemStatistic);
VARIANT_ENUM_CAST(Steam::ItemUpdateStatus);

VARIANT_ENUM_CAST(Steam::LeaderboardDataRequest);
VARIANT_ENUM_CAST(Steam::LeaderboardDisplayType);
VARIANT_ENUM_CAST(Steam::LeaderboardSortMethod);
VARIANT_ENUM_CAST(Steam::LeaderboardUploadScoreMethod);
VARIANT_ENUM_CAST(Steam::LobbyComparison);
VARIANT_ENUM_CAST(Steam::LobbyDistanceFilter);
VARIANT_ENUM_CAST(Steam::LobbyType);
VARIANT_ENUM_CAST(Steam::LocalFileChange);

VARIANT_ENUM_CAST(Steam::MarketNotAllowedReasonFlags);
VARIANT_ENUM_CAST(Steam::MatchMakingServerResponse);
VARIANT_ENUM_CAST(Steam::MouseCursor);

VARIANT_ENUM_CAST(Steam::NetworkingAvailability);
VARIANT_ENUM_CAST(Steam::NetworkingConfigDataType);
VARIANT_ENUM_CAST(Steam::NetworkingConfigScope);
VARIANT_ENUM_CAST(Steam::NetworkingConfigValue);
VARIANT_ENUM_CAST(Steam::NetworkingConnectionEnd);
VARIANT_ENUM_CAST(Steam::NetworkingConnectionState);
VARIANT_ENUM_CAST(Steam::NetworkingFakeIPType);
VARIANT_ENUM_CAST(Steam::NetworkingGetConfigValueResult);
VARIANT_ENUM_CAST(Steam::NetworkingIdentityType);
VARIANT_ENUM_CAST(Steam::NetworkingSocketsDebugOutputType);
VARIANT_ENUM_CAST(Steam::NotificationPosition);

VARIANT_ENUM_CAST(Steam::OverlayToStoreFlag);
VARIANT_ENUM_CAST(Steam::OverlayToWebPageMode);

VARIANT_ENUM_CAST(Steam::P2PSend);
VARIANT_ENUM_CAST(Steam::P2PSessionError);
VARIANT_ENUM_CAST(Steam::ParentalFeature);
VARIANT_ENUM_CAST(Steam::PartyBeaconLocationData);
VARIANT_ENUM_CAST(Steam::PartyBeaconLocationType);
VARIANT_BITFIELD_CAST(Steam::PersonaChange);
VARIANT_ENUM_CAST(Steam::PersonaState);
VARIANT_ENUM_CAST(Steam::PlayerResult);

VARIANT_BITFIELD_CAST(Steam::RemoteStoragePlatform);
VARIANT_ENUM_CAST(Steam::RemoteStoragePublishedFileVisibility);
VARIANT_ENUM_CAST(Steam::Result);

VARIANT_ENUM_CAST(Steam::SCEPadTriggerEffectMode);
VARIANT_ENUM_CAST(Steam::SocketConnectionType);
VARIANT_ENUM_CAST(Steam::SocketState);
VARIANT_ENUM_CAST(Steam::SteamAPIInitResult);

VARIANT_ENUM_CAST(Steam::TextFilteringContext);

VARIANT_ENUM_CAST(Steam::Universe);
VARIANT_ENUM_CAST(Steam::UGCContentDescriptorID);
VARIANT_ENUM_CAST(Steam::UGCMatchingUGCType);
VARIANT_ENUM_CAST(Steam::UGCQuery);
VARIANT_ENUM_CAST(Steam::UGCReadAction);
VARIANT_ENUM_CAST(Steam::UserHasLicenseForAppResult);
VARIANT_BITFIELD_CAST(Steam::UserRestriction);
VARIANT_ENUM_CAST(Steam::UserUGCList);
VARIANT_ENUM_CAST(Steam::UserUGCListSortOrder);

VARIANT_ENUM_CAST(Steam::VoiceResult);
VARIANT_ENUM_CAST(Steam::VRScreenshotType);

VARIANT_ENUM_CAST(Steam::WorkshopEnumerationType);
VARIANT_ENUM_CAST(Steam::WorkshopFileAction);
VARIANT_ENUM_CAST(Steam::WorkshopFileType);
VARIANT_ENUM_CAST(Steam::WorkshopVideoProvider);
VARIANT_ENUM_CAST(Steam::WorkshopVote);

VARIANT_ENUM_CAST(Steam::XboxOrigin);

#endif // GODOTSTEAM_H