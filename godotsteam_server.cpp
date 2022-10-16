/////////////////////////////////////////////////
///// SILENCE STEAMWORKS WARNINGS
/////////////////////////////////////////////////
//
// Turn off MSVC-only warning about strcpy
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#pragma warning(disable:4828)
#endif


/////////////////////////////////////////////////
///// HEADER INCLUDES
/////////////////////////////////////////////////
//
// Include GodotSteam Server header
#include "godotsteam_server.h"

// Include some Godot headers
#include "core/io/ip_address.h"
#include "core/io/ip.h"

// Include some system headers
#include "fstream"
#include "vector"


/////////////////////////////////////////////////
///// DEFINING CONSTANTS
/////////////////////////////////////////////////
//
// Define Steam Server API constants
#define FLAG_ACTIVE 0x01
#define FLAG_DEDICATED 0x04
#define FLAG_LINUX 0x08
#define FLAG_NONE 0x00
#define FLAG_PASSWORDED 0x10
#define FLAG_PRIVATE 0x20
#define FLAG_SECURE 0x02
#define STEAM_BUFFER_SIZE 255
#define STEAM_LARGE_BUFFER_SIZE 8160

// Define Steam API constants
#define INVALID_BREAKPAD_HANDLE 0
#define GAME_EXTRA_INFO_MAX 64
#define AUTH_TICKET_INVALID 0
#define API_CALL_INVALID 0x0
#define APP_ID_INVALID 0x0
#define DEPOT_ID_INVALID 0x0
#define STEAM_ACCOUNT_ID_MASK 0xFFFFFFFF
#define STEAM_ACCOUNT_INSTANCE_MASK 0x000FFFFF
#define STEAM_USER_CONSOLE_INSTANCE 2
#define STEAM_USER_DESKTOP_INSTANCE 1
#define STEAM_USER_WEB_INSTANCE 4
#define QUERY_PORT_ERROR 0xFFFE
#define QUERY_PORT_NOT_INITIALIZED 0xFFFF
#define STEAM_BUFFER_SIZE 255
#define STEAM_LARGE_BUFFER_SIZE 8160

// Define HTTP constants
#define HTTPCOOKIE_INVALID_HANDLE 0
#define HTTPREQUEST_INVALID_HANDLE 0

// Define Inventory constants
#define INVENTORY_RESULT_INVALID -1
#define ITEM_INSTANCE_ID_INVALID 0

// Define Remote Storage constants
#define FILE_NAME_MAX 260
#define PUBLISHED_DOCUMENT_CHANGE_DESCRIPTION_MAX 8000
#define PUBLISHED_DOCUMENT_DESCRIPTION_MAX 8000
#define PUBLISHED_DOCUMENT_TITLE_MAX 128 + 1
#define PUBLISHED_FILE_URL_MAX 256
#define TAG_LIST_MAX 1024 + 1
#define PUBLISHED_FILE_ID_INVALID 0
#define PUBLISHED_FILE_UPDATE_HANDLE_INVALID 0
#define UGC_FILE_STREAM_HANDLE_INVALID 0
#define UGC_HANDLE_INVALID 0
#define ENUMERATE_PUBLISHED_FILES_MAX_RESULTS 50
#define MAX_CLOUD_FILE_CHUNK_SIZE 100 * 1024 * 1024

// Define UGC constants
#define NUM_UGC_RESULTS_PER_PAGE 50
#define DEVELOPER_METADATA_MAX 5000
#define UGC_QUERY_HANDLE_INVALID 0
#define UGC_UPDATE_HANDLE_INVALID 0


/////////////////////////////////////////////////
///// STEAM SINGLETON? STEAM SINGLETON
/////////////////////////////////////////////////
//
SteamServer* SteamServer::singleton = NULL;


/////////////////////////////////////////////////
///// STEAM OBJECT WITH CALLBACKS
/////////////////////////////////////////////////
//
SteamServer::SteamServer():
	// Apps callbacks ////////////////////////////
	callbackDLCInstalled(this, &SteamServer::dlc_installed),
	callbackFileDetailsResult(this, &SteamServer::file_details_result),
	callbackNewLaunchURLParameters(this, &SteamServer::new_launch_url_parameters),
	callbackTimedTrialStatus(this, &SteamServer::timed_trial_status),

	// Game Server callbacks ////////////////////
	callbackServerConnectFailure(this, &SteamServer::server_connect_failure),
	callbackServerConnected(this, &SteamServer::server_connected),
	callbackServerDisconnected(this, &SteamServer::server_disconnected),
	callbackClientApproved(this, &SteamServer::client_approved),
	callbackClientDenied(this, &SteamServer::client_denied),
	callbackClientKicked(this, &SteamServer::client_kick),
	callbackPolicyResponse(this, &SteamServer::policy_response),
	callbackClientGroupStatus(this, &SteamServer::client_group_status),
	callbackAssociateClan(this, &SteamServer::associate_clan),
	callbackPlayerCompat(this, &SteamServer::player_compat),

	// Game Server Stat callbacks ///////////////
	callbackStatsStored(this, &SteamServer::stats_stored),
	callbackStatsUnloaded(this, &SteamServer::stats_unloaded),

	// HTTP callbacks ///////////////////////////
	callbackHTTPRequestCompleted(this, &SteamServer::http_request_completed),
	callbackHTTPRequestDataReceived(this, &SteamServer::http_request_data_received),
	callbackHTTPRequestHeadersReceived(this, &SteamServer::http_request_headers_received),

	// Inventory callbacks //////////////////////
	callbackInventoryDefinitionUpdate(this, &SteamServer::inventory_definition_update),
	callbackInventoryFullUpdate(this, &SteamServer::inventory_full_update),
	callbackInventoryResultReady(this, &SteamServer::inventory_result_ready),

	// Networking callbacks /////////////////////
	callbackP2PSessionConnectFail(this, &SteamServer::p2p_session_connect_fail),
	callbackP2PSessionRequest(this, &SteamServer::p2p_session_request),

	// Networking Messages callbacks ////////////
	callbackNetworkMessagesSessionRequest(this, &SteamServer::network_messages_session_request),
	callbackNetworkMessagesSessionFailed(this, &SteamServer::network_messages_session_failed),

	// Networking Sockets callbacks /////////////
	callbackNetworkConnectionStatusChanged(this, &SteamServer::network_connection_status_changed),
	callbackNetworkAuthenticationStatus(this, &SteamServer::network_authentication_status),
	callbackNetworkingFakeIPResult(this, &SteamServer::fake_ip_result),

	// Networking Utils callbacks ///////////////
	callbackRelayNetworkStatus(this, &SteamServer::relay_network_status),
	
	// Remote Storage callbacks /////////////////
	callbackLocalFileChanged(this, &SteamServer::local_file_changed),

	// UGC callbacks ////////////////////////////
	callbackItemDownloaded(this, &SteamServer::item_downloaded),
	callbackItemInstalled(this, &SteamServer::item_installed),
	callbackUserSubscribedItemsListChanged(this, &SteamServer::user_subscribed_items_list_changed),

	// Utility callbacks ////////////////////////
	callbackGamepadTextInputDismissed(this, &SteamServer::gamepad_text_input_dismissed),
	callbackIPCountry(this, &SteamServer::ip_country),
	callbackLowPower(this, &SteamServer::low_power),
	callbackSteamAPICallCompleted(this, &SteamServer::steam_api_call_completed),
	callbackSteamShutdown(this, &SteamServer::steam_shutdown),
	callbackAppResumingFromSuspend(this, &SteamServer::app_resuming_from_suspend),
	callbackFloatingGamepadTextInputDismissed(this, &SteamServer::floating_gamepad_text_input_dismissed)
{
	is_init_success = false;
	singleton = this;
}


/////////////////////////////////////////////////
///// INTERNAL FUNCTIONS
/////////////////////////////////////////////////
//
// Get the Steam singleton, obviously
SteamServer* SteamServer::get_singleton(){
	return singleton;
}

// Creating a Steam ID for internal use
CSteamID SteamServer::createSteamID(uint64_t steam_id, int account_type){
	CSteamID converted_steam_id;
	if(account_type < 0 || account_type >= k_EAccountTypeMax){
		account_type = 1;
	}
	converted_steam_id.Set(steam_id, k_EUniversePublic, EAccountType(account_type));
	return converted_steam_id;
}


/////////////////////////////////////////////////
///// MAIN FUNCTIONS
/////////////////////////////////////////////////
//
// Initialize the server to Steam.
bool SteamServer::serverInit(const String& ip, uint16 game_port, uint16 query_port, int server_mode, const String& version_string){
	// Convert the server mode back
	EServerMode mode;
	if(server_mode == 1){
		mode = eServerModeNoAuthentication;
	}
	else if(server_mode == 2){
		mode = eServerModeAuthentication;
	}
	else{
		mode = eServerModeAuthenticationAndSecure;
	}
	// Resolve address and convert it from IP_Address string to uint32_t
	IP_Address address;
	if(ip.is_valid_ip_address()){
		address = ip;
	}
	else{
		address = IP::get_singleton()->resolve_hostname(ip, IP::TYPE_IPV4);
	}
	// Resolution failed
	if(!address.is_valid()){
		return false;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	if(!SteamGameServer_Init(ip4, game_port, query_port, mode, version_string.utf8().get_data())){
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
///// APPS
/////////////////////////////////////////////////
//
//! Returns metadata for a DLC by index.
Array SteamServer::getDLCDataByIndex(){
	if(SteamApps() == NULL){
		return Array();
	}
	int32 count = SteamApps()->GetDLCCount();
	Array dlcData;
	for(int i = 0; i < count; i++){
		AppId_t app_id = 0;
		bool available = false;
		char name[STEAM_BUFFER_SIZE];
		bool success = SteamApps()->BGetDLCDataByIndex(i, &app_id, &available, name, STEAM_BUFFER_SIZE);
		if(success){
			Dictionary dlc;
			dlc["id"] = app_id;
			dlc["available"] = available;
			dlc["name"] = name;
			dlcData.append(dlc);
		}
	}
	return dlcData;
}

//! Check if given application/game is installed, not necessarily owned.
bool SteamServer::isAppInstalled(uint32_t app_id){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsAppInstalled((AppId_t)app_id);
}

//! Checks whether the current App ID is for Cyber Cafes.
bool SteamServer::isCybercafe(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsCybercafe();
}

//! Checks if the user owns a specific DLC and if the DLC is installed
bool SteamServer::isDLCInstalled(uint32_t dlc_id){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsDlcInstalled((AppId_t)dlc_id);
}

//! Checks if the license owned by the user provides low violence depots.
bool SteamServer::isLowViolence(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsLowViolence();
}

//! Checks if the active user is subscribed to the current App ID.
bool SteamServer::isSubscribed(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribed();
}

//! Checks if the active user is subscribed to a specified AppId.
bool SteamServer::isSubscribedApp(uint32_t app_id){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedApp((AppId_t)app_id);
}

//! Checks if the active user is accessing the current app_id via a temporary Family Shared license owned by another user.
//! If you need to determine the steam_id of the permanent owner of the license, use getAppOwner.
bool SteamServer::isSubscribedFromFamilySharing(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedFromFamilySharing();
}

//! Checks if the user is subscribed to the current app through a free weekend.
//! This function will return false for users who have a retail or other type of license.
//! Suggested you contact Valve on how to package and secure your free weekend properly.
bool SteamServer::isSubscribedFromFreeWeekend(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedFromFreeWeekend();
}

//! Check if game is a timed trial with limited playtime.
Dictionary SteamServer::isTimedTrial(){
	Dictionary trial;
	if(SteamApps() != NULL){
		uint32 allowed = 0;
		uint32 played = 0;
		if(SteamApps()->BIsTimedTrial(&allowed, &played)){
			trial["seconds_allowed"] = allowed;
			trial["seconds_played"] = played;
		}
	}
	return trial;
}

//! Checks if the user has a VAC ban on their account.
bool SteamServer::isVACBanned(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsVACBanned();
}

//! Return the build ID for this app; will change based on backend updates.
int SteamServer::getAppBuildId(){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetAppBuildId();
}

//! Gets the install folder for a specific AppID.
String SteamServer::getAppInstallDir(uint32_t app_id){
	if(SteamApps() == NULL){
		return "";
	}
	char buffer[STEAM_BUFFER_SIZE];
	SteamApps()->GetAppInstallDir((AppId_t)app_id, buffer, STEAM_BUFFER_SIZE);
	String app_dir = buffer;
	return app_dir;
}

//! Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
uint64_t SteamServer::getAppOwner(){
	if(SteamApps() == NULL){
		return 0;
	}
	CSteamID converted_steam_id = SteamApps()->GetAppOwner();
	return converted_steam_id.ConvertToUint64();
}

//! Gets a comma separated list of the languages the current app supports.
String SteamServer::getAvailableGameLanguages(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetAvailableGameLanguages();
}

//! Checks if the user is running from a beta branch, and gets the name of the branch if they are.
String SteamServer::getCurrentBetaName(){
	String beta_name = "";
	if(SteamApps() != NULL){
		char name_string[STEAM_LARGE_BUFFER_SIZE];
		if (SteamApps()->GetCurrentBetaName(name_string, STEAM_LARGE_BUFFER_SIZE)) {
			beta_name = String(name_string);
		}
	}
	return beta_name;
}

//! Gets the current language that the user has set.
String SteamServer::getCurrentGameLanguage(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetCurrentGameLanguage();
}

//! Get the number of DLC the user owns for a parent application/game.
int SteamServer::getDLCCount(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->GetDLCCount();
}

//! Gets the download progress for optional DLC.
Dictionary SteamServer::getDLCDownloadProgress(uint32_t dlc_id){
	Dictionary progress;
	if(SteamApps() == NULL){
		progress["ret"] = false;
	}
	else{
		uint64 downloaded = 0;
		uint64 total = 0;
		// Get the progress
		progress["ret"] = SteamApps()->GetDlcDownloadProgress((AppId_t)dlc_id, &downloaded, &total);
		if(progress["ret"]){
			progress["downloaded"] = uint64_t(downloaded);
			progress["total"] = uint64_t(total);
		}
	}
	return progress;
}

//! Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).
uint32_t SteamServer::getEarliestPurchaseUnixTime(uint32_t app_id){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)app_id);
}

//! Asynchronously retrieves metadata details about a specific file in the depot manifest.
void SteamServer::getFileDetails(const String& filename){
	if(SteamApps() != NULL){
		SteamApps()->GetFileDetails(filename.utf8().get_data());
	}
}

//! Gets a list of all installed depots for a given App ID.
//! @param app_id App ID to check.
//! @return Array of the installed depots, returned in mount order.
Array SteamServer::getInstalledDepots(uint32_t app_id){
	if(SteamApps() == NULL){
		return Array();
	}
	Array installed_depots;
	DepotId_t *depots = new DepotId_t[32];
	uint32 installed = SteamApps()->GetInstalledDepots((AppId_t)app_id, depots, 32);
	for(uint32 i = 0; i < installed; i++){
		installed_depots.append(depots[i]);
	}
	delete[] depots;
	return installed_depots;
}

//! Gets the command line if the game was launched via Steam URL, e.g. steam://run/<appid>//<command line>/. This method is preferable to launching with a command line via the operating system, which can be a security risk. In order for rich presence joins to go through this and not be placed on the OS command line, you must enable "Use launch command line" from the Installation > General page on your app.
String SteamServer::getLaunchCommandLine(){
	if(SteamApps() == NULL){
		return "";
	}
	char commands[STEAM_BUFFER_SIZE];
	SteamApps()->GetLaunchCommandLine(commands, STEAM_BUFFER_SIZE);
	String command_line;
	command_line += commands;
	return command_line;
}

//! Gets the associated launch parameter if the game is run via steam://run/<appid>/?param1=value1;param2=value2;param3=value3 etc.
String SteamServer::getLaunchQueryParam(const String& key){
	if(SteamApps() == NULL){
		return "";
	}
	return SteamApps()->GetLaunchQueryParam(key.utf8().get_data());
}

//! Allows you to install an optional DLC.
void SteamServer::installDLC(uint32_t dlc_id){
	if(SteamApps() != NULL){
		SteamApps()->InstallDLC((AppId_t)dlc_id);
	}
}

//! Allows you to force verify game content on next launch.
bool SteamServer::markContentCorrupt(bool missing_files_only){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->MarkContentCorrupt(missing_files_only);
}

//
bool SteamServer::setDLCContext(uint32_t app_id){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->SetDlcContext((AppId_t)app_id);
}


//! Allows you to uninstall an optional DLC.
void SteamServer::uninstallDLC(uint32_t dlc_id){
	if(SteamApps() != NULL){
		SteamApps()->UninstallDLC((AppId_t)dlc_id);
	}
}


/////////////////////////////////////////////////
///// GAME SERVER FUNCTIONS
/////////////////////////////////////////////////
//
// NOTE: The following, if set, must be set before calling LogOn; they may not be changed after.
//
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
void SteamServer::setModDir(const String& mod_directory){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetModDir(mod_directory.utf8().get_data());
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
// Begin process to login to a persistent game server account. You need to register for callbacks to determine the result of this operation.
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
void SteamServer::setMaxPlayerCount(int players_max){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetMaxPlayerCount(players_max);
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
void SteamServer::setPasswordProtected(bool password_protected){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetPasswordProtected(password_protected);
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
// Retrieve ticket to be sent to the entity who wishes to authenticate you (using BeginAuthSession API).
Dictionary SteamServer::getAuthSessionTicket(){
	// Create the dictionary to use
	Dictionary auth_ticket;
	if(SteamGameServer() != NULL){
		uint32_t ticket_size = 1024;
		PoolByteArray buffer;
		buffer.resize(ticket_size);
		uint32_t id = SteamGameServer()->GetAuthSessionTicket(buffer.write().ptr(), ticket_size, &ticket_size);
		// Add this data to the dictionary
		auth_ticket["id"] = id;
		auth_ticket["buffer"] = buffer;
		auth_ticket["size"] = ticket_size;
	}
	return auth_ticket;
}

// Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused.
uint32 SteamServer::beginAuthSession(PoolByteArray ticket, int ticket_size, uint64_t steam_id){
	if(SteamGameServer() == NULL){
		return -1;
	}
	CSteamID authSteamID = createSteamID(steam_id);
	return SteamGameServer()->BeginAuthSession(ticket.read().ptr(), ticket_size, authSteamID);
}

// Stop tracking started by beginAuthSession; called when no longer playing game with this entity;
void SteamServer::endAuthSession(uint64_t steam_id){
	if(SteamGameServer() != NULL){
		CSteamID authSteamID = createSteamID(steam_id);
		SteamGameServer()->EndAuthSession(authSteamID);
	}
}

// Cancel auth ticket from getAuthSessionTicket; called when no longer playing game with the entity you gave the ticket to.
void SteamServer::cancelAuthTicket(uint32_t auth_ticket){
	if(SteamGameServer() != NULL){
		SteamGameServer()->CancelAuthTicket(auth_ticket);
	}
}

// After receiving a user's authentication data, and passing it to sendUserConnectAndAuthenticate, use to determine if user owns DLC
int SteamServer::userHasLicenceForApp(uint64_t steam_id, uint32 app_id){
	if(SteamGameServer() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steam_id;
	return SteamGameServer()->UserHasLicenseForApp(userID, (AppId_t)app_id);
}

// Ask if user is in specified group; results returned by GSUserGroupStatus_t.
bool SteamServer::requestUserGroupStatus(uint64_t steam_id, int group_id){
	if(SteamGameServer() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steam_id;
	CSteamID clan_id = (uint64)group_id;
	return SteamGameServer()->RequestUserGroupStatus(userID, clan_id);
}

// NOTE: These are in GameSocketShare mode, where instead of ISteamGameServer creating sockets to talk to master server, it lets the game use its socket to forward messages back and forth.
//
// These are used when you've elected to multiplex the game server's UDP socket rather than having the master server updater use its own sockets.
Dictionary SteamServer::handleIncomingPacket(int packet, const String& ip, uint16 port){
	Dictionary result;
	if(SteamGameServer() == NULL){
		return result;
	}
	PoolByteArray data;
	data.resize(packet);
	// Resolve address and convert it from IP_Address string to uint32_t
	IP_Address address;
	if(ip.is_valid_ip_address()){
		address = ip;
	}
	else{
		address = IP::get_singleton()->resolve_hostname(ip, IP::TYPE_IPV4);
	}
	// Resolution failed
	if(!address.is_valid()){
		return result;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	if(SteamGameServer()->HandleIncomingPacket(data.write().ptr(), packet, ip4, port)){
		result["data"] = data;
	}
	return result;
}

// AFTER calling HandleIncomingPacket for any packets that came in that frame, call this. This gets a packet that the master server updater needs to send out on UDP. Returns 0 if there are no more packets.
Dictionary SteamServer::getNextOutgoingPacket(){
	Dictionary packet;
	if(SteamGameServer() == NULL){
		return packet;
	}
	PoolByteArray out;
	int maxOut = 16 * 1024;
	uint32 address;
	uint16 port;
	// Retrieve the packet information
	int length = SteamGameServer()->GetNextOutgoingPacket(&out, maxOut, &address, &port);
	// Place packet information in dictionary and return it
	packet["length"] = length;
	packet["out"] = out;
	packet["address"] = address;
	packet["port"] = port;
	return packet;
}

// Gets the public IP of the server according to Steam.
Dictionary SteamServer::getPublicIP(){
	Dictionary public_ip;
	if(SteamGameServer() != NULL){
		SteamIPAddress_t this_public_ip = SteamGameServer()->GetPublicIP();
		// Populate the dictionary for returning
		public_ip["ipv4"] = this_public_ip.m_unIPv4;
		public_ip["ipv6"] = this_public_ip.m_rgubIPv6;
		public_ip["type"] = this_public_ip.m_eType;
	}
	return public_ip;
}

// NOTE: These are heartbeat/advertisement functions.
//
// Call this as often as you like to tell the master server updater whether or not you want it to be active (default: off).
void SteamServer::setAdvertiseServerActive(bool active){
	if(SteamGameServer() == NULL){
		return;
	}
	SteamGameServer()->SetAdvertiseServerActive(active);
}

// Associate this game server with this clan for the purposes of computing player compatibility.
void SteamServer::associateWithClan(uint64_t clan_id){
	if(SteamGameServer() == NULL){
		return;
	}
	CSteamID group_id = (uint64)clan_id;
	SteamGameServer()->AssociateWithClan(group_id);
}

// Ask if any of the current players dont want to play with this new player - or vice versa.
void SteamServer::computeNewPlayerCompatibility(uint64_t steam_id){
	if(SteamGameServer() == NULL){
		return;
	}
	CSteamID userID = (uint64)steam_id;
	SteamGameServer()->ComputeNewPlayerCompatibility(userID);
}


/////////////////////////////////////////////////
///// GAME SERVER STATS
/////////////////////////////////////////////////
//
// Resets the unlock status of an achievement for the specified user.
bool SteamServer::clearUserAchievement(uint64_t steam_id, const String& name){
	if(SteamGameServerStats() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steam_id;
	return SteamGameServerStats()->ClearUserAchievement(userID, name.utf8().get_data());
}

// Gets the unlock status of the Achievement.
Dictionary SteamServer::getUserAchievement(uint64_t steam_id, const String& name){
	// Set dictionary to fill in
	Dictionary achievement;
	if(SteamGameServerStats() == NULL){
		return achievement;
	}
	CSteamID user_id = (uint64)steam_id;
	bool unlocked = false;
	bool result = SteamGameServerStats()->GetUserAchievement(user_id, name.utf8().get_data(), &unlocked);
	// Populate the dictionary
	achievement["result"] = result;
	achievement["user"] = steam_id;
	achievement["name"] = name;
	achievement["unlocked"] = unlocked;
	return achievement;
}

// Gets the current value of the a stat for the specified user.
uint32_t SteamServer::getUserStatInt(uint64_t steam_id, const String& name){
	if(SteamGameServerStats() != NULL){
		CSteamID userID = (uint64)steam_id;
		int32 value = 0;
		if(SteamGameServerStats()->GetUserStat(userID, name.utf8().get_data(), &value)){
			return value;
		}
	}
	return 0;
}

// Gets the current value of the a stat for the specified user.
float SteamServer::getUserStatFloat(uint64_t steam_id, const String& name){
	if(SteamGameServerStats() != NULL){
		CSteamID userID = (uint64)steam_id;
		float value = 0.0;
		if(SteamGameServerStats()->GetUserStat(userID, name.utf8().get_data(), &value)){
			return value;
		}
	}
	return 0.0;
}

// Asynchronously downloads stats and achievements for the specified user from the server.
void SteamServer::requestUserStats(uint64_t steam_id){
	if(SteamGameServerStats() != NULL){
		CSteamID userID = (uint64)steam_id;
		SteamAPICall_t api_call = SteamGameServerStats()->RequestUserStats(userID);
		callResultStatReceived.Set(api_call, this, &SteamServer::stats_received);
	}
}

// Unlocks an achievement for the specified user.
bool SteamServer::setUserAchievement(uint64_t steam_id, const String& name){
	if(SteamGameServerStats() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steam_id;
	return SteamGameServerStats()->SetUserAchievement(userID, name.utf8().get_data());
}

// Sets / updates the value of a given stat for the specified user.
bool SteamServer::setUserStatInt(uint64_t steam_id, const String& name, int32 stat){
	if(SteamGameServerStats() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steam_id;
	return SteamGameServerStats()->SetUserStat(userID, name.utf8().get_data(), stat);
}

// Sets / updates the value of a given stat for the specified user.
bool SteamServer::setUserStatFloat(uint64_t steam_id, const String& name, float stat){
	if(SteamGameServerStats() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steam_id;
	return SteamGameServerStats()->SetUserStat(userID, name.utf8().get_data(), stat);
}

// Send the changed stats and achievements data to the server for permanent storage for the specified user.
void SteamServer::storeUserStats(uint64_t steam_id){
	if(SteamGameServerStats() != NULL){
		CSteamID userID = (uint64)steam_id;
		SteamGameServerStats()->StoreUserStats(userID);
	}
}

// Updates an AVGRATE stat with new values for the specified user.
bool SteamServer::updateUserAvgRateStat(uint64_t steam_id, const String& name, float this_session, double session_length){
	if(SteamGameServerStats() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steam_id;
	return SteamGameServerStats()->UpdateUserAvgRateStat(userID, name.utf8().get_data(), this_session, session_length);
}


/////////////////////////////////////////////////
///// HTTP
/////////////////////////////////////////////////
//
//! Creates a cookie container to store cookies during the lifetime of the process. This API is just for during process lifetime, after steam restarts no cookies are persisted and you have no way to access the cookie container across repeat executions of your process.
uint32_t SteamServer::createCookieContainer(bool allow_responses_to_modify){
	if(SteamHTTP() == NULL){
		return 0;
	}
	return SteamHTTP()->CreateCookieContainer(allow_responses_to_modify);
}

//! Initializes a new HTTP request.
uint32_t SteamServer::createHTTPRequest(int request_method, const String& absolute_url){
	if(SteamHTTP() != NULL){
		return SteamHTTP()->CreateHTTPRequest((EHTTPMethod)request_method, absolute_url.utf8().get_data());
	}
	return HTTPREQUEST_INVALID_HANDLE;
}

//! Defers a request which has already been sent by moving it at the back of the queue.
bool SteamServer::deferHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->DeferHTTPRequest(request_handle);
}

//! Gets progress on downloading the body for the request.
float SteamServer::getHTTPDownloadProgressPct(uint32 request_handle){
	float percent_one = 0.0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPDownloadProgressPct(request_handle, &percent_one);
	}
	return percent_one;
}

//! Check if the reason the request failed was because we timed it out (rather than some harder failure).
bool SteamServer::getHTTPRequestWasTimedOut(uint32 request_handle){
	bool was_timed_out = false;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPRequestWasTimedOut(request_handle, &was_timed_out);
	}
	return was_timed_out;
}

//! Gets the body data from an HTTP response.
PoolByteArray SteamServer::getHTTPResponseBodyData(uint32 request_handle, uint32 buffer_size){
	PoolByteArray body_data;
	body_data.resize(buffer_size);
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodyData(request_handle, body_data.write().ptr(), buffer_size);
	}
	return body_data;
}

//! Gets the size of the body data from an HTTP response.
uint32 SteamServer::getHTTPResponseBodySize(uint32 request_handle){
	uint32 body_size = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodySize(request_handle, &body_size);
	}
	return body_size;
}

//! Checks if a header is present in an HTTP response and returns its size.
uint32 SteamServer::getHTTPResponseHeaderSize(uint32 request_handle, const String& header_name){
	uint32 response_header_size = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderSize(request_handle, header_name.utf8().get_data(), &response_header_size);
	}
	return response_header_size;
}

//! Gets a header value from an HTTP response.
uint8 SteamServer::getHTTPResponseHeaderValue(uint32 request_handle, const String& header_name, uint32 buffer_size){
	uint8 value_buffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderValue(request_handle, header_name.utf8().get_data(), &value_buffer, buffer_size);
	}
	return value_buffer;
}

//! Gets the body data from a streaming HTTP response.
uint8 SteamServer::getHTTPStreamingResponseBodyData(uint32 request_handle, uint32 offset, uint32 buffer_size){
	uint8 body_data_buffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPStreamingResponseBodyData(request_handle, offset, &body_data_buffer, buffer_size);
	}
	return body_data_buffer;
}

//! Prioritizes a request which has already been sent by moving it at the front of the queue.
bool SteamServer::prioritizeHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->PrioritizeHTTPRequest(request_handle);
}

//! Releases a cookie container, freeing the memory allocated within Steam.
bool SteamServer::releaseCookieContainer(uint32 cookie_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseCookieContainer(cookie_handle);
}

//! Releases an HTTP request handle, freeing the memory allocated within Steam.
bool SteamServer::releaseHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseHTTPRequest(request_handle);
}

//! Sends an HTTP request.
bool SteamServer::sendHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t call_handle;
	return SteamHTTP()->SendHTTPRequest(request_handle, &call_handle);
}

//! Sends an HTTP request and streams the response back in chunks.
bool SteamServer::sendHTTPRequestAndStreamResponse(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t call_handle;
	return SteamHTTP()->SendHTTPRequestAndStreamResponse(request_handle, &call_handle);
}

//! Adds a cookie to the specified cookie container that will be used with future requests.
bool SteamServer::setHTTPCookie(uint32 cookie_handle, const String& host, const String& url, const String& cookie){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetCookie(cookie_handle, host.utf8().get_data(), url.utf8().get_data(), cookie.utf8().get_data());
}

//! Set an absolute timeout in milliseconds for the HTTP request. This is the total time timeout which is different than the network activity timeout which is set with SetHTTPRequestNetworkActivityTimeout which can bump everytime we get more data.
bool SteamServer::setHTTPRequestAbsoluteTimeoutMS(uint32 request_handle, uint32 milliseconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestAbsoluteTimeoutMS(request_handle, milliseconds);
}

//! Set a context value for the request, which will be returned in the HTTPRequestCompleted_t callback after sending the request. This is just so the caller can easily keep track of which callbacks go with which request data. Must be called before sending the request.
bool SteamServer::setHTTPRequestContextValue(uint32 request_handle, uint64_t context_value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestContextValue(request_handle, context_value);
}

//! Associates a cookie container to use for an HTTP request.
bool SteamServer::setHTTPRequestCookieContainer(uint32 request_handle, uint32 cookie_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestCookieContainer(request_handle, cookie_handle);
}

//! Set a GET or POST parameter value on the HTTP request. Must be called prior to sending the request.
bool SteamServer::setHTTPRequestGetOrPostParameter(uint32 request_handle, const String& name, const String& value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestGetOrPostParameter(request_handle, name.utf8().get_data(), value.utf8().get_data());
}

//! Set a request header value for the HTTP request. Must be called before sending the request.
bool SteamServer::setHTTPRequestHeaderValue(uint32 request_handle, const String& header_name, const String& header_value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestHeaderValue(request_handle, header_name.utf8().get_data(), header_value.utf8().get_data());
}

//! Set the timeout in seconds for the HTTP request.
bool SteamServer::setHTTPRequestNetworkActivityTimeout(uint32 request_handle, uint32 timeout_seconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestNetworkActivityTimeout(request_handle, timeout_seconds);
}

//! Sets the body for an HTTP Post request.
uint8 SteamServer::setHTTPRequestRawPostBody(uint32 request_handle, const String& content_type, uint32 body_length){
	uint8 body = 0;
	if(SteamHTTP()){
		SteamHTTP()->SetHTTPRequestRawPostBody(request_handle, content_type.utf8().get_data(), &body, body_length);
	}
	return body;
}

//! Sets that the HTTPS request should require verified SSL certificate via machines certificate trust store. This currently only works Windows and macOS.
bool SteamServer::setHTTPRequestRequiresVerifiedCertificate(uint32 request_handle, bool require_verified_certificate){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestRequiresVerifiedCertificate(request_handle, require_verified_certificate);
}

//! Set additional user agent info for a request.
bool SteamServer::setHTTPRequestUserAgentInfo(uint32 request_handle, const String& user_agent_info){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestUserAgentInfo(request_handle, user_agent_info.utf8().get_data());
}


/////////////////////////////////////////////////
///// INVENTORY
/////////////////////////////////////////////////
//
//! When dealing with any inventory handles, you should call CheckResultSteamID on the result handle when it completes to verify that a remote player is not pretending to have a different user's inventory.
//! Also, you must call DestroyResult on the provided inventory result when you are done with it.
//!
//! Grant a specific one-time promotional item to the current user.
int32 SteamServer::addPromoItem(uint32 item){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->AddPromoItem(&new_inventory_handle, item)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Grant a specific one-time promotional items to the current user.
int32 SteamServer::addPromoItems(PoolIntArray items){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		int count = items.size();
		SteamItemDef_t *new_items = new SteamItemDef_t[items.size()];
		for(int i = 0; i < count; i++){
			new_items[i] = items[i];
		}
		if(SteamInventory()->AddPromoItems(&new_inventory_handle, new_items, count)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
		delete[] new_items;
	}
	return new_inventory_handle;
}

//! Checks whether an inventory result handle belongs to the specified Steam ID.
bool SteamServer::checkResultSteamID(uint64_t steam_id_expected, int32 this_inventory_handle){
	if(SteamInventory() == NULL){
		return false;
	}
	CSteamID steam_id = (uint64)steam_id_expected;
	// If no inventory handle is passed, use internal one
	if(this_inventory_handle == 0){
		this_inventory_handle = inventory_handle;
	}
	return SteamInventory()->CheckResultSteamID((SteamInventoryResult_t)this_inventory_handle, steam_id);
}

//! Consumes items from a user's inventory. If the quantity of the given item goes to zero, it is permanently removed.
int32 SteamServer::consumeItem(uint64_t item_consume, uint32 quantity){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
	 	if(SteamInventory()->ConsumeItem(&new_inventory_handle, (SteamItemInstanceID_t)item_consume, quantity)){
	 		// Update the internally stored handle
			inventory_handle = new_inventory_handle;
	 	}
	}
	return new_inventory_handle;
}

//! Deserializes a result set and verifies the signature bytes.
int32 SteamServer::deserializeResult(PoolByteArray buffer){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->DeserializeResult(&new_inventory_handle, &buffer, buffer.size(), false)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Destroys a result handle and frees all associated memory.
void SteamServer::destroyResult(int this_inventory_handle){
	if(SteamInventory() != NULL){
		// If no inventory handle is passed, use internal one
		if(this_inventory_handle == 0){
			this_inventory_handle = inventory_handle;
		}	
		SteamInventory()->DestroyResult((SteamInventoryResult_t)this_inventory_handle);
	}
}

//! Grant one item in exchange for a set of other items.
int32 SteamServer::exchangeItems(const PoolIntArray output_items, const uint32 output_quantity, const uint64_t input_items, const uint32 input_quantity){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->ExchangeItems(&new_inventory_handle, output_items.read().ptr(), &output_quantity, 1, (const uint64 *)input_items, &input_quantity, 1)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Grants specific items to the current user, for developers only.
int32 SteamServer::generateItems(const PoolIntArray items, const uint32 quantity){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->GenerateItems(&new_inventory_handle, items.read().ptr(), &quantity, items.size())){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Start retrieving all items in the current users inventory.
int32 SteamServer::getAllItems(){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->GetAllItems(&new_inventory_handle)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Gets a string property from the specified item definition.  Gets a property value for a specific item definition.
String SteamServer::getItemDefinitionProperty(uint32 definition, const String& name){
	if(SteamInventory() == NULL){
		return "";
	}
	uint32 buffer_size = STEAM_BUFFER_SIZE;
	char *buffer = new char[buffer_size];
	SteamInventory()->GetItemDefinitionProperty(definition, name.utf8().get_data(), buffer, &buffer_size);
	String property = String::utf8(buffer, buffer_size);
	delete[] buffer;
	return property;
}

//! Gets the state of a subset of the current user's inventory.
int32 SteamServer::getItemsByID(const uint64_t id_array, uint32 count){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->GetItemsByID(&new_inventory_handle, (const uint64 *)id_array, count)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! After a successful call to RequestPrices, you can call this method to get the pricing for a specific item definition.
uint64_t SteamServer::getItemPrice(uint32 definition){
	if(SteamInventory() == NULL){
		return 0;
	}
	uint64 price = 0;
	uint64 basePrice = 0;
	SteamInventory()->GetItemPrice(definition, &price, &basePrice);
	return price;
}

//! After a successful call to RequestPrices, you can call this method to get all the pricing for applicable item definitions. Use the result of GetNumItemsWithPrices as the the size of the arrays that you pass in.
Array SteamServer::getItemsWithPrices(uint32 length){
	if(SteamInventory() == NULL){
		return Array();
	}
	// Create the return array
	Array price_array;
	// Create a temporary array
	SteamItemDef_t *ids = new SteamItemDef_t[length];
	uint64 *prices = new uint64[length];
	uint64 *base_prices = new uint64[length];
	if(SteamInventory()->GetItemsWithPrices(ids, prices, base_prices, length)){
		for(uint32 i = 0; i < length; i++){
			Dictionary price_group;
			price_group["item"] = ids[i];
			price_group["price"] = (uint64_t)prices[i];
			price_group["base_prices"] = (uint64_t)base_prices[i];
			price_array.append(price_group);
		}
	}
	delete[] ids;
	delete[] prices;
	delete[] base_prices;
	return price_array;
}

//! After a successful call to RequestPrices, this will return the number of item definitions with valid pricing.
uint32 SteamServer::getNumItemsWithPrices(){
	if(SteamInventory() == NULL){
		return 0;
	}
	return SteamInventory()->GetNumItemsWithPrices();
}

//! Gets the dynamic properties from an item in an inventory result set.
String SteamServer::getResultItemProperty(uint32 index, const String& name, int32 this_inventory_handle){
	if(SteamInventory() != NULL){
		// Set up variables to fill
		uint32 buffer_size = 256;
		char *value = new char[buffer_size];
		// If no inventory handle is passed, use internal one
		if(this_inventory_handle == 0){
			this_inventory_handle = inventory_handle;
		}
		SteamInventory()->GetResultItemProperty((SteamInventoryResult_t)this_inventory_handle, index, name.utf8().get_data(), (char*)value, &buffer_size);
		String property = value;
		delete[] value;
		return property;
	}
	return "";
}

//! Get the items associated with an inventory result handle.
Array SteamServer::getResultItems(int32 this_inventory_handle){
	if(SteamInventory() == NULL){
		return Array();
	}
	// Set up return array
	Array items;
	uint32 size = 0;
	if(SteamInventory()->GetResultItems((SteamInventoryResult_t)this_inventory_handle, NULL, &size)){
		items.resize(size);
		SteamItemDetails_t *item_array = new SteamItemDetails_t[size];
		// If no inventory handle is passed, use internal one
		if(this_inventory_handle == 0){
			this_inventory_handle = inventory_handle;
		}
		if(SteamInventory()->GetResultItems((SteamInventoryResult_t)this_inventory_handle, item_array, &size)){
			for(uint32 i = 0; i < size; i++){
				items.push_back((uint64_t)item_array[i].m_itemId);
			}
		}
		delete[] item_array;
	}
	return items;
}

//! Find out the status of an asynchronous inventory result handle.
String SteamServer::getResultStatus(int32 this_inventory_handle){
	if(SteamInventory() == NULL){
		return "";
	}
	// If no inventory handle is passed, use internal one
	if(this_inventory_handle == 0){
		this_inventory_handle = inventory_handle;
	}
	int result = SteamInventory()->GetResultStatus((SteamInventoryResult_t)this_inventory_handle);
	// Parse result
	if(result == k_EResultPending){
		return "Still in progress.";
	}
	else if(result == k_EResultOK){
		return "Finished successfully.";
	}
	else if(result == k_EResultExpired){
		return "Finished but may be out-of-date.";
	}
	else if(result == k_EResultInvalidParam){
		return "ERROR: invalid API call parameters.";
	}
	else if(result == k_EResultServiceUnavailable){
		return "ERROR: server temporarily down; retry later.";
	}
	else if(result == k_EResultLimitExceeded){
		return "ERROR: operation would exceed per-user inventory limits.";
	}
	else{
		return "ERROR: generic / unknown.";
	}
}

//! Gets the server time at which the result was generated.
uint32 SteamServer::getResultTimestamp(int32 this_inventory_handle){
	if(SteamInventory() == NULL){
		return 0;
	}
	// If no inventory handle is passed, use internal one
	if(this_inventory_handle == 0){
		this_inventory_handle = inventory_handle;
	}
	return SteamInventory()->GetResultTimestamp((SteamInventoryResult_t)this_inventory_handle);
}

//! Grant all potential one-time promotional items to the current user.
int32 SteamServer::grantPromoItems(){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->GrantPromoItems(&new_inventory_handle)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Triggers an asynchronous load and refresh of item definitions.
bool SteamServer::loadItemDefinitions(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->LoadItemDefinitions();
}

//! Request the list of "eligible" promo items that can be manually granted to the given user.
void SteamServer::requestEligiblePromoItemDefinitionsIDs(uint64_t steam_id){
	if(SteamInventory() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamAPICall_t api_call = SteamInventory()->RequestEligiblePromoItemDefinitionsIDs(user_id);
		callResultEligiblePromoItemDefIDs.Set(api_call, this, &SteamServer::inventory_eligible_promo_item);
	}
}

//! Request prices for all item definitions that can be purchased in the user's local currency. A SteamInventoryRequestPricesResult_t call result will be returned with the user's local currency code. After that, you can call GetNumItemsWithPrices and GetItemsWithPrices to get prices for all the known item definitions, or GetItemPrice for a specific item definition.
void SteamServer::requestPrices(){
	if(SteamInventory() != NULL){
		SteamAPICall_t api_call = SteamInventory()->RequestPrices();
		callResultRequestPrices.Set(api_call, this, &SteamServer::inventory_request_prices_result);
	}
}

//! Serialized result sets contain a short signature which can't be forged or replayed across different game sessions.
String SteamServer::serializeResult(int32 this_inventory_handle){
	String result_serialized;
	if(SteamInventory() != NULL){
		// If no inventory handle is passed, use internal one
		if(this_inventory_handle == 0){
			this_inventory_handle = inventory_handle;
		}
		// Set up return array
		uint32 buffer_size = STEAM_BUFFER_SIZE;
		char *buffer = new char[buffer_size];
		if(SteamInventory()->SerializeResult((SteamInventoryResult_t)this_inventory_handle, buffer, &buffer_size)){
			result_serialized = String::utf8(buffer, buffer_size);
		}
		delete[] buffer;
	}
	return result_serialized;
}

//! Starts the purchase process for the user, given a "shopping cart" of item definitions that the user would like to buy. The user will be prompted in the Steam Overlay to complete the purchase in their local currency, funding their Steam Wallet if necessary, etc.
void SteamServer::startPurchase(const PoolIntArray items, const uint32 quantity){
	if(SteamInventory() != NULL){
		SteamAPICall_t api_call = SteamInventory()->StartPurchase(items.read().ptr(), &quantity, items.size());
		callResultStartPurchase.Set(api_call, this, &SteamServer::inventory_start_purchase_result);
	}
}

//! Transfer items between stacks within a user's inventory.
int32 SteamServer::transferItemQuantity(uint64_t item_id, uint32 quantity, uint64_t item_destination, bool split){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(split){
			if(SteamInventory()->TransferItemQuantity(&new_inventory_handle, (SteamItemInstanceID_t)item_id, quantity, k_SteamItemInstanceIDInvalid)){
				// Update the internally stored handle
				inventory_handle = new_inventory_handle;
			}
		}
		else{
			if(SteamInventory()->TransferItemQuantity(&new_inventory_handle, (SteamItemInstanceID_t)item_id, quantity, (SteamItemInstanceID_t)item_destination)){
				// Update the internally stored handle
				inventory_handle = new_inventory_handle;
			}
		}
	}
	return new_inventory_handle;
}

//! Trigger an item drop if the user has played a long enough period of time.
int32 SteamServer::triggerItemDrop(uint32 definition){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		if(SteamInventory()->TriggerItemDrop(&new_inventory_handle, (SteamItemDef_t)definition)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Starts a transaction request to update dynamic properties on items for the current user. This call is rate-limited by user, so property modifications should be batched as much as possible (e.g. at the end of a map or game session). After calling SetProperty or RemoveProperty for all the items that you want to modify, you will need to call SubmitUpdateProperties to send the request to the Steam servers. A SteamInventoryResultReady_t callback will be fired with the results of the operation.
void SteamServer::startUpdateProperties(){
	if(SteamInventory() != NULL){
		inventory_update_handle = SteamInventory()->StartUpdateProperties();
	}
}

//! Submits the transaction request to modify dynamic properties on items for the current user. See StartUpdateProperties.
int32 SteamServer::submitUpdateProperties(uint64_t this_inventory_update_handle){
	int32 new_inventory_handle = 0;
	if(SteamInventory() != NULL){
		// If no inventory update handle is passed, use internal one
		if(this_inventory_update_handle == 0){
			this_inventory_update_handle = inventory_update_handle;
		}
		if(SteamInventory()->SubmitUpdateProperties((SteamInventoryUpdateHandle_t)this_inventory_update_handle, &new_inventory_handle)){
			// Update the internally stored handle
			inventory_handle = new_inventory_handle;
		}
	}
	return new_inventory_handle;
}

//! Removes a dynamic property for the given item.
bool SteamServer::removeProperty(uint64_t item_id, const String& name, uint64_t this_inventory_update_handle){
	if(SteamInventory() == NULL){
		return false;
	}
	// If no inventory update handle is passed, use internal one
	if(this_inventory_update_handle == 0){
		this_inventory_update_handle = inventory_update_handle;
	}
	return SteamInventory()->RemoveProperty((SteamInventoryUpdateHandle_t)this_inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data());
}

//! Sets a dynamic property for the given item. Supported value types are strings.
bool SteamServer::setPropertyString(uint64_t item_id, const String& name, const String& value, uint64_t this_inventory_update_handle){
	if(SteamInventory() == NULL){
		return false;
	}
	// If no inventory update handle is passed, use internal one
	if(this_inventory_update_handle == 0){
		this_inventory_update_handle = inventory_update_handle;
	}
	return SteamInventory()->SetProperty((SteamInventoryUpdateHandle_t)this_inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value.utf8().get_data());
}

//! Sets a dynamic property for the given item. Supported value types are boolean.
bool SteamServer::setPropertyBool(uint64_t item_id, const String& name, bool value, uint64_t this_inventory_update_handle){
	if(SteamInventory() == NULL){
		return false;
	}
	// If no inventory update handle is passed, use internal one
	if(this_inventory_update_handle == 0){
		this_inventory_update_handle = inventory_update_handle;
	}
	return SteamInventory()->SetProperty((SteamInventoryUpdateHandle_t)this_inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value);
}

//! Sets a dynamic property for the given item. Supported value types are 64 bit integers.
bool SteamServer::setPropertyInt(uint64_t item_id, const String& name, uint64_t value, uint64_t this_inventory_update_handle){
	if(SteamInventory() == NULL){
		return false;
	}
	// If no inventory update handle is passed, use internal one
	if(this_inventory_update_handle == 0){
		this_inventory_update_handle = inventory_update_handle;
	}
	return SteamInventory()->SetProperty((SteamInventoryUpdateHandle_t)this_inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), (int64)value);
}

//! Sets a dynamic property for the given item. Supported value types are 32 bit floats.
bool SteamServer::setPropertyFloat(uint64_t item_id, const String& name, float value, uint64_t this_inventory_update_handle){
	if(SteamInventory() == NULL){
		return false;
	}
	// If no inventory update handle is passed, use internal one
	if(this_inventory_update_handle == 0){
		this_inventory_update_handle = inventory_update_handle;
	}
	return SteamInventory()->SetProperty((SteamInventoryUpdateHandle_t)this_inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value);
}


/////////////////////////////////////////////////
///// NETWORKING
/////////////////////////////////////////////////
//
//! This allows the game to specify accept an incoming packet.
bool SteamServer::acceptP2PSessionWithUser(uint64_t steam_id_remote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->AcceptP2PSessionWithUser(steam_id);
}

//! Allow or disallow P2P connections to fall back to being relayed through the Steam servers if a direct connection or NAT-traversal cannot be established.
bool SteamServer::allowP2PPacketRelay(bool allow) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	return SteamNetworking()->AllowP2PPacketRelay(allow);
}

//! Closes a P2P channel when you're done talking to a user on the specific channel.
bool SteamServer::closeP2PChannelWithUser(uint64_t steam_id_remote, int channel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->CloseP2PChannelWithUser(steam_id, channel);
}

//! This should be called when you're done communicating with a user, as this will free up all of the resources allocated for the connection under-the-hood.
bool SteamServer::closeP2PSessionWithUser(uint64_t steam_id_remote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->CloseP2PSessionWithUser(steam_id);
}

//! Fills out a P2PSessionState_t structure with details about the connection like whether or not there is an active connection.
Dictionary SteamServer::getP2PSessionState(uint64_t steam_id_remote) {
	Dictionary result;
	if (SteamNetworking() == NULL) {
		return result;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	P2PSessionState_t p2pSessionState;
	bool success = SteamNetworking()->GetP2PSessionState(steam_id, &p2pSessionState);
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

//! Calls IsP2PPacketAvailable() under the hood, returns the size of the available packet or zero if there is no such packet.
uint32_t SteamServer::getAvailableP2PPacketSize(int channel){
	if (SteamNetworking() == NULL) {
		return 0;
	}
	uint32_t messageSize = 0;
	return (SteamNetworking()->IsP2PPacketAvailable(&messageSize, channel)) ? messageSize : 0;
}

//! Reads in a packet that has been sent from another user via SendP2PPacket.
Dictionary SteamServer::readP2PPacket(uint32_t packet, int channel){
	Dictionary result;
	if (SteamNetworking() == NULL) {
		return result;
	}
	PoolByteArray data;
	data.resize(packet);
	CSteamID steam_id;
	uint32_t bytesRead = 0;
	if (SteamNetworking()->ReadP2PPacket(data.write().ptr(), packet, &bytesRead, &steam_id, channel)){
		data.resize(bytesRead);
		uint64_t steam_id_remote = steam_id.ConvertToUint64();
		result["data"] = data;
		result["steam_id_remote"] = steam_id_remote;
	}
	else {
		data.resize(0);
	}
	return result;
}

//! Sends a P2P packet to the specified user.
bool SteamServer::sendP2PPacket(uint64_t steam_id_remote, PoolByteArray data, int send_type, int channel){
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->SendP2PPacket(steam_id, data.read().ptr(), data.size(), EP2PSend(send_type), channel);
}


/////////////////////////////////////////////////
///// NETWORKING MESSAGES
/////////////////////////////////////////////////
//
//! AcceptSessionWithUser() should only be called in response to a SteamP2PSessionRequest_t callback SteamP2PSessionRequest_t will be posted if another user tries to send you a message, and you haven't tried to talk to them.
bool SteamServer::acceptSessionWithUser(const String& identity_reference){
	if(SteamNetworkingMessages() == NULL){
		return false;
	}
	return SteamNetworkingMessages()->AcceptSessionWithUser(networking_identities[identity_reference.utf8().get_data()]);
}

//! Call this  when you're done talking to a user on a specific channel. Once all open channels to a user have been closed, the open session to the user will be closed, and any new data from this user will trigger a SteamP2PSessionRequest_t callback.
bool SteamServer::closeChannelWithUser(const String& identity_reference, int channel){
	if(SteamNetworkingMessages() == NULL){
		return false;
	}
	return SteamNetworkingMessages()->CloseChannelWithUser(networking_identities[identity_reference.utf8().get_data()], channel);
}

//! Call this when you're done talking to a user to immediately free up resources under-the-hood.
bool SteamServer::closeSessionWithUser(const String& identity_reference){
	if(SteamNetworkingMessages() == NULL){
		return false;
	}
	return SteamNetworkingMessages()->CloseSessionWithUser(networking_identities[identity_reference.utf8().get_data()]);
}

//! Returns information about the latest state of a connection, if any, with the given peer.
Dictionary SteamServer::getSessionConnectionInfo(const String& identity_reference, bool get_connection, bool get_status){
	Dictionary connection_info;
	if(SteamNetworkingMessages() != NULL){
		SteamNetConnectionInfo_t this_info;
		SteamNetConnectionRealTimeStatus_t this_status;
		int connection_state = SteamNetworkingMessages()->GetSessionConnectionInfo(networking_identities[identity_reference.utf8().get_data()], &this_info, &this_status);
		// Parse the data to a dictionary
		connection_info["connection_state"] = connection_state;
		// If getting the connection information
		if(get_connection){
			char identity[STEAM_BUFFER_SIZE];
			this_info.m_identityRemote.ToString(identity, STEAM_BUFFER_SIZE);
			connection_info["identity"] = identity;
			connection_info["user_data"] = (uint64_t)this_info.m_nUserData;
			connection_info["listen_socket"] = this_info.m_hListenSocket;
			char ip_address[STEAM_BUFFER_SIZE];
			this_info.m_addrRemote.ToString(ip_address, STEAM_BUFFER_SIZE, true);
			connection_info["remote_address"] = ip_address;
			connection_info["remote_pop"] = this_info.m_idPOPRemote;
			connection_info["pop_relay"] = this_info.m_idPOPRelay;
			connection_info["connection_state"] = this_info.m_eState;
			connection_info["end_reason"] = this_info.m_eEndReason;
			connection_info["end_debug"] = this_info.m_szEndDebug;
			connection_info["debug_description"] = this_info.m_szConnectionDescription;
		}
		// If getting the quick status
		if(get_status){
			connection_info["state"] = this_status.m_eState;
			connection_info["ping"] = this_status.m_nPing;
			connection_info["local_quality"] = this_status.m_flConnectionQualityLocal;
			connection_info["remote_quality"] = this_status.m_flConnectionQualityRemote;
			connection_info["packets_out_per_second"] = this_status.m_flOutPacketsPerSec;
			connection_info["bytes_out_per_second"] = this_status.m_flOutBytesPerSec;
			connection_info["packets_in_per_second"] = this_status.m_flInPacketsPerSec;
			connection_info["bytes_in_per_second"] = this_status.m_flInBytesPerSec;
			connection_info["send_rate"] = this_status.m_nSendRateBytesPerSecond;
			connection_info["pending_unreliable"] = this_status.m_cbPendingUnreliable;
			connection_info["pending_reliable"] = this_status.m_cbPendingReliable;
			connection_info["sent_unacknowledged_reliable"] = this_status.m_cbSentUnackedReliable;
			connection_info["queue_time"] = (uint64_t)this_status.m_usecQueueTime;
		}
	}
	return connection_info;
}

//! Reads the next message that has been sent from another user via SendMessageToUser() on the given channel. Returns number of messages returned into your list.  (0 if no message are available on that channel.)
Array SteamServer::receiveMessagesOnChannel(int channel, int max_messages){
	Array messages;
	if(SteamNetworkingMessages() != NULL){
		// Allocate the space for the messages
		SteamNetworkingMessage_t** channel_messages = new SteamNetworkingMessage_t*[max_messages];
		// Get the messages
		int available_messages = SteamNetworkingMessages()->ReceiveMessagesOnChannel(channel, channel_messages, max_messages);
		// Loop through and create the messages as dictionaries then add to the messages array
		for(int i = 0; i < available_messages; i++){
			// Set up the mesage dictionary
			Dictionary message;
			// Get the data / message
			int message_size = channel_messages[i]->m_cbSize;
			PoolByteArray data;
			data.resize(message_size);
			uint8_t* source_data = (uint8_t*)channel_messages[i]->m_pData;
			uint8_t* output_data = data.write().ptr();
			for(int j = 0; j < message_size; j++){
				output_data[j] = source_data[j];
			}
			message["payload"] = data;
			message["size"] = message_size;
			message["connection"] = channel_messages[i]->m_conn;
			char identity[STEAM_BUFFER_SIZE];
			channel_messages[i]->m_identityPeer.ToString(identity, STEAM_BUFFER_SIZE);
			message["identity"] = identity;
			message["user_data"] = (uint64_t)channel_messages[i]->m_nConnUserData;
			message["time_received"] = (uint64_t)channel_messages[i]->m_usecTimeReceived;
			message["message_number"] = (uint64_t)channel_messages[i]->m_nMessageNumber;
			message["channel"] = channel_messages[i]->m_nChannel;
			message["flags"] = channel_messages[i]->m_nFlags;
			message["user_data"] = (uint64_t)channel_messages[i]->m_nUserData;
			messages.append(message);
			// Release the message
			channel_messages[i]->Release();
		}
		delete [] channel_messages;
	}
	return messages;
}

//! Sends a message to the specified host. If we don't already have a session with that user, a session is implicitly created. There might be some handshaking that needs to happen before we can actually begin sending message data.
int SteamServer::sendMessageToUser(const String& identity_reference, const PoolByteArray data, int flags, int channel){
	if(SteamNetworkingMessages() == NULL){
		return 0;
	}
	return SteamNetworkingMessages()->SendMessageToUser(networking_identities[identity_reference.utf8().get_data()], data.read().ptr(), data.size(), flags, channel);
}


/////////////////////////////////////////////////
///// NETWORKING SOCKETS
/////////////////////////////////////////////////
//
//! Creates a "server" socket that listens for clients to connect to by calling ConnectByIPAddress, over ordinary UDP (IPv4 or IPv6)
uint32 SteamServer::createListenSocketIP(const String& ip_reference, Array options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	const SteamNetworkingConfigValue_t *these_options = new SteamNetworkingConfigValue_t[sizeof(options)];
	these_options = convertOptionsArray(options);
	uint32 listen_socket = SteamNetworkingSockets()->CreateListenSocketIP(ip_addresses[ip_reference.utf8().get_data()], sizeof(options), these_options);
	delete[] these_options;
	return listen_socket;
}

//! Like CreateListenSocketIP, but clients will connect using ConnectP2P. The connection will be relayed through the Valve network.
uint32 SteamServer::createListenSocketP2P(int virtual_port, Array options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	uint32 listen_socket = SteamNetworkingSockets()->CreateListenSocketP2P(virtual_port, sizeof(options), convertOptionsArray(options));
	return listen_socket;
}

//! Begin connecting to a server that is identified using a platform-specific identifier. This uses the default rendezvous service, which depends on the platform and library configuration. (E.g. on Steam, it goes through the steam backend.) The traffic is relayed over the Steam Datagram Relay network.
uint32 SteamServer::connectP2P(const String& identity_reference, int virtual_port, Array options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->ConnectP2P(networking_identities[identity_reference.utf8().get_data()], virtual_port, sizeof(options), convertOptionsArray(options));
}

//! Client call to connect to a server hosted in a Valve data center, on the specified virtual port. You must have placed a ticket for this server into the cache, or else this connect attempt will fail!
uint32 SteamServer::connectToHostedDedicatedServer(const String& identity_reference, int virtual_port, Array options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->ConnectToHostedDedicatedServer(networking_identities[identity_reference.utf8().get_data()], virtual_port, sizeof(options), convertOptionsArray(options));
}

//! Accept an incoming connection that has been received on a listen socket.
int SteamServer::acceptConnection(uint32 connection_handle){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->AcceptConnection((HSteamNetConnection)connection_handle);
}

//! Disconnects from the remote host and invalidates the connection handle. Any unread data on the connection is discarded.
bool SteamServer::closeConnection(uint32 peer, int reason, const String& debug_message, bool linger){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->CloseConnection((HSteamNetConnection)peer, reason, debug_message.utf8().get_data(), linger);
}

//! Destroy a listen socket. All the connections that were accepted on the listen socket are closed ungracefully.
bool SteamServer::closeListenSocket(uint32 socket){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->CloseListenSocket((HSteamListenSocket)socket);
}

//! Create a pair of connections that are talking to each other, e.g. a loopback connection. This is very useful for testing, or so that your client/server code can work the same even when you are running a local "server".
Dictionary SteamServer::createSocketPair(bool loopback, const String& identity_reference1, const String& identity_reference2){
	// Create a dictionary to populate
	Dictionary connection_pair;
	if(SteamNetworkingSockets() != NULL){
		// Turn the strings back to structs - Should be a check for failure to parse from string
		const SteamNetworkingIdentity identity_struct1 = networking_identities[identity_reference1.utf8().get_data()];
		const SteamNetworkingIdentity identity_struct2 = networking_identities[identity_reference2.utf8().get_data()];
		// Get connections
		uint32 connection1 = 0;
		uint32 connection2 = 0;
		bool success = SteamNetworkingSockets()->CreateSocketPair(&connection1, &connection2, loopback, &identity_struct1, &identity_struct2);
		// Populate the dictionary
		connection_pair["success"] = success;
		connection_pair["connection1"] = connection1;
		connection_pair["connect2"] = connection2;
	}
	return connection_pair;
}

//! Send a message to the remote host on the specified connection.
Dictionary SteamServer::sendMessageToConnection(uint32 connection_handle, const PoolByteArray data, int flags){
	Dictionary message_response;
	if(SteamNetworkingSockets() != NULL){
		int64 number;
		int result = SteamNetworkingSockets()->SendMessageToConnection((HSteamNetConnection)connection_handle, data.read().ptr(), data.size(), flags, &number);
		// Populate the dictionary
		message_response["result"] = result;
		message_response["message_number"] = (uint64_t)number;
	}
	return message_response;
}

//! Send one or more messages without copying the message payload. This is the most efficient way to send messages. To use this function, you must first allocate a message object using ISteamNetworkingUtils::AllocateMessage. (Do not declare one on the stack or allocate your own.)
void SteamServer::sendMessages(int messages, const PoolByteArray data, uint32 connection_handle, int flags){
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingMessage_t *networkMessage;
		networkMessage = SteamNetworkingUtils()->AllocateMessage(0);
		networkMessage->m_pData = (void *)data.read().ptr();
		networkMessage->m_cbSize = data.size();
		networkMessage->m_conn = (HSteamNetConnection)connection_handle;
		networkMessage->m_nFlags = flags;
		int64 result;
		SteamNetworkingSockets()->SendMessages(messages, &networkMessage, &result);
		// Release the message
		networkMessage->Release();
	}
}

//! Flush any messages waiting on the Nagle timer and send them at the next transmission opportunity (often that means right now).
int SteamServer::flushMessagesOnConnection(uint32 connection_handle){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->FlushMessagesOnConnection((HSteamNetConnection)connection_handle);
}

//! Fetch the next available message(s) from the connection, if any. Returns the number of messages returned into your array, up to nMaxMessages. If the connection handle is invalid, -1 is returned. If no data is available, 0, is returned.
Array SteamServer::receiveMessagesOnConnection(uint32 connection_handle, int max_messages){
	Array messages;
	if(SteamNetworkingSockets() != NULL){
		// Allocate the space for the messages
		SteamNetworkingMessage_t** connection_messages = new SteamNetworkingMessage_t*[max_messages];
		// Get the messages
		int available_messages = SteamNetworkingSockets()->ReceiveMessagesOnConnection((HSteamNetConnection)connection_handle, connection_messages, max_messages);
		// Loop through and create the messages as dictionaries then add to the messages array
		for(int i = 0; i < available_messages; i++){
			// Create the message dictionary to send back
			Dictionary message;
			// Get the message data
			int message_size = connection_messages[i]->m_cbSize;
			PoolByteArray data;
			data.resize(message_size);
			uint8_t* source_data = (uint8_t*)connection_messages[i]->m_pData;
			uint8_t* output_data = data.write().ptr();
			for(int j = 0; j < message_size; j++){
				output_data[j] = source_data[j];
			}
			message["payload"] = data;
			message["size"] = message_size;
			message["connection"] = connection_messages[i]->m_conn;
			char identity[STEAM_BUFFER_SIZE];
			connection_messages[i]->m_identityPeer.ToString(identity, STEAM_BUFFER_SIZE);
			message["identity"] = identity;
			message["user_data"] = (uint64_t)connection_messages[i]->m_nConnUserData;
			message["time_received"] = (uint64_t)connection_messages[i]->m_usecTimeReceived;
			message["message_number"] = (uint64_t)connection_messages[i]->m_nMessageNumber;
			message["channel"] = connection_messages[i]->m_nChannel;
			message["flags"] = connection_messages[i]->m_nFlags;
			message["user_data"] = (uint64_t)connection_messages[i]->m_nUserData;
			messages.append(message);
			// Release the message
			connection_messages[i]->Release();
		}
		delete [] connection_messages;
	}
	return messages;
}

//! Create a new poll group.
uint32 SteamServer::createPollGroup(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->CreatePollGroup();
}

//! Destroy a poll group created with CreatePollGroup.
bool SteamServer::destroyPollGroup(uint32 poll_group){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->DestroyPollGroup((HSteamNetPollGroup)poll_group);
}

//! Assign a connection to a poll group. Note that a connection may only belong to a single poll group. Adding a connection to a poll group implicitly removes it from any other poll group it is in.
bool SteamServer::setConnectionPollGroup(uint32 connection_handle, uint32 poll_group){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->SetConnectionPollGroup((HSteamNetConnection)connection_handle, (HSteamNetPollGroup)poll_group);
}

//! Same as ReceiveMessagesOnConnection, but will return the next messages available on any connection in the poll group. Examine SteamNetworkingMessage_t::m_conn to know which connection. (SteamNetworkingMessage_t::m_nConnUserData might also be useful.)
Array SteamServer::receiveMessagesOnPollGroup(uint32 poll_group, int max_messages){
	Array messages;
	if(SteamNetworkingSockets() != NULL){
		// Allocate the space for the messages
		SteamNetworkingMessage_t** poll_messages = new SteamNetworkingMessage_t*[max_messages];
		// Get the messages
		int available_messages = SteamNetworkingSockets()->ReceiveMessagesOnPollGroup((HSteamNetPollGroup)poll_group, poll_messages, max_messages);
		// Loop through and create the messages as dictionaries then add to the messages array
		for(int i = 0; i < available_messages; i++){
			// Create the message dictionary to send back
			Dictionary message;
			// Get the message data
			int message_size = poll_messages[i]->m_cbSize;
			PoolByteArray data;
			data.resize(message_size);
			uint8_t* source_data = (uint8_t*)poll_messages[i]->m_pData;
			uint8_t* output_data = data.write().ptr();
			for(int j = 0; j < message_size; j++){
				output_data[j] = source_data[j];
			}
			message["payload"] = data;
			message["size"] = message_size;
			message["connection"] = poll_messages[i]->m_conn;
			char identity[STEAM_BUFFER_SIZE];
			poll_messages[i]->m_identityPeer.ToString(identity, STEAM_BUFFER_SIZE);
			message["identity"] = identity;
			message["user_data"] = (uint64_t)poll_messages[i]->m_nConnUserData;
			message["time_received"] = (uint64_t)poll_messages[i]->m_usecTimeReceived;
			message["message_number"] = (uint64_t)poll_messages[i]->m_nMessageNumber;
			message["channel"] = poll_messages[i]->m_nChannel;
			message["flags"] = poll_messages[i]->m_nFlags;
			message["user_data"] = (uint64_t)poll_messages[i]->m_nUserData;
			messages.append(message);
			// Release the message
			poll_messages[i]->Release();
		}
		delete [] poll_messages;
	}
	return messages;
}

//! Returns basic information about the high-level state of the connection. Returns false if the connection handle is invalid.
Dictionary SteamServer::getConnectionInfo(uint32 connection_handle){
	Dictionary connection_info;
	if(SteamNetworkingSockets() != NULL){
		SteamNetConnectionInfo_t info;
		if(SteamNetworkingSockets()->GetConnectionInfo((HSteamNetConnection)connection_handle, &info)){
			char identity[STEAM_BUFFER_SIZE];
			info.m_identityRemote.ToString(identity, STEAM_BUFFER_SIZE);
			connection_info["identity"] = identity;
			connection_info["user_data"] = (uint64_t)info.m_nUserData;
			connection_info["listen_socket"] = info.m_hListenSocket;
			char ip_address[STEAM_BUFFER_SIZE];
			info.m_addrRemote.ToString(ip_address, STEAM_BUFFER_SIZE, true);
			connection_info["remote_address"] = ip_address;
			connection_info["remote_pop"] = info.m_idPOPRemote;
			connection_info["pop_relay"] = info.m_idPOPRelay;
			connection_info["connection_state"] = info.m_eState;
			connection_info["end_reason"] = info.m_eEndReason;
			connection_info["end_debug"] = info.m_szEndDebug;
			connection_info["debug_description"] = info.m_szConnectionDescription;
		}
	}
	return connection_info;
}

//! Returns very detailed connection stats in diagnostic text format. Useful for dumping to a log, etc. The format of this information is subject to change.
Dictionary SteamServer::getDetailedConnectionStatus(uint32 connection){
	Dictionary connectionStatus;
	if(SteamNetworkingSockets() != NULL){
		char buffer[STEAM_LARGE_BUFFER_SIZE];
		int success = SteamNetworkingSockets()->GetDetailedConnectionStatus((HSteamNetConnection)connection, buffer, STEAM_LARGE_BUFFER_SIZE);
		// Add data to dictionary
		connectionStatus["success"] = success;
		connectionStatus["status"] = buffer;
	}
	// Send the data back to the user
	return connectionStatus; 
}

//! Fetch connection user data. Returns -1 if handle is invalid or if you haven't set any userdata on the connection.
uint64_t SteamServer::getConnectionUserData(uint32 peer){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->GetConnectionUserData((HSteamNetConnection)peer);
}

//! Set a name for the connection, used mostly for debugging
void SteamServer::setConnectionName(uint32 peer, const String& name){
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingSockets()->SetConnectionName((HSteamNetConnection)peer, name.utf8().get_data());
	}
}

//! Fetch connection name into your buffer, which is at least nMaxLen bytes. Returns false if handle is invalid.
String SteamServer::getConnectionName(uint32 peer){
	// Set empty string variable for use
	String connection_name = "";
	if(SteamNetworkingSockets() != NULL){
		char name[STEAM_BUFFER_SIZE];
		if(SteamNetworkingSockets()->GetConnectionName((HSteamNetConnection)peer, name, STEAM_BUFFER_SIZE)){
			connection_name += name;	
		}
	}
	return connection_name;
}

//! Returns local IP and port that a listen socket created using CreateListenSocketIP is bound to.
bool SteamServer::getListenSocketAddress(uint32 socket){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	SteamNetworkingIPAddr address;
	return SteamNetworkingSockets()->GetListenSocketAddress((HSteamListenSocket)socket, &address);
}

//! Get the identity assigned to this interface.
String SteamServer::getIdentity(){
	String identity_string = "";
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingIdentity this_identity;
		if(SteamNetworkingSockets()->GetIdentity(&this_identity)){
			char *this_buffer = new char[128];
			this_identity.ToString(this_buffer, 128);
			identity_string = String(this_buffer);
			delete[] this_buffer;
		}
	}
	return identity_string;
}

//! Indicate our desire to be ready participate in authenticated communications. If we are currently not ready, then steps will be taken to obtain the necessary certificates. (This includes a certificate for us, as well as any CA certificates needed to authenticate peers.)
int SteamServer::initAuthentication(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->InitAuthentication();
}

//! Query our readiness to participate in authenticated communications. A SteamNetAuthenticationStatus_t callback is posted any time this status changes, but you can use this function to query it at any time.
int SteamServer::getAuthenticationStatus(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->GetAuthenticationStatus(NULL);
}

// Call this when you receive a ticket from your backend / matchmaking system. Puts the ticket into a persistent cache, and optionally returns the parsed ticket.
//Dictionary SteamServer::receivedRelayAuthTicket(){
//	Dictionary ticket;
//	if(SteamNetworkingSockets() != NULL){
//		SteamDatagramRelayAuthTicket parsed_ticket;
//		PoolByteArray incoming_ticket;
//		incoming_ticket.resize(512);		
//		if(SteamNetworkingSockets()->ReceivedRelayAuthTicket(incoming_ticket.write().ptr(), 512, &parsed_ticket)){
//			char game_server;
//			parsed_ticket.m_identityGameserver.ToString(&game_server, 128);
//			ticket["game_server"] = game_server;
//			char authorized_client;
//			parsed_ticket.m_identityAuthorizedClient.ToString(&authorized_client, 128);
//			ticket["authorized_client"] = authorized_client;
//			ticket["public_ip"] = parsed_ticket.m_unPublicIP;		// uint32
//			ticket["expiry"] = parsed_ticket.m_rtimeTicketExpiry;	// RTime32
//			ticket["routing"] = parsed_ticket.m_routing.GetPopID();			// SteamDatagramHostAddress
//			ticket["app_id"] = parsed_ticket.m_nAppID;				// uint32
//			ticket["restrict_to_v_port"] = parsed_ticket.m_nRestrictToVirtualPort;	// int
//			ticket["number_of_extras"] = parsed_ticket.m_nExtraFields;		// int
//			ticket["extra_fields"] = parsed_ticket.m_vecExtraFields;		// ExtraField
//		}
//	}
//	return ticket;
//}

// Search cache for a ticket to talk to the server on the specified virtual port. If found, returns the number of seconds until the ticket expires, and optionally the complete cracked ticket. Returns 0 if we don't have a ticket.
//int SteamServer::findRelayAuthTicketForServer(int port){
//	int expires_in_seconds = 0;
//	if(SteamNetworkingSockets() != NULL){
//		expires_in_seconds = SteamNetworkingSockets()->FindRelayAuthTicketForServer(game_server, port, &relay_auth_ticket);
//	}
//	return expires_in_seconds;
//}



//! Returns the value of the SDR_LISTEN_PORT environment variable. This is the UDP server your server will be listening on. This will configured automatically for you in production environments.
uint16 SteamServer::getHostedDedicatedServerPort(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->GetHostedDedicatedServerPort();
}

//! Returns 0 if SDR_LISTEN_PORT is not set. Otherwise, returns the data center the server is running in. This will be k_SteamDatagramPOPID_dev in non-production envirionment.
uint32 SteamServer::getHostedDedicatedServerPOPId(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->GetHostedDedicatedServerPOPID();
}

// Return info about the hosted server. This contains the PoPID of the server, and opaque routing information that can be used by the relays to send traffic to your server.
//int SteamServer::getHostedDedicatedServerAddress(){
//	int result = 2;
//	if(SteamNetworkingSockets() != NULL){
//		result = SteamNetworkingSockets()->GetHostedDedicatedServerAddress(&hosted_address);
//	}
//	return result;
//}

//! Create a listen socket on the specified virtual port. The physical UDP port to use will be determined by the SDR_LISTEN_PORT environment variable. If a UDP port is not configured, this call will fail.
uint32 SteamServer::createHostedDedicatedServerListenSocket(int port, Array options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamGameServerNetworkingSockets()->CreateHostedDedicatedServerListenSocket(port, sizeof(options), convertOptionsArray(options));
}

// Generate an authentication blob that can be used to securely login with your backend, using SteamDatagram_ParseHostedServerLogin. (See steamdatagram_gamecoordinator.h)
//int SteamServer::getGameCoordinatorServerLogin(const String& app_data){
//	int result = 2;
//	if(SteamNetworkingSockets() != NULL){	
//		SteamDatagramGameCoordinatorServerLogin *server_login = new SteamDatagramGameCoordinatorServerLogin;
//		server_login->m_cbAppData = app_data.size();
//		strcpy(server_login->m_appData, app_data.utf8().get_data());
//		int signed_blob = k_cbMaxSteamDatagramGameCoordinatorServerLoginSerialized;
//		routing_blob.resize(signed_blob);
//		result = SteamNetworkingSockets()->GetGameCoordinatorServerLogin(server_login, &signed_blob, routing_blob.write().ptr());
//		delete server_login;
//	}
//	return result;
//}

// Returns a small set of information about the real-time state of the connection and the queue status of each lane.
Dictionary SteamServer::getConnectionRealTimeStatus(uint32 connection, int lanes, bool get_status){
	// Create the dictionary for returning
	Dictionary real_time_status;
	if(SteamNetworkingSockets() != NULL){
		SteamNetConnectionRealTimeStatus_t this_status;
		SteamNetConnectionRealTimeLaneStatus_t *lanes_array = new SteamNetConnectionRealTimeLaneStatus_t[lanes];
		int result = SteamNetworkingSockets()->GetConnectionRealTimeStatus((HSteamNetConnection)connection, &this_status, lanes, lanes_array);
		// Append the status
		real_time_status["response"] = result;
		// If the result is good, get more data
		if(result == 0){
			// Get the connection status if requested
			Dictionary connection_status;
			if(get_status){
				connection_status["state"] = this_status.m_eState;
				connection_status["ping"] = this_status.m_nPing;
				connection_status["local_quality"] = this_status.m_flConnectionQualityLocal;
				connection_status["remote_quality"] = this_status.m_flConnectionQualityRemote;
				connection_status["packets_out_per_second"] = this_status.m_flOutPacketsPerSec;
				connection_status["bytes_out_per_second"] = this_status.m_flOutBytesPerSec;
				connection_status["packets_in_per_second"] = this_status.m_flInPacketsPerSec;
				connection_status["bytes_in_per_second"] = this_status.m_flInBytesPerSec;
				connection_status["send_rate"] = this_status.m_nSendRateBytesPerSecond;
				connection_status["pending_unreliable"] = this_status.m_cbPendingUnreliable;
				connection_status["pending_reliable"] = this_status.m_cbPendingReliable;
				connection_status["sent_unacknowledged_reliable"] = this_status.m_cbSentUnackedReliable;
				connection_status["queue_time"] = (uint64_t)this_status.m_usecQueueTime;
			}
			real_time_status["connection_status"] = connection_status;
			// Get the lane information
			Array lanes_status;
			for(int i = 0; i < lanes; i++){
				Dictionary lane_status;
				lane_status["pending_unreliable"] = lanes_array[i].m_cbPendingUnreliable;
				lane_status["pending_reliable"] = lanes_array[i].m_cbPendingReliable;
				lane_status["sent_unacknowledged_reliable"] = lanes_array[i].m_cbSentUnackedReliable;
				lane_status["queue_time"] = (uint64_t)lanes_array[i].m_usecQueueTime;
				lanes_status.append(lane_status);
			}
			delete[] lanes_array;
			real_time_status["lanes_status"] = lanes_status;
		}
	}
	return real_time_status;
}

// Configure multiple outbound messages streams ("lanes") on a connection, and control head-of-line blocking between them.
// Messages within a given lane are always sent in the order they are queued, but messages from different lanes may be sent out of order.
// Each lane has its own message number sequence.  The first message sent on each lane will be assigned the number 1.
int SteamServer::configureConnectionLanes(uint32 connection, int lanes, Array priorities, Array weights) {
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	// Convert the priorities array to an int
	int *lane_priorities = new int[lanes];
	for(int i = 0; i < lanes; i++){
		lane_priorities[i] = priorities[i];
	}
	// Convert the weights array to an int
	uint16 *lane_weights = new uint16[lanes];
	for(int i = 0; i < lanes; i++){
		lane_weights[i] = weights[i];
	}
	int result = SteamNetworkingSockets()->ConfigureConnectionLanes((HSteamNetConnection)connection, lanes, lane_priorities, lane_weights);
	delete[] lane_priorities;
	delete[] lane_weights;
	return result;
}

// Certificate provision by the application. On Steam, we normally handle all this automatically and you will not need to use these advanced functions.
Dictionary SteamServer::getCertificateRequest(){
	Dictionary cert_information;
	if(SteamNetworkingSockets() != NULL){
		int *certificate = new int[512];
		int cert_size = 0;
		SteamNetworkingErrMsg error_message;
		if(SteamNetworkingSockets()->GetCertificateRequest(&cert_size, &certificate, error_message)){
			cert_information["certificate"] = certificate;
			cert_information["cert_size"] = cert_size;
			cert_information["error_message"] = error_message;
		}
		delete[] certificate;
	}
	return cert_information;
}

// Set the certificate. The certificate blob should be the output of SteamDatagram_CreateCert.
Dictionary SteamServer::setCertificate(const PoolByteArray& certificate){
	Dictionary certificate_data;
	if(SteamNetworkingSockets() != NULL){
		bool success = false;
		SteamNetworkingErrMsg error_message;
		success = SteamNetworkingSockets()->SetCertificate((void*)certificate.read().ptr(), certificate.size(), error_message);
		if(success){
			certificate_data["response"] = success;
			certificate_data["error"] = error_message;
		}
	}
	return certificate_data;
}

// Reset the identity associated with this instance. Any open connections are closed.  Any previous certificates, etc are discarded.
// You can pass a specific identity that you want to use, or you can pass NULL, in which case the identity will be invalid until you set it using SetCertificate.
// NOTE: This function is not actually supported on Steam!  It is included for use on other platforms where the active user can sign out and a new user can sign in.
void SteamServer::resetIdentity(const String& identity_reference){
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingIdentity resetting_identity = networking_identities[identity_reference.utf8().get_data()];
		SteamNetworkingSockets()->ResetIdentity(&resetting_identity);
	}
}

// Invoke all callback functions queued for this interface. See k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, etc.
// You don't need to call this if you are using Steam's callback dispatch mechanism (SteamAPI_RunCallbacks and SteamGameserver_RunCallbacks).
void SteamServer::runNetworkingCallbacks(){
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingSockets()->RunCallbacks();		
	}
}

// Begin asynchronous process of allocating a fake IPv4 address that other peers can use to contact us via P2P.
// IP addresses returned by this function are globally unique for a given appid.
// Returns false if a request was already in progress, true if a new request was started.
// A SteamNetworkingFakeIPResult_t will be posted when the request completes.
bool SteamServer::beginAsyncRequestFakeIP(int num_ports){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->BeginAsyncRequestFakeIP(num_ports);
}

// Return info about the FakeIP and port(s) that we have been assigned, if any.
// idxFirstPort is currently reserved and must be zero. Make sure and check SteamNetworkingFakeIPResult_t::m_eResult
Dictionary SteamServer::getFakeIP(int first_port){
	// Create the return dictionary
	Dictionary fake_ip;
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingFakeIPResult_t fake_ip_result;
		SteamNetworkingSockets()->GetFakeIP(first_port, &fake_ip_result);
		// Populate the dictionary
		fake_ip["result"] = fake_ip_result.m_eResult;
		fake_ip["identity_type"] = fake_ip_result.m_identity.m_eType;
		fake_ip["ip"] = fake_ip_result.m_unIP;
		char ports[8];
		for (size_t i = 0; i < sizeof(fake_ip_result.m_unPorts) / sizeof(fake_ip_result.m_unPorts[0]); i++){
			ports[i] = fake_ip_result.m_unPorts[i];
		}
		fake_ip["ports"] = ports;
	}
	return fake_ip;
}

// Create a listen socket that will listen for P2P connections sent to our FakeIP.
// A peer can initiate connections to this listen socket by calling ConnectByIPAddress.
uint32 SteamServer::createListenSocketP2PFakeIP(int fake_port, Array options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->CreateListenSocketP2PFakeIP(fake_port, sizeof(options), convertOptionsArray(options));
}

// If the connection was initiated using the "FakeIP" system, then we we can get an IP address for the remote host.  If the remote host had a global FakeIP at the time the connection was established, this function will return that global IP.
// Otherwise, a FakeIP that is unique locally will be allocated from the local FakeIP address space, and that will be returned.
Dictionary SteamServer::getRemoteFakeIPForConnection(uint32 connection){
	// Create the return dictionary
	Dictionary this_fake_address;
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingIPAddr fake_address;
		int result = SteamNetworkingSockets()->GetRemoteFakeIPForConnection((HSteamNetConnection)connection, &fake_address);
		// Send back the data
		this_fake_address["result"] = result;
		this_fake_address["port"] = fake_address.m_port;
		this_fake_address["ip_type"] = fake_address.GetFakeIPType();
		ip_addresses["fake_ip_address"] = fake_address;
		}

	return this_fake_address;
}

// Get an interface that can be used like a UDP port to send/receive datagrams to a FakeIP address.
// This is intended to make it easy to port existing UDP-based code to take advantage of SDR.
// To create a "client" port (e.g. the equivalent of an ephemeral UDP port) pass -1.
void SteamServer::createFakeUDPPort(int fake_server_port_index){
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingSockets()->CreateFakeUDPPort(fake_server_port_index);
	}
}


/////////////////////////////////////////////////
///// NETWORKING TYPES
/////////////////////////////////////////////////
//
// Create a new network identity and store it for use
bool SteamServer::addIdentity(const String& reference_name){
	networking_identities[reference_name.utf8().get_data()] = SteamNetworkingIdentity();
	if(networking_identities.count(reference_name.utf8().get_data()) > 0){
		return true;
	}
	return false;
}

// Clear a network identity's data
void SteamServer::clearIdentity(const String& reference_name){
	networking_identities[reference_name.utf8().get_data()].Clear();
}


// Get a list of all known network identities
Array SteamServer::getIdentities(){
	Array these_identities;
	// Loop through the map
	for(auto& identity : networking_identities){
		Dictionary this_identity;
		this_identity["reference_name"] = identity.first;
		this_identity["steam_id"] = (uint64_t)getIdentitySteamID64(identity.first);
		this_identity["type"] = networking_identities[identity.first].m_eType;
		these_identities.append(this_identity);
	}
	return these_identities;
}


// Return true if we are the invalid type.  Does not make any other validity checks (e.g. is SteamID actually valid)
bool SteamServer::isIdentityInvalid(const String& reference_name){
	return networking_identities[reference_name.utf8().get_data()].IsInvalid();
}

// Set a 32-bit Steam ID
void SteamServer::setIdentitySteamID(const String& reference_name, uint32 steam_id){
	networking_identities[reference_name.utf8().get_data()].SetSteamID(createSteamID(steam_id));
}

// Return black CSteamID (!IsValid()) if identity is not a SteamID
uint32 SteamServer::getIdentitySteamID(const String& reference_name){
	CSteamID steam_id = networking_identities[reference_name.utf8().get_data()].GetSteamID();
	return steam_id.ConvertToUint64();
}

// Takes SteamID as raw 64-bit number
void SteamServer::setIdentitySteamID64(const String& reference_name, uint64_t steam_id){
	networking_identities[reference_name.utf8().get_data()].SetSteamID64(steam_id);
}

// Returns 0 if identity is not SteamID
uint64_t SteamServer::getIdentitySteamID64(const String& reference_name){
	return networking_identities[reference_name.utf8().get_data()].GetSteamID64();
}

// Set to specified IP:port.
bool SteamServer::setIdentityIPAddr(const String& reference_name, const String& ip_address_name){
	if(ip_addresses.count(ip_address_name.utf8().get_data()) > 0){
		const SteamNetworkingIPAddr this_address = ip_addresses[ip_address_name.utf8().get_data()];
		networking_identities[reference_name.utf8().get_data()].SetIPAddr(this_address);
		return true;
	}
	return false;
}

// Returns null if we are not an IP address.
uint32 SteamServer::getIdentityIPAddr(const String& reference_name){
	const SteamNetworkingIPAddr* this_address = networking_identities[reference_name.utf8().get_data()].GetIPAddr();
	if (this_address == NULL){
		return 0;
	}
	return this_address->GetIPv4();
}

// Retrieve this identity's Playstation Network ID.
uint64_t SteamServer::getPSNID(const String& reference_name){
	return networking_identities[reference_name.utf8().get_data()].GetPSNID();
}

// Retrieve this identity's Google Stadia ID.
uint64_t SteamServer::getStadiaID(const String& reference_name){
	return networking_identities[reference_name.utf8().get_data()].GetStadiaID();
}

// Retrieve this identity's XBox pair ID.
String SteamServer::getXboxPairwiseID(const String& reference_name){
	return networking_identities[reference_name.utf8().get_data()].GetXboxPairwiseID();
}

// Set to localhost. (We always use IPv6 ::1 for this, not 127.0.0.1).
void SteamServer::setIdentityLocalHost(const String& reference_name){
	networking_identities[reference_name.utf8().get_data()].SetLocalHost();
}

// Return true if this identity is localhost.
bool SteamServer::isIdentityLocalHost(const String& reference_name){
	return networking_identities[reference_name.utf8().get_data()].IsLocalHost();
}

// Returns false if invalid length.
bool SteamServer::setGenericString(const String& reference_name, const String& this_string){
	return networking_identities[reference_name.utf8().get_data()].SetGenericString(this_string.utf8().get_data());
}

// Returns nullptr if not generic string type
String SteamServer::getGenericString(const String& reference_name){
	return networking_identities[reference_name.utf8().get_data()].GetGenericString();
}

// Returns false if invalid size.
bool SteamServer::setGenericBytes(const String& reference_name, uint8 data){
	const void *this_data = &data;
	return networking_identities[reference_name.utf8().get_data()].SetGenericBytes(this_data, sizeof(data));
}

// Returns null if not generic bytes type.
uint8 SteamServer::getGenericBytes(const String& reference_name){
	uint8 these_bytes = 0;
	if(!reference_name.empty()){
		int length = 0;
		const uint8* generic_bytes = networking_identities[reference_name.utf8().get_data()].GetGenericBytes(length);
		these_bytes = *generic_bytes;
	}
	return these_bytes;
}

// Add a new IP address struct
bool SteamServer::addIPAddress(const String& reference_name){
	ip_addresses[reference_name.utf8().get_data()] = SteamNetworkingIPAddr();
	if(ip_addresses.count(reference_name.utf8().get_data()) > 0){
		return true;
	}
	return false;
}

// Get a list of all IP address structs and their names
Array SteamServer::getIPAddresses(){
	Array these_addresses;
	// Loop through the map
	for(auto& address : ip_addresses){
		Dictionary this_address;
		this_address["reference_name"] = address.first;
		this_address["localhost"] = isAddressLocalHost(address.first);
		this_address["ip_address"] = getIPv4(address.first);
		these_addresses.append(this_address);
	}
	return these_addresses;
}

// IP Address - Set everything to zero. E.g. [::]:0
void SteamServer::clearIPAddress(const String& reference_name){
	ip_addresses[reference_name.utf8().get_data()].Clear();
}

// Return true if the IP is ::0. (Doesn't check port.)
bool SteamServer::isIPv6AllZeros(const String& reference_name){
	return ip_addresses[reference_name.utf8().get_data()].IsIPv6AllZeros();
}

// Set IPv6 address. IP is interpreted as bytes, so there are no endian issues. (Same as inaddr_in6.) The IP can be a mapped IPv4 address.
void SteamServer::setIPv6(const String& reference_name, uint8 ipv6, uint16 port){
	const uint8 *this_ipv6 = &ipv6;
	ip_addresses[reference_name.utf8().get_data()].SetIPv6(this_ipv6, port);
}

// Sets to IPv4 mapped address. IP and port are in host byte order.
void SteamServer::setIPv4(const String& reference_name, uint32 ip, uint16 port){
	ip_addresses[reference_name.utf8().get_data()].SetIPv4(ip, port);
}

// Return true if IP is mapped IPv4.
bool SteamServer::isIPv4(const String& reference_name){
	return ip_addresses[reference_name.utf8().get_data()].IsIPv4();
}

// Returns IP in host byte order (e.g. aa.bb.cc.dd as 0xaabbccdd). Returns 0 if IP is not mapped IPv4.
uint32 SteamServer::getIPv4(const String& reference_name){
	return ip_addresses[reference_name.utf8().get_data()].GetIPv4();
}

// Set to the IPv6 localhost address ::1, and the specified port.
void SteamServer::setIPv6LocalHost(const String& reference_name, uint16 port){
	ip_addresses[reference_name.utf8().get_data()].SetIPv6LocalHost(port);
}

// Set the Playstation Network ID for this identity.
void SteamServer::setPSNID(const String& reference_name, uint64_t psn_id){
	networking_identities[reference_name.utf8().get_data()].SetPSNID(psn_id);
}

// Set the Google Stadia ID for this identity.
void SteamServer::setStadiaID(const String& reference_name, uint64_t stadia_id){
	networking_identities[reference_name.utf8().get_data()].SetStadiaID(stadia_id);
}

// Set the Xbox Pairwise ID for this identity.
bool SteamServer::setXboxPairwiseID(const String& reference_name, const String& xbox_id){
	return networking_identities[reference_name.utf8().get_data()].SetXboxPairwiseID(xbox_id.utf8().get_data());
}

// Return true if this identity is localhost. (Either IPv6 ::1, or IPv4 127.0.0.1).
bool SteamServer::isAddressLocalHost(const String& reference_name){
	return ip_addresses[reference_name.utf8().get_data()].IsLocalHost();
}

// Parse back a string that was generated using ToString. If we don't understand the string, but it looks "reasonable" (it matches the pattern type:<type-data> and doesn't have any funky characters, etc), then we will return true, and the type is set to k_ESteamNetworkingIdentityType_UnknownType.
// false will only be returned if the string looks invalid.
bool SteamServer::parseIdentityString(const String& reference_name, const String& string_to_parse){
	if(!reference_name.empty() && !string_to_parse.empty()){
		if(networking_identities[reference_name.utf8().get_data()].ParseString(string_to_parse.utf8().get_data())){
			return true;
		}
		return false;
	}
	return false;
}

// Parse an IP address and optional port.  If a port is not present, it is set to 0. (This means that you cannot tell if a zero port was explicitly specified.).
bool SteamServer::parseIPAddressString(const String& reference_name, const String& string_to_parse){
	if(!reference_name.empty() && !string_to_parse.empty()){
		if(ip_addresses[reference_name.utf8().get_data()].ParseString(string_to_parse.utf8().get_data())){
			return true;
		}
		return false;
	}
	return false;
}

// Print to a string, with or without the port. Mapped IPv4 addresses are printed as dotted decimal (12.34.56.78), otherwise this will print the canonical form according to RFC5952.
// If you include the port, IPv6 will be surrounded by brackets, e.g. [::1:2]:80. Your buffer should be at least k_cchMaxString bytes to avoid truncation.
String SteamServer::toIPAddressString(const String& reference_name, bool with_port){
	String ip_address_string = "";
	char *this_buffer = new char[128];
	ip_addresses[reference_name.utf8().get_data()].ToString(this_buffer, 128, with_port);
	ip_address_string = String(this_buffer);
	delete[] this_buffer;
	return ip_address_string;
}

// Print to a human-readable string.  This is suitable for debug messages or any other time you need to encode the identity as a string.
// It has a URL-like format (type:<type-data>). Your buffer should be at least k_cchMaxString bytes big to avoid truncation.
String SteamServer::toIdentityString(const String& reference_name){
	String identity_string = "";
	char *this_buffer = new char[128];
	networking_identities[reference_name.utf8().get_data()].ToString(this_buffer, 128);
	identity_string = String(this_buffer);
	delete[] this_buffer;
	return identity_string;
}

// Helper function to turn an array of options into an array of SteamNetworkingConfigValue_t structs
const SteamNetworkingConfigValue_t* SteamServer::convertOptionsArray(Array options){
	// Get the number of option arrays in the array.
	int options_size = sizeof(options);
	// Create the array for options.
	SteamNetworkingConfigValue_t *option_array = new SteamNetworkingConfigValue_t[options_size];
	// If there are options
	if(options_size > 0){
		// Populate the options
		for(int i = 0; i < options_size; i++){
			SteamNetworkingConfigValue_t this_option;
			Array sent_option = options[i];
			// Get the configuration value.
			// This is a convoluted way of doing it but can't seem to cast the value as an enum so here we are.
			ESteamNetworkingConfigValue this_value = ESteamNetworkingConfigValue((int)sent_option[0]);
			if((int)sent_option[1] == 1){
				this_option.SetInt32(this_value, sent_option[2]);
			}
			else if((int)sent_option[1] == 2){
				this_option.SetInt64(this_value, sent_option[2]);
			}
			else if((int)sent_option[1] == 3){
				this_option.SetFloat(this_value, sent_option[2]);
			}
			else if((int)sent_option[1] == 4){
				char *this_string = { 0 };
				String passed_string = sent_option[2];
				strcpy(this_string, passed_string.utf8().get_data());
				this_option.SetString(this_value, this_string);
			}
			else{
				Object *this_pointer;
				this_pointer = sent_option[2];
				this_option.SetPtr(this_value, this_pointer);
			}
			option_array[i] = this_option;
		}
	}
	return option_array;
}


/////////////////////////////////////////////////
///// NETWORKING UTILS
/////////////////////////////////////////////////
//
//! If you know that you are going to be using the relay network (for example, because you anticipate making P2P connections), call this to initialize the relay network. If you do not call this, the initialization will be delayed until the first time you use a feature that requires access to the relay network, which will delay that first access.
void SteamServer::initRelayNetworkAccess(){
	if(SteamNetworkingUtils() != NULL){
		SteamNetworkingUtils()->InitRelayNetworkAccess();
	}
}

//! Fetch current status of the relay network.  If you want more details, you can pass a non-NULL value.
int SteamServer::getRelayNetworkStatus(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetRelayNetworkStatus(NULL);
}

//! Return location info for the current host. Returns the approximate age of the data, in seconds, or -1 if no data is available.
Dictionary SteamServer::getLocalPingLocation(){
	Dictionary ping_location;
	if(SteamNetworkingUtils() != NULL){
		SteamNetworkPingLocation_t location;
		float age = SteamNetworkingUtils()->GetLocalPingLocation(location);
		// Populate the dictionary
		PoolByteArray data;
		data.resize(512);
		uint8_t* output_data = data.write().ptr();
		for(int j = 0; j < 512; j++){
			output_data[j] = location.m_data[j];
		}
		ping_location["age"] = age;
		ping_location["location"] = data;
	}
	return ping_location;
}

//! Estimate the round-trip latency between two arbitrary locations, in milliseconds. This is a conservative estimate, based on routing through the relay network. For most basic relayed connections, this ping time will be pretty accurate, since it will be based on the route likely to be actually used.
int SteamServer::estimatePingTimeBetweenTwoLocations(PoolByteArray location1, PoolByteArray location2){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	// Add these locations to ping structs
	SteamNetworkPingLocation_t ping_location1;
	SteamNetworkPingLocation_t ping_location2;
	uint8_t* input_location_1 = (uint8*) location1.read().ptr();
	for(int j = 0; j < 512; j++){
		ping_location1.m_data[j] = input_location_1[j];
	}
	uint8_t* input_location_2 = (uint8*) location2.read().ptr();
	for(int j = 0; j < 512; j++){
		ping_location2.m_data[j] = (uint8) input_location_2[j];
	}
	return SteamNetworkingUtils()->EstimatePingTimeBetweenTwoLocations(ping_location1, ping_location2);
}

//! Same as EstimatePingTime, but assumes that one location is the local host. This is a bit faster, especially if you need to calculate a bunch of these in a loop to find the fastest one.
int SteamServer::estimatePingTimeFromLocalHost(PoolByteArray location){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	// Add this location to ping struct
	SteamNetworkPingLocation_t ping_location;
	uint8_t* input_location = (uint8*) location.read().ptr();
	for(int j = 0; j < 512; j++){
		ping_location.m_data[j] = input_location[j];
	}
	return SteamNetworkingUtils()->EstimatePingTimeFromLocalHost(ping_location);
}

//! Convert a ping location into a text format suitable for sending over the wire. The format is a compact and human readable. However, it is subject to change so please do not parse it yourself. Your buffer must be at least k_cchMaxSteamNetworkingPingLocationString bytes.
String SteamServer::convertPingLocationToString(PoolByteArray location){
	String location_string = "";
	if(SteamNetworkingUtils() != NULL){
		char *buffer = new char[512];
		// Add this location to ping struct
		SteamNetworkPingLocation_t ping_location;
		uint8_t* input_location = (uint8*) location.read().ptr();
		for(int j = 0; j < 512; j++){
			ping_location.m_data[j] = input_location[j];
		}
		SteamNetworkingUtils()->ConvertPingLocationToString(ping_location, buffer, k_cchMaxSteamNetworkingPingLocationString);
		location_string += buffer;
		delete[] buffer;
	}
	return location_string;
}

//! Parse back SteamNetworkPingLocation_t string. Returns false if we couldn't understand the string.
Dictionary SteamServer::parsePingLocationString(const String& location_string){
	Dictionary parse_string;
	if(SteamNetworkingUtils() != NULL){
		SteamNetworkPingLocation_t result;
		bool success = SteamNetworkingUtils()->ParsePingLocationString(location_string.utf8().get_data(), result);
		// Populate the dictionary
		PoolByteArray data;
		data.resize(512);
		uint8_t* output_data = data.write().ptr();
		for(int j = 0; j < 512; j++){
			output_data[j] = result.m_data[j];
		}
		parse_string["success"] = success;
		parse_string["ping_location"] = data;
	}
	return parse_string;
}

//! Check if the ping data of sufficient recency is available, and if it's too old, start refreshing it.
bool SteamServer::checkPingDataUpToDate(float max_age_in_seconds){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->CheckPingDataUpToDate(max_age_in_seconds);
}

//! Fetch ping time of best available relayed route from this host to the specified data center.
Dictionary SteamServer::getPingToDataCenter(uint32 pop_id){
	Dictionary data_center_ping;
	if(SteamNetworkingUtils() != NULL){
		SteamNetworkingPOPID via_relay_pop;
		int ping = SteamNetworkingUtils()->GetPingToDataCenter((SteamNetworkingPOPID)pop_id, &via_relay_pop);
		// Populate the dictionary
		data_center_ping["pop_relay"] = via_relay_pop;
		data_center_ping["ping"] = ping;
	}
	return data_center_ping;
}

//! Get *direct* ping time to the relays at the point of presence.
int SteamServer::getDirectPingToPOP(uint32 pop_id){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetDirectPingToPOP((SteamNetworkingPOPID)pop_id);
}

//! Get number of network points of presence in the config
int SteamServer::getPOPCount(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetPOPCount();
}

//! Get list of all POP IDs. Returns the number of entries that were filled into your list.
Array SteamServer::getPOPList(){
	Array pop_list;
	if(SteamNetworkingUtils() != NULL){
		SteamNetworkingPOPID *list = new SteamNetworkingPOPID[256];
		int pops = SteamNetworkingUtils()->GetPOPList(list, 256);
		// Iterate and add
		for(int i = 0; i < pops; i++){
			int pop_id = list[i];
			pop_list.append(pop_id);
		}
		delete[] list;
	}
	return pop_list;
}

// Set a configuration value.
//bool SteamServer::setConfigValue(int setting, int scope_type, uint32_t connection_handle, int data_type, auto value){
//	if(SteamNetworkingUtils() == NULL){
//		return false;
//	}
//	return SteamNetworkingUtils()->SetConfigValue((ESteamNetworkingConfigValue)setting, (ESteamNetworkingConfigScope)scope_type, connection_handle, (ESteamNetworkingConfigDataType)data_type, value);
//}

// Get a configuration value.
Dictionary SteamServer::getConfigValue(int config_value, int scope_type, uint32_t connection_handle){
	Dictionary config_info;
	if(SteamNetworkingUtils() != NULL){
		ESteamNetworkingConfigDataType data_type;
		size_t buffer_size;
		PoolByteArray config_result;
		int result = SteamNetworkingUtils()->GetConfigValue((ESteamNetworkingConfigValue)config_value, (ESteamNetworkingConfigScope)scope_type, connection_handle, &data_type, &config_result, &buffer_size);
		// Populate the dictionary
		config_info["result"] = result;
		config_info["type"] = data_type;
		config_info["value"] = config_result;
		config_info["buffer"] = (uint64_t)buffer_size;
	}
	return config_info;
}

//! Returns info about a configuration value.
Dictionary SteamServer::getConfigValueInfo(int config_value){
	Dictionary config_info;
	if(SteamNetworkingUtils() != NULL){
		ESteamNetworkingConfigDataType data_type;
		ESteamNetworkingConfigScope scope;
		if(SteamNetworkingUtils()->GetConfigValueInfo((ESteamNetworkingConfigValue)config_value, &data_type, &scope)){
			// Populate the dictionary
			config_info["type"] = data_type;
			config_info["scope"] = scope;
		}
	}
	return config_info;
}

// The following functions are handy shortcuts for common use cases.
bool SteamServer::setGlobalConfigValueInt32(int config, int32 value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetGlobalConfigValueInt32((ESteamNetworkingConfigValue)config, value);
}
bool SteamServer::setGlobalConfigValueFloat(int config, float value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetGlobalConfigValueFloat((ESteamNetworkingConfigValue)config, value);
}
bool SteamServer::setGlobalConfigValueString(int config, const String& value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetGlobalConfigValueString((ESteamNetworkingConfigValue)config, value.utf8().get_data());
}
bool SteamServer::setConnectionConfigValueInt32(uint32 connection, int config, int32 value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetConnectionConfigValueInt32(connection, (ESteamNetworkingConfigValue)config, value);
}
bool SteamServer::setConnectionConfigValueFloat(uint32 connection, int config, float value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetConnectionConfigValueFloat(connection, (ESteamNetworkingConfigValue)config, value);
}

bool SteamServer::setConnectionConfigValueString(uint32 connection, int config, const String& value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetConnectionConfigValueString(connection, (ESteamNetworkingConfigValue)config, value.utf8().get_data());
}

//! A general purpose high resolution local timer with the following properties: Monotonicity is guaranteed. The initial value will be at least 24*3600*30*1e6, i.e. about 30 days worth of microseconds. In this way, the timestamp value of 0 will always be at least "30 days ago". Also, negative numbers will never be returned. Wraparound / overflow is not a practical concern.
uint64_t SteamServer::getLocalTimestamp(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetLocalTimestamp();
}


/////////////////////////////////////////////////
///// UGC
/////////////////////////////////////////////////
//
//! Adds a dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
//! This is a soft-dependency that is displayed on the web. It is up to the application to determine whether the item can actually be used or not.
void SteamServer::addAppDependency(uint64_t published_file_id, uint32_t app_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->AddAppDependency(file_id, app);
		callResultAddAppDependency.Set(api_call, this, &SteamServer::add_app_dependency_result);
	}
}

//! Adds a workshop item as a dependency to the specified item. If the nParentPublishedFileID item is of type k_EWorkshopFileTypeCollection, than the nChildPublishedFileID is simply added to that collection.
//! Otherwise, the dependency is a soft one that is displayed on the web and can be retrieved via the ISteamUGC API using a combination of the m_unNumChildren member variable of the SteamUGCDetails_t struct and GetQueryUGCChildren.
void SteamServer::addDependency(uint64_t published_file_id, uint64_t child_published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t parent = (uint64_t)published_file_id;
		PublishedFileId_t child = (uint64_t)child_published_file_id;
		SteamAPICall_t api_call = SteamUGC()->AddDependency(parent, child);
		callResultAddUGCDependency.Set(api_call, this, &SteamServer::add_ugc_dependency_result);
	}
}

//! Adds a excluded tag to a pending UGC Query. This will only return UGC without the specified tag.
bool SteamServer::addExcludedTag(uint64_t query_handle, const String& tag_name){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddExcludedTag(handle, tag_name.utf8().get_data());
}

//! Adds a key-value tag pair to an item. Keys can map to multiple different values (1-to-many relationship).
bool SteamServer::addItemKeyValueTag(uint64_t update_handle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = (uint64_t)update_handle;
	return SteamUGC()->AddItemKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

//! Adds an additional preview file for the item.
bool SteamServer::addItemPreviewFile(uint64_t query_handle, const String& preview_file, int type){
	if(SteamUGC() == NULL){
		return false;
	}
	EItemPreviewType previewType;
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	if(type == 0){
		previewType = k_EItemPreviewType_Image;
	}
	else if(type == 1){
		previewType = k_EItemPreviewType_YouTubeVideo;
	}
	else if(type == 2){
		previewType = k_EItemPreviewType_Sketchfab;
	}
	else if(type == 3){
		previewType = k_EItemPreviewType_EnvironmentMap_HorizontalCross;
	}
	else if(type == 4){
		previewType = k_EItemPreviewType_EnvironmentMap_LatLong;
	}
	else{
		previewType = k_EItemPreviewType_ReservedMax;
	}
	return SteamUGC()->AddItemPreviewFile(handle, preview_file.utf8().get_data(), previewType);
}

//! Adds an additional video preview from YouTube for the item.
bool SteamServer::addItemPreviewVideo(uint64_t query_handle, const String& video_id){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddItemPreviewVideo(handle, video_id.utf8().get_data());
}

//! Adds a workshop item to the users favorites list.
void SteamServer::addItemToFavorites(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() != NULL){
		AppId_t app = (uint32_t)app_id;
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->AddItemToFavorites(app, file_id);
		callResultFavoriteItemListChanged.Set(api_call, this, &SteamServer::user_favorite_items_list_changed);
	}
}

//! Adds a required key-value tag to a pending UGC Query. This will only return workshop items that have a key = pKey and a value = pValue.
bool SteamServer::addRequiredKeyValueTag(uint64_t query_handle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddRequiredKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

//! Adds a required tag to a pending UGC Query. This will only return UGC with the specified tag.
bool SteamServer::addRequiredTag(uint64_t query_handle, const String& tag_name){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddRequiredTag(handle, tag_name.utf8().get_data());
}

//! Adds the requirement that the returned items from the pending UGC Query have at least one of the tags in the given set (logical "or"). For each tag group that is added, at least one tag from each group is required to be on the matching items.
bool SteamServer::addRequiredTagGroup(uint64_t query_handle, Array tag_array){
	bool added_tag_group = false;
	if(SteamUGC() != NULL){
		UGCQueryHandle_t handle = uint64(query_handle);
		SteamParamStringArray_t *tags = new SteamParamStringArray_t();
		tags->m_ppStrings = new const char*[tag_array.size()];
		uint32 strCount = tag_array.size();
		for (uint32 i=0; i < strCount; i++) {
			String str = (String)tag_array[i];
			tags->m_ppStrings[i] = str.utf8().get_data();
		}
		tags->m_nNumStrings = tag_array.size();
		added_tag_group = SteamUGC()->AddRequiredTagGroup(handle, tags);
		delete tags;
	}
	return added_tag_group;
}

//! Lets game servers set a specific workshop folder before issuing any UGC commands.
bool SteamServer::initWorkshopForGameServer(uint32_t workshop_depot_id){
	bool initialized_workshop = false;
	if(SteamUGC() != NULL){
		DepotId_t workshop = (uint32_t)workshop_depot_id;
		const char *folder = new char[256];
		initialized_workshop = SteamUGC()->BInitWorkshopForGameServer(workshop, (char*)folder);
		delete[] folder;
	}
	return initialized_workshop;
}

//! Creates a new workshop item with no content attached yet.
void SteamServer::createItem(uint32 app_id, int file_type){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->CreateItem((AppId_t)app_id, (EWorkshopFileType)file_type);
		callResultItemCreate.Set(api_call, this, &SteamServer::item_created);
	}
}

//! Query for all matching UGC. You can use this to list all of the available UGC for your app.
uint64_t SteamServer::createQueryAllUGCRequest(int query_type, int matching_type, uint32_t creator_id, uint32_t consumer_id, uint32 page){
	if(SteamUGC() == NULL){
		return 0;
	}
	EUGCQuery query;
	if(query_type == 0){
		query = k_EUGCQuery_RankedByVote;
	}
	else if(query_type == 1){
		query = k_EUGCQuery_RankedByPublicationDate;
	}
	else if(query_type == 2){
		query = k_EUGCQuery_AcceptedForGameRankedByAcceptanceDate;
	}
	else if(query_type == 3){
		query = k_EUGCQuery_RankedByTrend;
	}
	else if(query_type == 4){
		query = k_EUGCQuery_FavoritedByFriendsRankedByPublicationDate;
	}
	else if(query_type == 5){
		query = k_EUGCQuery_CreatedByFriendsRankedByPublicationDate;
	}
	else if(query_type == 6){
		query = k_EUGCQuery_RankedByNumTimesReported;
	}
	else if(query_type == 7){
		query = k_EUGCQuery_CreatedByFollowedUsersRankedByPublicationDate;
	}
	else if(query_type == 8){
		query = k_EUGCQuery_NotYetRated;
	}
	else if(query_type == 9){
		query = k_EUGCQuery_RankedByTotalVotesAsc;
	}
	else if(query_type == 10){
		query = k_EUGCQuery_RankedByVotesUp;
	}
	else if(query_type == 11){
		query = k_EUGCQuery_RankedByTextSearch;
	}
	else if(query_type == 12){
		query = k_EUGCQuery_RankedByTotalUniqueSubscriptions;
	}
	else if(query_type == 13){
		query = k_EUGCQuery_RankedByPlaytimeTrend;
	}
	else if(query_type == 14){
		query = k_EUGCQuery_RankedByTotalPlaytime;
	}
	else if(query_type == 15){
		query = k_EUGCQuery_RankedByAveragePlaytimeTrend;
	}
	else if(query_type == 16){
		query = k_EUGCQuery_RankedByLifetimeAveragePlaytime;
	}
	else if(query_type == 17){
		query = k_EUGCQuery_RankedByPlaytimeSessionsTrend;
	}
	else{
		query = k_EUGCQuery_RankedByLifetimePlaytimeSessions;
	}
	EUGCMatchingUGCType match;
	if(matching_type == 0){
		match = k_EUGCMatchingUGCType_All;
	}
	else if(matching_type == 1){
		match = k_EUGCMatchingUGCType_Items_Mtx;
	}
	else if(matching_type == 2){
		match = k_EUGCMatchingUGCType_Items_ReadyToUse;
	}
	else if(matching_type == 3){
		match = k_EUGCMatchingUGCType_Collections;
	}
	else if(matching_type == 4){
		match = k_EUGCMatchingUGCType_Artwork;
	}
	else if(matching_type == 5){
		match = k_EUGCMatchingUGCType_Videos;
	}
	else if(matching_type == 6){
		match = k_EUGCMatchingUGCType_Screenshots;
	}
	else if(matching_type == 7){
		match = k_EUGCMatchingUGCType_AllGuides;
	}
	else if(matching_type == 8){
		match = k_EUGCMatchingUGCType_WebGuides;
	}
	else if(matching_type == 9){
		match = k_EUGCMatchingUGCType_IntegratedGuides;
	}
	else if(matching_type == 10){
		match = k_EUGCMatchingUGCType_UsableInGame;
	}
	else if(matching_type == 11){
		match = k_EUGCMatchingUGCType_ControllerBindings;
	}
	else{
		match = k_EUGCMatchingUGCType_GameManagedItems;
	}
	AppId_t creator = (uint32_t)creator_id;
	AppId_t consumer = (uint32_t)consumer_id;
	UGCQueryHandle_t handle = SteamUGC()->CreateQueryAllUGCRequest(query, match, creator, consumer, page);
	return (uint64_t)handle;
}

//! Query for the details of specific workshop items.
uint64_t SteamServer::createQueryUGCDetailsRequest(Array published_file_ids){
	uint64_t this_handle = 0;
	if(SteamUGC() != NULL){
		uint32 fileCount = published_file_ids.size();
		if(fileCount != 0){
			PublishedFileId_t *file_ids = new PublishedFileId_t[fileCount];
			for(uint32 i = 0; i < fileCount; i++){
				file_ids[i] = (uint64_t)published_file_ids[i];
			}
			UGCQueryHandle_t handle = SteamUGC()->CreateQueryUGCDetailsRequest(file_ids, fileCount);
			delete[] file_ids;
			this_handle = (uint64_t)handle;
		}
	}
	return this_handle;
}

//! Query UGC associated with a user. You can use this to list the UGC the user is subscribed to amongst other things.
uint64_t SteamServer::createQueryUserUGCRequest(uint64_t steam_id, int list_type, int matching_ugc_type, int sort_order, uint32_t creator_id, uint32_t consumer_id, uint32 page){
	if(SteamUGC() == NULL){
		return 0;
	}
	// Get tue universe ID from the Steam ID
	CSteamID user_id = (uint64)steam_id;
	AccountID_t account = (AccountID_t)user_id.ConvertToUint64();
	EUserUGCList list;
	if(list_type == 0){
		list = k_EUserUGCList_Published;
	}
	else if(list_type == 1){
		list = k_EUserUGCList_VotedOn;
	}
	else if(list_type == 2){
		list = k_EUserUGCList_VotedUp;
	}
	else if(list_type == 3){
		list = k_EUserUGCList_VotedDown;
	}
	else if(list_type == 4){
		list = k_EUserUGCList_WillVoteLater;
	}
	else if(list_type == 5){
		list = k_EUserUGCList_Favorited;
	}
	else if(list_type == 6){
		list = k_EUserUGCList_Subscribed;
	}
	else if(list_type == 7){
		list = k_EUserUGCList_UsedOrPlayed;
	}
	else{
		list = k_EUserUGCList_Followed;
	}
	EUGCMatchingUGCType match;
	if(matching_ugc_type == 0){
		match = k_EUGCMatchingUGCType_All;
	}
	else if(matching_ugc_type == 1){
		match = k_EUGCMatchingUGCType_Items_Mtx;
	}
	else if(matching_ugc_type == 2){
		match = k_EUGCMatchingUGCType_Items_ReadyToUse;
	}
	else if(matching_ugc_type == 3){
		match = k_EUGCMatchingUGCType_Collections;
	}
	else if(matching_ugc_type == 4){
		match = k_EUGCMatchingUGCType_Artwork;
	}
	else if(matching_ugc_type == 5){
		match = k_EUGCMatchingUGCType_Videos;
	}
	else if(matching_ugc_type == 6){
		match = k_EUGCMatchingUGCType_Screenshots;
	}
	else if(matching_ugc_type == 7){
		match = k_EUGCMatchingUGCType_AllGuides;
	}
	else if(matching_ugc_type == 8){
		match = k_EUGCMatchingUGCType_WebGuides;
	}
	else if(matching_ugc_type == 9){
		match = k_EUGCMatchingUGCType_IntegratedGuides;
	}
	else if(matching_ugc_type == 10){
		match = k_EUGCMatchingUGCType_UsableInGame;
	}
	else if(matching_ugc_type == 11){
		match = k_EUGCMatchingUGCType_ControllerBindings;
	}
	else{
		match = k_EUGCMatchingUGCType_GameManagedItems;
	}
	EUserUGCListSortOrder sort;
	if(sort_order == 0){
		sort = k_EUserUGCListSortOrder_CreationOrderDesc;
	}
	else if(sort_order == 1){
		sort = k_EUserUGCListSortOrder_CreationOrderAsc;
	}
	else if(sort_order == 2){
		sort = k_EUserUGCListSortOrder_TitleAsc;
	}
	else if(sort_order == 3){
		sort = k_EUserUGCListSortOrder_LastUpdatedDesc;
	}
	else if(sort_order == 4){
		sort = k_EUserUGCListSortOrder_SubscriptionDateDesc;
	}
	else if(sort_order == 5){
		sort = k_EUserUGCListSortOrder_VoteScoreDesc;
	}
	else{
		sort = k_EUserUGCListSortOrder_ForModeration;
	}
	AppId_t creator = (int)creator_id;
	AppId_t consumer = (int)consumer_id;
	UGCQueryHandle_t handle = SteamUGC()->CreateQueryUserUGCRequest(account, list, match, sort, creator, consumer, page);
	return (uint64_t)handle;
}

//! Deletes the item without prompting the user.
void SteamServer::deleteItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->DeleteItem(file_id);
		callResultDeleteItem.Set(api_call, this, &SteamServer::item_deleted);
	}
}

//! Download new or update already installed item. If returns true, wait for DownloadItemResult_t. If item is already installed, then files on disk should not be used until callback received.
//! If item is not subscribed to, it will be cached for some time. If bHighPriority is set, any other item download will be suspended and this item downloaded ASAP.
bool SteamServer::downloadItem(uint64_t published_file_id, bool high_priority){
	if(SteamUGC() == NULL){
		return false;
	}
	PublishedFileId_t file_id = (uint64_t)published_file_id;
	return SteamUGC()->DownloadItem(file_id, high_priority);
}

//! Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
Dictionary SteamServer::getItemDownloadInfo(uint64_t published_file_id){
	Dictionary info;
	if(SteamUGC() == NULL){
		return info;
	}
	uint64 downloaded = 0;
	uint64 total = 0;
	info["ret"] = SteamUGC()->GetItemDownloadInfo((PublishedFileId_t)published_file_id, &downloaded, &total);
	if(info["ret"]){
		info["downloaded"] = uint64_t(downloaded);
		info["total"] = uint64_t(total);
	}
	return info;
}

//! Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled set.
Dictionary SteamServer::getItemInstallInfo(uint64_t published_file_id){
	Dictionary info;
	if(SteamUGC() == NULL){
		info["ret"] = false;
		return info;
	}
	PublishedFileId_t file_id = (uint64_t)published_file_id;
	uint64 sizeOnDisk;
	char folder[1024] = { 0 };
	uint32 timeStamp;
	info["ret"] = SteamUGC()->GetItemInstallInfo((PublishedFileId_t)file_id, &sizeOnDisk, folder, sizeof(folder), &timeStamp);
	if(info["ret"]){
		info["size"] = (int)sizeOnDisk;
		info["folder"] = folder;
		info["foldersize"] = (uint32)sizeof(folder);
		info["timestamp"] = timeStamp;
	}
	return info;
}

//! Gets the current state of a workshop item on this client.
uint32 SteamServer::getItemState(uint64_t published_file_id){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t file_id = (uint64_t)published_file_id;
	return SteamUGC()->GetItemState(file_id);
}

//! Gets the progress of an item update.
Dictionary SteamServer::getItemUpdateProgress(uint64_t update_handle){
	Dictionary updateProgress;
	if(SteamUGC() == NULL){
		return updateProgress;
	}
	UGCUpdateHandle_t handle = (uint64_t)update_handle;
	uint64 processed = 0;
	uint64 total = 0;
	EItemUpdateStatus status = SteamUGC()->GetItemUpdateProgress(handle, &processed, &total);
	updateProgress["status"] = status;
	updateProgress["processed"] = uint64_t(processed);
	updateProgress["total"] = uint64_t(total);
	return updateProgress;
}

//! Gets the total number of items the current user is subscribed to for the game or application.
uint32 SteamServer::getNumSubscribedItems(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUGC()->GetNumSubscribedItems();
}

//! Retrieve the details of an additional preview associated with an individual workshop item after receiving a querying UGC call result.
Dictionary SteamServer::getQueryUGCAdditionalPreview(uint64_t query_handle, uint32 index, uint32 preview_index){
	Dictionary preview;
	if(SteamUGC() == NULL){
		return preview;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	char *url_or_video_id = new char[256];
	char *original_filename = new char[256];
	EItemPreviewType previewType;
	bool success = SteamUGC()->GetQueryUGCAdditionalPreview(handle, index, preview_index, (char*)url_or_video_id, 256, (char*)original_filename, 256, &previewType);
	if(success){
		preview["success"] = success;
		preview["handle"] = (uint64_t)handle;
		preview["index"] = index;
		preview["preview"] = preview_index;
		preview["urlOrVideo"] = url_or_video_id;
		preview["filename"] = original_filename;
		preview["type"] = previewType;
	}
	delete[] url_or_video_id;
	delete[] original_filename;
	return preview;
}

//! Retrieve the ids of any child items of an individual workshop item after receiving a querying UGC call result. These items can either be a part of a collection or some other dependency (see AddDependency).
Dictionary SteamServer::getQueryUGCChildren(uint64_t query_handle, uint32 index, uint32_t child_count){
	Dictionary children;
	if(SteamUGC() == NULL){
		return children;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	PoolVector<uint64_t> vec;
	vec.resize(child_count);
	bool success = SteamUGC()->GetQueryUGCChildren(handle, index, (PublishedFileId_t*)vec.write().ptr(), child_count);
	if(success) {
		Array godot_arr;
		godot_arr.resize(child_count);
		for (uint32_t i = 0; i < child_count; i++) {
			godot_arr[i] = vec[i];
		}
		
		children["success"] = success;
		children["handle"] = (uint64_t)handle;
		children["index"] = index;
		children["children"] = godot_arr;
	}
	return children;
}

//! Retrieve the details of a key-value tag associated with an individual workshop item after receiving a querying UGC call result.
Dictionary SteamServer::getQueryUGCKeyValueTag(uint64_t query_handle, uint32 index, uint32 key_value_tag_index){
	Dictionary tag;
	if(SteamUGC() == NULL){
		return tag;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	char *key = new char[256];
	char *value = new char[256];
	bool success = SteamUGC()->GetQueryUGCKeyValueTag(handle, index, key_value_tag_index, (char*)key, 256, (char*)value, 256);
	if(success){
		tag["success"] = success;
		tag["handle"] = (uint64_t)handle;
		tag["index"] = index;
		tag["tag"] = key_value_tag_index;
		tag["key"] = key;
		tag["value"] = value;
	}
	delete[] key;
	delete[] value;
	return tag;
}

//! Retrieve the developer set metadata of an individual workshop item after receiving a querying UGC call result.
String SteamServer::getQueryUGCMetadata(uint64_t query_handle, uint32 index){
	String query_ugc_metadata = "";
	if(SteamUGC() != NULL){
		UGCQueryHandle_t handle = (uint64_t)query_handle;
		char *metadata = new char[5000];
		bool success = SteamUGC()->GetQueryUGCMetadata(handle, index, (char*)metadata, 5000);
		if(success){
			query_ugc_metadata = metadata;
		}
		delete[] metadata;
	}
	return query_ugc_metadata;
}

//! Retrieve the number of additional previews of an individual workshop item after receiving a querying UGC call result.
uint32 SteamServer::getQueryUGCNumAdditionalPreviews(uint64_t query_handle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumAdditionalPreviews(handle, index);
}

//! Retrieve the number of key-value tags of an individual workshop item after receiving a querying UGC call result.
uint32 SteamServer::getQueryUGCNumKeyValueTags(uint64_t query_handle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumKeyValueTags(handle, index);
}

//! Retrieve the number of tags for an individual workshop item after receiving a querying UGC call result. You should call this in a loop to get the details of all the workshop items returned.
uint32 SteamServer::getQueryUGCNumTags(uint64_t query_handle, uint32 index){
	if(SteamUGC() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumTags(handle, index);
}

//! Retrieve the URL to the preview image of an individual workshop item after receiving a querying UGC call result.
String SteamServer::getQueryUGCPreviewURL(uint64_t query_handle, uint32 index){
	String query_ugc_preview_url = "";
	if(SteamUGC() != NULL){
		UGCQueryHandle_t handle = (uint64_t)query_handle;
		char *url = new char[256];
		bool success = SteamUGC()->GetQueryUGCPreviewURL(handle, index, (char*)url, 256);
		if(success){
			query_ugc_preview_url = url;
		}
		delete[] url;
	}
	return query_ugc_preview_url;
}

//! Retrieve the details of an individual workshop item after receiving a querying UGC call result.
Dictionary SteamServer::getQueryUGCResult(uint64_t query_handle, uint32 index){
	Dictionary ugcResult;
	if(SteamUGC() == NULL){
		return ugcResult;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	SteamUGCDetails_t pDetails;
	bool success = SteamUGC()->GetQueryUGCResult(handle, index, &pDetails);
	if(success){
		ugcResult["result"] = (uint64_t)pDetails.m_eResult;
		ugcResult["file_id"] = (uint64_t)pDetails.m_nPublishedFileId;
		ugcResult["file_type"] = (uint64_t)pDetails.m_eFileType;
		ugcResult["creator_app_id"] = (uint32_t)pDetails.m_nCreatorAppID;
		ugcResult["consumer_app_id"] = (uint32_t)pDetails.m_nConsumerAppID;
		ugcResult["title"] = String::utf8(pDetails.m_rgchTitle);
		ugcResult["description"] = String::utf8(pDetails.m_rgchDescription);
		ugcResult["steam_id_owner"] = (uint64_t)pDetails.m_ulSteamIDOwner;
		ugcResult["time_created"] = pDetails.m_rtimeCreated;
		ugcResult["time_updated"] = pDetails.m_rtimeUpdated;
		ugcResult["time_added_to_user_list"] = pDetails.m_rtimeAddedToUserList;
		ugcResult["visibility"] = (uint64_t)pDetails.m_eVisibility;
		ugcResult["banned"] = pDetails.m_bBanned;
		ugcResult["accepted_for_use"] = pDetails.m_bAcceptedForUse;
		ugcResult["tags_truncated"] = pDetails.m_bTagsTruncated;
		ugcResult["tags"] = pDetails.m_rgchTags;
		ugcResult["handle_file"] = (uint64_t)pDetails.m_hFile;
		ugcResult["handle_preview_file"] = (uint64_t)pDetails.m_hPreviewFile;
		ugcResult["file_name"] = pDetails.m_pchFileName;
		ugcResult["file_size"] = pDetails.m_nFileSize;
		ugcResult["preview_file_size"] = pDetails.m_nPreviewFileSize;
		ugcResult["url"] = pDetails.m_rgchURL;
		ugcResult["votes_up"] = pDetails.m_unVotesUp;
		ugcResult["votes_down"] = pDetails.m_unVotesDown;
		ugcResult["score"] = pDetails.m_flScore;
		ugcResult["num_children"] = pDetails.m_unNumChildren;
	}
	return ugcResult;
}

//! Retrieve various statistics of an individual workshop item after receiving a querying UGC call result.
Dictionary SteamServer::getQueryUGCStatistic(uint64_t query_handle, uint32 index, int stat_type){
	Dictionary ugcStat;
	if(SteamUGC() == NULL){
		return ugcStat;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	EItemStatistic type;
		if(stat_type == 0){
		type = k_EItemStatistic_NumSubscriptions;
	}
	else if(stat_type == 1){
		type = k_EItemStatistic_NumFavorites;
	}
	else if(stat_type == 2){
		type = k_EItemStatistic_NumFollowers;
	}
	else if(stat_type == 3){
		type = k_EItemStatistic_NumUniqueSubscriptions;
	}
	else if(stat_type == 4){
		type = k_EItemStatistic_NumUniqueFavorites;
	}
	else if(stat_type == 5){
		type = k_EItemStatistic_NumUniqueFollowers;
	}
	else if(stat_type == 6){
		type = k_EItemStatistic_NumUniqueWebsiteViews;
	}
	else if(stat_type == 7){
		type = k_EItemStatistic_ReportScore;
	}
	else if(stat_type == 8){
		type = k_EItemStatistic_NumSecondsPlayed;
	}
	else if(stat_type == 9){
		type = k_EItemStatistic_NumPlaytimeSessions;
	}
	else if(stat_type == 10){
		type = k_EItemStatistic_NumComments;
	}
	else if(stat_type == 11){
		type = k_EItemStatistic_NumSecondsPlayedDuringTimePeriod;
	}
	else{
		type = k_EItemStatistic_NumPlaytimeSessionsDuringTimePeriod;
	}
	uint64 value = 0;
	bool success = SteamUGC()->GetQueryUGCStatistic(handle, index, type, &value);
	if(success){
		ugcStat["success"] = success;
		ugcStat["handle"] = (uint64_t)handle;
		ugcStat["index"] = index;
		ugcStat["type"] = type;
		ugcStat["value"] = (uint64_t)value;
	}	
	return ugcStat;
}

//! Retrieve the "nth" tag associated with an individual workshop item after receiving a querying UGC call result.
//! You should call this in a loop to get the details of all the workshop items returned.
String SteamServer::getQueryUGCTag(uint64_t query_handle, uint32 index, uint32 tag_index){
	if(SteamUGC() == NULL){
		return "";
	}
	// Set a default tag to return
	char *tag = new char[64];
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	SteamUGC()->GetQueryUGCTag(handle, index, tag_index, tag, 64);
	tag[63] = '\0';
	String tag_name = tag;
	delete[] tag;
	return tag_name;
}

//! Retrieve the "nth" display string (usually localized) for a tag, which is associated with an individual workshop item after receiving a querying UGC call result.
//! You should call this in a loop to get the details of all the workshop items returned.
String SteamServer::getQueryUGCTagDisplayName(uint64_t query_handle, uint32 index, uint32 tag_index){
	if(SteamUGC() == NULL){
		return "";
	}
	// Set a default tag name to return
	char *tag = new char[256];
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	SteamUGC()->GetQueryUGCTagDisplayName(handle, index, tag_index, tag, 256);
	tag[255] = '\0';
	String tagDisplay = tag;
	delete[] tag;
	return tagDisplay;
}

//! Gets a list of all of the items the current user is subscribed to for the current game.
Array SteamServer::getSubscribedItems(){
	if(SteamUGC() == NULL){
		return Array();
	}
	Array subscribed;
	uint32 num_items = SteamUGC()->GetNumSubscribedItems();
	PublishedFileId_t *items = new PublishedFileId_t[num_items];
	uint32 item_list = SteamUGC()->GetSubscribedItems(items, num_items);
	for(uint32 i = 0; i < item_list; i++){
		subscribed.append((uint64_t)items[i]);
	}
	delete[] items;
	return subscribed;
}

//! Gets the users vote status on a workshop item.
void SteamServer::getUserItemVote(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->GetUserItemVote(file_id);
		callResultGetUserItemVote.Set(api_call, this, &SteamServer::get_item_vote_result);
	}
}

//! Releases a UGC query handle when you are done with it to free up memory.
bool SteamServer::releaseQueryUGCRequest(uint64_t query_handle){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->ReleaseQueryUGCRequest(handle);
}

//! Removes the dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
void SteamServer::removeAppDependency(uint64_t published_file_id, uint32_t app_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveAppDependency(file_id, app);
		callResultRemoveAppDependency.Set(api_call, this, &SteamServer::remove_app_dependency_result);
	}
}

//! Removes a workshop item as a dependency from the specified item.
void SteamServer::removeDependency(uint64_t published_file_id, uint64_t child_published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		PublishedFileId_t childID = (uint64_t)child_published_file_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveDependency(file_id, childID);
		callResultRemoveUGCDependency.Set(api_call, this, &SteamServer::remove_ugc_dependency_result);
	}
}

//! Removes a workshop item from the users favorites list.
void SteamServer::removeItemFromFavorites(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveItemFromFavorites(app, file_id);
		callResultFavoriteItemListChanged.Set(api_call, this, &SteamServer::user_favorite_items_list_changed);
	}
}

//! Removes an existing key value tag from an item.
bool SteamServer::removeItemKeyValueTags(uint64_t update_handle, const String& key){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->RemoveItemKeyValueTags(handle, key.utf8().get_data());
}

//! Removes an existing preview from an item.
bool SteamServer::removeItemPreview(uint64_t update_handle, uint32 index){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->RemoveItemPreview(handle, index);
}

//! Send a UGC query to Steam.
void SteamServer::sendQueryUGCRequest(uint64_t update_handle){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamAPICall_t api_call = SteamUGC()->SendQueryUGCRequest(handle);
		callResultUGCQueryCompleted.Set(api_call, this, &SteamServer::ugc_query_completed);
	}
}

//! Sets whether results will be returned from the cache for the specific period of time on a pending UGC Query.
bool SteamServer::setAllowCachedResponse(uint64_t update_handle, uint32 max_age_seconds){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetAllowCachedResponse(handle, max_age_seconds);
}

//! Sets to only return items that have a specific filename on a pending UGC Query.
bool SteamServer::setCloudFileNameFilter(uint64_t update_handle, const String& match_cloud_filename){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetCloudFileNameFilter(handle, match_cloud_filename.utf8().get_data());
}

//! Sets the folder that will be stored as the content for an item.
bool SteamServer::setItemContent(uint64_t update_handle, const String& content_folder){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemContent(handle, content_folder.utf8().get_data());
}

//! Sets a new description for an item.
bool SteamServer::setItemDescription(uint64_t update_handle, const String& description){
	if(SteamUGC() == NULL){
		return false;
	}
	if ((uint32_t)description.length() > (uint32_t)k_cchPublishedDocumentDescriptionMax){
		printf("Description cannot have more than %d ASCII characters. Description not set.", k_cchPublishedDocumentDescriptionMax);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemDescription(handle, description.utf8().get_data());
}

//! Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
bool SteamServer::setItemMetadata(uint64_t update_handle, const String& metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	if (metadata.utf8().length() > 5000){
		printf("Metadata cannot be more than %d bytes. Metadata not set.", 5000);
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemMetadata(handle, metadata.utf8().get_data());
}

//! Sets the primary preview image for the item.
bool SteamServer::setItemPreview(uint64_t update_handle, const String& preview_file){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemPreview(handle, preview_file.utf8().get_data());
}

//! Sets arbitrary developer specified tags on an item.
bool SteamServer::setItemTags(uint64_t update_handle, Array tag_array){
	bool tags_set = false;
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamParamStringArray_t *tags = new SteamParamStringArray_t();
		tags->m_ppStrings = new const char*[tag_array.size()];
		uint32 strCount = tag_array.size();
		for (uint32 i=0; i < strCount; i++) {
			String str = (String)tag_array[i];
			tags->m_ppStrings[i] = str.utf8().get_data();
		}
		tags->m_nNumStrings = tag_array.size();
		tags_set = SteamUGC()->SetItemTags(handle, tags);
		delete tags;
	}
	return tags_set;
}

//! Sets a new title for an item.
bool SteamServer::setItemTitle(uint64_t update_handle, const String& title){
	if(SteamUGC() == NULL){
		return false;
	}
	if (title.length() > 255){
		printf("Title cannot have more than %d ASCII characters. Title not set.", 255);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemTitle(handle, title.utf8().get_data());
}

//! Sets the language of the title and description that will be set in this item update.
bool SteamServer::setItemUpdateLanguage(uint64_t update_handle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}

//! Sets the visibility of an item.
bool SteamServer::setItemVisibility(uint64_t update_handle, int visibility){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemVisibility(handle, (ERemoteStoragePublishedFileVisibility)visibility);
}

//! Sets the language to return the title and description in for the items on a pending UGC Query.
bool SteamServer::setLanguage(uint64_t query_handle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetLanguage(handle, language.utf8().get_data());
}

//! Sets whether workshop items will be returned if they have one or more matching tag, or if all tags need to match on a pending UGC Query.
bool SteamServer::setMatchAnyTag(uint64_t query_handle, bool match_any_tag){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetMatchAnyTag(handle, match_any_tag);
}

//! Sets whether the order of the results will be updated based on the rank of items over a number of days on a pending UGC Query.
bool SteamServer::setRankedByTrendDays(uint64_t query_handle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetRankedByTrendDays(handle, days);
}

//! Sets whether to return any additional images/videos attached to the items on a pending UGC Query.
bool SteamServer::setReturnAdditionalPreviews(uint64_t query_handle, bool return_additional_previews){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnAdditionalPreviews(handle, return_additional_previews);
}

//! Sets whether to return the IDs of the child items of the items on a pending UGC Query.
bool SteamServer::setReturnChildren(uint64_t query_handle, bool return_children){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnChildren(handle, return_children);
}

//! Sets whether to return any key-value tags for the items on a pending UGC Query.
bool SteamServer::setReturnKeyValueTags(uint64_t query_handle, bool return_key_value_tags){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnKeyValueTags(handle, return_key_value_tags);
}

//! Sets whether to return the full description for the items on a pending UGC Query.
bool SteamServer::setReturnLongDescription(uint64_t query_handle, bool return_long_description){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnLongDescription(handle, return_long_description);
}

//! Sets whether to return the developer specified metadata for the items on a pending UGC Query.
bool SteamServer::setReturnMetadata(uint64_t query_handle, bool return_metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnMetadata(handle, return_metadata);
}

//! Sets whether to only return IDs instead of all the details on a pending UGC Query.
bool SteamServer::setReturnOnlyIDs(uint64_t query_handle, bool return_only_ids){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnOnlyIDs(handle, return_only_ids);
}

//! Sets whether to return the the playtime stats on a pending UGC Query.
bool SteamServer::setReturnPlaytimeStats(uint64_t query_handle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnPlaytimeStats(handle, days);
}

//! Sets whether to only return the the total number of matching items on a pending UGC Query.
bool SteamServer::setReturnTotalOnly(uint64_t query_handle, bool return_total_only){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnTotalOnly(handle, return_total_only);
}

//! Sets a string to that items need to match in either the title or the description on a pending UGC Query.
bool SteamServer::setSearchText(uint64_t query_handle, const String& search_text){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetSearchText(handle, search_text.utf8().get_data());
}

//! Allows the user to rate a workshop item up or down.
void SteamServer::setUserItemVote(uint64_t published_file_id, bool vote_up){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->SetUserItemVote(file_id, vote_up);
		callResultSetUserItemVote.Set(api_call, this, &SteamServer::set_user_item_vote);
	}
}

//! Starts the item update process.
uint64_t SteamServer::startItemUpdate(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() == NULL){
		return 0;
	}
	AppId_t app = (uint32_t)app_id;
	PublishedFileId_t file_id = (uint64_t)published_file_id;
	return SteamUGC()->StartItemUpdate(app, file_id);
}

//! Start tracking playtime on a set of workshop items.
void SteamServer::startPlaytimeTracking(Array published_file_ids){
	if(SteamUGC() != NULL){
		uint32 fileCount = published_file_ids.size();
		if(fileCount > 0){
			PublishedFileId_t *file_ids = new PublishedFileId_t[fileCount];
			for(uint32 i = 0; i < fileCount; i++){
				file_ids[i] = (uint64_t)published_file_ids[i];
			}
			SteamAPICall_t api_call = SteamUGC()->StartPlaytimeTracking(file_ids, fileCount);
			callResultStartPlaytimeTracking.Set(api_call, this, &SteamServer::start_playtime_tracking);
			delete[] file_ids;
		}
	}
}

//! Stop tracking playtime on a set of workshop items.
void SteamServer::stopPlaytimeTracking(Array published_file_ids){
	if(SteamUGC() != NULL){
		uint32 fileCount = published_file_ids.size();
		if(fileCount > 0){
			PublishedFileId_t *file_ids = new PublishedFileId_t[fileCount];
			Array files;
			for(uint32 i = 0; i < fileCount; i++){
				file_ids[i] = (uint64_t)published_file_ids[i];
			}
			SteamAPICall_t api_call = SteamUGC()->StopPlaytimeTracking(file_ids, fileCount);
			callResultStopPlaytimeTracking.Set(api_call, this, &SteamServer::stop_playtime_tracking);
			delete[] file_ids;
		}
	}
}

//! Stop tracking playtime of all workshop items.
void SteamServer::stopPlaytimeTrackingForAllItems(){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->StopPlaytimeTrackingForAllItems();
		callResultStopPlaytimeTracking.Set(api_call, this, &SteamServer::stop_playtime_tracking);
	}
}

//! Returns any app dependencies that are associated with the given item.
void SteamServer::getAppDependencies(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->GetAppDependencies(file_id);
		callResultGetAppDependencies.Set(api_call, this, &SteamServer::get_app_dependencies_result);
	}
}

//! Uploads the changes made to an item to the Steam Workshop; to be called after setting your changes.
void SteamServer::submitItemUpdate(uint64_t update_handle, const String& change_note){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamAPICall_t api_call = SteamUGC()->SubmitItemUpdate(handle, change_note.utf8().get_data());
		callResultItemUpdate.Set(api_call, this, &SteamServer::item_updated);
	}
}

//! Subscribe to a workshop item. It will be downloaded and installed as soon as possible.
void SteamServer::subscribeItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->SubscribeItem(file_id);
		callResultSubscribeItem.Set(api_call, this, &SteamServer::subscribe_item);
	}
}

//! SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends.
void SteamServer::suspendDownloads(bool suspend){
	if(SteamUGC() != NULL){
		SteamUGC()->SuspendDownloads(suspend);
	}
}

//! Unsubscribe from a workshop item. This will result in the item being removed after the game quits.
void SteamServer::unsubscribeItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->UnsubscribeItem(file_id);
		callResultUnsubscribeItem.Set(api_call, this, &SteamServer::unsubscribe_item);
	}
}

//! Updates an existing additional preview file for the item.
bool SteamServer::updateItemPreviewFile(uint64_t update_handle, uint32 index, const String& preview_file){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->UpdateItemPreviewFile(handle, index, preview_file.utf8().get_data());
}

//! Updates an additional video preview from YouTube for the item.
bool SteamServer::updateItemPreviewVideo(uint64_t update_handle, uint32 index, const String& video_id){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->UpdateItemPreviewVideo(handle, index, video_id.utf8().get_data());
}

//! Show the app's latest Workshop EULA to the user in an overlay window, where they can accept it or not.
bool SteamServer::showWorkshopEULA(){
	if(SteamUGC() == NULL){
		return false;
	}
	return SteamUGC()->ShowWorkshopEULA();
}

//! Retrieve information related to the user's acceptance or not of the app's specific Workshop EULA.
void SteamServer::getWorkshopEULAStatus(){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->GetWorkshopEULAStatus();
		callResultWorkshopEULAStatus.Set(api_call, this, &SteamServer::workshop_eula_status);
	}
}

// Set the time range this item was created.
bool SteamServer::setTimeCreatedDateRange(uint64_t update_handle, uint32 start, uint32 end){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetTimeCreatedDateRange(handle, start, end);
}

// Set the time range this item was updated.
bool SteamServer::setTimeUpdatedDateRange(uint64_t update_handle, uint32 start, uint32 end){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetTimeUpdatedDateRange(handle, start, end);
}


/////////////////////////////////////////////////
///// UTILS
/////////////////////////////////////////////////
//
//! Filters the provided input message and places the filtered result into pchOutFilteredText.
String SteamServer::filterText(int context, uint64_t steam_id, const String& message){
	String new_message = "";
	if(SteamUtils() != NULL){
		char *filtered = new char[2048];
		CSteamID source_id = (uint64)steam_id;
		SteamUtils()->FilterText((ETextFilteringContext)context, source_id, message.utf8().get_data(), filtered, strlen(filtered)+1);
		new_message = filtered;
		delete[] filtered;
	}
	return new_message;
}

//! Used to get the failure reason of a call result. The primary usage for this function is debugging. The failure reasons are typically out of your control and tend to not be very important. Just keep retrying your API Call until it works.
String SteamServer::getAPICallFailureReason(){
	if(SteamUtils() == NULL){
		return "ERROR: Steam Utils not present.";
	}
	int failure = SteamUtils()->GetAPICallFailureReason(api_handle);
	// Parse the failure
	if(failure == k_ESteamAPICallFailureSteamGone){
		return "The local Steam process has stopped responding, it may have been forcefully closed or is frozen.";
	}
	else if(failure == k_ESteamAPICallFailureNetworkFailure){
		return "The network connection to the Steam servers has been lost, or was already broken.";
	}
	else if(failure == k_ESteamAPICallFailureInvalidHandle){
		return "The SteamAPICall_t handle passed in no longer exists.";
	}
	else if(failure == k_ESteamAPICallFailureMismatchedCallback){
		return "GetAPICallResult was called with the wrong callback type for this API call.";
	}
	else{
		return "No failure.";
	}
}

//! Get the Steam ID of the running application/game.
uint32_t SteamServer::getAppID(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetAppID();
}

//! Get the amount of battery power, clearly for laptops.
int SteamServer::getCurrentBatteryPower(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetCurrentBatteryPower();
}

//! Gets the image bytes from an image handle.
Dictionary SteamServer::getImageRGBA(int image){
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

//! Gets the size of a Steam image handle.
Dictionary SteamServer::getImageSize(int image){
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

//! Returns the number of IPC calls made since the last time this function was called.
uint32 SteamServer::getIPCCallCount(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetIPCCallCount();
}

//! Get the user's country by IP.
String SteamServer::getIPCountry(){
	if(SteamUtils() == NULL){
		return "";
	}
	return SteamUtils()->GetIPCountry();
}

//! Return amount of time, in seconds, user has spent in this session.
int SteamServer::getSecondsSinceAppActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceAppActive();
}

//! Returns the number of seconds since the user last moved the mouse.
int SteamServer::getSecondsSinceComputerActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceComputerActive();
}

//! Get the actual time.
int SteamServer::getServerRealTime(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetServerRealTime();
}

//! Get the Steam user interface language.
String SteamServer::getSteamUILanguage(){
	if(SteamUtils() == NULL){
		return "";
	}
	return SteamUtils()->GetSteamUILanguage();
}

//! Initializes text filtering. Returns false if filtering is unavailable for the language the user is currently running in. If the language is unsupported, the FilterText API will act as a passthrough.
bool SteamServer::initFilterText(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->InitFilterText();
}

//! Checks if an API Call is completed. Provides the backend of the CallResult wrapper.
Dictionary SteamServer::isAPICallCompleted(){
	Dictionary completed;
	if(SteamUtils() != NULL){
		bool failed = false;
		bool valid = SteamUtils()->IsAPICallCompleted(api_handle, &failed);
		// Populate the dictionary
		completed["completed"] = valid;
		completed["failed"] = failed;
	}
	return completed;
}

//! Returns true/false if Steam overlay is enabled.
bool SteamServer::isOverlayEnabled(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsOverlayEnabled();
}

//! Returns whether the current launcher is a Steam China launcher. You can cause the client to behave as the Steam China launcher by adding -dev -steamchina to the command line when running Steam.
bool SteamServer::isSteamChinaLauncher(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamChinaLauncher();
}

//! Returns true if currently running on the Steam Deck device
bool SteamServer::isSteamRunningOnSteamDeck(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamRunningOnSteamDeck();
}

//! Returns true if Steam & the Steam Overlay are running in Big Picture mode.
bool SteamServer::isSteamInBigPictureMode(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamInBigPictureMode();
}

//! Is Steam running in VR?
bool SteamServer::isSteamRunningInVR(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->IsSteamRunningInVR();
}

//! Checks if the HMD view will be streamed via Steam In-Home Streaming.
bool SteamServer::isVRHeadsetStreamingEnabled(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsVRHeadsetStreamingEnabled();	
}

//! Checks if the Overlay needs a present. Only required if using event driven render updates.
bool SteamServer::overlayNeedsPresent(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->BOverlayNeedsPresent();
}

//! Sets the inset of the overlay notification from the corner specified by SetOverlayNotificationPosition.
void SteamServer::setOverlayNotificationInset(int horizontal, int vertical){
	if(SteamUtils() != NULL){
		SteamUtils()->SetOverlayNotificationInset(horizontal, vertical);
	}
}

//! Set the position where overlay shows notifications.
void SteamServer::setOverlayNotificationPosition(int pos){
	if((pos >= 0) && (pos < 4) && (SteamUtils() != NULL)){
		SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
	}
}

//! Set whether the HMD content will be streamed via Steam In-Home Streaming.
void SteamServer::setVRHeadsetStreamingEnabled(bool enabled){
	if(SteamUtils() != NULL){
		SteamUtils()->SetVRHeadsetStreamingEnabled(enabled);
	}
}

//! Activates the Big Picture text input dialog which only supports gamepad input.
bool SteamServer::showGamepadTextInput(int input_mode, int line_input_mode, const String& description, uint32 max_text, const String& preset_text){
	if(SteamUtils() == NULL){
		return false;
	}
	// Convert modes
	EGamepadTextInputMode mode;
	if(input_mode == 0){
		mode = k_EGamepadTextInputModeNormal;
	}
	else{
		mode = k_EGamepadTextInputModePassword;
	}
	EGamepadTextInputLineMode lineMode;
	if(line_input_mode == 0){
		lineMode = k_EGamepadTextInputLineModeSingleLine;
	}
	else{
		lineMode = k_EGamepadTextInputLineModeMultipleLines;
	}
	return SteamUtils()->ShowGamepadTextInput(mode, lineMode, description.utf8().get_data(), max_text, preset_text.utf8().get_data());
}

//! Opens a floating keyboard over the game content and sends OS keyboard keys directly to the game.
//! The text field position is specified in pixels relative the origin of the game window and is used to position the floating keyboard in a way that doesn't cover the text field
bool SteamServer::showFloatingGamepadTextInput(int input_mode, int text_field_x_position, int text_field_y_position, int text_field_width, int text_field_height) {
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->ShowFloatingGamepadTextInput((EFloatingGamepadTextInputMode)input_mode, text_field_x_position, text_field_y_position, text_field_width, text_field_height);
}

//! In game launchers that don't have controller support you can call this to have Steam Input translate the controller input into mouse/kb to navigate the launcher
void SteamServer::setGameLauncherMode(bool mode) {
	if(SteamUtils() == NULL){
		return;
	}
	SteamUtils()->SetGameLauncherMode(mode);
}

//! Ask SteamUI to create and render its OpenVR dashboard.
void SteamServer::startVRDashboard(){
	if(SteamUtils() != NULL){
		SteamUtils()->StartVRDashboard();
	}
}

// Dismisses the floating keyboard.
bool SteamServer::dismissFloatingGamepadTextInput(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->DismissFloatingGamepadTextInput();
}


/////////////////////////////////////////////////
///// SIGNALS / CALLBACKS
/////////////////////////////////////////////////
//
// APPS CALLBACKS ///////////////////////////////
//
//! Triggered after the current user gains ownership of DLC and that DLC is installed.
void SteamServer::dlc_installed(DlcInstalled_t* call_data){
	uint32_t app_id = (AppId_t)call_data->m_nAppID;
	emit_signal("dlc_installed", app_id);
}

//! Called after requesting the details of a specific file.
void SteamServer::file_details_result(FileDetailsResult_t* file_data){
	uint32_t result = file_data->m_eResult;
	uint64_t file_size = file_data->m_ulFileSize;
	uint32_t flags = file_data->m_unFlags;
	uint8 *file_hash = new uint8[20];
	file_hash = file_data->m_FileSHA;
	emit_signal("file_details_result", result, file_size, file_hash, flags);
	delete[] file_hash;
}

//! Posted after the user executes a steam url with command line or query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running. The new params can be queried with getLaunchCommandLine and getLaunchQueryParam.
void SteamServer::new_launch_url_parameters(NewUrlLaunchParameters_t* call_data){
	emit_signal("new_launch_url_parameters");
}

//! Purpose: called for games in Timed Trial mode
void SteamServer::timed_trial_status(TimedTrialStatus_t* call_data){
	uint32_t app_id = call_data->m_unAppID;
	bool is_offline = call_data->m_bIsOffline;
	uint32 seconds_allowed = call_data->m_unSecondsAllowed;
	uint32 seconds_played = call_data->m_unSecondsPlayed;
	emit_signal("timed_trial_status", app_id, is_offline, seconds_allowed, seconds_played);
}

// GAME SERVER CALLBACKS ////////////////////////
//
// Logging the game server onto Steam
void SteamServer::server_connect_failure(SteamServerConnectFailure_t* serverData){
	int result = serverData->m_eResult;
	bool retrying = serverData->m_bStillRetrying;
	emit_signal("server_connect_failure", result, retrying);
}

// Server has connected to the Steam back-end; serverData has no fields.
void SteamServer::server_connected(SteamServersConnected_t* serverData){
	emit_signal("server_connected");
}

// Called if the client has lost connection to the Steam servers. Real-time services will be disabled until a matching SteamServersConnected_t has been posted.
void SteamServer::server_disconnected(SteamServersDisconnected_t* serverData){
	int result = serverData->m_eResult;
	emit_signal("server_disconnected", result);
}

// Client has been approved to connect to this game server.
void SteamServer::client_approved(GSClientApprove_t* clientData){
	uint64_t steam_id = clientData->m_SteamID.ConvertToUint64();
	uint64_t owner_id = clientData->m_OwnerSteamID.ConvertToUint64();
	emit_signal("client_approved", steam_id, owner_id);
}

// Client has been denied to connection to this game server.
void SteamServer::client_denied(GSClientDeny_t* clientData){
	uint64_t steam_id = clientData->m_SteamID.ConvertToUint64();
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
		reason = DENY_INCOMPATIBLE_ANTI_CHEAT;
	}
	else if(clientData->m_eDenyReason == k_EDenyMemoryCorruption){
		reason = DENY_MEMORY_CORRUPTION;
	}
	else if(clientData->m_eDenyReason == k_EDenyIncompatibleSoftware){
		reason = DENY_INCOMPATIBLE_SOFTWARE;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionLost){
		reason = DENY_STEAM_CONNECTION_LOST;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionError){
		reason = DENY_STEAM_CONNECTION_ERROR;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamResponseTimedOut){
		reason = DENY_STEAM_RESPONSE_TIMED_OUT;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamValidationStalled){
		reason = DENY_STEAM_VALIDATION_STALLED;
	}
	else{
		reason = DENY_STEAM_OWNER_LEFT_GUEST_USER;
	}
	emit_signal("client_denied", steam_id, reason);
}

// Request the game server should kick the user.
void SteamServer::client_kick(GSClientKick_t* clientData){
	uint64_t steam_id = clientData->m_SteamID.ConvertToUint64();
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
		reason = DENY_INCOMPATIBLE_ANTI_CHEAT;
	}
	else if(clientData->m_eDenyReason == k_EDenyMemoryCorruption){
		reason = DENY_MEMORY_CORRUPTION;
	}
	else if(clientData->m_eDenyReason == k_EDenyIncompatibleSoftware){
		reason = DENY_INCOMPATIBLE_SOFTWARE;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionLost){
		reason = DENY_STEAM_CONNECTION_LOST;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamConnectionError){
		reason = DENY_STEAM_CONNECTION_ERROR;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamResponseTimedOut){
		reason = DENY_STEAM_RESPONSE_TIMED_OUT;
	}
	else if(clientData->m_eDenyReason == k_EDenySteamValidationStalled){
		reason = DENY_STEAM_VALIDATION_STALLED;
	}
	else{
		reason = DENY_STEAM_OWNER_LEFT_GUEST_USER;
	}
	emit_signal("client_kick", steam_id, reason);
}

// Received when the game server requests to be displayed as secure (VAC protected).
// m_bSecure is true if the game server should display itself as secure to users, false otherwise.
void SteamServer::policy_response(GSPolicyResponse_t* policyData){
	uint8 secure = policyData->m_bSecure;
	emit_signal("policy_response", secure);
}

// Sent as a reply to RequestUserGroupStatus().
void SteamServer::client_group_status(GSClientGroupStatus_t* clientData){
	uint64_t steam_id = clientData->m_SteamIDUser.ConvertToUint64();
	uint64_t group_id = clientData->m_SteamIDGroup.ConvertToUint64();
	bool member = clientData->m_bMember;
	bool officer = clientData->m_bOfficer;
	emit_signal("client_group_status", steam_id, group_id, member, officer);
}

// Sent as a reply to AssociateWithClan().
void SteamServer::associate_clan(AssociateWithClanResult_t* clanData){
	int result;
	if(clanData->m_eResult == k_EResultOK){
		result = RESULT_OK;
	}
	else{
		result = RESULT_FAIL;
	}
	emit_signal("associate_clan", result);
}

// Sent as a reply to ComputeNewPlayerCompatibility().
void SteamServer::player_compat(ComputeNewPlayerCompatibilityResult_t* playerData){
	int result;
	if(playerData->m_eResult == k_EResultNoConnection){
		result = RESULT_NO_CONNECTION;
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
	int players_dont_like_candidate = playerData->m_cPlayersThatDontLikeCandidate;
	int players_candidate_doesnt_like = playerData->m_cPlayersThatCandidateDoesntLike;
	int clan_players_dont_like_candidate = playerData->m_cClanPlayersThatDontLikeCandidate;
	uint64_t steam_id = playerData->m_SteamIDCandidate.ConvertToUint64();
	emit_signal("player_compat", result, players_dont_like_candidate, players_candidate_doesnt_like, clan_players_dont_like_candidate, steam_id);
}

// GAME SERVER STATS CALLBACKS //////////////////
//
// Result when getting the latests stats and achievements for a user from the server.
void SteamServer::stats_received(GSStatsReceived_t* callData, bool bioFailure){
	EResult result = callData->m_eResult;
	uint64_t steam_id = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("stats_received", result, steam_id);
}

// Result of a request to store the user stats.
void SteamServer::stats_stored(GSStatsStored_t* callData){
	EResult result = callData->m_eResult;
	uint64_t steam_id = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("stats_stored", result, steam_id);
}

// Callback indicating that a user's stats have been unloaded.
void SteamServer::stats_unloaded(GSStatsUnloaded_t* callData){
	uint64_t steam_id = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("stats_unloaded", steam_id);
}

// HTTP CALLBACKS ///////////////////////////////
//
//! Result when an HTTP request completes. If you're using GetHTTPStreamingResponseBodyData then you should be using the HTTPRequestHeadersReceived_t or HTTPRequestDataReceived_t.
void SteamServer::http_request_completed(HTTPRequestCompleted_t* call_data){
	uint32 cookie_handle = call_data->m_hRequest;
	uint64_t context_value = call_data->m_ulContextValue;
	bool request_success = call_data->m_bRequestSuccessful;
	int status_code = call_data->m_eStatusCode;
	uint32 body_size = call_data->m_unBodySize;
	emit_signal("http_request_completed", cookie_handle, context_value, request_success, status_code, body_size);
}

//! Triggered when a chunk of data is received from a streaming HTTP request.
void SteamServer::http_request_data_received(HTTPRequestDataReceived_t* call_data){
	uint32 cookie_handle = call_data->m_hRequest;
	uint64_t context_value = call_data->m_ulContextValue;
	uint32 offset = call_data->m_cOffset;
	uint32 bytes_received = call_data->m_cBytesReceived;
	emit_signal("http_request_data_received", cookie_handle, context_value, offset, bytes_received);
}

//! Triggered when HTTP headers are received from a streaming HTTP request.
void SteamServer::http_request_headers_received(HTTPRequestHeadersReceived_t* call_data){
	uint32 cookie_handle = call_data->m_hRequest;
	uint64_t context_value = call_data->m_ulContextValue;
	emit_signal("http_request_headers_received", cookie_handle, context_value);
}

// INVENTORY CALLBACKS //////////////////////////
//
//! This callback is triggered whenever item definitions have been updated, which could be in response to LoadItemDefinitions or any time new item definitions are available (eg, from the dynamic addition of new item types while players are still in-game).
void SteamServer::inventory_definition_update(SteamInventoryDefinitionUpdate_t *call_data){
	// Create the return array
	Array definitions;
	// Set the array size variable
	uint32 size = 0;
	// Get the item defition IDs
	if(SteamInventory()->GetItemDefinitionIDs(NULL, &size)){
		SteamItemDef_t *id_array = new SteamItemDef_t[size];
		if(SteamInventory()->GetItemDefinitionIDs(id_array, &size)){
			// Loop through the temporary array and populate the return array
			for(uint32 i = 0; i < size; i++){
				definitions.append(id_array[i]);
			}
		}
		// Delete the temporary array
		delete[] id_array;
	}
	// Return the item array as a signal
	emit_signal("inventory_defintion_update", definitions);
}

//! Triggered when GetAllItems successfully returns a result which is newer / fresher than the last known result. (It will not trigger if the inventory hasn't changed, or if results from two overlapping calls are reversed in flight and the earlier result is already known to be stale/out-of-date.)
//! The regular SteamInventoryResultReady_t callback will still be triggered immediately afterwards; this is an additional notification for your convenience.
void SteamServer::inventory_full_update(SteamInventoryFullUpdate_t *call_data){
	// Set the handle
	inventory_handle = call_data->m_handle;
	// Send the handle back to the user
	emit_signal("inventory_full_update", call_data->m_handle);
}

//! This is fired whenever an inventory result transitions from k_EResultPending to any other completed state, see GetResultStatus for the complete list of states. There will always be exactly one callback per handle.
void SteamServer::inventory_result_ready(SteamInventoryResultReady_t *call_data){
	// Get the result
	int result = call_data->m_result;
	// Get the handle and pass it over
	inventory_handle = call_data->m_handle;
	emit_signal("inventory_result_ready", result, inventory_handle);
}

// NETWORKING CALLBACKS /////////////////////////
//
//! Called when packets can't get through to the specified user. All queued packets unsent at this point will be dropped, further attempts to send will retry making the connection (but will be dropped if we fail again).
void SteamServer::p2p_session_connect_fail(P2PSessionConnectFail_t* call_data) {
	uint64_t steam_id_remote = call_data->m_steamIDRemote.ConvertToUint64();
	uint8_t session_error = call_data->m_eP2PSessionError;
	emit_signal("p2p_session_connect_fail", steam_id_remote, session_error);
}

//! A user wants to communicate with us over the P2P channel via the sendP2PPacket. In response, a call to acceptP2PSessionWithUser needs to be made, if you want to open the network channel with them.
void SteamServer::p2p_session_request(P2PSessionRequest_t* call_data){
	uint64_t steam_id_remote = call_data->m_steamIDRemote.ConvertToUint64();
	emit_signal("p2p_session_request", steam_id_remote);
}

// NETWORKING MESSAGES CALLBACKS ////////////////
//
//! Posted when a remote host is sending us a message, and we do not already have a session with them.
void SteamServer::network_messages_session_request(SteamNetworkingMessagesSessionRequest_t* call_data){
	SteamNetworkingIdentity remote = call_data->m_identityRemote;
	char identity[STEAM_BUFFER_SIZE];
	remote.ToString(identity, STEAM_BUFFER_SIZE);
	emit_signal("network_messages_session_request", identity);
}

//! Posted when we fail to establish a connection, or we detect that communications have been disrupted it an unusual way.
void SteamServer::network_messages_session_failed(SteamNetworkingMessagesSessionFailed_t* call_data){
	SteamNetConnectionInfo_t info = call_data->m_info;
	// Parse out the reason for failure
	int reason = info.m_eEndReason;
	emit_signal("network_messages_session_failed", reason);
}

// NETWORKING SOCKETS CALLBACKS /////////////////
//
//! This callback is posted whenever a connection is created, destroyed, or changes state. The m_info field will contain a complete description of the connection at the time the change occurred and the callback was posted. In particular, m_info.m_eState will have the new connection state.
void SteamServer::network_connection_status_changed(SteamNetConnectionStatusChangedCallback_t* call_data){
	// Connection handle.
	uint64_t connect_handle = call_data->m_hConn;
	// Full connection info.
	SteamNetConnectionInfo_t connection_info = call_data->m_info;
	// Move connection info into a dictionary
	Dictionary connection;
	char identity[STEAM_BUFFER_SIZE];
	connection_info.m_identityRemote.ToString(identity, STEAM_BUFFER_SIZE);
	connection["identity"] = identity;
	connection["user_data"] = (uint64_t)connection_info.m_nUserData;
	connection["listen_socket"] = connection_info.m_hListenSocket;
	char ip_address[STEAM_BUFFER_SIZE];
	connection_info.m_addrRemote.ToString(ip_address, STEAM_BUFFER_SIZE, true);
	connection["remote_address"] = ip_address;
	connection["remote_pop"] = connection_info.m_idPOPRemote;
	connection["pop_relay"] = connection_info.m_idPOPRelay;
	connection["connection_state"] = connection_info.m_eState;
	connection["end_reason"] = connection_info.m_eEndReason;
	connection["end_debug"] = connection_info.m_szEndDebug;
	connection["debug_description"] = connection_info.m_szConnectionDescription;
	// Previous state (current state is in m_info.m_eState).
	int old_state = call_data->m_eOldState;
	// Send the data back via signal
	emit_signal("network_connection_status_changed", connect_handle, connection, old_state);
}

//! This callback is posted whenever the state of our readiness changes.
void SteamServer::network_authentication_status(SteamNetAuthenticationStatus_t* call_data){
	// Status.
	int available = call_data->m_eAvail;
	// Non-localized English language status. For diagnostic / debugging purposes only.
	char *debug_message = new char[256];
	sprintf(debug_message, "%s", call_data->m_debugMsg);
	// Send the data back via signal
	emit_signal("network_authentication_status", available, debug_message);
	delete[] debug_message;
}

// A struct used to describe a "fake IP" we have been assigned to use as an identifier.
// This callback is posted when ISteamNetworkingSoockets::BeginAsyncRequestFakeIP completes.
void SteamServer::fake_ip_result(SteamNetworkingFakeIPResult_t* call_data){
	int result = call_data->m_eResult;
	// Pass this new networking identity to the map
	networking_identities["fake_ip_identity"] = call_data->m_identity;
	uint32 ip = call_data->m_unIP;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char fake_ip[16];
	for(int i = 0; i < NBYTES; i++){
		octet[i] = ip >> (i * 8);
	}
	sprintf(fake_ip, "%d.%d.%d.%d", octet[0], octet[1], octet[2], octet[3]);
	// Get the ports as an array
	Array port_list;
	uint16* ports = call_data->m_unPorts;
	for(uint16 i = 0; i < sizeof(ports); i++){
		port_list.append(ports[i]);
	}
	emit_signal("fake_ip_result", result, "fake_ip_identity", fake_ip, port_list);
}

// NETWORKING UTILS CALLBACKS ///////////////////
//
//! A struct used to describe our readiness to use the relay network.
void SteamServer::relay_network_status(SteamRelayNetworkStatus_t* call_data){
	int available = call_data->m_eAvail;
	int ping_measurement = call_data->m_bPingMeasurementInProgress;
	int available_config = call_data->m_eAvailNetworkConfig;
	int available_relay = call_data->m_eAvailAnyRelay;
	char *debug_message = new char[256];
	sprintf(debug_message, "%s", call_data->m_debugMsg);
//	debug_message = call_data->m_debugMsg;
	emit_signal("relay_network_status", available, ping_measurement, available_config, available_relay, debug_message);
	delete[] debug_message;
}

// REMOTE STORAGE CALLBACKS /////////////////////
//
//! Purpose: one or more files for this app have changed locally after syncing to remote session changes.
//! Note: only posted if this happens DURING the local app session.
void SteamServer::local_file_changed(RemoteStorageLocalFileChange_t* call_data){
	emit_signal("local_file_changed");
}

// UGC CALLBACKS ////////////////////////////////
//
//! Called when a workshop item has been downloaded.
void SteamServer::item_downloaded(DownloadItemResult_t* call_data){
	EResult result = call_data->m_eResult;
	PublishedFileId_t file_id = call_data->m_nPublishedFileId;
	AppId_t app_id = call_data->m_unAppID;
	emit_signal("item_downloaded", result, (uint64_t)file_id, (uint32_t)app_id);
}

//! Called when a workshop item has been installed or updated.
void SteamServer::item_installed(ItemInstalled_t* call_data){
	AppId_t app_id = call_data->m_unAppID;
	PublishedFileId_t file_id = call_data->m_nPublishedFileId;
	emit_signal("item_installed", app_id, (uint64_t)file_id);
}

//! Purpose: signal that the list of subscribed items changed.
void SteamServer::user_subscribed_items_list_changed(UserSubscribedItemsListChanged_t* call_data){
	uint32 app_id = call_data->m_nAppID;
	emit_signal("user_subscribed_items_list_changed", app_id);
}

// UTILITY CALLBACKS ////////////////////////////
//
//! Called when the big picture gamepad text input has been closed.
void SteamServer::gamepad_text_input_dismissed(GamepadTextInputDismissed_t* call_data){
	const uint32 buffer_length = 1024+1;
	char *text = new char[buffer_length];
	uint32 length = buffer_length;
	if(call_data->m_bSubmitted){
		SteamUtils()->GetEnteredGamepadTextInput(text, buffer_length);
		length = SteamUtils()->GetEnteredGamepadTextLength();
	}
	emit_signal("gamepad_text_input_dismissed", call_data->m_bSubmitted, String::utf8(text, (int)length));
	delete[] text;
}

//! Called when the country of the user changed. The country should be updated with getIPCountry.
void SteamServer::ip_country(IPCountry_t* call_data){
	emit_signal("ip_country");
}

//! Called when running on a laptop and less than 10 minutes of battery is left, and then fires then every minute afterwards.
void SteamServer::low_power(LowBatteryPower_t* timeLeft){
	uint8 power = timeLeft->m_nMinutesBatteryLeft;
	emit_signal("low_power", power);
}

//! Called when a SteamAPICall_t has completed (or failed)
void SteamServer::steam_api_call_completed(SteamAPICallCompleted_t* call_data){
	uint64_t async_call = call_data->m_hAsyncCall;
	int callback = call_data->m_iCallback;
	uint32 parameter = call_data->m_cubParam;
	emit_signal("steam_api_call_completed", async_call, callback, parameter);
}

//! Called when Steam wants to shutdown.
void SteamServer::steam_shutdown(SteamShutdown_t* call_data){
	emit_signal("steam_shutdown");
}

//! Sent after the device returns from sleep/suspend mode.
void SteamServer::app_resuming_from_suspend(AppResumingFromSuspend_t *call_data) {
	emit_signal("app_resuming_from_suspend");
}

//! Sent after the device returns from sleep/suspend mode.
void SteamServer::floating_gamepad_text_input_dismissed(FloatingGamepadTextInputDismissed_t *call_data) {
	emit_signal("floating_gamepad_text_input_dismissed");
}


/////////////////////////////////////////////////
///// SIGNALS / CALL RESULTS ////////////////////
/////////////////////////////////////////////////
//
// STEAMWORKS ERROR SIGNAL //////////////////////
//
//! Intended to serve as generic error messaging for failed call results
void SteamServer::steamworksError(const String& failed_signal){
	// Emit the signal to inform the user of the failure
	emit_signal("steamworks_error", failed_signal, "io failure");
}

// INVENTORY CALL RESULTS ///////////////////////
//
//! Returned when you have requested the list of "eligible" promo items that can be manually granted to the given user. These are promo items of type "manual" that won't be granted automatically.
void SteamServer::inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("inventory_eligible_promo_item");
	}
	else{
		// Clean up call data
		CSteamID steam_id = call_data->m_steamID;
		int result = call_data->m_result;
		int eligible = call_data->m_numEligiblePromoItemDefs;
		bool cached = call_data->m_bCachedData;
		// Create the return array
		Array definitions;
		// Create the temporary ID array
		SteamItemDef_t *id_array = new SteamItemDef_t[eligible];
		// Convert eligible size
		uint32 array_size = (int)eligible;
		// Get the list
		if(SteamInventory()->GetEligiblePromoItemDefinitionIDs(steam_id, id_array, &array_size)){
			// Loop through the temporary array and populate the return array
			for(int i = 0; i < eligible; i++){
				definitions.append(id_array[i]);
			}
		}
		// Delete the temporary array
		delete[] id_array;
		// Return the item array as a signal
		emit_signal("inventory_eligible_promo_Item", result, cached, definitions);
	}
}

//! Returned after StartPurchase is called.
void SteamServer::inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("inventory_start_purchase_result");
	}
	else{
		if(call_data->m_result == k_EResultOK){
			uint64_t order_id = call_data->m_ulOrderID;
			uint64_t transaction_id = call_data->m_ulTransID;
			emit_signal("inventory_start_purchase_result", "success", order_id, transaction_id);
		}
		else{
			emit_signal("inventory_start_purchase_result", "failure", 0, 0);
		}
	}
}

//! Returned after RequestPrices is called.
void SteamServer::inventory_request_prices_result(SteamInventoryRequestPricesResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("inventory_request_prices_result");
	}
	else{
		int result = call_data->m_result;
		String currency = call_data->m_rgchCurrency;
		emit_signal("inventory_request_prices_result", result, currency);
	}
}


// REMOTE STORAGE CALL RESULTS //////////////////
//
//! Response when reading a file asyncrounously with FileReadAsync.
void SteamServer::file_read_async_complete(RemoteStorageFileReadAsyncComplete_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("file_read_async_complete");
	}
	else{
		uint64_t handle = call_data->m_hFileReadAsync;
		int result = call_data->m_eResult;
		uint32 offset = call_data->m_nOffset;
		uint32 read = call_data->m_cubRead;
		// Was read complete?
		PoolByteArray buffer;
		buffer.resize(read);
		bool complete = SteamRemoteStorage()->FileReadAsyncComplete(handle, buffer.write().ptr(), read);
		// Create a dictionary and populate it with the results
		Dictionary file_read;
		file_read["result"] = result;
		file_read["handle"] = handle;
		file_read["buffer"] = buffer;
		file_read["offset"] = offset;
		file_read["read"] = read;
		file_read["complete"] = complete;
		emit_signal("file_read_async_complete", file_read);
	}
}

//! Response to a file being shared.
void SteamServer::file_share_result(RemoteStorageFileShareResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("file_share_result");
	}
	else{
		int result = call_data->m_eResult;
		uint64_t handle = call_data->m_hFile;
		char name[k_cchFilenameMax];
		strcpy(name, call_data->m_rgchFilename);
		emit_signal("file_share_result", result, handle, name);
	}
}

//! Response when writing a file asyncrounously with FileWriteAsync.
void SteamServer::file_write_async_complete(RemoteStorageFileWriteAsyncComplete_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("file_write_async_complete");
	}
	else{
		int result = call_data->m_eResult;
		emit_signal("file_write_async_complete", result);
	}
}

//! Response when downloading UGC
void SteamServer::download_ugc_result(RemoteStorageDownloadUGCResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("download_ugc_result");
	}
	else{
		int result = call_data->m_eResult;
		uint64_t handle = call_data->m_hFile;
		uint32_t app_id = call_data->m_nAppID;
		int32 size = call_data->m_nSizeInBytes;
		char filename[k_cchFilenameMax];
		strcpy(filename, call_data->m_pchFileName);
		uint64_t owner_id = call_data->m_ulSteamIDOwner;
		// Pass some variable to download dictionary to bypass argument limit
		Dictionary download_data;
		download_data["handle"] = handle;
		download_data["app_id"] = app_id;
		download_data["size"] = size;
		download_data["filename"] = filename;
		download_data["owner_id"] = owner_id;
		emit_signal("download_ugc_result", result, download_data);
	}
}

//! Called when the user has unsubscribed from a piece of UGC. Result from ISteamUGC::UnsubscribeItem.
void SteamServer::unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("unsubscribe_item");
	}
	else{
		int result = call_data->m_eResult;
		int file_id = call_data->m_nPublishedFileId;
		emit_signal("unsubscribe_item", result, file_id);
	}
}

//! Called when the user has subscribed to a piece of UGC. Result from ISteamUGC::SubscribeItem.
void SteamServer::subscribe_item(RemoteStorageSubscribePublishedFileResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("subscribe_item");
	}
	else{
		int result = call_data->m_eResult;
		int file_id = call_data->m_nPublishedFileId;
		emit_signal("subscribe_item", result, file_id);
	}
}

// UGC CALL RESULTS /////////////////////////////
//
//! The result of a call to AddAppDependency.
void SteamServer::add_app_dependency_result(AddAppDependencyResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("add_app_dependency_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		AppId_t app_id = call_data->m_nAppID;
		emit_signal("add_app_dependency_result", result, (uint64_t)file_id, (uint32_t)app_id);
	}
}

//! The result of a call to AddDependency.
void SteamServer::add_ugc_dependency_result(AddUGCDependencyResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("add_ugc_dependency_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		PublishedFileId_t child_id = call_data->m_nChildPublishedFileId;
		emit_signal("add_ugc_dependency_result", result, (uint64_t)file_id, (uint64_t)child_id);
	}
}

//! Result of a workshop item being created.
void SteamServer::item_created(CreateItemResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("item_created");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		bool accept_tos = call_data->m_bUserNeedsToAcceptWorkshopLegalAgreement;
		emit_signal("item_created", result, (uint64_t)file_id, accept_tos);
	}
}

//! Called when getting the app dependencies for an item.
void SteamServer::get_app_dependencies_result(GetAppDependenciesResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("get_app_dependencies_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
//		AppId_t app_id = call_data->m_rgAppIDs;
		uint32 app_dependencies = call_data->m_nNumAppDependencies;
		uint32 total_app_dependencies = call_data->m_nTotalNumAppDependencies;
//		emit_signal("get_app_dependencies_result", result, (uint64_t)file_id, app_id, appDependencies, totalAppDependencies);
		emit_signal("get_app_dependencies_result", result, (uint64_t)file_id, app_dependencies, total_app_dependencies);
	}
}

//! Called when an attempt at deleting an item completes.
void SteamServer::item_deleted(DeleteItemResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("item_deleted");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		emit_signal("item_deleted", result, (uint64_t)file_id);
	}
}

//! Called when getting the users vote status on an item.
void SteamServer::get_item_vote_result(GetUserItemVoteResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("get_item_vote_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		bool vote_up = call_data->m_bVotedUp;
		bool vote_down = call_data->m_bVotedDown;
		bool vote_skipped = call_data->m_bVoteSkipped;
		emit_signal("get_item_vote_result", result, (uint64_t)file_id, vote_up, vote_down, vote_skipped);
	}
}

//! Purpose: The result of a call to RemoveAppDependency.
void SteamServer::remove_app_dependency_result(RemoveAppDependencyResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("remove_app_dependency_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		AppId_t app_id = call_data->m_nAppID;
		emit_signal("remove_app_dependency_result", result, (uint64_t)file_id, (uint32_t)app_id);
	}
}

//! Purpose: The result of a call to RemoveDependency.
void SteamServer::remove_ugc_dependency_result(RemoveUGCDependencyResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("remove_ugc_dependency_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		PublishedFileId_t child_id = call_data->m_nChildPublishedFileId;
		emit_signal("remove_ugc_dependency_result", result, (uint64_t)file_id, (uint64_t)child_id);
	}
}

//! Called when the user has voted on an item.
void SteamServer::set_user_item_vote(SetUserItemVoteResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("set_user_item_vote");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		bool vote_up = call_data->m_bVoteUp;
		emit_signal("set_user_item_vote", result, (uint64_t)file_id, vote_up);
	}
}

//! Called when workshop item playtime tracking has started.
void SteamServer::start_playtime_tracking(StartPlaytimeTrackingResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("start_playtime_tracking");
	}
	else{
		EResult result = call_data->m_eResult;
		emit_signal("start_playtime_tracking", result);
	}
}

//! Called when a UGC query request completes.
void SteamServer::ugc_query_completed(SteamUGCQueryCompleted_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("ugc_query_completed");
	}
	else{
		UGCQueryHandle_t handle = call_data->m_handle;
		EResult result = call_data->m_eResult;
		uint32 results_returned = call_data->m_unNumResultsReturned;
		uint32 total_matching = call_data->m_unTotalMatchingResults;
		bool cached = call_data->m_bCachedData;
		emit_signal("ugc_query_completed", (uint64_t)handle, result, results_returned, total_matching, cached);
	}
}

//! Called when workshop item playtime tracking has stopped.
void SteamServer::stop_playtime_tracking(StopPlaytimeTrackingResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("stop_playtime_tracking");
	}
	else{
		EResult result = call_data->m_eResult;
		emit_signal("stop_playtime_tracking", result);
	}
}

//! Result of a workshop item being updated.
void SteamServer::item_updated(SubmitItemUpdateResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("item_updated");
	}
	else{
		EResult result = call_data->m_eResult;
		bool accept_tos = call_data->m_bUserNeedsToAcceptWorkshopLegalAgreement;
		emit_signal("item_updated", result, accept_tos);
	}
}

//! Called when the user has added or removed an item to/from their favorites.
void SteamServer::user_favorite_items_list_changed(UserFavoriteItemsListChanged_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("user_favorite_items_list_changed");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		bool was_add_request = call_data->m_bWasAddRequest;
		emit_signal("user_favorite_items_list_changed", result, (uint64_t)file_id, was_add_request);
	}
}

//! Purpose: Status of the user's acceptable/rejection of the app's specific Workshop EULA.
void SteamServer::workshop_eula_status(WorkshopEULAStatus_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("workshop_eula_status");
	}
	else{
		int result = call_data->m_eResult;
		uint32 app_id = call_data->m_nAppID;
		// Slim down signal arguments since Godot seems to limit them to six max
		Dictionary eula_data;
		eula_data["version"] = call_data->m_unVersion;			// int
		eula_data["action"] = call_data->m_rtAction;			// int
		eula_data["accepted"] = call_data->m_bAccepted;			// bool
		eula_data["needs_action"] = call_data->m_bNeedsAction;	// bool
		emit_signal("workshop_eula_status", result, app_id, eula_data);
	}
}

// UTILITY CALL RESULTS /////////////////////////
//
//! CallResult for checkFileSignature.
void SteamServer::check_file_signature(CheckFileSignature_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("check_file_signature");
	}
	else{
		String signature;
		if(call_data->m_eCheckFileSignature == k_ECheckFileSignatureNoSignaturesFoundForThisApp){
			signature = "app not signed";
		}
		else if(call_data->m_eCheckFileSignature == k_ECheckFileSignatureNoSignaturesFoundForThisFile){
			signature = "file not signed";
		}
		else if(call_data->m_eCheckFileSignature == k_ECheckFileSignatureFileNotFound){
			signature = "file does not exist";		
		}
		else if(call_data->m_eCheckFileSignature == k_ECheckFileSignatureInvalidSignature){
			signature = "signature invalid";
		}
		else if(call_data->m_eCheckFileSignature == k_ECheckFileSignatureValidSignature){
			signature = "valid";		
		}
		else{
			signature = "invalid response";
		}
		emit_signal("check_file_signature", signature);
	}
}

/////////////////////////////////////////////////
///// BIND METHODS //////////////////////////////
/////////////////////////////////////////////////
//
void SteamServer::_bind_methods(){
	/////////////////////////////////////////////
	// FUNCTION BINDS ///////////////////////////
	/////////////////////////////////////////////
	//
	// MAIN BIND METHODS ////////////////////////
	ClassDB::bind_method("run_callbacks", &SteamServer::run_callbacks);
	ClassDB::bind_method(D_METHOD("serverInit", "ip", "game_port", "query_port", "server_mode", "version_string"), &SteamServer::serverInit);
	ClassDB::bind_method("serverReleaseCurrentThreadMemory", &SteamServer::serverReleaseCurrentThreadMemory);
	ClassDB::bind_method("serverShutdown", &SteamServer::serverShutdown);

	// APPS BIND METHODS ////////////////////////
	ClassDB::bind_method("getDLCDataByIndex", &SteamServer::getDLCDataByIndex);
	ClassDB::bind_method(D_METHOD("isAppInstalled", "app_id"), &SteamServer::isAppInstalled);
	ClassDB::bind_method("isCybercafe", &SteamServer::isCybercafe);
	ClassDB::bind_method(D_METHOD("isDLCInstalled", "dlc_id"), &SteamServer::isDLCInstalled);
	ClassDB::bind_method("isLowViolence", &SteamServer::isLowViolence);
	ClassDB::bind_method("isSubscribed", &SteamServer::isSubscribed);
	ClassDB::bind_method(D_METHOD("isSubscribedApp", "app_id"), &SteamServer::isSubscribedApp);
	ClassDB::bind_method("isSubscribedFromFamilySharing", &SteamServer::isSubscribedFromFamilySharing);
	ClassDB::bind_method("isSubscribedFromFreeWeekend", &SteamServer::isSubscribedFromFreeWeekend);
	ClassDB::bind_method("isTimedTrial", &SteamServer::isTimedTrial);
	ClassDB::bind_method("isVACBanned", &SteamServer::isVACBanned);
	ClassDB::bind_method("getAppBuildId", &SteamServer::getAppBuildId);
	ClassDB::bind_method(D_METHOD("getAppInstallDir", "app_id"), &SteamServer::getAppInstallDir);
	ClassDB::bind_method("getAppOwner", &SteamServer::getAppOwner);
	ClassDB::bind_method("getAvailableGameLanguages", &SteamServer::getAvailableGameLanguages);
	ClassDB::bind_method("getCurrentBetaName", &SteamServer::getCurrentBetaName);
	ClassDB::bind_method("getCurrentGameLanguage", &SteamServer::getCurrentGameLanguage);
	ClassDB::bind_method("getDLCCount", &SteamServer::getDLCCount);
	ClassDB::bind_method(D_METHOD("getDLCDownloadProgress", "dlc_id"), &SteamServer::getDLCDownloadProgress);
	ClassDB::bind_method(D_METHOD("getEarliestPurchaseUnixTime", "app_id"), &SteamServer::getEarliestPurchaseUnixTime);
	ClassDB::bind_method(D_METHOD("getFileDetails", "filename"), &SteamServer::getFileDetails);
	ClassDB::bind_method(D_METHOD("getInstalledDepots", "app_id"), &SteamServer::getInstalledDepots);
	ClassDB::bind_method("getLaunchCommandLine", &SteamServer::getLaunchCommandLine);
	ClassDB::bind_method(D_METHOD("getLaunchQueryParam", "key"), &SteamServer::getLaunchQueryParam);
	ClassDB::bind_method(D_METHOD("installDLC", "dlc_id"), &SteamServer::installDLC);
	ClassDB::bind_method(D_METHOD("markContentCorrupt", "missing_files_only"), &SteamServer::markContentCorrupt);
	ClassDB::bind_method(D_METHOD("setDLCContext", "app_id"), &SteamServer::setDLCContext);
	ClassDB::bind_method(D_METHOD("uninstallDLC", "dlc_id"), &SteamServer::uninstallDLC);

	// GAME SERVER BIND METHODS /////////////////
	ClassDB::bind_method(D_METHOD("associateWithClan", "clan_id"), &SteamServer::associateWithClan);
	ClassDB::bind_method(D_METHOD("beginAuthSession", "ticket", "ticket_size", "steam_id"), &SteamServer::beginAuthSession);
	ClassDB::bind_method("loggedOn", &SteamServer::loggedOn);
	ClassDB::bind_method("secure", &SteamServer::secure);
	ClassDB::bind_method(D_METHOD("cancelAuthTicket", "auth_ticket"), &SteamServer::cancelAuthTicket);
	ClassDB::bind_method("clearAllKeyValues", &SteamServer::clearAllKeyValues);
	ClassDB::bind_method(D_METHOD("computeNewPlayerCompatibility", "steam_id"), &SteamServer::computeNewPlayerCompatibility);
	ClassDB::bind_method(D_METHOD("setAdvertiseServerActive", "active"), &SteamServer::setAdvertiseServerActive);
	ClassDB::bind_method(D_METHOD("endAuthSession", "steam_id"), &SteamServer::endAuthSession);
	ClassDB::bind_method("getAuthSessionTicket", &SteamServer::getAuthSessionTicket);
	ClassDB::bind_method("getNextOutgoingPacket", &SteamServer::getNextOutgoingPacket);
	ClassDB::bind_method("getPublicIP", &SteamServer::getPublicIP);
	ClassDB::bind_method("getSteamID", &SteamServer::getSteamID);
	ClassDB::bind_method(D_METHOD("handleIncomingPacket", "packet", "ip", "port"), &SteamServer::handleIncomingPacket);
	ClassDB::bind_method("logOff", &SteamServer::logOff);
	ClassDB::bind_method(D_METHOD("logOn", "token"), &SteamServer::logOn);
	ClassDB::bind_method("logOnAnonymous", &SteamServer::logOnAnonymous);
	ClassDB::bind_method(D_METHOD("requestUserGroupStatus", "steam_id", "group_id"), &SteamServer::requestUserGroupStatus);
	ClassDB::bind_method(D_METHOD("setBotPlayerCount", "bots"), &SteamServer::setBotPlayerCount);
	ClassDB::bind_method(D_METHOD("setDedicatedServer", "dedicated"), &SteamServer::setDedicatedServer);
	ClassDB::bind_method(D_METHOD("setGameData", "data"), &SteamServer::setGameData);
	ClassDB::bind_method(D_METHOD("setGameDescription", "description"), &SteamServer::setGameDescription);
	ClassDB::bind_method(D_METHOD("setGameTags", "tags"), &SteamServer::setGameTags);
	ClassDB::bind_method(D_METHOD("setKeyValue", "key", "value"), &SteamServer::setKeyValue);
	ClassDB::bind_method(D_METHOD("setMapName", "map"), &SteamServer::setMapName);
	ClassDB::bind_method(D_METHOD("setMaxPlayerCount", "players_max"), &SteamServer::setMaxPlayerCount);
	ClassDB::bind_method(D_METHOD("setModDir", "mod_directory"), &SteamServer::setModDir);
	ClassDB::bind_method(D_METHOD("setPasswordProtected", "password_protected"), &SteamServer::setPasswordProtected);
	ClassDB::bind_method(D_METHOD("setProduct", "product"), &SteamServer::setProduct);
	ClassDB::bind_method(D_METHOD("setRegion", "region"), &SteamServer::setRegion);
	ClassDB::bind_method(D_METHOD("setServerName", "name"), &SteamServer::setServerName);
	ClassDB::bind_method(D_METHOD("setSpectatorPort", "port"), &SteamServer::setSpectatorPort);
	ClassDB::bind_method(D_METHOD("setSpectatorServerName", "name"), &SteamServer::setSpectatorServerName);
	ClassDB::bind_method(D_METHOD("userHasLicenceForApp", "steam_id", "app_id"), &SteamServer::userHasLicenceForApp);
	ClassDB::bind_method("wasRestartRequested", &SteamServer::wasRestartRequested);	
	
	// GAME SERVER STATS BIND METHODS ///////////
	ClassDB::bind_method(D_METHOD("clearUserAchievement", "steam_id", "name"), &SteamServer::clearUserAchievement);
	ClassDB::bind_method(D_METHOD("getUserAchievement", "steam_id", "name"), &SteamServer::getUserAchievement);
	ClassDB::bind_method(D_METHOD("getUserStatInt", "steam_id", "name"), &SteamServer::getUserStatInt);
	ClassDB::bind_method(D_METHOD("GetUserStatFloat", "steam_id", "name"), &SteamServer::getUserStatFloat);
	ClassDB::bind_method(D_METHOD("requestUserStats", "steam_id"), &SteamServer::requestUserStats);
	ClassDB::bind_method(D_METHOD("setUserAchievement", "steam_id", "name"), &SteamServer::setUserAchievement);
	ClassDB::bind_method(D_METHOD("setUserStatInt", "steam_id", "name", "stat"), &SteamServer::setUserStatInt);
	ClassDB::bind_method(D_METHOD("setUserStatFloat", "steam_id", "name", "stat"), &SteamServer::setUserStatFloat);
	ClassDB::bind_method(D_METHOD("storeUserStats", "steam_id"), &SteamServer::storeUserStats);
	ClassDB::bind_method(D_METHOD("updateUserAvgRateStat", "steam_id", "name", "this_session", "session_length"), &SteamServer::updateUserAvgRateStat);

	// HTTP BIND METHODS ////////////////////////
	ClassDB::bind_method(D_METHOD("createCookieContainer", "allow_response_to_modify"), &SteamServer::createCookieContainer);
	ClassDB::bind_method(D_METHOD("createHTTPRequest", "request_method", "absolute_url"), &SteamServer::createHTTPRequest);
	ClassDB::bind_method(D_METHOD("deferHTTPRequest", "request_handle"), &SteamServer::deferHTTPRequest);
	ClassDB::bind_method(D_METHOD("getHTTPDownloadProgressPct", "request_handle"), &SteamServer::getHTTPDownloadProgressPct);
	ClassDB::bind_method(D_METHOD("getHTTPRequestWasTimedOut", "request_handle"), &SteamServer::getHTTPRequestWasTimedOut);
	ClassDB::bind_method(D_METHOD("getHTTPResponseBodyData", "request_handle", "buffer_size"), &SteamServer::getHTTPResponseBodyData);
	ClassDB::bind_method(D_METHOD("getHTTPResponseBodySize", "request_handle"), &SteamServer::getHTTPResponseBodySize);
	ClassDB::bind_method(D_METHOD("getHTTPResponseHeaderSize", "request_handle", "header_name"), &SteamServer::getHTTPResponseHeaderSize);
	ClassDB::bind_method(D_METHOD("getHTTPResponseHeaderValue","request_handle", "header_name", "buffer_size"), &SteamServer::getHTTPResponseHeaderValue);
	ClassDB::bind_method(D_METHOD("getHTTPStreamingResponseBodyData", "request_handle", "offset", "buffer_size"), &SteamServer::getHTTPStreamingResponseBodyData);
	ClassDB::bind_method(D_METHOD("prioritizeHTTPRequest", "request_handle"), &SteamServer::prioritizeHTTPRequest);
	ClassDB::bind_method(D_METHOD("releaseCookieContainer", "cookie_handle"), &SteamServer::releaseCookieContainer);
	ClassDB::bind_method(D_METHOD("releaseHTTPRequest", "request_handle"), &SteamServer::releaseHTTPRequest);
	ClassDB::bind_method(D_METHOD("sendHTTPRequest", "request_handle"), &SteamServer::sendHTTPRequest);
	ClassDB::bind_method(D_METHOD("sendHTTPRequestAndStreamResponse", "request_handle"), &SteamServer::sendHTTPRequestAndStreamResponse);
	ClassDB::bind_method(D_METHOD("setHTTPCookie", "cookie_handle", "host", "url", "cookie"), &SteamServer::setHTTPCookie);
	ClassDB::bind_method(D_METHOD("setHTTPRequestAbsoluteTimeoutMS", "request_handle", "milliseconds"), &SteamServer::setHTTPRequestAbsoluteTimeoutMS);
	ClassDB::bind_method(D_METHOD("setHTTPRequestContextValue", "request_handle", "context_value"), &SteamServer::setHTTPRequestContextValue);
	ClassDB::bind_method(D_METHOD("setHTTPRequestCookieContainer", "request_handle", "cookie_handle"), &SteamServer::setHTTPRequestCookieContainer);
	ClassDB::bind_method(D_METHOD("setHTTPRequestGetOrPostParameter", "request_handle", "name", "value"), &SteamServer::setHTTPRequestGetOrPostParameter);
	ClassDB::bind_method(D_METHOD("setHTTPRequestHeaderValue", "request_handle", "header_name", "header_value"), &SteamServer::setHTTPRequestHeaderValue);
	ClassDB::bind_method(D_METHOD("setHTTPRequestNetworkActivityTimeout", "request_handle", "timeout_seconds"), &SteamServer::setHTTPRequestNetworkActivityTimeout);
	ClassDB::bind_method(D_METHOD("setHTTPRequestRawPostBody", "request_handle", "content_type", "body_length"), &SteamServer::setHTTPRequestRawPostBody);
	ClassDB::bind_method(D_METHOD("setHTTPRequestRequiresVerifiedCertificate", "request_handle", "require_verified_certificate"), &SteamServer::setHTTPRequestRequiresVerifiedCertificate);
	ClassDB::bind_method(D_METHOD("setHTTPRequestUserAgentInfo", "request_handle", "user_agent_info"), &SteamServer::setHTTPRequestUserAgentInfo);

	// INVENTORY BIND METHODS ///////////////////
	ClassDB::bind_method(D_METHOD("addPromoItem", "item"), &SteamServer::addPromoItem);
	ClassDB::bind_method(D_METHOD("addPromoItems", "items"), &SteamServer::addPromoItems);
	ClassDB::bind_method(D_METHOD("checkResultSteamID", "steam_id_expected", "this_inventory_handle"), &SteamServer::checkResultSteamID, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("consumeItem", "item_consume", "quantity"), &SteamServer::consumeItem);
	ClassDB::bind_method(D_METHOD("deserializeResult" "buffer"), &SteamServer::deserializeResult);
	ClassDB::bind_method(D_METHOD("destroyResult", "this_inventory_handle"), &SteamServer::destroyResult, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("exchangeItems", "output_items", "output_quantity", "input_items", "input_quantity"), &SteamServer::exchangeItems);
	ClassDB::bind_method(D_METHOD("generateItems", "items", "quantity"), &SteamServer::generateItems);
	ClassDB::bind_method("getAllItems", &SteamServer::getAllItems);
	ClassDB::bind_method(D_METHOD("getItemDefinitionProperty", "definition", "name"), &SteamServer::getItemDefinitionProperty);
	ClassDB::bind_method(D_METHOD("getItemsByID", "id_array", "count"), &SteamServer::getItemsByID);
	ClassDB::bind_method(D_METHOD("getItemPrice", "definition"), &SteamServer::getItemPrice);
	ClassDB::bind_method(D_METHOD("getItemsWithPrices", "length"), &SteamServer::getItemsWithPrices);
	ClassDB::bind_method("getNumItemsWithPrices", &SteamServer::getNumItemsWithPrices);
	ClassDB::bind_method(D_METHOD("getResultItemProperty", "index", "name", "this_inventory_handle"), &SteamServer::getResultItemProperty, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("getResultItems", "this_inventory_handle"), &SteamServer::getResultItems, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("getResultStatus", "this_inventory_handle"), &SteamServer::getResultStatus, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("getResultTimestamp", "this_inventory_handle"), &SteamServer::getResultTimestamp, DEFVAL(0));
	ClassDB::bind_method("grantPromoItems", &SteamServer::grantPromoItems);
	ClassDB::bind_method("loadItemDefinitions", &SteamServer::loadItemDefinitions);
	ClassDB::bind_method(D_METHOD("requestEligiblePromoItemDefinitionsIDs", "steam_id"), &SteamServer::requestEligiblePromoItemDefinitionsIDs);
	ClassDB::bind_method("requestPrices", &SteamServer::requestPrices);
	ClassDB::bind_method(D_METHOD("serializeResult", "this_inventory_handle"), &SteamServer::serializeResult, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("startPurchase", "items", "quantity"), &SteamServer::startPurchase);
	ClassDB::bind_method(D_METHOD("transferItemQuantity", "item_id", "quantity", "item_destination", "split"), &SteamServer::transferItemQuantity);
	ClassDB::bind_method(D_METHOD("triggerItemDrop", "definition"), &SteamServer::triggerItemDrop);
	ClassDB::bind_method("startUpdateProperties", &SteamServer::startUpdateProperties);
	ClassDB::bind_method(D_METHOD("submitUpdateProperties", "this_inventory_update_handle"), &SteamServer::submitUpdateProperties, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("removeProperty", "item_id", "name", "this_inventory_update_handle"), &SteamServer::removeProperty, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("setPropertyString", "item_id", "name", "value", "this_inventory_update_handle"), &SteamServer::setPropertyString, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("setPropertyBool", "item_id", "name", "value", "this_inventory_update_handle"), &SteamServer::setPropertyBool, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("setPropertyInt", "item_id", "name", "value", "this_inventory_update_handle"), &SteamServer::setPropertyInt, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("setPropertyFloat", "item_id", "name", "value", "this_inventory_update_handle"), &SteamServer::setPropertyFloat, DEFVAL(0));

	// NETWORKING BIND METHODS //////////////////
	ClassDB::bind_method(D_METHOD("acceptP2PSessionWithUser", "steam_id_remote"), &SteamServer::acceptP2PSessionWithUser);
	ClassDB::bind_method(D_METHOD("allowP2PPacketRelay", "allow"), &SteamServer::allowP2PPacketRelay);
	ClassDB::bind_method(D_METHOD("closeP2PChannelWithUser", "steam_id_remote", "channel"), &SteamServer::closeP2PChannelWithUser);
	ClassDB::bind_method(D_METHOD("closeP2PSessionWithUser", "steam_id_remote"), &SteamServer::closeP2PSessionWithUser);
	ClassDB::bind_method(D_METHOD("getP2PSessionState", "steam_id_remote"), &SteamServer::getP2PSessionState);
	ClassDB::bind_method(D_METHOD("getAvailableP2PPacketSize", "channel"), &SteamServer::getAvailableP2PPacketSize, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("readP2PPacket", "packet", "channel"), &SteamServer::readP2PPacket, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("sendP2PPacket", "steam_id_remote", "data", "send_type", "channel"), &SteamServer::sendP2PPacket, DEFVAL(0));

	// NETWORKING MESSAGES BIND METHODS /////////
	ClassDB::bind_method(D_METHOD("acceptSessionWithUser", "identity_reference"), &SteamServer::acceptSessionWithUser);
	ClassDB::bind_method(D_METHOD("closeChannelWithUser", "identity_reference", "channel"), &SteamServer::closeChannelWithUser);
	ClassDB::bind_method(D_METHOD("closeSessionWithUser", "identity_reference"), &SteamServer::closeSessionWithUser);
	ClassDB::bind_method(D_METHOD("getSessionConnectionInfo", "identity_reference", "get_connection", "get_status"), &SteamServer::getSessionConnectionInfo);
	ClassDB::bind_method(D_METHOD("receiveMessagesOnChannel", "channel", "max_messages"), &SteamServer::receiveMessagesOnChannel);
	ClassDB::bind_method(D_METHOD("sendMessageToUser", "identity_reference", "data", "flags", "channel"), &SteamServer::sendMessageToUser);
	
	// NETWORKING SOCKETS BIND METHODS //////////
	ClassDB::bind_method(D_METHOD("acceptConnection", "connection"), &SteamServer::acceptConnection);
	ClassDB::bind_method(D_METHOD("beginAsyncRequestFakeIP", "num_ports"), &SteamServer::beginAsyncRequestFakeIP);
	ClassDB::bind_method(D_METHOD("closeConnection", "peer", "reason", "debug_message", "linger"), &SteamServer::closeConnection);
	ClassDB::bind_method(D_METHOD("closeListenSocket", "socket"), &SteamServer::closeListenSocket);
	ClassDB::bind_method(D_METHOD("configureConnectionLanes", "connection", "lanes", "priorities", "weights"), &SteamServer::configureConnectionLanes);
	ClassDB::bind_method(D_METHOD("connectP2P", "identity_reference", "virtual_port", "options"), &SteamServer::connectP2P);
	ClassDB::bind_method(D_METHOD("connectToHostedDedicatedServer", "identity_reference", "virtual_port", "options"), &SteamServer::connectToHostedDedicatedServer);
	ClassDB::bind_method(D_METHOD("createFakeUDPPort", "fake_server_port"), &SteamServer::createFakeUDPPort);
	ClassDB::bind_method(D_METHOD("createHostedDedicatedServerListenSocket", "virtual_port", "options"), &SteamServer::createHostedDedicatedServerListenSocket);
	ClassDB::bind_method(D_METHOD("createListenSocketIP", "ip_reference", "options"), &SteamServer::createListenSocketIP);
	ClassDB::bind_method(D_METHOD("createListenSocketP2P", "virtual_port", "options"), &SteamServer::createListenSocketP2P);
	ClassDB::bind_method(D_METHOD("createListenSocketP2PFakeIP", "fake_port", "options"), &SteamServer::createListenSocketP2PFakeIP);
	ClassDB::bind_method("createPollGroup", &SteamServer::createPollGroup);
	ClassDB::bind_method(D_METHOD("createSocketPair", "loopback", "identity_reference1", "identity_reference2"), &SteamServer::createSocketPair);
	ClassDB::bind_method(D_METHOD("destroyPollGroup", "poll_group"), &SteamServer::destroyPollGroup);
//	ClassDB::bind_method(D_METHOD("findRelayAuthTicketForServer", "port"), &SteamServer::findRelayAuthTicketForServer);	<------ Uses datagram relay structs which were removed from base SDK
	ClassDB::bind_method(D_METHOD("flushMessagesOnConnection", "connection_handle"), &SteamServer::flushMessagesOnConnection);
	ClassDB::bind_method("getAuthenticationStatus", &SteamServer::getAuthenticationStatus);		
	ClassDB::bind_method("getCertificateRequest", &SteamServer::getCertificateRequest);
	ClassDB::bind_method(D_METHOD("getConnectionInfo", "connection_handle"), &SteamServer::getConnectionInfo);
	ClassDB::bind_method(D_METHOD("getConnectionName", "peer"), &SteamServer::getConnectionName);
	ClassDB::bind_method(D_METHOD("getConnectionRealTimeStatus", "connection_handle", "lanes", "get_status"), &SteamServer::getConnectionRealTimeStatus, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("getConnectionUserData", "peer"), &SteamServer::getConnectionUserData);
	ClassDB::bind_method(D_METHOD("getDetailedConnectionStatus", "connection_handle"), &SteamServer::getDetailedConnectionStatus);
	ClassDB::bind_method(D_METHOD("getFakeIP", "first_port"), &SteamServer::getFakeIP, DEFVAL(0));
//	ClassDB::bind_method(D_METHOD("getGameCoordinatorServerLogin", "app_data"), &SteamServer::getGameCoordinatorServerLogin);	<------ Uses datagram relay structs which were removed from base SDK
//	ClassDB::bind_method("getHostedDedicatedServerAddress", &SteamServer::getHostedDedicatedServerAddress);	<------ Uses datagram relay structs which were removed from base SDK
	ClassDB::bind_method("getHostedDedicatedServerPOPId", &SteamServer::getHostedDedicatedServerPOPId);
	ClassDB::bind_method("getHostedDedicatedServerPort", &SteamServer::getHostedDedicatedServerPort);
	ClassDB::bind_method(D_METHOD("getListenSocketAddress", "socket"), &SteamServer::getListenSocketAddress);
	ClassDB::bind_method("getIdentity", &SteamServer::getIdentity);
	ClassDB::bind_method(D_METHOD("getRemoteFakeIPForConnection", "connection"), &SteamServer::getRemoteFakeIPForConnection);
	ClassDB::bind_method("initAuthentication", &SteamServer::initAuthentication);
	ClassDB::bind_method(D_METHOD("receiveMessagesOnConnection", "connection", "max_messages"), &SteamServer::receiveMessagesOnConnection);	
	ClassDB::bind_method(D_METHOD("receiveMessagesOnPollGroup", "poll_group", "max_messages"), &SteamServer::receiveMessagesOnPollGroup);
//	ClassDB::bind_method("receivedRelayAuthTicket", &SteamServer::receivedRelayAuthTicket);	<------ Uses datagram relay structs which were removed from base SDK
	ClassDB::bind_method(D_METHOD("resetIdentity", "this_identity"), &SteamServer::resetIdentity);
	ClassDB::bind_method("runNetworkingCallbacks", &SteamServer::runNetworkingCallbacks);
	ClassDB::bind_method(D_METHOD("sendMessages", "messages", "data", "connection_handle", "flags"), &SteamServer::sendMessages);
	ClassDB::bind_method(D_METHOD("sendMessageToConnection", "connection_handle", "data", "flags"), &SteamServer::sendMessageToConnection);
	ClassDB::bind_method(D_METHOD("setCertificate", "certificate"), &SteamServer::setCertificate);	
	ClassDB::bind_method(D_METHOD("setConnectionPollGroup", "connection_handle", "poll_group"), &SteamServer::setConnectionPollGroup);
	ClassDB::bind_method(D_METHOD("setConnectionName", "peer", "name"), &SteamServer::setConnectionName);
	
	// NETWORKING TYPES BIND METHODS ////////////
	ClassDB::bind_method(D_METHOD("addIdentity", "reference_name"), &SteamServer::addIdentity);
	ClassDB::bind_method(D_METHOD("addIPAddress", "reference_name"), &SteamServer::addIPAddress);
	ClassDB::bind_method(D_METHOD("clearIdentity", "reference_name"), &SteamServer::clearIdentity);
	ClassDB::bind_method(D_METHOD("clearIPAddress", "reference_name"), &SteamServer::clearIPAddress);
	ClassDB::bind_method(D_METHOD("getGenericBytes", "reference_name"), &SteamServer::getGenericBytes);
	ClassDB::bind_method(D_METHOD("getGenericString", "reference_name"), &SteamServer::getGenericString);
	ClassDB::bind_method("getIdentities", &SteamServer::getIdentities);
	ClassDB::bind_method(D_METHOD("getIdentityIPAddr", "reference_name"), &SteamServer::getIdentityIPAddr);
	ClassDB::bind_method(D_METHOD("getIdentitySteamID", "reference_name"), &SteamServer::getIdentitySteamID);
	ClassDB::bind_method(D_METHOD("getIdentitySteamID64", "reference_name"), &SteamServer::getIdentitySteamID64);
	ClassDB::bind_method("getIPAddresses", &SteamServer::getIPAddresses);
	ClassDB::bind_method(D_METHOD("getIPv4", "reference_name"), &SteamServer::getIPv4);
	ClassDB::bind_method(D_METHOD("getPSNID", "reference_name"), &SteamServer::getPSNID);
	ClassDB::bind_method(D_METHOD("getStadiaID", "reference_name"), &SteamServer::getStadiaID);
	ClassDB::bind_method(D_METHOD("getXboxPairwiseID", "reference_name"), &SteamServer::getXboxPairwiseID);
	ClassDB::bind_method(D_METHOD("isAddressLocalHost", "reference_name"), &SteamServer::isAddressLocalHost);
	ClassDB::bind_method(D_METHOD("isIdentityInvalid", "reference_name"), &SteamServer::isIdentityInvalid);
	ClassDB::bind_method(D_METHOD("isIdentityLocalHost", "reference_name"), &SteamServer::isIdentityLocalHost);
	ClassDB::bind_method(D_METHOD("isIPv4", "reference_name"), &SteamServer::isIPv4);
	ClassDB::bind_method(D_METHOD("isIPv6AllZeros", "reference_name"), &SteamServer::isIPv6AllZeros);
	ClassDB::bind_method(D_METHOD("parseIdentityString", "reference_name", "string_to_parse"), &SteamServer::parseIdentityString);
	ClassDB::bind_method(D_METHOD("parseIPAddressString", "reference_name", "string_to_parse"), &SteamServer::parseIPAddressString);
	ClassDB::bind_method(D_METHOD("setGenericBytes", "reference_name", "data"), &SteamServer::setGenericBytes);
	ClassDB::bind_method(D_METHOD("setGenericString", "reference_name", "this_string"), &SteamServer::setGenericString);
	ClassDB::bind_method(D_METHOD("setIdentityIPAddr", "reference_name", "ip_address_name"), &SteamServer::setIdentityIPAddr);
	ClassDB::bind_method(D_METHOD("setIdentityLocalHost", "reference_name"), &SteamServer::setIdentityLocalHost);
	ClassDB::bind_method(D_METHOD("setIdentitySteamID", "reference_name", "steam_id"), &SteamServer::setIdentitySteamID);
	ClassDB::bind_method(D_METHOD("setIdentitySteamID64", "reference_name", "steam_id"), &SteamServer::setIdentitySteamID64);
	ClassDB::bind_method(D_METHOD("setIPv4", "reference_name", "ip", "port"), &SteamServer::setIPv4);
	ClassDB::bind_method(D_METHOD("setIPv6", "reference_name", "ipv6", "port"), &SteamServer::setIPv6);
	ClassDB::bind_method(D_METHOD("setIPv6LocalHost", "reference_name", "port"), &SteamServer::setIPv6LocalHost, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("setPSNID", "reference_name", "psn_id"), &SteamServer::setPSNID);
	ClassDB::bind_method(D_METHOD("setStadiaID", "reference_name", "stadia_id"), &SteamServer::setStadiaID);
	ClassDB::bind_method(D_METHOD("setXboxPairwiseID", "reference_name", "xbox_id"), &SteamServer::setXboxPairwiseID); 
	ClassDB::bind_method(D_METHOD("toIdentityString", "reference_name"), &SteamServer::toIdentityString);
	ClassDB::bind_method(D_METHOD("toIPAddressString", "reference_name", "with_port"), &SteamServer::toIPAddressString);
	
	// NETWORKING UTILS BIND METHODS ////////////
	ClassDB::bind_method(D_METHOD("checkPingDataUpToDate", "max_age_in_seconds"), &SteamServer::checkPingDataUpToDate);
	ClassDB::bind_method(D_METHOD("convertPingLocationToString", "location"), &SteamServer::convertPingLocationToString);
	ClassDB::bind_method(D_METHOD("estimatePingTimeBetweenTwoLocations", "location1", "location2"), &SteamServer::estimatePingTimeBetweenTwoLocations);
	ClassDB::bind_method(D_METHOD("estimatePingTimeFromLocalHost", "location"), &SteamServer::estimatePingTimeFromLocalHost);
	ClassDB::bind_method(D_METHOD("getConfigValue", "config_value", "scope_type", "connection_handle"), &SteamServer::getConfigValue);
	ClassDB::bind_method(D_METHOD("getConfigValueInfo", "config_value"), &SteamServer::getConfigValueInfo);
	ClassDB::bind_method(D_METHOD("getDirectPingToPOP", "pop_id"), &SteamServer::getDirectPingToPOP);
	ClassDB::bind_method("getLocalPingLocation", &SteamServer::getLocalPingLocation);
	ClassDB::bind_method("getLocalTimestamp", &SteamServer::getLocalTimestamp);
	ClassDB::bind_method(D_METHOD("getPingToDataCenter", "pop_id"), &SteamServer::getPingToDataCenter);
	ClassDB::bind_method("getPOPCount", &SteamServer::getPOPCount);
	ClassDB::bind_method("getPOPList", &SteamServer::getPOPList);
	ClassDB::bind_method("getRelayNetworkStatus", &SteamServer::getRelayNetworkStatus);
	ClassDB::bind_method("initRelayNetworkAccess", &SteamServer::initRelayNetworkAccess);
	ClassDB::bind_method(D_METHOD("parsePingLocationString", "string"), &SteamServer::parsePingLocationString);
	ClassDB::bind_method(D_METHOD("setConnectionConfigValueFloat", "connection", "config", "value"), &SteamServer::setConnectionConfigValueFloat);
	ClassDB::bind_method(D_METHOD("setConnectionConfigValueInt32", "connection", "config", "value"), &SteamServer::setConnectionConfigValueInt32);
	ClassDB::bind_method(D_METHOD("setConnectionConfigValueString", "connection", "config", "value"), &SteamServer::setConnectionConfigValueString);
//	ClassDB::bind_method(D_METHOD("setConfigValue", "setting", "scope_type", "connection_handle", "data_type", "value"), &SteamServer::setConfigValue);
	ClassDB::bind_method(D_METHOD("setGlobalConfigValueFloat", "config", "value"), &SteamServer::setGlobalConfigValueFloat);
	ClassDB::bind_method(D_METHOD("setGlobalConfigValueInt32", "config", "value"), &SteamServer::setGlobalConfigValueInt32);
	ClassDB::bind_method(D_METHOD("setGlobalConfigValueString", "config", "value"), &SteamServer::setGlobalConfigValueString);

	// UGC BIND METHODS ////////////////////
	ClassDB::bind_method(D_METHOD("addAppDependency", "published_file_id", "app_id"), &SteamServer::addAppDependency);
	ClassDB::bind_method(D_METHOD("addDependency", "published_file_id", "child_published_file_id"), &SteamServer::addDependency);
	ClassDB::bind_method(D_METHOD("addExcludedTag", "query_handle", "tag_name"), &SteamServer::addExcludedTag);
	ClassDB::bind_method(D_METHOD("addItemKeyValueTag", "query_handle", "key", "value"), &SteamServer::addItemKeyValueTag);
	ClassDB::bind_method(D_METHOD("addItemPreviewFile", "query_handle", "preview_file", "type"), &SteamServer::addItemPreviewFile);
	ClassDB::bind_method(D_METHOD("addItemPreviewVideo", "query_handle", "video_id"), &SteamServer::addItemPreviewVideo);
	ClassDB::bind_method(D_METHOD("addItemToFavorites", "app_id", "published_file_id"), &SteamServer::addItemToFavorites);
	ClassDB::bind_method(D_METHOD("addRequiredKeyValueTag", "query_handle", "key", "value"), &SteamServer::addRequiredKeyValueTag);
	ClassDB::bind_method(D_METHOD("addRequiredTag", "query_handle", "tag_name"), &SteamServer::addRequiredTag);
	ClassDB::bind_method(D_METHOD("addRequiredTagGroup", "query_handle", "tag_array"), &SteamServer::addRequiredTagGroup);
	ClassDB::bind_method(D_METHOD("initWorkshopForGameServer", "workshop_depot_id"), &SteamServer::initWorkshopForGameServer);
	ClassDB::bind_method(D_METHOD("createItem", "app_id", "file_type"), &SteamServer::createItem);
	ClassDB::bind_method(D_METHOD("createQueryAllUGCRequest", "query_type", "matching_type", "creator_id", "consumer_id", "page"), &SteamServer::createQueryAllUGCRequest);
	ClassDB::bind_method(D_METHOD("createQueryUGCDetailsRequest", "published_file_id"), &SteamServer::createQueryUGCDetailsRequest);
	ClassDB::bind_method(D_METHOD("createQueryUserUGCRequest", "account_id", "list_type", "matching_ugc_type", "sort_order", "creator_id", "consumer_id", "page"), &SteamServer::createQueryUserUGCRequest);
	ClassDB::bind_method(D_METHOD("deleteItem", "published_file_id"), &SteamServer::deleteItem);
	ClassDB::bind_method(D_METHOD("downloadItem", "published_file_id", "high_priority"), &SteamServer::downloadItem);
	ClassDB::bind_method(D_METHOD("getItemDownloadInfo", "published_file_id"), &SteamServer::getItemDownloadInfo);
	ClassDB::bind_method(D_METHOD("getItemInstallInfo", "published_file_id"), &SteamServer::getItemInstallInfo);
	ClassDB::bind_method(D_METHOD("getItemState", "published_file_id"), &SteamServer::getItemState);
	ClassDB::bind_method(D_METHOD("getItemUpdateProgress", "update_handle"), &SteamServer::getItemUpdateProgress);
	ClassDB::bind_method("getNumSubscribedItems", &SteamServer::getNumSubscribedItems);
	ClassDB::bind_method(D_METHOD("getQueryUGCAdditionalPreview", "query_handle", "index", "preview_index"), &SteamServer::getQueryUGCAdditionalPreview);
	ClassDB::bind_method(D_METHOD("getQueryUGCChildren", "query_handle", "index", "child_count"), &SteamServer::getQueryUGCChildren);
	ClassDB::bind_method(D_METHOD("getQueryUGCKeyValueTag", "query_handle", "index", "key_value_tag_index"), &SteamServer::getQueryUGCKeyValueTag);
	ClassDB::bind_method(D_METHOD("getQueryUGCMetadata", "query_handle", "index"), &SteamServer::getQueryUGCMetadata);
	ClassDB::bind_method(D_METHOD("getQueryUGCNumAdditionalPreviews", "query_handle", "index"), &SteamServer::getQueryUGCNumAdditionalPreviews);
	ClassDB::bind_method(D_METHOD("getQueryUGCNumKeyValueTags", "query_handle", "index"), &SteamServer::getQueryUGCNumKeyValueTags);
	ClassDB::bind_method(D_METHOD("getQueryUGCNumTags", "query_handle", "index"), &SteamServer::getQueryUGCNumTags);
	ClassDB::bind_method(D_METHOD("getQueryUGCPreviewURL", "query_handle", "index"), &SteamServer::getQueryUGCPreviewURL);
	ClassDB::bind_method(D_METHOD("getQueryUGCResult", "query_handle", "index"), &SteamServer::getQueryUGCResult);
	ClassDB::bind_method(D_METHOD("getQueryUGCStatistic", "query_handle", "index", "stat_type"), &SteamServer::getQueryUGCStatistic);
	ClassDB::bind_method(D_METHOD("getQueryUGCTag", "query_handle", "index", "tag_index"), &SteamServer::getQueryUGCTag);
	ClassDB::bind_method(D_METHOD("getQueryUGCTagDisplayName", "query_handle", "index", "tag_index"), &SteamServer::getQueryUGCTagDisplayName);
	ClassDB::bind_method("getSubscribedItems", &SteamServer::getSubscribedItems);
	ClassDB::bind_method(D_METHOD("getUserItemVote", "published_file_id"), &SteamServer::getUserItemVote);
	ClassDB::bind_method(D_METHOD("releaseQueryUGCRequest", "query_handle"), &SteamServer::releaseQueryUGCRequest);
	ClassDB::bind_method(D_METHOD("removeAppDependency", "published_file_id", "app_id"), &SteamServer::removeAppDependency);
	ClassDB::bind_method(D_METHOD("removeDependency", "published_file_id", "child_published_file_id"), &SteamServer::removeDependency);
	ClassDB::bind_method(D_METHOD("removeItemFromFavorites", "app_id", "published_file_id"), &SteamServer::removeItemFromFavorites);
	ClassDB::bind_method(D_METHOD("removeItemKeyValueTags", "update_handle", "key"), &SteamServer::removeItemKeyValueTags);
	ClassDB::bind_method(D_METHOD("removeItemPreview", "update_handle", "index"), &SteamServer::removeItemPreview);
	ClassDB::bind_method(D_METHOD("sendQueryUGCRequest", "update_handle"), &SteamServer::sendQueryUGCRequest);
	ClassDB::bind_method(D_METHOD("setAllowCachedResponse", "update_handle", "max_age_seconds"), &SteamServer::setAllowCachedResponse);
	ClassDB::bind_method(D_METHOD("setCloudFileNameFilter", "update_handle", "match_cloud_filename"), &SteamServer::setCloudFileNameFilter);
	ClassDB::bind_method(D_METHOD("setItemContent", "update_handle", "content_folder"), &SteamServer::setItemContent);
	ClassDB::bind_method(D_METHOD("setItemDescription", "update_handle", "description"), &SteamServer::setItemDescription);
	ClassDB::bind_method(D_METHOD("setItemMetadata", "update_handle", "metadata"), &SteamServer::setItemMetadata);
	ClassDB::bind_method(D_METHOD("setItemPreview", "update_handle", "preview_file"), &SteamServer::setItemPreview);
	ClassDB::bind_method(D_METHOD("setItemTags", "update_handle", "tag_array"), &SteamServer::setItemTags);
	ClassDB::bind_method(D_METHOD("setItemTitle", "update_handle", "title"), &SteamServer::setItemTitle);
	ClassDB::bind_method(D_METHOD("setItemUpdateLanguage", "update_handle", "language"), &SteamServer::setItemUpdateLanguage);
	ClassDB::bind_method(D_METHOD("setItemVisibility", "update_handle", "visibility"), &SteamServer::setItemVisibility);
	ClassDB::bind_method(D_METHOD("setLanguage", "query_handle", "language"), &SteamServer::setLanguage);
	ClassDB::bind_method(D_METHOD("setMatchAnyTag", "query_handle", "match_any_tag"), &SteamServer::setMatchAnyTag);
	ClassDB::bind_method(D_METHOD("setRankedByTrendDays", "query_handle", "days"), &SteamServer::setRankedByTrendDays);
	ClassDB::bind_method(D_METHOD("setReturnAdditionalPreviews", "query_handle", "return_additional_previews"), &SteamServer::setReturnAdditionalPreviews);
	ClassDB::bind_method(D_METHOD("setReturnChildren", "query_handle", "return_children"), &SteamServer::setReturnChildren);
	ClassDB::bind_method(D_METHOD("setReturnKeyValueTags", "query_handle", "return_key_value_tags"), &SteamServer::setReturnKeyValueTags);
	ClassDB::bind_method(D_METHOD("setReturnLongDescription", "query_handle", "return_long_description"), &SteamServer::setReturnLongDescription);
	ClassDB::bind_method(D_METHOD("setReturnMetadata", "query_handle", "return_metadata"), &SteamServer::setReturnMetadata);
	ClassDB::bind_method(D_METHOD("setReturnOnlyIDs", "query_handle", "return_only_ids"), &SteamServer::setReturnOnlyIDs);
	ClassDB::bind_method(D_METHOD("setReturnPlaytimeStats", "query_handle", "days"), &SteamServer::setReturnPlaytimeStats);
	ClassDB::bind_method(D_METHOD("setReturnTotalOnly", "query_handle", "return_total_only"), &SteamServer::setReturnTotalOnly);
	ClassDB::bind_method(D_METHOD("setSearchText", "query_handle", "search_text"), &SteamServer::setSearchText);
	ClassDB::bind_method(D_METHOD("setUserItemVote", "published_file_id", "vote_up"), &SteamServer::setUserItemVote);
	ClassDB::bind_method(D_METHOD("startItemUpdate", "app_id", "file_id"), &SteamServer::startItemUpdate);
	ClassDB::bind_method(D_METHOD("startPlaytimeTracking", "published_file_ids"), &SteamServer::startPlaytimeTracking);
	ClassDB::bind_method(D_METHOD("stopPlaytimeTracking", "published_file_ids"), &SteamServer::stopPlaytimeTracking);
	ClassDB::bind_method("stopPlaytimeTrackingForAllItems", &SteamServer::stopPlaytimeTrackingForAllItems);
	ClassDB::bind_method(D_METHOD("getAppDependencies", "published_file_id"), &SteamServer::getAppDependencies);
	ClassDB::bind_method(D_METHOD("submitItemUpdate", "update_handle", "change_note"), &SteamServer::submitItemUpdate);
	ClassDB::bind_method(D_METHOD("subscribeItem", "published_file_id"), &SteamServer::subscribeItem);
	ClassDB::bind_method(D_METHOD("suspendDownloads", "suspend"), &SteamServer::suspendDownloads);
	ClassDB::bind_method(D_METHOD("unsubscribeItem", "published_file_id"), &SteamServer::unsubscribeItem);
	ClassDB::bind_method(D_METHOD("updateItemPreviewFile", "update_handle", "index", "preview_file"), &SteamServer::updateItemPreviewFile);
	ClassDB::bind_method(D_METHOD("updateItemPreviewVideo", "update_handle", "index", "video_id"), &SteamServer::updateItemPreviewVideo);
	ClassDB::bind_method("showWorkshopEULA", &SteamServer::showWorkshopEULA);	
	ClassDB::bind_method("getWorkshopEULAStatus", &SteamServer::getWorkshopEULAStatus);
	ClassDB::bind_method(D_METHOD("setTimeCreatedDateRange", "update_handle", "start", "end"), &SteamServer::setTimeCreatedDateRange);
	ClassDB::bind_method(D_METHOD("setTimeUpdatedDateRange", "update_handle", "start", "end"), &SteamServer::setTimeUpdatedDateRange);

	// UTILS BIND METHODS ///////////////////////
	ClassDB::bind_method(D_METHOD("filterText", "context", "steam_id", "message"), &SteamServer::filterText);
	ClassDB::bind_method("getAPICallFailureReason", &SteamServer::getAPICallFailureReason);
	ClassDB::bind_method("getAppID", &SteamServer::getAppID);
	ClassDB::bind_method("getCurrentBatteryPower", &SteamServer::getCurrentBatteryPower);
	ClassDB::bind_method(D_METHOD("getImageRGBA", "image"), &SteamServer::getImageRGBA);
	ClassDB::bind_method(D_METHOD("getImageSize", "image"), &SteamServer::getImageSize);
	ClassDB::bind_method("getIPCCallCount", &SteamServer::getIPCCallCount);
	ClassDB::bind_method("getIPCountry", &SteamServer::getIPCountry);
	ClassDB::bind_method("getSecondsSinceAppActive", &SteamServer::getSecondsSinceAppActive);
	ClassDB::bind_method("getSecondsSinceComputerActive", &SteamServer::getSecondsSinceComputerActive);
	ClassDB::bind_method("getServerRealTime", &SteamServer::getServerRealTime);
	ClassDB::bind_method("getSteamUILanguage", &SteamServer::getSteamUILanguage);
	ClassDB::bind_method("initFilterText", &SteamServer::initFilterText);
	ClassDB::bind_method("isAPICallCompleted", &SteamServer::isAPICallCompleted);
	ClassDB::bind_method("isOverlayEnabled", &SteamServer::isOverlayEnabled);
	ClassDB::bind_method("isSteamChinaLauncher", &SteamServer::isSteamChinaLauncher);
	ClassDB::bind_method("isSteamInBigPictureMode", &SteamServer::isSteamInBigPictureMode);
	ClassDB::bind_method("isSteamRunningInVR", &SteamServer::isSteamRunningInVR);
	ClassDB::bind_method("isVRHeadsetStreamingEnabled", &SteamServer::isVRHeadsetStreamingEnabled);
	ClassDB::bind_method("overlayNeedsPresent", &SteamServer::overlayNeedsPresent);
	ClassDB::bind_method(D_METHOD("setOverlayNotificationInset", "horizontal", "vertical"), &SteamServer::setOverlayNotificationInset);
	ClassDB::bind_method(D_METHOD("setOverlayNotificationPosition", "pos"), &SteamServer::setOverlayNotificationPosition);
	ClassDB::bind_method(D_METHOD("setVRHeadsetStreamingEnabled", "enabled"), &SteamServer::setVRHeadsetStreamingEnabled);
	ClassDB::bind_method(D_METHOD("showGamepadTextInput", "input_mode", "line_input_mode", "description", "max_text", "preset_text"), &SteamServer::showGamepadTextInput);
	ClassDB::bind_method(D_METHOD("showFloatingGamepadTextInput", "input_mode", "text_field_x_position", "text_field_y_position", "text_field_width", "text_field_height"), &SteamServer::showFloatingGamepadTextInput);
	ClassDB::bind_method(D_METHOD("setGameLauncherMode", "mode"), &SteamServer::setGameLauncherMode);
	ClassDB::bind_method("startVRDashboard", &SteamServer::startVRDashboard);	
	ClassDB::bind_method("isSteamRunningOnSteamDeck", &SteamServer::isSteamRunningOnSteamDeck);
	ClassDB::bind_method("dismissFloatingGamepadTextInput", &SteamServer::dismissFloatingGamepadTextInput);


	/////////////////////////////////////////////
	// CALLBACK SIGNAL BINDS ////////////////////
	/////////////////////////////////////////////
	//
	// STEAMWORKS SIGNALS ///////////////////////
	ADD_SIGNAL(MethodInfo("steamworks_error", PropertyInfo(Variant::STRING, "failed_signal"), PropertyInfo(Variant::STRING, "io failure")));

	// APPS SIGNALS /////////////////////////////
	ADD_SIGNAL(MethodInfo("file_details_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_size"), PropertyInfo(Variant::INT, "file_hash"), PropertyInfo(Variant::INT, "flags")));
	ADD_SIGNAL(MethodInfo("dlc_installed", PropertyInfo(Variant::INT, "app")));
	ADD_SIGNAL(MethodInfo("new_launch_url_parameters"));
	ADD_SIGNAL(MethodInfo("timed_trial_status", PropertyInfo(Variant::INT, "app_id"), PropertyInfo(Variant::BOOL, "is_offline"), PropertyInfo(Variant::INT, "seconds_allowed"), PropertyInfo(Variant::INT, "seconds_played")));

	// GAME SERVER SIGNALS //////////////////////
	ADD_SIGNAL(MethodInfo("server_connect_failure", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::BOOL, "retrying")));
	ADD_SIGNAL(MethodInfo("server_connected"));
	ADD_SIGNAL(MethodInfo("server_disconnected", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("client_approved", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "owner_id")));
	ADD_SIGNAL(MethodInfo("client_denied", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("client_kick", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("policy_response", PropertyInfo(Variant::INT, "secure")));
	ADD_SIGNAL(MethodInfo("client_group_status", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "group_id"), PropertyInfo(Variant::BOOL, "member"), PropertyInfo(Variant::BOOL, "officer")));
	ADD_SIGNAL(MethodInfo("associate_clan", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("player_compat", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "players_dont_like_candidate"), PropertyInfo(Variant::INT, "players_candidate_doesnt_like"), PropertyInfo(Variant::INT, "clan_players_dont_like_candidate"), PropertyInfo(Variant::INT, "steam_id")));

	// GAME SERVER STATS SIGNALS ////////////////
	ADD_SIGNAL(MethodInfo("stats_received", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "steam_id")));
	ADD_SIGNAL(MethodInfo("stats_stored", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "steam_id")));
	ADD_SIGNAL(MethodInfo("stats_unloaded", PropertyInfo(Variant::INT, "steam_id")));

	// HTTP SIGNALS /////////////////////////////
	ADD_SIGNAL(MethodInfo("http_request_completed", PropertyInfo(Variant::INT, "cookie_handle"), PropertyInfo(Variant::INT, "context_value"), PropertyInfo(Variant::BOOL, "request_success"), PropertyInfo(Variant::INT, "status_code"), PropertyInfo(Variant::INT, "body_size")));
	ADD_SIGNAL(MethodInfo("http_request_data_received", PropertyInfo(Variant::INT, "cookie_handle"), PropertyInfo(Variant::INT, "context_value"), PropertyInfo(Variant::INT, "offset"), PropertyInfo(Variant::INT, "bytes_received")));
	ADD_SIGNAL(MethodInfo("http_request_headers_received", PropertyInfo(Variant::INT, "cookie_handle"), PropertyInfo(Variant::INT, "context_value")));

	// INVENTORY SIGNALS ////////////////////////
	ADD_SIGNAL(MethodInfo("inventory_definition_update", PropertyInfo(Variant::ARRAY, "definitions")));
	ADD_SIGNAL(MethodInfo("inventory_eligible_promo_item", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::BOOL, "cached"), PropertyInfo(Variant::ARRAY, "definitions")));
	ADD_SIGNAL(MethodInfo("inventory_full_update", PropertyInfo(Variant::INT, "inventory_handle")));
	ADD_SIGNAL(MethodInfo("inventory_result_ready", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "inventory_handle")));
	ADD_SIGNAL(MethodInfo("inventory_start_purchase_result", PropertyInfo(Variant::STRING, "result"), PropertyInfo(Variant::INT, "order_id"), PropertyInfo(Variant::INT, "transaction_id")));
	ADD_SIGNAL(MethodInfo("inventory_request_prices_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::STRING, "currency")));

	// NETWORKING SIGNALS ///////////////////////
	ADD_SIGNAL(MethodInfo("p2p_session_request", PropertyInfo(Variant::INT, "steam_id_remote")));
	ADD_SIGNAL(MethodInfo("p2p_session_connect_fail", PropertyInfo(Variant::INT, "steam_id_remote"), PropertyInfo(Variant::INT, "session_error")));

	// NETWORKING MESSAGES //////////////////////
	ADD_SIGNAL(MethodInfo("network_messages_session_request", PropertyInfo(Variant::STRING, "identity")));
	ADD_SIGNAL(MethodInfo("network_messages_session_failed", PropertyInfo(Variant::INT, "reason")));

	// NETWORKING SOCKETS SIGNALS ///////////////
	ADD_SIGNAL(MethodInfo("network_connection_status_changed", PropertyInfo(Variant::INT, "connect_handle"), PropertyInfo(Variant::DICTIONARY, "connection"), PropertyInfo(Variant::INT, "old_state")));
	ADD_SIGNAL(MethodInfo("network_authentication_status", PropertyInfo(Variant::INT, "available"), PropertyInfo(Variant::STRING, "debug_message")));
	ADD_SIGNAL(MethodInfo("fake_ip_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::STRING, "identity"), PropertyInfo(Variant::STRING, "fake_ip"), PropertyInfo(Variant::ARRAY, "port_list")));

	// NETWORKING UTILS SIGNALS /////////////////
	ADD_SIGNAL(MethodInfo("relay_network_status", PropertyInfo(Variant::INT, "available"), PropertyInfo(Variant::INT, "ping_measurement"), PropertyInfo(Variant::INT, "available_config"), PropertyInfo(Variant::INT, "available_relay"), PropertyInfo(Variant::STRING, "debug_message")));

	// REMOTE STORAGE SIGNALS ///////////////////
	ADD_SIGNAL(MethodInfo("file_read_async_complete", PropertyInfo(Variant::DICTIONARY, "file_read")));
	ADD_SIGNAL(MethodInfo("file_share_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "handle"), PropertyInfo(Variant::STRING, "name")));
	ADD_SIGNAL(MethodInfo("file_write_async_complete", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("download_ugc_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::DICTIONARY, "download_data")));
	ADD_SIGNAL(MethodInfo("unsubscribe_item", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id")));
	ADD_SIGNAL(MethodInfo("subscribe_item", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id")));
	ADD_SIGNAL(MethodInfo("local_file_changed"));

	// UGC SIGNALS //////////////////////////////
	ADD_SIGNAL(MethodInfo("add_app_dependency_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::INT, "app_id")));
	ADD_SIGNAL(MethodInfo("add_ugc_dependency_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::INT, "child_id")));
	ADD_SIGNAL(MethodInfo("item_created", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::BOOL, "accept_tos")));
	ADD_SIGNAL(MethodInfo("item_downloaded", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::INT, "app_id")));
	ADD_SIGNAL(MethodInfo("get_app_dependencies_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::INT, "app_dependencies"), PropertyInfo(Variant::INT, "total_app_dependencies")));
	ADD_SIGNAL(MethodInfo("item_deleted", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id")));
	ADD_SIGNAL(MethodInfo("get_item_vote_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::BOOL, "vote_up"), PropertyInfo(Variant::BOOL, "vote_down"), PropertyInfo(Variant::BOOL, "vote_skipped")));
	ADD_SIGNAL(MethodInfo("item_installed", PropertyInfo(Variant::INT, "app_id"), PropertyInfo(Variant::INT, "file_id")));
	ADD_SIGNAL(MethodInfo("remove_app_dependency_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::INT, "app_id")));
	ADD_SIGNAL(MethodInfo("remove_ugc_dependency_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::INT, "child_id")));
	ADD_SIGNAL(MethodInfo("set_user_item_vote", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::BOOL, "vote_up")));
	ADD_SIGNAL(MethodInfo("start_playtime_tracking", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("ugc_query_completed", PropertyInfo(Variant::INT, "handle"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "results_returned"), PropertyInfo(Variant::INT, "total_matching"), PropertyInfo(Variant::BOOL, "cached")));
	ADD_SIGNAL(MethodInfo("stop_playtime_tracking", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("item_updated", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::BOOL, "accept_tos")));
	ADD_SIGNAL(MethodInfo("user_favorite_items_list_changed", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "file_id"), PropertyInfo(Variant::BOOL, "was_add_request")));
	ADD_SIGNAL(MethodInfo("workshop_eula_status", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "app_id"), PropertyInfo(Variant::DICTIONARY, "eula_data")));
	ADD_SIGNAL(MethodInfo("user_subscribed_items_list_changed", PropertyInfo(Variant::INT, "app_id")));

	// UTILITY SIGNALS //////////////////////////
	ADD_SIGNAL(MethodInfo("check_file_signature", PropertyInfo(Variant::STRING, "signature")));
	ADD_SIGNAL(MethodInfo("gamepad_text_input_dismissed", PropertyInfo(Variant::BOOL, "submitted"), PropertyInfo(Variant::STRING, "entered_text")));
	ADD_SIGNAL(MethodInfo("ip_country"));
	ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));
	ADD_SIGNAL(MethodInfo("steam_api_call_completed", PropertyInfo(Variant::INT, "async_call"), PropertyInfo(Variant::INT, "callback"), PropertyInfo(Variant::INT, "parameter")));
	ADD_SIGNAL(MethodInfo("steam_shutdown"));
	ADD_SIGNAL(MethodInfo("app_resuming_from_suspend"));
	ADD_SIGNAL(MethodInfo("floating_gamepad_text_input_dismissed"));

	
	/////////////////////////////////////////////
	// CONSTANT BINDS
	/////////////////////////////////////////////
	//
	// STEAM API CONSTANTS //////////////////////
	BIND_CONSTANT(INVALID_BREAKPAD_HANDLE);												// (BREAKPAD_HANDLE)0
	BIND_CONSTANT(GAME_EXTRA_INFO_MAX); 												// 64
	BIND_CONSTANT(AUTH_TICKET_INVALID);													// 0
	BIND_CONSTANT(API_CALL_INVALID); 													// 0x0
	BIND_CONSTANT(APP_ID_INVALID); 														// 0x0
	BIND_CONSTANT(DEPOT_ID_INVALID); 													// 0x0
	BIND_CONSTANT(STEAM_ACCOUNT_ID_MASK); 												// 0xFFFFFFFF
	BIND_CONSTANT(STEAM_ACCOUNT_INSTANCE_MASK); 										// 0x000FFFFF
	BIND_CONSTANT(STEAM_USER_CONSOLE_INSTANCE); 										// 2
	BIND_CONSTANT(STEAM_USER_DESKTOP_INSTANCE); 										// 1
	BIND_CONSTANT(STEAM_USER_WEB_INSTANCE); 											// 4
	BIND_CONSTANT(QUERY_PORT_ERROR); 													// 0xFFFE
	BIND_CONSTANT(QUERY_PORT_NOT_INITIALIZED); 											// 0xFFFF

	// STEAM SERVER API CONSTANTS ///////////////
	BIND_CONSTANT(FLAG_ACTIVE);															// 0x01
	BIND_CONSTANT(FLAG_DEDICATED);														// 0x04
	BIND_CONSTANT(FLAG_LINUX);															// 0x08
	BIND_CONSTANT(FLAG_NONE);															// 0x00
	BIND_CONSTANT(FLAG_PASSWORDED);														// 0x10
	BIND_CONSTANT(FLAG_PRIVATE);														// 0x20
	BIND_CONSTANT(FLAG_SECURE);															// 0x02

	// HTTP CONSTANTS ///////////////////////////
	BIND_CONSTANT(INVALID_HTTPCOOKIE_HANDLE);											// 0
	BIND_CONSTANT(INVALID_HTTPREQUEST_HANDLE);											// 0

	// INVENTORY CONSTANTS //////////////////////
	BIND_CONSTANT(INVENTORY_RESULT_INVALID);											// -1
	BIND_CONSTANT(ITEM_INSTANCE_ID_INVALID);											// 0

	// REMOTE STORAGE CONSTANTS /////////////////
	BIND_CONSTANT(FILENAME_MAX); 														// 260
	BIND_CONSTANT(PUBLISHED_DOCUMENT_CHANGE_DESCRIPTION_MAX); 							// 8000
	BIND_CONSTANT(PUBLISHED_DOCUMENT_DESCRIPTION_MAX);									// 8000
	BIND_CONSTANT(PUBLISHED_DOCUMENT_TITLE_MAX);										// 128 + 1
	BIND_CONSTANT(PUBLISHED_FILE_URL_MAX);												// 256
	BIND_CONSTANT(TAG_LIST_MAX);														// 1024 + 1
	BIND_CONSTANT(PUBLISHED_FILE_ID_INVALID);											// 0
	BIND_CONSTANT(PUBLISHED_FILE_UPDATE_HANDLE_INVALID);								// 0xffffffffffffffffull
	BIND_CONSTANT(UGC_FILE_STREAM_HANDLE_INVALID);										// 0xffffffffffffffffull
	BIND_CONSTANT(UGC_HANDLE_INVALID);													// 0xffffffffffffffffull
	BIND_CONSTANT(ENUMERATE_PUBLISHED_FILES_MAX_RESULTS);								// 50
	BIND_CONSTANT(MAX_CLOUD_FILE_CHUNK_SIZE);											// 100 * 1024 * 1024

	// UGC CONSTANTS ////////////////////////////
	BIND_CONSTANT(NUM_UGC_RESULTS_PER_PAGE); 											// 50
	BIND_CONSTANT(DEVELOPER_METADATA_MAX); 												// 5000
	BIND_CONSTANT(UGC_QUERY_HANDLE_INVALID); 											// 0xffffffffffffffffull
	BIND_CONSTANT(UGC_UPDATE_HANDLE_INVALID); 											// 0xffffffffffffffffull

	/////////////////////////////////////////////
	// ENUM CONSTANT BINDS //////////////////////
	/////////////////////////////////////////////
	//
	// ACCOUNT TYPE /////////////////////////////
	BIND_CONSTANT(ACCOUNT_TYPE_INVALID);												// 0
	BIND_CONSTANT(ACCOUNT_TYPE_INDIVIDUAL);												// 1
	BIND_CONSTANT(ACCOUNT_TYPE_MULTISEAT);												// 2
	BIND_CONSTANT(ACCOUNT_TYPE_GAME_SERVER);											// 3
	BIND_CONSTANT(ACCOUNT_TYPE_ANON_GAME_SERVER);										// 4
	BIND_CONSTANT(ACCOUNT_TYPE_PENDING);												// 5
	BIND_CONSTANT(ACCOUNT_TYPE_CONTENT_SERVER);											// 6
	BIND_CONSTANT(ACCOUNT_TYPE_CLAN);													// 7
	BIND_CONSTANT(ACCOUNT_TYPE_CHAT);													// 8
	BIND_CONSTANT(ACCOUNT_TYPE_CONSOLE_USER);											// 9
	BIND_CONSTANT(ACCOUNT_TYPE_ANON_USER);												// 10
	BIND_CONSTANT(ACCOUNT_TYPE_MAX);													// 11

	// APP OWNERSHIP FLAGS //////////////////////
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_NONE);											// 0x0000
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_OWNS_LICENSE);									// 0x0001
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_FREE_LICENSE);									// 0x0002
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_REGION_RESTRICTED);								// 0x0004
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_LOW_VIOLENCE);									// 0x0008
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_INVALID_PLATFORM);								// 0x0010
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_SHARED_LICENSE);									// 0x0020
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_FREE_WEEKEND);									// 0x0040
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_RETAIL_LICENSE);									// 0x0080
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_LICENSE_LOCKED);									// 0x0100
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_LICENSE_PENDING);									// 0x0200
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_LICENSE_EXPIRED);									// 0x0400
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_LICENSE_PERMANENT);								// 0x0800
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_LICENSE_RECURRING);								// 0x1000
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_LICENSE_CANCELED);								// 0x2000
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_AUTO_GRANT);										// 0x4000
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_PENDING_GIFT);									// 0x8000
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_RENTAL_NOT_ACTIVATED);							// 0x10000
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_RENTAL);											// 0x20000
	BIND_CONSTANT(APP_OWNERSHIP_FLAGS_SITE_LICENSE);									// 0x40000

	// APP RELEASE STATE ////////////////////////
	BIND_CONSTANT(APP_RELEASE_STATE_UNKNOWN);											// 0
	BIND_CONSTANT(APP_RELEASE_STATE_UNAVAILABLE);										// 1
	BIND_CONSTANT(APP_RELEASE_STATE_PRERELEASE);										// 2
	BIND_CONSTANT(APP_RELEASE_STATE_PRELOAD_ONLY);										// 3
	BIND_CONSTANT(APP_RELEASE_STATE_RELEASED);											// 4

	// APP TYPE /////////////////////////////////
	BIND_CONSTANT(APP_TYPE_INVALID);													// 0x000
	BIND_CONSTANT(APP_TYPE_GAME);														// 0x001
	BIND_CONSTANT(APP_TYPE_APPLICATION);												// 0x002
	BIND_CONSTANT(APP_TYPE_TOOL);														// 0x004
	BIND_CONSTANT(APP_TYPE_DEMO);														// 0x008
	BIND_CONSTANT(APP_TYPE_MEDIA_DEPRECATED);											// 0x010
	BIND_CONSTANT(APP_TYPE_DLC);														// 0x020
	BIND_CONSTANT(APP_TYPE_GUIDE);														// 0x040
	BIND_CONSTANT(APP_TYPE_DRIVER);														// 0x080
	BIND_CONSTANT(APP_TYPE_CONFIG);														// 0x100
	BIND_CONSTANT(APP_TYPE_HARDWARE);													// 0x200
	BIND_CONSTANT(APP_TYPE_FRANCHISE);													// 0x400
	BIND_CONSTANT(APP_TYPE_VIDEO);														// 0x800
	BIND_CONSTANT(APP_TYPE_PLUGIN);														// 0x1000
	BIND_CONSTANT(APP_TYPE_MUSIC);														// 0x2000
	BIND_CONSTANT(APP_TYPE_SERIES);														// 0x4000
	BIND_CONSTANT(APP_TYPE_SHORTCUT);													// 0x40000000
	BIND_CONSTANT(APP_TYPE_DEPOT_ONLY);													// 0X80000000

	// AUTH SESSION RESPONSE ////////////////////
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_OK);											// 0
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_USER_NOT_CONNECTED_TO_STEAM);					// 1
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_NO_LICENSE_OR_EXPIRED);							// 2
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_VAC_BANNED);									// 3
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_LOGGED_IN_ELSEWHERE);							// 4
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_VAC_CHECK_TIMEDOUT);							// 5
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_AUTH_TICKET_CANCELED);							// 6
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID_ALREADY_USED);				// 7
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID);							// 8
	BIND_CONSTANT(AUTH_SESSION_RESPONSE_PUBLISHER_ISSUED_BAN);							// 9

	// BEGIN AUTH SESSION RESULT ////////////////
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_OK);										// 0
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET);							// 1
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST);							// 2
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION);							// 3
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH);								// 4
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET);							// 5

	// BROADCAST UPLOAD RESULT //////////////////
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_NONE);										// 0
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_OK);											// 1
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_INIT_FAILED);									// 2
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_FRAME_FAILED);								// 3
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_TIME_OUT);									// 4
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_BANDWIDTH_EXCEEDED);							// 5
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_LOW_FPS);										// 6
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_MISSING_KEYFRAMES);							// 7
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_NO_CONNECTION);								// 8
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_RELAY_FAILED);								// 9
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_SETTINGS_CHANGED);							// 10
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_MISSING_AUDIO);								// 11
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_TOO_FAR_BEHIND);								// 12
	BIND_CONSTANT(BROADCAST_UPLOAD_RESULT_TRANSCODE_BEHIND);							// 13

	// CHAT ENTRY TYPE //////////////////////////
	BIND_CONSTANT(CHAT_ENTRY_TYPE_INVALID);												// 0
	BIND_CONSTANT(CHAT_ENTRY_TYPE_CHAT_MSG);											// 1
	BIND_CONSTANT(CHAT_ENTRY_TYPE_TYPING);												// 2
	BIND_CONSTANT(CHAT_ENTRY_TYPE_INVITE_GAME);											// 3
	BIND_CONSTANT(CHAT_ENTRY_TYPE_EMOTE);												// 4
	BIND_CONSTANT(CHAT_ENTRY_TYPE_LEFT_CONVERSATION);									// 6
	BIND_CONSTANT(CHAT_ENTRY_TYPE_ENTERED);												// 7
	BIND_CONSTANT(CHAT_ENTRY_TYPE_WAS_KICKED);											// 8
	BIND_CONSTANT(CHAT_ENTRY_TYPE_WAS_BANNED);											// 9
	BIND_CONSTANT(CHAT_ENTRY_TYPE_DISCONNECTED);										// 10
	BIND_CONSTANT(CHAT_ENTRY_TYPE_HISTORICAL_CHAT);										// 11
	BIND_CONSTANT(CHAT_ENTRY_TYPE_LINK_BLOCKED);										// 14

	// CHAT ROOM ENTER RESPONSE /////////////////
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_SUCCESS);									// 1
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_DOESNT_EXIST);								// 2
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_NOT_ALLOWED);								// 3
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_FULL);										// 4
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_ERROR);										// 5
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_BANNED);										// 6
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_LIMITED);									// 7
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_CLAN_DISABLED);								// 8
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_COMMUNITY_BAN);								// 9
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_MEMBER_BLOCKED_YOU);							// 10
	BIND_CONSTANT(CHAT_ROOM_ENTER_RESPONSE_YOU_BLOCKED_MEMBER);							// 11

	// CHAT STEAM ID INSTANCE FLAGS /////////////
	BIND_CONSTANT(CHAT_ACCOUNT_INSTANCE_MASK);											// 0X00000FFF
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_CLAN);												// ((k_unSteamAccountInstanceMask + 1) >> 1)
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_LOBBY);											// ((k_unSteamAccountInstanceMask + 1) >> 2)
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_MMS_LOBBY);										// ((k_unSteamAccountInstanceMask + 1) >> 3)

	// DENY REASON //////////////////////////////
	BIND_CONSTANT(DENY_INVALID);														// 0
	BIND_CONSTANT(DENY_INVALID_VERSION);												// 1
	BIND_CONSTANT(DENY_GENERIC);														// 2
	BIND_CONSTANT(DENY_NOT_LOGGED_ON);													// 3
	BIND_CONSTANT(DENY_NO_LICENSE);														// 4
	BIND_CONSTANT(DENY_CHEATER);														// 5
	BIND_CONSTANT(DENY_LOGGED_IN_ELSEWHERE);											// 6
	BIND_CONSTANT(DENY_UNKNOWN_TEXT);													// 7
	BIND_CONSTANT(DENY_INCOMPATIBLE_ANTI_CHEAT);										// 8
	BIND_CONSTANT(DENY_MEMORY_CORRUPTION);												// 9
	BIND_CONSTANT(DENY_INCOMPATIBLE_SOFTWARE);											// 10
	BIND_CONSTANT(DENY_STEAM_CONNECTION_LOST);											// 11
	BIND_CONSTANT(DENY_STEAM_CONNECTION_ERROR);											// 12
	BIND_CONSTANT(DENY_STEAM_RESPONSE_TIMED_OUT);										// 13
	BIND_CONSTANT(DENY_STEAM_VALIDATION_STALLED);										// 14
	BIND_CONSTANT(DENY_STEAM_OWNER_LEFT_GUEST_USER);									// 15

	// GAME ID TYPE /////////////////////////////
	BIND_CONSTANT(GAME_TYPE_APP);														// 0
	BIND_CONSTANT(GAME_TYPE_GAME_MOD);													// 1
	BIND_CONSTANT(GAME_TYPE_SHORTCUT);													// 2
	BIND_CONSTANT(GAME_TYPE_P2P);														// 3

	// LAUNCH OPTION TYPE ///////////////////////
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_NONE);												// 0
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_DEFAULT);											// 1
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_SAFE_MODE);										// 2
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_MULTIPLAYER);										// 3
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_CONFIG);											// 4
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_OPEN_VR);											// 5
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_SERVER);											// 6
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_EDITOR);											// 7
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_MANUAL);											// 8
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_BENCHMARK);										// 9
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_OPTION1);											// 10
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_OPTION2);											// 11
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_OPTION3);											// 12
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_OCULUS_VR);										// 13
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_OPEN_VR_OVERLAY);									// 14
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_OS_VR);											// 15
	BIND_CONSTANT(LAUNCH_OPTION_TYPE_DIALOG);											// 1000

	// MARKETING MESSAGE FLAGS //////////////////
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_NONE);										// 0
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_HIGH_PRIORITY);								// (1<<0)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_WINDOWS);							// (1<<1)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_MAC);								// (1<<2)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_LINUX);								// (1<<3)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_RESTRICTIONS);

	// NOTIFICATION POSITION ////////////////////
	BIND_CONSTANT(POSITION_TOP_LEFT);													// 0
	BIND_CONSTANT(POSITION_TOP_RIGHT);													// 1
	BIND_CONSTANT(POSITION_BOTTOM_LEFT);												// 2
	BIND_CONSTANT(POSITION_BOTTOM_RIGHT);												// 3

	// RESULT ///////////////////////////////////
	BIND_CONSTANT(RESULT_OK);															// 1
	BIND_CONSTANT(RESULT_FAIL);															// 2
	BIND_CONSTANT(RESULT_NO_CONNECTION);												// 3
	BIND_CONSTANT(RESULT_INVALID_PASSWORD);												// 5
	BIND_CONSTANT(RESULT_LOGGED_IN_ELSEWHERE);											// 6
	BIND_CONSTANT(RESULT_INVALID_PROTOCOL_VER);											// 7
	BIND_CONSTANT(RESULT_INVALID_PARAM);												// 8
	BIND_CONSTANT(RESULT_FILE_NOT_FOUND);												// 9
	BIND_CONSTANT(RESULT_BUSY);															// 10
	BIND_CONSTANT(RESULT_INVALID_STATE);												// 11
	BIND_CONSTANT(RESULT_INVALID_NAME);													// 12
	BIND_CONSTANT(RESULT_INVALID_EMAIL);												// 13
	BIND_CONSTANT(RESULT_DUPLICATE_NAME);												// 14
	BIND_CONSTANT(RESULT_ACCESS_DENIED);												// 15
	BIND_CONSTANT(RESULT_TIMEOUT);														// 16
	BIND_CONSTANT(RESULT_BANNED);														// 17
	BIND_CONSTANT(RESULT_ACCOUNT_NOT_FOUND);											// 18
	BIND_CONSTANT(RESULT_INVALID_STEAM_ID);												// 19
	BIND_CONSTANT(RESULT_SERVICE_UNAVAILABLE);											// 20
	BIND_CONSTANT(RESULT_NOT_LOGGED_ON);												// 21
	BIND_CONSTANT(RESULT_PENDING);														// 22
	BIND_CONSTANT(RESULT_ENCRYPTION_FAILURE);											// 23
	BIND_CONSTANT(RESULT_INSUFFICIENT_PRIVILEGE);										// 24
	BIND_CONSTANT(RESULT_LIMIT_EXCEEDED);												// 25
	BIND_CONSTANT(RESULT_REVOKED);														// 26
	BIND_CONSTANT(RESULT_EXPIRED);														// 27
	BIND_CONSTANT(RESULT_ALREADY_REDEEMED);												// 28
	BIND_CONSTANT(RESULT_DUPLICATE_REQUEST);											// 29
	BIND_CONSTANT(RESULT_ALREADY_OWNED);												// 30
	BIND_CONSTANT(RESULT_IP_NOT_FOUND);													// 31
	BIND_CONSTANT(RESULT_PERSIST_FAILED);												// 32
	BIND_CONSTANT(RESULT_LOCKING_FAILED);												// 33
	BIND_CONSTANT(RESULT_LOG_ON_SESSION_REPLACED);										// 34
	BIND_CONSTANT(RESULT_CONNECT_FAILED);												// 35
	BIND_CONSTANT(RESULT_HANDSHAKE_FAILED);												// 36
	BIND_CONSTANT(RESULT_IO_FAILURE);													// 37
	BIND_CONSTANT(RESULT_REMOTE_DISCONNECT);											// 38
	BIND_CONSTANT(RESULT_SHOPPING_CART_NOT_FOUND);										// 39
	BIND_CONSTANT(RESULT_BLOCKED);														// 40
	BIND_CONSTANT(RESULT_IGNORED);														// 41
	BIND_CONSTANT(RESULT_NO_MATCH);														// 42
	BIND_CONSTANT(RESULT_ACCOUNT_DISABLED);												// 43
	BIND_CONSTANT(RESULT_SERVICE_READ_ONLY);											// 44
	BIND_CONSTANT(RESULT_ACCOUNT_NOT_FEATURED);											// 45
	BIND_CONSTANT(RESULT_ADMINISTRATOR_OK);												// 46
	BIND_CONSTANT(RESULT_CONTENT_VERSION);												// 47
	BIND_CONSTANT(RESULT_TRY_ANOTHER_CM);												// 48
	BIND_CONSTANT(RESULT_PASSWORD_REQUIRED_TO_KICK_SESSION);							// 49
	BIND_CONSTANT(RESULT_ALREADY_LOGGED_IN_ELSEWHERE);									// 50
	BIND_CONSTANT(RESULT_SUSPENDED);													// 51
	BIND_CONSTANT(RESULT_CANCELLED);													// 52
	BIND_CONSTANT(RESULT_DATA_CORRUPTION);												// 53
	BIND_CONSTANT(RESULT_DISK_FULL);													// 54
	BIND_CONSTANT(RESULT_REMOTE_CALL_FAILED);											// 55
	BIND_CONSTANT(RESULT_PASSWORD_UNSET);												// 56
	BIND_CONSTANT(RESULT_EXTERNAL_ACCOUNT_UNLINKED);									// 57
	BIND_CONSTANT(RESULT_PSN_TICKET_INVALID);											// 58
	BIND_CONSTANT(RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED);								// 59
	BIND_CONSTANT(RESULT_REMOTE_FILE_CONFLICT);											// 60
	BIND_CONSTANT(RESULT_ILLEGAL_PASSWORD);												// 61
	BIND_CONSTANT(RESULT_SAME_AS_PREVIOUS_VALUE);										// 62
	BIND_CONSTANT(RESULT_ACCOUNT_LOG_ON_DENIED);										// 63
	BIND_CONSTANT(RESULT_CANNOT_USE_OLD_PASSWORD);										// 64
	BIND_CONSTANT(RESULT_INVALID_LOGIN_AUTH_CODE);										// 65
	BIND_CONSTANT(RESULT_ACCOUNT_LOG_ON_DENIED_NO_MAIL);								// 66
	BIND_CONSTANT(RESULT_HARDWARE_NOT_CAPABLE_OF_IPT);									// 67
	BIND_CONSTANT(RESULT_IPT_INIT_ERROR);												// 68
	BIND_CONSTANT(RESULT_PARENTAL_CONTROL_RESTRICTED);									// 69
	BIND_CONSTANT(RESULT_FACEBOOK_QUERY_ERROR);											// 70
	BIND_CONSTANT(RESULT_EXPIRED_LOGIN_AUTH_CODE);										// 71
	BIND_CONSTANT(RESULT_IP_LOGIN_RESTRICTION_FAILED);									// 72
	BIND_CONSTANT(RESULT_ACCOUNT_LOCKED_DOWN);											// 73
	BIND_CONSTANT(RESULT_ACCOUNT_LOG_ON_DENIED_VERIFIED_EMAIL_REQUIRED);				// 74
	BIND_CONSTANT(RESULT_NO_MATCHING_URL);												// 75
	BIND_CONSTANT(RESULT_BAD_RESPONSE);													// 76
	BIND_CONSTANT(RESULT_REQUIRE_PASSWORD_REENTRY);										// 77
	BIND_CONSTANT(RESULT_VALUE_OUT_OF_RANGE);											// 78
	BIND_CONSTANT(RESULT_UNEXPECTED_ERROR);												// 79
	BIND_CONSTANT(RESULT_DISABLED);														// 80
	BIND_CONSTANT(RESULT_INVALID_CEG_SUBMISSION);										// 81
	BIND_CONSTANT(RESULT_RESTRICTED_DEVICE);											// 82
	BIND_CONSTANT(RESULT_REGION_LOCKED);												// 83
	BIND_CONSTANT(RESULT_RATE_LIMIT_EXCEEDED);											// 84
	BIND_CONSTANT(RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR);							// 85
	BIND_CONSTANT(RESULT_ITEM_DELETED);													// 86
	BIND_CONSTANT(RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE);								// 87
	BIND_CONSTANT(RESULT_TWO_FACTOR_CODE_MISMATCH);										// 88
	BIND_CONSTANT(RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH);							// 89
	BIND_CONSTANT(RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS);						// 90
	BIND_CONSTANT(RESULT_NOT_MODIFIED);													// 91
	BIND_CONSTANT(RESULT_NO_MOBILE_DEVICE);												// 92
	BIND_CONSTANT(RESULT_TIME_NOT_SYNCED);												// 93
	BIND_CONSTANT(RESULT_SMS_CODE_FAILED);												// 94
	BIND_CONSTANT(RESULT_ACCOUNT_LIMIT_EXCEEDED);										// 95
	BIND_CONSTANT(RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED);								// 96
	BIND_CONSTANT(RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED);								// 97
	BIND_CONSTANT(RESULT_REFUND_TO_WALLET);												// 98
	BIND_CONSTANT(RESULT_EMAIL_SEND_FAILURE);											// 99
	BIND_CONSTANT(RESULT_NOT_SETTLED);													// 100
	BIND_CONSTANT(RESULT_NEED_CAPTCHA);													// 101
	BIND_CONSTANT(RESULT_GSLT_DENIED);													// 102
	BIND_CONSTANT(RESULT_GS_OWNER_DENIED);												// 103
	BIND_CONSTANT(RESULT_INVALID_ITEM_TYPE);											// 104
	BIND_CONSTANT(RESULT_IP_BANNED);													// 105
	BIND_CONSTANT(RESULT_GSLT_EXPIRED);													// 106
	BIND_CONSTANT(RESULT_INSUFFICIENT_FUNDS);											// 107
	BIND_CONSTANT(RESULT_TOO_MANY_PENDING);												// 108

	// UNIVERSE /////////////////////////////////
	BIND_CONSTANT(UNIVERSE_INVALID);													// 0
	BIND_CONSTANT(UNIVERSE_PUBLIC);														// 1
	BIND_CONSTANT(UNIVERSE_BETA);														// 2
	BIND_CONSTANT(UNIVERSE_INTERNAL);													// 3
	BIND_CONSTANT(UNIVERSE_DEV);														// 4
	BIND_CONSTANT(UNIVERSE_MAX);														// 5

	// USER HAS LICENSE FOR APP RESULT //////////
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_HAS_LICENSE);									// 0
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_DOES_NOT_HAVE_LICENSE);						// 1
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_NO_AUTH);										// 2

	// VOICE RESULT /////////////////////////////
	BIND_CONSTANT(VOICE_RESULT_OK);														// 0
	BIND_CONSTANT(VOICE_RESULT_NOT_INITIALIZED);										// 1
	BIND_CONSTANT(VOICE_RESULT_NOT_RECORDING);											// 2
	BIND_CONSTANT(VOICE_RESULT_NO_DATE);												// 3
	BIND_CONSTANT(VOICE_RESULT_BUFFER_TOO_SMALL);										// 4
	BIND_CONSTANT(VOICE_RESULT_DATA_CORRUPTED);											// 5
	BIND_CONSTANT(VOICE_RESULT_RESTRICTED);												// 6

	// VR HMD TYPE
	BIND_CONSTANT(VR_HMD_TYPE_NONE);													// -1
	BIND_CONSTANT(VR_HMD_TYPE_UNKNOWN);													// 0
	BIND_CONSTANT(VR_HMD_TYPE_HTC_DEV);													// 1
	BIND_CONSTANT(VR_HMD_TYPE_HTC_VIVEPRE);												// 2
	BIND_CONSTANT(VR_HMD_TYPE_HTC_VIVE);												// 3
	BIND_CONSTANT(VR_HMD_TYPE_HTC_UNKNOWN);												// 20
	BIND_CONSTANT(VR_HMD_TYPE_OCULUS_DK1);												// 21
	BIND_CONSTANT(VR_HMD_TYPE_OCULUS_DK2);												// 22
	BIND_CONSTANT(VR_HMD_TYPE_OCULUS_RIFT);												// 23
	BIND_CONSTANT(VR_HMD_TYPE_OCULUS_UNKNOWN);											// 40

	// REGISTER ACTIVATION CODE RESULTS//////////
	BIND_CONSTANT(ACTIVATION_CODE_RESULT_OK);											// 0
	BIND_CONSTANT(ACTIVATION_CODE_RESULT_FAIL);											// 1
	BIND_CONSTANT(ACTIVATION_CODE_RESULT_ALREADY_REGISTERED);							// 2
	BIND_CONSTANT(ACTIVATION_CODE_RESULT_TIMEOUT);										// 3
	BIND_CONSTANT(ACTIVATION_CODE_RESULT_ALREADY_OWNED);								// 4

	// AVATAR ///////////////////////////////////
	BIND_CONSTANT(AVATAR_SMALL);														// 1
	BIND_CONSTANT(AVATAR_MEDIUM);														// 2
	BIND_CONSTANT(AVATAR_LARGE);														// 3

	// FRIEND FLAGS ENUM ////////////////////////
	BIND_CONSTANT(FRIEND_FLAG_NONE);													// 0X00
	BIND_CONSTANT(FRIEND_FLAG_BLOCKED);													// 0X01
	BIND_CONSTANT(FRIEND_FLAG_FRIENDSHIP_REQUESTED);									// 0X02
	BIND_CONSTANT(FRIEND_FLAG_IMMEDIATE);												// 0X04
	BIND_CONSTANT(FRIEND_FLAG_CLAN_MEMBER);												// 0X08
	BIND_CONSTANT(FRIEND_FLAG_ON_GAME_SERVER);											// 0X10
	BIND_CONSTANT(FRIEND_FLAG_REQUESTING_FRIENDSHIP);									// 0X80
	BIND_CONSTANT(FRIEND_FLAG_REQUESTING_INFO);											// 0X100
	BIND_CONSTANT(FRIEND_FLAG_IGNORED);													// 0X200
	BIND_CONSTANT(FRIEND_FLAG_IGNORED_FRIEND);											// 0X400
	BIND_CONSTANT(FRIEND_FLAG_CHAT_MEMBER);												// 0X1000
	BIND_CONSTANT(FRIEND_FLAG_ALL);														// 0XFFFF

	// FRIEND RELATIONSHIP //////////////////////
	BIND_CONSTANT(FRIEND_RELATION_NONE);												// 0
	BIND_CONSTANT(FRIEND_RELATION_BLOCKED);												// 1
	BIND_CONSTANT(FRIEND_RELATION_REQUEST_RECIPIENT);									// 2
	BIND_CONSTANT(FRIEND_RELATION_FRIEND);												// 3
	BIND_CONSTANT(FRIEND_RELATION_REQUEST_INITIATOR);									// 4
	BIND_CONSTANT(FRIEND_RELATION_IGNORED);												// 5
	BIND_CONSTANT(FRIEND_RELATION_IGNORED_FRIEND);										// 6
	BIND_CONSTANT(FRIEND_RELATION_SUGGESTED);											// 7
	BIND_CONSTANT(FRIEND_RELATION_MAX);													// 8

	// OVERLAY TO STORE FLAG ////////////////////
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_NONE);											// 0
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_ADD_TO_CART);									// 1
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_AND_TO_CART_AND_SHOW);							// 2

	// OVERLAY TO WEB PAGE MODE /////////////////
	BIND_CONSTANT(OVERLAY_TO_WEB_PAGE_MODE_DEFAULT);									// 0
	BIND_CONSTANT(OVERLAY_TO_WEB_PAGE_MODE_MODAL);										// 1

	// PERSONA CHANGE ///////////////////////////
	BIND_CONSTANT(PERSONA_CHANGE_NAME);													// 0X0001
	BIND_CONSTANT(PERSONA_CHANGE_STATUS);												// 0X0002
	BIND_CONSTANT(PERSONA_CHANGE_COME_ONLINE);											// 0X0004
	BIND_CONSTANT(PERSONA_CHANGE_GONE_OFFLINE);											// 0X0008
	BIND_CONSTANT(PERSONA_CHANGE_GAME_PLAYED);											// 0X0010
	BIND_CONSTANT(PERSONA_CHANGE_GAME_SERVER);											// 0X0020
	BIND_CONSTANT(PERSONA_CHANGE_AVATAR);												// 0X0040
	BIND_CONSTANT(PERSONA_CHANGE_JOINED_SOURCE);										// 0X0080
	BIND_CONSTANT(PERSONA_CHANGE_LEFT_SOURCE);											// 0X0100
	BIND_CONSTANT(PERSONA_CHANGE_RELATIONSHIP_CHANGED);									// 0X0200
	BIND_CONSTANT(PERSONA_CHANGE_NAME_FIRST_SET);										// 0X0400
	BIND_CONSTANT(PERSONA_CHANGE_FACEBOOK_INFO);										// 0X0800
	BIND_CONSTANT(PERSONA_CHANGE_NICKNAME);												// 0X1000
	BIND_CONSTANT(PERSONA_CHANGE_STEAM_LEVEL);											// 0X2000

	// PERSONA STATE ////////////////////////////
	BIND_CONSTANT(PERSONA_STATE_OFFLINE);												// 0
	BIND_CONSTANT(PERSONA_STATE_ONLINE);												// 1
	BIND_CONSTANT(PERSONA_STATE_BUSY);													// 2
	BIND_CONSTANT(PERSONA_STATE_AWAY);													// 3
	BIND_CONSTANT(PERSONA_STATE_SNOOZE);												// 4
	BIND_CONSTANT(PERSONA_STATE_LOOKING_TO_TRADE);										// 5
	BIND_CONSTANT(PERSONA_STATE_LOOKING_TO_PLAY);										// 6
	BIND_CONSTANT(PERSONA_STATE_MAX);													// 7

	// USER RESTRICTION /////////////////////////
	BIND_CONSTANT(USER_RESTRICTION_NONE);												// 0
	BIND_CONSTANT(USER_RESTRICTION_UNKNOWN);											// 1
	BIND_CONSTANT(USER_RESTRICTION_ANY_CHAT);											// 2
	BIND_CONSTANT(USER_RESTRICTION_VOICE_CHAT);											// 4
	BIND_CONSTANT(USER_RESTRICTION_GROUP_CHAT);											// 8
	BIND_CONSTANT(USER_RESTRICTION_RATING);												// 16
	BIND_CONSTANT(USER_RESTRICTION_GAME_INVITES);										// 32
	BIND_CONSTANT(USER_RESTRICTION_TRADING);											// 64

	// GAME SEARCH ERROR CODE ///////////////////
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_OK);											// 1
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_SEARCH_AREADY_IN_PROGRESS);					// 2
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_NO_SEARCH_IN_PROGRESS);						// 3
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_NOT_LOBBY_LEADER);								// 4
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_NO_HOST_AVAILABLE);							// 5
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_SEARCH_PARAMS_INVALID);						// 6
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_OFFLINE);										// 7
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_NOT_AUTHORIZED);								// 8
	BIND_CONSTANT(GAME_SEARCH_ERROR_CODE_UNKNOWN_ERROR);								// 9

	// GAME SEARCH PLAYER RESULT ////////////////
	BIND_CONSTANT(PLAYER_RESULT_FAILED_TO_CONNECT);										// 1
	BIND_CONSTANT(PLAYER_RESULT_ABANDONED);												// 2
	BIND_CONSTANT(PLAYER_RESULT_KICKED);												// 3
	BIND_CONSTANT(PLAYER_RESULT_INCOMPLETE);											// 4
	BIND_CONSTANT(PLAYER_RESULT_COMPLETED);	

	// HTML KEY MODIFIERS ///////////////////////
	BIND_CONSTANT(HTML_KEY_MODIFIER_NONE);												// 0
	BIND_CONSTANT(HTML_KEY_MODIFIER_ALT_DOWN);											// (1<<0)
	BIND_CONSTANT(HTML_KEY_MODIFIER_CTRL_DOWN);											// (1<<1)
	BIND_CONSTANT(HTML_KEY_MODIFIER_SHIFT_DOWN);										// (1<<2)

	// HTML MOUSE BUTTON ////////////////////////
	BIND_CONSTANT(HTML_MOUSE_BUTTON_LEFT);												// 0
	BIND_CONSTANT(HTML_MOUSE_BUTTON_RIGHT);												// 1
	BIND_CONSTANT(HTML_MOUSE_BUTTON_MIDDLE);											// 2

	// MOUSE CURSOR /////////////////////////////
	BIND_CONSTANT(DC_USER);																// 0
	BIND_CONSTANT(DC_NONE);																// 1
	BIND_CONSTANT(DC_ARROW);															// 2
	BIND_CONSTANT(DC_IBEAM);															// 3
	BIND_CONSTANT(DC_HOUR_GLASS);														// 4
	BIND_CONSTANT(DC_WAIT_ARROW);														// 5
	BIND_CONSTANT(DC_CROSSHAIR);														// 6
	BIND_CONSTANT(DC_UP);																// 7
	BIND_CONSTANT(DC_SIZE_NW);															// 8
	BIND_CONSTANT(DC_SIZE_SE);															// 9
	BIND_CONSTANT(DC_SIZE_NE);															// 10
	BIND_CONSTANT(DC_SIZE_SW);															// 11
	BIND_CONSTANT(DC_SIZE_W);															// 12
	BIND_CONSTANT(DC_SIZE_E);															// 13
	BIND_CONSTANT(DC_SIZE_N);															// 14
	BIND_CONSTANT(DC_SIZE_S);															// 15
	BIND_CONSTANT(DC_SIZE_WE);															// 16
	BIND_CONSTANT(DC_SIZE_NS);															// 17
	BIND_CONSTANT(DC_SIZE_ALL);															// 18
	BIND_CONSTANT(DC_NO);																// 19
	BIND_CONSTANT(DC_HAND);																// 20
	BIND_CONSTANT(DC_BLANK);															// 21
	BIND_CONSTANT(DC_MIDDLE_PAN);														// 22
	BIND_CONSTANT(DC_NORTH_PAN);														// 23
	BIND_CONSTANT(DC_NORTH_EAST_PAN);													// 24
	BIND_CONSTANT(DC_EAST_PAN);															// 25
	BIND_CONSTANT(DC_SOUTH_EAST_PAN);													// 26
	BIND_CONSTANT(DC_SOUTH_PAN);														// 27
	BIND_CONSTANT(DC_SOUTH_WEST_PAN);													// 28
	BIND_CONSTANT(DC_WEST_PAN);															// 29
	BIND_CONSTANT(DC_NORTH_WEST_PAN);													// 30
	BIND_CONSTANT(DC_ALIAS);															// 31
	BIND_CONSTANT(DC_CELL);																// 32
	BIND_CONSTANT(DC_COL_RESIZE);														// 33
	BIND_CONSTANT(DC_COPY_CUR);															// 34
	BIND_CONSTANT(DC_VERTICAL_TEXT);													// 35
	BIND_CONSTANT(DC_ROW_RESIZE);														// 36
	BIND_CONSTANT(DC_ZOOM_IN);															// 37
	BIND_CONSTANT(DC_ZOOM_OUT);															// 38
	BIND_CONSTANT(DC_HELP);																// 39
	BIND_CONSTANT(DC_CUSTOM);															// 40
	BIND_CONSTANT(DC_LAST);																// 41

	// HTTP METHOD //////////////////////////////
	BIND_CONSTANT(HTTP_METHOD_INVALID);													// 0
	BIND_CONSTANT(HTTP_METHOD_GET);														// 1
	BIND_CONSTANT(HTTP_METHOD_HEAD);													// 2
	BIND_CONSTANT(HTTP_METHOD_POST);													// 3
	BIND_CONSTANT(HTTP_METHOD_PUT);														// 4
	BIND_CONSTANT(HTTP_METHOD_DELETE);													// 5
	BIND_CONSTANT(HTTP_METHOD_OPTIONS);													// 6
	BIND_CONSTANT(HTTP_METHOD_PATCH);													// 7

	// HTTP STATUS CODE /////////////////////////
	BIND_CONSTANT(HTTP_STATUS_CODE_INVALID);											// 0
	BIND_CONSTANT(HTTP_STATUS_CODE_100_CONTINUE);										// 100
	BIND_CONSTANT(HTTP_STATUS_CODE_101_SWITCHING_PROTOCOLS);							// 101
	BIND_CONSTANT(HTTP_STATUS_CODE_200_OK);												// 200
	BIND_CONSTANT(HTTP_STATUS_CODE_201_CREATED);										// 201
	BIND_CONSTANT(HTTP_STATUS_CODE_202_ACCEPTED);										// 202
	BIND_CONSTANT(HTTP_STATUS_CODE_203_NON_AUTHORITATIVE);								// 203
	BIND_CONSTANT(HTTP_STATUS_CODE_204_NO_CONTENT);										// 204
	BIND_CONSTANT(HTTP_STATUS_CODE_205_RESET_CONTENT);									// 205
	BIND_CONSTANT(HTTP_STATUS_CODE_206_PARTIAL_CONTENT);								// 206
	BIND_CONSTANT(HTTP_STATUS_CODE_300_MULTIPLE_CHOICES);								// 300
	BIND_CONSTANT(HTTP_STATUS_CODE_301_MOVED_PERMANENTLY);								// 301
	BIND_CONSTANT(HTTP_STATUS_CODE_302_FOUND);											// 302
	BIND_CONSTANT(HTTP_STATUS_CODE_303_SEE_OTHER);										// 303
	BIND_CONSTANT(HTTP_STATUS_CODE_304_NOT_MODIFIED);									// 304
	BIND_CONSTANT(HTTP_STATUS_CODE_305_USE_PROXY);										// 305
	BIND_CONSTANT(HTTP_STATUS_CODE_307_TEMPORARY_REDIRECT);								// 307
	BIND_CONSTANT(HTTP_STATUS_CODE_400_BAD_REQUEST);									// 400
	BIND_CONSTANT(HTTP_STATUS_CODE_401_UNAUTHORIZED);									// 401
	BIND_CONSTANT(HTTP_STATUS_CODE_402_PAYMENT_REQUIRED);								// 402
	BIND_CONSTANT(HTTP_STATUS_CODE_403_FORBIDDEN);										// 403
	BIND_CONSTANT(HTTP_STATUS_CODE_404_NOT_FOUND);										// 404
	BIND_CONSTANT(HTTP_STATUS_CODE_405_METHOD_NOT_ALLOWED);								// 405
	BIND_CONSTANT(HTTP_STATUS_CODE_406_NOT_ACCEPTABLE);									// 406
	BIND_CONSTANT(HTTP_STATUS_CODE_407_PROXY_AUTH_REQUIRED);							// 407
	BIND_CONSTANT(HTTP_STATUS_CODE_408_REQUEST_TIMEOUT);								// 408
	BIND_CONSTANT(HTTP_STATUS_CODE_409_CONFLICT);										// 409
	BIND_CONSTANT(HTTP_STATUS_CODE_410_GONE);											// 410
	BIND_CONSTANT(HTTP_STATUS_CODE_411_LENGTH_REQUIRED);								// 411
	BIND_CONSTANT(HTTP_STATUS_CODE_412_PRECONDITION_FAILED);							// 412
	BIND_CONSTANT(HTTP_STATUS_CODE_413_REQUEST_ENTITY_TOO_LARGE);						// 413
	BIND_CONSTANT(HTTP_STATUS_CODE_414_REQUEST_URI_TOO_LONG);							// 414
	BIND_CONSTANT(HTTP_STATUS_CODE_415_UNSUPPORTED_MEDIA_TYPE);							// 415
	BIND_CONSTANT(HTTP_STATUS_CODE_416_REQUESTED_RANGE_NOT_SATISFIABLE);				// 416
	BIND_CONSTANT(HTTP_STATUS_CODE_417_EXPECTATION_FAILED);								// 417
	BIND_CONSTANT(HTTP_STATUS_CODE_4XX_UNKNOWN);										// 418
	BIND_CONSTANT(HTTP_STATUS_CODE_429_TOO_MANY_REQUESTS);								// 429
	BIND_CONSTANT(HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR);							// 500
	BIND_CONSTANT(HTTP_STATUS_CODE_501_NOT_IMPLEMENTED);								// 501
	BIND_CONSTANT(HTTP_STATUS_CODE_502_BAD_GATEWAY);									// 502
	BIND_CONSTANT(HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE);							// 503
	BIND_CONSTANT(HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT);								// 504
	BIND_CONSTANT(HTTP_STATUS_CODE_505_HTTP_VERSION_NOT_SUPPORTED);						// 505
	BIND_CONSTANT(HTTP_STATUS_CODE_5XX_UNKNOWN);										// 599

	// INPUT ACTION ORIGIN //////////////////////
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_NONE);											// 0
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_A);												// 1
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_B);												// 2
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_X);												// 3
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_Y);												// 4
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_BUMPER);										// 5
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_BUMPER);									// 6
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFTGRIP);										// 7
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHTGRIP);										// 8
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_START);											// 9
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_BACK);											// 10
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_PAD_TOUCH);									// 11
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_PAD_SWIPE);									// 12
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_PAD_CLICK);									// 13
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_NORTH);								// 14
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_SOUTH);								// 15
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_WEST);								// 16
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_EAST);								// 17
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_PAD_TOUCH);									// 18
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_PAD_SWIPE);									// 19
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_PAD_CLICK);									// 20
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_NORTH);							// 21
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_SOUTH);							// 22
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_WEST);								// 23
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_EAST);								// 24
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_TRIGGER_PULL);								// 25
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_TRIGGER_CLICK);								// 26
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_TRIGGER_PULL);								// 27
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_RIGHT_TRIGGER_CLICK);								// 28
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_STICK_MOVE);									// 29
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_STICK_CLICK);								// 30
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_NORTH);							// 31
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_SOUTH);							// 32
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_WEST);							// 33
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_EAST);							// 34
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_GYRO_MOVE);										// 35
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_GYRO_PITCH);										// 36
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_GYRO_YAW);										// 37
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_GYRO_ROLL);										// 38
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED0);						// 39
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED1);						// 40
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED2);						// 41
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED3);						// 42
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED4);						// 43
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED5);						// 44
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED6);						// 45
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED7);						// 46
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED8);						// 47
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED9);						// 48
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED10);						// 49
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_X);											// 50
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CIRCLE);										// 51
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_TRIANGLE);									// 52
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_SQUARE);										// 53
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_BUMPER);									// 54
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_BUMPER);								// 55
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_OPTIONS);										// 56
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_SHARE);										// 57
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_TOUCH);								// 58
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_SWIPE);								// 59
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_CLICK);								// 60
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_NORTH);							// 61
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_SOUTH);							// 62
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_WEST);							// 63
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_EAST);							// 64
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_TOUCH);								// 65
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_SWIPE);								// 66
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_CLICK);								// 67
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_NORTH);						// 68
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_SOUTH);						// 69
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_WEST);							// 70
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_EAST);							// 71
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_TOUCH);							// 72
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_SWIPE);							// 73
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_CLICK);							// 74
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_NORTH);						// 75
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_SOUTH);						// 76
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_WEST);						// 77
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_EAST);						// 78
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_TRIGGER_PULL);							// 79
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_TRIGGER_CLICK);							// 80
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_TRIGGER_PULL);							// 81
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_TRIGGER_CLICK);							// 82
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_MOVE);								// 83
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_CLICK);							// 84
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_NORTH);						// 85
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_SOUTH);						// 86
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_WEST);						// 87
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_EAST);						// 88
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_MOVE);							// 89
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_CLICK);							// 90
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_NORTH);						// 91
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_SOUTH);						// 92
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_WEST);						// 93
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_EAST);						// 94
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_DPAD_NORTH);									// 95
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_DPAD_SOUTH);									// 96
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_DPAD_WEST);									// 97
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_DPAD_EAST);									// 98
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_GYRO_MOVE);									// 99
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_GYRO_PITCH);									// 100
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_GYRO_YAW);									// 101
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_GYRO_ROLL);									// 102
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED0);									// 103
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED1);									// 104
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED2);									// 105
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED3);									// 106
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED4);									// 107
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED5);									// 108
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED6);									// 109
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED7);									// 110
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED8);									// 111
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED9);									// 112
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS4_RESERVED10);									// 113
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_A);										// 114
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_B);										// 115
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_X);										// 116
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_Y);										// 117
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_BUMPER);							// 118
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_BUMPER);							// 119
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_MENU);									// 120
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_VIEW);									// 121
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_TRIGGER_PULL);						// 122
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_TRIGGER_CLICK);						// 123
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_TRIGGER_PULL);						// 124
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_TRIGGER_CLICK);					// 125
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_MOVE);						// 126
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_CLICK);						// 127
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_NORTH);					// 128
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_SOUTH);					// 129
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_WEST);					// 130
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_EAST);					// 131
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_MOVE);						// 132
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_CLICK);						// 133
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_NORTH);					// 134
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_SOUTH);					// 135
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_WEST);					// 136
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_EAST);					// 137
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_NORTH);								// 138
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_SOUTH);								// 139
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_WEST);								// 140
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_EAST);								// 141
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED0);								// 142
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED1);								// 143
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED2);								// 144
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED3);								// 145
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED4);								// 146
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED5);								// 147
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED6);								// 148
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED7);								// 149
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED8);								// 150
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED9);								// 151
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED10);								// 152
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_A);										// 153
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_B);										// 154
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_X);										// 155
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_Y);										// 156
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_BUMPER);							// 157
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_BUMPER);							// 158
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_START);									// 159
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_BACK);									// 160
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_TRIGGER_PULL);						// 161
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_TRIGGER_CLICK);						// 162
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_TRIGGER_PULL);						// 163
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_TRIGGER_CLICK);					// 164
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_MOVE);						// 165
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_CLICK);						// 166
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_NORTH);					// 167
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_SOUTH);					// 168
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_WEST);					// 169
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_EAST);					// 170
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_MOVE);						// 171
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_CLICK);						// 172
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_NORTH);					// 173
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_SOUTH);					// 174
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_WEST);					// 175
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_EAST);					// 176
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_DPAD_NORTH);								// 177
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_DPAD_SOUTH);								// 178
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_DPAD_WEST);								// 179
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_DPAD_EAST);								// 180
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED0);								// 181
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED1);								// 182
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED2);								// 183
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED3);								// 184
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED4);								// 185
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED5);								// 186
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED6);								// 187
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED7);								// 188
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED8);								// 189
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED9);								// 190
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_XBOX_360_RESERVED10);								// 191
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_A);										// 192
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_B);										// 193
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_X);										// 194
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_Y);										// 195
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_BUMPER);								// 196
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_BUMPER);								// 197
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_PLUS);										// 198
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_MINUS);									// 199
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_CAPTURE);									// 200
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_TRIGGER_PULL);						// 201
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_TRIGGER_CLICK);						// 202
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_TRIGGER_PULL);						// 203
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_TRIGGER_CLICK);						// 204
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_MOVE);							// 205
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_CLICK);							// 206
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_NORTH);					// 207
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_SOUTH);					// 208
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_WEST);						// 209
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_EAST);						// 210
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_MOVE);							// 211
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_CLICK);						// 212
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_NORTH);					// 213
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_SOUTH);					// 214
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_WEST);					// 215
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_EAST);					// 216
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_DPAD_NORTH);								// 217
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_DPAD_SOUTH);								// 218
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_DPAD_WEST);								// 219
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_DPAD_EAST);								// 220
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_MOVE);							// 221
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_PITCH);							// 222
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_YAW);								// 223
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_ROLL);							// 224
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED0);								// 225
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED1);								// 226
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED2);								// 227
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED3);								// 228
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED4);								// 229
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED5);								// 230
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED6);								// 231
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED7);								// 232
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED8);								// 233
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED9);								// 234
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_SWITCH_RESERVED10);								// 235
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_OPTION);										// 242
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CREATE);										// 243
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_MUTE);										// 244
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTPAD_TOUCH);								// 245
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTPAD_SWIPE);								// 246
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTPAD_CLICK);								// 247
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADNORTH);							// 248
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADSOUTH);							// 249
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADWEST);							// 250
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADEAST);							// 251
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_TOUCH);								// 252
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_SWIPE);								// 253
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_CLICK);								// 254
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADNORTH);							// 255
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADSOUTH);							// 256
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADWEST);							// 257
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADEAST);							// 258
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CENTERPAD_TOUCH);								// 259
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CENTERPAD_SWIPE);								// 260
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CENTERPAD_CLICK);								// 261
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADNORTH);							// 262
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADSOUTH);							// 263
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADWEST);							// 264
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADEAST);							// 265
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_PULL);							// 266
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_CLICK);							// 267
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_PULL);							// 268
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_CLICK);							// 269
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_MOVE);								// 270
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_CLICK);								// 271
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADNORTH);							// 272
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADSOUTH);							// 273
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADWEST);							// 274
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADEAST);							// 275
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_MOVE);								// 276
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_CLICK);							// 277
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADNORTH);						// 278
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADSOUTH);						// 279
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADWEST);							// 280
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADEAST);							// 281
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_DPAD_NORTH);									// 282
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_DPAD_SOUTH);									// 283
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_DPAD_WEST);									// 284
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_DPAD_EAST);									// 285
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_GYRO_MOVE);									// 286
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_GYRO_PITCH);									// 287
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_GYRO_YAW);									// 288
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_GYRO_ROLL);									// 289
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_DPAD_MOVE);									// 290
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED1);									// 291
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED2);									// 292
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED3);									// 293
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED4);									// 294
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED5);									// 295
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED6);									// 296
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED7);									// 297
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED8);									// 298
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED9);									// 299
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED10);									// 300
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED11);									// 301
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED12);									// 302
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED13);									// 303
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED14);									// 304
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED15);									// 305
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED16);									// 306
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED17);									// 307
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED18);									// 308
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED19);									// 309
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_PS5_RESERVED20);									// 310
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_A);										// 311
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_B);										// 312
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_X);										// 313
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_Y);										// 314
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_L1);									// 315
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_R1);									// 316
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_MENU);									// 317
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_VIEW);									// 318
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_TOUCH);							// 319
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_SWIPE);							// 320
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_CLICK);							// 321
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADNORTH);						// 322
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADSOUTH);						// 323
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADWEST);						// 324
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADEAST);						// 325
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_TOUCH);						// 326
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_SWIPE);						// 327
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_CLICK);						// 328
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADNORTH);					// 329
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADSOUTH);					// 330
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADWEST);						// 331
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADEAST);						// 332
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_L2_SOFTPULL);							// 333
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_L2);									// 334
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_R2_SOFTPULL);							// 335
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_R2);									// 336
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_MOVE);						// 337
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_L3);									// 338
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADNORTH);					// 339
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADSOUTH);					// 340
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADWEST);					// 341
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADEAST);					// 342
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_TOUCH);						// 343
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_MOVE);						// 344
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_R3);									// 345
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADNORTH);					// 346
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADSOUTH);					// 347
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADWEST);					// 348
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADEAST);					// 349
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_TOUCH);						// 350
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_L4);									// 351
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_R4);									// 352
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_L5);									// 353
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_R5);									// 354
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_MOVE);								// 355
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_NORTH);							// 356
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_SOUTH);							// 357
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_WEST);								// 358
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_EAST);								// 359
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_MOVE);								// 360
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_PITCH);							// 361
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_YAW);								// 362
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_ROLL);								// 363
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED1);								// 364
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED2);								// 365
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED3);								// 366
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED4);								// 367
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED5);								// 368
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED6);								// 369
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED7);								// 370
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED8);								// 371
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED9);								// 372
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED10);							// 373
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED11);							// 374
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED12);							// 375
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED13);							// 376
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED14);							// 377
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED15);							// 378
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED16);							// 379
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED17);							// 380
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED18);							// 381
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED19);							// 382
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED20);							// 383
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_COUNT);											// 384
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_MAXIMUMPOSSIBLEVALUE);							// 32767

	// STEAM INPUT TYPE /////////////////////////
	BIND_CONSTANT(INPUT_TYPE_UNKNOWN);													// 0
	BIND_CONSTANT(INPUT_TYPE_STEAM_CONTROLLER);											// 1
	BIND_CONSTANT(INPUT_TYPE_XBOX360_CONTROLLER);										// 2
	BIND_CONSTANT(INPUT_TYPE_XBOXONE_CONTROLLER);										// 3
	BIND_CONSTANT(INPUT_TYPE_GENERIC_XINPUT);											// 4
	BIND_CONSTANT(INPUT_TYPE_PS4_CONTROLLER);											// 5
	BIND_CONSTANT(INPUT_TYPE_APPLE_MFI_CONTROLLER);										// 6
	BIND_CONSTANT(INPUT_TYPE_ANDROID_CONTROLLER);										// 7
	BIND_CONSTANT(INPUT_TYPE_SWITCH_JOYCON_PAIR);										// 8
	BIND_CONSTANT(INPUT_TYPE_SWITCH_JOYCON_SINGLE);										// 9
	BIND_CONSTANT(INPUT_TYPE_SWITCH_PRO_CONTROLLER);									// 10
	BIND_CONSTANT(INPUT_TYPE_MOBILE_TOUCH);												// 11
	BIND_CONSTANT(INPUT_TYPE_PS3_CONTROLLER);											// 12
	BIND_CONSTANT(INPUT_TYPE_PS5_CONTROLLER);											// 13
	BIND_CONSTANT(INPUT_TYPE_STEAM_DECK_CONTROLLER);									// 14
	BIND_CONSTANT(INPUT_TYPE_COUNT);													// 15
	BIND_CONSTANT(INPUT_TYPE_MAXIMUM_POSSIBLE_VALUE);									// 255

	// CONFIGURATION ENABLE TYPE ////////////////
	BIND_CONSTANT(INPUT_CONFIGURATION_ENABLE_TYPE_NONE);								// 0x0000
	BIND_CONSTANT(INPUT_CONFIGURATION_ENABLE_TYPE_PLAYSTATION);							// 0x0001
	BIND_CONSTANT(INPUT_CONFIGURATION_ENABLE_TYPE_XBOX);								// 0x0002
	BIND_CONSTANT(INPUT_CONFIGURATION_ENABLE_TYPE_GENERIC);								// 0x0004
	BIND_CONSTANT(INPUT_CONFIGURATION_ENABLE_TYPE_SWITCH);								// 0x0008
	
	// GLYPH SIZE ///////////////////////////////
	BIND_CONSTANT(INPUT_GLYPH_SIZE_SMALL);												// 0
	BIND_CONSTANT(INPUT_GLYPH_SIZE_MEDIUM);												// 1
	BIND_CONSTANT(INPUT_GLYPH_SIZE_LARGE);												// 2
	BIND_CONSTANT(INPUT_GLYPH_SIZE_COUNT);												// 3

	// GLYPH STYLE //////////////////////////////
	BIND_CONSTANT(INPUT_GLYPH_STYLE_KNOCKOUT);											// 0x0
	BIND_CONSTANT(INPUT_GLYPH_STYLE_LIGHT);												// 0x1
	BIND_CONSTANT(INPUT_GLYPH_STYLE_DARK);												// 0x2
	BIND_CONSTANT(INPUT_GLYPH_STYLE_NEUTRAL_COLOR_ABXY);								// 0x10
	BIND_CONSTANT(INPUT_GLYPH_STYLE_SOLID_ABXY);										// 0x20

	// STEAM ITEM FLAGS /////////////////////////
	BIND_CONSTANT(STEAM_ITEM_NO_TRADE);													// (1<<0)
	BIND_CONSTANT(STEAM_ITEM_REMOVED);													// (1<<8)
	BIND_CONSTANT(STEAM_ITEM_CONSUMED);													// (1<<9)

	// CHAT MEMBER STATE CHANGE /////////////////
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_ENTERED);									// 0X0001
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_LEFT);										// 0X0002
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_DISCONNECTED);								// 0X0004
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_KICKED);										// 0X0008
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_BANNED);										// 0X0010

	// LOBBY COMPARISON /////////////////////////
	BIND_CONSTANT(LOBBY_COMPARISON_EQUAL_TO_OR_LESS_THAN);								// -2
	BIND_CONSTANT(LOBBY_COMPARISON_LESS_THAN);											// -1
	BIND_CONSTANT(LOBBY_COMPARISON_EQUAL);												// 0
	BIND_CONSTANT(LOBBY_COMPARISON_GREATER_THAN);										// 1
	BIND_CONSTANT(LOBBY_COMPARISON_EQUAL_TO_GREATER_THAN);								// 2
	BIND_CONSTANT(LOBBY_COMPARISON_NOT_EQUAL);											// 3

	// LOBBY DISTANCE FILTER ////////////////////
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_CLOSE);											// 0
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_DEFAULT);										// 1
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_FAR);											// 2
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_WORLDWIDE);										// 3

	// LOBBY TYPE ///////////////////////////////
	BIND_CONSTANT(LOBBY_TYPE_PRIVATE);													// 0
	BIND_CONSTANT(LOBBY_TYPE_FRIENDS_ONLY);												// 1
	BIND_CONSTANT(LOBBY_TYPE_PUBLIC);													// 2
	BIND_CONSTANT(LOBBY_TYPE_INVISIBLE);												// 3

	// MATCHMAKING SERVER RESPONSE //////////////
	BIND_CONSTANT(SERVER_RESPONDED);													// 0
	BIND_CONSTANT(SERVER_FAILED_TO_RESPOND);											// 1
	BIND_CONSTANT(NO_SERVERS_LISTED_ON_MASTER_SERVER);									// 2

	// AUDIO PLAYBACK STATUS ////////////////////
	BIND_CONSTANT(AUDIO_PLAYBACK_UNDEFINED);											// 0
	BIND_CONSTANT(AUDIO_PLAYBACK_PLAYING);												// 1
	BIND_CONSTANT(AUDIO_PLAYBACK_PAUSED);												// 2
	BIND_CONSTANT(AUDIO_PLAYBACK_IDLE);													// 3

	// P2P SEND /////////////////////////////////
	BIND_CONSTANT(P2P_SEND_UNRELIABLE);													// 0
	BIND_CONSTANT(P2P_SEND_UNRELIABLE_NO_DELAY);										// 1
	BIND_CONSTANT(P2P_SEND_RELIABLE);													// 2
	BIND_CONSTANT(P2P_SEND_RELIABLE_WITH_BUFFERING);									// 3

	// P2P SESSION ERROR ////////////////////////
	BIND_CONSTANT(P2P_SESSION_ERROR_NONE);												// 0
	BIND_CONSTANT(P2P_SESSION_ERROR_NOT_RUNNING_APP);									// 1
	BIND_CONSTANT(P2P_SESSION_ERROR_NO_RIGHTS_TO_APP);									// 2
	BIND_CONSTANT(P2P_SESSION_ERROR_DESTINATION_NOT_LOGGED_ON);							// 3
	BIND_CONSTANT(P2P_SESSION_ERROR_TIMEOUT);											// 4
	BIND_CONSTANT(P2P_SESSION_ERROR_MAX);												// 5

	// SNET SOCKET CONNECTION TYPE //////////////
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_NOT_CONNECTED);							// 0
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_UDP);										// 1
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_UDP_RELAY);								// 2

	// SNET SOCKET STATE ////////////////////////
	BIND_CONSTANT(NET_SOCKET_STATE_INVALID);											// 0
	BIND_CONSTANT(NET_SOCKET_STATE_CONNECTED);											// 1
	BIND_CONSTANT(NET_SOCKET_STATE_INITIATED);											// 10
	BIND_CONSTANT(NET_SOCKET_STATE_LOCAL_CANDIDATE_FOUND);								// 11
	BIND_CONSTANT(NET_SOCKET_STATE_RECEIVED_REMOTE_CANDIDATES);							// 12
	BIND_CONSTANT(NET_SOCKET_STATE_CHALLENGE_HANDSHAKE);								// 15
	BIND_CONSTANT(NET_SOCKET_STATE_DISCONNECTING);										// 21
	BIND_CONSTANT(NET_SOCKET_STATE_LOCAL_DISCONNECT);									// 22
	BIND_CONSTANT(NET_SOCKET_STATE_TIMEOUT_DURING_CONNECT);								// 23
	BIND_CONSTANT(NET_SOCKET_STATE_REMOTE_END_DISCONNECTED);							// 24
	BIND_CONSTANT(NET_SOCKET_STATE_BROKEN);												// 25

	// NETWORKING CONFIGURATION VALUE ///////////
	BIND_CONSTANT(NETWORKING_CONFIG_INVALID);											// 0
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_LOSS_SEND);								// 2
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_LOSS_RECV);								// 3
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_LAG_SEND);								// 4
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_LAG_RECV);								// 5
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_REORDER_SEND);							// 6
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_REORDER_RECV);							// 7
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_REORDER_TIME);							// 8
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_DUP_SEND);								// 26
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_DUP_REVC);								// 27
	BIND_CONSTANT(NETWORKING_CONFIG_FAKE_PACKET_DUP_TIME_MAX);							// 28
	BIND_CONSTANT(NETWORKING_CONFIG_TIMEOUT_INITIAL);									// 24
	BIND_CONSTANT(NETWORKING_CONFIG_TIMEOUT_CONNECTED);									// 25
	BIND_CONSTANT(NETWORKING_CONFIG_SEND_BUFFER_SIZE);									// 9
	BIND_CONSTANT(NETWORKING_CONFIG_SEND_RATE_MIN);										// 10
	BIND_CONSTANT(NETWORKING_CONFIG_SEND_RATE_MAX);										// 11
	BIND_CONSTANT(NETWORKING_CONFIG_NAGLE_TIME);										// 12
	BIND_CONSTANT(NETWORKING_CONFIG_IP_ALLOW_WITHOUT_AUTH);								// 23
	BIND_CONSTANT(NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL_INITIAL);		// 19
	BIND_CONSTANT(NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL);				// 20
	BIND_CONSTANT(NETWORKING_CONFIG_SDR_CLIENT_MIN_PINGS_BEFORE_PING_ACCURATE);			// 21
	BIND_CONSTANT(NETWORKING_CONFIG_SDR_CLIENT_SINGLE_SOCKET);							// 22
	BIND_CONSTANT(NETWORKING_CONFIG_SDR_CLIENT_FORCE_RELAY_CLUSTER);					// 29
	BIND_CONSTANT(NETWORKING_CONFIG_SDR_CLIENT_DEBUG_TICKET_ADDRESS);					// 30
	BIND_CONSTANT(NETWORKING_CONFIG_SDR_CLIENT_FORCE_PROXY_ADDR);						// 31
	BIND_CONSTANT(NETWORKING_CONFIG_LOG_LEVEL_ACK_RTT);									// 13
	BIND_CONSTANT(NETWORKING_CONFIG_LOG_LEVEL_PACKET_DECODE);							// 14
	BIND_CONSTANT(NETWORKING_CONFIG_LOG_LEVEL_MESSAGE);									// 15
	BIND_CONSTANT(NETWORKING_CONFIG_LOG_LEVEL_PACKET_GAPS);								// 16
	BIND_CONSTANT(NETWORKING_CONFIG_LOG_LEVEL_P2P_RENDEZVOUS);							// 17
	BIND_CONSTANT(NETWORKING_CONFIG_LOG_LEVEL_SRD_RELAY_PINGS);							// 18

	// NETWORKING GET CONFIGURATION VALUE RESULT
	BIND_CONSTANT(NETWORKING_GET_CONFIG_VALUE_BAD_VALUE);								// -1
	BIND_CONSTANT(NETWORKING_GET_CONFIG_VALUE_BAD_SCOPE_OBJ);							// -2
	BIND_CONSTANT(NETWORKING_GET_CONFIG_VALUE_BUFFER_TOO_SMALL);						// -3
	BIND_CONSTANT(NETWORKING_GET_CONFIG_VALUE_OK);										// 1
	BIND_CONSTANT(NETWORKING_GET_CONFIG_VALUE_OK_INHERITED);							// 2
	BIND_CONSTANT(NETWORKING_GET_CONFIG_VALUE_FORCE_32BIT);								// 0X7FFFFFFF
	
	// NETWORKING CONNECTION STATE //////////////
	BIND_CONSTANT(CONNECTION_STATE_NONE);												// 0
	BIND_CONSTANT(CONNECTION_STATE_CONNECTING);											// 1
	BIND_CONSTANT(CONNECTION_STATE_FINDING_ROUTE);										// 2
	BIND_CONSTANT(CONNECTION_STATE_CONNECTED);											// 3
	BIND_CONSTANT(CONNECTION_STATE_CLOSED_BY_PEER);										// 4
	BIND_CONSTANT(CONNECTION_STATE_PROBLEM_DETECTED_LOCALLY);							// 5
	BIND_CONSTANT(CONNECTION_STATE_FIN_WAIT);											// -1
	BIND_CONSTANT(CONNECTION_STATE_LINGER);												// -2
	BIND_CONSTANT(CONNECTION_STATE_DEAD);												// -3
	BIND_CONSTANT(CONNECTION_STATE_FORCE32BIT);											// 0X7FFFFFFF

	// NETWORKING CONNECTION END ////////////////
	BIND_CONSTANT(CONNECTION_END_INVALID);												// 0
	BIND_CONSTANT(CONNECTION_END_APP_MIN);												// 1000
	BIND_CONSTANT(CONNECTION_END_MAX);													// 1999
	BIND_CONSTANT(CONNECTION_END_APP_EXCEPTION_MIN);									// 2000
	BIND_CONSTANT(CONNECTION_END_APP_EXCEPTION_MAX);									// 2999
	BIND_CONSTANT(CONNECTION_END_LOCAL_MIN);											// 3000
	BIND_CONSTANT(CONNECTION_END_LOCAL_OFFLINE_MODE);									// 3001
	BIND_CONSTANT(CONNECTION_END_LOCAL_MANY_RELAY_CONNECTIVITY);						// 3002
	BIND_CONSTANT(CONNECTION_END_LOCAL_HOSTED_sERVER_PRIMARY_RELAY);					// 3003
	BIND_CONSTANT(CONNECTION_END_LOCAL_NETWORK_CONFIG);									// 3004
	BIND_CONSTANT(CONNECTION_END_LOCAL_RIGHTS);											// 3005
	BIND_CONSTANT(CONNECTION_END_LOCAL_MAX);											// 3999
	BIND_CONSTANT(CONNECTION_END_REMOVE_MIN);											// 4000
	BIND_CONSTANT(CONNECTION_END_REMOTE_TIMEOUT);										// 4001
	BIND_CONSTANT(CONNECTION_END_REMOTE_BAD_CRYPT);										// 4002
	BIND_CONSTANT(CONNECTION_END_REMOTE_BAD_CERT);										// 4003
	BIND_CONSTANT(CONNECTION_END_REMOTE_NOT_LOGGED_IN);									// 4004
	BIND_CONSTANT(CONNECTION_END_REMOTE_NOT_RUNNING_APP);								// 4005
	BIND_CONSTANT(CONNECTION_END_BAD_PROTOCOL_VERSION);									// 4006
	BIND_CONSTANT(CONNECTION_END_REMOTE_MAX);											// 4999
	BIND_CONSTANT(CONNECTION_END_MISC_MIN);												// 5000
	BIND_CONSTANT(CONNECTION_END_MISC_GENERIC);											// 5001
	BIND_CONSTANT(CONNECTION_END_MISC_INTERNAL_ERROR);									// 5002
	BIND_CONSTANT(CONNECTION_END_MISC_TIMEOUT);											// 5003
	BIND_CONSTANT(CONNECTION_END_MISC_RELAY_CONNECTIVITY);								// 5004
	BIND_CONSTANT(CONNECTION_END_MISC_STEAM_CONNECTIVITY);								// 5005
	BIND_CONSTANT(CONNECTION_END_MISC_NO_RELAY_SESSIONS_TO_CLIENT);						// 5006
	BIND_CONSTANT(CONNECTION_END_MISC_MAX);												// 5999

	// NETWORKING IDENTITY TYPE /////////////////
	BIND_CONSTANT(IDENTITY_TYPE_INVALID);												// 0
	BIND_CONSTANT(IDENTITY_TYPE_STEAMID);												// 16
	BIND_CONSTANT(IDENTITY_TYPE_IP_ADDRESS);											// 1
	BIND_CONSTANT(IDENTITY_TYPE_GENERIC_STRING);										// 2
	BIND_CONSTANT(IDENTITY_TYPE_GENERIC_BYTES);											// 3
	BIND_CONSTANT(IDENTITY_TYPE_UNKNOWN_TYPE);											// 4
	BIND_CONSTANT(IDENTITY_TYPE_XBOX_PAIRWISE);											// 17
	BIND_CONSTANT(IDENTITY_TYPE_SONY_PSN);												// 18
	BIND_CONSTANT(IDENTITY_TYPE_GOOGLE_STADIA);											// 19 
	BIND_CONSTANT(IDENTITY_TYPE_FORCE_32BIT);											// 0X7FFFFFFF

	// NETWORKING SOCKET DEBUG OUTPUT ///////////
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_NONE);							// 0
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_BUG);								// 1
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_ERROR);							// 2
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_IMPORTANT);						// 3
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_WARNING);							// 4
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_MSG);								// 5
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_VERBOSE);							// 6,
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_DEBUG);							// 7
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_EVERYTHING);						// 8
	BIND_CONSTANT(NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_FORCE_32BIT);						// 0X7FFFFFFF
	
	// NETWORKING AVAILABILITY //////////////////
	BIND_CONSTANT(NETWORKING_AVAILABILITY_CANNOT_TRY);									// -102
	BIND_CONSTANT(NETWORKING_AVAILABILITY_FAILED);										// -101
	BIND_CONSTANT(NETWORKING_AVAILABILITY_PREVIOUSLY);									// -100
	BIND_CONSTANT(NETWORKING_AVAILABILITY_NEVER_TRIED);									// 1
	BIND_CONSTANT(NETWORKING_AVAILABILITY_WAITING);										// 2
	BIND_CONSTANT(NETWORKING_AVAILABILITY_ATTEMPTING);									// 3
	BIND_CONSTANT(NETWORKING_AVAILABILITY_CURRENT);										// 100
	BIND_CONSTANT(NETWORKING_AVAILABILITY_UNKNOWN);										// 0
	BIND_CONSTANT(NETWORKING_AVAILABILITY_FORCE_32BIT);									// 0X7FFFFFFF

	// NETWORKING CONFIGURATION SCOPE ///////////
	BIND_CONSTANT(NETWORKING_CONFIG_SCOPE_GLOBAL);										// 1
	BIND_CONSTANT(NETWORKING_CONFIG_SCOPE_SOCKETS_INTERFACE);							// 2
	BIND_CONSTANT(NETWORKING_CONFIG_SCOPE_LISTEN_SOCKET);								// 3
	BIND_CONSTANT(NETWORKING_CONFIG_SCOPE_CONNECTION);									// 4
	BIND_CONSTANT(NETWORKING_CONFIG_SCOPE_FORCE_32BIT);									// 0X7FFFFFFF

	// NETWORKING CONFIGURATION DATA TYPE ///////
	BIND_CONSTANT(NETWORKING_CONFIG_TYPE_INT32);										// 1
	BIND_CONSTANT(NETWORKING_CONFIG_TYPE_INT64);										// 2
	BIND_CONSTANT(NETWORKING_CONFIG_TYPE_FLOAT);										// 3
	BIND_CONSTANT(NETWORKING_CONFIG_TYPE_STRING);										// 4
	BIND_CONSTANT(NETWORKING_CONFIG_TYPE_FUNCTION_PTR);									// 5
	BIND_CONSTANT(NETWORKING_CONFIG_TYPE_FORCE_32BIT);									// 0x7fffffff

	// PARENTAL SETTINGS ////////////////////////
	BIND_CONSTANT(FEATURE_INVALID);														// 0
	BIND_CONSTANT(FEATURE_STORE);														// 1
	BIND_CONSTANT(FEATURE_COMMUNITY);													// 2
	BIND_CONSTANT(FEATURE_PROFILE);														// 3
	BIND_CONSTANT(FEATURE_FRIENDS);														// 4
	BIND_CONSTANT(FEATURE_NEWS);														// 5
	BIND_CONSTANT(FEATURE_TRADING);														// 6
	BIND_CONSTANT(FEATURE_SETTINGS);													// 7
	BIND_CONSTANT(FEATURE_CONSOLE);														// 8
	BIND_CONSTANT(FEATURE_BROWSER);														// 9
	BIND_CONSTANT(FEATURE_PARENTAL_SETUP);												// 10
	BIND_CONSTANT(FEATURE_LIBRARY);														// 11
	BIND_CONSTANT(FEATURE_TEST);														// 12
	BIND_CONSTANT(FEATURE_SITE_LICENSE);												// 13
	BIND_CONSTANT(FEATURE_MAX);

	// STEAM PARTY BEACON LOCATION TYPE /////////
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATIONTYPE_INVALID);								// 0
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATIONTYPE_CHAT_GROUP);							// 1
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_TYPE_MAX);

	// STEAM PARTY BEACON LOCATION DATA /////////
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA);									// 0
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_NAME);								// 1
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_URL_SMALL);
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_URL_MEDIUM);
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_URL_LARGE);

	// REMOTE STORAGE PLATFORM //////////////////
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_NONE);										// 0
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_WINDOWS);										// (1<<0)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_OSX);											// (1<<1)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_PS3);											// (1<<2)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_LINUX);										// (1<<3)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_RESERVED2);									// (1<<4)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_ALL);											// 0XFFFFFFFF

	// REMOTE STORAGE PUBLISHED FILE VISIBILITY /
	BIND_CONSTANT(REMOTE_STORAGE_PUBLISHED_VISIBLITY_PUBLIC);							// 0
	BIND_CONSTANT(REMOTE_STORAGE_PUBLISHED_VISIBLITY_FRIENDS_ONLY);						// 1
	BIND_CONSTANT(REMOTE_STORAGE_PUBLISHED_VISIBLITY_PRIVATE);							// 2

	// UGC READ ACTION //////////////////////////
	BIND_CONSTANT(UGC_READ_CONTINUE_READING_UNTIL_FINISHED);							// 0
	BIND_CONSTANT(UGC_READ_CONTINUE_READING);											// 1
	BIND_CONSTANT(UGC_READ_CLOSE);														// 2

	// WORKSHOP ENUMERATION TYPE ////////////////
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_RANKED_BY_VOTE);							// 0
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_RECENT);									// 1
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_TRENDING);									// 2
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_FAVORITES_OF_FRIENDS);						// 3
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_VOTED_BY_FRIENDS);							// 4
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_CONTENT_BY_FRIENDS);						// 5
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_RECENT_FROM_FOLLOWED_USERS);				// 6

	// WORKSHOP FILE ACTION /////////////////////
	BIND_CONSTANT(WORKSHOP_FILE_ACTION_PLAYED);											// 0
	BIND_CONSTANT(WORKSHOP_FILE_ACTION_COMPLETED);										// 1

	// WORKSHOP FILE TYPE ///////////////////////
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_FIRST);											// 0
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_COMMUNITY);										// 0
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_MICROTRANSACTION);									// 1
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_COLLECTION);										// 2
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_ART);												// 3
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_VIDEO);											// 4
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_SCREENSHOT);										// 5
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_GAME);												// 6
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_SOFTWARE);											// 7
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_CONCEPT);											// 8
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_WEB_GUIDE);										// 9
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_INTEGRATED_GUIDE);									// 10
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_MERCH);											// 11
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_CONTROLLER_BINDING);								// 12
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_STEAMWORKS_ACCESS_INVITE);							// 13
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_STEAM_VIDEO);										// 14
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_GAME_MANAGED_ITEM);								// 15
	BIND_CONSTANT(WORKSHOP_FILE_TYPE_MAX);												// 16

	// WORKSHOP VIDEO PROVIDER //////////////////
	BIND_CONSTANT(WORKSHOP_VIDEO_PROVIDER_NONE);										// 0
	BIND_CONSTANT(WORKSHOP_VIDEO_PROVIDER_YOUTUBE);										// 1

	// WORKSHOP VOTE ////////////////////////////
	BIND_CONSTANT(WORKSHOP_VOTE_UNVOTED);												// 0
	BIND_CONSTANT(WORKSHOP_VOTE_FOR);													// 1
	BIND_CONSTANT(WORKSHOP_VOTE_AGAINST);												// 2
	BIND_CONSTANT(WORKSHOP_VOTE_LATER);													// 3

	// LOCAL FILE CHANGES ///////////////////////
	BIND_CONSTANT(LOCAL_FILE_CHANGE_INVALID);											// 0
	BIND_CONSTANT(LOCAL_FILE_CHANGE_FILE_UPDATED);										// 1
	BIND_CONSTANT(LOCAL_FILE_CHANGE_FILE_DELETED);										// 2
	
	// FILE PATH TYPES //////////////////////////
	BIND_CONSTANT(FILE_PATH_TYPE_INVALID);												// 0
	BIND_CONSTANT(FILE_PATH_TYPE_ABSOLUTE);												// 1
	BIND_CONSTANT(FILE_PATH_TYPE_API_FILENAME);											// 2

	// VR SCREENSHOT TYPE ///////////////////////
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_NONE);												// 0
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_MONO);												// 1
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_STEREO);											// 2
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_MONO_CUBE_MAP);									// 3
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_MONO_PANORAMA);									// 4
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_STEREO_PANORAMA);									// 5

	// ITEM REVIEW TYPE /////////////////////////
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_IMAGE);												// 0
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_YOUTUBE_VIDEO);										// 1
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_SKETCHFAB);											// 2
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_HORIZONTAL_CROSS);					// 3
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_LAT_LONG);							// 4
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_RESERVED_MAX);										// 255

	// ITEM STATE ///////////////////////////////
	BIND_CONSTANT(ITEM_STATE_NONE);														// 0
	BIND_CONSTANT(ITEM_STATE_SUBSCRIBED);												// 1
	BIND_CONSTANT(ITEM_STATE_LEGACY_ITEM);												// 2
	BIND_CONSTANT(ITEM_STATE_INSTALLED);												// 4
	BIND_CONSTANT(ITEM_STATE_NEEDS_UPDATE);												// 8
	BIND_CONSTANT(ITEM_STATE_DOWNLOADING);												// 16
	BIND_CONSTANT(ITEM_STATE_DOWNLOAD_PENDING);											// 32

	// ITEM STATISTIC ///////////////////////////
	BIND_CONSTANT(ITEM_STATISTIC_NUM_SUBSCRIPTIONS);									// 0
	BIND_CONSTANT(ITEM_STATISTIC_NUM_FAVORITES);										// 1
	BIND_CONSTANT(ITEM_STATISTIC_NUM_FOLLOWERS);										// 2
	BIND_CONSTANT(ITEM_STATISTIC_NUM_UNIQUE_SUBSCRIPTIONS);								// 3
	BIND_CONSTANT(ITEM_STATISTIC_NUM_UNIQUE_FAVORITES);									// 4
	BIND_CONSTANT(ITEM_STATISTIC_NUM_UNIQUE_FOLLOWERS);									// 5
	BIND_CONSTANT(ITEM_STATISTIC_NUM_UNIQUE_WEBSITE_VIEWS);								// 6
	BIND_CONSTANT(ITEM_STATISTIC_REPORT_SCORE);											// 7
	BIND_CONSTANT(ITEM_STATISTIC_NUM_SECONDS_PLAYED);									// 8
	BIND_CONSTANT(ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS);								// 9
	BIND_CONSTANT(ITEM_STATISTIC_NUM_COMMENTS);											// 10
	BIND_CONSTANT(ITEM_STATISTIC_NUM_SECONDS_PLAYED_DURING_TIME_PERIOD);				// 11
	BIND_CONSTANT(ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS_DURING_TIME_PERIOD);				// 12

	// ITEM UPDATE STATUS ///////////////////////
	BIND_CONSTANT(ITEM_UPDATE_STATUS_INVALID);											// 0
	BIND_CONSTANT(ITEM_UPDATE_STATUS_PREPARING_CONFIG);									// 1
	BIND_CONSTANT(ITEM_UPDATE_STATUS_PREPARING_CONTENT);								// 2
	BIND_CONSTANT(ITEM_UPDATE_STATUS_UPLOADING_CONTENT);								// 3
	BIND_CONSTANT(ITEM_UPDATE_STATUS_UPLOADING_PREVIEW_FILE);							// 4
	BIND_CONSTANT(ITEM_UPDATE_STATUS_COMMITTING_CHANGES);								// 5

	// UGC MATCHING UGC TYPE ////////////////////
	BIND_CONSTANT(UGC_MATCHINGUGCTYPE_ITEMS);											// 0
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_ITEMS_MTX);										// 1
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_ITEMS_READY_TO_USE);							// 2
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_COLLECTIONS);									// 3
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_ARTWORK);										// 4
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_VIDEOS);										// 5
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_SCREENSHOTS);									// 6
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_ALL_GUIDES);									// 7
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_WEB_GUIDES);									// 8
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_INTEGRATED_GUIDES);								// 9
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_USABLE_IN_GAME);								// 10
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_CONTROLLER_BINDINGS);							// 11
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_GAME_MANAGED_ITEMS);							// 12
	BIND_CONSTANT(UGC_MATCHING_UGC_TYPE_ALL);											// ~0

	// UGC QUERY ////////////////////////////////
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_VOTE);												// 0
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_PUBLICATION_DATE);								// 1
	BIND_CONSTANT(UGC_QUERY_ACCEPTED_FOR_GAME_RANKED_BY_ACCEPTANCE_DATE);				// 2
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_TREND);											// 3
	BIND_CONSTANT(UGC_QUERY_FAVORITED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE);			// 4
	BIND_CONSTANT(UGC_QUERY_CREATED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE);				// 5
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_NUM_TIMES_REPORTED);								// 6
	BIND_CONSTANT(UGC_QUERY_CREATED_BY_FOLLOWED_USERS_RANKED_BY_PUBLICATION_DATE);		// 7
	BIND_CONSTANT(UGC_QUERY_NOT_YET_RATED);												// 8
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_TOTAL_VOTES_ASC);									// 9
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_VOTES_UP);										// 10
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_TEXT_SEARCH);										// 11
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_TOTAL_UNIQUE_SUBSCRIPTIONS);						// 12
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_PLAYTIME_TREND);									// 13
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_TOTAL_PLAYTIME);									// 14
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_AVERAGE_PLAYTIME_TREND);							// 15
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_LIFETIME_AVERAGE_PLAYTIME);						// 16
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_PLAYTIME_SESSIONS_TREND);							// 17
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_LIFETIME_PLAYTIME_SESSIONS);						// 18
	BIND_CONSTANT(UGC_QUERY_RANKED_BY_LAST_UPDATED_DATE);								// 19

	// USER UGC LIST ////////////////////////////
	BIND_CONSTANT(USER_UGC_LIST_PUBLISHED);												// 0
	BIND_CONSTANT(USER_UGC_LIST_VOTED_ON);												// 1
	BIND_CONSTANT(USER_UGC_LIST_VOTED_UP);												// 2
	BIND_CONSTANT(USER_UGC_LIST_VOTED_DOWN);											// 3
	BIND_CONSTANT(USER_UGC_LIST_FAVORITED);												// 5
	BIND_CONSTANT(USER_UGC_LIST_SUBSCRIBED);											// 6
	BIND_CONSTANT(USER_UGC_LIST_USED_OR_PLAYED);										// 7
	BIND_CONSTANT(USER_UGC_LIST_FOLLOWED);												// 8

	// USER UGC LIST ORT ORDER //////////////////
	BIND_CONSTANT(USERUGCLISTSORTORDER_CREATIONORDERDESC);								// 0
	BIND_CONSTANT(USERUGCLISTSORTORDER_CREATIONORDERASC);								// 1
	BIND_CONSTANT(USERUGCLISTSORTORDER_TITLEASC);										// 2
	BIND_CONSTANT(USERUGCLISTSORTORDER_LASTUPDATEDDESC);								// 3
	BIND_CONSTANT(USERUGCLISTSORTORDER_SUBSCRIPTIONDATEDESC);							// 4
	BIND_CONSTANT(USERUGCLISTSORTORDER_VOTESCOREDESC);									// 5
	BIND_CONSTANT(USERUGCLISTSORTORDER_FORMODERATION);									// 6

	// FAILURE TYPE /////////////////////////////
	BIND_CONSTANT(FAILURE_FLUSHED_CALLBACK_QUEUE);										// 0
	BIND_CONSTANT(FAILURE_PIPE_FAIL);													// 1

	// DURATION CONTROL PROGRESS ////////////////
	BIND_CONSTANT(DURATION_CONTROL_PROGRESS_FULL);										// 0
	BIND_CONSTANT(DURATION_CONTROL_PROGRESS_HALF);										// 1
	BIND_CONSTANT(DURATION_CONTROL_PROGRESS_NONE);										// 2

	// DURATION CONTROL NOTIFICATION ////////////
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_NONE);									// 0
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_1_HOUR);								// 1
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_3_HOURS);								// 3
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_HALF_PROGRESS);							// 3
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_NO_PROGRESS);							// 4

	// LEADERBOARD DATA REQUEST /////////////////
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_GLOBAL);										// 0
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_GLOBAL_AROUND_USER);							// 1
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_FRIENDS);									// 2
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_USERS);										// 3

	// LEADERBOARD DISPLAY TYPE /////////////////
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_NONE);										// 0
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_NUMERIC);									// 1
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS);								// 2
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS);							// 3

	// LEADERBOARD SORT METHOD //////////////////
	BIND_CONSTANT(LEADERBOARD_SORT_METHOD_NONE);										// 0
	BIND_CONSTANT(LEADERBOARD_SORT_METHOD_ASCENDING);									// 1
	BIND_CONSTANT(LEADERBOARD_SORT_METHOD_DESCENDING);									// 2

	// LEADERBOARD UPLOAD SCORE METHOD //////////
	BIND_CONSTANT(LEADERBOARD_UPLOAD_SCORE_METHOD);										// 0
	BIND_CONSTANT(LEADERBOARD_UPLOAD_SCORE_METHOD_KEEP_BEST);							// 1
	BIND_CONSTANT(LEADERBOARD_UPLOAD_SCORE_METHOD_FORCE_UPDATE);						// 2

	// STEAM USER STAT TYPE /////////////////////
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_INVALID);										// 0
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_INT);											// 1
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_FLOAT);											// 2
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_AVGRATE);										// 3
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_ACHIEVEMENTS);									// 4
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_GROUPACHIEVEMENTS);								// 5
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_MAX);

	// CHECK FILE SIGNATURE /////////////////////
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_INVALID_SIGNATURE);								// 0
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_VALID_SIGNATURE);								// 1
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_FILE_NOT_FOUND);									// 2
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_APP);				// 3
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_FILE);				// 4

	// GAMEPAD TEXT INPUT LINE MODE /////////////
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_LINE_MODE_SINGLE_LINE);							// 0
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_LINE_MODE_MULTIPLE_LINES);							// 1

	// GAMEPAD TEXT INPUT MODE //////////////////
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_MODE_NORMAL);										// 0
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_MODE_PASSWORD);									// 1

	// FLOATING GAMEPAD TEXT INPUT MODE /////////
	BIND_CONSTANT(FLOATING_GAMEPAD_TEXT_INPUT_MODE_SINGLE_LINE);						// 0
	BIND_CONSTANT(FLOATING_GAMEPAD_TEXT_INPUT_MODE_MULTIPLE_LINES);						// 1
	BIND_CONSTANT(FLOATING_GAMEPAD_TEXT_INPUT_MODE_EMAIL);								// 2
	BIND_CONSTANT(FLOATING_GAMEPAD_TEXT_INPUT_MODE_NUMERIC);							// 3

	// STEAM API CALL FAILURE ///////////////////
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_NONE);											// -1
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_STEAM_GONE);									// 0
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_NETWORK_FAILURE);								// 1
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_INVALID_HANDLE);								// 2
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_MISMATCHED_CALLBACK);							// 3

	// TEXT FILTERING CONTEXT ///////////////////
	BIND_CONSTANT(TEXT_FILTERING_CONTEXT_UNKNOWN);										// 0
	BIND_CONSTANT(TEXT_FILTERING_CONTEXT_GAME_CONTENT);									// 1
	BIND_CONSTANT(TEXT_FILTERING_CONTEXT_CHAT);											// 2
	BIND_CONSTANT(TEXT_FILTERING_CONTEXT_NAME);											// 3

	// PROFILE ITEM TYPES
	BIND_CONSTANT(PROFILE_ITEM_TYPE_ANIMATED_AVATAR);									// 0
	BIND_CONSTANT(PROFILE_ITEM_TYPE_AVATAR_FRAME);										// 1
	BIND_CONSTANT(PROFILE_ITEM_TYPE_PROFILE_MODIFIER);									// 2
	BIND_CONSTANT(PROFILE_ITEM_TYPE_PROFILE_BACKGROUND);								// 3
	BIND_CONSTANT(PROFILE_ITEM_TYPE_MINI_PROFILE_BACKGROUND);							// 4
	
	// PROFILE ITEM PROPERTIES
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_IMAGE_SMALL);									// 0
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_IMAGE_LARGE);									// 1
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_INTERNAL_NAME);									// 2
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_TITLE);											// 3
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_DESCRIPTION);									// 4
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_APP_ID);										// 5
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_TYPE_ID);										// 6
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_CLASS);											// 7
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_MOVIE_WEBM);									// 8
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_MOVIE_MP4);										// 9
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_MOVIE_WEBM_SMALL);								// 10
	BIND_CONSTANT(PROFILE_ITEM_PROPERTY_MOVIE_MP4_SMALL);								// 11

	// DUALSENSE PAD TRIGGER EFFECT MODES
	BIND_CONSTANT(PAD_TRIGGER_EFFECT_MODE_OFF);											// 0
	BIND_CONSTANT(PAD_TRIGGER_EFFECT_MODE_FEEDBACK);									// 1
	BIND_CONSTANT(PAD_TRIGGER_EFFECT_MODE_WEAPON);										// 2
	BIND_CONSTANT(PAD_TRIGGER_EFFECT_MODE_VIBRATION);									// 3
	BIND_CONSTANT(PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_FEEDBACK);					// 4
	BIND_CONSTANT(PAD_TRIGGER_EFFECT_MODE_SLOPE_FEEDBACK);								// 5
	BIND_CONSTANT(PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_VIBRATION);					// 6
}

SteamServer::~SteamServer(){
	if(is_init_success){
		SteamGameServer_Shutdown();
	}

	// Clear app ID, and singleton variables ////
	singleton = NULL;
	current_app_id = 0;
}
