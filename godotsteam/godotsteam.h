#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/object.h"
#include "scene/resources/texture.h"	// For avatars
#include "core/reference.h"
#include "core/dictionary.h"					// Contains array.h as well

class Steam: public Object {
	GDCLASS(Steam, Object);
	public:
		enum {
			OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
			TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
			FLAG_NONE=0x00, FLAG_BLOCKED=0x01, FLAG_FRIENDSHIP_REQUESTED=0x02, FLAG_IMMEDIATE=0x04, FLAG_CLAN_MEMBER=0x08, FLAG_ON_GAME_SERVER=0x10, FLAG_REQUESTING_FRIENDSHIP=0x80, FLAG_REQUESTING_INFO=0x100,
			FLAG_IGNORED=0x200, FLAG_IGNORED_FRIEND=0x400, FLAG_CHAT_MEMBER=0x1000, FLAG_ALL=0xFFFF,	
			ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
			RELATION_NONE=0, RELATION_BLOCKED=1, RELATION_REQUEST_RECIPIENT=2, RELATION_FRIEND=3, RELATION_REQUEST_INITIATOR=4, RELATION_IGNORED=5, RELATION_IGNORED_FRIEND=6, RELATION_SUGGESTED=7, RELATION_MAX=8,
			AUTH_SESSION_OK=0, AUTH_SESSION_STEAM_NOT_CONNECTED=1, AUTH_SESSION_NO_LICENSE=2, AUTH_SESSION_VAC_BANNED=3, AUTH_SESSION_LOGGED_IN_ELSEWHERE=4,
			AUTH_SESSION_VAC_CHECK_TIMEOUT=5, AUTH_SESSION_TICKET_CANCELED=6, AUTH_SESSION_TICKET_ALREADY_USED=7, AUTH_SESSION_TICKET_INVALID=8, AUTH_SESSION_PUBLISHER_BANNED=9,
			AVATAR_SMALL=0, AVATAR_MEDIUM, AVATAR_LARGE,
			GLOBAL=0, GLOBAL_AROUND_USER=1, FRIENDS=2, USERS=3,
			PERSONA_NAME_MAX_UTF16=128, PERSONA_NAME_MAX_UTF8=128,
			PERSONA_CHANGE_NAME=0x0001, PERSONA_CHANGE_STATUS=0x0002, PERSONA_CHANGE_COME_ONLINE=0x0004, PERSONA_CHANGE_GONE_OFFLINE=0x0008, PERSONA_CHANGE_GAME_PLAYED=0x0010, PERSONA_CHANGE_GAME_SERVER=0x0020, PERSONA_CHANGE_AVATAR=0x0040, PERSONA_CHANGE_JOINED_SOURCE=0x0080,
			PERSONA_CHANGE_LEFT_SOURCE=0x0100, PERSONA_CHANGE_RELATIONSHIP_CHANGED=0x0200, PERSONA_CHANGE_NAME_FIRST_SET=0x0400, PERSONA_CHANGE_FACEBOOK_INFO=0x0800, PERSONA_CHANGE_NICKNAME=0x1000, PERSONA_CHANGE_STEAM_LEVEL=0x2000,
			RESTRICTION_NONE=0, RESTRICTION_UNKNOWN=1, RESTRICTION_ANY_CHAT=2, RESTRICTION_VOICE_CHAT=4, RESTRICTION_GROUP_CHAT=8, RESTRICTION_RATING=16, RESTRICTION_GAME_INVITES=32, RESTRICTION_TRADING=64,
			CHAT_INVALID=0, CHAT_MESSAGE=1, CHAT_TYPING=2, CHAT_INVITE_GAME=3, CHAT_EMOTE=4, CHAT_LEFT=6, CHAT_ENTERED=7, CHAT_KICKED=8, CHAT_BANNED=9, CHAT_DISCONNECTED=10, CHAT_HISTORICAL=11, CHAT_LINK_BLOCKED=14,
			CHAT_METADATA_MAX=8192, MAX_RICH_PRESENCE_KEYS=20, MAX_RICH_PRESENCE_KEY_LENGTH=64, MAX_RICH_PRESENCE_VALUE_LENGTH=256,
			OVERLAY_TO_STORE_FLAG_NONE=0, OVERLAY_TO_STORE_FLAG_ADD_TO_CART=1, OVERLAY_TO_STORE_FLAG_ADD_TO_CART_AND_SHOW=2,
			LOBBY_OK=0, LOBBY_NO_CONNECTION=1, LOBBY_TIMEOUT=2, LOBBY_FAIL=3, LOBBY_ACCESS_DENIED=4, LOBBY_LIMIT_EXCEEDED=5,
			PRIVATE=0, FRIENDS_ONLY=1, PUBLIC=2, INVISIBLE=3, LOBBY_KEY_LENGTH=255,
			LOBBY_EQUAL_LESS_THAN=-2, LOBBY_LESS_THAN=-1, LOBBY_EQUAL=0, LOBBY_GREATER_THAN=1, LOBBY_EQUAL_GREATER_THAN=2, LOBBY_NOT_EQUAL=3,
			LOBBY_DISTANCE_CLOSE=0, LOBBY_DISTANCE_DEFAULT=1, LOBBY_DISTANCE_FAR=2, LOBBY_DISTANCE_WORLDWIDE=3,
			UGC_MAX_TITLE_CHARS=128, UGC_MAX_DESC_CHARS=8000, UGC_MAX_METADATA_CHARS=5000,
			UGC_ITEM_COMMUNITY=0, UGC_ITEM_MICROTRANSACTION=1, UGC_ITEM_COLLECTION=2, UGC_ITEM_ART=3, UGC_ITEM_VIDEO=4, UGC_ITEM_SCREENSHOT=5, UGC_ITEM_GAME=6, UGC_ITEM_SOFTWARE=7,
			UGC_ITEM_CONCEPT=8, UGC_ITEM_WEBGUIDE=9, UGC_ITEM_INTEGRATEDGUIDE=10, UGC_ITEM_MERCH=11, UGC_ITEM_CONTROLLERBINDING=12, UGC_ITEM_STEAMWORKSACCESSINVITE=13,
			UGC_ITEM_STEAMVIDEO=14, UGC_ITEM_GAMEMANAGEDITEM=15, UGC_ITEM_MAX=16,
			UGC_STATE_NONE=0, UGC_STATE_SUBSCRIBED=1, UGC_STATE_LEGACY=2, UGC_STATE_INSTALLED=4, UGC_STATE_UPDATE=8, UGC_STATE_DOWNLOADING=16, UGC_STATE_PENDING=32,
			UGC_FILE_VISIBLE_PUBLIC=0, UGC_FILE_VISIBLE_FRIENDS=1, UGC_FILE_VISIBLE_PRIVATE=2,
			STATUS_INVALID=0, STATUS_PREPARING_CONFIG=1, STATUS_PREPARING_CONTENT=2, STATUS_UPLOADING_CONTENT=3, STATUS_UPLOADING_PREVIEW=4, STATUS_COMMITTING_CHANGES=5,
			REMOTE_STORAGE_PLATFORM_NONE=0, REMOTE_STORAGE_PLATFORM_WINDOWS=(1<<0), REMOTE_STORAGE_PLATFORM_OSX=(1<<1), REMOTE_STORAGE_PLATFORM_PS3=(1<<2), 
			REMOTE_STORAGE_PLATFORM_LINUX=(1<<3), REMOTE_STORAGE_PLATFORM_RESERVED2=(1<<4), REMOTE_STORAGE_PLATFORM_ALL=0xffffffff,
			RESULT_OK=1, RESULT_FAIL=2, RESULT_NO_CONNECT=3, RESULT_INVALID_PASSWORD=5, RESULT_LOGGED_IN_ESLEWHERE=6, RESULT_INVALID_PROTOCAL=7, RESULT_INALID_PARAM=8, RESULT_FILE_NOT_FOUND=9, RESULT_BUSY=10, RESULT_INVALID_STATE=11, RESULT_INVALID_NAME=12,
			RESULT_INVALID_EMAIL=13, RESULT_DUPLICATE_NAME=14, RESULT_ACCESS_DENIED=15, RESULT_TIMEOUT=16, RESULT_BANNED=17, RESULT_ACCOUNT_NOT_FOUND=18, RESULT_INVALID_STEAM_ID=19, RESULT_SERVICE_UNAVAILABLE=20, RESULT_NOT_LOGGED_ON=21, RESULT_PENDING=22,
			RESULT_ENCRYPT_FAILURE=23, RESULT_INSUFFICIENT_PRIVILEGE=24, RESULT_LIMIT_EXCEEDED=25, RESULT_REVOKED=26, RESULT_EXPIRED=27, RESULT_ALREADY_REDEEMED=28, RESULT_DUPLICATE_REQUEST=29, RESULT_ALREADY_OWNED=30, RESULT_IP_NOT_FOUND=31, RESULT_PERSIST_FAILED=32,
			RESULT_LOCKING_FAILED=33, RESULT_LOGON_SESSION_REPLACED=34, RESULT_CONNECT_FAILED=35, RESULT_HANDSHAKE_FAILED=36, RESULT_IO_FAILURE=37, RESULT_REMOTE_DISCONNECT=38, RESULT_SHOPPING_CART_NOT_FOUND=39, RESULT_BLOCKED=40, RESULT_IGNORED=41, RESULT_NO_MATCH=42,
			RESULT_ACCOUNT_DISABLED=43, RESULT_SERVICE_READY_ONLY=44, RESULT_ACCOUNT_NOT_FEATURED=45, RESULT_ADMINISTRATOR_OK=46, RESULT_CONTENT_VERSION=47, RESULT_TRY_ANOTHER_CM=48, RESULT_PASSWORD_REQUIRED_TO_KICK=49, RESULT_ALREADY_LOGGED_ELSEWHERE=50,
			RESULT_SUSPENDED=51, RESULT_CANCELLED=52, RESULT_DATA_CORRUPTION=53, RESULT_DISK_FULL=54, RESULT_REMOTE_CALL_FAILED=55, RESULT_PASSWORD_UNSET=56, RESULT_EXTERNAL_ACCOUNT_UNLINKED=57, RESULT_PSN_TICKET_INVALID=58, RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED=59,
			RESULT_REMOTE_FILE_CONFLICT=60, RESULT_ILLEGAL_PASSWORD=61, RESULT_SAME_AS_PREVIOUS_VALUE=62, RESULT_ACCOUNT_LOGON_DENIED=63, RESULT_CANNOT_USE_OLD_PASSWORD=64, RESULT_INVALID_LOGIN_AUTH_CODE=65, RESULT_ACCOUNT_LOGON_DENIED_NO_MAIL=66, RESULT_HARDWARE_NOT_CAPABLE=67,
			RESULT_IP_INIT_ERROR=68, RESULT_PARENTAL_CONTROL_RESTRICTED=69, RESULT_FACEBOOK_QUERY_ERROR=70, RESULT_EXPIRED_LOGIN_AUTH_CODE=71, RESULT_IP_LOGIN_RESTRICTION_FAILED=72, RESULT_ACCOUNT_LOCKED_DOWN=73, RESULT_ACCOUNT_LOGON_DENIED_VERIFIED_EMAIL_REQUIRED=74,
			RESULT_NO_MATCHING_URL=75, RESULT_BAD_RESPONSE=76, RESULT_REQUIRED_PASSWORD_REENTRY=77, RESULT_VALUE_OUT_OF_RANGE=78, RESULT_UNEXPECTED_ERROR=79, RESULT_DISABLED=80, RESULT_INVALID_CEG_SUBMISSION=81, RESULT_RESTRICTED_DEVICE=82, RESULT_REGION_LOCKED=83,
			RESULT_RATE_LIMIT_EXCEEDED=84, RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR=85, RESULT_ITEM_DELETED=86, RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE=87, RESULT_TWO_FACTOR_CODE_MISMATCH=88, RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH=89, RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS=90,
			RESULT_NOT_MODIFIED=91, RESULT_NO_MOBILE_DEVICE=92, RESULT_TIME_NOT_SYNCED=93, RESULT_SMS_CODE_FAILED=94, RESULT_ACCOUNT_LIMIT_EXCEEDED=95, RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED=96, RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED=97, RESULT_REFUND_TO_WALLET=98,
			RESULT_EMAIL_SEND_FAILURE=99, RESULT_NOT_SETTLED=100, RESULT_NEED_CAPTCHA=101, RESULT_GSLT_DENIED=102, RESULT_GS_OWNER_DENIED=103,RESULT_INVALID_ITEM_TYPE=104, RESULT_IP_BANNED=105, RESULT_GSLT_EXPIRED=106, RESULT_INSUFFICIENT_FUNDS=107, RESULT_TOO_MANY_PENDING=108,
			CHAT_ROOM_SUCCESS=1, CHAT_ROOM_DOESNT_EXIST=2, CHAT_ROOM_NOT_ALLOWED=3, CHAT_ROOM_FULL=4, CHAT_ROOM_ERROR=5, CHAT_ROOM_BANNED=6, CHAT_ROOM_LIMITED=7, CHAT_ROOM_CLAN_DISABLED=8, CHAT_ROOM_COMMUNITY_BAN=9, CHAT_ROOM_MEMBER_BLOCKED_YOU=10, CHAT_ROOM_YOU_BLOCKED_MEMBER=11
		};
		static Steam* get_singleton();
		Steam();
		~Steam();

		CSteamID createSteamID(uint32_t steamID, int accountType=-1);
		// Steamworks ///////////////////////////////
		bool restartAppIfNecessary(int value);
		bool steamInit();
		bool isSteamRunning();
		// Apps /////////////////////////////////////
		bool isSubscribed();
		String getCurrentGameLanguage();
		bool isSubscribedApp(int value);
		bool isDLCInstalled(int value);
		int getDLCCount();
		uint64_t getAppOwner();
		int getAppBuildId();
		// Friends //////////////////////////////////
		String getPersonaName();
		void activateGameOverlay(const String& type);
		void activateGameOverlayToUser(const String& type, uint64_t steamID);
		void activateGameOverlayToWebPage(const String& url);
		void activateGameOverlayToStore(int appID=0);
		// Users ////////////////////////////////////
		uint64_t getSteamID();
		bool loggedOn();
		int getPlayerSteamLevel();
		int getGameBadgeLevel(int series, bool foil);
		// User Stats ///////////////////////////////
		bool clearAchievement(const String& name);
		Dictionary getAchievement(const String& name);
		Dictionary getAchievementAchievedPercent(const String& name);
		String getAchievementName(uint32_t iAchievement);
		float getStatFloat(const String& name);
		int getStatInt(const String& name);
		bool resetAllStats(bool achievementsToo=true);
		bool requestCurrentStats();
		void requestGlobalAchievementPercentages();
		bool setAchievement(const String& name);
		bool setStatFloat(const String& name, float value);
		bool setStatInt(const String& name, int value);
		bool storeStats();
		bool getAchievementAndUnlockTime(const String& name, bool achieved, uint32_t unlockTime);
		bool indicateAchievementProgress(const String& name, int currentProgress, int maxProgress);
		void findLeaderboard(const String& name);
		String getLeaderboardName();
		int getLeaderboardEntryCount();
		void downloadLeaderboardEntries(int start, int end, int type=GLOBAL);
		void downloadLeaderboardEntriesForUsers(Array usersID);
		void uploadLeaderboardScore(int score, bool keepBest=false, PoolIntArray details=PoolIntArray());
		void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t handle, int entryCount);
		uint64_t getLeaderboardHandle();
		Array getLeaderboardEntries();
		void setLeaderboardDetailsMax(int detailsMax);
		// Utils ////////////////////////////////////
		String getIPCountry();
		bool isOverlayEnabled();
		String getSteamUILanguage();
		int getAppID();
		int getSecondsSinceAppActive();
		void setOverlayNotificationPosition(int pos);
		int getCurrentBatteryPower();

	protected:
		static void _bind_methods();
		static Steam* singleton;

	private:
		bool isInitSuccess;
		// Apps
		uint64 currentAppID;
		// Leaderboards
		SteamLeaderboard_t leaderboardHandle;
		Array leaderboardEntries;
		int leaderboardDetailsMax;
		// User stats
		int numAchievements;
		bool statsInitialized;
		/////////////////////////////////////////
		// STRUCTS //////////////////////////////
		//
		// Achievement data
		struct AchievementData {
			int achievementID;
			const char achievement;
			char name[128];
			char description[256];
			bool achieved;
			int icon;
		};
		Vector<AchievementData> achievementData;
		/////////////////////////////////////////
		// STEAM CALLBACKS //////////////////////
		//
		// Getting all statistics and achievements from Steam
		STEAM_CALLBACK(Steam, _user_stats_received, UserStatsReceived_t);
		// Getting global achievement percentages
		CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
		void _global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure);
		// Storing user stats
		STEAM_CALLBACK(Steam, _user_stats_stored, UserStatsStored_t);
		// Storing user achievements
		STEAM_CALLBACK(Steam, _user_achievement_stored, UserAchievementStored_t);
		// Uploading scores to the leaderboard
		CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
		void _leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure);
		// Finding a leaderboard
		CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
		void _leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure);
		// Downloading scores from a leaderboard
		CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
		void _leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);
		// Utility callbacks ////////////////////
		//
		STEAM_CALLBACK(Steam, _overlay_toggled, GameOverlayActivated_t);
		STEAM_CALLBACK(Steam, _low_power, LowBatteryPower_t);
		// Run the Steamworks API callbacks /////
		void run_callbacks(){
			SteamAPI_RunCallbacks();
		}
};
#endif // GODOTSTEAM_H