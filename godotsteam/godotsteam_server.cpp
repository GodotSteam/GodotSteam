#include "godotsteam_server.h"
#include <steam/steam_gameserver.h>
#include "core/io/ip_address.h"
#include "core/io/ip.h"

SteamServer* SteamServer::singleton = NULL;

SteamServer::SteamServer(){
	isInitSuccess = false;
	singleton = this;
	tickets.clear();
}

SteamServer* SteamServer::get_singleton(){
	return singleton;
}
CSteamID SteamServer::createSteamID(uint32_t steamID, int accountType){
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
// Initialize the server to Steam.
bool SteamServer::serverInit(uint32 ip, uint16 steamPort, uint16 gamePort, uint16 queryPort, EServerMode serverMode, const String& versionString){
	if(!SteamInternal_GameServer_Init(ip, steamPort, gamePort, queryPort, serverMode, versionString)){
		return false;
	}
	return true;
}
// Frees all API-related memory associated with the calling thread. This memory is released automatically by RunCallbacks so single-threaded servers do not need to call this.
void SteamServer::serverReleaseCurrentThreadMemory(){
	SteamAPI_ReleaseCurrentThreadMemory();
}

// Shut down the server connection to Steam.
void SteamServer::serverShutdown(){
	SteamGameServer_Shutdown();
}
/////////////////////////////////////////////////
///// SERVER FUNCTIONS //////////////////////////
//
// NOTE: The following, if set, must be set before calling LogOn; they may not be changed after.
//
// This is called by SteamGameServer_Init and will usually not need to be called directly.
bool SteamServer::initGameServer(uint32 ip, uint16 steamPort, uint16 gamePort, uint16 queryPort, EServerMode serverMode, const String& versionString){
	if(SteamGameServer() == NULL){
		return false;
	}
	if(!SteamInternal_GameServer_Init(ip, steamPort, gamePort, queryPort, serverMode, versionString)){
		return false;
	}
	return true;
}
// Game product identifier; currently used by the master server for version checking purposes.
void SteamServer::setProduct(const String& product){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetProduct(product);
}
// Description of the game; required field and is displayed in the Steam server browser.
void SteamServer::setGameDescription(const String& description){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetGameDescription(description);
}
// If your game is a mod, pass the string that identifies it. Default is empty meaning the app is the original game.
void SteamServer::setModDir(const String& modDir){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetModDir(modDir);
}
// Is this a dedicated server? Default is false.
void SteamServer::setDedicatedServer(bool dedicated){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetDedicatedServer(dedicated);
}
// NOTE: The following are login functions.
//
// Being process to login to a persistent game server account. You need to register for callbacks to determine the result of this operation.
void SteamServer::logOn(const String& token){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamAPICall_t apiCall = SteamGameServer()->LogOn(token);
	callResultLogOn.Set(apiCall, this, &SteamServer::_log_on);
}
// Login to a generic, anonymous account.
void SteamServer::logOnAnonymous(){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->LogOnAnonymous();
}
// Begin process of logging game server out of Steam.
void SteamServer::logOff(){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->LogOff();
}
// Status functions.
bool SteamServer::loggedOn(){
	if(SteamGameServer() == NULL){
		return false;
	}
	return SteamGameServer()->BLoggedOn();
}
bool SteamServer::secure(){
	if(SteamGameServer() == NULL){
		return false;
	}
	return SteamGameServer()->BSecure();
}
uint64_t SteamServer::getSteamID(){
	if(SteamGameServer() == NULL){
		return 0;
	}
	CSteamID serverID = SteamGameServer()->GetSteamID();
	return serverID.ConvertToUint64();
}
// Returns true if the master server has requested a restart. Only returns true once per request.
bool SteamServer::wasRestartRequested(){
	if(SteamGameServer() == NULL){
		return false;
	}
	return SteamGameServer()->WasRestartRequested();
}
// NOTE: These are server state functions and can be changed at any time.
//
// Max player count that will be reported to server browser and client queries.
void SteamServer::setMaxPlayerCount(int max){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetMaxPlayerCount(max);
}
// Number of bots. Default is zero.
void SteamServer::setBotPlayerCount(int bots){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetBotPlayerCount(bots);
}
// Set the naem of the server as it will appear in the server browser.
void SteamServer::setServerName(const String& name){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetServerName(name);
}
// Set name of map to report in server browser.
void SteamServer::setMapName(const String& map){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetMapName(map);
}
// Let people know if your server requires a password.
void SteamServer::setPasswordProtected(bool protected){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetPasswordProtected(protected);
}
// Spectator server. Default is zero, meaning it is now used.
void SteamServer::setSpectatorPort(uint16 port){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetSpectatorPort(port);
}
// Name of spectator server. Only used if spectator port is non-zero.
void SteamServer::setSpectatorServerName(const String& name){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetSpectatorServerName(name);
}
// Call this to clear the whole list of key/values that are sent in rule queries.
void SteamServer::clearAllKeyValues(){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->ClearAllKeyValues();
}
// Call this to add/update a key/value pair.
void SteamServer::setKeyValue(const String& key, const String& value){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetKeyValue(key, value);
}
// Set a string defining game tags for this server; optional. Allows users to filter in matchmaking/server browser.
void SteamServer::setGameTags(const String& tags){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetGameTags(tags);
}
// Set a string defining game data for this server; optional. Allows users to filter in matchmaking/server browser.
void SteamServer::setGameData(const String& data){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetGameData(data);
}
// Region identifier; optional. Default is empty meaning 'world'.
void SteamServer::setRegion(const String& region){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetRegion(region);
}
// NOTE: These functions are player list management / authentication.
//
// Handles receiving a new connection from a Steam user.  This call will ask the Steam
// servers to validate the users identity, app ownership, and VAC status.  If the Steam servers 
// are off-line, then it will validate the cached ticket itself which will validate app ownership 
// and identity.  The AuthBlob here should be acquired on the game client using SteamUser()->InitiateGameConnection()
// and must then be sent up to the game server for authentication.
bool SteamServer::sendUserConnectAndAuthenticate(uint32 ipClient, const void *authBlob, uint32 authBlobSize, int steamID){
	if(SteamGameServer() == NULL){
		return false;
	}
	CSteamID authID = createSteamID(steamID);
	return SteamGameServer()->SendUserConnectAndAuthenticate(ipClient, *authBlob, authBlobSize, authID);
}
// Creates a fake user (ie, a bot) which will be listed as playing on the server, but skips validation.
uint64_t SteamServer::createUnauthenticatedUserConnection(){
	if(SteamGameServer() == NULL){
		return 0;
	}
	return SteamGameServer()->CreateUnauthenticatedUserConnection();
}
// Should be called whenever a user leaves server; lets Steam internally track which users are currently on which servers.
void SteamServer::sendUserDisconnect(int steamID){
	if(SteamGameServer() == NULL){
		return;
	}
	CSteamID disconnectID = createSteamID(steamID);
	SteamGameServer()->SendUserDisconnect(disconnectID);
}
// Update the data to be displayed in the server browser; for regular users you must call this after you recieve a GSUserValidationSuccess callback.
bool SteamServer::updateUserData(int steamID, const String& *name, uint32 score){
	if(SteamGameServer() == NULL){
		return false;
	}
	CSteamID updateID = createSteamID(steamID);
	return SteamGameServer()->BUpdateUserData(updateID, *name, score);
}
// NOTES: These are new auth system APIs, do not mix with old auth system APIs.
//
// Retrieve ticket to be sent to the entity who wishes to authenticate you (using BeginAuthSession API).
uint32 SteamServer::getAuthSessionTicket(void *ticket, int maxTicket, uint32 *pcbTicket){
	if(SteamGameServer() == NULL){
		return 0;
	}
	return SteamGameServer()->GetAuthSessionTicket(ticket, maxTicket, pcbTicket);
}
int SteamServer::beginAuthSession(const void *authTicket, int cbAuthTicket, int steamID){
	if(SteamGameServer() == NULL){
		return 1;
	}
	CSteamID authID = createSteamID(steamID);
	return SteamGameServer()->BeginAuthSession(authTicket, cbAuthTicket, authID)
}
// Stop tracking started by beginAuthSession; called when no longer playing game with this entity;
void SteamServer::endAuthSession(int steamID){
	if(SteamGameServer() == NULL){
		return;
	}
	CSteamID authID = createSteamID(steamID);
	SteamGameServer()->EndAuthSession(authID);
}
// Cancel auth ticket from getAuthSessionTicket; called when no longer playing game with the entity you gave the ticket to.
void SteamServer::cancelAuthTicket(int authTicket){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->CancelAuthTicket(authTicket);
}
// After receiving a user's authentication data, and passing it to sendUserConnectAndAuthenticate, use to determine if user owns DLC
int SteamServer::userHasLicenceForApp(int steamID, AppId_t appID){
	if(SteamGameServer() == NULL){
		return;
	}
	CSteamID userID = createSteamID(steamID);
	return SteamGameServer()->UserHasLicenseForApp(userID, appID);
}
// Ask if user is in specified group; results returned by GSUserGroupStatus_t.
bool SteamServer::requestUserGroupStatus(int steamID, int groupID){
	if(SteamGameServer() == NULL){
		return false;
	}
	CSteamID userID = createSteamID(steamID);
	CSteamID clanID = createSteamID(groupID);
	return SteamGameServer()->RequestUserGroupStatus(userID, clanID);
}
// NOTE: These are in GameSocketShare mode, where instead of ISteamGameServer creating sockets to talk to master server, it lets the game use its socket to forward messages back and forth.
//
// These are used when you've elected to multiplex the game server's UDP socket rather than having the master server updater use its own sockets.
bool SteamServer::handleIncomingPacket(const void *data, int cbData, uint32 ip, uint16 port){
	if(SteamGameServer() == NULL){
		return false;
	}
	return SteamGameServer()->HandleIncomingPacket(data, cbData, ip, port);
}
// AFTER calling HandleIncomingPacket for any packets that came in that frame, call this. This gets a packet that the master server updater needs to send out on UDP.
int SteamServer::getNextOutgoingPacket(void *out, int maxOut, uint32 *address, uint16 *port){
	if(SteamGameServer() == NULL){
		return 0;
	}
	return SteamGameServer()->GetNextOutgoingPacket(*out, maxOut, *address, *port);
}
// NOTE: These are heartbeat/advertisement functions.
//
// Call this as often as you like to tell the master server updater whether or not you want it to be active (default: off).
void SteamServer::enableHeartbeats(bool active){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->EnableHeartbeats(active);
}
// You usually don't need to modify this. Pass -1 to use the default value for iHeartbeatInterval. Some mods change this.
void SteamServer::setHeartbeatInterval(int interval){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetHeartbeatInterval(interval);
}
// Force a heartbeat to Steam at next opportunity.
void SteamServer::forceHeartbeat(){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->ForceHeartbeat();
}
// Associate this game server with this clan for the purposes of computing player compatibility.
void SteamServer::associateWithClan(uint64_t clanID){
	if(SteamGameServer() == NULL){
		return;
	}
	CSteamID groupID = createSteamID(clanID);
	SteamGameServer()->AssociateWithClan(groupID);
}
// Ask if any of the current players dont want to play with this new player - or vice versa.
void SteamServer::computeNewPlayerCompatibility(uint64_t steamID){
	if(SteamGameServer() == NULL){
		return;
	}
	CSteamID userID = createSteamID(steamID);
	SteamGameServer()->ComputeNewPlayerCompatibility(userID);
}

/////////////////////////////////////////////////
// SIGNALS //////////////////////////////////////
//
// Client has been approved to connect to this game server.
void SteamServer::_client_Approved(GSClientApprove_t* clientData){
	uint64_t steamID = clientData->m_SteamID.ConvertToUint64();
	uint64_t ownerID = clientData->m_OwnerSteamID.ConvertToUint64();
	emit_signal("client_approved", steamID, ownerID);
}
// Client has been denied to connection to this game server.
void SteamServer::_client_Denied(GSClientDeny_t* clientData){
	uint64_t steamID = clientData->m_SteamID.ConvertToUint64();
	int reason;
	// Convert reason.
	if(clientData->m_eDenyReason == k_EDenyInvalid){
		reason = DENY_INVALID;
	}
	else if(clientData->m_eDenyReason == k_EDenyInvalidVersion){
		reason = DENY_INVALID_VERSION;
	}
	else if(clientData->m_eDenyReason == k_EDenyGeneric){
		reason = DENY_GENERIC;
	}
	else if(clientData->m_eDenyReason == k_EDenyNotLoggedOn){
		reason = DENY_NOT_LOGGED_ON;
	}
	else if(clientData->m_eDenyReason == k_EDenyNoLicense){
		reason = DENY_NO_LICENSE;
	}
	else if(clientData->m_eDenyReason == k_EDenyCheater){
		reason = DENY_CHEATER;
	}
	else if(clientData->m_eDenyReason == k_EDenyLoggedInElseWhere){
		reason = DENY_LOGGED_IN_ELSEWHERE;
	}
	else if(clientData->m_eDenyReason == k_EDenyUnknownText){
		reason = DENY_UNKNOWN_TEXT;
	}
	else if(clientData->m_eDenyReason == k_EDenyIncompatibleAnticheat){
		reason = DENY_INCOMPAT_ANTICHEAT;
	}
	else if(clientData->m_eDenyReason == k_EDenyMemoryCorruption){
		reason = DENY_MEM_CORRUPTION;
	}
	else if(clientData->m_eDenyReason == k_EDenyIncompatibleSoftware){
		reason = DENY_INCOMPAT_SOFTWARE;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionLost){
		reason = DENY_STEAM_CONNECTION_LOST;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionError){
		reason = DENY_STEAM_CONNECTION_ERROR;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamResponseTimedOut){
		reason = DENY_STEAM_RESPONSE_TIMEOUT;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamValidationStalled){
		reason = DENY_STEAM_VALIDATION_STALLED;
	}
	else{
		reason = DENY_STEAM_OWNER_LEFT_GUEST;
	}
	String text = clientData->m_rgchOptionalText[128];
	emit_signal("client_denied", steamID, reason, text);
}
// Request the game server should kick the user.
void SteamServer::_client_Kick(GSClientKick_t* clientData){
	uint64_t steamID = clientData->m_SteamID.ConvertToUint64();
	int reason;
	// Convert reason.
	if(clientData->m_eDenyReason == k_EDenyInvalid){
		reason = DENY_INVALID;
	}
	else if(clientData->m_eDenyReason == k_EDenyInvalidVersion){
		reason = DENY_INVALID_VERSION;
	}
	else if(clientData->m_eDenyReason == k_EDenyGeneric){
		reason = DENY_GENERIC;
	}
	else if(clientData->m_eDenyReason == k_EDenyNotLoggedOn){
		reason = DENY_NOT_LOGGED_ON;
	}
	else if(clientData->m_eDenyReason == k_EDenyNoLicense){
		reason = DENY_NO_LICENSE;
	}
	else if(clientData->m_eDenyReason == k_EDenyCheater){
		reason = DENY_CHEATER;
	}
	else if(clientData->m_eDenyReason == k_EDenyLoggedInElseWhere){
		reason = DENY_LOGGED_IN_ELSEWHERE;
	}
	else if(clientData->m_eDenyReason == k_EDenyUnknownText){
		reason = DENY_UNKNOWN_TEXT;
	}
	else if(clientData->m_eDenyReason == k_EDenyIncompatibleAnticheat){
		reason = DENY_INCOMPAT_ANTICHEAT;
	}
	else if(clientData->m_eDenyReason == k_EDenyMemoryCorruption){
		reason = DENY_MEM_CORRUPTION;
	}
	else if(clientData->m_eDenyReason == k_EDenyIncompatibleSoftware){
		reason = DENY_INCOMPAT_SOFTWARE;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionLost){
		reason = DENY_STEAM_CONNECTION_LOST;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionError){
		reason = DENY_STEAM_CONNECTION_ERROR;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamResponseTimedOut){
		reason = DENY_STEAM_RESPONSE_TIMEOUT;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamValidationStalled){
		reason = DENY_STEAM_VALIDATION_STALLED;
	}
	else{
		reason = DENY_STEAM_OWNER_LEFT_GUEST;
	}
	emit_signal("client_kick", steamID, reason);
}
// Received when the game server requests to be displayed as secure (VAC protected).
// m_bSecure is true if the game server should display itself as secure to users, false otherwise.
void SteamServer::_policy_Response(GSPolicyResponse_t* policyData){
	uint8 secure = policyData->m_bSecure;
	emit_signal("server_secure", secure);
}
// Sent as a reply to RequestUserGroupStatus().
void SteamServer::_client_Group_Status(GSClientGroupStatus_t* clientData){
	uint64_t steamID = clientData->m_SteamIDUser.ConvertToUint64();
	uint64_t groupID = clientData->m_SteamIDGroup.ConvertToUint64();
	bool member = clientData->m_bMember;
	bool officer = clientData->m_bOfficer;
	emit_signal("group_status", steamID, groupID, member, officer);
}
// Sent as a reply to AssociateWithClan().
void SteamServer::_associate_Clan(AssociateWithClanResult_t* clanData){
	int result;
	if(clanData->m_eResult == k_EResultOK){
		result = RESULT_OK;
	}
	else{
		result = RESULT_FAIL;
	}
	emit_signal("clan_associate", result);
}
// Sent as a reply to ComputeNewPlayerCompatibility().
void SteamServer::_player_Compat(ComputeNewPlayerCompatibilityResult_t* playerData){
	int result;
	if(playerData->m_eResult == k_EResultNoConnection){
		result = RESULT_NO_CONNECT;
	}
	else if(playerData->m_eResult == k_EResultTimeout){
		result = RESULT_TIMEOUT;
	}
	else if(playerData->m_eResult == k_EResultFail){
		result = RESULT_FAIL;
	}
	else{
		result = RESULT_OK;
	}
	int playersDontLikeCandidate = playerData->m_cPlayersThatDontLikeCandidate;
	int playersCandidateDoesntLike = playerData->m_cPlayersThatCandidateDoesntLike;
	int clanPlayersDontLikeCandidate = playerData->m_cClanPlayersThatDontLikeCandidate;
	uint64_t steamID = playerData->m_SteamIDCandidate.ConvertToUint64();
	emit_signal("player_compatibility", result, playersDontLikeCandidate, playersCandidateDoesntLike, clanPlayersDontLikeCandidate, steamID);
}
/////////////////////////////////////////////////
// BIND METHODS /////////////////////////////////
//
void SteamServer::_bind_methods(){
	ClassDB::bind_method("serverInit", &SteamServer::serverInit);
	ClassDB::bind_method("serverReleaseCurrentThreadMemory", &SteamServer::serverReleaseCurrentThreadMemory);
	ClassDB::bind_method("serverShutdown", &SteamServer::serverShutdown);
	// Server Bind Methods //////////////////////
	ClassDB::bind_method("initGameServer", &SteamServer::initGameServer);
	ClassDB::bind_method("setProduct", &SteamServer::setProduct);
	ClassDB::bind_method("setGameDescription", &SteamServer::setGameDescription);
	ClassDB::bind_method("setModDir", &SteamServer::setModDir);
	ClassDB::bind_method("setDedicatedServer", &SteamServer::setDedicatedServer);
	ClassDB::bind_method("logOn", &SteamServer::logOn);
	ClassDB::bind_method("logOnAnonymous", &SteamServer::logOnAnonymous);
	ClassDB::bind_method("logOff", &SteamServer::logOff);
	ClassDB::bind_method("loggedOn", &SteamServer::loggedOn);
	ClassDB::bind_method("secure", &SteamServer::secure);
	ClassDB::bind_method("getSteamID", &SteamServer::getSteamID);
	ClassDB::bind_method("wasRestartRequested", &SteamServer::wasRestartRequested);
	ClassDB::bind_method("setMaxPlayerCount", &SteamServer::setMaxPlayerCount);
	ClassDB::bind_method("setBotPlayerCount", &SteamServer::setBotPlayerCount);
	ClassDB::bind_method("setServerName", &SteamServer::setServerName);
	ClassDB::bind_method("setMapName", &SteamServer::setMapName);
	ClassDB::bind_method("setPasswordProtected", &SteamServer::setPasswordProtected);
	ClassDB::bind_method("setSpectatorPort", &SteamServer::setSpectatorPort);
	ClassDB::bind_method("setSpectatorServerName", &SteamServer::setSpectatorServerName);
	ClassDB::bind_method("clearAllKeyValues", &SteamServer::clearAllKeyValues);
	ClassDB::bind_method("setKeyValue", &SteamServer::setKeyValue);
	ClassDB::bind_method("setGameTags", &SteamServer::setGameTags);
	ClassDB::bind_method("setGameData", &SteamServer::setGameData);
	ClassDB::bind_method("setRegion", &SteamServer::setRegion);
	ClassDB::bind_method("sendUserConnectAndAuthenticate", &SteamServer::sendUserConnectAndAuthenticate);
	ClassDB::bind_method("createUnauthenticatedUserConnection", &SteamServer::createUnauthenticatedUserConnection);
	ClassDB::bind_method("sendUserDisconnect", &SteamServer::sendUserDisconnect);
	ClassDB::bind_method("updateUserData", &SteamServer::updateUserData);
	ClassDB::bind_method("getAuthSessionTicket", &SteamServer::getAuthSessionTicket);
	ClassDB::bind_method("beginAuthSession", &SteamServer::beginAuthSession);
	ClassDB::bind_method("endAuthSession", &SteamServer::endAuthSession);
	ClassDB::bind_method("cancelAuthTicket", &SteamServer::cancelAuthTicket);
	ClassDB::bind_method("userHasLicenceForApp", &SteamServer::userHasLicenceForApp);
	ClassDB::bind_method("requestUserGroupStatus", &SteamServer::requestUserGroupStatus);
	ClassDB::bind_method("handleIncomingPacket", &SteamServer::handleIncomingPacket);
	ClassDB::bind_method("getNextOutgoingPacket", &SteamServer::getNextOutgoingPacket);
	ClassDB::bind_method("enableHeartbeats", &SteamServer::enableHeartbeats);
	ClassDB::bind_method("setHeartbeatInterval", &SteamServer::setHeartbeatInterval);
	ClassDB::bind_method("forceHeartbeat", &SteamServer::forceHeartbeat);
	// Signals //////////////////////////////////
	ADD_SIGNAL(MethodInfo("client_approved"), PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "ownerID"));
	ADD_SIGNAL(MethodInfo("client_denied"), PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "reason"));
	ADD_SIGNAL(MethodInfo("client_kick"), PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "reason"));
	ADD_SIGNAL(MethodInfo("client_achievement"), PropertyInfo(Variant::INT, "steamID"));
	ADD_SIGNAL(MethodInfo("server_secure"), PropertyInfo(Variant::INT, "secure"));
	ADD_SIGNAL(MethodInfo("gameplay_stats"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "rank"), PropertyInfo(Variant::INT, "totalConnects"), PropertyInfo(Variant::INT, "totalMinutesPlayed"));
	ADD_SIGNAL(MethodInfo("group_status"), PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "groupID"));
	ADD_SIGNAL(MethodInfo("server_reputation"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::INT, "bannedIP"), PropertyInfo(Variant::INT, "bannedPort"), PropertyInfo(Variant::INT, "bannedGameID"), PropertyInfo(Variant::INT, "banExpires"));
	ADD_SIGNAL(MethodInfo("clan_associate"), PropertyInfo(Variant::INT, "result"));
	ADD_SIGNAL(MethodInfo("player_compatibility"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "playersDontLikeCandidate"), PropertyInfo(Variant::INT, "playersCandidateDoesntLike"), PropertyInfo(Variant::INT, "clanPlayersDontLikeCandidate"), PropertyInfo(Variant::INT, "steamID"));
	// Result constants /////////////////////////
	BIND_CONSTANT(RESULT_OK);						// 1
	BIND_CONSTANT(RESULT_FAIL);						// 2
	BIND_CONSTANT(RESULT_NO_CONNECT);				// 3
	BIND_CONSTANT(RESULT_TIMEOUT);					// 16
	// Authorization mode constants /////////////
	BIND_CONSTANT(MODE_INVALID);					// 0
	BIND_CONSTANT(MODE_NO_AUTH);					// 1
	BIND_CONSTANT(MODE_AUTH);						// 2
	BIND_CONSTANT(MODE_AUTH_SECURE);				// 3
	// Authorization ticket constants ///////////
	BIND_CONSTANT(AUTH_OK);							// 0
	BIND_CONSTANT(AUTH_INVALID_TICKET);				// 1
	BIND_CONSTANT(AUTH_DUPLICATE);					// 2
	BIND_CONSTANT(AUTH_INVALID_VERSION);			// 3
	BIND_CONSTANT(AUTH_MISMATCH);					// 4
	BIND_CONSTANT(AUTH_EXPIRED_TICKET);				// 5
	// License constants ////////////////////////
	BIND_CONSTANT(LICENSE_HAS);						// 0
	BIND_CONSTANT(LICENSE_DOES_NOT);				// 1
	BIND_CONSTANT(LICENSE_NO_AUTH);					// 2
	// Client denial constants //////////////////
	BIND_CONSTANT(DENY_INVALID);					// 0
	BIND_CONSTANT(DENY_INVALID_VERSION);			// 1
	BIND_CONSTANT(DENY_GENERIC);					// 2
	BIND_CONSTANT(DENY_NOT_LOGGED_ON);				// 3
	BIND_CONSTANT(DENY_NO_LICENSE);					// 4
	BIND_CONSTANT(DENY_CHEATER);					// 5
	BIND_CONSTANT(DENY_LOGGED_IN_ELSEWHERE);		// 6
	BIND_CONSTANT(DENY_UNKNOWN_TEXT);				// 7
	BIND_CONSTANT(DENY_INCOMPAT_ANTICHEAT);			// 8
	BIND_CONSTANT(DENY_MEM_CORRUPTION);				// 9
	BIND_CONSTANT(DENY_INCOMPAT_SOFTWARE);			// 10
	BIND_CONSTANT(DENY_STEAM_CONNECTION_LOST);		// 11
	BIND_CONSTANT(DENY_STEAM_CONNECTION_ERROR);		// 12
	BIND_CONSTANT(DENY_STEAM_RESPONSE_TIMEOUT);		// 13
	BIND_CONSTANT(DENY_STEAM_VALIDATION_STALLED);	// 14
	BIND_CONSTANT(DENY_STEAM_OWNER_LEFT_GUEST);		// 15
}

SteamServer::~SteamServer(){
	if(isInitSuccess){
		SteamGameServer_Shutdown();
	}
	for(int i = 0; i < tickets.size(); i++){
		TicketData ticket = ticktes.get(i);
		memdelete_arr(ticket.buffer);
	}
	tickets.clear();
	singleton = NULL;
}