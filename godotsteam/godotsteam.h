#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "object.h"
#include "scene/resources/texture.h"	// For avatars
#include "reference.h"
#include "dictionary.h"					// Contains array.h as well

class Steam: public Object {
	GDCLASS(Steam, Object);
public:
	enum {
		OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
		TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
		ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
		AVATAR_SMALL=0, AVATAR_MEDIUM, AVATAR_LARGE,
		GLOBAL=0, GLOBAL_AROUND_USER=1, FRIENDS=2, USERS=3,
		LOBBY_OK=0, LOBBY_NO_CONNECTION=1, LOBBY_TIMEOUT=2, LOBBY_FAIL=3, LOBBY_ACCESS_DENIED=4, LOBBY_LIMIT_EXCEEDED=5,
		PRIVATE=0, FRIENDS_ONLY=1, PUBLIC=2, INVISIBLE=3, LOBBY_KEY_LENGTH=255,
		UGC_MAX_TITLE_CHARS=128, UGC_MAX_DESC_CHARS=8000, UGC_MAX_METADATA_CHARS=5000,
		UGC_ITEM_COMMUNITY=0, UGC_ITEM_MICROTRANSACTION=1,
		UGC_STATE_NONE=0, UGC_STATE_SUBSCRIBED=1, UGC_STATE_LEGACY=2, UGC_STATE_INSTALLED=4, UGC_STATE_UPDATE=8, UGC_STATE_DOWNLOADING=16, UGC_STATE_PENDING=32,
		STATUS_INVALID=0, STATUS_PREPARING_CONFIG=1, STATUS_PREPARING_CONTENT=2, STATUS_UPLOADING_CONTENT=3, STATUS_UPLOADING_PREVIEW=4, STATUS_COMMITTING_CHANGES=5
	};
	static Steam* get_singleton();
	Steam();
	~Steam();

	CSteamID createSteamID(uint32 steamID, int accountType=-1);
	Image drawAvatar(int size, uint8* buffer);
	// Steamworks ///////////////////////////////
	bool restartAppIfNecessary(int value);
	bool steamInit();
	bool isSteamRunning();
	// Apps /////////////////////////////////////
	bool hasOtherApp(int value);
	int getDLCCount();
	bool isDLCInstalled(int value);
	void requestAppProofOfPurchaseKey(int value);
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
	String getFriendPersonaName(int steam_id);
	void setGameInfo(const String& s_key, const String& s_value);
	void clearGameInfo();
	void inviteFriend(int id, const String& conString);
	void setPlayedWith(int steam_id);
	Array getRecentPlayers();
	void getFriendAvatar(int size=AVATAR_MEDIUM);
	Array getUserSteamGroups();
	Array getUserSteamFriends();
	void activateGameOverlay(const String& type);
	void activateGameOverlayToUser(const String& type, int steam_id);
	void activateGameOverlayToWebPage(const String& url);
	void activateGameOverlayToStore(int appid=0);
	void activateGameOverlayInviteDialog(int id);
	// Matchmaking //////////////////////////////
	void createLobby(int lobbyType, int cMaxMembers);
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
	bool fileWrite(const String& chFile, const PoolByteArray& vData, int32 cubData);
	Dictionary fileRead(const String& chFile, int32 cubDataToRead);
	bool fileForget(const String& chFile);
	bool fileDelete(const String& chFile);
	bool fileExists(const String& chFile);
	bool filePersisted(const String& chFile);
	int32 getFileSize(const String& chFile);
	int32 getFileTimestamp(const String& chFile);
	int32 getFileCount();
	bool isCloudEnabledForAccount();
	bool isCloudEnabledForApp();
	void setCloudEnabledForApp(bool bEnabled);
	// Screenshots //////////////////////////////
	void triggerScreenshot();
	// Users ////////////////////////////////////
	int getSteamID();
	bool loggedOn();
	int getPlayerSteamLevel();
	String getUserDataFolder();
	void advertiseGame(const String& server_ip, int port);
	int getGameBadgeLevel(int series, bool foil);
	// User Stats ///////////////////////////////
	bool clearAchievement(const String& s_key);
	bool getAchievement(const String& s_key);
	float getStatFloat(const String& s_key);
	int getStatInt(const String& s_key);
	bool resetAllStats(bool bAchievementsToo=true);
	bool requestCurrentStats();
	bool setAchievement(const String& s_key);
	bool setStatFloat(const String& s_key, float value);
	bool setStatInt(const String& s_key, int value);
	bool storeStats();
	void findLeaderboard(const String& lName);
	String getLeaderboardName();
	int getLeaderboardEntryCount();
	void downloadLeaderboardEntries(int rStart, int rEnd, int type=GLOBAL);
	void downloadLeaderboardEntriesForUsers(Array usersID);
	void uploadLeaderboardScore(int score, bool keepBest=false);
	void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount);
	void updateLeaderboardHandle(SteamLeaderboard_t lHandle);
	uint64 getLeaderboardHandle();
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
	bool downloadItem(int nPublishedFileID, bool bHighPriority);
	void suspendDownloads(bool bSuspend);

protected:
	static void _bind_methods();
	static Steam* singleton;
//	static void updateFriendList(int filter=9);

private:
	bool isInitSuccess;
	bool is_valid;

	SteamLeaderboard_t leaderboard_handle;
	Array leaderboard_entries;

	STEAM_CALLBACK(Steam, _lobby_created, LobbyCreated_t);
	STEAM_CALLBACK(Steam, _lobby_joined, LobbyEnter_t);
	STEAM_CALLBACK(Steam, _lobby_invite, LobbyInvite_t);
	STEAM_CALLBACK(Steam, _join_requested, GameRichPresenceJoinRequested_t);
	STEAM_CALLBACK(Steam, _overlay_toggled, GameOverlayActivated_t);
	STEAM_CALLBACK(Steam, _low_power, LowBatteryPower_t);
	STEAM_CALLBACK(Steam, _avatar_loaded, AvatarImageLoaded_t);
//	STEAM_CALLBACK(Steam, _leaderboard_loaded, LeaderboardFindResult_t);
	CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
	void _leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure);
//	STEAM_CALLBACK(Steam, _leaderboard_entries_loaded, LeaderboardScoresDownloaded_t);
	CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
	void _leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);
	STEAM_CALLBACK(Steam, _server_connected, SteamServersConnected_t);
	STEAM_CALLBACK(Steam, _server_disconnected, SteamServersDisconnected_t);
//	STEAM_CALLBACK(Steam, _request_proofofpurchase, AppProofOfPurchaseKeyResponse_t);
	STEAM_CALLBACK(Steam, _dlc_installed, DlcInstalled_t);

	void run_callbacks(){
		SteamAPI_RunCallbacks();
	}
};
#endif // GODOTSTEAM_H
