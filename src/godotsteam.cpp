#include "godotsteam.h"
#include "IP.hpp"

Steam *Steam::singleton = NULL;

Steam::Steam()
:
	lobbyDataUpdate(this, &Steam::_lobby_data_update),
	callSessionRequest(this, &Steam::_p2p_session_request),
	callSessionConnectFail(this, &Steam::_p2p_session_connect_fail)
{
	isInitSuccess = false;
	singleton = this;
//	tickets.clear();
}

Steam *Steam::get_singleton(){
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
CSteamID Steam::createSteamID(uint64_t steamID) {
	CSteamID cSteamID;
	cSteamID.SetFromUint64((uint64) steamID);
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
		// Load stats and achievements automatically
		SteamUserStats()->RequestCurrentStats();
	}
	return err;
}
bool Steam::initGameServer(String product, String gameDescription, String versionString, String modDir, uint16_t usSteamPort, uint16_t usGamePort, uint16_t usMasterServerUpdaterPort) {
	uint32 unIP = INADDR_ANY;
#ifdef USE_GS_AUTH_API
	EServerMode eMode = eServerModeAuthenticationAndSecure;
#else
	// Don't let Steam do authentication
	EServerMode eMode = eServerModeNoAuthentication;
#endif
	printf("Trying to init game server for %s (%s), version %s, mod dir = '%s'\n", product.utf8().get_data(), gameDescription.utf8().get_data(), versionString.utf8().get_data(), modDir.utf8().get_data());
	bool initRes = SteamGameServer_Init(unIP, usSteamPort, usGamePort, usMasterServerUpdaterPort, eMode, versionString.utf8().get_data());
	if (initRes) {
		if (SteamGameServer()) {
			// Set the "game dir".
			// This is currently required for all games.  However, soon we will be
			// using the AppID for most purposes, and this string will only be needed
			// for mods.  it may not be changed after the server has logged on
			SteamGameServer()->SetModDir(modDir.utf8().get_data());

			// These fields are currently required, but will go away soon.
			// See their documentation for more info
			SteamGameServer()->SetProduct(product.utf8().get_data());
			SteamGameServer()->SetGameDescription(gameDescription.utf8().get_data());

			// We don't support specators in our game.
			// .... but if we did:
			//SteamGameServer()->SetSpectatorPort( ... );
			//SteamGameServer()->SetSpectatorServerName( ... );

			// Initiate Anonymous logon.
			// Coming soon: Logging into authenticated, persistent game server account
			SteamGameServer()->LogOnAnonymous();

			// We want to actively update the master server with our presence so players can
			// find us via the steam matchmaking/server browser interfaces
#ifdef USE_GS_AUTH_API
			SteamGameServer()->EnableHeartbeats(true);
#endif
		} else {
			printf("SteamGameServer() interface is invalid\n");
		}
	}
	return initRes;
}
// Returns true/false if Steam is running
bool Steam::isSteamRunning(void){
	return SteamAPI_IsSteamRunning();
}
uint32 Steam::getServerPublicIP() {
	if (SteamGameServer() == NULL) {
		return 0;
	}
	return SteamGameServer()->GetPublicIP();
}
void Steam::_steam_servers_connected(SteamServersConnected_t *pLogonSuccess) {
	printf("Connected to Steam successfully\n");
}
void Steam::_steam_servers_connect_failure(SteamServerConnectFailure_t *pConnectFailure) {
	printf("Failed to connect to Steam\n");
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
		AppId_t appID;
		bool available;
		char name[128];
		bool success = SteamApps()->BGetDLCDataByIndex(i, &appID, &available, name, 128);
		if(success){
			Dictionary dlc;
			dlc["id"] = appID;
			dlc["available"] = &available;
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
//uint32_t Steam::getInstalledDepots(int appID, uint32* depots, uint32 maxDepots){
//	if(SteamApps() == NULL){
//		return 0;
//	}
//	return SteamApps()->GetInstalledDepots((AppId_t)appID, (DepotId_t*)depots, maxDepots);
//}
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
String Steam::getLaunchQueryParam(String key){
	if(SteamApps() == NULL){
		return "";
	}
	return SteamApps()->GetLaunchQueryParam(key.utf8().get_data());
}
// Gets the download progress for optional DLC.
//bool Steam::getDLCDownloadProgress(int appID, uint64* bytesDownloaded, uint64* bytesTotal){
//	if(SteamApps() == NULL){
//		return false;
//	}
//	return SteamApps()->GetDlcDownloadProgress((AppId_t)appID, bytesDownloaded, bytesTotal);
//}
// Return the build ID for this app; will change based on backend updates.
int Steam::getAppBuildId(){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetAppBuildId();
}
// Asynchronously retrieves metadata details about a specific file in the depot manifest.
void Steam::getFileDetails(String filename){
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
uint64_t Steam::getActionSetHandle(String actionSetName){
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
uint64_t Steam::getAnalogActionHandle(String actionName){
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
uint64_t Steam::getDigitalActionHandle(String actionName){
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
// Get number of friends user has
int Steam::getFriendCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendCount(0x04);
}
// Get the user's Steam username
String Steam::getPersonaName(){
	if(SteamFriends() == NULL){
		return "";
	}
	return SteamFriends()->GetPersonaName();
}
// Get given friend's Steam username
String Steam::getFriendPersonaName(uint64_t steamID){
	if(SteamFriends() != NULL && steamID > 0){
		CSteamID friendID = createSteamID(steamID);
		bool isDataLoading = SteamFriends()->RequestUserInformation(friendID, true);
		if(!isDataLoading){
			return SteamFriends()->GetFriendPersonaName(friendID);
		}
	}
	return "";
}
// Returns dictionary of friend game played if valid
Dictionary Steam::getFriendGamePlayedD(uint64_t steamID) {
	Dictionary friendGame;
	if (SteamFriends() == NULL) {
		return friendGame;
	}
	FriendGameInfo_t gameInfo;
	CSteamID userID = createSteamID(steamID);
	bool success = SteamFriends()->GetFriendGamePlayed(userID, &gameInfo);
	// If successful
	if (success) {
		// Is there a valid lobby?
		if (gameInfo.m_steamIDLobby.IsValid()) {
			friendGame["game_id"] = gameInfo.m_gameID.ToUint64();
			friendGame["id"] = gameInfo.m_gameID.AppID();
			// Convert the IP address back to a string
			const int NBYTES = 4;
			uint8 octet[NBYTES];
			char gameIP[16];
			for (int j = 0; j < NBYTES; j++) {
				octet[j] = gameInfo.m_unGameIP >> (j * 8);
			}
			sprintf_s(gameIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
			friendGame["ip"] = gameIP;
			friendGame["gamePort"] = gameInfo.m_usGamePort;
			friendGame["queryPort"] = (char)gameInfo.m_usQueryPort;
			friendGame["lobby"] = uint64_t(gameInfo.m_steamIDLobby.ConvertToUint64());
			friendGame["friendID"] = steamID;
		} else {
			friendGame["error"] = "No valid lobby";
		}
	}
	return friendGame;
}

Array Steam::getFriendGameLobbies() {
	Array returnArray;
	int cFriends = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);
	for (int i = 0; i < cFriends; i++) {
		CSteamID steamIDFriend = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagImmediate);
		uint64_t steamID = steamIDFriend.ConvertToUint64();
		Dictionary friendGameDictionary = getFriendGamePlayedD(steamID);
		if (!friendGameDictionary.empty() && !friendGameDictionary.has("error")) {
			returnArray.push_back(friendGameDictionary);
		}
	}
	return returnArray;
}
// Set the game information in Steam; used in 'View Game Info'
void Steam::setGameInfo(String key, String value){
	// Rich presence data is automatically shared between friends in the same game
	// Each user has a set of key/value pairs, up to 20 can be set
	// Two magic keys (status, connect)
	// setGameInfo() to an empty string deletes the key
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->SetRichPresence(key.utf8().get_data(), value.utf8().get_data());
}
// Clear the game information in Steam; used in 'View Game Info'
void Steam::clearGameInfo(){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ClearRichPresence();
}
// Invite friend to current game/lobby
void Steam::inviteFriend(uint64_t steamID, String connectString){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->InviteUserToGame(friendID, connectString.utf8().get_data());
}
// Set player as 'Played With' for game
void Steam::setPlayedWith(uint64_t steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->SetPlayedWith(friendID);
}
// Get list of players user has recently played game with
Array Steam::getRecentPlayers(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int rCount = SteamFriends()->GetCoplayFriendCount();
	Array recents;
	for(int i=0; i<rCount; i++){
		CSteamID playerID = SteamFriends()->GetCoplayFriend(i);
		if(SteamFriends()->GetFriendCoplayGame(playerID) == SteamUtils()->GetAppID()){
			Dictionary player;
			String name = SteamFriends()->GetFriendPersonaName(playerID);
			int status = SteamFriends()->GetFriendPersonaState(playerID);
			player["id"] = playerID.GetAccountID();
			player["name"] = name;
			player["status"] = status;
			recents.append(player);
		}
	}
	return recents;
}
// Get player's avatar
void Steam::getFriendAvatar(int size){
	if(size < AVATAR_SMALL || size > AVATAR_LARGE){
		return;
	}
	if(SteamFriends() == NULL){
		return;
	}
	int handle = -2;
	switch(size){
		case AVATAR_SMALL:{
			handle = SteamFriends()->GetSmallFriendAvatar( SteamUser()->GetSteamID() );
			size = 32; break;
		}
		case AVATAR_MEDIUM:{
			handle = SteamFriends()->GetMediumFriendAvatar( SteamUser()->GetSteamID() );
			size = 64; break;
		}
		case AVATAR_LARGE:{
			handle = SteamFriends()->GetLargeFriendAvatar( SteamUser()->GetSteamID() );
			size = 184; break;
		}
		default:
			return;
	}
	if(handle <= 0){
		if(handle == -1){
			// Still loading
			return;
		}
		// Invalid
		return;
	}
	// Has already loaded, simulate callback
	AvatarImageLoaded_t* avatarData = new AvatarImageLoaded_t;
	avatarData->m_steamID = SteamUser()->GetSteamID();
	avatarData->m_iImage = handle;
	avatarData->m_iWide = size;
	avatarData->m_iTall = size;
//	_avatar_loaded(avatarData);
	delete avatarData;
	return;
}
// Draw the given avatar
Image Steam::drawAvatar(int size, uint8* buffer){
	// Apply buffer to Image
	Image avatar;
	avatar.create(size, size, false, Image::FORMAT_RGBA8);
	for(int y = 0; y < size; y++){
		for(int x=0; x<size; x++){
			int i = 4*(x+y*size);
			float r = float(buffer[i])/255;
			float g = float(buffer[i+1])/255;
			float b = float(buffer[i+2])/255;
			float a = float(buffer[i+3])/255;
			avatar.set_pixel(x, y, Color(r, g, b, a));
		}
	}
	return avatar;
}
// Activates the overlay with optional dialog to open the following: "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite"
void Steam::activateGameOverlay(String url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
}
// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore"
void Steam::activateGameOverlayToUser(String url, uint64_t steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID overlayUserID = createSteamID(steamID);
	SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), overlayUserID);
}
// Activates the overlay with specified web address
void Steam::activateGameOverlayToWebPage(String url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToWebPage(url.utf8().get_data());
}
// Activates the overlay with the application/game Steam store page
void Steam::activateGameOverlayToStore(AppId_t appID){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToStore(appID, EOverlayToStoreFlag(0));
}
// Get list of user's Steam groups; a mix of different Steamworks API group functions
Array Steam::getUserSteamGroups(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int groupCount = SteamFriends()->GetClanCount();
	Array steamGroups;
	for(int i=0; i < groupCount; i++){
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
// Get a list of user's Steam friends; a mix of different Steamworks API friend functions
Array Steam::getUserSteamFriends(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int fCount = SteamFriends()->GetFriendCount(0x04);
	Array steamFriends;
	for(int i=0; i < fCount; i++){
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
// Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby
void Steam::activateGameOverlayInviteDialog(int steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID lobbyID = createSteamID(steamID);
	SteamFriends()->ActivateGameOverlayInviteDialog(lobbyID);
}
/////////////////////////////////////////////////
///// MATCHMAKING ///////////////////////////////
//
// Create a lobby on the Steam servers, if private the lobby will not be returned by any RequestLobbyList() call
void Steam::createLobby(int lobbyType, int maxMembers){
	if(SteamMatchmaking() == NULL){
		return;
	}
	ELobbyType eLobbyType;
	// Convert the lobby type back over
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
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(eLobbyType, maxMembers);
	callCreatedLobby.Set(hSteamAPICall, this, &Steam::_lobby_created);
}
void Steam::requestLobbyList() {
	if (SteamMatchmaking() == NULL) {
		return;
	}
	requestingLobbies = true;
	// request all lobbies for this game
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->RequestLobbyList();
	// set the function to call when this API call has completed
	steamCallResultLobbyMatchList.Set(hSteamAPICall, this, &Steam::_lobby_match_list);
}
// Join an existing lobby
void Steam::joinLobby(uint64_t steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->JoinLobby(lobbyID);
	callEnteredLobby.Set(hSteamAPICall, this, &Steam::_lobby_joined);
}
// Leave a lobby, this will take effect immediately on the client side, other users will be notified by LobbyChatUpdate_t callback
void Steam::leaveLobby(uint64_t steamIDLobby){
	CSteamID lobbyID = createSteamID(steamIDLobby);
	return SteamMatchmaking()->LeaveLobby(lobbyID);
}
// Invite another user to the lobby, the target user will receive a LobbyInvite_t callback, will return true if the invite is successfully sent, whether or not the target responds
bool Steam::inviteUserToLobby(uint64_t steamIDLobby, uint64_t steamIDInvitee){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	CSteamID inviteeID = createSteamID(steamIDInvitee);
	return SteamMatchmaking()->InviteUserToLobby(lobbyID, inviteeID);
}

int Steam::getNumLobbyMembers(uint64_t steamIDLobby) {
	if (SteamMatchmaking() == NULL) {
		return 0;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	return SteamMatchmaking()->GetNumLobbyMembers(lobbyID);
}

Array Steam::getLobbyMembersData(uint64_t steamIDLobby) {
	lobbyMembersData.clear();
	if (SteamMatchmaking() == NULL) {
		return lobbyMembersData;
	}

	CSteamID lobbyID = createSteamID(steamIDLobby);

	// list of users in lobby
	// iterate all the users in the lobby and show their details
	int cLobbyMembers = SteamMatchmaking()->GetNumLobbyMembers(lobbyID);
	CSteamID steamIDLobbyOwner = SteamMatchmaking()->GetLobbyOwner(lobbyID);
	for (int i = 0; i < cLobbyMembers; i++) {
		CSteamID steamIDLobbyMember = SteamMatchmaking()->GetLobbyMemberByIndex(lobbyID, i);

		bool bLobbyOwner = steamIDLobbyMember == steamIDLobbyOwner;
		Dictionary entryDict;
		uint64_t lobbyMemberID = (uint64_t)steamIDLobbyMember.ConvertToUint64();
		entryDict["steamIDLobbyMember"] = lobbyMemberID;
		entryDict["steamAccountIDLobbyMember"] = steamIDLobbyMember.GetAccountID();
		entryDict["owner"] = bLobbyOwner;
		lobbyMembersData.append(entryDict);
	}

	return lobbyMembersData;
}
// P2P //////////////////////////////
// see https://partner.steamgames.com/doc/api/ISteamNetworking
/**
 * This allows the game to specify accept an incoming packet.
 * This needs to be called before a real connection is established to a remote host, the game will get a chance to say whether or not the remote user is allowed to talk to them.
 * If you've called SendP2PPacket on the other user, this implicitly accepts the session request.
 * Note that this call should only be made in response to a P2PSessionRequest_t callback!
 * Returns: bool
 * true upon success; false only if steamIDRemote is invalid.
 */
bool Steam::acceptP2PSessionWithUser(uint64_t steamIDRemote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->AcceptP2PSessionWithUser(steamID);
}

/**
 * Allow or disallow P2P connections to fall back to being relayed through the Steam servers if a direct connection or NAT-traversal cannot be established.
 * This only applies to connections created after setting this value, or to existing connections that need to automatically reconnect after this value is set.
 * P2P packet relay is allowed by default.
 * Returns: bool
 * This function always returns true.
 */
bool Steam::allowP2PPacketRelay(bool bAllow) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	return SteamNetworking()->AllowP2PPacketRelay(bAllow);
}

/**
 * Closes a P2P channel when you're done talking to a user on the specific channel.
 * Once all channels to a user have been closed, the open session to the user will be closed and new data from this user will trigger a new P2PSessionRequest_t callback.
 * Returns: bool
 * true if the channel was successfully closed; otherwise, false if there was no active session or channel with the user.
 */
bool Steam::closeP2PChannelWithUser(uint64_t steamIDRemote, int nChannel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->CloseP2PChannelWithUser(steamID, nChannel);
}

/**
 * This should be called when you're done communicating with a user, as this will free up all of the resources allocated for the connection under-the-hood.
 * If the remote user tries to send data to you again, a new P2PSessionRequest_t callback will be posted.
 * Returns: bool
 * true if the session was successfully closed; otherwise, false if no connection was open with steamIDRemote.
 */
bool Steam::closeP2PSessionWithUser(uint64_t steamIDRemote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->CloseP2PSessionWithUser(steamID);
}

/**
 * Fills out a P2PSessionState_t structure with details about the connection like whether or not there is an active connection;
 * number of bytes queued on the connection; the last error code, if any; whether or not a relay server is being used; and the IP and Port of the remote user, if known
 * This should only needed for debugging purposes.
 */
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

/**
 * Calls IsP2PPacketAvailable() under the hood, returns the size of the available packet or zero if there is no such packet.
 * This should be called in a loop for each channel that you use. If there is a packet available you should call readP2PPacket to get the packet data.
 */
uint32_t Steam::getAvailableP2PPacketSize(int nChannel) {
	if (SteamNetworking() == NULL) {
		return 0;
	}
	uint32_t cubMsgSize = 0;
	return (SteamNetworking()->IsP2PPacketAvailable(&cubMsgSize, nChannel)) ? cubMsgSize : 0;
}

/**
 * Reads in a packet that has been sent from another user via SendP2PPacket.
 * If the cubDest buffer is too small for the packet, then the message will be truncated.
 * This call is not blocking, and will return empty dictionary if no data is available.
 * Before calling this you should have called IsP2PPacketAvailable.
 * Returns: bool
 * Filled dictionary if a packet was successfully read; otherwise, empty dictionary if no packet was available.
 */
Dictionary Steam::readP2PPacket(uint32_t cubDest, int nChannel) {
	Dictionary result;
	if (SteamNetworking() == NULL) {
		return result;
	}
	PoolByteArray data;
	data.resize(cubDest);
	CSteamID steamID;
	uint32_t bytesRead = 0;
	if (SteamNetworking()->ReadP2PPacket(data.write().ptr(), cubDest, &bytesRead, &steamID)) {
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
/**
 * Sends a P2P packet to the specified user.
 * This is a session-less API which automatically establishes NAT-traversing or Steam relay server connections.
 * NOTE: The first packet send may be delayed as the NAT-traversal code runs.
 * See EP2PSend for descriptions of the different ways of sending packets.
 * The type of data you send is arbitrary, you can use an off the shelf system like Protocol Buffers or Cap'n Proto to encode your packets in an efficient way,
 * or you can create your own messaging system.
 * Returns: bool
 * Triggers a P2PSessionRequest_t callback.
 * true if the packet was successfully sent.
 * Note that this does not mean successfully received, if we can't get through to the user after a timeout of 20 seconds, then an error will be posted
 * via the P2PSessionConnectFail_t callback.
 *
 * false upon the following conditions:
 * The packet is too large for the send type.
 * The target Steam ID is not valid.
 * There are too many bytes queued up to be sent.
 */
bool Steam::sendP2PPacket(uint64_t steamIDRemote, PoolByteArray vData, int eP2PSendType, int nChannel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steamID = createSteamID(steamIDRemote);
	return SteamNetworking()->SendP2PPacket(steamID, vData.read().ptr(), vData.size(), EP2PSend(eP2PSendType), nChannel);
}

/**
 * Purpose: another user has sent us a packet - do we accept?
 */
void Steam::_p2p_session_request(P2PSessionRequest_t *callData) {
	uint64_t steamIDRemote = callData->m_steamIDRemote.ConvertToUint64();
	owner->emit_signal("p2p_session_request", steamIDRemote);
}

/**
 * Purpose: we send a packet to another user but it failed
 */
void Steam::_p2p_session_connect_fail(P2PSessionConnectFail_t *callData) {
	uint64_t steamIDRemote = callData->m_steamIDRemote.ConvertToUint64();
	uint8_t p2p_session_error = callData->m_eP2PSessionError;
	owner->emit_signal("p2p_session_connect_fail", steamIDRemote, p2p_session_error);
}
/////////////////////////////////////////////////
///// MUSIC /////////////////////////////////////
//
// Is Steam music enabled
bool Steam::musicIsEnabled(){
	if(SteamMusic() == NULL){
		return false;
	}
	return SteamMusic()->BIsEnabled();
}
// Is Steam music playing something
bool Steam::musicIsPlaying(){
	if(SteamMusic() == NULL){
		return false;
	}
	return SteamMusic()->BIsPlaying();
}
// Get the volume level of the music
float Steam::musicGetVolume(){
	if(SteamMusic() == NULL){
		return 0;
	}
	return SteamMusic()->GetVolume();
}
// Pause whatever Steam music is playing
void Steam::musicPause(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->Pause();
}
// Play current track/album
void Steam::musicPlay(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->Play();
}
// Play next track/album
void Steam::musicPlayNext(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->PlayNext();
}
// Play previous track/album
void Steam::musicPlayPrev(){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->PlayPrevious();
}
// Set the volume of Steam music
void Steam::musicSetVolume(float value){
	if(SteamMusic() == NULL){
		return;
	}
	return SteamMusic()->SetVolume(value);
}
/////////////////////////////////////////////////
///// REMOTE STORAGE ////////////////////////////
//
// Write to given file from Steam Cloud
bool Steam::fileWrite(String file, const PoolByteArray& vData, int32_t cubData){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWrite(file.utf8().get_data(), vData.read().ptr(), cubData);
}
// Read given file from Steam Cloud
Dictionary Steam::fileRead(String file, int32_t cubDataToRead){
	Dictionary d;
	if(SteamRemoteStorage() == NULL){
		d["ret"] = false;
		return d;
	}
	PoolByteArray vData;
	vData.resize(cubDataToRead);
	d["ret"] = SteamRemoteStorage()->FileRead(file.utf8().get_data(), vData.write().ptr(), cubDataToRead);
	d["buf"] = vData;
	return d;
}
// Delete file from remote storage but leave it on local disk to remain accessible
bool Steam::fileForget(String file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileForget(file.utf8().get_data());
}
// Delete a given file in Steam Cloud
bool Steam::fileDelete(String file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileDelete(file.utf8().get_data());
}
// Check if a given file exists in Steam Cloud
bool Steam::fileExists(String file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileExists(file.utf8().get_data());
}
// Check if a given file is persisted in Steam Cloud
bool Steam::filePersisted(String file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FilePersisted(file.utf8().get_data());
}
// Get the size of a given file
int32_t Steam::getFileSize(String file){
	if(SteamRemoteStorage() == NULL){
		return -1;
	}
	return SteamRemoteStorage()->GetFileSize(file.utf8().get_data());
}
// Get the timestamp of when the file was uploaded/changed
int64_t Steam::getFileTimestamp(String file){
	if(SteamRemoteStorage() == NULL){
		return -1;
	}
	return SteamRemoteStorage()->GetFileTimestamp(file.utf8().get_data());
}
// Gets the total number of local files synchronized by Steam Cloud
int32_t Steam::getFileCount(){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->GetFileCount();
}
// Gets the file name and size of a file from the index
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
// Gets the number of bytes available, and used on the users Steam Cloud storage
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
// Obtains the platforms that the specified file will syncronize to
uint32_t Steam::getSyncPlatforms(String file){
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
// Set Steam Cloud enabled for this application
void Steam::setCloudEnabledForApp(bool enabled){
	if(SteamRemoteStorage() == NULL){
		return;
	}
	return SteamRemoteStorage()->SetCloudEnabledForApp(enabled);
}
/////////////////////////////////////////////////
///// SCREENSHOTS ///////////////////////////////
//
uint32_t Steam::addScreenshotToLibrary(String filename, String thumbnailFilename, int width, int height) {
	// TODO: implement
	return 0;
}

// Toggles whether the overlay handles screenshots
void Steam::hookScreenshots(bool hook){
	if(SteamScreenshots() == NULL){
		return;
	}
	SteamScreenshots()->HookScreenshots(hook);
}
// Checks if the app is hooking screenshots
bool Steam::isScreenshotsHooked(){
	if(SteamScreenshots() == NULL){
		return false;
	}
	return SteamScreenshots()->IsScreenshotsHooked();
}

bool Steam::setLocation(uint32_t screenshot, String location) {
	// TODO: implement
	return false;
}

// Causes Steam overlay to take a screenshot
void Steam::triggerScreenshot(){
	if(SteamScreenshots() == NULL){
		return;
	}
	SteamScreenshots()->TriggerScreenshot();
}
// Writes a screenshot to the user's Steam screenshot library
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
	int fileHash = fileData->m_FileSHA[20];
	uint32_t flags = fileData->m_unFlags;
	owner->emit_signal("file_details_result", result, fileSize, fileHash, flags);
}
// Signal the lobby has been created.
void Steam::_lobby_created(LobbyCreated_t* lobbyData, bool bIOFailure){
	int connect;
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
	uint64_t lobbyID = (uint64_t) lobbyData->m_ulSteamIDLobby;
	owner->emit_signal("lobby_created", connect, lobbyID);
}
// Sets lobby data
bool Steam::setLobbyData(uint64_t steamIDLobby, String key, String value) {
	if (SteamMatchmaking() == NULL) {
		return false;
	}
	return SteamMatchmaking()->SetLobbyData(createSteamID(steamIDLobby), key.utf8().get_data(), value.utf8().get_data());
}
// Gets lobby data
String Steam::getLobbyData(uint64_t steamIDLobby, String key) {
	if (SteamMatchmaking() == NULL) {
		return String();
	}
	return SteamMatchmaking()->GetLobbyData(createSteamID(steamIDLobby), key.utf8().get_data());
}
// Sets lobby member data
void Steam::setLobbyMemberData(uint64_t steamIDLobby, String key, String value) {
	if (SteamMatchmaking() == NULL) {
		return;
	}
	SteamMatchmaking()->SetLobbyMemberData(createSteamID(steamIDLobby), key.utf8().get_data(), value.utf8().get_data());
}
// Gets lobby member data
String Steam::getLobbyMemberData(uint64_t steamIDLobby, uint64_t steamIDUser, String key) {
	if (SteamMatchmaking() == NULL) {
		return String();
	}
	return SteamMatchmaking()->GetLobbyMemberData(createSteamID(steamIDLobby), createSteamID(steamIDUser), key.utf8().get_data());
}
/**
 * Triggers on calls to CreateLobby, JoinLobby, SetLobbyMemberData, RequestLobbyData, SetLobbyOwner.
 * If steamIDMember is a user in the lobby, then use GetLobbyMemberData to access per-user details;
 * otherwise, if steamIDMember == steamIDLobby, use GetLobbyData to access the lobby metadata.
 */
void Steam::_lobby_data_update(LobbyDataUpdate_t *callData) {
	uint8_t success = callData->m_bSuccess;
	uint64_t steamIDLobby = callData->m_ulSteamIDLobby;
	uint64_t steamIDMember = callData->m_ulSteamIDMember;
	owner->emit_signal("lobby_data_update", success, steamIDLobby, steamIDMember);
}
// Signal the lobby match list was performed
void Steam::_lobby_match_list(LobbyMatchList_t *pCallback, bool bIOFailure) {
	listLobbies.clear();
	requestingLobbies = false;

	if (bIOFailure)
	{
		// we had a Steam I/O failure - we probably timed out talking to the Steam back-end servers
		// doesn't matter in this case, we can just act if no lobbies were received
	}

	// lobbies are returned in order of closeness to the user, so add them to the list in that order
	for (uint32 iLobby = 0; iLobby < pCallback->m_nLobbiesMatching; iLobby++)
	{
		CSteamID steamIDLobby = SteamMatchmaking()->GetLobbyByIndex(iLobby);
		uint64_t lobbyID = (uint64_t) steamIDLobby.ConvertToUint64();
		Dictionary entryDict;
		entryDict["steamIDLobby"] = lobbyID;
		listLobbies.append(entryDict);
	}
	owner->emit_signal("lobby_match_list");
}
// Signal that lobby has been joined.
void Steam::_lobby_joined(LobbyEnter_t* lobbyData, bool bIOFailure){
	uint64_t lobbyID = (uint64_t) lobbyData->m_ulSteamIDLobby;
	uint32_t permissions = lobbyData->m_rgfChatPermissions;
	bool locked = lobbyData->m_bLocked;
	uint32_t response = lobbyData->m_EChatRoomEnterResponse;
	bool connection_failure = bIOFailure || (lobbyData->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess);
	owner->emit_signal("lobby_joined", lobbyID, permissions, locked, response, connection_failure);
}
// Signal that a lobby invite was sent.
void Steam::_lobby_invite(LobbyInvite_t* lobbyData){
	uint64_t inviterID = (uint64_t)lobbyData->m_ulSteamIDUser;
	uint64_t lobbyID = (uint64_t)lobbyData->m_ulSteamIDLobby;
	uint64_t gameID = (uint64_t)lobbyData->m_ulGameID;
	owner->emit_signal("lobby_invite", inviterID, lobbyID, gameID);
}
// Signal a game/lobby join has been requested.
void Steam::_join_requested(GameRichPresenceJoinRequested_t* callData){
	int steamID = callData->m_steamIDFriend.GetAccountID();
	String con_string = callData->m_rgchConnect;
	owner->emit_signal("join_requested", steamID, con_string);
}
// Signal that the avatar has been loaded.
void Steam::_avatar_loaded(AvatarImageLoaded_t* avatarData){
	if(avatarData->m_steamID != SteamUser()->GetSteamID()){
		return;
	}
	int size = avatarData->m_iWide;
	// Get image buffer
	int bufferSize = 4 * size * size;
	uint8* buffer = new uint8[bufferSize];
	bool success = SteamUtils()->GetImageRGBA(avatarData->m_iImage, buffer, bufferSize);
	if(!success){
		printf("[Steam] Failed to load image buffer from callback\n");
		return;
	}
	int avatarSize;
	if(size == 32){
		avatarSize = AVATAR_SMALL;
	}
	else if(size == 64){
		avatarSize = AVATAR_MEDIUM;
	}
	else if(size == 184){
		avatarSize = AVATAR_LARGE;
	}
	else{
		printf("[Steam] Invalid avatar size from callback\n");
		return;
	}
	Image avatar = drawAvatar(size, buffer);
//	call_deferred("owner->emit_signal", "avatar_loaded", avatarSize, avatar);	Temporarily disabled because of Image change in Godot.
}
// Signal number of current players (online + offline).
void Steam::_number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure){
	owner->emit_signal("number_of_current_players", callData->m_bSuccess && !bIOFailure, callData->m_cPlayers);
}
// Signal a leaderboard has been loaded or has failed.
void Steam::_leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure){
	leaderboard_handle = callData->m_hSteamLeaderboard;
	uint8_t found = callData->m_bLeaderboardFound;
	owner->emit_signal("leaderboard_loaded", (uint64_t)leaderboard_handle, found);
}
// Signal a leaderboard entry has been uploaded.
void Steam::_leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure){
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboard_handle){
		return;
	}
	owner->emit_signal("leaderboard_uploaded", callData->m_bSuccess && !bIOFailure, callData->m_nScore, callData->m_bScoreChanged, callData->m_nGlobalRankNew, callData->m_nGlobalRankPrevious);
}
// Signal leaderboard entries are downloaded.
void Steam::_leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure){
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboard_handle){
		return;
	}
	getDownloadedLeaderboardEntry(callData->m_hSteamLeaderboardEntries, callData->m_cEntryCount);
	owner->emit_signal("leaderboard_entries_loaded");
}
// Signal when overlay is triggered.
void Steam::_overlay_toggled(GameOverlayActivated_t* callData){
	if(callData->m_bActive){
		owner->emit_signal("overlay_toggled", true);
	}
	else{
		owner->emit_signal("overlay_toggled", false);
	}
}
// Signal when battery power is running low, less than 10 minutes left.
void Steam::_low_power(LowBatteryPower_t* timeLeft){
	uint8 power = timeLeft->m_nMinutesBatteryLeft;
	owner->emit_signal("low_power", power);
}
// When connected to a server.
void Steam::_server_connected(SteamServersConnected_t* connectData){
	owner->emit_signal("connection_changed", true);
}
// When disconnected from a server.
void Steam::_server_disconnected(SteamServersDisconnected_t* connectData){
	owner->emit_signal("connection_changed", false);
}
// Posted after the user gains ownership of DLC & that DLC is installed.
void Steam::_dlc_installed(DlcInstalled_t* callData){
	int appID = (AppId_t)callData->m_nAppID;
	owner->emit_signal("dlc_installed", appID);
}
// Response from getAuthSessionTicket.
void Steam::_get_auth_session_ticket_response(GetAuthSessionTicketResponse_t* callData){
	owner->emit_signal("get_auth_session_ticket_response", callData->m_hAuthTicket, callData->m_eResult);
}
// Called when an auth ticket has been validated.
void Steam::_validate_auth_ticket_response(ValidateAuthTicketResponse_t* callData){
	uint64_t authID = callData->m_SteamID.ConvertToUint64();
	uint32_t response = callData->m_eAuthSessionResponse;
	uint64_t ownerID = callData->m_OwnerSteamID.ConvertToUint64();
	owner->emit_signal("validate_auth_ticket_response", authID, response, ownerID);
}
// A screenshot has been requested by the user.
void Steam::_screenshot_ready(ScreenshotReady_t* callData){
	uint32_t handle = callData->m_hLocal;
	uint32_t result = callData->m_eResult;
	owner->emit_signal("screenshot_ready", handle, result);
}
// User stats are ready.
void Steam::_user_stats_received(UserStatsReceived_t* callData){
	uint64_t gameID = callData->m_nGameID;
	uint32_t result = callData->m_eResult;
	uint64_t userID = callData->m_steamIDUser.ConvertToUint64();
	owner->emit_signal("user_stats_received", gameID, result, userID);
}
// Result of an achievement icon that has been fetched.
void Steam::_user_achievement_icon_fetched(UserAchievementIconFetched_t* callData){
	uint64_t gameID = callData->m_nGameID.ToUint64();
	String achievementName = callData->m_rgchAchievementName;
	bool achieved = callData->m_bAchieved;
	int iconHandle = callData->m_nIconHandle;
	owner->emit_signal("user_achievement_icon_fetched", gameID, achievementName, achieved, iconHandle);
}
// Global achievements percentages are ready.
void Steam::_global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t* callData, bool bIOFailure){
	uint64_t gameID = callData->m_nGameID;
	uint32_t result = callData->m_eResult;
	owner->emit_signal("global_achievement_percentages_ready", gameID, result);
}
// Result of a workshop item being created.
void Steam::_workshop_item_created(CreateItemResult_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	owner->emit_signal("workshop_item_created", result, (uint64_t)fileID, acceptTOS);
}
// Result of a workshop item being updated.
void Steam::_workshop_item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	owner->emit_signal("workshop_item_updated", result, acceptTOS);
}
// Called when a workshop item has been installed or updated.
void Steam::_workshop_item_installed(ItemInstalled_t* callData){
	AppId_t appID = callData->m_unAppID;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	owner->emit_signal("workshop_item_installed", appID, (uint64_t)fileID);
}
/////////////////////////////////////////////////
///// USERS /////////////////////////////////////
//
// Get an authentication ticket
//uint32_t Steam::getAuthSessionTicket(){
//	if(SteamUser() == NULL){
//		return 0;
//	}
//	uint32_t cbTicket = 1024;
//	uint32_t *buf = memnew_arr(uint32_t, cbTicket);
//	uint32_t id = SteamUser()->GetAuthSessionTicket(buf, cbTicket, &cbTicket);
//	TicketData ticket = { id, buf, cbTicket };
//	tickets.push_back(ticket);
//	return id;
//}
// Cancels an auth ticket
//void Steam::cancelAuthTicket(uint32_t authTicket){
//	if(SteamUser() == NULL){
//		return;
//	}
//	for(int i=0; i<tickets.size(); i++){
//		TicketData ticket = tickets.get(i);
//		if (ticket.id == authTicket){
//			tickets.remove(i);
//			memdelete_arr(ticket.buf);
//			break;
//		}
//	}
//}
// Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused
//int Steam::beginAuthSession(uint32_t authTicket, uint64_t steamID){
//	if(SteamUser() == NULL){
//		return -1;
//	}
//	for(int i=0; i<tickets.size(); i++){
//		TicketData ticket = tickets.get(i);
//		if (ticket.id == authTicket){
//			CSteamID authSteamID = createSteamID(steamID);
//			return SteamUser()->BeginAuthSession(ticket.buf, ticket.size, authSteamID);
//		}
//	}
//	return -1;
//}
// Ends an auth session
//void Steam::endAuthSession(uint64_t steamID){
//	if(SteamUser() == NULL){
//		return;
//	}
//	CSteamID authSteamID = createSteamID(steamID);
//	return SteamUser()->EndAuthSession(authSteamID);
//}
// Get user's Steam ID
uint64_t Steam::getSteamID(){
	if(SteamUser() == NULL){
		return 0;
	}
	CSteamID cSteamID = SteamUser()->GetSteamID();
	return cSteamID.ConvertToUint64();
}
AccountID_t Steam::getSteamAccountID() {
	if (SteamUser() == NULL) {
		return 0;
	}
	CSteamID cSteamID = SteamUser()->GetSteamID();
	return cSteamID.GetAccountID();
}
// Check, true/false, if user is logged into Steam currently
bool Steam::loggedOn(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BLoggedOn();
}
// Get the user's Steam level
int Steam::getPlayerSteamLevel(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUser()->GetPlayerSteamLevel(); 
}
// Get the user's Steam installation path
String Steam::getUserDataFolder(){
	if(SteamUser() == NULL){
		return "";
	}
	const int cubBuffer = 256;
	char *pchBuffer = new char[cubBuffer];
	SteamUser()->GetUserDataFolder((char*)pchBuffer, cubBuffer);
	String data_path = pchBuffer;
	delete pchBuffer;
	return data_path;
}
// (LEGACY FUNCTION) Set data to be replicated to friends so that they can join your game
//void Steam::advertiseGame(String serverIP, int port){
//	if(SteamUser() == NULL){
//		return;
//	}
//	// Resolve address and convert it from IP_Address struct to uint32_t
//	IP_Address address;
//	if(serverIP.is_valid_ip_address()){
//		address = serverIP;
//	}
//	else{
//		address = IP::get_singleton()->resolve_hostname(serverIP, IP::TYPE_IPV4);
//	}
//	// Resolution failed - Godot 3.0 has is_invalid() to check this
//	if(address == IP_Address()){
//		return;
//	}
//	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
//	// Swap the bytes
//	uint8_t *ip4_p = (uint8_t *)&ip4;
//	for(int i=0; i<2; i++){
//		uint8_t temp = ip4_p[i];
//		ip4_p[i] = ip4_p[3-i];
//		ip4_p[3-i] = temp;
//	}
//	CSteamID gameserverID = SteamUser()->GetSteamID();
//	SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
//}
// Trading Card badges data access, if you only have one set of cards, the series will be 1
// The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1)
int Steam::getGameBadgeLevel(int series, bool foil){
	if(SteamUser()== NULL){
		return 0;
	}
	return SteamUser()->GetGameBadgeLevel(series, foil);
}
/////////////////////////////////////////////////
///// USER STATS ////////////////////////////////
//
// Clears a given achievement
bool Steam::clearAchievement(String name){
	return SteamUserStats()->ClearAchievement(name.utf8().get_data());
}
// Return true/false if use has given achievement
bool Steam::getAchievement(String name){
	bool achieved = false;
	SteamUserStats()->GetAchievement(name.utf8().get_data(), &achieved);
	return achieved;
}
// Returns the percentage of users who have unlocked the specified achievement
Dictionary Steam::getAchievementAchievedPercent(String name){
	Dictionary d;
	float percent = 0.f;
	bool achieved = false;
	if(SteamUserStats() == NULL){
		d["ret"] = false;
	} else {
		d["ret"] = SteamUserStats()->GetAchievementAchievedPercent(name.utf8().get_data(), &percent);
	}
	d["percent"] = percent;
	return d;
}

String Steam::getAchievementDisplayAttribute(String name, String key) {
	// TODO: implement
	return "";
}

int Steam::getAchievementIcon(String name) {
	// TODO: implement
	return 0;
}

String Steam::getAchievementName(uint32_t iAchievement) {
	// TODO: implement
	return "";
}

//  Get the amount of players currently playing the current game (online + offline)
void Steam::getNumberOfCurrentPlayers(){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->GetNumberOfCurrentPlayers();
//	callResultNumberOfCurrentPlayers.Set(apiCall, this, &Steam::_number_of_current_players);
}
// Get the number of achievements
uint32_t Steam::getNumAchievements(){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->GetNumAchievements();
}
// Get the value of a float statistic
float Steam::getStatFloat(String name){
	float statval = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statval);
	return statval;
}
// Get the value of an integer statistic
int Steam::getStatInt(String name){
	int32_t statval = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statval);
	return statval;
}
// Reset all Steam statistics; optional to reset achievements
bool Steam::resetAllStats(bool achievementsToo){
	SteamUserStats()->ResetAllStats(achievementsToo);
	return SteamUserStats()->StoreStats();
}
// Request all statistics and achievements from Steam servers
bool Steam::requestCurrentStats(){
	return SteamUserStats()->RequestCurrentStats();
}
// Asynchronously fetch the data for the percentages
void Steam::requestGlobalAchievementPercentages(){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->RequestGlobalAchievementPercentages();
//	callResultGlobalAchievementPercentagesReady.Set(apiCall, this, &Steam::_global_achievement_percentages_ready);
}
// Set a given achievement
bool Steam::setAchievement(String name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	SteamUserStats()->SetAchievement(name.utf8().get_data());
	return SteamUserStats()->StoreStats();
}
// Set a float statistic
bool Steam::setStatFloat(String name, float value){
	return SteamUserStats()->SetStat(name.utf8().get_data(), value);
}
// Set an integer statistic
bool Steam::setStatInt(String name, int value){
	return SteamUserStats()->SetStat(name.utf8().get_data(), value);
}
// Store all statistics, and achievements, on Steam servers; must be called to "pop" achievements
bool Steam::storeStats(){
	if(SteamUserStats() == NULL){
		return 0;
	}
	SteamUserStats()->StoreStats();
	return SteamUserStats()->RequestCurrentStats();
}
// Find a given leaderboard, by name
void Steam::findLeaderboard(String name){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->FindLeaderboard(name.utf8().get_data());
	callResultFindLeaderboard.Set(apiCall, this, &Steam::_leaderboard_loaded);
}
// Get the name of a leaderboard
String Steam::getLeaderboardName(){
	if(SteamUserStats() == NULL){
		return "";
	}
	return SteamUserStats()->GetLeaderboardName(leaderboard_handle);
}
// Get the total number of entries in a leaderboard, as of the last request
int Steam::getLeaderboardEntryCount(){
	if(SteamUserStats() == NULL){
		return -1;
	}
	return SteamUserStats()->GetLeaderboardEntryCount(leaderboard_handle);
}
// Request all rows for friends of user
void Steam::downloadLeaderboardEntries(int start, int end, int type){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntries(leaderboard_handle, ELeaderboardDataRequest(type), start, end);
	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
}
// Request a maximum of 100 users with only one outstanding call at a time
void Steam::downloadLeaderboardEntriesForUsers(Array usersID){
	if(SteamUserStats() == NULL){
		return;
	}
	int users_count = usersID.size();
	if(users_count == 0){
		return;
	}
	CSteamID *users = new CSteamID[users_count];
	for(int i = 0; i < users_count; i++){
		CSteamID user = createSteamID(usersID[i]);
		users[i] = user;
	}
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboard_handle, users, users_count);
	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
	delete[] users;
}
// Upload a leaderboard score for the user
void Steam::uploadLeaderboardScore(int score, bool keepBest){
	if(SteamUserStats() == NULL){
		return;
	}
	ELeaderboardUploadScoreMethod method = keepBest ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
	SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(leaderboard_handle, method, (int32)score, NULL, 0);
	callResultUploadScore.Set(apiCall, this, &Steam::_leaderboard_uploaded);
}
// Once all entries are accessed, the data will be freed up and the handle will become invalid, use this to store it
void Steam::getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount){
	if(SteamUserStats() == NULL){
		return;
	}
	leaderboard_entries.clear();
	for(int i = 0; i < entryCount; i++){
		LeaderboardEntry_t entry;
		SteamUserStats()->GetDownloadedLeaderboardEntry(eHandle, i, &entry, NULL, 0);
		Dictionary entryDict;
		uint64_t steamID = (uint64_t) entry.m_steamIDUser.ConvertToUint64();
		entryDict["score"] = entry.m_nScore;
		entryDict["steamID"] = steamID;
		entryDict["global_rank"] = entry.m_nGlobalRank;
		leaderboard_entries.append(entryDict);
	}
}
// Get the currently used leaderboard handle
uint64_t Steam::getLeaderboardHandle(){
	return leaderboard_handle;
}
// Get the currently used leaderboard entries
Array Steam::getLeaderboardEntries(){
	return leaderboard_entries;
}
// Get the currently used lobbies list
Array Steam::getLobbiesList() {
	return listLobbies;
}
// Get the achievement status, and the time it was unlocked if unlocked (in seconds since January 1, 19)
bool Steam::getAchievementAndUnlockTime(String name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	bool achieved;
	uint32_t unlockTime;
	return SteamUserStats()->GetAchievementAndUnlockTime(name.utf8().get_data(), &achieved, &unlockTime);
}
// Achievement progress, triggers an AchievementProgress callback, that is all.
// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
bool Steam::indicateAchievementProgress(String name, int curProgress, int maxProgress){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->IndicateAchievementProgress(name.utf8().get_data(), curProgress, maxProgress);
}
/////////////////////////////////////////////////
///// UTILS /////////////////////////////////////
//
// Get the user's country by IP
String Steam::getIPCountry(){
	return SteamUtils()->GetIPCountry();
}
// Returns true/false if Steam overlay is enabled
bool Steam::isOverlayEnabled(){
	return SteamUtils()->IsOverlayEnabled();
}
// Set the position where overlay shows notifications
void Steam::setOverlayNotificationPosition(int pos){
	if((pos < 0) || (pos > 3) || (SteamUtils() == NULL)){
		return;
	}
	SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
}
// Get the Steam user interface language
String Steam::getSteamUILanguage(){
	return SteamUtils()->GetSteamUILanguage();
}
// Get the Steam ID of the running application/game
int Steam::getAppID(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetAppID();
}

Dictionary Steam::getImageRGBA(int iImage) {
	// TODO: implement
	return Dictionary();
}

Dictionary Steam::getImageSize(int iImage) {
	// TODO: implement
	return Dictionary();
}

// Return amount of time, in seconds, user has spent in this session
int Steam::getSecondsSinceAppActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceAppActive();
}
// Get the amount of battery power, clearly for laptops
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
// Get the actual time
int Steam::getServerRealTime(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetServerRealTime();
}
// Returns true if Steam & the Steam Overlay are running in Big Picture mode
bool Steam::isSteamInBigPictureMode(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamInBigPictureMode();
}
// Ask SteamUI to create and render its OpenVR dashboard
void Steam::startVRDashboard(){
	if(SteamUtils() == NULL){
		return ;
	}
	SteamUtils()->StartVRDashboard();
}
/////////////////////////////////////////////////
///// WORKSHOP //////////////////////////////////
//
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
//int Steam::getItemUpdateProgress(uint64_t updateHandle, uint64_t *bytesProcessed, uint64_t* bytesTotal){
//	UGCUpdateHandle_t handle = (uint64_t)updateHandle;
//	return SteamUGC()->GetItemUpdateProgress(handle, (uint64*)&bytesProcessed, bytesTotal);
//}
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
bool Steam::setItemTitle(uint64_t updateHandle, String title){
	if(SteamUGC() == NULL){
		return false;
	}
	if (title.length() > UGC_MAX_TITLE_CHARS){
		printf("Title cannot have more than %d ASCII characters. Title not set.", UGC_MAX_TITLE_CHARS);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemTitle(handle, title.utf8().get_data());
}
// Sets a new description for an item.
bool Steam::setItemDescription(uint64_t updateHandle, String description){
	if(SteamUGC() == NULL){
		return false;
	}
	if (description.length() > UGC_MAX_DESC_CHARS){
		printf("Description cannot have more than %d ASCII characters. Description not set.", UGC_MAX_DESC_CHARS);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemDescription(handle, description.utf8().get_data());
}
// Sets the language of the title and description that will be set in this item update.
bool Steam::setItemUpdateLanguage(uint64_t updateHandle, String language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}
// Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
bool Steam::setItemMetadata(uint64_t updateHandle, String metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	if (metadata.length() > UGC_MAX_METADATA_CHARS){
		printf("Metadata cannot have more than %d ASCII characters. Metadata not set.", UGC_MAX_METADATA_CHARS);
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
//bool Steam::setItemTags(uint64_t updateHandle, const char ** stringArray, const int32 stringCount){
//	if(SteamUGC() == NULL){
//		return false;
//	}
//	UGCUpdateHandle_t handle = uint64(updateHandle);
//	SteamParamStringArray_t tags = { stringArray, stringCount };
//	return SteamUGC()->SetItemTags(updateHandle, &tags);
//}
// Sets the folder that will be stored as the content for an item.
bool Steam::setItemContent(uint64_t updateHandle, String contentFolder){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemContent(handle, contentFolder.utf8().get_data());
}
// Sets the primary preview image for the item.
bool Steam::setItemPreview(uint64_t updateHandle, String previewFile){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemPreview(handle, previewFile.utf8().get_data());
}
// Uploads the changes made to an item to the Steam Workshop; to be called after setting your changes.
void Steam::submitItemUpdate(uint64_t updateHandle, String changeNote){
	if(SteamUGC() == NULL){
		return;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	SteamAPICall_t apiCall = SteamUGC()->SubmitItemUpdate(handle, changeNote.utf8().get_data());
	callResultItemUpdate.Set(apiCall, this, &Steam::_workshop_item_updated);
}
// Gets a list of all of the items the current user is subscribed to for the current game.
//Array Steam::getSubscribedItems(){
//	if(SteamUGC() == NULL){
//		return Array();
//	}
//	int numItems = SteamUGC()->GetNumSubscribedItems();
//	// Need array for list of PublishedFileId_t
//	return SteamUGC()->GetSubscribedItems(items, numItems);
//}
// Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled set.
//bool Steam::getItemInstallInfo(int publishedFileID, uint64_t *sizeOnDisk, OUT_STRING_COUNT(folderSize) String *folder, uint32_t folderSize, uint32_t *timeStamp){
//	if(SteamUGC() == NULL){
//		return false;
//	}
//	PublishedFileId_t fileID = (int)publishedFileID;
//	return SteamUGC()->GetItemInstallInfo(fileID, sizeOnDisk, folder, folderSize, timeStamp);
//}
// Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
//bool Steam::getItemDownloadInfo(int publishedFileID, uint64_t *bytesDownloaded, uint64_t *bytesTotal){
//	if(SteamUGC() == NULL){
//		return false;
//	}
//	PublishedFileId_t fileID = int(publishedFileID);
//	return SteamUGC()->GetItemDownloadInfo(fileID, bytesDownloaded, bytesTotal);
//}

Steam::~Steam(){
	if(isInitSuccess){
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
//	for(int i=0; i<tickets.size(); i++){
//		TicketData ticket = tickets.get(i);
//		memdelete_arr(ticket.buf);
//	}
//	tickets.clear();
	singleton = NULL;
}