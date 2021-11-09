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
#define FLAG_ACTIVE = 0x01
#define FLAG_DEDICATED = 0x04
#define FLAG_LINUX = 0x08
#define FLAG_NONE = 0x00
#define FLAG_PASSWORDED = 0x10
#define FLAG_PRIVATE = 0x20
#define FLAG_SECURE = 0x02

// Define HTTP constants
#define HTTPCOOKIE_INVALID_HANDLE 0
#define HTTPREQUEST_INVALID_HANDLE 0

// Define Inventory constants
#define INVENTORY_RESULT_INVALID -1
#define ITEM_INSTANCE_ID_INVALID 0

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
	// App callbacks ////////////////////////////
	callbackDLCInstalled(this, &SteamServer::_dlc_installed),
	callbackFileDetailsResult(this, &SteamServer::_file_details_result),
	callbackNewLaunchURLParameters(this, &SteamServer::_new_launch_url_parameters),
//	callbackNewLaunchQueryParameters(this, &SteamServer::_new_launch_query_parameters),  Seems not to be found?
	callbackRegisterActivationCodeResponse(this, &SteamServer::_register_activation_code_response),
	callbackAppProofOfPurchaseKeyResponse(this, &SteamServer::_app_proof_of_purchase_key_response),
	callbackTimedTrialStatus(this, &SteamServer::_timed_trial_status),

	// HTTP callbacks ///////////////////////////
	callbackHTTPRequestCompleted(this, &SteamServer::_http_request_completed),
	callbackHTTPRequestDataReceived(this, &SteamServer::_http_request_data_received),
	callbackHTTPRequestHeadersReceived(this, &SteamServer::_http_request_headers_received),

	// Inventory callbacks //////////////////////
	callbackInventoryDefinitionUpdate(this, &SteamServer::_inventory_definition_update),
	callbackInventoryFullUpdate(this, &SteamServer::_inventory_full_update),
	callbackInventoryResultReady(this, &SteamServer::_inventory_result_ready),

	// Networking callbacks /////////////////////
	callbackP2PSessionConnectFail(this, &SteamServer::_p2p_session_connect_fail),
	callbackP2PSessionRequest(this, &SteamServer::_p2p_session_request),

	// Server callbacks /////////////////////////
	callbackServerConnectFailure(this, &SteamServer::_server_Connect_Failure),
	callbackServerConnected(this, &SteamServer::_server_Connected),
	callbackServerDisconnected(this, &SteamServer::_server_Disconnected),
	callbackClientApproved(this, &SteamServer::_client_Approved),
	callbackClientDenied(this, &SteamServer::_client_Denied),
	callbackClientKicked(this, &SteamServer::_client_Kick),
	callbackPolicyResponse(this, &SteamServer::_policy_Response),
	callbackClientGroupStatus(this, &SteamServer::_client_Group_Status),
	callbackAssociateClan(this, &SteamServer::_associate_Clan),
	callbackPlayerCompat(this, &SteamServer::_player_Compat),

	// Server Stat callbacks
	callbackStatsStored(this, &SteamServer::_stats_stored),
	callbackStatsUnloaded(this, &SteamServer::_stats_unloaded),

	// UGC callbacks ////////////////////////////
	callbackItemDownloaded(this, &SteamServer::_item_downloaded),
	callbackItemInstalled(this, &SteamServer::_item_installed),
	callbackUserSubscribedItemsListChanged(this, &SteamServer::_user_subscribed_items_list_changed),

	// Utility callbacks ////////////////////////
	callbackGamepadTextInputDismissed(this, &SteamServer::_gamepad_text_input_dismissed),
	callbackIPCountry(this, &SteamServer::_ip_country),
	callbackLowPower(this, &SteamServer::_low_power),
	callbackSteamAPICallCompleted(this, &SteamServer::_steam_api_call_completed),
	callbackSteamShutdown(this, &SteamServer::_steam_shutdown),
	callbackAppResumingFromSuspend(this, &SteamServer::_app_resuming_from_suspend),
	callbackFloatingGamepadTextInputDismissed(this, &SteamServer::_floating_gamepad_text_input_dismissed)
{
	is_init_success = false;
	singleton = this;
	current_app_id = 0;
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
	CSteamID cSteamID;
	if(account_type < 0 || account_type >= k_EAccountTypeMax){
		account_type = 1;
	}
	cSteamID.Set(steam_id, EUniverse(k_EUniversePublic), EAccountType(account_type));
	return cSteamID;
}


/////////////////////////////////////////////////
///// MAIN FUNCTIONS
/////////////////////////////////////////////////
//
// Initialize the server to Steam.
bool SteamServer::serverInit(Dictionary connect_data, int server_mode, const String& version_string){
	// Pass the dictionary data back as solitary variables
	const String& ip = connect_data["ip"];
	uint16 steamPort = connect_data["steam_port"];
	uint16 gamePort = connect_data["game_port"];
	uint16 queryPort = connect_data["query_port"];
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
	// Resolution failed - Godot 3.0 has is_invalid() to check this
	if(address == IP_Address()){
		return false;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	// Swap the bytes
	uint8_t *ip4_p = (uint8_t *)&ip4;
	for(int i = 0; i < 2; i++){
		uint8_t temp = ip4_p[i];
		ip4_p[i] = ip4_p[3-i];
		ip4_p[3-i] = temp;
	}
	if(!SteamInternal_GameServer_Init(*((uint32_t *)ip4_p), steamPort, gamePort, queryPort, mode, version_string.utf8().get_data())){
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
// Returns metadata for a DLC by index.
Array SteamServer::getDLCDataByIndex(){
	if(SteamApps() == NULL){
		return Array();
	}
	int count = SteamApps()->GetDLCCount();
	Array dlcData;
	for(int i = 0; i < count; i++){
		AppId_t app_id = 0;
		bool available = false;
		char name[128];
		bool success = SteamApps()->BGetDLCDataByIndex(i, &app_id, &available, name, 128);
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

// Check if given application/game is installed, not necessarily owned.
bool SteamServer::isAppInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsAppInstalled((AppId_t)value);
}

// Checks whether the current App ID is for Cyber Cafes.
bool SteamServer::isCybercafe(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsCybercafe();
}

// Checks if the user owns a specific DLC and if the DLC is installed
bool SteamServer::isDLCInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsDlcInstalled(value);
}

// Checks if the license owned by the user provides low violence depots.
bool SteamServer::isLowViolence(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsLowViolence();
}

// Checks if the active user is subscribed to the current App ID.
bool SteamServer::isSubscribed(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribed();
}

// Checks if the active user is subscribed to a specified AppId.
bool SteamServer::isSubscribedApp(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedApp((AppId_t)value);
}

//Checks if the active user is accessing the current app_id via a temporary Family Shared license owned by another user.
//If you need to determine the steam_id of the permanent owner of the license, use getAppOwner.
bool SteamServer::isSubscribedFromFamilySharing(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedFromFamilySharing();
}

// Checks if the user is subscribed to the current app through a free weekend.
// This function will return false for users who have a retail or other type of license.
// Suggested you contact Valve on how to package and secure your free weekend properly.
bool SteamServer::isSubscribedFromFreeWeekend(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedFromFreeWeekend();
}

// Check if game is a timed trial with limited playtime.
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

// Checks if the user has a VAC ban on their account.
bool SteamServer::isVACBanned(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsVACBanned();
}

// Return the build ID for this app; will change based on backend updates.
int SteamServer::getAppBuildId(){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetAppBuildId();
}

// Gets the install folder for a specific AppID.
String SteamServer::getAppInstallDir(AppId_t app_id){
	if(SteamApps() == NULL){
		return "";
	}
	const uint32 folderBuffer = 256;
	char *buffer = new char[folderBuffer];
	SteamApps()->GetAppInstallDir(app_id, (char*)buffer, folderBuffer);
	String appDir = buffer;
	delete[] buffer;
	return appDir;
}

// Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
uint64_t SteamServer::getAppOwner(){
	if(SteamApps() == NULL){
		return 0;
	}
	CSteamID cSteamID = SteamApps()->GetAppOwner();
	return cSteamID.ConvertToUint64();
}

// Gets a comma separated list of the languages the current app supports.
String SteamServer::getAvailableGameLanguages(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetAvailableGameLanguages();
}

// Checks if the user is running from a beta branch, and gets the name of the branch if they are.
String SteamServer::getCurrentBetaName(){
	String ret = "";
	if(SteamApps() != NULL){
		char str[1024];
		if (SteamApps()->GetCurrentBetaName(str, 1024)) {
			ret = String(str);
		}
	}
	return ret;
}

// Gets the current language that the user has set.
String SteamServer::getCurrentGameLanguage(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetCurrentGameLanguage();
}

// Get the number of DLC the user owns for a parent application/game.
int SteamServer::getDLCCount(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->GetDLCCount();
}

// Gets the download progress for optional DLC.
Dictionary SteamServer::getDLCDownloadProgress(uint32_t app_id){
	Dictionary progress;
	if(SteamApps() == NULL){
		progress["ret"] = false;
	}
	else{
		uint64 downloaded = 0;
		uint64 total = 0;
		// Get the progress
		progress["ret"] = SteamApps()->GetDlcDownloadProgress((AppId_t)app_id, &downloaded, &total);
		if(progress["ret"]){
			progress["downloaded"] = uint64_t(downloaded);
			progress["total"] = uint64_t(total);
		}
	}
	return progress;
}

// Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).
int SteamServer::getEarliestPurchaseUnixTime(int value){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
}

// Asynchronously retrieves metadata details about a specific file in the depot manifest.
void SteamServer::getFileDetails(const String& filename){
	if(SteamApps() != NULL){
		SteamApps()->GetFileDetails(filename.utf8().get_data());
	}
}

// Gets a list of all installed depots for a given App ID in mount order.
Array SteamServer::getInstalledDepots(uint32_t app_id){
	if(SteamApps() == NULL){
		return Array();
	}
	Array installedDepots;
	DepotId_t *depots = new DepotId_t[32];
	int installed = SteamApps()->GetInstalledDepots((AppId_t)app_id, depots, 32);
	for(int i = 0; i < installed; i++){
		installedDepots.append(depots[i]);
	}
	delete[] depots;
	return installedDepots;
}

// Gets the command line if the game was launched via Steam URL, e.g. steam://run/<appid>//<command line>/. This method is preferable to launching with a command line via the operating system, which can be a security risk. In order for rich presence joins to go through this and not be placed on the OS command line, you must enable "Use launch command line" from the Installation > General page on your app.
String SteamServer::getLaunchCommandLine(){
	if(SteamApps() == NULL){
		return "";
	}
	char commands;
	SteamApps()->GetLaunchCommandLine(&commands, 256);
	String commandLine;
	commandLine += commands;
	return commandLine;
}

// Gets the associated launch parameter if the game is run via steam://run/<appid>/?param1=value1;param2=value2;param3=value3 etc.
String SteamServer::getLaunchQueryParam(const String& key){
	if(SteamApps() == NULL){
		return "";
	}
	return SteamApps()->GetLaunchQueryParam(key.utf8().get_data());
}

// Allows you to install an optional DLC.
void SteamServer::installDLC(int value){
	if(SteamApps() != NULL){
		SteamApps()->InstallDLC((AppId_t)value);
	}
}

// Allows you to force verify game content on next launch.
bool SteamServer::markContentCorrupt(bool missing_files_only){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->MarkContentCorrupt(missing_files_only);
}

// Allows you to uninstall an optional DLC.
void SteamServer::uninstallDLC(int value){
	if(SteamApps() != NULL){
		SteamApps()->UninstallDLC((AppId_t)value);
	}
}

// Request all proof of purchase keys for the calling appid and asociated DLC.
// A series of AppProofOfPurchaseKeyResponse_t callbacks will be sent with appropriate appid values, ending with a final callback where the m_nAppId member is k_uAppIdInvalid (zero).
void SteamServer::requestAllProofOfPurchaseKeys(){
	if(SteamApps() != NULL){
		SteamApps()->RequestAllProofOfPurchaseKeys();
	}
}

// Request legacy cd-key for yourself or owned DLC. If you are interested in this data then make sure you provide us with a list of valid keys to be distributed to users when they purchase the game, before the game ships.
// You'll receive an AppProofOfPurchaseKeyResponse_t callback when the key is available (which may be immediately).
void SteamServer::requestAppProofOfPurchaseKey(AppId_t app_id){
	if(SteamApps() != NULL){
		SteamApps()->RequestAppProofOfPurchaseKey((AppId_t)app_id);
	}
}


/////////////////////////////////////////////////
///// HTTP
/////////////////////////////////////////////////
//
//Creates a cookie container to store cookies during the lifetime of the process. This API is just for during process lifetime, after steam restarts no cookies are persisted and you have no way to access the cookie container across repeat executions of your process.
void SteamServer::createCookieContainer(bool allow_responses_to_modify){
	if(SteamHTTP() != NULL){
		SteamHTTP()->CreateCookieContainer(allow_responses_to_modify);
	}
}

// Initializes a new HTTP request.
void SteamServer::createHTTPRequest(int request_method, const String& absolute_url){
	if(SteamHTTP() != NULL){
		SteamHTTP()->CreateHTTPRequest((EHTTPMethod)request_method, absolute_url.utf8().get_data());
	}
}

// Defers a request which has already been sent by moving it at the back of the queue.
bool SteamServer::deferHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->DeferHTTPRequest(request);
}

// Gets progress on downloading the body for the request.
float SteamServer::getHTTPDownloadProgressPct(uint32 request){
	float percentOut = 0.0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPDownloadProgressPct(request, &percentOut);
	}
	return percentOut;
}

// Check if the reason the request failed was because we timed it out (rather than some harder failure).
bool SteamServer::getHTTPRequestWasTimedOut(uint32 request){
	bool wasTimedOut = false;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPRequestWasTimedOut(request, &wasTimedOut);
	}
	return wasTimedOut;
}

// Gets the body data from an HTTP response.
uint8 SteamServer::getHTTPResponseBodyData(uint32 request, uint32 buffer_size){
	uint8 bodyData = 0; 
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodyData(request, &bodyData, buffer_size);
	}
	return bodyData;
}

// Gets the size of the body data from an HTTP response.
uint32 SteamServer::getHTTPResponseBodySize(uint32 request){
	uint32 bodySize = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodySize(request, &bodySize);
	}
	return bodySize;
}

// Checks if a header is present in an HTTP response and returns its size.
uint32 SteamServer::getHTTPResponseHeaderSize(uint32 request, const String& header_name){
	uint32 responseHeaderSize = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderSize(request, header_name.utf8().get_data(), &responseHeaderSize);
	}
	return responseHeaderSize;
}

// Gets a header value from an HTTP response.
uint8 SteamServer::getHTTPResponseHeaderValue(uint32 request, const String& header_name, uint32 buffer_size){
	uint8 valueBuffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderValue(request, header_name.utf8().get_data(), &valueBuffer, buffer_size);
	}
	return valueBuffer;
}

// Gets the body data from a streaming HTTP response.
uint8 SteamServer::getHTTPStreamingResponseBodyData(uint32 request, uint32 offset, uint32 buffer_size){
	uint8 bodyDataBuffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPStreamingResponseBodyData(request, offset, &bodyDataBuffer, buffer_size);
	}
	return bodyDataBuffer;
}

// Prioritizes a request which has already been sent by moving it at the front of the queue.
bool SteamServer::prioritizeHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->PrioritizeHTTPRequest(request);
}

// Releases a cookie container, freeing the memory allocated within Steam.
bool SteamServer::releaseCookieContainer(){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseCookieContainer(cookie_handle);
}

// Releases an HTTP request handle, freeing the memory allocated within Steam.
bool SteamServer::releaseHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseHTTPRequest(request);
}

// Sends an HTTP request.
bool SteamServer::sendHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t callHandle;
	return SteamHTTP()->SendHTTPRequest(request, &callHandle);
}

// Sends an HTTP request and streams the response back in chunks.
bool SteamServer::sendHTTPRequestAndStreamResponse(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t callHandle;
	return SteamHTTP()->SendHTTPRequestAndStreamResponse(request, &callHandle);
}

// Adds a cookie to the specified cookie container that will be used with future requests.
bool SteamServer::setHTTPCookie(const String& host, const String& url, const String& cookie){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetCookie(cookie_handle, host.utf8().get_data(), url.utf8().get_data(), cookie.utf8().get_data());
}

// Set an absolute timeout in milliseconds for the HTTP request. This is the total time timeout which is different than the network activity timeout which is set with SetHTTPRequestNetworkActivityTimeout which can bump everytime we get more data.
bool SteamServer::setHTTPRequestAbsoluteTimeoutMS(uint32 request, uint32 milliseconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestAbsoluteTimeoutMS(request, milliseconds);
}

// Set a context value for the request, which will be returned in the HTTPRequestCompleted_t callback after sending the request. This is just so the caller can easily keep track of which callbacks go with which request data. Must be called before sending the request.
bool SteamServer::setHTTPRequestContextValue(uint32 request, uint64_t context_value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestContextValue(request, context_value);
}

// Associates a cookie container to use for an HTTP request.
bool SteamServer::setHTTPRequestCookieContainer(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestCookieContainer(request, cookie_handle);
}

// Set a GET or POST parameter value on the HTTP request. Must be called prior to sending the request.
bool SteamServer::setHTTPRequestGetOrPostParameter(uint32 request, const String& name, const String& value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestGetOrPostParameter(request, name.utf8().get_data(), value.utf8().get_data());
}

// Set a request header value for the HTTP request. Must be called before sending the request.
bool SteamServer::setHTTPRequestHeaderValue(uint32 request, const String& header_name, const String& header_value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestHeaderValue(request, header_name.utf8().get_data(), header_value.utf8().get_data());
}

// Set the timeout in seconds for the HTTP request.
bool SteamServer::setHTTPRequestNetworkActivityTimeout(uint32 request, uint32 timeout_seconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestNetworkActivityTimeout(request, timeout_seconds);
}

// Sets the body for an HTTP Post request.
uint8 SteamServer::setHTTPRequestRawPostBody(uint32 request, const String& content_type, uint32 body_length){
	uint8 body = 0;
	if(SteamHTTP()){
		SteamHTTP()->SetHTTPRequestRawPostBody(request, content_type.utf8().get_data(), &body, body_length);
	}
	return body;
}

// Sets that the HTTPS request should require verified SSL certificate via machines certificate trust store. This currently only works Windows and macOS.
bool SteamServer::setHTTPRequestRequiresVerifiedCertificate(uint32 request, bool require_verified_certificate){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestRequiresVerifiedCertificate(request, require_verified_certificate);
}

// Set additional user agent info for a request.
bool SteamServer::setHTTPRequestUserAgentInfo(uint32 request, const String& user_agent_info){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestUserAgentInfo(request, user_agent_info.utf8().get_data());
}


/////////////////////////////////////////////////
///// INVENTORY
/////////////////////////////////////////////////
//
// When dealing with any inventory handles, you should call CheckResultSteamID on the result handle when it completes to verify that a remote player is not pretending to have a different user's inventory.
// Also, you must call DestroyResult on the provided inventory result when you are done with it.
//
// Grant a specific one-time promotional item to the current user.
bool SteamServer::addPromoItem(uint32 item){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->AddPromoItem(&inventory_handle, item);
}

// Grant a specific one-time promotional items to the current user.
bool SteamServer::addPromoItems(const PoolIntArray items){
	bool promo_items_added = false;
	if(SteamInventory() != NULL){
		int count = items.size();
		SteamItemDef_t *new_items = new SteamItemDef_t[items.size()];
		for(int i = 0; i < count; i++){
			new_items[i] = items[i];
		}
		promo_items_added = SteamInventory()->AddPromoItems(&inventory_handle, new_items, count);
		delete[] new_items;
	}
	return promo_items_added;
}

// Checks whether an inventory result handle belongs to the specified Steam ID.
bool SteamServer::checkResultSteamID(uint64_t steam_id_expected){
	if(SteamInventory() == NULL){
		return false;
	}
	CSteamID steam_id = (uint64)steam_id_expected;
	return SteamInventory()->CheckResultSteamID((SteamInventoryResult_t)inventory_handle, steam_id);
}

// Consumes items from a user's inventory. If the quantity of the given item goes to zero, it is permanently removed.
bool SteamServer::consumeItem(uint64_t item_consume, uint32 quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->ConsumeItem(&inventory_handle, (SteamItemInstanceID_t)item_consume, quantity);
}

// Deserializes a result set and verifies the signature bytes.
bool SteamServer::deserializeResult(){
	if(SteamInventory() == NULL){
		return false;
	}
	const int buffer = 1024;
	return SteamInventory()->DeserializeResult(&inventory_handle, &buffer, 1024, false);
}

// Destroys a result handle and frees all associated memory.
void SteamServer::destroyResult(){
	if(SteamInventory() != NULL){
		SteamInventory()->DestroyResult(inventory_handle);
	}
}

// Grant one item in exchange for a set of other items.
bool SteamServer::exchangeItems(const PoolIntArray output_items, const uint32 output_quantity, const uint64_t input_items, const uint32 input_quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->ExchangeItems(&inventory_handle, output_items.read().ptr(), &output_quantity, 1, (const uint64 *)input_items, &input_quantity, 1);
}

// Grants specific items to the current user, for developers only.
bool SteamServer::generateItems(const PoolIntArray items, const uint32 quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GenerateItems(&inventory_handle, items.read().ptr(), &quantity, items.size());
}

// Start retrieving all items in the current users inventory.
bool SteamServer::getAllItems(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GetAllItems(&inventory_handle);
}

// Gets a string property from the specified item definition.  Gets a property value for a specific item definition.
String SteamServer::getItemDefinitionProperty(uint32 definition, const String& name){
	if(SteamInventory() == NULL){
		return "";
	}
	uint32 buffer_size = 256;
	char *value = new char[buffer_size];
	SteamInventory()->GetItemDefinitionProperty(definition, name.utf8().get_data(), value, &buffer_size);
	String property = value;
	delete[] value;
	return property;
}

// Gets the state of a subset of the current user's inventory.
bool SteamServer::getItemsByID(const uint64_t id_array, uint32 count){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GetItemsByID(&inventory_handle, (const uint64 *)id_array, count);
}

// After a successful call to RequestPrices, you can call this method to get the pricing for a specific item definition.
uint64_t SteamServer::getItemPrice(uint32 definition){
	if(SteamInventory() == NULL){
		return 0;
	}
	uint64 price = 0;
	uint64 basePrice = 0;
	SteamInventory()->GetItemPrice(definition, &price, &basePrice);
	return price;
}

// After a successful call to RequestPrices, you can call this method to get all the pricing for applicable item definitions. Use the result of GetNumItemsWithPrices as the the size of the arrays that you pass in.
Array SteamServer::getItemsWithPrices(uint32 length){
	if(SteamInventory() == NULL){
		return Array();
	}
	// Create the return array
	Array priceArray;
	// Create a temporary array
	SteamItemDef_t *ids = new SteamItemDef_t[length];
	uint64 *prices = new uint64[length];
	uint64 *base_prices = new uint64[length];
	if(SteamInventory()->GetItemsWithPrices(ids, prices, base_prices, length)){
		for(uint32 i = 0; i < length; i++){
			Dictionary priceGroup;
			priceGroup["item"] = ids[i];
			priceGroup["price"] = (uint64_t)prices[i];
			priceGroup["base_prices"] = (uint64_t)base_prices[i];
			priceArray.append(priceGroup);
		}
	}
	delete[] ids;
	delete[] prices;
	delete[] base_prices;
	return priceArray;
}

// After a successful call to RequestPrices, this will return the number of item definitions with valid pricing.
uint32 SteamServer::getNumItemsWithPrices(){
	if(SteamInventory() == NULL){
		return 0;
	}
	return SteamInventory()->GetNumItemsWithPrices();
}

// Gets the dynamic properties from an item in an inventory result set.
String SteamServer::getResultItemProperty(uint32 index, const String& name){
	if(SteamInventory() != NULL){
		// Set up variables to fill
		uint32 buffer_size = 256;
		char *value = new char[buffer_size];
		SteamInventory()->GetResultItemProperty(inventory_handle, index, name.utf8().get_data(), (char*)value, &buffer_size);
		String property = value;
		delete[] value;
		return property;
	}
	return "";
}

// Get the items associated with an inventory result handle.
Array SteamServer::getResultItems(){
	if(SteamInventory() == NULL){
		return Array();
	}
	// Set up return array
	Array items;
	uint32 size = 0;
	if(SteamInventory()->GetResultItems(inventory_handle, NULL, &size)){
		itemDetails.resize(size);
		std::vector<SteamItemDetails_t> itemArray;
		if(SteamInventory()->GetResultItems(inventory_handle, itemArray.data(), &size)){
			for(uint32 i = 0; i < size; i++){
				items.push_back((uint64_t)itemArray[i].m_itemId);
			}
		}
	}
	return items;
}

// Find out the status of an asynchronous inventory result handle.
String SteamServer::getResultStatus(){
	if(SteamInventory() == NULL){
		return "";
	}
	int result = SteamInventory()->GetResultStatus(inventory_handle);
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

// Gets the server time at which the result was generated.
uint32 SteamServer::getResultTimestamp(){
	if(SteamInventory() == NULL){
		return 0;
	}
	return SteamInventory()->GetResultTimestamp(inventory_handle);
}

// Grant all potential one-time promotional items to the current user.
bool SteamServer::grantPromoItems(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GrantPromoItems(&inventory_handle);
}

// Triggers an asynchronous load and refresh of item definitions.
bool SteamServer::loadItemDefinitions(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->LoadItemDefinitions();
}

// Request the list of "eligible" promo items that can be manually granted to the given user.
void SteamServer::requestEligiblePromoItemDefinitionsIDs(uint64_t steam_id){
	if(SteamInventory() != NULL){
		CSteamID userID = (uint64)steam_id;
		SteamAPICall_t api_call = SteamInventory()->RequestEligiblePromoItemDefinitionsIDs(userID);
		callResultEligiblePromoItemDefIDs.Set(api_call, this, &SteamServer::_inventory_eligible_promo_item);
	}
}
// Request prices for all item definitions that can be purchased in the user's local currency. A SteamInventoryRequestPricesResult_t call result will be returned with the user's local currency code. After that, you can call GetNumItemsWithPrices and GetItemsWithPrices to get prices for all the known item definitions, or GetItemPrice for a specific item definition.
void SteamServer::requestPrices(){
	if(SteamInventory() != NULL){
		SteamAPICall_t api_call = SteamInventory()->RequestPrices();
		callResultRequestPrices.Set(api_call, this, &SteamServer::_inventory_request_prices_result);
	}
}

// Serialized result sets contain a short signature which can't be forged or replayed across different game sessions.
bool SteamServer::serializeResult(){
	bool result_serialized = false;
	if(SteamInventory() != NULL){
		// Set up return array
		static uint32 buffer_size = 0;
		if(SteamInventory()->SerializeResult(inventory_handle, NULL, &buffer_size)){
			char *buffer = new char[buffer_size];
			result_serialized = SteamInventory()->SerializeResult(inventory_handle, &buffer, &buffer_size);
			delete[] buffer;
		}
	}
	return result_serialized;
}

// Starts the purchase process for the user, given a "shopping cart" of item definitions that the user would like to buy. The user will be prompted in the Steam Overlay to complete the purchase in their local currency, funding their Steam Wallet if necessary, etc.
void SteamServer::startPurchase(const PoolIntArray items, const uint32 quantity){
	if(SteamInventory() != NULL){
		SteamAPICall_t api_call = SteamInventory()->StartPurchase(items.read().ptr(), &quantity, items.size());
		callResultStartPurchase.Set(api_call, this, &SteamServer::_inventory_start_purchase_result);
	}
}

// Transfer items between stacks within a user's inventory.
bool SteamServer::transferItemQuantity(uint64_t item_id, uint32 quantity, uint64_t item_destination, bool split){
	if(SteamInventory() == NULL){
		return false;
	}
	if(split){
		return SteamInventory()->TransferItemQuantity(&inventory_handle, (SteamItemInstanceID_t)item_id, quantity, k_SteamItemInstanceIDInvalid); 
	}
	else{
		return SteamInventory()->TransferItemQuantity(&inventory_handle, (SteamItemInstanceID_t)item_id, quantity, (SteamItemInstanceID_t)item_destination);
	}
}

// Trigger an item drop if the user has played a long enough period of time.
bool SteamServer::triggerItemDrop(uint32 definition){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->TriggerItemDrop(&inventory_handle, (SteamItemDef_t)definition);
}

// Starts a transaction request to update dynamic properties on items for the current user. This call is rate-limited by user, so property modifications should be batched as much as possible (e.g. at the end of a map or game session). After calling SetProperty or RemoveProperty for all the items that you want to modify, you will need to call SubmitUpdateProperties to send the request to the Steam servers. A SteamInventoryResultReady_t callback will be fired with the results of the operation.
void SteamServer::startUpdateProperties(){
	if(SteamInventory() != NULL){
		inventory_update_handle = SteamInventory()->StartUpdateProperties();
	}
}

//Submits the transaction request to modify dynamic properties on items for the current user. See StartUpdateProperties.
bool SteamServer::submitUpdateProperties(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SubmitUpdateProperties(inventory_update_handle, &inventory_handle);
}

// Removes a dynamic property for the given item.
bool SteamServer::removeProperty(uint64_t item_id, const String& name){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->RemoveProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data());
}

// Sets a dynamic property for the given item. Supported value types are strings.
bool SteamServer::setPropertyString(uint64_t item_id, const String& name, const String& value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value.utf8().get_data());
}

// Sets a dynamic property for the given item. Supported value types are boolean.
bool SteamServer::setPropertyBool(uint64_t item_id, const String& name, bool value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value);
}

// Sets a dynamic property for the given item. Supported value types are 64 bit integers.
bool SteamServer::setPropertyInt(uint64_t item_id, const String& name, uint64_t value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), (int64)value);
}

// Sets a dynamic property for the given item. Supported value types are 32 bit floats.
bool SteamServer::setPropertyFloat(uint64_t item_id, const String& name, float value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value);
}


/////////////////////////////////////////////////
///// NETWORKING
/////////////////////////////////////////////////
//
// This allows the game to specify accept an incoming packet.
bool SteamServer::acceptP2PSessionWithUser(uint64_t steam_id_remote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->AcceptP2PSessionWithUser(steam_id);
}

// Allow or disallow P2P connections to fall back to being relayed through the Steam servers if a direct connection or NAT-traversal cannot be established.
bool SteamServer::allowP2PPacketRelay(bool allow) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	return SteamNetworking()->AllowP2PPacketRelay(allow);
}

// Closes a P2P channel when you're done talking to a user on the specific channel.
bool SteamServer::closeP2PChannelWithUser(uint64_t steam_id_remote, int channel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->CloseP2PChannelWithUser(steam_id, channel);
}

// This should be called when you're done communicating with a user, as this will free up all of the resources allocated for the connection under-the-hood.
bool SteamServer::closeP2PSessionWithUser(uint64_t steam_id_remote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->CloseP2PSessionWithUser(steam_id);
}

// Fills out a P2PSessionState_t structure with details about the connection like whether or not there is an active connection.
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

// Calls IsP2PPacketAvailable() under the hood, returns the size of the available packet or zero if there is no such packet.
uint32_t SteamServer::getAvailableP2PPacketSize(int channel) {
	if (SteamNetworking() == NULL) {
		return 0;
	}
	uint32_t messageSize = 0;
	return (SteamNetworking()->IsP2PPacketAvailable(&messageSize, channel)) ? messageSize : 0;
}

// Reads in a packet that has been sent from another user via SendP2PPacket.
Dictionary SteamServer::readP2PPacket(uint32_t packet, int channel) {
	Dictionary result;
	if (SteamNetworking() == NULL) {
		return result;
	}
	PoolByteArray data;
	data.resize(packet);
	CSteamID steam_id;
	uint32_t bytesRead = 0;
	if (SteamNetworking()->ReadP2PPacket(data.write().ptr(), packet, &bytesRead, &steam_id, channel)) {
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

// Sends a P2P packet to the specified user.
bool SteamServer::sendP2PPacket(uint64_t steam_id_remote, PoolByteArray data, int send_type, int channel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = createSteamID(steam_id_remote);
	return SteamNetworking()->SendP2PPacket(steam_id, data.read().ptr(), data.size(), EP2PSend(send_type), channel);
}


/////////////////////////////////////////////////
///// SERVER FUNCTIONS
/////////////////////////////////////////////////
//
// NOTE: The following, if set, must be set before calling LogOn; they may not be changed after.
//
// This is called by SteamGameServer_Init and will usually not need to be called directly.
bool SteamServer::initGameServer(Dictionary connect_data, int server_mode, const String& version_string){
	if(SteamGameServer() == NULL){
		return false;
	}
	// Pass the dictionary data to solitary variables
	const String& ip = connect_data["ip"];
	uint16 steamPort = connect_data["steam_port"];
	uint16 gamePort = connect_data["game_port"];
	uint16 queryPort = connect_data["query_port"];
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
	// Resolution failed - Godot 3.0 has is_invalid() to check this
	if(address == IP_Address()){
		return false;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	// Swap the bytes
	uint8_t *ip4_p = (uint8_t *)&ip4;
	for(int i = 0; i < 2; i++){
		uint8_t temp = ip4_p[i];
		ip4_p[i] = ip4_p[3-i];
		ip4_p[3-i] = temp;
	}
	if(!SteamInternal_GameServer_Init(*((uint32_t *)ip4_p), steamPort, gamePort, queryPort, mode, version_string.utf8().get_data())){
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
int SteamServer::beginAuthSession(PoolByteArray ticket, int ticket_size, uint64_t steam_id){
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
int SteamServer::userHasLicenceForApp(uint64_t steam_id, AppId_t app_id){
	if(SteamGameServer() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steam_id;
	return SteamGameServer()->UserHasLicenseForApp(userID, app_id);
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
	// Swap the bytes
	uint8_t *ip4_p = (uint8_t *)&ip4;
	for(int i = 0; i < 2; i++){
		uint8_t temp = ip4_p[i];
		ip4_p[i] = ip4_p[3-i];
		ip4_p[3-i] = temp;
	}
	if(SteamGameServer()->HandleIncomingPacket(data.write().ptr(), packet, *((uint32_t *)ip4_p), port)){
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
// SERVER STATS
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
	CSteamID userID = (uint64)steam_id;
	bool unlocked = false;
	bool result = SteamGameServerStats()->GetUserAchievement(userID, name.utf8().get_data(), &unlocked);
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
		callResultStatReceived.Set(api_call, this, &SteamServer::_stat_received);
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
///// UGC
/////////////////////////////////////////////////
//
// Adds a dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
// This is a soft-dependency that is displayed on the web. It is up to the application to determine whether the item can actually be used or not.
void SteamServer::addAppDependency(uint64_t published_file_id, uint32_t app_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->AddAppDependency(fileID, app);
		callResultAddAppDependency.Set(api_call, this, &SteamServer::_add_app_dependency_result);
	}
}

// Adds a workshop item as a dependency to the specified item. If the nParentPublishedFileID item is of type k_EWorkshopFileTypeCollection, than the nChildPublishedFileID is simply added to that collection.
// Otherwise, the dependency is a soft one that is displayed on the web and can be retrieved via the ISteamUGC API using a combination of the m_unNumChildren member variable of the SteamUGCDetails_t struct and GetQueryUGCChildren.
void SteamServer::addDependency(uint64_t published_file_id, uint64_t child_published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t parent = (uint64_t)published_file_id;
		PublishedFileId_t child = (uint64_t)child_published_file_id;
		SteamAPICall_t api_call = SteamUGC()->AddDependency(parent, child);
		callResultAddUGCDependency.Set(api_call, this, &SteamServer::_add_ugc_dependency_result);
	}
}

// Adds a excluded tag to a pending UGC Query. This will only return UGC without the specified tag.
bool SteamServer::addExcludedTag(uint64_t query_handle, const String& tag_name){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddExcludedTag(handle, tag_name.utf8().get_data());
}

// Adds a key-value tag pair to an item. Keys can map to multiple different values (1-to-many relationship).
bool SteamServer::addItemKeyValueTag(uint64_t update_handle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = (uint64_t)update_handle;
	return SteamUGC()->AddItemKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

// Adds an additional preview file for the item.
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

// Adds an additional video preview from YouTube for the item.
bool SteamServer::addItemPreviewVideo(uint64_t query_handle, const String& video_id){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddItemPreviewVideo(handle, video_id.utf8().get_data());
}

// Adds a workshop item to the users favorites list.
void SteamServer::addItemToFavorite(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() != NULL){
		AppId_t app = (uint32_t)app_id;
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->AddItemToFavorites(app, fileID);
		callResultFavoriteItemListChanged.Set(api_call, this, &SteamServer::_user_favorite_items_list_changed);
	}
}

// Adds a required key-value tag to a pending UGC Query. This will only return workshop items that have a key = pKey and a value = pValue.
bool SteamServer::addRequiredKeyValueTag(uint64_t query_handle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddRequiredKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

// Adds a required tag to a pending UGC Query. This will only return UGC with the specified tag.
bool SteamServer::addRequiredTag(uint64_t query_handle, const String& tag_name){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddRequiredTag(handle, tag_name.utf8().get_data());
}

// Adds the requirement that the returned items from the pending UGC Query have at least one of the tags in the given set (logical "or"). For each tag group that is added, at least one tag from each group is required to be on the matching items.
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

// Lets game servers set a specific workshop folder before issuing any UGC commands.
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

// Creates a new workshop item with no content attached yet.
void SteamServer::createItem(AppId_t app_id, int file_type){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->CreateItem(app_id, (EWorkshopFileType)file_type);
		callResultItemCreate.Set(api_call, this, &SteamServer::_item_created);
	}
}

// Query for all matching UGC. You can use this to list all of the available UGC for your app.
uint64_t SteamServer::createQueryAllUGCRequest(int query_type, int matching_type, uint32_t creator_id, uint32_t consume_id, uint32 page){
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
	AppId_t consumer = (uint32_t)consume_id;
	UGCQueryHandle_t handle = SteamUGC()->CreateQueryAllUGCRequest(query, match, creator, consumer, page);
	return (uint64_t)handle;
}

// Query for the details of specific workshop items.
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

// Query UGC associated with a user. You can use this to list the UGC the user is subscribed to amongst other things.
uint64_t SteamServer::createQueryUserUGCRequest(int account_id, int list_type, int matching_ugc_type, int sort_order, int creator_id, int consume_id, uint32 page){
	if(SteamUGC() == NULL){
		return 0;
	}
	// Get tue universe ID from the Steam ID
	CSteamID userID = (uint64)account_id;
	AccountID_t account = userID.GetAccountID();
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
	AppId_t consumer = (int)consume_id;
	UGCQueryHandle_t handle = SteamUGC()->CreateQueryUserUGCRequest(account, list, match, sort, creator, consumer, page);
	return (uint64_t)handle;
}

// Deletes the item without prompting the user.
void SteamServer::deleteItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->DeleteItem(fileID);
		callResultDeleteItem.Set(api_call, this, &SteamServer::_item_deleted);
	}
}

// Download new or update already installed item. If returns true, wait for DownloadItemResult_t. If item is already installed, then files on disk should not be used until callback received.
// If item is not subscribed to, it will be cached for some time. If bHighPriority is set, any other item download will be suspended and this item downloaded ASAP.
bool SteamServer::downloadItem(uint64_t published_file_id, bool high_priority){
	if(SteamUGC() == NULL){
		return false;
	}
	PublishedFileId_t fileID = (uint64_t)published_file_id;
	return SteamUGC()->DownloadItem(fileID, high_priority);
}

// Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
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

// Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled set.
Dictionary SteamServer::getItemInstallInfo(uint64_t published_file_id){
	Dictionary info;
	if(SteamUGC() == NULL){
		info["ret"] = false;
		return info;
	}
	PublishedFileId_t fileID = (uint64_t)published_file_id;
	uint64 sizeOnDisk;
	char folder[1024] = { 0 };
	uint32 timeStamp;
	info["ret"] = SteamUGC()->GetItemInstallInfo((PublishedFileId_t)fileID, &sizeOnDisk, folder, sizeof(folder), &timeStamp);
	if(info["ret"]){
		info["size"] = (int)sizeOnDisk;
		info["folder"] = folder;
		info["foldersize"] = (uint32)sizeof(folder);
		info["timestamp"] = timeStamp;
	}
	return info;
}

// Gets the current state of a workshop item on this client.
int SteamServer::getItemState(uint64_t published_file_id){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t fileID = (uint64_t)published_file_id;
	return SteamUGC()->GetItemState(fileID);
}

// Gets the progress of an item update.
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

// Gets the total number of items the current user is subscribed to for the game or application.
uint32 SteamServer::getNumSubscribedItems(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUGC()->GetNumSubscribedItems();
}

// Retrieve the details of an additional preview associated with an individual workshop item after receiving a querying UGC call result.
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

// Retrieve the ids of any child items of an individual workshop item after receiving a querying UGC call result. These items can either be a part of a collection or some other dependency (see AddDependency).
Dictionary SteamServer::getQueryUGCChildren(uint64_t query_handle, uint32 index){
	Dictionary children;
	if(SteamUGC() == NULL){
		return children;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	PublishedFileId_t *child = new PublishedFileId_t[100];
	bool success = SteamUGC()->GetQueryUGCChildren(handle, index, (PublishedFileId_t*)child, 100);
	if(success){
		children["success"] = success;
		children["handle"] = (uint64_t)handle;
		children["index"] = index;
		children["children"] = child;
	}
	delete[] child;
	return children;
}

// Retrieve the details of a key-value tag associated with an individual workshop item after receiving a querying UGC call result.
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

// Retrieve the developer set metadata of an individual workshop item after receiving a querying UGC call result.
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

// Retrieve the number of additional previews of an individual workshop item after receiving a querying UGC call result.
uint32 SteamServer::getQueryUGCNumAdditionalPreviews(uint64_t query_handle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumAdditionalPreviews(handle, index);
}

// Retrieve the number of key-value tags of an individual workshop item after receiving a querying UGC call result.
uint32 SteamServer::getQueryUGCNumKeyValueTags(uint64_t query_handle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumKeyValueTags(handle, index);
}

// Retrieve the number of tags for an individual workshop item after receiving a querying UGC call result. You should call this in a loop to get the details of all the workshop items returned.
uint32 SteamServer::getQueryUGCNumTags(uint64_t query_handle, uint32 index){
	if(SteamUGC() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumTags(handle, index);
}

// Retrieve the URL to the preview image of an individual workshop item after receiving a querying UGC call result.
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

// Retrieve the details of an individual workshop item after receiving a querying UGC call result.
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
		ugcResult["fileID"] = (uint64_t)pDetails.m_nPublishedFileId;
		ugcResult["file_type"] = (uint64_t)pDetails.m_eFileType;
		ugcResult["creatorAppID"] = (uint64_t)pDetails.m_nCreatorAppID;
		ugcResult["consumerAppID"] = (uint64_t)pDetails.m_nConsumerAppID;
		ugcResult["title"] = String::utf8(pDetails.m_rgchTitle);
		ugcResult["description"] = String::utf8(pDetails.m_rgchDescription);
		ugcResult["steamIDOwner"] = (uint64_t)pDetails.m_ulSteamIDOwner;
		ugcResult["timeCreated"] = pDetails.m_rtimeCreated;
		ugcResult["timeUpdated"] = pDetails.m_rtimeUpdated;
		ugcResult["timeAddedToUserList"] = pDetails.m_rtimeAddedToUserList;
		ugcResult["visibility"] = (uint64_t)pDetails.m_eVisibility;
		ugcResult["banned"] = pDetails.m_bBanned;
		ugcResult["acceptedForUse"] = pDetails.m_bAcceptedForUse;
		ugcResult["tagsTruncated"] = pDetails.m_bTagsTruncated;
		ugcResult["tags"] = pDetails.m_rgchTags;
		ugcResult["handleFile"] = (uint64_t)pDetails.m_hFile;
		ugcResult["handlePreviewFile"] = (uint64_t)pDetails.m_hPreviewFile;
		ugcResult["fileName"] = pDetails.m_pchFileName;
		ugcResult["fileSize"] = pDetails.m_nFileSize;
		ugcResult["previewFileSize"] = pDetails.m_nPreviewFileSize;
		ugcResult["url"] = pDetails.m_rgchURL;
		ugcResult["votesUp"] = pDetails.m_unVotesUp;
		ugcResult["votesDown"] = pDetails.m_unVotesDown;
		ugcResult["score"] = pDetails.m_flScore;
		ugcResult["numChildren"] = pDetails.m_unNumChildren;
	}
	return ugcResult;
}
// Retrieve various statistics of an individual workshop item after receiving a querying UGC call result.
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

// Retrieve the "nth" tag associated with an individual workshop item after receiving a querying UGC call result.
// You should call this in a loop to get the details of all the workshop items returned.
String SteamServer::getQueryUGCTag(uint64_t query_handle, uint32 index, uint32 tag_index){
	// Set a default tag to return
	char *tag = new char[64];
	if(SteamUGC() != NULL){
		UGCQueryHandle_t handle = (uint64_t)query_handle;
		SteamUGC()->GetQueryUGCTag(handle, index, tag_index, tag, 64);
	}
	String tag_name = tag;
	delete[] tag;
	return tag_name;
}

// Retrieve the "nth" display string (usually localized) for a tag, which is associated with an individual workshop item after receiving a querying UGC call result.
// You should call this in a loop to get the details of all the workshop items returned.
String SteamServer::getQueryUGCTagDisplayName(uint64_t query_handle, uint32 index, uint32 tag_index){
	// Set a default tag name to return
	char *tag = new char[256];
	if(SteamUGC() != NULL){
		UGCQueryHandle_t handle = (uint64_t)query_handle;
		SteamUGC()->GetQueryUGCTagDisplayName(handle, index, tag_index, tag, 256);
	}
	String tagDisplay = tag;
	delete[] tag;
	return tagDisplay;
}

// Gets a list of all of the items the current user is subscribed to for the current game.
Array SteamServer::getSubscribedItems(){
	if(SteamUGC() == NULL){
		return Array();
	}
	Array subscribed;
	int numItems = SteamUGC()->GetNumSubscribedItems();
	PublishedFileId_t *items = new PublishedFileId_t[numItems];
	int itemList = SteamUGC()->GetSubscribedItems(items, numItems);
	for(int i = 0; i < itemList; i++){
		subscribed.append((uint64_t)items[i]);
	}
	delete[] items;
	return subscribed;
}

// Gets the users vote status on a workshop item.
void SteamServer::getUserItemVote(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->GetUserItemVote(fileID);
		callResultGetUserItemVote.Set(api_call, this, &SteamServer::_get_item_vote_result);
	}
}

// Releases a UGC query handle when you are done with it to free up memory.
bool SteamServer::releaseQueryUGCRequest(uint64_t query_handle){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->ReleaseQueryUGCRequest(handle);
}

// Removes the dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
void SteamServer::removeAppDependency(uint64_t published_file_id, uint32_t app_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveAppDependency(fileID, app);
		callResultRemoveAppDependency.Set(api_call, this, &SteamServer::_remove_app_dependency_result);
	}
}

// Removes a workshop item as a dependency from the specified item.
void SteamServer::removeDependency(uint64_t published_file_id, uint64_t child_published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		PublishedFileId_t childID = (uint64_t)child_published_file_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveDependency(fileID, childID);
		callResultRemoveUGCDependency.Set(api_call, this, &SteamServer::_remove_ugc_dependency_result);
	}
}

// Removes a workshop item from the users favorites list.
void SteamServer::removeItemFromFavorites(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveItemFromFavorites(app, fileID);
		callResultFavoriteItemListChanged.Set(api_call, this, &SteamServer::_user_favorite_items_list_changed);
	}
}

// Removes an existing key value tag from an item.
bool SteamServer::removeItemKeyValueTags(uint64_t update_handle, const String& key){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->RemoveItemKeyValueTags(handle, key.utf8().get_data());
}

// Removes an existing preview from an item.
bool SteamServer::removeItemPreview(uint64_t update_handle, uint32 index){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->RemoveItemPreview(handle, index);
}

// Send a UGC query to Steam.
void SteamServer::sendQueryUGCRequest(uint64_t update_handle){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamAPICall_t api_call = SteamUGC()->SendQueryUGCRequest(handle);
		callResultUGCQueryCompleted.Set(api_call, this, &SteamServer::_ugc_query_completed);
	}
}

// Sets whether results will be returned from the cache for the specific period of time on a pending UGC Query.
bool SteamServer::setAllowCachedResponse(uint64_t update_handle, uint32 max_age_seconds){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetAllowCachedResponse(handle, max_age_seconds);
}

// Sets to only return items that have a specific filename on a pending UGC Query.
bool SteamServer::setCloudFileNameFilter(uint64_t update_handle, const String& match_cloud_filename){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetCloudFileNameFilter(handle, match_cloud_filename.utf8().get_data());
}

// Sets the folder that will be stored as the content for an item.
bool SteamServer::setItemContent(uint64_t update_handle, const String& content_folder){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemContent(handle, content_folder.utf8().get_data());
}

// Sets a new description for an item.
bool SteamServer::setItemDescription(uint64_t update_handle, const String& description){
	if(SteamUGC() == NULL){
		return false;
	}
	if (description.length() > 255){
		printf("Description cannot have more than %d ASCII characters. Description not set.", 255);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemDescription(handle, description.utf8().get_data());
}

// Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
bool SteamServer::setItemMetadata(uint64_t update_handle, const String& metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	if (metadata.length() > 5000){
		printf("Metadata cannot have more than %d ASCII characters. Metadata not set.", 5000);
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemMetadata(handle, metadata.utf8().get_data());
}

// Sets the primary preview image for the item.
bool SteamServer::setItemPreview(uint64_t update_handle, const String& preview_file){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemPreview(handle, preview_file.utf8().get_data());
}

// Sets arbitrary developer specified tags on an item.
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

// Sets a new title for an item.
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

// Sets the language of the title and description that will be set in this item update.
bool SteamServer::setItemUpdateLanguage(uint64_t update_handle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}

// Sets the visibility of an item.
bool SteamServer::setItemVisibility(uint64_t update_handle, int visibility){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemVisibility(handle, (ERemoteStoragePublishedFileVisibility)visibility);
}

// Sets the language to return the title and description in for the items on a pending UGC Query.
bool SteamServer::setLanguage(uint64_t query_handle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetLanguage(handle, language.utf8().get_data());
}

// Sets whether workshop items will be returned if they have one or more matching tag, or if all tags need to match on a pending UGC Query.
bool SteamServer::setMatchAnyTag(uint64_t query_handle, bool match_any_tag){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetMatchAnyTag(handle, match_any_tag);
}

// Sets whether the order of the results will be updated based on the rank of items over a number of days on a pending UGC Query.
bool SteamServer::setRankedByTrendDays(uint64_t query_handle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetRankedByTrendDays(handle, days);
}

// Sets whether to return any additional images/videos attached to the items on a pending UGC Query.
bool SteamServer::setReturnAdditionalPreviews(uint64_t query_handle, bool return_additional_previews){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnAdditionalPreviews(handle, return_additional_previews);
}

// Sets whether to return the IDs of the child items of the items on a pending UGC Query.
bool SteamServer::setReturnChildren(uint64_t query_handle, bool return_children){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnChildren(handle, return_children);
}

// Sets whether to return any key-value tags for the items on a pending UGC Query.
bool SteamServer::setReturnKeyValueTags(uint64_t query_handle, bool return_key_value_tags){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnKeyValueTags(handle, return_key_value_tags);
}

// Sets whether to return the full description for the items on a pending UGC Query.
bool SteamServer::setReturnLongDescription(uint64_t query_handle, bool return_long_description){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnLongDescription(handle, return_long_description);
}

// Sets whether to return the developer specified metadata for the items on a pending UGC Query.
bool SteamServer::setReturnMetadata(uint64_t query_handle, bool return_metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnMetadata(handle, return_metadata);
}

// Sets whether to only return IDs instead of all the details on a pending UGC Query.
bool SteamServer::setReturnOnlyIDs(uint64_t query_handle, bool return_only_ids){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnOnlyIDs(handle, return_only_ids);
}

// Sets whether to return the the playtime stats on a pending UGC Query.
bool SteamServer::setReturnPlaytimeStats(uint64_t query_handle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnPlaytimeStats(handle, days);
}

// Sets whether to only return the the total number of matching items on a pending UGC Query.
bool SteamServer::setReturnTotalOnly(uint64_t query_handle, bool return_total_only){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnTotalOnly(handle, return_total_only);
}

// Sets a string to that items need to match in either the title or the description on a pending UGC Query.
bool SteamServer::setSearchText(uint64_t query_handle, const String& search_text){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetSearchText(handle, search_text.utf8().get_data());
}

// Allows the user to rate a workshop item up or down.
void SteamServer::setUserItemVote(uint64_t published_file_id, bool vote_up){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->SetUserItemVote(fileID, vote_up);
		callResultSetUserItemVote.Set(api_call, this, &SteamServer::_set_user_item_vote);
	}
}

// Starts the item update process.
uint64_t SteamServer::startItemUpdate(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() == NULL){
		return 0;
	}
	AppId_t app = (uint32_t)app_id;
	PublishedFileId_t fileID = (uint64_t)published_file_id;
	return SteamUGC()->StartItemUpdate(app, fileID);
}

// Start tracking playtime on a set of workshop items.
void SteamServer::startPlaytimeTracking(Array published_file_ids){
	if(SteamUGC() != NULL){
		uint32 fileCount = published_file_ids.size();
		if(fileCount > 0){
			PublishedFileId_t *file_ids = new PublishedFileId_t[fileCount];
			for(uint32 i = 0; i < fileCount; i++){
				file_ids[i] = (uint64_t)published_file_ids[i];
			}
			SteamAPICall_t api_call = SteamUGC()->StartPlaytimeTracking(file_ids, fileCount);
			callResultStartPlaytimeTracking.Set(api_call, this, &SteamServer::_start_playtime_tracking);
			delete[] file_ids;
		}
	}
}

// Stop tracking playtime on a set of workshop items.
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
			callResultStopPlaytimeTracking.Set(api_call, this, &SteamServer::_stop_playtime_tracking);
			delete[] file_ids;
		}
	}
}

// Stop tracking playtime of all workshop items.
void SteamServer::stopPlaytimeTrackingForAllItems(){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->StopPlaytimeTrackingForAllItems();
		callResultStopPlaytimeTracking.Set(api_call, this, &SteamServer::_stop_playtime_tracking);
	}
}

// Returns any app dependencies that are associated with the given item.
void SteamServer::getAppDependencies(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->GetAppDependencies(fileID);
		callResultGetAppDependencies.Set(api_call, this, &SteamServer::_get_app_dependencies_result);
	}
}

// Uploads the changes made to an item to the Steam Workshop; to be called after setting your changes.
void SteamServer::submitItemUpdate(uint64_t update_handle, const String& change_note){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamAPICall_t api_call = SteamUGC()->SubmitItemUpdate(handle, change_note.utf8().get_data());
		callResultItemUpdate.Set(api_call, this, &SteamServer::_item_updated);
	}
}

// Subscribe to a workshop item. It will be downloaded and installed as soon as possible.
void SteamServer::subscribeItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->SubscribeItem(fileID);
		callResultSubscribeItem.Set(api_call, this, &SteamServer::_subscribe_item);
	}
}

// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends.
void SteamServer::suspendDownloads(bool suspend){
	if(SteamUGC() != NULL){
		SteamUGC()->SuspendDownloads(suspend);
	}
}

// Unsubscribe from a workshop item. This will result in the item being removed after the game quits.
void SteamServer::unsubscribeItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->UnsubscribeItem(fileID);
		callResultUnsubscribeItem.Set(api_call, this, &SteamServer::_unsubscribe_item);
	}
}

// Updates an existing additional preview file for the item.
bool SteamServer::updateItemPreviewFile(uint64_t update_handle, uint32 index, const String& preview_file){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->UpdateItemPreviewFile(handle, index, preview_file.utf8().get_data());
}

// Updates an additional video preview from YouTube for the item.
bool SteamServer::updateItemPreviewVideo(uint64_t update_handle, uint32 index, const String& video_id){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->UpdateItemPreviewVideo(handle, index, video_id.utf8().get_data());
}

// Show the app's latest Workshop EULA to the user in an overlay window, where they can accept it or not.
bool SteamServer::showWorkshopEULA(){
	if(SteamUGC() == NULL){
		return false;
	}
	return SteamUGC()->ShowWorkshopEULA();
}

// Retrieve information related to the user's acceptance or not of the app's specific Workshop EULA.
void SteamServer::getWorkshopEULAStatus(){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->GetWorkshopEULAStatus();
		callResultWorkshopEULAStatus.Set(api_call, this, &SteamServer::_workshop_eula_status);
	}
}


/////////////////////////////////////////////////
///// UTILS
/////////////////////////////////////////////////
//
// Filters the provided input message and places the filtered result into pchOutFilteredText.
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

// Used to get the failure reason of a call result. The primary usage for this function is debugging. The failure reasons are typically out of your control and tend to not be very important. Just keep retrying your API Call until it works.
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

// Get the Steam ID of the running application/game.
int SteamServer::getAppID(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetAppID();
}

// Get the amount of battery power, clearly for laptops.
int SteamServer::getCurrentBatteryPower(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetCurrentBatteryPower();
}

// Gets the image bytes from an image handle.
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

// Gets the size of a Steam image handle.
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

// Returns the number of IPC calls made since the last time this function was called.
uint32 SteamServer::getIPCCallCount(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetIPCCallCount();
}

// Get the user's country by IP.
String SteamServer::getIPCountry(){
	if(SteamUtils() == NULL){
		return "";
	}
	return SteamUtils()->GetIPCountry();
}

// Return amount of time, in seconds, user has spent in this session.
int SteamServer::getSecondsSinceAppActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceAppActive();
}

// Returns the number of seconds since the user last moved the mouse.
int SteamServer::getSecondsSinceComputerActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceComputerActive();
}

// Get the actual time.
int SteamServer::getServerRealTime(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetServerRealTime();
}

// Get the Steam user interface language.
String SteamServer::getSteamUILanguage(){
	if(SteamUtils() == NULL){
		return "";
	}
	return SteamUtils()->GetSteamUILanguage();
}

// Initializes text filtering. Returns false if filtering is unavailable for the language the user is currently running in. If the language is unsupported, the FilterText API will act as a passthrough.
bool SteamServer::initFilterText(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->InitFilterText();
}

// Checks if an API Call is completed. Provides the backend of the CallResult wrapper.
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

// Returns true/false if Steam overlay is enabled.
bool SteamServer::isOverlayEnabled(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsOverlayEnabled();
}

// Returns whether the current launcher is a Steam China launcher. You can cause the client to behave as the Steam China launcher by adding -dev -steamchina to the command line when running Steam.
bool SteamServer::isSteamChinaLauncher(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamChinaLauncher();
}

// Returns true if Steam & the Steam Overlay are running in Big Picture mode.
bool SteamServer::isSteamInBigPictureMode(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamInBigPictureMode();
}

// Is Steam running in VR?
bool SteamServer::isSteamRunningInVR(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->IsSteamRunningInVR();
}

// Checks if the HMD view will be streamed via Steam In-Home Streaming.
bool SteamServer::isVRHeadsetStreamingEnabled(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsVRHeadsetStreamingEnabled();	
}

// Checks if the Overlay needs a present. Only required if using event driven render updates.
bool SteamServer::overlayNeedsPresent(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->BOverlayNeedsPresent();
}

// Sets the inset of the overlay notification from the corner specified by SetOverlayNotificationPosition.
void SteamServer::setOverlayNotificationInset(int horizontal, int vertical){
	if(SteamUtils() != NULL){
		SteamUtils()->SetOverlayNotificationInset(horizontal, vertical);
	}
}

// Set the position where overlay shows notifications.
void SteamServer::setOverlayNotificationPosition(int pos){
	if((pos >= 0) && (pos < 4) && (SteamUtils() != NULL)){
		SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
	}
}
// Set whether the HMD content will be streamed via Steam In-Home Streaming.
void SteamServer::setVRHeadsetStreamingEnabled(bool enabled){
	if(SteamUtils() != NULL){
		SteamUtils()->SetVRHeadsetStreamingEnabled(enabled);
	}
}
// Activates the Big Picture text input dialog which only supports gamepad input.
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

// Ask SteamUI to create and render its OpenVR dashboard.
void SteamServer::startVRDashboard(){
	if(SteamUtils() != NULL){
		SteamUtils()->StartVRDashboard();
	}
}


/////////////////////////////////////////////////
///// SIGNALS / CALLBACKS
/////////////////////////////////////////////////
//
// STEAMWORKS ERROR SIGNAL //////////////////////
//
// Intended to serve as generic error messaging for failed call results
void SteamServer::steamworksError(const String& failed_signal){
	// Print the error message showing the failed signal
	printf("[STEAM] IO Failure for call result: ");
	// Emit the signal to inform the user of the failure
	emit_signal("steamworks_error", failed_signal, "io failure");
}

// APPS CALLBACKS ///////////////////////////////
//
// Triggered after the current user gains ownership of DLC and that DLC is installed.
void SteamServer::_dlc_installed(DlcInstalled_t* callData){
	uint32_t app_id = (AppId_t)callData->m_nAppID;
	emit_signal("dlc_installed", app_id);
}

// Called after requesting the details of a specific file.
void SteamServer::_file_details_result(FileDetailsResult_t* fileData){
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

// Posted after the user executes a steam url with command line or query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running. The new params can be queried with getLaunchCommandLine and getLaunchQueryParam.
void SteamServer::_new_launch_url_parameters(NewUrlLaunchParameters_t* callData){
	emit_signal("new_launch_url_parameters");
}

// Posted after the user executes a steam url with query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running. The new params can be queried with getLaunchQueryParam.
// ISSUE: when compiling says it is an undeclared identifier
// void SteamServer::_new_launch_query_parameters(NewLaunchQueryParameters_t* callData){
//	emit_signal("new_launch_query_parameters");
//}

// Purpose: response to RegisterActivationCode()
void SteamServer::_register_activation_code_response(RegisterActivationCodeResponse_t* call_data){
	int result = call_data->m_eResult;
	uint32 package_registered = call_data->m_unPackageRegistered;
	emit_signal("register_activation_code_response", result, package_registered);
}

// Purpose: response to RequestAppProofOfPurchaseKey/RequestAllProofOfPurchaseKeys for supporting third-party CD keys, or other proof-of-purchase systems.
void SteamServer::_app_proof_of_purchase_key_response(AppProofOfPurchaseKeyResponse_t* call_data){
	int result = call_data->m_eResult;
	uint32 app_id = call_data->m_nAppID;
	uint32 key_length = call_data->m_cchKeyLength;
	char *key = new char[k_cubAppProofOfPurchaseKeyMax];
	sprintf(key, "%s", call_data->m_rgchKey);
	emit_signal("app_proof_of_purchase_key_response", result, app_id, key_length, key);
	delete[] key;
}

// Purpose: called for games in Timed Trial mode
void SteamServer::_timed_trial_status(TimedTrialStatus_t* call_data){
	int app_id = call_data->m_unAppID;
	bool is_offline = call_data->m_bIsOffline;
	uint32 seconds_allowed = call_data->m_unSecondsAllowed;
	uint32 seconds_played = call_data->m_unSecondsPlayed;
	emit_signal("timed_trial_status", app_id, is_offline, seconds_allowed, seconds_played);
}

// HTTP CALLBACKS ///////////////////////////////
//
// Result when an HTTP request completes. If you're using GetHTTPStreamingResponseBodyData then you should be using the HTTPRequestHeadersReceived_t or HTTPRequestDataReceived_t.
void SteamServer::_http_request_completed(HTTPRequestCompleted_t* callData){
	cookie_handle = callData->m_hRequest;
	uint64_t context_value = callData->m_ulContextValue;
	bool requestSuccess = callData->m_bRequestSuccessful;
	int statusCode = callData->m_eStatusCode;
	uint32 bodySize = callData->m_unBodySize;
	emit_signal("http_request_completed", context_value, requestSuccess, statusCode, bodySize);
}

// Triggered when a chunk of data is received from a streaming HTTP request.
void SteamServer::_http_request_data_received(HTTPRequestDataReceived_t* callData){
	cookie_handle = callData->m_hRequest;
	uint64_t context_value = callData->m_ulContextValue;
	uint32 offset = callData->m_cOffset;
	uint32 bytesReceived = callData->m_cBytesReceived;
	emit_signal("http_request_data_received", context_value, offset, bytesReceived);
}

// Triggered when HTTP headers are received from a streaming HTTP request.
void SteamServer::_http_request_headers_received(HTTPRequestHeadersReceived_t* callData){
	cookie_handle = callData->m_hRequest;
	uint64_t context_value = callData->m_ulContextValue;
	emit_signal("http_request_headers_received", context_value);
}

// INVENTORY CALLBACKS //////////////////////////
//
// This callback is triggered whenever item definitions have been updated, which could be in response to LoadItemDefinitions or any time new item definitions are available (eg, from the dynamic addition of new item types while players are still in-game).
void SteamServer::_inventory_definition_update(SteamInventoryDefinitionUpdate_t *callData){
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

// Returned when you have requested the list of "eligible" promo items that can be manually granted to the given user. These are promo items of type "manual" that won't be granted automatically.
void SteamServer::_inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *callData, bool ioFailure){
	// Clean up call data
	CSteamID steam_id = callData->m_steamID;
	int result = callData->m_result;
	int eligible = callData->m_numEligiblePromoItemDefs;
	bool cached = callData->m_bCachedData;
	// Create the return array
	Array definitions;
	// Create the temporary ID array
	SteamItemDef_t *id_array = new SteamItemDef_t[eligible];
	// Convert eligible size
	uint32 arraySize = (int)eligible;
	// Get the list
	if(SteamInventory()->GetEligiblePromoItemDefinitionIDs(steam_id, id_array, &arraySize)){
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

// Triggered when GetAllItems successfully returns a result which is newer / fresher than the last known result. (It will not trigger if the inventory hasn't changed, or if results from two overlapping calls are reversed in flight and the earlier result is already known to be stale/out-of-date.)
// The regular SteamInventoryResultReady_t callback will still be triggered immediately afterwards; this is an additional notification for your convenience.
void SteamServer::_inventory_full_update(SteamInventoryFullUpdate_t *callData){
	// Set the handle
	inventory_handle = callData->m_handle;
	// Send the handle back to the user
	emit_signal("inventory_full_update", callData->m_handle);
}

// This is fired whenever an inventory result transitions from k_EResultPending to any other completed state, see GetResultStatus for the complete list of states. There will always be exactly one callback per handle.
void SteamServer::_inventory_result_ready(SteamInventoryResultReady_t *callData){
	emit_signal("inventory_result_ready", callData->m_handle, callData->m_result);
}

// Returned after StartPurchase is called.
void SteamServer::_inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *callData, bool ioFailure){
	if(!ioFailure){
		if(callData->m_result == k_EResultOK){
			emit_signal("inventory_start_purchase_result", "success", (uint64_t)callData->m_ulOrderID, (uint64_t)callData->m_ulTransID);
		}
		else{
			emit_signal("inventory_start_purchase_result", "failure", 0, 0);
		}
	}
}

// Returned after RequestPrices is called.
void SteamServer::_inventory_request_prices_result(SteamInventoryRequestPricesResult_t *callData, bool ioFailure){
	if(!ioFailure){
		emit_signal("inventory_request_prices_result", callData->m_result, callData->m_rgchCurrency);
	}
}

// NETWORKING CALLBACKS /////////////////////////
//
// Called when packets can't get through to the specified user. All queued packets unsent at this point will be dropped, further attempts to send will retry making the connection (but will be dropped if we fail again).
void SteamServer::_p2p_session_connect_fail(P2PSessionConnectFail_t* callData) {
	uint64_t steam_id_remote = callData->m_steamIDRemote.ConvertToUint64();
	uint8_t sessionError = callData->m_eP2PSessionError;
	emit_signal("p2p_session_connect_fail", steam_id_remote, sessionError);
}

// A user wants to communicate with us over the P2P channel via the sendP2PPacket. In response, a call to acceptP2PSessionWithUser needs to be made, if you want to open the network channel with them.
void SteamServer::_p2p_session_request(P2PSessionRequest_t* callData){
	uint64_t steam_id_remote = callData->m_steamIDRemote.ConvertToUint64();
	emit_signal("p2p_session_request", steam_id_remote);
}

// REMOTE STORAGE CALLBACKS /////////////////////
//
// Called when the user has unsubscribed from a piece of UGC. Result from ISteamUGC::UnsubscribeItem.
void SteamServer::_unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t* callData, bool ioFailure){
	int result = callData->m_eResult;
	int fileID = callData->m_nPublishedFileId;
	emit_signal("unsubscribe_item", result, fileID);
}

// Called when the user has subscribed to a piece of UGC. Result from ISteamUGC::SubscribeItem.
void SteamServer::_subscribe_item(RemoteStorageSubscribePublishedFileResult_t* callData, bool ioFailure){
	int result = callData->m_eResult;
	int fileID = callData->m_nPublishedFileId;
	emit_signal("subscribe_item", result, fileID);
}

// SERVER CALLBACKS /////////////////////////////
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
	uint64_t steam_id = clientData->m_SteamID.ConvertToUint64();
	uint64_t ownerID = clientData->m_OwnerSteamID.ConvertToUint64();
	emit_signal("client_approved", steam_id, ownerID);
}

// Client has been denied to connection to this game server.
void SteamServer::_client_Denied(GSClientDeny_t* clientData){
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
void SteamServer::_client_Kick(GSClientKick_t* clientData){
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
void SteamServer::_policy_Response(GSPolicyResponse_t* policyData){
	uint8 secure = policyData->m_bSecure;
	emit_signal("server_secure", secure);
}

// Sent as a reply to RequestUserGroupStatus().
void SteamServer::_client_Group_Status(GSClientGroupStatus_t* clientData){
	uint64_t steam_id = clientData->m_SteamIDUser.ConvertToUint64();
	uint64_t group_id = clientData->m_SteamIDGroup.ConvertToUint64();
	bool member = clientData->m_bMember;
	bool officer = clientData->m_bOfficer;
	emit_signal("group_status", steam_id, group_id, member, officer);
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
	int playersDontLikeCandidate = playerData->m_cPlayersThatDontLikeCandidate;
	int playersCandidateDoesntLike = playerData->m_cPlayersThatCandidateDoesntLike;
	int clanPlayersDontLikeCandidate = playerData->m_cClanPlayersThatDontLikeCandidate;
	uint64_t steam_id = playerData->m_SteamIDCandidate.ConvertToUint64();
	emit_signal("player_compatibility", result, playersDontLikeCandidate, playersCandidateDoesntLike, clanPlayersDontLikeCandidate, steam_id);
}

// SERVER STATS CALLBACKS ///////////////////////
//
// Result when getting the latests stats and achievements for a user from the server.
void SteamServer::_stat_received(GSStatsReceived_t* callData, bool bioFailure){
	EResult result = callData->m_eResult;
	uint64_t steam_id = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("stat_received", result, steam_id);
}

// Result of a request to store the user stats.
void SteamServer::_stats_stored(GSStatsStored_t* callData){
	EResult result = callData->m_eResult;
	uint64_t steam_id = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("stats_stored", result, steam_id);
}

// Callback indicating that a user's stats have been unloaded.
void SteamServer::_stats_unloaded(GSStatsUnloaded_t* callData){
	uint64_t steam_id = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("stats_unloaded", steam_id);
}

// UGC CALLBACKS ////////////////////////////////
//
// The result of a call to AddAppDependency.
void SteamServer::_add_app_dependency_result(AddAppDependencyResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	AppId_t app_id = callData->m_nAppID;
	emit_signal("add_app_dependency_result", result, (uint64_t)fileID, (uint32_t)app_id);
}

// The result of a call to AddDependency.
void SteamServer::_add_ugc_dependency_result(AddUGCDependencyResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	PublishedFileId_t childID = callData->m_nChildPublishedFileId;
	emit_signal("add_ugc_dependency_result", result, (uint64_t)fileID, (uint64_t)childID);
}

// Result of a workshop item being created.
void SteamServer::_item_created(CreateItemResult_t *callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("item_created", result, (uint64_t)fileID, acceptTOS);
}

// Called when a workshop item has been downloaded.
void SteamServer::_item_downloaded(DownloadItemResult_t* callData){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	AppId_t app_id = callData->m_unAppID;
	emit_signal("item_downloaded", result, (uint64_t)fileID, (uint32_t)app_id);
}

// Called when getting the app dependencies for an item.
void SteamServer::_get_app_dependencies_result(GetAppDependenciesResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
//	AppId_t app_id = callData->m_rgAppIDs;
	uint32 appDependencies = callData->m_nNumAppDependencies;
	uint32 totalAppDependencies = callData->m_nTotalNumAppDependencies;
//	emit_signal("get_app_dependencies_result", result, (uint64_t)fileID, app_id, appDependencies, totalAppDependencies);
	emit_signal("get_app_dependencies_result", result, (uint64_t)fileID, appDependencies, totalAppDependencies);
}

// Called when an attempt at deleting an item completes.
void SteamServer::_item_deleted(DeleteItemResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	emit_signal("item_deleted", result, (uint64_t)fileID);
}

// Called when getting the users vote status on an item.
void SteamServer::_get_item_vote_result(GetUserItemVoteResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool vote_up = callData->m_bVotedUp;
	bool voteDown = callData->m_bVotedDown;
	bool voteSkipped = callData->m_bVoteSkipped;
	emit_signal("get_item_vote_result", result, (uint64_t)fileID, vote_up, voteDown, voteSkipped);
}

// Called when a workshop item has been installed or updated.
void SteamServer::_item_installed(ItemInstalled_t* callData){
	AppId_t app_id = callData->m_unAppID;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	emit_signal("item_installed", app_id, (uint64_t)fileID);
}

// Purpose: The result of a call to RemoveAppDependency.
void SteamServer::_remove_app_dependency_result(RemoveAppDependencyResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	AppId_t app_id = callData->m_nAppID;
	emit_signal("remove_app_dependency_result", result, (uint64_t)fileID, (uint32_t)app_id);
}

// Purpose: The result of a call to RemoveDependency.
void SteamServer::_remove_ugc_dependency_result(RemoveUGCDependencyResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	PublishedFileId_t childID = callData->m_nChildPublishedFileId;
	emit_signal("remove_ugc_dependency_result", result, (uint64_t)fileID, (uint64_t)childID);
}

// Called when the user has voted on an item.
void SteamServer::_set_user_item_vote(SetUserItemVoteResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool vote_up = callData->m_bVoteUp;
	emit_signal("set_user_item_vote", result, (uint64_t)fileID, vote_up);
}

// Called when workshop item playtime tracking has started.
void SteamServer::_start_playtime_tracking(StartPlaytimeTrackingResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	emit_signal("start_playtime_tracking", result);
}

// Called when a UGC query request completes.
void SteamServer::_ugc_query_completed(SteamUGCQueryCompleted_t* callData, bool ioFailure){
	UGCQueryHandle_t handle = callData->m_handle;
	EResult result = callData->m_eResult;
	uint32 resultsReturned = callData->m_unNumResultsReturned;
	uint32 totalMatching = callData->m_unTotalMatchingResults;
	bool cached = callData->m_bCachedData;
	emit_signal("ugc_query_completed", (uint64_t)handle, result, resultsReturned, totalMatching, cached);
}

// Called when workshop item playtime tracking has stopped.
void SteamServer::_stop_playtime_tracking(StopPlaytimeTrackingResult_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	emit_signal("stop_playtime_tracking", result);
}

// Result of a workshop item being updated.
void SteamServer::_item_updated(SubmitItemUpdateResult_t *callData, bool ioFailure){
	EResult result = callData->m_eResult;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("item_updated", result, acceptTOS);
}

// Called when the user has added or removed an item to/from their favorites.
void SteamServer::_user_favorite_items_list_changed(UserFavoriteItemsListChanged_t* callData, bool ioFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool wasAddRequest = callData->m_bWasAddRequest;
	emit_signal("user_favorite_items_list_changed", result, (uint64_t)fileID, wasAddRequest);
}

// Purpose: Status of the user's acceptable/rejection of the app's specific Workshop EULA.
void SteamServer::_workshop_eula_status(WorkshopEULAStatus_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("workshop_eula_status");
	}
	else{
		int result = call_data->m_eResult;
		int app_id = call_data->m_nAppID;
		// Slim down signal arguments since Godot seems to limit them to six max
		Dictionary eula_data;
		eula_data["version"] = call_data->m_unVersion;			// int
		eula_data["action"] = call_data->m_rtAction;			// int
		eula_data["accepted"] = call_data->m_bAccepted;			// bool
		eula_data["needs_action"] = call_data->m_bNeedsAction;	// bool
		emit_signal("workshop_eula_status", result, app_id, eula_data);
	}
}

// Purpose: signal that the list of subscribed items changed.
void SteamServer::_user_subscribed_items_list_changed(UserSubscribedItemsListChanged_t* call_data){
	int app_id = call_data->m_nAppID;
	emit_signal("user_subscribed_items_list_changed", app_id);
}

// UTILITY CALLBACKS ////////////////////////////
//
// CallResult for checkFileSignature.
void SteamServer::_check_file_signature(CheckFileSignature_t *callData, bool bioFailure){
	String signature;
	if(callData->m_eCheckFileSignature == k_ECheckFileSignatureNoSignaturesFoundForThisApp){
		signature = "app not signed";
	}
	else if(callData->m_eCheckFileSignature == k_ECheckFileSignatureNoSignaturesFoundForThisFile){
		signature = "file not signed";
	}
	else if(callData->m_eCheckFileSignature == k_ECheckFileSignatureFileNotFound){
		signature = "file does not exist";		
	}
	else if(callData->m_eCheckFileSignature == k_ECheckFileSignatureInvalidSignature){
		signature = "signature invalid";
	}
	else if(callData->m_eCheckFileSignature == k_ECheckFileSignatureValidSignature){
		signature = "valid";		
	}
	else{
		signature = "invalid response";
	}
	emit_signal("check_file_signature", signature);
}

// Called when the big picture gamepad text input has been closed.
void SteamServer::_gamepad_text_input_dismissed(GamepadTextInputDismissed_t* callData){
	char text = '\0';
	uint32 length = 0;
	if(callData->m_bSubmitted){
		SteamUtils()->GetEnteredGamepadTextInput(&text, callData->m_unSubmittedText);
		length = SteamUtils()->GetEnteredGamepadTextLength();
	}
	emit_signal("gamepad_text_input_dismissed", text, length);
}

// Called when the country of the user changed. The country should be updated with getIPCountry.
void SteamServer::_ip_country(IPCountry_t* callData){
	emit_signal("ip_country");
}

// Called when running on a laptop and less than 10 minutes of battery is left, and then fires then every minute afterwards.
void SteamServer::_low_power(LowBatteryPower_t* timeLeft){
	uint8 power = timeLeft->m_nMinutesBatteryLeft;
	emit_signal("low_power", power);
}

// Called when a SteamAPICall_t has completed (or failed)
void SteamServer::_steam_api_call_completed(SteamAPICallCompleted_t* callData){
	uint64_t asyncCall = callData->m_hAsyncCall;
	int callback = callData->m_iCallback;
	uint32 parameter = callData->m_cubParam;
	emit_signal("steam_api_call_completed", asyncCall, callback, parameter);
}

// Called when Steam wants to shutdown.
void SteamServer::_steam_shutdown(SteamShutdown_t* callData){
	emit_signal("steam_shutdown");
}

// Sent after the device returns from sleep/suspend mode.
void SteamServer::_app_resuming_from_suspend(AppResumingFromSuspend_t *call_data) {
	emit_signal("app_resuming_from_suspend");
}

// Sent after the device returns from sleep/suspend mode.
void SteamServer::_floating_gamepad_text_input_dismissed(FloatingGamepadTextInputDismissed_t *call_data) {
	emit_signal("floating_gamepad_text_input_dismissed");
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
	ClassDB::bind_method("serverInit", &SteamServer::serverInit);
	ClassDB::bind_method("serverReleaseCurrentThreadMemory", &SteamServer::serverReleaseCurrentThreadMemory);
	ClassDB::bind_method("serverShutdown", &SteamServer::serverShutdown);
	ClassDB::bind_method("run_callbacks", &SteamServer::run_callbacks);

	// APPS BIND METHODS ////////////////////////
	ClassDB::bind_method("getDLCDataByIndex", &SteamServer::getDLCDataByIndex);
	ClassDB::bind_method("isAppInstalled", &SteamServer::isAppInstalled);
	ClassDB::bind_method("isCybercafe", &SteamServer::isCybercafe);
	ClassDB::bind_method("isDLCInstalled", &SteamServer::isDLCInstalled);
	ClassDB::bind_method("isLowViolence", &SteamServer::isLowViolence);
	ClassDB::bind_method("isSubscribed", &SteamServer::isSubscribed);
	ClassDB::bind_method("isSubscribedApp", &SteamServer::isSubscribedApp);
	ClassDB::bind_method("isSubscribedFromFamilySharing", &SteamServer::isSubscribedFromFamilySharing);
	ClassDB::bind_method("isSubscribedFromFreeWeekend", &SteamServer::isSubscribedFromFreeWeekend);
	ClassDB::bind_method("isTimedTrial", &SteamServer::isTimedTrial);
	ClassDB::bind_method("isVACBanned", &SteamServer::isVACBanned);
	ClassDB::bind_method("getAppBuildId", &SteamServer::getAppBuildId);
	ClassDB::bind_method("getAppInstallDir", &SteamServer::getAppInstallDir);
	ClassDB::bind_method("getAppOwner", &SteamServer::getAppOwner);
	ClassDB::bind_method("getAvailableGameLanguages", &SteamServer::getAvailableGameLanguages);
	ClassDB::bind_method("getCurrentBetaName", &SteamServer::getCurrentBetaName);
	ClassDB::bind_method("getCurrentGameLanguage", &SteamServer::getCurrentGameLanguage);
	ClassDB::bind_method("getDLCCount", &SteamServer::getDLCCount);
	ClassDB::bind_method("getDLCDownloadProgress", &SteamServer::getDLCDownloadProgress);
	ClassDB::bind_method("getEarliestPurchaseUnixTime", &SteamServer::getEarliestPurchaseUnixTime);
	ClassDB::bind_method("getFileDetails", &SteamServer::getFileDetails);
	ClassDB::bind_method("getInstalledDepots", &SteamServer::getInstalledDepots);
	ClassDB::bind_method("getLaunchQueryParam", &SteamServer::getLaunchQueryParam);
	ClassDB::bind_method("installDLC", &SteamServer::installDLC);
	ClassDB::bind_method("markContentCorrupt", &SteamServer::markContentCorrupt);
	ClassDB::bind_method("uninstallDLC", &SteamServer::uninstallDLC);
	ClassDB::bind_method("requestAllProofOfPurchaseKeys", &SteamServer::requestAllProofOfPurchaseKeys);
	ClassDB::bind_method("requestAppProofOfPurchaseKey", &SteamServer::requestAppProofOfPurchaseKey);

	// HTTP BIND METHODS ////////////////////////
	ClassDB::bind_method("createCookieContainer", &SteamServer::createCookieContainer);
	ClassDB::bind_method("createHTTPRequest", &SteamServer::createHTTPRequest);
	ClassDB::bind_method("deferHTTPRequest", &SteamServer::deferHTTPRequest);
	ClassDB::bind_method("getHTTPDownloadProgressPct", &SteamServer::getHTTPDownloadProgressPct);
	ClassDB::bind_method("getHTTPRequestWasTimedOut", &SteamServer::getHTTPRequestWasTimedOut);
	ClassDB::bind_method("getHTTPResponseBodyData", &SteamServer::getHTTPResponseBodyData);
	ClassDB::bind_method("getHTTPResponseBodySize", &SteamServer::getHTTPResponseBodySize);
	ClassDB::bind_method("getHTTPResponseHeaderSize", &SteamServer::getHTTPResponseHeaderSize);
	ClassDB::bind_method("getHTTPResponseHeaderValue", &SteamServer::getHTTPResponseHeaderValue);
	ClassDB::bind_method("getHTTPStreamingResponseBodyData", &SteamServer::getHTTPStreamingResponseBodyData);
	ClassDB::bind_method("prioritizeHTTPRequest", &SteamServer::prioritizeHTTPRequest);
	ClassDB::bind_method("releaseCookieContainer", &SteamServer::releaseCookieContainer);
	ClassDB::bind_method("releaseHTTPRequest", &SteamServer::releaseHTTPRequest);
	ClassDB::bind_method("sendHTTPRequest", &SteamServer::sendHTTPRequest);
	ClassDB::bind_method("sendHTTPRequestAndStreamResponse", &SteamServer::sendHTTPRequestAndStreamResponse);
	ClassDB::bind_method("setHTTPCookie", &SteamServer::setHTTPCookie);
	ClassDB::bind_method("setHTTPRequestAbsoluteTimeoutMS", &SteamServer::setHTTPRequestAbsoluteTimeoutMS);
	ClassDB::bind_method("setHTTPRequestContextValue", &SteamServer::setHTTPRequestContextValue);
	ClassDB::bind_method("setHTTPRequestCookieContainer", &SteamServer::setHTTPRequestCookieContainer);
	ClassDB::bind_method("setHTTPRequestGetOrPostParameter", &SteamServer::setHTTPRequestGetOrPostParameter);
	ClassDB::bind_method("setHTTPRequestHeaderValue", &SteamServer::setHTTPRequestHeaderValue);
	ClassDB::bind_method("setHTTPRequestNetworkActivityTimeout", &SteamServer::setHTTPRequestNetworkActivityTimeout);
	ClassDB::bind_method("setHTTPRequestRawPostBody", &SteamServer::setHTTPRequestRawPostBody);
	ClassDB::bind_method("setHTTPRequestRequiresVerifiedCertificate", &SteamServer::setHTTPRequestRequiresVerifiedCertificate);
	ClassDB::bind_method("setHTTPRequestUserAgentInfo", &SteamServer::setHTTPRequestUserAgentInfo);

	// INVENTORY BIND METHODS ///////////////////
	ClassDB::bind_method("addPromoItem", &SteamServer::addPromoItem);
	ClassDB::bind_method("addPromoItems", &SteamServer::addPromoItems);
	ClassDB::bind_method("checkResultSteamID", &SteamServer::checkResultSteamID);
	ClassDB::bind_method("consumeItem", &SteamServer::consumeItem);
	ClassDB::bind_method("deserializeResult", &SteamServer::deserializeResult);
	ClassDB::bind_method("destroyResult", &SteamServer::exchangeItems);
	ClassDB::bind_method("exchangeItems", &SteamServer::exchangeItems);
	ClassDB::bind_method("generateItems", &SteamServer::generateItems);
	ClassDB::bind_method("getAllItems", &SteamServer::getAllItems);
	ClassDB::bind_method("getItemDefinitionProperty", &SteamServer::getItemDefinitionProperty);
	ClassDB::bind_method("getItemsByID", &SteamServer::getItemsByID);
	ClassDB::bind_method("getItemPrice", &SteamServer::getItemPrice);
	ClassDB::bind_method("getItemsWithPrices", &SteamServer::getItemsWithPrices);
	ClassDB::bind_method("getNumItemsWithPrices", &SteamServer::getNumItemsWithPrices);
	ClassDB::bind_method("getResultItemProperty", &SteamServer::getResultItems);
	ClassDB::bind_method("getResultItems", &SteamServer::getResultItems);
	ClassDB::bind_method("getResultStatus", &SteamServer::getResultStatus);
	ClassDB::bind_method("getResultTimestamp", &SteamServer::getResultTimestamp);
	ClassDB::bind_method("grantPromoItems", &SteamServer::grantPromoItems);
	ClassDB::bind_method("loadItemDefinitions", &SteamServer::loadItemDefinitions);
	ClassDB::bind_method("requestEligiblePromoItemDefinitionsIDs", &SteamServer::requestEligiblePromoItemDefinitionsIDs);
	ClassDB::bind_method("requestPrices", &SteamServer::requestPrices);
	ClassDB::bind_method("serializeResult", &SteamServer::serializeResult);
	ClassDB::bind_method("startPurchase", &SteamServer::startPurchase);
	ClassDB::bind_method("transferItemQuantity", &SteamServer::transferItemQuantity);
	ClassDB::bind_method("triggerItemDrop", &SteamServer::triggerItemDrop);
	ClassDB::bind_method("startUpdateProperties", &SteamServer::startUpdateProperties);
	ClassDB::bind_method("submitUpdateProperties", &SteamServer::submitUpdateProperties);
	ClassDB::bind_method("removeProperty", &SteamServer::removeProperty);
	ClassDB::bind_method("setPropertyString", &SteamServer::setPropertyString);
	ClassDB::bind_method("setPropertyBool", &SteamServer::setPropertyBool);
	ClassDB::bind_method("setPropertyInt", &SteamServer::setPropertyInt);
	ClassDB::bind_method("setPropertyFloat", &SteamServer::setPropertyFloat);

	// NETWORKING BIND METHODS //////////////////
	ClassDB::bind_method("acceptP2PSessionWithUser", &SteamServer::acceptP2PSessionWithUser);
	ClassDB::bind_method("allowP2PPacketRelay", &SteamServer::allowP2PPacketRelay);
	ClassDB::bind_method("closeP2PChannelWithUser", &SteamServer::closeP2PChannelWithUser);
	ClassDB::bind_method("closeP2PSessionWithUser", &SteamServer::closeP2PSessionWithUser);
	ClassDB::bind_method("getP2PSessionState", &SteamServer::getP2PSessionState);
	ClassDB::bind_method("getAvailableP2PPacketSize", &SteamServer::getAvailableP2PPacketSize);
	ClassDB::bind_method("readP2PPacket", &SteamServer::readP2PPacket);
	ClassDB::bind_method("sendP2PPacket", &SteamServer::sendP2PPacket);

	// SERVER BIND METHODS //////////////////////
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
	ClassDB::bind_method("getAuthSessionTicket", &SteamServer::getAuthSessionTicket);
	ClassDB::bind_method("beginAuthSession", &SteamServer::beginAuthSession);
	ClassDB::bind_method("endAuthSession", &SteamServer::endAuthSession);
	ClassDB::bind_method("cancelAuthTicket", &SteamServer::cancelAuthTicket);
	ClassDB::bind_method("userHasLicenceForApp", &SteamServer::userHasLicenceForApp);
	ClassDB::bind_method("requestUserGroupStatus", &SteamServer::requestUserGroupStatus);
	ClassDB::bind_method("handleIncomingPacket", &SteamServer::handleIncomingPacket);
	ClassDB::bind_method("getNextOutgoingPacket", &SteamServer::getNextOutgoingPacket);
	ClassDB::bind_method("setAdvertiseServerActive", &SteamServer::setAdvertiseServerActive);
	ClassDB::bind_method("associateWithClan", &SteamServer::associateWithClan);
	ClassDB::bind_method("computeNewPlayerCompatibility", &SteamServer::computeNewPlayerCompatibility);

	// SERVER STATS BIND METHODS ////////////////
	ClassDB::bind_method("clearUserAchievement", &SteamServer::clearUserAchievement);
	ClassDB::bind_method("getUserAchievement", &SteamServer::getUserAchievement);
	ClassDB::bind_method("getUserStatInt", &SteamServer::getUserStatInt);
	ClassDB::bind_method("getUserStatFloat", &SteamServer::getUserStatFloat);
	ClassDB::bind_method("requestUserStats", &SteamServer::requestUserStats);
	ClassDB::bind_method("setUserAchievement", &SteamServer::setUserAchievement);
	ClassDB::bind_method("setUserStatInt", &SteamServer::setUserStatInt);
	ClassDB::bind_method("setUserStatFloat", &SteamServer::setUserStatFloat);
	ClassDB::bind_method("storeUserStats", &SteamServer::storeUserStats);
	ClassDB::bind_method("updateUserAvgRateStat", &SteamServer::updateUserAvgRateStat);

	// UGC BIND METHODS ////////////////////
	ClassDB::bind_method("addAppDependency", &SteamServer::addAppDependency);
	ClassDB::bind_method("addDependency", &SteamServer::addDependency);
	ClassDB::bind_method("addExcludedTag", &SteamServer::addExcludedTag);
	ClassDB::bind_method("addItemKeyValueTag", &SteamServer::addItemKeyValueTag);
	ClassDB::bind_method("addItemPreviewFile", &SteamServer::addItemPreviewFile);
	ClassDB::bind_method("addItemPreviewVideo", &SteamServer::addItemPreviewVideo);
	ClassDB::bind_method("addItemToFavorite", &SteamServer::addItemToFavorite);
	ClassDB::bind_method("addRequiredKeyValueTag", &SteamServer::addRequiredKeyValueTag);
	ClassDB::bind_method("addRequiredTag", &SteamServer::addRequiredTag);
	ClassDB::bind_method("addRequiredTagGroup", &SteamServer::addRequiredTagGroup);
	ClassDB::bind_method("initWorkshopForGameServer", &SteamServer::initWorkshopForGameServer);
	ClassDB::bind_method("createItem", &SteamServer::createItem);
	ClassDB::bind_method("createQueryAllUGCRequest", &SteamServer::createQueryAllUGCRequest);
	ClassDB::bind_method("createQueryUGCDetailsRequest", &SteamServer::createQueryUGCDetailsRequest);
	ClassDB::bind_method("createQueryUserUGCRequest", &SteamServer::createQueryUserUGCRequest);
	ClassDB::bind_method("deleteItem", &SteamServer::deleteItem);
	ClassDB::bind_method("downloadItem", &SteamServer::downloadItem);
	ClassDB::bind_method("getItemDownloadInfo", &SteamServer::getItemDownloadInfo);
	ClassDB::bind_method("getItemInstallInfo", &SteamServer::getItemInstallInfo);
	ClassDB::bind_method("getItemState", &SteamServer::getItemState);
	ClassDB::bind_method("getItemUpdateProgress", &SteamServer::getItemUpdateProgress);
	ClassDB::bind_method("getNumSubscribedItems", &SteamServer::getNumSubscribedItems);
	ClassDB::bind_method("getQueryUGCAdditionalPreview", &SteamServer::getQueryUGCAdditionalPreview);
	ClassDB::bind_method("getQueryUGCChildren", &SteamServer::getQueryUGCChildren);
	ClassDB::bind_method("getQueryUGCKeyValueTag", &SteamServer::getQueryUGCKeyValueTag);
	ClassDB::bind_method("getQueryUGCNumTags", &SteamServer::getQueryUGCNumTags);
	ClassDB::bind_method("getQueryUGCMetadata", &SteamServer::getQueryUGCMetadata);
	ClassDB::bind_method("getQueryUGCNumAdditionalPreviews", &SteamServer::getQueryUGCNumAdditionalPreviews);
	ClassDB::bind_method("getQueryUGCNumKeyValueTags", &SteamServer::getQueryUGCNumKeyValueTags);
	ClassDB::bind_method("getQueryUGCPreviewURL", &SteamServer::getQueryUGCPreviewURL);
	ClassDB::bind_method("getQueryUGCResult", &SteamServer::getQueryUGCResult);
	ClassDB::bind_method("getQueryUGCStatistic", &SteamServer::getQueryUGCStatistic);
	ClassDB::bind_method("getQueryUGCTag", &SteamServer::getQueryUGCTag);
	ClassDB::bind_method("getQueryUGCTagDisplayName", &SteamServer::getQueryUGCTagDisplayName);
	ClassDB::bind_method("getSubscribedItems", &SteamServer::getSubscribedItems);
	ClassDB::bind_method("getUserItemVote", &SteamServer::getUserItemVote);
	ClassDB::bind_method("releaseQueryUGCRequest", &SteamServer::releaseQueryUGCRequest);
	ClassDB::bind_method("removeAppDependency", &SteamServer::removeAppDependency);
	ClassDB::bind_method("removeDependency", &SteamServer::removeDependency);
	ClassDB::bind_method("removeItemFromFavorites", &SteamServer::removeItemFromFavorites);
	ClassDB::bind_method("removeItemKeyValueTags", &SteamServer::removeItemKeyValueTags);
	ClassDB::bind_method("removeItemPreview", &SteamServer::removeItemPreview);
	ClassDB::bind_method("sendQueryUGCRequest", &SteamServer::sendQueryUGCRequest);
	ClassDB::bind_method("setAllowCachedResponse", &SteamServer::setAllowCachedResponse);
	ClassDB::bind_method("setCloudFileNameFilter", &SteamServer::setCloudFileNameFilter);
	ClassDB::bind_method("setItemContent", &SteamServer::setItemContent);
	ClassDB::bind_method("setItemDescription", &SteamServer::setItemDescription);
	ClassDB::bind_method("setItemMetadata", &SteamServer::setItemMetadata);
	ClassDB::bind_method("setItemPreview", &SteamServer::setItemPreview);
	ClassDB::bind_method("setItemTags", &SteamServer::setItemTags);
	ClassDB::bind_method("setItemTitle", &SteamServer::setItemTitle);
	ClassDB::bind_method("setItemUpdateLanguage", &SteamServer::setItemUpdateLanguage);
	ClassDB::bind_method("setItemVisibility", &SteamServer::setItemVisibility);
	ClassDB::bind_method("setLanguage", &SteamServer::setLanguage);
	ClassDB::bind_method("setMatchAnyTag", &SteamServer::setMatchAnyTag);
	ClassDB::bind_method("setRankedByTrendDays", &SteamServer::setRankedByTrendDays);
	ClassDB::bind_method("setReturnAdditionalPreviews", &SteamServer::setReturnAdditionalPreviews);
	ClassDB::bind_method("setReturnChildren", &SteamServer::setReturnChildren);
	ClassDB::bind_method("setReturnKeyValueTags", &SteamServer::setReturnKeyValueTags);
	ClassDB::bind_method("setReturnLongDescription", &SteamServer::setReturnLongDescription);
	ClassDB::bind_method("setReturnMetadata", &SteamServer::setReturnMetadata);
	ClassDB::bind_method("setReturnOnlyIDs", &SteamServer::setReturnOnlyIDs);
	ClassDB::bind_method("setReturnPlaytimeStats", &SteamServer::setReturnPlaytimeStats);
	ClassDB::bind_method("setReturnTotalOnly", &SteamServer::setReturnTotalOnly);
	ClassDB::bind_method("setSearchText", &SteamServer::setSearchText);
	ClassDB::bind_method("setUserItemVote", &SteamServer::setUserItemVote);
	ClassDB::bind_method("startItemUpdate", &SteamServer::startItemUpdate);
	ClassDB::bind_method("startPlaytimeTracking", &SteamServer::startPlaytimeTracking);
	ClassDB::bind_method("stopPlaytimeTracking", &SteamServer::stopPlaytimeTracking);
	ClassDB::bind_method("stopPlaytimeTrackingForAllItems", &SteamServer::stopPlaytimeTrackingForAllItems);
	ClassDB::bind_method("getAppDependencies", &SteamServer::getAppDependencies);
	ClassDB::bind_method("submitItemUpdate", &SteamServer::submitItemUpdate);
	ClassDB::bind_method("subscribeItem", &SteamServer::subscribeItem);
	ClassDB::bind_method("suspendDownloads", &SteamServer::suspendDownloads);
	ClassDB::bind_method("unsubscribeItem", &SteamServer::unsubscribeItem);
	ClassDB::bind_method("updateItemPreviewFile", &SteamServer::updateItemPreviewFile);
	ClassDB::bind_method("updateItemPreviewVideo", &SteamServer::updateItemPreviewVideo);
	ClassDB::bind_method("showWorkshopEULA", &SteamServer::showWorkshopEULA);
	ClassDB::bind_method("getWorkshopEULAStatus", &SteamServer::getWorkshopEULAStatus);

	// UTILS BIND METHODS ///////////////////////
	ClassDB::bind_method("filterText", &SteamServer::filterText);
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
	ClassDB::bind_method("setOverlayNotificationInset", &SteamServer::setOverlayNotificationInset);
	ClassDB::bind_method(D_METHOD("setOverlayNotificationPosition", "pos"), &SteamServer::setOverlayNotificationPosition);
	ClassDB::bind_method("setVRHeadsetStreamingEnabled", &SteamServer::setVRHeadsetStreamingEnabled);
	ClassDB::bind_method("showGamepadTextInput", &SteamServer::showGamepadTextInput);
	ClassDB::bind_method("startVRDashboard", &SteamServer::startVRDashboard);

	/////////////////////////////////////////////
	// CALLBACK SIGNAL BINDS ////////////////////
	/////////////////////////////////////////////
	//
	// STEAMWORKS SIGNALS ///////////////////////
	ADD_SIGNAL(MethodInfo("steamworks_error"));

	// APPS SIGNALS /////////////////////////////
	ADD_SIGNAL(MethodInfo("file_details_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "fileSize"), PropertyInfo(Variant::INT, "fileHash"), PropertyInfo(Variant::INT, "flags")));
	ADD_SIGNAL(MethodInfo("dlc_installed", PropertyInfo(Variant::INT, "app")));
	ADD_SIGNAL(MethodInfo("new_launch_url_parameters"));
//	ADD_SIGNAL(MethodInfo("new_launch_query_parameters"));
	ADD_SIGNAL(MethodInfo("register_activation_code_response"));
	ADD_SIGNAL(MethodInfo("app_proof_of_purchase_key_response"));
	ADD_SIGNAL(MethodInfo("timed_trial_status"));

	// HTTP SIGNALS /////////////////////////////
	ADD_SIGNAL(MethodInfo("http_request_completed"));
	ADD_SIGNAL(MethodInfo("http_request_data_received"));
	ADD_SIGNAL(MethodInfo("http_request_headers_received"));

	// INVENTORY SIGNALS ////////////////////////
	ADD_SIGNAL(MethodInfo("inventory_definition_update"));
	ADD_SIGNAL(MethodInfo("inventory_eligible_promo_item"));
	ADD_SIGNAL(MethodInfo("inventory_full_update"));
	ADD_SIGNAL(MethodInfo("inventory_result_ready"));
	ADD_SIGNAL(MethodInfo("inventory_start_purchase_result"));
	ADD_SIGNAL(MethodInfo("inventory_request_prices_result"));

	// NETWORKING SIGNALS ///////////////////////
	ADD_SIGNAL(MethodInfo("p2p_session_request"));
	ADD_SIGNAL(MethodInfo("p2p_session_connect_fail"));

	// REMOTE STORAGE SIGNALS ///////////////////
	ADD_SIGNAL(MethodInfo("unsubscribe_item"));
	ADD_SIGNAL(MethodInfo("subscribe_item"));

	// SERVER SIGNALS ///////////////////////////
	ADD_SIGNAL(MethodInfo("server_failure", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::BOOL, "retrying")));
	ADD_SIGNAL(MethodInfo("server_connected"));
	ADD_SIGNAL(MethodInfo("server_disconnected", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("client_approved", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "ownerID")));
	ADD_SIGNAL(MethodInfo("client_denied", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("client_kick", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("server_secure", PropertyInfo(Variant::INT, "secure")));
	ADD_SIGNAL(MethodInfo("gameplay_stats", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "rank"), PropertyInfo(Variant::INT, "totalConnects"), PropertyInfo(Variant::INT, "totalMinutesPlayed")));
	ADD_SIGNAL(MethodInfo("group_status", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "group_id"), PropertyInfo(Variant::BOOL, "member"), PropertyInfo(Variant::BOOL, "officer")));
	ADD_SIGNAL(MethodInfo("clan_associate", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("player_compatibility", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "playersDontLikeCandidate"), PropertyInfo(Variant::INT, "playersCandidateDoesntLike"), PropertyInfo(Variant::INT, "clanPlayersDontLikeCandidate"), PropertyInfo(Variant::INT, "steam_id")));

	// SERVER STATS SIGNALS /////////////////////
	ADD_SIGNAL(MethodInfo("stat_received"));
	ADD_SIGNAL(MethodInfo("stats_stored"));
	ADD_SIGNAL(MethodInfo("stats_unloaded"));

	// UGC SIGNALS //////////////////////////////
	ADD_SIGNAL(MethodInfo("add_app_dependency_result"));
	ADD_SIGNAL(MethodInfo("add_ugc_dependency_result"));
	ADD_SIGNAL(MethodInfo("item_created"));
	ADD_SIGNAL(MethodInfo("item_downloaded"));
	ADD_SIGNAL(MethodInfo("get_app_dependencies_result"));
	ADD_SIGNAL(MethodInfo("item_deleted"));
	ADD_SIGNAL(MethodInfo("get_item_vote_result"));
	ADD_SIGNAL(MethodInfo("item_installed"));
	ADD_SIGNAL(MethodInfo("remove_app_dependency_result"));
	ADD_SIGNAL(MethodInfo("remove_ugc_dependency_result"));
	ADD_SIGNAL(MethodInfo("set_user_item_vote"));
	ADD_SIGNAL(MethodInfo("start_playtime_tracking"));
	ADD_SIGNAL(MethodInfo("ugc_query_completed"));
	ADD_SIGNAL(MethodInfo("stop_playtime_tracking"));
	ADD_SIGNAL(MethodInfo("item_updated"));
	ADD_SIGNAL(MethodInfo("user_favorite_items_list_changed"));
	ADD_SIGNAL(MethodInfo("workshop_eula_status"));
	ADD_SIGNAL(MethodInfo("user_subscribed_items_list_changed"));

	// UTILITY SIGNALS //////////////////////////
	ADD_SIGNAL(MethodInfo("check_file_signature"));
	ADD_SIGNAL(MethodInfo("gamepad_text_input_dismissed"));
	ADD_SIGNAL(MethodInfo("ip_country"));
	ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));
	ADD_SIGNAL(MethodInfo("steam_api_call_completed"));
	ADD_SIGNAL(MethodInfo("steam_shutdown"));
	ADD_SIGNAL(MethodInfo("app_resuming_from_suspend"));
	ADD_SIGNAL(MethodInfo("floating_gamepad_text_input_dismissed"));

	/////////////////////////////////////////////
	// CONSTANT BINDS ///////////////////////////
	/////////////////////////////////////////////
	//
	// HTTP CONSTANTS ///////////////////////////
	BIND_CONSTANT(INVALID_HTTPCOOKIE_HANDLE);											// 0
	BIND_CONSTANT(INVALID_HTTPREQUEST_HANDLE);											// 0

	// INVENTORY CONSTANTS //////////////////////
	BIND_CONSTANT(INVENTORY_RESULT_INVALID);											// -1
	BIND_CONSTANT(ITEM_INSTANCE_ID_INVALID);											// 0

	// UGC CONSTANTS ////////////////////////////
	BIND_CONSTANT(NUM_UGC_RESULTS_PER_PAGE); 											// 50
	BIND_CONSTANT(DEVELOPER_METADATA_MAX); 												// 5000
	BIND_CONSTANT(UGC_QUERY_HANDLE_INVALID); 											// 0xffffffffffffffffull
	BIND_CONSTANT(UGC_UPDATE_HANDLE_INVALID); 											// 0xffffffffffffffffull

	/////////////////////////////////////////////
	// ENUM CONSTANT BINDS //////////////////////
	/////////////////////////////////////////////
	//
	// ACCOUNTTYPE ENUM CONSTANTS ///////////////
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

	// APPOWNERSHIPFLAGS ENUM CONSTANTS /////////
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

	// APPRELEASESTATE ENUM CONSTANTS ///////////
	BIND_CONSTANT(APP_RELEASE_STATE_UNKNOWN);											// 0
	BIND_CONSTANT(APP_RELEASE_STATE_UNAVAILABLE);										// 1
	BIND_CONSTANT(APP_RELEASE_STATE_PRERELEASE);										// 2
	BIND_CONSTANT(APP_RELEASE_STATE_PRELOAD_ONLY);										// 3
	BIND_CONSTANT(APP_RELEASE_STATE_RELEASED);											// 4

	// APPTYPE ENUM CONSTANTS ///////////////////
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

	// AUTHSESSIONRESPONSE ENUM CONSTANTS ///////
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

	// BEGINAUTHSESSIONRESULT ENUM CONSTANTS ////
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_OK);										// 0
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET);							// 1
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST);							// 2
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION);							// 3
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH);								// 4
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET);							// 5

	// BROADCASTUPLOADRESULT ENUM CONSTANTS /////
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

	// CHATENTRYTYPE ENUM CONSTANTS /////////////
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

	// CHATROOMENTERRESPONSE ENUM CONSTANTS /////
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

	// CHATSTEAMIDINSTANCEFLAGS ENUM CONSTANTS //
	BIND_CONSTANT(CHAT_ACCOUNT_INSTANCE_MASK);											// 0X00000FFF
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_CLAN);												// ((k_unSteamAccountInstanceMask + 1) >> 1)
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_LOBBY);											// ((k_unSteamAccountInstanceMask + 1) >> 2)
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_MMS_LOBBY);										// ((k_unSteamAccountInstanceMask + 1) >> 3)

	// DENYREASON ENUM CONSTANTS ////////////////
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

	// GAMEIDTYPE ENUM CONSTANTS ////////////////
	BIND_CONSTANT(GAME_TYPE_APP);														// 0
	BIND_CONSTANT(GAME_TYPE_GAME_MOD);													// 1
	BIND_CONSTANT(GAME_TYPE_SHORTCUT);													// 2
	BIND_CONSTANT(GAME_TYPE_P2P);														// 3

	// LAUNCHOPTIONTYPE ENUM CONSTANTS //////////////
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

	// MARKETINGMESSAGEFLAGS ENUM CONSTANTS /////////
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_NONE);										// 0
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_HIGH_PRIORITY);								// (1<<0)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_WINDOWS);							// (1<<1)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_MAC);								// (1<<2)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_LINUX);								// (1<<3)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_RESTRICTIONS);

	// NOTIFICATIONPOSITION ENUM CONSTANTS //////////
	BIND_CONSTANT(POSITION_TOP_LEFT);													// 0
	BIND_CONSTANT(POSITION_TOP_RIGHT);													// 1
	BIND_CONSTANT(POSITION_BOTTOM_LEFT);												// 2
	BIND_CONSTANT(POSITION_BOTTOM_RIGHT);												// 3

	// RESULT ENUM CONSTANTS ////////////////////
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

	// STEAMUSERSTATTYPE ENUM CONSTANTS /////////
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_INVALID);										// 0
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_INT);											// 1
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_FLOAT);											// 2
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_AVGRATE);										// 3
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_ACHIEVEMENTS);									// 4
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_GROUPACHIEVEMENTS);								// 5
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_MAX);											// 6

	// UNIVERSE ENUM CONSTANTS //////////////////
	BIND_CONSTANT(UNIVERSE_INVALID);													// 0
	BIND_CONSTANT(UNIVERSE_PUBLIC);														// 1
	BIND_CONSTANT(UNIVERSE_BETA);														// 2
	BIND_CONSTANT(UNIVERSE_INTERNAL);													// 3
	BIND_CONSTANT(UNIVERSE_DEV);														// 4
	BIND_CONSTANT(UNIVERSE_MAX);														// 5

	// USERHASLICENSEFORAPPRESULT ENUM CONSTANTS
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_HAS_LICENSE);									// 0
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_DOES_NOT_HAVE_LICENSE);						// 1
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_NO_AUTH);										// 2

	// VOICERESULT ENUM CONSTANTS ///////////////
	BIND_CONSTANT(VOICE_RESULT_OK);														// 0
	BIND_CONSTANT(VOICE_RESULT_NOT_INITIALIZED);										// 1
	BIND_CONSTANT(VOICE_RESULT_NOT_RECORDING);											// 2
	BIND_CONSTANT(VOICE_RESULT_NO_DATE);												// 3
	BIND_CONSTANT(VOICE_RESULT_BUFFER_TOO_SMALL);										// 4
	BIND_CONSTANT(VOICE_RESULT_DATA_CORRUPTED);											// 5
	BIND_CONSTANT(VOICE_RESULT_RESTRICTED);												// 6

	// VRHMDTYPE ENUM CONSTANTS /////////////////
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

	// HTTPMETHOD ENUM CONSTANTS ////////////////
	BIND_CONSTANT(HTTP_METHOD_INVALID);													// 0
	BIND_CONSTANT(HTTP_METHOD_GET);														// 1
	BIND_CONSTANT(HTTP_METHOD_HEAD);													// 2
	BIND_CONSTANT(HTTP_METHOD_POST);													// 3
	BIND_CONSTANT(HTTP_METHOD_PUT);														// 4
	BIND_CONSTANT(HTTP_METHOD_DELETE);													// 5
	BIND_CONSTANT(HTTP_METHOD_OPTIONS);													// 6
	BIND_CONSTANT(HTTP_METHOD_PATCH);													// 7

	// HTTPSTATUSCODE ENUM CONSTANTS ////////////
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

	// STEAMITEMFLAGS ENUM CONSTANTS ////////////
	BIND_CONSTANT(STEAM_ITEM_NO_TRADE);													// (1<<0)
	BIND_CONSTANT(STEAM_ITEM_REMOVED);													// (1<<8)
	BIND_CONSTANT(STEAM_ITEM_CONSUMED);													// (1<<9)

	// P2PSEND ENUM CONSTANTS ///////////////////
	BIND_CONSTANT(P2P_SEND_UNRELIABLE);													// 0
	BIND_CONSTANT(P2P_SEND_UNRELIABLE_NO_DELAY);										// 1
	BIND_CONSTANT(P2P_SEND_RELIABLE);													// 2
	BIND_CONSTANT(P2P_SEND_RELIABLE_WITH_BUFFERING);									// 3

	// P2PSESSIONERROR ENUM CONSTANTS ///////////
	BIND_CONSTANT(P2P_SESSION_ERROR_NONE);												// 0
	BIND_CONSTANT(P2P_SESSION_ERROR_NOT_RUNNING_APP);									// 1
	BIND_CONSTANT(P2P_SESSION_ERROR_NO_RIGHTS_TO_APP);									// 2
	BIND_CONSTANT(P2P_SESSION_ERROR_DESTINATION_NOT_LOGGED_ON);							// 3
	BIND_CONSTANT(P2P_SESSION_ERROR_TIMEOUT);											// 4
	BIND_CONSTANT(P2P_SESSION_ERROR_MAX);												// 5

	// SNETSOCKETCONNECTIONTYPE ENUM CONSTANTS //
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_NOT_CONNECTED);							// 0
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_UDP);										// 1
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_UDP_RELAY);								// 2

	// SNETSOCKETSTATE ENUM CONSTANTS ///////////
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

	// ITEMPREVIEWTYPE ENUM CONSTANTS ///////////
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_IMAGE);												// 0
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_YOUTUBE_VIDEO);										// 1
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_SKETCHFAB);											// 2
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_HORIZONTAL_CROSS);					// 3
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_LAT_LONG);							// 4
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_RESERVED_MAX);										// 255

	// ITEMSTATE ENUM CONSTANTS /////////////////
	BIND_CONSTANT(ITEM_STATE_NONE);														// 0
	BIND_CONSTANT(ITEM_STATE_SUBSCRIBED);												// 1
	BIND_CONSTANT(ITEM_STATE_LEGACY_ITEM);												// 2
	BIND_CONSTANT(ITEM_STATE_INSTALLED);												// 4
	BIND_CONSTANT(ITEM_STATE_NEEDS_UPDATE);												// 8
	BIND_CONSTANT(ITEM_STATE_DOWNLOADING);												// 16
	BIND_CONSTANT(ITEM_STATE_DOWNLOAD_PENDING);											// 32

	// ITEMSTATISTIC ENUM CONSTANTS /////////////
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

	// ITEMUPDATESTATUS ENUM CONSTANTS //////////
	BIND_CONSTANT(ITEM_UPDATE_STATUS_INVALID);											// 0
	BIND_CONSTANT(ITEM_UPDATE_STATUS_PREPARING_CONFIG);									// 1
	BIND_CONSTANT(ITEM_UPDATE_STATUS_PREPARING_CONTENT);								// 2
	BIND_CONSTANT(ITEM_UPDATE_STATUS_UPLOADING_CONTENT);								// 3
	BIND_CONSTANT(ITEM_UPDATE_STATUS_UPLOADING_PREVIEW_FILE);							// 4
	BIND_CONSTANT(ITEM_UPDATE_STATUS_COMMITTING_CHANGES);								// 5

	// UGCMATCHINGUGCTYPE ENUM CONSTANTS ////////
	BIND_CONSTANT(UGCMATCHINGUGCTYPE_ITEMS);											// 0
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

	// UGCQUERY ENUM CONSTANTS //////////////////
	BIND_CONSTANT(UGCQUERY_RANKED_BY_VOTE);												// 0
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
	BIND_CONSTANT(UGCQUERY_RANKED_BY_LIFETIME_PLAYTIME_SESSIONS);						// 18

	// USERUGCLIST ENUM CONSTANTS ///////////////
	BIND_CONSTANT(USER_UGC_LIST_PUBLISHED);												// 0
	BIND_CONSTANT(USER_UGC_LIST_VOTED_ON);												// 1
	BIND_CONSTANT(USER_UGC_LIST_VOTED_UP);												// 2
	BIND_CONSTANT(USER_UGC_LIST_VOTED_DOWN);											// 3
	BIND_CONSTANT(USER_UGC_LIST_FAVORITED);												// 5
	BIND_CONSTANT(USER_UGC_LIST_SUBSCRIBED);											// 6
	BIND_CONSTANT(USER_UGC_LIST_USED_OR_PLAYED);										// 7
	BIND_CONSTANT(USER_UGC_LIST_FOLLOWED);												// 8

	// USERUGCLISTSORTORDER ENUM CONSTANTS //////
	BIND_CONSTANT(USERUGCLISTSORTORDER_CREATIONORDERDESC);								// 0
	BIND_CONSTANT(USERUGCLISTSORTORDER_CREATIONORDERASC);								// 1
	BIND_CONSTANT(USERUGCLISTSORTORDER_TITLEASC);										// 2
	BIND_CONSTANT(USERUGCLISTSORTORDER_LASTUPDATEDDESC);								// 3
	BIND_CONSTANT(USERUGCLISTSORTORDER_SUBSCRIPTIONDATEDESC);							// 4
	BIND_CONSTANT(USERUGCLISTSORTORDER_VOTESCOREDESC);									// 5
	BIND_CONSTANT(USERUGCLISTSORTORDER_FORMODERATION);									// 6

	// CHECKFILESIGNATURE ENUM CONSTANTS ////////
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_INVALID_SIGNATURE);								// 0
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_VALID_SIGNATURE);								// 1
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_FILE_NOT_FOUND);									// 2
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_APP);				// 3
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_FILE);				// 4

	// GAMEPADTEXTINPUTLINEMODE ENUM CONSTANTS //
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_LINE_MODE_SINGLE_LINE);							// 0
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_LINE_MODE_MULTIPLE_LINES);							// 1

	// GAMEPADTEXTINPUTMODE ENUM CONSTANTS //////
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_MODE_NORMAL);										// 0
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_MODE_PASSWORD);									// 1

	// STEAMAPICALLFAILURE ENUM CONSTANTS ///////
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_NONE);											// -1
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_STEAM_GONE);									// 0
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_NETWORK_FAILURE);								// 1
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_INVALID_HANDLE);								// 2
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_MISMATCHED_CALLBACK);							// 3
}

SteamServer::~SteamServer(){
	if(is_init_success){
		SteamGameServer_Shutdown();
	}

	// Clear app ID, and singleton variables ////
	singleton = NULL;
	current_app_id = 0;
}