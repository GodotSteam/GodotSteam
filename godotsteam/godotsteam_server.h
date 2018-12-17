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
			RESULT_OK=1, RESULT_FAIL=2, RESULT_NO_CONNECT=3, RESULT_TIMEOUT=16,
			MODE_INVALID=0, MODE_NO_AUTH=1, MODE_AUTH=2, MODE_AUTH_SECURE=3,
			AUTH_OK=0, AUTH_INVALID_TICKET=1, AUTH_DUPLICATE=2, AUTH_INVALID_VERSION=3, AUTH_MISMATCH=4, AUTH_EXPIRED_TICKET=5
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
		void setPasswordProtected(bool protected);
		void setSpectatorPort(uint16 port);
		void setSpectatorServerName(const String& name);
		void clearAllKeyValues();
		void setKeyValue(const String& key, const String& value);
		void setGameTags(const String& tags);
		void setGameData(const String& data);
		void setRegion(const String& region);
		bool sendUserConnectAndAuthenticate(uint32 ipClient, const void *authBlob, uint32 authBlobSize, int steamID);
		uint64_t createUnauthenticatedUserConnection();
		void sendUserDisconnect(uint64_t steamID);
		bool updateUserData(uint64_t steamID, const String& *name, uint32 score);
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
		void associateWithClan(uint64_t clanID);
		void computeNewPlayerCompatibility(uint64_t steamID);

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
		}
		Vector<TicketData> tickets;
		/////////////////////////////////////////
		// STEAM SERVER CALLBACKS ///////////////
		//
		// Server callbacks
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
}
#endif // GODOTSTEAM_SERVER_H