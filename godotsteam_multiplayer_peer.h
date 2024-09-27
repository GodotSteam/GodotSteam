#ifndef GODOTSTEAM_MULTIPLAYER_PEER_H
#define GODOTSTEAM_MULTIPLAYER_PEER_H


// SILENCE STEAMWORKS WARNINGS
/////////////////////////////////////////////////
//
// Turn off MSVC-only warning about strcpy
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable : 4996)
#pragma warning(disable : 4828)
#endif


// INCLUDE HEADERS
/////////////////////////////////////////////////
//
// Include Godot headers
#include "core/os/os.h"
#include "scene/main/multiplayer_peer.h"

// Include Steam SDK headers through Godotsteam
//#include "../godotsteam/sdk/public/steam/steam_api_flat.h"

#include "godotsteam_connection_data.h"

#include "godotsteam_constants.h"


// DEFINE STUFF
/////////////////////////////////////////////////
// To-do: make these empty for release builds
#define DEBUG_DATA_SIGNAL_V(msg, value) \
	{                                   \
		Dictionary a;                   \
		a["msg"] = msg;                 \
		a["value"] = value;             \
		emit_signal("debug_data", a);   \
	}
#define DEBUG_DATA_SIGNAL(msg)        \
	{                                 \
		Dictionary a;                 \
		a["msg"] = msg;               \
		emit_signal("debug_data", a); \
	}
#define DEBUG_CON_DATA_SIGNAL(con, msg) \
	if (unlikely(con)) {                \
		Dictionary a;                   \
		a["msg"] = msg;                 \
		emit_signal("debug_data", a);   \
	}
#define DEBUG_CON_DATA_SIGNAL_V(con, msg, value) \
	if (unlikely(con)) {                         \
		Dictionary a;                            \
		a["msg"] = msg;                          \
		a["value"] = value;                      \
		emit_signal("debug_data", a);            \
	}


// TIME FOR CLASS
/////////////////////////////////////////////////
//
class SteamMultiplayerPeer : public MultiplayerPeer {

	GDCLASS(SteamMultiplayerPeer, MultiplayerPeer);


public:

	SteamMultiplayerPeer();
	~SteamMultiplayerPeer();


	// STEAMWORKS API ENUMS
	/////////////////////////////////////////////
	//
	enum LobbyState {
		LOBBY_STATE_NOT_CONNECTED,
		LOBBY_STATE_HOST_PENDING,
		LOBBY_STATE_HOSTING,
		LOBBY_STATE_CLIENT_PENDING,
		LOBBY_STATE_CLIENT
	} lobby_state = LobbyState::LOBBY_STATE_NOT_CONNECTED;


	enum LobbyType {
		LOBBY_TYPE_PRIVATE = ELobbyType::k_ELobbyTypePrivate,
		LOBBY_TYPE_FRIENDS_ONLY = ELobbyType::k_ELobbyTypeFriendsOnly,
		LOBBY_TYPE_PUBLIC = ELobbyType::k_ELobbyTypePublic,
		LOBBY_TYPE_INVISIBLE = ELobbyType::k_ELobbyTypeInvisible,
		LOBBY_TYPE_PRIVATE_UNIQUE = k_ELobbyTypePrivateUnique //this type must be created from Steam's web api.
	};

	
	// INTERNAL
	/////////////////////////////////////////////
	//
	void add_connection_peer(const CSteamID &steam_id, int peer_id);
	void add_pending_peer(const CSteamID &steam_id);

	
	int get_steam_transfer_flag();
	_FORCE_INLINE_ bool _is_active() const { return lobby_state != LobbyState::LOBBY_STATE_NOT_CONNECTED; }


	// FUNCTIONS
	/////////////////////////////////////////////
	//
	Dictionary collect_debug_data();
	Error create_lobby(LobbyType new_lobby_type, int max_players);

	Dictionary get_all_lobby_data();
	bool get_as_relay() { return as_relay; }
	Ref<ConnectionData> get_connection_by_peer(int peer_id);
	Array get_direct_messages();
	uint64_t get_lobby_id();
	String get_lobby_data(String key);
	bool get_no_delay() { return no_delay; }
	bool get_no_nagle() { return no_nagle; }
	int get_peer_by_steam_id(CSteamID steam_id);
	int get_peer_id_from_steam64(uint64_t steam_id);
	Dictionary get_peer_info(int peer_id);
	Dictionary get_peer_map();
	CSteamID get_steam_id_by_peer(int peer);
	uint64_t get_steam64_from_peer_id(int peer);

	Error join_lobby(uint64_t lobbyId);

	// void poll_channel(int nLocalChannel, void (*func)(SteamNetworkingMessage_t));
	void process_message(const SteamNetworkingMessage_t *msg);
	void process_ping(const SteamNetworkingMessage_t *msg);

	void removed_connection_peer(const CSteamID &steam_id);

	bool send_direct_message(PackedByteArray message_data);

	void set_as_relay(bool value) { as_relay = value; }
	bool set_lobby_data(String key, String data);
	void set_lobby_joinable(bool now_joinable);
	void set_no_delay(bool value) { no_delay = value; }
	void set_no_nagle(bool value) { no_nagle = value; }

	void set_steam_id_peer(CSteamID steam_id, int peer_id);
	virtual void set_target_peer(int p_peer_id) override;
	Dictionary steam_id_to_dict(CSteamID input);

	// MultiplayerPeer //////////////////////////
	virtual void close() override;

	virtual void disconnect_peer(int p_peer, bool p_force = false) override;

	virtual bool is_server() const override;
	virtual bool is_server_relay_supported() const override;

	virtual int get_available_packet_count() const override;
	virtual ConnectionStatus get_connection_status() const override;
	virtual int get_max_packet_size() const override;	
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size) override;
	virtual int get_packet_channel() const override;
	virtual TransferMode get_packet_mode() const override;
	virtual int get_packet_peer() const override;
	LobbyState get_state() { return lobby_state; }
	virtual int get_unique_id() const override;

	virtual void poll() override;
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size) override;


	// PROPERTIES
	/////////////////////////////////////////////
	//
	bool as_relay = false;
	bool no_delay = false;
	bool no_nagle = false;


protected:
	static void _bind_methods();


private:
	// ConnectionStatus connection_status = ConnectionStatus::CONNECTION_DISCONNECTED;
	HashMap<int64_t, Ref<ConnectionData>> connections_by_steam_id64;

	List<ConnectionData::Packet *> incoming_packets;

	CSteamID lobby_id = CSteamID();
	CSteamID lobby_owner = CSteamID();

	ConnectionData::Packet *next_received_packet = new ConnectionData::Packet; // This packet gets deleted at the first get_packet request
	ConnectionData::Packet *next_send_packet = new ConnectionData::Packet;

	HashMap<int, Ref<ConnectionData>> peer_id_to_steam_id;

	CSteamID steam_id = CSteamID();

	int32_t target_peer = -1;
	// TransferMode transfer_mode = TransferMode::TRANSFER_MODE_RELIABLE;

	int32_t unique_id = -1;


	// STEAM CALLBACKS
	/////////////////////////////////////////////
	//
	// Matchmaking callbacks ////////////////////
	STEAM_CALLBACK(SteamMultiplayerPeer, lobby_chat_update, LobbyChatUpdate_t, callbackLobbyChatUpdate);
	STEAM_CALLBACK(SteamMultiplayerPeer, lobby_data_update, LobbyDataUpdate_t, callbackLobbyDataUpdate);
	STEAM_CALLBACK(SteamMultiplayerPeer, lobby_joined, LobbyEnter_t, callbackLobbyJoined);
	STEAM_CALLBACK(SteamMultiplayerPeer, lobby_message, LobbyChatMsg_t, callbackLobbyMessage);

	// Networking Messages callbacks ////////
	STEAM_CALLBACK(SteamMultiplayerPeer, network_messages_session_failed, SteamNetworkingMessagesSessionFailed_t, callbackNetworkMessagesSessionFailed);
	STEAM_CALLBACK(SteamMultiplayerPeer, network_messages_session_request, SteamNetworkingMessagesSessionRequest_t, callbackNetworkMessagesSessionRequest);


	// STEAM CALL RESULTS
	/////////////////////////////////////////
	//
	// Matchmaking call results ///////////// stolen
	CCallResult<SteamMultiplayerPeer, LobbyCreated_t> callResultCreateLobby;
	void lobby_created(LobbyCreated_t *call_data, bool io_failure);
};


VARIANT_ENUM_CAST(SteamMultiplayerPeer::LobbyType);
VARIANT_ENUM_CAST(SteamMultiplayerPeer::LobbyState);


#endif // GODOTSTEAM_MULTIPLAYER_PEER_H