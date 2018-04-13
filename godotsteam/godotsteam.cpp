#include "godotsteam.h"
#include <steam/steam_api.h>

#include "core/io/ip_address.h"
#include "core/io/ip.h"

Steam* Steam::singleton = NULL;

Steam::Steam(){
	isInitSuccess = false;
	singleton = this;
	tickets.clear();
}

Steam* Steam::get_singleton(){
	return singleton;
}

CSteamID Steam::createSteamID(uint32 steamID, int accountType){
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
		AppId_t appID;
		bool available;
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
String Steam::getLaunchQueryParam(const String& key){
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
// Lookup the handle for an action Set.
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
// Get the handle of the specified analog action.
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
// Get number of friends user has.
int Steam::getFriendCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendCount(0x04);
}
// Get the user's Steam username.
String Steam::getPersonaName(){
	if(SteamFriends() == NULL){
		return "";
	}
	return SteamFriends()->GetPersonaName();
}
// Get given friend's Steam username.
String Steam::getFriendPersonaName(int steamID){
	if(SteamFriends() != NULL && steamID > 0){
		CSteamID friendID = createSteamID(steamID);
		bool isDataLoading = SteamFriends()->RequestUserInformation(friendID, true);
		if(!isDataLoading){
			return SteamFriends()->GetFriendPersonaName(friendID);
		}
	}
	return "";
}
// Set the game information in Steam; used in 'View Game Info'.
void Steam::setGameInfo(const String& key, const String& value){
	// Rich presence data is automatically shared between friends in the same game.
	// Each user has a set of key/value pairs, up to 20 can be set.
	// Two magic keys (status, connect).
	// setGameInfo() to an empty string deletes the key.
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->SetRichPresence(key.utf8().get_data(), value.utf8().get_data());
}
// Clear the game information in Steam; used in 'View Game Info'.
void Steam::clearGameInfo(){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ClearRichPresence();
}
// Invite friend to current game/lobby.
void Steam::inviteFriend(int steamID, const String& connectString){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->InviteUserToGame(friendID, connectString.utf8().get_data());
}
// Set player as 'Played With' for game.
void Steam::setPlayedWith(int steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->SetPlayedWith(friendID);
}
// Get list of players user has recently played game with.
Array Steam::getRecentPlayers(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int count = SteamFriends()->GetCoplayFriendCount();
	Array recents;
	for(int i=0; i<count; i++){
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
// Get player's avatar.
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
			// Still loading.
			return;
		}
		// Invalid.
		return;
	}
	// Has already loaded, simulate callback.
	AvatarImageLoaded_t* avatarData = new AvatarImageLoaded_t;
	avatarData->m_steamID = SteamUser()->GetSteamID();
	avatarData->m_iImage = handle;
	avatarData->m_iWide = size;
	avatarData->m_iTall = size;
	_avatar_loaded(avatarData);
	delete avatarData;
	return;
}
// Draw the given avatar.
Image Steam::drawAvatar(int size, uint8* buffer){
	// Apply buffer to Image.
	Image avatar(size, size, false, Image::FORMAT_RGBA);
	for(int y = 0; y < size; y++){
		for(int x=0; x<size; x++){
			int i = 4*(x+y*size);
			float r = float(buffer[i])/255;
			float g = float(buffer[i+1])/255;
			float b = float(buffer[i+2])/255;
			float a = float(buffer[i+3])/255;
			avatar.put_pixel(x, y, Color(r, g, b, a));
		}
	}
	return avatar;
}
// Activates the overlay with optional dialog to open the following: "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite".
void Steam::activateGameOverlay(const String& url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
}
// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore".
void Steam::activateGameOverlayToUser(const String& url, int steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID overlayUserID = createSteamID(steamID);
	SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), overlayUserID);
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
// Get list of user's Steam groups; a mix of different Steamworks API group functions.
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
// Get a list of user's Steam friends; a mix of different Steamworks API friend functions.
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
// Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
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
	SteamMatchmaking()->CreateLobby(eLobbyType, maxMembers);
}
// Join an existing lobby.
void Steam::joinLobby(int steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	SteamMatchmaking()->JoinLobby(lobbyID);
}
// Leave a lobby, this will take effect immediately on the client side, other users will be notified by LobbyChatUpdate_t callback.
void Steam::leaveLobby(int steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	return SteamMatchmaking()->LeaveLobby(lobbyID);
}
// Invite another user to the lobby, the target user will receive a LobbyInvite_t callback, will return true if the invite is successfully sent, whether or not the target responds.
bool Steam::inviteUserToLobby(int steamIDLobby, int steamIDInvitee){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	CSteamID inviteeID = createSteamID(steamIDInvitee);
	return SteamMatchmaking()->InviteUserToLobby(lobbyID, inviteeID);
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
///// REMOTE STORAGE ////////////////////////////
//
// Write to given file from Steam Cloud.
bool Steam::fileWrite(const String& file, const DVector<uint8_t>& data, int32_t dataSize){
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
	DVector<uint8_t> data;
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
uint32_t Steam::writeScreenshot(const DVector<uint8_t>& RGB, int width, int height){
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
	emit_signal("file_details_result", result, fileSize, fileHash, flags);
}
// Signal the lobby has been created.
void Steam::_lobby_created(LobbyCreated_t* lobbyData){
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
	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
	emit_signal("lobby_created", connect, lobbyID);
}
// Signal that lobby has been joined.
void Steam::_lobby_joined(LobbyEnter_t* lobbyData){
	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
	uint32 permissions = lobbyData->m_rgfChatPermissions;
	bool locked = lobbyData->m_bLocked;
	uint32 response = lobbyData->m_EChatRoomEnterResponse;
	emit_signal("lobby_joined", lobbyID, permissions, locked, response);
}
// Signal that a lobby invite was sent.
void Steam::_lobby_invite(LobbyInvite_t* lobbyData){
	int inviterID = (uint64)lobbyData->m_ulSteamIDUser;
	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
	int gameID = (uint64)lobbyData->m_ulGameID;
	emit_signal("lobby_invite", inviterID, lobbyID, gameID);
}
// Signal a game/lobby join has been requested.
void Steam::_join_requested(GameRichPresenceJoinRequested_t* callData){
	int steamID = callData->m_steamIDFriend.GetAccountID();
	String con_string = callData->m_rgchConnect;
	emit_signal("join_requested", steamID, con_string);
}
// Signal that the avatar has been loaded.
void Steam::_avatar_loaded(AvatarImageLoaded_t* avatarData){
	if(avatarData->m_steamID != SteamUser()->GetSteamID()){
		return;
	}
	int size = avatarData->m_iWide;
	// Get image buffer.
	int buffSize = 4*size*size;
	uint8* buffer = new uint8[buffSize];
	bool success = SteamUtils()->GetImageRGBA(avatarData->m_iImage,buffer,buffSize);
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
	call_deferred("emit_signal", "avatar_loaded", avatarSize, avatar);
}
// Signal number of current players (online + offline).
void Steam::_number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure){
	emit_signal("number_of_current_players", callData->m_bSuccess && bIOFailure, callData->m_cPlayers);
}
// Signal a leaderboard has been loaded or has failed.
void Steam::_leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure){
	leaderboard_handle = callData->m_hSteamLeaderboard;
	uint8_t found = callData->m_bLeaderboardFound;
	emit_signal("leaderboard_loaded", (uint64_t)leaderboard_handle, found);
}
// Signal a leaderboard entry has been uploaded.
void Steam::_leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure){
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboard_handle){
		return;
	}
	emit_signal("leaderboard_uploaded", callData->m_bSuccess && bIOFailure, callData->m_nScore, callData->m_bScoreChanged, callData->m_nGlobalRankNew, callData->m_nGlobalRankPrevious);
}
// Signal leaderboard entries are downloaded.
void Steam::_leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure){
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboard_handle){
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
	uint64_t gameID = callData->m_nGameID;
	uint32_t result = callData->m_eResult;
	uint64_t userID = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("user_stats_received", gameID, result, userID);
}
// Result of an achievement icon that has been fetched.
void Steam::_user_achievement_icon_fetched(UserAchievementIconFetched_t* callData){
	uint64_t gameID = callData->m_nGameID.ToUint64();
	String achievementName = callData->m_rgchAchievementName;
	bool achieved = callData->m_bAchieved;
	int iconHandle = callData->m_nIconHandle;
	emit_signal("user_achievement_icon_fetched", gameID, achievementName, achieved, iconHandle);
}
// Global achievements percentages are ready
void Steam::_global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t* callData, bool bIOFailure){
	uint64_t gameID = callData->m_nGameID;
	uint32_t result = callData->m_eResult;
	emit_signal("global_achievement_percentages_ready", gameID, result);
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
	for(int i=0; i<tickets.size(); i++){
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
	for(int i=0; i<tickets.size(); i++){
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
	return SteamUser()->EndAuthSession(authSteamID);
}
// Get user's Steam ID.
uint64_t Steam::getSteamID(){
	if(SteamUser() == NULL){
		return 0;
	}
	CSteamID cSteamID = SteamUser()->GetSteamID();
	return cSteamID.ConvertToUint64();
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
// Get the user's Steam installation path.
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
	// Resolve address and convert it from IP_Address struct to uint32_t.
	IP_Address address;
	if(serverIP.is_valid_ip_address()){
		address = serverIP;
	}
	else{
		address = IP::get_singleton()->resolve_hostname(serverIP, IP::TYPE_IPV4);
	}
	// Resolution failed - Godot 3.0 has is_invalid() to check this.
	if(address == IP_Address()){
		return;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	// Swap the bytes.
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
bool Steam::getAchievement(const String& name){
	if(SteamUserStats() == NULL){
		return false;
	}
	return SteamUserStats()->GetAchievement(name.utf8().get_data(), false);
}
// Returns the percentage of users who have unlocked the specified achievement.
Dictionary Steam::getAchievementAchievedPercent(const String& name){
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
	int32 statValue = 0;
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
	return SteamUserStats()->GetLeaderboardName(leaderboard_handle);
}
// Get the total number of entries in a leaderboard, as of the last request.
int Steam::getLeaderboardEntryCount(){
	if(SteamUserStats() == NULL){
		return -1;
	}
	return SteamUserStats()->GetLeaderboardEntryCount(leaderboard_handle);
}
// Request all rows for friends of user.
void Steam::downloadLeaderboardEntries(int start, int end, int type){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntries(leaderboard_handle, ELeaderboardDataRequest(type), start, end);
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
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboard_handle, users, usersCount);
	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
	delete[] users;
}
// Upload a leaderboard score for the user.
void Steam::uploadLeaderboardScore(int score, bool keepBest){
	if(SteamUserStats() == NULL){
		return;
	}
	ELeaderboardUploadScoreMethod method = keepBest ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
	SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(leaderboard_handle, method, (int32)score, NULL, 0);
	callResultUploadScore.Set(apiCall, this, &Steam::_leaderboard_uploaded);
}
// Once all entries are accessed, the data will be freed up and the handle will become invalid, use this to store it.
void Steam::getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount){
	if(SteamUserStats() == NULL){
		return;
	}
	leaderboard_entries.clear();
	LeaderboardEntry_t *entry = memnew(LeaderboardEntry_t);
	for(int i = 0; i < entryCount; i++){
		SteamUserStats()->GetDownloadedLeaderboardEntry(eHandle, i, entry, NULL, 0);
		Dictionary entryDict;
		entryDict["score"] = entry->m_nScore;
		entryDict["steamID"] = entry->m_steamIDUser.GetAccountID();
		entryDict["global_rank"] = entry->m_nGlobalRank;
		leaderboard_entries.append(entryDict);
	}
	memdelete(entry);
}
// Get the currently used leaderboard handle.
uint64_t Steam::getLeaderboardHandle(){
	return leaderboard_handle;
}
// Get the currently used leaderboard entries.
Array Steam::getLeaderboardEntries(){
	return leaderboard_entries;
}
// Get the achievement status, and the time it was unlocked if unlocked (in seconds since January 1, 19).
bool Steam::getAchievementAndUnlockTime(const String& name, bool achieved, uint32_t unlockTime){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->GetAchievementAndUnlockTime(name.utf8().get_data(), (bool *)achieved, (uint32_t *)unlockTime);
}
// Achievement progress, triggers an AchievementProgress callback, that is all.
// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
bool Steam::indicateAchievementProgress(const String& name, int curProgress, int maxProgress){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->IndicateAchievementProgress(name.utf8().get_data(), curProgress, maxProgress);
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
	bool ret = false;
	if(SteamUtils() != NULL){
		uint32 width;
		uint32 height;
		ret = SteamUtils()->GetImageSize(image, &width, &height);
		if(ret){
			DVector<uint8_t> data;
			data.resize(width * height * 4);
			ret = SteamUtils()->GetImageRGBA(image, data.write().ptr(), data.size());
			if (ret){
				d["buf"] = data;
			}
		}
	}
	d["ret"] = ret;
	return d;
}
// Gets the size of a Steam image handle.
Dictionary Steam::getImageSize(int image){
	Dictionary d;
	bool ret = false;
	if(SteamUtils() != NULL){
		uint32 width;
		uint32 height;
		ret = SteamUtils()->GetImageSize(image, &width, &height);
		d["width"] = width;
		d["height"] = height;
	}
	d["ret"] = ret;
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
// If item is not subscribed to, it will be cached for some time. If highPriority is set, any other item download will be suspended and this item downloaded ASAP.
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
bool Steam::setItemTitle(uint64_t updateHandle, const String& title){
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
bool Steam::setItemDescription(uint64_t updateHandle, const String& description){
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
/////////////////////////////////////////////////
///// BIND METHODS //////////////////////////////
//
void Steam::_bind_methods(){
	ObjectTypeDB::bind_method("restartAppIfNecessary", &Steam::restartAppIfNecessary);
	ObjectTypeDB::bind_method("steamInit", &Steam::steamInit);
	ObjectTypeDB::bind_method("isSteamRunning", &Steam::isSteamRunning);
	ObjectTypeDB::bind_method("run_callbacks", &Steam::run_callbacks);
	// Apps Bind Methods ////////////////////////
	ObjectTypeDB::bind_method("isSubscribed", &Steam::isSubscribed);
	ObjectTypeDB::bind_method("isLowViolence", &Steam::isLowViolence);
	ObjectTypeDB::bind_method("isCybercafe", &Steam::isCybercafe);
	ObjectTypeDB::bind_method("isVACBanned", &Steam::isVACBanned);
	ObjectTypeDB::bind_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
	ObjectTypeDB::bind_method("getAvailableGameLanguages", &Steam::getAvailableGameLanguages);
	ObjectTypeDB::bind_method("isSubscribedApp", &Steam::isSubscribedApp);
	ObjectTypeDB::bind_method("isDLCInstalled", &Steam::isDLCInstalled);
	ObjectTypeDB::bind_method("getEarliestPurchaseUnixTime", &Steam::getEarliestPurchaseUnixTime);
	ObjectTypeDB::bind_method("isSubscribedFromFreeWeekend", &Steam::isSubscribedFromFreeWeekend);
	ObjectTypeDB::bind_method("getDLCCount", &Steam::getDLCCount);
	ObjectTypeDB::bind_method("getDLCDataByIndex", &Steam::getDLCDataByIndex);
	ObjectTypeDB::bind_method("installDLC", &Steam::installDLC);
	ObjectTypeDB::bind_method("uninstallDLC", &Steam::uninstallDLC);
	ObjectTypeDB::bind_method("getCurrentBetaName", &Steam::getCurrentBetaName);
	ObjectTypeDB::bind_method("markContentCorrupt", &Steam::markContentCorrupt);
//	ObjectTypeDB::bind_method("getInstalledDepots", &Steam::getInstalledDepots);
	ObjectTypeDB::bind_method("getAppInstallDir", &Steam::getAppInstallDir);
	ObjectTypeDB::bind_method("isAppInstalled", &Steam::isAppInstalled);
	ObjectTypeDB::bind_method("getAppOwner", &Steam::getAppOwner);
	ObjectTypeDB::bind_method("getLaunchQueryParam", &Steam::getLaunchQueryParam);
//	ObjectTypeDB::bind_method("getDLCDownloadProgress", &Steam::getDLCDownloadProgress);
	ObjectTypeDB::bind_method("getAppBuildId", &Steam::getAppBuildId);
	ObjectTypeDB::bind_method("getFileDetails", &Steam::getFileDetails);
	// Controllers Bind Methods /////////////////
	ObjectTypeDB::bind_method("activateActionSet", &Steam::activateActionSet);
	ObjectTypeDB::bind_method("getActionSetHandle", &Steam::getActionSetHandle);
	ObjectTypeDB::bind_method("getAnalogActionData", &Steam::getAnalogActionData);
	ObjectTypeDB::bind_method("getAnalogActionHandle", &Steam::getAnalogActionHandle);
	ObjectTypeDB::bind_method("getAnalogActionOrigins", &Steam::getAnalogActionOrigins);
	ObjectTypeDB::bind_method("getConnectedControllers", &Steam::getConnectedControllers);
	ObjectTypeDB::bind_method("getControllerForGamepadIndex", &Steam::getControllerForGamepadIndex);
	ObjectTypeDB::bind_method("getCurrentActionSet", &Steam::getCurrentActionSet);
	ObjectTypeDB::bind_method("getDigitalActionData", &Steam::getDigitalActionData);
	ObjectTypeDB::bind_method("getDigitalActionHandle", &Steam::getDigitalActionHandle);
	ObjectTypeDB::bind_method("getDigitalActionOrigins", &Steam::getDigitalActionOrigins);
	ObjectTypeDB::bind_method("getMotionData", &Steam::getMotionData);
	ObjectTypeDB::bind_method("init", &Steam::init);
	ObjectTypeDB::bind_method("runFrame", &Steam::runFrame);
	ObjectTypeDB::bind_method("showBindingPanel", &Steam::showBindingPanel);
	ObjectTypeDB::bind_method("shutdown", &Steam::shutdown);
	ObjectTypeDB::bind_method("triggerVibration", &Steam::triggerVibration);
	// Friends Bind Methods /////////////////////
	ObjectTypeDB::bind_method("getFriendCount", &Steam::getFriendCount);
	ObjectTypeDB::bind_method("getPersonaName", &Steam::getPersonaName);
	ObjectTypeDB::bind_method("getFriendPersonaName", &Steam::getFriendPersonaName);
	ObjectTypeDB::bind_method(_MD("setGameInfo", "key", "value"), &Steam::setGameInfo);
	ObjectTypeDB::bind_method(_MD("clearGameInfo"), &Steam::clearGameInfo);
	ObjectTypeDB::bind_method(_MD("inviteFriend", "steamID", "connectString"), &Steam::inviteFriend);
	ObjectTypeDB::bind_method(_MD("setPlayedWith", "steamID"), &Steam::setPlayedWith);
	ObjectTypeDB::bind_method("getRecentPlayers", &Steam::getRecentPlayers);
	ObjectTypeDB::bind_method(_MD("getFriendAvatar", "size"), &Steam::getFriendAvatar, DEFVAL(AVATAR_MEDIUM));
	ObjectTypeDB::bind_method("getUserSteamGroups", &Steam::getUserSteamGroups);
	ObjectTypeDB::bind_method("getUserSteamFriends", &Steam::getUserSteamFriends);
	ObjectTypeDB::bind_method(_MD("activateGameOverlay", "type"), &Steam::activateGameOverlay, DEFVAL(""));
	ObjectTypeDB::bind_method(_MD("activateGameOverlayToUser", "type", "steamID"), &Steam::activateGameOverlayToUser, DEFVAL(""), DEFVAL(0));
	ObjectTypeDB::bind_method(_MD("activateGameOverlayToWebPage", "url"), &Steam::activateGameOverlayToWebPage);
	ObjectTypeDB::bind_method(_MD("activateGameOverlayToStore", "appID"), &Steam::activateGameOverlayToStore, DEFVAL(0));
	ObjectTypeDB::bind_method(_MD("activateGameOverlayInviteDialog", "steamID"), &Steam::activateGameOverlayInviteDialog);
	// Matchmaking Bind Methods /////////////////
	ObjectTypeDB::bind_method(_MD("createLobby", "type"), &Steam::createLobby, DEFVAL(2));
	ObjectTypeDB::bind_method("joinLobby", &Steam::joinLobby);
	ObjectTypeDB::bind_method("leaveLobby", &Steam::leaveLobby);
	ObjectTypeDB::bind_method("inviteUserToLobby", &Steam::inviteUserToLobby);
	// Music Bind Methods ///////////////////////
	ObjectTypeDB::bind_method("musicIsEnabled", &Steam::musicIsEnabled);
	ObjectTypeDB::bind_method("musicIsPlaying", &Steam::musicIsPlaying);
	ObjectTypeDB::bind_method("musicGetVolume", &Steam::musicGetVolume);
	ObjectTypeDB::bind_method("musicPause", &Steam::musicPause);
	ObjectTypeDB::bind_method("musicPlay", &Steam::musicPlay);
	ObjectTypeDB::bind_method("musicPlayNext", &Steam::musicPlayNext);
	ObjectTypeDB::bind_method("musicPlayPrev", &Steam::musicPlayPrev);
	ObjectTypeDB::bind_method("musicSetVolume", &Steam::musicSetVolume);
	// Remote Storage Bind Methods //////////////
	ObjectTypeDB::bind_method("fileWrite", &Steam::fileWrite);
	ObjectTypeDB::bind_method("fileRead", &Steam::fileRead);
	ObjectTypeDB::bind_method("fileForget", &Steam::fileForget);
	ObjectTypeDB::bind_method("fileDelete", &Steam::fileDelete);
	ObjectTypeDB::bind_method("fileExists", &Steam::fileExists);
	ObjectTypeDB::bind_method("filePersisted", &Steam::filePersisted);
	ObjectTypeDB::bind_method("getFileSize", &Steam::getFileSize);
	ObjectTypeDB::bind_method("getFileTimestamp", &Steam::getFileTimestamp);
	ObjectTypeDB::bind_method("getFileCount", &Steam::getFileCount);
	ObjectTypeDB::bind_method("isCloudEnabledForAccount", &Steam::isCloudEnabledForAccount);
	ObjectTypeDB::bind_method("isCloudEnabledForApp", &Steam::isCloudEnabledForApp);
	ObjectTypeDB::bind_method("setCloudEnabledForApp", &Steam::setCloudEnabledForApp);
	ObjectTypeDB::bind_method("getFileNameAndSize", &Steam::getFileNameAndSize);
	ObjectTypeDB::bind_method("getQuota", &Steam::getQuota);
	ObjectTypeDB::bind_method("getSyncPlatforms", &Steam::getSyncPlatforms);
	// Screenshot Bind Methods /////////////////
	ObjectTypeDB::bind_method("addScreenshotToLibrary", &Steam::addScreenshotToLibrary);
	ObjectTypeDB::bind_method("hookScreenshots", &Steam::hookScreenshots);
	ObjectTypeDB::bind_method("isScreenshotsHooked", &Steam::isScreenshotsHooked);
	ObjectTypeDB::bind_method("setLocation", &Steam::setLocation);
	ObjectTypeDB::bind_method("triggerScreenshot", &Steam::triggerScreenshot);
	ObjectTypeDB::bind_method("writeScreenshot", &Steam::writeScreenshot);
	// User Bind Methods ////////////////////////
	ObjectTypeDB::bind_method("getAuthSessionTicket", &Steam::getAuthSessionTicket);
	ObjectTypeDB::bind_method("cancelAuthTicket", &Steam::cancelAuthTicket);
	ObjectTypeDB::bind_method("beginAuthSession", &Steam::beginAuthSession);
	ObjectTypeDB::bind_method("endAuthSession", &Steam::endAuthSession);
	ObjectTypeDB::bind_method("getSteamID", &Steam::getSteamID);
	ObjectTypeDB::bind_method("loggedOn", &Steam::loggedOn);
	ObjectTypeDB::bind_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
	ObjectTypeDB::bind_method("getUserDataFolder", &Steam::getUserDataFolder);
	ObjectTypeDB::bind_method(_MD("advertiseGame", "serverIP", "port"), &Steam::advertiseGame);
	ObjectTypeDB::bind_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
	// User Stats Bind Methods //////////////////
	ObjectTypeDB::bind_method("clearAchievement", &Steam::clearAchievement);
	ObjectTypeDB::bind_method("getAchievement", &Steam::getAchievement);
	ObjectTypeDB::bind_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
	ObjectTypeDB::bind_method(_MD("getAchievementDisplayAttribute", "name", "key"), &Steam::getAchievementDisplayAttribute);
	ObjectTypeDB::bind_method(_MD("getAchievementIcon", "name"), &Steam::getAchievementIcon);
	ObjectTypeDB::bind_method(_MD("getAchievementName", "iAchievement"), &Steam::getAchievementName);
	ObjectTypeDB::bind_method("getNumAchievements", &Steam::getNumAchievements);
	ObjectTypeDB::bind_method("getNumberOfCurrentPlayers", &Steam::getNumberOfCurrentPlayers);
	ObjectTypeDB::bind_method("getStatFloat", &Steam::getStatFloat);
	ObjectTypeDB::bind_method("getStatInt", &Steam::getStatInt);
	ObjectTypeDB::bind_method("resetAllStats", &Steam::resetAllStats);
	ObjectTypeDB::bind_method("requestCurrentStats", &Steam::requestCurrentStats);
	ObjectTypeDB::bind_method("requestGlobalAchievementPercentages", &Steam::requestGlobalAchievementPercentages);
	ObjectTypeDB::bind_method("setAchievement", &Steam::setAchievement);
	ObjectTypeDB::bind_method("setStatFloat", &Steam::setStatFloat);
	ObjectTypeDB::bind_method("setStatInt", &Steam::setStatInt);
	ObjectTypeDB::bind_method("storeStats", &Steam::storeStats);
	ObjectTypeDB::bind_method(_MD("findLeaderboard", "name"), &Steam::findLeaderboard);
	ObjectTypeDB::bind_method("getLeaderboardName", &Steam::getLeaderboardName);
	ObjectTypeDB::bind_method("getLeaderboardEntryCount", &Steam::getLeaderboardEntryCount);
	ObjectTypeDB::bind_method(_MD("downloadLeaderboardEntries", "range_start", "range_end", "type"), &Steam::downloadLeaderboardEntries, DEFVAL(GLOBAL));
	ObjectTypeDB::bind_method(_MD("downloadLeaderboardEntriesForUsers", "usersID"), &Steam::downloadLeaderboardEntriesForUsers);
	ObjectTypeDB::bind_method(_MD("uploadLeaderboardScore", "score", "keep_best"), &Steam::uploadLeaderboardScore, DEFVAL(true));
	ObjectTypeDB::bind_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
	ObjectTypeDB::bind_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
	ObjectTypeDB::bind_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);
	// Utils Bind Methods ///////////////////////
	ObjectTypeDB::bind_method("getIPCountry", &Steam::getIPCountry);
	ObjectTypeDB::bind_method("isOverlayEnabled", &Steam::isOverlayEnabled);
	ObjectTypeDB::bind_method("getSteamUILanguage", &Steam::getSteamUILanguage);
	ObjectTypeDB::bind_method("getAppID", &Steam::getAppID);
	ObjectTypeDB::bind_method(_MD("getImageRGBA", "image"), &Steam::getImageRGBA);
	ObjectTypeDB::bind_method(_MD("getImageSize", "image"), &Steam::getImageSize);
	ObjectTypeDB::bind_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
	ObjectTypeDB::bind_method(_MD("setOverlayNotificationPosition", "pos"), &Steam::setOverlayNotificationPosition);
	ObjectTypeDB::bind_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
	ObjectTypeDB::bind_method("getServerRealTime", &Steam::getServerRealTime);
	ObjectTypeDB::bind_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
	ObjectTypeDB::bind_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
	ObjectTypeDB::bind_method("startVRDashboard", &Steam::startVRDashboard);
	// Workshop Bind Methods ////////////////////
	ObjectTypeDB::bind_method("downloadItem", &Steam::downloadItem);
	ObjectTypeDB::bind_method("suspendDownloads", &Steam::suspendDownloads);
	ObjectTypeDB::bind_method("startItemUpdate", &Steam::startItemUpdate);
	ObjectTypeDB::bind_method("getItemState", &Steam::getItemState);
//	ObjectTypeDB::bind_method("getItemUpdateProgress", &Steam::getItemUpdateProgress);
	ObjectTypeDB::bind_method("createItem", &Steam::createItem);
	ObjectTypeDB::bind_method("setItemTitle", &Steam::setItemTitle);
	ObjectTypeDB::bind_method("setItemDescription", &Steam::setItemDescription);
	ObjectTypeDB::bind_method("setItemUpdateLanguage", &Steam::setItemUpdateLanguage);
	ObjectTypeDB::bind_method("setItemMetadata", &Steam::setItemMetadata);
	ObjectTypeDB::bind_method("setItemVisibility", &Steam::setItemVisibility);
//	ObjectTypeDB::bind_method("setItemTags", &Steam::setItemTags);
	ObjectTypeDB::bind_method("setItemContent", &Steam::setItemContent);
	ObjectTypeDB::bind_method("setItemPreview", &Steam::setItemPreview);
	ObjectTypeDB::bind_method("submitItemUpdate", &Steam::submitItemUpdate);
//	ObjectTypeDB::bind_method("getSubscribedItems", &Steam::getSubscribedItems);
//	ObjectTypeDB::bind_method("getItemInstallInfo", &Steam::getItemInstallInfo);
//	ObjectTypeDB::bind_method("getItemDownloadInfo", &Steam::getItemDownloadInfo);
	// Signals //////////////////////////////////
	ADD_SIGNAL(MethodInfo("file_details_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "fileSize"), PropertyInfo(Variant::INT, "fileHash"), PropertyInfo(Variant::INT, "flags")));
	ADD_SIGNAL(MethodInfo("join_requested", PropertyInfo(Variant::INT, "from"), PropertyInfo(Variant::STRING, "connect_string")));
	ADD_SIGNAL(MethodInfo("avatar_loaded", PropertyInfo(Variant::INT, "size")));
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
	ADD_SIGNAL(MethodInfo("user_achievement_icon_fetched", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::STRING, "achievementName"), PropertyInfo(Variant::BOOL, "achieved"), PropertyInfo(Variant::INT, "iconHandle")));
	ADD_SIGNAL(MethodInfo("global_achievement_percentages_ready", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("workshop_item_created"));
	ADD_SIGNAL(MethodInfo("workshop_item_updated"));
	ADD_SIGNAL(MethodInfo("workshop_item_installed"));
	// Status constants //////////////////////////
	BIND_CONSTANT(OFFLINE);		// 0
	BIND_CONSTANT(ONLINE);		// 1
	BIND_CONSTANT(BUSY);		// 2
	BIND_CONSTANT(AWAY);		// 3
	BIND_CONSTANT(SNOOZE);		// 4
	BIND_CONSTANT(LF_TRADE);	// 5
	BIND_CONSTANT(LF_PLAY);		// 6
	BIND_CONSTANT(NOT_OFFLINE); // Custom
	BIND_CONSTANT(ALL); 		// Custom
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
	// Matchmaking types ////////////////////////
	BIND_CONSTANT(PRIVATE);				// Only way to join the lobby is to invite to someone else
	BIND_CONSTANT(FRIENDS_ONLY);		// Shows for friends or invitees, but not in lobby list
	BIND_CONSTANT(PUBLIC);				// Visible for friends and in lobby list
	BIND_CONSTANT(INVISIBLE);			// Returned by search, but not visible to other friends
	BIND_CONSTANT(LOBBY_KEY_LENGTH);	// Maximum number of characters a lobby metadata key can be
	// Matchmaking lobby responses //////////////
	BIND_CONSTANT(LOBBY_OK);				// Lobby was successfully created
	BIND_CONSTANT(LOBBY_NO_CONNECTION);		// Your Steam client doesn't have a connection to the back-end
	BIND_CONSTANT(LOBBY_TIMEOUT);			// Message to the Steam servers, but it didn't respond
	BIND_CONSTANT(LOBBY_FAIL);				// Server responded, but with an unknown internal error
	BIND_CONSTANT(LOBBY_ACCESS_DENIED);		// Game isn't set to allow lobbies, or your client does haven't rights to play the game
	BIND_CONSTANT(LOBBY_LIMIT_EXCEEDED);	// Game client has created too many lobbies
	// Remote storage ///////////////////////////
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_NONE);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_WINDOWS);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_OSX);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_PS3);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_LINUX);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_RESERVED2);
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_ALL);
	// Workshop item characters /////////////////
	BIND_CONSTANT(UGC_MAX_TITLE_CHARS);		// 128
	BIND_CONSTANT(UGC_MAX_DESC_CHARS);		// 8000
	BIND_CONSTANT(UGC_MAX_METADATA_CHARS);	// 5000
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
	// Workshop item stats //////////////////////
	BIND_CONSTANT(UGC_STATE_NONE);			// Not tracked on client
	BIND_CONSTANT(UGC_STATE_SUBSCRIBED);	// Current user is subscribed to this item, not just cached
	BIND_CONSTANT(UGC_STATE_LEGACY);		// Was created with ISteamRemoteStorage
	BIND_CONSTANT(UGC_STATE_INSTALLED);		// Is installed and usable (but maybe out of date)
	BIND_CONSTANT(UGC_STATE_UPDATE);		// Needs an update, either because it's not installed yet or creator updated content
	BIND_CONSTANT(UGC_STATE_DOWNLOADING);	// Update is currently downloading
	BIND_CONSTANT(UGC_STATE_PENDING);		// DownloadItem() was called for this item, content isn't available until DownloadItemResult_t is fired
	// Workshop item visibility//////////////////
	BIND_CONSTANT(UGC_FILE_VISIBLE_PUBLIC);
	BIND_CONSTANT(UGC_FILE_VISIBLE_FRIENDS);
	BIND_CONSTANT(UGC_FILE_VISIBLE_PRIVATE);
	// Workshop item update status //////////////
	BIND_CONSTANT(STATUS_INVALID);				// Update handle was invalid, job might be finished, listen to SubmitItemUpdateResult_t
	BIND_CONSTANT(STATUS_PREPARING_CONFIG);		// Update is processing configuration data
	BIND_CONSTANT(STATUS_PREPARING_CONTENT);	// Update is reading and processing content files
	BIND_CONSTANT(STATUS_UPLOADING_CONTENT);	// Update is uploading content changes to Steam
	BIND_CONSTANT(STATUS_UPLOADING_PREVIEW);	// Update is uploading new preview file image
	BIND_CONSTANT(STATUS_COMMITTING_CHANGES);	// Update is committing all changes
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