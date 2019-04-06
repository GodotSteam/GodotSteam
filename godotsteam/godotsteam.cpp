#include "godotsteam.h"
#include <steam/steam_api.h>

Steam* Steam::singleton = NULL;

Steam::Steam(){
	isInitSuccess = false;
	singleton = this;
	leaderboardDetailsMax = 0;
}

Steam* Steam::get_singleton(){
	return singleton;
}

CSteamID Steam::createSteamID(uint32_t steamID, int accountType){
	CSteamID cSteamID;
	if(accountType < 0 || accountType >= k_EAccountTypeMax){
		accountType = 1;
	}
	cSteamID.Set(steamID, EUniverse(k_EUniversePublic), EAccountType(accountType));
	return cSteamID;
}
/////////////////////////////////////////////////
///// STEAMWORKS FUNCTIONS //////////////////////
//
// Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't.
bool Steam::restartAppIfNecessary(int value){
	return SteamAPI_RestartAppIfNecessary((AppId_t)value);
}
// Initialize Steamworks
bool Steam::steamInit(){
	return SteamAPI_Init();
	//printf("Godot Steam initialing...\n");
	isInitSuccess = SteamAPI_Init();
	int err = FAILED;
	if(isInitSuccess){
		err = OK;
	}
	if(!SteamAPI_IsSteamRunning()){
		err = ERR_NO_CLIENT;
	}
	else if(!SteamUser()->BLoggedOn()){
		err = ERR_NO_CONNECTION;
	}
	if(err == OK && SteamUserStats() != NULL){
		// Load stats and achievements automatically.
		SteamUserStats()->RequestCurrentStats();
	}
	// Get this app ID
	currentAppID = SteamUtils()->GetAppID();
	return err;
}
// Returns true/false if Steam is running.
bool Steam::isSteamRunning(void){
	return SteamAPI_IsSteamRunning();
}
/////////////////////////////////////////////////
///// APPS //////////////////////////////////////
//
// Checks if the active user is subscribed to the current App ID.
bool Steam::isSubscribed(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribed();
}
// Gets the current language that the user has set.
String Steam::getCurrentGameLanguage(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetCurrentGameLanguage();
}
// Checks if the active user is subscribed to a specified AppId.
bool Steam::isSubscribedApp(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedApp((AppId_t)value);
}
// Checks if the user owns a specific DLC and if the DLC is installed
bool Steam::isDLCInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsDlcInstalled(value);
}
// Get the number of DLC the user owns for a parent application/game.
int Steam::getDLCCount(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->GetDLCCount();
}
// Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
uint64_t Steam::getAppOwner(){
	if(SteamApps() == NULL){
		return 0;
	}
	CSteamID cSteamID = SteamApps()->GetAppOwner();
	return cSteamID.ConvertToUint64();
}
// Return the build ID for this app; will change based on backend updates.
int Steam::getAppBuildId(){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetAppBuildId();
}
/////////////////////////////////////////////////
///// FRIENDS ///////////////////////////////////
//
// Get the user's Steam username.
String Steam::getPersonaName(){
	if(SteamFriends() == NULL){
		return "";
	}
	return SteamFriends()->GetPersonaName();
}
// Activates the overlay with optional dialog to open the following: "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite".
void Steam::activateGameOverlay(const String& url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
}
// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore".
void Steam::activateGameOverlayToUser(const String& url, uint64_t steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID userID = (uint64)steamID;
	SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), userID);
}
// Activates the overlay with specified web address.
void Steam::activateGameOverlayToWebPage(const String& url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToWebPage(url.utf8().get_data());
}
// Activates the overlay with the application/game Steam store page.
void Steam::activateGameOverlayToStore(int appID){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToStore(AppId_t(appID), EOverlayToStoreFlag(0));
}
/////////////////////////////////////////////////
///// SIGNALS ///////////////////////////////////
//
// Signal when overlay is triggered.
void Steam::_overlay_toggled(GameOverlayActivated_t* callData){
	if(callData->m_bActive){
		emit_signal("overlay_toggled", true);
	}
	else{
		emit_signal("overlay_toggled", false);
	}
}
// Signal when battery power is running low, less than 10 minutes left.
void Steam::_low_power(LowBatteryPower_t* timeLeft){
	uint8 power = timeLeft->m_nMinutesBatteryLeft;
	emit_signal("low_power", power);
}
// User stats are ready.
void Steam::_user_stats_received(UserStatsReceived_t* callData){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	CSteamID userID = callData->m_steamIDUser;
	uint64_t user = userID.ConvertToUint64();
	emit_signal("user_stats_received", game, result, user);
}
// User stats were stored correctly.
void Steam::_user_stats_stored(UserStatsStored_t* callData){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	emit_signal("user_stats_stored", game, result);
}
// User achievement was stored correctly.
void Steam::_user_achievement_stored(UserAchievementStored_t* callData){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	bool groupAchieve = callData->m_bGroupAchievement;
	String name = callData->m_rgchAchievementName;
	uint32_t currentProgress = callData->m_nCurProgress;
	uint32_t maxProgress = callData->m_nMaxProgress;
	emit_signal("user_achievement_stored", game, groupAchieve, name, currentProgress, maxProgress);
}
// Global achievements percentages are ready.
void Steam::_global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t* callData, bool bIOFailure){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	emit_signal("global_achievement_percentages_ready", game, result);
}
// Signal a leaderboard has been loaded or has failed.
void Steam::_leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure){
	leaderboardHandle = callData->m_hSteamLeaderboard;
	uint8_t found = callData->m_bLeaderboardFound;
	emit_signal("leaderboard_loaded", (uint64_t)leaderboardHandle, found);
}
// Signal a leaderboard entry has been uploaded.
void Steam::_leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure){
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboardHandle){
		return;
	}
	emit_signal("leaderboard_uploaded", callData->m_bSuccess, callData->m_nScore, callData->m_bScoreChanged, callData->m_nGlobalRankNew, callData->m_nGlobalRankPrevious);
}
// Signal leaderboard entries are downloaded.
void Steam::_leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure){
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboardHandle){
		return;
	}
	getDownloadedLeaderboardEntry(callData->m_hSteamLeaderboardEntries, callData->m_cEntryCount);
	emit_signal("leaderboard_entries_loaded");
}
/////////////////////////////////////////////////
///// USERS /////////////////////////////////////
//
// Get user's Steam ID.
uint64_t Steam::getSteamID(){
	if(SteamUser() == NULL){
		return 0;
	}
	CSteamID steamID = SteamUser()->GetSteamID();
	return steamID.ConvertToUint64();
}
// Check, true/false, if user is logged into Steam currently.
bool Steam::loggedOn(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BLoggedOn();
}
// Get the user's Steam level.
int Steam::getPlayerSteamLevel(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUser()->GetPlayerSteamLevel(); 
}
// Trading Card badges data access, if you only have one set of cards, the series will be 1.
// The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1).
int Steam::getGameBadgeLevel(int series, bool foil){
	if(SteamUser()== NULL){
		return 0;
	}
	return SteamUser()->GetGameBadgeLevel(series, foil);
}
/////////////////////////////////////////////////
///// USER STATS ////////////////////////////////
//
// Clears a given achievement.
bool Steam::clearAchievement(const String& name){
	if(SteamUserStats() == NULL){
		return false;
	}
	return SteamUserStats()->ClearAchievement(name.utf8().get_data());
}
// Return true/false if use has given achievement.
Dictionary Steam::getAchievement(const String& name){
	Dictionary achieve;
	bool achieved = false;
	if(SteamUserStats() == NULL){
		achieve["ret"] = false;
	}
	else{
		achieve["ret"] = SteamUserStats()->GetAchievement(name.utf8().get_data(), &achieved);
	}
	achieve["achieved"] = achieved;
	return achieve;
}
// Returns the percentage of users who have unlocked the specified achievement.
Dictionary Steam::getAchievementAchievedPercent(const String& name){
	Dictionary d;
	float percent = 0.f;
	if(SteamUserStats() == NULL){
		d["ret"] = false;
	} else {
		d["ret"] = SteamUserStats()->GetAchievementAchievedPercent(name.utf8().get_data(), &percent);
	}
	d["percent"] = percent;
	return d;
}
// Gets the 'API name' for an achievement index
String Steam::getAchievementName(uint32_t iAchievement){
	if(SteamUserStats() == NULL){
		return "";
	}
	return SteamUserStats()->GetAchievementName((uint32)iAchievement);
}
// Get the value of a float statistic.
float Steam::getStatFloat(const String& name){
	float statValue = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statValue);
	return statValue;
}
// Get the value of an integer statistic.
int Steam::getStatInt(const String& name){
	int32_t statValue = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statValue);
	return statValue;
}
// Reset all Steam statistics; optional to reset achievements.
bool Steam::resetAllStats(bool achievementsToo){
	SteamUserStats()->ResetAllStats(achievementsToo);
	return SteamUserStats()->StoreStats();
}
// Request all statistics and achievements from Steam servers.
bool Steam::requestCurrentStats(){
	if(SteamUserStats() == NULL){
		return false;
	}
	// If the user isn't logged in, you can't get stats
	if(!SteamUser()->BLoggedOn()){
		return false;
	}
	return SteamUserStats()->RequestCurrentStats();
}
// Asynchronously fetch the data for the percentages.
void Steam::requestGlobalAchievementPercentages(){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->RequestGlobalAchievementPercentages();
	callResultGlobalAchievementPercentagesReady.Set(apiCall, this, &Steam::_global_achievement_percentages_ready);
}
// Set a given achievement.
bool Steam::setAchievement(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	SteamUserStats()->SetAchievement(name.utf8().get_data());
	return SteamUserStats()->StoreStats();
}
// Set a float statistic.
bool Steam::setStatFloat(const String& name, float value){
	return SteamUserStats()->SetStat(name.utf8().get_data(), value);
}
// Set an integer statistic.
bool Steam::setStatInt(const String& name, int value){
	return SteamUserStats()->SetStat(name.utf8().get_data(), value);
}
// Store all statistics, and achievements, on Steam servers; must be called to "pop" achievements.
bool Steam::storeStats(){
	if(SteamUserStats() == NULL){
		return 0;
	}
	SteamUserStats()->StoreStats();
	return SteamUserStats()->RequestCurrentStats();
}
// Get the achievement status, and the time it was unlocked if unlocked (in seconds since January 1, 19).
bool Steam::getAchievementAndUnlockTime(const String& name, bool achieved, uint32_t unlockTime){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->GetAchievementAndUnlockTime(name.utf8().get_data(), (bool *)achieved, (uint32*)&unlockTime);
}
// Achievement progress, triggers an AchievementProgress callback, that is all.
// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
bool Steam::indicateAchievementProgress(const String& name, int currentProgress, int maxProgress){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->IndicateAchievementProgress(name.utf8().get_data(), currentProgress, maxProgress);
}
// Find a given leaderboard, by name.
void Steam::findLeaderboard(const String& name){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->FindLeaderboard(name.utf8().get_data());
	callResultFindLeaderboard.Set(apiCall, this, &Steam::_leaderboard_loaded);
}
// Get the name of a leaderboard.
String Steam::getLeaderboardName(){
	if(SteamUserStats() == NULL){
		return "";
	}
	return SteamUserStats()->GetLeaderboardName(leaderboardHandle);
}
// Get the total number of entries in a leaderboard, as of the last request.
int Steam::getLeaderboardEntryCount(){
	if(SteamUserStats() == NULL){
		return -1;
	}
	return SteamUserStats()->GetLeaderboardEntryCount(leaderboardHandle);
}
// Request all rows for friends of user.
void Steam::downloadLeaderboardEntries(int start, int end, int type){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntries(leaderboardHandle, ELeaderboardDataRequest(type), start, end);
	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
}
// Request a maximum of 100 users with only one outstanding call at a time.
void Steam::downloadLeaderboardEntriesForUsers(Array usersID){
	if(SteamUserStats() == NULL){
		return;
	}
	int usersCount = usersID.size();
	if(usersCount == 0){
		return;
	}
	CSteamID *users = new CSteamID[usersCount];
	for(int i = 0; i < usersCount; i++){
		CSteamID user = createSteamID(usersID[i]);
		users[i] = user;
	}
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboardHandle, users, usersCount);
	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
	delete[] users;
}
// Upload a leaderboard score for the user.
void Steam::uploadLeaderboardScore(int score, bool keepBest, PoolIntArray details){
	if(SteamUserStats() == NULL){
		return;
	}
	ELeaderboardUploadScoreMethod method = keepBest ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
	int detailsSize = details.size();
	const int32 *detailsPointer = NULL;
	if(detailsSize > 0){
		PoolIntArray::Read r = details.read();
		detailsPointer = r.ptr();
	}
	SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(leaderboardHandle, method, (int32)score, detailsPointer, detailsSize);
	callResultUploadScore.Set(apiCall, this, &Steam::_leaderboard_uploaded);
}
// Once all entries are accessed, the data will be freed up and the handle will become invalid, use this to store it.
void Steam::getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t handle, int entryCount){
	if(SteamUserStats() == NULL){
		return;
	}
	leaderboardEntries.clear();
	LeaderboardEntry_t *entry = memnew(LeaderboardEntry_t);
	PoolIntArray details;
	int32 *detailsPointer = NULL;
	if(leaderboardDetailsMax > 0) {
		details.resize(leaderboardDetailsMax);
		PoolIntArray::Write w = details.write();
		detailsPointer = w.ptr();
		for(int i = 0; i < leaderboardDetailsMax; i++){
			detailsPointer[i] = 0;
		}
	}
	for(int i = 0; i < entryCount; i++){
		SteamUserStats()->GetDownloadedLeaderboardEntry(handle, i, entry, detailsPointer, leaderboardDetailsMax);
		Dictionary entryDict;
		entryDict["score"] = entry->m_nScore;
		entryDict["steamID"] = uint64_t(entry->m_steamIDUser.ConvertToUint64());
		entryDict["global_rank"] = entry->m_nGlobalRank;
		if(leaderboardDetailsMax > 0) {
			PoolIntArray array;
			array.resize(leaderboardDetailsMax);
			PoolIntArray::Write w = array.write();
			int32_t *ptr = w.ptr();
			for(int j=0; j<leaderboardDetailsMax; j++){
				ptr[j] = detailsPointer[j];
			}
			entryDict["details"] = array;
		}
		leaderboardEntries.append(entryDict);
	}
	memdelete(entry);
}
void Steam::setLeaderboardDetailsMax(int detailsMax) {
	leaderboardDetailsMax = detailsMax;
}
// Get the currently used leaderboard handle.
uint64_t Steam::getLeaderboardHandle(){
	return leaderboardHandle;
}
// Get the currently used leaderboard entries.
Array Steam::getLeaderboardEntries(){
	return leaderboardEntries;
}
/////////////////////////////////////////////////
///// UTILS /////////////////////////////////////
//
// Get the user's country by IP.
String Steam::getIPCountry(){
	return SteamUtils()->GetIPCountry();
}
// Returns true/false if Steam overlay is enabled.
bool Steam::isOverlayEnabled(){
	return SteamUtils()->IsOverlayEnabled();
}
// Get the Steam user interface language.
String Steam::getSteamUILanguage(){
	return SteamUtils()->GetSteamUILanguage();
}
// Get the Steam ID of the running application/game.
int Steam::getAppID(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetAppID();
}
// Return amount of time, in seconds, user has spent in this session.
int Steam::getSecondsSinceAppActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceAppActive();
}
// Get the amount of battery power, clearly for laptops.
int Steam::getCurrentBatteryPower(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetCurrentBatteryPower();
}
/////////////////////////////////////////////////
///// BIND METHODS //////////////////////////////
//
void Steam::_bind_methods(){
	ClassDB::bind_method("restartAppIfNecessary", &Steam::restartAppIfNecessary);
	ClassDB::bind_method("steamInit", &Steam::steamInit);
	ClassDB::bind_method("isSteamRunning", &Steam::isSteamRunning);
	ClassDB::bind_method("run_callbacks", &Steam::run_callbacks);
	// Apps Bind Methods ////////////////////////
	ClassDB::bind_method("isSubscribed", &Steam::isSubscribed);
	ClassDB::bind_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
	ClassDB::bind_method("isSubscribedApp", &Steam::isSubscribedApp);
	ClassDB::bind_method("isDLCInstalled", &Steam::isDLCInstalled);
	ClassDB::bind_method("getDLCCount", &Steam::getDLCCount);
	ClassDB::bind_method("getAppOwner", &Steam::getAppOwner);
	ClassDB::bind_method("getAppBuildId", &Steam::getAppBuildId);
	// Friends Bind Methods /////////////////////
	ClassDB::bind_method("getPersonaName", &Steam::getPersonaName);
	ClassDB::bind_method(D_METHOD("activateGameOverlay", "type"), &Steam::activateGameOverlay, DEFVAL(""));
	ClassDB::bind_method(D_METHOD("activateGameOverlayToUser", "type", "steamID"), &Steam::activateGameOverlayToUser, DEFVAL(""), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("activateGameOverlayToWebPage", "url"), &Steam::activateGameOverlayToWebPage);
	ClassDB::bind_method(D_METHOD("activateGameOverlayToStore", "appID"), &Steam::activateGameOverlayToStore, DEFVAL(0));
	// User Bind Methods ////////////////////////
	ClassDB::bind_method("getSteamID", &Steam::getSteamID);
	ClassDB::bind_method("loggedOn", &Steam::loggedOn);
	ClassDB::bind_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
	ClassDB::bind_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
	// User Stats Bind Methods //////////////////
	ClassDB::bind_method("clearAchievement", &Steam::clearAchievement);
	ClassDB::bind_method("getAchievement", &Steam::getAchievement);
	ClassDB::bind_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
	ClassDB::bind_method(D_METHOD("getAchievementName", "iAchievement"), &Steam::getAchievementName);
	ClassDB::bind_method("getStatFloat", &Steam::getStatFloat);
	ClassDB::bind_method("getStatInt", &Steam::getStatInt);
	ClassDB::bind_method("resetAllStats", &Steam::resetAllStats);
	ClassDB::bind_method("requestCurrentStats", &Steam::requestCurrentStats);
	ClassDB::bind_method("requestGlobalAchievementPercentages", &Steam::requestGlobalAchievementPercentages);
	ClassDB::bind_method("setAchievement", &Steam::setAchievement);
	ClassDB::bind_method("setStatFloat", &Steam::setStatFloat);
	ClassDB::bind_method("setStatInt", &Steam::setStatInt);
	ClassDB::bind_method("storeStats", &Steam::storeStats);
	ClassDB::bind_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
	ClassDB::bind_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);
	ClassDB::bind_method(D_METHOD("findLeaderboard", "name"), &Steam::findLeaderboard);
	ClassDB::bind_method("getLeaderboardName", &Steam::getLeaderboardName);
	ClassDB::bind_method("getLeaderboardEntryCount", &Steam::getLeaderboardEntryCount);
	ClassDB::bind_method(D_METHOD("downloadLeaderboardEntries", "range_start", "range_end", "type"), &Steam::downloadLeaderboardEntries, DEFVAL(int(GLOBAL)));
	ClassDB::bind_method(D_METHOD("downloadLeaderboardEntriesForUsers", "usersID"), &Steam::downloadLeaderboardEntriesForUsers);
	ClassDB::bind_method(D_METHOD("uploadLeaderboardScore", "score", "keep_best", "details"), &Steam::uploadLeaderboardScore, DEFVAL(true), DEFVAL(PoolIntArray()));
	ClassDB::bind_method(D_METHOD("setLeaderboardDetailsMax", "details_max"), &Steam::setLeaderboardDetailsMax);
	ClassDB::bind_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
	// Utils Bind Methods ///////////////////////
	ClassDB::bind_method("getIPCountry", &Steam::getIPCountry);
	ClassDB::bind_method("isOverlayEnabled", &Steam::isOverlayEnabled);
	ClassDB::bind_method("getSteamUILanguage", &Steam::getSteamUILanguage);
	ClassDB::bind_method("getAppID", &Steam::getAppID);
	ClassDB::bind_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
	ClassDB::bind_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
	// Signals //////////////////////////////////
	ADD_SIGNAL(MethodInfo("overlay_toggled", PropertyInfo(Variant::BOOL, "active")));
	ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));
	ADD_SIGNAL(MethodInfo("user_stats_received", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "userID")));
	ADD_SIGNAL(MethodInfo("user_stats_stored"));
	ADD_SIGNAL(MethodInfo("user_achievement_stored"));
	ADD_SIGNAL(MethodInfo("global_achievement_percentages_ready", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("leaderboard_loaded", PropertyInfo(Variant::INT, "leaderboard"), PropertyInfo(Variant::INT, "found")));
	ADD_SIGNAL(MethodInfo("leaderboard_uploaded", PropertyInfo(Variant::BOOL, "success"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::BOOL, "score_changed"), PropertyInfo(Variant::INT, "global_rank_new"), PropertyInfo(Variant::INT, "global_rank_previous")));
	ADD_SIGNAL(MethodInfo("leaderboard_entries_loaded"));
	// Status constants /////////////////////////
	BIND_CONSTANT(OFFLINE);						// 0
	BIND_CONSTANT(ONLINE);						// 1
	BIND_CONSTANT(BUSY);						// 2
	BIND_CONSTANT(AWAY);						// 3
	BIND_CONSTANT(SNOOZE);						// 4
	BIND_CONSTANT(LF_TRADE);					// 5
	BIND_CONSTANT(LF_PLAY);						// 6
	BIND_CONSTANT(NOT_OFFLINE);					// Custom
	BIND_CONSTANT(ALL); 						// Custom
	// Friend flags /////////////////////////////
	BIND_CONSTANT(FLAG_NONE);						// 0x00
	BIND_CONSTANT(FLAG_BLOCKED);					// 0x01
	BIND_CONSTANT(FLAG_FRIENDSHIP_REQUESTED);		// 0x02
	BIND_CONSTANT(FLAG_IMMEDIATE);					// 0x04
	BIND_CONSTANT(FLAG_CLAN_MEMBER);				// 0x08
	BIND_CONSTANT(FLAG_ON_GAME_SERVER);				// 0x10
	BIND_CONSTANT(FLAG_REQUESTING_FRIENDSHIP);		// 0x80
	BIND_CONSTANT(FLAG_REQUESTING_INFO);			// 0x100
	BIND_CONSTANT(FLAG_IGNORED);					// 0x200
	BIND_CONSTANT(FLAG_IGNORED_FRIEND);				// 0x400
	BIND_CONSTANT(FLAG_CHAT_MEMBER);				// 0x1000
	BIND_CONSTANT(FLAG_ALL);						// 0xFFFF
	// Relationship constants ///////////////////
	BIND_CONSTANT(RELATION_NONE);					// 0
	BIND_CONSTANT(RELATION_BLOCKED);				// 1
	BIND_CONSTANT(RELATION_REQUEST_RECIPIENT);		// 2
	BIND_CONSTANT(RELATION_FRIEND);					// 3
	BIND_CONSTANT(RELATION_REQUEST_INITIATOR);		// 4
	BIND_CONSTANT(RELATION_IGNORED);				// 5
	BIND_CONSTANT(RELATION_IGNORED_FRIEND);			// 6
	BIND_CONSTANT(RELATION_SUGGESTED);				// 7
	BIND_CONSTANT(RELATION_MAX);					// 8
	// Initialization errors ////////////////////
	BIND_CONSTANT(ERR_NO_CLIENT);
	BIND_CONSTANT(ERR_NO_CONNECTION);
	// Authentication responses /////////////////
	BIND_CONSTANT(AUTH_SESSION_OK);
	BIND_CONSTANT(AUTH_SESSION_STEAM_NOT_CONNECTED);
	BIND_CONSTANT(AUTH_SESSION_NO_LICENSE);
	BIND_CONSTANT(AUTH_SESSION_VAC_BANNED);
	BIND_CONSTANT(AUTH_SESSION_LOGGED_IN_ELSEWHERE);
	BIND_CONSTANT(AUTH_SESSION_VAC_CHECK_TIMEOUT);
	BIND_CONSTANT(AUTH_SESSION_TICKET_CANCELED);
	BIND_CONSTANT(AUTH_SESSION_TICKET_ALREADY_USED);
	BIND_CONSTANT(AUTH_SESSION_TICKET_INVALID);
	BIND_CONSTANT(AUTH_SESSION_PUBLISHER_BANNED);
	// Avatar sizes /////////////////////////////
	BIND_CONSTANT(AVATAR_SMALL);
	BIND_CONSTANT(AVATAR_MEDIUM);
	BIND_CONSTANT(AVATAR_LARGE);
	// Overlay notification locations ///////////
	BIND_CONSTANT(TOP_LEFT);
	BIND_CONSTANT(TOP_RIGHT);
	BIND_CONSTANT(BOT_LEFT);
	BIND_CONSTANT(BOT_RIGHT);
	// Global user //////////////////////////////
	BIND_CONSTANT(GLOBAL);
	BIND_CONSTANT(GLOBAL_AROUND_USER);
	BIND_CONSTANT(FRIENDS);
	BIND_CONSTANT(USERS);
	// Persona name maximums ////////////////////
	BIND_CONSTANT(PERSONA_NAME_MAX_UTF16);			// 32
	BIND_CONSTANT(PERSONA_NAME_MAX_UTF8);			// 128
	// User restriction flags ///////////////////
	BIND_CONSTANT(RESTRICTION_NONE);				// 0
	BIND_CONSTANT(RESTRICTION_UNKNOWN);				// 1
	BIND_CONSTANT(RESTRICTION_ANY_CHAT);			// 2
	BIND_CONSTANT(RESTRICTION_VOICE_CHAT);			// 4
	BIND_CONSTANT(RESTRICTION_GROUP_CHAT);			// 8
	BIND_CONSTANT(RESTRICTION_RATING);				// 16
	BIND_CONSTANT(RESTRICTION_GAME_INVITES);		// 32
	BIND_CONSTANT(RESTRICTION_TRADING);				// 64
	// Chat room metadata limits ////////////////
	BIND_CONSTANT(CHAT_METADATA_MAX);				// 8192
	// Chat entry types /////////////////////////
	BIND_CONSTANT(CHAT_INVALID);					// 0
	BIND_CONSTANT(CHAT_MESSAGE);					// 1
	BIND_CONSTANT(CHAT_TYPING);						// 2
	BIND_CONSTANT(CHAT_INVITE_GAME);				// 3
	BIND_CONSTANT(CHAT_EMOTE);						// 4
	BIND_CONSTANT(CHAT_LEFT);						// 6
	BIND_CONSTANT(CHAT_ENTERED);					// 7
	BIND_CONSTANT(CHAT_KICKED);						// 8
	BIND_CONSTANT(CHAT_BANNED);						// 9
	BIND_CONSTANT(CHAT_DISCONNECTED);				// 10
	BIND_CONSTANT(CHAT_HISTORICAL);					// 11
	BIND_CONSTANT(CHAT_LINK_BLOCKED);				// 14
	// Rich presence data limits ////////////////
	BIND_CONSTANT(MAX_RICH_PRESENCE_KEYS);			// 20
	BIND_CONSTANT(MAX_RICH_PRESENCE_KEY_LENGTH);	// 64
	BIND_CONSTANT(MAX_RICH_PRESENCE_VALUE_LENGTH);	// 256
	// Store overlay parameters /////////////////
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_NONE);		// 0
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_ADD_TO_CART);			// 1
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_ADD_TO_CART_AND_SHOW);	// 2
	// Matchmaking types ////////////////////////
	BIND_CONSTANT(PRIVATE);						// Only way to join the lobby is to invite to someone else.
	BIND_CONSTANT(FRIENDS_ONLY);				// Shows for friends or invitees, but not in lobby list.
	BIND_CONSTANT(PUBLIC);						// Visible for friends and in lobby list.
	BIND_CONSTANT(INVISIBLE);					// Returned by search, but not visible to other friends.
	BIND_CONSTANT(LOBBY_KEY_LENGTH);			// Maximum number of characters a lobby metadata key can be.
	BIND_CONSTANT(LOBBY_EQUAL_LESS_THAN);		// -2
	BIND_CONSTANT(LOBBY_LESS_THAN);				// -1
	BIND_CONSTANT(LOBBY_EQUAL);					// 0
	BIND_CONSTANT(LOBBY_GREATER_THAN);			// 1
	BIND_CONSTANT(LOBBY_EQUAL_GREATER_THAN);	// 2
	BIND_CONSTANT(LOBBY_NOT_EQUAL);				// 3
	BIND_CONSTANT(LOBBY_DISTANCE_CLOSE);		// 0 - Only lobbies in the same immediate region will be returned.
	BIND_CONSTANT(LOBBY_DISTANCE_DEFAULT);		// 1 - Only lobbies in the same region or near by regions.
	BIND_CONSTANT(LOBBY_DISTANCE_FAR);			// 2 - For games that don't have many latency requirements, will return lobbies about half-way around the globe.
	BIND_CONSTANT(LOBBY_DISTANCE_WORLDWIDE);	// 3 - No filtering, will match lobbies as far as India to NY (not recommended, expect multiple seconds of latency between the clients).
	// Matchmaking lobby responses //////////////
	BIND_CONSTANT(LOBBY_OK);					// Lobby was successfully created.
	BIND_CONSTANT(LOBBY_NO_CONNECTION);			// Your Steam client doesn't have a connection to the back-end.
	BIND_CONSTANT(LOBBY_TIMEOUT);				// Message to the Steam servers, but it didn't respond.
	BIND_CONSTANT(LOBBY_FAIL);					// Server responded, but with an unknown internal error.
	BIND_CONSTANT(LOBBY_ACCESS_DENIED);			// Game isn't set to allow lobbies, or your client does haven't rights to play the game.
	BIND_CONSTANT(LOBBY_LIMIT_EXCEEDED);		// Game client has created too many lobbies.
	// Remote Storage
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_NONE);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_WINDOWS);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_OSX);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_PS3);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_LINUX);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_RESERVED2);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_ALL);
	// Workshop item characters /////////////////
	BIND_CONSTANT(UGC_MAX_TITLE_CHARS);			// 128
	BIND_CONSTANT(UGC_MAX_DESC_CHARS);			// 8000
	BIND_CONSTANT(UGC_MAX_METADATA_CHARS);		// 5000
	// Workshop item types //////////////////////
	BIND_CONSTANT(UGC_ITEM_COMMUNITY);				// Normal items that can be subscribed to.
	BIND_CONSTANT(UGC_ITEM_MICROTRANSACTION);		// Item that is meant to be voted on for the purpose of selling in-game.
	BIND_CONSTANT(UGC_ITEM_COLLECTION);				// A collection of Workshop items.
	BIND_CONSTANT(UGC_ITEM_ART);					// Artwork.
	BIND_CONSTANT(UGC_ITEM_VIDEO);					// External video.
	BIND_CONSTANT(UGC_ITEM_SCREENSHOT);				// Screenshot.
	BIND_CONSTANT(UGC_ITEM_GAME);					// Unused, used to be for Greenlight game entries
	BIND_CONSTANT(UGC_ITEM_SOFTWARE);				// Unused, used to be for Greenlight software entries.
	BIND_CONSTANT(UGC_ITEM_CONCEPT);				// Unused, used to be for Greenlight concepts.
	BIND_CONSTANT(UGC_ITEM_WEBGUIDE);				// Steam web guide.
	BIND_CONSTANT(UGC_ITEM_INTEGRATEDGUIDE);		// Application integrated guide.
	BIND_CONSTANT(UGC_ITEM_MERCH);					// Workshop merchandise meant to be voted on for the purpose of being sold.
	BIND_CONSTANT(UGC_ITEM_CONTROLLERBINDING);		// Steam Controller bindings.
	BIND_CONSTANT(UGC_ITEM_STEAMWORKSACCESSINVITE);	// Only used internally in Steam.
	BIND_CONSTANT(UGC_ITEM_STEAMVIDEO);				// Steam video.
	BIND_CONSTANT(UGC_ITEM_GAMEMANAGEDITEM);		// Managed completely by the game, not the user, and not shown on the web.
	BIND_CONSTANT(UGC_ITEM_MAX);					// Only used for enumerating.
	// Workshop item states /////////////////////
	BIND_CONSTANT(UGC_STATE_NONE);				// Not tracked on client.
	BIND_CONSTANT(UGC_STATE_SUBSCRIBED);		// Current user is subscribed to this item, not just cached.
	BIND_CONSTANT(UGC_STATE_LEGACY);			// Was created with ISteamRemoteStorage.
	BIND_CONSTANT(UGC_STATE_INSTALLED);			// Is installed and usable (but maybe out of date).
	BIND_CONSTANT(UGC_STATE_UPDATE);			// Needs an update, either because it's not installed yet or creator updated content.
	BIND_CONSTANT(UGC_STATE_DOWNLOADING);		// Update is currently downloading.
	BIND_CONSTANT(UGC_STATE_PENDING);			// DownloadItem() was called for this item, content isn't available until DownloadItemResult_t is fired.
	// Workshop item visibility//////////////////
	BIND_CONSTANT(UGC_FILE_VISIBLE_PUBLIC);
	BIND_CONSTANT(UGC_FILE_VISIBLE_FRIENDS);
	BIND_CONSTANT(UGC_FILE_VISIBLE_PRIVATE);
	// Workshop item update status //////////////
	BIND_CONSTANT(STATUS_INVALID);				// Update handle was invalid, job might be finished, listen to SubmitItemUpdateResult_t.
	BIND_CONSTANT(STATUS_PREPARING_CONFIG);		// Update is processing configuration data.
	BIND_CONSTANT(STATUS_PREPARING_CONTENT);	// Update is reading and processing content files.
	BIND_CONSTANT(STATUS_UPLOADING_CONTENT);	// Update is uploading content changes to Steam.
	BIND_CONSTANT(STATUS_UPLOADING_PREVIEW);	// Update is uploading new preview file image.
	BIND_CONSTANT(STATUS_COMMITTING_CHANGES);	// Update is committing all changes.
	// Result constants /////////////////////////
	BIND_CONSTANT(RESULT_OK);						// 1
	BIND_CONSTANT(RESULT_FAIL);						// 2
	BIND_CONSTANT(RESULT_NO_CONNECT);				// 3
	BIND_CONSTANT(RESULT_INVALID_PASSWORD);			// 5
	BIND_CONSTANT(RESULT_LOGGED_IN_ESLEWHERE);		// 6
	BIND_CONSTANT(RESULT_INVALID_PROTOCAL);			// 7
	BIND_CONSTANT(RESULT_INALID_PARAM);				// 8
	BIND_CONSTANT(RESULT_FILE_NOT_FOUND);			// 9
	BIND_CONSTANT(RESULT_BUSY);						// 10
	BIND_CONSTANT(RESULT_INVALID_STATE);			// 11
	BIND_CONSTANT(RESULT_INVALID_NAME);				// 12
	BIND_CONSTANT(RESULT_INVALID_EMAIL);			// 13
	BIND_CONSTANT(RESULT_DUPLICATE_NAME);			// 14
	BIND_CONSTANT(RESULT_ACCESS_DENIED);			// 15
	BIND_CONSTANT(RESULT_TIMEOUT);					// 16
	BIND_CONSTANT(RESULT_BANNED);					// 17
	BIND_CONSTANT(RESULT_ACCOUNT_NOT_FOUND);		// 18
	BIND_CONSTANT(RESULT_INVALID_STEAM_ID);			// 19
	BIND_CONSTANT(RESULT_SERVICE_UNAVAILABLE);		// 20
	BIND_CONSTANT(RESULT_NOT_LOGGED_ON);			// 21
	BIND_CONSTANT(RESULT_PENDING);					// 22
	BIND_CONSTANT(RESULT_ENCRYPT_FAILURE);			// 23
	BIND_CONSTANT(RESULT_INSUFFICIENT_PRIVILEGE);	// 24
	BIND_CONSTANT(RESULT_LIMIT_EXCEEDED);			// 25
	BIND_CONSTANT(RESULT_REVOKED);					// 26
	BIND_CONSTANT(RESULT_EXPIRED);					// 27
	BIND_CONSTANT(RESULT_ALREADY_REDEEMED);			// 28
	BIND_CONSTANT(RESULT_DUPLICATE_REQUEST);		// 29
	BIND_CONSTANT(RESULT_ALREADY_OWNED);			// 30
	BIND_CONSTANT(RESULT_IP_NOT_FOUND);				// 31
	BIND_CONSTANT(RESULT_PERSIST_FAILED);			// 32
	BIND_CONSTANT(RESULT_LOCKING_FAILED);			// 33
	BIND_CONSTANT(RESULT_LOGON_SESSION_REPLACED);	// 34
	BIND_CONSTANT(RESULT_CONNECT_FAILED);			// 35
	BIND_CONSTANT(RESULT_HANDSHAKE_FAILED);			// 36
	BIND_CONSTANT(RESULT_IO_FAILURE);				// 37
	BIND_CONSTANT(RESULT_REMOTE_DISCONNECT);		// 38
	BIND_CONSTANT(RESULT_SHOPPING_CART_NOT_FOUND);	// 39
	BIND_CONSTANT(RESULT_BLOCKED);					// 40
	BIND_CONSTANT(RESULT_IGNORED);					// 41
	BIND_CONSTANT(RESULT_NO_MATCH);					// 42
	BIND_CONSTANT(RESULT_ACCOUNT_DISABLED);			// 43
	BIND_CONSTANT(RESULT_SERVICE_READY_ONLY);		// 44
	BIND_CONSTANT(RESULT_ACCOUNT_NOT_FEATURED);		// 45
	BIND_CONSTANT(RESULT_ADMINISTRATOR_OK);			// 46
	BIND_CONSTANT(RESULT_CONTENT_VERSION);			// 47
	BIND_CONSTANT(RESULT_TRY_ANOTHER_CM);			// 48
	BIND_CONSTANT(RESULT_PASSWORD_REQUIRED_TO_KICK);// 49
	BIND_CONSTANT(RESULT_ALREADY_LOGGED_ELSEWHERE);	// 50
	BIND_CONSTANT(RESULT_SUSPENDED);				// 51
	BIND_CONSTANT(RESULT_CANCELLED);				// 52
	BIND_CONSTANT(RESULT_DATA_CORRUPTION);			// 53
	BIND_CONSTANT(RESULT_DISK_FULL);				// 54
	BIND_CONSTANT(RESULT_REMOTE_CALL_FAILED);		// 55
	BIND_CONSTANT(RESULT_PASSWORD_UNSET);			// 56
	BIND_CONSTANT(RESULT_EXTERNAL_ACCOUNT_UNLINKED);// 57
	BIND_CONSTANT(RESULT_PSN_TICKET_INVALID);		// 58
	BIND_CONSTANT(RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED);	// 59
	BIND_CONSTANT(RESULT_REMOTE_FILE_CONFLICT);		// 60
	BIND_CONSTANT(RESULT_ILLEGAL_PASSWORD);			// 61
	BIND_CONSTANT(RESULT_SAME_AS_PREVIOUS_VALUE);	// 62
	BIND_CONSTANT(RESULT_ACCOUNT_LOGON_DENIED);		// 63
	BIND_CONSTANT(RESULT_CANNOT_USE_OLD_PASSWORD);	// 64
	BIND_CONSTANT(RESULT_INVALID_LOGIN_AUTH_CODE);	// 65
	BIND_CONSTANT(RESULT_ACCOUNT_LOGON_DENIED_NO_MAIL);	// 66
	BIND_CONSTANT(RESULT_HARDWARE_NOT_CAPABLE);		// 67
	BIND_CONSTANT(RESULT_IP_INIT_ERROR);			// 68
	BIND_CONSTANT(RESULT_PARENTAL_CONTROL_RESTRICTED);	// 69
	BIND_CONSTANT(RESULT_FACEBOOK_QUERY_ERROR);		// 70
	BIND_CONSTANT(RESULT_EXPIRED_LOGIN_AUTH_CODE);	// 71
	BIND_CONSTANT(RESULT_IP_LOGIN_RESTRICTION_FAILED);	// 72
	BIND_CONSTANT(RESULT_ACCOUNT_LOCKED_DOWN);		// 73
	BIND_CONSTANT(RESULT_ACCOUNT_LOGON_DENIED_VERIFIED_EMAIL_REQUIRED);	// 74
	BIND_CONSTANT(RESULT_NO_MATCHING_URL);			// 75
	BIND_CONSTANT(RESULT_BAD_RESPONSE);				// 76
	BIND_CONSTANT(RESULT_REQUIRED_PASSWORD_REENTRY);// 77
	BIND_CONSTANT(RESULT_VALUE_OUT_OF_RANGE);		// 78
	BIND_CONSTANT(RESULT_UNEXPECTED_ERROR);			// 79
	BIND_CONSTANT(RESULT_DISABLED);					// 80
	BIND_CONSTANT(RESULT_INVALID_CEG_SUBMISSION);	// 81
	BIND_CONSTANT(RESULT_RESTRICTED_DEVICE);		// 82
	BIND_CONSTANT(RESULT_REGION_LOCKED);			// 83
	BIND_CONSTANT(RESULT_RATE_LIMIT_EXCEEDED);		// 84
	BIND_CONSTANT(RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR);	// 85
	BIND_CONSTANT(RESULT_ITEM_DELETED);				// 86
	BIND_CONSTANT(RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE);	// 87
	BIND_CONSTANT(RESULT_TWO_FACTOR_CODE_MISMATCH);	// 88
	BIND_CONSTANT(RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH);	// 89
	BIND_CONSTANT(RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS);	// 90
	BIND_CONSTANT(RESULT_NOT_MODIFIED);				// 91
	BIND_CONSTANT(RESULT_NO_MOBILE_DEVICE);			// 92
	BIND_CONSTANT(RESULT_TIME_NOT_SYNCED);			// 93
	BIND_CONSTANT(RESULT_SMS_CODE_FAILED);			// 94
	BIND_CONSTANT(RESULT_ACCOUNT_LIMIT_EXCEEDED);	// 95
	BIND_CONSTANT(RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED);	// 96
	BIND_CONSTANT(RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED);	// 97
	BIND_CONSTANT(RESULT_REFUND_TO_WALLET);			// 98
	BIND_CONSTANT(RESULT_EMAIL_SEND_FAILURE);		// 99
	BIND_CONSTANT(RESULT_NOT_SETTLED);				// 100
	BIND_CONSTANT(RESULT_NEED_CAPTCHA);				// 101
	BIND_CONSTANT(RESULT_GSLT_DENIED);				// 102
	BIND_CONSTANT(RESULT_GS_OWNER_DENIED);			// 103
	BIND_CONSTANT(RESULT_INVALID_ITEM_TYPE);		// 104
	BIND_CONSTANT(RESULT_IP_BANNED);				// 105
	BIND_CONSTANT(RESULT_GSLT_EXPIRED);				// 106
	BIND_CONSTANT(RESULT_INSUFFICIENT_FUNDS);		// 107
	BIND_CONSTANT(RESULT_TOO_MANY_PENDING);			// 108
	// Persona changes //////////////////////////
	BIND_CONSTANT(PERSONA_CHANGE_NAME);				// 0x0001
	BIND_CONSTANT(PERSONA_CHANGE_STATUS); 			// 0x0002
	BIND_CONSTANT(PERSONA_CHANGE_COME_ONLINE); 		// 0x0004
	BIND_CONSTANT(PERSONA_CHANGE_GONE_OFFLINE);		// 0x0008
	BIND_CONSTANT(PERSONA_CHANGE_GAME_PLAYED);		// 0x0010
	BIND_CONSTANT(PERSONA_CHANGE_GAME_SERVER);		// 0x0020
	BIND_CONSTANT(PERSONA_CHANGE_AVATAR);			// 0x0040
	BIND_CONSTANT(PERSONA_CHANGE_JOINED_SOURCE);	// 0x0080
	BIND_CONSTANT(PERSONA_CHANGE_LEFT_SOURCE);		// 0x0100
	BIND_CONSTANT(PERSONA_CHANGE_RELATIONSHIP_CHANGED);	// 0x0200
	BIND_CONSTANT(PERSONA_CHANGE_NAME_FIRST_SET);	// 0x0400
	BIND_CONSTANT(PERSONA_CHANGE_FACEBOOK_INFO);	// 0x0800
	BIND_CONSTANT(PERSONA_CHANGE_NICKNAME);			// 0x1000
	BIND_CONSTANT(PERSONA_CHANGE_STEAM_LEVEL);		// 0x2000
	// Chat room responses //////////////////////
	BIND_CONSTANT(CHAT_ROOM_SUCCESS);				// 1
	BIND_CONSTANT(CHAT_ROOM_DOESNT_EXIST);			// 2
	BIND_CONSTANT(CHAT_ROOM_NOT_ALLOWED);			// 3
	BIND_CONSTANT(CHAT_ROOM_FULL);					// 4
	BIND_CONSTANT(CHAT_ROOM_ERROR);					// 5
	BIND_CONSTANT(CHAT_ROOM_BANNED);				// 6
	BIND_CONSTANT(CHAT_ROOM_LIMITED);				// 7
	BIND_CONSTANT(CHAT_ROOM_CLAN_DISABLED);			// 8
	BIND_CONSTANT(CHAT_ROOM_COMMUNITY_BAN);			// 9
	BIND_CONSTANT(CHAT_ROOM_MEMBER_BLOCKED_YOU);	// 10
	BIND_CONSTANT(CHAT_ROOM_YOU_BLOCKED_MEMBER);	// 11
}

Steam::~Steam(){
	if(isInitSuccess){
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	singleton = NULL;
}
