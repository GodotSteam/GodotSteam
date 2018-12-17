#ifndef GODOTSTEAM_SERVER_H
#define GODOTSTEAM_SERVER_H

#include <inttypes.h>
#include <steam/steam_gameserver.h>

#include "object.h"
#include "scene/resources/texture.h"	// For avatars
#include "reference.h"
#include "dictionary.h"					// Contains array.h as well

class SteamServer: public Object {
	GDCLASS(SteamServer, Object);
	public:
		enum {
			RESULT_OK=1, RESULT_FAIL=2, RESULT_NO_CONNECT=3, RESULT_INVALID_PASSWORD=5, RESULT_LOGGED_IN_ESLEWHERE=6, RESULT_INVALID_PROTOCAL=7, RESULT_INALID_PARAM=8, RESULT_FILE_NOT_FOUND=9, RESULT_BUSY=10, RESULT_INVALID_STATE=11, RESULT_INVALID_NAME=12,
			RESULT_INVALID_EMAIL=13, RESULT_DUPLICATE_NAME=14, RESULT_ACCESS_DENIED=15, RESULT_TIMEOUT=16, RESULT_BANNED=17, RESULT_ACCOUNT_NOT_FOUND=18, RESULT_INVALID_STEAM_ID=19, RESULT_SERVICE_UNAVAILABLE=20, RESULT_NOT_LOGGED_ON=21, RESULT_PENDING=22,
			RESULT_ENCRYPT_FAILURE=23, RESULT_INSUFFICIENT_PRIVILEGE=24, RESULT_LIMIT_EXCEEDED=25, RESULT_REVOKED=26, RESULT_EXPIRED=27, RESULT_ALREADY_REDEEMED=28, RESULT_DUPLICATE_REQUEST=29, RESULT_ALREADY_OWNED=30, RESULT_IP_NOT_FOUND=31, RESULT_PERSIST_FAILED=32,
			RESULT_LOCKING_FAILED=33, RESULT_LOGON_SESSION_REPLACED=34, RESULT_CONNECT_FAILED=35, RESULT_HANDSHAKE_FAILED=36, RESULT_IO_FAILURE=37, RESULT_REMOTE_DISCONNECT=38, RESULT_SHOPPING_CART_NOT_FOUND=39, RESULT_BLOCKED=40, RESULT_IGNORED=41, RESULT_NO_MATCH=42,
			RESULT_ACCOUNT_DISABLED=43, RESULT_SERVICE_READY_ONLY=44, RESULT_ACCOUNT_NOT_FEATURED=45, RESULT_ADMINISTRATOR_OK=46, RESULT_CONTENT_VERSION=47, RESULT_TRY_ANOTHER_CM=48, RESULT_PASSWORD_REQUIRED_TO_KICK=49, RESULT_ALREADY_LOGGED_ELSEWHERE=50,
			RESULT_SUSPENDED=51, RESULT_CANCELLED=52, RESULT_DATA_CORRUPTION=53, RESULT_DISK_FULL=54, RESULT_REMOTE_CALL_FAILED=55, RESULT_PASSWORD_UNSET=56, RESULT_EXTERNAL_ACCOUNT_UNLINKED=57, RESULT_PSN_TICKET_INVALID=58, RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED=59,
			RESULT_REMOTE_FILE_CONFLICT=60, RESULT_ILLEGAL_PASSWORD=61, RESULT_SAME_AS_PREVIOUS_VALUE=62, RESULT_ACCOUNT_LOGON_DENIED=63, RESULT_CANNOT_USE_OLD_PASSWORD=64, RESULT_INVALID_LOGIN_AUTH_CODE=65, RESULT_ACCOUNT_LOGON_DENIED_NO_MAIL=66, RESULT_HARDWARE_NOT_CAPABLE=67,
			RESULT_IP_INIT_ERROR=68, RESULT_PARENTAL_CONTROL_RESTRICTED=69, RESULT_FACEBOOK_QUERY_ERROR=70, RESULT_EXPIRED_LOGIN_AUTH_CODE=71, RESULT_IP_LOGIN_RESTRICTION_FAILED=72, RESULT_ACCOUNT_LOCKED_DOWN=73, RESULT_ACCOUNT_LOGON_DENIED_VERIFIED_EMAIL_REQUIRED=74,
			RESULT_NO_MATCHING_URL=75, RESULT_BAD_RESPONSE=76, RESULT_REQUIRED_PASSWORD_REENTRY=77, RESULT_VALUE_OUT_OF_RANGE=78, RESULT_UNEXPECTED_ERROR=79, RESULT_DISABLED=80, RESULT_INVALID_CEG_SUBMISSION=81, RESULT_RESTRICTED_DEVICE=82, RESULT_REGION_LOCKED=83,
			RESULT_RATE_LIMIT_EXCEEDED=84, RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR=85, RESULT_ITEM_DELETED=86, RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE=87, RESULT_TWO_FACTOR_CODE_MISMATCH=88, RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH=89, RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS=90,
			RESULT_NOT_MODIFIED=91, RESULT_NO_MOBILE_DEVICE=92, RESULT_TIME_NOT_SYNCED=93, RESULT_SMS_CODE_FAILED=94, RESULT_ACCOUNT_LIMIT_EXCEEDED=95, RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED=96, RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED=97, RESULT_REFUND_TO_WALLET=98,
			RESULT_EMAIL_SEND_FAILURE=99, RESULT_NOT_SETTLED=100, RESULT_NEED_CAPTCHA=101, RESULT_GSLT_DENIED=102, RESULT_GS_OWNER_DENIED=103,RESULT_INVALID_ITEM_TYPE=104, RESULT_IP_BANNED=105, RESULT_GSLT_EXPIRED=106, RESULT_INSUFFICIENT_FUNDS=107, RESULT_TOO_MANY_PENDING=108,
			MODE_INVALID=0, MODE_NO_AUTH=1, MODE_AUTH=2, MODE_AUTH_SECURE=3,
			AUTH_OK=0, AUTH_INVALID_TICKET=1, AUTH_DUPLICATE=2, AUTH_INVALID_VERSION=3, AUTH_MISMATCH=4, AUTH_EXPIRED_TICKET=5,
			LICENSE_HAS=0, LICENSE_DOES_NOT=1, LICENSE_NO_AUTH=2,
			DENY_INVALID=0, DENY_INVALID_VERSION=1, DENY_GENERIC=2, DENY_NOT_LOGGED_ON=3, DENY_NO_LICENSE=4, DENY_CHEATER=5, DENY_LOGGED_IN_ELSEWHERE=6, DENY_UNKNOWN_TEXT=7, DENY_INCOMPAT_ANTICHEAT=8, DENY_MEM_CORRUPTION=9,
			DENY_INCOMPAT_SOFTWARE=10, DENY_STEAM_CONNECTION_LOST=11, DENY_STEAM_CONNECTION_ERROR=12, DENY_STEAM_RESPONSE_TIMEOUT=13, DENY_STEAM_VALIDATION_STALLED=14, DENY_STEAM_OWNER_LEFT_GUEST=15
		};
		static SteamServer* get_singleton();
		SteamServer();
		~SteamServer();

		CSteamID createSteamID(uint32_t steamID, int accountType=-1);
		// Steamworks ///////////////////////////
		bool serverInit(uint32 ip, uint16 steamPort, uint16 gamePort, uint16 queryPort, EServerMode serverMode, const String& versionString);
		void serverReleaseCurrentThreadMemory();
		void serverShutdown();
		// Server ///////////////////////////////
		bool initGameServer(uint32 ip, uint16 steamPort, uint16 gamePort, uint16 queryPort, EServerMode serverMode, const String& versionString);
		void setProduct(const String& product);
		void setGameDescription(const String& description);
		void setModDir(const String& modDir);
		void setDedicatedServer(bool dedicated);
		void logOn(const String& token);
		void logOnAnonymous();
		void logOff();
		bool loggedOn();
		bool secure();
		uint64_t getSteamID();
		bool wasRestartRequested();
		void setMaxPlayerCount(int max);
		void setBotPlayerCount(int bots);
		void setServerName(const String& name);
		void setMapName(const String& map);
		void setPasswordProtected(bool password);
		void setSpectatorPort(uint16 port);
		void setSpectatorServerName(const String& name);
		void clearAllKeyValues();
		void setKeyValue(const String& key, const String& value);
		void setGameTags(const String& tags);
		void setGameData(const String& data);
		void setRegion(const String& region);
		bool sendUserConnectAndAuthenticate(uint32 ipClient, const void *authBlob, uint32 authBlobSize, uint32_t steamID);
		uint64_t createUnauthenticatedUserConnection();
<<<<<<< HEAD:godotsteam/godotsteam_server.h
		void sendUserDisconnect(uint64_t steamID);
		bool updateUserData(uint64_t steamID, const String& *name, uint32 score);
=======
		void sendUserDisconnect(int steamID);
		bool updateUserData(int steamID, const String& name, uint32 score);
>>>>>>> bcb36607dd1fc27ed462b35a47cdf322950f44a8:godotsteam_server/godotsteam_server.h
		uint32 getAuthSessionTicket(void *ticket, int maxTicket, uint32 *pcbTicket);
		int beginAuthSession(const void *authTicket, int cbAuthTicket, int steamID);
		void endAuthSession(uint64_t steamID);
		void cancelAuthTicket(int authTicket);
		int userHasLicenceForApp(uint64_t steamID, AppId_t appID);
		bool requestUserGroupStatus(int steamID, int groupID);
		bool handleIncomingPacket(const void *data, int cbData, uint32 ip, uint16 port);
		int getNextOutgoingPacket(void *out, int maxOut, uint32 *address, uint16 *port);
		void enableHeartbeats(bool active);
		void setHeartbeatInterval(int interval);
		void forceHeartbeat();
<<<<<<< HEAD:godotsteam/godotsteam_server.h
		void associateWithClan(uint64_t clanID);
		void computeNewPlayerCompatibility(uint64_t steamID);
=======
		void associateWithClan(int clanID);
		void computeNewPlayerCompatibility(int steamID);
>>>>>>> bcb36607dd1fc27ed462b35a47cdf322950f44a8:godotsteam_server/godotsteam_server.h

	protected:
		static void _bind_methods();
		static SteamServer* singleton;
	private:
		bool isInitSuccess;
		// Authentication
		struct TicketData {
			uint32_t id;
			uint32_t *buffer;
			uint32_t size;
		};
		Vector<TicketData> tickets;
		/////////////////////////////////////////
		// STEAM SERVER CALLBACKS ///////////////
		//
		// Server callbacks
		STEAM_GAMESERVER_CALLBACK(SteamServer, _server_Connect_Failure, SteamServerConnectFailure_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _server_Connected, SteamServersConnected_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _server_Disconnected, SteamServersDisconnected_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Approved, GSClientApprove_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Denied, GSClientDeny_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Kick, GSClientKick_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _policy_Response, GSPolicyResponse_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _client_Group_Status, GSClientGroupStatus_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _associate_Clan, AssociateWithClanResult_t);
		STEAM_GAMESERVER_CALLBACK(SteamServer, _player_Compat, ComputeNewPlayerCompatibilityResult_t);
		// Run the Steamworks server API callbacks
		void run_callbacks(){
			SteamGameServer_RunCallbacks();
		}
};
#endif // GODOTSTEAM_SERVER_H