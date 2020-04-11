#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H
// Turn off MSVC-only warning about strcpy
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#pragma warning(disable:4996)
#pragma warning(disable:4828)
// Include INT types header
#include <inttypes.h>
// Include Steamworks API header
#include "steam/steam_api.h"
// Include Godot headers
#include "core/object.h"
#include "scene/resources/texture.h"
#include "core/reference.h"
#include "core/dictionary.h"

class Steam: public Object {
	GDCLASS(Steam, Object);

	public:
		// Steam API enums
		enum AccountType {
			AccountTypeInvalid = 0, AccountTypeIndividual = 1, AccountTypeMultiseat = 2, AccountTypeGameServer = 3, AccountTypeAnonGameServer = 4, AccountTypePending = 5, AccountTypeContentServer = 6, AccountTypeClan = 7, AccountTypeChat = 8, AccountTypeConsoleUser = 9, AccountTypeAnonUser = 10, AccountTypeMax = 11
		};
		enum AppOwnershipFlags {
			AppOwnershipFlags_None = 0x0000, AppOwnershipFlags_OwnsLicense = 0x0001, AppOwnershipFlags_FreeLicense = 0x0002, AppOwnershipFlags_RegionRestricted = 0x0004, AppOwnershipFlags_LowViolence = 0x0008, AppOwnershipFlags_InvalidPlatform = 0x0010, AppOwnershipFlags_SharedLicense = 0x0020,
			AppOwnershipFlags_FreeWeekend = 0x0040, AppOwnershipFlags_RetailLicense = 0x0080, AppOwnershipFlags_LicenseLocked = 0x0100, AppOwnershipFlags_LicensePending = 0x0200, AppOwnershipFlags_LicenseExpired = 0x0400, AppOwnershipFlags_LicensePermanent = 0x0800, AppOwnershipFlags_LicenseRecurring = 0x1000,
			AppOwnershipFlags_LicenseCanceled = 0x2000, AppOwnershipFlags_AutoGrant = 0x4000, AppOwnershipFlags_PendingGift = 0x8000, AppOwnershipFlags_RentalNotActivated = 0x10000, AppOwnershipFlags_Rental = 0x20000, AppOwnershipFlags_SiteLicense = 0x40000
		};
		enum AppReleaseState {
			AppReleaseState_Unknown = 0, AppReleaseState_Unavailable = 1, AppReleaseState_Prerelease = 2, AppReleaseState_PreloadOnly = 3, AppReleaseState_Released = 4
		};
		enum AppType {
			AppType_Invalid = 0x000, AppType_Game = 0x001, AppType_Application = 0x002, AppType_Tool = 0x004, AppType_Demo = 0x008, AppType_Media_DEPRECATED = 0x010, AppType_DLC = 0x020, AppType_Guide = 0x040, AppType_Driver = 0x080, AppType_Config = 0x100, AppType_Hardware = 0x200, AppType_Franchise = 0x400,
			AppType_Video = 0x800, AppType_Plugin = 0x1000, AppType_Music = 0x2000, AppType_Series = 0x4000, AppType_Shortcut = 0x40000000, AppType_DepotOnly = 0x80000000
		};
		enum AuthSessionResponse {
			AuthSessionResponseOK = 0, AuthSessionResponseUserNotConnectedToSteam = 1, AuthSessionResponseNoLicenseOrExpired = 2, AuthSessionResponseVACBanned = 3, AuthSessionResponseLoggedInElseWhere = 4, AuthSessionResponseVACCheckTimedOut = 5, AuthSessionResponseAuthTicketCanceled = 6,
			AuthSessionResponseAuthTicketInvalidAlreadyUsed = 7, AuthSessionResponseAuthTicketInvalid = 8, AuthSessionResponsePublisherIssuedBan = 9
		};
		enum BeginAuthSessionResult {
			BeginAuthSessionResultOK = 0, BeginAuthSessionResultInvalidTicket = 1, BeginAuthSessionResultDuplicateRequest = 2, BeginAuthSessionResultInvalidVersion = 3, BeginAuthSessionResultGameMismatch = 4, BeginAuthSessionResultExpiredTicket = 5
		};
		enum BroadcastUploadResult {
			BroadcastUploadResultNone = 0, BroadcastUploadResultOK = 1, BroadcastUploadResultInitFailed = 2, BroadcastUploadResultFrameFailed = 3, BroadcastUploadResultTimeout = 4, BroadcastUploadResultBandwidthExceeded = 5, BroadcastUploadResultLowFPS = 6, BroadcastUploadResultMissingKeyFrames = 7,
			BroadcastUploadResultNoConnection = 8, BroadcastUploadResultRelayFailed = 9, BroadcastUploadResultSettingsChanged = 10, BroadcastUploadResultMissingAudio = 11, BroadcastUploadResultTooFarBehind = 12, BroadcastUploadResultTranscodeBehind = 13
		};
		enum ChatEntryType {
			ChatEntryTypeInvalid = 0, ChatEntryTypeChatMsg = 1, ChatEntryTypeTyping = 2, ChatEntryTypeInviteGame = 3, ChatEntryTypeEmote = 4, ChatEntryTypeLeftConversation = 6, ChatEntryTypeEntered = 7, ChatEntryTypeWasKicked = 8, ChatEntryTypeWasBanned = 9, ChatEntryTypeDisconnected = 10,
			ChatEntryTypeHistoricalChat = 11, ChatEntryTypeLinkBlocked = 14
		};
		enum ChatRoomEnterResponse {
			ChatRoomEnterResponseSuccess = 1, ChatRoomEnterResponseDoesntExist = 2, ChatRoomEnterResponseNotAllowed = 3, ChatRoomEnterResponseFull = 4, ChatRoomEnterResponseError = 5, ChatRoomEnterResponseBanned = 6, ChatRoomEnterResponseLimited = 7, ChatRoomEnterResponseClanDisabled = 8,
			ChatRoomEnterResponseCommunityBan = 9, ChatRoomEnterResponseMemberBlockedYou = 10, ChatRoomEnterResponseYouBlockedMember = 11
		};
		enum ChatSteamIDInstanceFlags {
			ChatAccountInstanceMask = 0x00000FFF, ChatInstanceFlagClan = ((k_unSteamAccountInstanceMask + 1) >> 1), ChatInstanceFlagLobby = ((k_unSteamAccountInstanceMask + 1) >> 2), ChatInstanceFlagMMSLobby = ((k_unSteamAccountInstanceMask + 1) >> 3)
		};
		enum DenyReason {
			DenyInvalid = 0, DenyInvalidVersion = 1, DenyGeneric = 2, DenyNotLoggedOn = 3, DenyNoLicense = 4, DenyCheater = 5, DenyLoggedInElseWhere = 6, DenyUnknownText = 7, DenyIncompatibleAnticheat = 8, DenyMemoryCorruption = 9, DenyIncompatibleSoftware = 10, DenySteamConnectionLost = 11,
			DenySteamConnectionError = 12, DenySteamResponseTimedOut = 13, DenySteamValidationStalled = 14, DenySteamOwnerLeftGuestUser = 15
		};
		enum GameIDType {
			GameTypeApp = 0, GameTypeGameMod = 1, GameTypeShortcut = 2, GameTypeP2P = 3
		};
		enum LaunchOptionType {
			LaunchOptionType_None = 0, LaunchOptionType_Default = 1, LaunchOptionType_SafeMode = 2, LaunchOptionType_Multiplayer = 3, LaunchOptionType_Config = 4, LaunchOptionType_OpenVR = 5, LaunchOptionType_Server = 6, LaunchOptionType_Editor = 7, LaunchOptionType_Manual = 8,
			LaunchOptionType_Benchmark = 9, LaunchOptionType_Option1 = 10, LaunchOptionType_Option2 = 11, LaunchOptionType_Option3 = 12, LaunchOptionType_OculusVR = 13, LaunchOptionType_OpenVROverlay = 14, LaunchOptionType_OSVR = 15, LaunchOptionType_Dialog = 1000
		};
		enum MarketingMessageFlags {
			MarketingMessageFlagsNone = 0, MarketingMessageFlagsHighPriority = (1<<0), MarketingMessageFlagsPlatformWindows = (1<<1), MarketingMessageFlagsPlatformMac = (1<<2), MarketingMessageFlagsPlatformLinux = (1<<3), MarketingMessageFlagsPlatformRestrictions
		};
		enum NotificationPosition {
			PositionTopLeft = 0, PositionTopRight = 1, PositionBottomLeft = 2, PositionBottomRight = 3
		};
		enum Result {
			ResultOK = 1, ResultFail = 2, ResultNoConnection = 3, ResultInvalidPassword = 5, ResultLoggedInElsewhere = 6, ResultInvalidProtocolVer = 7, ResultInvalidParam = 8, ResultFileNotFound = 9, ResultBusy = 10, ResultInvalidState = 11, ResultInvalidName = 12, ResultInvalidEmail = 13,
			ResultDuplicateName = 14, ResultAccessDenied = 15, ResultTimeout = 16, ResultBanned = 17, ResultAccountNotFound = 18, ResultInvalidSteamID = 19, ResultServiceUnavailable = 20, ResultNotLoggedOn = 21, ResultPending = 22, ResultEncryptionFailure = 23, ResultInsufficientPrivilege = 24,
			ResultLimitExceeded = 25, ResultRevoked = 26, ResultExpired = 27, ResultAlreadyRedeemed = 28, ResultDuplicateRequest = 29, ResultAlreadyOwned = 30, ResultIPNotFound = 31, ResultPersistFailed = 32, ResultLockingFailed = 33, ResultLogonSessionReplaced = 34, ResultConnectFailed = 35,
			ResultHandshakeFailed = 36, ResultIOFailure = 37, ResultRemoteDisconnect = 38, ResultShoppingCartNotFound = 39, ResultBlocked = 40, ResultIgnored = 41, ResultNoMatch = 42, ResultAccountDisabled = 43, ResultServiceReadOnly = 44, ResultAccountNotFeatured = 45, ResultAdministratorOK = 46,
			ResultContentVersion = 47, ResultTryAnotherCM = 48, ResultPasswordRequiredToKickSession = 49, ResultAlreadyLoggedInElsewhere = 50, ResultSuspended = 51, ResultCancelled = 52, ResultDataCorruption = 53, ResultDiskFull = 54, ResultRemoteCallFailed = 55, ResultPasswordUnset = 56,
			ResultExternalAccountUnlinked = 57, ResultPSNTicketInvalid = 58, ResultExternalAccountAlreadyLinked = 59, ResultRemoteFileConflict = 60, ResultIllegalPassword = 61, ResultSameAsPreviousValue = 62, ResultAccountLogonDenied = 63, ResultCannotUseOldPassword = 64, ResultInvalidLoginAuthCode = 65,
			ResultAccountLogonDeniedNoMail = 66, ResultHardwareNotCapableOfIPT = 67, ResultIPTInitError = 68, ResultParentalControlRestricted = 69, ResultFacebookQueryError = 70, ResultExpiredLoginAuthCode = 71, ResultIPLoginRestrictionFailed = 72, ResultAccountLockedDown = 73,
			ResultAccountLogonDeniedVerifiedEmailRequired = 74, ResultNoMatchingURL = 75, ResultBadResponse = 76, ResultRequirePasswordReEntry = 77, ResultValueOutOfRange = 78, ResultUnexpectedError = 79, ResultDisabled = 80, ResultInvalidCEGSubmission = 81, ResultRestrictedDevice = 82,
			ResultRegionLocked = 83, ResultRateLimitExceeded = 84, ResultAccountLoginDeniedNeedTwoFactor = 85, ResultItemDeleted = 86, ResultAccountLoginDeniedThrottle = 87, ResultTwoFactorCodeMismatch = 88, ResultTwoFactorActivationCodeMismatch = 89, ResultAccountAssociatedToMultiplePartners = 90,
			ResultNotModified = 91, ResultNoMobileDevice = 92, ResultTimeNotSynced = 93, ResultSmsCodeFailed = 94, ResultAccountLimitExceeded = 95, ResultAccountActivityLimitExceeded = 96, ResultPhoneActivityLimitExceeded = 97, ResultRefundToWallet = 98, ResultEmailSendFailure = 99,
			ResultNotSettled = 100, ResultNeedCaptcha = 101, ResultGSLTDenied = 102, ResultGSOwnerDenied = 103, ResultInvalidItemType = 104, ResultIPBanned = 105, ResultGSLTExpired = 106, ResultInsufficientFunds = 107, ResultTooManyPending = 108
		};
		enum SteamUserStatType {
			SteamUserStatTypeINVALID = 0, SteamUserStatTypeINT = 1, SteamUserStatTypeFLOAT = 2, SteamUserStatTypeAVGRATE = 3, SteamUserStatTypeACHIEVEMENTS = 4, SteamUserStatTypeGROUPACHIEVEMENTS = 5, SteamUserStatTypeMAX = 6
		};
		enum Universe {
			UniverseInvalid = 0, UniversePublic = 1, UniverseBeta = 2, UniverseInternal = 3, UniverseDev = 4, UniverseMax = 5
		};
		enum UserHasLicenseForAppResult {
			UserHasLicenseResultHasLicense = 0, UserHasLicenseResultDoesNotHaveLicense = 1, UserHasLicenseResultNoAuth = 2
		};
		enum VoiceResult {
			VoiceResultOK = 0, VoiceResultNotInitialized = 1, VoiceResultNotRecording = 2, VoiceResultNoDate = 3, VoiceResultBufferTooSmall = 4, VoiceResultDataCorrupted = 5, VoiceResultRestricted = 6
		};
		enum VRHMDType {
			VRHMDType_None = -1, VRHMDType_Unknown = 0, VRHMDType_HTC_Dev = 1, VRHMDType_HTC_VivePre = 2, VRHMDType_HTC_Vive = 3, VRHMDType_HTC_Unknown = 20, VRHMDType_Oculus_DK1 = 21, VRHMDType_Oculus_DK2 = 22, VRHMDType_Oculus_Rift = 23, VRHMDType_Oculus_Unknown = 40
		};
		// Friends enums
		enum FriendFlags {
			FriendFlagNone = 0x00, FriendFlagBlocked = 0x01, FriendFlagFriendshipRequested = 0x02, FriendFlagImmediate = 0x04, FriendFlagClanMember = 0x08, FriendFlagOnGameServer = 0x10, FriendFlagRequestingFriendship = 0x80, FriendFlagRequestingInfo = 0x100,
			FriendFlagIgnored = 0x200, FriendFlagIgnoredFriend = 0x400, FriendFlagChatMember = 0x1000, FriendFlagAll = 0xFFFF
		};
		enum FriendRelationship {
			FriendRelationNone = 0, FriendRelationBlocked = 1, FriendRelationRequestRecipient = 2, FriendRelationFriend = 3, FriendRelationRequestInitiator = 4,FriendRelationIgnored = 5, FriendRelationIgnoredFriend = 6, FriendRelationSuggested = 7, FriendRelationMax = 8			
		};
		enum OverlayToStoreFlag {
			OverlayToStoreFlag_None = 0, OverlayToStoreFlag_AddToCart = 1, OverlayToStoreFlag_AndToCartAndShow = 2
		};
		enum PersonaChange {
			PersonaChangeName = 0x0001, PersonaChangeStatus = 0x0002, PersonaChangeComeOnline = 0x0004, PersonaChangeGoneOffline = 0x0008, PersonaChangeGamePlayed = 0x0010, PersonaChangeGameServer = 0x0020, PersonaChangeAvatar = 0x0040, PersonaChangeJoinedSource = 0x0080,
			PersonaChangeLeftSource = 0x0100, PersonaChangeRelationshipChanged = 0x0200, PersonaChangeNameFirstSet = 0x0400, PersonaChangeFacebookInfo = 0x0800, PersonaChangeNickname = 0x1000, PersonaChangeSteamLevel = 0x2000
		};
		enum PersonaState {
			PersonaStateOffline = 0, PersonaStateOnline = 1, PersonaStateBusy = 2, PersonaStateAway = 3, PersonaStateSnooze = 4, PersonaStateLookingToTrade = 5, PersonaStateLookingToPlay = 6, PersonaStateMax = 7
		};
		enum UserRestriction {
			UserRestrictionNone = 0, UserRestrictionUnknown = 1, UserRestrictionAnyChat = 2, UserRestrictionVoiceChat = 4, UserRestrictionGroupChat = 8, UserRestrictionRating = 16, UserRestrictionGameInvites = 32, UserRestrictionTrading = 64
		};
		// HTMLSurface enums
		enum HTMLKeyModifiers {
			HTMLKeyModifier_None = 0, HTMLKeyModifier_AltDown = (1<<0), HTMLKeyModifier_CtrlDown = (1<<1), HTMLKeyModifier_ShiftDown = (1<<2)
		};
		enum HTMLMouseButton {
			HTMLMouseButton_Left = 0, HTMLMouseButton_Right = 1, HTMLMouseButton_Middle = 2
		};
		enum MouseCursor {
			dc_user = 0, dc_none = 1, dc_arrow = 2, dc_ibeam = 3, dc_hourglass = 4, dc_waitarrow = 5, dc_crosshair = 6, dc_up = 7, dc_sizenw = 8, dc_sizese = 9, dc_sizene = 10, dc_sizesw = 11, dc_size2 = 12, dc_sizee = 13, dc_sizen = 14, dc_sizes = 15, dc_sizewe = 16, dc_sizens = 17,
			dc_sizeall = 18, dc_no = 19, dc_hand = 20, dc_blank = 21, dc_middle_pan = 22, dc_north_pan = 23, dc_north_east_pan = 24, dc_east_pan = 25, dc_south_east_pan = 26, dc_south_pan = 27, dc_south_west_pan = 28, dc_west_pan = 29, dc_north_west_pan = 30, dc_alias = 31, dc_cell = 32, dc_colresize = 33,
			dc_copycur = 34, dc_verticaltext = 35, dc_rowresize = 36, dc_zoomin = 37, dc_zoomout = 38, dc_help = 39, dc_custom = 40, dc_last = 41
		};
		// HTTP enums
		enum HTTPMethod {
			HTTPMethodInvalid = 0, HTTPMethodGET = 1, HTTPMethodHEAD = 2, HTTPMethodPOST = 3, HTTPMethodPUT = 4, HTTPMethodDELETE = 5, HTTPMethodOPTIONS = 6, HTTPMethodPATCH = 7
		};
		enum HTTPStatusCode {
			HTTPStatusCodeInvalid = 0, HTTPStatusCode100Continue = 100, HTTPStatusCode101SwitchingProtocols = 101, HTTPStatusCode200OK = 200, HTTPStatusCode201Created = 201, HTTPStatusCode202Accepted = 202, HTTPStatusCode203NonAuthoritative = 203, HTTPStatusCode204NoContent = 204,
			HTTPStatusCode205ResetContent = 205, HTTPStatusCode206PartialContent = 206, HTTPStatusCode300MultipleChoices = 300, HTTPStatusCode301MovedPermanently = 301, HTTPStatusCode302Found = 302, HTTPStatusCode303SeeOther = 303, HTTPStatusCode304NotModified = 304,
			HTTPStatusCode305UseProxy = 305, HTTPStatusCode307TemporaryRedirect = 307, HTTPStatusCode400BadRequest = 400, HTTPStatusCode401Unauthorized = 401, HTTPStatusCode402PaymentRequired = 402, HTTPStatusCode403Forbidden = 403, HTTPStatusCode404NotFound = 404,
			HTTPStatusCode405MethodNotAllowed = 405, HTTPStatusCode406NotAcceptable = 406, HTTPStatusCode407ProxyAuthRequired = 407, HTTPStatusCode408RequestTimeout = 408, HTTPStatusCode409Conflict = 409, HTTPStatusCode410Gone = 410, HTTPStatusCode411LengthRequired = 411,
			HTTPStatusCode412PreconditionFailed = 412, HTTPStatusCode413RequestEntityTooLarge = 413, HTTPStatusCode414RequestURITooLong = 414, HTTPStatusCode415UnsupportedMediaType = 415, HTTPStatusCode416RequestedRangeNotSatisfiable = 416, HTTPStatusCode417ExpectationFailed = 417,
			HTTPStatusCode4xxUnknown = 418, HTTPStatusCode429TooManyRequests = 429, HTTPStatusCode500InternalServerError = 500, HTTPStatusCode501NotImplemented = 501, HTTPStatusCode502BadGateway = 502, HTTPStatusCode503ServiceUnavailable = 503, HTTPStatusCode504GatewayTimeout = 504,
			HTTPStatusCode505HTTPVersionNotSupported = 505, HTTPStatusCode5xxUnknown = 599
		};
		// Input enums
		enum InputActionOrigin {
			InputActionOrigin_None = 0, InputActionOrigin_A = 1, InputActionOrigin_B = 2, InputActionOrigin_X = 3, InputActionOrigin_Y = 4, InputActionOrigin_LeftBumper = 5, InputActionOrigin_RightBumper = 6, InputActionOrigin_LeftGrip = 7, InputActionOrigin_RightGrip = 8,
			InputActionOrigin_Start = 9, InputActionOrigin_Back = 10, InputActionOrigin_LeftPad_Touch = 11, InputActionOrigin_LeftPad_Swipe = 12, InputActionOrigin_LeftPad_Click = 13, InputActionOrigin_LeftPad_DPadNorth = 14, InputActionOrigin_LeftPad_DPadSouth = 15,
			InputActionOrigin_LeftPad_DPadWest = 16, InputActionOrigin_LeftPad_DPadEast = 17, InputActionOrigin_RightPad_Touch = 18, InputActionOrigin_RightPad_Swipe = 19, InputActionOrigin_RightPad_Click = 20, InputActionOrigin_RightPad_DPadNorth = 21, InputActionOrigin_RightPad_DPadSouth = 22,
			InputActionOrigin_RightPad_DPadWest = 23, InputActionOrigin_RightPad_DPadEast = 24, InputActionOrigin_LeftTrigger_Pull = 25, InputActionOrigin_LeftTrigger_Click = 26, InputActionOrigin_RightTrigger_Pull = 27, InputActionOrigin_RightTrigger_Click = 28,
			InputActionOrigin_LeftStick_Move = 29, InputActionOrigin_LeftStick_Click = 30, InputActionOrigin_LeftStick_DPadNorth = 31, InputActionOrigin_LeftStick_DPadSouth = 32, InputActionOrigin_LeftStick_DPadWest = 33, InputActionOrigin_LeftStick_DPadEast = 34, InputActionOrigin_Gyro_Move = 35,
			InputActionOrigin_Gyro_Pitch = 36, InputActionOrigin_Gyro_Yaw = 37, InputActionOrigin_Gyro_Roll = 38, InputActionOrigin_SteamController_Reserved0 = 39, InputActionOrigin_SteamController_Reserved1 = 40, InputActionOrigin_SteamController_Reserved2 = 41,InputActionOrigin_SteamController_Reserved3 = 42,
			InputActionOrigin_SteamController_Reserved4 = 43, InputActionOrigin_SteamController_Reserved5 = 44, InputActionOrigin_SteamController_Reserved6 = 45, InputActionOrigin_SteamController_Reserved7 = 46, InputActionOrigin_SteamController_Reserved8 = 47, InputActionOrigin_SteamController_Reserved9 = 48,
			InputActionOrigin_SteamController_Reserved10 = 49, InputActionOrigin_PS4_X = 50, InputActionOrigin_PS4_Circle = 51, InputActionOrigin_PS4_Triangle = 52, InputActionOrigin_PS4_Square = 53, InputActionOrigin_PS4_LeftBumper = 54, InputActionOrigin_PS4_RightBumper = 55,
			InputActionOrigin_PS4_Options = 56, InputActionOrigin_PS4_Share = 57, InputActionOrigin_PS4_LeftPad_Touch = 58, InputActionOrigin_PS4_LeftPad_Swipe = 59, InputActionOrigin_PS4_LeftPad_Click = 60, InputActionOrigin_PS4_LeftPad_DPadNorth = 61, InputActionOrigin_PS4_LeftPad_DPadSouth = 62,
			InputActionOrigin_PS4_LeftPad_DPadWest = 63, InputActionOrigin_PS4_LeftPad_DPadEast = 64, InputActionOrigin_PS4_RightPad_Touch = 65, InputActionOrigin_PS4_RightPad_Swipe = 66, InputActionOrigin_PS4_RightPad_Click = 67, InputActionOrigin_PS4_RightPad_DPadNorth = 68,
			InputActionOrigin_PS4_RightPad_DPadSouth = 69, InputActionOrigin_PS4_RightPad_DPadWest = 70, InputActionOrigin_PS4_RightPad_DPadEast = 71, InputActionOrigin_PS4_CenterPad_Touch = 72, InputActionOrigin_PS4_CenterPad_Swipe = 73, InputActionOrigin_PS4_CenterPad_Click = 74,
			InputActionOrigin_PS4_CenterPad_DPadNorth = 75, InputActionOrigin_PS4_CenterPad_DPadSouth = 76, InputActionOrigin_PS4_CenterPad_DPadWest = 77, InputActionOrigin_PS4_CenterPad_DPadEast = 78, InputActionOrigin_PS4_LeftTrigger_Pull = 79, InputActionOrigin_PS4_LeftTrigger_Click = 80,
			InputActionOrigin_PS4_RightTrigger_Pull = 81, InputActionOrigin_PS4_RightTrigger_Click = 82, InputActionOrigin_PS4_LeftStick_Move = 83, InputActionOrigin_PS4_LeftStick_Click = 84, InputActionOrigin_PS4_LeftStick_DPadNorth = 85, InputActionOrigin_PS4_LeftStick_DPadSouth = 86,
			InputActionOrigin_PS4_LeftStick_DPadWest = 87, InputActionOrigin_PS4_LeftStick_DPadEast = 88, InputActionOrigin_PS4_RightStick_Move = 89, InputActionOrigin_PS4_RightStick_Click = 90, InputActionOrigin_PS4_RightStick_DPadNorth = 91, InputActionOrigin_PS4_RightStick_DPadSouth = 92,
			InputActionOrigin_PS4_RightStick_DPadWest = 93, InputActionOrigin_PS4_RightStick_DPadEast = 94, InputActionOrigin_PS4_DPad_North = 95, InputActionOrigin_PS4_DPad_South = 96, InputActionOrigin_PS4_DPad_West = 97, InputActionOrigin_PS4_DPad_East = 98, InputActionOrigin_PS4_Gyro_Move = 99,
			InputActionOrigin_PS4_Gyro_Pitch = 100, InputActionOrigin_PS4_Gyro_Yaw = 101, InputActionOrigin_PS4_Gyro_Roll = 102, InputActionOrigin_PS4_Reserved0 = 103, InputActionOrigin_PS4_Reserved1 = 104, InputActionOrigin_PS4_Reserved2 = 105, InputActionOrigin_PS4_Reserved3 = 106,
			InputActionOrigin_PS4_Reserved4 = 107, InputActionOrigin_PS4_Reserved5 = 108, InputActionOrigin_PS4_Reserved6 = 109, InputActionOrigin_PS4_Reserved7 = 110, InputActionOrigin_PS4_Reserved8 = 111, InputActionOrigin_PS4_Reserved9 = 112, InputActionOrigin_PS4_Reserved10 = 113,
			InputActionOrigin_XBoxOne_A = 114, InputActionOrigin_XBoxOne_B = 115, InputActionOrigin_XBoxOne_X = 116, InputActionOrigin_XBoxOne_Y = 117, InputActionOrigin_XBoxOne_LeftBumper = 118, InputActionOrigin_XBoxOne_RightBumper = 119, InputActionOrigin_XBoxOne_Menu = 120,
			InputActionOrigin_XBoxOne_View = 121, InputActionOrigin_XBoxOne_LeftTrigger_Pull = 122, InputActionOrigin_XBoxOne_LeftTrigger_Click = 123, InputActionOrigin_XBoxOne_RightTrigger_Pull = 124, InputActionOrigin_XBoxOne_RightTrigger_Click = 125, InputActionOrigin_XBoxOne_LeftStick_Move = 126, 
			InputActionOrigin_XBoxOne_LeftStick_Click = 127, InputActionOrigin_XBoxOne_LeftStick_DPadNorth = 128, InputActionOrigin_XBoxOne_LeftStick_DPadSouth = 129, InputActionOrigin_XBoxOne_LeftStick_DPadWest = 130, InputActionOrigin_XBoxOne_LeftStick_DPadEast = 131, InputActionOrigin_XBoxOne_RightStick_Move = 132,
			InputActionOrigin_XBoxOne_RightStick_Click = 133, InputActionOrigin_XBoxOne_RightStick_DPadNorth = 134, InputActionOrigin_XBoxOne_RightStick_DPadSouth = 135, InputActionOrigin_XBoxOne_RightStick_DPadWest = 136, InputActionOrigin_XBoxOne_RightStick_DPadEast = 137, InputActionOrigin_XBoxOne_DPad_North = 138,
			InputActionOrigin_XBoxOne_DPad_South = 139, InputActionOrigin_XBoxOne_DPad_West = 140, InputActionOrigin_XBoxOne_DPad_East = 141, InputActionOrigin_XBoxOne_Reserved0 = 142, InputActionOrigin_XBoxOne_Reserved1 = 143, InputActionOrigin_XBoxOne_Reserved2 = 144, InputActionOrigin_XBoxOne_Reserved3 = 145,
			InputActionOrigin_XBoxOne_Reserved4 = 146, InputActionOrigin_XBoxOne_Reserved5 = 147, InputActionOrigin_XBoxOne_Reserved6 = 148, InputActionOrigin_XBoxOne_Reserved7 = 149, InputActionOrigin_XBoxOne_Reserved8 = 150, InputActionOrigin_XBoxOne_Reserved9 = 151, InputActionOrigin_XBoxOne_Reserved10 = 152,
			InputActionOrigin_XBox360_A = 153, InputActionOrigin_XBox360_B = 154, InputActionOrigin_XBox360_X = 155, InputActionOrigin_XBox360_Y = 156, InputActionOrigin_XBox360_LeftBumper = 157, InputActionOrigin_XBox360_RightBumper = 158, InputActionOrigin_XBox360_Start = 159, InputActionOrigin_XBox360_Back = 160,
			InputActionOrigin_XBox360_LeftTrigger_Pull = 161, InputActionOrigin_XBox360_LeftTrigger_Click = 162, InputActionOrigin_XBox360_RightTrigger_Pull = 163, InputActionOrigin_XBox360_RightTrigger_Click = 164, InputActionOrigin_XBox360_LeftStick_Move = 165, InputActionOrigin_XBox360_LeftStick_Click = 166,
			InputActionOrigin_XBox360_LeftStick_DPadNorth = 167, InputActionOrigin_XBox360_LeftStick_DPadSouth = 168, InputActionOrigin_XBox360_LeftStick_DPadWest = 169, InputActionOrigin_XBox360_LeftStick_DPadEast = 170, InputActionOrigin_XBox360_RightStick_Move = 171, InputActionOrigin_XBox360_RightStick_Click = 172,
			InputActionOrigin_XBox360_RightStick_DPadNorth = 173, InputActionOrigin_XBox360_RightStick_DPadSouth = 174, InputActionOrigin_XBox360_RightStick_DPadWest = 175, InputActionOrigin_XBox360_RightStick_DPadEast = 176, InputActionOrigin_XBox360_DPad_North = 177, InputActionOrigin_XBox360_DPad_South = 178,
			InputActionOrigin_XBox360_DPad_West = 179, InputActionOrigin_XBox360_DPad_East = 180, InputActionOrigin_XBox360_Reserved0 = 181, InputActionOrigin_XBox360_Reserved1 = 182, InputActionOrigin_XBox360_Reserved2 = 183, InputActionOrigin_XBox360_Reserved3 = 184, InputActionOrigin_XBox360_Reserved4 = 185,
			InputActionOrigin_XBox360_Reserved5 = 186, InputActionOrigin_XBox360_Reserved6 = 187, InputActionOrigin_XBox360_Reserved7 = 188, InputActionOrigin_XBox360_Reserved8 = 189, InputActionOrigin_XBox360_Reserved9 = 190, InputActionOrigin_XBox360_Reserved10 = 191, InputActionOrigin_Switch_A = 192,
			InputActionOrigin_Switch_B = 193, InputActionOrigin_Switch_X = 194, InputActionOrigin_Switch_Y = 195, InputActionOrigin_Switch_LeftBumper = 196, InputActionOrigin_Switch_RightBumper = 197, InputActionOrigin_Switch_Plus = 198, InputActionOrigin_Switch_Minus = 199, InputActionOrigin_Switch_Capture = 200,
			InputActionOrigin_Switch_LeftTrigger_Pull = 201, InputActionOrigin_Switch_LeftTrigger_Click = 202, InputActionOrigin_Switch_RightTrigger_Pull = 203, InputActionOrigin_Switch_RightTrigger_Click = 204, InputActionOrigin_Switch_LeftStick_Move = 205, InputActionOrigin_Switch_LeftStick_Click = 206,
			InputActionOrigin_Switch_LeftStick_DPadNorth = 207, InputActionOrigin_Switch_LeftStick_DPadSouth = 208, InputActionOrigin_Switch_LeftStick_DPadWest = 209, InputActionOrigin_Switch_LeftStick_DPadEast = 210, InputActionOrigin_Switch_RightStick_Move = 211, InputActionOrigin_Switch_RightStick_Click = 212,
			InputActionOrigin_Switch_RightStick_DPadNorth = 213, InputActionOrigin_Switch_RightStick_DPadSouth = 214, InputActionOrigin_Switch_RightStick_DPadWest = 215, InputActionOrigin_Switch_RightStick_DPadEast = 216, InputActionOrigin_Switch_DPad_North = 217, InputActionOrigin_Switch_DPad_South = 218,
			InputActionOrigin_Switch_DPad_West = 219, InputActionOrigin_Switch_DPad_East = 220, InputActionOrigin_SwitchProGyro_Move = 221, InputActionOrigin_SwitchProGyro_Pitch = 222, InputActionOrigin_SwitchProGyro_Yaw = 223, InputActionOrigin_SwitchProGyro_Roll = 224, InputActionOrigin_Switch_Reserved0 = 225,
			InputActionOrigin_Switch_Reserved1 = 226, InputActionOrigin_Switch_Reserved2 = 227, InputActionOrigin_Switch_Reserved3 = 228, InputActionOrigin_Switch_Reserved4 = 229, InputActionOrigin_Switch_Reserved5 = 230, InputActionOrigin_Switch_Reserved6 = 231, InputActionOrigin_Switch_Reserved7 = 232,
			InputActionOrigin_Switch_Reserved8 = 233, InputActionOrigin_Switch_Reserved9 = 234, InputActionOrigin_Switch_Reserved10 = 235, InputActionOrigin_Count = 258, InputActionOrigin_MaximumPossibleValue = 32767
		};
		// Inventory enums
		enum SteamItemFlags {
			SteamItemNoTrade = (1<<0), SteamItemRemoved = (1<<8), SteamItemConsumed = (1<<9)
		};
		// Matchmaking enums
		enum ChatMemberStateChange {
			ChatMemberStateChangeEntered = 0x0001, ChatMemberStateChangeLeft = 0x0002, ChatMemberStateChangeDisconnected = 0x0004, ChatMemberStateChangeKicked = 0x0008, ChatMemberStateChangeBanned = 0x0010
		};
		enum LobbyComparison {
			LobbyComparisonEqualToOrLessThan = -2, LobbyComparisonLessThan = -1, LobbyComparisonEqual = 0, LobbyComparisonGreaterThan = 1, LobbyComparisonEqualToGreaterThan = 2, LobbyComparisonNotEqual = 3
		};
		enum LobbyDistanceFilter {
			LobbyDistanceFilterClose = 0, LobbyDistanceFilterDefault = 1, LobbyDistanceFilterFar = 2, LobbyDistanceFilterWorldwide = 3
		};
		enum LobbyType {
			LobbyTypePrivate = 0, LobbyTypeFriendsOnly = 1, LobbyTypePublic = 2, LobbyTypeInvisible = 3
		};
		// Matchmaking Servers enums
		enum MatchMakingServerResponse {
			ServerResponded = 0, ServerFaileToRespond = 1, NoServersListedOnMasterServer = 2
		};
		// Music enums
		enum AudioPlayback_Status {
			AudioPlayback_Undefined = 0, AudioPlayback_Playing = 1, AudioPlayback_Paused = 2, AudioPlayback_Idle = 3
		};
		// Networking enums
		enum P2PSend {
			P2PSendUnreliable = 0, P2PSendUnreliableNoDelay = 1, P2PSendReliable = 2, P2PSendReliableWithBuffering = 3
		};
		enum P2PSessionError {
			P2PSessionErrorNone = 0, P2PSessionErrorNotRunningApp = 1, P2PSessionErrorNoRightsToApp = 2, P2PSessionErrorDestinationNotLoggedOn = 3, P2PSessionErrorTimeout = 4, P2PSessionErrorMax = 5
		};
		enum SNetSocketConnectionType {
			NetSocketConnectionTypeNotConnected = 0, NetSocketConnectionTypeUDP = 1, NetSocketConnectionTypeUDPRelay = 2
		};
		enum SNetSocketState {
			NetSocketStateInvalid = 0, NetSocketStateConnected = 1, NetSocketStateInitiated = 10, NetSocketStateLocalCandidateFound = 11, NetSocketStateReceivedRemoteCandidates = 12, NetSocketStateChallengeHandshake = 15, NetSocketStateDisconnecting = 21, NetSocketStateLocalDisconnect = 22,
			NetSocketStateTimeoutDuringConnect = 23, NetSocketStateRemoteEndDisconnected = 24, NetSocketStateBroken = 25
		};
		// Steam Parties enums
		enum SteamPartyBeaconLocationType {
			SteamPartyBeaconLocationType_Invalid = 0, SteamPartyBeaconLocationType_ChatGroup = 1, SteamPartyBeaconLocationType_Max
		};
		enum SteamPartyBeaconLocationData {
			SteamPartyBeaconLocationData = 0, SteamPartyBeaconLocationDataName = 1, SteamPartyBeaconLocationDataURLSmall, SteamPartyBeaconLocationDataURLMedium, SteamPartyBeaconLocationDataURLLarge
		};
		// Remote Storage enums
		enum RemoteStoragePlatform {
			RemoteStoragePlatformNone = 0, RemoteStoragePlatformWindows = (1<<0), RemoteStoragePlatformOSX = (1<<1), RemoteStoragePlatformPS3 = (1<<2), RemoteStoragePlatformLinux = (1<<3), RemoteStoragePlatformReserved2 = (1<<4), RemoteStoragePlatformAll = 0xffffffff
		};
		enum RemoteStoragePublishedFileVisibility {
			RemoteStoragePublishedVisiblityPublic = 0, RemoteStoragePublishedVisiblityFriendsOnly = 1, RemoteStoragePublishedVisiblityPrivate = 2
		};
		enum UGCReadAction {
			UGCRead_ContinueReadingUntilFinished = 0, UGCRead_ContinueReading = 1, UGCRead_Close = 2
		};
		enum WorkshopEnumerationType {
			WorkshopEnumerationTypeRankedByVote = 0, WorkshopEnumerationTypeRecent = 1, WorkshopEnumerationTypeTrending = 2, WorkshopEnumerationTypeFavoritesOfFriends = 3, WorkshopEnumerationTypeVotedByFriends = 4, WorkshopEnumerationTypeContentByFriends = 5, WorkshopEnumerationTypeRecentFromFollowedUsers = 6
		};
		enum WorkshopFileAction {
			WorkshopFileActionPlayed = 0, WorkshopFileActionCompleted = 1
		};
		enum WorkshopFileType {
			WorkshopFileTypeFirst = 0, WorkshopFileTypeCommunity = 0, WorkshopFileTypeMicrotransaction = 1, WorkshopFileTypeCollection = 2, WorkshopFileTypeArt = 3, WorkshopFileTypeVideo = 4, WorkshopFileTypeScreenshot = 5, WorkshopFileTypeGame = 6, WorkshopFileTypeSoftware = 7, WorkshopFileTypeConcept = 8,
			WorkshopFileTypeWebGuide = 9, WorkshopFileTypeIntegratedGuide = 10, WorkshopFileTypeMerch = 11, WorkshopFileTypeControllerBinding = 12, WorkshopFileTypeSteamworksAccessInvite = 13, WorkshopFileTypeSteamVideo = 14, WorkshopFileTypeGameManagedItem = 15, WorkshopFileTypeMax = 16
		};
		enum WorkshopVideoProvider {
			WorkshopVideoProviderNone = 0, WorkshopVideoProviderYoutube = 1
		};
		enum WorkshopVote {
			WorkshopVoteUnvoted = 0, WorkshopVoteFor = 1, WorkshopVoteAgainst = 2, WorkshopVoteLater = 3
		};
		// Screenshot enums
		enum VRScreenshotType {
			VRScreenshotType_None = 0, VRScreenshotType_Mono = 1, VRScreenshotType_Stereo = 2, VRScreenshotType_MonoCubemap = 3, VRScreenshotType_MonoPanorama = 4, VRScreenshotType_StereoPanorama = 5
		};
		// UGC enums
		enum ItemPreviewType {
			ItemPreviewType_Image = 0, ItemPreviewType_YouTubeVideo = 1, ItemPreviewType_Sketchfab = 2, ItemPreviewType_EnvironmentMap_HorizontalCross = 3, ItemPreviewType_EnvironmentMap_LatLong = 4, ItemPreviewType_ReservedMax = 255
		};
		enum ItemState {
			ItemStateNone = 0, ItemStateSubscribed = 1, ItemStateLegacyItem = 2, ItemStateInstalled = 4, ItemStateNeedsUpdate = 8, ItemStateDownloading = 16, ItemStateDownloadPending = 32
		};
		enum ItemStatistic {
			ItemStatistic_NumSubscriptions = 0, ItemStatistic_NumFavorites = 1, ItemStatistic_NumFollowers = 2, ItemStatistic_NumUniqueSubscriptions = 3, ItemStatistic_NumUniqueFavorites = 4, ItemStatistic_NumUniqueFollowers = 5, ItemStatistic_NumUniqueWebsiteViews = 6, ItemStatistic_ReportScore = 7,
			ItemStatistic_NumSecondsPlayed = 8, ItemStatistic_NumPlaytimeSessions = 9, ItemStatistic_NumComments = 10, ItemStatistic_NumSecondsPlayedDuringTimePeriod = 11, ItemStatistic_NumPlaytimeSessionsDuringTimePeriod = 12
		};
		enum ItemUpdateStatus {
			ItemUpdateStatusInvalid = 0, ItemUpdateStatusPreparingConfig = 1, ItemUpdateStatusPreparingContent = 2, ItemUpdateStatusUploadingContent = 3, ItemUpdateStatusUploadingPreviewFile = 4, ItemUpdateStatusCommittingChanges = 5
		};
		enum UGCMatchingUGCType {
			UGCMatchingUGCType_Items = 0, UGCMatchingUGCType_Items_Mtx = 1, UGCMatchingUGCType_Items_ReadyToUse = 2, UGCMatchingUGCType_Collections = 3, UGCMatchingUGCType_Artwork = 4, UGCMatchingUGCType_Videos = 5, UGCMatchingUGCType_Screenshots = 6, UGCMatchingUGCType_AllGuides = 7,
			UGCMatchingUGCType_WebGuides = 8, UGCMatchingUGCType_IntegratedGuides = 9, UGCMatchingUGCType_UsableInGame = 10, UGCMatchingUGCType_ControllerBindings = 11, UGCMatchingUGCType_GameManagedItems = 12, UGCMatchingUGCType_All = ~0
		};
		enum UGCQuery {
			UGCQuery_RankedByVote = 0, UGCQuery_RankedByPublicationDate = 1, UGCQuery_AcceptedForGameRankedByAcceptanceDate = 2, UGCQuery_RankedByTrend = 3, UGCQuery_FavoritedByFriendsRankedByPublicationDate = 4, UGCQuery_CreatedByFriendsRankedByPublicationDate = 5, UGCQuery_RankedByNumTimesReported = 6,
			UGCQuery_CreatedByFollowedUsersRankedByPublicationDate = 7, UGCQuery_NotYetRated = 8, UGCQuery_RankedByTotalVotesAsc = 9, UGCQuery_RankedByVotesUp = 10, UGCQuery_RankedByTextSearch = 11, UGCQuery_RankedByTotalUniqueSubscriptions = 12, UGCQuery_RankedByPlaytimeTrend = 13,
			UGCQuery_RankedByTotalPlaytime = 14, UGCQuery_RankedByAveragePlaytimeTrend = 15, UGCQuery_RankedByLifetimeAveragePlaytime = 16, UGCQuery_RankedByPlaytimeSessionsTrend = 17, UGCQuery_RankedByLifetimePlaytimeSessions = 18
		};
		enum UserUGCList {
			UserUGCList_Published = 0, UserUGCList_VotedOn = 1, UserUGCList_VotedUp = 2, UserUGCList_VotedDown = 3, UserUGCList_Favorited = 5, UserUGCList_Subscribed = 6, UserUGCList_UsedOrPlayed = 7, UserUGCList_Followed = 8
		};
		enum UserUGCListSortOrder {
			UserUGCListSortOrder_CreationOrderDesc = 0, UserUGCListSortOrder_CreationOrderAsc = 1, UserUGCListSortOrder_TitleAsc = 2, UserUGCListSortOrder_LastUpdatedDesc = 3, UserUGCListSortOrder_SubscriptionDateDesc = 4, UserUGCListSortOrder_VoteScoreDesc = 5, UserUGCListSortOrder_ForModeration = 6
		};
		// User enums
		enum FailureType {
			FailureFlushedCallbackQueue = 0, FailurePipeFail = 1
		};
		enum DurationControlProgress {
			DurationControlProgress_Full = 0, DurationControlProgress_Half = 1, DurationControlProgress_None = 2
		};
		enum DurationControlNotification {
			DurationControlNotification_None = 0, DurationControlNotification_1Hour = 1, DurationControlNotification_3Hours = 3, DurationControlNotification_HalfProgress = 3, DurationControlNotification_NoProgress = 4
		};
		// User Stats enums
		enum LeaderboardDataRequest {
			LeaderboardDataRequestGlobal = 0, LeaderboardDataRequestGlobalAroundUser = 1, LeaderboardDataRequestFriends = 2, LeaderboardDataRequestUsers = 3
		};
		enum LeaderboardDisplayType {
			LeaderboardDisplayTypeNone = 0, LeaderboardDisplayTypeNumeric = 1, LeaderboardDisplayTypeTimeSeconds = 2, LeaderboardDisplayTypeTiemMilliSeconds = 3
		};
		enum LeaderboardSortMethod {
			LeaderboardSortMethodNone = 0, LeaderboardSortMethodAscending = 1, LeaderboardSortMethodDescending = 2
		};
		enum LeaderboardUploadScoreMethod {
			LeaderboardUploadScoreMethod = 0, LeaderboardUploadScoreMethodKeepBest = 1, LeaderboardUploadScoreMethodForceUpdate = 2
		};
		// Utils enums
		enum CheckFileSignature {
			CheckFileSignatureInvalidSignature = 0, CheckFileSignatureValidSignature = 1, CheckFileSignatureFileNotFound = 2, CheckFileSignatureNoSignaturesFoundForThisApp = 3, CheckFileSignatureNoSignaturesFoundForThisFile = 4
		};
		enum GamepadTextInputLineMode {
			GamepadTextInputLineModeSingleLine = 0, GamepadTextInputLineModeMultipleLines = 1
		};
		enum GamepadTextInputMode {
			GamepadTextInputModeNormal = 0, GamepadTextInputModePassword = 1
		};
		enum SteamAPICallFailure {
			SteamAPICallFailureNone = -1, SteamAPICallFailureSteamGone = 0, SteamAPICallFailureNetworkFailure = 1, SteamAPICallFailureInvalidHandle = 2, SteamAPICallFailureMismatchedCallback = 3
		};
		
		static Steam* get_singleton();
		Steam();
		~Steam();

		CSteamID createSteamID(uint32_t steamID, int accountType=-1);

		// Main /////////////////////////////////
		bool restartAppIfNecessary(int value);
		Dictionary steamInit();
		bool isSteamRunning();

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
		bool isVACBanned();
		int getAppBuildId();
		String getAppInstallDir(AppId_t appID);
		uint64_t getAppOwner();
		String getAvailableGameLanguages();
		String getCurrentBetaName();
		String getCurrentGameLanguage();
		int getDLCCount();
		Dictionary getDLCDownloadProgress(int appID);
		int getEarliestPurchaseUnixTime(int value);
		void getFileDetails(const String& filename);
		Array getInstalledDepots(int appID);
		String getLaunchCommandLine();
		String getLaunchQueryParam(const String& key);
		void installDLC(int value);
		bool markContentCorrupt(bool missingFilesOnly);
		void uninstallDLC(int value);
		
		// Friends //////////////////////////////
		void activateGameOverlay(const String& type);
		void activateGameOverlayInviteDialog(uint64_t steamID);
		void activateGameOverlayToStore(int appID=0);
		void activateGameOverlayToUser(const String& type, uint64_t steamID);
		void activateGameOverlayToWebPage(const String& url);
		void clearRichPresence();
		bool closeClanChatWindowInSteam(uint64_t chatID);
		void downloadClanActivityCounts(uint64_t clanID, int clansToRequest);
		void enumerateFollowingList(uint32 startIndex);
		uint64_t getChatMemberByIndex(uint64_t clanID, int user);
		Dictionary getClanActivityCounts(uint64_t clanID);
		uint64_t getClanByIndex(int clan);
		int getClanChatMemberCount(uint64_t clanID);
		Dictionary getClanChatMessage(uint64_t chatID, int message);
		int getClanCount();
		String getClanName(uint64_t clanID);
		uint64_t getClanOfficerByIndex(uint64_t clanID, int officer);
		int getClanOfficerCount(uint64_t clanID);
		uint64_t getClanOwner(uint64_t clanID);
		String getClanTag(uint64_t clanID);
		uint64_t getCoplayFriend(int friendNum);
		int getCoplayFriendCount();
		void getFollowerCount(uint64_t steamID);
		uint64_t getFriendByIndex(int friendNum, int friendFlags);
		int getFriendCoplayGame(uint64_t friendID);
		int getFriendCoplayTime(uint64_t friendID);
		int getFriendCount();
		int getFriendCountFromSource(uint64_t clanID);
		uint64_t getFriendFromSourceByIndex(uint64_t sourceID, int friendNum);
		Dictionary getFriendGamePlayed(uint64_t steamID);
		Dictionary getFriendMessage(uint64_t friendID, int message);
		String getFriendPersonaName(uint64_t steamID);
		String getFriendPersonaNameHistory(uint64_t steamID, int nameHistory);
		int getFriendPersonaState(uint64_t steamID);
		int getFriendRelationship(uint64_t steamID);
		String getFriendRichPresence(uint64_t friendID, const String& key);
		int getFriendRichPresenceKeyCount(uint64_t friendID);
		String getFriendRichPresenceKeyByIndex(uint64_t friendID, int key);		
		int getFriendsGroupCount();
		int getFriendsGroupIDByIndex(int friendGroup);
		int getFriendsGroupMembersCount(int friendGroup);
		Array getFriendsGroupMembersList(int friendGroup, int memberCount);
		String getFriendsGroupName(int friendGroup);
		int getFriendSteamLevel(uint64_t steamID);
		int getLargeFriendAvatar(uint64_t steamID);
		int getMediumFriendAvatar(uint64_t steamID);
		String getPersonaName();
		int getPersonaState();
		void getPlayerAvatar(int size=2, uint64_t steamID=0);
		String getPlayerNickname(uint64_t steamID);
		Array getRecentPlayers();
		int getSmallFriendAvatar(uint64_t steamID);
		Array getUserFriendsGroups();
		uint32 getUserRestrictions();
		Array getUserSteamFriends();
		Array getUserSteamGroups();
		bool hasFriend(uint64_t steamID, int friendFlags);
		bool inviteUserToGame(uint64_t friendID, const String& connectString);
		bool isClanChatAdmin(uint64_t chatID, uint64_t steamID);
		bool isClanPublic(uint64_t clanID);
		bool isClanOfficialGameGroup(uint64_t clanID);
		bool isClanChatWindowOpenInSteam(uint64_t chatID);
		void isFollowing(uint64_t steamID);
		bool isUserInSource(uint64_t steamID, uint64_t sourceID);
		void joinClanChatRoom(uint64_t clanID);
		bool leaveClanChatRoom(uint64_t clanID);
		bool openClanChatWindowInSteam(uint64_t chatID);
		bool replyToFriendMessage(uint64_t steamID, const String& message);
		void requestClanOfficerList(uint64_t clanID);
		void requestFriendRichPresence(uint64_t friendID);
		bool requestUserInformation(uint64_t steamID, bool requireNameOnly);
		bool sendClanChatMessage(uint64_t chatID, const String& text);
		void setInGameVoiceSpeaking(uint64_t steamID, bool speaking);
		bool setListenForFriendsMessages(bool intercept);
		void setPersonaName(const String& name);
		void setPlayedWith(uint64_t steamID);
		bool setRichPresence(const String& key, const String& value);

		// HTML Surface /////////////////////////
		void addHeader(const String& key, const String& value);
		void allowStartRequest(bool allowed);
		void copyToClipboard();
		void createBrowser(const String& userAgent, const String& userCSS);
		void executeJavascript(const String& script);
//		void fileLoadDialogResponse(const String& selectedFiles);
		void find(const String& search, bool currentlyInFind, bool reverse);
		void getLinkAtPosition(int x, int y);
		void goBack();
		void goForward();
		void htmlInit();
		void jsDialogResponse(bool result);
		void keyChar(uint32 unicodeChar, int keyModifiers);
		void keyDown(uint32 nativeKeyCode, int keyModifiers);
		void keyUp(uint32 nativeKeyCode, int keyModifiers);
		void loadURL(const String& url, const String& postData);
		void mouseDoubleClick(int mouseButton);
		void mouseDown(int mouseButton);
		void mouseMove(int x, int y);
		void mouseUp(int mouseButton);
		void mouseWheel(int32 delta);
		void pasteFromClipboard();
		void reload();
		void removeBrowser();
		void setBackgroundMode(bool backgroundMode);
//		void setCookie(const String& hostname, const String& key, const String& value, const String& path, uint32 expires, bool secure, bool httpOnly);
		void setHorizontalScroll(uint32 absolutePixelScroll);
		void setKeyFocus(bool hasKeyFocus);
		void setPageScaleFactor(float zoom, int pointX, int pointY);
		void setSize(uint32 width, uint32 height);
		void setVerticalScroll(uint32 absolutePixelScroll);
		bool htmlShutdown();
		void stopFind();
		void stopLoad();
		void viewSource();

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
		bool setHTTPRequestContextValue(uint32 request, uint64 contextValue);
		bool setHTTPRequestCookieContainer(uint32 request);
		bool setHTTPRequestGetOrPostParameter(uint32 request, const String& name, const String& value);
		bool setHTTPRequestHeaderValue(uint32 request, const String& headerName, const String& headerValue);
		bool setHTTPRequestNetworkActivityTimeout(uint32 request, uint32 timeoutSeconds);
		uint8 setHTTPRequestRawPostBody(uint32 request, const String& contentType, uint32 bodyLen);
		bool setHTTPRequestRequiresVerifiedCertificate(uint32 request, bool requireVerifiedCertificate);
		bool setHTTPRequestUserAgentInfo(uint32 request, const String& userAgentInfo);
		
		// Input ////////////////////////////////
		void activateActionSet(uint64_t inputHandle, uint64_t actionSetHandle);
		void activateActionSetLayer(uint64_t inputHandle, uint64_t actionSetLayerHandle);
		void deactivateActionSetLayer(uint64_t inputHandle, uint64_t actionSetHandle);
		void deactivateAllActionSetLayers(uint64_t inputHandle);
		uint64_t getActionSetHandle(const String& actionSetName);
		int getActionOriginFromXboxOrigin(uint64_t inputHandle, int origin);
		Array getActiveActionSetLayers(uint64_t inputHandle);
		Dictionary getAnalogActionData(uint64_t inputHandle, uint64_t analogActionHandle);
		uint64_t getAnalogActionHandle(const String& actionName);
		Array getAnalogActionOrigins(uint64_t inputHandle, uint64_t actionSetHandle, uint64_t analogActionHandle);
		Array getConnectedControllers();
		uint64_t getControllerForGamepadIndex(int index);
		uint64_t getCurrentActionSet(uint64_t inputHandle);
		Array getDeviceBindingRevision(uint64_t inputHandle);
		Dictionary getDigitalActionData(uint64_t inputHandle, uint64_t digitalActionHandle);
		uint64_t getDigitalActionHandle(const String& actionName);
		Array getDigitalActionOrigins(uint64_t inputHandle, uint64_t actionSetHandle, uint64_t digitalActionHandle);
		int getGamepadIndexForController(uint64_t inputHandle);
		String getGlyphForActionOrigin(int origin);
		String getInputTypeForHandle(uint64_t inputHandle);
		Dictionary getMotionData(uint64_t inputHandle);
		int getRemotePlaySessionID(uint64_t inputHandle);
		String getStringForActionOrigin(int origin);
		bool inputInit();
		bool inputShutdown();
		void runFrame();
		void setLEDColor(uint64_t inputHandle, int colorR, int colorG, int colorB, int flags);
		bool showBindingPanel(uint64_t inputHandle);
		void stopAnalogActionMomentum(uint64_t inputHandle, uint64_t action);
		int translateActionOrigin(int destinationInput, int sourceOrigin);
		void triggerHapticPulse(uint64_t inputHandle, int targetPad, int duration);
//		void triggerRepeatedHapticPulse(uint64_t inputHandle, int targetPad, int duration, int offset, int repeat, int flags);
		void triggerVibration(uint64_t inputHandle, uint16_t leftSpeed, uint16_t rightSpeed);

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

		// Matchmaking //////////////////////////
		Array getFavoriteGames();
		int addFavoriteGame(uint32 ip, uint16 port, uint16 queryPort, uint32 flags, uint32 lastPlayed);
		bool removeFavoriteGame(AppId_t appID, uint32 ip, uint16 port, uint16 queryPort, uint32 flags);
		void requestLobbyList();
		void addRequestLobbyListStringFilter(const String& keyToMatch, const String& valueToMatch, int comparisonType);
		void addRequestLobbyListNumericalFilter(const String& keyToMatch, int valueToMatch, int comparisonType);
		void addRequestLobbyListNearValueFilter(const String& keyToMatch, int valueToBeCloseTo);
		void addRequestLobbyListFilterSlotsAvailable(int slotsAvailable);
		void addRequestLobbyListDistanceFilter(int distanceFilter);
		void addRequestLobbyListResultCountFilter(int maxResults);
		void createLobby(int lobbyType, int maxMembers);
		void joinLobby(uint64_t steamIDLobby);
		void leaveLobby(uint64_t steamIDLobby);
		bool inviteUserToLobby(uint64_t steamIDLobby, uint64_t steamIDInvitee);
		int getNumLobbyMembers(uint64_t steamIDLobby);
		uint64_t getLobbyMemberByIndex(uint64_t steamIDLobby, int member);
		String getLobbyData(uint64_t steamIDLobby, const String& key);
		bool setLobbyData(uint64_t steamIDLobby, const String& key, const String& value);
		Dictionary getLobbyDataByIndex(uint64_t steamIDLobby);
		bool deleteLobbyData(uint64_t steamIDLobby, const String& key);
		String getLobbyMemberData(uint64_t steamIDLobby, uint64_t steamIDUser, const String& key);
		void setLobbyMemberData(uint64_t steamIDLobby, const String& key, const String& value);
		bool sendLobbyChatMsg(uint64_t steamIDLobby, const String& messageBody);
		bool requestLobbyData(uint64_t steamIDLobby);
		void setLobbyGameServer(uint64_t steamIDLobby, const String& serverIP, uint16 serverPort, uint64_t steamIDGameServer);
		Dictionary getLobbyGameServer(uint64_t steamIDLobby);
		bool setLobbyMemberLimit(uint64_t steamIDLobby, int maxMembers);
		int getLobbyMemberLimit(uint64_t steamIDLobby);
		bool setLobbyType(uint64_t steamIDLobby, int lobbyType);
		bool setLobbyJoinable(uint64_t steamIDLobby, bool joinable);
		uint64_t getLobbyOwner(uint64_t steamIDLobby);
		bool setLobbyOwner(uint64_t steamIDLobby, uint64_t steamIDNewOwner);
		bool setLinkedLobby(uint64_t steamIDLobby, uint64_t steamIDLobbyDependent);

		// Music ////////////////////////////////
		bool musicIsEnabled();
		bool musicIsPlaying();
		float musicGetVolume();
		void musicPause();
		void musicPlay();
		void musicPlayNext();
		void musicPlayPrev();
		void musicSetVolume(float value);

		// Networking ///////////////////////////
		bool acceptP2PSessionWithUser(uint64_t steamIDRemote);
		bool allowP2PPacketRelay(bool allow);
		bool closeP2PChannelWithUser(uint64_t steamIDRemote, int channel);
		bool closeP2PSessionWithUser(uint64_t steamIDRemote);
		Dictionary getP2PSessionState(uint64_t steamIDRemote);
		uint32_t getAvailableP2PPacketSize(int channel = 0);
		Dictionary readP2PPacket(uint32_t packet, int channel = 0);
		bool sendP2PPacket(uint64_t steamIDRemote, const PoolByteArray data, int eP2PSendType, int channel = 0);

		// Remote Play //////////////////////////
		uint32 getSessionCount();
		uint32 getSessionID(int index);
		uint64_t getSessionSteamID(uint32 sessionID);
		String getSessionClientName(uint32 sessionID);
		int getSessionClientFormFactor(uint32 sessionID);
		Dictionary getSessionClientResolution(uint32 sessionID);
		bool sendRemotePlayTogetherInvite(uint64_t friendID);

		// Remote Storage ///////////////////////
		bool fileWrite(const String& file, const PoolByteArray& data, int32_t dataSize);
		Dictionary fileRead(const String& file, int32_t dataToRead);
		bool fileForget(const String& file);
		bool fileDelete(const String& file);
		bool fileExists(const String& file);
		bool filePersisted(const String& file);
		int32_t getFileSize(const String& file);
		int64_t getFileTimestamp(const String& file);
		int32_t getFileCount();
		Dictionary getFileNameAndSize(int file);
		Dictionary getQuota();
		uint32_t getSyncPlatforms(const String& file);
		bool isCloudEnabledForAccount();
		bool isCloudEnabledForApp();
		void setCloudEnabledForApp(bool enabled);

		// Screenshots //////////////////////////
		uint32_t addScreenshotToLibrary(const String& filename, const String& thumbnailFilename, int width, int height);
		uint32_t addVRScreenshotToLibrary(int type, const String& filename, const String& vrFilename);
		void hookScreenshots(bool hook);
		bool isScreenshotsHooked();
		bool setLocation(uint32_t screenshot, const String& location);
		bool tagPublishedFile(uint32 screenshot, uint64_t fileID);
		bool tagUser(uint32 screenshot, uint64_t steamID);
		void triggerScreenshot();
		uint32_t writeScreenshot(const PoolByteArray& RGB, int width, int height);

		// UGC //////////////////////////////////
		void addAppDependency(int publishedFileID, int appID);
		void addDependency(int publishedFileID, int childPublishedFileID);
		bool addExcludedTag(uint64_t queryHandle, const String& tagName);
		bool addItemKeyValueTag(uint64_t queryHandle, const String& key, const String& value);
		bool addItemPreviewFile(uint64_t queryHandle, const String& previewFile, int type);
		bool addItemPreviewVideo(uint64_t queryHandle, const String& videoID);
		void addItemToFavorite(int appID, int publishedFileID);
		bool addRequiredKeyValueTag(uint64_t queryHandle, const String& key, const String& value);
		bool addRequiredTag(uint64_t queryHandle, const String& tagName);
		bool initWorkshopForGameServer(int workshopDepotID);
		void createItem(AppId_t appID, int fileType);
		uint64_t createQueryAllUGCRequest(int queryType, int matchingType, int creatorID, int consumerID, uint32 page);
		uint64_t createQueryUGCDetailsRequest(Array publishedFileID);
//		uint64_t createQueryUserUGCRequest(int accountID, int listType, int matchingUGCType, int sortOrder, int creatorID, int consumerID, uint32 page);
		void deleteItem(int publishedFileID);
		bool downloadItem(int publishedFileID, bool highPriority);
		Dictionary getItemDownloadInfo(int fileID);
		Dictionary getItemInstallInfo(int publishedFileID);
		int getItemState(int publishedFileID);
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
		void getUserItemVote(int publishedFileID);
		bool releaseQueryUGCRequest(uint64_t queryHandle);
		void removeAppDependency(int publishedFileID, int appID);
		void removeDependency(int publishedFileID, int childPublishedFileID);
		void removeItemFromFavorites(int appID, int publishedFileID);
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
		void setUserItemVote(int publishedFileID, bool voteUp);
		uint64_t startItemUpdate(int appID, int fileId);
		void startPlaytimeTracking(Array publishedFileIDs);
		void stopPlaytimeTracking(Array publishedFileIDs);
		void stopPlaytimeTrackingForAllItems();
		void getAppDependencies(int publishedFileID);
		void submitItemUpdate(uint64_t updateHandle, const String& changeNote);
		void subscribeItem(int publishedFileID);
		void suspendDownloads(bool suspend);
		void unsubscribeItem(int publishedFileID);
		bool updateItemPreviewFile(uint64_t updateHandle, uint32 index, const String& previewFile);
		bool updateItemPreviewVideo(uint64_t updateHandle, uint32 index, const String& videoID);

		// Users ////////////////////////////////
		uint32_t getAuthSessionTicket();
		Dictionary getAuthSessionTicketID();
		void cancelAuthTicket(uint32_t authTicket);
		int beginAuthSession(uint32_t authTicket, uint64_t steamID);
		void endAuthSession(uint64_t steamID);
		uint64_t getSteamID();
		bool loggedOn();
		int getPlayerSteamLevel();
		String getUserDataFolder();
		void advertiseGame(const String& serverIP, int port);
		int getGameBadgeLevel(int series, bool foil);

		// User Stats ///////////////////////////
		void attachLeaderboardUGC(uint64_t ugcHandle);
		bool clearAchievement(const String& name);
		void downloadLeaderboardEntries(int start, int end, int type=k_ELeaderboardDataRequestGlobal);
		void downloadLeaderboardEntriesForUsers(Array usersID);
		void findLeaderboard(const String& name);
		void findOrCreateLeaderboard(const String& name, int sortMethod, int displayType);
		Dictionary getAchievement(const String& name);
		Dictionary getAchievementAchievedPercent(const String& name);
		Dictionary getAchievementAndUnlockTime(const String& name);
		String getAchievementDisplayAttribute(const String& name, const String& key);
		int getAchievementIcon(const String& name);
		String getAchievementName(uint32_t achievement);
		int64 getGlobalStatInt(const String& name);
		double getGlobalStatFloat(const String& name);
		int64 getGlobalStatIntHistory(const String& name);
		double getGlobalStatFloatHistory(const String& name);
		Dictionary getLeaderboardDisplayType();
		int getLeaderboardEntryCount();
		String getLeaderboardName();
		Dictionary getLeaderboardSortMethod();
		Array getMostAchievedAchievementInfo();
		uint32_t getNumAchievements();
		void getNumberOfCurrentPlayers();
		float getStatFloat(const String& name);
		int getStatInt(const String& name);
		Dictionary getUserAchievement(uint64_t steamID, const String& name);
		Dictionary getUserAchievementAndUnlockTime(uint64_t steamID, const String& name);
		float getUserStatFloat(uint64_t steamID, const String& name);
		int getUserStatInt(uint64_t steamID, const String& name);
		bool indicateAchievementProgress(const String& name, int currentProgress, int maxProgress);
		bool requestCurrentStats();
		void requestGlobalAchievementPercentages();
		void requestGlobalStats(int historyDays);
		void requestUserStats(uint64_t steamID);
		bool resetAllStats(bool achievementsToo=true);
		bool setAchievement(const String& name);
		bool setStatFloat(const String& name, float value);
		bool setStatInt(const String& name, int value);
		bool storeStats();
		bool updateAvgRateStat(const String& name, float thisSession, double sessionLength);
		void uploadLeaderboardScore(int score, bool keepBest=false, PoolIntArray details=PoolIntArray());
		Array getLeaderboardEntries();
		void setLeaderboardDetailsMax(int detailsMax);

		// Utils ////////////////////////////////
		int getAppID();
		int getCurrentBatteryPower();
		bool overlayNeedsPresent();
		Dictionary getImageRGBA(int iImage);
		Dictionary getImageSize(int iImage);
		uint32 getIPCCallCount();
		String getIPCountry();
		int getSecondsSinceAppActive();
		int getSecondsSinceComputerActive();
		int getServerRealTime();
		String getSteamUILanguage();
		bool isOverlayEnabled();
		bool isSteamInBigPictureMode();
		bool isSteamRunningInVR();
		bool isVRHeadsetStreamingEnabled();
		void setOverlayNotificationInset(int horizontal, int vertical);
		void setOverlayNotificationPosition(int pos);
		void setVRHeadsetStreamingEnabled(bool enabled);
		bool showGamepadTextInput(int inputMode, int lineInputMode, const String& description, uint32 maxText, const String& presetText);
		void startVRDashboard();

	protected:
		static void _bind_methods();
		static Steam* singleton;

	private:
		// Main
		bool isInitSuccess;

		// Apps
		uint64 currentAppID;
		
		// HTML Surface
		uint32 browserHandle;

		// HTTP
		uint32 cookieHandle;
		uint32 requestHandle;

		// Inventory
		SteamInventoryUpdateHandle_t inventoryUpdateHandle;
		SteamInventoryResult_t inventoryHandle;
		SteamItemDetails_t inventoryDetails;

		// Matchmaking
		CSteamID clanActivity;

		// Leaderboards
		SteamLeaderboard_t leaderboardHandle;
		Array leaderboardEntriesArray;
		int leaderboardDetailsMax;

		// Remote Play
		uint32 sessionID;

		// User stats
		int numAchievements;
		bool statsInitialized;
		uint64 ugcHandle;

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

		// Friend session state info ////////////
		struct FriendSessionStateInfo {
			uint32 onlineSessionInstance;
			uint8 publishedToFriendsSessionInstance;
		};
		Vector<FriendSessionStateInfo> sessionInfo;

		// Achievement data /////////////////////
		struct AchievementData {
			int achievementID;
			const char achievement;
			char name[128];
			char description[256];
			bool achieved;
			int icon;
		};
		Vector<AchievementData> achievementData;

		// Leaderboard entry ////////////////////
		struct LeaderboardEntry {
			uint64_t steamID;
			int32 globalRank;
			int32 score;
			int32 details;
			uint64 ugcHandle; 
		};
		Vector<LeaderboardEntry> leaderboardEntry;

		// UGC item details /////////////////////
		struct UGCDetails {
			int publishedFileID;
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


		// Run the Steamworks API callbacks /////
		void run_callbacks(){
			SteamAPI_RunCallbacks();
		}

		/////////////////////////////////////////
		// STEAM CALLBACKS //////////////////////
		/////////////////////////////////////////
		//
		// Apps callbacks ///////////////////////
		STEAM_CALLBACK(Steam, _dlc_installed, DlcInstalled_t, callbackDLCInstalled);
		STEAM_CALLBACK(Steam, _file_details_result, FileDetailsResult_t, callbackFileDetailsResult);
		STEAM_CALLBACK(Steam, _new_launch_url_parameters, NewUrlLaunchParameters_t, callbackNewLaunchURLParameters);
//		STEAM_CALLBACK(Steam, _new_launch_query_parameters, NewLaunchQueryParameters_t, callbackNewLaunchQueryParameters);  Seems not to be found?

		// Friends callbacks ////////////////////
		STEAM_CALLBACK(Steam, _avatar_loaded, AvatarImageLoaded_t, callbackAvatarLoaded);
		STEAM_CALLBACK(Steam, _clan_activity_downloaded, DownloadClanActivityCountsResult_t, callbackClanActivityDownloaded);
		STEAM_CALLBACK(Steam, _friend_rich_presence_update, FriendRichPresenceUpdate_t, callbackFriendRichPresenceUpdate);
		STEAM_CALLBACK(Steam, _connected_chat_join, GameConnectedChatJoin_t, callbackConnectedChatJoin);
		STEAM_CALLBACK(Steam, _connected_chat_leave, GameConnectedChatLeave_t, callbackConnectedChatLeave);
		STEAM_CALLBACK(Steam, _connected_clan_chat_message, GameConnectedClanChatMsg_t, callbackConnectedClanChatMessage);
		STEAM_CALLBACK(Steam, _connected_friend_chat_message, GameConnectedFriendChatMsg_t, callbackConnectedFriendChatMessage);
		STEAM_CALLBACK(Steam, _join_requested, GameLobbyJoinRequested_t, callbackJoinRequested);
		STEAM_CALLBACK(Steam, _overlay_toggled, GameOverlayActivated_t, callbackOverlayToggled);
		STEAM_CALLBACK(Steam, _join_game_requested, GameRichPresenceJoinRequested_t, callbackJoinGameRequested);
		STEAM_CALLBACK(Steam, _change_server_requested, GameServerChangeRequested_t, callbackChangeServerRequested);
		STEAM_CALLBACK(Steam, _join_clan_chat_complete, JoinClanChatRoomCompletionResult_t, callbackJoinClanChatComplete);
		STEAM_CALLBACK(Steam, _persona_state_change, PersonaStateChange_t, callbackPersonaStateChange);
		STEAM_CALLBACK(Steam, _name_changed, SetPersonaNameResponse_t, callbackNameChanged);

		// HTML Surface callbacks ///////////////
		STEAM_CALLBACK(Steam, _html_browser_ready, HTML_BrowserReady_t, callbackHTMLBrowserReady);
		STEAM_CALLBACK(Steam, _html_can_go_backandforward, HTML_CanGoBackAndForward_t, callbackHTMLCanGoBackandforward);
		STEAM_CALLBACK(Steam, _html_changed_title, HTML_ChangedTitle_t, callbackHTMLChangedTitle);
		STEAM_CALLBACK(Steam, _html_close_browser, HTML_CloseBrowser_t, callbackHTMLCloseBrowser);
		STEAM_CALLBACK(Steam, _html_file_open_dialog, HTML_FileOpenDialog_t, callbackHTMLFileOpenDialog);
		STEAM_CALLBACK(Steam, _html_finished_request, HTML_FinishedRequest_t, callbackHTMLFinishedRequest);
		STEAM_CALLBACK(Steam, _html_hide_tooltip, HTML_HideToolTip_t, callbackHTMLHideTooltip);
		STEAM_CALLBACK(Steam, _html_horizontal_scroll, HTML_HorizontalScroll_t, callbackHTMLHorizontalScroll);
		STEAM_CALLBACK(Steam, _html_js_alert, HTML_JSAlert_t, callbackHTMLJSAlert);
		STEAM_CALLBACK(Steam, _html_js_confirm, HTML_JSConfirm_t, callbackHTMLJSConfirm);
		STEAM_CALLBACK(Steam, _html_link_at_position, HTML_LinkAtPosition_t, callbackHTMLLinkAtPosition);
		STEAM_CALLBACK(Steam, _html_needs_paint, HTML_NeedsPaint_t, callbackHTMLNeedsPaint);
		STEAM_CALLBACK(Steam, _html_new_window, HTML_NewWindow_t, callbackHTMLNewWindow);
		STEAM_CALLBACK(Steam, _html_open_link_in_new_tab, HTML_OpenLinkInNewTab_t, callbackHTMLOpenLinkInNewTab);
		STEAM_CALLBACK(Steam, _html_search_results, HTML_SearchResults_t, callbackHTMLSearchResults);
		STEAM_CALLBACK(Steam, _html_set_cursor, HTML_SetCursor_t, callbackHTMLSetCursor);
		STEAM_CALLBACK(Steam, _html_show_tooltip, HTML_ShowToolTip_t, callbackHTMLShowTooltip);
		STEAM_CALLBACK(Steam, _html_start_request, HTML_StartRequest_t, callbackHTMLStartRequest);
		STEAM_CALLBACK(Steam, _html_status_text, HTML_StatusText_t, callbackHTMLStatusText);
		STEAM_CALLBACK(Steam, _html_update_tooltip, HTML_UpdateToolTip_t, callbackHTMLUpdateTooltip);
		STEAM_CALLBACK(Steam, _html_url_changed, HTML_URLChanged_t, callbackHTMLURLChanged);
		STEAM_CALLBACK(Steam, _html_vertical_scroll, HTML_VerticalScroll_t, callbackHTMLVerticalScroll);

		// HTTP callbacks ///////////////////////
		STEAM_CALLBACK(Steam, _http_request_completed, HTTPRequestCompleted_t, callbackHTTPRequestCompleted);
		STEAM_CALLBACK(Steam, _http_request_data_received, HTTPRequestDataReceived_t, callbackHTTPRequestDataReceived);
		STEAM_CALLBACK(Steam, _http_request_headers_received, HTTPRequestHeadersReceived_t, callbackHTTPRequestHeadersReceived);

		// Inventory callbacks //////////////////
		STEAM_CALLBACK(Steam, _inventory_definition_update, SteamInventoryDefinitionUpdate_t, callbackInventoryDefinitionUpdate);
		STEAM_CALLBACK(Steam, _inventory_full_update, SteamInventoryFullUpdate_t, callbackInventoryFullUpdate);
		STEAM_CALLBACK(Steam, _inventory_result_ready, SteamInventoryResultReady_t, callbackInventoryResultReady);

		// Matchmaking callbacks ////////////////
		STEAM_CALLBACK(Steam, _favorites_list_accounts_updated, FavoritesListAccountsUpdated_t, callbackFavoritesListAccountsUpdated);
		STEAM_CALLBACK(Steam, _favorites_list_changed, FavoritesListChanged_t, callbackFavoritesListChanged);
		STEAM_CALLBACK(Steam, _lobby_message, LobbyChatMsg_t, callbackLobbyMessage);
		STEAM_CALLBACK(Steam, _lobby_chat_update, LobbyChatUpdate_t, callbackLobbyChatUpdate);
		STEAM_CALLBACK(Steam, _lobby_data_update, LobbyDataUpdate_t, callbackLobbyDataUpdate);
		STEAM_CALLBACK(Steam, _lobby_joined, LobbyEnter_t, callbackLobbyJoined);
		STEAM_CALLBACK(Steam, _lobby_game_created, LobbyGameCreated_t, callbackLobbyGameCreated);
		STEAM_CALLBACK(Steam, _lobby_invite, LobbyInvite_t, callbackLobbyInvite);

		// Networking callbacks /////////////////
		STEAM_CALLBACK(Steam, _p2p_session_connect_fail, P2PSessionConnectFail_t, callbackP2PSessionConnectFail);
		STEAM_CALLBACK(Steam, _p2p_session_request, P2PSessionRequest_t, callbackP2PSessionRequest);

		// Remote Play callbacks ////////////////
		STEAM_CALLBACK(Steam, _remote_play_session_connected, SteamRemotePlaySessionConnected_t, callbackRemotePlaySessionConnected);
		STEAM_CALLBACK(Steam, _remote_play_session_disconnected, SteamRemotePlaySessionDisconnected_t, callbackRemotePlaySessionDisconnected);

		// Screenshot callbacks /////////////////
		STEAM_CALLBACK(Steam, _screenshot_ready, ScreenshotReady_t, callbackScreenshotReady);
		STEAM_CALLBACK(Steam, _screenshot_requested, ScreenshotRequested_t, callbackScreenshotRequested);

		// UGC callbacks ////////////////////////
		STEAM_CALLBACK(Steam, _item_downloaded, DownloadItemResult_t, callbackItemDownloaded);
		STEAM_CALLBACK(Steam, _item_installed, ItemInstalled_t, callbackItemInstalled);

		// User callbacks ///////////////////////
		STEAM_CALLBACK(Steam, _client_game_server_deny, ClientGameServerDeny_t, callbackClientGameServerDeny);
		STEAM_CALLBACK(Steam, _encrypted_app_ticket_response, EncryptedAppTicketResponse_t, callbackEncryptedAppTicketResponse);
		STEAM_CALLBACK(Steam, _game_web_callback, GameWebCallback_t, callbackGameWebCallback);
		STEAM_CALLBACK(Steam, _get_auth_session_ticket_response, GetAuthSessionTicketResponse_t, callbackGetAuthSessionTicketResponse);
		STEAM_CALLBACK(Steam, _ipc_failure, IPCFailure_t, callbackIPCFailure);
		STEAM_CALLBACK(Steam, _licenses_updated, LicensesUpdated_t, callbackLicensesUpdated);
		STEAM_CALLBACK(Steam, _microstransaction_auth_response, MicroTxnAuthorizationResponse_t, callbackMicrotransactionAuthResponse);
		STEAM_CALLBACK(Steam, _steam_server_connected, SteamServersConnected_t, callbackSteamServerConnected);
		STEAM_CALLBACK(Steam, _steam_server_disconnected, SteamServersDisconnected_t, callbackSteamServerDisconnected);
		STEAM_CALLBACK(Steam, _store_auth_url_response, StoreAuthURLResponse_t, callbackStoreAuthURLResponse);
		STEAM_CALLBACK(Steam, _validate_auth_ticket_response, ValidateAuthTicketResponse_t, callbackValidateAuthTicketResponse);

		// User stat callbacks //////////////////
		STEAM_CALLBACK(Steam, _user_achievement_stored, UserAchievementStored_t, callbackUserAchievementStored);
		STEAM_CALLBACK(Steam, _current_stats_received, UserStatsReceived_t, callbackCurrentStatsReceived);
		STEAM_CALLBACK(Steam, _user_stats_stored, UserStatsStored_t, callbackUserStatsStored);
		STEAM_CALLBACK(Steam, _user_stats_unloaded, UserStatsUnloaded_t, callbackUserStatsUnloaded);

		// Utility callbacks ////////////////////
		STEAM_CALLBACK(Steam, _gamepad_text_input_dismissed, GamepadTextInputDismissed_t, callbackGamepadTextInputDismissed);
		STEAM_CALLBACK(Steam, _ip_country, IPCountry_t, callbackIPCountry);
		STEAM_CALLBACK(Steam, _low_power, LowBatteryPower_t, callbackLowPower);
		STEAM_CALLBACK(Steam, _steam_api_call_completed, SteamAPICallCompleted_t, callbackSteamAPICallCompleted);
		STEAM_CALLBACK(Steam, _steam_shutdown, SteamShutdown_t, callbackSteamShutdown);

		/////////////////////////////////////////
		// STEAM CALL RESULTS ///////////////////
		/////////////////////////////////////////
		//
		// Friends call results /////////////////
		CCallResult<Steam, ClanOfficerListResponse_t> callResultClanOfficerList;
		void _request_clan_officer_list(ClanOfficerListResponse_t *callData, bool bIOFailure);
		CCallResult<Steam, FriendsEnumerateFollowingList_t> callResultEnumerateFollowingList;
		void _enumerate_following_list(FriendsEnumerateFollowingList_t *callData, bool bIOFailure);
		CCallResult<Steam, FriendsGetFollowerCount_t> callResultFollowerCount;
		void _get_follower_count(FriendsGetFollowerCount_t *callData, bool bIOFailure);
		CCallResult<Steam, FriendsIsFollowing_t> callResultIsFollowing;
		void _is_following(FriendsIsFollowing_t *callData, bool bIOFailure);

		// Inventory call results ///////////////
		CCallResult<Steam, SteamInventoryEligiblePromoItemDefIDs_t> callResultEligiblePromoItemDefIDs;
		void _inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *callData, bool bIOFailure);
		CCallResult<Steam, SteamInventoryRequestPricesResult_t> callResultRequestPrices;
		void _inventory_request_prices_result(SteamInventoryRequestPricesResult_t *callData, bool bIOFailure);
		CCallResult<Steam, SteamInventoryStartPurchaseResult_t> callResultStartPurchase;
		void _inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *callData, bool bIOFailure);

		// Matchmaking call results /////////////
		CCallResult<Steam, LobbyCreated_t> callResultCreateLobby;
		void _lobby_created(LobbyCreated_t *callData, bool bIOFailure);
		CCallResult<Steam, LobbyMatchList_t> callResultLobbyList;
		void _lobby_match_list(LobbyMatchList_t *callData, bool bIOFailure);

		// Remote Storage call results //////////
		CCallResult<Steam, RemoteStorageUnsubscribePublishedFileResult_t> callResultUnsubscribeItem;
		void _unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t *callData, bool bIOFailure);
		CCallResult<Steam, RemoteStorageSubscribePublishedFileResult_t> callResultSubscribeItem;
		void _subscribe_item(RemoteStorageSubscribePublishedFileResult_t *callData, bool bIOFailure);

		// UGC call results /////////////////////
		CCallResult<Steam, AddAppDependencyResult_t> callResultAddAppDependency;
		void _add_app_dependency_result(AddAppDependencyResult_t *callData, bool bIOFailure);
		CCallResult<Steam, AddUGCDependencyResult_t> callResultAddUGCDependency;
		void _add_ugc_dependency_result(AddUGCDependencyResult_t *callData, bool bIOFailure);
		CCallResult<Steam, CreateItemResult_t> callResultItemCreate;
		void _item_created(CreateItemResult_t *callData, bool bIOFailure);
		CCallResult<Steam, GetAppDependenciesResult_t> callResultGetAppDependencies;
		void _get_app_dependencies_result(GetAppDependenciesResult_t *callData, bool bIOFailure);
		CCallResult<Steam, DeleteItemResult_t> callResultDeleteItem;
		void _item_deleted(DeleteItemResult_t *callData, bool bIOFailure);
		CCallResult<Steam, GetUserItemVoteResult_t> callResultGetUserItemVote;
		void _get_item_vote_result(GetUserItemVoteResult_t *callData, bool bIOFailure);
		CCallResult<Steam, RemoveAppDependencyResult_t> callResultRemoveAppDependency;
		void _remove_app_dependency_result(RemoveAppDependencyResult_t *callData, bool bIOFailure);
		CCallResult<Steam, RemoveUGCDependencyResult_t> callResultRemoveUGCDependency;
		void _remove_ugc_dependency_result(RemoveUGCDependencyResult_t *callData, bool bIOFailure);
		CCallResult<Steam, SetUserItemVoteResult_t> callResultSetUserItemVote;
		void _set_user_item_vote(SetUserItemVoteResult_t *callData, bool bIOFailure);
		CCallResult<Steam, StartPlaytimeTrackingResult_t> callResultStartPlaytimeTracking;
		void _start_playtime_tracking(StartPlaytimeTrackingResult_t *callData, bool bIOFailure);
		CCallResult<Steam, SteamUGCQueryCompleted_t> callResultUGCQueryCompleted;
		void _ugc_query_completed(SteamUGCQueryCompleted_t *callData, bool bIOFailure);
		CCallResult<Steam, StopPlaytimeTrackingResult_t> callResultStopPlaytimeTracking;
		void _stop_playtime_tracking(StopPlaytimeTrackingResult_t *callData, bool bIOFailure);
		CCallResult<Steam, SubmitItemUpdateResult_t> callResultItemUpdate;
		void _item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure);
		CCallResult<Steam, UserFavoriteItemsListChanged_t> callResultFavoriteItemListChanged;
		void _user_favorite_items_list_changed(UserFavoriteItemsListChanged_t *callData, bool bIOFailure);

		// User call results ////////////////////
		CCallResult<Steam, SteamServerConnectFailure_t> callResultSteamServerConnectFailure;
		void _steam_server_connect_failed(SteamServerConnectFailure_t *callData);

		// User stat call results ///////////////
		CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
		void _global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure);
		CCallResult<Steam, GlobalStatsReceived_t> callResultGetGlobalStatsReceived;
		void _global_stats_received(GlobalStatsReceived_t *callData, bool bIOFailure);
		CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
		void _leaderboard_find_result(LeaderboardFindResult_t *callData, bool bIOFailure);
		CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
		void _leaderboard_scores_downloaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);
		CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
		void _leaderboard_score_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure);
		CCallResult<Steam, LeaderboardUGCSet_t> callResultLeaderboardUGCSet;
		void _leaderboard_ugc_set(LeaderboardUGCSet_t *callData, bool bIOFailure);
		CCallResult<Steam, NumberOfCurrentPlayers_t> callResultNumberOfCurrentPlayers;
		void _number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure);
		CCallResult<Steam, UserStatsReceived_t> callResultUserStatsReceived;
		void _user_stats_received(UserStatsReceived_t *callData, bool bIOFailure);

		// Utility call results /////////////////
		CCallResult<Steam, CheckFileSignature_t> callResultCheckFileSignature;
		void _check_file_signature(CheckFileSignature_t *callData);
};
#endif // GODOTSTEAM_H