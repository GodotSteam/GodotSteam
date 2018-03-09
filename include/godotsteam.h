#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include "steam/steam_api.h"

#include "core/Godot.hpp"
#include "Reference.hpp"
#include "Image.hpp"			// For avatars; perhaps should be Texture.hpp
#include "core/Dictionary.hpp" 	// Contains array.h as well

using namespace godot;

class Steam : public GodotScript<Reference>{
	GODOT_CLASS(Steam);

	public:
		enum {
			OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
			TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
			ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
			AUTH_SESSION_OK=0, AUTH_SESSION_STEAM_NOT_CONNECTED=1, AUTH_SESSION_NO_LICENSE=2, AUTH_SESSION_VAC_BANNED=3, AUTH_SESSION_LOGGED_IN_ELSEWHERE=4,
			AUTH_SESSION_VAC_CHECK_TIMEOUT=5, AUTH_SESSION_TICKET_CANCELED=6, AUTH_SESSION_TICKET_ALREADY_USED=7, AUTH_SESSION_TICKET_INVALID=8, AUTH_SESSION_PUBLISHER_BANNED=9,
			AVATAR_SMALL=0, AVATAR_MEDIUM, AVATAR_LARGE,
			GLOBAL=0, GLOBAL_AROUND_USER=1, FRIENDS=2, USERS=3,
			LOBBY_OK=0, LOBBY_NO_CONNECTION=1, LOBBY_TIMEOUT=2, LOBBY_FAIL=3, LOBBY_ACCESS_DENIED=4, LOBBY_LIMIT_EXCEEDED=5,
			PRIVATE=0, FRIENDS_ONLY=1, PUBLIC=2, INVISIBLE=3, LOBBY_KEY_LENGTH=255,
			UGC_MAX_TITLE_CHARS=128, UGC_MAX_DESC_CHARS=8000, UGC_MAX_METADATA_CHARS=5000,
			UGC_ITEM_COMMUNITY=0, UGC_ITEM_MICROTRANSACTION=1,
			UGC_STATE_NONE=0, UGC_STATE_SUBSCRIBED=1, UGC_STATE_LEGACY=2, UGC_STATE_INSTALLED=4, UGC_STATE_UPDATE=8, UGC_STATE_DOWNLOADING=16, UGC_STATE_PENDING=32,
			STATUS_INVALID=0, STATUS_PREPARING_CONFIG=1, STATUS_PREPARING_CONTENT=2, STATUS_UPLOADING_CONTENT=3, STATUS_UPLOADING_PREVIEW=4, STATUS_COMMITTING_CHANGES=5,
			REMOTE_STORAGE_PLATFORM_NONE=0, REMOTE_STORAGE_PLATFORM_WINDOWS=(1<<0), REMOTE_STORAGE_PLATFORM_OSX=(1<<1), REMOTE_STORAGE_PLATFORM_PS3=(1<<2), 
			REMOTE_STORAGE_PLATFORM_LINUX=(1<<3), REMOTE_STORAGE_PLATFORM_RESERVED2=(1<<4), REMOTE_STORAGE_PLATFORM_ALL=0xffffffff
		};
		static Steam *get_singleton();
		Steam();
		~Steam();

		CSteamID createSteamID(uint32_t steamID, int accountType=-1);
		Image drawAvatar(int size, uint8* buffer);
		// Steamworks ///////////////////////////////
		bool restartAppIfNecessary(int value);
		bool steamInit();
		bool isSteamRunning();
		// Apps /////////////////////////////////////
		bool hasOtherApp(int value);
		int getDLCCount();
		bool isDLCInstalled(int value);
		bool isAppInstalled(int value);
		String getCurrentGameLanguage();
		bool isVACBanned();
		int getEarliestPurchaseUnixTime(int value);
		bool isSubscribedFromFreeWeekend();
		void installDLC(int value);
		void uninstallDLC(int value);
		bool isSubscribed();
		bool isLowViolence();
		bool isCybercafe();
		bool isSubscribedApp(int value);
		int getAppBuildId();
		// Friends //////////////////////////////////
		int getFriendCount();
		String getPersonaName();
		String getFriendPersonaName(int steamID);
		void setGameInfo(const String& key, const String& value);
		void clearGameInfo();
		void inviteFriend(int id, const String& connectString);
		void setPlayedWith(int steamID);
		Array getRecentPlayers();
		void getFriendAvatar(int size=AVATAR_MEDIUM);
		Array getUserSteamGroups();
		Array getUserSteamFriends();
		void activateGameOverlay(const String& type);
		void activateGameOverlayToUser(const String& type, int steamID);
		void activateGameOverlayToWebPage(const String& url);
		void activateGameOverlayToStore(int appID=0);
		void activateGameOverlayInviteDialog(int steamID);
		// Matchmaking //////////////////////////////
		void createLobby(int lobbyType, int maxMembers);
		void joinLobby(int steamIDLobby);
		void leaveLobby(int steamIDLobby);
		bool inviteUserToLobby(int steamIDLobby, int steamIDInvitee);
		// Music ////////////////////////////////////
		bool musicIsEnabled();
		bool musicIsPlaying();
		float musicGetVolume();
		void musicPause();
		void musicPlay();
		void musicPlayNext();
		void musicPlayPrev();
		void musicSetVolume(float value);
		// Remote Storage ///////////////////////////
		bool fileWrite(const String& file, const PoolByteArray& vData, int32_t cubData);
		Dictionary fileRead(const String& file, int32_t cubDataToRead);
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
		// Screenshots //////////////////////////////
		void hookScreenshots(bool hook);
		bool isScreenshotsHooked();
		void triggerScreenshot();
		uint32_t writeScreenshot(const PoolByteArray& RGB, int width, int height);
		// Users ////////////////////////////////////
//		uint32_t getAuthSessionTicket();
//		void cancelAuthTicket(uint32_t authTicket);
//		int beginAuthSession(uint32_t authTicket, uint64_t steamID);
//		void endAuthSession(uint64_t steamID);
		uint64_t getSteamID();
		bool loggedOn();
		int getPlayerSteamLevel();
		String getUserDataFolder();
//		void advertiseGame(const String& serverIP, int port);
		int getGameBadgeLevel(int series, bool foil);
		// User Stats ///////////////////////////////
		bool clearAchievement(const String& name);
		uint32_t getNumAchievements();
		void getNumberOfCurrentPlayers();
		bool getAchievement(const String& name);
		Dictionary getAchievementAchievedPercent(const String& name);
		float getStatFloat(const String& name);
		int getStatInt(const String& name);
		bool resetAllStats(bool achievementsToo=true);
		bool requestCurrentStats();
		void requestGlobalAchievementPercentages();
		bool setAchievement(const String& name);
		bool setStatFloat(const String& name, float value);
		bool setStatInt(const String& name, int value);
		bool storeStats();
		void findLeaderboard(const String& name);
		String getLeaderboardName();
		int getLeaderboardEntryCount();
		void downloadLeaderboardEntries(int start, int end, int type=GLOBAL);
		void downloadLeaderboardEntriesForUsers(Array usersID);
		void uploadLeaderboardScore(int score, bool keepBest=false);
		void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount);
		uint64_t getLeaderboardHandle();
		Array getLeaderboardEntries();
		bool getAchievementAndUnlockTime(const String& name, bool achieved, int unlockTime);
		bool indicateAchievementProgress(const String& name, int curProgress, int maxProgress);
		// Utils ////////////////////////////////////
		String getIPCountry();
		bool isOverlayEnabled();
		String getSteamUILanguage();
		int getAppID();
		int getSecondsSinceAppActive();
		void setOverlayNotificationPosition(int pos);
		int getCurrentBatteryPower();
		bool isSteamRunningInVR();
		int getServerRealTime();
		bool isSteamInBigPictureMode();
		void startVRDashboard();
		// Workshop /////////////////////////////////
		int getNumSubscribedItems();
		int getItemState(int publishedFileID);
		bool downloadItem(int publishedFileID, bool highPriority);
		void suspendDownloads(bool suspend);

	protected:
		static void _bind_methods();
		static Steam *singleton;

	private:
		bool isInitSuccess;
		// Leaderboards
		SteamLeaderboard_t leaderboard_handle;
		Array leaderboard_entries;
		// Authentication
		struct TicketData {
			uint32_t id;
			uint32_t *buf;
			uint32_t size;
		};
//		Vector<TicketData> tickets;	//-> Errors with 'Vector' does not name a type
		// Steam Callbacks //////////////////////////
//		CCallResult<Steam, LobbyCreated_t> callCreatedLobby;
//		void _lobby_created(LobbyCreated_t *callData);
//		CCallResult<Steam, LobbyEnter_t> callEnteredLobby;
//		void _lobby_joined(LobbyEnter_t *callData);
//		CCallResult<Steam, LobbyInvite_t> callInviteLobby;
//		void _lobby_invite(LobbyInvite_t *callData);
//		CCallResult<Steam, GameRichPresenceJoinRequested_t> callJoinRequested;
//		void _join_requested(GameRichPresenceJoinRequested_t *callData);
//		CCallResult<Steam, GameOverlayActivated_t> callOverlayActivated;
//		void _overlay_toggled(GameOverlayActivated_t *callData);
//		CCallResult<Steam, LowBatteryPower_t> callLowPower;
//		void _low_power(LowBatteryPower_t *callData);
//		CCallResult<Steam, AvatarImageLoaded_t> callImageLoaded;
//		void _avatar_loaded(AvatarImageLoaded_t *callData);
//		CCallResult<Steam, SteamServersConnected_t> callServerConnected;
//		void _server_connected(SteamServersConnected_t *callData);
//		CCallResult<Steam, SteamServersDisconnected_t> callServerDisconnected;
//		void _server_disconnected(SteamServersDisconnected_t *callData);
//		CCallResult<Steam, DlcInstalled_t> callInstallDLC;
//		void _dlc_installed(DlcInstalled_t *callData);
//		CCallResult<Steam, GetAuthSessionTicketResponse_t> callAuthResponse;
//		void _get_auth_session_ticket_response(GetAuthSessionTicketResponse_t *callData);
//		CCallResult<Steam, ValidateAuthTicketResponse_t> callAuthValidation;
//		void _validate_auth_ticket_response(ValidateAuthTicketResponse_t *callData);
//		CCallResult<Steam, ScreenshotReady_t> callReadyScreenshot;
//		void _screenshot_ready(ScreenshotReady_t *callData);
//		CCallResult<Steam, UserStatsReceived_t> callUserStatsReceived;
//		void _user_stats_received(UserStatsReceived_t *callData);
//		CCallResult<Steam, NumberOfCurrentPlayers_t> callResultNumberOfCurrentPlayers;
//		void _number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure);
//		CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
//		void _leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure);
//		CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
//		void _leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure);
//		CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
//		void _leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);
//		CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
//		void _global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure);
		// Run callbacks
		void run_callbacks(){
			SteamAPI_RunCallbacks();
		}

	public:
		// Register methods /////////////////////////
		static void _register_methods(){
			register_method("restartAppIfNecessary", &Steam::restartAppIfNecessary);
			register_method("steamInit", &Steam::steamInit);
			register_method("isSteamRunning", &Steam::isSteamRunning);
			register_method("run_callbacks", &Steam::run_callbacks);
			// Apps Bind Methods ////////////////////////
			register_method("hasOtherApp", &Steam::hasOtherApp);
			register_method("getDLCCount", &Steam::getDLCCount);
			register_method("isDLCInstalled", &Steam::isDLCInstalled);
			register_method("isAppInstalled", &Steam::isAppInstalled);
			register_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
			register_method("isVACBanned", &Steam::isVACBanned);
			register_method("getEarliestPurchaseUnixTime", &Steam::getEarliestPurchaseUnixTime);
			register_method("isSubscribedFromFreeWeekend", &Steam::isSubscribedFromFreeWeekend);
			register_method("installDLC", &Steam::installDLC);
			register_method("uninstallDLC", &Steam::uninstallDLC);
			register_method("isSubscribed", &Steam::isSubscribed);
			register_method("isLowViolence", &Steam::isLowViolence);
			register_method("isCybercafe", &Steam::isCybercafe);
			register_method("isSubscribedApp", &Steam::isSubscribedApp);
			register_method("getAppBuildId", &Steam::getAppBuildId);
			// Friends Bind Methods /////////////////////
			register_method("getFriendCount", &Steam::getFriendCount);
			register_method("getPersonaName", &Steam::getPersonaName);
			register_method("getFriendPersonaName", &Steam::getFriendPersonaName);
			register_method("setGameInfo", &Steam::setGameInfo);
			register_method("clearGameInfo", &Steam::clearGameInfo);
			register_method("inviteFriend", &Steam::inviteFriend);
			register_method("setPlayedWith", &Steam::setPlayedWith);
			register_method("getRecentPlayers", &Steam::getRecentPlayers);
			register_method("getFriendAvatar", &Steam::getFriendAvatar);
			register_method("getUserSteamGroups", &Steam::getUserSteamGroups);
			register_method("getUserSteamFriends", &Steam::getUserSteamFriends);
			register_method("activateGameOverlay", &Steam::activateGameOverlay);
			register_method("activateGameOverlayToUser", &Steam::activateGameOverlayToUser);
			register_method("activateGameOverlayToWebPage", &Steam::activateGameOverlayToWebPage);
			register_method("activateGameOverlayToStore", &Steam::activateGameOverlayToStore);
			register_method("activateGameOverlayInviteDialog", &Steam::activateGameOverlayInviteDialog);
			// Matchmaking Bind Methods /////////////////
			register_method("createLobby", &Steam::createLobby);
			register_method("joinLobby", &Steam::joinLobby);
			register_method("leaveLobby", &Steam::leaveLobby);
			register_method("inviteUserToLobby", &Steam::inviteUserToLobby);
			// Music Bind Methods ///////////////////////
			register_method("musicIsEnabled", &Steam::musicIsEnabled);
			register_method("musicIsPlaying", &Steam::musicIsPlaying);
			register_method("musicGetVolume", &Steam::musicGetVolume);
			register_method("musicPause", &Steam::musicPause);
			register_method("musicPlay", &Steam::musicPlay);
			register_method("musicPlayNext", &Steam::musicPlayNext);
			register_method("musicPlayPrev", &Steam::musicPlayPrev);
			register_method("musicSetVolume", &Steam::musicSetVolume);
			// Remote Storage Bind Methods //////////////
			register_method("fileWrite", &Steam::fileWrite);
			register_method("fileRead", &Steam::fileRead);
			register_method("fileForget", &Steam::fileForget);
			register_method("fileDelete", &Steam::fileDelete);
			register_method("fileExists", &Steam::fileExists);
			register_method("filePersisted", &Steam::filePersisted);
			register_method("getFileSize", &Steam::getFileSize);
			register_method("getFileTimestamp", &Steam::getFileTimestamp);
			register_method("getFileCount", &Steam::getFileCount);
			register_method("getFileNameAndSize", &Steam::getFileNameAndSize);
			register_method("getQuota", &Steam::getQuota);
			register_method("getSyncPlatforms", &Steam::getSyncPlatforms);
			register_method("isCloudEnabledForAccount", &Steam::isCloudEnabledForAccount);
			register_method("isCloudEnabledForApp", &Steam::isCloudEnabledForApp);
			register_method("setCloudEnabledForApp", &Steam::setCloudEnabledForApp);
			// Screenshoot Bind Methods /////////////////
			register_method("hookScreenshots", &Steam::hookScreenshots);
			register_method("isScreenshotsHooked", &Steam::isScreenshotsHooked);
			register_method("triggerScreenshot", &Steam::triggerScreenshot);
			register_method("writeScreenshot", &Steam::writeScreenshot);
			// User Bind Methods ////////////////////////
//			register_method("getAuthSessionTicket", &Steam::getAuthSessionTicket);
//			register_method("cancelAuthTicket", &Steam::cancelAuthTicket);
//			register_method("beginAuthSession", &Steam::beginAuthSession);
//			register_method("endAuthSession", &Steam::endAuthSession);
			register_method("getSteamID", &Steam::getSteamID);
			register_method("loggedOn", &Steam::loggedOn);
			register_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
			register_method("getUserDataFolder", &Steam::getUserDataFolder);
//			register_method("advertiseGame", &Steam::advertiseGame);
			register_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
			// User Stats Bind Methods //////////////////
			register_method("clearAchievement", &Steam::clearAchievement);
			register_method("getAchievement", &Steam::getAchievement);
			register_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
			register_method("getNumAchievements", &Steam::getNumAchievements);
			register_method("getNumberOfCurrentPlayers", &Steam::getNumberOfCurrentPlayers);
			register_method("getStatFloat", &Steam::getStatFloat);
			register_method("getStatInt", &Steam::getStatInt);
			register_method("resetAllStats", &Steam::resetAllStats);
			register_method("requestCurrentStats", &Steam::requestCurrentStats);
			register_method("requestGlobalAchievementPercentages", &Steam::requestGlobalAchievementPercentages);
			register_method("setAchievement", &Steam::setAchievement);
			register_method("setStatFloat", &Steam::setStatFloat);
			register_method("setStatInt", &Steam::setStatInt);
			register_method("storeStats", &Steam::storeStats);
			register_method("findLeaderboard", &Steam::findLeaderboard);
			register_method("getLeaderboardName", &Steam::getLeaderboardName);
			register_method("getLeaderboardEntryCount", &Steam::getLeaderboardEntryCount);
			register_method("downloadLeaderboardEntries", &Steam::downloadLeaderboardEntries);
			register_method("downloadLeaderboardEntriesForUsers", &Steam::downloadLeaderboardEntriesForUsers);
			register_method("uploadLeaderboardScore", &Steam::uploadLeaderboardScore);
			register_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
			register_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
			register_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);
			// Utils Bind Methods ///////////////////////
			register_method("getIPCountry", &Steam::getIPCountry);
			register_method("isOverlayEnabled", &Steam::isOverlayEnabled);
			register_method("getSteamUILanguage", &Steam::getSteamUILanguage);
			register_method("getAppID", &Steam::getAppID);
			register_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
			register_method("setOverlayNotificationPosition", &Steam::setOverlayNotificationPosition);
			register_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
			register_method("getServerRealTime", &Steam::getServerRealTime);
			register_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
			register_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
			register_method("startVRDashboard", &Steam::startVRDashboard);
			// Workshop Bind Methods ////////////////////
			register_method("getNumSubscribedItems", &Steam::getNumSubscribedItems);
			register_method("getItemState", &Steam::getItemState);
			register_method("downloadItem", &Steam::downloadItem);
			register_method("suspendDownloads", &Steam::suspendDownloads);
			// Signals //////////////////////////////////
			Dictionary args;

			args["from"] = Variant::INT;
			args["connect_string"] = Variant::STRING;
			register_signal<Steam>("join_requested", args);
			args.clear();

			args["size"] = Variant::INT;
			register_signal<Steam>("avatar_loaded", args);
			args.clear();

			args["success"] = Variant::BOOL;
			args["players"] = Variant::INT;
			register_signal<Steam>("number_of_current_players", args);
			args.clear();

			args["leaderboard"] = Variant::INT;
			args["found"] = Variant::INT;
			register_signal<Steam>("leaderboard_loaded", args);
			args.clear();

			args["success"] = Variant::BOOL;
			args["score"] = Variant::INT;
			args["score_changed"] = Variant::BOOL;
			args["global_rank_new"] = Variant::INT;
			args["global_rank_previous"] = Variant::INT;
			register_signal<Steam>("leaderboard_uploaded", args);
			args.clear();

			register_signal<Steam>("leaderboard_entries_loaded");

			args["active"] = Variant::BOOL;
			register_signal<Steam>("overlay_toggled", args);
			args.clear();

			args["power"] = Variant::INT;
			register_signal<Steam>("low_power", args);
			args.clear();

			args["lobby"] = Variant::INT;
			register_signal<Steam>("lobby_created", args);
			args.clear();

			args["lobby"] = Variant::INT;
			args["permissions"] = Variant::INT;
			args["locked"] = Variant::BOOL;
			args["response"] = Variant::INT;
			register_signal<Steam>("lobby_joined", args);
			args.clear();

			args["inviter"] = Variant::INT;
			args["lobby"] = Variant::INT;
			args["game"] = Variant::INT;
			register_signal<Steam>("lobby_invite", args);
			args.clear();

			args["connected"] = Variant::BOOL;
			register_signal<Steam>("connection_changed", args);
			args.clear();

			args["app"] = Variant::INT;
			register_signal<Steam>("dlc_installed", args);
			args.clear();

			args["ticket"] = Variant::INT;
			args["result"] = Variant::INT;
			register_signal<Steam>("get_auth_session_ticket_response", args);
			args.clear();

			args["steamID"] = Variant::INT;
			args["auth_session_reponse"] = Variant::INT;
			args["owner_steamID"] = Variant::INT;
			register_signal<Steam>("validate_auth_ticket_response", args);
			args.clear();

			args["screenshot_handle"] = Variant::INT;
			args["result"] = Variant::INT;
			register_signal<Steam>("screenshot_ready", args);
			args.clear();

			args["gameID"] = Variant::INT;
			args["result"] = Variant::INT;
			args["userID"] = Variant::INT;
			register_signal<Steam>("user_stats_received", args);
			args.clear();

			args["gameID"] = Variant::INT;
			args["result"] = Variant::INT;
			register_signal<Steam>("global_achievement_percentages_ready", args);
			args.clear();
		}
};
#endif // GODOTSTEAM_H