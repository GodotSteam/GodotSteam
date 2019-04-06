#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "object.h"
#include "scene/resources/texture.h"	// For avatars
#include "reference.h"
#include "dictionary.h"					// Contains array.h as well

class Steam: public Object {
	public:
		enum {
			OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
			TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
			ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
			GLOBAL=0, GLOBAL_AROUND_USER=1, FRIENDS=2, USERS=3,
			PERSONA_NAME_MAX_UTF16=128, PERSONA_NAME_MAX_UTF8=128,
			OVERLAY_TO_STORE_FLAG_NONE=0, OVERLAY_TO_STORE_FLAG_ADD_TO_CART=1, OVERLAY_TO_STORE_FLAG_ADD_TO_CART_AND_SHOW=2,
			PRIVATE=0, FRIENDS_ONLY=1, PUBLIC=2, INVISIBLE=3, LOBBY_KEY_LENGTH=255,
			STATUS_INVALID=0, STATUS_PREPARING_CONFIG=1, STATUS_PREPARING_CONTENT=2, STATUS_UPLOADING_CONTENT=3, STATUS_UPLOADING_PREVIEW=4, STATUS_COMMITTING_CHANGES=5,
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
			RESULT_EMAIL_SEND_FAILURE=99, RESULT_NOT_SETTLED=100, RESULT_NEED_CAPTCHA=101, RESULT_GSLT_DENIED=102, RESULT_GS_OWNER_DENIED=103,RESULT_INVALID_ITEM_TYPE=104, RESULT_IP_BANNED=105, RESULT_GSLT_EXPIRED=106, RESULT_INSUFFICIENT_FUNDS=107, RESULT_TOO_MANY_PENDING=108
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
		void uploadLeaderboardScore(int score, bool keepBest=false);
		void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t handle, int entryCount);
		uint64_t getLeaderboardHandle();
		Array getLeaderboardEntries();
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
		// User stat callbacks //////////////////
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
		OBJ_TYPE(Steam, Object);
		OBJ_CATEGORY("Steam");
};
#endif // GODOTSTEAM_H