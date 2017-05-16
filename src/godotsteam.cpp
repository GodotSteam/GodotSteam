// Include Godot and core files
#include "godot_cpp/Godot.hpp"
#include "godot_cpp/core/Dictionary.hpp"
#include "godot_cpp/Node.hpp"
#include "godot_cpp/Reference.hpp"
#include "godot_cpp/Texture.hpp"
#include "godot_cpp/IP.hpp"
// Include the Steamworks SDK
#include "sdk/public/steam/steam_api.h"

using namespace godot;

class Steam : public GodotScript<Node> {
	GODOT_CLASS(Steam)
	
	float version = 0.2;

	private :
		bool IS_INIT_SUCCESS = false;
		bool IS_VALID = false;
		// Run the Steamworks callbacks
		void run_callbacks(){
			SteamAPI_RunCallbacks();
		}

	public:
		enum {
			OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
			TOP_LEFT=0, TOP_RIGHT=1, BOT_LEFT=2, BOT_RIGHT=3,
			ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
			AVATAR_SMALL=0, AVATAR_MEDIUM, AVATAR_LARGE,
			GLOBAL=0, GLOBAL_AROUND_USER=1, FRIENDS=2, USERS=3,
			LOBBY_OK=0, LOBBY_NO_CONNECTION=1, LOBBY_TIMEOUT=2, LOBBY_FAIL=3, LOBBY_ACCESS_DENIED=4, LOBBY_LIMIT_EXCEEDED=5,
			PRIVATE=0, FRIENDS_ONLY=1, PUBLIC=2, INVISIBLE=3, LOBBY_KEY_LENGTH=255,
			UGC_MAX_TITLE_CHARS=128, UGC_MAX_DESC_CHARS=8000, UGC_MAX_METADATA_CHARS=5000,
			UGC_ITEM_COMMUNITY=0, UGC_ITEM_MICROTRANSACTION=1,
			UGC_STATE_NONE=0, UGC_STATE_SUBSCRIBED=1, UGC_STATE_LEGACY=2, UGC_STATE_INSTALLED=4, UGC_STATE_UPDATE=8, UGC_STATE_DOWNLOADING=16, UGC_STATE_PENDING=32,
			STATUS_INVALID=0, STATUS_PREPARING_CONFIG=1, STATUS_PREPARING_CONTENT=2, STATUS_UPLOADING_CONTENT=3, STATUS_UPLOADING_PREVIEW=4, STATUS_COMMITTING_CHANGES=5
		};
		CSteamID createSteamID(uint32 steamID, int accountType=-1){
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
		void _ready(){
			// Place for boot up process
			// Perhaps initializing Steam and pulling statistics
		}
		float getVersion(){
			return version;
		}
		// Initialize Steamworks
		bool steamInit(){
			return SteamAPI_Init();
			//printf("Godot Steam initialing...\n");
			IS_INIT_SUCCESS = SteamAPI_Init();
			int err = FAILED;
			if(IS_INIT_SUCCESS){
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
		bool isSteamRunning(void){
			return SteamAPI_IsSteamRunning();
		}
		/////////////////////////////////////////////////
		///// APPS //////////////////////////////////////
		//
		// Check if the user has a given application/game
		bool hasOtherApp(int value){
			if(SteamApps() == NULL){
				return false;
			}
			return SteamApps()->BIsSubscribedApp((AppId_t)value);
		}
		// Get the number of DLC the user owns for a parent application/game
		int getDLCCount(){
			if(SteamApps() == NULL){
				return false;
			}
			return SteamApps()->GetDLCCount();
		}
		// Takes AppID of DLC and checks if the user owns the DLC & if the DLC is installed
		bool isDLCInstalled(int value){
			if(SteamApps() == NULL){
				return false;
			}
			return SteamApps()->BIsDlcInstalled(value);
		}
		// Returns purchase key for given application/game. You'll receive an AppProofOfPurchaseKeyResponse_t callback when
		// the key is available (which may be immediately).
		void requestAppProofOfPurchaseKey(int value){
			if(SteamApps() == NULL){
				return;
			}
			SteamApps()->RequestAppProofOfPurchaseKey(value);
		}
		// Check if given application/game is installed, not necessarily owned
		bool isAppInstalled(int value){
			if(SteamApps() == NULL){
				return false;
			}
			return SteamApps()->BIsAppInstalled((AppId_t)value);
		}
		// Get the user's game language
		String getCurrentGameLanguage(){
			if(SteamApps() == NULL){
				return "None";
			}
			return SteamApps()->GetCurrentGameLanguage();
		}
		// Does the user have a VAC ban for this game
		bool isVACBanned(){
			if(SteamApps() == NULL){
				return false;
			}
			return SteamApps()->BIsVACBanned();
		}
		// Returns the Unix time of the purchase of the app
		int getEarliestPurchaseUnixTime(int value){
			if(SteamApps() == NULL){
				return 0;
			}
			return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
		}
		// Checks if the user is subscribed to the current app through a free weekend
		// This function will return false for users who have a retail or other type of license
		// Suggested you contact Valve on how to package and secure your free weekend properly
		bool isSubscribedFromFreeWeekend(){
			if(SteamApps() == NULL){
				return false;
			}
			return SteamApps()->BIsSubscribedFromFreeWeekend();
		}
		// Install/Uninstall control for optional DLC
		void installDLC(int value){
			if(SteamApps() == NULL){
				return;
			}
			SteamApps()->InstallDLC((AppId_t)value);
		}
		void uninstallDLC(int value){
			if(SteamApps() == NULL){
				return;
			}
			SteamApps()->UninstallDLC((AppId_t)value);
		}
		/////////////////////////////////////////////////
		///// FRIENDS ///////////////////////////////////
		//
		// Get number of friends user has
		int getFriendCount(){
			if(SteamFriends() == NULL){
				return 0;
			}
			return SteamFriends()->GetFriendCount(0x04);
		}
		// Get the user's Steam username
		String getPersonaName(){
			if(SteamFriends() == NULL){
				return "";
			}
			return SteamFriends()->GetPersonaName();
		}
		// Get given friend's Steam username
		String getFriendPersonaName(int steamID){
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
		void setGameInfo(const char *s_key, const char *s_value){
			// Rich presence data is automatically shared between friends in the same game
			// Each user has a set of key/value pairs, up to 20 can be set
			// Two magic keys (status, connect)
			// setGameInfo() to an empty string deletes the key
			if(SteamFriends() == NULL){
				return;
			}
			SteamFriends()->SetRichPresence(s_key, s_value);
		}
		// Clear the game information in Steam; used in 'View Game Info'
		void clearGameInfo(){
			if(SteamFriends() == NULL){
				return;
			}
			SteamFriends()->ClearRichPresence();
		}
		// Invite friend to current game/lobby
		void inviteFriend(int steamID, const char *conString){
			if(SteamFriends() == NULL){
				return;
			}
			CSteamID friendID = createSteamID(steamID);
			SteamFriends()->InviteUserToGame(friendID, conString);
		}
		// Set player as 'Played With' for game
		void setPlayedWith(int steamID){
			if(SteamFriends() == NULL){
				return;
			}
			CSteamID friendID = createSteamID(steamID);
			SteamFriends()->SetPlayedWith(friendID);
		}
		// Get list of players user has recently played game with
		Array getRecentPlayers(){
			if(SteamFriends() == NULL){
				return Array();
			}
			int rCount = SteamFriends()->GetCoplayFriendCount();
			Array recents;
			for(int i=0; i<rCount; i++){
				CSteamID rPlayerID = SteamFriends()->GetCoplayFriend(i);
				if(SteamFriends()->GetFriendCoplayGame(rPlayerID) == SteamUtils()->GetAppID()){
					Dictionary rPlayer;
					String rName = SteamFriends()->GetFriendPersonaName(rPlayerID);
					int rStatus = SteamFriends()->GetFriendPersonaState(rPlayerID);
					rPlayer["id"] = rPlayerID.GetAccountID();
					rPlayer["name"] = rName;
					rPlayer["status"] = rStatus;
					recents.append(rPlayer);
				}
			}
			return recents;
		}
		// Get player's avatar
		void getFriendAvatar(int size=AVATAR_MEDIUM){
			if(size < AVATAR_SMALL || size > AVATAR_LARGE){
				return;
			}
			if(SteamFriends() == NULL){
				return;
			}
			int iHandle = -2;
			switch(size){
				case AVATAR_SMALL:{
					iHandle = SteamFriends()->GetSmallFriendAvatar( SteamUser()->GetSteamID() );
					size = 32; break;
				}
				case AVATAR_MEDIUM:{
					iHandle = SteamFriends()->GetMediumFriendAvatar( SteamUser()->GetSteamID() );
					size = 64; break;
				}
				case AVATAR_LARGE:{
					iHandle = SteamFriends()->GetLargeFriendAvatar( SteamUser()->GetSteamID() );
					size = 184; break;
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
			AvatarImageLoaded_t* avatarData = new AvatarImageLoaded_t;
			avatarData->m_steamID = SteamUser()->GetSteamID();
			avatarData->m_iImage = iHandle;
			avatarData->m_iWide = size;
			avatarData->m_iTall = size;
//			_avatar_loaded(avatarData);
			delete avatarData;
			return;
		}
		// Draw the given avatar
		Image drawAvatar(int iSize, uint8* iBuffer){
			// Apply buffer to Image
			// FORMAT_RGBA changed to FORMAT_RGBA8 due to change in Godot 3.0 image header
			Image avatar(iSize,iSize,false,Image::FORMAT_RGBA8);
			for(int y = 0; y < iSize; y++){
				for(int x=0;x<iSize;x++){
					int i = 4*(x+y*iSize);
					float r = float(iBuffer[i])/255;
					float g = float(iBuffer[i+1])/255;
					float b = float(iBuffer[i+2])/255;
					float a = float(iBuffer[i+3])/255;
					avatar.put_pixel(x,y, Color(r,g,b,a) );
				}
			}
			return avatar;
		}
		// Activates the overlay with optional dialog to open the following: "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite"
		void activateGameOverlay(const char *url){
			if(SteamFriends() == NULL){
				return;
			}
			SteamFriends()->ActivateGameOverlay(url);
		}
		// Activates the overlay to the following: "steamid", "chat", "jointrade", "stats", "achievements", "friendadd", "friendremove", "friendrequestaccept", "friendrequestignore"
		void activateGameOverlayToUser(const char *url, int steamID){
			if(SteamFriends() == NULL){
				return;
			}
			CSteamID overlayUserID = createSteamID(steamID);
			SteamFriends()->ActivateGameOverlayToUser(url, overlayUserID);
		}
		// Activates the overlay with specified web address
		void activateGameOverlayToWebPage(const char *url){
			if(SteamFriends() == NULL){
				return;
			}
			SteamFriends()->ActivateGameOverlayToWebPage(url);
		}
		// Activates the overlay with the application/game Steam store page
		void activateGameOverlayToStore(int app_id=0){
			if(SteamFriends() == NULL){
				return;
			}
			SteamFriends()->ActivateGameOverlayToStore(AppId_t(app_id), EOverlayToStoreFlag(0));
		}
		// Get list of user's Steam groups; a mix of different Steamworks API group functions
		Array getUserSteamGroups(){
			if(SteamFriends() == NULL){
				return Array();
			}
			int groupCount = SteamFriends()->GetClanCount();
			Array steamGroups;
			for(int i=0; i < groupCount; i++){
				Dictionary groups;
				CSteamID groupID = SteamFriends()->GetClanByIndex(i);
				String gName = SteamFriends()->GetClanName(groupID);
				String gTag = SteamFriends()->GetClanTag(groupID);
				groups["id"] = groupID.GetAccountID();
				groups["name"] = gName;
				groups["tag"] = gTag;
				steamGroups.append(groups);
			}
			return steamGroups;
		}
		// Get a list of user's Steam friends; a mix of different Steamworks API friend functions
		Array getUserSteamFriends(){
			if(SteamFriends() == NULL){
				return Array();
			}
			int fCount = SteamFriends()->GetFriendCount(0x04);
			Array steamFriends;
			for(int i=0; i < fCount; i++){
				Dictionary friends;
				CSteamID friendID = SteamFriends()->GetFriendByIndex(i, 0x04);
				String fName = SteamFriends()->GetFriendPersonaName(friendID);
				int fStatus = SteamFriends()->GetFriendPersonaState(friendID);
				friends["id"] = friendID.GetAccountID();
				friends["name"] = fName;
				friends["status"] = fStatus;
				steamFriends.append(friends);
			}
			return steamFriends;
		}
		// Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby
		void activateGameOverlayInviteDialog(int steamID){
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
		void createLobby(int lobbyType, int cMaxMembers){
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
		void joinLobby(int steamIDLobby){
			if(SteamMatchmaking() == NULL){
				return;
			}
			CSteamID lobbyID = createSteamID(steamIDLobby);
			SteamMatchmaking()->JoinLobby(lobbyID);
		}
		// Leave a lobby, this will take effect immediately on the client side, other users will be notified by LobbyChatUpdate_t callback
		void leaveLobby(int steamIDLobby){
			CSteamID lobbyID = createSteamID(steamIDLobby);
			return SteamMatchmaking()->LeaveLobby(lobbyID);
		}
		// Invite another user to the lobby, the target user will receive a LobbyInvite_t callback, will return true if the invite is successfully sent, whether or not the target responds
		bool inviteUserToLobby(int steamIDLobby, int steamIDInvitee){
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
		bool musicIsEnabled(){
			if(SteamMusic() == NULL){
				return false;
			}
			return SteamMusic()->BIsEnabled();
		}
		// Is Steam music playing something
		bool musicIsPlaying(){
			if(SteamMusic() == NULL){
				return false;
			}
			return SteamMusic()->BIsPlaying();
		}
		// Get the volume level of the music
		float musicGetVolume(){
			if(SteamMusic() == NULL){
				return 0;
			}
			return SteamMusic()->GetVolume();
		}
		// Pause whatever Steam music is playing
		void musicPause(){
			if(SteamMusic() == NULL){
				return;
			}
			return SteamMusic()->Pause();
		}
		// Play current track/album
		void musicPlay(){
			if(SteamMusic() == NULL){
				return;
			}
			return SteamMusic()->Play();
		}
		// Play next track/album
		void musicPlayNext(){
			if(SteamMusic() == NULL){
				return;
			}
			return SteamMusic()->PlayNext();
		}
		// Play previous track/album
		void musicPlayPrev(){
			if(SteamMusic() == NULL){
				return;
			}
			return SteamMusic()->PlayPrevious();
		}
		// Set the volume of Steam music
		void musicSetVolume(float value){
			if(SteamMusic() == NULL){
				return;
			}
			return SteamMusic()->SetVolume(value);
		}
		/////////////////////////////////////////////////
		///// SCREENSHOTS ///////////////////////////////
		//
		// Causes Steam overlay to take a screenshot
		void triggerScreenshot(){
			if(SteamScreenshots() == NULL){
				return;
			}
			SteamScreenshots()->TriggerScreenshot();
		}
		/////////////////////////////////////////////////
		///// USERS /////////////////////////////////////
		//
		// Get user's Steam ID
		int getSteamID(){
			if(SteamUser() == NULL){
				return 0;
			}
			CSteamID cSteamID = SteamUser()->GetSteamID();
			return cSteamID.ConvertToUint64();
		}
		// Check, true/false, if user is logged into Steam currently
		bool loggedOn(){
			if(SteamUser() == NULL){
				return false;
			}
			return SteamUser()->BLoggedOn();
		}
		// Get the user's Steam level
		int getPlayerSteamLevel(){
			if(SteamUser() == NULL){
				return 0;
			}
			return SteamUser()->GetPlayerSteamLevel(); 
		}
		// Get the user's Steam installation path
		String getUserDataFolder(){
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
//		// (LEGACY FUNCTION) Set data to be replicated to friends so that they can join your game
//		void advertiseGame(const char server_ip, int port){
//			if(SteamUser() == NULL){
//				return;
//			}
//			// Resolve address and convert it from IP_Address struct to uint32_t
//			IP_Address addr;
//			if(server_ip.IS_VALID_ip_address()){
//				addr = server_ip;
//			}
//			else{
//				addr = IP::get_singleton()->resolve_hostname(server_ip, IP::TYPE_IPV4);
//			}
//			// Resolution failed - Godot 3.0 has is_invalid() to check this
//			if(addr == IP_Address()){
//				return;
//			}
//			uint32_t ip4 = *((uint32_t *)addr.get_ipv4());
//			// Swap the bytes
//			uint8_t *ip4_p = (uint8_t *)&ip4;
//			for(int i=0; i<2; i++){
//				uint8_t temp = ip4_p[i];
//				ip4_p[i] = ip4_p[3-i];
//				ip4_p[3-i] = temp;
//			}
//			CSteamID gameserverID = SteamUser()->GetSteamID();
//			SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
//		}
		// Trading Card badges data access, if you only have one set of cards, the series will be 1
		// The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1)
		int getGameBadgeLevel(int series, bool foil){
			if(SteamUser()== NULL){
				return 0;
			}
			return SteamUser()->GetGameBadgeLevel(series, foil);
		}
		/////////////////////////////////////////////////
		///// USER STATS ////////////////////////////////
		//
		// Clears a given achievement
		bool clearAchievement(const char *s_key){
			return SteamUserStats()->ClearAchievement(s_key);
		}
		// Return true/false if use has given achievement
		bool getAchievement(const char *s_key){
			bool achieved = false;
			SteamUserStats()->GetAchievement(s_key, &achieved);
			return achieved;
		}
		// Get the value of a float statistic
		float getStatFloat(const char *s_key){
			float statval = 0;
			SteamUserStats()->GetStat(s_key, &statval);
			return statval;
		}
		// Get the value of an integer statistic
		int getStatInt(const char *s_key){
			int32 statval = 0;
			SteamUserStats()->GetStat(s_key, &statval);
			return statval;
		}
		// Reset all Steam statistics; optional to reset achievements
		bool resetAllStats(bool bAchievementsToo=true){
			SteamUserStats()->ResetAllStats(bAchievementsToo);
			return SteamUserStats()->StoreStats();
		}
		// Request all statistics and achievements from Steam servers
		bool requestCurrentStats(){
			return SteamUserStats()->RequestCurrentStats();
		}
		// Set a given achievement
		bool setAchievement(const char *s_key){
			if(SteamUserStats() == NULL){
				return 0;
			}
			SteamUserStats()->SetAchievement(s_key);
			return SteamUserStats()->StoreStats();
		}
		// Set a float statistic
		bool setStatFloat(const char *s_key, float value){
			return SteamUserStats()->SetStat(s_key, value);
		}
		// Set an integer statistic
		bool setStatInt(const char *s_key, int value){
			return SteamUserStats()->SetStat(s_key, value);
		}
		// Store all statistics, and achievements, on Steam servers; must be called to "pop" achievements
		bool storeStats(){
			if(SteamUserStats() == NULL){
				return 0;
			}
			SteamUserStats()->StoreStats();
			return SteamUserStats()->RequestCurrentStats();
		}
//		// Find a given leaderboard, by name
//		void findLeaderboard(const char lName){
//			if(SteamUserStats() == NULL){
//				return;
//			}
//			SteamAPICall_t apiCall = SteamUserStats()->FindLeaderboard(lName);
//			callResultFindLeaderboard.Set(apiCall, this, &Steam::_leaderboard_loaded);
//		}
//		// Get the name of a leaderboard
//		String getLeaderboardName(){
//			if(SteamUserStats() == NULL){
//				return "";
//			}
//			return SteamUserStats()->GetLeaderboardName(leaderboard_handle);
//		}
//		// Get the total number of entries in a leaderboard, as of the last request
//		int getLeaderboardEntryCount(){
//			if(SteamUserStats() == NULL){
//				return -1;
//			}
//			return SteamUserStats()->GetLeaderboardEntryCount(leaderboard_handle);
//		}
//		// Request all rows for friends of user
//		void downloadLeaderboardEntries(int rStart, int rEnd, int type){
//			if(SteamUserStats() == NULL){
//				return;
//			}
//			//SteamAPICall_t apiCall = 
//			SteamUserStats()->DownloadLeaderboardEntries(leaderboard_handle, ELeaderboardDataRequest(type), rStart, rEnd);
//		}
//		// Request a maximum of 100 users with only one outstanding call at a time
//		void downloadLeaderboardEntriesForUsers(Array usersID){
//			if(SteamUserStats() == NULL){
//				return;
//			}
//			int users_count = usersID.size();
//			if(users_count == 0){
//				return;
//			}
//			CSteamID *pUsers = new CSteamID[users_count];
//			for(int i = 0; i < users_count; i++){
//				CSteamID user = createSteamID(usersID[i]);
//				pUsers[i] = user;
//			}
//			SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboard_handle, pUsers, users_count);
//			delete[] pUsers;
//		}
//		// Upload a leaderboard score for the user
//		void uploadLeaderboardScore(int score, bool keepBest=false){
//			if(SteamUserStats() == NULL){
//				return;
//			}
//			int method = 0;
//			if(keepBest){
//				method = 1;
//			}
//			else{
//				method = 2;
//			}
//			SteamUserStats()->UploadLeaderboardScore(leaderboard_handle, ELeaderboardUploadScoreMethod(method), (int32)score, NULL, 0);
//		}
//		// Once all entries are accessed, the data will be freed up and the handle will become invalid, use this to store it
//		void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount){
//			if(SteamUserStats() == NULL){
//				return;
//			}
//			leaderboard_entries.clear();
//			for(int i = 0; i < entryCount; i++){
//				LeaderboardEntry_t *entry=NULL;
//				SteamUserStats()->GetDownloadedLeaderboardEntry(eHandle, i, entry, NULL, 0);
//				Dictionary entryDict;
//				entryDict["score"] = entry->m_nScore;
//				entryDict["steam_id"] = entry->m_steamIDUser.GetAccountID();
//				entryDict["global_rank"] = entry->m_nGlobalRank;
//				leaderboard_entries.append(entryDict);
//			}
//		}
//		// Update the currently used leaderboard handle
//		void updateLeaderboardHandle(SteamLeaderboard_t lHandle){
//			leaderboard_handle = (uint64)lHandle;
//			IS_VALID = false;
//			if(leaderboard_handle > 0 && SteamUserStats() != NULL){
//				IS_VALID = true;
//			}
//		}
//		// Get the currently used leaderboard handle
//		uint64 getLeaderboardHandle(){
//			return leaderboard_handle;
//		}
//		// Get the currently used leaderboard entries
//		Array getLeaderboardEntries(){
//			return leaderboard_entries;
//		}
		// Get the achievement status, and the time it was unlocked if unlocked (in seconds since January 1, 19)
		bool getAchievementAndUnlockTime(const char *name, bool *achieved, int *unlockTime){
			if(SteamUserStats() == NULL){
				return 0;
			}
			return SteamUserStats()->GetAchievementAndUnlockTime(name, (bool *)achieved, (uint32_t *)unlockTime);
		}
		// Achievement progress, triggers an AchievementProgress callback, that is all.
		// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
		bool indicateAchievementProgress(const char *name, int curProgress, int maxProgress){
			if(SteamUserStats() == NULL){
				return 0;
			}
			return SteamUserStats()->IndicateAchievementProgress(name, curProgress, maxProgress);
		}
		/////////////////////////////////////////////////
		///// UTILS /////////////////////////////////////
		//
		// Get the user's country by IP
		String getIPCountry(){
			return SteamUtils()->GetIPCountry();
		}
		// Returns true/false if Steam overlay is enabled
		bool isOverlayEnabled(){
			return SteamUtils()->IsOverlayEnabled();
		}
		// Set the position where overlay shows notifications
		void setOverlayNotificationPosition(int pos){
			if((pos < 0) || (pos > 3) || (SteamUtils() == NULL)){
				return;
			}
			SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
		}
		// Get the Steam user interface language
		String getSteamUILanguage(){
			return SteamUtils()->GetSteamUILanguage();
		}
		// Get the Steam ID of the running application/game
		int getAppID(){
			if(SteamUtils() == NULL){
				return 0;
			}
			return SteamUtils()->GetAppID();
		}
		// Return amount of time, in seconds, user has spent in this session
		int getSecondsSinceAppActive(){
			if(SteamUtils() == NULL){
				return 0;
			}
			return SteamUtils()->GetSecondsSinceAppActive();
		}
		// Get the amount of battery power, clearly for laptops
		int getCurrentBatteryPower(){
			if(SteamUtils() == NULL){
				return 0;
			}
			return SteamUtils()->GetCurrentBatteryPower();
		}
		// Is Steam running in VR?
		bool isSteamRunningInVR(){
			if(SteamUtils() == NULL){
				return 0;
			}
			return SteamUtils()->IsSteamRunningInVR();
		}
		// Get the actual time
		int getServerRealTime(){
			if(SteamUtils() == NULL){
				return 0;
			}
			return SteamUtils()->GetServerRealTime();
		}
		// Returns true if Steam & the Steam Overlay are running in Big Picture mode
		bool isSteamInBigPictureMode(){
			if(SteamUtils() == NULL){
				return false;
			}
			return SteamUtils()->IsSteamInBigPictureMode();
		}
		// Ask SteamUI to create and render its OpenVR dashboard
		void startVRDashboard(){
			if(SteamUtils() == NULL){
				return ;
			}
			SteamUtils()->StartVRDashboard();
		}
		/////////////////////////////////////////////////
		///// WORKSHOP //////////////////////////////////
		//
		// Get the number of subscribed items for the user
		int getNumSubscribedItems(){
			if(SteamUGC() == NULL){
				return 0;
			}
			return SteamUGC()->GetNumSubscribedItems();
		}
		// Get the item's state
		int getItemState(int publishedFileID){
			if(SteamUGC() == NULL){
				return 0;
			}
			PublishedFileId_t fileID = (int)publishedFileID;
			return SteamUGC()->GetItemState(fileID);
		}
		// Download new or update already installed item. If returns true, wait for DownloadItemResult_t. If item is already installed, then files on disk should not be used until callback received.
		// If item is not subscribed to, it will be cached for some time. If bHighPriority is set, any other item download will be suspended and this item downloaded ASAP.
		bool downloadItem(int publishedFileID, bool bHighPriority){
			if(SteamUGC() == NULL){
				return 0;
			}
			PublishedFileId_t fileID = (int)publishedFileID;
			return SteamUGC()->DownloadItem(fileID, bHighPriority);
		}
		// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends
		void suspendDownloads(bool bSuspend){
			return SteamUGC()->SuspendDownloads(bSuspend);
		}
		/////////////////////////////////////////////////
		///// REGISTER METHODS //////////////////////////
		//
		static void _register_methods(){
			register_method("_ready", &Steam::_ready);
			register_method("getVersion", &Steam::getVersion);
			register_method("steamInit", &Steam::steamInit);
			register_method("isSteamRunning", &Steam::isSteamRunning);
			register_method("run_callbacks", &Steam::run_callbacks);
			// Apps Bind Methods ////////////////////////
			register_method("hasOtherApp", &Steam::hasOtherApp);
			register_method("getDLCCount", &Steam::getDLCCount);
			register_method("isDLCInstalled", &Steam::isDLCInstalled);
			register_method("requestAppProofOfPurchaseKey", &Steam::requestAppProofOfPurchaseKey);
			register_method("isAppInstalled", &Steam::isAppInstalled);
			register_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
			register_method("isVACBanned", &Steam::isVACBanned);
			register_method("getEarliestPurchaseUnixTime", &Steam::getEarliestPurchaseUnixTime);
			register_method("isSubscribedFromFreeWeekend", &Steam::isSubscribedFromFreeWeekend);
			register_method("installDLC", &Steam::installDLC);
			register_method("uninstallDLC", &Steam::uninstallDLC);
			// Friends Bind Methods /////////////////////
			register_method("getFriendCount", &Steam::getFriendCount);
			register_method("getPersonaName", &Steam::getPersonaName);
			register_method("getFriendPersonaName", &Steam::getFriendPersonaName);
			register_method("setGameInfo", &Steam::setGameInfo);
			register_method("clearGameInfo", &Steam::clearGameInfo);
			register_method("inviteFriend", &Steam::inviteFriend);
			register_method("setPlayedWith", &Steam::setPlayedWith);
			register_method("getRecentPlayers", &Steam::getRecentPlayers);
			register_method("getFriendAvatar", &Steam::getFriendAvatar);
			register_method("getUserSteamGroups", &Steam::getUserSteamGroups);
			register_method("getUserSteamFriends", &Steam::getUserSteamFriends);
			register_method("activateGameOverlay", &Steam::activateGameOverlay);
			register_method("activateGameOverlayToUser", &Steam::activateGameOverlayToUser);
			register_method("activateGameOverlayToWebPage", &Steam::activateGameOverlayToWebPage);
			register_method("activateGameOverlayToStore", &Steam::activateGameOverlayToStore);
			register_method("activateGameOverlayInviteDialog", &Steam::activateGameOverlayInviteDialog);
			// Matchmaking Bind Methods /////////////////
			register_method("createLobby", &Steam::createLobby);
			register_method("joinLobby", &Steam::joinLobby);
			register_method("leaveLobby", &Steam::leaveLobby);
			register_method("inviteUserToLobby", &Steam::inviteUserToLobby);
			// Music Bind Methods ///////////////////////
			register_method("musicIsEnabled", &Steam::musicIsEnabled);
			register_method("musicIsPlaying", &Steam::musicIsPlaying);
			register_method("musicGetVolume", &Steam::musicGetVolume);
			register_method("musicPause", &Steam::musicPause);
			register_method("musicPlay", &Steam::musicPlay);
			register_method("musicPlayNext", &Steam::musicPlayNext);
			register_method("musicPlayPrev", &Steam::musicPlayPrev);
			register_method("musicSetVolume", &Steam::musicSetVolume);
			// Screenshoot Bind Methods /////////////////
			register_method("triggerScreenshot", &Steam::triggerScreenshot);
			// User Bind Methods ////////////////////////
			register_method("getSteamID", &Steam::getSteamID);
			register_method("loggedOn", &Steam::loggedOn);
			register_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
			register_method("getUserDataFolder", &Steam::getUserDataFolder);
//			register_method("advertiseGame", &Steam::advertiseGame);
			register_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);
			// User Stats Bind Methods //////////////////
			register_method("clearAchievement", &Steam::clearAchievement);
			register_method("getAchievement", &Steam::getAchievement);
			register_method("getStatFloat", &Steam::getStatFloat);
			register_method("getStatInt", &Steam::getStatInt);
			register_method("resetAllStats", &Steam::resetAllStats);
			register_method("requestCurrentStats", &Steam::requestCurrentStats);
			register_method("setAchievement", &Steam::setAchievement);
			register_method("setStatFloat", &Steam::setStatFloat);
			register_method("setStatInt", &Steam::setStatInt);
			register_method("storeStats", &Steam::storeStats);
//			register_method("findLeaderboard", &Steam::findLeaderboard);
//			register_method("getLeaderboardName", &Steam::getLeaderboardName);
//			register_method("getLeaderboardEntryCount", &Steam::getLeaderboardEntryCount);
//			register_method("downloadLeaderboardEntries", &Steam::downloadLeaderboardEntries);
//			register_method("downloadLeaderboardEntriesForUsers", &Steam::downloadLeaderboardEntriesForUsers);
//			register_method("uploadLeaderboardScore", &Steam::uploadLeaderboardScore);
//			register_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
			register_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
			register_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);
			// Utils Bind Methods ///////////////////////
			register_method("getIPCountry", &Steam::getIPCountry);
			register_method("isOverlayEnabled", &Steam::isOverlayEnabled);
			register_method("getSteamUILanguage", &Steam::getSteamUILanguage);
			register_method("getAppID", &Steam::getAppID);
			register_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
			register_method("setOverlayNotificationPosition", &Steam::setOverlayNotificationPosition);
			register_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
			register_method("getServerRealTime", &Steam::getServerRealTime);
			register_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
			register_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
			register_method("startVRDashboard", &Steam::startVRDashboard);
			// Workshop Bind Methods ////////////////////
			register_method("getNumSubscribedItems", &Steam::getNumSubscribedItems);
			register_method("getItemState", &Steam::getItemState);
			register_method("downloadItem", &Steam::downloadItem);
			register_method("suspendDownloads", &Steam::suspendDownloads);
		}
};

GODOT_NATIVE_INIT(godot_native_init_options *options){
	godot::register_class<Steam>();
}