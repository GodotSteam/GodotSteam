#include "godotsteam.h"
#include "IP.hpp"

Steam *Steam::singleton = NULL;

Steam::Steam(){
	isInitSuccess = false;
	singleton = this;
	// tickets.clear();
}

Steam *Steam::get_singleton(){
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
// Returns purchase key for given application/game. You'll receive an AppProofOfPurchaseKeyResponse_t callback when
// the key is available (which may be immediately).
void Steam::requestAppProofOfPurchaseKey(int value){
	if(SteamApps() == NULL)	{
		return;
	}
	SteamApps()->RequestAppProofOfPurchaseKey(value);
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
		if(!SteamFriends()->RequestUserInformation(friendID, true)){
			return SteamFriends()->GetFriendPersonaName(friendID);
		}
	}
	return "";
}
// Set the game information in Steam; used in 'View Game Info'
void Steam::setGameInfo(const String &s_key, const String &s_value){
	// Rich presence data is automatically shared between friends in the same game
	// Each user has a set of key/value pairs, up to 20 can be set
	// Two magic keys (status, connect)
	// setGameInfo() to an empty string deletes the key
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->SetRichPresence(s_key.utf8().get_data(), s_value.utf8().get_data());
}
// Clear the game information in Steam; used in 'View Game Info'
void Steam::clearGameInfo(){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ClearRichPresence();
}
// Invite friend to current game/lobby
void Steam::inviteFriend(int steamID, const String &conString){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->InviteUserToGame(createSteamID(steamID), conString.utf8().get_data());
}
// Set player as 'Played With' for game
void Steam::setPlayedWith(int steamID){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->SetPlayedWith(createSteamID(steamID));
}
// Get list of players user has recently played game with
Array Steam::getRecentPlayers(){
	Array recents;
	if(SteamFriends() == NULL){
		return recents;
	}
	int rCount = SteamFriends()->GetCoplayFriendCount();
	for(int i = 0; i < rCount; ++i){
		CSteamID recentPlayerID = SteamFriends()->GetCoplayFriend(i);
		if(SteamFriends()->GetFriendCoplayGame(recentPlayerID) == SteamUtils()->GetAppID()){
			Dictionary recentPlayer;
			recentPlayer["id"] = recentPlayerID.GetAccountID();
			recentPlayer["name"] = (String)SteamFriends()->GetFriendPersonaName(recentPlayerID);
			recentPlayer["status"] = (int)SteamFriends()->GetFriendPersonaState(recentPlayerID);
			recents.append(recentPlayer);
		}
	}
	return recents;
}
// Get player's avatar
void Steam::getFriendAvatar(int size){
	if(!(size > AVATAR_SMALL && size < AVATAR_LARGE) || SteamFriends() == NULL){
		return;
	}
	int iHandle = -2;
	switch (size){
		case AVATAR_SMALL:{
			iHandle = SteamFriends()->GetSmallFriendAvatar(SteamUser()->GetSteamID());
			size = 32;
			break;
		}
		case AVATAR_MEDIUM:{
			iHandle = SteamFriends()->GetMediumFriendAvatar(SteamUser()->GetSteamID());
			size = 64;
			break;
		}
		case AVATAR_LARGE:{
			iHandle = SteamFriends()->GetLargeFriendAvatar(SteamUser()->GetSteamID());
			size = 184;
			break;
		}
		default:
			return;
		}
	if(iHandle <= 0){
		if(iHandle == -1){
			// Still loading
			return;
		}
		// Invalid
		return;
	}
	// Has already loaded, simulate callback
	AvatarImageLoaded_t *avatarData = new AvatarImageLoaded_t;
	avatarData->m_steamID = SteamUser()->GetSteamID();
	avatarData->m_iImage = iHandle;
	avatarData->m_iWide = size;
	avatarData->m_iTall = size;
	// _avatar_loaded(avatarData);
	delete avatarData;
	return;
}
// Draw the given avatar
Image Steam::drawAvatar(int iSize, uint8 *iBuffer){
	// Apply buffer to Image
	Image avatar;
	avatar.create(iSize, iSize, false, Image::FORMAT_RGBA8);
	for(int y = 0; y < iSize; ++y){
		for(int x = 0; x < iSize; ++x)
		{
			int i = 4 * (x + y * iSize);
			float r = float(iBuffer[i]) / 255;
			float g = float(iBuffer[i + 1]) / 255;
			float b = float(iBuffer[i + 2]) / 255;
			float a = float(iBuffer[i + 3]) / 255;
			avatar.set_pixel(x, y, Color(r, g, b, a));
		}
	}
	return avatar;
}
// Activates the overlay with optional dialog to open the following: "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite"
void Steam::activateGameOverlay(const String &url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
}
// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore"
void Steam::activateGameOverlayToUser(const String &url, int steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID overlayUserID = createSteamID(steamID);
	SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), overlayUserID);
}
// Activates the overlay with specified web address
void Steam::activateGameOverlayToWebPage(const String &url){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToWebPage(url.utf8().get_data());
}
// Activates the overlay with the application/game Steam store page
void Steam::activateGameOverlayToStore(int app_id){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ActivateGameOverlayToStore(AppId_t(app_id), EOverlayToStoreFlag(0));
}
// Get list of user's Steam groups; a mix of different Steamworks API group functions
Array Steam::getUserSteamGroups(){
	Array steamGroups;
	if(SteamFriends() == NULL){
		return steamGroups;
	}
	int groupCount = SteamFriends()->GetClanCount();
	for(int i = 0; i < groupCount; ++i){
		Dictionary group;
		CSteamID groupID = SteamFriends()->GetClanByIndex(i);
		group["id"] = groupID.GetAccountID();
		group["name"] = (String)SteamFriends()->GetClanName(groupID);
		group["tag"] = (String)SteamFriends()->GetClanTag(groupID);
		steamGroups.append(group);
	}
	return steamGroups;
}
// Get a list of user's Steam friends; a mix of different Steamworks API friend functions
Array Steam::getUserSteamFriends(){
	Array steamFriends;
	if(SteamFriends() == NULL){
		return steamFriends;
	}
	int fCount = SteamFriends()->GetFriendCount(0x04);
	for(int i = 0; i < fCount; ++i){
		Dictionary friends;
		CSteamID friendID = SteamFriends()->GetFriendByIndex(i, 0x04);
		friends["id"] = friendID.GetAccountID();
		friends["name"] = (String)SteamFriends()->GetFriendPersonaName(friendID);
		friends["status"] = (int)SteamFriends()->GetFriendPersonaState(friendID);
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
void Steam::createLobby(int lobbyType, int cMaxMembers){
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
	SteamMatchmaking()->CreateLobby(eLobbyType, cMaxMembers);
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
bool Steam::fileWrite(const String &chFile, const PoolByteArray &vData, int32 cubData){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWrite(chFile.utf8().get_data(), vData.read().ptr(), cubData);
}
// Read given file from Steam Cloud
Dictionary Steam::fileRead(const String &chFile, int32 cubDataToRead){
	Dictionary d;
	if(SteamRemoteStorage() == NULL){
		return d;
	}
	PoolByteArray vData;
	vData.resize(cubDataToRead);
	d["ret"] = SteamRemoteStorage()->FileRead(chFile.utf8().get_data(), vData.write().ptr(), cubDataToRead);
	d["buf"] = vData;
	return d;
}
// Delete file from remote storage but leave it on local disk to remain accessible
bool Steam::fileForget(const String &chFile){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileForget(chFile.utf8().get_data());
}
// Delete a given file in Steam Cloud
bool Steam::fileDelete(const String &chFile){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileDelete(chFile.utf8().get_data());
}
// Check if a given file exists in Steam Cloud
bool Steam::fileExists(const String &chFile){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileExists(chFile.utf8().get_data());
}
// Check if a given file is persisted in Steam Cloud
bool Steam::filePersisted(const String &chFile){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FilePersisted(chFile.utf8().get_data());
}
// Get the size of a given file
int32_t Steam::getFileSize(const String &chFile){
	if(SteamRemoteStorage() == NULL){
		return -1;
	}
	return SteamRemoteStorage()->GetFileSize(chFile.utf8().get_data());
}
// Get the timestamp of when the file was uploaded/changed
int64_t Steam::getFileTimestamp(const String &chFile){
	if(SteamRemoteStorage() == NULL){
		return -1;
	}
	return SteamRemoteStorage()->GetFileTimestamp(chFile.utf8().get_data());
}
// Gets the total number of local files synchronized by Steam Cloud
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
// Set Steam Cloud enabled for this application
void Steam::setCloudEnabledForApp(bool bEnabled){
	if(SteamRemoteStorage() == NULL){
		return;
	}

	return SteamRemoteStorage()->SetCloudEnabledForApp(bEnabled);
}
/////////////////////////////////////////////////
///// SCREENSHOTS ///////////////////////////////
//
// Toggles whether the overlay handles screenshots
void Steam::hookScreenshots(bool bHook){
	if(SteamScreenshots() == NULL){
		return;
	}

	SteamScreenshots()->HookScreenshots(bHook);
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
uint32_t Steam::writeScreenshot(const PoolByteArray &RGB, int nWidth, int nHeight){
	if(SteamScreenshots() == NULL){
		return 0;
	}

	return SteamScreenshots()->WriteScreenshot((void *)RGB.read().ptr(), RGB.size(), nWidth, nHeight);
}
/////////////////////////////////////////////////
///// SIGNALS ///////////////////////////////////
//
// Signal the lobby has been created
// void Steam::OnLobbyCreated(LobbyCreated_t *lobbyData){
// 	int connect;
// 	// Convert the lobby response back over
// 	if(lobbyData->m_eResult == k_EResultOK){
// 		connect = LOBBY_OK;
// 	}
// 	else if(lobbyData->m_eResult == k_EResultNoConnection){
// 		connect = LOBBY_NO_CONNECTION;
// 	}
// 	else if(lobbyData->m_eResult == k_EResultTimeout){
// 		connect = LOBBY_TIMEOUT;
// 	}
// 	else if(lobbyData->m_eResult == k_EResultFail){
// 		connect = LOBBY_FAIL;
// 	}
// 	else if(lobbyData->m_eResult == k_EResultAccessDenied){
// 		connect = LOBBY_ACCESS_DENIED;
// 	}
// 	else{
// 		connect = LOBBY_LIMIT_EXCEEDED;
// 	}
// 	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
// 	Array args;
// 	args.append(connect);
// 	args.append(lobbyID);
// 	emit_signal("lobby_created", args);
// }
// // Signal that lobby has been joined
// void Steam::_lobby_joined(LobbyEnter_t *lobbyData){
// 	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
// 	uint32 permissions = lobbyData->m_rgfChatPermissions;
// 	bool locked = lobbyData->m_bLocked;
// 	uint32 response = lobbyData->m_EChatRoomEnterResponse;
// 	Array args;
// 	args.append(lobbyID);
// 	args.append(permissions);
// 	args.append(locked);
// 	args.append(response);
// 	emit_signal("lobby_joined", args);
// }
// // Signal that a lobby invite was sent
// void Steam::_lobby_invite(LobbyInvite_t *lobbyData){
// 	int inviterID = (uint64)lobbyData->m_ulSteamIDUser;
// 	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
// 	int gameID = (uint64)lobbyData->m_ulGameID;
// 	Array args;
// 	args.append(inviterID);
// 	args.append(lobbyID);
// 	args.append(gameID);
// 	emit_signal("lobby_invite", args);
// }
// // Signal a game/lobby join has been requested
// void Steam::_join_requested(GameRichPresenceJoinRequested_t *callData){
// 	int steamID = callData->m_steamIDFriend.GetAccountID();
// 	String con_string = callData->m_rgchConnect;
// 	Array args;
// 	args.append(steamID);
// 	args.append(con_string);
// 	emit_signal("join_requested", args);
// }
// // Signal that the avatar has been loaded
// void Steam::_avatar_loaded(AvatarImageLoaded_t *avatarData){
// 	if(avatarData->m_steamID != SteamUser()->GetSteamID()){
// 		return;
// 	}
// 	int size = avatarData->m_iWide;
// 	// Get image buffer
// 	int buffSize = 4 * size * size;
// 	uint8 *iBuffer = new uint8[buffSize];
// 	bool success = SteamUtils()->GetImageRGBA(avatarData->m_iImage, iBuffer, buffSize);
// 	if(!success){
// 		printf("[Steam] Failed to load image buffer from callback\n");
// 		return;
// 	}
// 	int rSize;
// 	if(size == 32){
// 		rSize = AVATAR_SMALL;
// 	}
// 	else if(size == 64){
// 		rSize = AVATAR_MEDIUM;
// 	}
// 	else if(size == 184){
// 		rSize = AVATAR_LARGE;
// 	}
// 	else{
// 		printf("[Steam] Invalid avatar size from callback\n");
// 		return;
// 	}
// 	Image avatar = drawAvatar(size, iBuffer);
// 	Array args;
// 	args.append(rSize);
// 	// args.append(avatar);  // TODO: Figureout how to pass an image
// 	emit_signal("avatar_loaded", args);
// }
// // Signal number of current players (online + offline)
// void Steam::_number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure){
// 	Array args;
// 	args.append(callData->m_bSuccess && bIOFailure);
// 	args.append(callData->m_cPlayers);
// 	emit_signal("number_of_current_players", args);
// }
// // Signal a leaderboard has been loaded or has failed
// void Steam::_leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure){
// 	if(callData->m_bLeaderboardFound == 0){
// 		emit_signal("leaderboard_loaded");
// 	}
// 	else{
// 		Array args;
// 		args.append((uint8)callData->m_hSteamLeaderboard);  // uint8 sLeaderboard
// 		emit_signal("leaderboard_loaded", args);
// 	}
// }
// // Signal a leaderboard entry has been uploaded
// void Steam::_leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure){
// 	// Incorrect leaderboard
// 	if(callData->m_hSteamLeaderboard != leaderboard_handle){
// 		return;
// 	}
// 	Array args;
// 	args.append(callData->m_bSuccess && bIOFailure);
// 	args.append(callData->m_nScore);
// 	args.append(callData->m_bScoreChanged);
// 	args.append(callData->m_nGlobalRankNew);
// 	args.append(callData->m_nGlobalRankPrevious);
// 	emit_signal("leaderboard_uploaded", args);
// }
// // Signal leaderboard entries are downloaded
// void Steam::_leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData){
// 	// Incorrect leaderboard
// 	if(callData->m_hSteamLeaderboard != leaderboard_handle){
// 		return;
// 	}
// 	getDownloadedLeaderboardEntry(callData->m_hSteamLeaderboardEntries, callData->m_cEntryCount);
// 	emit_signal("leaderboard_entries_loaded");
// }
// // Signal when overlay is triggered
// void Steam::_overlay_toggled(GameOverlayActivated_t *callData){
// 	Array args;
// 	args.append(callData->m_bActive);
// 	emit_signal("overlay_toggled", args);
// }
// // Signal when battery power is running low, less than 10 minutes left
// void Steam::_low_power(LowBatteryPower_t *timeLeft){
// 	Array args;
// 	args.append(timeLeft->m_nMinutesBatteryLeft); // uint8 power
// 	emit_signal("low_power", args);
// }
// // When connected to a server
// void Steam::_server_connected(SteamServersConnected_t *conData){
// 	Array args;
// 	args.append(true);
// 	emit_signal("connection_changed", args);
// }
// // When disconnected from a server
// void Steam::_server_disconnected(SteamServersDisconnected_t *conData){
// 	Array args;
// 	args.append(false);
// 	emit_signal("connection_changed", args);
// }
// // Response to RequestAppProofOfPurchaseKey/RequestAllProofOfPurchaseKeys for supporting third-party CD keys, or other proof-of-purchase systems
// // CURRENTLY CAUSES ERROR AS CANNOT CONVERT FROM CHAR TO STRING
// // void Steam::_request_proofofpurchase(AppProofOfPurchaseKeyResponse_t* callData){
// // 	int appID = (uint32)callData->m_nAppID;
// // 	int keyLength = (uint32)callData->m_cchKeyLength;
// // 	String key = callData->m_rgchKey[k_cubAppProofOfPurchaseKeyMax];
// // 	emit_signal("request_proofofpurchase", appID, keyLength, key.utf8().get_data());
// // }
// // Posted after the user gains ownership of DLC & that DLC is installed
// void Steam::_dlc_installed(DlcInstalled_t *callData){
// 	Array args;
// 	args.append((AppId_t)callData->m_nAppID);  // int appID
// 	emit_signal("dlc_installed", args);
// }
// // Response from getAuthSessionTicket
// void Steam::_get_auth_session_ticket_response(GetAuthSessionTicketResponse_t *callData){
// 	Array args;
// 	args.append(callData->m_hAuthTicket);
// 	args.append(callData->m_eResult);
// 	emit_signal("get_auth_session_ticket_response", args);
// }
// // Called when an auth ticket has been validated
// void Steam::_validate_auth_ticket_response(ValidateAuthTicketResponse_t *callData){
// 	Array args;
// 	args.append(callData->m_SteamID.ConvertToUint64());  // uint64_t authID
// 	args.append(callData->m_eAuthSessionResponse);  // uint32_t response
// 	args.append(callData->m_OwnerSteamID.ConvertToUint64());  // uint64_t ownerID
// 	emit_signal("validate_auth_ticket_response", args);
// }
// // A screenshot has been requested by the user
// void Steam::_screenshot_ready(ScreenshotReady_t *callData){
// 	Array args;
// 	args.append(callData->m_hLocal);
// 	args.append(callData->m_eResult);
// 	emit_signal("screenshot_ready", args);
// }
/////////////////////////////////////////////////
///// USERS /////////////////////////////////////
//
// Get an authentication ticket
// uint32_t Steam::getAuthSessionTicket(){
// 	if(SteamUser() == NULL){
// 		return 0;
// 	}
// 	uint32_t cbTicket = 1024;
// 	uint32_t *buf = memnew_arr(uint32_t, cbTicket);
// 	uint32_t id = SteamUser()->GetAuthSessionTicket(buf, cbTicket, &cbTicket);
// 	TicketData ticket = {id, buf, cbTicket};
// 	tickets.push_back(ticket);
// 	return id;
// }
// // Cancels an auth ticket
// void Steam::cancelAuthTicket(uint32_t hAuthTicket){
// 	if(SteamUser() == NULL){
// 		return;
// 	}
// 	for(int i = 0; i < tickets.size(); ++i){
// 		TicketData ticket = tickets.get(i);
// 		if(ticket.id == hAuthTicket){
// 			tickets.remove(i);
// 			memdelete_arr(ticket.buf);
// 			break;
// 		}
// 	}
// }
// // Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused
// int Steam::beginAuthSession(uint32_t hAuthTicket, uint64_t steamID){
// 	if(SteamUser() == NULL){
// 		return -1;
// 	}
// 	for(int i = 0; i < tickets.size(); ++i){
// 		TicketData ticket = tickets.get(i);
// 		if(ticket.id == hAuthTicket){
// 			CSteamID authSteamID = createSteamID(steamID);
// 			return SteamUser()->BeginAuthSession(ticket.buf, ticket.size, authSteamID);
// 		}
// 	}
// 	return -1;
// }
// Ends an auth session
void Steam::endAuthSession(uint64_t steamID){
	if(SteamUser() == NULL){
		return;
	}
	return SteamUser()->EndAuthSession(createSteamID(steamID));
}
// Get user's Steam ID
uint64_t Steam::getSteamID(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUser()->GetSteamID().ConvertToUint64();
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
	SteamUser()->GetUserDataFolder((char *)pchBuffer, cubBuffer);
	String data_path = pchBuffer;
	delete pchBuffer;
	return data_path;
}
// // (LEGACY FUNCTION) Set data to be replicated to friends so that they can join your game
// void Steam::advertiseGame(const String &server_ip, int port){
// 	if(SteamUser() == NULL){
// 		return;
// 	}
// 	// Resolve address and convert it from IP_Address struct to uint32_t
// 	IP_Address addr;
// 	if(server_ip.is_valid_ip_address()){
// 		addr = server_ip;
// 	}
// 	else{
// 		addr = IP::get_singleton()->resolve_hostname(server_ip, IP::TYPE_IPV4);
// 	}
// 	// Resolution failed - Godot 3.0 has is_invalid() to check this
// 	if(addr == IP_Address()){
// 		return;
// 	}
// 	uint32_t ip4 = *((uint32_t *)addr.get_ipv4());
// 	// Swap the bytes
// 	uint8_t *ip4_p = (uint8_t *)&ip4;
// 	for(int i = 0; i < 2; ++i){
// 		uint8_t temp = ip4_p[i];
// 		ip4_p[i] = ip4_p[3 - i];
// 		ip4_p[3 - i] = temp;
// 	}
// 	CSteamID gameserverID = SteamUser()->GetSteamID();
// 	SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
// }
// Trading Card badges data access, if you only have one set of cards, the series will be 1
// The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1)
int Steam::getGameBadgeLevel(int series, bool foil){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUser()->GetGameBadgeLevel(series, foil);
}
/////////////////////////////////////////////////
///// USER STATS ////////////////////////////////
//
// Clears a given achievement
bool Steam::clearAchievement(const String &s_key){
	return SteamUserStats()->ClearAchievement(s_key.utf8().get_data());
}
// Return true/false if use has given achievement
bool Steam::getAchievement(const String &s_key){
	bool achieved = false;
	SteamUserStats()->GetAchievement(s_key.utf8().get_data(), &achieved);
	return achieved;
}
//  Get the amount of players currently playing the current game (online + offline)
void Steam::getNumberOfCurrentPlayers(){
	if(SteamUserStats() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->GetNumberOfCurrentPlayers();
	// callResultNumberOfCurrentPlayers.Set(apiCall, this, &Steam::_number_of_current_players);
}
// Get the value of a float statistic
float Steam::getStatFloat(const String &s_key){
	float statval = 0;
	SteamUserStats()->GetStat(s_key.utf8().get_data(), &statval);
	return statval;
}
// Get the value of an integer statistic
int Steam::getStatInt(const String &s_key){
	int32 statval = 0;
	SteamUserStats()->GetStat(s_key.utf8().get_data(), &statval);
	return statval;
}
// Reset all Steam statistics; optional to reset achievements
bool Steam::resetAllStats(bool bAchievementsToo){
	SteamUserStats()->ResetAllStats(bAchievementsToo);
	return SteamUserStats()->StoreStats();
}
// Request all statistics and achievements from Steam servers
bool Steam::requestCurrentStats(){
	return SteamUserStats()->RequestCurrentStats();
}
// Set a given achievement
bool Steam::setAchievement(const String &s_key){
	if(SteamUserStats() == NULL){
		return 0;
	}
	SteamUserStats()->SetAchievement(s_key.utf8().get_data());
	return SteamUserStats()->StoreStats();
}
// Set a float statistic
bool Steam::setStatFloat(const String &s_key, float value){
	return SteamUserStats()->SetStat(s_key.utf8().get_data(), value);
}
// Set an integer statistic
bool Steam::setStatInt(const String &s_key, int value){
	return SteamUserStats()->SetStat(s_key.utf8().get_data(), value);
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
void Steam::findLeaderboard(const String &lName){
	if(SteamUserStats() == NULL){
		return;
	}
	//SteamAPICall_t apiCall =
	SteamUserStats()->FindLeaderboard(lName.utf8().get_data());
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
void Steam::downloadLeaderboardEntries(int rStart, int rEnd, int type){
	if(SteamUserStats() == NULL){
		return;
	}
	//SteamAPICall_t apiCall =
	SteamUserStats()->DownloadLeaderboardEntries(leaderboard_handle, ELeaderboardDataRequest(type), rStart, rEnd);
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
	CSteamID *pUsers = new CSteamID[users_count];
	for(int i = 0; i < users_count; ++i){
		CSteamID user = createSteamID(usersID[i]);
		pUsers[i] = user;
	}
	SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboard_handle, pUsers, users_count);
	delete[] pUsers;
}
// Upload a leaderboard score for the user
void Steam::uploadLeaderboardScore(int score, bool keepBest){
	if(SteamUserStats() == NULL){
		return;
	}
	ELeaderboardUploadScoreMethod method = keepBest ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
	SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(leaderboard_handle, method, (int32)score, NULL, 0);
	// callResultUploadScore.Set(apiCall, this, &Steam::_leaderboard_uploaded);
}
// Once all entries are accessed, the data will be freed up and the handle will become invalid, use this to store it
void Steam::getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount){
	if(SteamUserStats() == NULL){
		return;
	}
	leaderboard_entries.clear();
	static LeaderboardEntry_t *entry = new LeaderboardEntry_t;
	for(int i = 0; i < entryCount; ++i){
		Dictionary entryDict;
		SteamUserStats()->GetDownloadedLeaderboardEntry(eHandle, i, entry, NULL, 0);
		entryDict["score"] = entry->m_nScore;
		entryDict["steam_id"] = entry->m_steamIDUser.GetAccountID();
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
bool Steam::getAchievementAndUnlockTime(const String &name, bool achieved, int unlockTime){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->GetAchievementAndUnlockTime(name.utf8().get_data(), (bool *)achieved, (uint32_t *)unlockTime);
}
// Achievement progress, triggers an AchievementProgress callback, that is all.
// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
bool Steam::indicateAchievementProgress(const String &name, int curProgress, int maxProgress){
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
		return;
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
// If item is not subscribed to, it will be cached for some time. If bHighPriority is set, any other item download will be suspended and this item downloaded ASAP.
bool Steam::downloadItem(int publishedFileID, bool bHighPriority){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->DownloadItem(fileID, bHighPriority);
}
// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends
void Steam::suspendDownloads(bool bSuspend){
	return SteamUGC()->SuspendDownloads(bSuspend);
}

Steam::~Steam(){
	if(isInitSuccess){
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	singleton = NULL;
}