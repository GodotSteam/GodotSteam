#include "godotsteam.h"
#include <steam/steam_api.h>

#include "core/io/ip_address.h"
#include "core/io/ip.h"

Steam* Steam::singleton = NULL;

Steam::Steam(){
	isInitSuccess = false;
	singleton = this;
	leaderboardDetailsMax = 0;
	tickets.clear();
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
// Checks if the license owned by the user provides low violence depots.
bool Steam::isLowViolence(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsLowViolence();
}
// Checks whether the current App ID is for Cyber Cafes.
bool Steam::isCybercafe(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsCybercafe();
}
// Checks if the user has a VAC ban on their account.
bool Steam::isVACBanned(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsVACBanned();
}
// Gets the current language that the user has set.
String Steam::getCurrentGameLanguage(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetCurrentGameLanguage();
}
// Gets a comma separated list of the languages the current app supports.
String Steam::getAvailableGameLanguages(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetAvailableGameLanguages();
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
// Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).
int Steam::getEarliestPurchaseUnixTime(int value){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
}
// Checks if the user is subscribed to the current app through a free weekend.
// This function will return false for users who have a retail or other type of license.
// Suggested you contact Valve on how to package and secure your free weekend properly.
bool Steam::isSubscribedFromFreeWeekend(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedFromFreeWeekend();
}
// Get the number of DLC the user owns for a parent application/game.
int Steam::getDLCCount(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->GetDLCCount();
}
// Returns metadata for a DLC by index.
Array Steam::getDLCDataByIndex(){
	if(SteamApps() == NULL){
		return Array();
	}
	int count = SteamApps()->GetDLCCount();
	Array dlcData;
	for(int i = 0; i < count; i++){
		AppId_t appID = 0;
		bool available = false;
		char name[128];
		bool success = SteamApps()->BGetDLCDataByIndex(i, &appID, &available, name, 128);
		if(success){
			Dictionary dlc;
			dlc["id"] = appID;
			dlc["available"] = available;
			dlc["name"] = name;
			dlcData.append(dlc);
		}
	}
	return dlcData;
}
// Allows you to install an optional DLC.
void Steam::installDLC(int value){
	if(SteamApps() == NULL){
		return;
	}
	SteamApps()->InstallDLC((AppId_t)value);
}
// Allows you to uninstall an optional DLC.
void Steam::uninstallDLC(int value){
	if(SteamApps() == NULL){
		return;
	}
	SteamApps()->UninstallDLC((AppId_t)value);
}
// Checks if the user is running from a beta branch, and gets the name of the branch if they are.
String Steam::getCurrentBetaName(){
	String ret = "";
	if(SteamApps() != NULL){
		char str[1024];
		if (SteamApps()->GetCurrentBetaName(str, 1024)) {
			ret = String(str);
		}
	}
	return ret;
}
// Allows you to force verify game content on next launch.
bool Steam::markContentCorrupt(bool missingFilesOnly){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->MarkContentCorrupt(missingFilesOnly);
}
// Gets a list of all installed depots for a given App ID in mount order.
Array Steam::getInstalledDepots(int appID){
	if(SteamApps() == NULL){
		return Array();
	}
	Array installedDepots;
	DepotId_t *depots = new DepotId_t[32];
	int installed = SteamApps()->GetInstalledDepots((AppId_t)appID, depots, 32);
	for(int i = 0; i < installed; i++){
		installedDepots.append(depots[i]);
	}
	return installedDepots;
}
// Gets the install folder for a specific AppID.
String Steam::getAppInstallDir(AppId_t appID){
	if(SteamApps() == NULL){
		return "";
	}
	const uint32 folderBuffer = 256;
	char *buffer = new char[folderBuffer];
	SteamApps()->GetAppInstallDir(appID, (char*)buffer, folderBuffer);
	String appDir = buffer;
	delete buffer;
	return appDir;
}
// Check if given application/game is installed, not necessarily owned.
bool Steam::isAppInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsAppInstalled((AppId_t)value);
}
// Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
uint64_t Steam::getAppOwner(){
	if(SteamApps() == NULL){
		return 0;
	}
	CSteamID cSteamID = SteamApps()->GetAppOwner();
	return cSteamID.ConvertToUint64();
}
// Gets the associated launch parameter if the game is run via steam://run/<appid>/?param1=value1;param2=value2;param3=value3 etc.
String Steam::getLaunchQueryParam(const String& key){
	if(SteamApps() == NULL){
		return "";
	}
	return SteamApps()->GetLaunchQueryParam(key.utf8().get_data());
}
// Gets the download progress for optional DLC.
Dictionary Steam::getDLCDownloadProgress(int appID){
	Dictionary progress;
	if(SteamApps() == NULL){
		progress["ret"] = false;
	}
	else{
		uint64 downloaded = 0;
		uint64 total = 0;
		// Get the progress
		progress["ret"] = SteamApps()->GetDlcDownloadProgress((AppId_t)appID, &downloaded, &total);
		if(progress["ret"]){
			progress["downloaded"] = uint64_t(downloaded);
			progress["total"] = uint64_t(total);
		}
	}
	return progress;
}
// Return the build ID for this app; will change based on backend updates.
int Steam::getAppBuildId(){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetAppBuildId();
}
// Asynchronously retrieves metadata details about a specific file in the depot manifest.
void Steam::getFileDetails(const String& filename){
	if(SteamApps() == NULL){
		return;
	}
//	SteamAPICall_t apiCall = SteamApps()->GetFileDetails(filename.utf8().get_data());
//	callResultFileDetails.Set(apiCall, this, &Steam::_file_details_result);
	SteamApps()->GetFileDetails(filename.utf8().get_data());
}
/////////////////////////////////////////////////
///// CONTROLLERS////////////////////////////////
//
// Reconfigure the controller to use the specified action set.
void Steam::activateActionSet(uint64_t controllerHandle, uint64_t actionSetHandle){
	if(SteamController() != NULL){
		SteamController()->ActivateActionSet((ControllerHandle_t)controllerHandle, (ControllerActionSetHandle_t)actionSetHandle);
	}
}
// Lookup the handle for an Action Set.
uint64_t Steam::getActionSetHandle(const String& actionSetName){
	if(SteamController() != NULL){
		return (uint64_t)SteamController()->GetActionSetHandle(actionSetName.utf8().get_data());
	}
	return 0;
}
// Returns the current state of the supplied analog game action.
Dictionary Steam::getAnalogActionData(uint64_t controllerHandle, uint64_t analogActionHandle){
	ControllerAnalogActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamController() != NULL){
		data = SteamController()->GetAnalogActionData((ControllerHandle_t)controllerHandle, (ControllerAnalogActionHandle_t)analogActionHandle);
	}
	d["eMode"] = data.eMode;
	d["x"] = data.x;
	d["y"] = data.y;
	d["bActive"] = data.bActive;
	return d;
}
// Get the handle of the specified Analog action.
uint64_t Steam::getAnalogActionHandle(const String& actionName){
	if(SteamController() != NULL){
		return (uint64_t)SteamController()->GetAnalogActionHandle(actionName.utf8().get_data());
	}
	return 0;
}
// Get the origin(s) for an analog action within an action.
Array Steam::getAnalogActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t analogActionHandle){
	Array list;
	if(SteamController() != NULL){
		EControllerActionOrigin out[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamController()->GetAnalogActionOrigins((ControllerHandle_t)controllerHandle, (ControllerActionSetHandle_t)actionSetHandle, (ControllerAnalogActionHandle_t)analogActionHandle, out);
		for (int i = 0; i < ret; i++){
			list.push_back((int)out[i]);
		}
	}
	return list;
}
// Get current controllers handles.
Array Steam::getConnectedControllers(){
	Array list;
	if(SteamController() != NULL){
		ControllerHandle_t handles[STEAM_CONTROLLER_MAX_COUNT];
		int ret = SteamController()->GetConnectedControllers(handles);
		for (int i = 0; i < ret; i++){
			list.push_back((uint64_t)handles[i]);
		}
	}
	return list;
}
// Returns the associated controller handle for the specified emulated gamepad.
uint64_t Steam::getControllerForGamepadIndex(int index){
	if(SteamController() != NULL){
		return (uint64_t)SteamController()->GetControllerForGamepadIndex(index);
	}
	return 0;
}
// Get the currently active action set for the specified controller.
uint64_t Steam::getCurrentActionSet(uint64_t controllerHandle){
	if(SteamController() != NULL){
		return (uint64_t)SteamController()->GetCurrentActionSet((ControllerHandle_t)controllerHandle);
	}
	return 0;
}
// Returns the current state of the supplied digital game action.
Dictionary Steam::getDigitalActionData(uint64_t controllerHandle, uint64_t digitalActionHandle){
	ControllerDigitalActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamController() != NULL){
		data = SteamController()->GetDigitalActionData((ControllerHandle_t)controllerHandle, (ControllerDigitalActionHandle_t)digitalActionHandle);
	}
	d["bState"] = data.bState;
	d["bActive"] = data.bActive;
	return d;
}
// Get the handle of the specified digital action.
uint64_t Steam::getDigitalActionHandle(const String& actionName){
	if(SteamController() != NULL){
		return (uint64_t)SteamController()->GetDigitalActionHandle(actionName.utf8().get_data());
	}
	return 0;
}
// Get the origin(s) for an analog action within an action.
Array Steam::getDigitalActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t digitalActionHandle){
	Array list;
	if(SteamController() != NULL){
		EControllerActionOrigin out[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamController()->GetDigitalActionOrigins((ControllerHandle_t)controllerHandle, (ControllerActionSetHandle_t)actionSetHandle, (ControllerDigitalActionHandle_t)digitalActionHandle, out);
		for (int i=0; i<ret; i++){
			list.push_back((int)out[i]);
		}
	}
	return list;
}
// Returns the associated gamepad index for the specified controller.
int Steam::getGamepadIndexForController(uint64_t controllerHandle){
	if(SteamController() != NULL){
		return SteamController()->GetGamepadIndexForController((ControllerHandle_t)controllerHandle);
	}
	return -1;
}
// Returns raw motion data for the specified controller.
Dictionary Steam::getMotionData(uint64_t controllerHandle){
	ControllerMotionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamController() != NULL){
		data = SteamController()->GetMotionData((ControllerHandle_t)controllerHandle);
	}
	d["rotQuatX"] = data.rotQuatX;
	d["rotQuatY"] = data.rotQuatY;
	d["rotQuatZ"] = data.rotQuatZ;
	d["rotQuatW"] = data.rotQuatW;
	d["posAccelX"] = data.posAccelX;
	d["posAccelY"] = data.posAccelY;
	d["posAccelZ"] = data.posAccelZ;
	d["rotVelX"] = data.rotVelX;
	d["rotVelY"] = data.rotVelY;
	d["rotVelZ"] = data.rotVelZ;
	return d;
}
// Start SteamControllers interface.
bool Steam::init(){
	if(SteamController() != NULL){
		return SteamController()->Init();
	}
	return false;
}
// Syncronize controllers.
void Steam::runFrame(){
	if(SteamController() != NULL){
		SteamController()->RunFrame();
	}
}
// Invokes the Steam overlay and brings up the binding screen.
bool Steam::showBindingPanel(uint64_t controllerHandle){
	if(SteamController() != NULL){
		return SteamController()->ShowBindingPanel((ControllerHandle_t)controllerHandle);
	}
	return false;
}
// Stop SteamControllers interface.
bool Steam::shutdown(){
	if(SteamController() != NULL){
		return SteamController()->Shutdown();
	}
	return false;
}
// Trigger a vibration event on supported controllers.
void Steam::triggerVibration(uint64_t controllerHandle, uint16_t leftSpeed, uint16_t rightSpeed){
	if(SteamController() != NULL){
		SteamController()->TriggerVibration((ControllerHandle_t)controllerHandle, (unsigned short)leftSpeed, (unsigned short)rightSpeed);
	}
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
// Sets the player name, stores it on the server and publishes the changes to all friends who are online.
void Steam::setPersonaName(const String& name){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->SetPersonaName(name.utf8().get_data());
}
// Gets the status of the current user.
int Steam::getPersonaState(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetPersonaState();
}
// Get number of friends user has.
int Steam::getFriendCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendCount(0x04);
}
// Returns the Steam ID of a user.
uint64_t Steam::getFriendByIndex(int friendNum, int friendFlags){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID friendID = SteamFriends()->GetFriendByIndex(friendNum, friendFlags);
	return friendID.ConvertToUint64();
}
// Returns a relationship to a user.
int Steam::getFriendRelationship(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendRelationship(userID);
}
// Returns the current status of the specified user.
int Steam::getFriendPersonaState(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendPersonaState(userID);
}
// Get given friend's Steam username.
String Steam::getFriendPersonaName(uint64_t steamID){
	if(SteamFriends() != NULL && steamID > 0){
		CSteamID userID = (uint64)steamID;
		bool isDataLoading = SteamFriends()->RequestUserInformation(userID, true);
		if(!isDataLoading){
			return SteamFriends()->GetFriendPersonaName(userID);
		}
	}
	return "";
}

// Returns dictionary of friend game played if valid
Dictionary Steam::getFriendGamePlayedD(uint64_t steamID){
	Dictionary friendGame;
	if(SteamFriends() == NULL){
		return friendGame;
	}
	FriendGameInfo_t gameInfo;
	CSteamID userID = (uint64)steamID;
	bool success = SteamFriends()->GetFriendGamePlayed(userID, &gameInfo);
	// If successful
	if(success){
		// Is there a valid lobby?
		if(gameInfo.m_steamIDLobby.IsValid()){
			friendGame["id"] = gameInfo.m_gameID.AppID();
			// Convert the IP address back to a string
			const int NBYTES = 4;
			uint8 octet[NBYTES];
			char gameIP[16];
			for(int j = 0; j < NBYTES; j++){
				octet[j] = gameInfo.m_unGameIP >> (j * 8);
			}
			sprintf_s(gameIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
			friendGame["ip"] = gameIP;
			friendGame["gamePort"] = gameInfo.m_usGamePort;
			friendGame["queryPort"] = (char)gameInfo.m_usQueryPort;
			friendGame["lobby"] = uint64_t(gameInfo.m_steamIDLobby.ConvertToUint64());
			friendGame["friendID"] = steamID;
		}
		else{
			friendGame["error"] = "No valid lobby";
		}
	}
	return friendGame;
}

// Returns an object of friend game played with helper functions if valid and returns true or false for easy if statements
bool Steam::getFriendGamePlayed(uint64_t steamID, Ref<FriendGameInfo> friendGameInfo) {
	if(SteamFriends() == NULL){
		return false;
	}
	FriendGameInfo_t gameInfo;
	CSteamID userID = (uint64)steamID;
	bool success = SteamFriends()->GetFriendGamePlayed(userID, &gameInfo);
	// If successful
	if(success) {// && gameInfo.m_steamIDLobby.IsValid()
		//m_steamIDLobbyIsValid

			Ref<FriendGameInfo> gameInfoObject = FriendGameInfo::new_from_struct(gameInfo, steamID);
			friendGameInfo = &gameInfoObject;
	}
	return success;
}

Array Steam::getFriendGameLobbies() {
	Array returnArray;
	int cFriends = SteamFriends()->GetFriendCount( k_EFriendFlagImmediate );
	for ( int i = 0; i < cFriends; i++ )
	{
		Ref<FriendGameInfo> friendGameInfo(memnew(FriendGameInfo));
		CSteamID steamIDFriend = SteamFriends()->GetFriendByIndex( i, k_EFriendFlagImmediate );

		if(getFriendGamePlayed(steamIDFriend.ConvertToUint64(), friendGameInfo) && friendGameInfo->m_steamIDLobbyIsValid())
		{
			// friendGameInfo->steamIDLobby is a valid lobby, you can join it or use RequestLobbyData() get its metadata
			returnArray.push_back(friendGameInfo);
		}
	}
	return returnArray;
}

// Accesses old friends names; returns an empty string when there are no more items in the history.
String Steam::getFriendPersonaNameHistory(uint64_t steamID, int nameHistory){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendPersonaNameHistory(userID, nameHistory);
}
// Get friend's steam level, obviously.
int Steam::getFriendSteamLevel(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendSteamLevel(userID);
}
// Returns nickname the current user has set for the specified player. Returns NULL if the no nickname has been set for that player.
String Steam::getPlayerNickname(uint64_t steamID){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetPlayerNickname(userID);
}
// Returns true if the specified user meets any of the criteria specified in iFriendFlags.
bool Steam::hasFriend(uint64_t steamID, int friendFlags){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->HasFriend(userID, friendFlags);
}
// For clans a user is a member of, they will have reasonably up-to-date information, but for others you'll have to download the info to have the latest.
void Steam::downloadClanActivityCounts(uint64_t clanID, int clansToRequest){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID clan = (uint64)clanID;
	SteamFriends()->DownloadClanActivityCounts(&clan, clansToRequest);
}
// Iterators for getting users in a chat room, lobby, game server or clan.
int Steam::getFriendCountFromSource(uint64_t clanID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->GetFriendCountFromSource(clan);
}
// Returns true if the local user can see that steamIDUser is a member or in steamIDSource.
uint64_t Steam::getFriendFromSourceByIndex(uint64_t sourceID, int friendNum){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID source = (uint64)sourceID;
	CSteamID friendID = SteamFriends()->GetFriendFromSourceByIndex(source, friendNum);
	return friendID.ConvertToUint64();
}
// Returns true if the local user can see that steamIDUser is a member or in steamIDSource.
bool Steam::isUserInSource(uint64_t steamID, uint64_t sourceID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	CSteamID source = (uint64)sourceID;
	return SteamFriends()->IsUserInSource(userID, source);
}
// User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI).
void Steam::setInGameVoiceSpeaking(uint64_t steamID, bool speaking){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID userID = (uint64)steamID;
	SteamFriends()->SetInGameVoiceSpeaking(userID, speaking);
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
// Set player as 'Played With' for game.
void Steam::setPlayedWith(uint64_t steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID userID = (uint64)steamID;
	SteamFriends()->SetPlayedWith(userID);
}
// Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
void Steam::activateGameOverlayInviteDialog(uint64_t steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID userID = (uint64)steamID;
	SteamFriends()->ActivateGameOverlayInviteDialog(userID);
}
// Gets the small (32x32) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getSmallFriendAvatar(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetSmallFriendAvatar(userID);
}
// Gets the medium (64x64) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getMediumFriendAvatar(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetMediumFriendAvatar(userID);
}
// Gets the large (184x184) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getLargeFriendAvatar(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetLargeFriendAvatar(userID);
}
// Requests information about a user - persona name & avatar; if bRequireNameOnly is set, then the avatar of a user isn't downloaded.
bool Steam::requestUserInformation(uint64_t steamID, bool requireNameOnly){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->RequestUserInformation(userID, requireNameOnly);
}
// Requests information about a clan officer list; when complete, data is returned in ClanOfficerListResponse_t call result.
void Steam::requestClanOfficerList(uint64_t clanID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID clan = (uint64)clanID;
	SteamAPICall_t apiCall = SteamFriends()->GetFollowerCount(clan);
	callResultClanOfficerList.Set(apiCall, this, &Steam::_request_clan_officer_list);
}
// Returns the steamID of the clan owner.
uint64_t Steam::getClanOwner(uint64_t clanID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	CSteamID ownerID = SteamFriends()->GetClanOwner(clan);
	return ownerID.ConvertToUint64();
}
// Returns the number of officers in a clan (including the owner).
int Steam::getClanOfficerCount(uint64_t clanID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->GetClanOfficerCount(clan);
}
// Returns the steamID of a clan officer, by index, of range [0,GetClanOfficerCount).
uint64_t Steam::getClanOfficerByIndex(uint64_t clanID, int officer){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	CSteamID officerID = SteamFriends()->GetClanOfficerByIndex(clan, officer);
	return officerID.ConvertToUint64();
}
// If current user is chat restricted, he can't send or receive any text/voice chat messages. The user can't see custom avatars. But the user can be online and send/recv game invites.
uint32 Steam::getUserRestrictions(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetUserRestrictions();
}
// Set the game information in Steam; used in 'View Game Info'
bool Steam::setRichPresence(const String& key, const String& value){
	// Rich presence data is automatically shared between friends in the same game.
	// Each user has a set of key/value pairs, up to 20 can be set.
	// Two magic keys (status, connect).
	// setGameInfo() to an empty string deletes the key.
	if(SteamFriends() == NULL){
		return false;
	}
	return SteamFriends()->SetRichPresence(key.utf8().get_data(), value.utf8().get_data());
}
// Clear the game information in Steam; used in 'View Game Info'.
void Steam::clearRichPresence(){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ClearRichPresence();
}
// Get a Rich Presence value from a specified friend (typically only used for debugging).
String Steam::getFriendRichPresence(uint64_t friendID, const String& key){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID user = (uint64)friendID;
	return SteamFriends()->GetFriendRichPresence(user, key.utf8().get_data());
}
// Gets the number of Rich Presence keys that are set on the specified user.
int Steam::getFriendRichPresenceKeyCount(uint64_t friendID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user = (uint64)friendID;
	return SteamFriends()->GetFriendRichPresenceKeyCount(user);
}
// Returns an empty string ("") if the index is invalid or the specified user has no Rich Presence data available.
String Steam::getFriendRichPresenceKeyByIndex(uint64_t friendID, int key){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID user = (uint64)friendID;
	return SteamFriends()->GetFriendRichPresenceKeyByIndex(user, key);
}
// Requests rich presence for a specific user.
void Steam::requestFriendRichPresence(uint64_t friendID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID user = (uint64)friendID;
	return SteamFriends()->RequestFriendRichPresence(user);
}
// Invite friend to current game/lobby.
bool Steam::inviteUserToGame(uint64_t steamID, const String& connectString){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->InviteUserToGame(userID, connectString.utf8().get_data());
}
// Allows the user to join Steam group (clan) chats right within the game.
void Steam::joinClanChatRoom(uint64_t clanID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID clan = (uint64)clanID;
	SteamFriends()->JoinClanChatRoom(clan);
}
// Leaves a Steam group chat that the user has previously entered with JoinClanChatRoom.
bool Steam::leaveClanChatRoom(uint64_t clanID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->LeaveClanChatRoom(clan);
}
// Get the number of users in a Steam group chat.
int Steam::getClanChatMemberCount(uint64_t clanID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->GetClanChatMemberCount(clan);
}
// Gets the Steam ID at the given index in a Steam group chat.
uint64_t Steam::getChatMemberByIndex(uint64_t clanID, int user){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	CSteamID chatID = SteamFriends()->GetChatMemberByIndex(clan, user);
	return chatID.ConvertToUint64();
}
// Sends a message to a Steam group chat room.
bool Steam::sendClanChatMessage(uint64_t chatID, const String& text){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->SendClanChatMessage(chat, text.utf8().get_data());
}
// Checks if a user in the Steam group chat room is an admin.
bool Steam::isClanChatAdmin(uint64_t chatID, uint64_t steamID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->IsClanChatAdmin(chat, userID);
}
// Checks if the Steam Group chat room is open in the Steam UI.
bool Steam::isClanChatWindowOpenInSteam(uint64_t chatID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->IsClanChatWindowOpenInSteam(chat);
}
// Opens the specified Steam group chat room in the Steam UI.
bool Steam::openClanChatWindowInSteam(uint64_t chatID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->OpenClanChatWindowInSteam(chat);
}
// Closes the specified Steam group chat room in the Steam UI.
bool Steam::closeClanChatWindowInSteam(uint64_t chatID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->CloseClanChatWindowInSteam(chat);
}
// Listens for Steam friends chat messages.
bool Steam::setListenForFriendsMessages(bool intercept){
	if(SteamFriends() == NULL){
		return false;
	}
	return SteamFriends()->SetListenForFriendsMessages(intercept);
}
// Sends a message to a Steam friend.
bool Steam::replyToFriendMessage(uint64_t steamID, const String& message){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->ReplyToFriendMessage(userID, message.utf8().get_data());
}
// Gets the number of users following the specified user.
void Steam::getFollowerCount(uint64_t steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID userID = (uint64)steamID;
	SteamAPICall_t apiCall = SteamFriends()->GetFollowerCount(userID);
	callResultFollowerCount.Set(apiCall, this, &Steam::_get_follower_count);
}
// Checks if the current user is following the specified user.
void Steam::isFollowing(uint64_t steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID userID = (uint64)steamID;
	SteamAPICall_t apiCall = SteamFriends()->IsFollowing(userID);
	callResultIsFollowing.Set(apiCall, this, &Steam::_is_following);
}
// Gets the list of users that the current user is following.
void Steam::enumerateFollowingList(uint32 startIndex){
	if(SteamFriends() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamFriends()->EnumerateFollowingList(startIndex);
	callResultEnumerateFollowingList.Set(apiCall, this, &Steam::_enumerate_following_list);
}
// Checks if the Steam group is public.
bool Steam::isClanPublic(uint64_t clanID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->IsClanPublic(clan);
}
// Checks if the Steam group is an official game group/community hub.
bool Steam::isClanOfficialGameGroup(uint64_t clanID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->IsClanOfficialGameGroup(clan);
}
// Get list of players user has recently played game with.
Array Steam::getRecentPlayers(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int count = SteamFriends()->GetCoplayFriendCount();
	Array recents;
	for(int i = 0; i < count; i++){
		CSteamID playerID = SteamFriends()->GetCoplayFriend(i);
		if(SteamFriends()->GetFriendCoplayGame(playerID) == SteamUtils()->GetAppID()){
			Dictionary player;
			String name = SteamFriends()->GetFriendPersonaName(playerID);
			int time = SteamFriends()->GetFriendCoplayTime(playerID);
			int status = SteamFriends()->GetFriendPersonaState(playerID);
			player["id"] = playerID.GetAccountID();
			player["name"] = name;
			player["time"] = time;
			player["status"] = status;
			recents.append(player);
		}
	}
	return recents;
}
// Get player's avatar.
void Steam::getPlayerAvatar(int size, uint64_t steamID){
	// If no Steam ID is given, use the current user's
	if(steamID == 0){
		steamID = getSteamID();
	}
	if(size < AVATAR_SMALL || size > AVATAR_LARGE){
		return;
	}
	if(SteamFriends() == NULL){
		return;
	}
	int handle = -2;
	switch(size){
		case AVATAR_SMALL:{
			handle = getSmallFriendAvatar(steamID);
			size = 32; break;
		}
		case AVATAR_MEDIUM:{
			handle = getMediumFriendAvatar(steamID);
			size = 64; break;
		}
		case AVATAR_LARGE:{
			handle = getLargeFriendAvatar(steamID);
			size = 184; break;
		}
		default:
			return;
	}
	if(handle <= 0){
		printf("[Steam] Error retrieving avatar handle.");
		return;
	}
	AvatarImageLoaded_t* avatarData = new AvatarImageLoaded_t;
	CSteamID avatarID = (uint64)steamID;
	avatarData->m_steamID = avatarID;
	avatarData->m_iImage = handle;
	avatarData->m_iWide = size;
	avatarData->m_iTall = size;
	_avatar_loaded(avatarData);
	return;
}
// Get list of friends groups (tags) the user has created. This is not to be confused with Steam groups.
Array Steam::getUserFriendsGroups(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int tagCount = SteamFriends()->GetFriendsGroupCount();
	Array friendsGroups;
	for(int i = 0; i < tagCount; i++){
		Dictionary tags;
		int16 friendsGroupID = SteamFriends()->GetFriendsGroupIDByIndex(i);
		String groupName = SteamFriends()->GetFriendsGroupName(friendsGroupID);
		int groupMembers = SteamFriends()->GetFriendsGroupMembersCount(friendsGroupID);
		tags["id"] = friendsGroupID;
		tags["name"] = groupName;
		tags["members"] = groupMembers;
		friendsGroups.append(tags);
	}
	return friendsGroups;
}
// Get list of user's Steam groups; a mix of different Steamworks API group functions.
Array Steam::getUserSteamGroups(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int groupCount = SteamFriends()->GetClanCount();
	Array steamGroups;
	for(int i = 0; i < groupCount; i++){
		Dictionary groups;
		CSteamID groupID = SteamFriends()->GetClanByIndex(i);
		String name = SteamFriends()->GetClanName(groupID);
		String tag = SteamFriends()->GetClanTag(groupID);
		groups["id"] = groupID.GetAccountID();
		groups["name"] = name;
		groups["tag"] = tag;
		steamGroups.append(groups);
	}
	return steamGroups;
}
// Get a list of user's Steam friends; a mix of different Steamworks API friend functions.
Array Steam::getUserSteamFriends(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int count = SteamFriends()->GetFriendCount(0x04);
	Array steamFriends;
	for(int i = 0; i < count; i++){
		Dictionary friends;
		CSteamID friendID = SteamFriends()->GetFriendByIndex(i, 0x04);
		String name = SteamFriends()->GetFriendPersonaName(friendID);
		int status = SteamFriends()->GetFriendPersonaState(friendID);
		friends["id"] = friendID.GetAccountID();
		friends["name"] = name;
		friends["status"] = status;
		steamFriends.append(friends);
	}
	return steamFriends;
}
/////////////////////////////////////////////////
///// MATCHMAKING ///////////////////////////////
//
Array Steam::getFavoriteGames(){
	if(SteamMatchmaking() == NULL){
		return Array();
	}
	int count = SteamMatchmaking()->GetFavoriteGameCount();
	Array favorites;
	for(int i = 0; i < count; i++){
		Dictionary favorite;
		AppId_t appID = 0;
		uint32 ip = 0;
		uint16 port = 0;
		uint16 queryPort = 0;
		uint32 flags = 0;
		uint32 lastPlayed = 0;
		favorite["ret"] = SteamMatchmaking()->GetFavoriteGame(i, &appID, &ip, &port, &queryPort, &flags, &lastPlayed);
		if(favorite["ret"]){
			favorite["app"] = appID;
			// Convert the IP address back to a string
			const int NBYTES = 4;
			uint8 octet[NBYTES];
			char favoriteIP[16];
			for(int j = 0; j < NBYTES; j++){
				octet[j] = ip >> (j * 8);
			}
			sprintf_s(favoriteIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
			favorite["ip"] = favoriteIP;
			favorite["port"] = port;
			favorite["query"] = queryPort;
			favorite["flags"] = flags;
			favorite["played"] = lastPlayed;
			favorites.append(favorite);
		}
	}
	return favorites;
}
// Adds the game server to the local list; updates the time played of the server if it already exists in the list.
//int Steam::addFavoriteGame(int appID, uint32 ip, uint16 port, uint16 queryPort, uint32 flags, uint32 lastPlayed){
//	if(SteamMatchmaking() == NULL){
//		return 0;
//	}
//	return SteamMatchmaking()->AddFavoriteGame((AppId_t)appID, ip, port, queryPort, flags, lastPlayed);
//}
// Removes the game server from the local storage; returns true if one was removed.
bool Steam::removeFavoriteGame(AppId_t appID, uint32 ip, uint16 port, uint16 queryPort, uint32 flags){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	return SteamMatchmaking()->RemoveFavoriteGame(appID, ip, port, queryPort, flags);
}
// Get a list of relevant lobbies.
void Steam::requestLobbyList(){
	if(SteamMatchmaking() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamMatchmaking()->RequestLobbyList();
	callResultLobbyList.Set(apiCall, this, &Steam::_lobby_match_list);
}
// Adds a string comparison filter to the next RequestLobbyList call.
void Steam::addRequestLobbyListStringFilter(const String& keyToMatch, const String& valueToMatch, int comparisonType){
	if(SteamMatchmaking() == NULL){
		return;
	}
	ELobbyComparison type;
	if(comparisonType == LOBBY_EQUAL_LESS_THAN){
		type = k_ELobbyComparisonEqualToOrLessThan;
	}
	else if(comparisonType == LOBBY_LESS_THAN){
		type = k_ELobbyComparisonLessThan;
	}
	else if(comparisonType == LOBBY_EQUAL){
		type = k_ELobbyComparisonEqual;
	}
	else if(comparisonType == LOBBY_GREATER_THAN){
		type = k_ELobbyComparisonGreaterThan;
	}
	else if(comparisonType == LOBBY_EQUAL_GREATER_THAN){
		type = k_ELobbyComparisonEqualToOrGreaterThan;
	}
	else{
		type = k_ELobbyComparisonNotEqual;
	}
	SteamMatchmaking()->AddRequestLobbyListStringFilter(keyToMatch.utf8().get_data(), valueToMatch.utf8().get_data(), type);
}
// Adds a numerical comparison filter to the next RequestLobbyList call.
void Steam::addRequestLobbyListNumericalFilter(const String& keyToMatch, int valueToMatch, int comparisonType){
	if(SteamMatchmaking() == NULL){
		return;
	}
	ELobbyComparison type;
	if(comparisonType == LOBBY_EQUAL_LESS_THAN){
		type = k_ELobbyComparisonEqualToOrLessThan;
	}
	else if(comparisonType == LOBBY_LESS_THAN){
		type = k_ELobbyComparisonLessThan;
	}
	else if(comparisonType == LOBBY_EQUAL){
		type = k_ELobbyComparisonEqual;
	}
	else if(comparisonType == LOBBY_GREATER_THAN){
		type = k_ELobbyComparisonGreaterThan;
	}
	else if(comparisonType == LOBBY_EQUAL_GREATER_THAN){
		type = k_ELobbyComparisonEqualToOrGreaterThan;
	}
	else{
		type = k_ELobbyComparisonNotEqual;
	}
	SteamMatchmaking()->AddRequestLobbyListNumericalFilter(keyToMatch.utf8().get_data(), valueToMatch, type);
}
// Returns results closest to the specified value. Multiple near filters can be added, with early filters taking precedence.
void Steam::addRequestLobbyListNearValueFilter(const String& keyToMatch, int valueToBeCloseTo){
	if(SteamMatchmaking() == NULL){
		return;
	}
	SteamMatchmaking()->AddRequestLobbyListNearValueFilter(keyToMatch.utf8().get_data(), valueToBeCloseTo);
}
// Returns only lobbies with the specified number of slots available.
void Steam::addRequestLobbyListFilterSlotsAvailable(int slotsAvailable){
	if(SteamMatchmaking() == NULL){
		return;
	}
	SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(slotsAvailable);
}
// Sets the distance for which we should search for lobbies (based on users IP address to location map on the Steam backed).
void Steam::addRequestLobbyListDistanceFilter(int distanceFilter){
	if(SteamMatchmaking() == NULL){
		return;
	}
	ELobbyDistanceFilter distance;
	if(distanceFilter == LOBBY_DISTANCE_CLOSE){
		distance = k_ELobbyDistanceFilterClose;
	}
	else if(distanceFilter == LOBBY_DISTANCE_DEFAULT){
		distance = k_ELobbyDistanceFilterDefault;
	}
	else if(distanceFilter == LOBBY_DISTANCE_FAR){
		distance = k_ELobbyDistanceFilterFar;
	}
	else{
		distance = k_ELobbyDistanceFilterWorldwide;
	}
	SteamMatchmaking()->AddRequestLobbyListDistanceFilter(distance);
}
// Sets how many results to return, the lower the count the faster it is to download the lobby results & details to the client.
void Steam::addRequestLobbyListResultCountFilter(int maxResults){
	if(SteamMatchmaking() == NULL){
		return;
	}
	SteamMatchmaking()->AddRequestLobbyListResultCountFilter(maxResults);
}
// Create a lobby on the Steam servers, if private the lobby will not be returned by any RequestLobbyList() call.
void Steam::createLobby(int lobbyType, int maxMembers){
	if(SteamMatchmaking() == NULL){
		return;
	}
	ELobbyType eLobbyType;
	// Convert the lobby type back over.
	if(lobbyType == PRIVATE){
		eLobbyType = k_ELobbyTypePrivate;
	}
	else if(lobbyType == FRIENDS_ONLY){
		eLobbyType = k_ELobbyTypeFriendsOnly;
	}
	else if(lobbyType == PUBLIC){
		eLobbyType = k_ELobbyTypePublic;
	}
	else{
		eLobbyType = k_ELobbyTypeInvisible;
	}
	SteamAPICall_t apiCall = SteamMatchmaking()->CreateLobby(eLobbyType, maxMembers);
	callResultCreateLobby.Set(apiCall, this, &Steam::_lobby_created);
}
// Join an existing lobby.
void Steam::joinLobby(uint64_t steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	SteamMatchmaking()->JoinLobby(lobbyID);
}
// Leave a lobby, this will take effect immediately on the client side, other users will be notified by LobbyChatUpdate_t callback.
void Steam::leaveLobby(uint64_t steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	SteamMatchmaking()->LeaveLobby(lobbyID);
}
// Invite another user to the lobby, the target user will receive a LobbyInvite_t callback, will return true if the invite is successfully sent, whether or not the target responds.
bool Steam::inviteUserToLobby(uint64_t steamIDLobby, uint64_t steamIDInvitee){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	CSteamID inviteeID = (uint64)steamIDInvitee;
	return SteamMatchmaking()->InviteUserToLobby(lobbyID, inviteeID);
}
// Lobby iteration, for viewing details of users in a lobby.
int Steam::getNumLobbyMembers(uint64_t steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->GetNumLobbyMembers(lobbyID);
}
// Returns the CSteamID of a user in the lobby.
uint64_t Steam::getLobbyMemberByIndex(uint64_t steamIDLobby, int member){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	CSteamID lobbyMember = SteamMatchmaking()->GetLobbyMemberByIndex(lobbyID, member);
	return lobbyMember.ConvertToUint64();
}
// Get data associated with this lobby.
String Steam::getLobbyData(uint64_t steamIDLobby, const String& key){
	if(SteamMatchmaking() == NULL){
		return "";
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->GetLobbyData(lobbyID, key.utf8().get_data());
}
// Sets a key/value pair in the lobby metadata.
bool Steam::setLobbyData(uint64_t steamIDLobby, const String& key, const String& value){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->SetLobbyData(lobbyID, key.utf8().get_data(), value.utf8().get_data());
}
// Get lobby data by the lobby's ID
Dictionary Steam::getLobbyDataByIndex(uint64_t steamIDLobby){
	Dictionary data;
	if(SteamMatchmaking() == NULL){
		return data;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	int dataCount = SteamMatchmaking()->GetLobbyDataCount(lobbyID);
	char key;
	char value;
	for(int i = 0; i < dataCount; i++){
		bool success = SteamMatchmaking()->GetLobbyDataByIndex(lobbyID, i, &key, LOBBY_KEY_LENGTH, &value, CHAT_METADATA_MAX);
		if(success){
			data["index"] = i;
			data["key"] = key;
			data["value"] = value;
		}
	}
	return data;
}
// Removes a metadata key from the lobby.
bool Steam::deleteLobbyData(uint64_t steamIDLobby, const String& key){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->DeleteLobbyData(lobbyID, key.utf8().get_data());
}
// Gets per-user metadata for someone in this lobby.
String Steam::getLobbyMemberData(uint64_t steamIDLobby, uint64_t steamIDUser, const String& key){
	if(SteamMatchmaking() == NULL){
		return "";
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	CSteamID userID = (uint64)steamIDUser;
	return SteamMatchmaking()->GetLobbyMemberData(lobbyID, userID, key.utf8().get_data());
}
// Sets per-user metadata (for the local user implicitly).
void Steam::setLobbyMemberData(uint64_t steamIDLobby, const String& key, const String& value){
	if(SteamMatchmaking() == NULL){
		return;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	SteamMatchmaking()->SetLobbyMemberData(lobbyID, key.utf8().get_data(), value.utf8().get_data());
}
// Broadcasts a chat message to the all the users in the lobby.
bool Steam::sendLobbyChatMsg(uint64_t steamIDLobby, const String& messageBody){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	int messageLength = messageBody.length();
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->SendLobbyChatMsg(lobbyID, messageBody.utf8().get_data(), messageLength);
}
// Refreshes metadata for a lobby you're not necessarily in right now.
bool Steam::requestLobbyData(uint64_t steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->RequestLobbyData(lobbyID);
}
// Sets the game server associated with the lobby.
void Steam::setLobbyGameServer(uint64_t steamIDLobby, const String& serverIP, uint16 serverPort, uint64_t steamIDGameServer){
	if(SteamMatchmaking() == NULL){
		return;
	}
	// Resolve address and convert it from IP_Address struct to uint32_t
	IP_Address address;
	if(serverIP.is_valid_ip_address()){
		address = serverIP;
	}
	else{
		address = IP::get_singleton()->resolve_hostname(serverIP, IP::TYPE_IPV4);
	}
	// Resolution failed - Godot 3.0 has is_invalid() to check this
	if(address == IP_Address()){
		return;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	// Swap the bytes
	uint8_t *ip4_p = (uint8_t *)&ip4;
	for(int i = 0; i < 2; i++){
		uint8_t temp = ip4_p[i];
		ip4_p[i] = ip4_p[3-i];
		ip4_p[3-i] = temp;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	// If setting a game server with no server (fake) Steam ID
	if(steamIDGameServer == 0){
		SteamMatchmaking()->SetLobbyGameServer(lobbyID, *((uint32_t *)ip4_p), serverPort, k_steamIDNil);
	}
	else{
		CSteamID gameID = (uint64)steamIDGameServer;
		SteamMatchmaking()->SetLobbyGameServer(lobbyID, *((uint32_t *)ip4_p), serverPort, gameID);
	}
}
// Returns the details of a game server set in a lobby - returns false if there is no game server set, or that lobby doesn't exist.
Dictionary Steam::getLobbyGameServer(uint64_t steamIDLobby){
	Dictionary gameServer;
	CSteamID lobbyID = (uint64)steamIDLobby;
	uint32 serverIP = 0;
	uint16 serverPort = 0;
	CSteamID serverID;
	gameServer["ret"] = SteamMatchmaking()->GetLobbyGameServer(lobbyID, &serverIP, &serverPort, &serverID);
	if(gameServer["ret"]){
		// Convert the IP address back to a string
		const int NBYTES = 4;
		uint8 octet[NBYTES];
		char ip[16];
		for(int i = 0; i < NBYTES; i++){
			octet[i] = serverIP >> (i * 8);
		}
		sprintf_s(ip, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
		gameServer["ip"] = ip;
		gameServer["port"] = serverPort;
		// Convert the server ID
		int server = serverID.ConvertToUint64();
		gameServer["id"] = server;
	}
	return gameServer;
}
// Set the limit on the # of users who can join the lobby.
bool Steam::setLobbyMemberLimit(uint64_t steamIDLobby, int maxMembers){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->SetLobbyMemberLimit(lobbyID, maxMembers);
}
// Returns the current limit on the # of users who can join the lobby; returns 0 if no limit is defined.
int Steam::getLobbyMemberLimit(uint64_t steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->GetLobbyMemberLimit(lobbyID);
}
// Updates which type of lobby it is.
bool Steam::setLobbyType(uint64_t steamIDLobby, int eLobbyType){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	// Set the lobby type correctly
	ELobbyType lobbyType;
	if(eLobbyType == PRIVATE){
		lobbyType = k_ELobbyTypePrivate;
	}
	else if(eLobbyType == FRIENDS_ONLY){
		lobbyType = k_ELobbyTypeFriendsOnly;
	}
	else if(eLobbyType == PUBLIC){
		lobbyType = k_ELobbyTypePublic;
	}
	else{
		lobbyType = k_ELobbyTypeInvisible;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->SetLobbyType(lobbyID, lobbyType);
}
// Sets whether or not a lobby is joinable - defaults to true for a new lobby.
bool Steam::setLobbyJoinable(uint64_t steamIDLobby, bool joinable){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->SetLobbyJoinable(lobbyID, joinable);
}
// Returns the current lobby owner.
uint64_t Steam::getLobbyOwner(uint64_t steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	CSteamID ownerID = SteamMatchmaking()->GetLobbyOwner(lobbyID);
	return ownerID.ConvertToUint64();
}
// Changes who the lobby owner is.
bool Steam::setLobbyOwner(uint64_t steamIDLobby, uint64_t steamIDNewOwner){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	CSteamID ownerID = (uint64)steamIDNewOwner;
	return SteamMatchmaking()->SetLobbyOwner(lobbyID, ownerID);
}
// Link two lobbies for the purposes of checking player compatibility.
bool Steam::setLinkedLobby(uint64_t steamIDLobby, uint64_t steamIDLobbyDependent){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	CSteamID dependentID = (uint64)steamIDLobbyDependent;
	return SteamMatchmaking()->SetLinkedLobby(lobbyID, dependentID);
}
/////////////////////////////////////////////////
///// MUSIC /////////////////////////////////////
//
// Is Steam music enabled.
bool Steam::musicIsEnabled(){
	if(SteamMusic() == NULL){
		return false;
	}
	return SteamMusic()->BIsEnabled();
}
// Is Steam music playing something.
bool Steam::musicIsPlaying(){
	if(SteamMusic() == NULL){
		return false;
	}
	return SteamMusic()->BIsPlaying();
}
// Get the volume level of the music.
float Steam::musicGetVolume(){
	if(SteamMusic() == NULL){
		return 0;
	}
	return SteamMusic()->GetVolume();
}
// Pause whatever Steam music is playing.
void Steam::musicPause(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->Pause();
}
// Play current track/album.
void Steam::musicPlay(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->Play();
}
// Play next track/album.
void Steam::musicPlayNext(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->PlayNext();
}
// Play previous track/album.
void Steam::musicPlayPrev(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->PlayPrevious();
}
// Set the volume of Steam music.
void Steam::musicSetVolume(float value){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->SetVolume(value);
}
/////////////////////////////////////////////////
///// NETWORKING ////////////////////////////////
//
// This allows the game to specify accept an incoming packet.
bool Steam::acceptP2PSessionWithUser(uint64_t steamIDRemote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->AcceptP2PSessionWithUser(steamID);
}
// Allow or disallow P2P connections to fall back to being relayed through the Steam servers if a direct connection or NAT-traversal cannot be established.
bool Steam::allowP2PPacketRelay(bool allow) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	return SteamNetworking()->AllowP2PPacketRelay(allow);
}
// Closes a P2P channel when you're done talking to a user on the specific channel.
bool Steam::closeP2PChannelWithUser(uint64_t steamIDRemote, int channel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->CloseP2PChannelWithUser(steamID, channel);
}
// This should be called when you're done communicating with a user, as this will free up all of the resources allocated for the connection under-the-hood.
bool Steam::closeP2PSessionWithUser(uint64_t steamIDRemote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->CloseP2PSessionWithUser(steamID);
}
// Fills out a P2PSessionState_t structure with details about the connection like whether or not there is an active connection.
Dictionary Steam::getP2PSessionState(uint64_t steamIDRemote) {
	Dictionary result;
	if (SteamNetworking() == NULL) {
		return result;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	P2PSessionState_t p2pSessionState;
	bool success = SteamNetworking()->GetP2PSessionState(steamID, &p2pSessionState);
	if (!success) {
		return result;
	}
	result["connection_active"] = p2pSessionState.m_bConnectionActive; // true if we've got an active open connection
	result["connecting"] = p2pSessionState.m_bConnecting; // true if we're currently trying to establish a connection
	result["session_error"] = p2pSessionState.m_eP2PSessionError; // last error recorded (see enum in isteamnetworking.h)
	result["using_relay"] = p2pSessionState.m_bUsingRelay; // true if it's going through a relay server (TURN)
	result["bytes_queued_for_send"] = p2pSessionState.m_nBytesQueuedForSend;
	result["packets_queued_for_send"] = p2pSessionState.m_nPacketsQueuedForSend;
	result["remote_ip"] = p2pSessionState.m_nRemoteIP; // potential IP:Port of remote host. Could be TURN server.
	result["remote_port"] = p2pSessionState.m_nRemotePort; // Only exists for compatibility with older authentication api's
	return result;
}
// Calls IsP2PPacketAvailable() under the hood, returns the size of the available packet or zero if there is no such packet.
uint32_t Steam::getAvailableP2PPacketSize(int channel) {
	if (SteamNetworking() == NULL) {
		return 0;
	}
	uint32_t messageSize = 0;
	return (SteamNetworking()->IsP2PPacketAvailable(&messageSize, channel)) ? messageSize : 0;
}
// Reads in a packet that has been sent from another user via SendP2PPacket.
Dictionary Steam::readP2PPacket(uint32_t packet, int channel) {
	Dictionary result;
	if (SteamNetworking() == NULL) {
		return result;
	}
	PoolByteArray data;
	data.resize(packet);
	CSteamID steamID;
	uint32_t bytesRead = 0;
	if (SteamNetworking()->ReadP2PPacket(data.write().ptr(), packet, &bytesRead, &steamID, channel)) {
		data.resize(bytesRead);
		uint64_t steamIDRemote = steamID.ConvertToUint64();
		result["data"] = data;
		result["steamIDRemote"] = steamIDRemote;
	}
	else {
		data.resize(0);
	}
	return result;
}
// Sends a P2P packet to the specified user.
bool Steam::sendP2PPacket(uint64_t steamIDRemote, PoolByteArray data, int sendType, int channel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->SendP2PPacket(steamID, data.read().ptr(), data.size(), EP2PSend(sendType), channel);
}
/////////////////////////////////////////////////
///// REMOTE STORAGE ////////////////////////////
//
// Write to given file from Steam Cloud.
bool Steam::fileWrite(const String& file, const PoolByteArray& data, int32_t dataSize){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWrite(file.utf8().get_data(), data.read().ptr(), dataSize);
}
// Read given file from Steam Cloud.
Dictionary Steam::fileRead(const String& file, int32_t dataToRead){
	Dictionary d;
	if(SteamRemoteStorage() == NULL){
		d["ret"] = false;
		return d;
	}
	PoolByteArray data;
	data.resize(dataToRead);
	d["ret"] = SteamRemoteStorage()->FileRead(file.utf8().get_data(), data.write().ptr(), dataToRead);
	d["buf"] = data;
	return d;
}
// Delete file from remote storage but leave it on local disk to remain accessible.
bool Steam::fileForget(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileForget(file.utf8().get_data());
}
// Delete a given file in Steam Cloud.
bool Steam::fileDelete(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileDelete(file.utf8().get_data());
}
// Check if a given file exists in Steam Cloud.
bool Steam::fileExists(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileExists(file.utf8().get_data());
}
// Check if a given file is persisted in Steam Cloud.
bool Steam::filePersisted(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FilePersisted(file.utf8().get_data());
}
// Get the size of a given file.
int32_t Steam::getFileSize(const String& file){
	if(SteamRemoteStorage() == NULL){
		return -1;
	}
	return SteamRemoteStorage()->GetFileSize(file.utf8().get_data());
}
// Get the timestamp of when the file was uploaded/changed.
int64_t Steam::getFileTimestamp(const String& file){
	if(SteamRemoteStorage() == NULL){
		return -1;
	}
	return SteamRemoteStorage()->GetFileTimestamp(file.utf8().get_data());
}
// Gets the total number of local files synchronized by Steam Cloud.
int32_t Steam::getFileCount(){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->GetFileCount();
}
// Gets the file name and size of a file from the index.
Dictionary Steam::getFileNameAndSize(int file){
	Dictionary d;
	String name = "";
	int32_t size = 0;
	if(SteamRemoteStorage() != NULL){
		name = String(SteamRemoteStorage()->GetFileNameAndSize(file, &size));
	}
	d["name"] = name;
	d["size"] = size;
	return d;
}
// Gets the number of bytes available, and used on the users Steam Cloud storage.
Dictionary Steam::getQuota(){
	Dictionary d;
	uint64_t total = 0;
	uint64_t available = 0;
	if(SteamRemoteStorage() != NULL){
		SteamRemoteStorage()->GetQuota((uint64*)&total, (uint64*)&available);
	}
	d["total_bytes"] = total;
	d["available_bytes"] = available;
	return d;
}
// Obtains the platforms that the specified file will syncronize to.
uint32_t Steam::getSyncPlatforms(const String& file){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->GetSyncPlatforms(file.utf8().get_data());
}
// Is Steam Cloud enabled on the user's account?
bool Steam::isCloudEnabledForAccount(){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->IsCloudEnabledForAccount();
}
// Is Steam Cloud enabled for this application?
bool Steam::isCloudEnabledForApp(){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->IsCloudEnabledForApp();
}
// Set Steam Cloud enabled for this application.
void Steam::setCloudEnabledForApp(bool enabled){
	if(SteamRemoteStorage() == NULL){
		return;
	}
	return SteamRemoteStorage()->SetCloudEnabledForApp(enabled);
}
/////////////////////////////////////////////////
///// SCREENSHOTS ///////////////////////////////
//
// Adds a screenshot to the user's Steam screenshot library from disk.
uint32_t Steam::addScreenshotToLibrary(const String& filename, const String& thumbnailFilename, int width, int height){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->AddScreenshotToLibrary(filename.utf8().get_data(), thumbnailFilename.utf8().get_data(), width, height);
}
// Toggles whether the overlay handles screenshots.
void Steam::hookScreenshots(bool hook){
	if(SteamScreenshots() == NULL){
		return;
	}
	SteamScreenshots()->HookScreenshots(hook);
}
// Checks if the app is hooking screenshots.
bool Steam::isScreenshotsHooked(){
	if(SteamScreenshots() == NULL){
		return false;
	}
	return SteamScreenshots()->IsScreenshotsHooked();
}
// Sets optional metadata about a screenshot's location.
bool Steam::setLocation(uint32_t screenshot, const String& location){
	if(SteamScreenshots() == NULL){
		return false;
	}
	ScreenshotHandle handle = (ScreenshotHandle)screenshot;
	return SteamScreenshots()->SetLocation(handle, location.utf8().get_data());
}
// Causes Steam overlay to take a screenshot.
void Steam::triggerScreenshot(){
	if(SteamScreenshots() == NULL){
		return;
	}
	SteamScreenshots()->TriggerScreenshot();
}
// Writes a screenshot to the user's Steam screenshot library.
uint32_t Steam::writeScreenshot(const PoolByteArray& RGB, int width, int height){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->WriteScreenshot((void*)RGB.read().ptr(), RGB.size(), width, height);
}
/////////////////////////////////////////////////
///// SIGNALS ///////////////////////////////////
//
// Signal for file details acquired.
void Steam::_file_details_result(FileDetailsResult_t* fileData){
	uint32_t result = fileData->m_eResult;
	uint64_t fileSize = fileData->m_ulFileSize;
	uint32_t flags = fileData->m_unFlags;
	PoolByteArray fileHash;
	fileHash.resize(20);
	PoolByteArray::Write w = fileHash.write();
	for (int i=0; i<20; i++){
		w[i] = fileData->m_FileSHA[i];
	}
	emit_signal("file_details_result", result, fileSize, fileHash, flags);
}
// Signal the lobby has been created.
void Steam::_lobby_created(LobbyCreated_t* lobbyData, bool bIOFailure){
	int connect = lobbyData->m_eResult;
	// Convert the lobby response back over.
	if(lobbyData->m_eResult == k_EResultOK){
		connect = LOBBY_OK;
	}
	else if(lobbyData->m_eResult == k_EResultNoConnection){
		connect = LOBBY_NO_CONNECTION;
	}
	else if(lobbyData->m_eResult == k_EResultTimeout){
		connect = LOBBY_TIMEOUT;
	}
	else if(lobbyData->m_eResult == k_EResultFail){
		connect = LOBBY_FAIL;
	}
	else if(lobbyData->m_eResult == k_EResultAccessDenied){
		connect = LOBBY_ACCESS_DENIED;
	}
	else{
		connect = LOBBY_LIMIT_EXCEEDED;
	}
	CSteamID lobbyID = lobbyData->m_ulSteamIDLobby;
	uint64_t lobby = lobbyID.ConvertToUint64();
	emit_signal("lobby_created", connect, lobby);
}
// Signal that lobby has been joined.
void Steam::_lobby_joined(LobbyEnter_t* lobbyData){
	CSteamID steamLobbyID = lobbyData->m_ulSteamIDLobby;
	uint64_t lobbyID = steamLobbyID.ConvertToUint64();
	uint32_t permissions = lobbyData->m_rgfChatPermissions;
	bool locked = lobbyData->m_bLocked;
	uint32_t response = lobbyData->m_EChatRoomEnterResponse;
	emit_signal("lobby_joined", lobbyID, permissions, locked, response);
}
// Signal that a lobby invite was sent.
void Steam::_lobby_invite(LobbyInvite_t* lobbyData){
	CSteamID inviterID = lobbyData->m_ulSteamIDUser;
	uint64_t inviter = inviterID.ConvertToUint64();
	CSteamID lobbyID = lobbyData->m_ulSteamIDLobby;
	uint64_t lobby = lobbyID.ConvertToUint64();
	CSteamID gameID = lobbyData->m_ulGameID;
	uint64_t game = gameID.ConvertToUint64();
	emit_signal("lobby_invite", inviter, lobby, game);
}
// Signal a lobby join has been requested.
void Steam::_join_requested(GameLobbyJoinRequested_t* callData){
	uint64_t steamID = callData->m_steamIDFriend.GetAccountID();
	CSteamID lobbyID = callData->m_steamIDLobby;
	uint64_t lobby = lobbyID.ConvertToUint64();
	emit_signal("join_requested", steamID, lobby);
}
// Signal that a game join has been requested
void Steam::_join_game_requested(GameRichPresenceJoinRequested_t* callData){
	uint64_t steamID = callData->m_steamIDFriend.GetAccountID();
	String con_string = callData->m_rgchConnect;
	emit_signal("join_game_requested", steamID, con_string);
}
// Signal that the avatar has been loaded.
void Steam::_avatar_loaded(AvatarImageLoaded_t* avatarData){
	uint32 width, height;
	bool success = SteamUtils()->GetImageSize(avatarData->m_iImage, &width, &height);
	if(!success){
		printf("[Steam] Failed to get image size.\n");
		return;
	}
	PoolByteArray data;
	data.resize(width * height * 4);
	success = SteamUtils()->GetImageRGBA(avatarData->m_iImage, data.write().ptr(), data.size());
	if(!success){
		printf("[Steam] Failed to load image buffer from callback\n");
		return;
	}
	CSteamID steamID = avatarData->m_steamID;
	uint64_t avatarID = steamID.ConvertToUint64();
	call_deferred("emit_signal", "avatar_loaded", avatarID, width, data);
}
// Reports the result of an attempt to change the user's persona name.
void Steam::_name_changed(SetPersonaNameResponse_t *callData){
	bool success = callData->m_bSuccess;
	bool localSuccess = callData->m_bLocalSuccess;
	EResult result = callData->m_result;
	emit_signal("name_changed", success, localSuccess, result);
}
// A DownloadClanActivityCounts() call has finished.
void Steam::_clan_activity_downloaded(DownloadClanActivityCountsResult_t *callData){
	bool success = callData->m_bSuccess;
	// Set up the dictionary to populate
	Dictionary activity;
	if(success){
		int online = 0;
		int inGame = 0;
		int chatting = 0;
		activity["ret"] = SteamFriends()->GetClanActivityCounts(clanActivity, &online, &inGame, &chatting);
		if(activity["ret"]){
			activity["online"] = online;
			activity["ingame"] = inGame;
			activity["chatting"] = chatting;
		}
	}
	emit_signal("clan_activity_downloaded", activity);
}
// Marks the return of a request officer list call.
void Steam::_request_clan_officer_list(ClanOfficerListResponse_t *callData, bool bIOFailure){
	Array officersList;
	String message;
	if(!callData->m_bSuccess || bIOFailure){
		message = "Clan officer list response failed.";
	}
	else{
		CSteamID ownerSteamID = SteamFriends()->GetClanOwner(callData->m_steamIDClan);
		int officers = SteamFriends()->GetClanOfficerCount(callData->m_steamIDClan);
		message = "The owner of the clan is: " + String(SteamFriends()->GetFriendPersonaName(ownerSteamID)) + " (" + itos(ownerSteamID.ConvertToUint64()) + ") and there are " + itos(callData->m_cOfficers) + " officers.";
		for(int i = 0; i < officers; i++){
			Dictionary officer;
			CSteamID officerSteamID = SteamFriends()->GetClanOfficerByIndex(callData->m_steamIDClan, i);
			String name = SteamFriends()->GetFriendPersonaName(officerSteamID);
			int id = officerSteamID.ConvertToUint64();
			officer["id"] = id;
			officer["name"] = name;
			officersList.append(officer);
		}
	}
	emit_signal("request_clan_officer_list", message, officersList);
}
// Callback indicating updated data about friends rich presence information.
void Steam::_friend_rich_presence_update(FriendRichPresenceUpdate_t *callData){
	uint64_t steamID = callData->m_steamIDFriend.ConvertToUint64();
	AppId_t appID = callData->m_nAppID;
	emit_signal("friend_rich_presence_updated", steamID, appID);
}
void Steam::_join_clan_chat_complete(JoinClanChatRoomCompletionResult_t *callData){
	uint64_t chatID = callData->m_steamIDClanChat.ConvertToUint64();
	EChatRoomEnterResponse response = callData->m_eChatRoomEnterResponse;
	emit_signal("chat_join_complete", chatID, response);
}
// A chat message has been received for a clan chat the game has joined.
void Steam::_connected_clan_chat_message(GameConnectedClanChatMsg_t *callData){
	Dictionary chat;
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	CSteamID userID;
	chat["ret"] = SteamFriends()->GetClanChatMessage(callData->m_steamIDClanChat, callData->m_iMessageID, text, 2048, &type, &userID);
	chat["text"] = String(text);
	chat["type"] = type;
	chat["chatter"] = uint64_t(userID.ConvertToUint64());
	emit_signal("clan_chat_message", chat);
}
// A chat message has been received from a friend.
void Steam::_connected_friend_chat_message(GameConnectedFriendChatMsg_t *callData){
	uint64_t steamID = callData->m_steamIDUser.ConvertToUint64();
	int message = callData->m_iMessageID;
	Dictionary chat;
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	chat["ret"] = SteamFriends()->GetFriendMessage(createSteamID(steamID), message, text, 2048, &type);
	chat["text"] = String(text);
	emit_signal("friend_chat_message", chat);
}
// A user has joined a clan chat.
void Steam::_connected_chat_join(GameConnectedChatJoin_t *callData){
	uint64_t chatID = callData->m_steamIDClanChat.ConvertToUint64();
	uint64_t steamID = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("chat_joined", chatID, steamID);
}
// A user has left the chat we're in.
void Steam::_connected_chat_leave(GameConnectedChatLeave_t *callData){
	uint64_t chatID = callData->m_steamIDClanChat.ConvertToUint64();
	uint64_t steamID = callData->m_steamIDUser.ConvertToUint64();
	bool kicked = callData->m_bKicked;
	bool dropped = callData->m_bDropped;
	emit_signal("chat_left", chatID, steamID, kicked, dropped);
}
void Steam::_get_follower_count(FriendsGetFollowerCount_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	uint64_t steamID = callData->m_steamID.ConvertToUint64();
	int count = callData->m_nCount;
	emit_signal("follower_count", result, steamID, count);
}
void Steam::_is_following(FriendsIsFollowing_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	uint64_t steamID = callData->m_steamID.ConvertToUint64();
	bool following = callData->m_bIsFollowing;
	emit_signal("is_following", result, steamID, following);
}
void Steam::_enumerate_following_list(FriendsEnumerateFollowingList_t *callData, bool bIOFailure){
	Array following;
	String message;
	int followersParsed = 0;
	if(callData->m_eResult != k_EResultOK || bIOFailure){
		message = "Failed to acquire list.";
	}
	else{
		message = "Retrieved " + itos(callData->m_nResultsReturned) + " of " + itos(callData->m_nTotalResultCount) + " people followed.";
		int32 count = callData->m_nTotalResultCount;
		for(int i = 0; i < count; i++){
			Dictionary follow;
			int num = i;
			uint64_t id = callData->m_rgSteamID[i].ConvertToUint64();
			follow["num"] = num;
			follow["id"] = id;
			following.append(follow);
		}
		followersParsed += callData->m_nResultsReturned;
		// There are more followers so make another callback.
		if(followersParsed < count){
			SteamAPICall_t apiCall = SteamFriends()->EnumerateFollowingList(callData->m_nResultsReturned);
			callResultEnumerateFollowingList.Set(apiCall, this, &Steam::_enumerate_following_list);
		}
	}
	emit_signal("following_list", message, following);
}
// Signal for a user change
void Steam::_persona_state_change(PersonaStateChange_t *callData){
	uint64_t steamID = callData->m_ulSteamID;
	int flags = callData->m_nChangeFlags;
	emit_signal("persona_state_change", steamID, flags);
}
// Signal for list of matching lobbies
void Steam::_lobby_match_list(LobbyMatchList_t *callData, bool bIOFailure){
	uint32 lobbyCount = callData->m_nLobbiesMatching;
	Array lobbies;
	for(int i = 0; i < lobbyCount; i++){
		CSteamID lobbyID = SteamMatchmaking()->GetLobbyByIndex(i);
		uint64_t lobby = lobbyID.ConvertToUint64();
		lobbies.append(lobby);
	}	
	emit_signal("lobby_match_list", lobbies);
}
// Signal when a lobby game is created
void Steam::_lobby_game_created(LobbyGameCreated_t *callData){
	uint64_t lobbyID = callData->m_ulSteamIDLobby;
	uint64_t serverID = callData->m_ulSteamIDGameServer;
	uint32 ip = callData->m_unIP;
	uint16 port = callData->m_usPort;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char serverIP[16];
	for(int i = 0; i < NBYTES; i++){
		octet[i] = ip >> (i * 8);
	}
	sprintf_s(serverIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
	emit_signal("lobby_game_created", lobbyID, serverID, serverIP, port);
}
// Signal when a lobby chat message is received
void Steam::_lobby_Message(LobbyChatMsg_t *callData){
	uint64 lobbyID = callData->m_ulSteamIDLobby;
	uint64 userID = callData->m_ulSteamIDUser;
	uint8 chatType = callData->m_eChatEntryType;
	uint32 chatID = callData->m_iChatID;
	// Get the chat message data
	String data;
	// Get chat type
	EChatEntryType type;
	if(chatType == CHAT_INVALID){
		type = k_EChatEntryTypeInvalid;
	}
	else if(chatType == CHAT_MESSAGE){
		type = k_EChatEntryTypeChatMsg;
	}
	else if(chatType == CHAT_TYPING){
		type = k_EChatEntryTypeTyping;
	}
	else if(chatType == CHAT_INVITE_GAME){
		type = k_EChatEntryTypeInviteGame;
	}
	else if(chatType == CHAT_EMOTE){
		type = k_EChatEntryTypeEmote;
	}
	else if(chatType == CHAT_LEFT){
		type = k_EChatEntryTypeLeftConversation;
	}
	else if(chatType == CHAT_ENTERED){
		type = k_EChatEntryTypeEntered;
	}
	else if(chatType == CHAT_KICKED){
		type = k_EChatEntryTypeWasKicked;
	}
	else if(chatType == CHAT_BANNED){
		type = k_EChatEntryTypeWasBanned;
	}
	else if(chatType == CHAT_DISCONNECTED){
		type = k_EChatEntryTypeDisconnected;
	}
	else if(chatType == CHAT_HISTORICAL){
		type = k_EChatEntryTypeHistoricalChat;
	}
	else if(chatType == CHAT_LINK_BLOCKED){
		type = k_EChatEntryTypeLinkBlocked;
	}
	CSteamID steamID = userID;
	SteamMatchmaking()->GetLobbyChatEntry((CSteamID)lobbyID, chatID, &steamID, &data, 4096, &type);
	emit_signal("lobby_message_received", data.utf8().get_data(), type);
}
// Signal number of current players (online + offline).
void Steam::_number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure){
	emit_signal("number_of_current_players", callData->m_bSuccess && bIOFailure, callData->m_cPlayers);
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
// When connected to a server.
void Steam::_server_connected(SteamServersConnected_t* connectData){
	emit_signal("connection_changed", true);
}
// When disconnected from a server.
void Steam::_server_disconnected(SteamServersDisconnected_t* connectData){
	emit_signal("connection_changed", false);
}
// Posted after the user gains ownership of DLC & that DLC is installed.
void Steam::_dlc_installed(DlcInstalled_t* callData){
	int appID = (AppId_t)callData->m_nAppID;
	emit_signal("dlc_installed", appID);
}
// Response from getAuthSessionTicket.
void Steam::_get_auth_session_ticket_response(GetAuthSessionTicketResponse_t* callData){
	emit_signal("get_auth_session_ticket_response", callData->m_hAuthTicket, callData->m_eResult);
}
// Called when an auth ticket has been validated.
void Steam::_validate_auth_ticket_response(ValidateAuthTicketResponse_t* callData){
	uint64_t authID = callData->m_SteamID.ConvertToUint64();
	uint32_t response = callData->m_eAuthSessionResponse;
	uint64_t ownerID = callData->m_OwnerSteamID.ConvertToUint64();
	emit_signal("validate_auth_ticket_response", authID, response, ownerID);
}
// A screenshot has been requested by the user.
void Steam::_screenshot_ready(ScreenshotReady_t* callData){
	uint32_t handle = callData->m_hLocal;
	uint32_t result = callData->m_eResult;
	emit_signal("screenshot_ready", handle, result);
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
// Result of an achievement icon that has been fetched.
void Steam::_user_achievement_icon_fetched(UserAchievementIconFetched_t* callData){
	uint64_t gameID = callData->m_nGameID.ToUint64();
	String achievementName = callData->m_rgchAchievementName;
	bool achieved = callData->m_bAchieved;
	int iconHandle = callData->m_nIconHandle;
	emit_signal("user_achievement_icon_fetched", gameID, achievementName, achieved, iconHandle);
}
// Global achievements percentages are ready.
void Steam::_global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t* callData, bool bIOFailure){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	emit_signal("global_achievement_percentages_ready", game, result);
}
// Result of a workshop item being created.
void Steam::_workshop_item_created(CreateItemResult_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("workshop_item_created", result, (uint64_t)fileID, acceptTOS);
}
// Result of a workshop item being updated.
void Steam::_workshop_item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("workshop_item_updated", result, acceptTOS);
}
// Called when a workshop item has been installed or updated.
void Steam::_workshop_item_installed(ItemInstalled_t* callData){
	AppId_t appID = callData->m_unAppID;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	emit_signal("workshop_item_installed", appID, (uint64_t)fileID);
}
// User has sent a packet
void Steam::_p2p_session_request(P2PSessionRequest_t *callData){
	uint64_t steamIDRemote = callData->m_steamIDRemote.ConvertToUint64();
	emit_signal("p2p_session_request", steamIDRemote);
}
// We send a packet to another user but it failed
void Steam::_p2p_session_connect_fail(P2PSessionConnectFail_t *callData) {
	uint64_t steamIDRemote = callData->m_steamIDRemote.ConvertToUint64();
	uint8_t sessionError = callData->m_eP2PSessionError;
	emit_signal("p2p_session_connect_fail", steamIDRemote, sessionError);
}
/////////////////////////////////////////////////
///// USERS /////////////////////////////////////
//
// Get an authentication ticket.
uint32_t Steam::getAuthSessionTicket(){
	if(SteamUser() == NULL){
		return 0;
	}
	uint32_t ticketSize = 1024;
	uint32_t *buffer = memnew_arr(uint32_t, ticketSize);
	uint32_t id = SteamUser()->GetAuthSessionTicket(buffer, ticketSize, &ticketSize);
	TicketData ticket = { id, buffer, ticketSize };
	tickets.push_back(ticket);
	return id;
}
// Cancels an auth ticket.
void Steam::cancelAuthTicket(uint32_t authTicket){
	if(SteamUser() == NULL){
		return;
	}
	for(int i = 0; i < tickets.size(); i++){
		TicketData ticket = tickets.get(i);
		if (ticket.id == authTicket){
			tickets.remove(i);
			memdelete_arr(ticket.buffer);
			break;
		}
	}
}
// Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused.
int Steam::beginAuthSession(uint32_t authTicket, uint64_t steamID){
	if(SteamUser() == NULL){
		return -1;
	}
	for(int i = 0; i < tickets.size(); i++){
		TicketData ticket = tickets.get(i);
		if (ticket.id == authTicket){
			CSteamID authSteamID = createSteamID(steamID);
			return SteamUser()->BeginAuthSession(ticket.buffer, ticket.size, authSteamID);
		}
	}
	return -1;
}
// Ends an auth session.
void Steam::endAuthSession(uint64_t steamID){
	if(SteamUser() == NULL){
		return;
	}
	CSteamID authSteamID = createSteamID(steamID);
	SteamUser()->EndAuthSession(authSteamID);
}
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
// Get the user's Steam installation path (this function is depreciated).
String Steam::getUserDataFolder(){
	if(SteamUser() == NULL){
		return "";
	}
	const int bufferSize = 256;
	char *buffer = new char[bufferSize];
	SteamUser()->GetUserDataFolder((char*)buffer, bufferSize);
	String data_path = buffer;
	delete buffer;
	return data_path;
}
// (LEGACY FUNCTION) Set data to be replicated to friends so that they can join your game.
void Steam::advertiseGame(const String& serverIP, int port){
	if(SteamUser() == NULL){
		return;
	}
	// Resolve address and convert it from IP_Address struct to uint32_t
	IP_Address address;
	if(serverIP.is_valid_ip_address()){
		address = serverIP;
	}
	else{
		address = IP::get_singleton()->resolve_hostname(serverIP, IP::TYPE_IPV4);
	}
	// Resolution failed - Godot 3.0 has is_invalid() to check this
	if(address == IP_Address()){
		return;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	// Swap the bytes
	uint8_t *ip4_p = (uint8_t *)&ip4;
	for(int i = 0; i < 2; i++){
		uint8_t temp = ip4_p[i];
		ip4_p[i] = ip4_p[3-i];
		ip4_p[3-i] = temp;
	}
	CSteamID gameserverID = SteamUser()->GetSteamID();
	SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
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
// Get general attributes for an achievement
String Steam::getAchievementDisplayAttribute(const String& name, const String& key){
	if(SteamUserStats() == NULL){
		return "";
	}
	return SteamUserStats()->GetAchievementDisplayAttribute(name.utf8().get_data(), key.utf8().get_data());
}
//Gets the icon for an achievement
int Steam::getAchievementIcon(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->GetAchievementIcon(name.utf8().get_data());
}
// Gets the 'API name' for an achievement index
String Steam::getAchievementName(uint32_t iAchievement){
	if(SteamUserStats() == NULL){
		return "";
	}
	return SteamUserStats()->GetAchievementName((uint32)iAchievement);
}
//  Get the amount of players currently playing the current game (online + offline).
void Steam::getNumberOfCurrentPlayers(){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->GetNumberOfCurrentPlayers();
	callResultNumberOfCurrentPlayers.Set(apiCall, this, &Steam::_number_of_current_players);
}
// Get the number of achievements.
uint32_t Steam::getNumAchievements(){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->GetNumAchievements();
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
// Set the position where overlay shows notifications.
void Steam::setOverlayNotificationPosition(int pos){
	if((pos < 0) || (pos > 3) || (SteamUtils() == NULL)){
		return;
	}
	SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
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
// Gets the image bytes from an image handle.
Dictionary Steam::getImageRGBA(int image){
	Dictionary d;
	bool success = false;
	if(SteamUtils() != NULL){
		uint32 width;
		uint32 height;
		success = SteamUtils()->GetImageSize(image, &width, &height);
		if(success){
			PoolByteArray data;
			data.resize(width * height * 4);
			success = SteamUtils()->GetImageRGBA(image, data.write().ptr(), data.size());
			if(success){
				d["buffer"] = data;
			}
		}
	}
	d["success"] = success;
	return d;
}
// Gets the size of a Steam image handle.
Dictionary Steam::getImageSize(int image){
	Dictionary d;
	bool success = false;
	if(SteamUtils() != NULL){
		uint32 width;
		uint32 height;
		success = SteamUtils()->GetImageSize(image, &width, &height);
		d["width"] = width;
		d["height"] = height;
	}
	d["success"] = success;
	return d;
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
// Is Steam running in VR?
bool Steam::isSteamRunningInVR(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->IsSteamRunningInVR();
}
// Get the actual time.
int Steam::getServerRealTime(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetServerRealTime();
}
// Returns true if Steam & the Steam Overlay are running in Big Picture mode.
bool Steam::isSteamInBigPictureMode(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamInBigPictureMode();
}
// Ask SteamUI to create and render its OpenVR dashboard.
void Steam::startVRDashboard(){
	if(SteamUtils() == NULL){
		return ;
	}
	SteamUtils()->StartVRDashboard();
}
/////////////////////////////////////////////////
///// WORKSHOP //////////////////////////////////
//
// Download new or update already installed item. If returns true, wait for DownloadItemResult_t. If item is already installed, then files on disk should not be used until callback received.
// If item is not subscribed to, it will be cached for some time. If bHighPriority is set, any other item download will be suspended and this item downloaded ASAP.
bool Steam::downloadItem(int publishedFileID, bool highPriority){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->DownloadItem(fileID, highPriority);
}
// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends.
void Steam::suspendDownloads(bool suspend){
	return SteamUGC()->SuspendDownloads(suspend);
}
// Starts the item update process.
uint64_t Steam::startItemUpdate(AppId_t appID, int publishedFileID){
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->StartItemUpdate(appID, fileID);
}
// Gets the current state of a workshop item on this client.
int Steam::getItemState(int publishedFileID){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->GetItemState(fileID);
}
// Gets the progress of an item update.
Dictionary Steam::getItemUpdateProgress(uint64_t updateHandle){
	Dictionary updateProgress;
	UGCUpdateHandle_t handle = (uint64_t)updateHandle;
	uint64 processed = 0;
	uint64 total = 0;
	EItemUpdateStatus status = SteamUGC()->GetItemUpdateProgress(handle, &processed, &total);
	updateProgress["status"] = status;
	updateProgress["processed"] = uint64_t(processed);
	updateProgress["total"] = uint64_t(total);
	return updateProgress;
}
//
void Steam::createItem(AppId_t appID, int fileType){
	if(SteamUGC() == NULL){
		return;
	}
	EWorkshopFileType workshopType;
	// Convert the file type back over.
	if(fileType == UGC_ITEM_MAX){
		workshopType = k_EWorkshopFileTypeMax;
	}
	else if(fileType == UGC_ITEM_MICROTRANSACTION){
		workshopType = k_EWorkshopFileTypeMicrotransaction;
	}
	else if(fileType == UGC_ITEM_COLLECTION){
		workshopType = k_EWorkshopFileTypeCollection;
	}
	else if(fileType == UGC_ITEM_ART){
		workshopType = k_EWorkshopFileTypeArt;
	}
	else if(fileType == UGC_ITEM_VIDEO){
		workshopType = k_EWorkshopFileTypeVideo;
	}
	else if(fileType == UGC_ITEM_SCREENSHOT){
		workshopType = k_EWorkshopFileTypeScreenshot;
	}
	else if(fileType == UGC_ITEM_GAME){
		workshopType = k_EWorkshopFileTypeGame;
	}
	else if(fileType == UGC_ITEM_SOFTWARE){
		workshopType = k_EWorkshopFileTypeSoftware;
	}
	else if(fileType == UGC_ITEM_CONCEPT){
		workshopType = k_EWorkshopFileTypeConcept;
	}
	else if(fileType == UGC_ITEM_WEBGUIDE){
		workshopType = k_EWorkshopFileTypeWebGuide;
	}
	else if(fileType == UGC_ITEM_INTEGRATEDGUIDE){
		workshopType = k_EWorkshopFileTypeIntegratedGuide;
	}
	else if(fileType == UGC_ITEM_MERCH){
		workshopType = k_EWorkshopFileTypeMerch;
	}
	else if(fileType == UGC_ITEM_CONTROLLERBINDING){
		workshopType = k_EWorkshopFileTypeControllerBinding;
	}
	else if(fileType == UGC_ITEM_STEAMWORKSACCESSINVITE){
		workshopType = k_EWorkshopFileTypeSteamworksAccessInvite;
	}
	else if(fileType == UGC_ITEM_STEAMVIDEO){
		workshopType = k_EWorkshopFileTypeSteamVideo;
	}
	else if(fileType == UGC_ITEM_GAMEMANAGEDITEM){
		workshopType = k_EWorkshopFileTypeGameManagedItem;
	}
	else{
		workshopType = k_EWorkshopFileTypeCommunity;
	}
	SteamAPICall_t apiCall = SteamUGC()->CreateItem(appID, workshopType);
	callResultItemCreate.Set(apiCall, this, &Steam::_workshop_item_created);
}
// Sets a new title for an item.
bool Steam::setItemTitle(uint64_t updateHandle, const String& title){
	if(SteamUGC() == NULL){
		return false;
	}
	if (title.length() > UGC_MAX_TITLE_CHARS){
		printf("Title cannot have more than %ld ASCII characters. Title not set.", UGC_MAX_TITLE_CHARS);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemTitle(handle, title.utf8().get_data());
}
// Sets a new description for an item.
bool Steam::setItemDescription(uint64_t updateHandle, const String& description){
	if(SteamUGC() == NULL){
		return false;
	}
	if (description.length() > UGC_MAX_DESC_CHARS){
		printf("Description cannot have more than %ld ASCII characters. Description not set.", UGC_MAX_DESC_CHARS);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemDescription(handle, description.utf8().get_data());
}
// Sets the language of the title and description that will be set in this item update.
bool Steam::setItemUpdateLanguage(uint64_t updateHandle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}
// Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
bool Steam::setItemMetadata(uint64_t updateHandle, const String& metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	if (metadata.length() > UGC_MAX_METADATA_CHARS){
		printf("Metadata cannot have more than %ld ASCII characters. Metadata not set.", UGC_MAX_METADATA_CHARS);
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemMetadata(handle, metadata.utf8().get_data());
}
// Sets the visibility of an item.
bool Steam::setItemVisibility(uint64_t updateHandle, int visibility){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	ERemoteStoragePublishedFileVisibility itemVisibility;
	// Convert the visibility type back over.
	if(visibility == UGC_FILE_VISIBLE_PUBLIC){
		itemVisibility = k_ERemoteStoragePublishedFileVisibilityPublic;
	}
	else if(visibility == UGC_FILE_VISIBLE_FRIENDS){
		itemVisibility = k_ERemoteStoragePublishedFileVisibilityFriendsOnly;
	}
	else{
		itemVisibility = k_ERemoteStoragePublishedFileVisibilityPrivate;
	}
	return SteamUGC()->SetItemVisibility(handle, itemVisibility);
}
// Sets arbitrary developer specified tags on an item.
//bool Steam::setItemTags(uint64_t updateHandle, const String ** tagArray){
//	if(SteamUGC() == NULL){
//		return false;
//	}
//	UGCUpdateHandle_t handle = uint64(updateHandle);
//	SteamParamStringArray_t *tags = {tagArray};
//	return SteamUGC()->SetItemTags(updateHandle, tags);
//}
// Sets the folder that will be stored as the content for an item.
bool Steam::setItemContent(uint64_t updateHandle, const String& contentFolder){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemContent(handle, contentFolder.utf8().get_data());
}
// Sets the primary preview image for the item.
bool Steam::setItemPreview(uint64_t updateHandle, const String& previewFile){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemPreview(handle, previewFile.utf8().get_data());
}
// Uploads the changes made to an item to the Steam Workshop; to be called after setting your changes.
void Steam::submitItemUpdate(uint64_t updateHandle, const String& changeNote){
	if(SteamUGC() == NULL){
		return;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	SteamAPICall_t apiCall = SteamUGC()->SubmitItemUpdate(handle, changeNote.utf8().get_data());
	callResultItemUpdate.Set(apiCall, this, &Steam::_workshop_item_updated);
}
// Gets a list of all of the items the current user is subscribed to for the current game.
Array Steam::getSubscribedItems(){
	if(SteamUGC() == NULL){
		return Array();
	}
	Array subscribed;
	int numItems = SteamUGC()->GetNumSubscribedItems();
	PublishedFileId_t *items = new PublishedFileId_t[numItems];
	uint32 itemList = SteamUGC()->GetSubscribedItems(items, numItems);
	for(int i = 0; i < itemList; i++){
		subscribed.append((uint64_t)items[i]);
	}
	delete items;
	return subscribed;
}
// Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled set.
//Dictionary Steam::getItemInstallInfo(int fileID){
//	Dictionary info;
//	if(SteamUGC() == NULL){
//		info["ret"] = false;
//	}
//	else{
//		uint64 *sizeOnDisk;
//		String *folder;
//		uint32 *timeStamp;
//		info["ret"] = SteamUGC()->GetItemInstallInfo((PublishedFileId_t)fileID, sizeOnDisk, folder, folderSize, timeStamp);
//		if(info["ret"]){
//			info["size"] = sizeOnDisk;
//			info["folder"] = folder;
//			info["foldersize"] = folder.size();
//			info["timestamp"] = timeStamp;
//		}
//	}
//	return info;
//}
// Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
Dictionary Steam::getItemDownloadInfo(int fileID){
	Dictionary info;
	if(SteamUGC() == NULL){
		info["ret"] = false;
	}
	uint64 downloaded = 0;
	uint64 total = 0;
	info["ret"] = SteamUGC()->GetItemDownloadInfo((PublishedFileId_t)fileID, &downloaded, &total);
	if(info["ret"]){
		info["downloaded"] = uint64_t(downloaded);
		info["total"] = uint64_t(total);
	}
	return info;
}
/////////////////////////////////////////////////
///// BIND METHODS //////////////////////////////
//

void FriendGameInfo::_bind_methods(){
	
	//
	//define properties so that godot likes us :)
	//

	//set and gets

	ClassDB::bind_method(D_METHOD("set_friendID", "_friendID"), &FriendGameInfo::set_friendID);
	ClassDB::bind_method(D_METHOD("get_friendID"), &FriendGameInfo::get_friendID);

	ClassDB::bind_method(D_METHOD("set_gameID", "_gameID"), &FriendGameInfo::set_gameID);
	ClassDB::bind_method(D_METHOD("get_gameID"), &FriendGameInfo::get_gameID);

	ClassDB::bind_method(D_METHOD("set_appID", "_appID"), &FriendGameInfo::set_appID);
	ClassDB::bind_method(D_METHOD("get_appID"), &FriendGameInfo::get_appID);

	ClassDB::bind_method(D_METHOD("set_gameIP", "_gameIP"), &FriendGameInfo::set_gameIP);
	ClassDB::bind_method(D_METHOD("get_gameIP"), &FriendGameInfo::get_gameIP);
	ClassDB::bind_method(D_METHOD("get_gameIP_str"), &FriendGameInfo::get_gameIP_str);

	ClassDB::bind_method(D_METHOD("set_gamePort", "_gamePort"), &FriendGameInfo::set_gamePort);
	ClassDB::bind_method(D_METHOD("get_gamePort"), &FriendGameInfo::get_gamePort);

	ClassDB::bind_method(D_METHOD("set_queryPort", "_queryPort"), &FriendGameInfo::set_queryPort);
	ClassDB::bind_method(D_METHOD("get_queryPort"), &FriendGameInfo::get_queryPort);

	ClassDB::bind_method(D_METHOD("set_steamIDLobby", "_steamIDLobby"), &FriendGameInfo::set_steamIDLobby);
	ClassDB::bind_method(D_METHOD("get_steamIDLobby"), &FriendGameInfo::get_steamIDLobby);


	//extra helper functions
	ClassDB::bind_method(D_METHOD("get_dictionary"), &FriendGameInfo::get_dictionary);
	ClassDB::bind_method(D_METHOD("m_steamIDLobbyIsValid"), &FriendGameInfo::m_steamIDLobbyIsValid);

	//properties need the set and get methods registered so they get defined after the methods
	ADD_PROPERTY(PropertyInfo(Variant::INT, "gameID"), "set_gameID", "get_gameID");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "gameIP"), "set_gameIP", "get_gameIP");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "gamePort"), "set_gamePort", "get_gamePort");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "queryPort"), "set_queryPort", "get_queryPort");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "steamIDLobby"), "set_steamIDLobby", "get_steamIDLobby");


	//ClassDB::bind_method(D_METHOD("new_from_struct", "gameInfoStruct"), &FriendGameInfo::get_dictionary);//probably shouldn't be bound
}

void Steam::_bind_methods(){
	ClassDB::bind_method("restartAppIfNecessary", &Steam::restartAppIfNecessary);
	ClassDB::bind_method("steamInit", &Steam::steamInit);
	ClassDB::bind_method("isSteamRunning", &Steam::isSteamRunning);
	ClassDB::bind_method("run_callbacks", &Steam::run_callbacks);
	// Apps Bind Methods ////////////////////////
	ClassDB::bind_method("isSubscribed", &Steam::isSubscribed);
	ClassDB::bind_method("isLowViolence", &Steam::isLowViolence);
	ClassDB::bind_method("isCybercafe", &Steam::isCybercafe);
	ClassDB::bind_method("isVACBanned", &Steam::isVACBanned);
	ClassDB::bind_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
	ClassDB::bind_method("getAvailableGameLanguages", &Steam::getAvailableGameLanguages);
	ClassDB::bind_method("isSubscribedApp", &Steam::isSubscribedApp);
	ClassDB::bind_method("isDLCInstalled", &Steam::isDLCInstalled);
	ClassDB::bind_method("getEarliestPurchaseUnixTime", &Steam::getEarliestPurchaseUnixTime);
	ClassDB::bind_method("isSubscribedFromFreeWeekend", &Steam::isSubscribedFromFreeWeekend);
	ClassDB::bind_method("getDLCCount", &Steam::getDLCCount);
	ClassDB::bind_method("getDLCDataByIndex", &Steam::getDLCDataByIndex);
	ClassDB::bind_method("installDLC", &Steam::installDLC);
	ClassDB::bind_method("uninstallDLC", &Steam::uninstallDLC);
	ClassDB::bind_method("getCurrentBetaName", &Steam::getCurrentBetaName);
	ClassDB::bind_method("markContentCorrupt", &Steam::markContentCorrupt);
	ClassDB::bind_method("getInstalledDepots", &Steam::getInstalledDepots);
	ClassDB::bind_method("getAppInstallDir", &Steam::getAppInstallDir);
	ClassDB::bind_method("isAppInstalled", &Steam::isAppInstalled);
	ClassDB::bind_method("getAppOwner", &Steam::getAppOwner);
	ClassDB::bind_method("getLaunchQueryParam", &Steam::getLaunchQueryParam);
	ClassDB::bind_method("getDLCDownloadProgress", &Steam::getDLCDownloadProgress);
	ClassDB::bind_method("getAppBuildId", &Steam::getAppBuildId);
	ClassDB::bind_method("getFileDetails", &Steam::getFileDetails);
	// Controllers Bind Methods /////////////////
	ClassDB::bind_method("activateActionSet", &Steam::activateActionSet);
	ClassDB::bind_method("getActionSetHandle", &Steam::getActionSetHandle);
	ClassDB::bind_method("getAnalogActionData", &Steam::getAnalogActionData);
	ClassDB::bind_method("getAnalogActionHandle", &Steam::getAnalogActionHandle);
	ClassDB::bind_method("getAnalogActionOrigins", &Steam::getAnalogActionOrigins);
	ClassDB::bind_method("getConnectedControllers", &Steam::getConnectedControllers);
	ClassDB::bind_method("getControllerForGamepadIndex", &Steam::getControllerForGamepadIndex);
	ClassDB::bind_method("getCurrentActionSet", &Steam::getCurrentActionSet);
	ClassDB::bind_method("getDigitalActionData", &Steam::getDigitalActionData);
	ClassDB::bind_method("getDigitalActionHandle", &Steam::getDigitalActionHandle);
	ClassDB::bind_method("getDigitalActionOrigins", &Steam::getDigitalActionOrigins);
	ClassDB::bind_method("getMotionData", &Steam::getMotionData);
	ClassDB::bind_method("init", &Steam::init);
	ClassDB::bind_method("runFrame", &Steam::runFrame);
	ClassDB::bind_method("showBindingPanel", &Steam::showBindingPanel);
	ClassDB::bind_method("shutdown", &Steam::shutdown);
	ClassDB::bind_method("triggerVibration", &Steam::triggerVibration);
	// Friends Bind Methods /////////////////////
	ClassDB::bind_method("getPersonaName", &Steam::getPersonaName);
	ClassDB::bind_method("setPersonaName", &Steam::setPersonaName);
	ClassDB::bind_method("getPersonaState", &Steam::getPersonaState);
	ClassDB::bind_method("getFriendCount", &Steam::getFriendCount);
	ClassDB::bind_method("getFriendByIndex", &Steam::getFriendByIndex);
	ClassDB::bind_method("getFriendRelationship", &Steam::getFriendRelationship);
	ClassDB::bind_method("getFriendPersonaState", &Steam::getFriendPersonaState);
	ClassDB::bind_method("getFriendPersonaName", &Steam::getFriendPersonaName);
	ClassDB::bind_method(D_METHOD("getFriendGamePlayedD", "steamID"), &Steam::getFriendGamePlayedD);
	ClassDB::bind_method(D_METHOD("getFriendGamePlayed", "steamID", "friendGameInfo"), &Steam::getFriendGamePlayed);
	ClassDB::bind_method(D_METHOD("getFriendGameLobbies"), &Steam::getFriendGameLobbies);
	ClassDB::bind_method("getFriendPersonaNameHistory", &Steam::getFriendPersonaNameHistory);
	ClassDB::bind_method("getFriendSteamLevel", &Steam::getFriendSteamLevel);
	ClassDB::bind_method("getPlayerNickname", &Steam::getPlayerNickname);
	ClassDB::bind_method("hasFriend", &Steam::hasFriend);
	ClassDB::bind_method("downloadClanActivityCounts", &Steam::downloadClanActivityCounts);
	ClassDB::bind_method("getFriendCountFromSource", &Steam::getFriendCountFromSource);
	ClassDB::bind_method("getFriendFromSourceByIndex", &Steam::getFriendFromSourceByIndex);
	ClassDB::bind_method("isUserInSource", &Steam::isUserInSource);
	ClassDB::bind_method("setInGameVoiceSpeaking", &Steam::setInGameVoiceSpeaking);
	ClassDB::bind_method(D_METHOD("activateGameOverlay", "type"), &Steam::activateGameOverlay, DEFVAL(""));
	ClassDB::bind_method(D_METHOD("activateGameOverlayToUser", "type", "steamID"), &Steam::activateGameOverlayToUser, DEFVAL(""), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("activateGameOverlayToWebPage", "url"), &Steam::activateGameOverlayToWebPage);
	ClassDB::bind_method(D_METHOD("activateGameOverlayToStore", "appID"), &Steam::activateGameOverlayToStore, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("setPlayedWith", "steamID"), &Steam::setPlayedWith);
	ClassDB::bind_method(D_METHOD("activateGameOverlayInviteDialog", "steamID"), &Steam::activateGameOverlayInviteDialog);
	ClassDB::bind_method("getSmallFriendAvatar", &Steam::getSmallFriendAvatar);
	ClassDB::bind_method("getMediumFriendAvatar", &Steam::getMediumFriendAvatar);
	ClassDB::bind_method("getLargeFriendAvatar", &Steam::getLargeFriendAvatar);
	ClassDB::bind_method("requestUserInformation", &Steam::requestUserInformation);
	ClassDB::bind_method("requestClanOfficerList", &Steam::requestClanOfficerList);
	ClassDB::bind_method("getClanOwner", &Steam::getClanOwner);
	ClassDB::bind_method("getClanOfficerCount", &Steam::getClanOfficerCount);
	ClassDB::bind_method("getClanOfficerByIndex", &Steam::getClanOfficerByIndex);
	ClassDB::bind_method("getUserRestrictions", &Steam::getUserRestrictions);
	ClassDB::bind_method("setRichPresence", &Steam::setRichPresence);
	ClassDB::bind_method("clearRichPresence", &Steam::clearRichPresence);
	ClassDB::bind_method("getFriendRichPresence", &Steam::getFriendRichPresence);
	ClassDB::bind_method("getFriendRichPresenceKeyCount", &Steam::getFriendRichPresenceKeyCount);
	ClassDB::bind_method("getFriendRichPresenceKeyByIndex", &Steam::getFriendRichPresenceKeyByIndex);
	ClassDB::bind_method("requestFriendRichPresence", &Steam::requestFriendRichPresence);
	ClassDB::bind_method("inviteUserToGame", &Steam::inviteUserToGame);
	ClassDB::bind_method("joinClanChatRoom", &Steam::joinClanChatRoom);
	ClassDB::bind_method("leaveClanChatRoom", &Steam::leaveClanChatRoom);
	ClassDB::bind_method("getClanChatMemberCount", &Steam::getClanChatMemberCount);
	ClassDB::bind_method("getChatMemberByIndex", &Steam::getChatMemberByIndex);
	ClassDB::bind_method("sendClanChatMessage", &Steam::sendClanChatMessage);
	ClassDB::bind_method("isClanChatAdmin", &Steam::isClanChatAdmin);
	ClassDB::bind_method("isClanChatWindowOpenInSteam", &Steam::isClanChatWindowOpenInSteam);
	ClassDB::bind_method("openClanChatWindowInSteam", &Steam::openClanChatWindowInSteam);
	ClassDB::bind_method("closeClanChatWindowInSteam", &Steam::closeClanChatWindowInSteam);
	ClassDB::bind_method("setListenForFriendsMessages", &Steam::setListenForFriendsMessages);
	ClassDB::bind_method("replyToFriendMessage", &Steam::replyToFriendMessage);
	ClassDB::bind_method("getFollowerCount", &Steam::getFollowerCount);
	ClassDB::bind_method("isFollowing", &Steam::isFollowing);
	ClassDB::bind_method("enumerateFollowingList", &Steam::enumerateFollowingList);
	ClassDB::bind_method("isClanPublic", &Steam::isClanPublic);
	ClassDB::bind_method("isClanOfficialGameGroup", &Steam::isClanOfficialGameGroup);
	ClassDB::bind_method("getRecentPlayers", &Steam::getRecentPlayers);
	ClassDB::bind_method(D_METHOD("getPlayerAvatar", "size", "steamID"), &Steam::getPlayerAvatar, DEFVAL(int(AVATAR_MEDIUM)), DEFVAL(0));
	ClassDB::bind_method("getUserFriendsGroups", &Steam::getUserFriendsGroups);
	ClassDB::bind_method("getUserSteamGroups", &Steam::getUserSteamGroups);
	ClassDB::bind_method("getUserSteamFriends", &Steam::getUserSteamFriends);
	// Matchmaking Bind Methods /////////////////
	ClassDB::bind_method("getFavoriteGames", &Steam::getFavoriteGames);
//	ClassDB::bind_method("addFavoriteGame", &Steam::addFavoriteGame);
	ClassDB::bind_method("removeFavoriteGame", &Steam::removeFavoriteGame);
	ClassDB::bind_method("requestLobbyList", &Steam::requestLobbyList);
	ClassDB::bind_method("addRequestLobbyListStringFilter", &Steam::addRequestLobbyListStringFilter);
	ClassDB::bind_method("addRequestLobbyListNumericalFilter", &Steam::addRequestLobbyListNumericalFilter);
	ClassDB::bind_method("addRequestLobbyListNearValueFilter", &Steam::addRequestLobbyListNearValueFilter);
	ClassDB::bind_method("addRequestLobbyListFilterSlotsAvailable", &Steam::addRequestLobbyListFilterSlotsAvailable);
	ClassDB::bind_method("addRequestLobbyListDistanceFilter", &Steam::addRequestLobbyListDistanceFilter);
	ClassDB::bind_method("addRequestLobbyListResultCountFilter", &Steam::addRequestLobbyListResultCountFilter);
	ClassDB::bind_method(D_METHOD("createLobby", "type"), &Steam::createLobby, DEFVAL(2));
	ClassDB::bind_method("joinLobby", &Steam::joinLobby);
	ClassDB::bind_method("leaveLobby", &Steam::leaveLobby);
	ClassDB::bind_method("inviteUserToLobby", &Steam::inviteUserToLobby);
	ClassDB::bind_method("getNumLobbyMembers", &Steam::getNumLobbyMembers);
	ClassDB::bind_method("getLobbyMemberByIndex", &Steam::getLobbyMemberByIndex);
	ClassDB::bind_method("getLobbyData", &Steam::getLobbyData);
	ClassDB::bind_method("setLobbyData", &Steam::setLobbyData);
	ClassDB::bind_method("getLobbyDataByIndex", &Steam::getLobbyDataByIndex);
	ClassDB::bind_method("deleteLobbyData", &Steam::deleteLobbyData);
	ClassDB::bind_method("getLobbyMemberData", &Steam::getLobbyMemberData);
	ClassDB::bind_method("setLobbyMemberData", &Steam::setLobbyMemberData);
	ClassDB::bind_method("sendLobbyChatMsg", &Steam::sendLobbyChatMsg);
	ClassDB::bind_method("requestLobbyData", &Steam::requestLobbyData);
	ClassDB::bind_method("setLobbyGameServer", &Steam::setLobbyGameServer);
	ClassDB::bind_method("getLobbyGameServer", &Steam::getLobbyGameServer);
	ClassDB::bind_method("setLobbyMemberLimit", &Steam::setLobbyMemberLimit);
	ClassDB::bind_method("getLobbyMemberLimit", &Steam::getLobbyMemberLimit);
	ClassDB::bind_method("setLobbyType", &Steam::setLobbyType);
	ClassDB::bind_method("setLobbyJoinable", &Steam::setLobbyJoinable);
	ClassDB::bind_method("getLobbyOwner", &Steam::getLobbyOwner);
	ClassDB::bind_method("setLobbyOwner", &Steam::setLobbyOwner);
	ClassDB::bind_method("setLinkedLobby", &Steam::setLinkedLobby);
	// Music Bind Methods ///////////////////////
	ClassDB::bind_method("musicIsEnabled", &Steam::musicIsEnabled);
	ClassDB::bind_method("musicIsPlaying", &Steam::musicIsPlaying);
	ClassDB::bind_method("musicGetVolume", &Steam::musicGetVolume);
	ClassDB::bind_method("musicPause", &Steam::musicPause);
	ClassDB::bind_method("musicPlay", &Steam::musicPlay);
	ClassDB::bind_method("musicPlayNext", &Steam::musicPlayNext);
	ClassDB::bind_method("musicPlayPrev", &Steam::musicPlayPrev);
	ClassDB::bind_method("musicSetVolume", &Steam::musicSetVolume);
	// P2P Bind Methods /////////////////////////
	ClassDB::bind_method("acceptP2PSessionWithUser", &Steam::acceptP2PSessionWithUser);
	ClassDB::bind_method("allowP2PPacketRelay", &Steam::allowP2PPacketRelay);
	ClassDB::bind_method("closeP2PChannelWithUser", &Steam::closeP2PChannelWithUser);
	ClassDB::bind_method("closeP2PSessionWithUser", &Steam::closeP2PSessionWithUser);
	ClassDB::bind_method("getP2PSessionState", &Steam::getP2PSessionState);
	ClassDB::bind_method("getAvailableP2PPacketSize", &Steam::getAvailableP2PPacketSize);
	ClassDB::bind_method("readP2PPacket", &Steam::readP2PPacket);
	ClassDB::bind_method("sendP2PPacket", &Steam::sendP2PPacket);
	// Remote Storage Bind Methods //////////////
	ClassDB::bind_method("fileWrite", &Steam::fileWrite);
	ClassDB::bind_method("fileRead", &Steam::fileRead);
	ClassDB::bind_method("fileForget", &Steam::fileForget);
	ClassDB::bind_method("fileDelete", &Steam::fileDelete);
	ClassDB::bind_method("fileExists", &Steam::fileExists);
	ClassDB::bind_method("filePersisted", &Steam::filePersisted);
	ClassDB::bind_method("getFileSize", &Steam::getFileSize);
	ClassDB::bind_method("getFileTimestamp", &Steam::getFileTimestamp);
	ClassDB::bind_method("getFileCount", &Steam::getFileCount);
	ClassDB::bind_method("getFileNameAndSize", &Steam::getFileNameAndSize);
	ClassDB::bind_method("getQuota", &Steam::getQuota);
	ClassDB::bind_method("getSyncPlatforms", &Steam::getSyncPlatforms);
	ClassDB::bind_method("isCloudEnabledForAccount", &Steam::isCloudEnabledForAccount);
	ClassDB::bind_method("isCloudEnabledForApp", &Steam::isCloudEnabledForApp);
	ClassDB::bind_method("setCloudEnabledForApp", &Steam::setCloudEnabledForApp);
	// Screenshoot Bind Methods /////////////////
	ClassDB::bind_method("addScreenshotToLibrary", &Steam::addScreenshotToLibrary);
	ClassDB::bind_method("hookScreenshots", &Steam::hookScreenshots);
	ClassDB::bind_method("isScreenshotsHooked", &Steam::isScreenshotsHooked);
	ClassDB::bind_method("setLocation", &Steam::setLocation);
	ClassDB::bind_method("triggerScreenshot", &Steam::triggerScreenshot);
	ClassDB::bind_method("writeScreenshot", &Steam::writeScreenshot);
	// User Bind Methods ////////////////////////
	ClassDB::bind_method("getAuthSessionTicket", &Steam::getAuthSessionTicket);
	ClassDB::bind_method("cancelAuthTicket", &Steam::cancelAuthTicket);
	ClassDB::bind_method("beginAuthSession", &Steam::beginAuthSession);
	ClassDB::bind_method("endAuthSession", &Steam::endAuthSession);
	ClassDB::bind_method("getSteamID", &Steam::getSteamID);
	ClassDB::bind_method("loggedOn", &Steam::loggedOn);
	ClassDB::bind_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
	ClassDB::bind_method("getUserDataFolder", &Steam::getUserDataFolder);
	ClassDB::bind_method(D_METHOD("advertiseGame", "serverIP", "port"), &Steam::advertiseGame);
	ClassDB::bind_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
	// User Stats Bind Methods //////////////////
	ClassDB::bind_method("clearAchievement", &Steam::clearAchievement);
	ClassDB::bind_method("getAchievement", &Steam::getAchievement);
	ClassDB::bind_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
	ClassDB::bind_method(D_METHOD("getAchievementDisplayAttribute", "name", "key"), &Steam::getAchievementDisplayAttribute);
	ClassDB::bind_method(D_METHOD("getAchievementIcon", "name"), &Steam::getAchievementIcon);
	ClassDB::bind_method(D_METHOD("getAchievementName", "iAchievement"), &Steam::getAchievementName);
	ClassDB::bind_method("getNumAchievements", &Steam::getNumAchievements);
	ClassDB::bind_method("getNumberOfCurrentPlayers", &Steam::getNumberOfCurrentPlayers);
	ClassDB::bind_method("getStatFloat", &Steam::getStatFloat);
	ClassDB::bind_method("getStatInt", &Steam::getStatInt);
	ClassDB::bind_method("resetAllStats", &Steam::resetAllStats);
	ClassDB::bind_method("requestCurrentStats", &Steam::requestCurrentStats);
	ClassDB::bind_method("requestGlobalAchievementPercentages", &Steam::requestGlobalAchievementPercentages);
	ClassDB::bind_method("setAchievement", &Steam::setAchievement);
	ClassDB::bind_method("setStatFloat", &Steam::setStatFloat);
	ClassDB::bind_method("setStatInt", &Steam::setStatInt);
	ClassDB::bind_method("storeStats", &Steam::storeStats);
	ClassDB::bind_method(D_METHOD("findLeaderboard", "name"), &Steam::findLeaderboard);
	ClassDB::bind_method("getLeaderboardName", &Steam::getLeaderboardName);
	ClassDB::bind_method("getLeaderboardEntryCount", &Steam::getLeaderboardEntryCount);
	ClassDB::bind_method(D_METHOD("downloadLeaderboardEntries", "range_start", "range_end", "type"), &Steam::downloadLeaderboardEntries, DEFVAL(int(GLOBAL)));
	ClassDB::bind_method(D_METHOD("downloadLeaderboardEntriesForUsers", "usersID"), &Steam::downloadLeaderboardEntriesForUsers);
	ClassDB::bind_method(D_METHOD("uploadLeaderboardScore", "score", "keep_best", "details"), &Steam::uploadLeaderboardScore, DEFVAL(true), DEFVAL(PoolIntArray()));
	ClassDB::bind_method(D_METHOD("setLeaderboardDetailsMax", "details_max"), &Steam::setLeaderboardDetailsMax);
	ClassDB::bind_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
	ClassDB::bind_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
	ClassDB::bind_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);
	// Utils Bind Methods ///////////////////////
	ClassDB::bind_method("getIPCountry", &Steam::getIPCountry);
	ClassDB::bind_method("isOverlayEnabled", &Steam::isOverlayEnabled);
	ClassDB::bind_method("getSteamUILanguage", &Steam::getSteamUILanguage);
	ClassDB::bind_method("getAppID", &Steam::getAppID);
	ClassDB::bind_method(D_METHOD("getImageRGBA", "image"), &Steam::getImageRGBA);
	ClassDB::bind_method(D_METHOD("getImageSize", "image"), &Steam::getImageSize);
	ClassDB::bind_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
	ClassDB::bind_method(D_METHOD("setOverlayNotificationPosition", "pos"), &Steam::setOverlayNotificationPosition);
	ClassDB::bind_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
	ClassDB::bind_method("getServerRealTime", &Steam::getServerRealTime);
	ClassDB::bind_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
	ClassDB::bind_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
	ClassDB::bind_method("startVRDashboard", &Steam::startVRDashboard);
	// Workshop Bind Methods ////////////////////
	ClassDB::bind_method("downloadItem", &Steam::downloadItem);
	ClassDB::bind_method("suspendDownloads", &Steam::suspendDownloads);
	ClassDB::bind_method("startItemUpdate", &Steam::startItemUpdate);
	ClassDB::bind_method("getItemState", &Steam::getItemState);
	ClassDB::bind_method("getItemUpdateProgress", &Steam::getItemUpdateProgress);
	ClassDB::bind_method("createItem", &Steam::createItem);
	ClassDB::bind_method("setItemTitle", &Steam::setItemTitle);
	ClassDB::bind_method("setItemDescription", &Steam::setItemDescription);
	ClassDB::bind_method("setItemUpdateLanguage", &Steam::setItemUpdateLanguage);
	ClassDB::bind_method("setItemMetadata", &Steam::setItemMetadata);
	ClassDB::bind_method("setItemVisibility", &Steam::setItemVisibility);
//	ClassDB::bind_method("setItemTags", &Steam::setItemTags);
	ClassDB::bind_method("setItemContent", &Steam::setItemContent);
	ClassDB::bind_method("setItemPreview", &Steam::setItemPreview);
	ClassDB::bind_method("submitItemUpdate", &Steam::submitItemUpdate);
	ClassDB::bind_method("getSubscribedItems", &Steam::getSubscribedItems);
//	ClassDB::bind_method("getItemInstallInfo", &Steam::getItemInstallInfo);
	ClassDB::bind_method("getItemDownloadInfo", &Steam::getItemDownloadInfo);
	// Signals //////////////////////////////////
	ADD_SIGNAL(MethodInfo("file_details_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "fileSize"), PropertyInfo(Variant::INT, "fileHash"), PropertyInfo(Variant::INT, "flags")));
	ADD_SIGNAL(MethodInfo("join_requested", PropertyInfo(Variant::INT, "from"), PropertyInfo(Variant::STRING, "connect_string")));
	ADD_SIGNAL(MethodInfo("avatar_loaded", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "size")));
	ADD_SIGNAL(MethodInfo("name_changed"));
	ADD_SIGNAL(MethodInfo("clan_activity_downloaded"));
	ADD_SIGNAL(MethodInfo("request_clan_officer_list"));
	ADD_SIGNAL(MethodInfo("friend_rich_presence_update"));
	ADD_SIGNAL(MethodInfo("join_clan_chat_complete"));
	ADD_SIGNAL(MethodInfo("connected_clan_chat_message"));
	ADD_SIGNAL(MethodInfo("connected_chat_join"));
	ADD_SIGNAL(MethodInfo("connected_chat_leave"));
	ADD_SIGNAL(MethodInfo("get_follower_count"));
	ADD_SIGNAL(MethodInfo("is_following"));
	ADD_SIGNAL(MethodInfo("enumerate_following_list"));
	ADD_SIGNAL(MethodInfo("persona_state_change"));
	ADD_SIGNAL(MethodInfo("lobby_match_list"));
	ADD_SIGNAL(MethodInfo("lobby_game_created"));
	ADD_SIGNAL(MethodInfo("lobby_message_received"));
	ADD_SIGNAL(MethodInfo("number_of_current_players", PropertyInfo(Variant::BOOL, "success"), PropertyInfo(Variant::INT, "players")));
	ADD_SIGNAL(MethodInfo("leaderboard_loaded", PropertyInfo(Variant::INT, "leaderboard"), PropertyInfo(Variant::INT, "found")));
	ADD_SIGNAL(MethodInfo("leaderboard_uploaded", PropertyInfo(Variant::BOOL, "success"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::BOOL, "score_changed"), PropertyInfo(Variant::INT, "global_rank_new"), PropertyInfo(Variant::INT, "global_rank_previous")));
	ADD_SIGNAL(MethodInfo("leaderboard_entries_loaded"));
	ADD_SIGNAL(MethodInfo("overlay_toggled", PropertyInfo(Variant::BOOL, "active")));
	ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));
	ADD_SIGNAL(MethodInfo("lobby_created", PropertyInfo(Variant::INT, "lobby")));
	ADD_SIGNAL(MethodInfo("lobby_joined", PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "permissions"), PropertyInfo(Variant::BOOL, "locked"), PropertyInfo(Variant::INT, "response")));
	ADD_SIGNAL(MethodInfo("lobby_invite", PropertyInfo(Variant::INT, "inviter"), PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "game")));
	ADD_SIGNAL(MethodInfo("connection_changed", PropertyInfo(Variant::BOOL, "connected")));
	ADD_SIGNAL(MethodInfo("dlc_installed", PropertyInfo(Variant::INT, "app")));
	ADD_SIGNAL(MethodInfo("get_auth_session_ticket_response", PropertyInfo(Variant::INT, "ticket"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("validate_auth_ticket_response", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "auth_session_reponse"), PropertyInfo(Variant::INT, "owner_steamID")));
	ADD_SIGNAL(MethodInfo("screenshot_ready", PropertyInfo(Variant::INT, "screenshot_handle"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("user_stats_received", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "userID")));
	ADD_SIGNAL(MethodInfo("user_stats_stored"));
	ADD_SIGNAL(MethodInfo("user_achievement_stored"));
	ADD_SIGNAL(MethodInfo("user_achievement_icon_fetched", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::STRING, "achievementName"), PropertyInfo(Variant::BOOL, "achieved"), PropertyInfo(Variant::INT, "iconHandle")));
	ADD_SIGNAL(MethodInfo("global_achievement_percentages_ready", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("workshop_item_created"));
	ADD_SIGNAL(MethodInfo("workshop_item_updated"));
	ADD_SIGNAL(MethodInfo("workshop_item_installed"));
	ADD_SIGNAL(MethodInfo("p2p_session_request"));
	ADD_SIGNAL(MethodInfo("p2p_session_connect_fail"));
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
	// P2P constants ////////////////////////////
	BIND_CONSTANT(EP2P_SEND_UNRELIABLE);			// 0
	BIND_CONSTANT(EP2P_SEND_UNRELIABLE_NO_DELAY);	// 1 
	BIND_CONSTANT(EP2P_SEND_RELIABLE);				// 2
	BIND_CONSTANT(EP2P_SEND_RELIABLE_WITH_BUFFERING);	//3
	// Game ID types ////////////////////////////
	BIND_CONSTANT(GAMEID_TYPE_APP);					// 0
	BIND_CONSTANT(GAMEID_TYPE_MOD);					// 1
	BIND_CONSTANT(GAMEID_TYPE_SHORTCUT);			// 2
	BIND_CONSTANT(GAMEID_TYPE_P2P);					// 3
}

Steam::~Steam(){
	if(isInitSuccess){
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	for(int i = 0; i < tickets.size(); i++){
		TicketData ticket = tickets.get(i);
		memdelete_arr(ticket.buffer);
	}
	tickets.clear();
	singleton = NULL;
}

/////////////////////////////////////////////////
///// FRIEND GAME INFO FUNCTIONS ////////////////
/////////////////////////////////////////////////

//returns a dictionary of the
Dictionary FriendGameInfo::get_dictionary() {
	Dictionary friendGameDictionary;

	friendGameDictionary["id"] = gameID;
	friendGameDictionary["ip"] = get_gameIP_str();
	friendGameDictionary["gamePort"] = uint16(gamePort);
	friendGameDictionary["queryPort"] = uint16(queryPort);
	friendGameDictionary["lobby"] = uint64_t(steamIDLobby);
	
	return friendGameDictionary;
}

//creates a new FriendGameInfo object from a filled FriendGameInfo_t struct
Ref<FriendGameInfo> FriendGameInfo::new_from_struct(FriendGameInfo_t gameInfoStruct, uint64_t _friendID) {
	Ref<FriendGameInfo> newGameInfo(memnew(FriendGameInfo));
	newGameInfo->set_friendID(_friendID);
	newGameInfo->set_gameID(uint64_t(gameInfoStruct.m_gameID.ToUint64()));
	newGameInfo->set_appID(uint32_t(gameInfoStruct.m_gameID.AppID()));
	newGameInfo->set_gameIP(uint32_t(gameInfoStruct.m_unGameIP));
	newGameInfo->set_gamePort(uint16_t(gameInfoStruct.m_usGamePort));
	newGameInfo->set_queryPort(uint16_t(gameInfoStruct.m_usQueryPort));
	newGameInfo->set_steamIDLobby(uint64_t(gameInfoStruct.m_steamIDLobby.ConvertToUint64()));
	return newGameInfo;
}
