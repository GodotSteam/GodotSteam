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
	if(!SteamInternal_GameServer_Init(ip, steamPort, gamePort, queryPort, serverMode, versionString.utf8().get_data())){
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
	if(!SteamInternal_GameServer_Init(ip, steamPort, gamePort, queryPort, serverMode, versionString.utf8().get_data())){
		return false;
	}
	return true;
}
// Game product identifier; currently used by the master server for version checking purposes.
void SteamServer::setProduct(const String& product){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetProduct(product.utf8().get_data());
}
// Description of the game; required field and is displayed in the Steam server browser.
void SteamServer::setGameDescription(const String& description){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetGameDescription(description.utf8().get_data());
}
// If your game is a mod, pass the string that identifies it. Default is empty meaning the app is the original game.
void SteamServer::setModDir(const String& modDir){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetModDir(modDir.utf8().get_data());
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
	SteamGameServer()->LogOn(token.utf8().get_data());
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
	SteamGameServer()->SetServerName(name.utf8().get_data());
}
// Set name of map to report in server browser.
void SteamServer::setMapName(const String& map){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetMapName(map.utf8().get_data());
}
// Let people know if your server requires a password.
void SteamServer::setPasswordProtected(bool password){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetPasswordProtected(password);
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
	SteamGameServer()->SetSpectatorServerName(name.utf8().get_data());
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
	SteamGameServer()->SetKeyValue(key.utf8().get_data(), value.utf8().get_data());
}
// Set a string defining game tags for this server; optional. Allows users to filter in matchmaking/server browser.
void SteamServer::setGameTags(const String& tags){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetGameTags(tags.utf8().get_data());
}
// Set a string defining game data for this server; optional. Allows users to filter in matchmaking/server browser.
void SteamServer::setGameData(const String& data){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetGameData(data.utf8().get_data());
}
// Region identifier; optional. Default is empty meaning 'world'.
void SteamServer::setRegion(const String& region){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetRegion(region.utf8().get_data());
}
// NOTE: These functions are player list management / authentication.
//
// Handles receiving a new connection from a Steam user.  This call will ask the Steam
// servers to validate the users identity, app ownership, and VAC status.  If the Steam servers 
// are off-line, then it will validate the cached ticket itself which will validate app ownership 
// and identity.  The AuthBlob here should be acquired on the game client using SteamUser()->InitiateGameConnection()
// and must then be sent up to the game server for authentication.
bool SteamServer::sendUserConnectAndAuthenticate(uint32 ipClient, const void *authBlob, uint32 authBlobSize, uint32_t steamID){
	if(SteamGameServer() == NULL){
		return false;
	}
	CSteamID authID = createSteamID(steamID);
	return SteamGameServer()->SendUserConnectAndAuthenticate(ipClient, authBlob, authBlobSize, &authID);
}
// Creates a fake user (ie, a bot) which will be listed as playing on the server, but skips validation.
uint64_t SteamServer::createUnauthenticatedUserConnection(){
	if(SteamGameServer() == NULL){
		return 0;
	}
	CSteamID steamID = SteamGameServer()->CreateUnauthenticatedUserConnection();
	return steamID.ConvertToUint64();
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
bool SteamServer::updateUserData(int steamID, const String& name, uint32 score){
	if(SteamGameServer() == NULL){
		return false;
	}
	CSteamID updateID = createSteamID(steamID);
	return SteamGameServer()->BUpdateUserData(updateID, name.utf8().get_data(), score);
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
	return SteamGameServer()->BeginAuthSession(authTicket, cbAuthTicket, authID);
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
		return 0;
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
	return SteamGameServer()->GetNextOutgoingPacket(out, maxOut, address, port);
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
// Logging the game server onto Steam
void SteamServer::_server_Connect_Failure(SteamServerConnectFailure_t* serverData){
	int result = serverData->m_eResult;
	bool retrying = serverData->m_bStillRetrying;
	emit_signal("server_failure", result, retrying);
}
// Server has connected to the Steam back-end; serverData has no fields.
void SteamServer::_server_Connected(SteamServersConnected_t* serverData){
	emit_signal("server_connected");
}
// Called if the client has lost connection to the Steam servers. Real-time services will be disabled until a matching SteamServersConnected_t has been posted.
void SteamServer::_server_Disconnected(SteamServersDisconnected_t* serverData){
	int result = serverData->m_eResult;
	emit_signal("server_disconnected", result);
}
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
	emit_signal("client_denied", steamID, reason);
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
	ADD_SIGNAL(MethodInfo("server_failure", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::BOOL, "retrying")));
	ADD_SIGNAL(MethodInfo("server_connected"));
	ADD_SIGNAL(MethodInfo("server_disconnected", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("client_approved", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "ownerID")));
	ADD_SIGNAL(MethodInfo("client_denied", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("client_kick", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("server_secure", PropertyInfo(Variant::INT, "secure")));
	ADD_SIGNAL(MethodInfo("gameplay_stats", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "rank"), PropertyInfo(Variant::INT, "totalConnects"), PropertyInfo(Variant::INT, "totalMinutesPlayed")));
	ADD_SIGNAL(MethodInfo("group_status", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "groupID"), PropertyInfo(Variant::BOOL, "member"), PropertyInfo(Variant::BOOL, "officer")));
	ADD_SIGNAL(MethodInfo("clan_associate", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("player_compatibility", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "playersDontLikeCandidate"), PropertyInfo(Variant::INT, "playersCandidateDoesntLike"), PropertyInfo(Variant::INT, "clanPlayersDontLikeCandidate"), PropertyInfo(Variant::INT, "steamID")));
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
		TicketData ticket = tickets.get(i);
		memdelete_arr(ticket.buffer);
	}
	tickets.clear();
	singleton = NULL;
}