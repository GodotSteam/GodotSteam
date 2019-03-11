#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include "steam/steam_gameserver.h"
#include "steam/steam_api.h"

#include "core/Godot.hpp"
#include "Reference.hpp"
#include "Image.hpp"			// For avatars; perhaps should be Texture.hpp
#include "core/Dictionary.hpp" 	// Contains array.h as well

//#define USE_GS_AUTH_API

// INADDR_ANY constant, can be found in winsock.h on Windows
#define INADDR_ANY (uint32)0x00000000

using namespace godot;

class Steam : public GodotScript<Reference>{
	GODOT_CLASS(Steam);

	public:
		enum {
			OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
			TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
			OK=0,FAILED=1,ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
			AUTH_SESSION_OK=0, AUTH_SESSION_STEAM_NOT_CONNECTED=1, AUTH_SESSION_NO_LICENSE=2, AUTH_SESSION_VAC_BANNED=3, AUTH_SESSION_LOGGED_IN_ELSEWHERE=4,
			AUTH_SESSION_VAC_CHECK_TIMEOUT=5, AUTH_SESSION_TICKET_CANCELED=6, AUTH_SESSION_TICKET_ALREADY_USED=7, AUTH_SESSION_TICKET_INVALID=8, AUTH_SESSION_PUBLISHER_BANNED=9,
			AVATAR_SMALL=0, AVATAR_MEDIUM, AVATAR_LARGE,
			GLOBAL=0, GLOBAL_AROUND_USER=1, FRIENDS=2, USERS=3,
			LOBBY_OK=0, LOBBY_NO_CONNECTION=1, LOBBY_TIMEOUT=2, LOBBY_FAIL=3, LOBBY_ACCESS_DENIED=4, LOBBY_LIMIT_EXCEEDED=5,
			PRIVATE=0, FRIENDS_ONLY=1, PUBLIC=2, INVISIBLE=3, LOBBY_KEY_LENGTH=255,
			UGC_MAX_TITLE_CHARS=128, UGC_MAX_DESC_CHARS=8000, UGC_MAX_METADATA_CHARS=5000,
			UGC_ITEM_COMMUNITY=0, UGC_ITEM_MICROTRANSACTION=1, UGC_ITEM_COLLECTION=2, UGC_ITEM_ART=3, UGC_ITEM_VIDEO=4, UGC_ITEM_SCREENSHOT=5, UGC_ITEM_GAME=6, UGC_ITEM_SOFTWARE=7,
			UGC_ITEM_CONCEPT=8, UGC_ITEM_WEBGUIDE=9, UGC_ITEM_INTEGRATEDGUIDE=10, UGC_ITEM_MERCH=11, UGC_ITEM_CONTROLLERBINDING=12, UGC_ITEM_STEAMWORKSACCESSINVITE=13,
			UGC_ITEM_STEAMVIDEO=14, UGC_ITEM_GAMEMANAGEDITEM=15, UGC_ITEM_MAX=16,
			UGC_STATE_NONE=0, UGC_STATE_SUBSCRIBED=1, UGC_STATE_LEGACY=2, UGC_STATE_INSTALLED=4, UGC_STATE_UPDATE=8, UGC_STATE_DOWNLOADING=16, UGC_STATE_PENDING=32,
			UGC_FILE_VISIBLE_PUBLIC=0, UGC_FILE_VISIBLE_FRIENDS=1, UGC_FILE_VISIBLE_PRIVATE=2,
			STATUS_INVALID=0, STATUS_PREPARING_CONFIG=1, STATUS_PREPARING_CONTENT=2, STATUS_UPLOADING_CONTENT=3, STATUS_UPLOADING_PREVIEW=4, STATUS_COMMITTING_CHANGES=5,
			REMOTE_STORAGE_PLATFORM_NONE=0, REMOTE_STORAGE_PLATFORM_WINDOWS=(1<<0), REMOTE_STORAGE_PLATFORM_OSX=(1<<1), REMOTE_STORAGE_PLATFORM_PS3=(1<<2), 
			REMOTE_STORAGE_PLATFORM_LINUX=(1<<3), REMOTE_STORAGE_PLATFORM_RESERVED2=(1<<4), REMOTE_STORAGE_PLATFORM_ALL=0xffffffff
		};
		static Steam *get_singleton();
		Steam();
		~Steam();

		CSteamID createSteamID(uint32_t steamID, int accountType/*=-1*/);
		CSteamID createSteamID(uint64_t steamID);
		Image drawAvatar(int size, uint8* buffer);
		// Steamworks ///////////////////////////////
		bool restartAppIfNecessary(int value);
		bool steamInit();
		bool initGameServer(String product, String gameDescription, String versionString, String modDir, uint16_t usSteamPort, uint16_t usGamePort, uint16_t usMasterServerUpdaterPort);
		uint32 getServerPublicIP();
		bool isSteamRunning();
		// Apps /////////////////////////////////////
		bool isSubscribed();
		bool isLowViolence();
		bool isCybercafe();
		bool isVACBanned();
		String getCurrentGameLanguage();
		String getAvailableGameLanguages();
		bool isSubscribedApp(int value);
		bool isDLCInstalled(int value);
		int getEarliestPurchaseUnixTime(int value);
		bool isSubscribedFromFreeWeekend();
		int getDLCCount();
		Array getDLCDataByIndex();
		void installDLC(int value);
		void uninstallDLC(int value);
		String getCurrentBetaName();
		bool markContentCorrupt(bool missingFilesOnly);
		//	uint32_t getInstalledDepots(int appID, uint32* depots, uint32 maxDepots);
		String getAppInstallDir(AppId_t appID);
		bool isAppInstalled(int value);
		uint64_t getAppOwner();
		String getLaunchQueryParam(String key);
	//	bool getDLCDownloadProgress(int appID, uint64* bytesDownloaded, uint64* bytesTotal);
		int getAppBuildId();
		void getFileDetails(String filename);
		// Controller ///////////////////////////////
		void activateActionSet(uint64_t controllerHandle, uint64_t actionSetHandle);
		uint64_t getActionSetHandle(String actionSetName);
		Dictionary getAnalogActionData(uint64_t controllerHandle, uint64_t analogActionHandle);
		uint64_t getAnalogActionHandle(String actionName);
		Array getAnalogActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t analogActionHandle);
		Array getConnectedControllers();
		uint64_t getControllerForGamepadIndex(int index);
		uint64_t getCurrentActionSet(uint64_t controllerHandle);
		Dictionary getDigitalActionData(uint64_t controllerHandle, uint64_t digitalActionHandle);
		uint64_t getDigitalActionHandle(String actionName);
		Array getDigitalActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t digitalActionHandle);
		int getGamepadIndexForController(uint64_t controllerHandle);
		Dictionary getMotionData(uint64_t controllerHandle);
		bool init();
		void runFrame();
		bool showBindingPanel(uint64_t controllerHandle);
		bool shutdown();
		void triggerVibration(uint64_t controllerHandle, uint16_t leftSpeed, uint16_t rightSpeed);
		// Friends //////////////////////////////////
		int getFriendCount();
		String getPersonaName();
		String getFriendPersonaName(uint64_t steamID);
		void setGameInfo(String key, String value);
		void clearGameInfo();
		void inviteFriend(uint64_t id, String connectString);
		void setPlayedWith(uint64_t steamID);
		Array getRecentPlayers();
		void getFriendAvatar(int size=AVATAR_MEDIUM);
		Array getUserSteamGroups();
		Array getUserSteamFriends();
		void activateGameOverlay(String type);
		void activateGameOverlayToUser(String type, uint64_t steamID);
		void activateGameOverlayToWebPage(String url);
		void activateGameOverlayToStore(AppId_t appID=0);
		void activateGameOverlayInviteDialog(int steamID);
		// Matchmaking //////////////////////////////
		void createLobby(int lobbyType, int maxMembers);
		void requestLobbyList();
		void joinLobby(uint64_t steamIDLobby);
		void leaveLobby(uint64_t steamIDLobby);
		bool inviteUserToLobby(uint64_t steamIDLobby, uint64_t steamIDInvitee);
		Array getLobbyData(uint64_t steamIDLobby);
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
		bool fileWrite(String file, const PoolByteArray& vData, int32_t cubData);
		Dictionary fileRead(String file, int32_t cubDataToRead);
		bool fileForget(String file);
		bool fileDelete(String file);
		bool fileExists(String file);
		bool filePersisted(String file);
		int32_t getFileSize(String file);
		int64_t getFileTimestamp(String file);
		int32_t getFileCount();
		Dictionary getFileNameAndSize(int file);
		Dictionary getQuota();
		uint32_t getSyncPlatforms(String file);
		bool isCloudEnabledForAccount();
		bool isCloudEnabledForApp();
		void setCloudEnabledForApp(bool enabled);
		// Screenshots //////////////////////////////
		uint32_t addScreenshotToLibrary(String filename, String thumbnailFilename, int width, int height);
		void hookScreenshots(bool hook);
		bool isScreenshotsHooked();
		bool setLocation(uint32_t screenshot, String location);
		void triggerScreenshot();
		uint32_t writeScreenshot(const PoolByteArray& RGB, int width, int height);
		// Users ////////////////////////////////////
//		uint32_t getAuthSessionTicket();
//		void cancelAuthTicket(uint32_t authTicket);
//		int beginAuthSession(uint32_t authTicket, uint64_t steamID);
//		void endAuthSession(uint64_t steamID);
		uint64_t getSteamID();
		AccountID_t getSteamAccountID();
		bool loggedOn();
		int getPlayerSteamLevel();
		String getUserDataFolder();
//		void advertiseGame(String serverIP, int port);
		int getGameBadgeLevel(int series, bool foil);
		// User Stats ///////////////////////////////
		bool clearAchievement(String name);
		uint32_t getNumAchievements();
		void getNumberOfCurrentPlayers();
		bool getAchievement(String name);
		Dictionary getAchievementAchievedPercent(String name);
		String getAchievementDisplayAttribute(String name, String key);
		int getAchievementIcon(String name);
		String getAchievementName(uint32_t iAchievement);
		float getStatFloat(String name);
		int getStatInt(String name);
		bool resetAllStats(bool achievementsToo=true);
		bool requestCurrentStats();
		void requestGlobalAchievementPercentages();
		bool setAchievement(String name);
		bool setStatFloat(String name, float value);
		bool setStatInt(String name, int value);
		bool storeStats();
		void findLeaderboard(String name);
		String getLeaderboardName();
		int getLeaderboardEntryCount();
		void downloadLeaderboardEntries(int start, int end, int type=GLOBAL);
		void downloadLeaderboardEntriesForUsers(Array usersID);
		void uploadLeaderboardScore(int score, bool keepBest=false);
		void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount);
		uint64_t getLeaderboardHandle();
		Array getLeaderboardEntries();
		Array getLobbiesList();
		bool getAchievementAndUnlockTime(String name);  // TODO: rework return values
		bool indicateAchievementProgress(String name, int curProgress, int maxProgress);
		// Utils ////////////////////////////////////
		String getIPCountry();
		bool isOverlayEnabled();
		String getSteamUILanguage();
		int getAppID();
		Dictionary getImageRGBA(int iImage);
		Dictionary getImageSize(int iImage);
		int getSecondsSinceAppActive();
		void setOverlayNotificationPosition(int pos);
		int getCurrentBatteryPower();
		bool isSteamRunningInVR();
		int getServerRealTime();
		bool isSteamInBigPictureMode();
		void startVRDashboard();
		// Workshop /////////////////////////////////
		bool downloadItem(int publishedFileID, bool highPriority);
		void suspendDownloads(bool suspend);
		uint64_t startItemUpdate(AppId_t appID, int fileId);
		int getItemState(int publishedFileID);
	//	int getItemUpdateProgress(uint64_t handle, uint64_t *bytesProcessed, uint64_t* bytesTotal);
		void createItem(AppId_t appID, int fileType);
		bool setItemTitle(uint64_t updateHandle, String title);
		bool setItemDescription(uint64_t updateHandle, String description);
		bool setItemUpdateLanguage(uint64_t updateHandle, String language);
		bool setItemMetadata(uint64_t updateHandle, String metadata);
		bool setItemVisibility(uint64_t updateHandle, int visibility);
	//	bool setItemTags(uint64_t updateHandle, const PoolStringArray ** stringArray, const int32 stringCount);
		bool setItemContent(uint64_t updateHandle, String contentFolder);
		bool setItemPreview(uint64_t updateHandle, String previewFile);
		void submitItemUpdate(uint64_t updateHandle, String changeNote);
	//	Array getSubscribedItems();
	//	bool getItemInstallInfo(int fileID, uint64_t *sizeOnDisk, char *folder, uint32_t folderSize, uint32_t *timeStamp);
	//	bool getItemDownloadInfo(int fileID, uint64_t *bytesDownloaded, uint64_t *bytesTotal);

	protected:
		static void _bind_methods();
		static Steam *singleton;

	private:
		bool isInitSuccess;
		bool requestingLobbies = false;
		// Leaderboards
		SteamLeaderboard_t leaderboard_handle;
		Array leaderboard_entries;
		Array listLobbies;
		Array lobbyData;
		// Authentication
		struct TicketData {
			uint32_t id;
			uint32_t *buf;
			uint32_t size;
		};
//		Vector<TicketData> tickets;	//-> Errors with 'Vector' does not name a type
		/////////////////////////////////////////
		// Steam Callbacks //////////////////////
		//

		// Tells us when we have successfully connected to Steam
		STEAM_GAMESERVER_CALLBACK(Steam, _steam_servers_connected, SteamServersConnected_t);

		// Tells us when there was a failure to connect to Steam
		STEAM_GAMESERVER_CALLBACK(Steam, _steam_servers_connect_failure, SteamServerConnectFailure_t);

		// Apps callbacks
		CCallResult<Steam, DlcInstalled_t> callInstallDLC;
		void _dlc_installed(DlcInstalled_t *callData);
		CCallResult<Steam, FileDetailsResult_t> callResultFileDetails;
		void _file_details_result(FileDetailsResult_t *callData);
		// Friends callbacks
		CCallResult<Steam, AvatarImageLoaded_t> callImageLoaded;
		void _avatar_loaded(AvatarImageLoaded_t *callData);
		// Matchmaking callbacks
		CCallResult<Steam, LobbyCreated_t> callCreatedLobby;
		void _lobby_created(LobbyCreated_t *callData, bool bIOFailure);
		CCallResult<Steam, LobbyMatchList_t> steamCallResultLobbyMatchList;
		void _lobby_match_list(LobbyMatchList_t *pCallback, bool bIOFailure);
		CCallResult<Steam, LobbyEnter_t> callEnteredLobby;
		void _lobby_joined(LobbyEnter_t *callData);
		CCallResult<Steam, LobbyInvite_t> callInviteLobby;
		void _lobby_invite(LobbyInvite_t *callData);
		CCallResult<Steam, GameRichPresenceJoinRequested_t> callJoinRequested;
		void _join_requested(GameRichPresenceJoinRequested_t *callData);
		CCallResult<Steam, SteamServersConnected_t> callServerConnected;
		void _server_connected(SteamServersConnected_t *callData);
		CCallResult<Steam, SteamServersDisconnected_t> callServerDisconnected;
		void _server_disconnected(SteamServersDisconnected_t *callData);
		// Screenshot callbacks
		CCallResult<Steam, ScreenshotReady_t> callReadyScreenshot;
		void _screenshot_ready(ScreenshotReady_t *callData);
		// User callbacks
		CCallResult<Steam, GetAuthSessionTicketResponse_t> callAuthResponse;
		void _get_auth_session_ticket_response(GetAuthSessionTicketResponse_t *callData);
		CCallResult<Steam, ValidateAuthTicketResponse_t> callAuthValidation;
		void _validate_auth_ticket_response(ValidateAuthTicketResponse_t *callData);
		// User stat callbacks		
		CCallResult<Steam, NumberOfCurrentPlayers_t> callResultNumberOfCurrentPlayers;
		void _number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure);
		CCallResult<Steam, UserStatsReceived_t> callUserStatsReceived;
		void _user_stats_received(UserStatsReceived_t *callData);
		CCallResult<Steam, UserAchievementIconFetched_t> callUserAchieveIconFetched;
		void _user_achievement_icon_fetched(UserAchievementIconFetched_t *callData);
		CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
		void _leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure);
		CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
		void _leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure);
		CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
		void _leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);
		CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
		void _global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure);
		// Utility callbacks
		CCallResult<Steam, GameOverlayActivated_t> callOverlayActivated;
		void _overlay_toggled(GameOverlayActivated_t *callData);
		CCallResult<Steam, LowBatteryPower_t> callLowPower;
		void _low_power(LowBatteryPower_t *callData);
		// Workshop callbacks
		CCallResult<Steam, ItemInstalled_t> callResultItemInstalled;
		void _workshop_item_installed(ItemInstalled_t *callData);
		CCallResult<Steam, CreateItemResult_t> callResultItemCreate;
		void _workshop_item_created(CreateItemResult_t *callData, bool bIOFailure);
		CCallResult<Steam, SubmitItemUpdateResult_t> callResultItemUpdate;
		void _workshop_item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure);
		// Run callbacks
		void run_callbacks(){
			SteamAPI_RunCallbacks();
		}
		void run_server_callbacks() {
			SteamGameServer_RunCallbacks();
		}

	public:
		// Register methods /////////////////////////
		static void _register_methods(){
			register_method("restartAppIfNecessary", &Steam::restartAppIfNecessary);
			register_method("steamInit", &Steam::steamInit);
			register_method("initGameServer", &Steam::initGameServer);
			register_method("getServerPublicIP", &Steam::getServerPublicIP);
			register_method("isSteamRunning", &Steam::isSteamRunning);
			register_method("run_callbacks", &Steam::run_callbacks);
			register_method("run_server_callbacks", &Steam::run_server_callbacks);
			// Apps Bind Methods ////////////////////////
			register_method("isSubscribed", &Steam::isSubscribed);
			register_method("isLowViolence", &Steam::isLowViolence);
			register_method("isCybercafe", &Steam::isCybercafe);
			register_method("isVACBanned", &Steam::isVACBanned);
			register_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
			register_method("getAvailableGameLanguages", &Steam::getAvailableGameLanguages);
			register_method("isSubscribedApp", &Steam::isSubscribedApp);
			register_method("isDLCInstalled", &Steam::isDLCInstalled);
			register_method("getEarliestPurchaseUnixTime", &Steam::getEarliestPurchaseUnixTime);
			register_method("isSubscribedFromFreeWeekend", &Steam::isSubscribedFromFreeWeekend);
			register_method("getDLCCount", &Steam::getDLCCount);
			register_method("getDLCDataByIndex", &Steam::getDLCDataByIndex);
			register_method("installDLC", &Steam::installDLC);
			register_method("uninstallDLC", &Steam::uninstallDLC);
			register_method("getCurrentBetaName", &Steam::getCurrentBetaName);
			register_method("markContentCorrupt", &Steam::markContentCorrupt);
		//	register_method("getInstalledDepots", &Steam::getInstalledDepots);
			register_method("getAppInstallDir", &Steam::getAppInstallDir);
			register_method("isAppInstalled", &Steam::isAppInstalled);
			register_method("getAppOwner", &Steam::getAppOwner);
			register_method("getLaunchQueryParam", &Steam::getLaunchQueryParam);
		//	register_method("getDLCDownloadProgress", &Steam::getDLCDownloadProgress);
			register_method("getAppBuildId", &Steam::getAppBuildId);
			register_method("getFileDetails", &Steam::getFileDetails);
			// Controllers Bind Methods /////////////////
			register_method("activateActionSet", &Steam::activateActionSet);
			register_method("getActionSetHandle", &Steam::getActionSetHandle);
			register_method("getAnalogActionData", &Steam::getAnalogActionData);
			register_method("getAnalogActionHandle", &Steam::getAnalogActionHandle);
			register_method("getAnalogActionOrigins", &Steam::getAnalogActionOrigins);
			register_method("getConnectedControllers", &Steam::getConnectedControllers);
			register_method("getControllerForGamepadIndex", &Steam::getControllerForGamepadIndex);
			register_method("getCurrentActionSet", &Steam::getCurrentActionSet);
			register_method("getDigitalActionData", &Steam::getDigitalActionData);
			register_method("getDigitalActionHandle", &Steam::getDigitalActionHandle);
			register_method("getDigitalActionOrigins", &Steam::getDigitalActionOrigins);
			register_method("getMotionData", &Steam::getMotionData);
			register_method("init", &Steam::init);
			register_method("runFrame", &Steam::runFrame);
			register_method("showBindingPanel", &Steam::showBindingPanel);
			register_method("shutdown", &Steam::shutdown);
			register_method("triggerVibration", &Steam::triggerVibration);
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
			register_method("requestLobbyList", &Steam::requestLobbyList);
			register_method("getLobbiesList", &Steam::getLobbiesList);
			register_method("joinLobby", &Steam::joinLobby);
			register_method("leaveLobby", &Steam::leaveLobby);
			register_method("inviteUserToLobby", &Steam::inviteUserToLobby);
			register_method("getLobbyData", &Steam::getLobbyData);
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
			register_method("addScreenshotToLibrary", &Steam::addScreenshotToLibrary);
			register_method("hookScreenshots", &Steam::hookScreenshots);
			register_method("isScreenshotsHooked", &Steam::isScreenshotsHooked);
			register_method("setLocation", &Steam::setLocation);
			register_method("triggerScreenshot", &Steam::triggerScreenshot);
			register_method("writeScreenshot", &Steam::writeScreenshot);
			// User Bind Methods ////////////////////////
//			register_method("getAuthSessionTicket", &Steam::getAuthSessionTicket);
//			register_method("cancelAuthTicket", &Steam::cancelAuthTicket);
//			register_method("beginAuthSession", &Steam::beginAuthSession);
//			register_method("endAuthSession", &Steam::endAuthSession);
			register_method("getSteamID", &Steam::getSteamID);
			register_method("getSteamAccountID", &Steam::getSteamAccountID);
			register_method("loggedOn", &Steam::loggedOn);
			register_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
			register_method("getUserDataFolder", &Steam::getUserDataFolder);
//			register_method("advertiseGame", &Steam::advertiseGame);
			register_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
			// User Stats Bind Methods //////////////////
			register_method("clearAchievement", &Steam::clearAchievement);
			register_method("getAchievement", &Steam::getAchievement);
			register_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
			register_method("getAchievementDisplayAttribute", &Steam::getAchievementDisplayAttribute);
			register_method("getAchievementIcon", &Steam::getAchievementIcon);
			register_method("getAchievementName", &Steam::getAchievementName);
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
			register_method("getImageRGBA", &Steam::getImageRGBA);
			register_method("getImageSize", &Steam::getImageSize);
			register_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
			register_method("setOverlayNotificationPosition", &Steam::setOverlayNotificationPosition);
			register_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
			register_method("getServerRealTime", &Steam::getServerRealTime);
			register_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
			register_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
			register_method("startVRDashboard", &Steam::startVRDashboard);
			// Workshop Bind Methods ////////////////////
			register_method("downloadItem", &Steam::downloadItem);
			register_method("suspendDownloads", &Steam::suspendDownloads);
			register_method("startItemUpdate", &Steam::startItemUpdate);
			register_method("getItemState", &Steam::getItemState);
		//	register_method("getItemUpdateProgress", &Steam::getItemUpdateProgress);
			register_method("createItem", &Steam::createItem);
			register_method("setItemTitle", &Steam::setItemTitle);
			register_method("setItemDescription", &Steam::setItemDescription);
			register_method("setItemUpdateLanguage", &Steam::setItemUpdateLanguage);
			register_method("setItemMetadata", &Steam::setItemMetadata);
			register_method("setItemVisibility", &Steam::setItemVisibility);
		//	register_method("setItemTags", &Steam::setItemTags);
			register_method("setItemContent", &Steam::setItemContent);
			register_method("setItemPreview", &Steam::setItemPreview);
			register_method("submitItemUpdate", &Steam::submitItemUpdate);
		//	register_method("getSubscribedItems", &Steam::getSubscribedItems);
		//	register_method("getItemInstallInfo", &Steam::getItemInstallInfo);
		//	register_method("getItemDownloadInfo", &Steam::getItemDownloadInfo);
			// Signals //////////////////////////////////
			Dictionary args;
			args["result"] = Variant::INT;
			args["fileSize"] = Variant::INT;
			args["fileHash"] = Variant::INT;
			args["flags"] = Variant::INT;
			register_signal<Steam>("file_details_result", args);
			args.clear();

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

			register_signal<Steam>("lobby_match_list");

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
			args["userID"] = Variant::INT;
			register_signal<Steam>("user_achievement_icon_fetched", args);
			args.clear();

			args["gameID"] = Variant::INT;
			args["result"] = Variant::INT;
			register_signal<Steam>("global_achievement_percentages_ready", args);
			args.clear();

			register_signal<Steam>("workshop_item_created");

			register_signal<Steam>("workshop_item_updated");

			register_signal<Steam>("workshop_item_installed");
		}
};
#endif // GODOTSTEAM_H