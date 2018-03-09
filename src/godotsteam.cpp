#include "godotsteam.h"
#include "IP.hpp"

Steam *Steam::singleton = NULL;

Steam::Steam(){
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
// Returns true/false if Steam is running
bool Steam::isSteamRunning(void){
	return SteamAPI_IsSteamRunning();
}
/////////////////////////////////////////////////
///// APPS //////////////////////////////////////
//
// Check if the user has a given application/game
bool Steam::hasOtherApp(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedApp((AppId_t)value);
}
// Get the number of DLC the user owns for a parent application/game
int Steam::getDLCCount(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->GetDLCCount();
}
// Takes AppID of DLC and checks if the user owns the DLC & if the DLC is installed
bool Steam::isDLCInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsDlcInstalled(value);
}
// Check if given application/game is installed, not necessarily owned
bool Steam::isAppInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsAppInstalled((AppId_t)value);
}
// Get the user's game language
String Steam::getCurrentGameLanguage(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetCurrentGameLanguage();
}
// Does the user have a VAC ban for this game
bool Steam::isVACBanned(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsVACBanned();
}
// Returns the Unix time of the purchase of the app
int Steam::getEarliestPurchaseUnixTime(int value){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
}
// Checks if the user is subscribed to the current app through a free weekend
// This function will return false for users who have a retail or other type of license
// Suggested you contact Valve on how to package and secure your free weekend properly
bool Steam::isSubscribedFromFreeWeekend(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedFromFreeWeekend();
}
// Install/Uninstall control for optional DLC
void Steam::installDLC(int value){
	if(SteamApps() == NULL){
		return;
	}
	SteamApps()->InstallDLC((AppId_t)value);
}
void Steam::uninstallDLC(int value){
	if(SteamApps() == NULL){
		return;
	}
	SteamApps()->UninstallDLC((AppId_t)value);
}
// Is subscribed lacks notes from Valve
bool Steam::isSubscribed(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribed();
}
// Presumably if Steam is set to low violence; lacks notes from Valve
bool Steam::isLowViolence(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsLowViolence();
}
// Presumably if users is a cyber cafe; lacks notes from Valve
bool Steam::isCybercafe(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsCybercafe();
}
// Only use to check ownership of another game related to yours: a demo, etc.
bool Steam::isSubscribedApp(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedApp((AppId_t)value);
}
// Return the build ID for this app; will change based on backend updates
int Steam::getAppBuildId(){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetAppBuildId();
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
// Set the game information in Steam; used in 'View Game Info'
void Steam::setGameInfo(const String& key, const String& value){
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
void Steam::inviteFriend(int steamID, const String& connectString){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->InviteUserToGame(friendID, connectString.utf8().get_data());
}
// Set player as 'Played With' for game
void Steam::setPlayedWith(int steamID){
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
void Steam::activateGameOverlay(const String& url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
}
// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore"
void Steam::activateGameOverlayToUser(const String& url, int steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID overlayUserID = createSteamID(steamID);
	SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), overlayUserID);
}
// Activates the overlay with specified web address
void Steam::activateGameOverlayToWebPage(const String& url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToWebPage(url.utf8().get_data());
}
// Activates the overlay with the application/game Steam store page
void Steam::activateGameOverlayToStore(int appID){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToStore(AppId_t(appID), EOverlayToStoreFlag(0));
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
	SteamMatchmaking()->CreateLobby(eLobbyType, maxMembers);
}
// Join an existing lobby
void Steam::joinLobby(int steamIDLobby){
	if(SteamMatchmaking() == NULL){
		return;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	SteamMatchmaking()->JoinLobby(lobbyID);
}
// Leave a lobby, this will take effect immediately on the client side, other users will be notified by LobbyChatUpdate_t callback
void Steam::leaveLobby(int steamIDLobby){
	CSteamID lobbyID = createSteamID(steamIDLobby);
	return SteamMatchmaking()->LeaveLobby(lobbyID);
}
// Invite another user to the lobby, the target user will receive a LobbyInvite_t callback, will return true if the invite is successfully sent, whether or not the target responds
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
bool Steam::fileWrite(const String& file, const PoolByteArray& vData, int32_t cubData){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWrite(file.utf8().get_data(), vData.read().ptr(), cubData);
}
// Read given file from Steam Cloud
Dictionary Steam::fileRead(const String& file, int32_t cubDataToRead){
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
bool Steam::fileForget(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileForget(file.utf8().get_data());
}
// Delete a given file in Steam Cloud
bool Steam::fileDelete(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileDelete(file.utf8().get_data());
}
// Check if a given file exists in Steam Cloud
bool Steam::fileExists(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileExists(file.utf8().get_data());
}
// Check if a given file is persisted in Steam Cloud
bool Steam::filePersisted(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FilePersisted(file.utf8().get_data());
}
// Get the size of a given file
int32_t Steam::getFileSize(const String& file){
	if(SteamRemoteStorage() == NULL){
		return -1;
	}
	return SteamRemoteStorage()->GetFileSize(file.utf8().get_data());
}
// Get the timestamp of when the file was uploaded/changed
int64_t Steam::getFileTimestamp(const String& file){
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
//void Steam::advertiseGame(const String& serverIP, int port){
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
bool Steam::clearAchievement(const String& name){
	return SteamUserStats()->ClearAchievement(name.utf8().get_data());
}
// Return true/false if use has given achievement
bool Steam::getAchievement(const String& name){
	bool achieved = false;
	SteamUserStats()->GetAchievement(name.utf8().get_data(), &achieved);
	return achieved;
}
// Returns the percentage of users who have unlocked the specified achievement
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
float Steam::getStatFloat(const String& name){
	float statval = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statval);
	return statval;
}
// Get the value of an integer statistic
int Steam::getStatInt(const String& name){
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
bool Steam::setAchievement(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	SteamUserStats()->SetAchievement(name.utf8().get_data());
	return SteamUserStats()->StoreStats();
}
// Set a float statistic
bool Steam::setStatFloat(const String& name, float value){
	return SteamUserStats()->SetStat(name.utf8().get_data(), value);
}
// Set an integer statistic
bool Steam::setStatInt(const String& name, int value){
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
void Steam::findLeaderboard(const String& name){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->FindLeaderboard(name.utf8().get_data());
//	callResultFindLeaderboard.Set(apiCall, this, &Steam::_leaderboard_loaded);
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
//	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
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
//	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
	delete[] users;
}
// Upload a leaderboard score for the user
void Steam::uploadLeaderboardScore(int score, bool keepBest){
	if(SteamUserStats() == NULL){
		return;
	}
	ELeaderboardUploadScoreMethod method = keepBest ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
	SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(leaderboard_handle, method, (int32)score, NULL, 0);
//	callResultUploadScore.Set(apiCall, this, &Steam::_leaderboard_uploaded);
}
// Once all entries are accessed, the data will be freed up and the handle will become invalid, use this to store it
void Steam::getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount){
	if(SteamUserStats() == NULL){
		return;
	}
	leaderboard_entries.clear();
	static LeaderboardEntry_t *entry = new LeaderboardEntry_t;
	for(int i = 0; i < entryCount; i++){
		SteamUserStats()->GetDownloadedLeaderboardEntry(eHandle, i, entry, NULL, 0);
		Dictionary entryDict;
		entryDict["score"] = entry->m_nScore;
		entryDict["steamID"] = entry->m_steamIDUser.GetAccountID();
		entryDict["global_rank"] = entry->m_nGlobalRank;
		leaderboard_entries.append(entryDict);
	}
	delete entry;
}
// Get the currently used leaderboard handle
uint64_t Steam::getLeaderboardHandle(){
	return leaderboard_handle;
}
// Get the currently used leaderboard entries
Array Steam::getLeaderboardEntries(){
	return leaderboard_entries;
}
// Get the achievement status, and the time it was unlocked if unlocked (in seconds since January 1, 19)
bool Steam::getAchievementAndUnlockTime(const String& name, bool achieved, int unlockTime){
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
// Get the number of subscribed items for the user
int Steam::getNumSubscribedItems(){
	if(SteamUGC() == NULL){
		return 0;
	}
	return SteamUGC()->GetNumSubscribedItems();
}
// Get the item's state
int Steam::getItemState(int publishedFileID){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->GetItemState(fileID);
}
// Download new or update already installed item. If returns true, wait for DownloadItemResult_t. If item is already installed, then files on disk should not be used until callback received.
// If item is not subscribed to, it will be cached for some time. If highPriority is set, any other item download will be suspended and this item downloaded ASAP.
bool Steam::downloadItem(int publishedFileID, bool highPriority){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->DownloadItem(fileID, highPriority);
}
// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends
void Steam::suspendDownloads(bool suspend){
	return SteamUGC()->SuspendDownloads(suspend);
}

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