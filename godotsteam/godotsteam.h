#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

/////////////////////////////////////////////////
// SILENCE STEAMWORKS WARNINGS
/////////////////////////////////////////////////
//
// Turn off MSVC-only warning about strcpy
#include <cstdint>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#pragma warning(disable:4828)
#endif

/////////////////////////////////////////////////
// INCLUDE HEADERS
/////////////////////////////////////////////////
//
// Include INT types header
#include <inttypes.h>

// Include Steamworks API headers
#include "steam/steam_api.h"
#include "steam/steamnetworkingfakeip.h"
#include "steam/isteamdualsense.h"

// Include Godot headers
#include "Godot.hpp"
#include "Object.hpp"
#include "OS.hpp"
#include "Engine.hpp"
#include "Texture.hpp"
#include "String.hpp"
#include "Reference.hpp"
#include "Dictionary.hpp"
#include "IP.hpp"

// Include GodotSteam headers
#include "godotsteam_constants.h"

// Include some system headers
#include "map"

using namespace godot;

class Steam: public Object {
	GODOT_CLASS(Steam, Object);

public:
	// Used by Godot for GDNative
	static void _register_methods();
	void _init(){ }

	// Steamworks
	static Steam *get_singleton();
	Steam();
	~Steam();


	/////////////////////////////////////////
	// STEAMWORKS FUNCTIONS
	/////////////////////////////////////////
	//
	CSteamID createSteamID(uint64_t steam_id);

	// Main /////////////////////////////////
	uint32_t getSteamID32(uint64_t steam_id);
	bool isAnonAccount(uint64_t steam_id);
	bool isAnonUserAccount(uint64_t steam_id);
	bool isChatAccount(uint64_t steam_id);
	bool isClanAccount(uint64_t steam_id);
	bool isConsoleUserAccount(uint64_t steam_id);
	bool isIndividualAccount(uint64_t steam_id);
	bool isLobby(uint64_t steam_id);
	bool isSteamRunning();
	bool restartAppIfNecessary(uint32 app_id);
	void steamworksError(String failed_signal);
	Dictionary steamInit(bool retrieve_stats);
	Dictionary steamInitEx(bool retrieve_stats);
	void steamShutdown();

	// Apps /////////////////////////////////
	Array getDLCDataByIndex();
	bool isAppInstalled(uint32_t app_id);
	bool isCybercafe();
	bool isDLCInstalled(uint32_t dlc_id);
	bool isLowViolence();
	bool isSubscribed();
	bool isSubscribedApp(uint32_t app_id);
	bool isSubscribedFromFamilySharing();
	bool isSubscribedFromFreeWeekend();
	Dictionary isTimedTrial();
	bool isVACBanned();
	int getAppBuildId();
	Dictionary getAppInstallDir(uint32_t app_id);
	uint64_t getAppOwner();
	String getAvailableGameLanguages();
	String getCurrentBetaName();
	String getCurrentGameLanguage();
	int32 getDLCCount();
	Dictionary getDLCDownloadProgress(uint32_t dlc_id);
	uint32_t getEarliestPurchaseUnixTime(uint32_t app_id);
	void getFileDetails(String filename);
	Array getInstalledDepots(uint32_t app_id);
	String getLaunchCommandLine();
	String getLaunchQueryParam(String key);
	void installDLC(uint32_t dlc_id);
	bool markContentCorrupt(bool missing_files_only);
	bool setDLCContext(uint32_t app_id);
	void uninstallDLC(uint32_t dlc_id);

	// Friends //////////////////////////////
	void activateGameOverlay(String type);
	void activateGameOverlayInviteDialog(uint64_t steam_id);
	void activateGameOverlayInviteDialogConnectString(String connect_string);
	void activateGameOverlayToStore(uint32_t app_id = 0);
	void activateGameOverlayToUser(String type, uint64_t steam_id);
	void activateGameOverlayToWebPage(String url);
	void clearRichPresence();
	bool closeClanChatWindowInSteam(uint64_t chat_id);
	void downloadClanActivityCounts(uint64_t clan_id, int clans_to_request);
	void enumerateFollowingList(uint32 start_index);
	uint64_t getChatMemberByIndex(uint64_t clan_id, int user);
	Dictionary getClanActivityCounts(uint64_t clan_id);
	uint64_t getClanByIndex(int clan);
	int getClanChatMemberCount(uint64_t clan_id);
	Dictionary getClanChatMessage(uint64_t chat_id, int message);
	int getClanCount();
	String getClanName(uint64_t clan_id);
	uint64_t getClanOfficerByIndex(uint64_t clan_id, int officer);
	int getClanOfficerCount(uint64_t clan_id);
	uint64_t getClanOwner(uint64_t clan_id);
	String getClanTag(uint64_t clan_id);
	uint64_t getCoplayFriend(int friend_number);
	int getCoplayFriendCount();
	void getFollowerCount(uint64_t steam_id);
	uint64_t getFriendByIndex(int friend_number, int friend_flags);
	uint32 getFriendCoplayGame(uint64_t friend_id);
	int getFriendCoplayTime(uint64_t friend_id);
	int getFriendCount(int friend_flags);
	int getFriendCountFromSource(uint64_t source_id);
	uint64_t getFriendFromSourceByIndex(uint64_t source_id, int friend_number);
	Dictionary getFriendGamePlayed(uint64_t steam_id);
	Dictionary getFriendMessage(uint64_t friend_id, int message);
	String getFriendPersonaName(uint64_t steam_id);
	String getFriendPersonaNameHistory(uint64_t steam_id, int name_history);
	int getFriendPersonaState(uint64_t steam_id);
	int getFriendRelationship(uint64_t steam_id);
	String getFriendRichPresence(uint64_t friend_id, String key);
	int getFriendRichPresenceKeyCount(uint64_t friend_id);
	String getFriendRichPresenceKeyByIndex(uint64_t friend_id, int key);		
	int getFriendsGroupCount();
	int16 getFriendsGroupIDByIndex(int16 friend_group);
	int getFriendsGroupMembersCount(int16 friend_group);
	Array getFriendsGroupMembersList(int16 friend_group, int member_count);
	String getFriendsGroupName(int16 friend_group);
	int getFriendSteamLevel(uint64_t steam_id);
	int getLargeFriendAvatar(uint64_t steam_id);
	int getMediumFriendAvatar(uint64_t steam_id);
	String getPersonaName();
	int getPersonaState();
	void getPlayerAvatar(int size = 2, uint64_t steam_id = 0);
	String getPlayerNickname(uint64_t steam_id);
	String getProfileItemPropertyString(uint64_t steam_id, int item_type, int item_property);
	uint32 getProfileItemPropertyInt(uint64_t steam_id, int item_type, int item_property);
	Array getRecentPlayers();
	int getSmallFriendAvatar(uint64_t steam_id);
	Array getUserFriendsGroups();
	uint32 getUserRestrictions();
	Array getUserSteamFriends();
	Array getUserSteamGroups();
	bool hasEquippedProfileItem(uint64_t steam_id, int item_type);
	bool hasFriend(uint64_t steam_id, int friend_flags);
	bool inviteUserToGame(uint64_t friend_id, String connect_string);
	bool isClanChatAdmin(uint64_t chat_id, uint64_t steam_id);
	bool isClanPublic(uint64_t clan_id);
	bool isClanOfficialGameGroup(uint64_t clan_id);
	bool isClanChatWindowOpenInSteam(uint64_t chat_id);
	void isFollowing(uint64_t steam_id);
	bool isUserInSource(uint64_t steam_id, uint64_t source_id);
	void joinClanChatRoom(uint64_t clan_id);
	bool leaveClanChatRoom(uint64_t clan_id);
	bool openClanChatWindowInSteam(uint64_t chat_id);
	bool registerProtocolInOverlayBrowser(String protocol);
	bool replyToFriendMessage(uint64_t steam_id, String message);
	void requestClanOfficerList(uint64_t clan_id);
	void requestEquippedProfileItems(uint64_t steam_id);
	void requestFriendRichPresence(uint64_t friend_id);
	bool requestUserInformation(uint64_t steam_id, bool require_name_only);
	bool sendClanChatMessage(uint64_t chat_id, String text);
	void setInGameVoiceSpeaking(uint64_t steam_id, bool speaking);
	bool setListenForFriendsMessages(bool intercept);
	void setPersonaName(String name);
	void setPlayedWith(uint64_t steam_id);
	bool setRichPresence(String key, String value);

	// Game Search //////////////////////////
	int addGameSearchParams(String key, String values);
	int searchForGameWithLobby(uint64_t lobby_id, int player_min, int player_max);
	int searchForGameSolo(int player_min, int player_max);
	int acceptGame();
	int declineGame();
	String retrieveConnectionDetails(uint64_t host_id);
	int endGameSearch();
	int setGameHostParams(String key, String value);
	int setConnectionDetails(String details, int connection_details);
	int requestPlayersForGame(int player_min, int player_max, int max_team_size);
	int hostConfirmGameStart(uint64_t game_id);
	int cancelRequestPlayersForGame();
	int submitPlayerResult(uint64_t game_id, uint64_t player_id, int player_result);
	int endGame(uint64_t game_id);

	// HTML Surface /////////////////////////
	void addHeader(String key, String value, uint32 this_handle = 0);
	void allowStartRequest(bool allowed, uint32 this_handle = 0);
	void copyToClipboard(uint32 this_handle = 0);
	void createBrowser(String user_agent, String user_css);
	void executeJavascript(String javascript, uint32 this_handle = 0);
	void find(String search, bool currently_in_find, bool reverse, uint32 this_handle = 0);
	void getLinkAtPosition(int x, int y, uint32 this_handle = 0);
	void goBack(uint32 this_handle = 0);
	void goForward(uint32 this_handle = 0);
	void htmlInit();
	void jsDialogResponse(bool result, uint32 this_handle = 0);
	void keyChar(uint32 unicode_char, int key_modifiers, uint32 this_handle = 0);
	void keyDown(uint32 native_key_code, int key_modifiers, uint32 this_handle = 0);
	void keyUp(uint32 native_key_code, int key_modifiers, uint32 this_handle = 0);
	void loadURL(String url, String post_data, uint32 this_handle = 0);
	void mouseDoubleClick(int mouse_button, uint32 this_handle = 0);
	void mouseDown(int mouse_button, uint32 this_handle = 0);
	void mouseMove(int x, int y, uint32 this_handle = 0);
	void mouseUp(int mouse_button, uint32 this_handle = 0);
	void mouseWheel(int32 delta, uint32 this_handle = 0);
	void pasteFromClipboard(uint32 this_handle = 0);
	void reload(uint32 this_handle = 0);
	void removeBrowser(uint32 this_handle = 0);
	void setBackgroundMode(bool background_mode, uint32 this_handle = 0);
	void setCookie(String hostname, String key, String value, String path, uint32 expires, bool secure, bool http_only);
	void setHorizontalScroll(uint32 absolute_pixel_scroll, uint32 this_handle = 0);
	void setKeyFocus(bool has_key_focus, uint32 this_handle = 0);
	void setPageScaleFactor(float zoom, int point_x, int point_y, uint32 this_handle = 0);
	void setSize(uint32 width, uint32 height, uint32 this_handle = 0);
	void setVerticalScroll(uint32 absolute_pixel_scroll, uint32 this_handle = 0);
	bool htmlShutdown();
	void stopFind(uint32 this_handle = 0);
	void stopLoad(uint32 this_handle = 0);
	void viewSource(uint32 this_handle = 0);

	// HTTP /////////////////////////////////
	uint32_t createCookieContainer( bool allow_responses_to_modify);
	uint32_t createHTTPRequest(int request_method, String absolute_url);
	bool deferHTTPRequest(uint32 request_handle);
	float getHTTPDownloadProgressPct(uint32 request_handle);
	bool getHTTPRequestWasTimedOut(uint32 request_handle);
	PoolByteArray getHTTPResponseBodyData(uint32 request_handle, uint32 buffer_size);
	uint32 getHTTPResponseBodySize(uint32 request_handle);
	uint32 getHTTPResponseHeaderSize(uint32 request_handle, String header_name);
	uint8 getHTTPResponseHeaderValue(uint32 request_handle, String header_name, uint32 buffer_size);
	uint8 getHTTPStreamingResponseBodyData(uint32 request_handle, uint32 offset, uint32 buffer_size);
	bool prioritizeHTTPRequest(uint32 request_handle);
	bool releaseCookieContainer(uint32 cookie_handle);
	bool releaseHTTPRequest(uint32 request_handle);
	bool sendHTTPRequest(uint32 request_handle);
	bool sendHTTPRequestAndStreamResponse(uint32 request_handle);
	bool setHTTPCookie(uint32 cookie_handle, String host, String url, String cookie);
	bool setHTTPRequestAbsoluteTimeoutMS(uint32 request_handle, uint32 milliseconds);
	bool setHTTPRequestContextValue(uint32 request_handle, uint64_t context_value);
	bool setHTTPRequestCookieContainer(uint32 request_handle, uint32 cookie_handle);
	bool setHTTPRequestGetOrPostParameter(uint32 request_handle, String name, String value);
	bool setHTTPRequestHeaderValue(uint32 request_handle, String header_name, String header_value);
	bool setHTTPRequestNetworkActivityTimeout(uint32 request_handle, uint32 timeout_seconds);
	uint8 setHTTPRequestRawPostBody(uint32 request_handle, String content_type, uint32 body_length);
	bool setHTTPRequestRequiresVerifiedCertificate(uint32 request_handle, bool require_verified_certificate);
	bool setHTTPRequestUserAgentInfo(uint32 request_handle, String user_agent_info);
	
	// Input ////////////////////////////////
	void activateActionSet(uint64_t input_handle, uint64_t action_set_handle);
	void activateActionSetLayer(uint64_t input_handle, uint64_t action_set_layer_handle);
	void deactivateActionSetLayer(uint64_t input_handle, uint64_t action_set_handle);
	void deactivateAllActionSetLayers(uint64_t input_handle);
	void enableDeviceCallbacks();
//		void enableActionEventCallbacks();
	uint64_t getActionSetHandle(String action_set_name);
	int getActionOriginFromXboxOrigin(uint64_t input_handle, int origin);
	Array getActiveActionSetLayers(uint64_t input_handle);
	Dictionary getAnalogActionData(uint64_t input_handle, uint64_t analog_action_handle);
	uint64_t getAnalogActionHandle(String action_name);
	Array getAnalogActionOrigins(uint64_t input_handle, uint64_t action_set_handle, uint64_t analog_action_handle);
	Array getConnectedControllers();
	uint64_t getControllerForGamepadIndex(int index);
	uint64_t getCurrentActionSet(uint64_t input_handle);
	Array getDeviceBindingRevision(uint64_t input_handle);
	Dictionary getDigitalActionData(uint64_t input_handle, uint64_t digital_action_handle);
	uint64_t getDigitalActionHandle(String action_name);
	Array getDigitalActionOrigins(uint64_t input_handle, uint64_t action_set_handle, uint64_t digital_action_handle);
	int getGamepadIndexForController(uint64_t input_handle);
	String getGlyphForActionOrigin(int origin);
	String getGlyphForXboxOrigin(int origin);
	String getGlyphPNGForActionOrigin(int origin, int size, uint32 flags);
	String getGlyphSVGForActionOrigin(int origin, uint32 flags);
	int getInputTypeForHandle(uint64_t input_handle);
	Dictionary getMotionData(uint64_t input_handle);
	int getRemotePlaySessionID(uint64_t input_handle);
	uint16 getSessionInputConfigurationSettings();
	String getStringForActionOrigin(int origin);
	String getStringForAnalogActionName(uint64_t action_handle);
	String getStringForDigitalActionName(uint64_t action_handle);
	String getStringForXboxOrigin(int origin);
	bool inputInit(bool explicitly_call_runframe = false);
	bool inputShutdown();
	void inputActionEventCallback(SteamInputActionEvent_t* call_data);
	bool newDataAvailable();
	void runFrame(bool reserved_value = true);
	void setLEDColor(uint64_t input_handle, int color_r, int color_g, int color_b, int flags);
	bool showBindingPanel(uint64_t input_handle);
	void stopAnalogActionMomentum(uint64_t input_handle, uint64_t action);
	int translateActionOrigin(int destination_input, int source_origin);
	void triggerHapticPulse(uint64_t input_handle, int target_pad, int duration);
	void triggerRepeatedHapticPulse(uint64_t input_handle, int target_pad, int duration, int offset, int repeat, int flags);
	void triggerSimpleHapticEvent(uint64_t input_handle, int haptic_location, uint8 intensity, String gain_db, uint8 other_intensity, String other_gain_db);
	void triggerVibration(uint64_t input_handle, uint16_t left_speed, uint16_t right_speed);
	void triggerVibrationExtended(uint64_t input_handle, uint16_t left_speed, uint16_t right_speed, uint16_t left_trigger_speed, uint16_t right_trigger_speed);
	bool setInputActionManifestFilePath(String manifest_path);
	void setDualSenseTriggerEffect(uint64_t input_handle, int parameter_index, int trigger_mask, int effect_mode, int position, int amplitude, int frequency);
	bool waitForData(bool wait_forever, uint32 timeout);

	// Inventory ////////////////////////////
	int32 addPromoItem(uint32 item);
	int32 addPromoItems(PoolIntArray items);
	bool checkResultSteamID(uint64_t steam_id_expected, int32 this_inventory_handle = 0);
	int32 consumeItem(uint64_t item_consume, uint32 quantity);
	int32 deserializeResult(PoolByteArray buffer);
	void destroyResult(int32 this_inventory_handle = 0);
	int32 exchangeItems(PoolIntArray output_items, PoolIntArray output_quantity, PoolIntArray input_items, PoolIntArray input_quantity);
	int32 generateItems(PoolIntArray items, PoolIntArray quantity);
	int32 getAllItems();
	String getItemDefinitionProperty(uint32 definition, String name);
	int32 getItemsByID(PoolIntArray id_array);
	uint64_t getItemPrice(uint32 definition);
	Array getItemsWithPrices();
	String getResultItemProperty(uint32 index, String name, int32 this_inventory_handle = 0);
	Array getResultItems(int32 this_inventory_handle = 0);
	String getResultStatus(int32 this_inventory_handle = 0);
	uint32 getResultTimestamp(int32 this_inventory_handle = 0);
	int32 grantPromoItems();
	bool loadItemDefinitions();
	void requestEligiblePromoItemDefinitionsIDs(uint64_t steam_id);
	void requestPrices();
	String serializeResult(int32 this_inventory_handle = 0);
	void startPurchase(PoolIntArray items, PoolIntArray quantity);
	int32 transferItemQuantity(uint64_t item_id, uint32 quantity, uint64_t item_destination, bool split);
	int32 triggerItemDrop(uint32 definition);
	void startUpdateProperties();
	int32 submitUpdateProperties(uint64_t this_inventory_update_handle = 0);
	bool removeProperty(uint64_t item_id, String name, uint64_t this_inventory_update_handle = 0);
	bool setPropertyString(uint64_t item_id, String name, String value, uint64_t this_inventory_update_handle = 0);
	bool setPropertyBool(uint64_t item_id, String name, bool value, uint64_t this_inventory_update_handle = 0);
	bool setPropertyInt(uint64_t item_id, String name, uint64_t value, uint64_t this_inventory_update_handle = 0);
	bool setPropertyFloat(uint64_t item_id, String name, float value, uint64_t this_inventory_update_handle = 0);

	// Matchmaking //////////////////////////
	Array getFavoriteGames();
	int addFavoriteGame(uint32 ip, uint16 port, uint16 query_port, uint32 flags, uint32 last_played);
	bool removeFavoriteGame(uint32 app_id, uint32 ip, uint16 port, uint16 query_port, uint32 flags);
	void requestLobbyList();
	void addRequestLobbyListStringFilter(String key_to_match, String value_to_match, int comparison_type);
	void addRequestLobbyListNumericalFilter(String key_to_match, int value_to_match, int comparison_type);
	void addRequestLobbyListNearValueFilter(String key_to_match, int value_to_be_close_to);
	void addRequestLobbyListFilterSlotsAvailable(int slots_available);
	void addRequestLobbyListDistanceFilter(int distance_filter);
	void addRequestLobbyListResultCountFilter(int max_results);
	void createLobby(int lobby_type, int max_members);
	void joinLobby(uint64_t steam_lobby_id);
	void leaveLobby(uint64_t steam_lobby_id);
	bool inviteUserToLobby(uint64_t steam_lobby_id, uint64_t steam_id_invitee);
	int getNumLobbyMembers(uint64_t steam_lobby_id);
	uint64_t getLobbyMemberByIndex(uint64_t steam_lobby_id, int member);
	String getLobbyData(uint64_t steam_lobby_id, String key);
	bool setLobbyData(uint64_t steam_lobby_id, String key, String value);
	Dictionary getAllLobbyData(uint64_t steam_lobby_id);
	bool deleteLobbyData(uint64_t steam_lobby_id, String key);
	String getLobbyMemberData(uint64_t steam_lobby_id, uint64_t steam_id_user, String key);
	void setLobbyMemberData(uint64_t steam_lobby_id, String key, String value);
	bool sendLobbyChatMsg(uint64_t steam_lobby_id, String message_body);
	bool requestLobbyData(uint64_t steam_lobby_id);
	void setLobbyGameServer(uint64_t steam_lobby_id, String server_ip, uint16 server_port, uint64_t steam_id_game_server);
	Dictionary getLobbyGameServer(uint64_t steam_lobby_id);
	bool setLobbyMemberLimit(uint64_t steam_lobby_id, int max_members);
	int getLobbyMemberLimit(uint64_t steam_lobby_id);
	bool setLobbyType(uint64_t steam_lobby_id, int lobby_type);
	bool setLobbyJoinable(uint64_t steam_lobby_id, bool joinable);
	uint64_t getLobbyOwner(uint64_t steam_lobby_id);
	bool setLobbyOwner(uint64_t steam_lobby_id, uint64_t steam_id_new_owner);

	// Matchmaking Servers //////////////////
	void cancelQuery(uint64_t server_list_request = 0);
	void cancelServerQuery(int server_query);
	int getServerCount(uint64_t server_list_request = 0);
	Dictionary getServerDetails(int server, uint64_t server_list_request = 0);
	bool isRefreshing(uint64_t server_list_request = 0);
	int pingServer(String ip, uint16 port);
	int playerDetails(String ip, uint16 port);
	void refreshQuery(uint64_t server_list_request = 0);
	void refreshServer(int server, uint64_t server_list_request = 0);
	void releaseRequest(uint64_t server_list_request = 0);
	uint64_t requestFavoritesServerList(uint32 app_id, Array filters);
	uint64_t requestFriendsServerList(uint32 app_id, Array filters);
	uint64_t requestHistoryServerList(uint32 app_id, Array filters);
	uint64_t requestInternetServerList(uint32 app_id, Array filters);
	uint64_t requestLANServerList(uint32 app_id);
	uint64_t requestSpectatorServerList(uint32 app_id, Array filters);
	int serverRules(String ip, uint16 port);

	// Music ////////////////////////////////
	bool musicIsEnabled();
	bool musicIsPlaying();
	int getPlaybackStatus();
	float musicGetVolume();
	void musicPause();
	void musicPlay();
	void musicPlayNext();
	void musicPlayPrev();
	void musicSetVolume(float volume);

	// Music Remote /////////////////////////
	bool activationSuccess(bool activate);
	bool isCurrentMusicRemote();
	bool currentEntryDidChange();
	bool currentEntryIsAvailable(bool available);
	bool currentEntryWillChange();
	bool deregisterSteamMusicRemote();
	bool enableLooped(bool loop);
	bool enablePlaylists(bool playlists);
	bool enablePlayNext(bool next);
	bool enablePlayPrevious(bool previous);
	bool enableQueue(bool queue);
	bool enableShuffled(bool shuffle);
	bool playlistDidChange();
	bool playlistWillChange();
	bool queueDidChange();
	bool queueWillChange();
	bool registerSteamMusicRemote(String name);
	bool resetPlaylistEntries();
	bool resetQueueEntries();
	bool setCurrentPlaylistEntry(int id);
	bool setCurrentQueueEntry(int id);
	bool setDisplayName(String name);
	bool setPlaylistEntry(int id, int position, String entry_text);
	bool setPNGIcon64x64(PoolByteArray icon);
	bool setQueueEntry(int id, int position, String entry_text);
	bool updateCurrentEntryCoverArt(PoolByteArray art);
	bool updateCurrentEntryElapsedSeconds(int seconds);
	bool updateCurrentEntryText(String text);
	bool updateLooped(bool looped);
	bool updatePlaybackStatus(int status);
	bool updateShuffled(bool shuffle);
	bool updateVolume(float volume);

	// Networking ///////////////////////////
	bool acceptP2PSessionWithUser(uint64_t steam_id_remote);
	bool allowP2PPacketRelay(bool allow);
	bool closeP2PChannelWithUser(uint64_t steam_id_remote, int channel);
	bool closeP2PSessionWithUser(uint64_t steam_id_remote);
	Dictionary getP2PSessionState(uint64_t steam_id_remote);
	uint32_t getAvailableP2PPacketSize(int channel = 0);
	Dictionary readP2PPacket(uint32_t packet, int channel = 0);
	bool sendP2PPacket(uint64_t steam_id_remote, const PoolByteArray data, int send_type, int channel = 0);

	// Networking Messages //////////////////
	bool acceptSessionWithUser(String identity_reference);
	bool closeChannelWithUser(String identity_reference, int channel);
	bool closeSessionWithUser(String identity_reference);
	Dictionary getSessionConnectionInfo(String identity_reference, bool get_connection, bool get_status);
	Array receiveMessagesOnChannel(int channel, int max_messages);
	int sendMessageToUser(String identity_reference, const PoolByteArray data, int flags, int channel);

	// Networking Sockets ///////////////////
	int acceptConnection(uint32 connection_handle);
	bool beginAsyncRequestFakeIP(int num_ports);
	bool closeConnection(uint32 peer, int reason, String debug_message, bool linger);
	bool closeListenSocket(uint32 socket);
	int configureConnectionLanes(uint32 connection, int lanes, Array priorities, Array weights);
	uint32 connectP2P(String identity_reference, int virtual_port, Array options);
	uint32 connectByIPAddress(String ip_address_with_port, Array options);
	uint32 connectToHostedDedicatedServer(String identity_reference, int virtual_port, Array options);
	void createFakeUDPPort(int fake_server_port);
	uint32 createHostedDedicatedServerListenSocket(int virtual_port, Array options);
	uint32 createListenSocketIP(String ip_reference, Array options);
	uint32 createListenSocketP2P(int virtual_port, Array options);
	uint32 createListenSocketP2PFakeIP(int fake_port, Array options);
	uint32 createPollGroup();
	Dictionary createSocketPair(bool loopback, String identity_reference1, String identity_reference2);
	bool destroyPollGroup(uint32 poll_group);
//			int findRelayAuthTicketForServer(int port);	<------ Uses datagram relay structs which were removed from base SDK
	int flushMessagesOnConnection(uint32 connection_handle);
	int getAuthenticationStatus();
	Dictionary getCertificateRequest();
	Dictionary getConnectionInfo(uint32 connection_handle);
	String getConnectionName(uint32 peer);
	Dictionary getConnectionRealTimeStatus(uint32 connection_handle, int lanes, bool get_status = true);
	uint64_t getConnectionUserData(uint32 peer);
	Dictionary getDetailedConnectionStatus(uint32 connection_handle);
	Dictionary getFakeIP(int first_port = 0);
//			int getGameCoordinatorServerLogin(String app_data);	<------ Uses datagram relay structs which were removed from base SDK
//			int getHostedDedicatedServerAddress();	<------ Uses datagram relay structs which were removed from base SDK
	uint32 getHostedDedicatedServerPOPId();
	uint16 getHostedDedicatedServerPort();
	String getListenSocketAddress(uint32 socket, bool with_port = true);
	String getIdentity();
	Dictionary getRemoteFakeIPForConnection(uint32 connection);
	int initAuthentication();
	Array receiveMessagesOnConnection(uint32 connection, int max_messages);
	Array receiveMessagesOnPollGroup(uint32 poll_group, int max_messages);
//			Dictionary receivedRelayAuthTicket();	<------ Uses datagram relay structs which were removed from base SDK
	void resetIdentity(String this_identity);
	void runNetworkingCallbacks();
	void sendMessages(int messages, const PoolByteArray data, uint32 connection_handle, int flags);
	Dictionary sendMessageToConnection(uint32 connection_handle, const PoolByteArray data, int flags);
	Dictionary setCertificate(const PoolByteArray& certificate);		
	bool setConnectionPollGroup(uint32 connection_handle, uint32 poll_group);
	void setConnectionName(uint32 peer, String name);

	// Networking Types /////////////////////
	bool addIdentity(String reference_name);
	bool addIPAddress(String reference_name);
	void clearIdentity(String reference_name);
	void clearIPAddress(String reference_name);
	uint8 getGenericBytes(String reference_name);
	String getGenericString(String reference_name);
	Array getIdentities();
	uint32 getIdentityIPAddr(String reference_name);
	uint32 getIdentitySteamID(String reference_name);
	uint64_t getIdentitySteamID64(String reference_name);
	Array getIPAddresses();
	uint32 getIPv4(String reference_name);
	uint64_t getPSNID(String reference_name);
	uint64_t getStadiaID(String reference_name);
	String getXboxPairwiseID(String reference_name);
	bool isAddressLocalHost(String reference_name);
	bool isIdentityInvalid(String reference_name);
	bool isIdentityLocalHost(String reference_name);
	bool isIPv4(String reference_name);
	bool isIPv6AllZeros(String reference_name);
	bool parseIdentityString(String reference_name, String string_to_parse);
	bool parseIPAddressString(String reference_name, String string_to_parse);
	bool setGenericBytes(String reference_name, uint8 data);
	bool setGenericString(String reference_name, String this_string);
	bool setIdentityIPAddr(String reference_name, String ip_address_name);
	void setIdentityLocalHost(String reference_name);
	void setIdentitySteamID(String reference_name, uint32 steam_id);
	void setIdentitySteamID64(String reference_name, uint64_t steam_id);
	void setIPv4(String reference_name, uint32 ip, uint16 port);
	void setIPv6(String reference_name, uint8 ipv6, uint16 port);
	void setIPv6LocalHost(String reference_name, uint16 port = 0);
	void setPSNID(String reference_name, uint64_t psn_id);
	void setStadiaID(String reference_name, uint64_t stadia_id);
	bool setXboxPairwiseID(String reference_name, String xbox_id);
	String toIdentityString(String reference_name);
	String toIPAddressString(String reference_name, bool with_port);
	const SteamNetworkingConfigValue_t* convertOptionsArray(Array options);

	// Networking Utils /////////////////////
	bool checkPingDataUpToDate(float max_age_in_seconds);
	String convertPingLocationToString(PoolByteArray location);
	int estimatePingTimeBetweenTwoLocations(PoolByteArray location1, PoolByteArray location2);
	int estimatePingTimeFromLocalHost(PoolByteArray location);
	Dictionary getConfigValue(int config_value, int scope_type, uint32_t connection_handle);
	Dictionary getConfigValueInfo(int config_value);
	int getDirectPingToPOP(uint32 pop_id);
	Dictionary getLocalPingLocation();
	uint64_t getLocalTimestamp();
	Dictionary getPingToDataCenter(uint32 pop_id);
	int getPOPCount();
	Array getPOPList();
	int getRelayNetworkStatus();
	void initRelayNetworkAccess();
	Dictionary parsePingLocationString(String location_string);
	bool setConnectionConfigValueFloat(uint32 connection, int config, float value);
	bool setConnectionConfigValueInt32(uint32 connection, int config, int32 value);
	bool setConnectionConfigValueString(uint32 connection, int config, String value);
//		bool setConfigValue(int setting, int scope_type, uint32_t connection_handle, int data_type, auto value);
	bool setGlobalConfigValueFloat(int config, float value);		
	bool setGlobalConfigValueInt32(int config, int32 value);
	bool setGlobalConfigValueString(int config, String value);

	// Parental Settings ////////////////////
	bool isParentalLockEnabled();
	bool isParentalLockLocked();
	bool isAppBlocked(uint32 app_id);
	bool isAppInBlockList(uint32 app_id);
	bool isFeatureBlocked(int feature);
	bool isFeatureInBlockList(int feature);

	// Parties //////////////////////////////
	void cancelReservation(uint64_t beacon_id, uint64_t steam_id);
	void changeNumOpenSlots(uint64_t beacon_id, uint32 open_slots);
	void createBeacon(uint32 open_slots, uint64_t location_id, int type, String connect_string, String beacon_metadata);
	bool destroyBeacon(uint64_t beacon_id);
	Array getAvailableBeaconLocations(uint32 max);
	uint64_t getBeaconByIndex(uint32 index);
	Dictionary getBeaconDetails(uint64_t beacon_id);
	String getBeaconLocationData(uint64_t location_id, int location_type, int location_data);
	uint32 getNumActiveBeacons();
	void joinParty(uint64_t beacon_id);
	void onReservationCompleted(uint64_t beacon_id, uint64_t steam_id);

	// Remote Play //////////////////////////
	uint32 getSessionCount();
	uint32 getSessionID(uint32 index);
	uint64_t getSessionSteamID(uint32 session_id);
	String getSessionClientName(uint32 session_id);
	int getSessionClientFormFactor(uint32 session_id);
	Dictionary getSessionClientResolution(uint32 session_id);
	bool sendRemotePlayTogetherInvite(uint64_t friend_id);
	bool startRemotePlayTogether(bool show_overlay = true);

	// Remote Storage ///////////////////////
	bool beginFileWriteBatch();
	bool endFileWriteBatch();
	bool fileDelete(String file);
	bool fileExists(String file);
	bool fileForget(String file);
	bool filePersisted(String file);
	Dictionary fileRead(String file, int32_t data_to_read);
	void fileReadAsync(String file, uint32 offset, uint32_t data_to_read);
	void fileShare(String file);
	bool fileWrite(String file, PoolByteArray data, int32 size = 0);
	void fileWriteAsync(String file, PoolByteArray data, int32 size = 0);
	bool fileWriteStreamCancel(uint64_t write_handle);
	bool fileWriteStreamClose(uint64_t write_handle);
	uint64_t fileWriteStreamOpen(String file);
	bool fileWriteStreamWriteChunk(uint64_t write_handle, PoolByteArray data);
	int32 getCachedUGCCount();
	uint64_t getCachedUGCHandle(int32 content);
	int32_t getFileCount();
	Dictionary getFileNameAndSize(int file);
	int32_t getFileSize(String file);
	int64_t getFileTimestamp(String file);
	Dictionary getLocalFileChange(int file);
	uint32_t getLocalFileChangeCount();
	Dictionary getQuota();
	Dictionary getSyncPlatforms(String file);
	Dictionary getUGCDetails(uint64_t content);
	Dictionary getUGCDownloadProgress(uint64_t content);
	bool isCloudEnabledForAccount();
	bool isCloudEnabledForApp();
	void setCloudEnabledForApp(bool enabled);
	bool setSyncPlatforms(String file, int platform);
	void ugcDownload(uint64_t content, uint32 priority);
	void ugcDownloadToLocation(uint64_t content, String location, uint32 priority);
	PoolByteArray ugcRead(uint64_t content, int32 data_size, uint32 offset, int action);

	// Screenshots //////////////////////////
	uint32_t addScreenshotToLibrary(String filename, String thumbnail_filename, int width, int height);
	uint32_t addVRScreenshotToLibrary(int type, String filename, String vr_filename);
	void hookScreenshots(bool hook);
	bool isScreenshotsHooked();
	bool setLocation(uint32_t screenshot, String location);
	bool tagPublishedFile(uint32_t screenshot, uint64_t file_id);
	bool tagUser(uint32_t screenshot, uint64_t steam_id);
	void triggerScreenshot();
	uint32_t writeScreenshot(const PoolByteArray& rgb, int width, int height);

	// UGC //////////////////////////////////
	void addAppDependency(uint64_t published_file_id, uint32_t app_id);
	bool addContentDescriptor(uint64_t update_handle, int descriptor_id);
	void addDependency(uint64_t published_file_id, uint64_t child_published_file_id);
	bool addExcludedTag(uint64_t query_handle, String tag_name);
	bool addItemKeyValueTag(uint64_t query_handle, String key, String value);
	bool addItemPreviewFile(uint64_t query_handle, String preview_file, int type);
	bool addItemPreviewVideo(uint64_t query_handle, String video_id);
	void addItemToFavorites(uint32_t app_id, uint64_t published_file_id);
	bool addRequiredKeyValueTag(uint64_t query_handle, String key, String value);
	bool addRequiredTag(uint64_t query_handle, String tag_name);
	bool addRequiredTagGroup(uint64_t query_handle, Array tag_array);
	bool initWorkshopForGameServer(uint32_t workshop_depot_id);
	void createItem(uint32 app_id, int file_type);
	uint64_t createQueryAllUGCRequest(int query_type, int matching_type, uint32_t creator_id, uint32_t consumer_id, uint32 page);
	uint64_t createQueryUGCDetailsRequest(Array published_file_id);
	uint64_t createQueryUserUGCRequest(uint64_t steam_id, int list_type, int matching_ugc_type, int sort_order, uint32_t creator_id, uint32_t consumer_id, uint32 page);
	void deleteItem(uint64_t published_file_id);
	bool downloadItem(uint64_t published_file_id, bool high_priority);
	Dictionary getItemDownloadInfo(uint64_t published_file_id);
	Dictionary getItemInstallInfo(uint64_t published_file_id);
	uint32 getItemState(uint64_t published_file_id);
	Dictionary getItemUpdateProgress(uint64_t update_handle);
	uint32 getNumSubscribedItems();
	Dictionary getQueryUGCAdditionalPreview(uint64_t query_handle, uint32 index, uint32 preview_index);
	Dictionary getQueryUGCChildren(uint64_t query_handle, uint32 index, uint32_t child_count);
	Dictionary getQueryUGCContentDescriptors(uint64_t query_handle, uint32 index, uint32_t max_entries);
	Dictionary getQueryUGCKeyValueTag(uint64_t query_handle, uint32 index, uint32 key_value_tag_index);
	String getQueryUGCMetadata(uint64_t query_handle, uint32 index);
	uint32 getQueryUGCNumAdditionalPreviews(uint64_t query_handle, uint32 index);
	uint32 getQueryUGCNumKeyValueTags(uint64_t query_handle, uint32 index);
	uint32 getQueryUGCNumTags(uint64_t query_handle, uint32 index);
	String getQueryUGCPreviewURL(uint64_t query_handle, uint32 index);
	Dictionary getQueryUGCResult(uint64_t query_handle, uint32 index);
	Dictionary getQueryUGCStatistic(uint64_t query_handle, uint32 index, int stat_type);
	String getQueryUGCTag(uint64_t query_handle, uint32 index, uint32 tag_index);
	String getQueryUGCTagDisplayName(uint64_t query_handle, uint32 index, uint32 tag_index);
	Array getSubscribedItems();
	Array getUserContentDescriptorPreferences(uint32 max_entries);
	void getUserItemVote(uint64_t published_file_id);
	bool releaseQueryUGCRequest(uint64_t query_handle);
	void removeAppDependency(uint64_t published_file_id, uint32_t app_id);
	bool removeContentDescriptor(uint64_t update_handle, int descriptor_id);
	void removeDependency(uint64_t published_file_id, uint64_t child_published_file_id);
	void removeItemFromFavorites(uint32_t app_id, uint64_t published_file_id);
	bool removeItemKeyValueTags(uint64_t update_handle, String key);
	bool removeItemPreview(uint64_t update_handle, uint32 index);
	void sendQueryUGCRequest(uint64_t update_handle);
	bool setAllowCachedResponse(uint64_t update_handle, uint32 max_age_seconds);
	bool setCloudFileNameFilter(uint64_t update_handle, String match_cloud_filename);
	bool setItemContent(uint64_t update_handle, String content_folder);
	bool setItemDescription(uint64_t update_handle, String description);
	bool setItemMetadata(uint64_t update_handle, String ugc_metadata);
	bool setItemPreview(uint64_t update_handle, String preview_file);
	bool setItemTags(uint64_t update_handle, Array tag_array, bool allow_admin_tags = false);
	bool setItemTitle(uint64_t update_handle, String title);
	bool setItemUpdateLanguage(uint64_t update_handle, String language);
	bool setItemVisibility(uint64_t update_handle, int visibility);
	bool setLanguage(uint64_t query_handle, String language);
	bool setMatchAnyTag(uint64_t query_handle, bool match_any_tag);
	bool setRankedByTrendDays(uint64_t query_handle, uint32 days);
	bool setReturnAdditionalPreviews(uint64_t query_handle, bool return_additional_previews);
	bool setReturnChildren(uint64_t query_handle, bool return_children);
	bool setReturnKeyValueTags(uint64_t query_handle, bool return_key_value_tags);
	bool setReturnLongDescription(uint64_t query_handle, bool return_long_description);
	bool setReturnMetadata(uint64_t query_handle, bool return_metadata);
	bool setReturnOnlyIDs(uint64_t query_handle, bool return_only_ids);
	bool setReturnPlaytimeStats(uint64_t query_handle, uint32 days);
	bool setReturnTotalOnly(uint64_t query_handle, bool return_total_only);
	bool setSearchText(uint64_t query_handle, String search_text);
	void setUserItemVote(uint64_t published_file_id, bool vote_up);
	uint64_t startItemUpdate(uint32_t app_id, uint64_t file_id);
	void startPlaytimeTracking(Array published_file_ids);
	void stopPlaytimeTracking(Array published_file_ids);
	void stopPlaytimeTrackingForAllItems();
	void getAppDependencies(uint64_t published_file_id);
	void submitItemUpdate(uint64_t update_handle, String change_note);
	void subscribeItem(uint64_t published_file_id);
	void suspendDownloads(bool suspend);
	void unsubscribeItem(uint64_t published_file_id);
	bool updateItemPreviewFile(uint64_t update_handle, uint32 index, String preview_file);
	bool updateItemPreviewVideo(uint64_t update_handle, uint32 index, String video_id);
	bool showWorkshopEULA();
	void getWorkshopEULAStatus();
	bool setTimeCreatedDateRange(uint64_t update_handle, uint32 start, uint32 end);
	bool setTimeUpdatedDateRange(uint64_t update_handle, uint32 start, uint32 end);

	// Users ////////////////////////////////
	void advertiseGame(String server_ip, int port);
	int beginAuthSession(PoolByteArray ticket, int ticket_size, uint64_t steam_id);
	void cancelAuthTicket(uint32_t auth_ticket);
	Dictionary decompressVoice(const PoolByteArray& voice, uint32 voice_size, uint32 sample_rate);
	void endAuthSession(uint64_t steam_id);
	Dictionary getAuthSessionTicket(String identity_reference = "");
	uint32 getAuthTicketForWebApi(String service_identity = "");
	Dictionary getAvailableVoice();
	void getDurationControl();
	Dictionary getEncryptedAppTicket();
	int getGameBadgeLevel(int series, bool foil);
	int getPlayerSteamLevel();
	uint64_t getSteamID();
	Dictionary getVoice();
	uint32 getVoiceOptimalSampleRate();
	Dictionary initiateGameConnection(uint64_t server_id, uint32 server_ip, uint16 server_port, bool secure);
	bool isBehindNAT();
	bool isPhoneIdentifying();
	bool isPhoneRequiringVerification();
	bool isPhoneVerified();
	bool isTwoFactorEnabled();
	bool loggedOn();
	void requestEncryptedAppTicket(String secret);
	void requestStoreAuthURL(String redirect);
	void startVoiceRecording();
	bool setDurationControlOnlineState(int new_state);
	void stopVoiceRecording();
	void terminateGameConnection(uint32 server_ip, uint16 server_port);
	int userHasLicenseForApp(uint64_t steam_id, uint32_t app_id);

	// User Stats ///////////////////////////
	void attachLeaderboardUGC(uint64_t ugc_handle, uint64_t this_leaderboard = 0);
	bool clearAchievement(String achievement_name);
	void downloadLeaderboardEntries(int start, int end, int type = k_ELeaderboardDataRequestGlobal, uint64_t this_leaderboard = 0);
	void downloadLeaderboardEntriesForUsers(Array users_id, uint64_t this_leaderboard = 0);
	void findLeaderboard(String leaderboard_name);
	void findOrCreateLeaderboard(String leaderboard_name, int sort_method, int display_type);
	Dictionary getAchievement(String achievement_name);
	Dictionary getAchievementAchievedPercent(String achievement_name);
	Dictionary getAchievementAndUnlockTime(String achievement_name);
	String getAchievementDisplayAttribute(String achievement_name, String key);
	int getAchievementIcon(String achievement_name);
	String getAchievementName(uint32_t achievement);
	Dictionary getAchievementProgressLimitsInt(String achievement_name);
	Dictionary getAchievementProgressLimitsFloat(String achievement_name);
	uint64_t getGlobalStatInt(String stat_name);
	double getGlobalStatFloat(String stat_name);
	uint64_t getGlobalStatIntHistory(String stat_name);
	double getGlobalStatFloatHistory(String stat_name);
	Dictionary getLeaderboardDisplayType(uint64_t this_leaderboard = 0);
	int getLeaderboardEntryCount(uint64_t this_leaderboard = 0);
	String getLeaderboardName(uint64_t this_leaderboard = 0);
	Dictionary getLeaderboardSortMethod(uint64_t this_leaderboard = 0);
	Dictionary getMostAchievedAchievementInfo();
	Dictionary getNextMostAchievedAchievementInfo(int iterator);
	uint32_t getNumAchievements();
	void getNumberOfCurrentPlayers();
	float getStatFloat(String stat_name);
	int getStatInt(String stat_name);
	Dictionary getUserAchievement(uint64_t steam_id, String name);
	Dictionary getUserAchievementAndUnlockTime(uint64_t steam_id, String name);
	float getUserStatFloat(uint64_t steam_id, String name);
	int getUserStatInt(uint64_t steam_id, String name);
	bool indicateAchievementProgress(String name, int current_progress, int max_progress);
	bool requestCurrentStats();
	void requestGlobalAchievementPercentages();
	void requestGlobalStats(int history_days);
	void requestUserStats(uint64_t steam_id);
	bool resetAllStats(bool achievements_too = true);
	bool setAchievement(String name);
	int setLeaderboardDetailsMax(int max);
	bool setStatFloat(String name, float value);
	bool setStatInt(String name, int value);
	bool storeStats();
	bool updateAvgRateStat(String name, float this_session, double session_length);
	void uploadLeaderboardScore(int score, bool keep_best = false, PoolIntArray details = PoolIntArray(), uint64_t this_leaderboard = 0);
	Array getLeaderboardEntries();

	// Utils ////////////////////////////////
	String filterText(int context, uint64_t steam_id, String message);
	String getAPICallFailureReason();
	uint32_t getAppID();
	int getCurrentBatteryPower();
	Dictionary getImageRGBA(int image);
	Dictionary getImageSize(int image);
	uint32 getIPCCallCount();
	String getIPCountry();
	int getSecondsSinceAppActive();
	int getSecondsSinceComputerActive();
	int getServerRealTime();
	String getSteamUILanguage();
	bool initFilterText();
	Dictionary isAPICallCompleted();
	bool isOverlayEnabled();
	bool isSteamChinaLauncher();
	bool isSteamInBigPictureMode();
	bool isSteamRunningInVR();
	bool isVRHeadsetStreamingEnabled();
	bool overlayNeedsPresent();
	void setOverlayNotificationInset(int horizontal, int vertical);
	void setOverlayNotificationPosition(int pos);
	void setVRHeadsetStreamingEnabled(bool enabled);
	bool showGamepadTextInput(int input_mode, int line_input_mode, String description, uint32 max_text, String preset_text);
	bool showFloatingGamepadTextInput(int input_mode, int text_field_x_position, int text_field_y_position, int text_field_width, int text_field_height);
	void setGameLauncherMode(bool mode);
	void startVRDashboard();
	bool isSteamRunningOnSteamDeck();
	bool dismissFloatingGamepadTextInput();
	bool dismissGamepadTextInput();

	// Video ////////////////////////////////
	void getOPFSettings(uint32_t app_id);
	String getOPFStringForApp(uint32_t app_id);
	void getVideoURL(uint32_t app_id);
	Dictionary isBroadcasting();

protected:
	static void _bind_methods();
	static Steam *singleton;

private:
	// Main
	bool is_init_success;
	bool were_callbacks_embedded;

	// Apps
	uint64_t current_app_id = 0;
	
	// Friends
	CSteamID clan_activity;

	// HTML Surface
	uint32 browser_handle;

	// Inventory
	SteamInventoryUpdateHandle_t inventory_update_handle;
	SteamInventoryResult_t inventory_handle;
	SteamItemDetails_t inventory_details;

	// Leaderboards
	SteamLeaderboard_t leaderboard_handle;
	Array leaderboard_entries_array;
	int leaderboard_details_max = LEADERBOARD_DETAIL_MAX;

	// Matchmaking Server
	HServerListRequest server_list_request;
	ISteamMatchmakingServerListResponse* server_list_response;
	ISteamMatchmakingPingResponse *ping_response;
	ISteamMatchmakingPlayersResponse *player_response;
	ISteamMatchmakingRulesResponse *rules_response;

	// Networking Messages
//		std::map<int, SteamNetworkingMessage_t> network_messages;

	// Networking Sockets
	uint32 network_connection;
	uint32 network_poll_group;
	uint64_t networking_microseconds = 0;
	SteamNetworkingIdentity networking_identity;
//		SteamDatagramHostedAddress hosted_address;
	PoolByteArray routing_blob;
//		SteamDatagramRelayAuthTicket relay_auth_ticket;
	std::map<String, SteamNetworkingIdentity> networking_identities;
	std::map<String, SteamNetworkingIPAddr> ip_addresses;

	// Parties
	uint64 party_beacon_id;

	// Remote Storage
	uint64_t write_stream_handle = 0;

	// User stats
	int number_achievements = 0;
	bool stats_initialized = false;

	// Utils
	uint64_t api_handle = 0;

	// Run the Steamworks API callbacks /////
	void run_callbacks(){
		SteamAPI_RunCallbacks();
	}


	/////////////////////////////////////////
	// STEAM CALLBACKS
	/////////////////////////////////////////
	//
	// Apps callbacks ///////////////////////
	STEAM_CALLBACK(Steam, dlc_installed, DlcInstalled_t, callbackDLCInstalled);
	STEAM_CALLBACK(Steam, file_details_result, FileDetailsResult_t, callbackFileDetailsResult);
	STEAM_CALLBACK(Steam, new_launch_url_parameters, NewUrlLaunchParameters_t, callbackNewLaunchURLParameters);
	STEAM_CALLBACK(Steam, timed_trial_status, TimedTrialStatus_t, callbackTimedTrialStatus);

	// Friends callbacks ////////////////////
	STEAM_CALLBACK(Steam, avatar_loaded, AvatarImageLoaded_t, callbackAvatarLoaded);
	STEAM_CALLBACK(Steam, avatar_image_loaded, AvatarImageLoaded_t, callbackAvatarImageLoaded);
	STEAM_CALLBACK(Steam, clan_activity_downloaded, DownloadClanActivityCountsResult_t, callbackClanActivityDownloaded);
	STEAM_CALLBACK(Steam, friend_rich_presence_update, FriendRichPresenceUpdate_t, callbackFriendRichPresenceUpdate);
	STEAM_CALLBACK(Steam, connected_chat_join, GameConnectedChatJoin_t, callbackConnectedChatJoin);
	STEAM_CALLBACK(Steam, connected_chat_leave, GameConnectedChatLeave_t, callbackConnectedChatLeave);
	STEAM_CALLBACK(Steam, connected_clan_chat_message, GameConnectedClanChatMsg_t, callbackConnectedClanChatMessage);
	STEAM_CALLBACK(Steam, connected_friend_chat_message, GameConnectedFriendChatMsg_t, callbackConnectedFriendChatMessage);
	STEAM_CALLBACK(Steam, join_requested, GameLobbyJoinRequested_t, callbackJoinRequested);
	STEAM_CALLBACK(Steam, overlay_toggled, GameOverlayActivated_t, callbackOverlayToggled);
	STEAM_CALLBACK(Steam, join_game_requested, GameRichPresenceJoinRequested_t, callbackJoinGameRequested);
	STEAM_CALLBACK(Steam, change_server_requested, GameServerChangeRequested_t, callbackChangeServerRequested);
	STEAM_CALLBACK(Steam, join_clan_chat_complete, JoinClanChatRoomCompletionResult_t, callbackJoinClanChatComplete);
	STEAM_CALLBACK(Steam, persona_state_change, PersonaStateChange_t, callbackPersonaStateChange);
	STEAM_CALLBACK(Steam, name_changed, SetPersonaNameResponse_t, callbackNameChanged);
	STEAM_CALLBACK(Steam, overlay_browser_protocol, OverlayBrowserProtocolNavigation_t, callbackOverlayBrowserProtocol);
	STEAM_CALLBACK(Steam, unread_chat_messages_changed, UnreadChatMessagesChanged_t, callbackUnreadChatMessagesChanged);
	STEAM_CALLBACK(Steam, equipped_profile_items_changed, EquippedProfileItemsChanged_t, callbackEquippedProfileItemsChanged);

	// Game Search callbacks ////////////////
	STEAM_CALLBACK(Steam, search_for_game_progress, SearchForGameProgressCallback_t, callbackSearchForGameProgress);
	STEAM_CALLBACK(Steam, search_for_game_result, SearchForGameResultCallback_t, callbackSearchForGameResult);
	STEAM_CALLBACK(Steam, request_players_for_game_progress, RequestPlayersForGameProgressCallback_t, callbackRequestPlayersForGameProgress);
	STEAM_CALLBACK(Steam, request_players_for_game_result, RequestPlayersForGameResultCallback_t, callbackRequestPlayersForGameResult);
	STEAM_CALLBACK(Steam, request_players_for_game_final_result, RequestPlayersForGameFinalResultCallback_t, callbackRequestPlayersForGameFinalResult);
	STEAM_CALLBACK(Steam, submit_player_result, SubmitPlayerResultResultCallback_t, callbackSubmitPlayerResult);
	STEAM_CALLBACK(Steam, end_game_result, EndGameResultCallback_t, callbackEndGameResult);

	// HTML Surface callbacks ///////////////
	STEAM_CALLBACK(Steam, html_browser_ready, HTML_BrowserReady_t, callbackHTMLBrowserReady);
	STEAM_CALLBACK(Steam, html_can_go_backandforward, HTML_CanGoBackAndForward_t, callbackHTMLCanGoBackandforward);
	STEAM_CALLBACK(Steam, html_changed_title, HTML_ChangedTitle_t, callbackHTMLChangedTitle);
	STEAM_CALLBACK(Steam, html_close_browser, HTML_CloseBrowser_t, callbackHTMLCloseBrowser);
	STEAM_CALLBACK(Steam, html_file_open_dialog, HTML_FileOpenDialog_t, callbackHTMLFileOpenDialog);
	STEAM_CALLBACK(Steam, html_finished_request, HTML_FinishedRequest_t, callbackHTMLFinishedRequest);
	STEAM_CALLBACK(Steam, html_hide_tooltip, HTML_HideToolTip_t, callbackHTMLHideTooltip);
	STEAM_CALLBACK(Steam, html_horizontal_scroll, HTML_HorizontalScroll_t, callbackHTMLHorizontalScroll);
	STEAM_CALLBACK(Steam, html_js_alert, HTML_JSAlert_t, callbackHTMLJSAlert);
	STEAM_CALLBACK(Steam, html_js_confirm, HTML_JSConfirm_t, callbackHTMLJSConfirm);
	STEAM_CALLBACK(Steam, html_link_at_position, HTML_LinkAtPosition_t, callbackHTMLLinkAtPosition);
	STEAM_CALLBACK(Steam, html_needs_paint, HTML_NeedsPaint_t, callbackHTMLNeedsPaint);
	STEAM_CALLBACK(Steam, html_new_window, HTML_NewWindow_t, callbackHTMLNewWindow);
	STEAM_CALLBACK(Steam, html_open_link_in_new_tab, HTML_OpenLinkInNewTab_t, callbackHTMLOpenLinkInNewTab);
	STEAM_CALLBACK(Steam, html_search_results, HTML_SearchResults_t, callbackHTMLSearchResults);
	STEAM_CALLBACK(Steam, html_set_cursor, HTML_SetCursor_t, callbackHTMLSetCursor);
	STEAM_CALLBACK(Steam, html_show_tooltip, HTML_ShowToolTip_t, callbackHTMLShowTooltip);
	STEAM_CALLBACK(Steam, html_start_request, HTML_StartRequest_t, callbackHTMLStartRequest);
	STEAM_CALLBACK(Steam, html_status_text, HTML_StatusText_t, callbackHTMLStatusText);
	STEAM_CALLBACK(Steam, html_update_tooltip, HTML_UpdateToolTip_t, callbackHTMLUpdateTooltip);
	STEAM_CALLBACK(Steam, html_url_changed, HTML_URLChanged_t, callbackHTMLURLChanged);
	STEAM_CALLBACK(Steam, html_vertical_scroll, HTML_VerticalScroll_t, callbackHTMLVerticalScroll);

	// HTTP callbacks ///////////////////////
	STEAM_CALLBACK(Steam, http_request_completed, HTTPRequestCompleted_t, callbackHTTPRequestCompleted);
	STEAM_CALLBACK(Steam, http_request_data_received, HTTPRequestDataReceived_t, callbackHTTPRequestDataReceived);
	STEAM_CALLBACK(Steam, http_request_headers_received, HTTPRequestHeadersReceived_t, callbackHTTPRequestHeadersReceived);

	// Input callbacks //////////////////////
	STEAM_CALLBACK(Steam, input_device_connected, SteamInputDeviceConnected_t, callbackInputDeviceConnected);
	STEAM_CALLBACK(Steam, input_device_disconnected, SteamInputDeviceDisconnected_t, callbackInputDeviceDisconnected);
	STEAM_CALLBACK(Steam, input_configuration_loaded, SteamInputConfigurationLoaded_t, callbackInputConfigurationLoaded);
	STEAM_CALLBACK(Steam, input_gamepad_slot_change, SteamInputGamepadSlotChange_t, callbackInputGamePadSlotChange);

	// Inventory callbacks //////////////////
	STEAM_CALLBACK(Steam, inventory_definition_update, SteamInventoryDefinitionUpdate_t, callbackInventoryDefinitionUpdate);
	STEAM_CALLBACK(Steam, inventory_full_update, SteamInventoryFullUpdate_t, callbackInventoryFullUpdate);
	STEAM_CALLBACK(Steam, inventory_result_ready, SteamInventoryResultReady_t, callbackInventoryResultReady);

	// Matchmaking callbacks ////////////////
	STEAM_CALLBACK(Steam, favorites_list_accounts_updated, FavoritesListAccountsUpdated_t, callbackFavoritesListAccountsUpdated);
	STEAM_CALLBACK(Steam, favorites_list_changed, FavoritesListChanged_t, callbackFavoritesListChanged);
	STEAM_CALLBACK(Steam, lobby_message, LobbyChatMsg_t, callbackLobbyMessage);
	STEAM_CALLBACK(Steam, lobby_chat_update, LobbyChatUpdate_t, callbackLobbyChatUpdate);
	STEAM_CALLBACK(Steam, lobby_data_update, LobbyDataUpdate_t, callbackLobbyDataUpdate);
	STEAM_CALLBACK(Steam, lobby_joined, LobbyEnter_t, callbackLobbyJoined);
	STEAM_CALLBACK(Steam, lobby_game_created, LobbyGameCreated_t, callbackLobbyGameCreated);
	STEAM_CALLBACK(Steam, lobby_invite, LobbyInvite_t, callbackLobbyInvite);
	STEAM_CALLBACK(Steam, lobby_kicked, LobbyKicked_t, callbackLobbyKicked);

	// Music callbacks //////////////////////
	STEAM_CALLBACK(Steam, music_playback_status_has_changed, PlaybackStatusHasChanged_t, callbackMusicPlaybackStatusHasChanged);
	STEAM_CALLBACK(Steam, music_volume_has_changed, VolumeHasChanged_t, callbackMusicVolumeHasChanged);

	// Music Remote callbacks ///////////////
	STEAM_CALLBACK(Steam, music_player_remote_to_front, MusicPlayerRemoteToFront_t, callbackMusicPlayerRemoteToFront);
	STEAM_CALLBACK(Steam, music_player_remote_will_activate, MusicPlayerRemoteWillActivate_t, callbackMusicPlayerRemoteWillActivate);
	STEAM_CALLBACK(Steam, music_player_remote_will_deactivate, MusicPlayerRemoteWillDeactivate_t, callbackMusicPlayerRemoteWillDeactivate);
	STEAM_CALLBACK(Steam, music_player_selects_playlist_entry, MusicPlayerSelectsPlaylistEntry_t, callbackMusicPlayerSelectsPlaylistEntry);
	STEAM_CALLBACK(Steam, music_player_selects_queue_entry, MusicPlayerSelectsQueueEntry_t, callbackMusicPlayerSelectsQueueEntry);
	STEAM_CALLBACK(Steam, music_player_wants_looped, MusicPlayerWantsLooped_t, callbackMusicPlayerWantsLooped);
	STEAM_CALLBACK(Steam, music_player_wants_pause, MusicPlayerWantsPause_t, callbackMusicPlayerWantsPause);
	STEAM_CALLBACK(Steam, music_player_wants_playing_repeat_status, MusicPlayerWantsPlayingRepeatStatus_t, callbackMusicPlayerWantsPlayingRepeatStatus);
	STEAM_CALLBACK(Steam, music_player_wants_play_next, MusicPlayerWantsPlayNext_t, callbackMusicPlayerWantsPlayNext);
	STEAM_CALLBACK(Steam, music_player_wants_play_previous, MusicPlayerWantsPlayPrevious_t, callbackMusicPlayerWantsPlayPrevious);
	STEAM_CALLBACK(Steam, music_player_wants_play, MusicPlayerWantsPlay_t, callbackMusicPlayerWantsPlay);
	STEAM_CALLBACK(Steam, music_player_wants_shuffled, MusicPlayerWantsShuffled_t, callbackMusicPlayerWantsShuffled);
	STEAM_CALLBACK(Steam, music_player_wants_volume, MusicPlayerWantsVolume_t, callbackMusicPlayerWantsVolume);
	STEAM_CALLBACK(Steam, music_player_will_quit, MusicPlayerWillQuit_t, callbackMusicPlayerWillQuit);

	// Networking callbacks /////////////////
	STEAM_CALLBACK(Steam, p2p_session_connect_fail, P2PSessionConnectFail_t, callbackP2PSessionConnectFail);
	STEAM_CALLBACK(Steam, p2p_session_request, P2PSessionRequest_t, callbackP2PSessionRequest);

	// Networking Messages callbacks ////////
	STEAM_CALLBACK(Steam, network_messages_session_request, SteamNetworkingMessagesSessionRequest_t, callbackNetworkMessagesSessionRequest);
	STEAM_CALLBACK(Steam, network_messages_session_failed, SteamNetworkingMessagesSessionFailed_t, callbackNetworkMessagesSessionFailed);

	// Networking Sockets callbacks /////////
	STEAM_CALLBACK(Steam, network_connection_status_changed, SteamNetConnectionStatusChangedCallback_t, callbackNetworkConnectionStatusChanged);
	STEAM_CALLBACK(Steam, network_authentication_status, SteamNetAuthenticationStatus_t, callbackNetworkAuthenticationStatus);
	STEAM_CALLBACK(Steam, fake_ip_result, SteamNetworkingFakeIPResult_t, callbackNetworkingFakeIPResult);

	// Networking Utils callbacks ///////////
	STEAM_CALLBACK(Steam, relay_network_status, SteamRelayNetworkStatus_t, callbackRelayNetworkStatus);

	// Parental Settings callbacks //////////
	STEAM_CALLBACK(Steam, parental_setting_changed, SteamParentalSettingsChanged_t, callbackParentlSettingChanged);

	// Parties callbacks ////////////////////
	STEAM_CALLBACK(Steam, reservation_notification, ReservationNotificationCallback_t, callbackReserveNotification);
	STEAM_CALLBACK(Steam, available_beacon_locations_updated, AvailableBeaconLocationsUpdated_t, callbackAvailableBeaconLocationsUpdated);
	STEAM_CALLBACK(Steam, active_beacons_updated, ActiveBeaconsUpdated_t, callbackActiveBeaconsUpdated);

	// Remote Play callbacks ////////////////
	STEAM_CALLBACK(Steam, remote_play_session_connected, SteamRemotePlaySessionConnected_t, callbackRemotePlaySessionConnected);
	STEAM_CALLBACK(Steam, remote_play_session_disconnected, SteamRemotePlaySessionDisconnected_t, callbackRemotePlaySessionDisconnected);

	// Remote Storage callbacks /////////////
	STEAM_CALLBACK(Steam, local_file_changed, RemoteStorageLocalFileChange_t, callbackLocalFileChanged);

	// Screenshot callbacks /////////////////
	STEAM_CALLBACK(Steam, screenshot_ready, ScreenshotReady_t, callbackScreenshotReady);
	STEAM_CALLBACK(Steam, screenshot_requested, ScreenshotRequested_t, callbackScreenshotRequested);

	// UGC callbacks ////////////////////////
	STEAM_CALLBACK(Steam, item_downloaded, DownloadItemResult_t, callbackItemDownloaded);
	STEAM_CALLBACK(Steam, item_installed, ItemInstalled_t, callbackItemInstalled);
	STEAM_CALLBACK(Steam, user_subscribed_items_list_changed, UserSubscribedItemsListChanged_t, callbackUserSubscribedItemsListChanged);

	// User callbacks ///////////////////////
	STEAM_CALLBACK(Steam, client_game_server_deny, ClientGameServerDeny_t, callbackClientGameServerDeny);
	STEAM_CALLBACK(Steam, game_web_callback, GameWebCallback_t, callbackGameWebCallback);
	STEAM_CALLBACK(Steam, get_auth_session_ticket_response, GetAuthSessionTicketResponse_t, callbackGetAuthSessionTicketResponse);
	STEAM_CALLBACK(Steam, get_ticket_for_web_api, GetTicketForWebApiResponse_t, callbackGetTicketForWebApiResponse);
	STEAM_CALLBACK(Steam, ipc_failure, IPCFailure_t, callbackIPCFailure);
	STEAM_CALLBACK(Steam, licenses_updated, LicensesUpdated_t, callbackLicensesUpdated);
	STEAM_CALLBACK(Steam, microtransaction_auth_response, MicroTxnAuthorizationResponse_t, callbackMicrotransactionAuthResponse);
	STEAM_CALLBACK(Steam, steam_server_connected, SteamServersConnected_t, callbackSteamServerConnected);
	STEAM_CALLBACK(Steam, steam_server_disconnected, SteamServersDisconnected_t, callbackSteamServerDisconnected);
	STEAM_CALLBACK(Steam, validate_auth_ticket_response, ValidateAuthTicketResponse_t, callbackValidateAuthTicketResponse);

	// User stat callbacks //////////////////
	STEAM_CALLBACK(Steam, user_achievement_stored, UserAchievementStored_t, callbackUserAchievementStored);
	STEAM_CALLBACK(Steam, current_stats_received, UserStatsReceived_t, callbackCurrentStatsReceived);
	STEAM_CALLBACK(Steam, user_stats_stored, UserStatsStored_t, callbackUserStatsStored);
	STEAM_CALLBACK(Steam, user_stats_unloaded, UserStatsUnloaded_t, callbackUserStatsUnloaded);

	// Utility callbacks ////////////////////
	STEAM_CALLBACK(Steam, gamepad_text_input_dismissed, GamepadTextInputDismissed_t, callbackGamepadTextInputDismissed);
	STEAM_CALLBACK(Steam, ip_country, IPCountry_t, callbackIPCountry);
	STEAM_CALLBACK(Steam, low_power, LowBatteryPower_t, callbackLowPower);
	STEAM_CALLBACK(Steam, steam_api_call_completed, SteamAPICallCompleted_t, callbackSteamAPICallCompleted);
	STEAM_CALLBACK(Steam, steam_shutdown, SteamShutdown_t, callbackSteamShutdown);
	STEAM_CALLBACK(Steam, app_resuming_from_suspend, AppResumingFromSuspend_t, callbackAppResumingFromSuspend);
	STEAM_CALLBACK(Steam, floating_gamepad_text_input_dismissed, FloatingGamepadTextInputDismissed_t, callbackFloatingGamepadTextInputDismissed);
	STEAM_CALLBACK(Steam, filter_text_dictionary_changed, FilterTextDictionaryChanged_t, callbackFilterTextDictionaryChanged);

	// Video callbacks //////////////////////
	STEAM_CALLBACK(Steam, get_opf_settings_result, GetOPFSettingsResult_t, callbackGetOPFSettingsResult);
	STEAM_CALLBACK(Steam, get_video_result, GetVideoURLResult_t, callbackGetVideoResult);


	/////////////////////////////////////////
	// STEAM CALL RESULTS
	/////////////////////////////////////////
	//
	// Friends call results /////////////////
	CCallResult<Steam, ClanOfficerListResponse_t> callResultClanOfficerList;
	void request_clan_officer_list(ClanOfficerListResponse_t *call_data, bool io_failure);
	CCallResult<Steam, FriendsEnumerateFollowingList_t> callResultEnumerateFollowingList;
	void enumerate_following_list(FriendsEnumerateFollowingList_t *call_data, bool io_failure);
	CCallResult<Steam, EquippedProfileItems_t> callResultEquippedProfileItems;
	void equipped_profile_items(EquippedProfileItems_t *call_data, bool io_failure);
	CCallResult<Steam, FriendsGetFollowerCount_t> callResultFollowerCount;
	void get_follower_count(FriendsGetFollowerCount_t *call_data, bool io_failure);
	CCallResult<Steam, FriendsIsFollowing_t> callResultIsFollowing;
	void is_following(FriendsIsFollowing_t *call_data, bool io_failure);

	// Inventory call results ///////////////
	CCallResult<Steam, SteamInventoryEligiblePromoItemDefIDs_t> callResultEligiblePromoItemDefIDs;
	void inventory_eligible_promo_item(SteamInventoryEligiblePromoItemDefIDs_t *call_data, bool io_failure);
	CCallResult<Steam, SteamInventoryRequestPricesResult_t> callResultRequestPrices;
	void inventory_request_prices_result(SteamInventoryRequestPricesResult_t *call_data, bool io_failure);
	CCallResult<Steam, SteamInventoryStartPurchaseResult_t> callResultStartPurchase;
	void inventory_start_purchase_result(SteamInventoryStartPurchaseResult_t *call_data, bool io_failure);

	// Matchmaking call results /////////////
	CCallResult<Steam, LobbyCreated_t> callResultCreateLobby;
	void lobby_created(LobbyCreated_t *call_data, bool io_failure);
	CCallResult<Steam, LobbyMatchList_t> callResultLobbyList;
	void lobby_match_list(LobbyMatchList_t *call_data, bool io_failure);

	// Matchmaking Server call results //////
	void server_Responded(gameserveritem_t server);
	void server_Failed_To_Respond();

	// Parties call results /////////////////
	CCallResult<Steam, JoinPartyCallback_t> callResultJoinParty;
	void join_party(JoinPartyCallback_t *call_data, bool io_failure);
	CCallResult<Steam, CreateBeaconCallback_t> callResultCreateBeacon;
	void create_beacon(CreateBeaconCallback_t *call_data, bool io_failure);
	CCallResult<Steam, ChangeNumOpenSlotsCallback_t> callResultChangeNumOpenSlots;
	void change_num_open_slots(ChangeNumOpenSlotsCallback_t *call_data, bool io_failure);

	// Remote Storage call results //////////
	CCallResult<Steam, RemoteStorageFileReadAsyncComplete_t> callResultFileReadAsyncComplete;
	void file_read_async_complete(RemoteStorageFileReadAsyncComplete_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageFileShareResult_t> callResultFileShareResult;
	void file_share_result(RemoteStorageFileShareResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageFileWriteAsyncComplete_t> callResultFileWriteAsyncComplete;
	void file_write_async_complete(RemoteStorageFileWriteAsyncComplete_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageDownloadUGCResult_t> callResultDownloadUGCResult;
	void download_ugc_result(RemoteStorageDownloadUGCResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageUnsubscribePublishedFileResult_t> callResultUnsubscribeItem;
	void unsubscribe_item(RemoteStorageUnsubscribePublishedFileResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoteStorageSubscribePublishedFileResult_t> callResultSubscribeItem;
	void subscribe_item(RemoteStorageSubscribePublishedFileResult_t *call_data, bool io_failure);

	// UGC call results /////////////////////
	CCallResult<Steam, AddAppDependencyResult_t> callResultAddAppDependency;
	void add_app_dependency_result(AddAppDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, AddUGCDependencyResult_t> callResultAddUGCDependency;
	void add_ugc_dependency_result(AddUGCDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, CreateItemResult_t> callResultItemCreate;
	void item_created(CreateItemResult_t *call_data, bool io_failure);
	CCallResult<Steam, GetAppDependenciesResult_t> callResultGetAppDependencies;
	void get_app_dependencies_result(GetAppDependenciesResult_t *call_data, bool io_failure);
	CCallResult<Steam, DeleteItemResult_t> callResultDeleteItem;
	void item_deleted(DeleteItemResult_t *call_data, bool io_failure);
	CCallResult<Steam, GetUserItemVoteResult_t> callResultGetUserItemVote;
	void get_item_vote_result(GetUserItemVoteResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoveAppDependencyResult_t> callResultRemoveAppDependency;
	void remove_app_dependency_result(RemoveAppDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, RemoveUGCDependencyResult_t> callResultRemoveUGCDependency;
	void remove_ugc_dependency_result(RemoveUGCDependencyResult_t *call_data, bool io_failure);
	CCallResult<Steam, SetUserItemVoteResult_t> callResultSetUserItemVote;
	void set_user_item_vote(SetUserItemVoteResult_t *call_data, bool io_failure);
	CCallResult<Steam, StartPlaytimeTrackingResult_t> callResultStartPlaytimeTracking;
	void start_playtime_tracking(StartPlaytimeTrackingResult_t *call_data, bool io_failure);
	CCallResult<Steam, SteamUGCQueryCompleted_t> callResultUGCQueryCompleted;
	void ugc_query_completed(SteamUGCQueryCompleted_t *call_data, bool io_failure);
	CCallResult<Steam, StopPlaytimeTrackingResult_t> callResultStopPlaytimeTracking;
	void stop_playtime_tracking(StopPlaytimeTrackingResult_t *call_data, bool io_failure);
	CCallResult<Steam, SubmitItemUpdateResult_t> callResultItemUpdate;
	void item_updated(SubmitItemUpdateResult_t *call_data, bool io_failure);
	CCallResult<Steam, UserFavoriteItemsListChanged_t> callResultFavoriteItemListChanged;
	void user_favorite_items_list_changed(UserFavoriteItemsListChanged_t *call_data, bool io_failure);
	CCallResult<Steam, WorkshopEULAStatus_t> callResultWorkshopEULAStatus;
	void workshop_eula_status(WorkshopEULAStatus_t *call_data, bool io_failure);

	// User call results ////////////////////
	CCallResult<Steam, DurationControl_t> callResultDurationControl;
	void duration_control(DurationControl_t *call_data, bool io_failure);
	CCallResult<Steam, EncryptedAppTicketResponse_t> callResultEncryptedAppTicketResponse;
	void encrypted_app_ticket_response(EncryptedAppTicketResponse_t *call_data, bool io_failure);
	CCallResult<Steam, SteamServerConnectFailure_t> callResultSteamServerConnectFailure;
	void steam_server_connect_failed(SteamServerConnectFailure_t *call_data, bool io_failure);
	CCallResult<Steam, StoreAuthURLResponse_t> callResultStoreAuthURLResponse;
	void store_auth_url_response(StoreAuthURLResponse_t *call_data, bool io_failure);

	// User stat call results ///////////////
	CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
	void global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *call_data, bool io_failure);
	CCallResult<Steam, GlobalStatsReceived_t> callResultGetGlobalStatsReceived;
	void global_stats_received(GlobalStatsReceived_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
	void leaderboard_find_result(LeaderboardFindResult_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
	void leaderboard_scores_downloaded(LeaderboardScoresDownloaded_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
	void leaderboard_score_uploaded(LeaderboardScoreUploaded_t *call_data, bool io_failure);
	CCallResult<Steam, LeaderboardUGCSet_t> callResultLeaderboardUGCSet;
	void leaderboard_ugc_set(LeaderboardUGCSet_t *call_data, bool io_failure);
	CCallResult<Steam, NumberOfCurrentPlayers_t> callResultNumberOfCurrentPlayers;
	void number_of_current_players(NumberOfCurrentPlayers_t *call_data, bool io_failure);
	CCallResult<Steam, UserStatsReceived_t> callResultUserStatsReceived;
	void user_stats_received(UserStatsReceived_t *call_data, bool io_failure);

	// Utility call results /////////////////
	CCallResult<Steam, CheckFileSignature_t> callResultCheckFileSignature;
	void check_file_signature(CheckFileSignature_t *call_data, bool io_failure);
};

#endif // GODOTSTEAM_H