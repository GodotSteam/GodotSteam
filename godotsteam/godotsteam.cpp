// Turn off MSVC-only warning about strcpy
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#pragma warning(disable:4828)
#endif
// Include GodotSteam headear
#include "godotsteam.h"
// Include Steamworks API header
#include "steam/steam_api.h"
// Include some Godot headers
#include "core/io/ip_address.h"
#include "core/io/ip.h"
// Include some system headers
#include "fstream"
#include "vector"

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
#define PORT_QUERY_ERROR 0xFFFE
#define PORT_QUERY_NOT_INITIALIZED 0xFFFF
// Define Friends constants
#define CHAT_METADATA_MAX 8192
#define ENUMERATED_FOLLOWERS_MAX 50
#define FRIENDS_GROUP_LIMIT 100
#define INVALID_FRIEND_GROUP_ID -1
#define MAX_FRIENDS_GROUP_NAME 64
#define MAX_RICH_PRESENCE_KEY_LENGTH 64
#define MAX_RICH_PRESENCE_KEYS 20
#define MAX_RICH_PRESENCE_VALUE_LENTH 256
#define PERSONA_NAME_MAX_UTF8 128
#define PERSONA_NAME_MAX_UTF16 32
// Define HTML Surface constants
#define INVALID_HTMLBROWSER 0
// Define HTTP constants
#define HTTPCOOKIE_INVALID_HANDLE 0
#define HTTPREQUEST_INVALID_HANDLE 0
// Define Input constants
#define INPUT_MAX_ANALOG_ACTIONS 16
#define INPUT_MAX_ANALOG_ACTION_DATA 1.0f
#define INPUT_MAX_COUNT 16
#define INPUT_MAX_DIGITAL_ACTIONS 128
#define INPUT_MAX_ORIGINS 8
#define INPUT_MIN_ANALOG_ACTION_DATA -1.0f
// Define Inventory constants
#define INVENTORY_RESULT_INVALID -1
#define ITEM_INSTANCE_ID_INVALID 0
// Define Matchmaking constants
#define SERVER_QUERY_INVALID 0xffffffff
#define MAX_LOBBY_KEY_LENGTH 255
#define FAVORITE_FLAG_FAVORITE 0x01
#define FAVORITE_FLAG_HISTORY 0x02
#define FAVORITE_FLAG_NONE 0x00
// Define Matchmaking Servers constants
#define MAX_GAME_SERVER_GAME_DATA 2048
#define MAX_GAME_SERVER_GAME_DESCRIPTION 64
#define MAX_GAME_SERVER_GAME_DIR 32
#define MAX_GAME_SERVER_MAP_NAME 32
#define MAX_GAME_SERVER_NAME 64
#define MAX_GAME_SERVER_TAGS 128
// Define Music Remote constants
#define MUSIC_NAME_MAX_LENGTH 255
#define MUSIC_PNG_MAX_LENGTH 65535
// Define Remote Play constants
#define DEVICE_FORM_FACTOR_UNKNOWN 0
#define DEVICE_FORM_FACTOR_PHONE 1
#define DEVICE_FORM_FACTOR_TABLET 2
#define DEVICE_FORM_FACTOR_COMPUTER 3
#define DEVICE_FORM_FACTOR_TV 4
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
// Define Screenshot constants
#define SCREENSHOT_INVALID_HANDLE 0
#define UFS_TAG_TYPE_MAX 255
#define UFS_TAG_VALUE_MAX 255
#define MAX_TAGGED_PUBLISHED_FILES 32
#define MAX_TAGGED_USERS 32
#define SCREENSHOT_THUMB_WIDTH 200
// Define UGC constants
#define NUM_UGC_RESULTS_PER_PAGE 50
#define DEVELOPER_METADATA_MAX 5000
#define UGC_QUERY_HANDLE_INVALID 0
#define UGC_UPDATE_HANDLE_INVALID 0
// Define User Stats constants
#define LEADERBOARD_DETAIL_MAX 64
#define LEADERBOARD_NAME_MAX 128
#define STAT_NAME_MAX 128

Steam* Steam::singleton = NULL;

Steam::Steam()
	:
	// App callbacks ////////////////////////////
	callbackDLCInstalled(this, &Steam::_dlc_installed),
	callbackFileDetailsResult(this, &Steam::_file_details_result),
	callbackNewLaunchURLParameters(this, &Steam::_new_launch_url_parameters),
//	callbackNewLaunchQueryParameters(this, &Steam::_new_launch_query_parameters),  Seems not to be found?

	// Friends callbacks ////////////////////////
	callbackAvatarLoaded(this, &Steam::_avatar_loaded),
	callbackClanActivityDownloaded(this, &Steam::_clan_activity_downloaded),
	callbackFriendRichPresenceUpdate(this, &Steam::_friend_rich_presence_update),
	callbackConnectedChatJoin(this, &Steam::_connected_chat_join),
	callbackConnectedChatLeave(this, &Steam::_connected_chat_leave),
	callbackConnectedClanChatMessage(this, &Steam::_connected_clan_chat_message),
	callbackConnectedFriendChatMessage(this, &Steam::_connected_friend_chat_message),
	callbackJoinRequested(this, &Steam::_join_requested),
	callbackOverlayToggled(this, &Steam::_overlay_toggled),
	callbackJoinGameRequested(this, &Steam::_join_game_requested),
	callbackChangeServerRequested(this, &Steam::_change_server_requested),
	callbackJoinClanChatComplete(this, &Steam::_join_clan_chat_complete),
	callbackPersonaStateChange(this, &Steam::_persona_state_change),
	callbackNameChanged(this, &Steam::_name_changed),
	callbackOverlayBrowserProtocol(this, &Steam::_overlay_browser_protocol),

	// HTML Surface callbacks ///////////////////
	callbackHTMLBrowserReady(this, &Steam::_html_browser_ready),
	callbackHTMLCanGoBackandforward(this, &Steam::_html_can_go_backandforward),
	callbackHTMLChangedTitle(this, &Steam::_html_changed_title),
	callbackHTMLCloseBrowser(this, &Steam::_html_close_browser),
	callbackHTMLFileOpenDialog(this, &Steam::_html_file_open_dialog),
	callbackHTMLFinishedRequest(this, &Steam::_html_finished_request),
	callbackHTMLHideTooltip(this, &Steam::_html_hide_tooltip),
	callbackHTMLHorizontalScroll(this, &Steam::_html_horizontal_scroll),
	callbackHTMLJSAlert(this, &Steam::_html_js_alert),
	callbackHTMLJSConfirm(this, &Steam::_html_js_confirm),
	callbackHTMLLinkAtPosition(this, &Steam::_html_link_at_position),
	callbackHTMLNeedsPaint(this, &Steam::_html_needs_paint),
	callbackHTMLNewWindow(this, &Steam::_html_new_window),
	callbackHTMLOpenLinkInNewTab(this, &Steam::_html_open_link_in_new_tab),
	callbackHTMLSearchResults(this, &Steam::_html_search_results),
	callbackHTMLSetCursor(this, &Steam::_html_set_cursor),
	callbackHTMLShowTooltip(this, &Steam::_html_show_tooltip),
	callbackHTMLStartRequest(this, &Steam::_html_start_request),
	callbackHTMLStatusText(this, &Steam::_html_status_text),
	callbackHTMLUpdateTooltip(this, &Steam::_html_update_tooltip),
	callbackHTMLURLChanged(this, &Steam::_html_url_changed),
	callbackHTMLVerticalScroll(this, &Steam::_html_vertical_scroll),

	// HTTP callbacks ///////////////////////////
	callbackHTTPRequestCompleted(this, &Steam::_http_request_completed),
	callbackHTTPRequestDataReceived(this, &Steam::_http_request_data_received),
	callbackHTTPRequestHeadersReceived(this, &Steam::_http_request_headers_received),

	// Inventory callbacks //////////////////////
	callbackInventoryDefinitionUpdate(this, &Steam::_inventory_definition_update),
	callbackInventoryFullUpdate(this, &Steam::_inventory_full_update),
	callbackInventoryResultReady(this, &Steam::_inventory_result_ready),

	// Matchmaking callbacks ////////////////////
	callbackFavoritesListAccountsUpdated(this, &Steam::_favorites_list_accounts_updated),
	callbackFavoritesListChanged(this, &Steam::_favorites_list_changed),
	callbackLobbyMessage(this, &Steam::_lobby_message),
	callbackLobbyChatUpdate(this, &Steam::_lobby_chat_update),
	callbackLobbyDataUpdate(this, &Steam::_lobby_data_update),
	callbackLobbyJoined(this, &Steam::_lobby_joined),
	callbackLobbyGameCreated(this, &Steam::_lobby_game_created),
	callbackLobbyInvite(this, &Steam::_lobby_invite),

	// Music Remote callbacks ///////////////////
	callbackMusicPlayerRemoteToFront(this, &Steam::_music_player_remote_to_front),
	callbackMusicPlayerRemoteWillActivate(this, &Steam::_music_player_remote_will_activate),
	callbackMusicPlayerRemoteWillDeactivate(this, &Steam::_music_player_remote_will_deactivate),
	callbackMusicPlayerSelectsPlaylistEntry(this, &Steam::_music_player_selects_playlist_entry),
	callbackMusicPlayerSelectsQueueEntry(this, &Steam::_music_player_selects_queue_entry),
	callbackMusicPlayerWantsLooped(this, &Steam::_music_player_wants_looped),
	callbackMusicPlayerWantsPause(this, &Steam::_music_player_wants_pause),
	callbackMusicPlayerWantsPlayingRepeatStatus(this, &Steam::_music_player_wants_playing_repeat_status),
	callbackMusicPlayerWantsPlayNext(this, &Steam::_music_player_wants_play_next),
	callbackMusicPlayerWantsPlayPrevious(this, &Steam::_music_player_wants_play_previous),
	callbackMusicPlayerWantsPlay(this, &Steam::_music_player_wants_play),
	callbackMusicPlayerWantsShuffled(this, &Steam::_music_player_wants_shuffled),
	callbackMusicPlayerWantsVolume(this, &Steam::_music_player_wants_volume),
	callbackMusicPlayerWillQuit(this, &Steam::_music_player_will_quit),

	// Networking callbacks /////////////////////
	callbackP2PSessionConnectFail(this, &Steam::_p2p_session_connect_fail),
	callbackP2PSessionRequest(this, &Steam::_p2p_session_request),

	// Parties //////////////////////////////////
	callbackJoinParty(this, &Steam::_join_party),
	callbackCreateBeacon(this, &Steam::_create_beacon),
	callbackReserveNotification(this, &Steam::_reservation_notification),
	callbackChangeNumOpenSlots(this, &Steam::_change_num_open_slots),
	callbackAvailableBeaconLocationsUpdated(this, &Steam::_available_beacon_locations_updated),
	callbackActiveBeaconsUpdated(this, &Steam::_active_beacons_updated),

	// Remote Play callbacks ////////////////////
	callbackRemotePlaySessionConnected(this, &Steam::_remote_play_session_connected),
	callbackRemotePlaySessionDisconnected(this, &Steam::_remote_play_session_disconnected),

	// Screenshot callbacks /////////////////////
	callbackScreenshotReady(this, &Steam::_screenshot_ready),
	callbackScreenshotRequested(this, &Steam::_screenshot_requested),

	// UGC callbacks ////////////////////////////
	callbackItemDownloaded(this, &Steam::_item_downloaded),
	callbackItemInstalled(this, &Steam::_item_installed),

	// User callbacks ///////////////////////////
	callbackClientGameServerDeny(this, &Steam::_client_game_server_deny),
	callbackGameWebCallback(this, &Steam::_game_web_callback),
	callbackGetAuthSessionTicketResponse(this, &Steam::_get_auth_session_ticket_response),
	callbackIPCFailure(this, &Steam::_ipc_failure),
	callbackLicensesUpdated(this, &Steam::_licenses_updated),
	callbackMicrotransactionAuthResponse(this, &Steam::_microstransaction_auth_response),
	callbackSteamServerConnected(this, &Steam::_steam_server_connected),
	callbackSteamServerDisconnected(this, &Steam::_steam_server_disconnected),
	callbackValidateAuthTicketResponse(this, &Steam::_validate_auth_ticket_response),

	// User stat callbacks //////////////////////
	callbackUserAchievementStored(this, &Steam::_user_achievement_stored),
	callbackCurrentStatsReceived(this, &Steam::_current_stats_received),
	callbackUserStatsStored(this, &Steam::_user_stats_stored),
	callbackUserStatsUnloaded(this, &Steam::_user_stats_unloaded),

	// Utility callbacks ////////////////////////
	callbackGamepadTextInputDismissed(this, &Steam::_gamepad_text_input_dismissed),
	callbackIPCountry(this, &Steam::_ip_country),
	callbackLowPower(this, &Steam::_low_power),
	callbackSteamAPICallCompleted(this, &Steam::_steam_api_call_completed),
	callbackSteamShutdown(this, &Steam::_steam_shutdown),

	// Video callbacks //////////////////////////
//	callbackBroadcastUploadStart(this, &Steam::_broadcast_upload_start),	// In documentation but not in actual SDK?
//	callbackBroadcastUploadStop(this, &Steam::_broadcast_upload_stop),		// In documentation but not in actual SDK?
	callbackGetOPFSettingsResult(this, &Steam::_get_opf_settings_result),
	callbackGetVideoResult(this, &Steam::_get_video_result)
{
	isInitSuccess = false;
	singleton = this;
	leaderboardDetailsMax = 10;
	tickets.clear();
	currentAppID = 0;
}

Steam* Steam::get_singleton(){
	return singleton;
}

CSteamID Steam::createSteamID(uint32_t steamID, int accountType){
	CSteamID cSteamID;
	if(accountType < 0 || accountType >= k_EAccountTypeMax){
		accountType = 1;
	}
	cSteamID.Set(steamID, k_EUniversePublic, EAccountType(accountType));
	return cSteamID;
}

/////////////////////////////////////////////////
///// MAIN FUNCTIONS ////////////////////////////
/////////////////////////////////////////////////
//
// Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't.
bool Steam::restartAppIfNecessary(int value){
	return SteamAPI_RestartAppIfNecessary((AppId_t)value);
}
// Initialize Steamworks
Dictionary Steam::steamInit(){
	// Create the response dictionary
	Dictionary initialize;
	// Attempt to initialize Steamworks
	isInitSuccess = SteamAPI_Init();
	// Set the default status response
	int status = RESULT_FAIL;
	String verbal = "Steamworks failed to initialize.";
	// Steamworks initialized with no problems
	if(isInitSuccess){
		status = RESULT_OK;
		verbal = "Steamworks active.";
	}
	// The Steam client is not running
	if(!SteamAPI_IsSteamRunning()){
		status = RESULT_SERVICE_UNAVAILABLE;
		verbal = "Steam not running.";
	}
	// The user is not logged into Steam or there is no active connection to Steam
	else if(!SteamUser()->BLoggedOn()){
		status = RESULT_NOT_LOGGED_ON;
		verbal = "Not logged on / no connection to Steam.";
	}
	// Steam is connected and active, so load the stats and achievements
	if(status == RESULT_OK && SteamUserStats() != NULL){
		SteamUserStats()->RequestCurrentStats();
	}
	// Get this app ID
	currentAppID = getAppID();
	// Compile the response
	initialize["status"] = status;
	initialize["verbal"] = verbal;
	// Return the Steamworks status
	return initialize;
}
// Returns true/false if Steam is running.
bool Steam::isSteamRunning(void){
	return SteamAPI_IsSteamRunning();
}

/////////////////////////////////////////////////
///// APPS //////////////////////////////////////
/////////////////////////////////////////////////
//
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
// Check if given application/game is installed, not necessarily owned.
bool Steam::isAppInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsAppInstalled((AppId_t)value);
}
// Checks whether the current App ID is for Cyber Cafes.
bool Steam::isCybercafe(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsCybercafe();
}
// Checks if the user owns a specific DLC and if the DLC is installed
bool Steam::isDLCInstalled(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsDlcInstalled(value);
}
// Checks if the license owned by the user provides low violence depots.
bool Steam::isLowViolence(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsLowViolence();
}
// Checks if the active user is subscribed to the current App ID.
bool Steam::isSubscribed(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribed();
}
// Checks if the active user is subscribed to a specified AppId.
bool Steam::isSubscribedApp(int value){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedApp((AppId_t)value);
}
//Checks if the active user is accessing the current appID via a temporary Family Shared license owned by another user.
//If you need to determine the steamID of the permanent owner of the license, use getAppOwner.
bool Steam::isSubscribedFromFamilySharing(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsSubscribedFromFamilySharing();
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
// Check if game is a timed trial with limited playtime.
Dictionary Steam::isTimedTrial(){
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
bool Steam::isVACBanned(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->BIsVACBanned();
}
// Return the build ID for this app; will change based on backend updates.
int Steam::getAppBuildId(){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetAppBuildId();
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
// Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
uint64_t Steam::getAppOwner(){
	if(SteamApps() == NULL){
		return 0;
	}
	CSteamID cSteamID = SteamApps()->GetAppOwner();
	return cSteamID.ConvertToUint64();
}
// Gets a comma separated list of the languages the current app supports.
String Steam::getAvailableGameLanguages(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetAvailableGameLanguages();
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
// Gets the current language that the user has set.
String Steam::getCurrentGameLanguage(){
	if(SteamApps() == NULL){
		return "None";
	}
	return SteamApps()->GetCurrentGameLanguage();
}
// Get the number of DLC the user owns for a parent application/game.
int Steam::getDLCCount(){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->GetDLCCount();
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
// Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).
int Steam::getEarliestPurchaseUnixTime(int value){
	if(SteamApps() == NULL){
		return 0;
	}
	return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
}
// Asynchronously retrieves metadata details about a specific file in the depot manifest.
void Steam::getFileDetails(const String& filename){
	if(SteamApps() != NULL){
		SteamApps()->GetFileDetails(filename.utf8().get_data());
	}
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
	delete depots;
	return installedDepots;
}
// Gets the command line if the game was launched via Steam URL, e.g. steam://run/<appid>//<command line>/. This method is preferable to launching with a command line via the operating system, which can be a security risk. In order for rich presence joins to go through this and not be placed on the OS command line, you must enable "Use launch command line" from the Installation > General page on your app.
String Steam::getLaunchCommandLine(){
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
String Steam::getLaunchQueryParam(const String& key){
	if(SteamApps() == NULL){
		return "";
	}
	return SteamApps()->GetLaunchQueryParam(key.utf8().get_data());
}
// Allows you to install an optional DLC.
void Steam::installDLC(int value){
	if(SteamApps() != NULL){
		SteamApps()->InstallDLC((AppId_t)value);
	}
}
// Allows you to force verify game content on next launch.
bool Steam::markContentCorrupt(bool missingFilesOnly){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->MarkContentCorrupt(missingFilesOnly);
}
// Allows you to uninstall an optional DLC.
void Steam::uninstallDLC(int value){
	if(SteamApps() != NULL){
		SteamApps()->UninstallDLC((AppId_t)value);
	}
}

/////////////////////////////////////////////////
///// FRIENDS ///////////////////////////////////
/////////////////////////////////////////////////
//
// Activates the overlay with optional dialog to open the following: "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite".
void Steam::activateGameOverlay(const String& url){
	if(SteamFriends() != NULL){
		SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
	}
}
// Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
void Steam::activateGameOverlayInviteDialog(uint64_t steamID){
	if(SteamFriends() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamFriends()->ActivateGameOverlayInviteDialog(userID);
	}
}
// Activates the overlay with the application/game Steam store page.
void Steam::activateGameOverlayToStore(int appID){
	if(SteamFriends() != NULL){
		SteamFriends()->ActivateGameOverlayToStore(AppId_t(appID), EOverlayToStoreFlag(0));
	}
}
// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore".
void Steam::activateGameOverlayToUser(const String& url, uint64_t steamID){
	if(SteamFriends() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), userID);
	}
}
// Activates the overlay with specified web address.
void Steam::activateGameOverlayToWebPage(const String& url){
	if(SteamFriends() != NULL){
		SteamFriends()->ActivateGameOverlayToWebPage(url.utf8().get_data());
	}
}
// Clear the game information in Steam; used in 'View Game Info'.
void Steam::clearRichPresence(){
	if(SteamFriends() != NULL){
		SteamFriends()->ClearRichPresence();
	}
}
// Closes the specified Steam group chat room in the Steam UI.
bool Steam::closeClanChatWindowInSteam(uint64_t chatID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->CloseClanChatWindowInSteam(chat);
}
// For clans a user is a member of, they will have reasonably up-to-date information, but for others you'll have to download the info to have the latest.
void Steam::downloadClanActivityCounts(uint64_t clanID, int clansToRequest){
	if(SteamFriends() != NULL){
		CSteamID clan = (uint64)clanID;
		SteamFriends()->DownloadClanActivityCounts(&clan, clansToRequest);
	}
}
// Gets the list of users that the current user is following.
void Steam::enumerateFollowingList(uint32 startIndex){
	if(SteamFriends() != NULL){
		SteamAPICall_t apiCall = SteamFriends()->EnumerateFollowingList(startIndex);
		callResultEnumerateFollowingList.Set(apiCall, this, &Steam::_enumerate_following_list);
	}
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
// Gets the most recent information we have about what the users in a Steam Group are doing.
Dictionary Steam::getClanActivityCounts(uint64_t clanID){
	Dictionary activity;
	if(SteamFriends() == NULL){
		return activity;
	}
	CSteamID clan = (uint64)clanID;
	int online = 0;
	int ingame = 0;
	int chatting = 0;
	bool success = SteamFriends()->GetClanActivityCounts(clan, &online, &ingame, &chatting);
	// Add these to the dictionary if successful
	if(success){
		activity["clan"] = clanID;
		activity["online"] = online;
		activity["ingame"] = ingame;
		activity["chatting"] = chatting;
	}
	return activity;
}
uint64_t Steam::getClanByIndex(int clan){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetClanByIndex(clan).ConvertToUint64();
}
// Get the number of users in a Steam group chat.
int Steam::getClanChatMemberCount(uint64_t clanID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->GetClanChatMemberCount(clan);
}
//  Gets the data from a Steam group chat room message.  This should only ever be called in response to a GameConnectedClanChatMsg_t callback.
Dictionary getClanChatMessage(uint64_t chatID, int message){
	Dictionary chatMessage;
	if(SteamFriends() == NULL){
		return chatMessage;
	}
	CSteamID chat = (uint64)chatID;
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	CSteamID userID;
	chatMessage["ret"] = SteamFriends()->GetClanChatMessage(chat, message, text, 2048, &type, &userID);
	chatMessage["text"] = String(text);
	chatMessage["type"] = type;
	uint64_t user = userID.ConvertToUint64();
	chatMessage["chatter"] = user;
	return chatMessage;
}
// Gets the number of Steam groups that the current user is a member of.  This is used for iteration, after calling this then GetClanByIndex can be used to get the Steam ID of each Steam group.
int Steam::getClanCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetClanCount();
}
// Gets the display name for the specified Steam group; if the local client knows about it.
String Steam::getClanName(uint64_t clanID){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->GetClanName(clan);
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
// Returns the number of officers in a clan (including the owner).
int Steam::getClanOfficerCount(uint64_t clanID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->GetClanOfficerCount(clan);
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
// Gets the unique tag (abbreviation) for the specified Steam group; If the local client knows about it.  The Steam group abbreviation is a unique way for people to identify the group and is limited to 12 characters. In some games this will appear next to the name of group members.
String Steam::getClanTag(uint64_t clanID){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->GetClanTag(clan);	
}
// Gets the Steam ID of the recently played with user at the given index.
uint64_t Steam::getCoplayFriend(int friendNum){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID friendID = SteamFriends()->GetCoplayFriend(friendNum);
	return friendID.ConvertToUint64();
}
// Gets the number of players that the current users has recently played with, across all games.  This is used for iteration, after calling this then GetCoplayFriend can be used to get the Steam ID of each player.  These players are have been set with previous calls to SetPlayedWith.
int Steam::getCoplayFriendCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetCoplayFriendCount();
}
// Gets the number of users following the specified user.
void Steam::getFollowerCount(uint64_t steamID){
	if(SteamFriends() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamAPICall_t apiCall = SteamFriends()->GetFollowerCount(userID);
		callResultFollowerCount.Set(apiCall, this, &Steam::_get_follower_count);
	}
}
// Returns the Steam ID of a user.
uint64_t Steam::getFriendByIndex(int friendNum, int friendFlags){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID friendID = SteamFriends()->GetFriendByIndex(friendNum, friendFlags);
	return friendID.ConvertToUint64();
}
// Gets the app ID of the game that user played with someone on their recently-played-with list.
int Steam::getFriendCoplayGame(uint64_t friendID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID steamID = (uint64)friendID;
	return SteamFriends()->GetFriendCoplayGame(steamID);
}
// Gets the timestamp of when the user played with someone on their recently-played-with list.  The time is provided in Unix epoch format (seconds since Jan 1st 1970).
int Steam::getFriendCoplayTime(uint64_t friendID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID steamID = (uint64)friendID;
	return SteamFriends()->GetFriendCoplayTime(steamID);
}
// Get number of friends user has.
int Steam::getFriendCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendCount(0x04);
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
// Returns dictionary of friend game played if valid
Dictionary Steam::getFriendGamePlayed(uint64_t steamID){
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
			sprintf(gameIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
			friendGame["ip"] = gameIP;
			friendGame["gamePort"] = gameInfo.m_usGamePort;
			friendGame["queryPort"] = (char)gameInfo.m_usQueryPort;
			friendGame["lobby"] = uint64_t(gameInfo.m_steamIDLobby.ConvertToUint64());
		}
		else{
			friendGame["id"] = gameInfo.m_gameID.AppID();
			friendGame["ip"] = "0.0.0.0";
			friendGame["gamePort"] = "0";
			friendGame["queryPort"] = "0";
			friendGame["lobby"] = "No valid lobby";
		}
	}
	return friendGame;
}
// Gets the data from a Steam friends message. This should only ever be called in response to a GameConnectedFriendChatMsg_t callback.
Dictionary Steam::getFriendMessage(uint64_t friendID, int message){
	Dictionary chat;
	if(SteamFriends() == NULL){
		return chat;
	}
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	chat["ret"] = SteamFriends()->GetFriendMessage(createSteamID(friendID), message, text, 2048, &type);
	chat["text"] = String(text);
	return chat;
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
// Accesses old friends names; returns an empty string when there are no more items in the history.
String Steam::getFriendPersonaNameHistory(uint64_t steamID, int nameHistory){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendPersonaNameHistory(userID, nameHistory);
}
// Returns the current status of the specified user.
int Steam::getFriendPersonaState(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendPersonaState(userID);
}
// Returns a relationship to a user.
int Steam::getFriendRelationship(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendRelationship(userID);
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
// Gets the number of friends groups (tags) the user has created.  This is used for iteration, after calling this then GetFriendsGroupIDByIndex can be used to get the ID of each friend group.  This is not to be confused with Steam groups. Those can be obtained with GetClanCount.
int Steam::getFriendsGroupCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendsGroupCount();
}
// Gets the friends group ID for the given index.
int Steam::getFriendsGroupIDByIndex(int friendGroup){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendsGroupIDByIndex(friendGroup);
}
// Gets the number of friends in a given friends group.  This should be called before getting the list of friends with GetFriendsGroupMembersList.
int Steam::getFriendsGroupMembersCount(int friendGroup){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendsGroupMembersCount(friendGroup);
}
// Gets the number of friends in the given friends group.  If fewer friends exist than requested those positions' Steam IDs will be invalid.  You must call GetFriendsGroupMembersCount before calling this to set up the pOutSteamIDMembers array with an appropriate size!
Array Steam::getFriendsGroupMembersList(int friendGroup, int memberCount){
	Array memberList;
	if(SteamFriends() == NULL){
		return memberList;
	}
	CSteamID friendIDs;
	SteamFriends()->GetFriendsGroupMembersList((FriendsGroupID_t)friendGroup, &friendIDs, memberCount);
	uint64_t friends = friendIDs.ConvertToUint64();
	memberList.append(friends);
	return memberList;
}
// Gets the name for the given friends group.
String Steam::getFriendsGroupName(int friendGroup){
	if(SteamFriends() == NULL){
		return "";
	}
	return SteamFriends()->GetFriendsGroupName(friendGroup);
}
// Get friend's steam level, obviously.
int Steam::getFriendSteamLevel(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetFriendSteamLevel(userID);
}
// Gets the large (184x184) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getLargeFriendAvatar(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetLargeFriendAvatar(userID);
}
// Gets the medium (64x64) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getMediumFriendAvatar(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetMediumFriendAvatar(userID);
}
// Get the user's Steam username.
String Steam::getPersonaName(){
	if(SteamFriends() == NULL){
		return "";
	}
	return SteamFriends()->GetPersonaName();
}
// Gets the status of the current user.
int Steam::getPersonaState(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetPersonaState();
}
// Get player's avatar.
void Steam::getPlayerAvatar(int size, uint64_t steamID){
	// If no Steam ID is given, use the current user's
	if(steamID == 0){
		steamID = getSteamID();
	}
	if(SteamFriends() != NULL || size > 0 || size < 4){
		int handle = -2;
		switch(size){
			case 1:{
				handle = getSmallFriendAvatar(steamID);
				size = 32; break;
			}
			case 2:{
				handle = getMediumFriendAvatar(steamID);
				size = 64; break;
			}
			case 3:{
				handle = getLargeFriendAvatar(steamID);
				size = 184; break;
			}
			default:
				return;
		}
		if(handle <= 0){
			printf("[Steam] Error retrieving avatar handle.");
		}
		AvatarImageLoaded_t* avatarData = new AvatarImageLoaded_t;
		CSteamID avatarID = (uint64)steamID;
		avatarData->m_steamID = avatarID;
		avatarData->m_iImage = handle;
		avatarData->m_iWide = size;
		avatarData->m_iTall = size;
		_avatar_loaded(avatarData);
	}
}
// Returns nickname the current user has set for the specified player. Returns NULL if the no nickname has been set for that player.
String Steam::getPlayerNickname(uint64_t steamID){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetPlayerNickname(userID);
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
// Gets the small (32x32) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getSmallFriendAvatar(uint64_t steamID){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->GetSmallFriendAvatar(userID);
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
// If current user is chat restricted, he can't send or receive any text/voice chat messages. The user can't see custom avatars. But the user can be online and send/recv game invites.
uint32 Steam::getUserRestrictions(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetUserRestrictions();
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
// Returns true if the specified user meets any of the criteria specified in iFriendFlags.
bool Steam::hasFriend(uint64_t steamID, int friendFlags){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->HasFriend(userID, friendFlags);
}
// Invite friend to current game/lobby.
bool Steam::inviteUserToGame(uint64_t steamID, const String& connectString){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->InviteUserToGame(userID, connectString.utf8().get_data());
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
// Checks if the Steam Group chat room is open in the Steam UI.
bool Steam::isClanChatWindowOpenInSteam(uint64_t chatID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->IsClanChatWindowOpenInSteam(chat);
}
// Checks if the current user is following the specified user.
void Steam::isFollowing(uint64_t steamID){
	if(SteamFriends() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamAPICall_t apiCall = SteamFriends()->IsFollowing(userID);
		callResultIsFollowing.Set(apiCall, this, &Steam::_is_following);
	}
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
// Allows the user to join Steam group (clan) chats right within the game.
void Steam::joinClanChatRoom(uint64_t clanID){
	if(SteamFriends() != NULL){
		CSteamID clan = (uint64)clanID;
		SteamFriends()->JoinClanChatRoom(clan);
	}
}
// Leaves a Steam group chat that the user has previously entered with JoinClanChatRoom.
bool Steam::leaveClanChatRoom(uint64_t clanID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID clan = (uint64)clanID;
	return SteamFriends()->LeaveClanChatRoom(clan);
}
// Opens the specified Steam group chat room in the Steam UI.
bool Steam::openClanChatWindowInSteam(uint64_t chatID){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->OpenClanChatWindowInSteam(chat);
}
// Call this before calling ActivateGameOverlayToWebPage() to have the Steam Overlay Browser block navigations to your specified protocol (scheme) uris and instead dispatch a OverlayBrowserProtocolNavigation_t callback to your game.
bool Steam::registerProtocolInOverlayBrowser(const String& protocol){
	if(SteamFriends() == NULL){
		return false;
	}
	return SteamFriends()->RegisterProtocolInOverlayBrowser(protocol.utf8().get_data());
}
// Sends a message to a Steam friend.
bool Steam::replyToFriendMessage(uint64_t steamID, const String& message){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->ReplyToFriendMessage(userID, message.utf8().get_data());
}
// Requests information about a clan officer list; when complete, data is returned in ClanOfficerListResponse_t call result.
void Steam::requestClanOfficerList(uint64_t clanID){
	if(SteamFriends() != NULL){
		CSteamID clan = (uint64)clanID;
		SteamAPICall_t apiCall = SteamFriends()->GetFollowerCount(clan);
		callResultClanOfficerList.Set(apiCall, this, &Steam::_request_clan_officer_list);
	}
}
// Requests rich presence for a specific user.
void Steam::requestFriendRichPresence(uint64_t friendID){
	if(SteamFriends() != NULL){
		CSteamID user = (uint64)friendID;
		return SteamFriends()->RequestFriendRichPresence(user);
	}
}
// Requests information about a user - persona name & avatar; if bRequireNameOnly is set, then the avatar of a user isn't downloaded.
bool Steam::requestUserInformation(uint64_t steamID, bool requireNameOnly){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamFriends()->RequestUserInformation(userID, requireNameOnly);
}
// Sends a message to a Steam group chat room.
bool Steam::sendClanChatMessage(uint64_t chatID, const String& text){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chatID;
	return SteamFriends()->SendClanChatMessage(chat, text.utf8().get_data());
}
// User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI).
void Steam::setInGameVoiceSpeaking(uint64_t steamID, bool speaking){
	if(SteamFriends() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamFriends()->SetInGameVoiceSpeaking(userID, speaking);
	}
}
// Listens for Steam friends chat messages.
bool Steam::setListenForFriendsMessages(bool intercept){
	if(SteamFriends() == NULL){
		return false;
	}
	return SteamFriends()->SetListenForFriendsMessages(intercept);
}
// Sets the player name, stores it on the server and publishes the changes to all friends who are online.
void Steam::setPersonaName(const String& name){
	if(SteamFriends() != NULL){
		SteamFriends()->SetPersonaName(name.utf8().get_data());
	}
}
// Set player as 'Played With' for game.
void Steam::setPlayedWith(uint64_t steamID){
	if(SteamFriends() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamFriends()->SetPlayedWith(userID);
	}
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

/////////////////////////////////////////////////
///// HTML SURFACE //////////////////////////////
/////////////////////////////////////////////////
//
// Add a header to any HTTP requests from this browser.
void Steam::addHeader(const String& key, const String& value){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->AddHeader(browserHandle, key.utf8().get_data(), value.utf8().get_data());
	}
}
// Sets whether a pending load is allowed or if it should be canceled.  NOTE:You MUST call this in response to a HTML_StartRequest_t callback.
void Steam::allowStartRequest(bool allowed){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->AllowStartRequest(browserHandle, allowed);
	}
}
// Copy the currently selected text from the current page in an HTML surface into the local clipboard.
void Steam::copyToClipboard(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->CopyToClipboard(browserHandle);
	}
}
// Create a browser object for displaying of an HTML page. NOTE: You MUST call RemoveBrowser when you are done using this browser to free up the resources associated with it. Failing to do so will result in a memory leak.
void Steam::createBrowser(const String& userAgent, const String& userCSS){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->CreateBrowser(userAgent.utf8().get_data(), userCSS.utf8().get_data());
	}
}
// Run a javascript script in the currently loaded page.
void Steam::executeJavascript(const String& script){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->ExecuteJavascript(browserHandle, script.utf8().get_data());
	}
}
// Allows you to react to a page wanting to open a file load dialog. NOTE:You MUST call this in response to a HTML_FileOpenDialog_t callback.
//void Steam::fileLoadDialogResponse(const String& selectedFiles){
//	if(SteamHTMLSurface() != NULL){
//		SteamHTMLSurface()->FileLoadDialogResponse(browserHandle, selectedFiles.utf8().get_data());
//	}
//}
// Find a string in the current page of an HTML surface. This is the equivalent of "ctrl+f" in your browser of choice. It will highlight all of the matching strings. You should call StopFind when the input string has changed or you want to stop searching.
void Steam::find(const String& search, bool currentlyInFind, bool reverse){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->Find(browserHandle, search.utf8().get_data(), currentlyInFind, reverse);
	}
}
// Retrieves details about a link at a specific position on the current page in an HTML surface.
void Steam::getLinkAtPosition(int x, int y){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->GetLinkAtPosition(browserHandle, x, y);
	}
}
// Navigate back in the page history.
void Steam::goBack(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->GoBack(browserHandle);
	}
}
// Navigate forward in the page history
void Steam::goForward(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->GoForward(browserHandle);
	}
}
// Initializes the HTML Surface API. This must be called prior to using any other functions in this interface. You MUST call Shutdown when you are done using the interface to free up the resources associated with it. Failing to do so will result in a memory leak!
void Steam::htmlInit(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->Init();
	}
}
// Allows you to react to a page wanting to open a javascript modal dialog notification.
void Steam::jsDialogResponse(bool result){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->JSDialogResponse(browserHandle, result);
	}
}
// cUnicodeChar is the unicode character point for this keypress (and potentially multiple chars per press).
void Steam::keyChar(uint32 unicodeChar, int keyModifiers){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->KeyChar(browserHandle, unicodeChar, (ISteamHTMLSurface::EHTMLKeyModifiers)keyModifiers);
	}
}
// Keyboard interactions, native keycode is the virtual key code value from your OS.
void Steam::keyDown(uint32 nativeKeyCode, int keyModifiers){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->KeyDown(browserHandle, nativeKeyCode, (ISteamHTMLSurface::EHTMLKeyModifiers)keyModifiers);
	}
}
// Keyboard interactions, native keycode is the virtual key code value from your OS.
void Steam::keyUp(uint32 nativeKeyCode, int keyModifiers){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->KeyUp(browserHandle, nativeKeyCode, (ISteamHTMLSurface::EHTMLKeyModifiers)keyModifiers);
	}
}
// Navigate to a specified URL. If you send POST data with pchPostData then the data should be formatted as: name1=value1&name2=value2. You can load any URI scheme supported by Chromium Embedded Framework including but not limited to: http://, https://, ftp://, and file:///. If no scheme is specified then http:// is used.
void Steam::loadURL(const String& url, const String& postData){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->LoadURL(browserHandle, url.utf8().get_data(), postData.utf8().get_data());
	}
}
// Tells an HTML surface that a mouse button has been double clicked. The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
void Steam::mouseDoubleClick(int mouseButton){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseDoubleClick(browserHandle, (ISteamHTMLSurface::EHTMLMouseButton)mouseButton);
	}
}
// Tells an HTML surface that a mouse button has been pressed. The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
void Steam::mouseDown(int mouseButton){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseDown(browserHandle, (ISteamHTMLSurface::EHTMLMouseButton)mouseButton);
	}
}
// Tells an HTML surface where the mouse is.
void Steam::mouseMove(int x, int y){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseMove(browserHandle, x, y);
	}
}
// Tells an HTML surface that a mouse button has been released. The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
void Steam::mouseUp(int mouseButton){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseUp(browserHandle, (ISteamHTMLSurface::EHTMLMouseButton)mouseButton);
	}
}
// Tells an HTML surface that the mouse wheel has moved.
void Steam::mouseWheel(int32 delta){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseWheel(browserHandle, delta);
	}
}
// Paste from the local clipboard to the current page in an HTML surface.
void Steam::pasteFromClipboard(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->PasteFromClipboard(browserHandle);
	}
}
// Refreshes the current page. The reload will most likely hit the local cache instead of going over the network. This is equivalent to F5 or Ctrl+R in your browser of choice.
void Steam::reload(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->Reload(browserHandle);
	}
}
// You MUST call this when you are done with an HTML surface, freeing the resources associated with it. Failing to call this will result in a memory leak!
void Steam::removeBrowser(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->RemoveBrowser(browserHandle);
	}
}
// Enable/disable low-resource background mode, where javascript and repaint timers are throttled, resources are more aggressively purged from memory, and audio/video elements are paused. When background mode is enabled, all HTML5 video and audio objects will execute ".pause()" and gain the property "._steam_background_paused = 1". When background mode is disabled, any video or audio objects with that property will resume with ".play()".
void Steam::setBackgroundMode(bool backgroundMode){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetBackgroundMode(browserHandle, backgroundMode);
	}
}
// Set a webcookie for a specific hostname. You can read more about the specifics of setting cookies here on wikipedia.
//void Steam::setCookie(const String& hostname, const String& key, const String& value, const String& path, uint32 expires, bool secure, bool httpOnly){
//	if(SteamHTMLSurface() != NULL){
//		SteamHTMLSurface()->SetCookie(hostname.utf8().get_data(), key.utf8().get_data(), value.utf8().get_data(), path.utf8().get_data(), expires, secure, httpOnly);
//	}
//}
// Scroll the current page horizontally.
void Steam::setHorizontalScroll(uint32 absolutePixelScroll){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetHorizontalScroll(browserHandle, absolutePixelScroll);
	}
}
// Tell a HTML surface if it has key focus currently, controls showing the I-beam cursor in text controls amongst other things.
void Steam::setKeyFocus(bool hasKeyFocus){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetKeyFocus(browserHandle, hasKeyFocus);	
	}
}
// Zoom the current page in an HTML surface. The current scale factor is available from HTML_NeedsPaint_t.flPageScale, HTML_HorizontalScroll_t.flPageScale, and HTML_VerticalScroll_t.flPageScale.
void Steam::setPageScaleFactor(float zoom, int pointX, int pointY){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetPageScaleFactor(browserHandle, zoom, pointX, pointY);
	}
}
// Sets the display size of a surface in pixels.
void Steam::setSize(uint32 width, uint32 height){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetSize(browserHandle, width, height);
	}
}
// Scroll the current page vertically.
void Steam::setVerticalScroll(uint32 absolutePixelScroll){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetVerticalScroll(browserHandle, absolutePixelScroll);
	}
}
// Shutdown the ISteamHTMLSurface interface, releasing the memory and handles. You MUST call this when you are done using this interface to prevent memory and handle leaks. After calling this then all of the functions provided in this interface will fail until you call Init to reinitialize again.
bool Steam::htmlShutdown(){
	if(SteamHTMLSurface() == NULL){
		return false;
	}
	return SteamHTMLSurface()->Shutdown();
}
// Cancel a currently running find.
void Steam::stopFind(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->StopFind(browserHandle);
	}
}
// Stop the load of the current HTML page.
void Steam::stopLoad(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->StopLoad(browserHandle);
	}
}
// Open the current pages HTML source code in default local text editor, used for debugging.
void Steam::viewSource(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->ViewSource(browserHandle);
	}
}

/////////////////////////////////////////////////
///// HTTP //////////////////////////////////////
/////////////////////////////////////////////////
//
//Creates a cookie container to store cookies during the lifetime of the process. This API is just for during process lifetime, after steam restarts no cookies are persisted and you have no way to access the cookie container across repeat executions of your process.
void Steam::createCookieContainer(bool allowResponsesToModify){
	if(SteamHTTP() != NULL){
		SteamHTTP()->CreateCookieContainer(allowResponsesToModify);
	}
}
// Initializes a new HTTP request.
void Steam::createHTTPRequest(int requestMethod, const String& absoluteURL){
	if(SteamHTTP() != NULL){
		SteamHTTP()->CreateHTTPRequest((EHTTPMethod)requestMethod, absoluteURL.utf8().get_data());
	}
}
// Defers a request which has already been sent by moving it at the back of the queue.
bool Steam::deferHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->DeferHTTPRequest(request);
}
// Gets progress on downloading the body for the request.
float Steam::getHTTPDownloadProgressPct(uint32 request){
	float percentOut = 0.0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPDownloadProgressPct(request, &percentOut);
	}
	return percentOut;
}
// Check if the reason the request failed was because we timed it out (rather than some harder failure).
bool Steam::getHTTPRequestWasTimedOut(uint32 request){
	bool wasTimedOut = false;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPRequestWasTimedOut(request, &wasTimedOut);
	}
	return wasTimedOut;
}
// Gets the body data from an HTTP response.
uint8 Steam::getHTTPResponseBodyData(uint32 request, uint32 bufferSize){
	uint8 bodyData = 0; 
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodyData(request, &bodyData, bufferSize);
	}
	return bodyData;
}
// Gets the size of the body data from an HTTP response.
uint32 Steam::getHTTPResponseBodySize(uint32 request){
	uint32 bodySize = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodySize(request, &bodySize);
	}
	return bodySize;
}
// Checks if a header is present in an HTTP response and returns its size.
uint32 Steam::getHTTPResponseHeaderSize(uint32 request, const String& headerName){
	uint32 responseHeaderSize = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderSize(request, headerName.utf8().get_data(), &responseHeaderSize);
	}
	return responseHeaderSize;
}
// Gets a header value from an HTTP response.
uint8 Steam::getHTTPResponseHeaderValue(uint32 request, const String& headerName, uint32 bufferSize){
	uint8 valueBuffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderValue(request, headerName.utf8().get_data(), &valueBuffer, bufferSize);
	}
	return valueBuffer;
}
// Gets the body data from a streaming HTTP response.
uint8 Steam::getHTTPStreamingResponseBodyData(uint32 request, uint32 offset, uint32 bufferSize){
	uint8 bodyDataBuffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPStreamingResponseBodyData(request, offset, &bodyDataBuffer, bufferSize);
	}
	return bodyDataBuffer;
}
// Prioritizes a request which has already been sent by moving it at the front of the queue.
bool Steam::prioritizeHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->PrioritizeHTTPRequest(request);
}
// Releases a cookie container, freeing the memory allocated within Steam.
bool Steam::releaseCookieContainer(){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseCookieContainer(cookieHandle);
}
// Releases an HTTP request handle, freeing the memory allocated within Steam.
bool Steam::releaseHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseHTTPRequest(request);
}
// Sends an HTTP request.
bool Steam::sendHTTPRequest(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t callHandle;
	return SteamHTTP()->SendHTTPRequest(request, &callHandle);
}
// Sends an HTTP request and streams the response back in chunks.
bool Steam::sendHTTPRequestAndStreamResponse(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t callHandle;
	return SteamHTTP()->SendHTTPRequestAndStreamResponse(request, &callHandle);
}
// Adds a cookie to the specified cookie container that will be used with future requests.
bool Steam::setCookie(const String& host, const String& url, const String& cookie){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetCookie(cookieHandle, host.utf8().get_data(), url.utf8().get_data(), cookie.utf8().get_data());
}
// Set an absolute timeout in milliseconds for the HTTP request. This is the total time timeout which is different than the network activity timeout which is set with SetHTTPRequestNetworkActivityTimeout which can bump everytime we get more data.
bool Steam::setHTTPRequestAbsoluteTimeoutMS(uint32 request, uint32 milliseconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestAbsoluteTimeoutMS(request, milliseconds);
}
// Set a context value for the request, which will be returned in the HTTPRequestCompleted_t callback after sending the request. This is just so the caller can easily keep track of which callbacks go with which request data. Must be called before sending the request.
bool Steam::setHTTPRequestContextValue(uint32 request, uint64 contextValue){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestContextValue(request, contextValue);
}
// Associates a cookie container to use for an HTTP request.
bool Steam::setHTTPRequestCookieContainer(uint32 request){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestCookieContainer(request, cookieHandle);
}
// Set a GET or POST parameter value on the HTTP request. Must be called prior to sending the request.
bool Steam::setHTTPRequestGetOrPostParameter(uint32 request, const String& name, const String& value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestGetOrPostParameter(request, name.utf8().get_data(), value.utf8().get_data());
}
// Set a request header value for the HTTP request. Must be called before sending the request.
bool Steam::setHTTPRequestHeaderValue(uint32 request, const String& headerName, const String& headerValue){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestHeaderValue(request, headerName.utf8().get_data(), headerValue.utf8().get_data());
}
// Set the timeout in seconds for the HTTP request.
bool Steam::setHTTPRequestNetworkActivityTimeout(uint32 request, uint32 timeoutSeconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestNetworkActivityTimeout(request, timeoutSeconds);
}
// Sets the body for an HTTP Post request.
uint8 Steam::setHTTPRequestRawPostBody(uint32 request, const String& contentType, uint32 bodyLen){
	uint8 body = 0;
	if(SteamHTTP()){
		SteamHTTP()->SetHTTPRequestRawPostBody(request, contentType.utf8().get_data(), &body, bodyLen);
	}
	return body;
}
// Sets that the HTTPS request should require verified SSL certificate via machines certificate trust store. This currently only works Windows and macOS.
bool Steam::setHTTPRequestRequiresVerifiedCertificate(uint32 request, bool requireVerifiedCertificate){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestRequiresVerifiedCertificate(request, requireVerifiedCertificate);
}
// Set additional user agent info for a request.
bool Steam::setHTTPRequestUserAgentInfo(uint32 request, const String& userAgentInfo){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestUserAgentInfo(request, userAgentInfo.utf8().get_data());
}

/////////////////////////////////////////////////
///// INPUT /////////////////////////////////////
/////////////////////////////////////////////////
//
// Reconfigure the controller to use the specified action set.
void Steam::activateActionSet(uint64_t inputHandle, uint64_t actionSetHandle){
	if(SteamInput() != NULL){
		SteamInput()->ActivateActionSet((InputHandle_t)inputHandle, (ControllerActionSetHandle_t)actionSetHandle);
	}
}
// Reconfigure the controller to use the specified action set layer.
void Steam::activateActionSetLayer(uint64_t inputHandle, uint64_t actionSetLayerHandle){
	if(SteamInput() != NULL){
		SteamInput()->ActivateActionSetLayer((InputHandle_t)inputHandle, (ControllerActionSetHandle_t)actionSetLayerHandle);
	}
}
// Reconfigure the controller to stop using the specified action set.
void Steam::deactivateActionSetLayer(uint64_t inputHandle, uint64_t actionSetHandle){
	if(SteamInput() != NULL){
		SteamInput()->DeactivateActionSetLayer((InputHandle_t)inputHandle, (ControllerActionSetHandle_t)actionSetHandle);
	}
}
// Reconfigure the controller to stop using all action set layers.
void Steam::deactivateAllActionSetLayers(uint64_t inputHandle){
	if(SteamInput() != NULL){
		SteamInput()->DeactivateAllActionSetLayers((InputHandle_t)inputHandle);
	}
}
// Lookup the handle for an Action Set. Best to do this once on startup, and store the handles for all future API calls.
uint64_t Steam::getActionSetHandle(const String& actionSetName){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetActionSetHandle(actionSetName.utf8().get_data());
	}
	return 0;
}
// Get an action origin that you can use in your glyph look up table or passed into GetGlyphForActionOrigin or GetStringForActionOrigin.
int Steam::getActionOriginFromXboxOrigin(uint64_t inputHandle, int origin){
	if(SteamInput() == NULL){
		return 0;
	}
	return SteamInput()->GetActionOriginFromXboxOrigin((InputHandle_t)inputHandle, (EXboxOrigin)origin);
}
// Fill an array with all of the currently active action set layers for a specified controller handle.
Array Steam::getActiveActionSetLayers(uint64_t inputHandle){
	Array handles;
	if(SteamInput() != NULL){
		InputActionSetHandle_t *out = new InputActionSetHandle_t[INPUT_MAX_COUNT];
		int ret = SteamInput()->GetActiveActionSetLayers(inputHandle, out);
		for(int i = 0; i < ret; i++){
			handles.push_back((int)out[i]);
		}
	}
	return handles;
}
// Returns the current state of the supplied analog game action.
Dictionary Steam::getAnalogActionData(uint64_t inputHandle, uint64_t analogActionHandle){
	ControllerAnalogActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamInput() != NULL){
		data = SteamInput()->GetAnalogActionData((InputHandle_t)inputHandle, (ControllerAnalogActionHandle_t)analogActionHandle);
	}
	d["eMode"] = data.eMode;
	d["x"] = data.x;
	d["y"] = data.y;
	d["bActive"] = data.bActive;
	return d;
}
// Get the handle of the specified Analog action.
uint64_t Steam::getAnalogActionHandle(const String& actionName){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetAnalogActionHandle(actionName.utf8().get_data());
	}
	return 0;
}
// Get the origin(s) for an analog action within an action.
Array Steam::getAnalogActionOrigins(uint64_t inputHandle, uint64_t actionSetHandle, uint64_t analogActionHandle){
	Array list;
	if(SteamInput() != NULL){
		EInputActionOrigin *out = new EInputActionOrigin[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamInput()->GetAnalogActionOrigins((InputHandle_t)inputHandle, (ControllerActionSetHandle_t)actionSetHandle, (ControllerAnalogActionHandle_t)analogActionHandle, out);
		for (int i = 0; i < ret; i++){
			list.push_back((int)out[i]);
		}
	}
	return list;
}
// Get current controllers handles.
Array Steam::getConnectedControllers(){
	Array list;
	if(SteamInput() != NULL){
		InputHandle_t *handles = new InputHandle_t[INPUT_MAX_COUNT];
		int ret = SteamInput()->GetConnectedControllers(handles);
		printf("[Steam] Inputs found %d controllers.", ret);
		for (int i = 0; i < ret; i++){
			list.push_back((uint64_t)handles[i]);
		}
	}
	return list;
}
// Returns the associated controller handle for the specified emulated gamepad.
uint64_t Steam::getControllerForGamepadIndex(int index){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetControllerForGamepadIndex(index);
	}
	return 0;
}
// Get the currently active action set for the specified controller.
uint64_t Steam::getCurrentActionSet(uint64_t inputHandle){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetCurrentActionSet((InputHandle_t)inputHandle);
	}
	return 0;
}
// Get's the major and minor device binding revisions for Steam Input API configurations. Minor revisions are for small changes such as adding a new option action or updating localization in the configuration. When updating a Minor revision only one new configuration needs to be update with the "Use Action Block" flag set. Major revisions are to be used when changing the number of action sets or otherwise reworking configurations to the degree that older configurations are no longer usable. When a user's binding disagree's with the major revision of the current official configuration Steam will forcibly update the user to the new configuration. New configurations will need to be made for every controller when updating the Major revision.
Array Steam::getDeviceBindingRevision(uint64_t inputHandle){
	Array revision;
	if(SteamInput() != NULL){
		int major = 0;
		int minor = 0;
		bool success = SteamInput()->GetDeviceBindingRevision((InputHandle_t)inputHandle, &major, &minor);
		if(success){
			revision.append(major);
			revision.append(minor);
		}
	}
	return revision;
}
// Returns the current state of the supplied digital game action.
Dictionary Steam::getDigitalActionData(uint64_t inputHandle, uint64_t digitalActionHandle){
	ControllerDigitalActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamInput() != NULL){
		data = SteamInput()->GetDigitalActionData((InputHandle_t)inputHandle, (ControllerDigitalActionHandle_t)digitalActionHandle);
	}
	d["bState"] = data.bState;
	d["bActive"] = data.bActive;
	return d;
}
// Get the handle of the specified digital action.
uint64_t Steam::getDigitalActionHandle(const String& actionName){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetDigitalActionHandle(actionName.utf8().get_data());
	}
	return 0;
}
// Get the origin(s) for an analog action within an action.
Array Steam::getDigitalActionOrigins(uint64_t inputHandle, uint64_t actionSetHandle, uint64_t digitalActionHandle){
	Array list;
	if(SteamInput() != NULL){
		EInputActionOrigin *out = new EInputActionOrigin[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamInput()->GetDigitalActionOrigins((InputHandle_t)inputHandle, (ControllerActionSetHandle_t)actionSetHandle, (ControllerDigitalActionHandle_t)digitalActionHandle, out);
		for (int i=0; i<ret; i++){
			list.push_back((int)out[i]);
		}
	}
	return list;
}
// Returns the associated gamepad index for the specified controller.
int Steam::getGamepadIndexForController(uint64_t inputHandle){
	if(SteamInput() != NULL){
		return SteamInput()->GetGamepadIndexForController((InputHandle_t)inputHandle);
	}
	return -1;
}
// Get a local path to art for on-screen glyph for a particular origin.
String Steam::getGlyphForActionOrigin(int origin){
	if(SteamInput() != NULL){
		return "";
	}
	return SteamInput()->GetGlyphForActionOrigin((EInputActionOrigin)origin);
}
// Get the input type (device model) for the specified controller. 
String Steam::getInputTypeForHandle(uint64_t inputHandle){
	if(SteamInput() == NULL){
		return "";
	}
	ESteamInputType inputType = SteamInput()->GetInputTypeForHandle((InputHandle_t)inputHandle);
	if(inputType == k_ESteamInputType_SteamController){
		return "Steam controller";
	}
	else if(inputType == k_ESteamInputType_XBox360Controller){
		return "XBox 360 controller";
	}
	else if(inputType == k_ESteamInputType_XBoxOneController){
		return "XBox One controller";
	}
	else if(inputType == k_ESteamInputType_GenericGamepad){
		return "Generic XInput";
	}
	else if(inputType == k_ESteamInputType_PS4Controller){
		return "PS4 controller";
	}
	else{
		return "Unknown";
	}
}
// Returns raw motion data for the specified controller.
Dictionary Steam::getMotionData(uint64_t inputHandle){
	ControllerMotionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamInput() != NULL){
		data = SteamInput()->GetMotionData((InputHandle_t)inputHandle);
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
// Get the Steam Remote Play session ID associated with a device, or 0 if there is no session associated with it. See isteamremoteplay.h for more information on Steam Remote Play sessions.
int Steam::getRemotePlaySessionID(uint64_t inputHandle){
	if(SteamInput() == NULL){
		return 0;
	}
	return SteamInput()->GetRemotePlaySessionID((InputHandle_t)inputHandle);
}
// Returns a localized string (from Steam's language setting) for the specified origin.
String Steam::getStringForActionOrigin(int origin){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetStringForActionOrigin((EInputActionOrigin)origin);
}
// Start SteamInputs interface.
bool Steam::inputInit(){
	if(SteamInput() != NULL){
		return SteamInput()->Init();
	}
	return false;
}
// Stop SteamInputs interface.
bool Steam::inputShutdown(){
	if(SteamInput() != NULL){
		return SteamInput()->Shutdown();
	}
	return false;
}
// Set the controller LED color on supported controllers.
void Steam::setLEDColor(uint64_t inputHandle, int colorR, int colorG, int colorB, int flags){
	if(SteamInput() != NULL){
		SteamInput()->SetLEDColor((InputHandle_t)inputHandle, colorR, colorG, colorB, flags);
	}
}
// Syncronize controllers.
void Steam::runFrame(){
	if(SteamInput() != NULL){
		SteamInput()->RunFrame();
	}
}
// Invokes the Steam overlay and brings up the binding screen.
bool Steam::showBindingPanel(uint64_t inputHandle){
	if(SteamInput() != NULL){
		return SteamInput()->ShowBindingPanel((InputHandle_t)inputHandle);
	}
	return false;
}
// Stops the momentum of an analog action (where applicable, ie a touchpad w/ virtual trackball settings).
void Steam::stopAnalogActionMomentum(uint64_t inputHandle, uint64_t action){
	if(SteamInput() != NULL){
		SteamInput()->StopAnalogActionMomentum((InputHandle_t)inputHandle, (InputAnalogActionHandle_t)action);
	}
}
// Get the equivalent origin for a given controller type or the closest controller type that existed in the SDK you built into your game if eDestinationInputType is k_ESteamInputType_Unknown. This action origin can be used in your glyph look up table or passed into GetGlyphForActionOrigin or GetStringForActionOrigin.
int Steam::translateActionOrigin(int destinationInput, int sourceOrigin){
	if(SteamInput() == NULL){
		return 0;
	}
	return SteamInput()->TranslateActionOrigin((ESteamInputType)destinationInput, (EInputActionOrigin)sourceOrigin);
}

// Triggers a (low-level) haptic pulse on supported controllers.
void Steam::triggerHapticPulse(uint64_t inputHandle, int targetPad, int duration){
	if(SteamInput() != NULL){
		SteamInput()->TriggerHapticPulse((InputHandle_t)inputHandle, (ESteamControllerPad)targetPad, duration);
	}
}
// Triggers a repeated haptic pulse on supported controllers.
//void Steam::triggerRepeatedHapticPulse(uint64_t inputHandle, int targetPad, int duration, int offset, int repeat, int flags){
//	if(SteamInput() != NULL){
//		SteamInput()->TriggerRepeatedHapticPulse((InputHandle_t)inputHandle, (ESteamControllerPad)targetPad, duration, offset, repeat, flags);
//	}
//}
// Trigger a vibration event on supported controllers.
void Steam::triggerVibration(uint64_t inputHandle, uint16_t leftSpeed, uint16_t rightSpeed){
	if(SteamInput() != NULL){
		SteamInput()->TriggerVibration((InputHandle_t)inputHandle, (unsigned short)leftSpeed, (unsigned short)rightSpeed);
	}
}

/////////////////////////////////////////////////
///// INVENTORY /////////////////////////////////
/////////////////////////////////////////////////
//
// When dealing with any inventory handles, you should call CheckResultSteamID on the result handle when it completes to verify that a remote player is not pretending to have a different user's inventory.
// Also, you must call DestroyResult on the provided inventory result when you are done with it.
//
// Grant a specific one-time promotional item to the current user.
bool Steam::addPromoItem(uint32 item){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->AddPromoItem(&inventoryHandle, item);
}
// Grant a specific one-time promotional items to the current user.
bool Steam::addPromoItems(const PoolIntArray items){
	if(SteamInventory() == NULL){
		return false;
	}
	int count = items.size();
	SteamItemDef_t *newItems = new SteamItemDef_t[items.size()];
	for(int i = 0; i < count; i++){
		newItems[i] = items[i];
	}
	return SteamInventory()->AddPromoItems(&inventoryHandle, newItems, count);
}
// Checks whether an inventory result handle belongs to the specified Steam ID.
bool Steam::checkResultSteamID(uint64_t steamIDExpected){
	if(SteamInventory() == NULL){
		return false;
	}
	CSteamID steamID = (uint64)steamIDExpected;
	return SteamInventory()->CheckResultSteamID((SteamInventoryResult_t)inventoryHandle, steamID);
}
// Consumes items from a user's inventory. If the quantity of the given item goes to zero, it is permanently removed.
bool Steam::consumeItem(uint64_t itemConsume, uint32 quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->ConsumeItem(&inventoryHandle, (SteamItemInstanceID_t)itemConsume, quantity);
}
// Deserializes a result set and verifies the signature bytes.
bool Steam::deserializeResult(){
	if(SteamInventory() == NULL){
		return false;
	}
	const int buffer = 1024;
	return SteamInventory()->DeserializeResult(&inventoryHandle, &buffer, 1024, false);
}
// Destroys a result handle and frees all associated memory.
void Steam::destroyResult(){
	if(SteamInventory() != NULL){
		SteamInventory()->DestroyResult(inventoryHandle);
	}
}
// Grant one item in exchange for a set of other items.
bool Steam::exchangeItems(const PoolIntArray outputItems, const uint32 outputQuantity, const uint64_t inputItems, const uint32 inputQuantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->ExchangeItems(&inventoryHandle, outputItems.read().ptr(), &outputQuantity, 1, (const uint64 *)inputItems, &inputQuantity, 1);
}
// Grants specific items to the current user, for developers only.
bool Steam::generateItems(const PoolIntArray items, const uint32 quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GenerateItems(&inventoryHandle, items.read().ptr(), &quantity, items.size());
}
// Start retrieving all items in the current users inventory.
bool Steam::getAllItems(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GetAllItems(&inventoryHandle);
}
// Gets a string property from the specified item definition.  Gets a property value for a specific item definition.
String Steam::getItemDefinitionProperty(uint32 definition, const String& name){
	if(SteamInventory() == NULL){
		return "";
	}
	uint32 bufferSize = 256;
	char *value = new char[bufferSize];
	SteamInventory()->GetItemDefinitionProperty(definition, name.utf8().get_data(), value, &bufferSize);
	String property = value;
	delete value;
	return property;
}
// Gets the state of a subset of the current user's inventory.
bool Steam::getItemsByID(const uint64_t idArray, uint32 count){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GetItemsByID(&inventoryHandle, (const uint64 *)idArray, count);
}
// After a successful call to RequestPrices, you can call this method to get the pricing for a specific item definition.
uint64_t Steam::getItemPrice(uint32 definition){
	if(SteamInventory() == NULL){
		return 0;
	}
	uint64 price = 0;
	uint64 basePrice = 0;
	SteamInventory()->GetItemPrice(definition, &price, &basePrice);
	return price;
}
// After a successful call to RequestPrices, you can call this method to get all the pricing for applicable item definitions. Use the result of GetNumItemsWithPrices as the the size of the arrays that you pass in.
Array Steam::getItemsWithPrices(uint32 length){
	if(SteamInventory() == NULL){
		return Array();
	}
	// Create the return array
	Array priceArray;
	// Create a temporary array
	SteamItemDef_t *ids = new SteamItemDef_t[length];
	uint64 *prices = new uint64[length];
	uint64 *basePrices = new uint64[length];
	if(SteamInventory()->GetItemsWithPrices(ids, prices, basePrices, length)){
		for(uint32 i = 0; i < length; i++){
			Dictionary priceGroup;
			priceGroup["item"] = ids[i];
			priceGroup["price"] = (uint64_t)prices[i];
			priceGroup["base_prices"] = (uint64_t)basePrices[i];
			priceArray.append(priceGroup);
		}
	}
	return priceArray;
}
// After a successful call to RequestPrices, this will return the number of item definitions with valid pricing.
uint32 Steam::getNumItemsWithPrices(){
	if(SteamInventory() == NULL){
		return 0;
	}
	return SteamInventory()->GetNumItemsWithPrices();
}
// Gets the dynamic properties from an item in an inventory result set.
String Steam::getResultItemProperty(uint32 index, const String& name){
	if(SteamInventory() != NULL){
		// Set up variables to fill
		uint32 bufferSize = 256;
		char *value = new char[bufferSize];
		SteamInventory()->GetResultItemProperty(inventoryHandle, index, name.utf8().get_data(), (char*)value, &bufferSize);
		String property = value;
		delete value;
		return property;
	}
	return "";
}
// Get the items associated with an inventory result handle.
Array Steam::getResultItems(){
	if(SteamInventory() == NULL){
		return Array();
	}
	// Set up return array
	Array items;
	uint32 size = 0;
	if(SteamInventory()->GetResultItems(inventoryHandle, NULL, &size)){
		itemDetails.resize(size);
		std::vector<SteamItemDetails_t> itemArray;
		if(SteamInventory()->GetResultItems(inventoryHandle, itemArray.data(), &size)){
			for(uint32 i = 0; i < size; i++){
				items.push_back((uint64_t)itemArray[i].m_itemId);
			}
		}
	}
	return items;
}
// Find out the status of an asynchronous inventory result handle.
String Steam::getResultStatus(){
	if(SteamInventory() == NULL){
		return "";
	}
	int result = SteamInventory()->GetResultStatus(inventoryHandle);
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
uint32 Steam::getResultTimestamp(){
	if(SteamInventory() == NULL){
		return 0;
	}
	return SteamInventory()->GetResultTimestamp(inventoryHandle);
}
// Grant all potential one-time promotional items to the current user.
bool Steam::grantPromoItems(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GrantPromoItems(&inventoryHandle);
}
// Triggers an asynchronous load and refresh of item definitions.
bool Steam::loadItemDefinitions(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->LoadItemDefinitions();
}
// Request the list of "eligible" promo items that can be manually granted to the given user.
void Steam::requestEligiblePromoItemDefinitionsIDs(uint64_t steamID){
	if(SteamInventory() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamAPICall_t apiCall = SteamInventory()->RequestEligiblePromoItemDefinitionsIDs(userID);
		callResultEligiblePromoItemDefIDs.Set(apiCall, this, &Steam::_inventory_eligible_promo_item);
	}
}
// Request prices for all item definitions that can be purchased in the user's local currency. A SteamInventoryRequestPricesResult_t call result will be returned with the user's local currency code. After that, you can call GetNumItemsWithPrices and GetItemsWithPrices to get prices for all the known item definitions, or GetItemPrice for a specific item definition.
void Steam::requestPrices(){
	if(SteamInventory() != NULL){
		SteamAPICall_t apiCall = SteamInventory()->RequestPrices();
		callResultRequestPrices.Set(apiCall, this, &Steam::_inventory_request_prices_result);
	}
}
// 
bool Steam::serializeResult(){
	if(SteamInventory() == NULL){
		return false;
	}
	// Set up return array
	static uint32 bufferSize = 0;
	if(SteamInventory()->SerializeResult(inventoryHandle, NULL, &bufferSize)){
		char *buffer = new char[bufferSize];
		return SteamInventory()->SerializeResult(inventoryHandle, &buffer, &bufferSize);
	}
	return false;
}
// Starts the purchase process for the user, given a "shopping cart" of item definitions that the user would like to buy. The user will be prompted in the Steam Overlay to complete the purchase in their local currency, funding their Steam Wallet if necessary, etc.
void Steam::startPurchase(const PoolIntArray items, const uint32 quantity){
	if(SteamInventory() != NULL){
		SteamAPICall_t apiCall = SteamInventory()->StartPurchase(items.read().ptr(), &quantity, items.size());
		callResultStartPurchase.Set(apiCall, this, &Steam::_inventory_start_purchase_result);
	}
}
// Transfer items between stacks within a user's inventory.
bool Steam::transferItemQuantity(uint64_t itemID, uint32 quantity, uint64_t itemDestination, bool split){
	if(SteamInventory() == NULL){
		return false;
	}
	if(split){
		return SteamInventory()->TransferItemQuantity(&inventoryHandle, (SteamItemInstanceID_t)itemID, quantity, k_SteamItemInstanceIDInvalid); 
	}
	else{
		return SteamInventory()->TransferItemQuantity(&inventoryHandle, (SteamItemInstanceID_t)itemID, quantity, (SteamItemInstanceID_t)itemDestination);
	}
}
// Trigger an item drop if the user has played a long enough period of time.
bool Steam::triggerItemDrop(uint32 definition){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->TriggerItemDrop(&inventoryHandle, (SteamItemDef_t)definition);
}
// Starts a transaction request to update dynamic properties on items for the current user. This call is rate-limited by user, so property modifications should be batched as much as possible (e.g. at the end of a map or game session). After calling SetProperty or RemoveProperty for all the items that you want to modify, you will need to call SubmitUpdateProperties to send the request to the Steam servers. A SteamInventoryResultReady_t callback will be fired with the results of the operation.
void Steam::startUpdateProperties(){
	if(SteamInventory() != NULL){
		inventoryUpdateHandle = SteamInventory()->StartUpdateProperties();
	}
}
//Submits the transaction request to modify dynamic properties on items for the current user. See StartUpdateProperties.
bool Steam::submitUpdateProperties(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SubmitUpdateProperties(inventoryUpdateHandle, &inventoryHandle);
}
// Removes a dynamic property for the given item.
bool Steam::removeProperty(uint64_t itemID, const String& name){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->RemoveProperty(inventoryUpdateHandle, (SteamItemInstanceID_t)itemID, name.utf8().get_data());
}
// Sets a dynamic property for the given item. Supported value types are strings.
bool Steam::setPropertyString(uint64_t itemID, const String& name, const String& value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventoryUpdateHandle, (SteamItemInstanceID_t)itemID, name.utf8().get_data(), value.utf8().get_data());
}
// Sets a dynamic property for the given item. Supported value types are boolean.
bool Steam::setPropertyBool(uint64_t itemID, const String& name, bool value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventoryUpdateHandle, (SteamItemInstanceID_t)itemID, name.utf8().get_data(), value);
}
// Sets a dynamic property for the given item. Supported value types are 64 bit integers.
bool Steam::setPropertyInt(uint64_t itemID, const String& name, uint64_t value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventoryUpdateHandle, (SteamItemInstanceID_t)itemID, name.utf8().get_data(), (int64)value);
}
// Sets a dynamic property for the given item. Supported value types are 32 bit floats.
bool Steam::setPropertyFloat(uint64_t itemID, const String& name, float value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventoryUpdateHandle, (SteamItemInstanceID_t)itemID, name.utf8().get_data(), value);
}

/////////////////////////////////////////////////
///// MATCHMAKING ///////////////////////////////
/////////////////////////////////////////////////
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
			sprintf(favoriteIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
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
int Steam::addFavoriteGame(uint32 ip, uint16 port, uint16 queryPort, uint32 flags, uint32 lastPlayed){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	return SteamMatchmaking()->AddFavoriteGame(currentAppID, ip, port, queryPort, flags, lastPlayed);
}
// Removes the game server from the local storage; returns true if one was removed.
bool Steam::removeFavoriteGame(AppId_t appID, uint32 ip, uint16 port, uint16 queryPort, uint32 flags){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	return SteamMatchmaking()->RemoveFavoriteGame(appID, ip, port, queryPort, flags);
}
// Get a list of relevant lobbies.
void Steam::requestLobbyList(){
	if(SteamMatchmaking() != NULL){
		SteamAPICall_t apiCall = SteamMatchmaking()->RequestLobbyList();
		callResultLobbyList.Set(apiCall, this, &Steam::_lobby_match_list);
	}
}
// Adds a string comparison filter to the next RequestLobbyList call.
void Steam::addRequestLobbyListStringFilter(const String& keyToMatch, const String& valueToMatch, int comparisonType){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListStringFilter(keyToMatch.utf8().get_data(), valueToMatch.utf8().get_data(), (ELobbyComparison)comparisonType);
	}
}
// Adds a numerical comparison filter to the next RequestLobbyList call.
void Steam::addRequestLobbyListNumericalFilter(const String& keyToMatch, int valueToMatch, int comparisonType){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListNumericalFilter(keyToMatch.utf8().get_data(), valueToMatch, (ELobbyComparison)comparisonType);
	}
}
// Returns results closest to the specified value. Multiple near filters can be added, with early filters taking precedence.
void Steam::addRequestLobbyListNearValueFilter(const String& keyToMatch, int valueToBeCloseTo){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListNearValueFilter(keyToMatch.utf8().get_data(), valueToBeCloseTo);
	}
}
// Returns only lobbies with the specified number of slots available.
void Steam::addRequestLobbyListFilterSlotsAvailable(int slotsAvailable){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(slotsAvailable);
	}
}
// Sets the distance for which we should search for lobbies (based on users IP address to location map on the Steam backed).
void Steam::addRequestLobbyListDistanceFilter(int distanceFilter){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListDistanceFilter((ELobbyDistanceFilter)distanceFilter);
	}
}
// Sets how many results to return, the lower the count the faster it is to download the lobby results & details to the client.
void Steam::addRequestLobbyListResultCountFilter(int maxResults){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListResultCountFilter(maxResults);
	}
}
// Create a lobby on the Steam servers, if private the lobby will not be returned by any RequestLobbyList() call.
void Steam::createLobby(int lobbyType, int maxMembers){
	if(SteamMatchmaking() != NULL){
		SteamAPICall_t apiCall = SteamMatchmaking()->CreateLobby((ELobbyType)lobbyType, maxMembers);
		callResultCreateLobby.Set(apiCall, this, &Steam::_lobby_created);
	}
}
// Join an existing lobby.
void Steam::joinLobby(uint64_t steamIDLobby){
	if(SteamMatchmaking() != NULL){
		CSteamID lobbyID = (uint64)steamIDLobby;
		SteamMatchmaking()->JoinLobby(lobbyID);
	}
}
// Leave a lobby, this will take effect immediately on the client side, other users will be notified by LobbyChatUpdate_t callback.
void Steam::leaveLobby(uint64_t steamIDLobby){
	if(SteamMatchmaking() != NULL){
		CSteamID lobbyID = (uint64)steamIDLobby;
		SteamMatchmaking()->LeaveLobby(lobbyID);
	}
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
		bool success = SteamMatchmaking()->GetLobbyDataByIndex(lobbyID, i, &key, MAX_LOBBY_KEY_LENGTH, &value, CHAT_METADATA_MAX);
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
	if(SteamMatchmaking() != NULL){
		CSteamID lobbyID = (uint64)steamIDLobby;
		SteamMatchmaking()->SetLobbyMemberData(lobbyID, key.utf8().get_data(), value.utf8().get_data());
	}
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
	if(SteamMatchmaking() != NULL){
		// Resolve address and convert it from IP_Address string to uint32_t
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
		sprintf(ip, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
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
bool Steam::setLobbyType(uint64_t steamIDLobby, int lobbyType){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobbyID = (uint64)steamIDLobby;
	return SteamMatchmaking()->SetLobbyType(lobbyID, (ELobbyType)lobbyType);
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
/////////////////////////////////////////////////
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
	if(SteamMusic() != NULL){
		SteamMusic()->Pause();
	}
}
// Play current track/album.
void Steam::musicPlay(){
	if(SteamMusic() != NULL){
		SteamMusic()->Play();
	}
}
// Play next track/album.
void Steam::musicPlayNext(){
	if(SteamMusic() != NULL){
		SteamMusic()->PlayNext();
	}
}
// Play previous track/album.
void Steam::musicPlayPrev(){
	if(SteamMusic() != NULL){
		SteamMusic()->PlayPrevious();
	}
}
// Set the volume of Steam music.
void Steam::musicSetVolume(float value){
	if(SteamMusic() != NULL){
		SteamMusic()->SetVolume(value);
	}
}

/////////////////////////////////////////////////
///// MUSIC REMOTE //////////////////////////////
/////////////////////////////////////////////////
//
// These functions do not have any offical notes or comments.  All descriptions are assumed.
//
// If remote access was successfully activated.
bool Steam::activationSuccess(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->BActivationSuccess(value);
}
// Is a remote music client / host connected?
bool Steam::isCurrentMusicRemote(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->BIsCurrentMusicRemote();
}
// Did the currenty music entry just change?
bool Steam::currentEntryDidChange(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->CurrentEntryDidChange();
}
// Is the current music entry available?
bool Steam::currentEntryIsAvailable(bool available){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->CurrentEntryIsAvailable(available);
}
// Will the current music entry change?
bool Steam::currentEntryWillChange(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->CurrentEntryWillChange();
}
// Disconnect from remote music client / host.
bool Steam::deregisterSteamMusicRemote(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->DeregisterSteamMusicRemote();
}
// Enable track loop on client.
bool Steam::enableLooped(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->EnableLooped(value);
}
// Enable playlists on client.
bool Steam::enablePlaylists(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->EnablePlaylists(value);
}
// Play the next track on client.
bool Steam::enablePlayNext(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->EnablePlayNext(value);
}
// Play previous track on client.
bool Steam::enablePlayPrevious(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->EnablePlayPrevious(value);
}
// Enable the music queue on the client.
bool Steam::enableQueue(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->EnableQueue(value);
}
// Enable shuffle on the client.
bool Steam::enableShuffled(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->EnableShuffled(value);
}
// Has the playlist changed?
bool Steam::playlistDidChange(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->PlaylistDidChange();
}
// Will the playlist change?
bool Steam::playlistWillChange(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->PlaylistWillChange();
}
// Did the song queue change?
bool Steam::queueDidChange(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->QueueDidChange();
}
// Will the song queue change?
bool Steam::queueWillChange(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->QueueWillChange();
}
// Connect to a music remote client / host?
bool Steam::registerSteamMusicRemote(const String& name){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->RegisterSteamMusicRemote(name.utf8().get_data());
}
// Reset the playlist entries.
bool Steam::resetPlaylistEntries(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->ResetPlaylistEntries();
}
// Reset the song queue entries.
bool Steam::resetQueueEntries(){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->ResetQueueEntries();
}
// Set a new current playlist.
bool Steam::setCurrentPlaylistEntry(int id){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->SetCurrentPlaylistEntry(id);
}
// Set a new current song queue.
bool Steam::setCurrentQueueEntry(int id){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->SetCurrentQueueEntry(id);
}
// Set a new display name.
bool Steam::setDisplayName(const String& name){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->SetDisplayName(name.utf8().get_data());
}
// Set a new playlist entry.
bool Steam::setPlaylistEntry(int id, int position, const String& entryText){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->SetPlaylistEntry(id, position, entryText.utf8().get_data());
}
// Set a PNG icon for a song? A playlist?
//bool Steam::setPNGIcon64x64(const PoolByteArray& icon){
//	if(SteamMusicRemote() == NULL){
//		return false;
//	}
//	return SteamMusicRemote()->SetPNGIcon_64x64(icon.read().ptr(), icon.size());
//}
// Set a new queue entry.
bool Steam::setQueueEntry(int id, int position, const String& entryText){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->SetQueueEntry(id, position, entryText.utf8().get_data());
}
// Update the current song entry's cover art.
//bool Steam::updateCurrentEntryCoverArt(const PoolByteArray& art){
//	if(SteamMusicRemote() == NULL){
//		return false;
//	}
//	return SteamMusicRemote()->UpdateCurrentEntryCoverArt(art.read().ptr(), art.size());
//}
// Update the current seconds that have elapsed for an entry.
bool Steam::updateCurrentEntryElapsedSeconds(int value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->UpdateCurrentEntryElapsedSeconds(value);
}
// Update the current song entry's text?
bool Steam::updateCurrentEntryText(const String& text){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->UpdateCurrentEntryText(text.utf8().get_data());
}
// Update looped or not.
bool Steam::updateLooped(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->UpdateLooped(value);
}
// Update the current playback status; 0 - undefined, 1 - playing, 2 - paused, 3 - idle.
bool Steam::updatePlaybackStatus(int status){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->UpdatePlaybackStatus((AudioPlayback_Status)status);
}
// Update whether to shuffle or not.
bool Steam::updateShuffled(bool value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->UpdateShuffled(value);
}
// Volume is between 0.0 and 1.0.
bool Steam::updateVolume(float value){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->UpdateVolume(value);
}

/////////////////////////////////////////////////
///// NETWORKING ////////////////////////////////
/////////////////////////////////////////////////
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
///// PARTIES ///////////////////////////////////
/////////////////////////////////////////////////
//
// Get the list of locations in which you can post a party beacon.
Array Steam::getAvailableBeaconLocations(uint32 max){
	if(SteamParties() == NULL){
		return Array();
	}
	Array beaconLocations;
	uint32 locations = 0;
	SteamPartyBeaconLocation_t *beacons = new SteamPartyBeaconLocation_t[256];
	if(SteamParties()->GetNumAvailableBeaconLocations(&locations)){
		// If max is lower than locations, set it to locations
		if(max < locations){
			max = locations;
		}
		// Now get the beacon location list
		if(SteamParties()->GetAvailableBeaconLocations(beacons, max)){
			for(uint32 i = 0; i < max; i++){
				Dictionary beaconData;
				beaconData["type"] = beacons[i].m_eType;
				beaconData["locationID"] = (uint64_t)beacons[i].m_ulLocationID;
				beaconLocations.append(beaconData);
			}
		}
	}
	return beaconLocations;
}
// Create a beacon. You can only create one beacon at a time. Steam will display the beacon in the specified location, and let up to unOpenSlots users "follow" the beacon to your party.
void Steam::createBeacon(uint32 openSlots, uint64_t location, int type, const String& connectString, const String& metadata){
	if(SteamParties() != NULL){
		// Add data to the beacon location struct
		SteamPartyBeaconLocation_t *beaconData = new SteamPartyBeaconLocation_t;
		beaconData->m_eType = (ESteamPartyBeaconLocationType)type;
		beaconData->m_ulLocationID = location;
		SteamParties()->CreateBeacon(openSlots, beaconData, connectString.utf8().get_data(), metadata.utf8().get_data());
	}
}
// When a user follows your beacon, Steam will reserve one of the open party slots for them, and send your game a ReservationNotificationCallback_t callback. When that user joins your party, call OnReservationCompleted to notify Steam that the user has joined successfully.
void Steam::onReservationCompleted(uint64_t beacon, uint64_t steamID){
	if(SteamParties() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamParties()->OnReservationCompleted(beacon, userID);
	}
}
// If a user joins your party through other matchmaking (perhaps a direct Steam friend, or your own matchmaking system), your game should reduce the number of open slots that Steam is managing through the party beacon. For example, if you created a beacon with five slots, and Steam sent you two ReservationNotificationCallback_t callbacks, and then a third user joined directly, you would want to call ChangeNumOpenSlots with a value of 2 for unOpenSlots. That value represents the total number of new users that you would like Steam to send to your party.
void Steam::changeNumOpenSlots(uint64_t beacon, uint32 openSlots){
	if(SteamParties() != NULL){
		SteamParties()->ChangeNumOpenSlots(beacon, openSlots);
	}
}
// Call this method to destroy the Steam party beacon. This will immediately cause Steam to stop showing the beacon in the target location. Note that any users currently in-flight may still arrive at your party expecting to join.
bool Steam::destroyBeacon(uint64_t beacon){
	if(SteamParties() == NULL){
		return false;
	}
	return SteamParties()->DestroyBeacon(beacon);
}
// Get the number of active party beacons created by other users for your game, that are visible to the current user.
uint32 Steam::getNumActiveBeacons(){
	if(SteamParties() == NULL){
		return 0;
	}
	return SteamParties()->GetNumActiveBeacons();
}
// Use with ISteamParties::GetNumActiveBeacons to iterate the active beacons visible to the current user. unIndex is a zero-based index, so iterate over the range [0, GetNumActiveBeacons() - 1]. The return is a PartyBeaconID_t that can be used with ISteamParties::GetBeaconDetails to get information about the beacons suitable for display to the user.
uint64_t Steam::getBeaconByIndex(uint32 index){
	if(SteamParties() == NULL){
		return 0;
	}
	return SteamParties()->GetBeaconByIndex(index);
}
// Get details about the specified beacon. You can use the ISteamFriends API to get further details about pSteamIDBeaconOwner, and ISteamParties::GetBeaconLocationData to get further details about pLocation. The pchMetadata contents are specific to your game, and will be whatever was set (if anything) by the game process that created the beacon.
Dictionary Steam::getBeaconDetails(uint64_t beacon){
	Dictionary details;
	if(SteamParties() != NULL){
		CSteamID owner;
		SteamPartyBeaconLocation_t location;
		char metadata;
		if(SteamParties()->GetBeaconDetails(beacon, &owner, &location, &metadata, 2048)){
			details["id"] = beacon;
			details["owner"] = (uint64_t)owner.ConvertToUint64();
			details["type"] = location.m_eType;
			details["location"] = (uint64_t)location.m_ulLocationID;
			details["metadata"] = metadata;
		}
	}
	return details;
}
// When the user indicates they wish to join the party advertised by a given beacon, call this method. On success, Steam will reserve a slot for this user in the party and return the necessary "join game" string to use to complete the connection.
void Steam::joinParty(uint64_t beacon){
	if(SteamParties() != NULL){
		SteamParties()->JoinParty(beacon);
	}
}
// Query general metadata for the given beacon location. For instance the Name, or the URL for an icon if the location type supports icons (for example, the icon for a Steam Chat Room Group).
String Steam::getBeaconLocationData(uint64_t locationID, int locationType, int locationData){
	if(SteamParties() == NULL){
		return "";
	}
	char *beaconData = new char[2048];
	// Add data to SteamPartyBeaconLocation struct
	SteamPartyBeaconLocation_t *beacon = new SteamPartyBeaconLocation_t;
	beacon->m_eType = (ESteamPartyBeaconLocationType)locationType;
	beacon->m_ulLocationID = locationID;
	if(SteamParties()->GetBeaconLocationData(*beacon, (ESteamPartyBeaconLocationData)locationData, (char*)beaconData, 2048)){
		String location = beaconData;
		return location;
	}
	else{
		return "";
	}
}

/////////////////////////////////////////////////
///// REMOTE PLAY ///////////////////////////////
/////////////////////////////////////////////////
//
// Get the number of currently connected Steam Remote Play sessions.
uint32 Steam::getSessionCount(){
	if(SteamRemotePlay() == NULL){
		return 0;
	}
	return SteamRemotePlay()->GetSessionCount();
}
// Get the currently connected Steam Remote Play session ID at the specified index.
uint32 Steam::getSessionID(int index){
	if(SteamRemotePlay() == NULL){
		return 0;
	}
	return SteamRemotePlay()->GetSessionID(index);
}
// Get the SteamID of the connected user.
uint64_t Steam::getSessionSteamID(uint32 sessionID){
	if(SteamRemotePlay() == NULL){
		return 0;
	}
	CSteamID steamID = SteamRemotePlay()->GetSessionSteamID(sessionID);
	return steamID.ConvertToUint64();
}
// Get the name of the session client device.
String Steam::getSessionClientName(uint32 sessionID){
	if(SteamRemotePlay() == NULL){
		return "";
	}
	return SteamRemotePlay()->GetSessionClientName(sessionID);
}
// Get the form factor of the session client device.
int Steam::getSessionClientFormFactor(uint32 sessionID){
	if(SteamRemotePlay() == NULL){
		return 0;
	}
	return SteamRemotePlay()->GetSessionClientFormFactor(sessionID);	
}
// Get the resolution, in pixels, of the session client device. This is set to 0x0 if the resolution is not available.
Dictionary Steam::getSessionClientResolution(uint32 sessionID){
	Dictionary resolution;
	if (SteamNetworking() == NULL) {
		return resolution;
	}
	int resolutionX = 0;
	int resolutionY = 0;
	bool success = SteamRemotePlay()->BGetSessionClientResolution(sessionID, &resolutionX, &resolutionY);
	if(success){
		resolution["success"] = success;
		resolution["x"] = resolutionX;
		resolution["y"] = resolutionY;
	}	
	return resolution;
}
// Invite a friend to join the game using Remote Play Together
bool Steam::sendRemotePlayTogetherInvite(uint64_t friendID){
	if(SteamRemotePlay() == NULL){
		return false;
	}
	CSteamID steamID = (uint64)friendID;
	return SteamRemotePlay()->BSendRemotePlayTogetherInvite(steamID);
}

/////////////////////////////////////////////////
///// REMOTE STORAGE ////////////////////////////
/////////////////////////////////////////////////
//
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
// Delete file from remote storage but leave it on local disk to remain accessible.
bool Steam::fileForget(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileForget(file.utf8().get_data());
}
// Check if a given file is persisted in Steam Cloud.
bool Steam::filePersisted(const String& file){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FilePersisted(file.utf8().get_data());
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
// Starts an asynchronous read from a file. The offset and amount to read should be valid for the size of the file, as indicated by GetFileSize or GetFileTimestamp.
void Steam::fileReadAsync(const String& file, uint32 offset, uint32_t dataToRead){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t apiCall = SteamRemoteStorage()->FileReadAsync(file.utf8().get_data(), offset, dataToRead);
		callResultFileReadAsyncComplete.Set(apiCall, this, &Steam::_file_read_async_complete);
	}
}
// Share a file.
void Steam::fileShare(const String& file){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t apiCall = SteamRemoteStorage()->FileShare(file.utf8().get_data());
		callResultFileShareResult.Set(apiCall, this, &Steam::_file_share_result);
	}
}
// Write to given file from Steam Cloud.
bool Steam::fileWrite(const String& file, const PoolByteArray& data, int32_t dataSize){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWrite(file.utf8().get_data(), data.read().ptr(), dataSize);
}
// Creates a new file and asynchronously writes the raw byte data to the Steam Cloud, and then closes the file. If the target file already exists, it is overwritten.
void Steam::fileWriteAsync(const String& file, const PoolByteArray& data, int32_t dataSize){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t apiCall = SteamRemoteStorage()->FileWriteAsync(file.utf8().get_data(), data.read().ptr(), dataSize);
		callResultFileWriteAsyncComplete.Set(apiCall, this, &Steam::_file_write_async_complete);
	}
}
// Cancels a file write stream that was started by FileWriteStreamOpen.  This trashes all of the data written and closes the write stream, but if there was an existing file with this name, it remains untouched.
bool Steam::fileWriteStreamCancel(uint64_t writeHandle){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWriteStreamCancel((UGCFileWriteStreamHandle_t)writeHandle);
}
// Closes a file write stream that was started by FileWriteStreamOpen. This flushes the stream to the disk, overwriting the existing file if there was one.
bool Steam::fileWriteStreamClose(uint64_t writeHandle){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWriteStreamClose((UGCFileWriteStreamHandle_t)writeHandle);
}
// Creates a new file output stream allowing you to stream out data to the Steam Cloud file in chunks. If the target file already exists, it is not overwritten until FileWriteStreamClose has been called. To write data out to this stream you can use FileWriteStreamWriteChunk, and then to close or cancel you use FileWriteStreamClose and FileWriteStreamCancel respectively.
uint64_t Steam::fileWriteStreamOpen(const String& file){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->FileWriteStreamOpen(file.utf8().get_data());
}
// Writes a blob of data to the file write stream.
bool fileWriteStreamWriteChunk(uint64_t writeHandle, PoolByteArray& data, int32_t dataSize){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWriteStreamWriteChunk((UGCFileWriteStreamHandle_t)writeHandle, data.read().ptr(), dataSize);
}
// Gets the number of cached UGC.
int32 Steam::getCachedUGCCount(){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->GetCachedUGCCount();
}
// Gets the cached UGC's handle.
uint64_t getCachedUGCHandle(int content){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->GetCachedUGCHandle(content);
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
	if(SteamRemoteStorage() != NULL){
		SteamRemoteStorage()->SetCloudEnabledForApp(enabled);
	}
}




/////////////////////////////////////////////////
///// SCREENSHOTS ///////////////////////////////
/////////////////////////////////////////////////
//
// Adds a screenshot to the user's Steam screenshot library from disk.
uint32_t Steam::addScreenshotToLibrary(const String& filename, const String& thumbnailFilename, int width, int height){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->AddScreenshotToLibrary(filename.utf8().get_data(), thumbnailFilename.utf8().get_data(), width, height);
}
// Adds a VR screenshot to the user's Steam screenshot library from disk in the supported type.
uint32_t Steam::addVRScreenshotToLibrary(int type, const String& filename, const String& vrFilename){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->AddVRScreenshotToLibrary((EVRScreenshotType)type, filename.utf8().get_data(), vrFilename.utf8().get_data());
}
// Toggles whether the overlay handles screenshots.
void Steam::hookScreenshots(bool hook){
	if(SteamScreenshots() != NULL){
		SteamScreenshots()->HookScreenshots(hook);
	}
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
//
bool Steam::tagPublishedFile(uint32 screenshot, uint64_t fileID){
	if(SteamScreenshots() == NULL){
		return false;
	}
	PublishedFileId_t file = (uint64)fileID;
	return SteamScreenshots()->TagPublishedFile((ScreenshotHandle)screenshot, file);
}
// Tags a Steam user as being visible in the screenshot.  You can tag up to the value declared by k_nScreenshotMaxTaggedUsers in a single screenshot. Tagging more users than that will just be discarded.  This function has a built in delay before saving the tag which allows you to call it repeatedly for each item.  You can get the handle to tag the screenshot once it has been successfully saved from the ScreenshotReady_t callback or via the WriteScreenshot, AddScreenshotToLibrary, AddVRScreenshotToLibrary calls.
bool Steam::tagUser(uint32 screenshot, uint64_t steamID){
	if(SteamScreenshots() == NULL){
		return false;
	}
	CSteamID userID = (uint64)steamID;
	return SteamScreenshots()->TagUser((ScreenshotHandle)screenshot, userID);
}
// Causes Steam overlay to take a screenshot.
void Steam::triggerScreenshot(){
	if(SteamScreenshots() != NULL){
		SteamScreenshots()->TriggerScreenshot();
	}
}
// Writes a screenshot to the user's Steam screenshot library.
uint32_t Steam::writeScreenshot(const PoolByteArray& RGB, int width, int height){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->WriteScreenshot((void*)RGB.read().ptr(), RGB.size(), width, height);
}

/////////////////////////////////////////////////
///// UGC ///////////////////////////////////////
/////////////////////////////////////////////////
//
// Adds a dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
// This is a soft-dependency that is displayed on the web. It is up to the application to determine whether the item can actually be used or not.
void Steam::addAppDependency(int publishedFileID, int appID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		AppId_t app = (int)appID;
		SteamAPICall_t apiCall = SteamUGC()->AddAppDependency(fileID, app);
		callResultAddAppDependency.Set(apiCall, this, &Steam::_add_app_dependency_result);
	}
}
// Adds a workshop item as a dependency to the specified item. If the nParentPublishedFileID item is of type k_EWorkshopFileTypeCollection, than the nChildPublishedFileID is simply added to that collection.
// Otherwise, the dependency is a soft one that is displayed on the web and can be retrieved via the ISteamUGC API using a combination of the m_unNumChildren member variable of the SteamUGCDetails_t struct and GetQueryUGCChildren.
void Steam::addDependency(int publishedFileID, int childPublishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t parent = (int)publishedFileID;
		PublishedFileId_t child = (int)childPublishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->AddDependency(parent, child);
		callResultAddUGCDependency.Set(apiCall, this, &Steam::_add_ugc_dependency_result);
	}
}
// Adds a excluded tag to a pending UGC Query. This will only return UGC without the specified tag.
bool Steam::addExcludedTag(uint64_t queryHandle, const String& tagName){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->AddExcludedTag(handle, tagName.utf8().get_data());
}
// Adds a key-value tag pair to an item. Keys can map to multiple different values (1-to-many relationship).
bool Steam::addItemKeyValueTag(uint64_t updateHandle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = (uint64_t)updateHandle;
	return SteamUGC()->AddItemKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}
// Adds an additional preview file for the item.
bool Steam::addItemPreviewFile(uint64_t queryHandle, const String& previewFile, int type){
	if(SteamUGC() == NULL){
		return false;
	}
	EItemPreviewType previewType;
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
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
	return SteamUGC()->AddItemPreviewFile(handle, previewFile.utf8().get_data(), previewType);
}
// Adds an additional video preview from YouTube for the item.
bool Steam::addItemPreviewVideo(uint64_t queryHandle, const String& videoID){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->AddItemPreviewVideo(handle, videoID.utf8().get_data());
}
// Adds a workshop item to the users favorites list.
void Steam::addItemToFavorite(int appID, int publishedFileID){
	if(SteamUGC() != NULL){
		AppId_t app = (int)appID;
		PublishedFileId_t fileID = (int)publishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->AddItemToFavorites(app, fileID);
		callResultFavoriteItemListChanged.Set(apiCall, this, &Steam::_user_favorite_items_list_changed);
	}
}
// Adds a required key-value tag to a pending UGC Query. This will only return workshop items that have a key = pKey and a value = pValue.
bool Steam::addRequiredKeyValueTag(uint64_t queryHandle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->AddRequiredKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}
// Adds a required tag to a pending UGC Query. This will only return UGC with the specified tag.
bool Steam::addRequiredTag(uint64_t queryHandle, const String& tagName){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->AddRequiredTag(handle, tagName.utf8().get_data());
}
// Lets game servers set a specific workshop folder before issuing any UGC commands.
bool Steam::initWorkshopForGameServer(int workshopDepotID){
	if(SteamUGC() == NULL){
		return false;
	}
	DepotId_t workshop = (int)workshopDepotID;
	const char *folder = new char[256];
	return SteamUGC()->BInitWorkshopForGameServer(workshop, (char*)folder);
}
// Creates a new workshop item with no content attached yet.
void Steam::createItem(AppId_t appID, int fileType){
	if(SteamUGC() != NULL){
		SteamAPICall_t apiCall = SteamUGC()->CreateItem(appID, (EWorkshopFileType)fileType);
		callResultItemCreate.Set(apiCall, this, &Steam::_item_created);
	}
}
// Query for all matching UGC. You can use this to list all of the available UGC for your app.
uint64_t Steam::createQueryAllUGCRequest(int queryType, int matchingType, int creatorID, int consumerID, uint32 page){
	if(SteamUGC() == NULL){
		return 0;
	}
	EUGCQuery query;
	if(queryType == 0){
		query = k_EUGCQuery_RankedByVote;
	}
	else if(queryType == 1){
		query = k_EUGCQuery_RankedByPublicationDate;
	}
	else if(queryType == 2){
		query = k_EUGCQuery_AcceptedForGameRankedByAcceptanceDate;
	}
	else if(queryType == 3){
		query = k_EUGCQuery_RankedByTrend;
	}
	else if(queryType == 4){
		query = k_EUGCQuery_FavoritedByFriendsRankedByPublicationDate;
	}
	else if(queryType == 5){
		query = k_EUGCQuery_CreatedByFriendsRankedByPublicationDate;
	}
	else if(queryType == 6){
		query = k_EUGCQuery_RankedByNumTimesReported;
	}
	else if(queryType == 7){
		query = k_EUGCQuery_CreatedByFollowedUsersRankedByPublicationDate;
	}
	else if(queryType == 8){
		query = k_EUGCQuery_NotYetRated;
	}
	else if(queryType == 9){
		query = k_EUGCQuery_RankedByTotalVotesAsc;
	}
	else if(queryType == 10){
		query = k_EUGCQuery_RankedByVotesUp;
	}
	else if(queryType == 11){
		query = k_EUGCQuery_RankedByTextSearch;
	}
	else if(queryType == 12){
		query = k_EUGCQuery_RankedByTotalUniqueSubscriptions;
	}
	else if(queryType == 13){
		query = k_EUGCQuery_RankedByPlaytimeTrend;
	}
	else if(queryType == 14){
		query = k_EUGCQuery_RankedByTotalPlaytime;
	}
	else if(queryType == 15){
		query = k_EUGCQuery_RankedByAveragePlaytimeTrend;
	}
	else if(queryType == 16){
		query = k_EUGCQuery_RankedByLifetimeAveragePlaytime;
	}
	else if(queryType == 17){
		query = k_EUGCQuery_RankedByPlaytimeSessionsTrend;
	}
	else{
		query = k_EUGCQuery_RankedByLifetimePlaytimeSessions;
	}
	EUGCMatchingUGCType match;
	if(matchingType == 0){
		match = k_EUGCMatchingUGCType_All;
	}
	else if(matchingType == 1){
		match = k_EUGCMatchingUGCType_Items_Mtx;
	}
	else if(matchingType == 2){
		match = k_EUGCMatchingUGCType_Items_ReadyToUse;
	}
	else if(matchingType == 3){
		match = k_EUGCMatchingUGCType_Collections;
	}
	else if(matchingType == 4){
		match = k_EUGCMatchingUGCType_Artwork;
	}
	else if(matchingType == 5){
		match = k_EUGCMatchingUGCType_Videos;
	}
	else if(matchingType == 6){
		match = k_EUGCMatchingUGCType_Screenshots;
	}
	else if(matchingType == 7){
		match = k_EUGCMatchingUGCType_AllGuides;
	}
	else if(matchingType == 8){
		match = k_EUGCMatchingUGCType_WebGuides;
	}
	else if(matchingType == 9){
		match = k_EUGCMatchingUGCType_IntegratedGuides;
	}
	else if(matchingType == 10){
		match = k_EUGCMatchingUGCType_UsableInGame;
	}
	else if(matchingType == 11){
		match = k_EUGCMatchingUGCType_ControllerBindings;
	}
	else{
		match = k_EUGCMatchingUGCType_GameManagedItems;
	}
	AppId_t creator = (int)creatorID;
	AppId_t consumer = (int)consumerID;
	UGCQueryHandle_t handle = SteamUGC()->CreateQueryAllUGCRequest(query, match, creator, consumer, page);
	return (uint64_t)handle;
}
// Query for the details of specific workshop items.
uint64_t Steam::createQueryUGCDetailsRequest(Array publishedFileIDs){
	if(SteamUGC() == NULL){
		return 0;
	}
	uint32 fileCount = publishedFileIDs.size();
	if(fileCount == 0){
		return 0;
	}
	PublishedFileId_t *fileIDs = new PublishedFileId_t[fileCount];
	for(uint32 i = 0; i < fileCount; i++){
		fileIDs[i] = (uint64_t)publishedFileIDs[i];
	}
	UGCQueryHandle_t handle = SteamUGC()->CreateQueryUGCDetailsRequest(fileIDs, fileCount);
	return (uint64_t)handle;
}
// WEIRD ERROR WHERE GODOT WON'T ALLOW MORE THAN 5 ARGUMENTS
// Query UGC associated with a user. You can use this to list the UGC the user is subscribed to amongst other things.
//uint64_t Steam::createQueryUserUGCRequest(int accountID, int listType, int matchingUGCType, int sortOrder, int creatorID, int consumerID, uint32 page){
//	if(SteamUGC() == NULL){
//		return 0;
//	}
//	AccountID_t account = (int)accountID;
//	EUserUGCList list;
//	if(listType == 0){
//		list = k_EUserUGCList_Published;
//	}
//	else if(listType == 1){
//		list = k_EUserUGCList_VotedOn;
//	}
//	else if(listType == 2){
//		list = k_EUserUGCList_VotedUp;
//	}
//	else if(listType == 3){
//		list = k_EUserUGCList_VotedDown;
//	}
//	else if(listType == 4){
//		list = k_EUserUGCList_WillVoteLater;
//	}
//	else if(listType == 5){
//		list = k_EUserUGCList_Favorited;
//	}
//	else if(listType == 6){
//		list = k_EUserUGCList_Subscribed;
//	}
//	else if(listType == 7){
//		list = k_EUserUGCList_UsedOrPlayed;
//	}
//	else{
//		list = k_EUserUGCList_Followed;
//	}
//	EUGCMatchingUGCType match;
//	if(matchingUGCType == 0){
//		match = k_EUGCMatchingUGCType_All;
//	}
//	else if(matchingUGCType == 1){
//		match = k_EUGCMatchingUGCType_Items_Mtx;
//	}
//	else if(matchingUGCType == 2){
//		match = k_EUGCMatchingUGCType_Items_ReadyToUse;
//	}
//	else if(matchingUGCType == 3){
//		match = k_EUGCMatchingUGCType_Collections;
//	}
//	else if(matchingUGCType == 4){
//		match = k_EUGCMatchingUGCType_Artwork;
//	}
//	else if(matchingUGCType == 5){
//		match = k_EUGCMatchingUGCType_Videos;
//	}
//	else if(matchingUGCType == 6){
//		match = k_EUGCMatchingUGCType_Screenshots;
//	}
//	else if(matchingUGCType == 7){
//		match = k_EUGCMatchingUGCType_AllGuides;
//	}
//	else if(matchingUGCType == 8){
//		match = k_EUGCMatchingUGCType_WebGuides;
//	}
//	else if(matchingUGCType == 9){
//		match = k_EUGCMatchingUGCType_IntegratedGuides;
//	}
//	else if(matchingUGCType == 10){
//		match = k_EUGCMatchingUGCType_UsableInGame;
//	}
//	else if(matchingUGCType == 11){
//		match = k_EUGCMatchingUGCType_ControllerBindings;
//	}
//	else{
//		match = k_EUGCMatchingUGCType_GameManagedItems;
//	}
//	EUserUGCListSortOrder sort;
//	if(sortOrder == 0){
//		sort = k_EUserUGCListSortOrder_CreationOrderDesc;
//	}
//	else if(sortOrder == 1){
//		sort = k_EUserUGCListSortOrder_CreationOrderAsc;
//	}
//	else if(sortOrder == 2){
//		sort = k_EUserUGCListSortOrder_TitleAsc;
//	}
//	else if(sortOrder == 3){
//		sort = k_EUserUGCListSortOrder_LastUpdatedDesc;
//	}
//	else if(sortOrder == 4){
//		sort = k_EUserUGCListSortOrder_SubscriptionDateDesc;
//	}
//	else if(sortOrder == 5){
//		sort = k_EUserUGCListSortOrder_VoteScoreDesc;
//	}
//	else{
//		sort = k_EUserUGCListSortOrder_ForModeration;
//	}
//	AppId_t creator = (int)creatorID;
//	AppId_t consumer = (int)consumerID;
//	UGCQueryHandle_t handle = SteamUGC()->CreateQueryUserUGCRequest(account, list, match, sort, creator, consumer, page);
//	return (uint64_t)handle;
//}
// Deletes the item without prompting the user.
void Steam::deleteItem(int publishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->DeleteItem(fileID);
		callResultDeleteItem.Set(apiCall, this, &Steam::_item_deleted);
	}
}
// Download new or update already installed item. If returns true, wait for DownloadItemResult_t. If item is already installed, then files on disk should not be used until callback received.
// If item is not subscribed to, it will be cached for some time. If bHighPriority is set, any other item download will be suspended and this item downloaded ASAP.
bool Steam::downloadItem(int publishedFileID, bool highPriority){
	if(SteamUGC() == NULL){
		return false;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->DownloadItem(fileID, highPriority);
}
// Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
Dictionary Steam::getItemDownloadInfo(int fileID){
	Dictionary info;
	if(SteamUGC() == NULL){
		return info;
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
// Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled set.
Dictionary Steam::getItemInstallInfo(int publishedFileID){
	Dictionary info;
	if(SteamUGC() == NULL){
		info["ret"] = false;
		return info;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
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
	if(SteamUGC() == NULL){
		return updateProgress;
	}
	UGCUpdateHandle_t handle = (uint64_t)updateHandle;
	uint64 processed = 0;
	uint64 total = 0;
	EItemUpdateStatus status = SteamUGC()->GetItemUpdateProgress(handle, &processed, &total);
	updateProgress["status"] = status;
	updateProgress["processed"] = uint64_t(processed);
	updateProgress["total"] = uint64_t(total);
	return updateProgress;
}
// Gets the total number of items the current user is subscribed to for the game or application.
uint32 Steam::getNumSubscribedItems(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUGC()->GetNumSubscribedItems();
}
// Retrieve the details of an additional preview associated with an individual workshop item after receiving a querying UGC call result.
Dictionary Steam::getQueryUGCAdditionalPreview(uint64_t queryHandle, uint32 index, uint32 previewIndex){
	Dictionary preview;
	if(SteamUGC() == NULL){
		return preview;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	char *urlOrVideoID = new char[256];
	char *originalFilename = new char[256];
	EItemPreviewType previewType;
	bool success = SteamUGC()->GetQueryUGCAdditionalPreview(handle, index, previewIndex, (char*)urlOrVideoID, 256, (char*)originalFilename, 256, &previewType);
	if(success){
		preview["success"] = success;
		preview["handle"] = (uint64_t)handle;
		preview["index"] = index;
		preview["preview"] = previewIndex;
		preview["urlOrVideo"] = urlOrVideoID;
		preview["filename"] = originalFilename;
		preview["type"] = previewType;
	}
	return preview;
}
// Retrieve the ids of any child items of an individual workshop item after receiving a querying UGC call result. These items can either be a part of a collection or some other dependency (see AddDependency).
Dictionary Steam::getQueryUGCChildren(uint64_t queryHandle, uint32 index){
	Dictionary children;
	if(SteamUGC() == NULL){
		return children;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	PublishedFileId_t *child = new PublishedFileId_t[100];
	bool success = SteamUGC()->GetQueryUGCChildren(handle, index, (PublishedFileId_t*)child, 100);
	if(success){
		children["success"] = success;
		children["handle"] = (uint64_t)handle;
		children["index"] = index;
		children["children"] = child;
	}
	return children;
}
// Retrieve the details of a key-value tag associated with an individual workshop item after receiving a querying UGC call result.
Dictionary Steam::getQueryUGCKeyValueTag(uint64_t queryHandle, uint32 index, uint32 keyValueTagIndex){
	Dictionary tag;
	if(SteamUGC() == NULL){
		return tag;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	char *key = new char[256];
	char *value = new char[256];
	bool success = SteamUGC()->GetQueryUGCKeyValueTag(handle, index, keyValueTagIndex, (char*)key, 256, (char*)value, 256);
	if(success){
		tag["success"] = success;
		tag["handle"] = (uint64_t)handle;
		tag["index"] = index;
		tag["tag"] = keyValueTagIndex;
		tag["key"] = key;
		tag["value"] = value;
	}
	return tag;
}
// Retrieve the developer set metadata of an individual workshop item after receiving a querying UGC call result.
String Steam::getQueryUGCMetadata(uint64_t queryHandle, uint32 index){
	if(SteamUGC() == NULL){
		return "";
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	char *metadata = new char[256];
	bool success = SteamUGC()->GetQueryUGCMetadata(handle, index, (char*)metadata, 256);
	if(success){
		String ugcMetadata = metadata;
		return ugcMetadata;
	}
	return "";
}
// Retrieve the number of additional previews of an individual workshop item after receiving a querying UGC call result.
uint32 Steam::getQueryUGCNumAdditionalPreviews(uint64_t queryHandle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->GetQueryUGCNumAdditionalPreviews(handle, index);
}
// Retrieve the number of key-value tags of an individual workshop item after receiving a querying UGC call result.
uint32 Steam::getQueryUGCNumKeyValueTags(uint64_t queryHandle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->GetQueryUGCNumKeyValueTags(handle, index);
}
// Retrieve the URL to the preview image of an individual workshop item after receiving a querying UGC call result.
String Steam::getQueryUGCPreviewURL(uint64_t queryHandle, uint32 index){
	if(SteamUGC() == NULL){
		return "";
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	char *url = new char[256];
	bool success = SteamUGC()->GetQueryUGCPreviewURL(handle, index, (char*)url, 256);
	if(success){
		String previewURL = url;
		return previewURL;
	}
	return "";
}
// Retrieve the details of an individual workshop item after receiving a querying UGC call result.
Dictionary Steam::getQueryUGCResult(uint64_t queryHandle, uint32 index){
	Dictionary ugcResult;
	if(SteamUGC() == NULL){
		return ugcResult;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	SteamUGCDetails_t pDetails;
	bool success = SteamUGC()->GetQueryUGCResult(handle, index, &pDetails);
	if(success){
		ugcResult["result"] = (uint64_t)pDetails.m_eResult;
		ugcResult["fileID"] = (uint64_t)pDetails.m_nPublishedFileId;
		ugcResult["fileType"] = (uint64_t)pDetails.m_eFileType;
		ugcResult["creatorAppID"] = (uint64_t)pDetails.m_nCreatorAppID;
		ugcResult["consumerAppID"] = (uint64_t)pDetails.m_nConsumerAppID;
		ugcResult["title"] = pDetails.m_rgchTitle;
		ugcResult["description"] = pDetails.m_rgchDescription;
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
Dictionary Steam::getQueryUGCStatistic(uint64_t queryHandle, uint32 index, int statType){
	Dictionary ugcStat;
	if(SteamUGC() == NULL){
		return ugcStat;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	EItemStatistic type;
		if(statType == 0){
		type = k_EItemStatistic_NumSubscriptions;
	}
	else if(statType == 1){
		type = k_EItemStatistic_NumFavorites;
	}
	else if(statType == 2){
		type = k_EItemStatistic_NumFollowers;
	}
	else if(statType == 3){
		type = k_EItemStatistic_NumUniqueSubscriptions;
	}
	else if(statType == 4){
		type = k_EItemStatistic_NumUniqueFavorites;
	}
	else if(statType == 5){
		type = k_EItemStatistic_NumUniqueFollowers;
	}
	else if(statType == 6){
		type = k_EItemStatistic_NumUniqueWebsiteViews;
	}
	else if(statType == 7){
		type = k_EItemStatistic_ReportScore;
	}
	else if(statType == 8){
		type = k_EItemStatistic_NumSecondsPlayed;
	}
	else if(statType == 9){
		type = k_EItemStatistic_NumPlaytimeSessions;
	}
	else if(statType == 10){
		type = k_EItemStatistic_NumComments;
	}
	else if(statType == 11){
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
// Gets a list of all of the items the current user is subscribed to for the current game.
Array Steam::getSubscribedItems(){
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
	delete items;
	return subscribed;
}
// Gets the users vote status on a workshop item.
void Steam::getUserItemVote(int publishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->GetUserItemVote(fileID);
		callResultGetUserItemVote.Set(apiCall, this, &Steam::_get_item_vote_result);
	}
}
// Releases a UGC query handle when you are done with it to free up memory.
bool Steam::releaseQueryUGCRequest(uint64_t queryHandle){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->ReleaseQueryUGCRequest(handle);
}
// Removes the dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
void Steam::removeAppDependency(int publishedFileID, int appID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		AppId_t app = (int)appID;
		SteamAPICall_t apiCall = SteamUGC()->RemoveAppDependency(fileID, app);
		callResultRemoveAppDependency.Set(apiCall, this, &Steam::_remove_app_dependency_result);
	}
}
// Removes a workshop item as a dependency from the specified item.
void Steam::removeDependency(int publishedFileID, int childPublishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		PublishedFileId_t childID = (int)childPublishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->RemoveDependency(fileID, childID);
		callResultRemoveUGCDependency.Set(apiCall, this, &Steam::_remove_ugc_dependency_result);
	}
}
// Removes a workshop item from the users favorites list.
void Steam::removeItemFromFavorites(int appID, int publishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		AppId_t app = (int)appID;
		SteamAPICall_t apiCall = SteamUGC()->RemoveItemFromFavorites(app, fileID);
		callResultFavoriteItemListChanged.Set(apiCall, this, &Steam::_user_favorite_items_list_changed);
	}
}
// Removes an existing key value tag from an item.
bool Steam::removeItemKeyValueTags(uint64_t updateHandle, const String& key){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->RemoveItemKeyValueTags(handle, key.utf8().get_data());
}
// Removes an existing preview from an item.
bool Steam::removeItemPreview(uint64_t updateHandle, uint32 index){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->RemoveItemPreview(handle, index);
}
// Send a UGC query to Steam.
void Steam::sendQueryUGCRequest(uint64_t updateHandle){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(updateHandle);
		SteamAPICall_t apiCall = SteamUGC()->SendQueryUGCRequest(handle);
		callResultUGCQueryCompleted.Set(apiCall, this, &Steam::_ugc_query_completed);
	}
}
// Sets whether results will be returned from the cache for the specific period of time on a pending UGC Query.
bool Steam::setAllowCachedResponse(uint64_t updateHandle, uint32 maxAgeSeconds){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetAllowCachedResponse(handle, maxAgeSeconds);
}
// Sets to only return items that have a specific filename on a pending UGC Query.
bool Steam::setCloudFileNameFilter(uint64_t updateHandle, const String& matchCloudFilename){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetCloudFileNameFilter(handle, matchCloudFilename.utf8().get_data());
}
// Sets the folder that will be stored as the content for an item.
bool Steam::setItemContent(uint64_t updateHandle, const String& contentFolder){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemContent(handle, contentFolder.utf8().get_data());
}
// Sets a new description for an item.
bool Steam::setItemDescription(uint64_t updateHandle, const String& description){
	if(SteamUGC() == NULL){
		return false;
	}
	if (description.length() > 255){
		printf("Description cannot have more than %d ASCII characters. Description not set.", 255);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemDescription(handle, description.utf8().get_data());
}
// Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
bool Steam::setItemMetadata(uint64_t updateHandle, const String& metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	if (metadata.length() > 255){
		printf("Metadata cannot have more than %d ASCII characters. Metadata not set.", 255);
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemMetadata(handle, metadata.utf8().get_data());
}
// Sets the primary preview image for the item.
bool Steam::setItemPreview(uint64_t updateHandle, const String& previewFile){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemPreview(handle, previewFile.utf8().get_data());
}
// Sets arbitrary developer specified tags on an item.
bool Steam::setItemTags(uint64_t updateHandle, Array tagArray){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	SteamParamStringArray_t *pTags = new SteamParamStringArray_t();
	pTags->m_ppStrings = new const char*[tagArray.size()];
	uint32 strCount = tagArray.size();
	for (uint32 i=0; i < strCount; i++) {
		String str = (String)tagArray[i];
		pTags->m_ppStrings[i] = str.utf8().get_data();
	}
	pTags->m_nNumStrings = tagArray.size();
	return SteamUGC()->SetItemTags(handle, pTags);
}
// Sets a new title for an item.
bool Steam::setItemTitle(uint64_t updateHandle, const String& title){
	if(SteamUGC() == NULL){
		return false;
	}
	if (title.length() > 255){
		printf("Title cannot have more than %d ASCII characters. Title not set.", 255);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemTitle(handle, title.utf8().get_data());
}
// Sets the language of the title and description that will be set in this item update.
bool Steam::setItemUpdateLanguage(uint64_t updateHandle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}
// Sets the visibility of an item.
bool Steam::setItemVisibility(uint64_t updateHandle, int visibility){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemVisibility(handle, (ERemoteStoragePublishedFileVisibility)visibility);
}
// Sets the language to return the title and description in for the items on a pending UGC Query.
bool Steam::setLanguage(uint64_t queryHandle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetLanguage(handle, language.utf8().get_data());
}
// Sets whether workshop items will be returned if they have one or more matching tag, or if all tags need to match on a pending UGC Query.
bool Steam::setMatchAnyTag(uint64_t queryHandle, bool matchAnyTag){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetMatchAnyTag(handle, matchAnyTag);
}
// Sets whether the order of the results will be updated based on the rank of items over a number of days on a pending UGC Query.
bool Steam::setRankedByTrendDays(uint64_t queryHandle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetRankedByTrendDays(handle, days);
}
// Sets whether to return any additional images/videos attached to the items on a pending UGC Query.
bool Steam::setReturnAdditionalPreviews(uint64_t queryHandle, bool returnAdditionalPreviews){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnAdditionalPreviews(handle, returnAdditionalPreviews);
}
// Sets whether to return the IDs of the child items of the items on a pending UGC Query.
bool Steam::setReturnChildren(uint64_t queryHandle, bool returnChildren){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnChildren(handle, returnChildren);
}
// Sets whether to return any key-value tags for the items on a pending UGC Query.
bool Steam::setReturnKeyValueTags(uint64_t queryHandle, bool returnKeyValueTags){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnKeyValueTags(handle, returnKeyValueTags);
}
// Sets whether to return the full description for the items on a pending UGC Query.
bool Steam::setReturnLongDescription(uint64_t queryHandle, bool returnLongDescription){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnLongDescription(handle, returnLongDescription);
}
// Sets whether to return the developer specified metadata for the items on a pending UGC Query.
bool Steam::setReturnMetadata(uint64_t queryHandle, bool returnMetadata){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnMetadata(handle, returnMetadata);
}
// Sets whether to only return IDs instead of all the details on a pending UGC Query.
bool Steam::setReturnOnlyIDs(uint64_t queryHandle, bool returnOnlyIDs){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnOnlyIDs(handle, returnOnlyIDs);
}
// Sets whether to return the the playtime stats on a pending UGC Query.
bool Steam::setReturnPlaytimeStats(uint64_t queryHandle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnPlaytimeStats(handle, days);
}
// Sets whether to only return the the total number of matching items on a pending UGC Query.
bool Steam::setReturnTotalOnly(uint64_t queryHandle, bool returnTotalOnly){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetReturnTotalOnly(handle, returnTotalOnly);
}
// Sets a string to that items need to match in either the title or the description on a pending UGC Query.
bool Steam::setSearchText(uint64_t queryHandle, const String& searchText){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)queryHandle;
	return SteamUGC()->SetSearchText(handle, searchText.utf8().get_data());
}
// Allows the user to rate a workshop item up or down.
void Steam::setUserItemVote(int publishedFileID, bool voteUp){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->SetUserItemVote(fileID, voteUp);
		callResultSetUserItemVote.Set(apiCall, this, &Steam::_set_user_item_vote);
	}
}
// Starts the item update process.
uint64_t Steam::startItemUpdate(int appID, int publishedFileID){
	if(SteamUGC() == NULL){
		return 0;
	}
	AppId_t app = (int)appID;
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->StartItemUpdate(app, fileID);
}
// Start tracking playtime on a set of workshop items.
void Steam::startPlaytimeTracking(Array publishedFileIDs){
	if(SteamUGC() != NULL){
		uint32 fileCount = publishedFileIDs.size();
		if(fileCount > 0){
			PublishedFileId_t *fileIDs = new PublishedFileId_t[fileCount];
			for(uint32 i = 0; i < fileCount; i++){
				fileIDs[i] = (uint64_t)publishedFileIDs[i];
			}
			SteamAPICall_t apiCall = SteamUGC()->StartPlaytimeTracking(fileIDs, fileCount);
			callResultStartPlaytimeTracking.Set(apiCall, this, &Steam::_start_playtime_tracking);
		}
	}
}
// Stop tracking playtime on a set of workshop items.
void Steam::stopPlaytimeTracking(Array publishedFileIDs){
	if(SteamUGC() != NULL){
		uint32 fileCount = publishedFileIDs.size();
		if(fileCount > 0){
			PublishedFileId_t *fileIDs = new PublishedFileId_t[fileCount];
			Array files;
			for(uint32 i = 0; i < fileCount; i++){
				fileIDs[i] = (uint64_t)publishedFileIDs[i];
			}
			SteamAPICall_t apiCall = SteamUGC()->StopPlaytimeTracking(fileIDs, fileCount);
			callResultStopPlaytimeTracking.Set(apiCall, this, &Steam::_stop_playtime_tracking);
		}
	}
}
// Stop tracking playtime of all workshop items.
void Steam::stopPlaytimeTrackingForAllItems(){
	if(SteamUGC() != NULL){
		SteamAPICall_t apiCall = SteamUGC()->StopPlaytimeTrackingForAllItems();
		callResultStopPlaytimeTracking.Set(apiCall, this, &Steam::_stop_playtime_tracking);
	}
}
// Returns any app dependencies that are associated with the given item.
void Steam::getAppDependencies(int publishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->GetAppDependencies(fileID);
		callResultGetAppDependencies.Set(apiCall, this, &Steam::_get_app_dependencies_result);
	}
}
// Uploads the changes made to an item to the Steam Workshop; to be called after setting your changes.
void Steam::submitItemUpdate(uint64_t updateHandle, const String& changeNote){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(updateHandle);
		SteamAPICall_t apiCall = SteamUGC()->SubmitItemUpdate(handle, changeNote.utf8().get_data());
		callResultItemUpdate.Set(apiCall, this, &Steam::_item_updated);
	}
}
// Subscribe to a workshop item. It will be downloaded and installed as soon as possible.
void Steam::subscribeItem(int publishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->SubscribeItem(fileID);
		callResultSubscribeItem.Set(apiCall, this, &Steam::_subscribe_item);
	}
}
// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends.
void Steam::suspendDownloads(bool suspend){
	if(SteamUGC() != NULL){
		SteamUGC()->SuspendDownloads(suspend);
	}
}
// Unsubscribe from a workshop item. This will result in the item being removed after the game quits.
void Steam::unsubscribeItem(int publishedFileID){
	if(SteamUGC() != NULL){
		PublishedFileId_t fileID = (int)publishedFileID;
		SteamAPICall_t apiCall = SteamUGC()->UnsubscribeItem(fileID);
		callResultUnsubscribeItem.Set(apiCall, this, &Steam::_unsubscribe_item);
	}
}
// Updates an existing additional preview file for the item.
bool Steam::updateItemPreviewFile(uint64_t updateHandle, uint32 index, const String& previewFile){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->UpdateItemPreviewFile(handle, index, previewFile.utf8().get_data());
}
// Updates an additional video preview from YouTube for the item.
bool Steam::updateItemPreviewVideo(uint64_t updateHandle, uint32 index, const String& videoID){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->UpdateItemPreviewVideo(handle, index, videoID.utf8().get_data());
}

/////////////////////////////////////////////////
///// USERS /////////////////////////////////////
/////////////////////////////////////////////////
//
// Set the rich presence data for an unsecured game server that the user is playing on. This allows friends to be able to view the game info and join your game.
void Steam::advertiseGame(const String& serverIP, int port){
	if(SteamUser() != NULL){
		// Resolve address and convert it from IP_Address struct to uint32_t
		IP_Address address;
		if(serverIP.is_valid_ip_address()){
			address = serverIP;
		}
		else{
			address = IP::get_singleton()->resolve_hostname(serverIP, IP::TYPE_IPV4);
		}
		// Resolution failed - Godot 3.0 has is_invalid() to check this
		if(address != IP_Address()){
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
// Cancels an auth ticket.
void Steam::cancelAuthTicket(uint32_t authTicket){
	if(SteamUser() != NULL){
		for(int i = 0; i < tickets.size(); i++){
			TicketData ticket = tickets.get(i);
			if (ticket.id == authTicket){
				tickets.remove(i);
				memdelete_arr(ticket.buffer);
				break;
			}
		}
	}
}
// Decodes the compressed voice data returned by GetVoice.
Dictionary Steam::decompressVoice(const PoolByteArray& voice, uint32 voiceSize, uint32 sampleRate){
	Dictionary decompressed;
	if(SteamUser() != NULL){
		uint32 written = 0;
		PoolByteArray outputBuffer;
		int result = SteamUser()->DecompressVoice(voice.read().ptr(), voiceSize, &outputBuffer, sizeof(outputBuffer), &written, sampleRate);
		if(result == 0){
			decompressed["uncompressed"] = outputBuffer;
			decompressed["size"] = written;
		}
	}
	return decompressed;
}
// Ends an auth session.
void Steam::endAuthSession(uint64_t steamID){
	if(SteamUser() != NULL){
		CSteamID authSteamID = createSteamID(steamID);
		SteamUser()->EndAuthSession(authSteamID);
	}
}
// Get an authentication ticket ID.
uint32_t Steam::getAuthSessionTicketID(){
	if(SteamUser() == NULL){
		return 0;
	}
	uint32_t ticketSize = 1024;
	PoolByteArray buffer;
	buffer.resize(ticketSize);
	uint32_t id = SteamUser()->GetAuthSessionTicket(buffer.write().ptr(), ticketSize, &ticketSize);
	TicketData ticket = {id, (uint32_t *)buffer.read().ptr(), ticketSize};
	tickets.push_back(ticket);
	return id;
}
// Get the authentication ticket data.
Dictionary Steam::getAuthSessionTicket(){
	// Create the dictionary to use
	Dictionary authTicket;
	if(SteamUser() != NULL){
		uint32_t ticketSize = 1024;
		PoolByteArray buffer;
		buffer.resize(ticketSize);
		uint32_t id = SteamUser()->GetAuthSessionTicket(buffer.write().ptr(), ticketSize, &ticketSize);
		TicketData ticket = {id, (uint32_t *)buffer.read().ptr(), ticketSize};
		tickets.push_back(ticket);
		// Add this data to the dictionary
		authTicket["id"] = id;
		authTicket["buffer"] = buffer;
		authTicket["size"] = ticketSize;
	}
	return authTicket;
}
// Checks to see if there is captured audio data available from GetVoice, and gets the size of the data.
int Steam::getAvailableVoice(){
	if(SteamUser() == NULL){
		return 0;
	}
	uint32 bytesAvailable = 0;
	return SteamUser()->GetAvailableVoice(&bytesAvailable, NULL, 0);
}
// Retrieves anti indulgence / duration control for current user / game combination.
void Steam::getDurationControl(){
	if(SteamUser() != NULL){
		SteamAPICall_t apiCall = SteamUser()->GetDurationControl();
		callResultDurationControl.Set(apiCall, this, &Steam::_duration_control);
	}
}
// Retrieve an encrypted ticket. This should be called after requesting an encrypted app ticket with RequestEncryptedAppTicket and receiving the EncryptedAppTicketResponse_t call result.
Dictionary Steam::getEncryptedAppTicket(){
	Dictionary encrypted;
	if(SteamUser() != NULL){
		uint32_t ticketSize = 1024;
		PoolByteArray buffer;
		buffer.resize(ticketSize);
		if(SteamUser()->GetEncryptedAppTicket(buffer.write().ptr(), ticketSize, &ticketSize)){
			encrypted["buffer"] = buffer;
			encrypted["size"] = ticketSize;
		}
	}
	return encrypted;
}
// Trading Card badges data access, if you only have one set of cards, the series will be 1.
// The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1).
int Steam::getGameBadgeLevel(int series, bool foil){
	if(SteamUser()== NULL){
		return 0;
	}
	return SteamUser()->GetGameBadgeLevel(series, foil);
}
// Get the user's Steam level.
int Steam::getPlayerSteamLevel(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUser()->GetPlayerSteamLevel(); 
}
// Get user's Steam ID.
uint64_t Steam::getSteamID(){
	if(SteamUser() == NULL){
		return 0;
	}
	CSteamID steamID = SteamUser()->GetSteamID();
	return steamID.ConvertToUint64();
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
// Read captured audio data from the microphone buffer.
uint32 Steam::getVoice(){
	if(SteamUser() == NULL){
		return 0;
	}
	uint32 written = 0;
	uint8 buffer[1024];
	int response = SteamUser()->GetVoice(true, &buffer, 1024, &written, false, NULL, 0, NULL, 0);
	if(response == 1){
		return written;
	}
	return 0;
}
// Gets the native sample rate of the Steam voice decoder.
uint32 Steam::getVoiceOptimalSampleRate(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUser()->GetVoiceOptimalSampleRate();
}
// This starts the state machine for authenticating the game client with the game server. It is the client portion of a three-way handshake between the client, the game server, and the steam servers.
Dictionary Steam::initiateGameConnection(uint64_t serverID, uint32 serverIP, uint16 serverPort, bool secure){
	Dictionary connection;
	if(SteamUser() != NULL){
		PoolByteArray auth;
		int authSize = 2048;
		auth.resize(authSize);
		CSteamID server = (uint64)serverID;
		if(SteamUser()->InitiateGameConnection(&auth, authSize, server, serverIP, serverPort, secure) > 0){
			connection["auth_blob"] = auth;
			connection["server_id"] = serverID;
			connection["server_ip"] = serverIP;
			connection["server_port"] = serverPort;
		}
	}
	return connection;
}
// Checks if the current users looks like they are behind a NAT device.
bool Steam::isBehindNAT(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BIsBehindNAT();
}
// Checks whether the user's phone number is used to uniquely identify them.
bool Steam::isPhoneIdentifying(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BIsPhoneIdentifying();
}
// Checks whether the current user's phone number is awaiting (re)verification.
bool Steam::isPhoneRequiringVerification(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BIsPhoneRequiringVerification();
}
// Checks whether the current user has verified their phone number.
bool Steam::isPhoneVerified(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BIsPhoneVerified();
}
// Checks whether the current user has Steam Guard two factor authentication enabled on their account.
bool Steam::isTwoFactorEnabled(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BIsTwoFactorEnabled();
}
// Check, true/false, if user is logged into Steam currently.
bool Steam::loggedOn(){
	if(SteamUser() == NULL){
		return false;
	}
	return SteamUser()->BLoggedOn();
}
// Requests an application ticket encrypted with the secret "encrypted app ticket key".
void Steam::requestEncryptedAppTicket(const String& secret){
	if(SteamUser() != NULL){
		SteamAPICall_t apiCall = SteamUser()->RequestEncryptedAppTicket((void*)secret.utf8().get_data(), sizeof(secret));
		callResultEncryptedAppTicketResponse.Set(apiCall, this, &Steam::_encrypted_app_ticket_response);
	}
}
// Requests a URL which authenticates an in-game browser for store check-out, and then redirects to the specified URL.
void Steam::requestStoreAuthURL(const String& redirect){
	if(SteamUser() != NULL){
		SteamAPICall_t apiCall = SteamUser()->RequestStoreAuthURL(redirect.utf8().get_data());
		callResultStoreAuthURLResponse.Set(apiCall, this, &Steam::_store_auth_url_response);
	}
}
// Starts voice recording.
void Steam::startVoiceRecording(){
	if(SteamUser() != NULL){
		SteamUser()->StartVoiceRecording();
	}
}
// Stops voice recording.
void Steam::stopVoiceRecording(){
	if(SteamUser() != NULL){
		SteamUser()->StopVoiceRecording();
	}
}
// Notify the game server that we are disconnecting. NOTE: This is part of the old user authentication API and should not be mixed with the new API.
void Steam::terminateGameConnection(uint32 serverIP, uint16 serverPort){
	if(SteamUser() != NULL){
		SteamUser()->TerminateGameConnection(serverIP, serverPort);
	}
}
// Checks if the user owns a specific piece of Downloadable Content (DLC). This can only be called after sending the users auth ticket to ISteamGameServer::BeginAuthSession.
int Steam::userHasLicenseForApp(uint64_t steamID, int appID){
	if(SteamUser() == NULL){
		return 2;
	}
	CSteamID userID = (uint64)steamID;
	return SteamUser()->UserHasLicenseForApp(userID, (AppId_t)appID);
}

/////////////////////////////////////////////////
///// USER STATS ////////////////////////////////
/////////////////////////////////////////////////
//
// Attaches a piece of user generated content the current user's entry on a leaderboard.
void Steam::attachLeaderboardUGC(uint64_t ugcHandle){
	if(SteamUserStats() != NULL){
		SteamAPICall_t apiCall = SteamUserStats()->AttachLeaderboardUGC(leaderboardHandle, (UGCHandle_t)ugcHandle);
		callResultLeaderboardUGCSet.Set(apiCall, this, &Steam::_leaderboard_ugc_set);
	}
}
// Clears a given achievement.
bool Steam::clearAchievement(const String& name){
	if(SteamUserStats() == NULL){
		return false;
	}
	return SteamUserStats()->ClearAchievement(name.utf8().get_data());
}
// Request all rows for friends of user.
void Steam::downloadLeaderboardEntries(int start, int end, int type){
	if(SteamUserStats() != NULL){
		SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntries(leaderboardHandle, ELeaderboardDataRequest(type), start, end);
		callResultEntries.Set(apiCall, this, &Steam::_leaderboard_scores_downloaded);
	}
}
// Request a maximum of 100 users with only one outstanding call at a time.
void Steam::downloadLeaderboardEntriesForUsers(Array usersID){
	if(SteamUserStats() != NULL){
		int usersCount = usersID.size();
		if(usersCount > 0){
			CSteamID *users = new CSteamID[usersCount];
			for(int i = 0; i < usersCount; i++){
				CSteamID user = createSteamID(usersID[i]);
				users[i] = user;
			}
			SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboardHandle, users, usersCount);
			callResultEntries.Set(apiCall, this, &Steam::_leaderboard_scores_downloaded);
			delete[] users;
		}
	}
}
// Find a given leaderboard, by name.
void Steam::findLeaderboard(const String& name){
	if(SteamUserStats() != NULL){
		SteamAPICall_t apiCall = SteamUserStats()->FindLeaderboard(name.utf8().get_data());
		callResultFindLeaderboard.Set(apiCall, this, &Steam::_leaderboard_find_result);
	}
}
// Gets a leaderboard by name, it will create it if it's not yet created.
void Steam::findOrCreateLeaderboard(const String& name, int sortMethod, int displayType){
	if(SteamUserStats() != NULL){
		SteamAPICall_t apiCall = SteamUserStats()->FindOrCreateLeaderboard(name.utf8().get_data(), (ELeaderboardSortMethod)sortMethod, (ELeaderboardDisplayType)displayType);
		callResultFindLeaderboard.Set(apiCall, this, &Steam::_leaderboard_find_result);
	}
}
// Return true/false if user has given achievement and the bool status of it being achieved or not.
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
	Dictionary achieve;
	float percent = 0.f;
	if(SteamUserStats() == NULL){
		achieve["ret"] = false;
	} else {
		achieve["ret"] = SteamUserStats()->GetAchievementAchievedPercent(name.utf8().get_data(), &percent);
	}
	achieve["percent"] = percent;
	return achieve;
}
// Get the achievement status, and the time it was unlocked if unlocked (in seconds since January 1, 19).
Dictionary Steam::getAchievementAndUnlockTime(const String& name){
	Dictionary achieve;
	if(SteamUserStats() == NULL){
		return achieve;
	}
	bool achieved = false;
	uint32 unlockTime = 0;
	// Get the data from Steam
	bool retrieved = SteamUserStats()->GetAchievementAndUnlockTime(name.utf8().get_data(), &achieved, &unlockTime);
	if(retrieved){
		achieve["retrieve"] = retrieved;
		achieve["achieved"] = achieved;
		achieve["unlocked"] = unlockTime;
	}
	return achieve;
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
String Steam::getAchievementName(uint32_t achievement){
	if(SteamUserStats() == NULL){
		return "";
	}
	return SteamUserStats()->GetAchievementName(achievement);
}
// For achievements that have related Progress stats, use this to query what the bounds of that progress are. You may want this info to selectively call IndicateAchievementProgress when appropriate milestones of progress have been made, to show a progress notification to the user.
Dictionary Steam::getAchievementProgressLimitsInt(const String& name){
	Dictionary progress;
	if(SteamUserStats() != NULL){
		int32 min = 0;
		int32 max = 0;
		if(SteamUserStats()->GetAchievementProgressLimits(name.utf8().get_data(), &min, &max)){
			progress["name"] = name;
			progress["min"] = min;
			progress["max"] = max;
		}
	}
	return progress;
}
// For achievements that have related Progress stats, use this to query what the bounds of that progress are. You may want this info to selectively call IndicateAchievementProgress when appropriate milestones of progress have been made, to show a progress notification to the user.
Dictionary Steam::getAchievementProgressLimitsFloat(const String& name){
	Dictionary progress;
	if(SteamUserStats() != NULL){
		float min = 0.0;
		float max = 0.0;
		if(SteamUserStats()->GetAchievementProgressLimits(name.utf8().get_data(), &min, &max)){
			progress["name"] = name;
			progress["min"] = min;
			progress["max"] = max;
		}
	}
	return progress;
}
// Gets the lifetime totals for an aggregated stat; as an int
int64 Steam::getGlobalStatInt(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	int64 stat = 0;
	SteamUserStats()->GetGlobalStat(name.utf8().get_data(), &stat);
	return stat;
}
// Gets the lifetime totals for an aggregated stat; as an int
double Steam::getGlobalStatFloat(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	double stat = 0;
	SteamUserStats()->GetGlobalStat(name.utf8().get_data(), &stat);
	return stat;
}
// Gets the daily history for an aggregated stat; int.
int64 Steam::getGlobalStatIntHistory(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	int64 history = 0;
	SteamUserStats()->GetGlobalStatHistory(name.utf8().get_data(), &history, 60);
	return history;
}
// Gets the daily history for an aggregated stat; float / double.
double Steam::getGlobalStatFloatHistory(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	double history = 0;
	SteamUserStats()->GetGlobalStatHistory(name.utf8().get_data(), &history, 60);
	return history;
}
// Returns the display type of a leaderboard handle.
Dictionary Steam::getLeaderboardDisplayType(){
	// Create a diciontary to return
	Dictionary display;
	if(SteamUserStats() != NULL){
		int display_type = SteamUserStats()->GetLeaderboardDisplayType(leaderboardHandle);
		// Add a verbal response
		if(display_type == 3){
			display["result"] = 3;
			display["verbal"] = "Display is time in milliseconds";
		}
		else if(display_type == 2){
			display["result"] = 2;
			display["verbal"] = "Display is time in seconds";
		}
		else if(display_type == 1){
			display["result"] = 1;
			display["verbal"] = "Display is simple numerical value";
		}
		else{
			display["result"] = 0;
			display["verbal"] = "Display type or leaderboard handle is invalid";
		}
	}
	return display;
}
// Get the total number of entries in a leaderboard, as of the last request.
int Steam::getLeaderboardEntryCount(){
	if(SteamUserStats() == NULL){
		return -1;
	}
	return SteamUserStats()->GetLeaderboardEntryCount(leaderboardHandle);
}
// Get the name of a leaderboard.
String Steam::getLeaderboardName(){
	if(SteamUserStats() == NULL){
		return "";
	}
	return SteamUserStats()->GetLeaderboardName(leaderboardHandle);
}
// Returns the sort order of a leaderboard handle.
Dictionary Steam::getLeaderboardSortMethod(){
	// Create a dictionary to return
	Dictionary sort;
	if(SteamUserStats() != NULL){
		// Get the sort method int	
		int sort_method = SteamUserStats()->GetLeaderboardSortMethod(leaderboardHandle);
		// Add a verbal response
		if(sort_method == 2){
			sort["result"] = 2;
			sort["verbal"] = "Top score is highest number";
		}
		else if(sort_method == 1){
			sort["result"] = 1;
			sort["verbal"] = "Top score is lowest number";
		}
		else{
			sort["result"] = 0;
			sort["verbal"] = "Sort method or leaderboard handle is invalid";
		}
	}	
	return sort;
}
// Gets the info on the most achieved achievement for the game.
Array Steam::getMostAchievedAchievementInfo(){
	if(SteamUserStats() == NULL){
		return Array();
	}
	Array achieve;
	char name;
	float percent = 0;
	bool achieved = false;
	// Get the data from Steam
	int result = SteamUserStats()->GetMostAchievedAchievementInfo(&name, 64, &percent, &achieved);
	while(result != -1){
		Dictionary entry;
		entry["rank"] = result;
		entry["name"] = name;
		entry["percent"] = percent;
		entry["achieved"] = achieved;
		achieve.append(entry);
		// Get the next most achieved achievement
		result = SteamUserStats()->GetNextMostAchievedAchievementInfo(result, &name, 64, &percent, &achieved);
	}
	return achieve;
}
// Get the number of achievements.
uint32_t Steam::getNumAchievements(){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->GetNumAchievements();
}
//  Get the amount of players currently playing the current game (online + offline).
void Steam::getNumberOfCurrentPlayers(){
	if(SteamUserStats() != NULL){
		SteamAPICall_t apiCall = SteamUserStats()->GetNumberOfCurrentPlayers();
		callResultNumberOfCurrentPlayers.Set(apiCall, this, &Steam::_number_of_current_players);
	}
}
// Get the value of a float statistic.
float Steam::getStatFloat(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	float statValue = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statValue);
	return statValue;
}
// Get the value of an integer statistic.
int Steam::getStatInt(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	int32_t statValue = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statValue);
	return statValue;
}
// Gets the unlock status of the Achievement.
Dictionary Steam::getUserAchievement(uint64_t steamID, const String& name){
	Dictionary achieve;
	if(SteamUserStats() == NULL){
		return achieve;
	}
	bool achieved = false;
	CSteamID userID = (uint64)steamID;
	// Get the data from Steam
	bool success = SteamUserStats()->GetUserAchievement(userID, name.utf8().get_data(), &achieved);
	if(success){
		achieve["steam_id"] = steamID;
		achieve["retrieved"] = success;
		achieve["name"] = name;
		achieve["achieved"] = achieved;
	}
	return achieve;
}
// Gets the achievement status, and the time it was unlocked if unlocked.
Dictionary Steam::getUserAchievementAndUnlockTime(uint64_t steamID, const String& name){
	Dictionary achieve;
		if(SteamUserStats() == NULL){
		return achieve;
	}
	bool achieved = false;
	uint32 unlocked = 0;
	CSteamID userID = (uint64)steamID;
	// Get the data from Steam
	bool success = SteamUserStats()->GetUserAchievementAndUnlockTime(userID, name.utf8().get_data(), &achieved, &unlocked);
	if(success){
		achieve["retrieved"] = success;
		achieve["name"] = name;
		achieve["achieved"] = achieved;
		achieve["unlocked"] = unlocked;
	}
	return achieve;
}
// Gets the current value of a float stat for the specified user.
float Steam::getUserStatFloat(uint64_t steamID, const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	float statValue = 0;
	CSteamID userID = (uint64)steamID;
	SteamUserStats()->GetUserStat(userID, name.utf8().get_data(), &statValue);
	return statValue;
}
// Gets the current value of an integer stat for the specified user.
int Steam::getUserStatInt(uint64_t steamID, const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	int32_t statValue = 0;
	CSteamID userID = (uint64)steamID;
	SteamUserStats()->GetUserStat(userID, name.utf8().get_data(), &statValue);
	return statValue;
}
// Achievement progress, triggers an AchievementProgress callback, that is all.
// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
bool Steam::indicateAchievementProgress(const String& name, int currentProgress, int maxProgress){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->IndicateAchievementProgress(name.utf8().get_data(), currentProgress, maxProgress);
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
	if(SteamUserStats() != NULL){
		SteamAPICall_t apiCall = SteamUserStats()->RequestGlobalAchievementPercentages();
		callResultGlobalAchievementPercentagesReady.Set(apiCall, this, &Steam::_global_achievement_percentages_ready);
	}
}
// Asynchronously fetches global stats data, which is available for stats marked as "aggregated" in the App Admin panel of the Steamworks website.  The limit is 60.
void Steam::requestGlobalStats(int historyDays){
	if(SteamUserStats() != NULL){
		SteamAPICall_t apiCall = SteamUserStats()->RequestGlobalStats(historyDays);
		callResultGetGlobalStatsReceived.Set(apiCall, this, &Steam::_global_stats_received);
	}
}
// Asynchronously downloads stats and achievements for the specified user from the server.
void Steam::requestUserStats(uint64_t steamID){
	if(SteamUserStats() != NULL){
		CSteamID userID = (uint64)steamID;
		SteamAPICall_t apiCall = SteamUserStats()->RequestUserStats(userID);
		callResultUserStatsReceived.Set(apiCall, this, &Steam::_user_stats_received);
	}
}
// Reset all Steam statistics; optional to reset achievements.
bool Steam::resetAllStats(bool achievementsToo){
	SteamUserStats()->ResetAllStats(achievementsToo);
	return SteamUserStats()->StoreStats();
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
// Updates an AVGRATE stat with new values.
bool Steam::updateAvgRateStat(const String& name, float thisSession, double sessionLength){
	if(SteamUserStats() == NULL){
		return false;
	}
	return SteamUserStats()->UpdateAvgRateStat(name.utf8().get_data(), thisSession, sessionLength);
}
// Upload a leaderboard score for the user.
void Steam::uploadLeaderboardScore(int score, bool keepBest, PoolIntArray details){
	if(SteamUserStats() != NULL){
		ELeaderboardUploadScoreMethod method = keepBest ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
		int detailsSize = details.size();
		const int32 *detailsPointer = NULL;
		if(detailsSize > 0){
			PoolIntArray::Read r = details.read();
			detailsPointer = r.ptr();
		}
		SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(leaderboardHandle, method, (int32)score, detailsPointer, detailsSize);
		callResultUploadScore.Set(apiCall, this, &Steam::_leaderboard_score_uploaded);
	}
}
// Set the max leaderboard details
void Steam::setLeaderboardDetailsMax(int detailsMax){
	if(SteamUserStats() != NULL){
		leaderboardDetailsMax = detailsMax;
	}
}
// Get the currently used leaderboard entries.
Array Steam::getLeaderboardEntries(){
	if(SteamUserStats() == NULL){
		return Array();
	}
	return leaderboardEntriesArray;
}

/////////////////////////////////////////////////
///// UTILS /////////////////////////////////////
/////////////////////////////////////////////////
//
// Filters the provided input message and places the filtered result into pchOutFilteredText.
String Steam::filterText(const String& message, bool legalOnly){
	String new_message = "";
	if(SteamUtils() != NULL){
		char *filtered = new char[2048];
		SteamUtils()->FilterText(filtered, 2048, message.utf8().get_data(), legalOnly);
		new_message = filtered;
	}
	return new_message;
}
// Used to get the failure reason of a call result. The primary usage for this function is debugging. The failure reasons are typically out of your control and tend to not be very important. Just keep retrying your API Call until it works.
String Steam::getAPICallFailureReason(){
	if(SteamUtils() == NULL){
		return "ERROR: Steam Utils not present.";
	}
	int failure = SteamUtils()->GetAPICallFailureReason(apiHandle);
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
int Steam::getAppID(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetAppID();
}
// Get the amount of battery power, clearly for laptops.
int Steam::getCurrentBatteryPower(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetCurrentBatteryPower();
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
// Returns the number of IPC calls made since the last time this function was called.
uint32 Steam::getIPCCallCount(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetIPCCallCount();
}
// Get the user's country by IP.
String Steam::getIPCountry(){
	if(SteamUtils() == NULL){
		return "";
	}
	return SteamUtils()->GetIPCountry();
}
// Return amount of time, in seconds, user has spent in this session.
int Steam::getSecondsSinceAppActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceAppActive();
}
// Returns the number of seconds since the user last moved the mouse.
int Steam::getSecondsSinceComputerActive(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetSecondsSinceComputerActive();
}
// Get the actual time.
int Steam::getServerRealTime(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetServerRealTime();
}
// Get the Steam user interface language.
String Steam::getSteamUILanguage(){
	if(SteamUtils() == NULL){
		return "";
	}
	return SteamUtils()->GetSteamUILanguage();
}
// Initializes text filtering. Returns false if filtering is unavailable for the language the user is currently running in. If the language is unsupported, the FilterText API will act as a passthrough.
bool Steam::initFilterText(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->InitFilterText();
}
// Checks if an API Call is completed. Provides the backend of the CallResult wrapper.
Dictionary Steam::isAPICallCompleted(){
	Dictionary completed;
	if(SteamUtils() != NULL){
		bool failed = false;
		bool valid = SteamUtils()->IsAPICallCompleted(apiHandle, &failed);
		// Populate the dictionary
		completed["completed"] = valid;
		completed["failed"] = failed;
	}
	return completed;
}
// Returns true/false if Steam overlay is enabled.
bool Steam::isOverlayEnabled(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsOverlayEnabled();
}
// Returns whether the current launcher is a Steam China launcher. You can cause the client to behave as the Steam China launcher by adding -dev -steamchina to the command line when running Steam.
bool Steam::isSteamChinaLauncher(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamChinaLauncher();
}
// Returns true if Steam & the Steam Overlay are running in Big Picture mode.
bool Steam::isSteamInBigPictureMode(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamInBigPictureMode();
}
// Is Steam running in VR?
bool Steam::isSteamRunningInVR(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->IsSteamRunningInVR();
}
// Checks if the HMD view will be streamed via Steam In-Home Streaming.
bool Steam::isVRHeadsetStreamingEnabled(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsVRHeadsetStreamingEnabled();	
}
// Checks if the Overlay needs a present. Only required if using event driven render updates.
bool Steam::overlayNeedsPresent(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->BOverlayNeedsPresent();
}
// Sets the inset of the overlay notification from the corner specified by SetOverlayNotificationPosition.
void Steam::setOverlayNotificationInset(int horizontal, int vertical){
	if(SteamUtils() != NULL){
		SteamUtils()->SetOverlayNotificationInset(horizontal, vertical);
	}
}
// Set the position where overlay shows notifications.
void Steam::setOverlayNotificationPosition(int pos){
	if((pos > 0) || (pos < 4) || (SteamUtils() != NULL)){
		SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
	}
}
// Set whether the HMD content will be streamed via Steam In-Home Streaming.
void Steam::setVRHeadsetStreamingEnabled(bool enabled){
	if(SteamUtils() != NULL){
		SteamUtils()->SetVRHeadsetStreamingEnabled(enabled);
	}
}
// Activates the Big Picture text input dialog which only supports gamepad input.
bool Steam::showGamepadTextInput(int inputMode, int lineInputMode, const String& description, uint32 maxText, const String& presetText){
	if(SteamUtils() == NULL){
		return false;
	}
	// Convert modes
	EGamepadTextInputMode mode;
	if(inputMode == 0){
		mode = k_EGamepadTextInputModeNormal;
	}
	else{
		mode = k_EGamepadTextInputModePassword;
	}
	EGamepadTextInputLineMode lineMode;
	if(lineInputMode == 0){
		lineMode = k_EGamepadTextInputLineModeSingleLine;
	}
	else{
		lineMode = k_EGamepadTextInputLineModeMultipleLines;
	}
	return SteamUtils()->ShowGamepadTextInput(mode, lineMode, description.utf8().get_data(), maxText, presetText.utf8().get_data());
}
// Ask SteamUI to create and render its OpenVR dashboard.
void Steam::startVRDashboard(){
	if(SteamUtils() != NULL){
		SteamUtils()->StartVRDashboard();
	}
}

/////////////////////////////////////////////////
///// VIDEO /////////////////////////////////////
/////////////////////////////////////////////////
//
// Get the OPF details for 360 video playback.
void Steam::getOPFSettings(int appID){
	if(SteamVideo() != NULL){
		SteamVideo()->GetOPFSettings((AppId_t)appID);
	}
}
// Gets the OPF string for the specified video App ID.
String Steam::getOPFStringForApp(int appID){
	String opf_string = "";
	if(SteamVideo() != NULL){
		int32 size = 48000;
		char *buffer = new char[size];
		if(SteamVideo()->GetOPFStringForApp((AppId_t)appID, buffer, &size)){
			opf_string = buffer;
		}
	}
	return opf_string;
}
// Asynchronously gets the URL suitable for streaming the video associated with the specified video app ID.
void Steam::getVideoURL(int appID){
	if(SteamVideo() != NULL){
		SteamVideo()->GetVideoURL((AppId_t)appID);
	}
}
// Checks if the user is currently live broadcasting and gets the number of users.
Dictionary Steam::isBroadcasting(){
	Dictionary broadcast;
	if(SteamVideo() != NULL){
		int viewers = 0;
		bool broadcasting = SteamVideo()->IsBroadcasting(&viewers);
		// Populate the dictionary
		broadcast["broadcasting"] = broadcasting;
		broadcast["viewers"] = viewers;
	}
	return broadcast;
}

/////////////////////////////////////////////////
///// SIGNALS / CALLBACKS ///////////////////////
/////////////////////////////////////////////////
// Apps callbacks ///////////////////////////////
//
// Triggered after the current user gains ownership of DLC and that DLC is installed.
void Steam::_dlc_installed(DlcInstalled_t* callData){
	int appID = (AppId_t)callData->m_nAppID;
	emit_signal("dlc_installed", appID);
}
// Called after requesting the details of a specific file.
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
// Posted after the user executes a steam url with command line or query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running. The new params can be queried with getLaunchCommandLine and getLaunchQueryParam.
void Steam::_new_launch_url_parameters(NewUrlLaunchParameters_t* callData){
	emit_signal("new_launch_url_parameters");
}
// Posted after the user executes a steam url with query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running. The new params can be queried with getLaunchQueryParam.
// ISSUE: when compiling says it is an undeclared identifier
// void Steam::_new_launch_query_parameters(NewLaunchQueryParameters_t* callData){
//	emit_signal("new_launch_query_parameters");
//}

// Friends callbacks ////////////////////////////
//
// Called when a large avatar is loaded if you have tried requesting it when it was unavailable.
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
// Called when a Steam group activity has received.
void Steam::_clan_activity_downloaded(DownloadClanActivityCountsResult_t* callData){
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
// Called when Rich Presence data has been updated for a user, this can happen automatically when friends in the same game update their rich presence, or after a call to requestFriendRichPresence.
void Steam::_friend_rich_presence_update(FriendRichPresenceUpdate_t* callData){
	uint64_t steamID = callData->m_steamIDFriend.ConvertToUint64();
	AppId_t appID = callData->m_nAppID;
	emit_signal("friend_rich_presence_updated", steamID, appID);
}
// Returns the result of enumerateFollowingList.
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
// Returns the result of getFollowerCount.
void Steam::_get_follower_count(FriendsGetFollowerCount_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	uint64_t steamID = callData->m_steamID.ConvertToUint64();
	int count = callData->m_nCount;
	emit_signal("follower_count", result, steamID, count);
}
// Returns the result of isFollowing.
void Steam::_is_following(FriendsIsFollowing_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	uint64_t steamID = callData->m_steamID.ConvertToUint64();
	bool following = callData->m_bIsFollowing;
	emit_signal("is_following", result, steamID, following);
}
// Called when a user has joined a Steam group chat that the we are in.
void Steam::_connected_chat_join(GameConnectedChatJoin_t* callData){
	uint64_t chatID = callData->m_steamIDClanChat.ConvertToUint64();
	uint64_t steamID = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("chat_joined", chatID, steamID);
}
// Called when a user has left a Steam group chat that the we are in.
void Steam::_connected_chat_leave(GameConnectedChatLeave_t* callData){
	uint64_t chatID = callData->m_steamIDClanChat.ConvertToUint64();
	uint64_t steamID = callData->m_steamIDUser.ConvertToUint64();
	bool kicked = callData->m_bKicked;
	bool dropped = callData->m_bDropped;
	emit_signal("chat_left", chatID, steamID, kicked, dropped);
}
// Called when a chat message has been received in a Steam group chat that we are in.
void Steam::_connected_clan_chat_message(GameConnectedClanChatMsg_t* callData){
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
// Called when chat message has been received from a friend
void Steam::_connected_friend_chat_message(GameConnectedFriendChatMsg_t* callData){
	uint64_t steamID = callData->m_steamIDUser.ConvertToUint64();
	int message = callData->m_iMessageID;
	Dictionary chat;
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	chat["ret"] = SteamFriends()->GetFriendMessage(createSteamID(steamID), message, text, 2048, &type);
	chat["text"] = String(text);
	emit_signal("friend_chat_message", chat);
}
// Called when the user tries to join a lobby from their friends list or from an invite. The game client should attempt to connect to specified lobby when this is received. If the game isn't running yet then the game will be automatically launched with the command line parameter +connect_lobby <64-bit lobby Steam ID> instead.
void Steam::_join_requested(GameLobbyJoinRequested_t* callData){
	CSteamID lobbyID = callData->m_steamIDLobby;
	uint64_t lobby = lobbyID.ConvertToUint64();
	CSteamID friendID = callData->m_steamIDFriend;
	uint64_t steamID = friendID.ConvertToUint64();
	emit_signal("join_requested", lobby, steamID);
}
// Posted when the Steam Overlay activates or deactivates. The game can use this to be pause or resume single player games.
void Steam::_overlay_toggled(GameOverlayActivated_t* callData){
	if(callData->m_bActive){
		emit_signal("overlay_toggled", true);
	}
	else{
		emit_signal("overlay_toggled", false);
	}
}
// Called when the user tries to join a game from their friends list or after a user accepts an invite by a friend with inviteUserToGame.
void Steam::_join_game_requested(GameRichPresenceJoinRequested_t* callData){
	CSteamID steamID = callData->m_steamIDFriend;
	uint64_t user = steamID.ConvertToUint64();
	String connect = callData->m_rgchConnect;
	emit_signal("join_game_requested", user, connect);
}
// This callback is made when joining a game. If the user is attempting to join a lobby, then the callback GameLobbyJoinRequested_t will be made.
void Steam::_change_server_requested(GameServerChangeRequested_t* callData){
	String server = callData->m_rgchServer;
	String password = callData->m_rgchPassword;
	emit_signal("change_server_requested", server, password);
}
void Steam::_join_clan_chat_complete(JoinClanChatRoomCompletionResult_t* callData){
	uint64_t chatID = callData->m_steamIDClanChat.ConvertToUint64();
	EChatRoomEnterResponse response = callData->m_eChatRoomEnterResponse;
	emit_signal("chat_join_complete", chatID, response);
}
// Signal for a user change
void Steam::_persona_state_change(PersonaStateChange_t* callData){
	uint64_t steamID = callData->m_ulSteamID;
	int flags = callData->m_nChangeFlags;
	emit_signal("persona_state_change", steamID, flags);
}
// Reports the result of an attempt to change the user's persona name.
void Steam::_name_changed(SetPersonaNameResponse_t* callData){
	bool success = callData->m_bSuccess;
	bool localSuccess = callData->m_bLocalSuccess;
	EResult result = callData->m_result;
	emit_signal("name_changed", success, localSuccess, result);
}
// Dispatched when an overlay browser instance is navigated to a protocol/scheme registered by RegisterProtocolInOverlayBrowser().
void Steam::_overlay_browser_protocol(OverlayBrowserProtocolNavigation_t* callData){
	String uri = callData->rgchURI;
	emit_signal("overlay_browser_protocol", uri);
}

// HTML Surface callbacks ///////////////////////
// 
// A new browser was created and is ready for use.
void Steam::_html_browser_ready(HTML_BrowserReady_t* callData){
	browserHandle = callData->unBrowserHandle;
	emit_signal("html_browser_ready");
}
// Called when page history status has changed the ability to go backwards and forward.
void Steam::_html_can_go_backandforward(HTML_CanGoBackAndForward_t* callData){
	browserHandle = callData->unBrowserHandle;
	bool goBack = callData->bCanGoBack;
	bool goForward = callData->bCanGoForward;
	emit_signal("html_can_go_backandforward", goBack, goForward);
}
// Called when the current page in a browser gets a new title.
void Steam::_html_changed_title(HTML_ChangedTitle_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& title = callData->pchTitle;
	emit_signal("html_changed_title", title);
}
// Called when the browser has been requested to close due to user interaction; usually because of a javascript window.close() call.
void Steam::_html_close_browser(HTML_CloseBrowser_t* callData){
	browserHandle = callData->unBrowserHandle;
	emit_signal("html_close_browser");
}
// Called when a browser surface has received a file open dialog from a <input type="file"> click or similar, you must call FileLoadDialogResponse with the file(s) the user selected.
void Steam::_html_file_open_dialog(HTML_FileOpenDialog_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& title = callData->pchTitle;
	const String& initialFile = callData->pchInitialFile;
	emit_signal("html_file_open_dialog", title, initialFile);
}
// Called when a browser has finished loading a page.
void Steam::_html_finished_request(HTML_FinishedRequest_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& url = callData->pchURL;
	const String& title = callData->pchPageTitle;
	emit_signal("html_finished_request", url, title);
}
// Called when a a browser wants to hide a tooltip.
void Steam::_html_hide_tooltip(HTML_HideToolTip_t* callData){
	browserHandle = callData->unBrowserHandle;
	emit_signal("html_hide_tooltip");
}
// Provides details on the visibility and size of the horizontal scrollbar.
void Steam::_html_horizontal_scroll(HTML_HorizontalScroll_t* callData){
	browserHandle = callData->unBrowserHandle;
	// Create dictionary to bypass argument limit in Godot
	Dictionary scrollData;
	scrollData["scrollMax"] = callData->unScrollMax;
	scrollData["scrollCurrent"] = callData->unScrollCurrent;
	scrollData["pageScale"] = callData->flPageScale;
	scrollData["visible"] = callData->bVisible;
	scrollData["pageSize"] = callData->unPageSize;
	emit_signal("html_horizontal_scroll", scrollData);
}
// Called when the browser wants to display a Javascript alert dialog, call JSDialogResponse when the user dismisses this dialog; or right away to ignore it.
void Steam::_html_js_alert(HTML_JSAlert_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& message = callData->pchMessage;
	emit_signal("html_js_alert", message);
}
// Called when the browser wants to display a Javascript confirmation dialog, call JSDialogResponse when the user dismisses this dialog; or right away to ignore it.
void Steam::_html_js_confirm(HTML_JSConfirm_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& message = callData->pchMessage;
	emit_signal("html_js_confirm", message);
}
// Result of a call to GetLinkAtPosition.
void Steam::_html_link_at_position(HTML_LinkAtPosition_t* callData){
	browserHandle = callData->unBrowserHandle;
	// Create dictionary to bypass Godot argument limit
	Dictionary linkData;
	linkData["x"] = callData->x;
	linkData["y"] = callData->y;
	linkData["url"] = callData->pchURL;
	linkData["input"] = callData->bInput;
	linkData["liveLink"] = callData->bLiveLink;
	emit_signal("html_link_at_position", linkData);
}
// Called when a browser surface has a pending paint. This is where you get the actual image data to render to the screen.
void Steam::_html_needs_paint(HTML_NeedsPaint_t* callData){
	browserHandle = callData->unBrowserHandle;
	// Create dictionary to bypass Godot argument limit
	Dictionary pageData;
	pageData["bgra"] = callData->pBGRA;
	pageData["wide"] = callData->unWide;
	pageData["tall"] = callData->unTall;
	pageData["updateX"] = callData->unUpdateX;
	pageData["updateY"] = callData->unUpdateY;
	pageData["updateWide"] = callData->unUpdateWide;
	pageData["updateTall"] = callData->unUpdateTall;
	pageData["scrollX"] = callData->unScrollX;
	pageData["scrollY"] = callData->unScrollY;
	pageData["pageScale"] = callData->flPageScale;
	pageData["pageSerial"] = callData->unPageSerial;
	emit_signal("html_needs_paint", pageData);
}
// A browser has created a new HTML window.
void Steam::_html_new_window(HTML_NewWindow_t* callData){
	browserHandle = callData->unBrowserHandle;
	// Create a dictionary to bypass Godot argument limit
	Dictionary windowData;
	windowData["url"] = callData->pchURL;
	windowData["x"] = callData->unX;
	windowData["y"] = callData->unY;
	windowData["wide"] = callData->unWide;
	windowData["tall"] = callData->unTall;
	windowData["newHandle"] = callData->unNewWindow_BrowserHandle_IGNORE;
	emit_signal("html_new_window", windowData);
}
// The browser has requested to load a url in a new tab.
void Steam::_html_open_link_in_new_tab(HTML_OpenLinkInNewTab_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& url = callData->pchURL;
	emit_signal("html_open_link_in_new_tab", url);
}
// Results from a search.
void Steam::_html_search_results(HTML_SearchResults_t* callData){
	browserHandle = callData->unBrowserHandle;
	uint32 results = callData->unResults;
	uint32 currentMatch = callData->unCurrentMatch;
	emit_signal("html_search_results", results, currentMatch);
}
// Called when a browser wants to change the mouse cursor.
void Steam::_html_set_cursor(HTML_SetCursor_t* callData){
	browserHandle = callData->unBrowserHandle;
	uint32 mouseCursor = callData->eMouseCursor;
	emit_signal("html_set_cursor", mouseCursor);
}
// Called when a browser wants to display a tooltip.
void Steam::_html_show_tooltip(HTML_ShowToolTip_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& message = callData->pchMsg;
	emit_signal("html_show_tooltip", message);
}
// Called when a browser wants to navigate to a new page.
void Steam::_html_start_request(HTML_StartRequest_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& url = callData->pchURL;
	const String& target = callData->pchTarget;
	const String& postData = callData->pchPostData;
	bool redirect = callData->bIsRedirect;
	emit_signal("html_start_request", url, target, postData, redirect);
}
// Called when a browser wants you to display an informational message. This is most commonly used when you hover over links.
void Steam::_html_status_text(HTML_StatusText_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& message = callData->pchMsg;
	emit_signal("html_status_text", message);
}
// Called when the text of an existing tooltip has been updated.
void Steam::_html_update_tooltip(HTML_UpdateToolTip_t* callData){
	browserHandle = callData->unBrowserHandle;
	const String& message = callData->pchMsg;
	emit_signal("html_update_tooltip", message);
}
// Called when the browser is navigating to a new url.
void Steam::_html_url_changed(HTML_URLChanged_t* callData){
	browserHandle = callData->unBrowserHandle;
	// Create a dictionary to bypass Godot argument limit
	Dictionary urlData;
	urlData["url"] = callData->pchURL;
	urlData["postData"] = callData->pchPostData;
	urlData["redirect"] = callData->bIsRedirect;
	urlData["title"] = callData->pchPageTitle;
	urlData["newNavigation"] = callData->bNewNavigation;
	emit_signal("html_url_changed", urlData);
}
// Provides details on the visibility and size of the vertical scrollbar.
void Steam::_html_vertical_scroll(HTML_VerticalScroll_t* callData){
	browserHandle = callData->unBrowserHandle;
	// Create dictionary to bypass argument limit in Godot
	Dictionary scrollData;
	scrollData["scrollMax"] = callData->unScrollMax;
	scrollData["scrollCurrent"] = callData->unScrollCurrent;
	scrollData["pageScale"] = callData->flPageScale;
	scrollData["visible"] = callData->bVisible;
	scrollData["pageSize"] = callData->unPageSize;
	emit_signal("html_vertical_scroll", scrollData);
}

// HTTP callbacks ///////////////////////////////
//
// Result when an HTTP request completes. If you're using GetHTTPStreamingResponseBodyData then you should be using the HTTPRequestHeadersReceived_t or HTTPRequestDataReceived_t.
void Steam::_http_request_completed(HTTPRequestCompleted_t* callData){
	cookieHandle = callData->m_hRequest;
	uint64_t contextValue = callData->m_ulContextValue;
	bool requestSuccess = callData->m_bRequestSuccessful;
	int statusCode = callData->m_eStatusCode;
	uint32 bodySize = callData->m_unBodySize;
	emit_signal("http_request_completed", contextValue, requestSuccess, statusCode, bodySize);
}
// Triggered when a chunk of data is received from a streaming HTTP request.
void Steam::_http_request_data_received(HTTPRequestDataReceived_t* callData){
	cookieHandle = callData->m_hRequest;
	uint64_t contextValue = callData->m_ulContextValue;
	uint32 offset = callData->m_cOffset;
	uint32 bytesReceived = callData->m_cBytesReceived;
	emit_signal("http_request_data_received", contextValue, offset, bytesReceived);
}
// Triggered when HTTP headers are received from a streaming HTTP request.
void Steam::_http_request_headers_received(HTTPRequestHeadersReceived_t* callData){
	cookieHandle = callData->m_hRequest;
	uint64_t contextValue = callData->m_ulContextValue;
	emit_signal("http_request_headers_received", contextValue);
}

// Inventory callbacks //////////////////////////
//
// This callback is triggered whenever item definitions have been updated, which could be in response to LoadItemDefinitions or any time new item definitions are available (eg, from the dynamic addition of new item types while players are still in-game).
void Steam::_inventory_definition_update(SteamInventoryDefinitionUpdate_t *callData){
	// Create the return array
	Array definitions;
	// Set the array size variable
	uint32 size = 0;
	// Get the item defition IDs
	if(SteamInventory()->GetItemDefinitionIDs(NULL, &size)){
		SteamItemDef_t *idArray = new SteamItemDef_t[size];
		if(SteamInventory()->GetItemDefinitionIDs(idArray, &size)){
			// Loop through the temporary array and populate the return array
			for(uint32 i = 0; i < size; i++){
				definitions.append(idArray[i]);
			}
		}
		// Delete the temporary array
		delete idArray;
	}
	// Return the item array as a signal
	emit_signal("inventory_defintion_update", definitions);
}
// Returned when you have requested the list of "eligible" promo items that can be manually granted to the given user. These are promo items of type "manual" that won't be granted automatically.
void Steam::_inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *callData, bool bIOFailure){
	// Clean up call data
	CSteamID steamID = callData->m_steamID;
	int result = callData->m_result;
	int eligible = callData->m_numEligiblePromoItemDefs;
	bool cached = callData->m_bCachedData;
	// Create the return array
	Array definitions;
	// Create the temporary ID array
	SteamItemDef_t *idArray = new SteamItemDef_t[eligible];
	// Convert eligible size
	uint32 arraySize = (int)eligible;
	// Get the list
	if(SteamInventory()->GetEligiblePromoItemDefinitionIDs(steamID, idArray, &arraySize)){
		// Loop through the temporary array and populate the return array
		for(int i = 0; i < eligible; i++){
			definitions.append(idArray[i]);
		}
	}
	// Delete the temporary array
	delete idArray;
	// Return the item array as a signal
	emit_signal("inventory_eligible_promo_Item", result, cached, definitions);
}
// Triggered when GetAllItems successfully returns a result which is newer / fresher than the last known result. (It will not trigger if the inventory hasn't changed, or if results from two overlapping calls are reversed in flight and the earlier result is already known to be stale/out-of-date.)
// The regular SteamInventoryResultReady_t callback will still be triggered immediately afterwards; this is an additional notification for your convenience.
void Steam::_inventory_full_update(SteamInventoryFullUpdate_t *callData){
	// Set the handle
	inventoryHandle = callData->m_handle;
	// Send the handle back to the user
	emit_signal("inventory_full_update", callData->m_handle);
}
// This is fired whenever an inventory result transitions from k_EResultPending to any other completed state, see GetResultStatus for the complete list of states. There will always be exactly one callback per handle.
void Steam::_inventory_result_ready(SteamInventoryResultReady_t *callData){
	emit_signal("inventory_result_ready", callData->m_handle, callData->m_result);
}
// Returned after StartPurchase is called.
void Steam::_inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *callData, bool bIOFailure){
	if(!bIOFailure){
		if(callData->m_result == k_EResultOK){
			emit_signal("inventory_start_purchase_result", "success", (uint64_t)callData->m_ulOrderID, (uint64_t)callData->m_ulTransID);
		}
		else{
			emit_signal("inventory_start_purchase_result", "failure", 0, 0);
		}
	}
}
// Returned after RequestPrices is called.
void Steam::_inventory_request_prices_result(SteamInventoryRequestPricesResult_t *callData, bool bIOFailure){
	if(!bIOFailure){
		emit_signal("inventory_request_prices_result", callData->m_result, callData->m_rgchCurrency);
	}
}

// Matchmaking callbacks ////////////////////////
//
// Called when an account on your favorites list is updated
void Steam::_favorites_list_accounts_updated(FavoritesListAccountsUpdated_t* callData){
	emit_signal("favorites_list_accounts_updated", callData->m_eResult);
}
// A server was added/removed from the favorites list, you should refresh now.
void Steam::_favorites_list_changed(FavoritesListChanged_t* callData){
	Dictionary favorite;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char favoriteIP[16];
	for(int j = 0; j < NBYTES; j++){
		octet[j] = callData->m_nIP >> (j * 8);
	}
	sprintf(favoriteIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
	favorite["ip"] = favoriteIP; 
	favorite["queryPort"] = callData->m_nQueryPort;
	favorite["connPort"] = callData->m_nConnPort;
	favorite["appID"] = callData->m_nAppID;
	favorite["flags"] = callData->m_nFlags;
	favorite["add"] = callData->m_bAdd;
	favorite["accountID"] = callData->m_unAccountId;
	emit_signal("favorites_list_changed", favorite);
}
// Signal when a lobby chat message is received
void Steam::_lobby_message(LobbyChatMsg_t* callData){
	CSteamID userID = callData->m_ulSteamIDUser;
	uint8 chatType = callData->m_eChatEntryType;
	// Convert the chat type over
	EChatEntryType type = (EChatEntryType)chatType;
	// Get the chat message data
	char buffer[4096] = {0};
	int result = SteamMatchmaking()->GetLobbyChatEntry(callData->m_ulSteamIDLobby, callData->m_iChatID, &userID, &buffer, 4096, &type);
	uint64_t user = userID.ConvertToUint64();
	emit_signal("lobby_message", result, user, String(buffer), chatType);
}
// A lobby chat room state has changed, this is usually sent when a user has joined or left the lobby.
void Steam::_lobby_chat_update(LobbyChatUpdate_t* callData){
	uint64_t lobbyID = callData->m_ulSteamIDLobby;
	uint64_t changedID = callData->m_ulSteamIDUserChanged;
	uint64_t makingChangeID = callData->m_ulSteamIDMakingChange;
	uint32 chatState = callData->m_rgfChatMemberStateChange;
	emit_signal("lobby_chat_update", lobbyID, changedID, makingChangeID, chatState);
}
// Signal the lobby has been created.
void Steam::_lobby_created(LobbyCreated_t *lobbyData, bool bIOFailure){
	int connect = lobbyData->m_eResult;
	CSteamID lobbyID = lobbyData->m_ulSteamIDLobby;
	uint64_t lobby = lobbyID.ConvertToUint64();
	emit_signal("lobby_created", connect, lobby);
}
// The lobby metadata has changed.
void Steam::_lobby_data_update(LobbyDataUpdate_t* callData){
	uint64_t memberID = callData->m_ulSteamIDMember;
	uint64_t lobbyID = callData->m_ulSteamIDLobby;
	uint8 success = callData->m_bSuccess;
	char key;
	// Is the lobby
	if(memberID == lobbyID){
		SteamMatchmaking()->GetLobbyMemberData(callData->m_ulSteamIDLobby, callData->m_ulSteamIDMember, &key);
	}
	// Is a user in the lobby
	else{
		SteamMatchmaking()->GetLobbyData(callData->m_ulSteamIDLobby, &key);
	}
	emit_signal("lobby_data_update", success, lobbyID, memberID, key);
}
// Recieved upon attempting to enter a lobby. Lobby metadata is available to use immediately after receiving this.
void Steam::_lobby_joined(LobbyEnter_t* lobbyData){
	CSteamID steamLobbyID = lobbyData->m_ulSteamIDLobby;
	uint64_t lobbyID = steamLobbyID.ConvertToUint64();
	uint32_t permissions = lobbyData->m_rgfChatPermissions;
	bool locked = lobbyData->m_bLocked;
	uint32_t response = lobbyData->m_EChatRoomEnterResponse;
	emit_signal("lobby_joined", lobbyID, permissions, locked, response);
}
// A game server has been set via SetLobbyGameServer for all of the members of the lobby to join. It's up to the individual clients to take action on this; the typical game behavior is to leave the lobby and connect to the specified game server; but the lobby may stay open throughout the session if desired.
void Steam::_lobby_game_created(LobbyGameCreated_t* callData){
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
	sprintf(serverIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
	emit_signal("lobby_game_created", lobbyID, serverID, serverIP, port);
}
// Someone has invited you to join a Lobby. Normally you don't need to do anything with this, as the Steam UI will also display a '<user> has invited you to the lobby, join?' notification and message. If the user outside a game chooses to join, your game will be launched with the parameter +connect_lobby <64-bit lobby id>, or with the callback GameLobbyJoinRequested_t if they're already in-game.
void Steam::_lobby_invite(LobbyInvite_t* lobbyData){
	CSteamID inviterID = lobbyData->m_ulSteamIDUser;
	uint64_t inviter = inviterID.ConvertToUint64();
	CSteamID lobbyID = lobbyData->m_ulSteamIDLobby;
	uint64_t lobby = lobbyID.ConvertToUint64();
	CSteamID gameID = lobbyData->m_ulGameID;
	uint64_t game = gameID.ConvertToUint64();
	emit_signal("lobby_invite", inviter, lobby, game);
}
// Result when requesting the lobby list. You should iterate over the returned lobbies with getLobbyByIndex, from 0 to m_nLobbiesMatching-1.
void Steam::_lobby_match_list(LobbyMatchList_t *callData, bool bIOFailure){
	int lobbyCount = callData->m_nLobbiesMatching;
	Array lobbies;
	for(int i = 0; i < lobbyCount; i++){
		CSteamID lobbyID = SteamMatchmaking()->GetLobbyByIndex(i);
		uint64_t lobby = lobbyID.ConvertToUint64();
		lobbies.append(lobby);
	}	
	emit_signal("lobby_match_list", lobbies);
}

// Music Remote callbacks ///////////////////////
//
// The majority of callback for Music Remote have no fields and no descriptions. They seem to be primarily fired as responses to functions.
void Steam::_music_player_remote_to_front(MusicPlayerRemoteToFront_t* callData){
	emit_signal("music_player_remote_to_front");
}
void Steam::_music_player_remote_will_activate(MusicPlayerRemoteWillActivate_t* callData){
	emit_signal("music_player_remote_will_activate");
}
void Steam::_music_player_remote_will_deactivate(MusicPlayerRemoteWillDeactivate_t* callData){
	emit_signal("music_player_remote_will_deactivate");
}
void Steam::_music_player_selects_playlist_entry(MusicPlayerSelectsPlaylistEntry_t* callData){
	int entry = callData->nID;
	emit_signal("music_player_selects_playlist_entry", entry);
}
void Steam::_music_player_selects_queue_entry(MusicPlayerSelectsQueueEntry_t* callData){
	int entry = callData->nID;
	emit_signal("music_player_selects_queue_entry", entry);
}
void Steam::_music_player_wants_looped(MusicPlayerWantsLooped_t* callData){
	bool looped = callData->m_bLooped;
	emit_signal("music_player_wants_looped", looped);
}
void Steam::_music_player_wants_pause(MusicPlayerWantsPause_t* callData){
	emit_signal("music_player_wants_pause");
}
void Steam::_music_player_wants_playing_repeat_status(MusicPlayerWantsPlayingRepeatStatus_t* callData){
	int status = callData->m_nPlayingRepeatStatus;
	emit_signal("music_player_wants_playing_repeat_status", status);
}
void Steam::_music_player_wants_play_next(MusicPlayerWantsPlayNext_t* callData){
	emit_signal("music_player_wants_play_next");
}
void Steam::_music_player_wants_play_previous(MusicPlayerWantsPlayPrevious_t* callData){
	emit_signal("music_player_wants_play_previous");
}
void Steam::_music_player_wants_play(MusicPlayerWantsPlay_t* callData){
	emit_signal("music_player_wants_play");
}
void Steam::_music_player_wants_shuffled(MusicPlayerWantsShuffled_t* callData){
	bool shuffled = callData->m_bShuffled;
	emit_signal("music_player_wants_shuffled", shuffled);
}
void Steam::_music_player_wants_volume(MusicPlayerWantsVolume_t* callData){
	float volume = callData->m_flNewVolume;
	emit_signal("music_player_wants_volume", volume);
}
void Steam::_music_player_will_quit(MusicPlayerWillQuit_t* callData){
	emit_signal("music_player_will_quit");
}

// Networking callbacks /////////////////////////
//
// Called when packets can't get through to the specified user. All queued packets unsent at this point will be dropped, further attempts to send will retry making the connection (but will be dropped if we fail again).
void Steam::_p2p_session_connect_fail(P2PSessionConnectFail_t* callData) {
	uint64_t steamIDRemote = callData->m_steamIDRemote.ConvertToUint64();
	uint8_t sessionError = callData->m_eP2PSessionError;
	emit_signal("p2p_session_connect_fail", steamIDRemote, sessionError);
}
// A user wants to communicate with us over the P2P channel via the sendP2PPacket. In response, a call to acceptP2PSessionWithUser needs to be made, if you want to open the network channel with them.
void Steam::_p2p_session_request(P2PSessionRequest_t* callData){
	uint64_t steamIDRemote = callData->m_steamIDRemote.ConvertToUint64();
	emit_signal("p2p_session_request", steamIDRemote);
}

// Parties callbacks ////////////////////////////
//
// This callback is used as a call response for ISteamParties::JoinParty. On success, you will have reserved a slot in the beacon-owner's party, and should use m_rgchConnectString to connect to their game and complete the process.
void Steam::_join_party(JoinPartyCallback_t* callData){
	int result = callData->m_eResult;
	uint64_t beaconID = callData->m_ulBeaconID;
	uint64_t steamID = callData->m_SteamIDBeaconOwner.ConvertToUint64();
	String connectString = callData->m_rgchConnectString;
	emit_signal("join_party", result, beaconID, steamID, connectString);
}
// This callback is used as a call response for ISteamParties::CreateBeacon. If successful, your beacon has been posted in the desired location and you may start receiving ISteamParties::ReservationNotificationCallback_t callbacks for users following the beacon. 
void Steam::_create_beacon(CreateBeaconCallback_t* callData){
	int result = callData->m_eResult;
	uint64_t beaconID = callData->m_ulBeaconID;
	emit_signal("create_beacon", result, beaconID);
}
// After creating a beacon, when a user "follows" that beacon Steam will send you this callback to know that you should be prepared for the user to join your game. When they do join, be sure to call ISteamParties::OnReservationCompleted to let Steam know.
void Steam::_reservation_notification(ReservationNotificationCallback_t* callData){
	uint64_t beaconID = callData->m_ulBeaconID;
	uint64_t steamID = callData->m_steamIDJoiner.ConvertToUint64();
	emit_signal("reservation_notifications", beaconID, steamID);
}
// Call result for ISteamParties::ChangeNumOpenSlots. 
void Steam::_change_num_open_slots(ChangeNumOpenSlotsCallback_t* callData){
	int result = callData->m_eResult;
	emit_signal("change_num_open_slots", result);
}
// Notification that the list of available locations for posting a beacon has been updated. 
void Steam::_available_beacon_locations_updated(AvailableBeaconLocationsUpdated_t* callData){
	emit_signal("available_beacon_locations_updated");
}
// Notification that the list of active beacons visible to the current user has changed. 
void Steam::_active_beacons_updated(ActiveBeaconsUpdated_t* callData){
	emit_signal("active_beacons_updated");
}

// Remote Play callbacks ////////////////////////
//
// The session ID of the session that just connected.
void Steam::_remote_play_session_connected(SteamRemotePlaySessionConnected_t* callData){
	uint32 sessionID = callData->m_unSessionID;
	emit_signal("remote_play_session_connected", sessionID);
}
// The session ID of the session that just disconnected.
void Steam::_remote_play_session_disconnected(SteamRemotePlaySessionDisconnected_t* callData){
	uint32 sessionID = callData->m_unSessionID;
	emit_signal("remote_play_session_disconnected", sessionID);
}

// Remote Storage callbacks /////////////////////
//
// Response when reading a file asyncrounously with FileReadAsync.
void Steam::_file_read_async_complete(RemoteStorageFileReadAsyncComplete_t* callData, bool bIOFailure){
	uint64_t handle = callData->m_hFileReadAsync;
	int result = callData->m_eResult;
	uint32 offset = callData->m_nOffset;
	uint32 read = callData->m_cubRead;
	// Was read complete?
	PoolByteArray buffer;
	buffer.resize(read);
	bool complete = SteamRemoteStorage()->FileReadAsyncComplete(handle, buffer.write().ptr(), read);
	emit_signal("file_read_async_complete", handle, result, offset, read, complete);
}
// Response to a file being shared.
void Steam::_file_share_result(RemoteStorageFileShareResult_t* callData, bool bIOFailure){
	int result = callData->m_eResult;
	uint64_t handle = callData->m_hFile;
	char name = callData->m_rgchFilename[k_cchFilenameMax];
	emit_signal("file_share_result", result, handle, name);
}
// Response when writing a file asyncrounously with FileWriteAsync.
void Steam::_file_write_async_complete(RemoteStorageFileWriteAsyncComplete_t* callData, bool bIOFailure){
	int result = callData->m_eResult;
	emit_signal("file_write_async_complete", result);
}
// Response when downloading UGC
void Steam::_download_ugc_result(RemoteStorageDownloadUGCResult_t* callData, bool bIOFailure){
	int result = callData->m_eResult;
	uint64_t handle = callData->m_hFile;
	int appID = callData->m_nAppID;
	int32 size = callData->m_nSizeInBytes;
	char filename = callData->m_pchFileName[k_cchFilenameMax];
	uint64_t ownerID = callData->m_ulSteamIDOwner;
	// Pass some variable to download dictionary to bypass argument limit
	Dictionary download_data;
	download_data["handle"] = handle;
	download_data["appID"] = appID;
	download_data["size"] = size;
	download_data["filename"] = filename;
	download_data["ownerID"] = ownerID;
	emit_signal("download_ugc_result", result, download_data);
}
// Called when the user has unsubscribed from a piece of UGC. Result from ISteamUGC::UnsubscribeItem.
void Steam::_unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t* callData, bool bIOFailure){
	int result = callData->m_eResult;
	int fileID = callData->m_nPublishedFileId;
	emit_signal("unsubscribe_item", result, fileID);
}
// Called when the user has subscribed to a piece of UGC. Result from ISteamUGC::SubscribeItem.
void Steam::_subscribe_item(RemoteStorageSubscribePublishedFileResult_t* callData, bool bIOFailure){
	int result = callData->m_eResult;
	int fileID = callData->m_nPublishedFileId;
	emit_signal("subscribe_item", result, fileID);
}

// Screenshot callbacks /////////////////////////
//
// A screenshot successfully written or otherwise added to the library and can now be tagged.
void Steam::_screenshot_ready(ScreenshotReady_t* callData){
	uint32_t handle = callData->m_hLocal;
	uint32_t result = callData->m_eResult;
	emit_signal("screenshot_ready", handle, result);
}
// A screenshot has been requested by the user from the Steam screenshot hotkey. This will only be called if hookScreenshots has been enabled, in which case Steam will not take the screenshot itself.
void Steam::_screenshot_requested(ScreenshotRequested_t* callData){
	emit_signal("screenshot_requested");
}

// UGC callbacks ////////////////////////////////
//
// The result of a call to AddAppDependency.
void Steam::_add_app_dependency_result(AddAppDependencyResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	AppId_t appID = callData->m_nAppID;
	emit_signal("add_app_dependency_result", result, (uint64_t)fileID, (int)appID);
}
// The result of a call to AddDependency.
void Steam::_add_ugc_dependency_result(AddUGCDependencyResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	PublishedFileId_t childID = callData->m_nChildPublishedFileId;
	emit_signal("add_ugc_dependency_result", result, (uint64_t)fileID, (uint64_t)childID);
}
// Result of a workshop item being created.
void Steam::_item_created(CreateItemResult_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("item_created", result, (uint64_t)fileID, acceptTOS);
}
// Called when a workshop item has been downloaded.
void Steam::_item_downloaded(DownloadItemResult_t* callData){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	AppId_t appID = callData->m_unAppID;
	emit_signal("item_downloaded", result, (uint64_t)fileID, (int)appID);
}
// Called when getting the app dependencies for an item.
void Steam::_get_app_dependencies_result(GetAppDependenciesResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
//	AppId_t appID = callData->m_rgAppIDs;
	uint32 appDependencies = callData->m_nNumAppDependencies;
	uint32 totalAppDependencies = callData->m_nTotalNumAppDependencies;
//	emit_signal("get_app_dependencies_result", result, (uint64_t)fileID, appID, appDependencies, totalAppDependencies);
	emit_signal("get_app_dependencies_result", result, (uint64_t)fileID, appDependencies, totalAppDependencies);
}
// Called when an attempt at deleting an item completes.
void Steam::_item_deleted(DeleteItemResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	emit_signal("item_deleted", result, (uint64_t)fileID);
}
// Called when getting the users vote status on an item.
void Steam::_get_item_vote_result(GetUserItemVoteResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool voteUp = callData->m_bVotedUp;
	bool voteDown = callData->m_bVotedDown;
	bool voteSkipped = callData->m_bVoteSkipped;
	emit_signal("get_item_vote_result", result, (uint64_t)fileID, voteUp, voteDown, voteSkipped);
}
// Called when a workshop item has been installed or updated.
void Steam::_item_installed(ItemInstalled_t* callData){
	AppId_t appID = callData->m_unAppID;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	emit_signal("item_installed", appID, (uint64_t)fileID);
}
// Purpose: The result of a call to RemoveAppDependency.
void Steam::_remove_app_dependency_result(RemoveAppDependencyResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	AppId_t appID = callData->m_nAppID;
	emit_signal("remove_app_dependency_result", result, (uint64_t)fileID, (int)appID);
}
// Purpose: The result of a call to RemoveDependency.
void Steam::_remove_ugc_dependency_result(RemoveUGCDependencyResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	PublishedFileId_t childID = callData->m_nChildPublishedFileId;
	emit_signal("remove_ugc_dependency_result", result, (uint64_t)fileID, (uint64_t)childID);
}
// Called when the user has voted on an item.
void Steam::_set_user_item_vote(SetUserItemVoteResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool voteUp = callData->m_bVoteUp;
	emit_signal("set_user_item_vote", result, (uint64_t)fileID, voteUp);
}
// Called when workshop item playtime tracking has started.
void Steam::_start_playtime_tracking(StartPlaytimeTrackingResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	emit_signal("start_playtime_tracking", result);
}
// Called when a UGC query request completes.
void Steam::_ugc_query_completed(SteamUGCQueryCompleted_t* callData, bool bIOFailure){
	UGCQueryHandle_t handle = callData->m_handle;
	EResult result = callData->m_eResult;
	uint32 resultsReturned = callData->m_unNumResultsReturned;
	uint32 totalMatching = callData->m_unTotalMatchingResults;
	bool cached = callData->m_bCachedData;
	emit_signal("ugc_query_completed", (uint64_t)handle, result, resultsReturned, totalMatching, cached);
}
// Called when workshop item playtime tracking has stopped.
void Steam::_stop_playtime_tracking(StopPlaytimeTrackingResult_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	emit_signal("stop_playtime_tracking", result);
}
// Result of a workshop item being updated.
void Steam::_item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("item_updated", result, acceptTOS);
}
// Called when the user has added or removed an item to/from their favorites.
void Steam::_user_favorite_items_list_changed(UserFavoriteItemsListChanged_t* callData, bool bIOFailure){
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool wasAddRequest = callData->m_bWasAddRequest;
	emit_signal("user_favorite_items_list_changed", result, (uint64_t)fileID, wasAddRequest);
}

// User callbacks ///////////////////////////////
//
// Sent by the Steam server to the client telling it to disconnect from the specified game server, which it may be in the process of or already connected to. The game client should immediately disconnect upon receiving this message. This can usually occur if the user doesn't have rights to play on the game server.
void Steam::_client_game_server_deny(ClientGameServerDeny_t* callData){
	uint32 appID = callData->m_uAppID;
	uint32 serverIP = callData->m_unGameServerIP;
	uint16 serverPort = callData->m_usGameServerPort;
	uint16 secure = callData->m_bSecure;
	uint32 reason = callData->m_uReason;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char ip[16];
	for(int j = 0; j < NBYTES; j++){
		octet[j] = serverIP >> (j * 8);
	}
	sprintf(ip, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
	emit_signal("client_game_server_deny", appID, ip, serverPort, secure, reason);
}
// Sent for games with enabled anti indulgence / duration control, for enabled users. Lets the game know whether persistent rewards or XP should be granted at normal rate, half rate, or zero rate.
void Steam::_duration_control(DurationControl_t* callData, bool bIOFailure){
	int result = callData->m_eResult;
	int appid = callData->m_appid;
	bool applicable = callData->m_bApplicable;
	int32 secsLast = callData->m_csecsLast5h;
	int progress = callData->m_progress;
	int notification = callData->m_notification;
	String verbal = "";
	// Get a more verbal response
	if(notification == 1){
		verbal = "you've been playing for an hour";
	}
	else if(notification == 2){
		verbal = "you've been playing for 3 hours; take a break";
	}
	else if(notification == 3){
		verbal = "your xp / progress is half normal";
	}
	else if(notification == 4){
		verbal = "your xp / progress is zero";
	}
	else{
		verbal = "no notification";
	}
	// Create dictionary due to "too many arguments" issue
	Dictionary duration;
	duration["appID"] = appid;
	duration["applicable"] = applicable;
	duration["seconds_last_5hrs"] = secsLast;
	duration["progress"] = progress;
	duration["notification"] = notification;
	duration["notification_verbal"] = verbal;
	emit_signal("duration_control", result, duration);
}
// Called when an encrypted application ticket has been received.
void Steam::_encrypted_app_ticket_response(EncryptedAppTicketResponse_t* callData, bool bIOFailure){
	String result;
	if(callData->m_eResult == k_EResultOK){
		result = "ok";
	}
	else if(callData->m_eResult == k_EResultNoConnection){
		result = "no connection";
	}
	else if(callData->m_eResult == k_EResultDuplicateRequest){
		result = "duplicate request";
	}
	else{
		result = "limit exceeded";
	}
	emit_signal("encrypted_app_ticket_response", result);
}
// Sent to your game in response to a steam://gamewebcallback/ command from a user clicking a link in the Steam overlay browser. You can use this to add support for external site signups where you want to pop back into the browser after some web page signup sequence, and optionally get back some detail about that.
void Steam::_game_web_callback(GameWebCallback_t* callData){
	String url = callData->m_szURL;
	emit_signal("game_web_callback", url);
}
// Result when creating an auth session ticket.
void Steam::_get_auth_session_ticket_response(GetAuthSessionTicketResponse_t* callData){
	emit_signal("get_auth_session_ticket_response", callData->m_hAuthTicket, callData->m_eResult);
}
// Called when the callback system for this client is in an error state (and has flushed pending callbacks). When getting this message the client should disconnect from Steam, reset any stored Steam state and reconnect. This usually occurs in the rare event the Steam client has some kind of fatal error.
void Steam::_ipc_failure(IPCFailure_t *callData){
	uint8 type = callData->m_eFailureType;
	emit_signal("ipc_failure", type);
}
// Called whenever the users licenses (owned packages) changes.
void Steam::_licenses_updated(LicensesUpdated_t* callData){
	emit_signal("licenses_updated");
}
// Called when a user has responded to a microtransaction authorization request.
void Steam::_microstransaction_auth_response(MicroTxnAuthorizationResponse_t *callData){
	uint32 appID = callData->m_unAppID;
	uint64_t orderID = callData->m_ulOrderID;
	bool authorized;
	if(callData->m_bAuthorized == 1){
		authorized = true;
	}
	else{
		authorized = false;
	}
	emit_signal("microstransaction_auth_response", appID, orderID, authorized);
}
// Called when a connection attempt has failed. This will occur periodically if the Steam client is not connected, and has failed when retrying to establish a connection.
void Steam::_steam_server_connect_failed(SteamServerConnectFailure_t *callData, bool bIOFailure){
	int result = callData->m_eResult;
	bool retrying = callData->m_bStillRetrying;
	emit_signal("steam_server_connected_failed", result, retrying);
}
// Called when a connections to the Steam back-end has been established. This means the Steam client now has a working connection to the Steam servers. Usually this will have occurred before the game has launched, and should only be seen if the user has dropped connection due to a networking issue or a Steam server update.
void Steam::_steam_server_connected(SteamServersConnected_t* connectData){
	emit_signal("steam_server_connected");
}
// Called if the client has lost connection to the Steam servers. Real-time services will be disabled until a matching SteamServersConnected_t has been posted.
void Steam::_steam_server_disconnected(SteamServersDisconnected_t* connectData){
	emit_signal("steam_server_disconnected");
}
// Response when we have recieved the authentication URL after a call to requestStoreAuthURL.
void Steam::_store_auth_url_response(StoreAuthURLResponse_t* callData, bool bIOFailure){
	String url = callData->m_szURL;
	emit_signal("store_auth_url_response", url);
}
// Called when an auth ticket has been validated.
void Steam::_validate_auth_ticket_response(ValidateAuthTicketResponse_t* callData){
	uint64_t authID = callData->m_SteamID.ConvertToUint64();
	uint32_t response = callData->m_eAuthSessionResponse;
	uint64_t ownerID = callData->m_OwnerSteamID.ConvertToUint64();
	emit_signal("validate_auth_ticket_response", authID, response, ownerID);
}
//
// User Stats callbacks /////////////////////////
//
// Global achievements percentages are ready.
void Steam::_global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	emit_signal("global_achievement_percentages_ready", game, result);
}
// Called when the global stats have been received from the server.
void Steam::_global_stats_received(GlobalStatsReceived_t* callData, bool bIOFailure){
	uint64_t gameID = callData->m_nGameID;
	String result;
	if(callData->m_eResult == k_EResultOK){
		result = "ok";
	}
	else if(callData->m_eResult == k_EResultInvalidState){
		result = "invalid";
	}
	else{
		result = "fail";
	}
	emit_signal("global_stats_received", gameID, result);
}
// Result when finding a leaderboard.
void Steam::_leaderboard_find_result(LeaderboardFindResult_t *callData, bool bIOFailure){
	leaderboardHandle = callData->m_hSteamLeaderboard;
	uint8_t found = callData->m_bLeaderboardFound;
	emit_signal("leaderboard_find_result", (uint64_t)leaderboardHandle, found);
}
// Called when scores for a leaderboard have been downloaded and are ready to be retrieved. After calling you must use GetDownloadedLeaderboardEntry to retrieve the info for each downloaded entry.
void Steam::_leaderboard_scores_downloaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure){
	// Set up a message to fill in
	String message;
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboardHandle){
		message = "Leaderboard handle was incorrect";
	}
	if(!bIOFailure){
		// Clear previous leaderboard entries
		leaderboardEntriesArray.clear();
		// Create the entry pointer and details array
		LeaderboardEntry_t *entry = memnew(LeaderboardEntry_t);
		PoolIntArray details;
		int32 *detailsPointer = NULL;
		// Resize array
		if(leaderboardDetailsMax > 0){
			details.resize(leaderboardDetailsMax);
			PoolIntArray::Write w = details.write();
			detailsPointer = w.ptr();
			for(int i = 0; i < leaderboardDetailsMax; i++){
				detailsPointer[i] = 0;
			}
		}
		// Loop through the entries and add them as dictionaries to the array
		for(int i = 0; i < callData->m_cEntryCount; i++){
			if(SteamUserStats()->GetDownloadedLeaderboardEntry(callData->m_hSteamLeaderboardEntries, i, entry, detailsPointer, leaderboardDetailsMax)){
				Dictionary entryDict;
				entryDict["score"] = entry->m_nScore;
				entryDict["steamID"] = uint64_t(entry->m_steamIDUser.ConvertToUint64());
				entryDict["global_rank"] = entry->m_nGlobalRank;
				entryDict["ugc_handle"] = uint64_t(entry->m_hUGC);
				if(leaderboardDetailsMax > 0){
					PoolIntArray array;
					array.resize(leaderboardDetailsMax);
					PoolIntArray::Write w = array.write();
					int32_t *ptr = w.ptr();
					for(int j = 0; j < leaderboardDetailsMax; j++){
						ptr[j] = detailsPointer[j];
					}
					entryDict["details"] = array;
				}
				leaderboardEntriesArray.append(entryDict);
			}
			message = "Leaderboard entries successfully retrieved";
		}
		memdelete(entry);
	} else {
		message = "There was an IO failure";
	}
	// Emit the signal, with array, back
	emit_signal("leaderboard_scores_downloaded", message, leaderboardEntriesArray);
}
// Result indicating that a leaderboard score has been uploaded.
void Steam::_leaderboard_score_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure){
	// Incorrect leaderboard
	if(callData->m_hSteamLeaderboard != leaderboardHandle){
		return;
	}
	emit_signal("leaderboard_score_uploaded", callData->m_bSuccess, callData->m_nScore, callData->m_bScoreChanged, callData->m_nGlobalRankNew, callData->m_nGlobalRankPrevious);
}
// Result indicating that user generated content has been attached to one of the current user's leaderboard entries.
void Steam::_leaderboard_ugc_set(LeaderboardUGCSet_t* callData, bool bIOFailure){
	leaderboardHandle = callData->m_hSteamLeaderboard;
	String result;
	if(callData->m_eResult == k_EResultOK){
		result = "ok";
	}
	else if(callData->m_eResult == k_EResultTimeout){
		result = "timeout";
	}
	else{
		result = "invalid";
	}
	emit_signal("leaderboard_ugc_set", (uint64_t)leaderboardHandle, result);
}
// Gets the current number of players for the current AppId.
void Steam::_number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure){
	emit_signal("number_of_current_players", callData->m_bSuccess && bIOFailure, callData->m_cPlayers);
}
// Result of a request to store the achievements on the server, or an "indicate progress" call. If both m_nCurProgress and m_nMaxProgress are zero, that means the achievement has been fully unlocked.
void Steam::_user_achievement_stored(UserAchievementStored_t* callData){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	bool groupAchieve = callData->m_bGroupAchievement;
	String name = callData->m_rgchAchievementName;
	uint32_t currentProgress = callData->m_nCurProgress;
	uint32_t maxProgress = callData->m_nMaxProgress;
	emit_signal("user_achievement_stored", game, groupAchieve, name, currentProgress, maxProgress);
}
// Called when the latest stats and achievements for the local user have been received from the server.
void Steam::_current_stats_received(UserStatsReceived_t* callData){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	CSteamID userID = callData->m_steamIDUser;
	uint64_t user = userID.ConvertToUint64();
	emit_signal("current_stats_received", game, result, user);
}
// Called when the latest stats and achievements for a specific user (including the local user) have been received from the server.
void Steam::_user_stats_received(UserStatsReceived_t* callData, bool bIOFailure){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	CSteamID userID = callData->m_steamIDUser;
	uint64_t user = userID.ConvertToUint64();
	emit_signal("user_stats_received", game, result, user);
}
// Result of a request to store the user stats.
void Steam::_user_stats_stored(UserStatsStored_t* callData){
	CSteamID gameID = callData->m_nGameID;
	uint64_t game = gameID.ConvertToUint64();
	uint32_t result = callData->m_eResult;
	emit_signal("user_stats_stored", game, result);
}
// Callback indicating that a user's stats have been unloaded. Call RequestUserStats again before accessing stats for this user.
void Steam::_user_stats_unloaded(UserStatsUnloaded_t* callData){
	CSteamID steamID = callData->m_steamIDUser;
	uint64_t user = steamID.ConvertToUint64();
	emit_signal("user_stats_unloaded", user);
}

// Utility callbacks ////////////////////////////
//
// CallResult for checkFileSignature.
void Steam::_check_file_signature(CheckFileSignature_t *callData){
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
void Steam::_gamepad_text_input_dismissed(GamepadTextInputDismissed_t* callData){
	char text;
	uint32 length = 0;
	if(callData->m_bSubmitted){
		SteamUtils()->GetEnteredGamepadTextInput(&text, callData->m_unSubmittedText);
		length = SteamUtils()->GetEnteredGamepadTextLength();
	}
	emit_signal("gamepad_text_input_dismissed", text, length);
}
// Called when the country of the user changed. The country should be updated with getIPCountry.
void Steam::_ip_country(IPCountry_t* callData){
	emit_signal("ip_country");
}
// Called when running on a laptop and less than 10 minutes of battery is left, and then fires then every minute afterwards.
void Steam::_low_power(LowBatteryPower_t* timeLeft){
	uint8 power = timeLeft->m_nMinutesBatteryLeft;
	emit_signal("low_power", power);
}
// Called when a SteamAPICall_t has completed (or failed)
void Steam::_steam_api_call_completed(SteamAPICallCompleted_t* callData){
	uint64_t asyncCall = callData->m_hAsyncCall;
	int callback = callData->m_iCallback;
	uint32 parameter = callData->m_cubParam;
	emit_signal("steam_api_call_completed", asyncCall, callback, parameter);
}
// Called when Steam wants to shutdown.
void Steam::_steam_shutdown(SteamShutdown_t* callData){
	emit_signal("steam_shutdown");
}

// Video callbacks //////////////////////////////
//
// Automatically called whenever the user starts broadcasting.	// In documentation but not in actual SDK?
//void Steam::_broadcast_upload_start(BroadcastUploadStart_t* callData){
//	emit_signal("broadcast_upload_start");
//}
// Automatically called whenever the user stops broadcasting.	// In documentation but not in actual SDK?
//void Steam::_broadcast_upload_stop(BroadcastUploadStop_t* callData){
//	int result = callData->m_eResult;
//	emit_signal("broadcast_upload_stop", result);
//}
// Triggered when the OPF Details for 360 video playback are retrieved. After receiving this you can use GetOPFStringForApp to access the OPF details.
void Steam::_get_opf_settings_result(GetOPFSettingsResult_t* callData){
	int result = callData->m_eResult;
	int appid = callData->m_unVideoAppID;
	emit_signal("broadcast_upload_stop", result, appid);
}
// Provides the result of a call to GetVideoURL.
void Steam::_get_video_result(GetVideoURLResult_t* callData){
	int result = callData->m_eResult;
	int appid = callData->m_unVideoAppID;
	String url = callData->m_rgchURL;
	emit_signal("get_video_result", result, appid, url);
}

/////////////////////////////////////////////////
///// BIND METHODS //////////////////////////////
/////////////////////////////////////////////////
//
void Steam::_bind_methods(){
	/////////////////////////////////////////////
	// FUNCTION BINDS ///////////////////////////
	/////////////////////////////////////////////
	//
	// Steam Main Bind Methods //////////////////
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
	ClassDB::bind_method("isTimedTrial", &Steam::isTimedTrial);
	
	// Friends Bind Methods /////////////////////
	ClassDB::bind_method("getPersonaName", &Steam::getPersonaName);
	ClassDB::bind_method("setPersonaName", &Steam::setPersonaName);
	ClassDB::bind_method("getPersonaState", &Steam::getPersonaState);
	ClassDB::bind_method("getFriendCount", &Steam::getFriendCount);
	ClassDB::bind_method("getFriendByIndex", &Steam::getFriendByIndex);
	ClassDB::bind_method("getFriendRelationship", &Steam::getFriendRelationship);
	ClassDB::bind_method("getFriendPersonaState", &Steam::getFriendPersonaState);
	ClassDB::bind_method("getFriendPersonaName", &Steam::getFriendPersonaName);
	ClassDB::bind_method("getFriendGamePlayed", &Steam::getFriendGamePlayed);
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
	ClassDB::bind_method(D_METHOD("getPlayerAvatar", "size", "steamID"), &Steam::getPlayerAvatar, DEFVAL(2), DEFVAL(0));
	ClassDB::bind_method("getUserFriendsGroups", &Steam::getUserFriendsGroups);
	ClassDB::bind_method("getUserSteamGroups", &Steam::getUserSteamGroups);
	ClassDB::bind_method("getUserSteamFriends", &Steam::getUserSteamFriends);
	ClassDB::bind_method("registerProtocolInOverlayBrowser", &Steam::registerProtocolInOverlayBrowser);
	
	// HTML Surface Bind Methods ////////////////
	ClassDB::bind_method("addHeader", &Steam::addHeader);
	ClassDB::bind_method("allowStartRequest", &Steam::allowStartRequest);
	ClassDB::bind_method("copyToClipboard", &Steam::copyToClipboard);
	ClassDB::bind_method("createBrowser", &Steam::createBrowser);
	ClassDB::bind_method("executeJavascript", &Steam::executeJavascript);
//	ClassDB::bind_method("fileLoadDialogResponse", &Steam::fileLoadDialogResponse);
	ClassDB::bind_method("find", &Steam::find);
	ClassDB::bind_method("getLinkAtPosition", &Steam::getLinkAtPosition);
	ClassDB::bind_method("goBack", &Steam::goBack);
	ClassDB::bind_method("goForward", &Steam::goForward);
	ClassDB::bind_method("htmlInit", &Steam::htmlInit);
	ClassDB::bind_method("jsDialogResponse", &Steam::jsDialogResponse);
	ClassDB::bind_method("keyChar", &Steam::keyChar);
	ClassDB::bind_method("keyDown", &Steam::keyDown);
	ClassDB::bind_method("keyUp", &Steam::keyUp);
	ClassDB::bind_method("loadURL", &Steam::loadURL);
	ClassDB::bind_method("mouseDoubleClick", &Steam::mouseDoubleClick);
	ClassDB::bind_method("mouseDown", &Steam::mouseDown);
	ClassDB::bind_method("mouseMove", &Steam::mouseMove);
	ClassDB::bind_method("mouseUp", &Steam::mouseUp);
	ClassDB::bind_method("mouseWheel", &Steam::mouseWheel);
	ClassDB::bind_method("pasteFromClipboard", &Steam::pasteFromClipboard);
	ClassDB::bind_method("reload", &Steam::reload);
	ClassDB::bind_method("removeBrowser", &Steam::removeBrowser);
	ClassDB::bind_method("setBackgroundMode", &Steam::setBackgroundMode);
//	ClassDB::bind_method("setCookie", &Steam::setCookie);
	ClassDB::bind_method("setHorizontalScroll", &Steam::setHorizontalScroll);
	ClassDB::bind_method("setKeyFocus", &Steam::setKeyFocus);
	ClassDB::bind_method("setPageScaleFactor", &Steam::setPageScaleFactor);
	ClassDB::bind_method("setSize", &Steam::setSize);
	ClassDB::bind_method("setVerticalScroll", &Steam::setVerticalScroll);
	ClassDB::bind_method("htmlShutdown", &Steam::htmlShutdown);
	ClassDB::bind_method("stopFind", &Steam::stopFind);
	ClassDB::bind_method("stopLoad", &Steam::stopLoad);
	ClassDB::bind_method("viewSource", &Steam::viewSource);

	// Input Bind Methods /////////////////
	ClassDB::bind_method("activateActionSet", &Steam::activateActionSet);
	ClassDB::bind_method("activateActionSetLayer", &Steam::activateActionSetLayer);
	ClassDB::bind_method("deactivateActionSetLayer", &Steam::deactivateActionSetLayer);
	ClassDB::bind_method("deactivateAllActionSetLayers", &Steam::deactivateAllActionSetLayers);
	ClassDB::bind_method("getActionSetHandle", &Steam::getActionSetHandle);
	ClassDB::bind_method("getActionOriginFromXboxOrigin", &Steam::getActionOriginFromXboxOrigin);
	ClassDB::bind_method("getActiveActionSetLayers", &Steam::getActiveActionSetLayers);
	ClassDB::bind_method("getAnalogActionData", &Steam::getAnalogActionData);
	ClassDB::bind_method("getAnalogActionHandle", &Steam::getAnalogActionHandle);
	ClassDB::bind_method("getAnalogActionOrigins", &Steam::getAnalogActionOrigins);
	ClassDB::bind_method("getConnectedControllers", &Steam::getConnectedControllers);
	ClassDB::bind_method("getControllerForGamepadIndex", &Steam::getControllerForGamepadIndex);
	ClassDB::bind_method("getCurrentActionSet", &Steam::getCurrentActionSet);
	ClassDB::bind_method("getDeviceBindingRevision", &Steam::getDeviceBindingRevision);
	ClassDB::bind_method("getDigitalActionData", &Steam::getDigitalActionData);
	ClassDB::bind_method("getDigitalActionHandle", &Steam::getDigitalActionHandle);
	ClassDB::bind_method("getDigitalActionOrigins", &Steam::getDigitalActionOrigins);
	ClassDB::bind_method("getGamepadIndexForController", &Steam::getGamepadIndexForController);
	ClassDB::bind_method("getGlyphForActionOrigin", &Steam::getGlyphForActionOrigin);
	ClassDB::bind_method("getInputTypeForHandle", &Steam::getInputTypeForHandle);
	ClassDB::bind_method("getMotionData", &Steam::getMotionData);
	ClassDB::bind_method("getRemotePlaySessionID", &Steam::getRemotePlaySessionID);
	ClassDB::bind_method("getStringForActionOrigin", &Steam::getStringForActionOrigin);
	ClassDB::bind_method("inputInit", &Steam::inputInit);
	ClassDB::bind_method("inputShutdown", &Steam::inputShutdown);
	ClassDB::bind_method("runFrame", &Steam::runFrame);
	ClassDB::bind_method("setLEDColor", &Steam::setLEDColor);
	ClassDB::bind_method("showBindingPanel", &Steam::showBindingPanel);
	ClassDB::bind_method("stopAnalogActionMomentum", &Steam::stopAnalogActionMomentum);
	ClassDB::bind_method("translateActionOrigin", &Steam::translateActionOrigin);
	ClassDB::bind_method("triggerHapticPulse", &Steam::triggerHapticPulse);
//	ClassDB::bind_method("triggerRepeatedHapticPulse", &Steam::triggerRepeatedHapticPulse);
	ClassDB::bind_method("triggerVibration", &Steam::triggerVibration);

	// Inventory Bind Methods ///////////////////
	ClassDB::bind_method("addPromoItem", &Steam::addPromoItem);
	ClassDB::bind_method("addPromoItems", &Steam::addPromoItems);
	ClassDB::bind_method("checkResultSteamID", &Steam::checkResultSteamID);
	ClassDB::bind_method("consumeItem", &Steam::consumeItem);
	ClassDB::bind_method("deserializeResult", &Steam::deserializeResult);
	ClassDB::bind_method("destroyResult", &Steam::exchangeItems);
	ClassDB::bind_method("exchangeItems", &Steam::exchangeItems);
	ClassDB::bind_method("generateItems", &Steam::generateItems);
	ClassDB::bind_method("getAllItems", &Steam::getAllItems);
	ClassDB::bind_method("getItemDefinitionProperty", &Steam::getItemDefinitionProperty);
	ClassDB::bind_method("getItemsByID", &Steam::getItemsByID);
	ClassDB::bind_method("getItemPrice", &Steam::getItemPrice);
	ClassDB::bind_method("getItemsWithPrices", &Steam::getItemsWithPrices);
	ClassDB::bind_method("getNumItemsWithPrices", &Steam::getNumItemsWithPrices);
	ClassDB::bind_method("getResultItemProperty", &Steam::getResultItems);
	ClassDB::bind_method("getResultItems", &Steam::getResultItems);
	ClassDB::bind_method("getResultStatus", &Steam::getResultStatus);
	ClassDB::bind_method("getResultTimestamp", &Steam::getResultTimestamp);
	ClassDB::bind_method("grantPromoItems", &Steam::grantPromoItems);
	ClassDB::bind_method("loadItemDefinitions", &Steam::loadItemDefinitions);
	ClassDB::bind_method("requestEligiblePromoItemDefinitionsIDs", &Steam::requestEligiblePromoItemDefinitionsIDs);
	ClassDB::bind_method("requestPrices", &Steam::requestPrices);
	ClassDB::bind_method("serializeResult", &Steam::serializeResult);
	ClassDB::bind_method("startPurchase", &Steam::startPurchase);
	ClassDB::bind_method("transferItemQuantity", &Steam::transferItemQuantity);
	ClassDB::bind_method("triggerItemDrop", &Steam::triggerItemDrop);
	ClassDB::bind_method("startUpdateProperties", &Steam::startUpdateProperties);
	ClassDB::bind_method("submitUpdateProperties", &Steam::submitUpdateProperties);
	ClassDB::bind_method("removeProperty", &Steam::removeProperty);
	ClassDB::bind_method("setPropertyString", &Steam::setPropertyString);
	ClassDB::bind_method("setPropertyBool", &Steam::setPropertyBool);
	ClassDB::bind_method("setPropertyInt", &Steam::setPropertyInt);
	ClassDB::bind_method("setPropertyFloat", &Steam::setPropertyFloat);

	// Matchmaking Bind Methods /////////////////
	ClassDB::bind_method("getFavoriteGames", &Steam::getFavoriteGames);
	ClassDB::bind_method("addFavoriteGame", &Steam::addFavoriteGame);
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
	
	// Music Remote Bind Methods ////////////////
	ClassDB::bind_method("activationSuccess", &Steam::activationSuccess);
	ClassDB::bind_method("isCurrentMusicRemote", &Steam::isCurrentMusicRemote);
	ClassDB::bind_method("currentEntryDidChange", &Steam::currentEntryDidChange);
	ClassDB::bind_method("currentEntryIsAvailable", &Steam::currentEntryIsAvailable);
	ClassDB::bind_method("currentEntryWillChange", &Steam::currentEntryWillChange);
	ClassDB::bind_method("deregisterSteamMusicRemote", &Steam::deregisterSteamMusicRemote);
	ClassDB::bind_method("enableLooped", &Steam::enableLooped);
	ClassDB::bind_method("enablePlaylists", &Steam::enablePlaylists);
	ClassDB::bind_method("enablePlayNext", &Steam::enablePlayNext);
	ClassDB::bind_method("enablePlayPrevious", &Steam::enablePlayPrevious);
	ClassDB::bind_method("enableQueue", &Steam::enableQueue);
	ClassDB::bind_method("enableShuffled", &Steam::enableShuffled);
	ClassDB::bind_method("playlistDidChange", &Steam::playlistDidChange);
	ClassDB::bind_method("playlistWillChange", &Steam::playlistWillChange);
	ClassDB::bind_method("queueDidChange", &Steam::queueDidChange);
	ClassDB::bind_method("queueWillChange", &Steam::queueWillChange);
	ClassDB::bind_method("registerSteamMusicRemote", &Steam::registerSteamMusicRemote);
	ClassDB::bind_method("resetPlaylistEntries", &Steam::resetPlaylistEntries);
	ClassDB::bind_method("resetQueueEntries", &Steam::resetQueueEntries);
	ClassDB::bind_method("setCurrentPlaylistEntry", &Steam::setCurrentPlaylistEntry);
	ClassDB::bind_method("setCurrentQueueEntry", &Steam::setCurrentQueueEntry);
	ClassDB::bind_method("setDisplayName", &Steam::setDisplayName);
	ClassDB::bind_method("setPlaylistEntry", &Steam::setPlaylistEntry);
//	ClassDB::bind_method("setPNGIcon64x64", &Steam::setPNGIcon64x64);
	ClassDB::bind_method("setQueueEntry", &Steam::setQueueEntry);
//	ClassDB::bind_method("updateCurrentEntryCoverArt", &Steam::updateCurrentEntryCoverArt);
	ClassDB::bind_method("updateCurrentEntryElapsedSeconds", &Steam::updateCurrentEntryElapsedSeconds);
	ClassDB::bind_method("updateCurrentEntryText", &Steam::updateCurrentEntryText);
	ClassDB::bind_method("updateLooped", &Steam::updateLooped);
	ClassDB::bind_method("updatePlaybackStatus", &Steam::updatePlaybackStatus);
	ClassDB::bind_method("updateShuffled", &Steam::updateShuffled);
	ClassDB::bind_method("updateVolume", &Steam::updateVolume);

	// Networking Bind Methods //////////////////
	ClassDB::bind_method("acceptP2PSessionWithUser", &Steam::acceptP2PSessionWithUser);
	ClassDB::bind_method("allowP2PPacketRelay", &Steam::allowP2PPacketRelay);
	ClassDB::bind_method("closeP2PChannelWithUser", &Steam::closeP2PChannelWithUser);
	ClassDB::bind_method("closeP2PSessionWithUser", &Steam::closeP2PSessionWithUser);
	ClassDB::bind_method("getP2PSessionState", &Steam::getP2PSessionState);
	ClassDB::bind_method("getAvailableP2PPacketSize", &Steam::getAvailableP2PPacketSize);
	ClassDB::bind_method("readP2PPacket", &Steam::readP2PPacket);
	ClassDB::bind_method("sendP2PPacket", &Steam::sendP2PPacket);

	// Parties Bind Methods /////////////////////
	ClassDB::bind_method("getAvailableBeaconLocations", &Steam::getAvailableBeaconLocations);
	ClassDB::bind_method("createBeacon", &Steam::createBeacon);
	ClassDB::bind_method("onReservationCompleted", &Steam::onReservationCompleted);
	ClassDB::bind_method("changeNumOpenSlots", &Steam::changeNumOpenSlots);
	ClassDB::bind_method("destroyBeacon", &Steam::destroyBeacon);
	ClassDB::bind_method("getNumActiveBeacons", &Steam::getNumActiveBeacons);
	ClassDB::bind_method("getBeaconByIndex", &Steam::getBeaconByIndex);
	ClassDB::bind_method("getBeaconDetails", &Steam::getBeaconDetails);
	ClassDB::bind_method("joinParty", &Steam::joinParty);
	ClassDB::bind_method("getBeaconLocationData", &Steam::getBeaconLocationData);

	// Remote Play Bind Methods /////////////////
	ClassDB::bind_method("getSessionCount", &Steam::getSessionCount);
	ClassDB::bind_method("getSessionID", &Steam::getSessionID);
	ClassDB::bind_method("getSessionSteamID", &Steam::getSessionSteamID);
	ClassDB::bind_method("getSessionClientName", &Steam::getSessionClientName);
	ClassDB::bind_method("getSessionClientFormFactor", &Steam::getSessionClientFormFactor);
	ClassDB::bind_method("getSessionClientResolution", &Steam::getSessionClientResolution);
	ClassDB::bind_method("sendRemotePlayTogetherInvite", &Steam::sendRemotePlayTogetherInvite);

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
	ClassDB::bind_method("addVRScreenshotToLibrary", &Steam::addVRScreenshotToLibrary);
	ClassDB::bind_method("hookScreenshots", &Steam::hookScreenshots);
	ClassDB::bind_method("isScreenshotsHooked", &Steam::isScreenshotsHooked);
	ClassDB::bind_method("setLocation", &Steam::setLocation);
	ClassDB::bind_method("tagPublishedFile", &Steam::tagPublishedFile);
	ClassDB::bind_method("taguser", &Steam::tagUser);
	ClassDB::bind_method("triggerScreenshot", &Steam::triggerScreenshot);
	ClassDB::bind_method("writeScreenshot", &Steam::writeScreenshot);
	
	// UGC Bind Methods ////////////////////
	ClassDB::bind_method("addAppDependency", &Steam::addAppDependency);
	ClassDB::bind_method("addDependency", &Steam::addDependency);
	ClassDB::bind_method("addExcludedTag", &Steam::addExcludedTag);
	ClassDB::bind_method("addItemKeyValueTag", &Steam::addItemKeyValueTag);
	ClassDB::bind_method("addItemPreviewFile", &Steam::addItemPreviewFile);
	ClassDB::bind_method("addItemPreviewVideo", &Steam::addItemPreviewVideo);
	ClassDB::bind_method("addItemToFavorite", &Steam::addItemToFavorite);
	ClassDB::bind_method("addRequiredKeyValueTag", &Steam::addRequiredKeyValueTag);
	ClassDB::bind_method("addRequiredTag", &Steam::addRequiredTag);
	ClassDB::bind_method("initWorkshopForGameServer", &Steam::initWorkshopForGameServer);
	ClassDB::bind_method("createItem", &Steam::createItem);
	ClassDB::bind_method("createQueryAllUGCRequest", &Steam::createQueryAllUGCRequest);
	ClassDB::bind_method("createQueryUGCDetailsRequest", &Steam::createQueryUGCDetailsRequest);
//	ClassDB::bind_method("createQueryUserUGCRequest", &Steam::createQueryUserUGCRequest);
	ClassDB::bind_method("deleteItem", &Steam::deleteItem);
	ClassDB::bind_method("downloadItem", &Steam::downloadItem);
	ClassDB::bind_method("getItemDownloadInfo", &Steam::getItemDownloadInfo);
	ClassDB::bind_method("getItemInstallInfo", &Steam::getItemInstallInfo);
	ClassDB::bind_method("getItemState", &Steam::getItemState);
	ClassDB::bind_method("getItemUpdateProgress", &Steam::getItemUpdateProgress);
	ClassDB::bind_method("getSubscribedItems", &Steam::getSubscribedItems);
	ClassDB::bind_method("getQueryUGCAdditionalPreview", &Steam::getQueryUGCAdditionalPreview);
	ClassDB::bind_method("getQueryUGCChildren", &Steam::getQueryUGCChildren);
	ClassDB::bind_method("getQueryUGCKeyValueTag", &Steam::getQueryUGCKeyValueTag);
	ClassDB::bind_method("getQueryUGCMetadata", &Steam::getQueryUGCMetadata);
	ClassDB::bind_method("getQueryUGCNumAdditionalPreviews", &Steam::getQueryUGCNumAdditionalPreviews);
	ClassDB::bind_method("getQueryUGCNumKeyValueTags", &Steam::getQueryUGCNumKeyValueTags);
	ClassDB::bind_method("getQueryUGCPreviewURL", &Steam::getQueryUGCPreviewURL);
	ClassDB::bind_method("getQueryUGCResult", &Steam::getQueryUGCResult);
	ClassDB::bind_method("getQueryUGCStatistic", &Steam::getQueryUGCStatistic);
	ClassDB::bind_method("getUserItemVote", &Steam::getUserItemVote);
	ClassDB::bind_method("releaseQueryUGCRequest", &Steam::releaseQueryUGCRequest);
	ClassDB::bind_method("removeAppDependency", &Steam::removeAppDependency);
	ClassDB::bind_method("removeDependency", &Steam::removeDependency);
	ClassDB::bind_method("removeItemFromFavorites", &Steam::removeItemFromFavorites);
	ClassDB::bind_method("removeItemKeyValueTags", &Steam::removeItemKeyValueTags);
	ClassDB::bind_method("removeItemPreview", &Steam::removeItemPreview);
	ClassDB::bind_method("sendQueryUGCRequest", &Steam::sendQueryUGCRequest);
	ClassDB::bind_method("setAllowCachedResponse", &Steam::setAllowCachedResponse);
	ClassDB::bind_method("setCloudFileNameFilter", &Steam::setCloudFileNameFilter);
	ClassDB::bind_method("setItemContent", &Steam::setItemContent);
	ClassDB::bind_method("setItemDescription", &Steam::setItemDescription);
	ClassDB::bind_method("setItemMetadata", &Steam::setItemMetadata);
	ClassDB::bind_method("setItemPreview", &Steam::setItemPreview);
	ClassDB::bind_method("setItemTags", &Steam::setItemTags);
	ClassDB::bind_method("setItemTitle", &Steam::setItemTitle);
	ClassDB::bind_method("setItemUpdateLanguage", &Steam::setItemUpdateLanguage);
	ClassDB::bind_method("setItemVisibility", &Steam::setItemVisibility);
	ClassDB::bind_method("setLanguage", &Steam::setLanguage);
	ClassDB::bind_method("setMatchAnyTag", &Steam::setMatchAnyTag);
	ClassDB::bind_method("setRankedByTrendDays", &Steam::setRankedByTrendDays);
	ClassDB::bind_method("setReturnAdditionalPreviews", &Steam::setReturnAdditionalPreviews);
	ClassDB::bind_method("setReturnChildren", &Steam::setReturnChildren);
	ClassDB::bind_method("setReturnKeyValueTags", &Steam::setReturnKeyValueTags);
	ClassDB::bind_method("setReturnLongDescription", &Steam::setReturnLongDescription);
	ClassDB::bind_method("setReturnMetadata", &Steam::setReturnMetadata);
	ClassDB::bind_method("setReturnOnlyIDs", &Steam::setReturnOnlyIDs);
	ClassDB::bind_method("setReturnPlaytimeStats", &Steam::setReturnPlaytimeStats);
	ClassDB::bind_method("setReturnTotalOnly", &Steam::setReturnTotalOnly);
	ClassDB::bind_method("setSearchText", &Steam::setSearchText);
	ClassDB::bind_method("setUserItemVote", &Steam::setUserItemVote);
	ClassDB::bind_method("startItemUpdate", &Steam::startItemUpdate);
	ClassDB::bind_method("startPlaytimeTracking", &Steam::startPlaytimeTracking);
	ClassDB::bind_method("stopPlaytimeTracking", &Steam::stopPlaytimeTracking);
	ClassDB::bind_method("stopPlaytimeTrackingForAllItems", &Steam::stopPlaytimeTrackingForAllItems);
	ClassDB::bind_method("getAppDependencies", &Steam::getAppDependencies);
	ClassDB::bind_method("submitItemUpdate", &Steam::submitItemUpdate);
	ClassDB::bind_method("subscribeItem", &Steam::subscribeItem);
	ClassDB::bind_method("suspendDownloads", &Steam::suspendDownloads);
	ClassDB::bind_method("unsubscribeItem", &Steam::unsubscribeItem);
	ClassDB::bind_method("updateItemPreviewFile", &Steam::updateItemPreviewFile);
	ClassDB::bind_method("updateItemPreviewVideo", &Steam::updateItemPreviewVideo);

	// User Bind Methods ////////////////////////
	ClassDB::bind_method(D_METHOD("advertiseGame", "serverIP", "port"), &Steam::advertiseGame);
	ClassDB::bind_method("beginAuthSession", &Steam::beginAuthSession);
	ClassDB::bind_method("cancelAuthTicket", &Steam::cancelAuthTicket);
	ClassDB::bind_method("decompressVoice", &Steam::decompressVoice);
	ClassDB::bind_method("endAuthSession", &Steam::endAuthSession);
	ClassDB::bind_method("getAuthSessionTicket", &Steam::getAuthSessionTicket);
	ClassDB::bind_method("getAuthSessionTicketID", &Steam::getAuthSessionTicketID);
	ClassDB::bind_method("getAvailableVoice", &Steam::getAvailableVoice);
	ClassDB::bind_method("getDurationControl", &Steam::getDurationControl);
	ClassDB::bind_method("getEncryptedAppTicket", &Steam::getEncryptedAppTicket);
	ClassDB::bind_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
	ClassDB::bind_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
	ClassDB::bind_method("getSteamID", &Steam::getSteamID);
	ClassDB::bind_method("getUserDataFolder", &Steam::getUserDataFolder);
	ClassDB::bind_method("getVoice", &Steam::getVoice);
	ClassDB::bind_method("getVoiceOptimalSampleRate", &Steam::getVoiceOptimalSampleRate);
	ClassDB::bind_method("initiateGameConnection", &Steam::initiateGameConnection);
	ClassDB::bind_method("isBehindNAT", &Steam::isBehindNAT);
	ClassDB::bind_method("isPhoneIdentifying", &Steam::isPhoneIdentifying);
	ClassDB::bind_method("isPhoneRequiringVerification", &Steam::isPhoneRequiringVerification);
	ClassDB::bind_method("isPhoneVerified", &Steam::isPhoneVerified);
	ClassDB::bind_method("isTwoFactorEnabled", &Steam::isTwoFactorEnabled);
	ClassDB::bind_method("loggedOn", &Steam::loggedOn);
	ClassDB::bind_method("requestEncryptedAppTicket", &Steam::requestEncryptedAppTicket);
	ClassDB::bind_method("requestStoreAuthURL", &Steam::requestStoreAuthURL);
	ClassDB::bind_method("startVoiceRecording", &Steam::startVoiceRecording);
	ClassDB::bind_method("stopVoiceRecording", &Steam::stopVoiceRecording);
	ClassDB::bind_method("terminateGameConnection", &Steam::terminateGameConnection);
	ClassDB::bind_method("userHasLicenseForApp", &Steam::userHasLicenseForApp);
	
	// User Stats Bind Methods //////////////////
	ClassDB::bind_method("attachLeaderboardUGC", &Steam::attachLeaderboardUGC);
	ClassDB::bind_method("clearAchievement", &Steam::clearAchievement);
	ClassDB::bind_method(D_METHOD("downloadLeaderboardEntries", "range_start", "range_end", "type"), &Steam::downloadLeaderboardEntries, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("downloadLeaderboardEntriesForUsers", "usersID"), &Steam::downloadLeaderboardEntriesForUsers);
	ClassDB::bind_method(D_METHOD("findLeaderboard", "name"), &Steam::findLeaderboard);
	ClassDB::bind_method("findOrCreateLeaderboard", &Steam::findOrCreateLeaderboard);
	ClassDB::bind_method("getAchievement", &Steam::getAchievement);
	ClassDB::bind_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
	ClassDB::bind_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
	ClassDB::bind_method(D_METHOD("getAchievementDisplayAttribute", "name", "key"), &Steam::getAchievementDisplayAttribute);
	ClassDB::bind_method(D_METHOD("getAchievementIcon", "name"), &Steam::getAchievementIcon);
	ClassDB::bind_method(D_METHOD("getAchievementName", "achievement"), &Steam::getAchievementName);
//	ClassDB::bind_method("getGlobalStatInt", &Steam::getGlobalStatInt);
	ClassDB::bind_method("getGlobalStatFloat", &Steam::getGlobalStatFloat);
//	ClassDB::bind_method("getGlobalStatIntHistory", &Steam::getGlobalStatIntHistory);
	ClassDB::bind_method("getGlobalStatFloatHistory", &Steam::getGlobalStatFloatHistory);
	ClassDB::bind_method("getLeaderboardDisplayType", &Steam::getLeaderboardDisplayType);
	ClassDB::bind_method("getLeaderboardEntryCount", &Steam::getLeaderboardEntryCount);
	ClassDB::bind_method("getLeaderboardName", &Steam::getLeaderboardName);
	ClassDB::bind_method("getLeaderboardSortMethod", &Steam::getLeaderboardSortMethod);
	ClassDB::bind_method("getMostAchievedAchievementInfo", &Steam::getMostAchievedAchievementInfo);
	ClassDB::bind_method("getNumAchievements", &Steam::getNumAchievements);
	ClassDB::bind_method("getNumberOfCurrentPlayers", &Steam::getNumberOfCurrentPlayers);
	ClassDB::bind_method("getStatFloat", &Steam::getStatFloat);
	ClassDB::bind_method("getStatInt", &Steam::getStatInt);
	ClassDB::bind_method("getUserAchievement", &Steam::getUserAchievement);
	ClassDB::bind_method("getUserAchievementAndUnlockTime", &Steam::getUserAchievementAndUnlockTime);
	ClassDB::bind_method("getUserStatFloat", &Steam::getUserStatFloat);
	ClassDB::bind_method("getUserStatInt", &Steam::getUserStatInt);
	ClassDB::bind_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);
	ClassDB::bind_method("requestCurrentStats", &Steam::requestCurrentStats);
	ClassDB::bind_method("requestGlobalAchievementPercentages", &Steam::requestGlobalAchievementPercentages);
	ClassDB::bind_method("requestGlobalStats", &Steam::requestGlobalStats);
	ClassDB::bind_method("requestUserStats", &Steam::requestUserStats);
	ClassDB::bind_method("resetAllStats", &Steam::resetAllStats);
	ClassDB::bind_method("setAchievement", &Steam::setAchievement);
	ClassDB::bind_method("setStatFloat", &Steam::setStatFloat);
	ClassDB::bind_method("setStatInt", &Steam::setStatInt);
	ClassDB::bind_method("storeStats", &Steam::storeStats);
	ClassDB::bind_method("updateAvgRateStat", &Steam::updateAvgRateStat);
	ClassDB::bind_method(D_METHOD("uploadLeaderboardScore", "score", "keep_best", "details"), &Steam::uploadLeaderboardScore, DEFVAL(true), DEFVAL(PoolIntArray()));
	ClassDB::bind_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
	ClassDB::bind_method(D_METHOD("setLeaderboardDetailsMax", "detailsMax"), &Steam::setLeaderboardDetailsMax);
	ClassDB::bind_method("getAchievementProgressLimitsInt", &Steam::getAchievementProgressLimitsInt);
	ClassDB::bind_method("getAchievementProgressLimitsFloat", &Steam::getAchievementProgressLimitsFloat);

	// Utils Bind Methods ///////////////////////
	ClassDB::bind_method("overlayNeedsPresent", &Steam::overlayNeedsPresent);
	ClassDB::bind_method("getAppID", &Steam::getAppID);
	ClassDB::bind_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
	ClassDB::bind_method(D_METHOD("getImageRGBA", "image"), &Steam::getImageRGBA);
	ClassDB::bind_method(D_METHOD("getImageSize", "image"), &Steam::getImageSize);
	ClassDB::bind_method("getIPCCallCount", &Steam::getIPCCallCount);
	ClassDB::bind_method("getIPCountry", &Steam::getIPCountry);
	ClassDB::bind_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
	ClassDB::bind_method("getSecondsSinceComputerActive", &Steam::getSecondsSinceComputerActive);
	ClassDB::bind_method("getServerRealTime", &Steam::getServerRealTime);
	ClassDB::bind_method("getSteamUILanguage", &Steam::getSteamUILanguage);
	ClassDB::bind_method("isOverlayEnabled", &Steam::isOverlayEnabled);
	ClassDB::bind_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
	ClassDB::bind_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
	ClassDB::bind_method("isVRHeadsetStreamingEnabled", &Steam::isVRHeadsetStreamingEnabled);
	ClassDB::bind_method("setOverlayNotificationInset", &Steam::setOverlayNotificationInset);
	ClassDB::bind_method(D_METHOD("setOverlayNotificationPosition", "pos"), &Steam::setOverlayNotificationPosition);
	ClassDB::bind_method("setVRHeadsetStreamingEnabled", &Steam::setVRHeadsetStreamingEnabled);
	ClassDB::bind_method("showGamepadTextInput", &Steam::showGamepadTextInput);
	ClassDB::bind_method("startVRDashboard", &Steam::startVRDashboard);
	ClassDB::bind_method("filterText", &Steam::filterText);
	ClassDB::bind_method("getAPICallFailureReason", &Steam::getAPICallFailureReason);
	ClassDB::bind_method("initFilterText", &Steam::initFilterText);
	ClassDB::bind_method("isAPICallCompleted", &Steam::isAPICallCompleted);
	ClassDB::bind_method("isSteamChinaLauncher", &Steam::isSteamChinaLauncher);
	
	/////////////////////////////////////////////
	// CALLBACK SIGNAL BINDS ////////////////////
	/////////////////////////////////////////////
	//
	// Apps Signals /////////////////////////////
	ADD_SIGNAL(MethodInfo("file_details_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "fileSize"), PropertyInfo(Variant::INT, "fileHash"), PropertyInfo(Variant::INT, "flags")));
	ADD_SIGNAL(MethodInfo("dlc_installed", PropertyInfo(Variant::INT, "app")));
	ADD_SIGNAL(MethodInfo("new_launch_url_parameters"));
//	ADD_SIGNAL(MethodInfo("new_launch_query_parameters"));
	
	// Friends Signals //////////////////////////
	ADD_SIGNAL(MethodInfo("avatar_loaded", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "size")));
	ADD_SIGNAL(MethodInfo("request_clan_officer_list"));
	ADD_SIGNAL(MethodInfo("clan_activity_downloaded"));
	ADD_SIGNAL(MethodInfo("friend_rich_presence_update"));
	ADD_SIGNAL(MethodInfo("enumerate_following_list"));
	ADD_SIGNAL(MethodInfo("get_follower_count"));
	ADD_SIGNAL(MethodInfo("is_following"));
	ADD_SIGNAL(MethodInfo("connected_chat_join"));
	ADD_SIGNAL(MethodInfo("connected_chat_leave"));
	ADD_SIGNAL(MethodInfo("connected_clan_chat_message"));
	ADD_SIGNAL(MethodInfo("_connected_friend_chat_message"));
	ADD_SIGNAL(MethodInfo("join_requested", PropertyInfo(Variant::INT, "from"), PropertyInfo(Variant::STRING, "connect_string")));
	ADD_SIGNAL(MethodInfo("overlay_toggled", PropertyInfo(Variant::BOOL, "active")));
	ADD_SIGNAL(MethodInfo("join_game_requested"));
	ADD_SIGNAL(MethodInfo("change_server_requested"));
	ADD_SIGNAL(MethodInfo("join_clan_chat_complete"));
	ADD_SIGNAL(MethodInfo("persona_state_change"));
	ADD_SIGNAL(MethodInfo("name_changed"));
	ADD_SIGNAL(MethodInfo("overlay_browser_protocol"));

	// HTML Surface Signals /////////////////////
	ADD_SIGNAL(MethodInfo("html_browser_ready"));
	ADD_SIGNAL(MethodInfo("html_can_go_backandforward"));
	ADD_SIGNAL(MethodInfo("html_changed_title"));
	ADD_SIGNAL(MethodInfo("html_close_browser"));
	ADD_SIGNAL(MethodInfo("html_file_open_dialog"));
	ADD_SIGNAL(MethodInfo("html_finished_request"));
	ADD_SIGNAL(MethodInfo("html_hide_tooltip"));
	ADD_SIGNAL(MethodInfo("html_horizontal_scroll"));
	ADD_SIGNAL(MethodInfo("html_js_alert"));
	ADD_SIGNAL(MethodInfo("html_js_confirm"));
	ADD_SIGNAL(MethodInfo("html_link_at_position"));
	ADD_SIGNAL(MethodInfo("html_needs_paint"));
	ADD_SIGNAL(MethodInfo("html_new_window"));
	ADD_SIGNAL(MethodInfo("html_open_link_in_new_tab"));
	ADD_SIGNAL(MethodInfo("html_search_results"));
	ADD_SIGNAL(MethodInfo("html_set_cursor"));
	ADD_SIGNAL(MethodInfo("html_show_tooltip"));
	ADD_SIGNAL(MethodInfo("html_start_request"));
	ADD_SIGNAL(MethodInfo("html_status_text"));
	ADD_SIGNAL(MethodInfo("html_update_tooltip"));
	ADD_SIGNAL(MethodInfo("html_url_changed"));
	ADD_SIGNAL(MethodInfo("html_vertical_scroll"));

	// HTTP Signals /////////////////////////////
	ADD_SIGNAL(MethodInfo("http_request_completed"));
	ADD_SIGNAL(MethodInfo("http_request_data_received"));
	ADD_SIGNAL(MethodInfo("http_request_headers_received"));

	// Inventory signals ////////////////////////
	ADD_SIGNAL(MethodInfo("inventory_definition_update"));
	ADD_SIGNAL(MethodInfo("inventory_eligible_promo_item"));
	ADD_SIGNAL(MethodInfo("inventory_full_update"));
	ADD_SIGNAL(MethodInfo("inventory_result_ready"));
	ADD_SIGNAL(MethodInfo("inventory_start_purchase_result"));
	ADD_SIGNAL(MethodInfo("inventory_request_prices_result"));

	// Matchmaking Signals //////////////////////
	ADD_SIGNAL(MethodInfo("favorites_list_accounts_updated"));
	ADD_SIGNAL(MethodInfo("favorites_list_changed"));
	ADD_SIGNAL(MethodInfo("lobby_message"));
	ADD_SIGNAL(MethodInfo("lobby_chat_update"));
	ADD_SIGNAL(MethodInfo("lobby_created"));
	ADD_SIGNAL(MethodInfo("lobby_data_update"));
	ADD_SIGNAL(MethodInfo("lobby_joined", PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "permissions"), PropertyInfo(Variant::BOOL, "locked"), PropertyInfo(Variant::INT, "response")));
	ADD_SIGNAL(MethodInfo("lobby_game_created"));
	ADD_SIGNAL(MethodInfo("lobby_invite", PropertyInfo(Variant::INT, "inviter"), PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "game")));
	ADD_SIGNAL(MethodInfo("lobby_match_list"));
	
	// Music Remote Signals /////////////////////
	ADD_SIGNAL(MethodInfo("music_player_remote_to_front"));
	ADD_SIGNAL(MethodInfo("music_player_remote_will_activate"));
	ADD_SIGNAL(MethodInfo("music_player_remote_will_deactivate"));
	ADD_SIGNAL(MethodInfo("music_player_selects_playlist_entry"));
	ADD_SIGNAL(MethodInfo("music_player_selects_queue_entry"));
	ADD_SIGNAL(MethodInfo("music_player_wants_looped"));
	ADD_SIGNAL(MethodInfo("music_player_wants_pause"));
	ADD_SIGNAL(MethodInfo("music_player_wants_playing_repeat_status"));
	ADD_SIGNAL(MethodInfo("music_player_wants_play_next"));
	ADD_SIGNAL(MethodInfo("music_player_wants_play_previous"));
	ADD_SIGNAL(MethodInfo("music_player_wants_play"));
	ADD_SIGNAL(MethodInfo("music_player_wants_shuffled"));
	ADD_SIGNAL(MethodInfo("music_player_wants_volume"));
	ADD_SIGNAL(MethodInfo("music_player_will_quit"));

	// Networking Signals ///////////////////////
	ADD_SIGNAL(MethodInfo("p2p_session_request"));
	ADD_SIGNAL(MethodInfo("p2p_session_connect_fail"));

	// Parties //////////////////////////////////
	ADD_SIGNAL(MethodInfo("join_party"));
	ADD_SIGNAL(MethodInfo("create_beacon"));
	ADD_SIGNAL(MethodInfo("reservation_notification"));
	ADD_SIGNAL(MethodInfo("change_num_open_slots"));
	ADD_SIGNAL(MethodInfo("available_beacon_locations_updated"));
	ADD_SIGNAL(MethodInfo("active_beacons_updated"));

	// Remote Play Signals //////////////////////
	ADD_SIGNAL(MethodInfo("remote_play_session_connected"));
	ADD_SIGNAL(MethodInfo("remote_play_session_disconnected"));

	// Remote Storage Signals ///////////////////
	ADD_SIGNAL(MethodInfo("file_read_async_complete"));
	ADD_SIGNAL(MethodInfo("file_share_result"));
	ADD_SIGNAL(MethodInfo("file_write_async_complete"));
	ADD_SIGNAL(MethodInfo("download_ugc_result"));
	ADD_SIGNAL(MethodInfo("unsubscribe_item"));
	ADD_SIGNAL(MethodInfo("subscribe_item"));

	// Screenshot Signals ///////////////////////
	ADD_SIGNAL(MethodInfo("screenshot_ready", PropertyInfo(Variant::INT, "screenshot_handle"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("screenshot_requested"));

	// UGC Signals //////////////////////////////
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

	// User Signals /////////////////////////////
	ADD_SIGNAL(MethodInfo("client_game_server_deny"));
	ADD_SIGNAL(MethodInfo("duration_control"));
	ADD_SIGNAL(MethodInfo("encrypted_app_ticket_response"));
	ADD_SIGNAL(MethodInfo("game_web_callback"));
	ADD_SIGNAL(MethodInfo("get_auth_session_ticket_response", PropertyInfo(Variant::INT, "ticket"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("ipc_failure"));	
	ADD_SIGNAL(MethodInfo("licenses_updated"));
	ADD_SIGNAL(MethodInfo("microstransaction_auth_response"));
	ADD_SIGNAL(MethodInfo("steam_server_connect_failed"));
	ADD_SIGNAL(MethodInfo("steam_server_connected"));
	ADD_SIGNAL(MethodInfo("steam_server_disconnected"));
	ADD_SIGNAL(MethodInfo("store_auth_url_response"));
	ADD_SIGNAL(MethodInfo("validate_auth_ticket_response", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "auth_session_reponse"), PropertyInfo(Variant::INT, "owner_steamID")));

	// User Stats Signals ///////////////////////
	ADD_SIGNAL(MethodInfo("global_achievement_percentages_ready", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("global_stats_received"));
	ADD_SIGNAL(MethodInfo("leaderboard_find_result", PropertyInfo(Variant::INT, "leaderboard"), PropertyInfo(Variant::INT, "found")));
	ADD_SIGNAL(MethodInfo("leaderboard_scores_downloaded"));
	ADD_SIGNAL(MethodInfo("leaderboard_score_uploaded", PropertyInfo(Variant::BOOL, "success"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::BOOL, "score_changed"), PropertyInfo(Variant::INT, "global_rank_new"), PropertyInfo(Variant::INT, "global_rank_previous")));
	ADD_SIGNAL(MethodInfo("leaderboard_ugc_set"));
	ADD_SIGNAL(MethodInfo("number_of_current_players", PropertyInfo(Variant::BOOL, "success"), PropertyInfo(Variant::INT, "players")));
	ADD_SIGNAL(MethodInfo("user_achievement_stored"));
	ADD_SIGNAL(MethodInfo("current_stats_received", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "userID")));
	ADD_SIGNAL(MethodInfo("user_stats_received", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "userID")));
	ADD_SIGNAL(MethodInfo("user_stats_stored"));
	ADD_SIGNAL(MethodInfo("user_stats_unloaded"));

	// Utility Signals //////////////////////////
	ADD_SIGNAL(MethodInfo("check_file_signature"));
	ADD_SIGNAL(MethodInfo("gamepad_text_input_dismissed"));
	ADD_SIGNAL(MethodInfo("ip_country"));
	ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));
	ADD_SIGNAL(MethodInfo("steam_api_call_completed"));
	ADD_SIGNAL(MethodInfo("steam_shutdown"));

	// Video Signals ////////////////////////////
//	ADD_SIGNAL(MethodInfo("broadcast_upload_start"));	// In documentation but not in actual SDK?
//	ADD_SIGNAL(MethodInfo("broadcast_upload_stop"));	// In documentation but not in actual SDK?
	ADD_SIGNAL(MethodInfo("get_opf_settings_result"));
	ADD_SIGNAL(MethodInfo("get_video_result"));

	/////////////////////////////////////////////
	// CONSTANT BINDS ///////////////////////////
	/////////////////////////////////////////////
	//
	// Steam API constants //////////////////////
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

	// Friends constants ////////////////////////
	BIND_CONSTANT(CHAT_METADATA_MAX);													// 8192
	BIND_CONSTANT(ENUMERATED_FOLLOWERS_MAX);											// 50
	BIND_CONSTANT(FRIENDS_GROUP_LIMIT);													// 100
	BIND_CONSTANT(INVALID_FRIEND_GROUP_ID);												// -1
	BIND_CONSTANT(MAX_FRIENDS_GROUP_NAME);												// 64
	BIND_CONSTANT(MAX_RICH_PRESENCE_KEY_LENGTH);										// 64
	BIND_CONSTANT(MAX_RICH_PRESENCE_KEYS);												// 20
	BIND_CONSTANT(MAX_RICH_PRESENCE_VALUE_LENTH);										// 256
	BIND_CONSTANT(PERSONA_NAME_MAX_UTF8);												// 128
	BIND_CONSTANT(PERSONA_NAME_MAX_UTF16);												// 32

	// HTML Surface constants ///////////////////
	BIND_CONSTANT(INVALID_HTMLBROWSER);													// 0

	// HTTP constants ///////////////////////////
	BIND_CONSTANT(INVALID_HTTPCOOKIE_HANDLE);											// 0
	BIND_CONSTANT(INVALID_HTTPREQUEST_HANDLE);											// 0

	// Input constants //////////////////////////
	BIND_CONSTANT(INPUT_MAX_ANALOG_ACTIONS);											// 16
	BIND_CONSTANT(INPUT_MAX_ANALOG_ACTION_DATA);										// 1.0f
	BIND_CONSTANT(INPUT_MAX_COUNT);														// 16
	BIND_CONSTANT(INPUT_MAX_DIGITAL_ACTIONS);											// 128
	BIND_CONSTANT(INPUT_MAX_ORIGINS);													// 8
	BIND_CONSTANT(INPUT_MIN_ANALOG_ACTION_DATA);										// -1.0f

	// Inventory constants //////////////////////
	BIND_CONSTANT(INVENTORY_RESULT_INVALID);											// -1
	BIND_CONSTANT(ITEM_INSTANCE_ID_INVALID);											// 0

	// Matchmaking constants ////////////////////
	BIND_CONSTANT(SERVER_QUERY_INVALID);												// 0xffffffff
	BIND_CONSTANT(MAX_LOBBY_KEY_LENGTH);												// 255
	BIND_CONSTANT(FAVORITE_FLAG_FAVORITE);												// 0x01
	BIND_CONSTANT(FAVORITE_FLAG_HISTORY);												// 0x02
	BIND_CONSTANT(FAVORITE_FLAG_NONE);													// 0x00

	// Matchmaking Servers constants ////////////
	BIND_CONSTANT(MAX_GAME_SERVER_GAME_DATA);											// 2048
	BIND_CONSTANT(MAX_GAME_SERVER_GAME_DESCRIPTION);									// 64
	BIND_CONSTANT(MAX_GAME_SERVER_GAME_DIR);											// 32
	BIND_CONSTANT(MAX_GAME_SERVER_MAP_NAME);											// 32
	BIND_CONSTANT(MAX_GAME_SERVER_NAME);												// 64
	BIND_CONSTANT(MAX_GAME_SERVER_TAGS);												// 128

	// Music Remote constants ///////////////////
	BIND_CONSTANT(MUSIC_NAME_MAX_LENGTH); 												// 255
	BIND_CONSTANT(MUSIC_PNG_MAX_LENGTH); 												// 65535

	// Remote Play constants ////////////////////
	BIND_CONSTANT(DEVICE_FORM_FACTOR_UNKNOWN);											// 0
	BIND_CONSTANT(DEVICE_FORM_FACTOR_PHONE);											// 1
	BIND_CONSTANT(DEVICE_FORM_FACTOR_TABLET);											// 2
	BIND_CONSTANT(DEVICE_FORM_FACTOR_COMPUTER);											// 3
	BIND_CONSTANT(DEVICE_FORM_FACTOR_TV);												// 4

	// Remote Storage constants /////////////////
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

	// Screenshot constants /////////////////////
	BIND_CONSTANT(INVALID_SCREENSHOT_HANDLE); 											// 0
	BIND_CONSTANT(UFS_TAG_TYPE_MAX); 													// 255
	BIND_CONSTANT(UFS_TAG_VALUE_MAX); 													// 255
	BIND_CONSTANT(MAX_TAGGED_PUBLISHED_FILES); 											// 32
	BIND_CONSTANT(MAX_TAGGED_USERS); 													// 32
	BIND_CONSTANT(SCREENSHOT_THUMB_WIDTH); 												// 200

	// UGC constants ////////////////////////////
	BIND_CONSTANT(NUM_UGC_RESULTS_PER_PAGE); 											// 50
	BIND_CONSTANT(DEVELOPER_METADATA_MAX); 												// 5000
	BIND_CONSTANT(UGC_QUERY_HANDLE_INVALID); 											// 0xffffffffffffffffull
	BIND_CONSTANT(UGC_UPDATE_HANDLE_INVALID); 											// 0xffffffffffffffffull

	// User Stats constants /////////////////////
	BIND_CONSTANT(LEADERBOARD_DETAIL_MAX);												// 64
	BIND_CONSTANT(LEADERBOARD_NAME_MAX);												// 128
	BIND_CONSTANT(STAT_NAME_MAX);														// 128

	/////////////////////////////////////////////
	// ENUM CONSTANT BINDS //////////////////////
	/////////////////////////////////////////////
	//
	// AccountType enum constants ///////////////
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

	// AppOwnershipFlags enum constants /////////
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

	// AppReleaseState enum constants ///////////
	BIND_CONSTANT(APP_RELEASE_STATE_UNKNOWN);											// 0
	BIND_CONSTANT(APP_RELEASE_STATE_UNAVAILABLE);										// 1
	BIND_CONSTANT(APP_RELEASE_STATE_PRERELEASE);										// 2
	BIND_CONSTANT(APP_RELEASE_STATE_PRELOAD_ONLY);										// 3
	BIND_CONSTANT(APP_RELEASE_STATE_RELEASED);											// 4

	// AppType enum constants ///////////////////
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

	// AuthSessionResponse enum constants ///////
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

	// BeginAuthSessionResult enum constants ////
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_OK);										// 0
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET);							// 1
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST);							// 2
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION);							// 3
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH);								// 4
	BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET);							// 5

	// BroadcastUploadResult enum constants /////
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

	// ChatEntryType enum constants /////////////
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

	// ChatRoomEnterResponse enum constants /////
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

	// ChatSteamIDInstanceFlags enum constants //
	BIND_CONSTANT(CHAT_ACCOUNT_INSTANCE_MASK);											// 0X00000FFF
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_CLAN);												// ((k_unSteamAccountInstanceMask + 1) >> 1)
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_LOBBY);											// ((k_unSteamAccountInstanceMask + 1) >> 2)
	BIND_CONSTANT(CHAT_INSTANCE_FLAG_MMS_LOBBY);										// ((k_unSteamAccountInstanceMask + 1) >> 3)

	// DenyReason enum constants ////////////////
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

	// GameIDType enum constants ////////////////
	BIND_CONSTANT(GAME_TYPE_APP);														// 0
	BIND_CONSTANT(GAME_TYPE_GAME_MOD);													// 1
	BIND_CONSTANT(GAME_TYPE_SHORTCUT);													// 2
	BIND_CONSTANT(GAME_TYPE_P2P);														// 3

	// LaunchOptionType enum constants //////////////
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

	// MarketingMessageFlags enum constants /////////
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_NONE);										// 0
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_HIGH_PRIORITY);								// (1<<0)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_WINDOWS);							// (1<<1)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_MAC);								// (1<<2)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_LINUX);								// (1<<3)
	BIND_CONSTANT(MARKETING_MESSAGE_FLAGS_PLATFORM_RESTRICTIONS);

	// NotificationPosition enum constants //////////
	BIND_CONSTANT(POSITION_TOP_LEFT);													// 0
	BIND_CONSTANT(POSITION_TOP_RIGHT);													// 1
	BIND_CONSTANT(POSITION_BOTTOM_LEFT);												// 2
	BIND_CONSTANT(POSITION_BOTTOM_RIGHT);												// 3

	// Result enum constants ////////////////////
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

	// SteamUserStatType enum constants /////////
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_INVALID);										// 0
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_INT);											// 1
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_FLOAT);											// 2
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_AVGRATE);										// 3
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_ACHIEVEMENTS);									// 4
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_GROUPACHIEVEMENTS);								// 5
	BIND_CONSTANT(STEAM_USER_STAT_TYPE_MAX);											// 6

	// Universe enum constants //////////////////
	BIND_CONSTANT(UNIVERSE_INVALID);													// 0
	BIND_CONSTANT(UNIVERSE_PUBLIC);														// 1
	BIND_CONSTANT(UNIVERSE_BETA);														// 2
	BIND_CONSTANT(UNIVERSE_INTERNAL);													// 3
	BIND_CONSTANT(UNIVERSE_DEV);														// 4
	BIND_CONSTANT(UNIVERSE_MAX);														// 5

	// UserHasLicenseForAppResult enum constants
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_HAS_LICENSE);									// 0
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_DOES_NOT_HAVE_LICENSE);						// 1
	BIND_CONSTANT(USER_HAS_LICENSE_RESULT_NO_AUTH);										// 2

	// VoiceResult enum constants ///////////////
	BIND_CONSTANT(VOICE_RESULT_OK);														// 0
	BIND_CONSTANT(VOICE_RESULT_NOT_INITIALIZED);										// 1
	BIND_CONSTANT(VOICE_RESULT_NOT_RECORDING);											// 2
	BIND_CONSTANT(VOICE_RESULT_NO_DATE);												// 3
	BIND_CONSTANT(VOICE_RESULT_BUFFER_TOO_SMALL);										// 4
	BIND_CONSTANT(VOICE_RESULT_DATA_CORRUPTED);											// 5
	BIND_CONSTANT(VOICE_RESULT_RESTRICTED);												// 6

	// VRHMDType enum constants /////////////////
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

	// FriendFlags enum constants ///////////////
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

	// FriendRelationship enum constants ////////
	BIND_CONSTANT(FRIEND_RELATION_NONE);												// 0
	BIND_CONSTANT(FRIEND_RELATION_BLOCKED);												// 1
	BIND_CONSTANT(FRIEND_RELATION_REQUEST_RECIPIENT);									// 2
	BIND_CONSTANT(FRIEND_RELATION_FRIEND);												// 3
	BIND_CONSTANT(FRIEND_RELATION_REQUEST_INITIATOR);									// 4
	BIND_CONSTANT(FRIEND_RELATION_IGNORED);												// 5
	BIND_CONSTANT(FRIEND_RELATION_IGNORED_FRIEND);										// 6
	BIND_CONSTANT(FRIEND_RELATION_SUGGESTED);											// 7
	BIND_CONSTANT(FRIEND_RELATION_MAX);													// 8

	// OverlayToStoreFlag enum constants ////////
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_NONE);											// 0
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_ADD_TO_CART);									// 1
	BIND_CONSTANT(OVERLAY_TO_STORE_FLAG_AND_TO_CART_AND_SHOW);							// 2

	// PersonaChange enum constants /////////////
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

	// PersonaState enum constants //////////////
	BIND_CONSTANT(PERSONA_STATE_OFFLINE);												// 0
	BIND_CONSTANT(PERSONA_STATE_ONLINE);												// 1
	BIND_CONSTANT(PERSONA_STATE_BUSY);													// 2
	BIND_CONSTANT(PERSONA_STATE_AWAY);													// 3
	BIND_CONSTANT(PERSONA_STATE_SNOOZE);												// 4
	BIND_CONSTANT(PERSONA_STATE_LOOKING_TO_TRADE);										// 5
	BIND_CONSTANT(PERSONA_STATE_LOOKING_TO_PLAY);										// 6
	BIND_CONSTANT(PERSONA_STATE_MAX);													// 7

	// UserRestriction enum constants ///////////
	BIND_CONSTANT(USER_RESTRICTION_NONE);												// 0
	BIND_CONSTANT(USER_RESTRICTION_UNKNOWN);											// 1
	BIND_CONSTANT(USER_RESTRICTION_ANY_CHAT);											// 2
	BIND_CONSTANT(USER_RESTRICTION_VOICE_CHAT);											// 4
	BIND_CONSTANT(USER_RESTRICTION_GROUP_CHAT);											// 8
	BIND_CONSTANT(USER_RESTRICTION_RATING);												// 16
	BIND_CONSTANT(USER_RESTRICTION_GAME_INVITES);										// 32
	BIND_CONSTANT(USER_RESTRICTION_TRADING);											// 64

	// HTMLKeyModifiers enum constants //////////
	BIND_CONSTANT(HTML_KEY_MODIFIER_NONE);												// 0
	BIND_CONSTANT(HTML_KEY_MODIFIER_ALT_DOWN);											// (1<<0)
	BIND_CONSTANT(HTML_KEY_MODIFIER_CTRL_DOWN);											// (1<<1)
	BIND_CONSTANT(HTML_KEY_MODIFIER_SHIFT_DOWN);										// (1<<2)

	// HTMLMouseButton enum constants ///////////
	BIND_CONSTANT(HTML_MOUSE_BUTTON_LEFT);												// 0
	BIND_CONSTANT(HTML_MOUSE_BUTTON_RIGHT);												// 1
	BIND_CONSTANT(HTML_MOUSE_BUTTON_MIDDLE);											// 2

	// MouseCursor enum constants ///////////////
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

	// HTTPMethod enum constants ////////////////
	BIND_CONSTANT(HTTP_METHOD_INVALID);													// 0
	BIND_CONSTANT(HTTP_METHOD_GET);														// 1
	BIND_CONSTANT(HTTP_METHOD_HEAD);													// 2
	BIND_CONSTANT(HTTP_METHOD_POST);													// 3
	BIND_CONSTANT(HTTP_METHOD_PUT);														// 4
	BIND_CONSTANT(HTTP_METHOD_DELETE);													// 5
	BIND_CONSTANT(HTTP_METHOD_OPTIONS);													// 6
	BIND_CONSTANT(HTTP_METHOD_PATCH);													// 7

	// HTTPStatusCode enum constants ////////////
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

	// InputActionOrigin enum constants /////////
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
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_COUNT);											// 258
	BIND_CONSTANT(INPUT_ACTION_ORIGIN_MAXIMUMPOSSIBLEVALUE);							// 32767

	// SteamItemFlags enum constants ////////////
	BIND_CONSTANT(STEAM_ITEM_NO_TRADE);													// (1<<0)
	BIND_CONSTANT(STEAM_ITEM_REMOVED);													// (1<<8)
	BIND_CONSTANT(STEAM_ITEM_CONSUMED);													// (1<<9)

	// ChatMemberStateChange enum constants /////
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_ENTERED);									// 0X0001
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_LEFT);										// 0X0002
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_DISCONNECTED);								// 0X0004
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_KICKED);										// 0X0008
	BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_BANNED);										// 0X0010

	// LobbyComparison enum constants ///////////
	BIND_CONSTANT(LOBBY_COMPARISON_EQUAL_TO_OR_LESS_THAN);								// -2
	BIND_CONSTANT(LOBBY_COMPARISON_LESS_THAN);											// -1
	BIND_CONSTANT(LOBBY_COMPARISON_EQUAL);												// 0
	BIND_CONSTANT(LOBBY_COMPARISON_GREATER_THAN);										// 1
	BIND_CONSTANT(LOBBY_COMPARISON_EQUAL_TO_GREATER_THAN);								// 2
	BIND_CONSTANT(LOBBY_COMPARISON_NOT_EQUAL);											// 3

	// LobbyDistanceFilter enum constants ///////
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_CLOSE);											// 0
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_DEFAULT);										// 1
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_FAR);											// 2
	BIND_CONSTANT(LOBBY_DISTANCE_FILTER_WORLDWIDE);										// 3

	// LobbyType enum constants /////////////////
	BIND_CONSTANT(LOBBY_TYPE_PRIVATE);													// 0
	BIND_CONSTANT(LOBBY_TYPE_FRIENDS_ONLY);												// 1
	BIND_CONSTANT(LOBBY_TYPE_PUBLIC);													// 2
	BIND_CONSTANT(LOBBY_TYPE_INVISIBLE);												// 3

	// MatchMakingServerResponse enum constants /
	BIND_CONSTANT(SERVER_RESPONDED);													// 0
	BIND_CONSTANT(SERVER_FAILED_TO_RESPOND);											// 1
	BIND_CONSTANT(NO_SERVERS_LISTED_ON_MASTER_SERVER);									// 2

	// AudioPlayback_Status enum constants //////
	BIND_CONSTANT(AUDIO_PLAYBACK_UNDEFINED);											// 0
	BIND_CONSTANT(AUDIO_PLAYBACK_PLAYING);												// 1
	BIND_CONSTANT(AUDIO_PLAYBACK_PAUSED);												// 2
	BIND_CONSTANT(AUDIO_PLAYBACK_IDLE);													// 3

	// P2PSend enum constants ///////////////////
	BIND_CONSTANT(P2P_SEND_UNRELIABLE);													// 0
	BIND_CONSTANT(P2P_SEND_UNRELIABLE_NO_DELAY);										// 1
	BIND_CONSTANT(P2P_SEND_RELIABLE);													// 2
	BIND_CONSTANT(P2P_SEND_RELIABLE_WITH_BUFFERING);									// 3

	// P2PSessionError enum constants ///////////
	BIND_CONSTANT(P2P_SESSION_ERROR_NONE);												// 0
	BIND_CONSTANT(P2P_SESSION_ERROR_NOT_RUNNING_APP);									// 1
	BIND_CONSTANT(P2P_SESSION_ERROR_NO_RIGHTS_TO_APP);									// 2
	BIND_CONSTANT(P2P_SESSION_ERROR_DESTINATION_NOT_LOGGED_ON);							// 3
	BIND_CONSTANT(P2P_SESSION_ERROR_TIMEOUT);											// 4
	BIND_CONSTANT(P2P_SESSION_ERROR_MAX);												// 5

	// SNetSocketConnectionType enum constants //
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_NOT_CONNECTED);							// 0
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_UDP);										// 1
	BIND_CONSTANT(NET_SOCKET_CONNECTION_TYPE_UDP_RELAY);								// 2

	// SNetSocketState enum constants ///////////
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

	// SteamPartyBeaconLocationType enum constants
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATIONTYPE_INVALID);								// 0
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATIONTYPE_CHAT_GROUP);							// 1
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_TYPE_MAX);

	// SteamPartyBeaconLocationData enum constants
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA);									// 0
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_NAME);								// 1
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_URL_SMALL);
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_URL_MEDIUM);
	BIND_CONSTANT(STEAM_PARTY_BEACON_LOCATION_DATA_URL_LARGE);

	// RemoteStoragePlatform enum constants /////
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_NONE);										// 0
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_WINDOWS);										// (1<<0)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_OSX);											// (1<<1)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_PS3);											// (1<<2)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_LINUX);										// (1<<3)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_RESERVED2);									// (1<<4)
	BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_ALL);											// 0XFFFFFFFF

	// RemoteStoragePublishedFileVisibility enum constants
	BIND_CONSTANT(REMOTE_STORAGE_PUBLISHED_VISIBLITY_PUBLIC);							// 0
	BIND_CONSTANT(REMOTE_STORAGE_PUBLISHED_VISIBLITY_FRIENDS_ONLY);						// 1
	BIND_CONSTANT(REMOTE_STORAGE_PUBLISHED_VISIBLITY_PRIVATE);							// 2

	// UGCReadAction enum constants /////////////
	BIND_CONSTANT(UGC_READ_CONTINUE_READING_UNTIL_FINISHED);							// 0
	BIND_CONSTANT(UGC_READ_CONTINUE_READING);											// 1
	BIND_CONSTANT(UGC_READ_CLOSE);														// 2

	// WorkshopEnumerationType enum constants ///
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_RANKED_BY_VOTE);							// 0
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_RECENT);									// 1
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_TRENDING);									// 2
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_FAVORITES_OF_FRIENDS);						// 3
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_VOTED_BY_FRIENDS);							// 4
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_CONTENT_BY_FRIENDS);						// 5
	BIND_CONSTANT(WORKSHOP_ENUMERATION_TYPE_RECENT_FROM_FOLLOWED_USERS);				// 6

	// WorkshopFileAction enum constants ////////
	BIND_CONSTANT(WORKSHOP_FILE_ACTION_PLAYED);											// 0
	BIND_CONSTANT(WORKSHOP_FILE_ACTION_COMPLETED);										// 1

	// WorkshopFileType enum constants //////////
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

	// WorkshopVideoProvider enum constants /////
	BIND_CONSTANT(WORKSHOP_VIDEO_PROVIDER_NONE);										// 0
	BIND_CONSTANT(WORKSHOP_VIDEO_PROVIDER_YOUTUBE);										// 1

	// WorkshopVote enum constants //////////////
	BIND_CONSTANT(WORKSHOP_VOTE_UNVOTED);												// 0
	BIND_CONSTANT(WORKSHOP_VOTE_FOR);													// 1
	BIND_CONSTANT(WORKSHOP_VOTE_AGAINST);												// 2
	BIND_CONSTANT(WORKSHOP_VOTE_LATER);													// 3

	// VRScreenshotType enum constants //////////
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_NONE);												// 0
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_MONO);												// 1
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_STEREO);											// 2
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_MONO_CUBE_MAP);									// 3
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_MONO_PANORAMA);									// 4
	BIND_CONSTANT(VR_SCREENSHOT_TYPE_STEREO_PANORAMA);									// 5

	// ItemPreviewType enum constants ///////////
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_IMAGE);												// 0
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_YOUTUBE_VIDEO);										// 1
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_SKETCHFAB);											// 2
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_HORIZONTAL_CROSS);					// 3
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_LAT_LONG);							// 4
	BIND_CONSTANT(ITEM_PREVIEW_TYPE_RESERVED_MAX);										// 255

	// ItemState enum constants /////////////////
	BIND_CONSTANT(ITEM_STATE_NONE);														// 0
	BIND_CONSTANT(ITEM_STATE_SUBSCRIBED);												// 1
	BIND_CONSTANT(ITEM_STATE_LEGACY_ITEM);												// 2
	BIND_CONSTANT(ITEM_STATE_INSTALLED);												// 4
	BIND_CONSTANT(ITEM_STATE_NEEDS_UPDATE);												// 8
	BIND_CONSTANT(ITEM_STATE_DOWNLOADING);												// 16
	BIND_CONSTANT(ITEM_STATE_DOWNLOAD_PENDING);											// 32

	// ItemStatistic enum constants /////////////
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

	// ItemUpdateStatus enum constants //////////
	BIND_CONSTANT(ITEM_UPDATE_STATUS_INVALID);											// 0
	BIND_CONSTANT(ITEM_UPDATE_STATUS_PREPARING_CONFIG);									// 1
	BIND_CONSTANT(ITEM_UPDATE_STATUS_PREPARING_CONTENT);								// 2
	BIND_CONSTANT(ITEM_UPDATE_STATUS_UPLOADING_CONTENT);								// 3
	BIND_CONSTANT(ITEM_UPDATE_STATUS_UPLOADING_PREVIEW_FILE);							// 4
	BIND_CONSTANT(ITEM_UPDATE_STATUS_COMMITTING_CHANGES);								// 5

	// UGCMatchingUGCType enum constants ////////
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

	// UGCQuery enum constants //////////////////
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

	// UserUGCList enum constants ///////////////
	BIND_CONSTANT(USER_UGC_LIST_PUBLISHED);												// 0
	BIND_CONSTANT(USER_UGC_LIST_VOTED_ON);												// 1
	BIND_CONSTANT(USER_UGC_LIST_VOTED_UP);												// 2
	BIND_CONSTANT(USER_UGC_LIST_VOTED_DOWN);											// 3
	BIND_CONSTANT(USER_UGC_LIST_FAVORITED);												// 5
	BIND_CONSTANT(USER_UGC_LIST_SUBSCRIBED);											// 6
	BIND_CONSTANT(USER_UGC_LIST_USED_OR_PLAYED);										// 7
	BIND_CONSTANT(USER_UGC_LIST_FOLLOWED);												// 8

	// UserUGCListSortOrder enum constants //////
	BIND_CONSTANT(USERUGCLISTSORTORDER_CREATIONORDERDESC);								// 0
	BIND_CONSTANT(USERUGCLISTSORTORDER_CREATIONORDERASC);								// 1
	BIND_CONSTANT(USERUGCLISTSORTORDER_TITLEASC);										// 2
	BIND_CONSTANT(USERUGCLISTSORTORDER_LASTUPDATEDDESC);								// 3
	BIND_CONSTANT(USERUGCLISTSORTORDER_SUBSCRIPTIONDATEDESC);							// 4
	BIND_CONSTANT(USERUGCLISTSORTORDER_VOTESCOREDESC);									// 5
	BIND_CONSTANT(USERUGCLISTSORTORDER_FORMODERATION);									// 6

	// FailureType enum constants ///////////////
	BIND_CONSTANT(FAILURE_FLUSHED_CALLBACK_QUEUE);										// 0
	BIND_CONSTANT(FAILURE_PIPE_FAIL);													// 1

	// DurationControlProgress enum constants ///
	BIND_CONSTANT(DURATION_CONTROL_PROGRESS_FULL);										// 0
	BIND_CONSTANT(DURATION_CONTROL_PROGRESS_HALF);										// 1
	BIND_CONSTANT(DURATION_CONTROL_PROGRESS_NONE);										// 2

	// DurationControlNotification enum constants
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_NONE);									// 0
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_1_HOUR);								// 1
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_3_HOURS);								// 3
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_HALF_PROGRESS);							// 3
	BIND_CONSTANT(DURATION_CONTROL_NOTIFICATION_NO_PROGRESS);							// 4

	// LeaderboardDataRequest enum constants ////
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_GLOBAL);										// 0
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_GLOBAL_AROUND_USER);							// 1
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_FRIENDS);									// 2
	BIND_CONSTANT(LEADERBOARD_DATA_REQUEST_USERS);										// 3

	// LeaderboardDisplayType enum constants ////
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_NONE);										// 0
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_NUMERIC);									// 1
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS);								// 2
	BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS);							// 3

	// LeaderboardSortMethod enum constants /////
	BIND_CONSTANT(LEADERBOARD_SORT_METHOD_NONE);										// 0
	BIND_CONSTANT(LEADERBOARD_SORT_METHOD_ASCENDING);									// 1
	BIND_CONSTANT(LEADERBOARD_SORT_METHOD_DESCENDING);									// 2

	// LeaderboardUploadScoreMethod enum constants
	BIND_CONSTANT(LEADERBOARD_UPLOAD_SCORE_METHOD);										// 0
	BIND_CONSTANT(LEADERBOARD_UPLOAD_SCORE_METHOD_KEEP_BEST);							// 1
	BIND_CONSTANT(LEADERBOARD_UPLOAD_SCORE_METHOD_FORCE_UPDATE);						// 2

	// CheckFileSignature enum constants ////////
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_INVALID_SIGNATURE);								// 0
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_VALID_SIGNATURE);								// 1
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_FILE_NOT_FOUND);									// 2
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_APP);				// 3
	BIND_CONSTANT(CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_FILE);				// 4

	// GamepadTextInputLineMode enum constants //
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_LINE_MODE_SINGLE_LINE);							// 0
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_LINE_MODE_MULTIPLE_LINES);							// 1

	// GamepadTextInputMode enum constants //////
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_MODE_NORMAL);										// 0
	BIND_CONSTANT(GAMEPAD_TEXT_INPUT_MODE_PASSWORD);									// 1

	// SteamAPICallFailure enum constants ///////
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_NONE);											// -1
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_STEAM_GONE);									// 0
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_NETWORK_FAILURE);								// 1
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_INVALID_HANDLE);								// 2
	BIND_CONSTANT(STEAM_API_CALL_FAILURE_MISMATCHED_CALLBACK);							// 3
}

Steam::~Steam(){
	// Store stats then shut down ///////////////
	if(isInitSuccess){
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	// Clear all tickets in memory //////////////
	for(int i = 0; i < tickets.size(); i++){
		TicketData ticket = tickets.get(i);
		memdelete_arr(ticket.buffer);
	}
	// Clear tickets, app ID, and singleton variables /////
	tickets.clear();
	singleton = NULL;
	currentAppID = 0;
}
