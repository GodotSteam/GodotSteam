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
// Include GodotSteam header
#include "godotsteam.h"

// Include some Godot headers
//#include "ip_address.hpp"
#include "IP.hpp"

// Include some system headers
#include "fstream"
#include "vector"

using namespace godot;

/////////////////////////////////////////////////
///// DEFINING CONSTANTS
/////////////////////////////////////////////////
//
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


/////////////////////////////////////////////////
///// DECLARING STRUCTS
/////////////////////////////////////////////////
//
struct SteamNetworkingIdentity networkingIdentity;
struct SteamNetworkingConfigValue_t networkingConfigValue;
struct SteamNetworkingIPAddr networkingIPAddress;
struct SteamNetworkPingLocation_t networkPingLocation;


/////////////////////////////////////////////////
///// STEAM OBJECT WITH CALLBACKS
/////////////////////////////////////////////////
//
Steam::Steam():
	// App callbacks ////////////////////////////
	callbackDLCInstalled(this, &Steam::_dlc_installed),
	callbackFileDetailsResult(this, &Steam::_file_details_result),
	callbackNewLaunchURLParameters(this, &Steam::_new_launch_url_parameters),
//	callbackNewLaunchQueryParameters(this, &Steam::_new_launch_query_parameters),  Seems not to be found?
	callbackRegisterActivationCodeResponse(this, &Steam::_register_activation_code_response),
	callbackAppProofOfPurchaseKeyResponse(this, &Steam::_app_proof_of_purchase_key_response),
	callbackTimedTrialStatus(this, &Steam::_timed_trial_status),

	// App List callbacks ///////////////////////
	callbackAppInstalled(this, &Steam::_app_installed),
	callbackAppUninstalled(this, &Steam::_app_uninstalled),

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
	callbackUnreadChatMessagesChanged(this, &Steam::_unread_chat_messages_changed),

	// Game Search callbacks ////////////////////
	callbackSearchForGameProgress(this, &Steam::_search_for_game_progress),
	callbackSearchForGameResult(this, &Steam::_search_for_game_result),
	callbackRequestPlayersForGameProgress(this, &Steam::_request_players_for_game_progress),
	callbackRequestPlayersForGameResult(this, &Steam::_request_players_for_game_result),
	callbackRequestPlayersForGameFinalResult(this, &Steam::_request_players_for_game_final_result),
	callbackSubmitPlayerResult(this, &Steam::_submit_player_result),
	callbackEndGameResult(this, &Steam::_end_game_result),

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

	// Input callbacks //////////////////////////
//	callbackInputActionEvent(this, &Steam::_input_action_event),
	callbackInputDeviceConnected(this, &Steam::_input_device_connected),
	callbackInputDeviceDisconnected(this, &Steam::_input_device_disconnected),
	callbackInputConfigurationLoaded(this, &Steam::_input_configuration_loaded),

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
	callbackLobbyKicked(this, &Steam::_lobby_kicked),

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

	// Networking Messages callbacks ////////////
	callbackNetworkMessagesSessionRequest(this, &Steam::_network_messages_session_request),
	callbackNetworkMessagesSessionFailed(this, &Steam::_network_messages_session_failed),

	// Networking Sockets callbacks /////////////
	callbackNetworkConnectionStatusChanged(this, &Steam::_network_connection_status_changed),
	callbackNetworkAuthenticationStatus(this, &Steam::_network_authentication_status),

	// Networking Utils callbacks ///////////////
	callbackRelayNetworkStatus(this, &Steam::_relay_network_status),

	// Parental Settings callbacks //////////////
	callbackParentlSettingChanged(this, &Steam::_parental_setting_changed),

	// Parties callbacks ////////////////////////
	callbackReserveNotification(this, &Steam::_reservation_notification),
	callbackAvailableBeaconLocationsUpdated(this, &Steam::_available_beacon_locations_updated),
	callbackActiveBeaconsUpdated(this, &Steam::_active_beacons_updated),

	// Remote Play callbacks ////////////////////
	callbackRemotePlaySessionConnected(this, &Steam::_remote_play_session_connected),
	callbackRemotePlaySessionDisconnected(this, &Steam::_remote_play_session_disconnected),

	// Remote Storage callbacks /////////////////
	callbackLocalFileChanged(this, &Steam::_local_file_changed),

	// Screenshot callbacks /////////////////////
	callbackScreenshotReady(this, &Steam::_screenshot_ready),
	callbackScreenshotRequested(this, &Steam::_screenshot_requested),

	// UGC callbacks ////////////////////////////
	callbackItemDownloaded(this, &Steam::_item_downloaded),
	callbackItemInstalled(this, &Steam::_item_installed),
	callbackUserSubscribedItemsListChanged(this, &Steam::_user_subscribed_items_list_changed),

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
	callbackAppResumingFromSuspend(this, &Steam::_app_resuming_from_suspend),
	callbackFloatingGamepadTextInputDismissed(this, &Steam::_floating_gamepad_text_input_dismissed),

	// Video callbacks //////////////////////////
//	callbackBroadcastUploadStart(this, &Steam::_broadcast_upload_start),	// In documentation but not in actual SDK?
//	callbackBroadcastUploadStop(this, &Steam::_broadcast_upload_stop),		// In documentation but not in actual SDK?
	callbackGetOPFSettingsResult(this, &Steam::_get_opf_settings_result),
	callbackGetVideoResult(this, &Steam::_get_video_result)
{
	is_init_success = false;
	current_app_id = 0;
}


/////////////////////////////////////////////////
///// INTERNAL FUNCTIONS
/////////////////////////////////////////////////
//
// Creating a Steam ID for internal use
CSteamID Steam::createSteamID(uint32 steam_id, int account_type){
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
// Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't.
bool Steam::restartAppIfNecessary(int value){
	return SteamAPI_RestartAppIfNecessary((AppId_t)value);
}

// Initialize Steamworks
Dictionary Steam::steamInit(bool retrieve_stats){
	// Create the response dictionary
	Dictionary initialize;
	// Attempt to initialize Steamworks
	is_init_success = SteamAPI_Init();
	// Set the default status response
	int status = RESULT_FAIL;
	String verbal = "Steamworks failed to initialize.";
	// Steamworks initialized with no problems
	if(is_init_success){
		status = RESULT_OK;
		verbal = "Steamworks active.";
	}
	// The Steam client is not running
	if(!isSteamRunning()){
		status = RESULT_SERVICE_UNAVAILABLE;
		verbal = "Steam not running.";
	}
	else if(SteamUser() == NULL){
		status = RESULT_UNEXPECTED_ERROR;
		verbal = "Invalid app ID or app not installed.";
	}
	// Steam is connected and active, so load the stats and achievements if requested
	if(status == RESULT_OK && SteamUserStats() != NULL && retrieve_stats){
		requestCurrentStats();
	}
	// Get this app ID
	current_app_id = getAppID();
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
///// APPS
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

//Checks if the active user is accessing the current app_id via a temporary Family Shared license owned by another user.
//If you need to determine the steam_id of the permanent owner of the license, use getAppOwner.
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
String Steam::getAppInstallDir(AppId_t app_id){
	if(SteamApps() == NULL){
		return "";
	}
	const uint32 folder_buffer = 256;
	char *buffer = new char[folder_buffer];
	SteamApps()->GetAppInstallDir(app_id, (char*)buffer, folder_buffer);
	String app_dir = buffer;
	delete[] buffer;
	return app_dir;
}

// Gets the Steam ID of the original owner of the current app. If it's different from the current user then it is borrowed.
uint64_t Steam::getAppOwner(){
	if(SteamApps() == NULL){
		return 0;
	}
	CSteamID converted_steam_id = SteamApps()->GetAppOwner();
	return converted_steam_id.ConvertToUint64();
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
	String beta_name = "";
	if(SteamApps() != NULL){
		char name_string[1024];
		if (SteamApps()->GetCurrentBetaName(name_string, 1024)) {
			beta_name = String(name_string);
		}
	}
	return beta_name;
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
Dictionary Steam::getDLCDownloadProgress(uint32_t app_id){
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
Array Steam::getInstalledDepots(uint32_t app_id){
	if(SteamApps() == NULL){
		return Array();
	}
	Array installed_depots;
	DepotId_t *depots = new DepotId_t[32];
	int installed = SteamApps()->GetInstalledDepots((AppId_t)app_id, depots, 32);
	for(int i = 0; i < installed; i++){
		installed_depots.append(depots[i]);
	}
	delete[] depots;
	return installed_depots;
}

// Gets the command line if the game was launched via Steam URL, e.g. steam://run/<appid>//<command line>/. This method is preferable to launching with a command line via the operating system, which can be a security risk. In order for rich presence joins to go through this and not be placed on the OS command line, you must enable "Use launch command line" from the Installation > General page on your app.
String Steam::getLaunchCommandLine(){
	if(SteamApps() == NULL){
		return "";
	}
	char commands;
	SteamApps()->GetLaunchCommandLine(&commands, 256);
	String command_line;
	command_line += commands;
	return command_line;
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
bool Steam::markContentCorrupt(bool missing_files_only){
	if(SteamApps() == NULL){
		return false;
	}
	return SteamApps()->MarkContentCorrupt(missing_files_only);
}

// Allows you to uninstall an optional DLC.
void Steam::uninstallDLC(int value){
	if(SteamApps() != NULL){
		SteamApps()->UninstallDLC((AppId_t)value);
	}
}

// Request all proof of purchase keys for the calling appid and asociated DLC.
// A series of AppProofOfPurchaseKeyResponse_t callbacks will be sent with appropriate appid values, ending with a final callback where the m_nAppId member is k_uAppIdInvalid (zero).
void Steam::requestAllProofOfPurchaseKeys(){
	if(SteamApps() != NULL){
		SteamApps()->RequestAllProofOfPurchaseKeys();
	}
}

// Request legacy cd-key for yourself or owned DLC. If you are interested in this data then make sure you provide us with a list of valid keys to be distributed to users when they purchase the game, before the game ships.
// You'll receive an AppProofOfPurchaseKeyResponse_t callback when the key is available (which may be immediately).
void Steam::requestAppProofOfPurchaseKey(AppId_t app_id){
	if(SteamApps() != NULL){
		SteamApps()->RequestAppProofOfPurchaseKey((AppId_t)app_id);
	}
}


/////////////////////////////////////////////////
///// APP LISTS
/////////////////////////////////////////////////
//
// This is a restricted interface that can only be used by previously approved apps, contact your Steam Account Manager if you believe you need access to this API.
//
// Get the number of installed apps for this player.
uint32 Steam::getNumInstalledApps(){
	if(SteamAppList() == NULL){
		return 0;
	}
	return SteamAppList()->GetNumInstalledApps();
}

// Get a list of app IDs for installed apps for this player.
Array Steam::getInstalledApps(uint32 max_app_ids){
	Array installed_apps;
	if(SteamAppList() != NULL){
		uint32 *app_ids = nullptr;
		uint32 these_apps = SteamAppList()->GetInstalledApps(app_ids, max_app_ids);
		// Which is greater?
		if(these_apps < max_app_ids){
			max_app_ids = these_apps;
		}
		// Parse it
		for(uint32 i = 0; i < max_app_ids; i++){
			installed_apps.append(app_ids[i]);
		}
	}
	return installed_apps;
}

// Get a given app ID's name.
String Steam::getAppName(AppId_t app_id, int name_max){
	String app_name = "";
	if(SteamAppList() != NULL){
		char app;
		int name_found = SteamAppList()->GetAppName((AppId_t)app_id, &app, name_max);
		if(name_found != -1){
			app_name += app;
		}
	}
	return app_name;
}

// Get a given app ID's install directory.
String Steam::getAppListInstallDir(AppId_t app_id, int name_max){
	String dir_name = "";
	if(SteamAppList() != NULL){
		char directory;
		int name_found = SteamAppList()->GetAppInstallDir((AppId_t)app_id, &directory, name_max);
		if(name_found != -1){
			dir_name += directory;
		}
	}
	return dir_name;
}

// Get a given app ID's build.
int Steam::getAppListBuildId(AppId_t app_id){
	if(SteamAppList() == NULL){
		return 0;
	}
	return SteamAppList()->GetAppBuildId((AppId_t)app_id);
}


/////////////////////////////////////////////////
///// FRIENDS
/////////////////////////////////////////////////
//
// Activates the overlay with optional dialog to open the following: "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite".
void Steam::activateGameOverlay(const String& url){
	if(SteamFriends() != NULL){
		SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
	}
}

// Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
void Steam::activateGameOverlayInviteDialog(uint64_t steam_id){
	if(SteamFriends() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamFriends()->ActivateGameOverlayInviteDialog(user_id);
	}
}

// Activates the game overlay to open an invite dialog that will send the provided Rich Presence connect string to selected friends.
void Steam::activateGameOverlayInviteDialogConnectString(const String& connect_string){
	if(SteamFriends() != NULL){
		SteamFriends()->ActivateGameOverlayInviteDialogConnectString(connect_string.utf8().get_data());
	}
}

// Activates the overlay with the application/game Steam store page.
void Steam::activateGameOverlayToStore(uint32_t app_id){
	if(SteamFriends() != NULL){
		SteamFriends()->ActivateGameOverlayToStore(AppId_t(app_id), EOverlayToStoreFlag(0));
	}
}

// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore".
void Steam::activateGameOverlayToUser(const String& url, uint64_t steam_id){
	if(SteamFriends() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), user_id);
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
bool Steam::closeClanChatWindowInSteam(uint64_t chat_id){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chat_id;
	return SteamFriends()->CloseClanChatWindowInSteam(chat);
}

// For clans a user is a member of, they will have reasonably up-to-date information, but for others you'll have to download the info to have the latest.
void Steam::downloadClanActivityCounts(uint64_t clan_id, int clans_to_request){
	if(SteamFriends() != NULL){
		clan_activity = (uint64)clan_id;
		SteamFriends()->DownloadClanActivityCounts(&clan_activity, clans_to_request);
	}
}

// Gets the list of users that the current user is following.
void Steam::enumerateFollowingList(uint32 start_index){
	if(SteamFriends() != NULL){
		SteamAPICall_t api_call = SteamFriends()->EnumerateFollowingList(start_index);
		callResultEnumerateFollowingList.Set(api_call, this, &Steam::_enumerate_following_list);
	}
}

// Gets the Steam ID at the given index in a Steam group chat.
uint64_t Steam::getChatMemberByIndex(uint64_t clan_id, int user){
	if(SteamFriends() == NULL){
		return 0;
	}
	clan_activity = (uint64)clan_id;
	CSteamID chat_id = SteamFriends()->GetChatMemberByIndex(clan_activity, user);
	return chat_id.ConvertToUint64();
}

// Gets the most recent information we have about what the users in a Steam Group are doing.
Dictionary Steam::getClanActivityCounts(uint64_t clan_id){
	Dictionary activity;
	if(SteamFriends() == NULL){
		return activity;
	}
	clan_activity = (uint64)clan_id;
	int online = 0;
	int ingame = 0;
	int chatting = 0;
	bool success = SteamFriends()->GetClanActivityCounts(clan_activity, &online, &ingame, &chatting);
	// Add these to the dictionary if successful
	if(success){
		activity["clan"] = clan_id;
		activity["online"] = online;
		activity["ingame"] = ingame;
		activity["chatting"] = chatting;
	}
	return activity;
}

// Gets the Steam group's Steam ID at the given index.
uint64_t Steam::getClanByIndex(int clan){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetClanByIndex(clan).ConvertToUint64();
}

// Get the number of users in a Steam group chat.
int Steam::getClanChatMemberCount(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	clan_activity = (uint64)clan_id;
	return SteamFriends()->GetClanChatMemberCount(clan_activity);
}

//  Gets the data from a Steam group chat room message.  This should only ever be called in response to a GameConnectedClanChatMsg_t callback.
Dictionary getClanChatMessage(uint64_t chat_id, int message){
	Dictionary chat_message;
	if(SteamFriends() == NULL){
		return chat_message;
	}
	CSteamID chat = (uint64)chat_id;
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	CSteamID user_id;
	chat_message["ret"] = SteamFriends()->GetClanChatMessage(chat, message, text, 2048, &type, &user_id);
	chat_message["text"] = String(text);
	chat_message["type"] = type;
	uint64_t user = user_id.ConvertToUint64();
	chat_message["chatter"] = user;
	return chat_message;
}

// Gets the number of Steam groups that the current user is a member of.  This is used for iteration, after calling this then GetClanByIndex can be used to get the Steam ID of each Steam group.
int Steam::getClanCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetClanCount();
}

// Gets the display name for the specified Steam group; if the local client knows about it.
String Steam::getClanName(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return "";
	}
	clan_activity = (uint64)clan_id;
	return String(SteamFriends()->GetClanName(clan_activity));
}

// Returns the steam_id of a clan officer, by index, of range [0,GetClanOfficerCount).
uint64_t Steam::getClanOfficerByIndex(uint64_t clan_id, int officer){
	if(SteamFriends() == NULL){
		return 0;
	}
	clan_activity = (uint64)clan_id;
	CSteamID officer_id = SteamFriends()->GetClanOfficerByIndex(clan_activity, officer);
	return officer_id.ConvertToUint64();
}

// Returns the number of officers in a clan (including the owner).
int Steam::getClanOfficerCount(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	clan_activity = (uint64)clan_id;
	return SteamFriends()->GetClanOfficerCount(clan_activity);
}

// Returns the steam_id of the clan owner.
uint64_t Steam::getClanOwner(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	clan_activity = (uint64)clan_id;
	CSteamID owner_id = SteamFriends()->GetClanOwner(clan_activity);
	return owner_id.ConvertToUint64();
}

// Gets the unique tag (abbreviation) for the specified Steam group; If the local client knows about it.  The Steam group abbreviation is a unique way for people to identify the group and is limited to 12 characters. In some games this will appear next to the name of group members.
String Steam::getClanTag(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return "";
	}
	clan_activity = (uint64)clan_id;
	return String(SteamFriends()->GetClanTag(clan_activity));
}

// Gets the Steam ID of the recently played with user at the given index.
uint64_t Steam::getCoplayFriend(int friend_number){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID friend_id = SteamFriends()->GetCoplayFriend(friend_number);
	return friend_id.ConvertToUint64();
}

// Gets the number of players that the current users has recently played with, across all games.  This is used for iteration, after calling this then GetCoplayFriend can be used to get the Steam ID of each player.  These players are have been set with previous calls to SetPlayedWith.
int Steam::getCoplayFriendCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetCoplayFriendCount();
}

// Gets the number of users following the specified user.
void Steam::getFollowerCount(uint64_t steam_id){
	if(SteamFriends() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamAPICall_t api_call = SteamFriends()->GetFollowerCount(user_id);
		callResultFollowerCount.Set(api_call, this, &Steam::_get_follower_count);
	}
}

// Returns the Steam ID of a user.
uint64_t Steam::getFriendByIndex(int friend_number, int friend_flags){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID friend_id = SteamFriends()->GetFriendByIndex(friend_number, friend_flags);
	return friend_id.ConvertToUint64();
}

// Gets the app ID of the game that user played with someone on their recently-played-with list.
int Steam::getFriendCoplayGame(uint64_t friend_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID steam_id = (uint64)friend_id;
	return SteamFriends()->GetFriendCoplayGame(steam_id);
}

// Gets the timestamp of when the user played with someone on their recently-played-with list.  The time is provided in Unix epoch format (seconds since Jan 1st 1970).
int Steam::getFriendCoplayTime(uint64_t friend_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID steam_id = (uint64)friend_id;
	return SteamFriends()->GetFriendCoplayTime(steam_id);
}

// Get number of friends user has.
int Steam::getFriendCount(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendCount(0x04);
}

// Iterators for getting users in a chat room, lobby, game server or clan.
int Steam::getFriendCountFromSource(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	clan_activity = (uint64)clan_id;
	return SteamFriends()->GetFriendCountFromSource(clan_activity);
}

// Gets the Steam ID at the given index from a source (Steam group, chat room, lobby, or game server).
uint64_t Steam::getFriendFromSourceByIndex(uint64_t source_id, int friend_number){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID source = (uint64)source_id;
	CSteamID friend_id = SteamFriends()->GetFriendFromSourceByIndex(source, friend_number);
	return friend_id.ConvertToUint64();
}

// Returns dictionary of friend game played if valid
Dictionary Steam::getFriendGamePlayed(uint64_t steam_id){
	Dictionary friend_game;
	if(SteamFriends() == NULL){
		return friend_game;
	}
	FriendGameInfo_t game_info;
	CSteamID user_id = (uint64)steam_id;
	bool success = SteamFriends()->GetFriendGamePlayed(user_id, &game_info);
	// If successful
	if(success){
		// Is there a valid lobby?
		if(game_info.m_steamIDLobby.IsValid()){
			friend_game["id"] = game_info.m_gameID.AppID();
			// Convert the IP address back to a string
			const int NBYTES = 4;
			uint8 octet[NBYTES];
			char gameIP[16];
			for(int j = 0; j < NBYTES; j++){
				octet[j] = game_info.m_unGameIP >> (j * 8);
			}
			sprintf(gameIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
			friend_game["ip"] = gameIP;
			friend_game["gamePort"] = game_info.m_usGamePort;
			friend_game["query_port"] = (char)game_info.m_usQueryPort;
			friend_game["lobby"] = uint64_t(game_info.m_steamIDLobby.ConvertToUint64());
		}
		else{
			friend_game["id"] = game_info.m_gameID.AppID();
			friend_game["ip"] = "0.0.0.0";
			friend_game["gamePort"] = "0";
			friend_game["query_port"] = "0";
			friend_game["lobby"] = "No valid lobby";
		}
	}
	return friend_game;
}

// Gets the data from a Steam friends message. This should only ever be called in response to a GameConnectedFriendChatMsg_t callback.
Dictionary Steam::getFriendMessage(uint64_t friend_id, int message){
	Dictionary chat;
	if(SteamFriends() == NULL){
		return chat;
	}
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	chat["ret"] = SteamFriends()->GetFriendMessage((uint64)friend_id, message, text, 2048, &type);
	chat["text"] = String(text);
	return chat;
}

// Get given friend's Steam username.
String Steam::getFriendPersonaName(uint64_t steam_id){
	if(SteamFriends() != NULL && steam_id > 0){
		CSteamID user_id = (uint64)steam_id;
		bool is_data_loading = SteamFriends()->RequestUserInformation(user_id, true);
		if(!is_data_loading){
			return String(SteamFriends()->GetFriendPersonaName(user_id));
		}
	}
	return "";
}

// Accesses old friends names; returns an empty string when there are no more items in the history.
String Steam::getFriendPersonaNameHistory(uint64_t steam_id, int name_history){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID user_id = (uint64)steam_id;
	return String(SteamFriends()->GetFriendPersonaNameHistory(user_id, name_history));
}

// Returns the current status of the specified user.
int Steam::getFriendPersonaState(uint64_t steam_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->GetFriendPersonaState(user_id);
}

// Returns a relationship to a user.
int Steam::getFriendRelationship(uint64_t steam_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->GetFriendRelationship(user_id);
}

// Get a Rich Presence value from a specified friend (typically only used for debugging).
String Steam::getFriendRichPresence(uint64_t friend_id, const String& key){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID user = (uint64)friend_id;
	return SteamFriends()->GetFriendRichPresence(user, key.utf8().get_data());
}

// Gets the number of Rich Presence keys that are set on the specified user.
int Steam::getFriendRichPresenceKeyCount(uint64_t friend_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user = (uint64)friend_id;
	return SteamFriends()->GetFriendRichPresenceKeyCount(user);
}

// Returns an empty string ("") if the index is invalid or the specified user has no Rich Presence data available.
String Steam::getFriendRichPresenceKeyByIndex(uint64_t friend_id, int key){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID user = (uint64)friend_id;
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
int Steam::getFriendsGroupIDByIndex(int friend_group){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendsGroupIDByIndex(friend_group);
}

// Gets the number of friends in a given friends group.  This should be called before getting the list of friends with GetFriendsGroupMembersList.
int Steam::getFriendsGroupMembersCount(int friend_group){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetFriendsGroupMembersCount(friend_group);
}

// Gets the number of friends in the given friends group.  If fewer friends exist than requested those positions' Steam IDs will be invalid.  You must call GetFriendsGroupMembersCount before calling this to set up the pOutSteamIDMembers array with an appropriate size!
Array Steam::getFriendsGroupMembersList(int friend_group, int member_count){
	Array member_list;
	if(SteamFriends() == NULL){
		return member_list;
	}
	CSteamID friend_ids;
	SteamFriends()->GetFriendsGroupMembersList((FriendsGroupID_t)friend_group, &friend_ids, member_count);
	uint64_t friends = friend_ids.ConvertToUint64();
	member_list.append(friends);
	return member_list;
}

// Gets the name for the given friends group.
String Steam::getFriendsGroupName(int friend_group){
	if(SteamFriends() == NULL){
		return "";
	}
	return String(SteamFriends()->GetFriendsGroupName(friend_group));
}

// Get friend's steam level, obviously.
int Steam::getFriendSteamLevel(uint64_t steam_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->GetFriendSteamLevel(user_id);
}

// Gets the large (184x184) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getLargeFriendAvatar(uint64_t steam_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->GetLargeFriendAvatar(user_id);
}

// Gets the medium (64x64) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getMediumFriendAvatar(uint64_t steam_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->GetMediumFriendAvatar(user_id);
}

// Get the user's Steam username.
String Steam::getPersonaName(){
	if(SteamFriends() == NULL){
		return "";
	}
	return String(SteamFriends()->GetPersonaName());
}

// Gets the status of the current user.
int Steam::getPersonaState(){
	if(SteamFriends() == NULL){
		return 0;
	}
	return SteamFriends()->GetPersonaState();
}

// Get player's avatar.
void Steam::getPlayerAvatar(int size, uint64_t steam_id){
	// If no Steam ID is given, use the current user's
	if(steam_id == 0){
		steam_id = getSteamID();
	}
	if(SteamFriends() != NULL || size > 0 || size < 4){
		int handle = -2;
		switch(size){
			case 1:{
				handle = getSmallFriendAvatar(steam_id);
				size = 32; break;
			}
			case 2:{
				handle = getMediumFriendAvatar(steam_id);
				size = 64; break;
			}
			case 3:{
				handle = getLargeFriendAvatar(steam_id);
				size = 184; break;
			}
			default:
				return;
		}
		if(handle <= 0){
			printf("[Steam] Error retrieving avatar handle.");
		}
		AvatarImageLoaded_t* avatar_data = new AvatarImageLoaded_t;
		CSteamID avatar_id = (uint64)steam_id;
		avatar_data->m_steamID = avatar_id;
		avatar_data->m_iImage = handle;
		avatar_data->m_iWide = size;
		avatar_data->m_iTall = size;
		_avatar_loaded(avatar_data);
		delete avatar_data;
	}
}

// Returns nickname the current user has set for the specified player. Returns NULL if the no nickname has been set for that player.
String Steam::getPlayerNickname(uint64_t steam_id){
	if(SteamFriends() == NULL){
		return "";
	}
	CSteamID user_id = (uint64)steam_id;
	return String(SteamFriends()->GetPlayerNickname(user_id));
}

// Get list of players user has recently played game with.
Array Steam::getRecentPlayers(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int count = SteamFriends()->GetCoplayFriendCount();
	Array recents;
	for(int i = 0; i < count; i++){
		CSteamID player_id = SteamFriends()->GetCoplayFriend(i);
		if(SteamFriends()->GetFriendCoplayGame(player_id) == SteamUtils()->GetAppID()){
			Dictionary player;
			int time = SteamFriends()->GetFriendCoplayTime(player_id);
			int status = SteamFriends()->GetFriendPersonaState(player_id);
			player["id"] = player_id.GetAccountID();
			player["name"] = String(SteamFriends()->GetFriendPersonaName(player_id));
			player["time"] = time;
			player["status"] = status;
			recents.append(player);
		}
	}
	return recents;
}

// Gets the small (32x32) avatar of the current user, which is a handle to be used in GetImageRGBA(), or 0 if none set.
int Steam::getSmallFriendAvatar(uint64_t steam_id){
	if(SteamFriends() == NULL){
		return 0;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->GetSmallFriendAvatar(user_id);
}

// Get list of friends groups (tags) the user has created. This is not to be confused with Steam groups.
Array Steam::getUserFriendsGroups(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int tag_count = SteamFriends()->GetFriendsGroupCount();
	Array friends_groups;
	for(int i = 0; i < tag_count; i++){
		Dictionary tags;
		int16 friends_group_id = SteamFriends()->GetFriendsGroupIDByIndex(i);
		String group_name = SteamFriends()->GetFriendsGroupName(friends_group_id);
		int group_members = SteamFriends()->GetFriendsGroupMembersCount(friends_group_id);
		tags["id"] = friends_group_id;
		tags["name"] = group_name;
		tags["members"] = group_members;
		friends_groups.append(tags);
	}
	return friends_groups;
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
	Array steam_friends;
	for(int i = 0; i < count; i++){
		Dictionary friends;
		CSteamID friend_id = SteamFriends()->GetFriendByIndex(i, 0x04);
		int status = SteamFriends()->GetFriendPersonaState(friend_id);
		friends["id"] = friend_id.GetAccountID();
		friends["name"] = String(SteamFriends()->GetFriendPersonaName(friend_id));
		friends["status"] = status;
		steam_friends.append(friends);
	}
	return steam_friends;
}

// Get list of user's Steam groups; a mix of different Steamworks API group functions.
Array Steam::getUserSteamGroups(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int group_count = SteamFriends()->GetClanCount();
	Array steam_groups;
	for(int i = 0; i < group_count; i++){
		Dictionary groups;
		CSteamID group_id = SteamFriends()->GetClanByIndex(i);
		String name = SteamFriends()->GetClanName(group_id);
		String tag = SteamFriends()->GetClanTag(group_id);
		groups["id"] = group_id.GetAccountID();
		groups["name"] = name;
		groups["tag"] = tag;
		steam_groups.append(groups);
	}
	return steam_groups;
}

// Returns true if the specified user meets any of the criteria specified in iFriendFlags.
bool Steam::hasFriend(uint64_t steam_id, int friend_flags){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->HasFriend(user_id, friend_flags);
}

// Invite friend to current game/lobby.
bool Steam::inviteUserToGame(uint64_t steam_id, const String& connect_string){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->InviteUserToGame(user_id, connect_string.utf8().get_data());
}

// Checks if a user in the Steam group chat room is an admin.
bool Steam::isClanChatAdmin(uint64_t chat_id, uint64_t steam_id){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chat_id;
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->IsClanChatAdmin(chat, user_id);
}

// Checks if the Steam group is public.
bool Steam::isClanPublic(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return false;
	}
	clan_activity = (uint64)clan_id;
	return SteamFriends()->IsClanPublic(clan_activity);
}

// Checks if the Steam group is an official game group/community hub.
bool Steam::isClanOfficialGameGroup(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return false;
	}
	clan_activity = (uint64)clan_id;
	return SteamFriends()->IsClanOfficialGameGroup(clan_activity);
}

// Checks if the Steam Group chat room is open in the Steam UI.
bool Steam::isClanChatWindowOpenInSteam(uint64_t chat_id){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chat_id;
	return SteamFriends()->IsClanChatWindowOpenInSteam(chat);
}

// Checks if the current user is following the specified user.
void Steam::isFollowing(uint64_t steam_id){
	if(SteamFriends() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamAPICall_t api_call = SteamFriends()->IsFollowing(user_id);
		callResultIsFollowing.Set(api_call, this, &Steam::_is_following);
	}
}

// Returns true if the local user can see that steam_id_user is a member or in steamIDSource.
bool Steam::isUserInSource(uint64_t steam_id, uint64_t source_id){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID user_id = (uint64)steam_id;
	CSteamID source = (uint64)source_id;
	return SteamFriends()->IsUserInSource(user_id, source);
}

// Allows the user to join Steam group (clan) chats right within the game.
void Steam::joinClanChatRoom(uint64_t clan_id){
	if(SteamFriends() != NULL){
		clan_activity = (uint64)clan_id;
		SteamFriends()->JoinClanChatRoom(clan_activity);
	}
}

// Leaves a Steam group chat that the user has previously entered with JoinClanChatRoom.
bool Steam::leaveClanChatRoom(uint64_t clan_id){
	if(SteamFriends() == NULL){
		return false;
	}
	clan_activity = (uint64)clan_id;
	return SteamFriends()->LeaveClanChatRoom(clan_activity);
}

// Opens the specified Steam group chat room in the Steam UI.
bool Steam::openClanChatWindowInSteam(uint64_t chat_id){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chat_id;
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
bool Steam::replyToFriendMessage(uint64_t steam_id, const String& message){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->ReplyToFriendMessage(user_id, message.utf8().get_data());
}

// Requests information about a clan officer list; when complete, data is returned in ClanOfficerListResponse_t call result.
void Steam::requestClanOfficerList(uint64_t clan_id){
	if(SteamFriends() != NULL){
		clan_activity = (uint64)clan_id;
		SteamAPICall_t api_call = SteamFriends()->GetFollowerCount(clan_activity);
		callResultClanOfficerList.Set(api_call, this, &Steam::_request_clan_officer_list);
	}
}

// Requests rich presence for a specific user.
void Steam::requestFriendRichPresence(uint64_t friend_id){
	if(SteamFriends() != NULL){
		CSteamID user = (uint64)friend_id;
		return SteamFriends()->RequestFriendRichPresence(user);
	}
}

// Requests information about a user - persona name & avatar; if bRequireNameOnly is set, then the avatar of a user isn't downloaded.
bool Steam::requestUserInformation(uint64_t steam_id, bool require_name_only){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamFriends()->RequestUserInformation(user_id, require_name_only);
}

// Sends a message to a Steam group chat room.
bool Steam::sendClanChatMessage(uint64_t chat_id, const String& text){
	if(SteamFriends() == NULL){
		return false;
	}
	CSteamID chat = (uint64)chat_id;
	return SteamFriends()->SendClanChatMessage(chat, text.utf8().get_data());
}

// User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI).
void Steam::setInGameVoiceSpeaking(uint64_t steam_id, bool speaking){
	if(SteamFriends() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamFriends()->SetInGameVoiceSpeaking(user_id, speaking);
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
void Steam::setPlayedWith(uint64_t steam_id){
	if(SteamFriends() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamFriends()->SetPlayedWith(user_id);
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
///// GAME SEARCH
/////////////////////////////////////////////////
//
// A keyname and a list of comma separated values: one of which is must be found in order for the match to qualify; fails if a search is currently in progress.
int Steam::addGameSearchParams(const String& key, const String& values){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->AddGameSearchParams(key.utf8().get_data(), values.utf8().get_data());
}

// All players in lobby enter the queue and await a SearchForGameNotificationCallback_t callback. Fails if another search is currently in progress. If not the owner of the lobby or search already in progress this call fails. Periodic callbacks will be sent as queue time estimates change.
int Steam::searchForGameWithLobby(uint64_t lobby_id, int player_min, int player_max){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	CSteamID lobby = (uint64)lobby_id;
	return SteamGameSearch()->SearchForGameWithLobby(lobby, player_min, player_max);
}

// User enter the queue and await a SearchForGameNotificationCallback_t callback. fails if another search is currently in progress. Periodic callbacks will be sent as queue time estimates change.
int Steam::searchForGameSolo(int player_min, int player_max){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->SearchForGameSolo(player_min, player_max);
}

// After receiving SearchForGameResultCallback_t, accept or decline the game. Multiple SearchForGameResultCallback_t will follow as players accept game until the host starts or cancels the game.
int Steam::acceptGame(){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->AcceptGame();
}

// After receiving SearchForGameResultCallback_t, accept or decline the game. Multiple SearchForGameResultCallback_t will follow as players accept game until the host starts or cancels the game.
int Steam::declineGame(){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->DeclineGame();
}

// After receiving GameStartedByHostCallback_t get connection details to server.
String Steam::retrieveConnectionDetails(uint64_t host_id){
	if(SteamGameSearch() == NULL){
		return "";
	}
	CSteamID host = (uint64)host_id;
	char connection_details;
	SteamGameSearch()->RetrieveConnectionDetails(host, &connection_details, 256);
	// Messy conversion from char to String
	String details;
	details += connection_details;
	return details;
}

// Leaves queue if still waiting.
int Steam::endGameSearch(){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->EndGameSearch();
}

// A keyname and a list of comma separated values: all the values you allow.
int Steam::setGameHostParams(const String& key, const String& value){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->SetGameHostParams(key.utf8().get_data(), value.utf8().get_data());
}

// Set connection details for players once game is found so they can connect to this server.
int Steam::setConnectionDetails(const String& details, int connection_details){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->SetConnectionDetails(details.utf8().get_data(), connection_details);
}

// Mark server as available for more players with nPlayerMin,nPlayerMax desired. Accept no lobbies with playercount greater than nMaxTeamSize.
int Steam::requestPlayersForGame(int player_min, int player_max, int max_team_size){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->RequestPlayersForGame(player_min, player_max, max_team_size);
}

// Accept the player list and release connection details to players.
int Steam::hostConfirmGameStart(uint64_t game_id){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->HostConfirmGameStart(game_id);
}

// Cancel request and leave the pool of game hosts looking for players.
int Steam::cancelRequestPlayersForGame(){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->CancelRequestPlayersForGame();
}

// Submit a result for one player. does not end the game. ullUniqueGameID continues to describe this game.
int Steam::submitPlayerResult(uint64_t game_id, uint64_t player_id, int player_result){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	CSteamID player = (uint64)player_id;
	return SteamGameSearch()->SubmitPlayerResult(game_id, player, (EPlayerResult_t)player_result);
}

// Ends the game. no further SubmitPlayerResults for ullUniqueGameID will be accepted.
int Steam::endGame(uint64_t game_id){
	if(SteamGameSearch() == NULL){
		return 9;
	}
	return SteamGameSearch()->EndGame(game_id);
}


/////////////////////////////////////////////////
///// HTML SURFACE
/////////////////////////////////////////////////
//
// Add a header to any HTTP requests from this browser.
void Steam::addHeader(const String& key, const String& value){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->AddHeader(browser_handle, key.utf8().get_data(), value.utf8().get_data());
	}
}

// Sets whether a pending load is allowed or if it should be canceled.  NOTE:You MUST call this in response to a HTML_StartRequest_t callback.
void Steam::allowStartRequest(bool allowed){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->AllowStartRequest(browser_handle, allowed);
	}
}

// Copy the currently selected text from the current page in an HTML surface into the local clipboard.
void Steam::copyToClipboard(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->CopyToClipboard(browser_handle);
	}
}

// Create a browser object for displaying of an HTML page. NOTE: You MUST call RemoveBrowser when you are done using this browser to free up the resources associated with it. Failing to do so will result in a memory leak.
void Steam::createBrowser(const String& user_agent, const String& user_css){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->CreateBrowser(user_agent.utf8().get_data(), user_css.utf8().get_data());
	}
}

// Run a javascript script in the currently loaded page.
void Steam::executeJavascript(const String& script){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->ExecuteJavascript(browser_handle, script.utf8().get_data());
	}
}

// Find a string in the current page of an HTML surface. This is the equivalent of "ctrl+f" in your browser of choice. It will highlight all of the matching strings. You should call StopFind when the input string has changed or you want to stop searching.
void Steam::find(const String& search, bool currently_in_find, bool reverse){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->Find(browser_handle, search.utf8().get_data(), currently_in_find, reverse);
	}
}

// Retrieves details about a link at a specific position on the current page in an HTML surface.
void Steam::getLinkAtPosition(int x, int y){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->GetLinkAtPosition(browser_handle, x, y);
	}
}

// Navigate back in the page history.
void Steam::goBack(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->GoBack(browser_handle);
	}
}

// Navigate forward in the page history
void Steam::goForward(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->GoForward(browser_handle);
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
		SteamHTMLSurface()->JSDialogResponse(browser_handle, result);
	}
}

// cUnicodeChar is the unicode character point for this keypress (and potentially multiple chars per press).
void Steam::keyChar(uint32 unicode_char, int key_modifiers){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->KeyChar(browser_handle, unicode_char, (ISteamHTMLSurface::EHTMLKeyModifiers)key_modifiers);
	}
}

// Keyboard interactions, native keycode is the virtual key code value from your OS.
void Steam::keyDown(uint32 native_key_code, int key_modifiers){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->KeyDown(browser_handle, native_key_code, (ISteamHTMLSurface::EHTMLKeyModifiers)key_modifiers);
	}
}

// Keyboard interactions, native keycode is the virtual key code value from your OS.
void Steam::keyUp(uint32 native_key_code, int key_modifiers){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->KeyUp(browser_handle, native_key_code, (ISteamHTMLSurface::EHTMLKeyModifiers)key_modifiers);
	}
}

// Navigate to a specified URL. If you send POST data with pchPostData then the data should be formatted as: name1=value1&name2=value2. You can load any URI scheme supported by Chromium Embedded Framework including but not limited to: http://, https://, ftp://, and file:///. If no scheme is specified then http:// is used.
void Steam::loadURL(const String& url, const String& post_data){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->LoadURL(browser_handle, url.utf8().get_data(), post_data.utf8().get_data());
	}
}

// Tells an HTML surface that a mouse button has been double clicked. The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
void Steam::mouseDoubleClick(int mouse_button){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseDoubleClick(browser_handle, (ISteamHTMLSurface::EHTMLMouseButton)mouse_button);
	}
}

// Tells an HTML surface that a mouse button has been pressed. The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
void Steam::mouseDown(int mouse_button){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseDown(browser_handle, (ISteamHTMLSurface::EHTMLMouseButton)mouse_button);
	}
}

// Tells an HTML surface where the mouse is.
void Steam::mouseMove(int x, int y){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseMove(browser_handle, x, y);
	}
}

// Tells an HTML surface that a mouse button has been released. The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
void Steam::mouseUp(int mouse_button){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseUp(browser_handle, (ISteamHTMLSurface::EHTMLMouseButton)mouse_button);
	}
}

// Tells an HTML surface that the mouse wheel has moved.
void Steam::mouseWheel(int32 delta){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->MouseWheel(browser_handle, delta);
	}
}

// Paste from the local clipboard to the current page in an HTML surface.
void Steam::pasteFromClipboard(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->PasteFromClipboard(browser_handle);
	}
}

// Refreshes the current page. The reload will most likely hit the local cache instead of going over the network. This is equivalent to F5 or Ctrl+R in your browser of choice.
void Steam::reload(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->Reload(browser_handle);
	}
}

// You MUST call this when you are done with an HTML surface, freeing the resources associated with it. Failing to call this will result in a memory leak!
void Steam::removeBrowser(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->RemoveBrowser(browser_handle);
	}
}

// Enable/disable low-resource background mode, where javascript and repaint timers are throttled, resources are more aggressively purged from memory, and audio/video elements are paused. When background mode is enabled, all HTML5 video and audio objects will execute ".pause()" and gain the property "._steam_background_paused = 1". When background mode is disabled, any video or audio objects with that property will resume with ".play()".
void Steam::setBackgroundMode(bool background_mode){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetBackgroundMode(browser_handle, background_mode);
	}
}

// Set a webcookie for a specific hostname. You can read more about the specifics of setting cookies here on wikipedia.
void Steam::setCookie(const String& hostname, const String& key, const String& value, const String& path, uint32 expires, bool secure, bool http_only){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetCookie(hostname.utf8().get_data(), key.utf8().get_data(), value.utf8().get_data(), path.utf8().get_data(), expires, secure, http_only);
	}
}

// Scroll the current page horizontally.
void Steam::setHorizontalScroll(uint32 absolute_pixel_scroll){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetHorizontalScroll(browser_handle, absolute_pixel_scroll);
	}
}

// Tell a HTML surface if it has key focus currently, controls showing the I-beam cursor in text controls amongst other things.
void Steam::setKeyFocus(bool has_key_focus){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetKeyFocus(browser_handle, has_key_focus);	
	}
}

// Zoom the current page in an HTML surface. The current scale factor is available from HTML_NeedsPaint_t.flPageScale, HTML_HorizontalScroll_t.flPageScale, and HTML_VerticalScroll_t.flPageScale.
void Steam::setPageScaleFactor(float zoom, int point_x, int point_y){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetPageScaleFactor(browser_handle, zoom, point_x, point_y);
	}
}

// Sets the display size of a surface in pixels.
void Steam::setSize(uint32 width, uint32 height){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetSize(browser_handle, width, height);
	}
}

// Scroll the current page vertically.
void Steam::setVerticalScroll(uint32 absolute_pixel_scroll){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->SetVerticalScroll(browser_handle, absolute_pixel_scroll);
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
		SteamHTMLSurface()->StopFind(browser_handle);
	}
}

// Stop the load of the current HTML page.
void Steam::stopLoad(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->StopLoad(browser_handle);
	}
}

// Open the current pages HTML source code in default local text editor, used for debugging.
void Steam::viewSource(){
	if(SteamHTMLSurface() != NULL){
		SteamHTMLSurface()->ViewSource(browser_handle);
	}
}


/////////////////////////////////////////////////
///// HTTP
/////////////////////////////////////////////////
//
//Creates a cookie container to store cookies during the lifetime of the process. This API is just for during process lifetime, after steam restarts no cookies are persisted and you have no way to access the cookie container across repeat executions of your process.
void Steam::createCookieContainer(bool allow_responses_to_modify){
	if(SteamHTTP() != NULL){
		SteamHTTP()->CreateCookieContainer(allow_responses_to_modify);
	}
}

// Initializes a new HTTP request.
uint32_t Steam::createHTTPRequest(int request_method, const String& absolute_url){
	if(SteamHTTP() != NULL){
		return SteamHTTP()->CreateHTTPRequest((EHTTPMethod)request_method, absolute_url.utf8().get_data());
	}
	return HTTPREQUEST_INVALID_HANDLE;
}

// Defers a request which has already been sent by moving it at the back of the queue.
bool Steam::deferHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->DeferHTTPRequest(request_handle);
}

// Gets progress on downloading the body for the request.
float Steam::getHTTPDownloadProgressPct(uint32 request_handle){
	float percent_one = 0.0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPDownloadProgressPct(request_handle, &percent_one);
	}
	return percent_one;
}

// Check if the reason the request failed was because we timed it out (rather than some harder failure).
bool Steam::getHTTPRequestWasTimedOut(uint32 request_handle){
	bool was_timed_out = false;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPRequestWasTimedOut(request_handle, &was_timed_out);
	}
	return was_timed_out;
}

// Gets the body data from an HTTP response.
PoolByteArray Steam::getHTTPResponseBodyData(uint32 request_handle, uint32 buffer_size){
	PoolByteArray body_data;
	body_data.resize(buffer_size);
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodyData(request_handle, body_data.write().ptr(), buffer_size);
	}
	return body_data;
}

// Gets the size of the body data from an HTTP response.
uint32 Steam::getHTTPResponseBodySize(uint32 request_handle){
	uint32 body_size = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseBodySize(request_handle, &body_size);
	}
	return body_size;
}

// Checks if a header is present in an HTTP response and returns its size.
uint32 Steam::getHTTPResponseHeaderSize(uint32 request_handle, const String& header_name){
	uint32 response_header_size = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderSize(request_handle, header_name.utf8().get_data(), &response_header_size);
	}
	return response_header_size;
}

// Gets a header value from an HTTP response.
uint8 Steam::getHTTPResponseHeaderValue(uint32 request_handle, const String& header_name, uint32 buffer_size){
	uint8 value_buffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPResponseHeaderValue(request_handle, header_name.utf8().get_data(), &value_buffer, buffer_size);
	}
	return value_buffer;
}

// Gets the body data from a streaming HTTP response.
uint8 Steam::getHTTPStreamingResponseBodyData(uint32 request_handle, uint32 offset, uint32 buffer_size){
	uint8 body_data_buffer = 0;
	if(SteamHTTP() != NULL){
		SteamHTTP()->GetHTTPStreamingResponseBodyData(request_handle, offset, &body_data_buffer, buffer_size);
	}
	return body_data_buffer;
}

// Prioritizes a request which has already been sent by moving it at the front of the queue.
bool Steam::prioritizeHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->PrioritizeHTTPRequest(request_handle);
}

// Releases a cookie container, freeing the memory allocated within Steam.
bool Steam::releaseCookieContainer(uint32 cookie_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseCookieContainer(cookie_handle);
}

// Releases an HTTP request handle, freeing the memory allocated within Steam.
bool Steam::releaseHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->ReleaseHTTPRequest(request_handle);
}

// Sends an HTTP request.
bool Steam::sendHTTPRequest(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t call_handle;
	return SteamHTTP()->SendHTTPRequest(request_handle, &call_handle);
}

// Sends an HTTP request and streams the response back in chunks.
bool Steam::sendHTTPRequestAndStreamResponse(uint32 request_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	SteamAPICall_t call_handle;
	return SteamHTTP()->SendHTTPRequestAndStreamResponse(request_handle, &call_handle);
}

// Adds a cookie to the specified cookie container that will be used with future requests.
bool Steam::setHTTPCookie(uint32 cookie_handle, const String& host, const String& url, const String& cookie){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetCookie(cookie_handle, host.utf8().get_data(), url.utf8().get_data(), cookie.utf8().get_data());
}

// Set an absolute timeout in milliseconds for the HTTP request. This is the total time timeout which is different than the network activity timeout which is set with SetHTTPRequestNetworkActivityTimeout which can bump everytime we get more data.
bool Steam::setHTTPRequestAbsoluteTimeoutMS(uint32 request_handle, uint32 milliseconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestAbsoluteTimeoutMS(request_handle, milliseconds);
}

// Set a context value for the request, which will be returned in the HTTPRequestCompleted_t callback after sending the request. This is just so the caller can easily keep track of which callbacks go with which request data. Must be called before sending the request.
bool Steam::setHTTPRequestContextValue(uint32 request_handle, uint64_t context_value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestContextValue(request_handle, context_value);
}

// Associates a cookie container to use for an HTTP request.
bool Steam::setHTTPRequestCookieContainer(uint32 request_handle, uint32 cookie_handle){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestCookieContainer(request_handle, cookie_handle);
}

// Set a GET or POST parameter value on the HTTP request. Must be called prior to sending the request.
bool Steam::setHTTPRequestGetOrPostParameter(uint32 request_handle, const String& name, const String& value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestGetOrPostParameter(request_handle, name.utf8().get_data(), value.utf8().get_data());
}

// Set a request header value for the HTTP request. Must be called before sending the request.
bool Steam::setHTTPRequestHeaderValue(uint32 request_handle, const String& header_name, const String& header_value){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestHeaderValue(request_handle, header_name.utf8().get_data(), header_value.utf8().get_data());
}

// Set the timeout in seconds for the HTTP request.
bool Steam::setHTTPRequestNetworkActivityTimeout(uint32 request_handle, uint32 timeout_seconds){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestNetworkActivityTimeout(request_handle, timeout_seconds);
}

// Sets the body for an HTTP Post request.
uint8 Steam::setHTTPRequestRawPostBody(uint32 request_handle, const String& content_type, uint32 body_length){
	uint8 body = 0;
	if(SteamHTTP()){
		SteamHTTP()->SetHTTPRequestRawPostBody(request_handle, content_type.utf8().get_data(), &body, body_length);
	}
	return body;
}

// Sets that the HTTPS request should require verified SSL certificate via machines certificate trust store. This currently only works Windows and macOS.
bool Steam::setHTTPRequestRequiresVerifiedCertificate(uint32 request_handle, bool require_verified_certificate){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestRequiresVerifiedCertificate(request_handle, require_verified_certificate);
}

// Set additional user agent info for a request.
bool Steam::setHTTPRequestUserAgentInfo(uint32 request_handle, const String& user_agent_info){
	if(SteamHTTP() == NULL){
		return false;
	}
	return SteamHTTP()->SetHTTPRequestUserAgentInfo(request_handle, user_agent_info.utf8().get_data());
}


/////////////////////////////////////////////////
///// INPUT
/////////////////////////////////////////////////
//
// Reconfigure the controller to use the specified action set.
void Steam::activateActionSet(uint64_t input_handle, uint64_t action_set_handle){
	if(SteamInput() != NULL){
		SteamInput()->ActivateActionSet((InputHandle_t)input_handle, (ControllerActionSetHandle_t)action_set_handle);
	}
}

// Reconfigure the controller to use the specified action set layer.
void Steam::activateActionSetLayer(uint64_t input_handle, uint64_t action_set_layer_handle){
	if(SteamInput() != NULL){
		SteamInput()->ActivateActionSetLayer((InputHandle_t)input_handle, (ControllerActionSetHandle_t)action_set_layer_handle);
	}
}

// Reconfigure the controller to stop using the specified action set.
void Steam::deactivateActionSetLayer(uint64_t input_handle, uint64_t action_set_handle){
	if(SteamInput() != NULL){
		SteamInput()->DeactivateActionSetLayer((InputHandle_t)input_handle, (ControllerActionSetHandle_t)action_set_handle);
	}
}

// Reconfigure the controller to stop using all action set layers.
void Steam::deactivateAllActionSetLayers(uint64_t input_handle){
	if(SteamInput() != NULL){
		SteamInput()->DeactivateAllActionSetLayers((InputHandle_t)input_handle);
	}
}

// Lookup the handle for an Action Set. Best to do this once on startup, and store the handles for all future API calls.
uint64_t Steam::getActionSetHandle(const String& action_set_name){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetActionSetHandle(action_set_name.utf8().get_data());
	}
	return 0;
}

// Get an action origin that you can use in your glyph look up table or passed into GetGlyphForActionOrigin or GetStringForActionOrigin.
int Steam::getActionOriginFromXboxOrigin(uint64_t input_handle, int origin){
	if(SteamInput() == NULL){
		return 0;
	}
	return SteamInput()->GetActionOriginFromXboxOrigin((InputHandle_t)input_handle, (EXboxOrigin)origin);
}

// Fill an array with all of the currently active action set layers for a specified controller handle.
Array Steam::getActiveActionSetLayers(uint64_t input_handle){
	Array handles;
	if(SteamInput() != NULL){
		InputActionSetHandle_t *out = new InputActionSetHandle_t[INPUT_MAX_COUNT];
		int ret = SteamInput()->GetActiveActionSetLayers(input_handle, out);
		for(int i = 0; i < ret; i++){
			handles.push_back((int)out[i]);
		}
		delete[] out;
	}
	return handles;
}

// Returns the current state of the supplied analog game action.
Dictionary Steam::getAnalogActionData(uint64_t input_handle, uint64_t analog_action_handle){
	ControllerAnalogActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamInput() != NULL){
		data = SteamInput()->GetAnalogActionData((InputHandle_t)input_handle, (ControllerAnalogActionHandle_t)analog_action_handle);
	}
	d["eMode"] = data.eMode;
	d["x"] = data.x;
	d["y"] = data.y;
	d["bActive"] = data.bActive;
	return d;
}

// Get the handle of the specified Analog action.
uint64_t Steam::getAnalogActionHandle(const String& action_name){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetAnalogActionHandle(action_name.utf8().get_data());
	}
	return 0;
}

// Get the origin(s) for an analog action within an action.
Array Steam::getAnalogActionOrigins(uint64_t input_handle, uint64_t action_set_handle, uint64_t analog_action_handle){
	Array list;
	if(SteamInput() != NULL){
		EInputActionOrigin *out = new EInputActionOrigin[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamInput()->GetAnalogActionOrigins((InputHandle_t)input_handle, (ControllerActionSetHandle_t)action_set_handle, (ControllerAnalogActionHandle_t)analog_action_handle, out);
		for (int i = 0; i < ret; i++){
			list.push_back((int)out[i]);
		}
		delete[] out;
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
		delete[] handles;
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
uint64_t Steam::getCurrentActionSet(uint64_t input_handle){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetCurrentActionSet((InputHandle_t)input_handle);
	}
	return 0;
}

// Get's the major and minor device binding revisions for Steam Input API configurations. Minor revisions are for small changes such as adding a new option action or updating localization in the configuration. When updating a Minor revision only one new configuration needs to be update with the "Use Action Block" flag set. Major revisions are to be used when changing the number of action sets or otherwise reworking configurations to the degree that older configurations are no longer usable. When a user's binding disagree's with the major revision of the current official configuration Steam will forcibly update the user to the new configuration. New configurations will need to be made for every controller when updating the Major revision.
Array Steam::getDeviceBindingRevision(uint64_t input_handle){
	Array revision;
	if(SteamInput() != NULL){
		int major = 0;
		int minor = 0;
		bool success = SteamInput()->GetDeviceBindingRevision((InputHandle_t)input_handle, &major, &minor);
		if(success){
			revision.append(major);
			revision.append(minor);
		}
	}
	return revision;
}

// Returns the current state of the supplied digital game action.
Dictionary Steam::getDigitalActionData(uint64_t input_handle, uint64_t digital_action_handle){
	ControllerDigitalActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamInput() != NULL){
		data = SteamInput()->GetDigitalActionData((InputHandle_t)input_handle, (ControllerDigitalActionHandle_t)digital_action_handle);
	}
	d["bState"] = data.bState;
	d["bActive"] = data.bActive;
	return d;
}

// Get the handle of the specified digital action.
uint64_t Steam::getDigitalActionHandle(const String& action_name){
	if(SteamInput() != NULL){
		return (uint64_t)SteamInput()->GetDigitalActionHandle(action_name.utf8().get_data());
	}
	return 0;
}

// Get the origin(s) for an analog action within an action.
Array Steam::getDigitalActionOrigins(uint64_t input_handle, uint64_t action_set_handle, uint64_t digital_action_handle){
	Array list;
	if(SteamInput() != NULL){
		EInputActionOrigin *out = new EInputActionOrigin[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamInput()->GetDigitalActionOrigins((InputHandle_t)input_handle, (ControllerActionSetHandle_t)action_set_handle, (ControllerDigitalActionHandle_t)digital_action_handle, out);
		for (int i=0; i<ret; i++){
			list.push_back((int)out[i]);
		}
		delete[] out;
	}
	return list;
}

// Returns the associated gamepad index for the specified controller.
int Steam::getGamepadIndexForController(uint64_t input_handle){
	if(SteamInput() != NULL){
		return SteamInput()->GetGamepadIndexForController((InputHandle_t)input_handle);
	}
	return -1;
}

// Get a local path to art for on-screen glyph for a particular origin.
String Steam::getGlyphForActionOrigin(int origin){
	if(SteamInput() != NULL){
		return "";
	}
	return SteamInput()->GetGlyphForActionOrigin_Legacy((EInputActionOrigin)origin);
}

// Get the input type (device model) for the specified controller. 
String Steam::getInputTypeForHandle(uint64_t input_handle){
	if(SteamInput() == NULL){
		return "";
	}
	ESteamInputType inputType = SteamInput()->GetInputTypeForHandle((InputHandle_t)input_handle);
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
	else if(inputType == k_ESteamInputType_AppleMFiController){
		return "Apple iController";
	}
	else if(inputType == k_ESteamInputType_AndroidController){
		return "Android Controller";
	}
	else if(inputType == k_ESteamInputType_SwitchJoyConPair){
		return "Switch Jon Cons (Pair)";
	}
	else if(inputType == k_ESteamInputType_SwitchJoyConSingle){
		return "Switch Jon Con (Single)";
	}
	else if(inputType == k_ESteamInputType_SwitchProController){
		return "Switch Pro Controller";
	}
	else if(inputType == k_ESteamInputType_MobileTouch){
		return "Mobile Touch";
	}
	else if(inputType == k_ESteamInputType_PS3Controller){
		return "PS3 Controller";
	}
	else if(inputType == k_ESteamInputType_PS5Controller){
		return "PS5 Controller";
	}
	else{
		return "Unknown";
	}
}

// Returns raw motion data for the specified controller.
Dictionary Steam::getMotionData(uint64_t input_handle){
	ControllerMotionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if(SteamInput() != NULL){
		data = SteamInput()->GetMotionData((InputHandle_t)input_handle);
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
int Steam::getRemotePlaySessionID(uint64_t input_handle){
	if(SteamInput() == NULL){
		return 0;
	}
	return SteamInput()->GetRemotePlaySessionID((InputHandle_t)input_handle);
}

// Returns a localized string (from Steam's language setting) for the specified origin.
String Steam::getStringForActionOrigin(int origin){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetStringForActionOrigin((EInputActionOrigin)origin);
}

// Start SteamInputs interface.
bool Steam::inputInit(bool explicitly_call_run_frame){
	if(SteamInput() != NULL){
		return SteamInput()->Init(false);
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
void Steam::setLEDColor(uint64_t input_handle, int color_r, int color_g, int color_b, int flags){
	if(SteamInput() != NULL){
		SteamInput()->SetLEDColor((InputHandle_t)input_handle, color_r, color_g, color_b, flags);
	}
}

// Syncronize controllers.
void Steam::runFrame(bool reserved_value){
	if(SteamInput() != NULL){
		SteamInput()->RunFrame(reserved_value);
	}
}

// Invokes the Steam overlay and brings up the binding screen.
bool Steam::showBindingPanel(uint64_t input_handle){
	if(SteamInput() != NULL){
		return SteamInput()->ShowBindingPanel((InputHandle_t)input_handle);
	}
	return false;
}

// Stops the momentum of an analog action (where applicable, ie a touchpad w/ virtual trackball settings).
void Steam::stopAnalogActionMomentum(uint64_t input_handle, uint64_t action){
	if(SteamInput() != NULL){
		SteamInput()->StopAnalogActionMomentum((InputHandle_t)input_handle, (InputAnalogActionHandle_t)action);
	}
}

// Get the equivalent origin for a given controller type or the closest controller type that existed in the SDK you built into your game if eDestinationInputType is k_ESteamInputType_Unknown. This action origin can be used in your glyph look up table or passed into GetGlyphForActionOrigin or GetStringForActionOrigin.
int Steam::translateActionOrigin(int destination_input, int source_origin){
	if(SteamInput() == NULL){
		return 0;
	}
	return SteamInput()->TranslateActionOrigin((ESteamInputType)destination_input, (EInputActionOrigin)source_origin);
}

// Triggers a (low-level) haptic pulse on supported controllers.
void Steam::triggerHapticPulse(uint64_t input_handle, int target_pad, int duration){
	if(SteamInput() != NULL){
		SteamInput()->Legacy_TriggerHapticPulse((InputHandle_t)input_handle, (ESteamControllerPad)target_pad, duration);
	}
}

// Triggers a repeated haptic pulse on supported controllers.
void Steam::triggerRepeatedHapticPulse(uint64_t input_handle, int target_pad, int duration, int offset, int repeat, int flags){
	if(SteamInput() != NULL){
		SteamInput()->Legacy_TriggerRepeatedHapticPulse((InputHandle_t)input_handle, (ESteamControllerPad)target_pad, duration, offset, repeat, flags);
	}
}

// Trigger a vibration event on supported controllers.
void Steam::triggerVibration(uint64_t input_handle, uint16_t left_speed, uint16_t right_speed){
	if(SteamInput() != NULL){
		SteamInput()->TriggerVibration((InputHandle_t)input_handle, (unsigned short)left_speed, (unsigned short)right_speed);
	}
}

// Set the absolute path to the Input Action Manifest file containing the in-game actions and file paths to the official configurations. Used in games that bundle Steam Input configurations inside of the game depot instead of using the Steam Workshop.
bool Steam::setInputActionManifestFilePath(const String& manifest_path){
	if(SteamInput() == NULL){
		return false;
	}
	return SteamInput()->SetInputActionManifestFilePath(manifest_path.utf8().get_data());
}

// Waits on an IPC event from Steam sent when there is new data to be fetched from the data drop. Returns true when data was recievied before the timeout expires. Useful for games with a dedicated input thread.
bool Steam::waitForData(bool wait_forever, uint32 timeout){
	if(SteamInput() == NULL){
		return false;
	}
	return SteamInput()->BWaitForData(wait_forever, timeout);
}

// Returns true if new data has been received since the last time action data was accessed via GetDigitalActionData or GetAnalogActionData. The game will still need to call SteamInput()->RunFrame() or SteamAPI_RunCallbacks() before this to update the data stream.
bool Steam::newDataAvailable(){
	if(SteamInput() == NULL){
		return false;
	}
	return SteamInput()->BNewDataAvailable();
}

// Enable SteamInputDeviceConnected_t and SteamInputDeviceDisconnected_t callbacks. Each controller that is already connected will generate a device connected callback when you enable them.
void Steam::enableDeviceCallbacks(){
	if(SteamInput() != NULL){
		SteamInput()->EnableDeviceCallbacks();
	}
}

// Enable SteamInputActionEvent_t callbacks. Directly calls your callback function for lower latency than standard Steam callbacks. Supports one callback at a time. Note: this is called within either SteamInput()->RunFrame or by SteamAPI_RunCallbacks
//void Steam::enableActionEventCallbacks(SteamInputActionEventCallbackPointer call_back){
//	if(SteamInput() != NULL){
//		SteamInput()->EnableActionEventCallbacks(call_back);
//	}
//}

// Get a local path to a PNG file for the provided origin's glyph. 
String Steam::getGlyphPNGForActionOrigin(int origin, int size, uint32 flags){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetGlyphPNGForActionOrigin((EInputActionOrigin)origin, (ESteamInputGlyphSize)size, flags);
}

// Get a local path to a SVG file for the provided origin's glyph. 
String Steam::getGlyphSVGForActionOrigin(int origin, uint32 flags){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetGlyphSVGForActionOrigin((EInputActionOrigin)origin, flags);
}

// Trigger a vibration event on supported controllers including Xbox trigger impulse rumble - Steam will translate these commands into haptic pulses for Steam Controllers.
void Steam::triggerVibrationExtended(uint64_t input_handle, uint16_t left_speed, uint16_t right_speed, uint16_t left_trigger_speed, uint16_t right_trigger_speed){
	if(SteamInput() != NULL){
		SteamInput()->TriggerVibrationExtended((InputHandle_t)input_handle, (unsigned short)left_speed, (unsigned short)right_speed, (unsigned short)left_trigger_speed, (unsigned short)right_trigger_speed);
	}
}

// Send a haptic pulse, works on Steam Deck and Steam Controller devices.
void Steam::triggerSimpleHapticEvent(uint64_t input_handle, int haptic_location, uint8 intensity, const String& gain_db, uint8 other_intensity, const String& other_gain_db){
	if(SteamInput() != NULL){
		// Convert the strings over to char
		char *gain = new char[gain_db.length()];
		strcpy(gain, gain_db.utf8().get_data());
		char *other_gain = new char[other_gain_db.length()];
		strcpy(gain, gain_db.utf8().get_data());
		SteamInput()->TriggerSimpleHapticEvent((InputHandle_t)input_handle, (EControllerHapticLocation)haptic_location, intensity, *gain, other_intensity, *other_gain);
		delete[] gain;
		delete[] other_gain;
	}
}

// Returns a localized string (from Steam's language setting) for the specified Xbox controller origin.
String Steam::getStringForXboxOrigin(int origin){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetStringForXboxOrigin((EXboxOrigin)origin);
}

// Get a local path to art for on-screen glyph for a particular Xbox controller origin
String Steam::getGlyphForXboxOrigin(int origin){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetGlyphForXboxOrigin((EXboxOrigin)origin);
}

// Get a bitmask of the Steam Input Configuration types opted in for the current session. Returns ESteamInputConfigurationEnableType values.	
// Note: user can override the settings from the Steamworks Partner site so the returned values may not exactly match your default configuration.
uint16 Steam::getSessionInputConfigurationSettings(){
	if(SteamInput() == NULL){
		return 0;
	}
	return SteamInput()->GetSessionInputConfigurationSettings();
}

// Returns a localized string (from Steam's language setting) for the user-facing action name corresponding to the specified handle.
String Steam::getStringForDigitalActionName(uint64_t action_handle){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetStringForDigitalActionName((InputDigitalActionHandle_t)action_handle);
}

// Returns a localized string (from Steam's language setting) for the user-facing action name corresponding to the specified handle.
String Steam::getStringForAnalogActionName(uint64_t action_handle){
	if(SteamInput() == NULL){
		return "";
	}
	return SteamInput()->GetStringForAnalogActionName((InputAnalogActionHandle_t)action_handle);
}


/////////////////////////////////////////////////
///// INVENTORY
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
	return SteamInventory()->AddPromoItem(&inventory_handle, item);
}

// Grant a specific one-time promotional items to the current user.
//bool Steam::addPromoItems(const PoolIntArray items){
//	bool promo_items_added = false;
//	if(SteamInventory() != NULL){
//		int count = items.size();
//		SteamItemDef_t *new_items = new SteamItemDef_t[items.size()];
//		for(int i = 0; i < count; i++){
//			new_items[i] = items[i];
//		}
//		promo_items_added = SteamInventory()->AddPromoItems(&inventory_handle, new_items, count);
//		delete[] new_items;
//	}
//	return promo_items_added;
//}

// Checks whether an inventory result handle belongs to the specified Steam ID.
bool Steam::checkResultSteamID(uint64_t steamIDExpected){
	if(SteamInventory() == NULL){
		return false;
	}
	CSteamID steam_id = (uint64)steamIDExpected;
	return SteamInventory()->CheckResultSteamID((SteamInventoryResult_t)inventory_handle, steam_id);
}

// Consumes items from a user's inventory. If the quantity of the given item goes to zero, it is permanently removed.
bool Steam::consumeItem(uint64_t item_consume, uint32 quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->ConsumeItem(&inventory_handle, (SteamItemInstanceID_t)item_consume, quantity);
}

// Deserializes a result set and verifies the signature bytes.
bool Steam::deserializeResult(){
	if(SteamInventory() == NULL){
		return false;
	}
	const int buffer = 1024;
	return SteamInventory()->DeserializeResult(&inventory_handle, &buffer, 1024, false);
}

// Destroys a result handle and frees all associated memory.
void Steam::destroyResult(){
	if(SteamInventory() != NULL){
		SteamInventory()->DestroyResult(inventory_handle);
	}
}

// Grant one item in exchange for a set of other items.
bool Steam::exchangeItems(const PoolIntArray output_items, const uint32 output_quantity, const uint64_t input_items, const uint32 input_quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->ExchangeItems(&inventory_handle, output_items.read().ptr(), &output_quantity, 1, (const uint64 *)input_items, &input_quantity, 1);
}

// Grants specific items to the current user, for developers only.
bool Steam::generateItems(const PoolIntArray items, const uint32 quantity){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GenerateItems(&inventory_handle, items.read().ptr(), &quantity, items.size());
}

// Start retrieving all items in the current users inventory.
bool Steam::getAllItems(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GetAllItems(&inventory_handle);
}

// Gets a string property from the specified item definition.  Gets a property value for a specific item definition.
String Steam::getItemDefinitionProperty(uint32 definition, const String& name){
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
bool Steam::getItemsByID(const uint64_t id_array, uint32 count){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GetItemsByID(&inventory_handle, (const uint64 *)id_array, count);
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
Array Steam::getResultItems(){
	if(SteamInventory() == NULL){
		return Array();
	}
	// Set up return array
	Array items;
	uint32 size = 0;
	if(SteamInventory()->GetResultItems(inventory_handle, NULL, &size)){
		items.resize(size);
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
String Steam::getResultStatus(){
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
uint32 Steam::getResultTimestamp(){
	if(SteamInventory() == NULL){
		return 0;
	}
	return SteamInventory()->GetResultTimestamp(inventory_handle);
}

// Grant all potential one-time promotional items to the current user.
bool Steam::grantPromoItems(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->GrantPromoItems(&inventory_handle);
}

// Triggers an asynchronous load and refresh of item definitions.
bool Steam::loadItemDefinitions(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->LoadItemDefinitions();
}

// Request the list of "eligible" promo items that can be manually granted to the given user.
void Steam::requestEligiblePromoItemDefinitionsIDs(uint64_t steam_id){
	if(SteamInventory() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamAPICall_t api_call = SteamInventory()->RequestEligiblePromoItemDefinitionsIDs(user_id);
		callResultEligiblePromoItemDefIDs.Set(api_call, this, &Steam::_inventory_eligible_promo_item);
	}
}

// Request prices for all item definitions that can be purchased in the user's local currency. A SteamInventoryRequestPricesResult_t call result will be returned with the user's local currency code. After that, you can call GetNumItemsWithPrices and GetItemsWithPrices to get prices for all the known item definitions, or GetItemPrice for a specific item definition.
void Steam::requestPrices(){
	if(SteamInventory() != NULL){
		SteamAPICall_t api_call = SteamInventory()->RequestPrices();
		callResultRequestPrices.Set(api_call, this, &Steam::_inventory_request_prices_result);
	}
}

// Serialized result sets contain a short signature which can't be forged or replayed across different game sessions.
bool Steam::serializeResult(){
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
void Steam::startPurchase(const PoolIntArray items, const uint32 quantity){
	if(SteamInventory() != NULL){
		SteamAPICall_t api_call = SteamInventory()->StartPurchase(items.read().ptr(), &quantity, items.size());
		callResultStartPurchase.Set(api_call, this, &Steam::_inventory_start_purchase_result);
	}
}

// Transfer items between stacks within a user's inventory.
bool Steam::transferItemQuantity(uint64_t item_id, uint32 quantity, uint64_t item_destination, bool split){
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
bool Steam::triggerItemDrop(uint32 definition){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->TriggerItemDrop(&inventory_handle, (SteamItemDef_t)definition);
}

// Starts a transaction request to update dynamic properties on items for the current user. This call is rate-limited by user, so property modifications should be batched as much as possible (e.g. at the end of a map or game session). After calling SetProperty or RemoveProperty for all the items that you want to modify, you will need to call SubmitUpdateProperties to send the request to the Steam servers. A SteamInventoryResultReady_t callback will be fired with the results of the operation.
void Steam::startUpdateProperties(){
	if(SteamInventory() != NULL){
		inventory_update_handle = SteamInventory()->StartUpdateProperties();
	}
}

//Submits the transaction request to modify dynamic properties on items for the current user. See StartUpdateProperties.
bool Steam::submitUpdateProperties(){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SubmitUpdateProperties(inventory_update_handle, &inventory_handle);
}

// Removes a dynamic property for the given item.
bool Steam::removeProperty(uint64_t item_id, const String& name){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->RemoveProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data());
}

// Sets a dynamic property for the given item. Supported value types are strings.
bool Steam::setPropertyString(uint64_t item_id, const String& name, const String& value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value.utf8().get_data());
}

// Sets a dynamic property for the given item. Supported value types are boolean.
bool Steam::setPropertyBool(uint64_t item_id, const String& name, bool value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value);
}

// Sets a dynamic property for the given item. Supported value types are 64 bit integers.
bool Steam::setPropertyInt(uint64_t item_id, const String& name, uint64_t value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), (int64)value);
}

// Sets a dynamic property for the given item. Supported value types are 32 bit floats.
bool Steam::setPropertyFloat(uint64_t item_id, const String& name, float value){
	if(SteamInventory() == NULL){
		return false;
	}
	return SteamInventory()->SetProperty(inventory_update_handle, (SteamItemInstanceID_t)item_id, name.utf8().get_data(), value);
}


/////////////////////////////////////////////////
///// MATCHMAKING
/////////////////////////////////////////////////
//
// Gets an array of the details of the favorite game servers.
Array Steam::getFavoriteGames(){
	if(SteamMatchmaking() == NULL){
		return Array();
	}
	int count = SteamMatchmaking()->GetFavoriteGameCount();
	Array favorites;
	for(int i = 0; i < count; i++){
		Dictionary favorite;
		AppId_t app_id = 0;
		uint32 ip = 0;
		uint16 port = 0;
		uint16 query_port = 0;
		uint32 flags = 0;
		uint32 last_played = 0;
		favorite["ret"] = SteamMatchmaking()->GetFavoriteGame(i, &app_id, &ip, &port, &query_port, &flags, &last_played);
		if(favorite["ret"]){
			favorite["app"] = app_id;
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
			favorite["query"] = query_port;
			favorite["flags"] = flags;
			favorite["played"] = last_played;
			favorites.append(favorite);
		}
	}
	return favorites;
}

// Adds the game server to the local list; updates the time played of the server if it already exists in the list.
int Steam::addFavoriteGame(uint32 ip, uint16 port, uint16 query_port, uint32 flags, uint32 last_played){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	return SteamMatchmaking()->AddFavoriteGame((AppId_t)current_app_id, ip, port, query_port, flags, last_played);
}

// Removes the game server from the local storage; returns true if one was removed.
bool Steam::removeFavoriteGame(AppId_t app_id, uint32 ip, uint16 port, uint16 query_port, uint32 flags){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	return SteamMatchmaking()->RemoveFavoriteGame(app_id, ip, port, query_port, flags);
}

// Get a list of relevant lobbies.
void Steam::requestLobbyList(){
	if(SteamMatchmaking() != NULL){
		SteamAPICall_t api_call = SteamMatchmaking()->RequestLobbyList();
		callResultLobbyList.Set(api_call, this, &Steam::_lobby_match_list);
	}
}

// Adds a string comparison filter to the next RequestLobbyList call.
void Steam::addRequestLobbyListStringFilter(const String& key_to_match, const String& value_to_match, int comparison_type){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListStringFilter(key_to_match.utf8().get_data(), value_to_match.utf8().get_data(), (ELobbyComparison)comparison_type);
	}
}

// Adds a numerical comparison filter to the next RequestLobbyList call.
void Steam::addRequestLobbyListNumericalFilter(const String& key_to_match, int value_to_match, int comparison_type){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListNumericalFilter(key_to_match.utf8().get_data(), value_to_match, (ELobbyComparison)comparison_type);
	}
}

// Returns results closest to the specified value. Multiple near filters can be added, with early filters taking precedence.
void Steam::addRequestLobbyListNearValueFilter(const String& key_to_match, int value_to_be_close_to){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListNearValueFilter(key_to_match.utf8().get_data(), value_to_be_close_to);
	}
}

// Returns only lobbies with the specified number of slots available.
void Steam::addRequestLobbyListFilterSlotsAvailable(int slots_available){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(slots_available);
	}
}

// Sets the distance for which we should search for lobbies (based on users IP address to location map on the Steam backed).
void Steam::addRequestLobbyListDistanceFilter(int distance_filter){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListDistanceFilter((ELobbyDistanceFilter)distance_filter);
	}
}

// Sets how many results to return, the lower the count the faster it is to download the lobby results & details to the client.
void Steam::addRequestLobbyListResultCountFilter(int max_results){
	if(SteamMatchmaking() != NULL){
		SteamMatchmaking()->AddRequestLobbyListResultCountFilter(max_results);
	}
}

// Create a lobby on the Steam servers, if private the lobby will not be returned by any RequestLobbyList() call.
void Steam::createLobby(int lobby_type, int max_members){
	if(SteamMatchmaking() != NULL){
		SteamAPICall_t api_call = SteamMatchmaking()->CreateLobby((ELobbyType)lobby_type, max_members);
		callResultCreateLobby.Set(api_call, this, &Steam::_lobby_created);
	}
}

// Join an existing lobby.
void Steam::joinLobby(uint64_t steam_lobby_id){
	if(SteamMatchmaking() != NULL){
		CSteamID lobby_id = (uint64)steam_lobby_id;
		SteamMatchmaking()->JoinLobby(lobby_id);
	}
}

// Leave a lobby, this will take effect immediately on the client side, other users will be notified by LobbyChatUpdate_t callback.
void Steam::leaveLobby(uint64_t steam_lobby_id){
	if(SteamMatchmaking() != NULL){
		CSteamID lobby_id = (uint64)steam_lobby_id;
		SteamMatchmaking()->LeaveLobby(lobby_id);
	}
}

// Invite another user to the lobby, the target user will receive a LobbyInvite_t callback, will return true if the invite is successfully sent, whether or not the target responds.
bool Steam::inviteUserToLobby(uint64_t steam_lobby_id, uint64_t steam_id_invitee){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	CSteamID invitee_id = (uint64)steam_id_invitee;
	return SteamMatchmaking()->InviteUserToLobby(lobby_id, invitee_id);
}

// Lobby iteration, for viewing details of users in a lobby.
int Steam::getNumLobbyMembers(uint64_t steam_lobby_id){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->GetNumLobbyMembers(lobby_id);
}

// Returns the CSteamID of a user in the lobby.
uint64_t Steam::getLobbyMemberByIndex(uint64_t steam_lobby_id, int member){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	CSteamID lobbyMember = SteamMatchmaking()->GetLobbyMemberByIndex(lobby_id, member);
	return lobbyMember.ConvertToUint64();
}

// Get data associated with this lobby.
String Steam::getLobbyData(uint64_t steam_lobby_id, const String& key){
	if(SteamMatchmaking() == NULL){
		return "";
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->GetLobbyData(lobby_id, key.utf8().get_data());
}

// Sets a key/value pair in the lobby metadata.
bool Steam::setLobbyData(uint64_t steam_lobby_id, const String& key, const String& value){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->SetLobbyData(lobby_id, key.utf8().get_data(), value.utf8().get_data());
}

// Get lobby data by the lobby's ID
Dictionary Steam::getAllLobbyData(uint64_t steam_lobby_id){
	Dictionary data;
	if(SteamMatchmaking() == NULL){
		return data;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	int dataCount = SteamMatchmaking()->GetLobbyDataCount(lobby_id);
	char key;
	char value;
	for(int i = 0; i < dataCount; i++){
		bool success = SteamMatchmaking()->GetLobbyDataByIndex(lobby_id, i, &key, MAX_LOBBY_KEY_LENGTH, &value, CHAT_METADATA_MAX);
		if(success){
			data["index"] = i;
			data["key"] = key;
			data["value"] = value;
		}
	}
	return data;
}

// Removes a metadata key from the lobby.
bool Steam::deleteLobbyData(uint64_t steam_lobby_id, const String& key){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->DeleteLobbyData(lobby_id, key.utf8().get_data());
}

// Gets per-user metadata for someone in this lobby.
String Steam::getLobbyMemberData(uint64_t steam_lobby_id, uint64_t steam_id_user, const String& key){
	if(SteamMatchmaking() == NULL){
		return "";
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	CSteamID user_id = (uint64)steam_id_user;
	return SteamMatchmaking()->GetLobbyMemberData(lobby_id, user_id, key.utf8().get_data());
}

// Sets per-user metadata (for the local user implicitly).
void Steam::setLobbyMemberData(uint64_t steam_lobby_id, const String& key, const String& value){
	if(SteamMatchmaking() != NULL){
		CSteamID lobby_id = (uint64)steam_lobby_id;
		SteamMatchmaking()->SetLobbyMemberData(lobby_id, key.utf8().get_data(), value.utf8().get_data());
	}
}

// Broadcasts a chat message to the all the users in the lobby.
bool Steam::sendLobbyChatMsg(uint64_t steam_lobby_id, const String& message_body){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->SendLobbyChatMsg(lobby_id, message_body.utf8().get_data(), 4096);
}

// Refreshes metadata for a lobby you're not necessarily in right now.
bool Steam::requestLobbyData(uint64_t steam_lobby_id){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->RequestLobbyData(lobby_id);
}

// Sets the game server associated with the lobby.
//void Steam::setLobbyGameServer(uint64_t steam_lobby_id, const String& server_ip, uint16 server_port, uint64_t steam_id_game_server){
//	if(SteamMatchmaking() != NULL){
//		// Resolve address and convert it from IP_Address string to uint32_t
//		IP_Address address;
//		if(server_ip.is_valid_ip_address()){
//			address = server_ip;
//		}
//		else{
//			address = IP::get_singleton()->resolve_hostname(server_ip, IP::TYPE_IPV4);
//		}
//		// Resolution failed - Godot 3.0 has is_invalid() to check this
//		if(address == IP_Address()){
//			return;
//		}
//		uint32_t ip4 = *((uint32_t *)address.get_ipv4());
//		// Swap the bytes
//		uint8_t *ip4_p = (uint8_t *)&ip4;
//		for(int i = 0; i < 2; i++){
//			uint8_t temp = ip4_p[i];
//			ip4_p[i] = ip4_p[3-i];
//			ip4_p[3-i] = temp;
//		}
//		CSteamID lobby_id = (uint64)steam_lobby_id;
//		// If setting a game server with no server (fake) Steam ID
//		if(steam_id_game_server == 0){
//			SteamMatchmaking()->SetLobbyGameServer(lobby_id, *((uint32_t *)ip4_p), server_port, k_steamIDNil);
//		}
//		else{
//			CSteamID game_id = (uint64)steam_id_game_server;
//			SteamMatchmaking()->SetLobbyGameServer(lobby_id, *((uint32_t *)ip4_p), server_port, game_id);
//		}
//	}
//}

// Returns the details of a game server set in a lobby - returns false if there is no game server set, or that lobby doesn't exist.
Dictionary Steam::getLobbyGameServer(uint64_t steam_lobby_id){
	Dictionary game_server;
	CSteamID lobby_id = (uint64)steam_lobby_id;
	uint32 server_ip = 0;
	uint16 server_port = 0;
	CSteamID server_id;
	game_server["ret"] = SteamMatchmaking()->GetLobbyGameServer(lobby_id, &server_ip, &server_port, &server_id);
	if(game_server["ret"]){
		// Convert the IP address back to a string
		const int NBYTES = 4;
		uint8 octet[NBYTES];
		char ip[16];
		for(int i = 0; i < NBYTES; i++){
			octet[i] = server_ip >> (i * 8);
		}
		sprintf(ip, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
		game_server["ip"] = ip;
		game_server["port"] = server_port;
		// Convert the server ID
		uint64_t server = server_id.ConvertToUint64();
		game_server["id"] = server;
	}
	return game_server;
}

// Set the limit on the # of users who can join the lobby.
bool Steam::setLobbyMemberLimit(uint64_t steam_lobby_id, int max_members){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->SetLobbyMemberLimit(lobby_id, max_members);
}

// Returns the current limit on the # of users who can join the lobby; returns 0 if no limit is defined.
int Steam::getLobbyMemberLimit(uint64_t steam_lobby_id){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->GetLobbyMemberLimit(lobby_id);
}

// Updates which type of lobby it is.
bool Steam::setLobbyType(uint64_t steam_lobby_id, int lobby_type){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->SetLobbyType(lobby_id, (ELobbyType)lobby_type);
}

// Sets whether or not a lobby is joinable - defaults to true for a new lobby.
bool Steam::setLobbyJoinable(uint64_t steam_lobby_id, bool joinable){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	return SteamMatchmaking()->SetLobbyJoinable(lobby_id, joinable);
}

// Returns the current lobby owner.
uint64_t Steam::getLobbyOwner(uint64_t steam_lobby_id){
	if(SteamMatchmaking() == NULL){
		return 0;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	CSteamID owner_id = SteamMatchmaking()->GetLobbyOwner(lobby_id);
	return owner_id.ConvertToUint64();
}

// Changes who the lobby owner is.
bool Steam::setLobbyOwner(uint64_t steam_lobby_id, uint64_t steam_id_new_owner){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	CSteamID owner_id = (uint64)steam_id_new_owner;
	return SteamMatchmaking()->SetLobbyOwner(lobby_id, owner_id);
}

// Link two lobbies for the purposes of checking player compatibility.
bool Steam::setLinkedLobby(uint64_t steam_lobby_id, uint64_t steam_id_lobby_dependent){
	if(SteamMatchmaking() == NULL){
		return false;
	}
	CSteamID lobby_id = (uint64)steam_lobby_id;
	CSteamID dependent_id = (uint64)steam_id_lobby_dependent;
	return SteamMatchmaking()->SetLinkedLobby(lobby_id, dependent_id);
}


/////////////////////////////////////////////////
///// MATCHMAKING SERVERS
/////////////////////////////////////////////////
//
// Cancel an outstanding server list request.
void Steam::cancelQuery(uint64_t server_list_request){
	if(SteamMatchmakingServers() != NULL){
		SteamMatchmakingServers()->CancelQuery((HServerListRequest)server_list_request);
	}
}

// Cancel an outstanding individual server query.
void Steam::cancelServerQuery(int server_query){
	if(SteamMatchmakingServers() != NULL){
		SteamMatchmakingServers()->CancelServerQuery((HServerQuery)server_query);
	}
}

// Gets the number of servers in the given list.
int Steam::getServerCount(uint64_t server_list_request){
	if(SteamMatchmakingServers() == NULL){
		return 0;
	}
	return SteamMatchmakingServers()->GetServerCount((HServerListRequest)server_list_request);
}

// Get the details of a given server in the list.
Dictionary Steam::getServerDetails(uint64_t server_list_request, int server){
	// Create a dictionary to populate
	Dictionary game_server;
	if(SteamMatchmakingServers() != NULL){
		gameserveritem_t* server_item = new gameserveritem_t;
		SteamMatchmakingServers()->GetServerDetails((HServerListRequest)server_list_request, server);
		// Populate the dictionary
		game_server["ping"] = server_item->m_nPing;
		game_server["success_response"] = server_item->m_bHadSuccessfulResponse;
		game_server["no_refresh"] = server_item->m_bDoNotRefresh;
		game_server["game_dir"] = server_item->m_szGameDir[k_cbMaxGameServerGameDir];
		game_server["map"] = server_item->m_szMap[k_cbMaxGameServerMapName];
		game_server["description"] = server_item->m_szGameDescription[k_cbMaxGameServerGameDescription];
		game_server["app_id"] = server_item->m_nAppID;
		game_server["players"] = server_item->m_nPlayers;
		game_server["max_players"] = server_item->m_nMaxPlayers;
		game_server["bot_players"] = server_item->m_nBotPlayers;
		game_server["password"] = server_item->m_bPassword;
		game_server["secure"] = server_item->m_bSecure;
		game_server["last_played"] = server_item->m_ulTimeLastPlayed;
		game_server["server_version"] = server_item->m_nServerVersion;
		// Clean up
		delete server_item;
	}
	// Return the dictionary
	return game_server;
}

// Returns true if the list is currently refreshing its server list.
bool Steam::isRefreshing(uint64_t server_list_request){
	if(SteamMatchmakingServers() == NULL){
		return false;
	}
	return SteamMatchmakingServers()->IsRefreshing((HServerListRequest)server_list_request);
}

// Queries an individual game servers directly via IP/Port to request an updated ping time and other details from the server.
//int Steam::pingServer(const String& ip, uint16 port){
//	if(SteamMatchmakingServers() == NULL){
//		return 0;
//	}
//	// Resolve address and convert it from IP_Address string to uint32_t
//	IP_Address address;
//	if(ip.is_valid_ip_address()){
//		address = ip;
//	}
//	else{
//		address = IP::get_singleton()->resolve_hostname(ip, IP::TYPE_IPV4);
//	}
//	// Resolution failed - Godot 3.0 has is_invalid() to check this
//	if(address == IP_Address()){
//		return 0;
//	}
//	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
//	// Swap the bytes
//	uint8_t *ip4_p = (uint8_t *)&ip4;
//	for(int i = 0; i < 2; i++){
//		uint8_t temp = ip4_p[i];
//		ip4_p[i] = ip4_p[3-i];
//		ip4_p[3-i] = temp;
//	}
//	return SteamMatchmakingServers()->PingServer(*((uint32_t *)ip4_p), port, ping_response);
//}

// Request the list of players currently playing on a server.
int Steam::playerDetails(uint32 ip, uint16 port){
	if(SteamMatchmakingServers() == NULL){
		return 0;
	}
	return SteamMatchmakingServers()->PlayerDetails(ip, port, player_response);
}

// Ping every server in your list again but don't update the list of servers. Query callback installed when the server list was requested will be used again to post notifications and RefreshComplete, so the callback must remain valid until another RefreshComplete is called on it or the request is released with ReleaseRequest( hRequest ).
void Steam::refreshQuery(uint64_t server_list_request){
	if(SteamMatchmakingServers() != NULL){
		SteamMatchmakingServers()->RefreshQuery((HServerListRequest)server_list_request);
	}
}

// Refresh a single server inside of a query (rather than all the servers).
void Steam::refreshServer(uint64_t server_list_request, int server){
	if(SteamMatchmakingServers() != NULL){
		SteamMatchmakingServers()->RefreshServer((HServerListRequest)server_list_request, server);
	}
}

// Releases the asynchronous request object and cancels any pending query on it if there's a pending query in progress.
void Steam::releaseRequest(uint64_t server_list_request){
	if(SteamMatchmakingServers() != NULL){
		SteamMatchmakingServers()->ReleaseRequest((HServerListRequest)server_list_request);
	}
}

// Request a new list of servers of a particular type.  These calls each correspond to one of the EMatchMakingType values.
void Steam::requestFavoritesServerList(int app_id, Array filters){
	if(SteamMatchmakingServers() != NULL){
		uint32 filter_size = filters.size();
		MatchMakingKeyValuePair_t* filters_array = new MatchMakingKeyValuePair_t[filter_size];
		for (uint32 i=0; i < filter_size; i++){
			// Create a new filter pair to populate
			MatchMakingKeyValuePair_t *filter_array = new MatchMakingKeyValuePair_t();
			// Get the key/value pair
			Array pair = filters[i];
			// Get the key from the filter pair
			String key = pair[0];
			char *this_key = new char[256];
			strcpy(this_key, key.utf8().get_data());
			filter_array->m_szKey[i] = *this_key;
			delete[] this_key;
			// Get the value from the filter pair
			String value = pair[1];
			char *this_value = new char[256];
			strcpy(this_value, value.utf8().get_data());
			filter_array->m_szValue[i] = *this_value;
			delete[] this_value;
			// Append this to the array
			filters_array[i] = *filter_array;
			// Free up the memory
			delete filter_array;
		}
		server_list_request = SteamMatchmakingServers()->RequestFavoritesServerList((AppId_t)app_id, &filters_array, filter_size, server_list_response);
		delete[] filters_array;
	}
}

// Request a new list of servers of a particular type.  These calls each correspond to one of the EMatchMakingType values.
void Steam::requestFriendsServerList(int app_id, Array filters){
	if(SteamMatchmakingServers() != NULL){
		uint32 filter_size = filters.size();
		MatchMakingKeyValuePair_t* filters_array = new MatchMakingKeyValuePair_t[filter_size];
		for (uint32 i=0; i < filter_size; i++){
			// Create a new filter pair to populate
			MatchMakingKeyValuePair_t *filter_array = new MatchMakingKeyValuePair_t();
			// Get the key/value pair
			Array pair = filters[i];
			// Get the key from the filter pair
			String key = pair[0];
			char *this_key = new char[256];
			strcpy(this_key, key.utf8().get_data());
			filter_array->m_szKey[i] = *this_key;
			delete[] this_key;
			// Get the value from the filter pair
			String value = pair[1];
			char *this_value = new char[256];
			strcpy(this_value, value.utf8().get_data());
			filter_array->m_szValue[i] = *this_value;
			delete[] this_value;
			// Append this to the array
			filters_array[i] = *filter_array;
			// Free up the memory
			delete filter_array;
		}
		server_list_request = SteamMatchmakingServers()->RequestFriendsServerList((AppId_t)app_id, &filters_array, filter_size, server_list_response);
		delete[] filters_array;
	}
}

// Request a new list of servers of a particular type.  These calls each correspond to one of the EMatchMakingType values.
void Steam::requestHistoryServerList(int app_id, Array filters){
	if(SteamMatchmakingServers() != NULL){
		uint32 filter_size = filters.size();
		MatchMakingKeyValuePair_t* filters_array = new MatchMakingKeyValuePair_t[filter_size];
		for (uint32 i=0; i < filter_size; i++){
			// Create a new filter pair to populate
			MatchMakingKeyValuePair_t *filter_array = new MatchMakingKeyValuePair_t();
			// Get the key/value pair
			Array pair = filters[i];
			// Get the key from the filter pair
			String key = pair[0];
			char *this_key = new char[256];
			strcpy(this_key, key.utf8().get_data());
			filter_array->m_szKey[i] = *this_key;
			delete[] this_key;
			// Get the value from the filter pair
			String value = pair[1];
			char *this_value = new char[256];
			strcpy(this_value, value.utf8().get_data());
			filter_array->m_szValue[i] = *this_value;
			delete[] this_value;
			// Append this to the array
			filters_array[i] = *filter_array;
			// Free up the memory
			delete filter_array;
		}
		server_list_request = SteamMatchmakingServers()->RequestHistoryServerList((AppId_t)app_id, &filters_array, filter_size, server_list_response);
		delete[] filters_array;
	}
}

// Request a new list of servers of a particular type.  These calls each correspond to one of the EMatchMakingType values.
void Steam::requestInternetServerList(int app_id, Array filters){
	if(SteamMatchmakingServers() != NULL){
		uint32 filter_size = filters.size();
		MatchMakingKeyValuePair_t* filters_array = new MatchMakingKeyValuePair_t[filter_size];
		for (uint32 i=0; i < filter_size; i++){
			// Create a new filter pair to populate
			MatchMakingKeyValuePair_t *filter_array = new MatchMakingKeyValuePair_t();
			// Get the key/value pair
			Array pair = filters[i];
			// Get the key from the filter pair
			String key = pair[0];
			char *this_key = new char[256];
			strcpy(this_key, key.utf8().get_data());
			filter_array->m_szKey[i] = *this_key;
			delete[] this_key;
			// Get the value from the filter pair
			String value = pair[1];
			char *this_value = new char[256];
			strcpy(this_value, value.utf8().get_data());
			filter_array->m_szValue[i] = *this_value;
			delete[] this_value;
			// Append this to the array
			filters_array[i] = *filter_array;
			// Free up the memory
			delete filter_array;
		}
		server_list_request = SteamMatchmakingServers()->RequestInternetServerList((AppId_t)app_id, &filters_array, filter_size, server_list_response);
		delete[] filters_array;
	}
}

// Request a new list of servers of a particular type.  These calls each correspond to one of the EMatchMakingType values.
void Steam::requestLANServerList(int app_id){
	if(SteamMatchmakingServers() != NULL){
		server_list_request = SteamMatchmakingServers()->RequestLANServerList((AppId_t)app_id, server_list_response);
	}
}

// Request a new list of servers of a particular type.  These calls each correspond to one of the EMatchMakingType values.
void Steam::requestSpectatorServerList(int app_id, Array filters){
	if(SteamMatchmakingServers() != NULL){
		uint32 filter_size = filters.size();
		MatchMakingKeyValuePair_t* filters_array = new MatchMakingKeyValuePair_t[filter_size];
		for (uint32 i=0; i < filter_size; i++){
			// Create a new filter pair to populate
			MatchMakingKeyValuePair_t *filter_array = new MatchMakingKeyValuePair_t();
			// Get the key/value pair
			Array pair = filters[i];
			// Get the key from the filter pair
			String key = pair[0];
			char *this_key = new char[256];
			strcpy(this_key, key.utf8().get_data());
			filter_array->m_szKey[i] = *this_key;
			delete[] this_key;
			// Get the value from the filter pair
			String value = pair[1];
			char *this_value = new char[256];
			strcpy(this_value, value.utf8().get_data());
			filter_array->m_szValue[i] = *this_value;
			delete[] this_value;
			// Append this to the array
			filters_array[i] = *filter_array;
			// Free up the memory
			delete filter_array;
		}
		server_list_request = SteamMatchmakingServers()->RequestSpectatorServerList((AppId_t)app_id, &filters_array, filter_size, server_list_response);
		delete[] filters_array;
	}
}

// Request the list of rules that the server is running (See ISteamGameServer::SetKeyValue() to set the rules server side)
int Steam::serverRules(uint32 ip, uint16 port){
	if(SteamMatchmakingServers() == NULL){
		return 0;
	}
	return SteamMatchmakingServers()->ServerRules(ip, port, rules_response);
}


/////////////////////////////////////////////////
///// MUSIC
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
///// MUSIC REMOTE
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
bool Steam::setPlaylistEntry(int id, int position, const String& entry_text){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->SetPlaylistEntry(id, position, entry_text.utf8().get_data());
}

// Set a PNG icon for a song? A playlist?
//bool Steam::setPNGIcon64x64(const PoolByteArray& icon){
//	if(SteamMusicRemote() == NULL){
//		return false;
//	}
//	return SteamMusicRemote()->SetPNGIcon_64x64(icon.read().ptr(), icon.size());
//}

// Set a new queue entry.
bool Steam::setQueueEntry(int id, int position, const String& entry_text){
	if(SteamMusicRemote() == NULL){
		return false;
	}
	return SteamMusicRemote()->SetQueueEntry(id, position, entry_text.utf8().get_data());
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
///// NETWORKING
/////////////////////////////////////////////////
//
// This allows the game to specify accept an incoming packet.
bool Steam::acceptP2PSessionWithUser(uint64_t steam_id_remote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = (uint64)steam_id_remote;
	return SteamNetworking()->AcceptP2PSessionWithUser(steam_id);
}

// Allow or disallow P2P connections to fall back to being relayed through the Steam servers if a direct connection or NAT-traversal cannot be established.
bool Steam::allowP2PPacketRelay(bool allow) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	return SteamNetworking()->AllowP2PPacketRelay(allow);
}

// Closes a P2P channel when you're done talking to a user on the specific channel.
bool Steam::closeP2PChannelWithUser(uint64_t steam_id_remote, int channel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = (uint64)steam_id_remote;
	return SteamNetworking()->CloseP2PChannelWithUser(steam_id, channel);
}

// This should be called when you're done communicating with a user, as this will free up all of the resources allocated for the connection under-the-hood.
bool Steam::closeP2PSessionWithUser(uint64_t steam_id_remote) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = (uint64)steam_id_remote;
	return SteamNetworking()->CloseP2PSessionWithUser(steam_id);
}

// Fills out a P2PSessionState_t structure with details about the connection like whether or not there is an active connection.
Dictionary Steam::getP2PSessionState(uint64_t steam_id_remote) {
	Dictionary result;
	if (SteamNetworking() == NULL) {
		return result;
	}
	CSteamID steam_id = (uint64)steam_id_remote;
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
bool Steam::sendP2PPacket(uint64_t steam_id_remote, PoolByteArray data, int send_type, int channel) {
	if (SteamNetworking() == NULL) {
		return false;
	}
	CSteamID steam_id = (uint64)steam_id_remote;
	return SteamNetworking()->SendP2PPacket(steam_id, data.read().ptr(), data.size(), EP2PSend(send_type), channel);
}


/////////////////////////////////////////////////
///// NETWORKING MESSAGES
/////////////////////////////////////////////////
//
// Sends a message to the specified host. If we don't already have a session with that user, a session is implicitly created. There might be some handshaking that needs to happen before we can actually begin sending message data.
int Steam::sendMessageToUser(const String& message, int flags, int channel){
	if(SteamNetworkingMessages() == NULL){
		return 0;
	}
	return SteamNetworkingMessages()->SendMessageToUser(networkingIdentity, message.utf8().get_data(), message.length(), flags, channel);
}

// Reads the next message that has been sent from another user via SendMessageToUser() on the given channel. Returns number of messages returned into your list.  (0 if no message are available on that channel.)
Array Steam::receiveMessagesOnChannel(int channel, int max_messages){
	Array messages;
	if(SteamNetworkingMessages() != NULL){
		// Allocate the space for the messages
		SteamNetworkingMessage_t *channel_messages;
		channel_messages = SteamNetworkingUtils()->AllocateMessage(0);
		// Get the messages
		int available_messages = SteamNetworkingMessages()->ReceiveMessagesOnChannel(channel, &channel_messages, max_messages);
		// Which is bigger, max messages or available messages?
		int message_iteration = available_messages;
		if(available_messages < max_messages){
			message_iteration = max_messages;
		}
		// Loop through and create the messages as dictionaries then add to the messages array
		for(int i = 1; i < message_iteration; i++){
			Dictionary message;
			message["payload"] = channel_messages[i].m_pData;
			message["size"] = channel_messages[i].m_cbSize;
			message["connection"] = channel_messages[i].m_conn;
			char identity;
			channel_messages[i].m_identityPeer.ToString(&identity, 128);
			message["identity"] = identity;
			message["user_data"] = (uint64_t)channel_messages[i].m_nConnUserData;
			message["time_received"] = (uint64_t)channel_messages[i].m_usecTimeReceived;
			message["message_number"] = (uint64_t)channel_messages[i].m_nMessageNumber;
			message["channel"] = channel_messages[i].m_nChannel;
			message["flags"] = channel_messages[i].m_nFlags;
			message["user_data"] = (uint64_t)channel_messages[i].m_nUserData;
			messages.append(message);
		}
		// Release the messages
		channel_messages->Release();
	}
	return messages;
}

// AcceptSessionWithUser() should only be called in response to a SteamP2PSessionRequest_t callback SteamP2PSessionRequest_t will be posted if another user tries to send you a message, and you haven't tried to talk to them.
bool Steam::acceptSessionWithUser(){
	if(SteamNetworkingMessages() == NULL){
		return false;
	}
	return SteamNetworkingMessages()->AcceptSessionWithUser(networkingIdentity);
}

// Call this when you're done talking to a user to immediately free up resources under-the-hood.
bool Steam::closeSessionWithUser(){
	if(SteamNetworkingMessages() == NULL){
		return false;
	}
	return SteamNetworkingMessages()->CloseSessionWithUser(networkingIdentity);
}

// Call this  when you're done talking to a user on a specific channel. Once all open channels to a user have been closed, the open session to the user will be closed, and any new data from this user will trigger a SteamP2PSessionRequest_t callback.
bool Steam::closeChannelWithUser(int channel){
	if(SteamNetworkingMessages() == NULL){
		return false;
	}
	return SteamNetworkingMessages()->CloseChannelWithUser(networkingIdentity, channel);
}

// Returns information about the latest state of a connection, if any, with the given peer.
int Steam::getSessionConnectionInfo(){
	if(SteamNetworkingMessages() == NULL){
		return 0;
	}
	SteamNetConnectionInfo_t info;
	SteamNetworkingQuickConnectionStatus status;
	return SteamNetworkingMessages()->GetSessionConnectionInfo(networkingIdentity, &info, &status);
}


/////////////////////////////////////////////////
///// NETWORKING SOCKETS
/////////////////////////////////////////////////
//
// Creates a "server" socket that listens for clients to connect to by calling ConnectByIPAddress, over ordinary UDP (IPv4 or IPv6)
uint32 Steam::createListenSocketIP(const int options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->CreateListenSocketIP(networkingIPAddress, options, &networkingConfigValue);
}

// Creates a connection and begins talking to a "server" over UDP at the given IPv4 or IPv6 address. The remote host must be listening with a matching call to ISteamnetworkingSockets::CreateListenSocketIP on the specified port.
uint32 Steam::connectByIPAddress(uint32 ip, uint16 port, Array options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	// Set the address
	networkingIPAddress.SetIPv4(ip, port);
	int number_of_options = options.size();
	return SteamNetworkingSockets()->ConnectByIPAddress(networkingIPAddress, number_of_options, &networkingConfigValue);
}

// Like CreateListenSocketIP, but clients will connect using ConnectP2P. The connection will be relayed through the Valve network.
uint32 Steam::createListenSocketP2P(int port, int option_size){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->CreateListenSocketP2P(port, option_size, &networkingConfigValue);
}

// Begin connecting to a server that is identified using a platform-specific identifier. This uses the default rendezvous service, which depends on the platform and library configuration. (E.g. on Steam, it goes through the steam backend.) The traffic is relayed over the Steam Datagram Relay network.
uint32 Steam::connectP2P(int port, int number_of_options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->ConnectP2P(networkingIdentity, port, number_of_options, &networkingConfigValue);
}

// Accept an incoming connection that has been received on a listen socket.
int Steam::acceptConnection(uint32 connection){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->AcceptConnection((HSteamNetConnection)connection);
}

// Disconnects from the remote host and invalidates the connection handle. Any unread data on the connection is discarded.
bool Steam::closeConnection(uint32 peer, int reason, bool linger){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	char debug;
	return SteamNetworkingSockets()->CloseConnection((HSteamNetConnection)peer, reason, &debug, linger);
}

// Destroy a listen socket. All the connections that were accepted on the listen socket are closed ungracefully.
bool Steam::closeListenSocket(uint32 socket){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->CloseListenSocket((HSteamListenSocket)socket);
}

// Create a pair of connections that are talking to each other, e.g. a loopback connection. This is very useful for testing, or so that your client/server code can work the same even when you are running a local "server".
Dictionary Steam::createSocketPair(bool loopback, const String& identity1, const String& identity2){
	// Create a dictionary to populate
	Dictionary connection_pair;
	if(SteamNetworkingSockets() != NULL){
		// Turn the strings back to structs - Should be a check for failure to parse from string
		SteamNetworkingIdentity identity_struct1;
		SteamNetworkingIdentity identity_struct2;
		identity_struct1.ParseString(identity1.utf8().get_data());
		identity_struct2.ParseString(identity2.utf8().get_data());
		// Get connections
		uint32 connection1 = 0;
		uint32 connection2 = 0;
		bool success = SteamNetworkingSockets()->CreateSocketPair(&connection1, &connection2, loopback, &identity_struct1, &identity_struct2);
		// Populate the dictionary
		connection_pair["success"] = success;
		connection_pair["connection1"] = connection1;
		connection_pair["connection2"] = connection2;
	}
	return connection_pair;
}

// Send a message to the remote host on the specified connection.
int Steam::sendMessageToConnection(uint32 connection, const String& message, int flags){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	int64 number;
	return SteamNetworkingSockets()->SendMessageToConnection((HSteamNetConnection)connection, message.utf8().get_data(), message.length(), flags, &number);
}

// Send one or more messages without copying the message payload. This is the most efficient way to send messages. To use this function, you must first allocate a message object using ISteamNetworkingUtils::AllocateMessage. (Do not declare one on the stack or allocate your own.)
void Steam::sendMessages(int messages, const PoolStringArray& message, uint32 connection, int flags){
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingMessage_t *networkMessage;
		networkMessage = SteamNetworkingUtils()->AllocateMessage(0);
		networkMessage->m_pData = (void *)message.read().ptr();
		networkMessage->m_cbSize = message.size();
		networkMessage->m_conn = (HSteamNetConnection)connection;
		networkMessage->m_nFlags = flags;
		int64 result;
		SteamNetworkingSockets()->SendMessages(messages, &networkMessage, &result);
		// Release the message
		networkMessage->Release();
	}
}

// Flush any messages waiting on the Nagle timer and send them at the next transmission opportunity (often that means right now).
int Steam::flushMessagesOnConnection(uint32 connection){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->FlushMessagesOnConnection((HSteamNetConnection)connection);
}

// Fetch the next available message(s) from the connection, if any. Returns the number of messages returned into your array, up to nMaxMessages. If the connection handle is invalid, -1 is returned. If no data is available, 0, is returned.
Array Steam::receiveMessagesOnConnection(uint32 connection, int max_messages){
	Array messages;
	if(SteamNetworkingSockets() != NULL){
		// Allocate the space for the messages
		SteamNetworkingMessage_t *connection_messages;
		connection_messages = SteamNetworkingUtils()->AllocateMessage(0);
		// Get the messages
		int available_messages = SteamNetworkingSockets()->ReceiveMessagesOnConnection((HSteamNetConnection)connection, &connection_messages, max_messages);
		// Which is bigger, max messages or available messages?
		int message_iteration = available_messages;
		if(available_messages < max_messages){
			message_iteration = max_messages;
		}
		// Loop through and create the messages as dictionaries then add to the messages array
		for(int i = 1; i < message_iteration; i++){
			Dictionary message;
			message["payload"] = connection_messages[i].m_pData;
			message["size"] = connection_messages[i].m_cbSize;
			message["connection"] = connection_messages[i].m_conn;
			char identity;
			connection_messages[i].m_identityPeer.ToString(&identity, 128);
			message["identity"] = identity;
			message["user_data"] = (uint64_t)connection_messages[i].m_nConnUserData;
			message["time_received"] = (uint64_t)connection_messages[i].m_usecTimeReceived;
			message["message_number"] = (uint64_t)connection_messages[i].m_nMessageNumber;
			message["channel"] = connection_messages[i].m_nChannel;
			message["flags"] = connection_messages[i].m_nFlags;
			message["user_data"] = (uint64_t)connection_messages[i].m_nUserData;
			messages.append(message);
		}
		// Release the messages
		connection_messages->Release();
	}
	return messages;
}

// Create a new poll group.
uint32 Steam::createPollGroup(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->CreatePollGroup();
}

// Destroy a poll group created with CreatePollGroup.
bool Steam::destroyPollGroup(uint32 poll_group){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->DestroyPollGroup((HSteamNetPollGroup)poll_group);
}

// Assign a connection to a poll group. Note that a connection may only belong to a single poll group. Adding a connection to a poll group implicitly removes it from any other poll group it is in.
bool Steam::setConnectionPollGroup(uint32 connection, uint32 poll_group){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->SetConnectionPollGroup((HSteamNetConnection)connection, (HSteamNetPollGroup)poll_group);
}

// Same as ReceiveMessagesOnConnection, but will return the next messages available on any connection in the poll group. Examine SteamNetworkingMessage_t::m_conn to know which connection. (SteamNetworkingMessage_t::m_nConnUserData might also be useful.)
Array Steam::receiveMessagesOnPollGroup(uint32 poll_group, int max_messages){
	Array messages;
	if(SteamNetworkingSockets() != NULL){
		// Allocate the space for the messages
		SteamNetworkingMessage_t *poll_messages;
		poll_messages = SteamNetworkingUtils()->AllocateMessage(0);
		// Get the messages
		int available_messages = SteamNetworkingSockets()->ReceiveMessagesOnPollGroup((HSteamNetPollGroup)poll_group, &poll_messages, max_messages);
		// Which is bigger, max messages or available messages?
		int message_iteration = available_messages;
		if(available_messages < max_messages){
			message_iteration = max_messages;
		}
		// Loop through and create the messages as dictionaries then add to the messages array
		for(int i = 1; i < message_iteration; i++){
			Dictionary message;
			message["payload"] = poll_messages[i].m_pData;
			message["size"] = poll_messages[i].m_cbSize;
			message["connection"] = poll_messages[i].m_conn;
			char identity;
			poll_messages[i].m_identityPeer.ToString(&identity, 128);
			message["identity"] = identity;
			message["user_data"] = (uint64_t)poll_messages[i].m_nConnUserData;
			message["time_received"] = (uint64_t)poll_messages[i].m_usecTimeReceived;
			message["message_number"] = (uint64_t)poll_messages[i].m_nMessageNumber;
			message["channel"] = poll_messages[i].m_nChannel;
			message["flags"] = poll_messages[i].m_nFlags;
			message["user_data"] = (uint64_t)poll_messages[i].m_nUserData;
			messages.append(message);
		}
		// Release the messages
		poll_messages->Release();
	}
	return messages;
}

// Returns basic information about the high-level state of the connection. Returns false if the connection handle is invalid.
bool Steam::getConnectionInfo(uint32 connection){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	SteamNetConnectionInfo_t info;
	return SteamNetworkingSockets()->GetConnectionInfo((HSteamNetConnection)connection, &info);	
}

// Returns a small set of information about the real-time state of the connection. Returns false if the connection handle is invalid, or the connection has ended.
bool Steam::getQuickConnectionStatus(uint32 connection){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	SteamNetworkingQuickConnectionStatus stats;
	return SteamNetworkingSockets()->GetQuickConnectionStatus((HSteamNetConnection)connection, &stats);
}

// Returns very detailed connection stats in diagnostic text format. Useful for dumping to a log, etc. The format of this information is subject to change.
Dictionary Steam::getDetailedConnectionStatus(uint32 connection){
	Dictionary connectionStatus;
	if(SteamNetworkingSockets() != NULL){
		char buffer;
		int success = SteamNetworkingSockets()->GetDetailedConnectionStatus((HSteamNetConnection)connection, &buffer, 2048);
		// Add data to dictionary
		connectionStatus["success"] = success;
		connectionStatus["status"] = buffer;
	}
	// Send the data back to the user
	return connectionStatus; 
}

// Fetch connection user data. Returns -1 if handle is invalid or if you haven't set any userdata on the connection.
uint64_t Steam::getConnectionUserData(uint32 peer){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->GetConnectionUserData((HSteamNetConnection)peer);
}

// Set a name for the connection, used mostly for debugging
void Steam::setConnectionName(uint32 peer, const String& name){
	if(SteamNetworkingSockets() != NULL){
		SteamNetworkingSockets()->SetConnectionName((HSteamNetConnection)peer, name.utf8().get_data());
	}
}

// Fetch connection name into your buffer, which is at least nMaxLen bytes. Returns false if handle is invalid.
String Steam::getConnectionName(uint32 peer){
	// Set empty string variable for use
	String connection_name = "";
	if(SteamNetworkingSockets() != NULL){
		char name;
		if(SteamNetworkingSockets()->GetConnectionName((HSteamNetConnection)peer, &name, 256)){
			connection_name += name;	
		}
	}
	return connection_name;
}

// Returns local IP and port that a listen socket created using CreateListenSocketIP is bound to.
bool Steam::getListenSocketAddress(uint32 socket){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	SteamNetworkingIPAddr address;
	return SteamNetworkingSockets()->GetListenSocketAddress((HSteamListenSocket)socket, &address);
}

// Get the identity assigned to this interface.
bool Steam::getIdentity(){
	if(SteamNetworkingSockets() == NULL){
		return false;
	}
	return SteamNetworkingSockets()->GetIdentity(&networkingIdentity);
}

// Indicate our desire to be ready participate in authenticated communications. If we are currently not ready, then steps will be taken to obtain the necessary certificates. (This includes a certificate for us, as well as any CA certificates needed to authenticate peers.)
int Steam::initAuthentication(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->InitAuthentication();
}

// Query our readiness to participate in authenticated communications. A SteamNetAuthenticationStatus_t callback is posted any time this status changes, but you can use this function to query it at any time.
int Steam::getAuthenticationStatus(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	SteamNetAuthenticationStatus_t details;
	return SteamNetworkingSockets()->GetAuthenticationStatus(&details);
}

// Call this when you receive a ticket from your backend / matchmaking system. Puts the ticket into a persistent cache, and optionally returns the parsed ticket.
//Dictionary Steam::receivedRelayAuthTicket(){
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
//int Steam::findRelayAuthTicketForServer(int port){
//	int expires_in_seconds = 0;
//	if(SteamNetworkingSockets() != NULL){
//		expires_in_seconds = SteamNetworkingSockets()->FindRelayAuthTicketForServer(game_server, port, &relay_auth_ticket);
//	}
//	return expires_in_seconds;
//}

// Client call to connect to a server hosted in a Valve data center, on the specified virtual port. You must have placed a ticket for this server into the cache, or else this connect attempt will fail!
uint32 Steam::connectToHostedDedicatedServer(int port, int options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->ConnectToHostedDedicatedServer(networkingIdentity, port, options, &networkingConfigValue);
}

// Returns the value of the SDR_LISTEN_PORT environment variable. This is the UDP server your server will be listening on. This will configured automatically for you in production environments.
uint16 Steam::getHostedDedicatedServerPort(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->GetHostedDedicatedServerPort();
}

// Returns 0 if SDR_LISTEN_PORT is not set. Otherwise, returns the data center the server is running in. This will be k_SteamDatagramPOPID_dev in non-production envirionment.
uint32 Steam::getHostedDedicatedServerPOPId(){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->GetHostedDedicatedServerPOPID();
}

// Return info about the hosted server. This contains the PoPID of the server, and opaque routing information that can be used by the relays to send traffic to your server.
//int Steam::getHostedDedicatedServerAddress(){
//	int result = 2;
//	if(SteamNetworkingSockets() != NULL){
//		result = SteamNetworkingSockets()->GetHostedDedicatedServerAddress(&hosted_address);
//	}
//	return result;
//}

// Create a listen socket on the specified virtual port. The physical UDP port to use will be determined by the SDR_LISTEN_PORT environment variable. If a UDP port is not configured, this call will fail.
uint32 Steam::createHostedDedicatedServerListenSocket(int port, int options){
	if(SteamNetworkingSockets() == NULL){
		return 0;
	}
	return SteamNetworkingSockets()->CreateHostedDedicatedServerListenSocket(port, options, &networkingConfigValue);
}

// Generate an authentication blob that can be used to securely login with your backend, using SteamDatagram_ParseHostedServerLogin. (See steamdatagram_gamecoordinator.h)
//int Steam::getGameCoordinatorServerLogin(const String& app_data){
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


/////////////////////////////////////////////////
///// NETWORKING UTILS
/////////////////////////////////////////////////
//
// If you know that you are going to be using the relay network (for example, because you anticipate making P2P connections), call this to initialize the relay network. If you do not call this, the initialization will be delayed until the first time you use a feature that requires access to the relay network, which will delay that first access.
void Steam::initRelayNetworkAccess(){
	if(SteamNetworkingUtils() != NULL){
		SteamNetworkingUtils()->InitRelayNetworkAccess();
	}
}

// Fetch current status of the relay network.  If you want more details, you can pass a non-NULL value.
int Steam::getRelayNetworkStatus(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetRelayNetworkStatus(NULL);
}

// Return location info for the current host. Returns the approximate age of the data, in seconds, or -1 if no data is available.
float Steam::getLocalPingLocation(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	SteamNetworkPingLocation_t result;
	return SteamNetworkingUtils()->GetLocalPingLocation(result);
}

// Estimate the round-trip latency between two arbitrary locations, in milliseconds. This is a conservative estimate, based on routing through the relay network. For most basic relayed connections, this ping time will be pretty accurate, since it will be based on the route likely to be actually used.
int Steam::estimatePingTimeBetweenTwoLocations(uint8 location1, uint8 location2){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	// Add these locations to ping structs
	SteamNetworkPingLocation_t ping_location1;
	SteamNetworkPingLocation_t ping_location2;
	ping_location1.m_data[1] = {location1};
	ping_location2.m_data[1] = {location2};
	return SteamNetworkingUtils()->EstimatePingTimeBetweenTwoLocations(ping_location1, ping_location2);
}

// Same as EstimatePingTime, but assumes that one location is the local host. This is a bit faster, especially if you need to calculate a bunch of these in a loop to find the fastest one.
int Steam::estimatePingTimeFromLocalHost(uint8 location){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	// Add this location to ping struct
	SteamNetworkPingLocation_t ping_location;
	ping_location.m_data[1] = {location};
	return SteamNetworkingUtils()->EstimatePingTimeFromLocalHost(ping_location);
}

// Convert a ping location into a text format suitable for sending over the wire. The format is a compact and human readable. However, it is subject to change so please do not parse it yourself. Your buffer must be at least k_cchMaxSteamNetworkingPingLocationString bytes.
String Steam::convertPingLocationToString(uint8 location){
	String location_string = "";
	if(SteamNetworkingUtils() != NULL){
		char buffer;
		// Add this location to ping struct
		SteamNetworkPingLocation_t ping_location;
		ping_location.m_data[1] = {location};
		SteamNetworkingUtils()->ConvertPingLocationToString(ping_location, &buffer, 1024);
		location_string += buffer;
	}
	return location_string;
}

// Parse back SteamNetworkPingLocation_t string. Returns false if we couldn't understand the string.
bool Steam::parsePingLocationString(const String& string){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	SteamNetworkPingLocation_t result;
	return SteamNetworkingUtils()->ParsePingLocationString(string.utf8().get_data(), result);
}

// Check if the ping data of sufficient recency is available, and if it's too old, start refreshing it.
bool Steam::checkPingDataUpToDate(float max_age_in_seconds){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->CheckPingDataUpToDate(max_age_in_seconds);
}

// IN DOCUMENTATION BUT NOT IN ACTUAL HEADER
// Return true if we are taking ping measurements to update our ping location or select optimal routing. Ping measurement typically takes a few seconds, perhaps up to 10 seconds.
//bool Steam::isPingMeasurementInProgress(){
//	if(SteamNetworkingUtils() == NULL){
//		return false;
//	}
//	return SteamNetworkingUtils()->IsPingMeasurementInProgress();
//}

// Fetch ping time of best available relayed route from this host to the specified data center.
int Steam::getPingToDataCenter(uint32 pop_id, uint64_t via_replay_pop){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetPingToDataCenter(pop_id, (SteamNetworkingPOPID *)via_replay_pop);
}

// Get *direct* ping time to the relays at the point of presence.
int Steam::getDirectPingToPOP(uint32 pop_id){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetDirectPingToPOP(pop_id);
}

// Get number of network points of presence in the config
int Steam::getPOPCount(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetPOPCount();
}

// Get list of all POP IDs. Returns the number of entries that were filled into your list.
int Steam::getPOPList(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	SteamNetworkingPOPID list;
	return SteamNetworkingUtils()->GetPOPList(&list, 256);
}

// Set a configuration value.
//bool Steam::setConfigValue(int setting, int scope_type, int data_type, const void* value){
//	if(SteamNetworkingUtils() == NULL){
//		return false;
//	}
//	return SteamNetworkingUtils()->SetConfigValue((ESteamNetworkingConfigValue)setting, (ESteamNetworkingConfigScope)scope_type, NULL, (ESteamNetworkingConfigDataType)data_type, value);
//}

// Get a configuration value.
//Dictionary Steam::getConfigValue(int value, int scope_type){
//	Dictionary config_value;
//	if(SteamNetworkingUtils() != NULL){
//		ESteamNetworkingConfigDataType data_type;
//		size_t buffer_size;
//		PoolByteArray config_result;
//		int result = SteamNetworkingUtils()->GetConfigValue((ESteamNetworkingConfigValue)value, (ESteamNetworkingConfigScope)scope_type, NULL, &data_type, &result, &buffer_size);
//		// Populate the dictionary
//		config_value["result"] = result;
//		config_value["type"] = data_type;
//		config_value["value"] = result;
//		config_value["buffer"] = buffer_size;
//	}
//	return config_value;
//}

// Returns info about a configuration value.
Dictionary Steam::getConfigValueInfo(int value){
	Dictionary config_info;
	if(SteamNetworkingUtils() != NULL){
		const char *name;
		ESteamNetworkingConfigDataType data_type;
		ESteamNetworkingConfigScope scope;
		ESteamNetworkingConfigValue next_value;
		if(SteamNetworkingUtils()->GetConfigValueInfo((ESteamNetworkingConfigValue)value, &name, &data_type, &scope, &next_value)){
			// Populate the dictionary
			config_info["value"] = name;
			config_info["type"] = data_type;
			config_info["scope"] = scope;
			config_info["next_value"] = next_value;
		}
	}
	return config_info;
}

// Returns info about a configuration value.  Returns false if the value does not exist.
int Steam::getFirstConfigValue(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetFirstConfigValue();
}

// The following functions are handy shortcuts for common use cases.
bool Steam::setGlobalConfigValueInt32(int config, int32 value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetGlobalConfigValueInt32((ESteamNetworkingConfigValue)config, value);
}
bool Steam::setGlobalConfigValueFloat(int config, float value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetGlobalConfigValueFloat((ESteamNetworkingConfigValue)config, value);
}
bool Steam::setGlobalConfigValueString(int config, const String& value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetGlobalConfigValueString((ESteamNetworkingConfigValue)config, value.utf8().get_data());
}
bool Steam::setConnectionConfigValueInt32(uint32 connection, int config, int32 value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetConnectionConfigValueInt32(connection, (ESteamNetworkingConfigValue)config, value);
}
bool Steam::setConnectionConfigValueFloat(uint32 connection, int config, float value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetConnectionConfigValueFloat(connection, (ESteamNetworkingConfigValue)config, value);
}
bool Steam::setConnectionConfigValueString(uint32 connection, int config, const String& value){
	if(SteamNetworkingUtils() == NULL){
		return false;
	}
	return SteamNetworkingUtils()->SetConnectionConfigValueString(connection, (ESteamNetworkingConfigValue)config, value.utf8().get_data());
}

// Allocate and initialize a message object. Usually the reason you call this is to pass it to ISteamNetworkingSockets::SendMessages. The returned object will have all of the relevant fields cleared to zero.
void Steam::allocateMessage(int buffer){
	if(SteamNetworkingUtils() != NULL){
		network_messages = SteamNetworkingUtils()->AllocateMessage(buffer);
	}
}

// A general purpose high resolution local timer with the following properties: Monotonicity is guaranteed. The initial value will be at least 24*3600*30*1e6, i.e. about 30 days worth of microseconds. In this way, the timestamp value of 0 will always be at least "30 days ago". Also, negative numbers will never be returned. Wraparound / overflow is not a practical concern.
uint32 Steam::getLocalTimestamp(){
	if(SteamNetworkingUtils() == NULL){
		return 0;
	}
	return SteamNetworkingUtils()->GetLocalTimestamp();
}

// Set a function to receive network-related information that is useful for debugging.
//void Steam::setDebugOutputFunction(int detail_level, String& function){
//	if(SteamNetworkingUtils() != NULL){
//		SteamNetworkingUtils()->SetDebugOutputFunction((ESteamNetworkingSocketsDebugOutputType)detail_level, function.utf8().get_data());
//	}
//}


/////////////////////////////////////////////////
///// PARENTAL SETTINGS
/////////////////////////////////////////////////
//
// There are no notes about these functions, names can assume functionality.
bool Steam::isParentalLockEnabled(){
	if(SteamParentalSettings() == NULL){
		return false;
	}
	return SteamParentalSettings()->BIsParentalLockEnabled();
}

bool Steam::isParentalLockLocked(){
	if(SteamParentalSettings() == NULL){
		return false;
	}
	return SteamParentalSettings()->BIsParentalLockLocked();
}

bool Steam::isAppBlocked(AppId_t app_id){
	if(SteamParentalSettings() == NULL){
		return false;
	}
	return SteamParentalSettings()->BIsAppBlocked(app_id);
}

bool Steam::isAppInBlockList(AppId_t app_id){
	if(SteamParentalSettings() == NULL){
		return false;
	}
	return SteamParentalSettings()->BIsAppInBlockList(app_id);
}

bool Steam::isFeatureBlocked(int feature){
	if(SteamParentalSettings() == NULL){
		return false;
	}
	return SteamParentalSettings()->BIsFeatureBlocked((EParentalFeature)feature);
}

bool Steam::isFeatureInBlockList(int feature){
	if(SteamParentalSettings() == NULL){
		return false;
	}
	return SteamParentalSettings()->BIsFeatureInBlockList((EParentalFeature)feature);
}


/////////////////////////////////////////////////
///// PARTIES
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
				Dictionary beacon_data;
				beacon_data["type"] = beacons[i].m_eType;
				beacon_data["location_id"] = (uint64_t)beacons[i].m_ulLocationID;
				beaconLocations.append(beacon_data);
			}
		}
	}
	delete[] beacons;
	return beaconLocations;
}

// Create a beacon. You can only create one beacon at a time. Steam will display the beacon in the specified location, and let up to unOpenSlots users "follow" the beacon to your party.
void Steam::createBeacon(uint32 open_slots, uint64_t location, int type, const String& connect_string, const String& metadata){
	if(SteamParties() != NULL){
		// Add data to the beacon location struct
		SteamPartyBeaconLocation_t *beacon_data = new SteamPartyBeaconLocation_t;
		beacon_data->m_eType = (ESteamPartyBeaconLocationType)type;
		beacon_data->m_ulLocationID = location;
		SteamAPICall_t api_call = SteamParties()->CreateBeacon(open_slots, beacon_data, connect_string.utf8().get_data(), metadata.utf8().get_data());
		callResultCreateBeacon.Set(api_call, this, &Steam::_create_beacon);
		delete beacon_data;
	}
}

// When a user follows your beacon, Steam will reserve one of the open party slots for them, and send your game a ReservationNotificationCallback_t callback. When that user joins your party, call OnReservationCompleted to notify Steam that the user has joined successfully.
void Steam::onReservationCompleted(uint64_t beacon, uint64_t steam_id){
	if(SteamParties() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamParties()->OnReservationCompleted(beacon, user_id);
	}
}

// To cancel a reservation (due to timeout or user input), call this. Steam will open a new reservation slot. Note: The user may already be in-flight to your game, so it's possible they will still connect and try to join your party.
void cancelReservation(uint64_t beacon, uint64_t steam_id){
	if(SteamParties() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamParties()->CancelReservation(beacon, user_id);
	}
}

// If a user joins your party through other matchmaking (perhaps a direct Steam friend, or your own matchmaking system), your game should reduce the number of open slots that Steam is managing through the party beacon. For example, if you created a beacon with five slots, and Steam sent you two ReservationNotificationCallback_t callbacks, and then a third user joined directly, you would want to call ChangeNumOpenSlots with a value of 2 for unOpenSlots. That value represents the total number of new users that you would like Steam to send to your party.
void Steam::changeNumOpenSlots(uint64_t beacon, uint32 open_slots){
	if(SteamParties() != NULL){
		SteamAPICall_t api_call = SteamParties()->ChangeNumOpenSlots(beacon, open_slots);
		callResultChangeNumOpenSlots.Set(api_call, this, &Steam::_change_num_open_slots);
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
		SteamAPICall_t api_call = SteamParties()->JoinParty(beacon);
		callResultJoinParty.Set(api_call, this, &Steam::_join_party);
	}
}

// Query general metadata for the given beacon location. For instance the Name, or the URL for an icon if the location type supports icons (for example, the icon for a Steam Chat Room Group).
String Steam::getBeaconLocationData(uint64_t location_id, int location_type, int location_data){
	String beacon_location_data = "";
	if(SteamParties() != NULL){
		char *beacon_data = new char[2048];
		// Add data to SteamPartyBeaconLocation struct
		SteamPartyBeaconLocation_t *beacon = new SteamPartyBeaconLocation_t;
		beacon->m_eType = (ESteamPartyBeaconLocationType)location_type;
		beacon->m_ulLocationID = location_id;
		if(SteamParties()->GetBeaconLocationData(*beacon, (ESteamPartyBeaconLocationData)location_data, (char*)beacon_data, 2048)){
			beacon_location_data = beacon_data;
		}
		delete[] beacon_data;
		delete beacon;
	}
	return beacon_location_data;
}


/////////////////////////////////////////////////
///// REMOTE PLAY 
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
uint64_t Steam::getSessionSteamID(uint32 session_id){
	if(SteamRemotePlay() == NULL){
		return 0;
	}
	CSteamID steam_id = SteamRemotePlay()->GetSessionSteamID(session_id);
	return steam_id.ConvertToUint64();
}

// Get the name of the session client device.
String Steam::getSessionClientName(uint32 session_id){
	if(SteamRemotePlay() == NULL){
		return "";
	}
	return SteamRemotePlay()->GetSessionClientName(session_id);
}

// Get the form factor of the session client device.
int Steam::getSessionClientFormFactor(uint32 session_id){
	if(SteamRemotePlay() == NULL){
		return 0;
	}
	return SteamRemotePlay()->GetSessionClientFormFactor(session_id);	
}

// Get the resolution, in pixels, of the session client device. This is set to 0x0 if the resolution is not available.
Dictionary Steam::getSessionClientResolution(uint32 session_id){
	Dictionary resolution;
	if (SteamNetworking() == NULL) {
		return resolution;
	}
	int resolutionX = 0;
	int resolutionY = 0;
	bool success = SteamRemotePlay()->BGetSessionClientResolution(session_id, &resolutionX, &resolutionY);
	if(success){
		resolution["success"] = success;
		resolution["x"] = resolutionX;
		resolution["y"] = resolutionY;
	}	
	return resolution;
}

// Invite a friend to join the game using Remote Play Together
bool Steam::sendRemotePlayTogetherInvite(uint64_t friend_id){
	if(SteamRemotePlay() == NULL){
		return false;
	}
	CSteamID steam_id = (uint64)friend_id;
	return SteamRemotePlay()->BSendRemotePlayTogetherInvite(steam_id);
}


/////////////////////////////////////////////////
///// REMOTE STORAGE
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
Dictionary Steam::fileRead(const String& file, int32_t data_to_read){
	Dictionary d;
	if(SteamRemoteStorage() == NULL){
		d["ret"] = false;
		return d;
	}
	PoolByteArray data;
	data.resize(data_to_read);
	d["ret"] = SteamRemoteStorage()->FileRead(file.utf8().get_data(), data.write().ptr(), data_to_read);
	d["buf"] = data;
	return d;
}

// Starts an asynchronous read from a file. The offset and amount to read should be valid for the size of the file, as indicated by GetFileSize or GetFileTimestamp.
void Steam::fileReadAsync(const String& file, uint32 offset, uint32_t data_to_read){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t api_call = SteamRemoteStorage()->FileReadAsync(file.utf8().get_data(), offset, data_to_read);
		callResultFileReadAsyncComplete.Set(api_call, this, &Steam::_file_read_async_complete);
	}
}

// Share a file.
void Steam::fileShare(const String& file){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t api_call = SteamRemoteStorage()->FileShare(file.utf8().get_data());
		callResultFileShareResult.Set(api_call, this, &Steam::_file_share_result);
	}
}

// Write to given file from Steam Cloud.
bool Steam::fileWrite(const String& file, const PoolByteArray& data, int32_t data_size){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWrite(file.utf8().get_data(), data.read().ptr(), data_size);
}

// Creates a new file and asynchronously writes the raw byte data to the Steam Cloud, and then closes the file. If the target file already exists, it is overwritten.
void Steam::fileWriteAsync(const String& file, const PoolByteArray& data, int32_t data_size){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t api_call = SteamRemoteStorage()->FileWriteAsync(file.utf8().get_data(), data.read().ptr(), data_size);
		callResultFileWriteAsyncComplete.Set(api_call, this, &Steam::_file_write_async_complete);
	}
}

// Cancels a file write stream that was started by FileWriteStreamOpen.  This trashes all of the data written and closes the write stream, but if there was an existing file with this name, it remains untouched.
bool Steam::fileWriteStreamCancel(uint64_t write_handle){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWriteStreamCancel((UGCFileWriteStreamHandle_t)write_handle);
}

// Closes a file write stream that was started by FileWriteStreamOpen. This flushes the stream to the disk, overwriting the existing file if there was one.
bool Steam::fileWriteStreamClose(uint64_t write_handle){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWriteStreamClose((UGCFileWriteStreamHandle_t)write_handle);
}

// Creates a new file output stream allowing you to stream out data to the Steam Cloud file in chunks. If the target file already exists, it is not overwritten until FileWriteStreamClose has been called. To write data out to this stream you can use FileWriteStreamWriteChunk, and then to close or cancel you use FileWriteStreamClose and FileWriteStreamCancel respectively.
uint64_t Steam::fileWriteStreamOpen(const String& file){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->FileWriteStreamOpen(file.utf8().get_data());
}

// Writes a blob of data to the file write stream.
bool Steam::fileWriteStreamWriteChunk(uint64_t write_handle, PoolByteArray& data, int32_t data_size){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->FileWriteStreamWriteChunk((UGCFileWriteStreamHandle_t)write_handle, data.read().ptr(), data_size);
}

// Gets the number of cached UGC.
int32 Steam::getCachedUGCCount(){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->GetCachedUGCCount();
}

// Gets the cached UGC's handle.
uint64_t Steam::getCachedUGCHandle(int content){
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
Dictionary Steam::getSyncPlatforms(const String& file){
	// Set dictionary to populate
	Dictionary platforms;
	if(SteamRemoteStorage() == NULL){
		return platforms;
	}
	// Get the bitwise platform
	uint32_t platform = SteamRemoteStorage()->GetSyncPlatforms(file.utf8().get_data());
	// Assign this to bitwise in the dictionary
	platforms["bitwise"] = platform;
	// Now assign the verbose platform
	if(platform == 0){
		platforms["verbose"] = "none";
	}
	else if(platform == (1<<0)){
		platforms["verbose"] = "windows";
	}
	else if(platform == (1<<1)){
		platforms["verbose"] = "osx";
	}
	else if(platform == (1<<2)){
		platforms["verbose"] = "playstation 3";
	}
	else if(platform == (1<<3)){
		platforms["verbose"] = "linux / steamos";
	}
	else if(platform == (1<<4)){
		platforms["verbose"] = "reserved";
	}
	else{
		platforms["verbose"] = "all";
	}
	// Return the dictionary
	return platforms;
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

// Allows you to specify which operating systems a file will be synchronized to. Use this if you have a multiplatform game but have data which is incompatible between platforms.
bool Steam::setSyncPlatforms(const String& file, int platform){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->SetSyncPlatforms(file.utf8().get_data(), (ERemoteStoragePlatform)platform);
}

// Downloads a UGC file.  A priority value of 0 will download the file immediately, otherwise it will wait to download the file until all downloads with a lower priority value are completed.  Downloads with equal priority will occur simultaneously.
void Steam::ugcDownload(uint64_t content, uint32 priority){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t api_call = SteamRemoteStorage()->UGCDownload((UGCHandle_t)content, priority);
		callResultDownloadUGCResult.Set(api_call, this, &Steam::_download_ugc_result);
	}
}

// Downloads a UGC file to a specific location.
void Steam::ugcDownloadToLocation(uint64_t content, const String& location, uint32 priority){
	if(SteamRemoteStorage() != NULL){
		SteamAPICall_t api_call = SteamRemoteStorage()->UGCDownloadToLocation((UGCHandle_t)content, location.utf8().get_data(), priority);
		callResultDownloadUGCResult.Set(api_call, this, &Steam::_download_ugc_result);
	}
}

// After download, gets the content of the file. 
PoolByteArray Steam::ugcRead(uint64_t content, int32 data_size, uint32 offset, int action){
	PoolByteArray file_contents;
	file_contents.resize(data_size);
	if(SteamRemoteStorage() != NULL){
		SteamRemoteStorage()->UGCRead((UGCHandle_t)content, file_contents.write().ptr(), data_size, offset, (EUGCReadAction)action);
	}
	return file_contents;
}

// Indicate to Steam the beginning / end of a set of local file operations - for example, writing a game save that requires updating two files.
bool Steam::beginFileWriteBatch(){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->BeginFileWriteBatch();
}

// Indicate to Steam the beginning / end of a set of local file operations - for example, writing a game save that requires updating two files.
bool Steam::endFileWriteBatch(){
	if(SteamRemoteStorage() == NULL){
		return false;
	}
	return SteamRemoteStorage()->EndFileWriteBatch();	
}

// Cloud dynamic state change notification, used to get the total number of files changed; paired with getLocalFileChange
uint32_t Steam::getLocalFileChangeCount(){
	if(SteamRemoteStorage() == NULL){
		return 0;
	}
	return SteamRemoteStorage()->GetLocalFileChangeCount();
}

// Cloud dynamic state change notification, for iteration with getLocalFileChangeCount
Dictionary Steam::getLocalFileChange(int file){
	Dictionary file_change;
	if(SteamRemoteStorage() != NULL){
		ERemoteStorageLocalFileChange change_type;
		ERemoteStorageFilePathType file_path_type;
		String changed_file = SteamRemoteStorage()->GetLocalFileChange(file, &change_type, &file_path_type);
		// Add these to the dictionary
		file_change["file"] = changed_file;
		file_change["change_type"] = change_type;
		file_change["path_type"] = file_path_type;
	}
	return file_change;
}


/////////////////////////////////////////////////
///// SCREENSHOTS
/////////////////////////////////////////////////
//
// Adds a screenshot to the user's Steam screenshot library from disk.
uint32_t Steam::addScreenshotToLibrary(const String& filename, const String& thumbnail_filename, int width, int height){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->AddScreenshotToLibrary(filename.utf8().get_data(), thumbnail_filename.utf8().get_data(), width, height);
}

// Adds a VR screenshot to the user's Steam screenshot library from disk in the supported type.
uint32_t Steam::addVRScreenshotToLibrary(int type, const String& filename, const String& vr_filename){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->AddVRScreenshotToLibrary((EVRScreenshotType)type, filename.utf8().get_data(), vr_filename.utf8().get_data());
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

// Tags a published file as being visible in the screenshot.
bool Steam::tagPublishedFile(uint32 screenshot, uint64_t file_id){
	if(SteamScreenshots() == NULL){
		return false;
	}
	PublishedFileId_t file = (uint64)file_id;
	return SteamScreenshots()->TagPublishedFile((ScreenshotHandle)screenshot, file);
}

// Tags a Steam user as being visible in the screenshot.  You can tag up to the value declared by k_nScreenshotMaxTaggedUsers in a single screenshot. Tagging more users than that will just be discarded.  This function has a built in delay before saving the tag which allows you to call it repeatedly for each item.  You can get the handle to tag the screenshot once it has been successfully saved from the ScreenshotReady_t callback or via the WriteScreenshot, AddScreenshotToLibrary, AddVRScreenshotToLibrary calls.
bool Steam::tagUser(uint32 screenshot, uint64_t steam_id){
	if(SteamScreenshots() == NULL){
		return false;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamScreenshots()->TagUser((ScreenshotHandle)screenshot, user_id);
}

// Causes Steam overlay to take a screenshot.
void Steam::triggerScreenshot(){
	if(SteamScreenshots() != NULL){
		SteamScreenshots()->TriggerScreenshot();
	}
}

// Writes a screenshot to the user's Steam screenshot library.
uint32_t Steam::writeScreenshot(const PoolByteArray& rgb, int width, int height){
	if(SteamScreenshots() == NULL){
		return 0;
	}
	return SteamScreenshots()->WriteScreenshot((void*)rgb.read().ptr(), rgb.size(), width, height);
}


/////////////////////////////////////////////////
///// UGC
/////////////////////////////////////////////////
//
// Adds a dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
// This is a soft-dependency that is displayed on the web. It is up to the application to determine whether the item can actually be used or not.
void Steam::addAppDependency(uint64_t published_file_id, uint32_t app_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->AddAppDependency(file_id, app);
		callResultAddAppDependency.Set(api_call, this, &Steam::_add_app_dependency_result);
	}
}

// Adds a workshop item as a dependency to the specified item. If the nParentPublishedFileID item is of type k_EWorkshopFileTypeCollection, than the nChildPublishedFileID is simply added to that collection.
// Otherwise, the dependency is a soft one that is displayed on the web and can be retrieved via the ISteamUGC API using a combination of the m_unNumChildren member variable of the SteamUGCDetails_t struct and GetQueryUGCChildren.
void Steam::addDependency(uint64_t published_file_id, uint64_t child_published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t parent = (uint64_t)published_file_id;
		PublishedFileId_t child = (uint64_t)child_published_file_id;
		SteamAPICall_t api_call = SteamUGC()->AddDependency(parent, child);
		callResultAddUGCDependency.Set(api_call, this, &Steam::_add_ugc_dependency_result);
	}
}

// Adds a excluded tag to a pending UGC Query. This will only return UGC without the specified tag.
bool Steam::addExcludedTag(uint64_t query_handle, const String& tag_name){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddExcludedTag(handle, tag_name.utf8().get_data());
}

// Adds a key-value tag pair to an item. Keys can map to multiple different values (1-to-many relationship).
bool Steam::addItemKeyValueTag(uint64_t update_handle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = (uint64_t)update_handle;
	return SteamUGC()->AddItemKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

// Adds an additional preview file for the item.
bool Steam::addItemPreviewFile(uint64_t query_handle, const String& preview_file, int type){
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
bool Steam::addItemPreviewVideo(uint64_t query_handle, const String& video_id){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddItemPreviewVideo(handle, video_id.utf8().get_data());
}

// Adds a workshop item to the users favorites list.
void Steam::addItemToFavorites(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() != NULL){
		AppId_t app = (uint32_t)app_id;
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->AddItemToFavorites(app, file_id);
		callResultFavoriteItemListChanged.Set(api_call, this, &Steam::_user_favorite_items_list_changed);
	}
}

// Adds a required key-value tag to a pending UGC Query. This will only return workshop items that have a key = pKey and a value = pValue.
bool Steam::addRequiredKeyValueTag(uint64_t query_handle, const String& key, const String& value){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddRequiredKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

// Adds a required tag to a pending UGC Query. This will only return UGC with the specified tag.
bool Steam::addRequiredTag(uint64_t query_handle, const String& tag_name){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->AddRequiredTag(handle, tag_name.utf8().get_data());
}

// Adds the requirement that the returned items from the pending UGC Query have at least one of the tags in the given set (logical "or"). For each tag group that is added, at least one tag from each group is required to be on the matching items.
bool Steam::addRequiredTagGroup(uint64_t query_handle, Array tag_array){
	bool added_tag_group = false;
	if(SteamUGC() != NULL){
		UGCQueryHandle_t handle = uint64(query_handle);
		SteamParamStringArray_t *tags = new SteamParamStringArray_t();
		tags->m_ppStrings = new const char*[tag_array.size()];
		uint32 strCount = tag_array.size();
		for (uint32 i=0; i < strCount; i++) {
			String str = tag_array[i];
			tags->m_ppStrings[i] = str.utf8().get_data();
		}
		tags->m_nNumStrings = tag_array.size();
		added_tag_group = SteamUGC()->AddRequiredTagGroup(handle, tags);
		delete tags;
	}
	return added_tag_group;
}

// Lets game servers set a specific workshop folder before issuing any UGC commands.
bool Steam::initWorkshopForGameServer(uint32_t workshop_depot_id){
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
void Steam::createItem(AppId_t app_id, int file_type){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->CreateItem(app_id, (EWorkshopFileType)file_type);
		callResultItemCreate.Set(api_call, this, &Steam::_item_created);
	}
}

// Query for all matching UGC. You can use this to list all of the available UGC for your app.
uint64_t Steam::createQueryAllUGCRequest(int query_type, int matching_type, uint32_t creator_id, uint32_t consumer_id, uint32 page){
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

// Query for the details of specific workshop items.
uint64_t Steam::createQueryUGCDetailsRequest(Array published_file_ids){
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
uint64_t Steam::createQueryUserUGCRequest(int account_id, int list_type, int matching_ugc_type, int sort_order, int creator_id, int consumer_id, uint32 page){
	if(SteamUGC() == NULL){
		return 0;
	}
	// Get tue universe ID from the Steam ID
	CSteamID user_id = createSteamID(account_id, 1);
	AccountID_t account = user_id.GetAccountID();
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

// Deletes the item without prompting the user.
void Steam::deleteItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->DeleteItem(file_id);
		callResultDeleteItem.Set(api_call, this, &Steam::_item_deleted);
	}
}

// Download new or update already installed item. If returns true, wait for DownloadItemResult_t. If item is already installed, then files on disk should not be used until callback received.
// If item is not subscribed to, it will be cached for some time. If bHighPriority is set, any other item download will be suspended and this item downloaded ASAP.
bool Steam::downloadItem(uint64_t published_file_id, bool high_priority){
	if(SteamUGC() == NULL){
		return false;
	}
	PublishedFileId_t file_id = (uint64_t)published_file_id;
	return SteamUGC()->DownloadItem(file_id, high_priority);
}

// Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
Dictionary Steam::getItemDownloadInfo(uint64_t published_file_id){
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
Dictionary Steam::getItemInstallInfo(uint64_t published_file_id){
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

// Gets the current state of a workshop item on this client.
int Steam::getItemState(uint64_t published_file_id){
	if(SteamUGC() == NULL){
		return 0;
	}
	PublishedFileId_t file_id = (uint64_t)published_file_id;
	return SteamUGC()->GetItemState(file_id);
}

// Gets the progress of an item update.
Dictionary Steam::getItemUpdateProgress(uint64_t update_handle){
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
uint32 Steam::getNumSubscribedItems(){
	if(SteamUser() == NULL){
		return 0;
	}
	return SteamUGC()->GetNumSubscribedItems();
}

// Retrieve the details of an additional preview associated with an individual workshop item after receiving a querying UGC call result.
Dictionary Steam::getQueryUGCAdditionalPreview(uint64_t query_handle, uint32 index, uint32 preview_index){
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
Dictionary Steam::getQueryUGCChildren(uint64_t query_handle, uint32 index){
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
Dictionary Steam::getQueryUGCKeyValueTag(uint64_t query_handle, uint32 index, uint32 key_value_tag_index){
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
String Steam::getQueryUGCMetadata(uint64_t query_handle, uint32 index){
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
uint32 Steam::getQueryUGCNumAdditionalPreviews(uint64_t query_handle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumAdditionalPreviews(handle, index);
}

// Retrieve the number of key-value tags of an individual workshop item after receiving a querying UGC call result.
uint32 Steam::getQueryUGCNumKeyValueTags(uint64_t query_handle, uint32 index){
	if(SteamUser() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumKeyValueTags(handle, index);
}

// Retrieve the number of tags for an individual workshop item after receiving a querying UGC call result. You should call this in a loop to get the details of all the workshop items returned.
uint32 Steam::getQueryUGCNumTags(uint64_t query_handle, uint32 index){
	if(SteamUGC() == NULL){
		return 0;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->GetQueryUGCNumTags(handle, index);
}

// Retrieve the URL to the preview image of an individual workshop item after receiving a querying UGC call result.
String Steam::getQueryUGCPreviewURL(uint64_t query_handle, uint32 index){
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
Dictionary Steam::getQueryUGCResult(uint64_t query_handle, uint32 index){
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
		ugcResult["creatorAppID"] = (uint64_t)pDetails.m_nCreatorAppID;
		ugcResult["consumerAppID"] = (uint64_t)pDetails.m_nConsumerAppID;
		ugcResult["title"] = String(pDetails.m_rgchTitle);
		ugcResult["description"] = String(pDetails.m_rgchDescription);
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
Dictionary Steam::getQueryUGCStatistic(uint64_t query_handle, uint32 index, int stat_type){
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
String Steam::getQueryUGCTag(uint64_t query_handle, uint32 index, uint32 tag_index){
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
String Steam::getQueryUGCTagDisplayName(uint64_t query_handle, uint32 index, uint32 tag_index){
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
	delete[] items;
	return subscribed;
}

// Gets the users vote status on a workshop item.
void Steam::getUserItemVote(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->GetUserItemVote(file_id);
		callResultGetUserItemVote.Set(api_call, this, &Steam::_get_item_vote_result);
	}
}

// Releases a UGC query handle when you are done with it to free up memory.
bool Steam::releaseQueryUGCRequest(uint64_t query_handle){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->ReleaseQueryUGCRequest(handle);
}

// Removes the dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
void Steam::removeAppDependency(uint64_t published_file_id, uint32_t app_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveAppDependency(file_id, app);
		callResultRemoveAppDependency.Set(api_call, this, &Steam::_remove_app_dependency_result);
	}
}

// Removes a workshop item as a dependency from the specified item.
void Steam::removeDependency(uint64_t published_file_id, uint64_t child_published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		PublishedFileId_t childID = (uint64_t)child_published_file_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveDependency(file_id, childID);
		callResultRemoveUGCDependency.Set(api_call, this, &Steam::_remove_ugc_dependency_result);
	}
}

// Removes a workshop item from the users favorites list.
void Steam::removeItemFromFavorites(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		AppId_t app = (uint32_t)app_id;
		SteamAPICall_t api_call = SteamUGC()->RemoveItemFromFavorites(app, file_id);
		callResultFavoriteItemListChanged.Set(api_call, this, &Steam::_user_favorite_items_list_changed);
	}
}

// Removes an existing key value tag from an item.
bool Steam::removeItemKeyValueTags(uint64_t update_handle, const String& key){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->RemoveItemKeyValueTags(handle, key.utf8().get_data());
}

// Removes an existing preview from an item.
bool Steam::removeItemPreview(uint64_t update_handle, uint32 index){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->RemoveItemPreview(handle, index);
}

// Send a UGC query to Steam.
void Steam::sendQueryUGCRequest(uint64_t update_handle){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamAPICall_t api_call = SteamUGC()->SendQueryUGCRequest(handle);
		callResultUGCQueryCompleted.Set(api_call, this, &Steam::_ugc_query_completed);
	}
}

// Sets whether results will be returned from the cache for the specific period of time on a pending UGC Query.
bool Steam::setAllowCachedResponse(uint64_t update_handle, uint32 max_age_seconds){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetAllowCachedResponse(handle, max_age_seconds);
}

// Sets to only return items that have a specific filename on a pending UGC Query.
bool Steam::setCloudFileNameFilter(uint64_t update_handle, const String& match_cloud_filename){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetCloudFileNameFilter(handle, match_cloud_filename.utf8().get_data());
}

// Sets the folder that will be stored as the content for an item.
bool Steam::setItemContent(uint64_t update_handle, const String& content_folder){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemContent(handle, content_folder.utf8().get_data());
}

// Sets a new description for an item.
bool Steam::setItemDescription(uint64_t update_handle, const String& description){
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

// Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
bool Steam::setItemMetadata(uint64_t update_handle, const String& metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	if (metadata.utf8().length() > 5000){
		printf("Metadata cannot be more than %d bytes. Metadata not set.", 5000);
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemMetadata(handle, metadata.utf8().get_data());
}

// Sets the primary preview image for the item.
bool Steam::setItemPreview(uint64_t update_handle, const String& preview_file){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemPreview(handle, preview_file.utf8().get_data());
}

// Sets arbitrary developer specified tags on an item.
bool Steam::setItemTags(uint64_t update_handle, Array tag_array){
	bool tags_set = false;
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamParamStringArray_t *tags = new SteamParamStringArray_t();
		tags->m_ppStrings = new const char*[tag_array.size()];
		uint32 strCount = tag_array.size();
		for (uint32 i=0; i < strCount; i++) {
			String str = tag_array[i];
			tags->m_ppStrings[i] = str.utf8().get_data();
		}
		tags->m_nNumStrings = tag_array.size();
		tags_set = SteamUGC()->SetItemTags(handle, tags);
		delete tags;
	}
	return tags_set;
}

// Sets a new title for an item.
bool Steam::setItemTitle(uint64_t update_handle, const String& title){
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
bool Steam::setItemUpdateLanguage(uint64_t update_handle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}

// Sets the visibility of an item.
bool Steam::setItemVisibility(uint64_t update_handle, int visibility){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->SetItemVisibility(handle, (ERemoteStoragePublishedFileVisibility)visibility);
}

// Sets the language to return the title and description in for the items on a pending UGC Query.
bool Steam::setLanguage(uint64_t query_handle, const String& language){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetLanguage(handle, language.utf8().get_data());
}

// Sets whether workshop items will be returned if they have one or more matching tag, or if all tags need to match on a pending UGC Query.
bool Steam::setMatchAnyTag(uint64_t query_handle, bool match_any_tag){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetMatchAnyTag(handle, match_any_tag);
}

// Sets whether the order of the results will be updated based on the rank of items over a number of days on a pending UGC Query.
bool Steam::setRankedByTrendDays(uint64_t query_handle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetRankedByTrendDays(handle, days);
}

// Sets whether to return any additional images/videos attached to the items on a pending UGC Query.
bool Steam::setReturnAdditionalPreviews(uint64_t query_handle, bool return_additional_previews){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnAdditionalPreviews(handle, return_additional_previews);
}

// Sets whether to return the IDs of the child items of the items on a pending UGC Query.
bool Steam::setReturnChildren(uint64_t query_handle, bool return_children){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnChildren(handle, return_children);
}

// Sets whether to return any key-value tags for the items on a pending UGC Query.
bool Steam::setReturnKeyValueTags(uint64_t query_handle, bool return_key_value_tags){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnKeyValueTags(handle, return_key_value_tags);
}

// Sets whether to return the full description for the items on a pending UGC Query.
bool Steam::setReturnLongDescription(uint64_t query_handle, bool return_long_description){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnLongDescription(handle, return_long_description);
}

// Sets whether to return the developer specified metadata for the items on a pending UGC Query.
bool Steam::setReturnMetadata(uint64_t query_handle, bool return_metadata){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnMetadata(handle, return_metadata);
}

// Sets whether to only return IDs instead of all the details on a pending UGC Query.
bool Steam::setReturnOnlyIDs(uint64_t query_handle, bool return_only_ids){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnOnlyIDs(handle, return_only_ids);
}

// Sets whether to return the the playtime stats on a pending UGC Query.
bool Steam::setReturnPlaytimeStats(uint64_t query_handle, uint32 days){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnPlaytimeStats(handle, days);
}

// Sets whether to only return the the total number of matching items on a pending UGC Query.
bool Steam::setReturnTotalOnly(uint64_t query_handle, bool return_total_only){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetReturnTotalOnly(handle, return_total_only);
}

// Sets a string to that items need to match in either the title or the description on a pending UGC Query.
bool Steam::setSearchText(uint64_t query_handle, const String& search_text){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCQueryHandle_t handle = (uint64_t)query_handle;
	return SteamUGC()->SetSearchText(handle, search_text.utf8().get_data());
}

// Allows the user to rate a workshop item up or down.
void Steam::setUserItemVote(uint64_t published_file_id, bool vote_up){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->SetUserItemVote(file_id, vote_up);
		callResultSetUserItemVote.Set(api_call, this, &Steam::_set_user_item_vote);
	}
}

// Starts the item update process.
uint64_t Steam::startItemUpdate(uint32_t app_id, uint64_t published_file_id){
	if(SteamUGC() == NULL){
		return 0;
	}
	AppId_t app = (uint32_t)app_id;
	PublishedFileId_t file_id = (uint64_t)published_file_id;
	return SteamUGC()->StartItemUpdate(app, file_id);
}

// Start tracking playtime on a set of workshop items.
void Steam::startPlaytimeTracking(Array published_file_ids){
	if(SteamUGC() != NULL){
		uint32 fileCount = published_file_ids.size();
		if(fileCount > 0){
			PublishedFileId_t *file_ids = new PublishedFileId_t[fileCount];
			for(uint32 i = 0; i < fileCount; i++){
				file_ids[i] = (uint64_t)published_file_ids[i];
			}
			SteamAPICall_t api_call = SteamUGC()->StartPlaytimeTracking(file_ids, fileCount);
			callResultStartPlaytimeTracking.Set(api_call, this, &Steam::_start_playtime_tracking);
			delete[] file_ids;
		}
	}
}

// Stop tracking playtime on a set of workshop items.
void Steam::stopPlaytimeTracking(Array published_file_ids){
	if(SteamUGC() != NULL){
		uint32 fileCount = published_file_ids.size();
		if(fileCount > 0){
			PublishedFileId_t *file_ids = new PublishedFileId_t[fileCount];
			Array files;
			for(uint32 i = 0; i < fileCount; i++){
				file_ids[i] = (uint64_t)published_file_ids[i];
			}
			SteamAPICall_t api_call = SteamUGC()->StopPlaytimeTracking(file_ids, fileCount);
			callResultStopPlaytimeTracking.Set(api_call, this, &Steam::_stop_playtime_tracking);
			delete[] file_ids;
		}
	}
}

// Stop tracking playtime of all workshop items.
void Steam::stopPlaytimeTrackingForAllItems(){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->StopPlaytimeTrackingForAllItems();
		callResultStopPlaytimeTracking.Set(api_call, this, &Steam::_stop_playtime_tracking);
	}
}

// Returns any app dependencies that are associated with the given item.
void Steam::getAppDependencies(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->GetAppDependencies(file_id);
		callResultGetAppDependencies.Set(api_call, this, &Steam::_get_app_dependencies_result);
	}
}

// Uploads the changes made to an item to the Steam Workshop; to be called after setting your changes.
void Steam::submitItemUpdate(uint64_t update_handle, const String& change_note){
	if(SteamUGC() != NULL){
		UGCUpdateHandle_t handle = uint64(update_handle);
		SteamAPICall_t api_call = SteamUGC()->SubmitItemUpdate(handle, change_note.utf8().get_data());
		callResultItemUpdate.Set(api_call, this, &Steam::_item_updated);
	}
}

// Subscribe to a workshop item. It will be downloaded and installed as soon as possible.
void Steam::subscribeItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->SubscribeItem(file_id);
		callResultSubscribeItem.Set(api_call, this, &Steam::_subscribe_item);
	}
}

// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends.
void Steam::suspendDownloads(bool suspend){
	if(SteamUGC() != NULL){
		SteamUGC()->SuspendDownloads(suspend);
	}
}

// Unsubscribe from a workshop item. This will result in the item being removed after the game quits.
void Steam::unsubscribeItem(uint64_t published_file_id){
	if(SteamUGC() != NULL){
		PublishedFileId_t file_id = (uint64_t)published_file_id;
		SteamAPICall_t api_call = SteamUGC()->UnsubscribeItem(file_id);
		callResultUnsubscribeItem.Set(api_call, this, &Steam::_unsubscribe_item);
	}
}

// Updates an existing additional preview file for the item.
bool Steam::updateItemPreviewFile(uint64_t update_handle, uint32 index, const String& preview_file){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->UpdateItemPreviewFile(handle, index, preview_file.utf8().get_data());
}

// Updates an additional video preview from YouTube for the item.
bool Steam::updateItemPreviewVideo(uint64_t update_handle, uint32 index, const String& video_id){
	if(SteamUGC() == NULL){
		return false;
	}
	UGCUpdateHandle_t handle = uint64(update_handle);
	return SteamUGC()->UpdateItemPreviewVideo(handle, index, video_id.utf8().get_data());
}

// Show the app's latest Workshop EULA to the user in an overlay window, where they can accept it or not.
bool Steam::showWorkshopEULA(){
	if(SteamUGC() == NULL){
		return false;
	}
	return SteamUGC()->ShowWorkshopEULA();
}

// Retrieve information related to the user's acceptance or not of the app's specific Workshop EULA.
void Steam::getWorkshopEULAStatus(){
	if(SteamUGC() != NULL){
		SteamAPICall_t api_call = SteamUGC()->GetWorkshopEULAStatus();
		callResultWorkshopEULAStatus.Set(api_call, this, &Steam::_workshop_eula_status);
	}
}


/////////////////////////////////////////////////
///// USERS
/////////////////////////////////////////////////
//
// Set the rich presence data for an unsecured game server that the user is playing on. This allows friends to be able to view the game info and join your game.
//void Steam::advertiseGame(const String& server_ip, int port){
//	if(SteamUser() != NULL){
//		// Resolve address and convert it from IP_Address struct to uint32_t
//		IP_Address address;
//		if(server_ip.is_valid_ip_address()){
//			address = server_ip;
//		}
//		else{
//			address = IP::get_singleton()->resolve_hostname(server_ip, IP::TYPE_IPV4);
//		}
//		// Resolution failed - Godot 3.0 has is_invalid() to check this
//		if(address != IP_Address()){
//			uint32_t ip4 = *((uint32_t *)address.get_ipv4());
//			// Swap the bytes
//			uint8_t *ip4_p = (uint8_t *)&ip4;
//			for(int i = 0; i < 2; i++){
//				uint8_t temp = ip4_p[i];
//				ip4_p[i] = ip4_p[3-i];
///				ip4_p[3-i] = temp;
//			}
//			CSteamID gameserverID = SteamUser()->GetSteamID();
//			SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
//		}
//	}
//}

// Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused.
int Steam::beginAuthSession(PoolByteArray ticket, int ticket_size, uint64_t steam_id){
	if(SteamUser() == NULL){
		return -1;
	}
	CSteamID authSteamID = (uint64)steam_id;
	return SteamUser()->BeginAuthSession(ticket.read().ptr(), ticket_size, authSteamID);
}

// Cancels an auth ticket.
void Steam::cancelAuthTicket(uint32_t auth_ticket){
	if(SteamUser() != NULL){
		SteamUser()->CancelAuthTicket(auth_ticket);
	}
}

// Decodes the compressed voice data returned by GetVoice.
Dictionary Steam::decompressVoice(const PoolByteArray& voice, uint32 voice_size, uint32 sample_rate){
	Dictionary decompressed;
	if(SteamUser() != NULL){
		uint32 written = 0;
		PoolByteArray outputBuffer;
		int result = SteamUser()->DecompressVoice(voice.read().ptr(), voice_size, &outputBuffer, sizeof(outputBuffer), &written, sample_rate);
		if(result == 0){
			decompressed["uncompressed"] = outputBuffer;
			decompressed["size"] = written;
		}
	}
	return decompressed;
}

// Ends an auth session.
void Steam::endAuthSession(uint64_t steam_id){
	if(SteamUser() != NULL){
		CSteamID authSteamID = (uint64)steam_id;
		SteamUser()->EndAuthSession(authSteamID);
	}
}

// Get the authentication ticket data.
Dictionary Steam::getAuthSessionTicket(){
	// Create the dictionary to use
	Dictionary auth_ticket;
	if(SteamUser() != NULL){
		uint32_t ticket_size = 1024;
		PoolByteArray buffer;
		buffer.resize(ticket_size);
		uint32_t id = SteamUser()->GetAuthSessionTicket(buffer.write().ptr(), ticket_size, &ticket_size);
		// Add this data to the dictionary
		auth_ticket["id"] = id;
		auth_ticket["buffer"] = buffer;
		auth_ticket["size"] = ticket_size;
	}
	return auth_ticket;
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
		SteamAPICall_t api_call = SteamUser()->GetDurationControl();
		callResultDurationControl.Set(api_call, this, &Steam::_duration_control);
	}
}

// Retrieve an encrypted ticket. This should be called after requesting an encrypted app ticket with RequestEncryptedAppTicket and receiving the EncryptedAppTicketResponse_t call result.
Dictionary Steam::getEncryptedAppTicket(){
	Dictionary encrypted;
	if(SteamUser() != NULL){
		uint32_t ticket_size = 1024;
		PoolByteArray buffer;
		buffer.resize(ticket_size);
		if(SteamUser()->GetEncryptedAppTicket(buffer.write().ptr(), ticket_size, &ticket_size)){
			encrypted["buffer"] = buffer;
			encrypted["size"] = ticket_size;
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
	CSteamID steam_id = SteamUser()->GetSteamID();
	return steam_id.ConvertToUint64();
}

// Get the user's Steam installation path (this function is depreciated).
String Steam::getUserDataFolder(){
	if(SteamUser() == NULL){
		return "";
	}
	const int buffer_size = 256;
	char *buffer = new char[buffer_size];
	SteamUser()->GetUserDataFolder((char*)buffer, buffer_size);
	String data_path = buffer;
	delete[] buffer;
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
Dictionary Steam::initiateGameConnection(uint64_t server_id, uint32 server_ip, uint16 server_port, bool secure){
	Dictionary connection;
	if(SteamUser() != NULL){
		PoolByteArray auth;
		int authSize = 2048;
		auth.resize(authSize);
		CSteamID server = (uint64)server_id;
		if(SteamUser()->InitiateGameConnection_DEPRECATED(&auth, authSize, server, server_ip, server_port, secure) > 0){
			connection["auth_blob"] = auth;
			connection["server_id"] = server_id;
			connection["server_ip"] = server_ip;
			connection["server_port"] = server_port;
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
		SteamAPICall_t api_call = SteamUser()->RequestEncryptedAppTicket((void*)secret.utf8().get_data(), sizeof(secret));
		callResultEncryptedAppTicketResponse.Set(api_call, this, &Steam::_encrypted_app_ticket_response);
	}
}

// Requests a URL which authenticates an in-game browser for store check-out, and then redirects to the specified URL.
void Steam::requestStoreAuthURL(const String& redirect){
	if(SteamUser() != NULL){
		SteamAPICall_t api_call = SteamUser()->RequestStoreAuthURL(redirect.utf8().get_data());
		callResultStoreAuthURLResponse.Set(api_call, this, &Steam::_store_auth_url_response);
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
void Steam::terminateGameConnection(uint32 server_ip, uint16 server_port){
	if(SteamUser() != NULL){
		SteamUser()->TerminateGameConnection_DEPRECATED(server_ip, server_port);
	}
}

// Checks if the user owns a specific piece of Downloadable Content (DLC). This can only be called after sending the users auth ticket to ISteamGameServer::BeginAuthSession.
int Steam::userHasLicenseForApp(uint64_t steam_id, uint32_t app_id){
	if(SteamUser() == NULL){
		return 2;
	}
	CSteamID user_id = (uint64)steam_id;
	return SteamUser()->UserHasLicenseForApp(user_id, (AppId_t)app_id);
}


/////////////////////////////////////////////////
///// USER STATS
/////////////////////////////////////////////////
//
// Attaches a piece of user generated content the current user's entry on a leaderboard.
void Steam::attachLeaderboardUGC(uint64_t ugcHandle, uint64_t this_leaderboard){
	if(SteamUserStats() != NULL){
		// If no leaderboard is passed, use internal one
		if(this_leaderboard == 0){
			this_leaderboard = leaderboard_handle;
		}
		SteamAPICall_t api_call = SteamUserStats()->AttachLeaderboardUGC((SteamLeaderboard_t)this_leaderboard, (UGCHandle_t)ugcHandle);
		callResultLeaderboardUGCSet.Set(api_call, this, &Steam::_leaderboard_ugc_set);
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
void Steam::downloadLeaderboardEntries(int start, int end, int type, uint64_t this_leaderboard){
	if(SteamUserStats() != NULL){
		// If no leaderboard is passed, use internal one
		if(this_leaderboard == 0){
			this_leaderboard = leaderboard_handle;
		}
		SteamAPICall_t api_call = SteamUserStats()->DownloadLeaderboardEntries((SteamLeaderboard_t)this_leaderboard, ELeaderboardDataRequest(type), start, end);
		callResultEntries.Set(api_call, this, &Steam::_leaderboard_scores_downloaded);
	}
}

// Request a maximum of 100 users with only one outstanding call at a time.
void Steam::downloadLeaderboardEntriesForUsers(Array users_id, uint64_t this_leaderboard){
	if(SteamUserStats() != NULL){
		// If no leaderboard is passed, use internal one
		if(this_leaderboard == 0){
			this_leaderboard = leaderboard_handle;
		}
		int usersCount = users_id.size();
		if(usersCount > 0){
			CSteamID *users = new CSteamID[usersCount];
			for(int i = 0; i < usersCount; i++){
				CSteamID user = (uint64)(users_id[i], 1);
				users[i] = user;
			}
			SteamAPICall_t api_call = SteamUserStats()->DownloadLeaderboardEntriesForUsers((SteamLeaderboard_t)this_leaderboard, users, usersCount);
			callResultEntries.Set(api_call, this, &Steam::_leaderboard_scores_downloaded);
			delete[] users;
		}
	}
}

// Find a given leaderboard, by name.
void Steam::findLeaderboard(const String& name){
	if(SteamUserStats() != NULL){
		SteamAPICall_t api_call = SteamUserStats()->FindLeaderboard(name.utf8().get_data());
		callResultFindLeaderboard.Set(api_call, this, &Steam::_leaderboard_find_result);
	}
}

// Gets a leaderboard by name, it will create it if it's not yet created.
void Steam::findOrCreateLeaderboard(const String& name, int sort_method, int display_type){
	if(SteamUserStats() != NULL){
		SteamAPICall_t api_call = SteamUserStats()->FindOrCreateLeaderboard(name.utf8().get_data(), (ELeaderboardSortMethod)sort_method, (ELeaderboardDisplayType)display_type);
		callResultFindLeaderboard.Set(api_call, this, &Steam::_leaderboard_find_result);
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

// Gets the icon for an achievement
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
uint64_t Steam::getGlobalStatInt(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	int64 stat = 0;
	SteamUserStats()->GetGlobalStat(name.utf8().get_data(), &stat);
	return (uint64_t)stat;
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
uint64_t Steam::getGlobalStatIntHistory(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	int64 history = 0;
	SteamUserStats()->GetGlobalStatHistory(name.utf8().get_data(), &history, 60);
	return (uint64_t)history;
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
Dictionary Steam::getLeaderboardDisplayType(uint64_t this_leaderboard){
	// Create a diciontary to return
	Dictionary display;
	if(SteamUserStats() != NULL){
		// If no leaderboard is passed, use internal one
		if(this_leaderboard == 0){
			this_leaderboard = leaderboard_handle;
		}
		int display_type = SteamUserStats()->GetLeaderboardDisplayType((SteamLeaderboard_t)this_leaderboard);
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
int Steam::getLeaderboardEntryCount(uint64_t this_leaderboard){
	if(SteamUserStats() == NULL){
		return -1;
	}
	// If no leaderboard is passed, use internal one
	if(this_leaderboard == 0){
		this_leaderboard = leaderboard_handle;
	}
	return SteamUserStats()->GetLeaderboardEntryCount((SteamLeaderboard_t)this_leaderboard);
}

// Get the name of a leaderboard.
String Steam::getLeaderboardName(uint64_t this_leaderboard){
	if(SteamUserStats() == NULL){
		return "";
	}
	// If no leaderboard is passed, use internal one
	if(this_leaderboard == 0){
		this_leaderboard = leaderboard_handle;
	}
	return SteamUserStats()->GetLeaderboardName((SteamLeaderboard_t)this_leaderboard);
}

// Returns the sort order of a leaderboard handle.
Dictionary Steam::getLeaderboardSortMethod(uint64_t this_leaderboard){
	// Create a dictionary to return
	Dictionary sort;
	if(SteamUserStats() != NULL){
		// If no leaderboard is passed, use internal one
		if(this_leaderboard == 0){
			this_leaderboard = leaderboard_handle;
		}
		// Get the sort method int	
		int sort_method = SteamUserStats()->GetLeaderboardSortMethod((SteamLeaderboard_t)this_leaderboard);
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
Dictionary Steam::getMostAchievedAchievementInfo(){
	Dictionary entry;
	if(SteamUserStats() == NULL){
		return entry;
	}
	char *name = new char[64];
	float percent = 0;
	bool achieved = false;
	// Get the data from Steam
	int result = SteamUserStats()->GetMostAchievedAchievementInfo(name, 64, &percent, &achieved);
	if(result > -1){
		entry["rank"] = result;
		entry["name"] = name;
		entry["percent"] = percent;
		entry["achieved"] = achieved;
	}
	delete[] name;
	return entry;
}

// Gets the info on the next most achieved achievement for the game.
Dictionary Steam::getNextMostAchievedAchievementInfo(int iterator){
	Dictionary entry;
	if(SteamUserStats() == NULL){
		return entry;
	}
	char *name = new char[64];
	float percent = 0;
	bool achieved = false;
	// Get the data from Steam
	int result = SteamUserStats()->GetNextMostAchievedAchievementInfo(iterator, name, 64, &percent, &achieved);
	if(result > -1){
		entry["rank"] = result;
		entry["name"] = name;
		entry["percent"] = percent;
		entry["achieved"] = achieved;
	}
	delete[] name;
	return entry;
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
		SteamAPICall_t api_call = SteamUserStats()->GetNumberOfCurrentPlayers();
		callResultNumberOfCurrentPlayers.Set(api_call, this, &Steam::_number_of_current_players);
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
Dictionary Steam::getUserAchievement(uint64_t steam_id, const String& name){
	Dictionary achieve;
	if(SteamUserStats() == NULL){
		return achieve;
	}
	bool achieved = false;
	CSteamID user_id = (uint64)steam_id;
	// Get the data from Steam
	bool success = SteamUserStats()->GetUserAchievement(user_id, name.utf8().get_data(), &achieved);
	if(success){
		achieve["steam_id"] = steam_id;
		achieve["retrieved"] = success;
		achieve["name"] = name;
		achieve["achieved"] = achieved;
	}
	return achieve;
}

// Gets the achievement status, and the time it was unlocked if unlocked.
Dictionary Steam::getUserAchievementAndUnlockTime(uint64_t steam_id, const String& name){
	Dictionary achieve;
		if(SteamUserStats() == NULL){
		return achieve;
	}
	bool achieved = false;
	uint32 unlocked = 0;
	CSteamID user_id = (uint64)steam_id;
	// Get the data from Steam
	bool success = SteamUserStats()->GetUserAchievementAndUnlockTime(user_id, name.utf8().get_data(), &achieved, &unlocked);
	if(success){
		achieve["retrieved"] = success;
		achieve["name"] = name;
		achieve["achieved"] = achieved;
		achieve["unlocked"] = unlocked;
	}
	return achieve;
}

// Gets the current value of a float stat for the specified user.
float Steam::getUserStatFloat(uint64_t steam_id, const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	float statValue = 0;
	CSteamID user_id = (uint64)steam_id;
	SteamUserStats()->GetUserStat(user_id, name.utf8().get_data(), &statValue);
	return statValue;
}

// Gets the current value of an integer stat for the specified user.
int Steam::getUserStatInt(uint64_t steam_id, const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	int32_t statValue = 0;
	CSteamID user_id = (uint64)steam_id;
	SteamUserStats()->GetUserStat(user_id, name.utf8().get_data(), &statValue);
	return statValue;
}

// Achievement progress, triggers an AchievementProgress callback, that is all.
// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
bool Steam::indicateAchievementProgress(const String& name, int current_progress, int max_progress){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->IndicateAchievementProgress(name.utf8().get_data(), current_progress, max_progress);
}

// Request all statistics and achievements from Steam servers.
bool Steam::requestCurrentStats(){
	if(SteamUserStats() == NULL){
		return false;
	}
	// If the user isn't logged in, you can't get stats
	if(!loggedOn()){
		return false;
	}
	return SteamUserStats()->RequestCurrentStats();
}

// Asynchronously fetch the data for the percentages.
void Steam::requestGlobalAchievementPercentages(){
	if(SteamUserStats() != NULL){
		SteamAPICall_t api_call = SteamUserStats()->RequestGlobalAchievementPercentages();
		callResultGlobalAchievementPercentagesReady.Set(api_call, this, &Steam::_global_achievement_percentages_ready);
	}
}

// Asynchronously fetches global stats data, which is available for stats marked as "aggregated" in the App Admin panel of the Steamworks website.  The limit is 60.
void Steam::requestGlobalStats(int history_days){
	if(SteamUserStats() != NULL){
		SteamAPICall_t api_call = SteamUserStats()->RequestGlobalStats(history_days);
		callResultGetGlobalStatsReceived.Set(api_call, this, &Steam::_global_stats_received);
	}
}

// Asynchronously downloads stats and achievements for the specified user from the server.
void Steam::requestUserStats(uint64_t steam_id){
	if(SteamUserStats() != NULL){
		CSteamID user_id = (uint64)steam_id;
		SteamAPICall_t api_call = SteamUserStats()->RequestUserStats(user_id);
		callResultUserStatsReceived.Set(api_call, this, &Steam::_user_stats_received);
	}
}

// Reset all Steam statistics; optional to reset achievements.
bool Steam::resetAllStats(bool achievements_too){
	return SteamUserStats()->ResetAllStats(achievements_too);
}

// Set a given achievement.
bool Steam::setAchievement(const String& name){
	if(SteamUserStats() == NULL){
		return 0;
	}
	return SteamUserStats()->SetAchievement(name.utf8().get_data());
}

// Set the maximum number of details to return for leaderboard entries
int Steam::setLeaderboardDetailsMax(int max){
	// If the user submitted too high of a maximum, set to the real max
	if(max > k_cLeaderboardDetailsMax){
		max = k_cLeaderboardDetailsMax;
	}
	// If the user submitted too low of a maximum, set it to 0
	if(max < 0){
		max = 0;
	}
	// Now set the internal variable
	leaderboard_details_max = max;
	return leaderboard_details_max;
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
	return SteamUserStats()->StoreStats();
}

// Updates an AVGRATE stat with new values.
bool Steam::updateAvgRateStat(const String& name, float this_session, double session_length){
	if(SteamUserStats() == NULL){
		return false;
	}
	return SteamUserStats()->UpdateAvgRateStat(name.utf8().get_data(), this_session, session_length);
}

// Upload a leaderboard score for the user.
void Steam::uploadLeaderboardScore(int score, bool keep_best, PoolIntArray details, uint64_t this_leaderboard){
	if(SteamUserStats() != NULL){
		// If no leaderboard is passed, use internal one
		if(this_leaderboard == 0){
			this_leaderboard = leaderboard_handle;
		}
		ELeaderboardUploadScoreMethod method = keep_best ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
		int detailsSize = details.size();
		const int32 *detailsPointer = NULL;
		if(detailsSize > 0){
			PoolIntArray::Read r = details.read();
			detailsPointer = r.ptr();
		}
		SteamAPICall_t api_call = SteamUserStats()->UploadLeaderboardScore((SteamLeaderboard_t)this_leaderboard, method, (int32)score, detailsPointer, detailsSize);
		callResultUploadScore.Set(api_call, this, &Steam::_leaderboard_score_uploaded);
	}
}

// Get the currently used leaderboard entries.
Array Steam::getLeaderboardEntries(){
	if(SteamUserStats() == NULL){
		return Array();
	}
	return leaderboard_entries_array;
}


/////////////////////////////////////////////////
///// UTILS
/////////////////////////////////////////////////
//
// Filters the provided input message and places the filtered result into pchOutFilteredText.
String Steam::filterText(int context, uint64_t steam_id, const String& message){
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
String Steam::getAPICallFailureReason(){
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
		bool valid = SteamUtils()->IsAPICallCompleted(api_handle, &failed);
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

// Returns true if currently running on the Steam Deck device
bool Steam::isSteamRunningOnSteamDeck(){
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->IsSteamRunningOnSteamDeck();
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
	if((pos >= 0) && (pos < 4) && (SteamUtils() != NULL)){
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
bool Steam::showGamepadTextInput(int input_mode, int line_input_mode, const String& description, uint32 max_text, const String& preset_text){
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

// Opens a floating keyboard over the game content and sends OS keyboard keys directly to the game.
// The text field position is specified in pixels relative the origin of the game window and is used to position the floating keyboard in a way that doesn't cover the text field
bool Steam::showFloatingGamepadTextInput(int input_mode, int text_field_x_position, int text_field_y_position, int text_field_width, int text_field_height) {
	if(SteamUtils() == NULL){
		return false;
	}
	return SteamUtils()->ShowFloatingGamepadTextInput((EFloatingGamepadTextInputMode)input_mode, text_field_x_position, text_field_y_position, text_field_width, text_field_height);
}

// In game launchers that don't have controller support you can call this to have Steam Input translate the controller input into mouse/kb to navigate the launcher
void Steam::setGameLauncherMode(bool mode) {
	if(SteamUtils() == NULL){
		return;
	}
	SteamUtils()->SetGameLauncherMode(mode);
}

// Ask SteamUI to create and render its OpenVR dashboard.
void Steam::startVRDashboard(){
	if(SteamUtils() != NULL){
		SteamUtils()->StartVRDashboard();
	}
}


/////////////////////////////////////////////////
///// VIDEO
/////////////////////////////////////////////////
//
// Get the OPF details for 360 video playback.
void Steam::getOPFSettings(uint32_t app_id){
	if(SteamVideo() != NULL){
		SteamVideo()->GetOPFSettings((AppId_t)app_id);
	}
}

// Gets the OPF string for the specified video App ID.
String Steam::getOPFStringForApp(uint32_t app_id){
	String opf_string = "";
	if(SteamVideo() != NULL){
		int32 size = 48000;
		char *buffer = new char[size];
		if(SteamVideo()->GetOPFStringForApp((AppId_t)app_id, buffer, &size)){
			opf_string = buffer;
		}
		delete[] buffer;
	}
	return opf_string;
}

// Asynchronously gets the URL suitable for streaming the video associated with the specified video app ID.
void Steam::getVideoURL(uint32_t app_id){
	if(SteamVideo() != NULL){
		SteamVideo()->GetVideoURL((AppId_t)app_id);
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
///// SIGNALS / CALLBACKS
/////////////////////////////////////////////////
//
// APPS CALLBACKS ///////////////////////////////
//
// Triggered after the current user gains ownership of DLC and that DLC is installed.
void Steam::_dlc_installed(DlcInstalled_t* call_data){
	uint32_t app_id = (AppId_t)call_data->m_nAppID;
	emit_signal("dlc_installed", app_id);
}

// Called after requesting the details of a specific file.
void Steam::_file_details_result(FileDetailsResult_t* file_data){
	uint32_t result = file_data->m_eResult;
	uint64_t fileSize = file_data->m_ulFileSize;
	uint32_t flags = file_data->m_unFlags;
	PoolByteArray fileHash;
	fileHash.resize(20);
	PoolByteArray::Write w = fileHash.write();
	for (int i=0; i<20; i++){
		w[i] = file_data->m_FileSHA[i];
	}
	emit_signal("file_details_result", result, fileSize, fileHash, flags);
}

// Posted after the user executes a steam url with command line or query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running. The new params can be queried with getLaunchCommandLine and getLaunchQueryParam.
void Steam::_new_launch_url_parameters(NewUrlLaunchParameters_t* call_data){
	emit_signal("new_launch_url_parameters");
}

// Posted after the user executes a steam url with query parameters such as steam://run/<appid>//?param1=value1;param2=value2;param3=value3; while the game is already running. The new params can be queried with getLaunchQueryParam.
// ISSUE: when compiling says it is an undeclared identifier
// void Steam::_new_launch_query_parameters(NewLaunchQueryParameters_t* call_data){
//	emit_signal("new_launch_query_parameters");
//}

// Purpose: response to RegisterActivationCode()
void Steam::_register_activation_code_response(RegisterActivationCodeResponse_t* call_data){
	int result = call_data->m_eResult;
	uint32 package_registered = call_data->m_unPackageRegistered;
	emit_signal("register_activation_code_response", result, package_registered);
}

// Purpose: response to RequestAppProofOfPurchaseKey/RequestAllProofOfPurchaseKeys for supporting third-party CD keys, or other proof-of-purchase systems.
void Steam::_app_proof_of_purchase_key_response(AppProofOfPurchaseKeyResponse_t* call_data){
	int result = call_data->m_eResult;
	uint32 app_id = call_data->m_nAppID;
	uint32 key_length = call_data->m_cchKeyLength;
	char key = call_data->m_rgchKey[k_cubAppProofOfPurchaseKeyMax];
	emit_signal("app_proof_of_purchase_key_response", result, app_id, key_length, key);
}

// Purpose: called for games in Timed Trial mode
void Steam::_timed_trial_status(TimedTrialStatus_t* call_data){
	int app_id = call_data->m_unAppID;
	bool is_offline = call_data->m_bIsOffline;
	uint32 seconds_allowed = call_data->m_unSecondsAllowed;
	uint32 seconds_played = call_data->m_unSecondsPlayed;
	emit_signal("timed_trial_status", app_id, is_offline, seconds_allowed, seconds_played);
}

// APP LIST CALLBACKS ///////////////////////////
//
// Sent when a new app is installed.
void Steam::_app_installed(SteamAppInstalled_t* call_data){
	uint32_t app_id = (AppId_t)call_data->m_nAppID;
	uint32_t install_folder_index = call_data->m_iInstallFolderIndex;
	emit_signal("app_installed", app_id, install_folder_index);
}

// Sent when an app is uninstalled.
void Steam::_app_uninstalled(SteamAppUninstalled_t* call_data){
	uint32_t app_id = (AppId_t)call_data->m_nAppID;
	uint32_t install_folder_index = call_data->m_iInstallFolderIndex;
	emit_signal("app_uninstalled", app_id, install_folder_index);	
}

// FRIENDS CALLBACKS ////////////////////////////
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
	CSteamID steam_id = avatarData->m_steamID;
	uint64_t avatar_id = steam_id.ConvertToUint64();
	call_deferred("emit_signal", "avatar_loaded", avatar_id, width, data);
}

// Called when a Steam group activity has received.
void Steam::_clan_activity_downloaded(DownloadClanActivityCountsResult_t* call_data){
	bool success = call_data->m_bSuccess;
	// Set up the dictionary to populate
	Dictionary activity;
	if(success){
		int online = 0;
		int inGame = 0;
		int chatting = 0;
		activity["ret"] = SteamFriends()->GetClanActivityCounts(clan_activity, &online, &inGame, &chatting);
		if(activity["ret"]){
			activity["online"] = online;
			activity["ingame"] = inGame;
			activity["chatting"] = chatting;
		}
	}
	emit_signal("clan_activity_downloaded", activity);
}

// Called when Rich Presence data has been updated for a user, this can happen automatically when friends in the same game update their rich presence, or after a call to requestFriendRichPresence.
void Steam::_friend_rich_presence_update(FriendRichPresenceUpdate_t* call_data){
	uint64_t steam_id = call_data->m_steamIDFriend.ConvertToUint64();
	AppId_t app_id = call_data->m_nAppID;
	emit_signal("friend_rich_presence_updated", steam_id, app_id);
}

// Called when a user has joined a Steam group chat that the we are in.
void Steam::_connected_chat_join(GameConnectedChatJoin_t* call_data){
	uint64_t chat_id = call_data->m_steamIDClanChat.ConvertToUint64();
	uint64_t steam_id = call_data->m_steamIDUser.ConvertToUint64();
	emit_signal("chat_joined", chat_id, steam_id);
}

// Called when a user has left a Steam group chat that the we are in.
void Steam::_connected_chat_leave(GameConnectedChatLeave_t* call_data){
	uint64_t chat_id = call_data->m_steamIDClanChat.ConvertToUint64();
	uint64_t steam_id = call_data->m_steamIDUser.ConvertToUint64();
	bool kicked = call_data->m_bKicked;
	bool dropped = call_data->m_bDropped;
	emit_signal("chat_left", chat_id, steam_id, kicked, dropped);
}

// Called when a chat message has been received in a Steam group chat that we are in.
void Steam::_connected_clan_chat_message(GameConnectedClanChatMsg_t* call_data){
	Dictionary chat;
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	CSteamID user_id;
	chat["ret"] = SteamFriends()->GetClanChatMessage(call_data->m_steamIDClanChat, call_data->m_iMessageID, text, 2048, &type, &user_id);
	chat["text"] = String(text);
	chat["type"] = type;
	chat["chatter"] = uint64_t(user_id.ConvertToUint64());
	emit_signal("clan_chat_message", chat);
}

// Called when chat message has been received from a friend
void Steam::_connected_friend_chat_message(GameConnectedFriendChatMsg_t* call_data){
	uint64_t steam_id = call_data->m_steamIDUser.ConvertToUint64();
	int message = call_data->m_iMessageID;
	Dictionary chat;
	char text[2048];
	EChatEntryType type = k_EChatEntryTypeInvalid;
	chat["ret"] = SteamFriends()->GetFriendMessage((uint64)steam_id, message, text, 2048, &type);
	chat["text"] = String(text);
	emit_signal("friend_chat_message", chat);
}

// Called when the user tries to join a lobby from their friends list or from an invite. The game client should attempt to connect to specified lobby when this is received. If the game isn't running yet then the game will be automatically launched with the command line parameter +connect_lobby <64-bit lobby Steam ID> instead.
void Steam::_join_requested(GameLobbyJoinRequested_t* call_data){
	CSteamID lobby_id = call_data->m_steamIDLobby;
	uint64_t lobby = lobby_id.ConvertToUint64();
	CSteamID friend_id = call_data->m_steamIDFriend;
	uint64_t steam_id = friend_id.ConvertToUint64();
	emit_signal("join_requested", lobby, steam_id);
}

// Posted when the Steam Overlay activates or deactivates. The game can use this to be pause or resume single player games.
void Steam::_overlay_toggled(GameOverlayActivated_t* call_data){
	if(call_data->m_bActive){
		emit_signal("overlay_toggled", true);
	}
	else{
		emit_signal("overlay_toggled", false);
	}
}

// Called when the user tries to join a game from their friends list or after a user accepts an invite by a friend with inviteUserToGame.
void Steam::_join_game_requested(GameRichPresenceJoinRequested_t* call_data){
	CSteamID steam_id = call_data->m_steamIDFriend;
	uint64_t user = steam_id.ConvertToUint64();
	String connect = call_data->m_rgchConnect;
	emit_signal("join_game_requested", user, connect);
}

// This callback is made when joining a game. If the user is attempting to join a lobby, then the callback GameLobbyJoinRequested_t will be made.
void Steam::_change_server_requested(GameServerChangeRequested_t* call_data){
	String server = call_data->m_rgchServer;
	String password = call_data->m_rgchPassword;
	emit_signal("change_server_requested", server, password);
}

//
void Steam::_join_clan_chat_complete(JoinClanChatRoomCompletionResult_t* call_data){
	uint64_t chat_id = call_data->m_steamIDClanChat.ConvertToUint64();
	EChatRoomEnterResponse response = call_data->m_eChatRoomEnterResponse;
	emit_signal("chat_join_complete", chat_id, response);
}

// Signal for a user change
void Steam::_persona_state_change(PersonaStateChange_t* call_data){
	uint64_t steam_id = call_data->m_ulSteamID;
	int flags = call_data->m_nChangeFlags;
	emit_signal("persona_state_change", steam_id, flags);
}

// Reports the result of an attempt to change the user's persona name.
void Steam::_name_changed(SetPersonaNameResponse_t* call_data){
	bool success = call_data->m_bSuccess;
	bool localSuccess = call_data->m_bLocalSuccess;
	EResult result = call_data->m_result;
	emit_signal("name_changed", success, localSuccess, result);
}

// Dispatched when an overlay browser instance is navigated to a protocol/scheme registered by RegisterProtocolInOverlayBrowser().
void Steam::_overlay_browser_protocol(OverlayBrowserProtocolNavigation_t* call_data){
	String uri = call_data->rgchURI;
	emit_signal("overlay_browser_protocol", uri);
}

// Purpose: Invoked when the status of unread messages changes
void Steam::_unread_chat_messages_changed(UnreadChatMessagesChanged_t* call_data){
	emit_signal("unread_chat_messages_changed");
}

// GAME SEARCH CALLBACKS ////////////////////////
//
// There are no notes about this in Valve's header files or documentation.
void Steam::_search_for_game_progress(SearchForGameProgressCallback_t* call_data){
	EResult result = call_data->m_eResult;
	uint64_t searchID = call_data->m_ullSearchID;
	uint64_t lobby_id = call_data->m_lobbyID.ConvertToUint64();
	uint64_t steamIDEndedSearch = call_data->m_steamIDEndedSearch.ConvertToUint64();
	// Create a dictionary for search progress
	Dictionary searchProgress;
	searchProgress["lobby_id"] = lobby_id;
	searchProgress["ended_search_id"] = steamIDEndedSearch;
	searchProgress["seconds_remaining_estimate"] = call_data->m_nSecondsRemainingEstimate;
	searchProgress["players_searching"] = call_data->m_cPlayersSearching;
	emit_signal("search_for_game_progress", result, searchID, searchProgress);
}

// Notification to all players searching that a game has been found.
void Steam::_search_for_game_result(SearchForGameResultCallback_t* call_data){
	EResult result = call_data->m_eResult;
	uint64_t searchID = call_data->m_ullSearchID;
	uint64_t host_id = call_data->m_steamIDHost.ConvertToUint64();
	// Create a dictionary for search results
	Dictionary searchResult;
	searchResult["count_players_ingame"] = call_data->m_nCountPlayersInGame;
	searchResult["count_accepted_game"] = call_data->m_nCountAcceptedGame;
	searchResult["host_id"] = host_id;
	searchResult["final_callback"] = call_data->m_bFinalCallback;
	emit_signal("search_for_game_result", result, searchID, searchResult);
}

// Callback from RequestPlayersForGame when the matchmaking service has started or ended search; callback will also follow a call from CancelRequestPlayersForGame - m_bSearchInProgress will be false.
void Steam::_request_players_for_game_progress(RequestPlayersForGameProgressCallback_t* call_data){
	EResult result = call_data->m_eResult;
	uint64_t searchID = call_data->m_ullSearchID;
	emit_signal("request_players_for_game_progress", result, searchID);
}

// Callback from RequestPlayersForGame, one of these will be sent per player followed by additional callbacks when players accept or decline the game.
void Steam::_request_players_for_game_result(RequestPlayersForGameResultCallback_t* call_data){
	EResult result = call_data->m_eResult;
	uint64_t searchID = call_data->m_ullSearchID;
	uint64_t player_id = call_data->m_SteamIDPlayerFound.ConvertToUint64();
	uint64_t lobby_id = call_data->m_SteamIDLobby.ConvertToUint64();
	uint64_t uniqueGameID = call_data->m_ullUniqueGameID;
	// Create a dictionary for the player data
	Dictionary playerData;
	playerData["player_id"] = player_id;
	playerData["lobby_id"] = lobby_id;
	playerData["player_accept_state"] = call_data->m_ePlayerAcceptState;
	playerData["player_index"] = call_data->m_nPlayerIndex;
	playerData["total_players"] = call_data->m_nTotalPlayersFound;
	playerData["total_players_accepted_game"] = call_data->m_nTotalPlayersAcceptedGame;
	playerData["suggested_team_index"] = call_data->m_nSuggestedTeamIndex;
	playerData["unique_game_id"] = uniqueGameID;
	// Send the data back via signal
	emit_signal("request_players_for_game_result", result, searchID, playerData);
}

// There are no notes about this in Valve's header files or documentation.
void Steam::_request_players_for_game_final_result(RequestPlayersForGameFinalResultCallback_t* call_data){
	EResult result = call_data->m_eResult;
	uint64_t searchID = call_data->m_ullSearchID;
	uint64_t game_id = call_data->m_ullUniqueGameID;
	emit_signal("request_players_for_game_final", result, searchID, game_id);
}

// This callback confirms that results were received by the matchmaking service for this player.
void Steam::_submit_player_result(SubmitPlayerResultResultCallback_t* call_data){
	EResult result = call_data->m_eResult;
	uint64_t game_id = call_data->ullUniqueGameID;
	uint64_t player_id = call_data->steamIDPlayer.ConvertToUint64();
	emit_signal("submit_player_result", result, game_id, player_id);
}

// This callback confirms that the game is recorded as complete on the matchmaking service, the next call to RequestPlayersForGame will generate a new unique game ID.
void Steam::_end_game_result(EndGameResultCallback_t* call_data){
	EResult result = call_data->m_eResult;
	uint64_t game_id = call_data->ullUniqueGameID;
	emit_signal("end_game_result", result, game_id);
}

// HTML SURFACE CALLBACKS ///////////////////////
// 
// A new browser was created and is ready for use.
void Steam::_html_browser_ready(HTML_BrowserReady_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	emit_signal("html_browser_ready");
}

// Called when page history status has changed the ability to go backwards and forward.
void Steam::_html_can_go_backandforward(HTML_CanGoBackAndForward_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	bool goBack = call_data->bCanGoBack;
	bool goForward = call_data->bCanGoForward;
	emit_signal("html_can_go_backandforward", goBack, goForward);
}

// Called when the current page in a browser gets a new title.
void Steam::_html_changed_title(HTML_ChangedTitle_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& title = call_data->pchTitle;
	emit_signal("html_changed_title", title);
}

// Called when the browser has been requested to close due to user interaction; usually because of a javascript window.close() call.
void Steam::_html_close_browser(HTML_CloseBrowser_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	emit_signal("html_close_browser");
}

// Called when a browser surface has received a file open dialog from a <input type="file"> click or similar, you must call FileLoadDialogResponse with the file(s) the user selected.
void Steam::_html_file_open_dialog(HTML_FileOpenDialog_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& title = call_data->pchTitle;
	const String& initialFile = call_data->pchInitialFile;
	// Allows you to react to a page wanting to open a file load dialog. NOTE: You MUST call this in response to a HTML_FileOpenDialog_t callback.
	// So it is added here unless there is a case to use it separately.
	SteamHTMLSurface()->FileLoadDialogResponse(browser_handle, &call_data->pchInitialFile);
	// Send the signal back to the user
	emit_signal("html_file_open_dialog", title, initialFile);
}

// Called when a browser has finished loading a page.
void Steam::_html_finished_request(HTML_FinishedRequest_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& url = call_data->pchURL;
	const String& title = call_data->pchPageTitle;
	emit_signal("html_finished_request", url, title);
}

// Called when a a browser wants to hide a tooltip.
void Steam::_html_hide_tooltip(HTML_HideToolTip_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	emit_signal("html_hide_tooltip");
}

// Provides details on the visibility and size of the horizontal scrollbar.
void Steam::_html_horizontal_scroll(HTML_HorizontalScroll_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	// Create dictionary to bypass argument limit in Godot
	Dictionary scrollData;
	scrollData["scrollMax"] = call_data->unScrollMax;
	scrollData["scrollCurrent"] = call_data->unScrollCurrent;
	scrollData["pageScale"] = call_data->flPageScale;
	scrollData["visible"] = call_data->bVisible;
	scrollData["pageSize"] = call_data->unPageSize;
	emit_signal("html_horizontal_scroll", scrollData);
}

// Called when the browser wants to display a Javascript alert dialog, call JSDialogResponse when the user dismisses this dialog; or right away to ignore it.
void Steam::_html_js_alert(HTML_JSAlert_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& message = call_data->pchMessage;
	emit_signal("html_js_alert", message);
}

// Called when the browser wants to display a Javascript confirmation dialog, call JSDialogResponse when the user dismisses this dialog; or right away to ignore it.
void Steam::_html_js_confirm(HTML_JSConfirm_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& message = call_data->pchMessage;
	emit_signal("html_js_confirm", message);
}

// Result of a call to GetLinkAtPosition.
void Steam::_html_link_at_position(HTML_LinkAtPosition_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	// Create dictionary to bypass Godot argument limit
	Dictionary linkData;
	linkData["x"] = call_data->x;
	linkData["y"] = call_data->y;
	linkData["url"] = call_data->pchURL;
	linkData["input"] = call_data->bInput;
	linkData["liveLink"] = call_data->bLiveLink;
	emit_signal("html_link_at_position", linkData);
}

// Called when a browser surface has a pending paint. This is where you get the actual image data to render to the screen.
void Steam::_html_needs_paint(HTML_NeedsPaint_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	// Create dictionary to bypass Godot argument limit
	Dictionary pageData;
	pageData["bgra"] = call_data->pBGRA;
	pageData["wide"] = call_data->unWide;
	pageData["tall"] = call_data->unTall;
	pageData["updateX"] = call_data->unUpdateX;
	pageData["updateY"] = call_data->unUpdateY;
	pageData["updateWide"] = call_data->unUpdateWide;
	pageData["updateTall"] = call_data->unUpdateTall;
	pageData["scrollX"] = call_data->unScrollX;
	pageData["scrollY"] = call_data->unScrollY;
	pageData["pageScale"] = call_data->flPageScale;
	pageData["pageSerial"] = call_data->unPageSerial;
	emit_signal("html_needs_paint", pageData);
}

// A browser has created a new HTML window.
void Steam::_html_new_window(HTML_NewWindow_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	// Create a dictionary to bypass Godot argument limit
	Dictionary windowData;
	windowData["url"] = call_data->pchURL;
	windowData["x"] = call_data->unX;
	windowData["y"] = call_data->unY;
	windowData["wide"] = call_data->unWide;
	windowData["tall"] = call_data->unTall;
	windowData["newHandle"] = call_data->unNewWindow_BrowserHandle_IGNORE;
	emit_signal("html_new_window", windowData);
}

// The browser has requested to load a url in a new tab.
void Steam::_html_open_link_in_new_tab(HTML_OpenLinkInNewTab_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& url = call_data->pchURL;
	emit_signal("html_open_link_in_new_tab", url);
}

// Results from a search.
void Steam::_html_search_results(HTML_SearchResults_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	uint32 results = call_data->unResults;
	uint32 currentMatch = call_data->unCurrentMatch;
	emit_signal("html_search_results", results, currentMatch);
}

// Called when a browser wants to change the mouse cursor.
void Steam::_html_set_cursor(HTML_SetCursor_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	uint32 mouseCursor = call_data->eMouseCursor;
	emit_signal("html_set_cursor", mouseCursor);
}

// Called when a browser wants to display a tooltip.
void Steam::_html_show_tooltip(HTML_ShowToolTip_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& message = call_data->pchMsg;
	emit_signal("html_show_tooltip", message);
}

// Called when a browser wants to navigate to a new page.
void Steam::_html_start_request(HTML_StartRequest_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& url = call_data->pchURL;
	const String& target = call_data->pchTarget;
	const String& post_data = call_data->pchPostData;
	bool redirect = call_data->bIsRedirect;
	emit_signal("html_start_request", url, target, post_data, redirect);
}

// Called when a browser wants you to display an informational message. This is most commonly used when you hover over links.
void Steam::_html_status_text(HTML_StatusText_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& message = call_data->pchMsg;
	emit_signal("html_status_text", message);
}

// Called when the text of an existing tooltip has been updated.
void Steam::_html_update_tooltip(HTML_UpdateToolTip_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	const String& message = call_data->pchMsg;
	emit_signal("html_update_tooltip", message);
}

// Called when the browser is navigating to a new url.
void Steam::_html_url_changed(HTML_URLChanged_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	// Create a dictionary to bypass Godot argument limit
	Dictionary urlData;
	urlData["url"] = call_data->pchURL;
	urlData["post_data"] = call_data->pchPostData;
	urlData["redirect"] = call_data->bIsRedirect;
	urlData["title"] = call_data->pchPageTitle;
	urlData["newNavigation"] = call_data->bNewNavigation;
	emit_signal("html_url_changed", urlData);
}

// Provides details on the visibility and size of the vertical scrollbar.
void Steam::_html_vertical_scroll(HTML_VerticalScroll_t* call_data){
	browser_handle = call_data->unBrowserHandle;
	// Create dictionary to bypass argument limit in Godot
	Dictionary scrollData;
	scrollData["scrollMax"] = call_data->unScrollMax;
	scrollData["scrollCurrent"] = call_data->unScrollCurrent;
	scrollData["pageScale"] = call_data->flPageScale;
	scrollData["visible"] = call_data->bVisible;
	scrollData["pageSize"] = call_data->unPageSize;
	emit_signal("html_vertical_scroll", scrollData);
}

// HTTP CALLBACKS ///////////////////////////////
//
// Result when an HTTP request completes. If you're using GetHTTPStreamingResponseBodyData then you should be using the HTTPRequestHeadersReceived_t or HTTPRequestDataReceived_t.
void Steam::_http_request_completed(HTTPRequestCompleted_t* call_data){
	uint32 cookie_handle = call_data->m_hRequest;
	uint64_t context_value = call_data->m_ulContextValue;
	bool requestSuccess = call_data->m_bRequestSuccessful;
	int statusCode = call_data->m_eStatusCode;
	uint32 body_size = call_data->m_unBodySize;
	emit_signal("http_request_completed", cookie_handle, context_value, requestSuccess, statusCode, body_size);
}

// Triggered when a chunk of data is received from a streaming HTTP request.
void Steam::_http_request_data_received(HTTPRequestDataReceived_t* call_data){
	uint32 cookie_handle = call_data->m_hRequest;
	uint64_t context_value = call_data->m_ulContextValue;
	uint32 offset = call_data->m_cOffset;
	uint32 bytesReceived = call_data->m_cBytesReceived;
	emit_signal("http_request_data_received", cookie_handle, context_value, offset, bytesReceived);
}

// Triggered when HTTP headers are received from a streaming HTTP request.
void Steam::_http_request_headers_received(HTTPRequestHeadersReceived_t* call_data){
	uint32 cookie_handle = call_data->m_hRequest;
	uint64_t context_value = call_data->m_ulContextValue;
	emit_signal("http_request_headers_received", cookie_handle, context_value);
}

// INPUT CALLBACKS //////////////////////////////
//
// Purpose: when callbacks are enabled this fires each time a controller action state changes
//void Steam::_input_action_event(SteamInputActionEvent_t* call_data){
//	uint64_t input_handle = call_data->controllerHandle;
//	int event_type = call_data->eEventType;
//	// Needs analogAction and digitalAction structs
//	emit_signal("input_action_event", input_handle, event_type);
//}

// Purpose: called when a new controller has been connected, will fire once per controller if multiple new controllers connect in the same frame
void Steam::_input_device_connected(SteamInputDeviceConnected_t* call_data){
	uint64_t input_handle = call_data->m_ulConnectedDeviceHandle;
	emit_signal("input_device_connected", input_handle);
}

//Purpose: called when a new controller has been connected, will fire once per controller if multiple new controllers connect in the same frame
void Steam::_input_device_disconnected(SteamInputDeviceDisconnected_t* call_data){
	uint64_t input_handle = call_data->m_ulDisconnectedDeviceHandle;
	emit_signal("input_device_disconnected", input_handle);
}

// Purpose: called when a controller configuration has been loaded, will fire once per controller per focus change for Steam Input enabled controllers
void Steam::_input_configuration_loaded(SteamInputConfigurationLoaded_t* call_data){
	uint32_t app_id = call_data->m_unAppID;
	uint64_t device_handle = call_data->m_ulDeviceHandle;
	// Split into a dictionary since Godot won't allow more than 6 arguments
	Dictionary config_data;
	uint64_t mapping_creator = call_data->m_ulMappingCreator.ConvertToUint64();
	config_data["mapping_creator"] = mapping_creator;
	config_data["major_revision"] = call_data->m_unMajorRevision;						// uint32
	config_data["minor_revision"] = call_data->m_unMinorRevision;						// uint32
	config_data["uses_steam_input_api"] = call_data->m_bUsesSteamInputAPI;				// bool
	config_data["uses_gamepad_api"] = call_data->m_bUsesGamepadAPI;						// bool
	emit_signal("input_configuration_loaded", app_id, device_handle, config_data);
}

// INVENTORY CALLBACKS //////////////////////////
//
// This callback is triggered whenever item definitions have been updated, which could be in response to LoadItemDefinitions or any time new item definitions are available (eg, from the dynamic addition of new item types while players are still in-game).
void Steam::_inventory_definition_update(SteamInventoryDefinitionUpdate_t *call_data){
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

// Triggered when GetAllItems successfully returns a result which is newer / fresher than the last known result. (It will not trigger if the inventory hasn't changed, or if results from two overlapping calls are reversed in flight and the earlier result is already known to be stale/out-of-date.)
// The regular SteamInventoryResultReady_t callback will still be triggered immediately afterwards; this is an additional notification for your convenience.
void Steam::_inventory_full_update(SteamInventoryFullUpdate_t *call_data){
	// Set the handle
	inventory_handle = call_data->m_handle;
	// Send the handle back to the user
	emit_signal("inventory_full_update", call_data->m_handle);
}

// This is fired whenever an inventory result transitions from k_EResultPending to any other completed state, see GetResultStatus for the complete list of states. There will always be exactly one callback per handle.
void Steam::_inventory_result_ready(SteamInventoryResultReady_t *call_data){
	// Get the result
	int result = call_data->m_result;
	// Get the handle and pass it over
	inventory_handle = call_data->m_handle;
	emit_signal("inventory_result_ready", result, inventory_handle);
}

// MATCHMAKING CALLBACKS ////////////////////////
//
// Called when an account on your favorites list is updated
void Steam::_favorites_list_accounts_updated(FavoritesListAccountsUpdated_t* call_data){
	int result = call_data->m_eResult;
	emit_signal("favorites_list_accounts_updated", result);
}

// A server was added/removed from the favorites list, you should refresh now.
void Steam::_favorites_list_changed(FavoritesListChanged_t* call_data){
	Dictionary favorite;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char favoriteIP[16];
	for(int j = 0; j < NBYTES; j++){
		octet[j] = call_data->m_nIP >> (j * 8);
	}
	sprintf(favoriteIP, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
	favorite["ip"] = favoriteIP; 
	favorite["query_port"] = call_data->m_nQueryPort;
	favorite["connPort"] = call_data->m_nConnPort;
	favorite["app_id"] = call_data->m_nAppID;
	favorite["flags"] = call_data->m_nFlags;
	favorite["add"] = call_data->m_bAdd;
	favorite["account_id"] = call_data->m_unAccountId;
	emit_signal("favorites_list_changed", favorite);
}

// Signal when a lobby chat message is received
void Steam::_lobby_message(LobbyChatMsg_t* call_data){
	CSteamID user_id = call_data->m_ulSteamIDUser;
	uint8 chatType = call_data->m_eChatEntryType;
	// Convert the chat type over
	EChatEntryType type = (EChatEntryType)chatType;
	// Get the chat message data
	char buffer[4096];
	int result = SteamMatchmaking()->GetLobbyChatEntry(call_data->m_ulSteamIDLobby, call_data->m_iChatID, &user_id, &buffer, 4096, &type);
	uint64_t user = user_id.ConvertToUint64();
	emit_signal("lobby_message", result, user, String(buffer), chatType);
}

// A lobby chat room state has changed, this is usually sent when a user has joined or left the lobby.
void Steam::_lobby_chat_update(LobbyChatUpdate_t* call_data){
	uint64_t lobby_id = call_data->m_ulSteamIDLobby;
	uint64_t changedID = call_data->m_ulSteamIDUserChanged;
	uint64_t makingChangeID = call_data->m_ulSteamIDMakingChange;
	uint32 chatState = call_data->m_rgfChatMemberStateChange;
	emit_signal("lobby_chat_update", lobby_id, changedID, makingChangeID, chatState);
}

// The lobby metadata has changed.
void Steam::_lobby_data_update(LobbyDataUpdate_t* call_data){
	uint64_t memberID = call_data->m_ulSteamIDMember;
	uint64_t lobby_id = call_data->m_ulSteamIDLobby;
	uint8 success = call_data->m_bSuccess;
	char key = '\0';
	// Is the lobby
	if(memberID == lobby_id){
		SteamMatchmaking()->GetLobbyData(call_data->m_ulSteamIDLobby, &key);
	}
	// Is a user in the lobby
	else{
		SteamMatchmaking()->GetLobbyMemberData(call_data->m_ulSteamIDLobby, call_data->m_ulSteamIDMember, &key);
	}
	emit_signal("lobby_data_update", success, lobby_id, memberID, key);
}

// Posted if a user is forcefully removed from a lobby; can occur if a user loses connection to Steam.
void Steam::_lobby_kicked(LobbyKicked_t* call_data){
	CSteamID steamLobbyID = call_data->m_ulSteamIDLobby;
	uint64_t lobby_id = steamLobbyID.ConvertToUint64();
	CSteamID steamAdminID = call_data->m_ulSteamIDAdmin;
	uint64_t adminID = steamAdminID.ConvertToUint64();
	uint8 due_to_disconnect = call_data->m_bKickedDueToDisconnect;
	emit_signal("lobby_kicked", lobby_id, adminID, due_to_disconnect);
}

// Received upon attempting to enter a lobby. Lobby metadata is available to use immediately after receiving this.
void Steam::_lobby_joined(LobbyEnter_t* lobbyData){
	CSteamID steamLobbyID = lobbyData->m_ulSteamIDLobby;
	uint64_t lobby_id = steamLobbyID.ConvertToUint64();
	uint32_t permissions = lobbyData->m_rgfChatPermissions;
	bool locked = lobbyData->m_bLocked;
	uint32_t response = lobbyData->m_EChatRoomEnterResponse;
	emit_signal("lobby_joined", lobby_id, permissions, locked, response);
}

// A game server has been set via SetLobbyGameServer for all of the members of the lobby to join. It's up to the individual clients to take action on this; the typical game behavior is to leave the lobby and connect to the specified game server; but the lobby may stay open throughout the session if desired.
void Steam::_lobby_game_created(LobbyGameCreated_t* call_data){
	uint64_t lobby_id = call_data->m_ulSteamIDLobby;
	uint64_t server_id = call_data->m_ulSteamIDGameServer;
	uint32 ip = call_data->m_unIP;
	uint16 port = call_data->m_usPort;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char server_ip[16];
	for(int i = 0; i < NBYTES; i++){
		octet[i] = ip >> (i * 8);
	}
	sprintf(server_ip, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
	emit_signal("lobby_game_created", lobby_id, server_id, server_ip, port);
}

// Someone has invited you to join a Lobby. Normally you don't need to do anything with this, as the Steam UI will also display a '<user> has invited you to the lobby, join?' notification and message. If the user outside a game chooses to join, your game will be launched with the parameter +connect_lobby <64-bit lobby id>, or with the callback GameLobbyJoinRequested_t if they're already in-game.
void Steam::_lobby_invite(LobbyInvite_t* lobbyData){
	CSteamID inviterID = lobbyData->m_ulSteamIDUser;
	uint64_t inviter = inviterID.ConvertToUint64();
	CSteamID lobby_id = lobbyData->m_ulSteamIDLobby;
	uint64_t lobby = lobby_id.ConvertToUint64();
	CSteamID game_id = lobbyData->m_ulGameID;
	uint64_t game = game_id.ConvertToUint64();
	emit_signal("lobby_invite", inviter, lobby, game);
}

// MUSIC REMOTE CALLBACKS ///////////////////////
//
// The majority of callback for Music Remote have no fields and no descriptions. They seem to be primarily fired as responses to functions.
void Steam::_music_player_remote_to_front(MusicPlayerRemoteToFront_t* call_data){
	emit_signal("music_player_remote_to_front");
}
void Steam::_music_player_remote_will_activate(MusicPlayerRemoteWillActivate_t* call_data){
	emit_signal("music_player_remote_will_activate");
}
void Steam::_music_player_remote_will_deactivate(MusicPlayerRemoteWillDeactivate_t* call_data){
	emit_signal("music_player_remote_will_deactivate");
}
void Steam::_music_player_selects_playlist_entry(MusicPlayerSelectsPlaylistEntry_t* call_data){
	int entry = call_data->nID;
	emit_signal("music_player_selects_playlist_entry", entry);
}
void Steam::_music_player_selects_queue_entry(MusicPlayerSelectsQueueEntry_t* call_data){
	int entry = call_data->nID;
	emit_signal("music_player_selects_queue_entry", entry);
}
void Steam::_music_player_wants_looped(MusicPlayerWantsLooped_t* call_data){
	bool looped = call_data->m_bLooped;
	emit_signal("music_player_wants_looped", looped);
}
void Steam::_music_player_wants_pause(MusicPlayerWantsPause_t* call_data){
	emit_signal("music_player_wants_pause");
}
void Steam::_music_player_wants_playing_repeat_status(MusicPlayerWantsPlayingRepeatStatus_t* call_data){
	int status = call_data->m_nPlayingRepeatStatus;
	emit_signal("music_player_wants_playing_repeat_status", status);
}
void Steam::_music_player_wants_play_next(MusicPlayerWantsPlayNext_t* call_data){
	emit_signal("music_player_wants_play_next");
}
void Steam::_music_player_wants_play_previous(MusicPlayerWantsPlayPrevious_t* call_data){
	emit_signal("music_player_wants_play_previous");
}
void Steam::_music_player_wants_play(MusicPlayerWantsPlay_t* call_data){
	emit_signal("music_player_wants_play");
}
void Steam::_music_player_wants_shuffled(MusicPlayerWantsShuffled_t* call_data){
	bool shuffled = call_data->m_bShuffled;
	emit_signal("music_player_wants_shuffled", shuffled);
}
void Steam::_music_player_wants_volume(MusicPlayerWantsVolume_t* call_data){
	float volume = call_data->m_flNewVolume;
	emit_signal("music_player_wants_volume", volume);
}
void Steam::_music_player_will_quit(MusicPlayerWillQuit_t* call_data){
	emit_signal("music_player_will_quit");
}

// NETWORKING CALLBACKS /////////////////////////
//
// Called when packets can't get through to the specified user. All queued packets unsent at this point will be dropped, further attempts to send will retry making the connection (but will be dropped if we fail again).
void Steam::_p2p_session_connect_fail(P2PSessionConnectFail_t* call_data) {
	uint64_t steam_id_remote = call_data->m_steamIDRemote.ConvertToUint64();
	uint8_t sessionError = call_data->m_eP2PSessionError;
	emit_signal("p2p_session_connect_fail", steam_id_remote, sessionError);
}

// A user wants to communicate with us over the P2P channel via the sendP2PPacket. In response, a call to acceptP2PSessionWithUser needs to be made, if you want to open the network channel with them.
void Steam::_p2p_session_request(P2PSessionRequest_t* call_data){
	uint64_t steam_id_remote = call_data->m_steamIDRemote.ConvertToUint64();
	emit_signal("p2p_session_request", steam_id_remote);
}

// NETWORKING MESSAGES CALLBACKS ////////////////
//
// Posted when a remote host is sending us a message, and we do not already have a session with them.
void Steam::_network_messages_session_request(SteamNetworkingMessagesSessionRequest_t* call_data){
	SteamNetworkingIdentity remote = call_data->m_identityRemote;
	char identity;
	remote.ToString(&identity, 128);
	emit_signal("network_messages_session_request", identity);
}

// Posted when we fail to establish a connection, or we detect that communications have been disrupted it an unusual way.
void Steam::_network_messages_session_failed(SteamNetworkingMessagesSessionFailed_t* call_data){
	SteamNetConnectionInfo_t info = call_data->m_info;
	// Parse out the reason for failure
	int reason = info.m_eEndReason;
	emit_signal("network_messages_session_failed", reason);
}

// NETWORKING SOCKETS CALLBACKS /////////////////
//
// This callback is posted whenever a connection is created, destroyed, or changes state. The m_info field will contain a complete description of the connection at the time the change occurred and the callback was posted. In particular, m_info.m_eState will have the new connection state.
void Steam::_network_connection_status_changed(SteamNetConnectionStatusChangedCallback_t* call_data){
	// Connection handle.
	uint64_t connect_handle = call_data->m_hConn;
	// Full connection info.
	SteamNetConnectionInfo_t connectionInfo = call_data->m_info;
	// Move connection info into a dictionary
	Dictionary connection;
	char identity;
	connectionInfo.m_identityRemote.ToString(&identity, 128);
	connection["identity"] = identity;
	connection["user_data"] = (uint64_t)connectionInfo.m_nUserData;
	connection["listen_socket"] = connectionInfo.m_hListenSocket;
	char ip_address;
	connectionInfo.m_addrRemote.ToString(&ip_address, 128, true);
	connection["remote_address"] = ip_address;
	connection["remote_pop"] = connectionInfo.m_idPOPRemote;
	connection["pop_relay"] = connectionInfo.m_idPOPRelay;
	connection["connection_state"] = connectionInfo.m_eState;
	connection["end_reason"] = connectionInfo.m_eEndReason;
	connection["end_debug"] = connectionInfo.m_szEndDebug;
	connection["debug_description"] = connectionInfo.m_szConnectionDescription;
	// Previous state (current state is in m_info.m_eState).
	int old_state = call_data->m_eOldState;
	// Send the data back via signal
	emit_signal("network_connection_status_changed", connect_handle, connection, old_state);
}

// This callback is posted whenever the state of our readiness changes.
void Steam::_network_authentication_status(SteamNetAuthenticationStatus_t* call_data){
	// Status.
	int available = call_data->m_eAvail;
	// Non-localized English language status. For diagnostic / debugging purpsoes only.
	char *debug_message = new char[256];
	debug_message = call_data->m_debugMsg;
	// Send the data back via signal
	emit_signal("network_authentication_status", available, debug_message);
	delete[] debug_message;
}

// NETWORKING UTILS CALLBACKS ///////////////////
//
// A struct used to describe our readiness to use the relay network.
void Steam::_relay_network_status(SteamRelayNetworkStatus_t* call_data){
	int available = call_data->m_eAvail;
	int pingMeasurement = call_data->m_bPingMeasurementInProgress;
	int availableConfig = call_data->m_eAvailNetworkConfig;
	int availableRelay = call_data->m_eAvailAnyRelay;
	char *debug_message = new char[256];
	debug_message = call_data->m_debugMsg;
	emit_signal("relay_network_status", available, pingMeasurement, availableConfig, availableRelay, debug_message);
	delete[] debug_message;
}

// PARENTAL SETTINGS CALLBACKS //////////////////
//
// Purpose: Callback for querying UGC
void Steam::_parental_setting_changed(SteamParentalSettingsChanged_t* call_data){
	emit_signal("parental_setting_changed");
}

// PARTIES CALLBACKS ////////////////////////////
//
// After creating a beacon, when a user "follows" that beacon Steam will send you this callback to know that you should be prepared for the user to join your game. When they do join, be sure to call ISteamParties::OnReservationCompleted to let Steam know.
void Steam::_reservation_notification(ReservationNotificationCallback_t* call_data){
	uint64_t beaconID = call_data->m_ulBeaconID;
	uint64_t steam_id = call_data->m_steamIDJoiner.ConvertToUint64();
	emit_signal("reservation_notifications", beaconID, steam_id);
}

// Notification that the list of available locations for posting a beacon has been updated. 
void Steam::_available_beacon_locations_updated(AvailableBeaconLocationsUpdated_t* call_data){
	emit_signal("available_beacon_locations_updated");
}

// Notification that the list of active beacons visible to the current user has changed. 
void Steam::_active_beacons_updated(ActiveBeaconsUpdated_t* call_data){
	emit_signal("active_beacons_updated");
}

// REMOTE PLAY CALLBACKS ////////////////////////
//
// The session ID of the session that just connected.
void Steam::_remote_play_session_connected(SteamRemotePlaySessionConnected_t* call_data){
	uint32 session_id = call_data->m_unSessionID;
	emit_signal("remote_play_session_connected", session_id);
}

// The session ID of the session that just disconnected.
void Steam::_remote_play_session_disconnected(SteamRemotePlaySessionDisconnected_t* call_data){
	uint32 session_id = call_data->m_unSessionID;
	emit_signal("remote_play_session_disconnected", session_id);
}

// REMOTE STORAGE CALLBACKS /////////////////////
//
// Purpose: one or more files for this app have changed locally after syncing to remote session changes.
// Note: only posted if this happens DURING the local app session.
void Steam::_local_file_changed(RemoteStorageLocalFileChange_t* call_data){
	emit_signal("local_file_changed");
}

// SCREENSHOT CALLBACKS /////////////////////////
//
// A screenshot successfully written or otherwise added to the library and can now be tagged.
void Steam::_screenshot_ready(ScreenshotReady_t* call_data){
	uint32_t handle = call_data->m_hLocal;
	uint32_t result = call_data->m_eResult;
	emit_signal("screenshot_ready", handle, result);
}

// A screenshot has been requested by the user from the Steam screenshot hotkey. This will only be called if hookScreenshots has been enabled, in which case Steam will not take the screenshot itself.
void Steam::_screenshot_requested(ScreenshotRequested_t* call_data){
	emit_signal("screenshot_requested");
}

// UGC CALLBACKS ////////////////////////////////
//
// Called when a workshop item has been downloaded.
void Steam::_item_downloaded(DownloadItemResult_t* call_data){
	EResult result = call_data->m_eResult;
	PublishedFileId_t file_id = call_data->m_nPublishedFileId;
	AppId_t app_id = call_data->m_unAppID;
	emit_signal("item_downloaded", result, (uint64_t)file_id, (uint32_t)app_id);
}

// Called when a workshop item has been installed or updated.
void Steam::_item_installed(ItemInstalled_t* call_data){
	AppId_t app_id = call_data->m_unAppID;
	PublishedFileId_t file_id = call_data->m_nPublishedFileId;
	emit_signal("item_installed", app_id, (uint64_t)file_id);
}

// Purpose: signal that the list of subscribed items changed.
void Steam::_user_subscribed_items_list_changed(UserSubscribedItemsListChanged_t* call_data){
	int app_id = call_data->m_nAppID;
	emit_signal("user_subscribed_items_list_changed", app_id);
}

// USER CALLBACKS ///////////////////////////////
//
// Sent by the Steam server to the client telling it to disconnect from the specified game server, which it may be in the process of or already connected to. The game client should immediately disconnect upon receiving this message. This can usually occur if the user doesn't have rights to play on the game server.
void Steam::_client_game_server_deny(ClientGameServerDeny_t* call_data){
	uint32 app_id = call_data->m_uAppID;
	uint32 server_ip = call_data->m_unGameServerIP;
	uint16 server_port = call_data->m_usGameServerPort;
	uint16 secure = call_data->m_bSecure;
	uint32 reason = call_data->m_uReason;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char ip[16];
	for(int j = 0; j < NBYTES; j++){
		octet[j] = server_ip >> (j * 8);
	}
	sprintf(ip, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
	emit_signal("client_game_server_deny", app_id, ip, server_port, secure, reason);
}

// Sent to your game in response to a steam://gamewebcallback/ command from a user clicking a link in the Steam overlay browser. You can use this to add support for external site signups where you want to pop back into the browser after some web page signup sequence, and optionally get back some detail about that.
void Steam::_game_web_callback(GameWebCallback_t* call_data){
	String url = call_data->m_szURL;
	emit_signal("game_web_callback", url);
}

// Result when creating an auth session ticket.
void Steam::_get_auth_session_ticket_response(GetAuthSessionTicketResponse_t* call_data){
	uint32 auth_ticket = call_data->m_hAuthTicket;
	int result = call_data->m_eResult;
	emit_signal("get_auth_session_ticket_response", auth_ticket, result);
}

// Called when the callback system for this client is in an error state (and has flushed pending callbacks). When getting this message the client should disconnect from Steam, reset any stored Steam state and reconnect. This usually occurs in the rare event the Steam client has some kind of fatal error.
void Steam::_ipc_failure(IPCFailure_t *call_data){
	uint8 type = call_data->m_eFailureType;
	emit_signal("ipc_failure", type);
}

// Called whenever the users licenses (owned packages) changes.
void Steam::_licenses_updated(LicensesUpdated_t* call_data){
	emit_signal("licenses_updated");
}

// Called when a user has responded to a microtransaction authorization request.
void Steam::_microstransaction_auth_response(MicroTxnAuthorizationResponse_t *call_data){
	uint32 app_id = call_data->m_unAppID;
	uint64_t orderID = call_data->m_ulOrderID;
	bool authorized;
	if(call_data->m_bAuthorized == 1){
		authorized = true;
	}
	else{
		authorized = false;
	}
	emit_signal("microstransaction_auth_response", app_id, orderID, authorized);
}

// Called when a connections to the Steam back-end has been established. This means the Steam client now has a working connection to the Steam servers. Usually this will have occurred before the game has launched, and should only be seen if the user has dropped connection due to a networking issue or a Steam server update.
void Steam::_steam_server_connected(SteamServersConnected_t* connectData){
	emit_signal("steam_server_connected");
}

// Called if the client has lost connection to the Steam servers. Real-time services will be disabled until a matching SteamServersConnected_t has been posted.
void Steam::_steam_server_disconnected(SteamServersDisconnected_t* connectData){
	emit_signal("steam_server_disconnected");
}

// Called when an auth ticket has been validated.
void Steam::_validate_auth_ticket_response(ValidateAuthTicketResponse_t* call_data){
	uint64_t authID = call_data->m_SteamID.ConvertToUint64();
	uint32_t response = call_data->m_eAuthSessionResponse;
	uint64_t owner_id = call_data->m_OwnerSteamID.ConvertToUint64();
	emit_signal("validate_auth_ticket_response", authID, response, owner_id);
}

// USER STATS CALLBACKS /////////////////////////
//
// Result of a request to store the achievements on the server, or an "indicate progress" call. If both m_nCurProgress and m_nMaxProgress are zero, that means the achievement has been fully unlocked.
void Steam::_user_achievement_stored(UserAchievementStored_t* call_data){
	CSteamID game_id = call_data->m_nGameID;
	uint64_t game = game_id.ConvertToUint64();
	bool groupAchieve = call_data->m_bGroupAchievement;
	String name = call_data->m_rgchAchievementName;
	uint32_t current_progress = call_data->m_nCurProgress;
	uint32_t max_progress = call_data->m_nMaxProgress;
	emit_signal("user_achievement_stored", game, groupAchieve, name, current_progress, max_progress);
}

// Called when the latest stats and achievements for the local user have been received from the server.
void Steam::_current_stats_received(UserStatsReceived_t* call_data){
	CSteamID game_id = call_data->m_nGameID;
	uint64_t game = game_id.ConvertToUint64();
	uint32_t result = call_data->m_eResult;
	CSteamID user_id = call_data->m_steamIDUser;
	uint64_t user = user_id.ConvertToUint64();
	emit_signal("current_stats_received", game, result, user);
}

// Result of a request to store the user stats.
void Steam::_user_stats_stored(UserStatsStored_t* call_data){
	CSteamID game_id = call_data->m_nGameID;
	uint64_t game = game_id.ConvertToUint64();
	uint32_t result = call_data->m_eResult;
	emit_signal("user_stats_stored", game, result);
}

// Callback indicating that a user's stats have been unloaded. Call RequestUserStats again before accessing stats for this user.
void Steam::_user_stats_unloaded(UserStatsUnloaded_t* call_data){
	CSteamID steam_id = call_data->m_steamIDUser;
	uint64_t user = steam_id.ConvertToUint64();
	emit_signal("user_stats_unloaded", user);
}

// UTILITY CALLBACKS ////////////////////////////
//
// Called when the big picture gamepad text input has been closed.
void Steam::_gamepad_text_input_dismissed(GamepadTextInputDismissed_t* call_data){
	const uint32 buffer_length = 1024+1;
	char *text = new char[buffer_length];
	uint32 length = buffer_length;
	if(call_data->m_bSubmitted){
		SteamUtils()->GetEnteredGamepadTextInput(text, buffer_length);
		length = SteamUtils()->GetEnteredGamepadTextLength();
	}
	emit_signal("gamepad_text_input_dismissed", call_data->m_bSubmitted, text, length);
	delete[] text;
}

// Called when the country of the user changed. The country should be updated with getIPCountry.
void Steam::_ip_country(IPCountry_t* call_data){
	emit_signal("ip_country");
}

// Called when running on a laptop and less than 10 minutes of battery is left, and then fires then every minute afterwards.
void Steam::_low_power(LowBatteryPower_t* timeLeft){
	uint8 power = timeLeft->m_nMinutesBatteryLeft;
	emit_signal("low_power", power);
}

// Called when a SteamAPICall_t has completed (or failed)
void Steam::_steam_api_call_completed(SteamAPICallCompleted_t* call_data){
	uint64_t asyncCall = call_data->m_hAsyncCall;
	int callback = call_data->m_iCallback;
	uint32 parameter = call_data->m_cubParam;
	emit_signal("steam_api_call_completed", asyncCall, callback, parameter);
}

// Called when Steam wants to shutdown.
void Steam::_steam_shutdown(SteamShutdown_t* call_data){
	emit_signal("steam_shutdown");
}

// Sent after the device returns from sleep/suspend mode.
void Steam::_app_resuming_from_suspend(AppResumingFromSuspend_t *call_data) {
	emit_signal("app_resuming_from_suspend");
}

// Sent after the device returns from sleep/suspend mode.
void Steam::_floating_gamepad_text_input_dismissed(FloatingGamepadTextInputDismissed_t *call_data) {
	emit_signal("floating_gamepad_text_input_dismissed");
}

// VIDEO CALLBACKS //////////////////////////////
//
// Automatically called whenever the user starts broadcasting.	// In documentation but not in actual SDK?
//void Steam::_broadcast_upload_start(BroadcastUploadStart_t* call_data){
//	emit_signal("broadcast_upload_start");
//}

// Automatically called whenever the user stops broadcasting.	// In documentation but not in actual SDK?
//void Steam::_broadcast_upload_stop(BroadcastUploadStop_t* call_data){
//	int result = call_data->m_eResult;
//	emit_signal("broadcast_upload_stop", result);
//}

// Triggered when the OPF Details for 360 video playback are retrieved. After receiving this you can use GetOPFStringForApp to access the OPF details.
void Steam::_get_opf_settings_result(GetOPFSettingsResult_t* call_data){
	int result = call_data->m_eResult;
	int appid = call_data->m_unVideoAppID;
	emit_signal("broadcast_upload_stop", result, appid);
}

// Provides the result of a call to GetVideoURL.
void Steam::_get_video_result(GetVideoURLResult_t* call_data){
	int result = call_data->m_eResult;
	int appid = call_data->m_unVideoAppID;
	String url = call_data->m_rgchURL;
	emit_signal("get_video_result", result, appid, url);
}


/////////////////////////////////////////////////
///// SIGNALS / CALL RESULTS ////////////////////
/////////////////////////////////////////////////
//
// STEAMWORKS ERROR SIGNAL //////////////////////
//
// Intended to serve as generic error messaging for failed call results
void Steam::steamworksError(const String& failed_signal){
	// Print the error message showing the failed signal
	printf("[STEAM] IO Failure for call result: ");
	// Emit the signal to inform the user of the failure
	emit_signal("steamworks_error", failed_signal, "io failure");
}

// FRIENDS CALL RESULTS /////////////////////////
//
// Marks the return of a request officer list call.
void Steam::_request_clan_officer_list(ClanOfficerListResponse_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("request_clan_officer_list");
	}
	else{
		Array officersList;
		String message;
		if(!call_data->m_bSuccess || io_failure){
			message = "Clan officer list response failed.";
		}
		else{
			CSteamID ownerSteamID = SteamFriends()->GetClanOwner(call_data->m_steamIDClan);
			uint64_t owner_id = ownerSteamID.ConvertToUint64();
			int officers = SteamFriends()->GetClanOfficerCount(call_data->m_steamIDClan);
			message = "The owner of the clan is: " + String(SteamFriends()->GetFriendPersonaName(ownerSteamID)) + " (" + String(owner_id) + ") and there are " + String(call_data->m_cOfficers) + " officers.";
			for(int i = 0; i < officers; i++){
				Dictionary officer;
				CSteamID officerSteamID = SteamFriends()->GetClanOfficerByIndex(call_data->m_steamIDClan, i);
				uint64_t id = officerSteamID.ConvertToUint64();
				officer["id"] = id;
				officer["name"] = String(SteamFriends()->GetFriendPersonaName(officerSteamID));
				officersList.append(officer);
			}
		}
		emit_signal("request_clan_officer_list", message, officersList);
	}
}

// Returns the result of enumerateFollowingList.
void Steam::_enumerate_following_list(FriendsEnumerateFollowingList_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("enumerate_following_list");
	}
	else{
		Array following;
		String message;
		int followersParsed = 0;
		if(call_data->m_eResult != k_EResultOK || io_failure){
			message = "Failed to acquire list.";
		}
		else{
			message = "Retrieved " + String(call_data->m_nResultsReturned) + " of " + String(call_data->m_nTotalResultCount) + " people followed.";
			int32 count = call_data->m_nTotalResultCount;
			for(int i = 0; i < count; i++){
				Dictionary follow;
				int num = i;
				uint64_t id = call_data->m_rgSteamID[i].ConvertToUint64();
				follow["num"] = num;
				follow["id"] = id;
				following.append(follow);
			}
			followersParsed += call_data->m_nResultsReturned;
			// There are more followers so make another callback.
			if(followersParsed < count){
				SteamAPICall_t api_call = SteamFriends()->EnumerateFollowingList(call_data->m_nResultsReturned);
				callResultEnumerateFollowingList.Set(api_call, this, &Steam::_enumerate_following_list);
			}
		}
		emit_signal("following_list", message, following);
	}
}

// Returns the result of getFollowerCount.
void Steam::_get_follower_count(FriendsGetFollowerCount_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("get_follower_count");
	}
	else{
		EResult result = call_data->m_eResult;
		uint64_t steam_id = call_data->m_steamID.ConvertToUint64();
		int count = call_data->m_nCount;
		emit_signal("follower_count", result, steam_id, count);
	}
}

// Returns the result of isFollowing.
void Steam::_is_following(FriendsIsFollowing_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("is_following");
	}
	else{
		EResult result = call_data->m_eResult;
		uint64_t steam_id = call_data->m_steamID.ConvertToUint64();
		bool following = call_data->m_bIsFollowing;
		emit_signal("is_following", result, steam_id, following);
	}
}

// INVENTORY CALL RESULTS ///////////////////////
//
// Returned when you have requested the list of "eligible" promo items that can be manually granted to the given user. These are promo items of type "manual" that won't be granted automatically.
void Steam::_inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *call_data, bool io_failure){
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
}

// Returned after StartPurchase is called.
void Steam::_inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *call_data, bool io_failure){
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

// Returned after RequestPrices is called.
void Steam::_inventory_request_prices_result(SteamInventoryRequestPricesResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("inventory_request_prices_result");
	}
	else{
		int result = call_data->m_result;
		String currency = call_data->m_rgchCurrency;
		emit_signal("inventory_request_prices_result", result, currency);
	}
}

// MATCHMAKING CALL RESULTS /////////////////////
//
// Signal the lobby has been created.
void Steam::_lobby_created(LobbyCreated_t *lobbyData, bool io_failure){
	if(io_failure){
		steamworksError("lobby_created");
	}
	else{
		int connect = lobbyData->m_eResult;
		CSteamID lobby_id = lobbyData->m_ulSteamIDLobby;
		uint64_t lobby = lobby_id.ConvertToUint64();
		emit_signal("lobby_created", connect, lobby);
	}
}

// Result when requesting the lobby list. You should iterate over the returned lobbies with getLobbyByIndex, from 0 to m_nLobbiesMatching-1.
void Steam::_lobby_match_list(LobbyMatchList_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("lobby_match_list");
	}
	else{
		int lobbyCount = call_data->m_nLobbiesMatching;
		Array lobbies;
		for(int i = 0; i < lobbyCount; i++){
			CSteamID lobby_id = SteamMatchmaking()->GetLobbyByIndex(i);
			uint64_t lobby = lobby_id.ConvertToUint64();
			lobbies.append(lobby);
		}	
		emit_signal("lobby_match_list", lobbies);
	}
}

// MATCHMAKING SERVER CALL RESULTS //////////////
//
void Steam::_server_Responded(gameserveritem_t server){
	emit_signal("server_responded");
}

//
void Steam::_server_Failed_To_Respond(){
	emit_signal("server_failed_to_respond");
}

// PARTIES CALL RESULTS /////////////////////////
//
// This callback is used as a call response for ISteamParties::JoinParty. On success, you will have reserved a slot in the beacon-owner's party, and should use m_rgchConnectString to connect to their game and complete the process.
void Steam::_join_party(JoinPartyCallback_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("join_party");
	}
	else{
		int result = call_data->m_eResult;
		uint64_t beaconID = call_data->m_ulBeaconID;
		uint64_t steam_id = call_data->m_SteamIDBeaconOwner.ConvertToUint64();
		String connect_string = call_data->m_rgchConnectString;
		emit_signal("join_party", result, beaconID, steam_id, connect_string);
	}
}

// This callback is used as a call response for ISteamParties::CreateBeacon. If successful, your beacon has been posted in the desired location and you may start receiving ISteamParties::ReservationNotificationCallback_t callbacks for users following the beacon. 
void Steam::_create_beacon(CreateBeaconCallback_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("create_beacon");
	}
	else{
		int result = call_data->m_eResult;
		uint64_t beaconID = call_data->m_ulBeaconID;
		emit_signal("create_beacon", result, beaconID);
	}
}

// Call result for ISteamParties::ChangeNumOpenSlots. 
void Steam::_change_num_open_slots(ChangeNumOpenSlotsCallback_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("change_num_open_slots");
	}
	else{
		int result = call_data->m_eResult;
		emit_signal("change_num_open_slots", result);
	}
}

// REMOTE STORAGE CALL RESULTS //////////////////
//
// Response when reading a file asyncrounously with FileReadAsync.
void Steam::_file_read_async_complete(RemoteStorageFileReadAsyncComplete_t* call_data, bool io_failure){
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
		emit_signal("file_read_async_complete", handle, result, offset, read, complete);
	}
}

// Response to a file being shared.
void Steam::_file_share_result(RemoteStorageFileShareResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("file_share_result");
	}
	else{
		int result = call_data->m_eResult;
		uint64_t handle = call_data->m_hFile;
		char name = call_data->m_rgchFilename[k_cchFilenameMax];
		emit_signal("file_share_result", result, handle, name);
	}
}

// Response when writing a file asyncrounously with FileWriteAsync.
void Steam::_file_write_async_complete(RemoteStorageFileWriteAsyncComplete_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("file_write_async_complete");
	}
	else{
		int result = call_data->m_eResult;
		emit_signal("file_write_async_complete", result);
	}
}

// Response when downloading UGC
void Steam::_download_ugc_result(RemoteStorageDownloadUGCResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("download_ugc_result");
	}
	else{
		int result = call_data->m_eResult;
		uint64_t handle = call_data->m_hFile;
		uint32_t app_id = call_data->m_nAppID;
		int32 size = call_data->m_nSizeInBytes;
		char filename = call_data->m_pchFileName[k_cchFilenameMax];
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

// Called when the user has unsubscribed from a piece of UGC. Result from ISteamUGC::UnsubscribeItem.
void Steam::_unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("unsubscribe_item");
	}
	else{
		int result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		emit_signal("unsubscribe_item", result, (uint64_t)file_id);
	}
}

// Called when the user has subscribed to a piece of UGC. Result from ISteamUGC::SubscribeItem.
void Steam::_subscribe_item(RemoteStorageSubscribePublishedFileResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("subscribe_item");
	}
	else{
		int result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		emit_signal("subscribe_item", result, (uint64_t)file_id);
	}
}

// UGC CALL RESULTS /////////////////////////////
//
// The result of a call to AddAppDependency.
void Steam::_add_app_dependency_result(AddAppDependencyResult_t* call_data, bool io_failure){
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

// The result of a call to AddDependency.
void Steam::_add_ugc_dependency_result(AddUGCDependencyResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("add_ugc_dependency_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		PublishedFileId_t childID = call_data->m_nChildPublishedFileId;
		emit_signal("add_ugc_dependency_result", result, (uint64_t)file_id, (uint64_t)childID);
	}
}

// Result of a workshop item being created.
void Steam::_item_created(CreateItemResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("item_created");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		bool acceptTOS = call_data->m_bUserNeedsToAcceptWorkshopLegalAgreement;
		emit_signal("item_created", result, (uint64_t)file_id, acceptTOS);
	}
}

// Called when getting the app dependencies for an item.
void Steam::_get_app_dependencies_result(GetAppDependenciesResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("get_app_dependencies_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
//		AppId_t app_id = call_data->m_rgAppIDs;
		uint32 appDependencies = call_data->m_nNumAppDependencies;
		uint32 totalAppDependencies = call_data->m_nTotalNumAppDependencies;
//		emit_signal("get_app_dependencies_result", result, (uint64_t)file_id, app_id, appDependencies, totalAppDependencies);
		emit_signal("get_app_dependencies_result", result, (uint64_t)file_id, appDependencies, totalAppDependencies);
	}
}

// Called when an attempt at deleting an item completes.
void Steam::_item_deleted(DeleteItemResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("item_deleted");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		emit_signal("item_deleted", result, (uint64_t)file_id);
	}
}

// Called when getting the users vote status on an item.
void Steam::_get_item_vote_result(GetUserItemVoteResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("get_item_vote_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		bool vote_up = call_data->m_bVotedUp;
		bool voteDown = call_data->m_bVotedDown;
		bool voteSkipped = call_data->m_bVoteSkipped;
		emit_signal("get_item_vote_result", result, (uint64_t)file_id, vote_up, voteDown, voteSkipped);
	}
}

// Purpose: The result of a call to RemoveAppDependency.
void Steam::_remove_app_dependency_result(RemoveAppDependencyResult_t* call_data, bool io_failure){
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

// Purpose: The result of a call to RemoveDependency.
void Steam::_remove_ugc_dependency_result(RemoveUGCDependencyResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("remove_ugc_dependency_result");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		PublishedFileId_t childID = call_data->m_nChildPublishedFileId;
		emit_signal("remove_ugc_dependency_result", result, (uint64_t)file_id, (uint64_t)childID);
	}
}

// Called when the user has voted on an item.
void Steam::_set_user_item_vote(SetUserItemVoteResult_t* call_data, bool io_failure){
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

// Called when workshop item playtime tracking has started.
void Steam::_start_playtime_tracking(StartPlaytimeTrackingResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("start_playtime_tracking");
	}
	else{
		EResult result = call_data->m_eResult;
		emit_signal("start_playtime_tracking", result);
	}
}

// Called when a UGC query request completes.
void Steam::_ugc_query_completed(SteamUGCQueryCompleted_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("ugc_query_completed");
	}
	else{
		UGCQueryHandle_t handle = call_data->m_handle;
		EResult result = call_data->m_eResult;
		uint32 resultsReturned = call_data->m_unNumResultsReturned;
		uint32 totalMatching = call_data->m_unTotalMatchingResults;
		bool cached = call_data->m_bCachedData;
		emit_signal("ugc_query_completed", (uint64_t)handle, result, resultsReturned, totalMatching, cached);
	}
}

// Called when workshop item playtime tracking has stopped.
void Steam::_stop_playtime_tracking(StopPlaytimeTrackingResult_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("stop_playtime_tracking");
	}
	else{
		EResult result = call_data->m_eResult;
		emit_signal("stop_playtime_tracking", result);
	}
}

// Result of a workshop item being updated.
void Steam::_item_updated(SubmitItemUpdateResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("item_updated");
	}
	else{
		EResult result = call_data->m_eResult;
		bool acceptTOS = call_data->m_bUserNeedsToAcceptWorkshopLegalAgreement;
		emit_signal("item_updated", result, acceptTOS);
	}
}

// Called when the user has added or removed an item to/from their favorites.
void Steam::_user_favorite_items_list_changed(UserFavoriteItemsListChanged_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("user_favorite_items_list_changed");
	}
	else{
		EResult result = call_data->m_eResult;
		PublishedFileId_t file_id = call_data->m_nPublishedFileId;
		bool wasAddRequest = call_data->m_bWasAddRequest;
		emit_signal("user_favorite_items_list_changed", result, (uint64_t)file_id, wasAddRequest);
	}
}

// Purpose: Status of the user's acceptable/rejection of the app's specific Workshop EULA.
void Steam::_workshop_eula_status(WorkshopEULAStatus_t* call_data, bool io_failure){
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

// USERS CALL RESULTS ///////////////////////////
//
// Sent for games with enabled anti indulgence / duration control, for enabled users. Lets the game know whether persistent rewards or XP should be granted at normal rate, half rate, or zero rate.
void Steam::_duration_control(DurationControl_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("duration_control");
	}
	else{
		int result = call_data->m_eResult;
		int appid = call_data->m_appid;
		bool applicable = call_data->m_bApplicable;
		int32 secsLast = call_data->m_csecsLast5h;
		int progress = call_data->m_progress;
		int notification = call_data->m_notification;
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
		duration["app_id"] = appid;
		duration["applicable"] = applicable;
		duration["seconds_last_5hrs"] = secsLast;
		duration["progress"] = progress;
		duration["notification"] = notification;
		duration["notification_verbal"] = verbal;
		emit_signal("duration_control", result, duration);
	}
}

// Called when an encrypted application ticket has been received.
void Steam::_encrypted_app_ticket_response(EncryptedAppTicketResponse_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("encrypted_app_ticket_response");
	}
	else{
		String result;
		if(call_data->m_eResult == k_EResultOK){
			result = "ok";
		}
		else if(call_data->m_eResult == k_EResultNoConnection){
			result = "no connection";
		}
		else if(call_data->m_eResult == k_EResultDuplicateRequest){
			result = "duplicate request";
		}
		else{
			result = "limit exceeded";
		}
		emit_signal("encrypted_app_ticket_response", result);
	}
}

// Called when a connection attempt has failed. This will occur periodically if the Steam client is not connected, and has failed when retrying to establish a connection.
void Steam::_steam_server_connect_failed(SteamServerConnectFailure_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("steam_server_connect_failed");
	}
	else{
		int result = call_data->m_eResult;
		bool retrying = call_data->m_bStillRetrying;
		emit_signal("steam_server_connected_failed", result, retrying);
	}
}

// Response when we have recieved the authentication URL after a call to requestStoreAuthURL.
void Steam::_store_auth_url_response(StoreAuthURLResponse_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("store_auth_url_response");
	}
	else{
		String url = call_data->m_szURL;
		emit_signal("store_auth_url_response", url);
	}
}

// USER STATS CALL RESULTS //////////////////////
//
// Global achievements percentages are ready.
void Steam::_global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("global_achievement_percentages_ready");
	}
	else{
		CSteamID game_id = call_data->m_nGameID;
		uint64_t game = game_id.ConvertToUint64();
		uint32_t result = call_data->m_eResult;
		emit_signal("global_achievement_percentages_ready", game, result);
	}
}

// Called when the global stats have been received from the server.
void Steam::_global_stats_received(GlobalStatsReceived_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("global_stats_received");
	}
	else{
		uint64_t game_id = call_data->m_nGameID;
		String result;
		if(call_data->m_eResult == k_EResultOK){
			result = "ok";
		}
		else if(call_data->m_eResult == k_EResultInvalidState){
			result = "invalid";
		}
		else{
			result = "fail";
		}
		emit_signal("global_stats_received", game_id, result);
	}
}

// Result when finding a leaderboard.
void Steam::_leaderboard_find_result(LeaderboardFindResult_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("leaderboard_find_result");
	}
	else{
		leaderboard_handle = call_data->m_hSteamLeaderboard;
		uint8_t found = call_data->m_bLeaderboardFound;
		emit_signal("leaderboard_find_result", (uint64_t)leaderboard_handle, found);
	}
}

// Called when scores for a leaderboard have been downloaded and are ready to be retrieved. After calling you must use GetDownloadedLeaderboardEntry to retrieve the info for each downloaded entry.
void Steam::_leaderboard_scores_downloaded(LeaderboardScoresDownloaded_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("leaderboard_scores_downloaded");
	}
	else{
		// Set up a message to fill in
		String message;
		// Incorrect leaderboard
		if(call_data->m_hSteamLeaderboard != leaderboard_handle){
			message = "Leaderboard handle was incorrect";
		}
		if(!io_failure){
			// Clear previous leaderboard entries
			leaderboard_entries_array.clear();
			// Create the entry pointer and details array
			LeaderboardEntry_t *entry = new LeaderboardEntry_t;
			PoolIntArray details;
			int32 *detailsPointer = NULL;
			// Resize array
			if(leaderboard_details_max > 0){
				details.resize(leaderboard_details_max);
				PoolIntArray::Write w = details.write();
				detailsPointer = w.ptr();
				for(int i = 0; i < leaderboard_details_max; i++){
					detailsPointer[i] = 0;
				}
			}
			// Loop through the entries and add them as dictionaries to the array
			for(int i = 0; i < call_data->m_cEntryCount; i++){
				if(SteamUserStats()->GetDownloadedLeaderboardEntry(call_data->m_hSteamLeaderboardEntries, i, entry, detailsPointer, leaderboard_details_max)){
					Dictionary entryDict;
					entryDict["score"] = entry->m_nScore;
					entryDict["steam_id"] = uint64_t(entry->m_steamIDUser.ConvertToUint64());
					entryDict["global_rank"] = entry->m_nGlobalRank;
					entryDict["ugc_handle"] = uint64_t(entry->m_hUGC);
					if(leaderboard_details_max > 0){
						PoolIntArray array;
						array.resize(leaderboard_details_max);
						PoolIntArray::Write w = array.write();
						int32_t *ptr = w.ptr();
						for(int j = 0; j < leaderboard_details_max; j++){
							ptr[j] = detailsPointer[j];
						}
						entryDict["details"] = array;
					}
					leaderboard_entries_array.append(entryDict);
				}
				message = "Leaderboard entries successfully retrieved";
			}
			delete entry;
		} else {
			message = "There was an IO failure";
		}
		// Emit the signal, with array, back
		emit_signal("leaderboard_scores_downloaded", message, leaderboard_entries_array);
	}
}

// Result indicating that a leaderboard score has been uploaded.
void Steam::_leaderboard_score_uploaded(LeaderboardScoreUploaded_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("leaderboard_scores_uploaded");
	}
	else{
		// Incorrect leaderboard
		if(call_data->m_hSteamLeaderboard != leaderboard_handle){
			return;
		}
		uint8 success = call_data->m_bSuccess;
		int32 score = call_data->m_nScore;
		uint8 score_changed = call_data->m_bScoreChanged;
		int global_rank_new = call_data->m_nGlobalRankNew;
		int global_rank_prev = call_data->m_nGlobalRankPrevious;
		emit_signal("leaderboard_score_uploaded", success, score, score_changed, global_rank_new, global_rank_prev);
	}
}

// Result indicating that user generated content has been attached to one of the current user's leaderboard entries.
void Steam::_leaderboard_ugc_set(LeaderboardUGCSet_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("leaderboard_ugc_set");
	}
	else{
		leaderboard_handle = call_data->m_hSteamLeaderboard;
		String result;
		if(call_data->m_eResult == k_EResultOK){
			result = "ok";
		}
		else if(call_data->m_eResult == k_EResultTimeout){
			result = "timeout";
		}
		else{
			result = "invalid";
		}
		emit_signal("leaderboard_ugc_set", (uint64_t)leaderboard_handle, result);
	}
}

// Gets the current number of players for the current AppId.
void Steam::_number_of_current_players(NumberOfCurrentPlayers_t *call_data, bool io_failure){
	if(io_failure){
		steamworksError("number_of_current_players");
	}
	else{
		uint8 success = call_data->m_bSuccess;
		int32 players = call_data->m_cPlayers;
		emit_signal("number_of_current_players", success, players);
	}
}

// Called when the latest stats and achievements for a specific user (including the local user) have been received from the server.
void Steam::_user_stats_received(UserStatsReceived_t* call_data, bool io_failure){
	if(io_failure){
		steamworksError("user_stats_received");
	}
	else{
		CSteamID game_id = call_data->m_nGameID;
		uint64_t game = game_id.ConvertToUint64();
		uint32_t result = call_data->m_eResult;
		CSteamID user_id = call_data->m_steamIDUser;
		uint64_t user = user_id.ConvertToUint64();
		emit_signal("user_stats_received", game, result, user);
	}
}

// UTILITY CALL RESULTS /////////////////////////
//
// CallResult for checkFileSignature.
void Steam::_check_file_signature(CheckFileSignature_t *call_data, bool io_failure){
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
void Steam::_register_methods(){
	/////////////////////////////////////////////
	// FUNCTION BINDS ///////////////////////////
	/////////////////////////////////////////////
	//
	// STEAM MAIN BIND METHODS //////////////////
	register_method("restartAppIfNecessary", &Steam::restartAppIfNecessary);
	register_method("steamInit", &Steam::steamInit);
	register_method("isSteamRunning", &Steam::isSteamRunning);
	register_method("run_callbacks", &Steam::run_callbacks);
	
	// APPS BIND METHODS ////////////////////////
	register_method("isSubscribed", &Steam::isSubscribed);
	register_method("isLowViolence", &Steam::isLowViolence);
	register_method("isCybercafe", &Steam::isCybercafe);
	register_method("isVACBanned", &Steam::isVACBanned);
	register_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
	register_method("getAvailableGameLanguages", &Steam::getAvailableGameLanguages);
	register_method("isSubscribedApp", &Steam::isSubscribedApp);
	register_method("isDLCInstalled", &Steam::isDLCInstalled);
	register_method("getEarliestPurchaseUnixTime", &Steam::getEarliestPurchaseUnixTime);
	register_method("isSubscribedFromFreeWeekend", &Steam::isSubscribedFromFreeWeekend);
	register_method("getDLCCount", &Steam::getDLCCount);
	register_method("getDLCDataByIndex", &Steam::getDLCDataByIndex);
	register_method("installDLC", &Steam::installDLC);
	register_method("uninstallDLC", &Steam::uninstallDLC);
	register_method("getCurrentBetaName", &Steam::getCurrentBetaName);
	register_method("markContentCorrupt", &Steam::markContentCorrupt);
	register_method("getInstalledDepots", &Steam::getInstalledDepots);
	register_method("getAppInstallDir", &Steam::getAppInstallDir);
	register_method("isAppInstalled", &Steam::isAppInstalled);
	register_method("getAppOwner", &Steam::getAppOwner);
	register_method("getLaunchQueryParam", &Steam::getLaunchQueryParam);
	register_method("getDLCDownloadProgress", &Steam::getDLCDownloadProgress);
	register_method("getAppBuildId", &Steam::getAppBuildId);
	register_method("getFileDetails", &Steam::getFileDetails);
	register_method("isTimedTrial", &Steam::isTimedTrial);
	register_method("requestAllProofOfPurchaseKeys", &Steam::requestAllProofOfPurchaseKeys);
	register_method("requestAppProofOfPurchaseKey", &Steam::requestAppProofOfPurchaseKey);
	
	// APP LIST BIND METHODS ////////////////////
	register_method("getNumInstalledApps", &Steam::getNumInstalledApps);
	register_method("getInstalledApps", &Steam::getInstalledApps);
	register_method("getAppName", &Steam::getAppName);
	register_method("getAppListInstallDir", &Steam::getAppInstallDir);
	register_method("getAppListBuildId", &Steam::getAppBuildId);

	// Friends Bind Methods /////////////////////
	register_method("getPersonaName", &Steam::getPersonaName);
	register_method("setPersonaName", &Steam::setPersonaName);
	register_method("getPersonaState", &Steam::getPersonaState);
	register_method("getFriendCount", &Steam::getFriendCount);
	register_method("getFriendByIndex", &Steam::getFriendByIndex);
	register_method("getFriendRelationship", &Steam::getFriendRelationship);
	register_method("getFriendPersonaState", &Steam::getFriendPersonaState);
	register_method("getFriendPersonaName", &Steam::getFriendPersonaName);
	register_method("getFriendGamePlayed", &Steam::getFriendGamePlayed);
	register_method("getFriendPersonaNameHistory", &Steam::getFriendPersonaNameHistory);
	register_method("getFriendSteamLevel", &Steam::getFriendSteamLevel);
	register_method("getPlayerNickname", &Steam::getPlayerNickname);
	register_method("hasFriend", &Steam::hasFriend);
	register_method("downloadClanActivityCounts", &Steam::downloadClanActivityCounts);
	register_method("getFriendCountFromSource", &Steam::getFriendCountFromSource);
	register_method("getFriendFromSourceByIndex", &Steam::getFriendFromSourceByIndex);
	register_method("isUserInSource", &Steam::isUserInSource);
	register_method("setInGameVoiceSpeaking", &Steam::setInGameVoiceSpeaking);
	register_method("activateGameOverlay", &Steam::activateGameOverlay);
	register_method("activateGameOverlayToUser", &Steam::activateGameOverlayToUser);
	register_method("activateGameOverlayToWebPage", &Steam::activateGameOverlayToWebPage);
	register_method("activateGameOverlayToStore", &Steam::activateGameOverlayToStore);
	register_method("setPlayedWith", &Steam::setPlayedWith);
	register_method("activateGameOverlayInviteDialog", &Steam::activateGameOverlayInviteDialog);
	register_method("activateGameOverlayInviteDialogConnectString", &Steam::activateGameOverlayInviteDialogConnectString);
	register_method("getSmallFriendAvatar", &Steam::getSmallFriendAvatar);
	register_method("getMediumFriendAvatar", &Steam::getMediumFriendAvatar);
	register_method("getLargeFriendAvatar", &Steam::getLargeFriendAvatar);
	register_method("requestUserInformation", &Steam::requestUserInformation);
	register_method("requestClanOfficerList", &Steam::requestClanOfficerList);
	register_method("getClanOwner", &Steam::getClanOwner);
	register_method("getClanOfficerCount", &Steam::getClanOfficerCount);
	register_method("getClanOfficerByIndex", &Steam::getClanOfficerByIndex);
	register_method("getUserRestrictions", &Steam::getUserRestrictions);
	register_method("setRichPresence", &Steam::setRichPresence);
	register_method("clearRichPresence", &Steam::clearRichPresence);
	register_method("getFriendRichPresence", &Steam::getFriendRichPresence);
	register_method("getFriendRichPresenceKeyCount", &Steam::getFriendRichPresenceKeyCount);
	register_method("getFriendRichPresenceKeyByIndex", &Steam::getFriendRichPresenceKeyByIndex);
	register_method("requestFriendRichPresence", &Steam::requestFriendRichPresence);
	register_method("inviteUserToGame", &Steam::inviteUserToGame);
	register_method("joinClanChatRoom", &Steam::joinClanChatRoom);
	register_method("leaveClanChatRoom", &Steam::leaveClanChatRoom);
	register_method("getClanChatMemberCount", &Steam::getClanChatMemberCount);
	register_method("getChatMemberByIndex", &Steam::getChatMemberByIndex);
	register_method("sendClanChatMessage", &Steam::sendClanChatMessage);
	register_method("isClanChatAdmin", &Steam::isClanChatAdmin);
	register_method("isClanChatWindowOpenInSteam", &Steam::isClanChatWindowOpenInSteam);
	register_method("openClanChatWindowInSteam", &Steam::openClanChatWindowInSteam);
	register_method("closeClanChatWindowInSteam", &Steam::closeClanChatWindowInSteam);
	register_method("setListenForFriendsMessages", &Steam::setListenForFriendsMessages);
	register_method("replyToFriendMessage", &Steam::replyToFriendMessage);
	register_method("getFollowerCount", &Steam::getFollowerCount);
	register_method("isFollowing", &Steam::isFollowing);
	register_method("enumerateFollowingList", &Steam::enumerateFollowingList);
	register_method("isClanPublic", &Steam::isClanPublic);
	register_method("isClanOfficialGameGroup", &Steam::isClanOfficialGameGroup);
	register_method("getRecentPlayers", &Steam::getRecentPlayers);
	register_method("getPlayerAvatar", &Steam::getPlayerAvatar);
	register_method("getUserFriendsGroups", &Steam::getUserFriendsGroups);
	register_method("getUserSteamGroups", &Steam::getUserSteamGroups);
	register_method("getUserSteamFriends", &Steam::getUserSteamFriends);
	register_method("registerProtocolInOverlayBrowser", &Steam::registerProtocolInOverlayBrowser);
	
	// GAME SEARCH BIND METHODS /////////////////
	register_method("addGameSearchParams", &Steam::addGameSearchParams);
	register_method("searchForGameWithLobby", &Steam::searchForGameWithLobby);
	register_method("searchForGameSolo", &Steam::searchForGameSolo);
	register_method("acceptGame", &Steam::acceptGame);
	register_method("declineGame", &Steam::declineGame);
	register_method("retrieveConnectionDetails", &Steam::retrieveConnectionDetails);
	register_method("endGameSearch", &Steam::endGameSearch);
	register_method("setGameHostParams", &Steam::setGameHostParams);
	register_method("setConnectionDetails", &Steam::setConnectionDetails);
	register_method("requestPlayersForGame", &Steam::requestPlayersForGame);
	register_method("hostConfirmGameStart", &Steam::hostConfirmGameStart);
	register_method("cancelRequestPlayersForGame", &Steam::cancelRequestPlayersForGame);
	register_method("submitPlayerResult", &Steam::submitPlayerResult);
	register_method("endGame", &Steam::endGame);

	// HTML SURFACE BIND METHODS ////////////////
	register_method("addHeader", &Steam::addHeader);
	register_method("allowStartRequest", &Steam::allowStartRequest);
	register_method("copyToClipboard", &Steam::copyToClipboard);
	register_method("createBrowser", &Steam::createBrowser);
	register_method("executeJavascript", &Steam::executeJavascript);
	register_method("find", &Steam::find);
	register_method("getLinkAtPosition", &Steam::getLinkAtPosition);
	register_method("goBack", &Steam::goBack);
	register_method("goForward", &Steam::goForward);
	register_method("htmlInit", &Steam::htmlInit);
	register_method("jsDialogResponse", &Steam::jsDialogResponse);
	register_method("keyChar", &Steam::keyChar);
	register_method("keyDown", &Steam::keyDown);
	register_method("keyUp", &Steam::keyUp);
	register_method("loadURL", &Steam::loadURL);
	register_method("mouseDoubleClick", &Steam::mouseDoubleClick);
	register_method("mouseDown", &Steam::mouseDown);
	register_method("mouseMove", &Steam::mouseMove);
	register_method("mouseUp", &Steam::mouseUp);
	register_method("mouseWheel", &Steam::mouseWheel);
	register_method("pasteFromClipboard", &Steam::pasteFromClipboard);
	register_method("reload", &Steam::reload);
	register_method("removeBrowser", &Steam::removeBrowser);
	register_method("setBackgroundMode", &Steam::setBackgroundMode);
	register_method("setCookie", &Steam::setCookie);
	register_method("setHorizontalScroll", &Steam::setHorizontalScroll);
	register_method("setKeyFocus", &Steam::setKeyFocus);
	register_method("setPageScaleFactor", &Steam::setPageScaleFactor);
	register_method("setSize", &Steam::setSize);
	register_method("setVerticalScroll", &Steam::setVerticalScroll);
	register_method("htmlShutdown", &Steam::htmlShutdown);
	register_method("stopFind", &Steam::stopFind);
	register_method("stopLoad", &Steam::stopLoad);
	register_method("viewSource", &Steam::viewSource);

	// HTTP BIND METHODS ////////////////////////
	register_method("createCookieContainer", &Steam::createCookieContainer);
	register_method("createHTTPRequest", &Steam::createHTTPRequest);
	register_method("deferHTTPRequest", &Steam::deferHTTPRequest);
	register_method("getHTTPDownloadProgressPct", &Steam::getHTTPDownloadProgressPct);
	register_method("getHTTPRequestWasTimedOut", &Steam::getHTTPRequestWasTimedOut);
	register_method("getHTTPResponseBodyData", &Steam::getHTTPResponseBodyData);
	register_method("getHTTPResponseBodySize", &Steam::getHTTPResponseBodySize);
	register_method("getHTTPResponseHeaderSize", &Steam::getHTTPResponseHeaderSize);
	register_method("getHTTPResponseHeaderValue", &Steam::getHTTPResponseHeaderValue);
	register_method("getHTTPStreamingResponseBodyData", &Steam::getHTTPStreamingResponseBodyData);
	register_method("prioritizeHTTPRequest", &Steam::prioritizeHTTPRequest);
	register_method("releaseCookieContainer", &Steam::releaseCookieContainer);
	register_method("releaseHTTPRequest", &Steam::releaseHTTPRequest);
	register_method("sendHTTPRequest", &Steam::sendHTTPRequest);
	register_method("sendHTTPRequestAndStreamResponse", &Steam::sendHTTPRequestAndStreamResponse);
	register_method("setHTTPCookie", &Steam::setHTTPCookie);
	register_method("setHTTPRequestAbsoluteTimeoutMS", &Steam::setHTTPRequestAbsoluteTimeoutMS);
	register_method("setHTTPRequestContextValue", &Steam::setHTTPRequestContextValue);
	register_method("setHTTPRequestCookieContainer", &Steam::setHTTPRequestCookieContainer);
	register_method("setHTTPRequestGetOrPostParameter", &Steam::setHTTPRequestGetOrPostParameter);
	register_method("setHTTPRequestHeaderValue", &Steam::setHTTPRequestHeaderValue);
	register_method("setHTTPRequestNetworkActivityTimeout", &Steam::setHTTPRequestNetworkActivityTimeout);
	register_method("setHTTPRequestRawPostBody", &Steam::setHTTPRequestRawPostBody);
	register_method("setHTTPRequestRequiresVerifiedCertificate", &Steam::setHTTPRequestRequiresVerifiedCertificate);
	register_method("setHTTPRequestUserAgentInfo", &Steam::setHTTPRequestUserAgentInfo);

	// INPUT BIND METHODS /////////////////
	register_method("activateActionSet", &Steam::activateActionSet);
	register_method("activateActionSetLayer", &Steam::activateActionSetLayer);
	register_method("deactivateActionSetLayer", &Steam::deactivateActionSetLayer);
	register_method("deactivateAllActionSetLayers", &Steam::deactivateAllActionSetLayers);
	register_method("getActionSetHandle", &Steam::getActionSetHandle);
	register_method("getActionOriginFromXboxOrigin", &Steam::getActionOriginFromXboxOrigin);
	register_method("getActiveActionSetLayers", &Steam::getActiveActionSetLayers);
	register_method("getAnalogActionData", &Steam::getAnalogActionData);
	register_method("getAnalogActionHandle", &Steam::getAnalogActionHandle);
	register_method("getAnalogActionOrigins", &Steam::getAnalogActionOrigins);
	register_method("getConnectedControllers", &Steam::getConnectedControllers);
	register_method("getControllerForGamepadIndex", &Steam::getControllerForGamepadIndex);
	register_method("getCurrentActionSet", &Steam::getCurrentActionSet);
	register_method("getDeviceBindingRevision", &Steam::getDeviceBindingRevision);
	register_method("getDigitalActionData", &Steam::getDigitalActionData);
	register_method("getDigitalActionHandle", &Steam::getDigitalActionHandle);
	register_method("getDigitalActionOrigins", &Steam::getDigitalActionOrigins);
	register_method("getGamepadIndexForController", &Steam::getGamepadIndexForController);
	register_method("getGlyphForActionOrigin", &Steam::getGlyphForActionOrigin);
	register_method("getInputTypeForHandle", &Steam::getInputTypeForHandle);
	register_method("getMotionData", &Steam::getMotionData);
	register_method("getRemotePlaySessionID", &Steam::getRemotePlaySessionID);
	register_method("getStringForActionOrigin", &Steam::getStringForActionOrigin);
	register_method("inputInit", &Steam::inputInit);
	register_method("inputShutdown", &Steam::inputShutdown);
	register_method("runFrame", &Steam::runFrame);
	register_method("setLEDColor", &Steam::setLEDColor);
	register_method("showBindingPanel", &Steam::showBindingPanel);
	register_method("stopAnalogActionMomentum", &Steam::stopAnalogActionMomentum);
	register_method("translateActionOrigin", &Steam::translateActionOrigin);
	register_method("triggerHapticPulse", &Steam::triggerHapticPulse);
	register_method("triggerRepeatedHapticPulse", &Steam::triggerRepeatedHapticPulse);
	register_method("triggerVibration", &Steam::triggerVibration);
	register_method("setInputActionManifestFilePath", &Steam::setInputActionManifestFilePath);
	register_method("waitForData", &Steam::waitForData);
	register_method("newDataAvailable", &Steam::newDataAvailable);
	register_method("enableDeviceCallbacks", &Steam::enableDeviceCallbacks);
//	register_method("enableActionEventCallbacks", &Steam::enableActionEventCallbacks);
	register_method("getGlyphPNGForActionOrigin", &Steam::getGlyphPNGForActionOrigin);
	register_method("getGlyphSVGForActionOrigin", &Steam::getGlyphSVGForActionOrigin);
	register_method("triggerVibrationExtended", &Steam::triggerVibrationExtended);
	register_method("triggerSimpleHapticEvent", &Steam::triggerSimpleHapticEvent);
	register_method("getStringForXboxOrigin", &Steam::getStringForXboxOrigin);
	register_method("getGlyphForXboxOrigin", &Steam::getGlyphForXboxOrigin);
	register_method("getSessionInputConfigurationSettings", &Steam::getSessionInputConfigurationSettings);
	register_method("getStringForDigitalActionName", &Steam::getStringForDigitalActionName);
	register_method("getStringForAnalogActionName", &Steam::getStringForAnalogActionName);

	// INVENTORY BIND METHODS ///////////////////
	register_method("addPromoItem", &Steam::addPromoItem);
//	register_method("addPromoItems", &Steam::addPromoItems);
	register_method("checkResultSteamID", &Steam::checkResultSteamID);
	register_method("consumeItem", &Steam::consumeItem);
	register_method("deserializeResult", &Steam::deserializeResult);
	register_method("destroyResult", &Steam::destroyResult);
	register_method("exchangeItems", &Steam::exchangeItems);
	register_method("generateItems", &Steam::generateItems);
	register_method("getAllItems", &Steam::getAllItems);
	register_method("getItemDefinitionProperty", &Steam::getItemDefinitionProperty);
	register_method("getItemsByID", &Steam::getItemsByID);
	register_method("getItemPrice", &Steam::getItemPrice);
	register_method("getItemsWithPrices", &Steam::getItemsWithPrices);
	register_method("getNumItemsWithPrices", &Steam::getNumItemsWithPrices);
	register_method("getResultItemProperty", &Steam::getResultItemProperty);
	register_method("getResultItems", &Steam::getResultItems);
	register_method("getResultStatus", &Steam::getResultStatus);
	register_method("getResultTimestamp", &Steam::getResultTimestamp);
	register_method("grantPromoItems", &Steam::grantPromoItems);
	register_method("loadItemDefinitions", &Steam::loadItemDefinitions);
	register_method("requestEligiblePromoItemDefinitionsIDs", &Steam::requestEligiblePromoItemDefinitionsIDs);
	register_method("requestPrices", &Steam::requestPrices);
	register_method("serializeResult", &Steam::serializeResult);
	register_method("startPurchase", &Steam::startPurchase);
	register_method("transferItemQuantity", &Steam::transferItemQuantity);
	register_method("triggerItemDrop", &Steam::triggerItemDrop);
	register_method("startUpdateProperties", &Steam::startUpdateProperties);
	register_method("submitUpdateProperties", &Steam::submitUpdateProperties);
	register_method("removeProperty", &Steam::removeProperty);
	register_method("setPropertyString", &Steam::setPropertyString);
	register_method("setPropertyBool", &Steam::setPropertyBool);
	register_method("setPropertyInt", &Steam::setPropertyInt);
	register_method("setPropertyFloat", &Steam::setPropertyFloat);

	// MATCHMAKING BIND METHODS /////////////////
	register_method("getFavoriteGames", &Steam::getFavoriteGames);
	register_method("addFavoriteGame", &Steam::addFavoriteGame);
	register_method("removeFavoriteGame", &Steam::removeFavoriteGame);
	register_method("requestLobbyList", &Steam::requestLobbyList);
	register_method("addRequestLobbyListStringFilter", &Steam::addRequestLobbyListStringFilter);
	register_method("addRequestLobbyListNumericalFilter", &Steam::addRequestLobbyListNumericalFilter);
	register_method("addRequestLobbyListNearValueFilter", &Steam::addRequestLobbyListNearValueFilter);
	register_method("addRequestLobbyListFilterSlotsAvailable", &Steam::addRequestLobbyListFilterSlotsAvailable);
	register_method("addRequestLobbyListDistanceFilter", &Steam::addRequestLobbyListDistanceFilter);
	register_method("addRequestLobbyListResultCountFilter", &Steam::addRequestLobbyListResultCountFilter);
	register_method("createLobby", &Steam::createLobby);
	register_method("joinLobby", &Steam::joinLobby);
	register_method("leaveLobby", &Steam::leaveLobby);
	register_method("inviteUserToLobby", &Steam::inviteUserToLobby);
	register_method("getNumLobbyMembers", &Steam::getNumLobbyMembers);
	register_method("getLobbyMemberByIndex", &Steam::getLobbyMemberByIndex);
	register_method("getLobbyData", &Steam::getLobbyData);
	register_method("setLobbyData", &Steam::setLobbyData);
	register_method("getAllLobbyData", &Steam::getAllLobbyData);
	register_method("deleteLobbyData", &Steam::deleteLobbyData);
	register_method("getLobbyMemberData", &Steam::getLobbyMemberData);
	register_method("setLobbyMemberData", &Steam::setLobbyMemberData);
	register_method("sendLobbyChatMsg", &Steam::sendLobbyChatMsg);
	register_method("requestLobbyData", &Steam::requestLobbyData);
//	register_method("setLobbyGameServer", &Steam::setLobbyGameServer);
	register_method("getLobbyGameServer", &Steam::getLobbyGameServer);
	register_method("setLobbyMemberLimit", &Steam::setLobbyMemberLimit);
	register_method("getLobbyMemberLimit", &Steam::getLobbyMemberLimit);
	register_method("setLobbyType", &Steam::setLobbyType);
	register_method("setLobbyJoinable", &Steam::setLobbyJoinable);
	register_method("getLobbyOwner", &Steam::getLobbyOwner);
	register_method("setLobbyOwner", &Steam::setLobbyOwner);
	register_method("setLinkedLobby", &Steam::setLinkedLobby);

	// MATCHMAKING SERVERS BIND METHODS /////////
	register_method("cancelQuery", &Steam::cancelQuery);
	register_method("cancelServerQuery", &Steam::cancelServerQuery);
	register_method("getServerCount", &Steam::getServerCount);
	register_method("getServerDetails", &Steam::getServerDetails);
	register_method("isRefreshing", &Steam::isRefreshing);
//	register_method("pingServer", &Steam::pingServer);
	register_method("playerDetails", &Steam::playerDetails);
	register_method("refreshQuery", &Steam::refreshQuery);
	register_method("refreshServer", &Steam::refreshServer);
	register_method("releaseRequest", &Steam::releaseRequest);
	register_method("requestFavoritesServerList", &Steam::requestFavoritesServerList);
	register_method("requestFriendsServerList", &Steam::requestFriendsServerList);
	register_method("requestHistoryServerList", &Steam::requestHistoryServerList);
	register_method("requestInternetServerList", &Steam::requestInternetServerList);
	register_method("requestLANServerList", &Steam::requestLANServerList);
	register_method("requestSpectatorServerList", &Steam::requestSpectatorServerList);
	register_method("serverRules", &Steam::serverRules);
	
	// MUSIC BIND METHODS ///////////////////////
	register_method("musicIsEnabled", &Steam::musicIsEnabled);
	register_method("musicIsPlaying", &Steam::musicIsPlaying);
	register_method("musicGetVolume", &Steam::musicGetVolume);
	register_method("musicPause", &Steam::musicPause);
	register_method("musicPlay", &Steam::musicPlay);
	register_method("musicPlayNext", &Steam::musicPlayNext);
	register_method("musicPlayPrev", &Steam::musicPlayPrev);
	register_method("musicSetVolume", &Steam::musicSetVolume);
	
	// MUSIC REMOTE BIND METHODS ////////////////
	register_method("activationSuccess", &Steam::activationSuccess);
	register_method("isCurrentMusicRemote", &Steam::isCurrentMusicRemote);
	register_method("currentEntryDidChange", &Steam::currentEntryDidChange);
	register_method("currentEntryIsAvailable", &Steam::currentEntryIsAvailable);
	register_method("currentEntryWillChange", &Steam::currentEntryWillChange);
	register_method("deregisterSteamMusicRemote", &Steam::deregisterSteamMusicRemote);
	register_method("enableLooped", &Steam::enableLooped);
	register_method("enablePlaylists", &Steam::enablePlaylists);
	register_method("enablePlayNext", &Steam::enablePlayNext);
	register_method("enablePlayPrevious", &Steam::enablePlayPrevious);
	register_method("enableQueue", &Steam::enableQueue);
	register_method("enableShuffled", &Steam::enableShuffled);
	register_method("playlistDidChange", &Steam::playlistDidChange);
	register_method("playlistWillChange", &Steam::playlistWillChange);
	register_method("queueDidChange", &Steam::queueDidChange);
	register_method("queueWillChange", &Steam::queueWillChange);
	register_method("registerSteamMusicRemote", &Steam::registerSteamMusicRemote);
	register_method("resetPlaylistEntries", &Steam::resetPlaylistEntries);
	register_method("resetQueueEntries", &Steam::resetQueueEntries);
	register_method("setCurrentPlaylistEntry", &Steam::setCurrentPlaylistEntry);
	register_method("setCurrentQueueEntry", &Steam::setCurrentQueueEntry);
	register_method("setDisplayName", &Steam::setDisplayName);
	register_method("setPlaylistEntry", &Steam::setPlaylistEntry);
//	register_method("setPNGIcon64x64", &Steam::setPNGIcon64x64);
	register_method("setQueueEntry", &Steam::setQueueEntry);
//	register_method("updateCurrentEntryCoverArt", &Steam::updateCurrentEntryCoverArt);
	register_method("updateCurrentEntryElapsedSeconds", &Steam::updateCurrentEntryElapsedSeconds);
	register_method("updateCurrentEntryText", &Steam::updateCurrentEntryText);
	register_method("updateLooped", &Steam::updateLooped);
	register_method("updatePlaybackStatus", &Steam::updatePlaybackStatus);
	register_method("updateShuffled", &Steam::updateShuffled);
	register_method("updateVolume", &Steam::updateVolume);

	// NETWORKING BIND METHODS //////////////////
	register_method("acceptP2PSessionWithUser", &Steam::acceptP2PSessionWithUser);
	register_method("allowP2PPacketRelay", &Steam::allowP2PPacketRelay);
	register_method("closeP2PChannelWithUser", &Steam::closeP2PChannelWithUser);
	register_method("closeP2PSessionWithUser", &Steam::closeP2PSessionWithUser);
	register_method("getP2PSessionState", &Steam::getP2PSessionState);
	register_method("getAvailableP2PPacketSize", &Steam::getAvailableP2PPacketSize);
	register_method("readP2PPacket", &Steam::readP2PPacket);
	register_method("sendP2PPacket", &Steam::sendP2PPacket);
	
	// NETWORKING MESSAGES BIND METHODS /////////
	register_method("sendMessageToUser", &Steam::sendMessageToUser);
	register_method("receiveMessagesOnChannel", &Steam::receiveMessagesOnChannel);
	register_method("acceptSessionWithUser", &Steam::acceptSessionWithUser);
	register_method("closeSessionWithUser", &Steam::closeSessionWithUser);
	register_method("closeChannelWithUser", &Steam::closeChannelWithUser);
	register_method("getSessionConnectionInfo", &Steam::getSessionConnectionInfo);

	// NETWORKING SOCKETS BIND METHODS //////////
	register_method("createListenSocketIP", &Steam::createListenSocketIP);
	register_method("connectByIPAddress", &Steam::connectByIPAddress);
	register_method("createListenSocketP2P", &Steam::createListenSocketP2P);
	register_method("connectP2P", &Steam::connectP2P);
	register_method("acceptConnection", &Steam::acceptConnection);
	register_method("closeConnection", &Steam::closeConnection);
	register_method("closeListenSocket", &Steam::closeListenSocket);
	register_method("createSocketPair", &Steam::createSocketPair);
	register_method("sendMessageToConnection", &Steam::sendMessageToConnection);
	register_method("sendMessages", &Steam::sendMessages);
	register_method("flushMessagesOnConnection", &Steam::flushMessagesOnConnection);
	register_method("receiveMessagesOnConnection", &Steam::receiveMessagesOnConnection);
	register_method("createPollGroup", &Steam::createPollGroup);
	register_method("destroyPollGroup", &Steam::destroyPollGroup);
	register_method("setConnectionPollGroup", &Steam::setConnectionPollGroup);
	register_method("receiveMessagesOnPollGroup", &Steam::receiveMessagesOnPollGroup);
	register_method("getConnectionInfo", &Steam::getConnectionInfo);
	register_method("getQuickConnectionStatus", &Steam::getQuickConnectionStatus);
	register_method("getDetailedConnectionStatus", &Steam::getDetailedConnectionStatus);
	register_method("getConnectionUserData", &Steam::getConnectionUserData);
	register_method("setConnectionName", &Steam::setConnectionName);
	register_method("getConnectionName", &Steam::getConnectionName);
	register_method("getListenSocketAddress", &Steam::getListenSocketAddress);
	register_method("getIdentity", &Steam::getIdentity);
	register_method("initAuthentication", &Steam::initAuthentication);
	register_method("getAuthenticationStatus", &Steam::getAuthenticationStatus);
//	register_method("receivedRelayAuthTicket", &Steam::receivedRelayAuthTicket);	<------ Uses datagram relay structs which were removed from base SDK
//	register_method("findRelayAuthTicketForServer", &Steam::findRelayAuthTicketForServer);	<------ Uses datagram relay structs which were removed from base SDK
	register_method("connectToHostedDedicatedServer", &Steam::connectToHostedDedicatedServer);
	register_method("getHostedDedicatedServerPort", &Steam::getHostedDedicatedServerPort);
	register_method("getHostedDedicatedServerPOPId", &Steam::getHostedDedicatedServerPOPId);
//	register_method("getHostedDedicatedServerAddress", &Steam::getHostedDedicatedServerAddress);	<------ Uses datagram relay structs which were removed from base SDK
	register_method("createHostedDedicatedServerListenSocket", &Steam::createHostedDedicatedServerListenSocket);
//	register_method("getGameCoordinatorServerLogin", &Steam::getGameCoordinatorServerLogin);	<------ Uses datagram relay structs which were removed from base SDK

	// NETWORKING UTILS BIND METHODS ////////////
	register_method("initRelayNetworkAccess", &Steam::initRelayNetworkAccess);
	register_method("getRelayNetworkStatus", &Steam::getRelayNetworkStatus);
	register_method("getLocalPingLocation", &Steam::getLocalPingLocation);
	register_method("estimatePingTimeBetweenTwoLocations", &Steam::estimatePingTimeBetweenTwoLocations);
	register_method("estimatePingTimeFromLocalHost", &Steam::estimatePingTimeFromLocalHost);
	register_method("convertPingLocationToString", &Steam::convertPingLocationToString);
	register_method("parsePingLocationString", &Steam::parsePingLocationString);
	register_method("checkPingDataUpToDate", &Steam::checkPingDataUpToDate);
//	register_method("isPingMeasurementInProgress", &Steam::isPingMeasurementInProgress);
	register_method("getPingToDataCenter", &Steam::getPingToDataCenter);
	register_method("getDirectPingToPOP", &Steam::getDirectPingToPOP);
	register_method("getPOPCount", &Steam::getPOPCount);
	register_method("getPOPList", &Steam::getPOPList);
///	register_method("setConfigValue", &Steam::setConfigValue);
///	register_method("getConfigValue", &Steam::getConfigValue);
	register_method("getConfigValueInfo", &Steam::getConfigValueInfo);
	register_method("getFirstConfigValue", &Steam::getFirstConfigValue);
	register_method("setGlobalConfigValueInt32", &Steam::setGlobalConfigValueInt32);
	register_method("setGlobalConfigValueFloat", &Steam::setGlobalConfigValueFloat);
	register_method("setGlobalConfigValueString", &Steam::setGlobalConfigValueString);
	register_method("setConnectionConfigValueInt32", &Steam::setConnectionConfigValueInt32);
	register_method("setConnectionConfigValueFloat", &Steam::setConnectionConfigValueFloat);
	register_method("setConnectionConfigValueString", &Steam::setConnectionConfigValueString);
	register_method("allocateMessage", &Steam::allocateMessage);
	register_method("getLocalTimestamp", &Steam::getLocalTimestamp);
//	register_method("setDebugOutputFunction", &Steam::setDebugOutputFunction);

	// PARENTAL SETTINGS BIND METHODS ///////////
	register_method("isParentalLockEnabled", &Steam::isParentalLockEnabled);
	register_method("isParentalLockLocked", &Steam::isParentalLockLocked);
	register_method("isAppBlocked", &Steam::isAppBlocked);
	register_method("isAppInBlockList", &Steam::isAppInBlockList);
	register_method("isFeatureBlocked", &Steam::isFeatureBlocked);
	register_method("isFeatureInBlockList", &Steam::isFeatureInBlockList);

	// PARTIES BIND METHODS /////////////////////
	register_method("getAvailableBeaconLocations", &Steam::getAvailableBeaconLocations);
	register_method("createBeacon", &Steam::createBeacon);
	register_method("onReservationCompleted", &Steam::onReservationCompleted);
	register_method("changeNumOpenSlots", &Steam::changeNumOpenSlots);
	register_method("destroyBeacon", &Steam::destroyBeacon);
	register_method("getNumActiveBeacons", &Steam::getNumActiveBeacons);
	register_method("getBeaconByIndex", &Steam::getBeaconByIndex);
	register_method("getBeaconDetails", &Steam::getBeaconDetails);
	register_method("joinParty", &Steam::joinParty);
	register_method("getBeaconLocationData", &Steam::getBeaconLocationData);

	// REMOTE PLAY BIND METHODS /////////////////
	register_method("getSessionCount", &Steam::getSessionCount);
	register_method("getSessionID", &Steam::getSessionID);
	register_method("getSessionSteamID", &Steam::getSessionSteamID);
	register_method("getSessionClientName", &Steam::getSessionClientName);
	register_method("getSessionClientFormFactor", &Steam::getSessionClientFormFactor);
	register_method("getSessionClientResolution", &Steam::getSessionClientResolution);
	register_method("sendRemotePlayTogetherInvite", &Steam::sendRemotePlayTogetherInvite);

	// REMOTE STORAGE BIND METHODS //////////////
	register_method("fileWrite", &Steam::fileWrite);
	register_method("fileRead", &Steam::fileRead);
	register_method("fileForget", &Steam::fileForget);
	register_method("fileDelete", &Steam::fileDelete);
	register_method("fileExists", &Steam::fileExists);
	register_method("filePersisted", &Steam::filePersisted);
	register_method("getFileSize", &Steam::getFileSize);
	register_method("getFileTimestamp", &Steam::getFileTimestamp);
	register_method("getFileCount", &Steam::getFileCount);
	register_method("getFileNameAndSize", &Steam::getFileNameAndSize);
	register_method("getQuota", &Steam::getQuota);
	register_method("getSyncPlatforms", &Steam::getSyncPlatforms);
	register_method("isCloudEnabledForAccount", &Steam::isCloudEnabledForAccount);
	register_method("isCloudEnabledForApp", &Steam::isCloudEnabledForApp);
	register_method("setCloudEnabledForApp", &Steam::setCloudEnabledForApp);
	register_method("setSyncPlatforms", &Steam::setSyncPlatforms);
	register_method("ugcDownload", &Steam::ugcDownload);
	register_method("ugcDownloadToLocation", &Steam::ugcDownloadToLocation);
	register_method("ugcRead", &Steam::ugcRead);
	register_method("beginFileWriteBatch", &Steam::beginFileWriteBatch);
	register_method("endFileWriteBatch", &Steam::endFileWriteBatch);
	register_method("getLocalFileChangeCount", &Steam::getLocalFileChangeCount);
	register_method("getLocalFileChange", &Steam::getLocalFileChange);

	// SCREENSHOT BIND METHODS //////////////////
	register_method("addScreenshotToLibrary", &Steam::addScreenshotToLibrary);
	register_method("addVRScreenshotToLibrary", &Steam::addVRScreenshotToLibrary);
	register_method("hookScreenshots", &Steam::hookScreenshots);
	register_method("isScreenshotsHooked", &Steam::isScreenshotsHooked);
	register_method("setLocation", &Steam::setLocation);
	register_method("tagPublishedFile", &Steam::tagPublishedFile);
	register_method("taguser", &Steam::tagUser);
	register_method("triggerScreenshot", &Steam::triggerScreenshot);
	register_method("writeScreenshot", &Steam::writeScreenshot);
	
	// UGC BIND METHODS ////////////////////
	register_method("addAppDependency", &Steam::addAppDependency);
	register_method("addDependency", &Steam::addDependency);
	register_method("addExcludedTag", &Steam::addExcludedTag);
	register_method("addItemKeyValueTag", &Steam::addItemKeyValueTag);
	register_method("addItemPreviewFile", &Steam::addItemPreviewFile);
	register_method("addItemPreviewVideo", &Steam::addItemPreviewVideo);
	register_method("addItemToFavorites", &Steam::addItemToFavorites);
	register_method("addRequiredKeyValueTag", &Steam::addRequiredKeyValueTag);
	register_method("addRequiredTag", &Steam::addRequiredTag);
	register_method("addRequiredTagGroup", &Steam::addRequiredTagGroup);
	register_method("initWorkshopForGameServer", &Steam::initWorkshopForGameServer);
	register_method("createItem", &Steam::createItem);
	register_method("createQueryAllUGCRequest", &Steam::createQueryAllUGCRequest);
	register_method("createQueryUGCDetailsRequest", &Steam::createQueryUGCDetailsRequest);
	register_method("createQueryUserUGCRequest", &Steam::createQueryUserUGCRequest);
	register_method("deleteItem", &Steam::deleteItem);
	register_method("downloadItem", &Steam::downloadItem);
	register_method("getItemDownloadInfo", &Steam::getItemDownloadInfo);
	register_method("getItemInstallInfo", &Steam::getItemInstallInfo);
	register_method("getItemState", &Steam::getItemState);
	register_method("getItemUpdateProgress", &Steam::getItemUpdateProgress);
	register_method("getSubscribedItems", &Steam::getSubscribedItems);
	register_method("getQueryUGCAdditionalPreview", &Steam::getQueryUGCAdditionalPreview);
	register_method("getQueryUGCChildren", &Steam::getQueryUGCChildren);
	register_method("getQueryUGCKeyValueTag", &Steam::getQueryUGCKeyValueTag);
	register_method("getQueryUGCMetadata", &Steam::getQueryUGCMetadata);
	register_method("getQueryUGCNumAdditionalPreviews", &Steam::getQueryUGCNumAdditionalPreviews);
	register_method("getQueryUGCNumKeyValueTags", &Steam::getQueryUGCNumKeyValueTags);
	register_method("getQueryUGCNumTags", &Steam::getQueryUGCNumTags);
	register_method("getQueryUGCPreviewURL", &Steam::getQueryUGCPreviewURL);
	register_method("getQueryUGCResult", &Steam::getQueryUGCResult);
	register_method("getQueryUGCStatistic", &Steam::getQueryUGCStatistic);
	register_method("getQueryUGCTag", &Steam::getQueryUGCTag);
	register_method("getQueryUGCTagDisplayName", &Steam::getQueryUGCTagDisplayName);
	register_method("getUserItemVote", &Steam::getUserItemVote);
	register_method("releaseQueryUGCRequest", &Steam::releaseQueryUGCRequest);
	register_method("removeAppDependency", &Steam::removeAppDependency);
	register_method("removeDependency", &Steam::removeDependency);
	register_method("removeItemFromFavorites", &Steam::removeItemFromFavorites);
	register_method("removeItemKeyValueTags", &Steam::removeItemKeyValueTags);
	register_method("removeItemPreview", &Steam::removeItemPreview);
	register_method("sendQueryUGCRequest", &Steam::sendQueryUGCRequest);
	register_method("setAllowCachedResponse", &Steam::setAllowCachedResponse);
	register_method("setCloudFileNameFilter", &Steam::setCloudFileNameFilter);
	register_method("setItemContent", &Steam::setItemContent);
	register_method("setItemDescription", &Steam::setItemDescription);
	register_method("setItemMetadata", &Steam::setItemMetadata);
	register_method("setItemPreview", &Steam::setItemPreview);
	register_method("setItemTags", &Steam::setItemTags);
	register_method("setItemTitle", &Steam::setItemTitle);
	register_method("setItemUpdateLanguage", &Steam::setItemUpdateLanguage);
	register_method("setItemVisibility", &Steam::setItemVisibility);
	register_method("setLanguage", &Steam::setLanguage);
	register_method("setMatchAnyTag", &Steam::setMatchAnyTag);
	register_method("setRankedByTrendDays", &Steam::setRankedByTrendDays);
	register_method("setReturnAdditionalPreviews", &Steam::setReturnAdditionalPreviews);
	register_method("setReturnChildren", &Steam::setReturnChildren);
	register_method("setReturnKeyValueTags", &Steam::setReturnKeyValueTags);
	register_method("setReturnLongDescription", &Steam::setReturnLongDescription);
	register_method("setReturnMetadata", &Steam::setReturnMetadata);
	register_method("setReturnOnlyIDs", &Steam::setReturnOnlyIDs);
	register_method("setReturnPlaytimeStats", &Steam::setReturnPlaytimeStats);
	register_method("setReturnTotalOnly", &Steam::setReturnTotalOnly);
	register_method("setSearchText", &Steam::setSearchText);
	register_method("setUserItemVote", &Steam::setUserItemVote);
	register_method("startItemUpdate", &Steam::startItemUpdate);
	register_method("startPlaytimeTracking", &Steam::startPlaytimeTracking);
	register_method("stopPlaytimeTracking", &Steam::stopPlaytimeTracking);
	register_method("stopPlaytimeTrackingForAllItems", &Steam::stopPlaytimeTrackingForAllItems);
	register_method("getAppDependencies", &Steam::getAppDependencies);
	register_method("submitItemUpdate", &Steam::submitItemUpdate);
	register_method("subscribeItem", &Steam::subscribeItem);
	register_method("suspendDownloads", &Steam::suspendDownloads);
	register_method("unsubscribeItem", &Steam::unsubscribeItem);
	register_method("updateItemPreviewFile", &Steam::updateItemPreviewFile);
	register_method("updateItemPreviewVideo", &Steam::updateItemPreviewVideo);
	register_method("showWorkshopEULA", &Steam::showWorkshopEULA);
	register_method("getWorkshopEULAStatus", &Steam::getWorkshopEULAStatus);

	// USER BIND METHODS ////////////////////////
//	register_method("advertiseGame", &Steam::advertiseGame);
	register_method("beginAuthSession", &Steam::beginAuthSession);
	register_method("cancelAuthTicket", &Steam::cancelAuthTicket);
	register_method("decompressVoice", &Steam::decompressVoice);
	register_method("endAuthSession", &Steam::endAuthSession);
	register_method("getAuthSessionTicket", &Steam::getAuthSessionTicket);
	register_method("getAvailableVoice", &Steam::getAvailableVoice);
	register_method("getDurationControl", &Steam::getDurationControl);
	register_method("getEncryptedAppTicket", &Steam::getEncryptedAppTicket);
	register_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
	register_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
	register_method("getSteamID", &Steam::getSteamID);
	register_method("getUserDataFolder", &Steam::getUserDataFolder);
	register_method("getVoice", &Steam::getVoice);
	register_method("getVoiceOptimalSampleRate", &Steam::getVoiceOptimalSampleRate);
	register_method("initiateGameConnection", &Steam::initiateGameConnection);
	register_method("isBehindNAT", &Steam::isBehindNAT);
	register_method("isPhoneIdentifying", &Steam::isPhoneIdentifying);
	register_method("isPhoneRequiringVerification", &Steam::isPhoneRequiringVerification);
	register_method("isPhoneVerified", &Steam::isPhoneVerified);
	register_method("isTwoFactorEnabled", &Steam::isTwoFactorEnabled);
	register_method("loggedOn", &Steam::loggedOn);
	register_method("requestEncryptedAppTicket", &Steam::requestEncryptedAppTicket);
	register_method("requestStoreAuthURL", &Steam::requestStoreAuthURL);
	register_method("startVoiceRecording", &Steam::startVoiceRecording);
	register_method("stopVoiceRecording", &Steam::stopVoiceRecording);
	register_method("terminateGameConnection", &Steam::terminateGameConnection);
	register_method("userHasLicenseForApp", &Steam::userHasLicenseForApp);
	
	// User Stats Bind Methods //////////////////
	register_method("attachLeaderboardUGC", &Steam::attachLeaderboardUGC);
	register_method("clearAchievement", &Steam::clearAchievement);
	register_method("downloadLeaderboardEntries", &Steam::downloadLeaderboardEntries);
	register_method("downloadLeaderboardEntriesForUsers", &Steam::downloadLeaderboardEntriesForUsers);
	register_method("findLeaderboard", &Steam::findLeaderboard);
	register_method("findOrCreateLeaderboard", &Steam::findOrCreateLeaderboard);
	register_method("getAchievement", &Steam::getAchievement);
	register_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
	register_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
	register_method("getAchievementDisplayAttribute", &Steam::getAchievementDisplayAttribute);
	register_method("getAchievementIcon", &Steam::getAchievementIcon);
	register_method("getAchievementName", &Steam::getAchievementName);
	register_method("getGlobalStatInt", &Steam::getGlobalStatInt);
	register_method("getGlobalStatFloat", &Steam::getGlobalStatFloat);
	register_method("getGlobalStatIntHistory", &Steam::getGlobalStatIntHistory);
	register_method("getGlobalStatFloatHistory", &Steam::getGlobalStatFloatHistory);
	register_method("getLeaderboardDisplayType", &Steam::getLeaderboardDisplayType);
	register_method("getLeaderboardEntryCount",  &Steam::getLeaderboardEntryCount);
	register_method("getLeaderboardName", &Steam::getLeaderboardName);
	register_method("getLeaderboardSortMethod", &Steam::getLeaderboardSortMethod);
	register_method("getMostAchievedAchievementInfo", &Steam::getMostAchievedAchievementInfo);
	register_method("getNextMostAchievedAchievementInfo", &Steam::getNextMostAchievedAchievementInfo);
	register_method("getNumAchievements", &Steam::getNumAchievements);
	register_method("getNumberOfCurrentPlayers", &Steam::getNumberOfCurrentPlayers);
	register_method("getStatFloat", &Steam::getStatFloat);
	register_method("getStatInt", &Steam::getStatInt);
	register_method("getUserAchievement", &Steam::getUserAchievement);
	register_method("getUserAchievementAndUnlockTime", &Steam::getUserAchievementAndUnlockTime);
	register_method("getUserStatFloat", &Steam::getUserStatFloat);
	register_method("getUserStatInt", &Steam::getUserStatInt);
	register_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);
	register_method("requestCurrentStats", &Steam::requestCurrentStats);
	register_method("requestGlobalAchievementPercentages", &Steam::requestGlobalAchievementPercentages);
	register_method("requestGlobalStats", &Steam::requestGlobalStats);
	register_method("requestUserStats", &Steam::requestUserStats);
	register_method("resetAllStats", &Steam::resetAllStats);
	register_method("setAchievement", &Steam::setAchievement);
	register_method("setStatFloat", &Steam::setStatFloat);
	register_method("setStatInt", &Steam::setStatInt);
	register_method("storeStats", &Steam::storeStats);
	register_method("updateAvgRateStat", &Steam::updateAvgRateStat);
	register_method("uploadLeaderboardScore", &Steam::uploadLeaderboardScore);
	register_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
	register_method("getAchievementProgressLimitsInt", &Steam::getAchievementProgressLimitsInt);
	register_method("getAchievementProgressLimitsFloat", &Steam::getAchievementProgressLimitsFloat);

	// UTILS BIND METHODS ///////////////////////
	register_method("overlayNeedsPresent", &Steam::overlayNeedsPresent);
	register_method("getAppID", &Steam::getAppID);
	register_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
	register_method("getImageRGBA", &Steam::getImageRGBA);
	register_method("getImageSize", &Steam::getImageSize);
	register_method("getIPCCallCount", &Steam::getIPCCallCount);
	register_method("getIPCountry", &Steam::getIPCountry);
	register_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
	register_method("getSecondsSinceComputerActive", &Steam::getSecondsSinceComputerActive);
	register_method("getServerRealTime", &Steam::getServerRealTime);
	register_method("getSteamUILanguage", &Steam::getSteamUILanguage);
	register_method("isOverlayEnabled", &Steam::isOverlayEnabled);
	register_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
	register_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
	register_method("isVRHeadsetStreamingEnabled", &Steam::isVRHeadsetStreamingEnabled);
	register_method("setOverlayNotificationInset", &Steam::setOverlayNotificationInset);
	register_method("setOverlayNotificationPosition", &Steam::setOverlayNotificationPosition);
	register_method("setVRHeadsetStreamingEnabled", &Steam::setVRHeadsetStreamingEnabled);
	register_method("showGamepadTextInput", &Steam::showGamepadTextInput);
	register_method("startVRDashboard", &Steam::startVRDashboard);
	register_method("filterText", &Steam::filterText);
	register_method("getAPICallFailureReason", &Steam::getAPICallFailureReason);
	register_method("initFilterText", &Steam::initFilterText);
	register_method("isAPICallCompleted", &Steam::isAPICallCompleted);
	register_method("isSteamChinaLauncher", &Steam::isSteamChinaLauncher);
	register_method("isSteamChinaLauncher", &Steam::isSteamChinaLauncher);
	register_method("isSteamRunningOnSteamDeck", &Steam::isSteamRunningOnSteamDeck);
	register_method("showFloatingGamepadTextInput", &Steam::showFloatingGamepadTextInput);
	register_method("setGameLauncherMode", &Steam::setGameLauncherMode);

	/////////////////////////////////////////////
	// CALLBACK SIGNAL BINDS ////////////////////
	/////////////////////////////////////////////
	//
	// STEAMWORKS SIGNALS ///////////////////////
	register_signal<Steam>("steamworks_error", "failed_signal", GODOT_VARIANT_TYPE_STRING, "io_failure", GODOT_VARIANT_TYPE_STRING);

	// APPS SIGNALS /////////////////////////////
	register_signal<Steam>("file_details_result", "result", GODOT_VARIANT_TYPE_INT, "fileSize", GODOT_VARIANT_TYPE_INT, "fileHash", GODOT_VARIANT_TYPE_INT, "flags", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("dlc_installed", "app", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("new_launch_url_parameters", godot::Dictionary());
//	register_signal<Steam>("new_launch_query_parameters");
	register_signal<Steam>("register_activation_code_response", godot::Dictionary());
	register_signal<Steam>("app_proof_of_purchase_key_response", godot::Dictionary());
	register_signal<Steam>("timed_trial_status", godot::Dictionary());
	
	// APP LIST SIGNALS /////////////////////////
	register_signal<Steam>("app_installed", godot::Dictionary());
	register_signal<Steam>("app_uninstalled", godot::Dictionary());

	// FRIENDS SIGNALS //////////////////////////
	register_signal<Steam>("avatar_loaded", "steamID", GODOT_VARIANT_TYPE_INT, "width", GODOT_VARIANT_TYPE_INT, "data", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
	register_signal<Steam>("request_clan_officer_list", "message", GODOT_VARIANT_TYPE_STRING, "officers_list", GODOT_VARIANT_TYPE_ARRAY);
	register_signal<Steam>("clan_activity_downloaded", "activity", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("friend_rich_presence_update", "steamID", GODOT_VARIANT_TYPE_INT, "appID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("enumerate_following_list", "message", GODOT_VARIANT_TYPE_STRING, "following", GODOT_VARIANT_TYPE_ARRAY);
	register_signal<Steam>("get_follower_count", "result", GODOT_VARIANT_TYPE_INT, "steamID", GODOT_VARIANT_TYPE_INT, "count", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("is_following", "result", GODOT_VARIANT_TYPE_INT, "steamID", GODOT_VARIANT_TYPE_INT, "following", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("connected_chat_join", "chatID", GODOT_VARIANT_TYPE_INT, "steamID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("connected_chat_leave", "chatID", GODOT_VARIANT_TYPE_INT, "steamID", GODOT_VARIANT_TYPE_INT, "kicked", GODOT_VARIANT_TYPE_BOOL, "dropped", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("connected_clan_chat_message", "chat", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("_connected_friend_chat_message", "chat", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("join_requested", "from", GODOT_VARIANT_TYPE_INT, "connect_string", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("overlay_toggled", "active", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("join_game_requested", "lobby", GODOT_VARIANT_TYPE_INT, "steamID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("change_server_requested", "server", GODOT_VARIANT_TYPE_STRING, "password", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("join_clan_chat_complete", "chatID", GODOT_VARIANT_TYPE_INT, "response", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("persona_state_change", "steamID", GODOT_VARIANT_TYPE_INT, "flags", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("name_changed", "success", GODOT_VARIANT_TYPE_BOOL, "local_success", GODOT_VARIANT_TYPE_BOOL, "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("overlay_browser_protocol", "uri", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("unread_chat_messages_changed", godot::Dictionary());

	// GAME SERVER SIGNALS //////////////////////
	register_signal<Steam>("search_for_game_progress", "result", GODOT_VARIANT_TYPE_INT, "searchID", GODOT_VARIANT_TYPE_INT, "search_progress", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("search_for_game_result", "result", GODOT_VARIANT_TYPE_INT, "searchID", GODOT_VARIANT_TYPE_INT, "search_result", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("request_players_for_game_progress", "result", GODOT_VARIANT_TYPE_INT, "searchID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("request_players_for_game_result", "result", GODOT_VARIANT_TYPE_INT, "searchID", GODOT_VARIANT_TYPE_INT, "player_data", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("request_players_for_game_final_result", "result", GODOT_VARIANT_TYPE_INT, "searchID", GODOT_VARIANT_TYPE_INT, "gameID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("submit_player_result", "result", GODOT_VARIANT_TYPE_INT, "gameID", GODOT_VARIANT_TYPE_INT, "playerID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("end_game_result", "result", GODOT_VARIANT_TYPE_INT, "gameID", GODOT_VARIANT_TYPE_INT);

	// HTML SURFACE SIGNALS /////////////////////
	register_signal<Steam>("html_browser_ready", godot::Dictionary());
	register_signal<Steam>("html_can_go_backandforward", "go_back", GODOT_VARIANT_TYPE_BOOL, "go_forward", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("html_changed_title", "title", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_close_browser", godot::Dictionary());
	register_signal<Steam>("html_file_open_dialog", "title", GODOT_VARIANT_TYPE_STRING, "initial_file", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_finished_request", "url", GODOT_VARIANT_TYPE_STRING, "title", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_hide_tooltip", godot::Dictionary());
	register_signal<Steam>("html_horizontal_scroll", "scroll_data", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("html_js_alert", "message", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_js_confirm", "message", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_link_at_position", "link_data", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("html_needs_paint", "page_data", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("html_new_window", "window_data", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("html_open_link_in_new_tab", "url", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_search_results", "results", GODOT_VARIANT_TYPE_INT, "current_match", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("html_set_cursor", "mouse_cursor", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("html_show_tooltip", "message", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_start_request", "url", GODOT_VARIANT_TYPE_STRING, "target", GODOT_VARIANT_TYPE_STRING, "post_data", GODOT_VARIANT_TYPE_STRING, "redirect", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("html_status_text", "message", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_update_tooltip", "message", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("html_url_changed", "url_data", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("html_vertical_scroll", "scroll_data", GODOT_VARIANT_TYPE_DICTIONARY);

	// HTTP SIGNALS /////////////////////////////
	register_signal<Steam>("http_request_completed", "context_value", GODOT_VARIANT_TYPE_INT, "request_success", GODOT_VARIANT_TYPE_BOOL, "status_code", GODOT_VARIANT_TYPE_INT, "body_size", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("http_request_data_received", "context_value", GODOT_VARIANT_TYPE_INT, "offset", GODOT_VARIANT_TYPE_INT, "bytes_received", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("http_request_headers_received", "context_value", GODOT_VARIANT_TYPE_INT);

	// INPUT SIGNALS ////////////////////////////
//	register_signal<Steam>("input_action_event", godot::Dictionary());
	register_signal<Steam>("input_device_connected", godot::Dictionary());
	register_signal<Steam>("input_device_disconnected", godot::Dictionary());
	register_signal<Steam>("input_configuration_loaded", godot::Dictionary());

	// INVENTORY SIGNALS ////////////////////////
	register_signal<Steam>("inventory_definition_update", "definitions", GODOT_VARIANT_TYPE_ARRAY);
	register_signal<Steam>("inventory_eligible_promo_item", "result", GODOT_VARIANT_TYPE_INT, "cached", GODOT_VARIANT_TYPE_BOOL, "definitions", GODOT_VARIANT_TYPE_ARRAY);
	register_signal<Steam>("inventory_full_update", "inventory_handle", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("inventory_result_ready", "result", GODOT_VARIANT_TYPE_INT, "inventory_handle", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("inventory_start_purchase_result", "result", GODOT_VARIANT_TYPE_STRING, "order_id", GODOT_VARIANT_TYPE_INT, "transaction_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("inventory_request_prices_result", "result", GODOT_VARIANT_TYPE_INT, "currency", GODOT_VARIANT_TYPE_STRING);

	// MATCHMAKING SIGNALS //////////////////////
	register_signal<Steam>("favorites_list_accounts_updated", "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("favorites_list_changed", "favorite", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("lobby_message", "result", GODOT_VARIANT_TYPE_INT, "user", GODOT_VARIANT_TYPE_INT, "buffer", GODOT_VARIANT_TYPE_STRING, "chat_type", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("lobby_chat_update", "lobby_id", GODOT_VARIANT_TYPE_INT, "changed_id", GODOT_VARIANT_TYPE_INT, "making_change_id", GODOT_VARIANT_TYPE_INT, "chat_state", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("lobby_created", "connect", GODOT_VARIANT_TYPE_INT, "lobby", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("lobby_data_update", "success", GODOT_VARIANT_TYPE_INT, "lobby_id", GODOT_VARIANT_TYPE_INT, "member_id", GODOT_VARIANT_TYPE_INT, "key", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("lobby_joined", "lobby", GODOT_VARIANT_TYPE_INT, "permissions", GODOT_VARIANT_TYPE_INT, "locked", GODOT_VARIANT_TYPE_BOOL, "response", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("lobby_game_created", "lobby_id", GODOT_VARIANT_TYPE_INT, "server_id", GODOT_VARIANT_TYPE_INT, "server_ip", GODOT_VARIANT_TYPE_INT, "port", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("lobby_invite", "inviter", GODOT_VARIANT_TYPE_INT, "lobby", GODOT_VARIANT_TYPE_INT, "game", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("lobby_match_list", "lobbies", GODOT_VARIANT_TYPE_ARRAY);
	register_signal<Steam>("lobby_kicked", "lobby_id", GODOT_VARIANT_TYPE_INT, "admin_id", GODOT_VARIANT_TYPE_INT, "due_to_disconnect", GODOT_VARIANT_TYPE_INT);
	
	// MUSIC REMOTE SIGNALS /////////////////////
	register_signal<Steam>("music_player_remote_to_front", godot::Dictionary());
	register_signal<Steam>("music_player_remote_will_activate", godot::Dictionary());
	register_signal<Steam>("music_player_remote_will_deactivate", godot::Dictionary());
	register_signal<Steam>("music_player_selects_playlist_entry", "entry", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("music_player_selects_queue_entry", "entry", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("music_player_wants_looped", "looped", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("music_player_wants_pause", godot::Dictionary());
	register_signal<Steam>("music_player_wants_playing_repeat_status", "status", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("music_player_wants_play_next", godot::Dictionary());
	register_signal<Steam>("music_player_wants_play_previous", godot::Dictionary());
	register_signal<Steam>("music_player_wants_play", godot::Dictionary());
	register_signal<Steam>("music_player_wants_shuffled", "shuffled", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("music_player_wants_volume", "volume", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("music_player_will_quit", godot::Dictionary());

	// NETWORKING SIGNALS ///////////////////////
	register_signal<Steam>("p2p_session_request", "steamID_remote", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("p2p_session_connect_fail", "steamID_remote", GODOT_VARIANT_TYPE_INT, "session_error", GODOT_VARIANT_TYPE_INT);

	// NETWORKING MESSAGES //////////////////////
	register_signal<Steam>("network_messages_session_request", godot::Dictionary());
	register_signal<Steam>("network_messages_session_failed", "reason", GODOT_VARIANT_TYPE_INT);

	// NETWORKING SOCKETS SIGNALS ///////////////
	register_signal<Steam>("network_connection_status_changed", "connect_handle", GODOT_VARIANT_TYPE_INT, "old_state", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("network_authentication_status", "available", GODOT_VARIANT_TYPE_INT, "debug_message", GODOT_VARIANT_TYPE_STRING);

	// NETWORKING UTILS SIGNALS /////////////////
	register_signal<Steam>("relay_network_status", "available", GODOT_VARIANT_TYPE_INT, "ping_measurement", GODOT_VARIANT_TYPE_INT, "available_relay", GODOT_VARIANT_TYPE_INT, "debug_message", GODOT_VARIANT_TYPE_STRING);

	// PARTIES //////////////////////////////////
	register_signal<Steam>("join_party", "result", GODOT_VARIANT_TYPE_INT, "beacon_id", GODOT_VARIANT_TYPE_INT, "steam_id", GODOT_VARIANT_TYPE_INT, "connect_string", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("create_beacon", "result", GODOT_VARIANT_TYPE_INT, "beacon_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("reservation_notification", "beacon_id", GODOT_VARIANT_TYPE_INT, "steam_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("change_num_open_slots", "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("available_beacon_locations_updated", godot::Dictionary());
	register_signal<Steam>("active_beacons_updated", godot::Dictionary());

	// REMOTE PLAY SIGNALS //////////////////////
	register_signal<Steam>("remote_play_session_connected", "session_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("remote_play_session_disconnected", "session_id", GODOT_VARIANT_TYPE_INT);

	// REMOTE STORAGE SIGNALS ///////////////////
	register_signal<Steam>("file_read_async_complete", "handle", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT, "offset", GODOT_VARIANT_TYPE_INT, "read", GODOT_VARIANT_TYPE_INT, "complete", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("file_share_result", "result", GODOT_VARIANT_TYPE_INT, "handle", GODOT_VARIANT_TYPE_INT, "name", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("file_write_async_complete", "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("download_ugc_result", "result", GODOT_VARIANT_TYPE_INT, "download_data", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("unsubscribe_item", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("subscribe_item", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("local_file_changed", godot::Dictionary());

	// SCREENSHOT SIGNALS ///////////////////////
	register_signal<Steam>("screenshot_ready", "screenshot_handle", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("screenshot_requested", godot::Dictionary());

	// UGC SIGNALS //////////////////////////////
	register_signal<Steam>("add_app_dependency_result", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "app_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("add_ugc_dependency_result", "result", GODOT_VARIANT_TYPE_INT, "fild_id", GODOT_VARIANT_TYPE_INT, "child_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("item_created", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "accept_tos", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("item_downloaded", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "app_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("get_app_dependencies_result", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "app_dependencies", GODOT_VARIANT_TYPE_INT, "total_app_dependencies", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("item_deleted", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("get_item_vote_result", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "vote_up", GODOT_VARIANT_TYPE_BOOL, "vote_down", GODOT_VARIANT_TYPE_BOOL, "vote_skipped", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("item_installed", "app_id", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("remove_app_dependency_result", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "app_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("remove_ugc_dependency_result", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "child_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("set_user_item_vote", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "vote_up", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("start_playtime_tracking", "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("ugc_query_completed", "handle", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT, "results_returned", GODOT_VARIANT_TYPE_INT, "total_matching", GODOT_VARIANT_TYPE_INT, "cached", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("stop_playtime_tracking", "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("item_updated", "result", GODOT_VARIANT_TYPE_INT, "accept_tos", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("user_favorite_items_list_changed", "result", GODOT_VARIANT_TYPE_INT, "file_id", GODOT_VARIANT_TYPE_INT, "was_add_request", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("workshop_eula_status", godot::Dictionary());
	register_signal<Steam>("user_subscribed_items_list_changed", godot::Dictionary());

	// USER SIGNALS /////////////////////////////
	register_signal<Steam>("client_game_server_deny", "app_id", GODOT_VARIANT_TYPE_INT, "ip", GODOT_VARIANT_TYPE_INT, "server_port", GODOT_VARIANT_TYPE_INT, "secure", GODOT_VARIANT_TYPE_INT, "reason", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("duration_control", "result", GODOT_VARIANT_TYPE_INT, "duration", GODOT_VARIANT_TYPE_DICTIONARY);
	register_signal<Steam>("encrypted_app_ticket_response", "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("game_web_callback", "url", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("get_auth_session_ticket_response", "ticket", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("ipc_failure", "type", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("licenses_updated", godot::Dictionary());
	register_signal<Steam>("microstransaction_auth_response", "app_id", GODOT_VARIANT_TYPE_INT, "order_id", GODOT_VARIANT_TYPE_INT, "authorized", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("steam_server_connect_failed", "result", GODOT_VARIANT_TYPE_INT, "retrying", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Steam>("steam_server_connected", godot::Dictionary());
	register_signal<Steam>("steam_server_disconnected", godot::Dictionary());
	register_signal<Steam>("store_auth_url_response", "url", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("validate_auth_ticket_response", "steamID", GODOT_VARIANT_TYPE_INT, "auth_session_reponse", GODOT_VARIANT_TYPE_INT, "owner_steamID", GODOT_VARIANT_TYPE_INT);

	// USER STATS SIGNALS ///////////////////////
	register_signal<Steam>("global_achievement_percentages_ready", "gameID", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("global_stats_received", "game_id", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("leaderboard_find_result", "leaderboard", GODOT_VARIANT_TYPE_INT, "found", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("leaderboard_scores_downloaded", "message", GODOT_VARIANT_TYPE_STRING, "leaderboard_entries_array", GODOT_VARIANT_TYPE_ARRAY);
	register_signal<Steam>("leaderboard_score_uploaded", "success", GODOT_VARIANT_TYPE_BOOL, "score", GODOT_VARIANT_TYPE_INT, "score_changed", GODOT_VARIANT_TYPE_BOOL, "global_rank_new", GODOT_VARIANT_TYPE_INT, "global_rank_previous", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("leaderboard_ugc_set", "leaderboard_handle", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("number_of_current_players", "success", GODOT_VARIANT_TYPE_BOOL, "players", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("user_achievement_stored", "gameID", GODOT_VARIANT_TYPE_INT, "groupAchieve", GODOT_VARIANT_TYPE_BOOL, "achievementName", GODOT_VARIANT_TYPE_STRING, "currentProgress", GODOT_VARIANT_TYPE_INT, "maxProgress", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("current_stats_received", "gameID", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT, "userID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("user_stats_received", "gameID", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT, "userID", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("user_stats_stored", "game", GODOT_VARIANT_TYPE_INT, "result", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("user_stats_unloaded", "user", GODOT_VARIANT_TYPE_INT);

	// UTILITY SIGNALS //////////////////////////
	register_signal<Steam>("check_file_signature", "signature", GODOT_VARIANT_TYPE_STRING);
	register_signal<Steam>("gamepad_text_input_dismissed", "text", GODOT_VARIANT_TYPE_STRING, "length", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("ip_country", godot::Dictionary());
	register_signal<Steam>("low_power", "power", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("steam_api_call_completed", "asyn_call", GODOT_VARIANT_TYPE_INT, "callback", GODOT_VARIANT_TYPE_INT, "parameter", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("steam_shutdown", godot::Dictionary());
	register_signal<Steam>("app_resuming_from_suspend", godot::Dictionary());
	register_signal<Steam>("floating_gamepad_text_input_dismissed", godot::Dictionary());

	// VIDEO SIGNALS ////////////////////////////
//	register_signal<Steam>("broadcast_upload_start");	// In documentation but not in actual SDK?
//	register_signal<Steam>("broadcast_upload_stop");	// In documentation but not in actual SDK?
	register_signal<Steam>("get_opf_settings_result", "result", GODOT_VARIANT_TYPE_INT, "app_id", GODOT_VARIANT_TYPE_INT);
	register_signal<Steam>("get_video_result", "result", GODOT_VARIANT_TYPE_INT, "app_id", GODOT_VARIANT_TYPE_INT, "url", GODOT_VARIANT_TYPE_STRING);
}

Steam::~Steam(){
	// Store stats then shut down ///////////////
	if(is_init_success){
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}

	// Clear app ID variable ////////////////////
	current_app_id = 0;
}
