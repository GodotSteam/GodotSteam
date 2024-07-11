#include "steam_multiplayer_peer.h"
#include "core/io/json.h"
#include "../godotsteam/godotsteam.h"

VARIANT_ENUM_CAST(SteamMultiplayerPeer::LOBBY_TYPE);
VARIANT_ENUM_CAST(SteamMultiplayerPeer::CHAT_CHANGE);
VARIANT_ENUM_CAST(SteamMultiplayerPeer::LOBBY_STATE);

SteamMultiplayerPeer::SteamMultiplayerPeer() :
		callbackLobbyMessage(this, &SteamMultiplayerPeer::lobby_message_scb),
		callbackLobbyChatUpdate(this, &SteamMultiplayerPeer::lobby_chat_update_scb),
		callbackNetworkMessagesSessionRequest(this, &SteamMultiplayerPeer::network_messages_session_request_scb),
		callbackNetworkMessagesSessionFailed(this, &SteamMultiplayerPeer::network_messages_session_failed_scb),
		callbackLobbyJoined(this, &SteamMultiplayerPeer::lobby_joined_scb),
		callbackLobbyDataUpdate(this, &SteamMultiplayerPeer::lobby_data_update_scb) {
	if (SteamUser() != NULL) {
		steam_id = SteamUser()->GetSteamID();
	}
}

SteamMultiplayerPeer::~SteamMultiplayerPeer() {
	if (lobby_id != CSteamID()) {
		SteamMatchmaking()->LeaveLobby(lobby_id);
		//todo disconnect all connected peers?
	}
}

uint64_t SteamMultiplayerPeer::get_lobby_id() {
	return lobby_id.ConvertToUint64();
}

void SteamMultiplayerPeer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_lobby", "lobby_type", "max_players"), &SteamMultiplayerPeer::create_lobby, DEFVAL(32));
	ClassDB::bind_method(D_METHOD("connect_lobby", "lobby_id"), &SteamMultiplayerPeer::join_lobby);
	ClassDB::bind_method(D_METHOD("get_state"), &SteamMultiplayerPeer::get_state);
	ClassDB::bind_method(D_METHOD("get_lobby_id"), &SteamMultiplayerPeer::get_lobby_id);
	ClassDB::bind_method(D_METHOD("collect_debug_data"), &SteamMultiplayerPeer::collect_debug_data);

	ClassDB::bind_method(D_METHOD("get_no_nagle"), &SteamMultiplayerPeer::get_no_nagle);
	ClassDB::bind_method(D_METHOD("set_no_nagle"), &SteamMultiplayerPeer::set_no_nagle);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_nagle"), "set_no_nagle", "get_no_nagle");

	ClassDB::bind_method(D_METHOD("get_no_delay"), &SteamMultiplayerPeer::get_no_delay);
	ClassDB::bind_method(D_METHOD("set_no_delay"), &SteamMultiplayerPeer::set_no_delay);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_delay"), "set_no_delay", "get_no_delay");

	ClassDB::bind_method(D_METHOD("get_as_relay"), &SteamMultiplayerPeer::get_as_relay);
	ClassDB::bind_method(D_METHOD("set_as_relay"), &SteamMultiplayerPeer::set_as_relay);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "as_relay"), "set_as_relay", "get_as_relay");

	ClassDB::bind_method(D_METHOD("get_steam64_from_peer_id","peer_id"), &SteamMultiplayerPeer::get_steam64_from_peer_id);
	ClassDB::bind_method(D_METHOD("get_peer_id_from_steam64","steam_id"), &SteamMultiplayerPeer::get_peer_id_from_steam64);
	ClassDB::bind_method(D_METHOD("get_peer_map"), &SteamMultiplayerPeer::get_peer_map);

	ClassDB::bind_method(D_METHOD("send_direct_message","message"), &SteamMultiplayerPeer::send_direct_message);
	ClassDB::bind_method(D_METHOD("get_direct_messages"), &SteamMultiplayerPeer::get_direct_messages);
	ClassDB::bind_method(D_METHOD("get_lobby_data","key"), &SteamMultiplayerPeer::get_lobby_data);
	ClassDB::bind_method(D_METHOD("set_lobby_data","key","value"), &SteamMultiplayerPeer::set_lobby_data);
	ClassDB::bind_method(D_METHOD("get_all_lobby_data"), &SteamMultiplayerPeer::get_all_lobby_data);

	ClassDB::bind_method(D_METHOD("set_lobby_joinable","joinable"), &SteamMultiplayerPeer::set_lobby_joinable);


	BIND_ENUM_CONSTANT(LOBBY_TYPE_PRIVATE);
	BIND_ENUM_CONSTANT(LOBBY_TYPE_FRIENDS_ONLY);
	BIND_ENUM_CONSTANT(LOBBY_TYPE_PUBLIC);
	BIND_ENUM_CONSTANT(LOBBY_TYPE_INVISIBLE);
	// BIND_ENUM_CONSTANT(PRIVATE_UNIQUE);

	BIND_ENUM_CONSTANT(CHAT_CHANGE_ENTERED);
	BIND_ENUM_CONSTANT(CHAT_CHANGE_LEFT);
	BIND_ENUM_CONSTANT(CHAT_CHANGE_DISCONNECTED);
	BIND_ENUM_CONSTANT(CHAT_CHANGE_KICKED);
	BIND_ENUM_CONSTANT(CHAT_CHANGE_BANNED);

	BIND_ENUM_CONSTANT(LOBBY_STATE_NOT_CONNECTED);
	BIND_ENUM_CONSTANT(LOBBY_STATE_HOST_PENDING);
	BIND_ENUM_CONSTANT(LOBBY_STATE_HOSTING);
	BIND_ENUM_CONSTANT(LOBBY_STATE_CLIENT_PENDING);
	BIND_ENUM_CONSTANT(LOBBY_STATE_CLIENT);

	// MATCHMAKING SIGNALS //////////////////////
	// todo these
	ADD_SIGNAL(MethodInfo("favorites_list_accounts_updated", PropertyInfo(Variant::INT, "result")));
	ADD_SIGNAL(MethodInfo("favorites_list_changed", PropertyInfo(Variant::DICTIONARY, "favorite")));
	ADD_SIGNAL(MethodInfo("lobby_message", PropertyInfo(Variant::INT, "lobby_id"), PropertyInfo(Variant::INT, "user"), PropertyInfo(Variant::STRING, "message"), PropertyInfo(Variant::INT, "chat_type")));
	ADD_SIGNAL(MethodInfo("lobby_chat_update", PropertyInfo(Variant::INT, "lobby_id"), PropertyInfo(Variant::INT, "changed_id"), PropertyInfo(Variant::INT, "making_change_id"), PropertyInfo(Variant::INT, "chat_state")));
	ADD_SIGNAL(MethodInfo("lobby_created", PropertyInfo(Variant::INT, "connect"), PropertyInfo(Variant::INT, "lobby_id")));
	ADD_SIGNAL(MethodInfo("lobby_data_update"));
	ADD_SIGNAL(MethodInfo("lobby_joined", PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "permissions"), PropertyInfo(Variant::BOOL, "locked"), PropertyInfo(Variant::INT, "response")));
	ADD_SIGNAL(MethodInfo("lobby_game_created", PropertyInfo(Variant::INT, "lobby_id"), PropertyInfo(Variant::INT, "server_id"), PropertyInfo(Variant::STRING, "server_ip"), PropertyInfo(Variant::INT, "port")));
	ADD_SIGNAL(MethodInfo("lobby_invite", PropertyInfo(Variant::INT, "inviter"), PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "game")));
	ADD_SIGNAL(MethodInfo("lobby_match_list", PropertyInfo(Variant::ARRAY, "lobbies")));
	ADD_SIGNAL(MethodInfo("lobby_kicked", PropertyInfo(Variant::INT, "lobby_id"), PropertyInfo(Variant::INT, "admin_id"), PropertyInfo(Variant::INT, "due_to_disconnect")));
	ADD_SIGNAL(MethodInfo("network_session_failed", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "reason"), PropertyInfo(Variant::INT, "connection_state")));
	// debug
	ADD_SIGNAL(MethodInfo("debug_data", PropertyInfo(Variant::DICTIONARY, "data")));
}

int SteamMultiplayerPeer::get_available_packet_count() const {
	return incoming_packets.size();
}

Error SteamMultiplayerPeer::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

	delete next_received_packet;
	next_received_packet = incoming_packets.front()->get();
	r_buffer_size = next_received_packet->size;
	*r_buffer = (const uint8_t *)(&next_received_packet->data);
	incoming_packets.pop_front();
	return OK;
}

int SteamMultiplayerPeer::_get_steam_transfer_flag() {
	auto transferMode = get_transfer_mode();

	auto flags = (k_nSteamNetworkingSend_NoNagle * no_nagle) |
			(k_nSteamNetworkingSend_NoDelay * no_delay);

	switch (transferMode) {
		case TransferMode::TRANSFER_MODE_RELIABLE:
			return k_nSteamNetworkingSend_Reliable | flags;
			break;
		case TransferMode::TRANSFER_MODE_UNRELIABLE:
			return k_nSteamNetworkingSend_Unreliable | flags;
			break;
		case TransferMode::TRANSFER_MODE_UNRELIABLE_ORDERED:
			//UNRELIABLE ORDERED NOT SUPPORTED! SENDING AS RELIABLE!
			//maybe I should find a way do this properly? number unreliable packets, discarding out of order ones.
			return k_nSteamNetworkingSend_Reliable | flags;
			break;
	}
	ERR_FAIL_V_MSG(-1, "flags error. not sure what happened!?");
}

Error SteamMultiplayerPeer::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	int transferMode = _get_steam_transfer_flag();
	auto channel = get_transfer_channel() + CHANNEL_MANAGEMENT::SIZE;

	if (target_peer == 0) { // send to ALL
		auto returnValue = OK;
		for (auto E = connections_by_steamId64.begin(); E; ++E) {
			auto packet = new Packet(p_buffer, p_buffer_size, transferMode, channel);
			auto errorCode = E->value->send(packet);
			if (errorCode != OK) {
				DEBUG_DATA_SIGNAL_V("put_packet failed!", errorCode);
				returnValue = errorCode;
			}
		}
		return returnValue;
	} else {
		auto packet = new Packet(p_buffer, p_buffer_size, transferMode, channel);
		return get_connection_by_peer(target_peer)->send(packet);
	}
}

int SteamMultiplayerPeer::get_max_packet_size() const {
	return k_cbMaxSteamNetworkingSocketsMessageSizeSend;
}

bool SteamMultiplayerPeer::is_server_relay_supported() const {
	return as_relay;
}

void SteamMultiplayerPeer::set_target_peer(int p_peer_id) {
	target_peer = p_peer_id;
};

int SteamMultiplayerPeer::get_packet_peer() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, 1, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, 1, "No packets to get!");

	return connections_by_steamId64[incoming_packets.front()->get()->sender.ConvertToUint64()]->peer_id;
}

SteamMultiplayerPeer::TransferMode SteamMultiplayerPeer::get_packet_mode() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, TRANSFER_MODE_RELIABLE, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE, "No pending packets, cannot get transfer mode!");

	if (incoming_packets.front()->get()->transfer_mode & k_nSteamNetworkingSend_Reliable) {
		return TRANSFER_MODE_RELIABLE;
	} else {
		return TRANSFER_MODE_UNRELIABLE;
	}
}

int SteamMultiplayerPeer::get_packet_channel() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, TRANSFER_MODE_RELIABLE, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE, "No pending packets, cannot get channel!");

	return incoming_packets.front()->get()->channel;
}

void SteamMultiplayerPeer::disconnect_peer(int p_peer, bool p_force) {
	ERR_FAIL_MSG("ERROR:: SteamMultiplayerPeer::disconnect_peer not yet implemented");
	// SteamMatchmaking()
}

bool SteamMultiplayerPeer::is_server() const {
	return unique_id == 1;
}

#define MAX_MESSAGE_COUNT 255
void SteamMultiplayerPeer::poll() {
	{
		SteamNetworkingMessage_t *messages[MAX_MESSAGE_COUNT];
		int count = SteamNetworkingMessages()->ReceiveMessagesOnChannel(CHANNEL_MANAGEMENT::SIZE, messages, MAX_MESSAGE_COUNT);
		for (int i = 0; i < count; i++) {
			auto msg = messages[i];
			process_message(msg);
			msg->Release();
		}
	}
	{
		auto a = PingPayload();
		for (auto E = connections_by_steamId64.begin(); E; ++E) {
			// Commented out auto key to appease the compiler gods
//			auto key = E->key; // this is unused?
			Ref<SteamMultiplayerPeer::ConnectionData> value = E->value;
			auto t = value->last_msg_timestamp + MAX_TIME_WITHOUT_MESSAGE; // pretty sure this will wrap. Should I fix this?

			if (value->peer_id == -1 || t < OS::get_singleton()->get_ticks_msec()) {
				value->ping(a);
			}
		}
	}
	{
		SteamNetworkingMessage_t *messages[MAX_MESSAGE_COUNT];
		int count = SteamNetworkingMessages()->ReceiveMessagesOnChannel(CHANNEL_MANAGEMENT::PING_CHANNEL, messages, MAX_MESSAGE_COUNT);
		for (int i = 0; i < count; i++) {
			auto msg = messages[i];
			process_ping(msg);
			msg->Release();
		}
	}
}

void SteamMultiplayerPeer::process_message(const SteamNetworkingMessage_t *msg) {
	ERR_FAIL_COND_MSG(msg->GetSize() > MAX_STEAM_PACKET_SIZE, "PACKET TOO LARGE!");

	auto packet = new Packet;
	packet->channel = 0;
	packet->sender = msg->m_identityPeer.GetSteamID();
	packet->size = msg->GetSize();
	packet->transfer_mode = -1;
	auto rawData = (uint8_t *)msg->GetData();
	memcpy(packet->data, rawData, packet->size);
	incoming_packets.push_back(packet);
}
void SteamMultiplayerPeer::process_ping(const SteamNetworkingMessage_t *msg) {
	if (sizeof(PingPayload) != msg->GetSize()) {
		print_error("wrong size of payload");
		return;
	}
	auto data = (PingPayload *)msg->GetData();
	if (data->peer_id == -1) {
		// respond to ping
		auto p = PingPayload();
		p.peer_id = unique_id;
		p.steam_id = SteamUser()->GetSteamID();
		auto err = connections_by_steamId64[msg->m_identityPeer.GetSteamID64()]->ping(p);
		if (err != OK) {
			DEBUG_DATA_SIGNAL_V("process_ping: ping failed?", err);
		}
	} else {
		auto connection = connections_by_steamId64[data->steam_id.ConvertToUint64()];
		if (connection->peer_id == -1) {
			set_steam_id_peer(data->steam_id, data->peer_id);
		}
		// collect ping data
	}
}

void SteamMultiplayerPeer::close() {
	ERR_FAIL_COND_MSG(lobby_id == CSteamID(), "CAN'T LEAVE A LOBBY IF YOUR'E NOT IN ONE!");
	SteamMatchmaking()->LeaveLobby(lobby_id);
	lobby_state = LOBBY_STATE::LOBBY_STATE_NOT_CONNECTED;
}

int SteamMultiplayerPeer::get_unique_id() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, 0, "The multiplayer instance isn't currently active.");
	return unique_id;
}

SteamMultiplayerPeer::ConnectionStatus SteamMultiplayerPeer::get_connection_status() const {
	if (lobby_state == LOBBY_STATE::LOBBY_STATE_NOT_CONNECTED) {
		return ConnectionStatus::CONNECTION_DISCONNECTED;
	} else if (lobby_state == LOBBY_STATE::LOBBY_STATE_CLIENT || lobby_state == LOBBY_STATE::LOBBY_STATE_HOSTING) {
		return ConnectionStatus::CONNECTION_CONNECTED;
	} else {
		return ConnectionStatus::CONNECTION_CONNECTING;
	}
}

int SteamMultiplayerPeer::get_peer_by_steam_id(CSteamID steamId) {
	ERR_FAIL_COND_V_MSG(connections_by_steamId64.has(steamId.ConvertToUint64()) == false, -1, "STEAMID NOT CONNECTED!");
	return connections_by_steamId64[steamId.ConvertToUint64()]->peer_id;
}

CSteamID SteamMultiplayerPeer::get_steam_id_by_peer(int peer) {
	ERR_FAIL_COND_V_MSG(peerId_to_steamId.has(peer) == false, CSteamID(), "PEER DOES NOT EXIST!");
	return peerId_to_steamId[peer]->steam_id;
}

void SteamMultiplayerPeer::set_steam_id_peer(CSteamID steamId, int peer_id) {
	ERR_FAIL_COND_MSG(connections_by_steamId64.has(steamId.ConvertToUint64()) == false, "STEAMID MISSING!");
	auto con = connections_by_steamId64[steamId.ConvertToUint64()];
	if (con->peer_id == -1) {
		con->peer_id = peer_id;
		peerId_to_steamId[peer_id] = con;
		emit_signal("peer_connected", peer_id);
	} else if (con->peer_id == peer_id) {
		//nothing happens, set peer that already exists
	} else {
		DEBUG_DATA_SIGNAL_V("THIS STEAM ID GOT WRONG PEER ID", (uint64_t)steamId.ConvertToUint64());
		DEBUG_DATA_SIGNAL_V("PEER ID WAS", con->peer_id);
		DEBUG_DATA_SIGNAL_V("TRYING TO SET AS", peer_id);
	}
}

Ref<SteamMultiplayerPeer::ConnectionData> SteamMultiplayerPeer::get_connection_by_peer(int peer_id) {
	if (peerId_to_steamId.has(peer_id)) {
		return peerId_to_steamId[peer_id];
	}
	return nullptr;
}

void SteamMultiplayerPeer::add_connection_peer(const CSteamID &steamId, int peer_id) {
	ERR_FAIL_COND_MSG(steamId == SteamUser()->GetSteamID(), "YOU CANNOT ADD A PEER THAT IS YOU!");

	Ref<ConnectionData> connectionData = Ref<ConnectionData>(memnew(ConnectionData(steamId)));
	connections_by_steamId64[steamId.ConvertToUint64()] = connectionData;
	auto this_ping = connectionData->ping();
	if (this_ping != OK) {
		DEBUG_DATA_SIGNAL_V("add_connection_peer: Error sending ping", this_ping);
	}
	ERR_FAIL_COND_MSG(this_ping != OK, "Message failed to join?");
}

void SteamMultiplayerPeer::add_pending_peer(const CSteamID &steamId) {
	add_connection_peer(steamId, -1);
}

void SteamMultiplayerPeer::removed_connection_peer(const CSteamID &steamId) {
	int peerId = get_peer_by_steam_id(steamId);
	peerId_to_steamId.erase(peerId);

	emit_signal("peer_disconnected", peerId);
	connections_by_steamId64.erase(steamId.ConvertToUint64());
}

Error SteamMultiplayerPeer::create_lobby(LOBBY_TYPE lobby_type, int max_players) {
	ERR_FAIL_COND_V_MSG(SteamMatchmaking() == NULL, ERR_DOES_NOT_EXIST, "`SteamMatchmaking()` is null.");
	ERR_FAIL_COND_V_MSG(lobby_state != LOBBY_STATE::LOBBY_STATE_NOT_CONNECTED, ERR_ALREADY_IN_USE, "CANNOT CREATE A LOBBY WHILE IN A LOBBY!");

	SteamAPICall_t api_call = SteamMatchmaking()->CreateLobby((ELobbyType)lobby_type, max_players);
	callResultCreateLobby.Set(api_call, this, &SteamMultiplayerPeer::lobby_created_scb);
	unique_id = 1;
	lobby_state = LOBBY_STATE::LOBBY_STATE_HOST_PENDING;
	return OK;
}

void SteamMultiplayerPeer::lobby_created_scb(LobbyCreated_t *lobby_data, bool io_failure) {
	if (io_failure) {
		lobby_state = LOBBY_STATE::LOBBY_STATE_NOT_CONNECTED;
		ERR_FAIL_MSG("lobby_created failed? idk wtf is happening");
		// steamworksError("lobby_created");
	} else {
		lobby_state = LOBBY_STATE::LOBBY_STATE_HOSTING;
		int connect = lobby_data->m_eResult;
		lobby_id = lobby_data->m_ulSteamIDLobby;
		uint64_t lobby = lobby_id.ConvertToUint64();
		emit_signal(SNAME("lobby_created"), connect, lobby); // why do I do this? edit: no really, why am I doing this? // why are you doing this? :)
	}
}

Error SteamMultiplayerPeer::join_lobby(uint64_t lobbyId) {
	ERR_FAIL_COND_V_MSG(SteamMatchmaking() == NULL, ERR_DOES_NOT_EXIST, "`SteamMatchmaking()` is null.");
	ERR_FAIL_COND_V_MSG(lobby_state != LOBBY_STATE::LOBBY_STATE_NOT_CONNECTED, ERR_ALREADY_IN_USE, "CANNOT JOIN A LOBBY WHILE IN A LOBBY!");

	if (SteamMatchmaking() != NULL) {
		lobby_state = LOBBY_STATE::LOBBY_STATE_CLIENT_PENDING;
		this->lobby_id.SetFromUint64(lobbyId);
		// unique_id = SteamUser()->GetSteamID().GetAccountID();
		unique_id = generate_unique_id();
		SteamMatchmaking()->JoinLobby(this->lobby_id);
	}
	return OK;
}

void SteamMultiplayerPeer::lobby_message_scb(LobbyChatMsg_t *call_data) {
	if (lobby_id != call_data->m_ulSteamIDLobby) {
		DEBUG_DATA_SIGNAL("lobby_message_scb: recived message on that isn't for this lobby?");
		return;
	}
	Packet *packet = new Packet;

	packet->sender = call_data->m_ulSteamIDUser;
	if (SteamUser()->GetSteamID() == packet->sender) {
		return;
	}
	uint8 chat_type = call_data->m_eChatEntryType;
	// Convert the chat type over
	EChatEntryType type = (EChatEntryType)chat_type;
	// Get the chat message data
	packet->size = SteamMatchmaking()->GetLobbyChatEntry(lobby_id, call_data->m_iChatID, &(packet->sender), &(packet->data), MAX_STEAM_PACKET_SIZE, &type);
	packet->channel = -1;

	incoming_packets.push_back(packet);
};

//this should happen when someone joins or leaves
void SteamMultiplayerPeer::lobby_chat_update_scb(LobbyChatUpdate_t *call_data) {
	if (lobby_id != call_data->m_ulSteamIDLobby) {
		return;
	}
	CSteamID userChanged = CSteamID(call_data->m_ulSteamIDUserChanged);
	switch (CHAT_CHANGE(call_data->m_rgfChatMemberStateChange)) {
		case CHAT_CHANGE::CHAT_CHANGE_ENTERED:
			if (userChanged != SteamUser()->GetSteamID()) {
				add_pending_peer(userChanged);
			}
			break;
		case CHAT_CHANGE::CHAT_CHANGE_LEFT:
		case CHAT_CHANGE::CHAT_CHANGE_DISCONNECTED:
		case CHAT_CHANGE::CHAT_CHANGE_KICKED:
		case CHAT_CHANGE::CHAT_CHANGE_BANNED:
			if (userChanged != SteamUser()->GetSteamID()) {
				removed_connection_peer(userChanged);
			}
			// todo emit signal based on what happened to that user!
			break;
		default:
			ERR_PRINT("WTF is this lobby chat update status?!");
	}
};

//this happens when you recive a message request from someone.
void SteamMultiplayerPeer::network_messages_session_request_scb(SteamNetworkingMessagesSessionRequest_t *t) {
	DEBUG_CON_DATA_SIGNAL(get_state() != LOBBY_STATE_HOSTING && get_state() != LOBBY_STATE_CLIENT, "RECIVING A CONNECTION BEFORE YOU'RE PROPERLY IN A LOBBY");
	// search for lobby member
	CSteamID requester = t->m_identityRemote.GetSteamID();
	int currentLobbySize = SteamMatchmaking()->GetNumLobbyMembers(lobby_id);
	for (int i = 0; i < currentLobbySize; i++) {
		if (SteamMatchmaking()->GetLobbyMemberByIndex(lobby_id, i) == requester) {
			bool didWork = SteamNetworkingMessages()->AcceptSessionWithUser(t->m_identityRemote);
			ERR_FAIL_COND_MSG(didWork, "Message failed to join?");
			return;
		}
	}
	ERR_PRINT(String("CONNECTION ATTEMPTED BY PLAYER NOT IN LOBBY!?:") + String::num_uint64(requester.GetAccountID()));
};

void SteamMultiplayerPeer::network_messages_session_failed_scb(SteamNetworkingMessagesSessionFailed_t *call_data) {
	SteamNetConnectionInfo_t info = call_data->m_info;
	/// Indicates who this session was with. Depending on the connection type and phase of the connection, we might not know.
	uint64_t other_end_steamid = info.m_identityRemote.GetSteamID64();
	ERR_PRINT(String("Connection terminated. See ESteamNetConnectionEnd for reason #") + String::num(info.m_eEndReason)
	+ String(". and ESteamNetworkingConnectionState for connection state #") + String::num(info.m_eState));
	emit_signal("network_session_failed", other_end_steamid, info.m_eEndReason, info.m_eState);
	/// Send a human-readable explanation of the problem to debug signal.
	DEBUG_DATA_SIGNAL_V("network_messages_session_failed_scb",
	String::utf8(info.m_szEndDebug) + String("; ") + String::utf8(info.m_szConnectionDescription));
}

void SteamMultiplayerPeer::lobby_data_update_scb(LobbyDataUpdate_t *call_data) {
	// These were meant to be sent back with a signal that was commented out
	// Commenting these out to appease the compiler gods as one shadows a member of the class
//	uint64_t member_id = call_data->m_ulSteamIDMember;
//	uint64_t lobby_id = call_data->m_ulSteamIDLobby;
//	uint8 success = call_data->m_bSuccess;
	return;
	// emit_signal("lobby_data_update", success, lobby_id, member_id);
}

//called when a player joins a lobby. Including when the host creates and joins
void SteamMultiplayerPeer::lobby_joined_scb(LobbyEnter_t *lobbyData) {
	if( lobbyData->m_ulSteamIDLobby != this->lobby_id.ConvertToUint64() ){
		//joined a lobby that isn't this lobby
		//it's not unusual to be in more than one lobby
		return;
	}

	if (lobbyData->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess) {
		auto sm = SteamMatchmaking();
		lobby_owner = sm->GetLobbyOwner(lobby_id);
		if (unique_id == 1) {
			// don't do stuff if you're already the host
		} else {
			lobby_state = LOBBY_STATE::LOBBY_STATE_CLIENT;
			add_pending_peer(lobby_owner); //frist add the lobby owner

			int count = sm->GetNumLobbyMembers(lobby_id);
			for (int i = 0; i < count; i++) {
				CSteamID member = sm->GetLobbyMemberByIndex(lobby_id, i);
				if (member != SteamUser()->GetSteamID() && member != lobby_owner) { // lobby owner was added above. should happen FIRST
					add_pending_peer(member);
				}
			}
		}
	} else {
		String output = "";
		switch (lobbyData->m_EChatRoomEnterResponse) {
			// k_EChatRoomEnterResponseSuccess: 			output = "k_EChatRoomEnterResponseSuccess"; break;
			case k_EChatRoomEnterResponseDoesntExist:
				output = "Doesn't Exist";
				break;
			case k_EChatRoomEnterResponseNotAllowed:
				output = "Not Allowed";
				break;
			case k_EChatRoomEnterResponseFull:
				output = "Full";
				break;
			case k_EChatRoomEnterResponseError:
				output = "Error";
				break;
			case k_EChatRoomEnterResponseBanned:
				output = "Banned";
				break;
			case k_EChatRoomEnterResponseLimited:
				output = "Limited";
				break;
			case k_EChatRoomEnterResponseClanDisabled:
				output = "Clan Disabled";
				break;
			case k_EChatRoomEnterResponseCommunityBan:
				output = "Community Ban";
				break;
			case k_EChatRoomEnterResponseMemberBlockedYou:
				output = "Member Blocked You";
				break;
			case k_EChatRoomEnterResponseYouBlockedMember:
				output = "You Blocked Member";
				break;
			case k_EChatRoomEnterResponseRatelimitExceeded:
				output = "Ratelimit Exceeded";
				break;
		};
		if (output.length() != 0) {
			ERR_PRINT("Joined lobby failed!" + output);
			lobby_state = LOBBY_STATE::LOBBY_STATE_NOT_CONNECTED;
			DEBUG_DATA_SIGNAL_V(output, lobbyData->m_EChatRoomEnterResponse);
			return;
		}
	}
}

Dictionary steamIdToDict(CSteamID input) {
	auto output = Dictionary();
	output["GetAccountID"] = input.GetAccountID();
	output["GetUnAccountInstance"] = input.GetUnAccountInstance();
	output["GetEAccountType"] = input.GetEAccountType();
	output["GetEUniverse"] = input.GetEUniverse();
	return output;
}

String SteamMultiplayerPeer::convertEResultToString(EResult e) {
	switch (e) {
		case k_EResultNone:
			return String("k_EResultNone");
		case k_EResultOK:
			return String("k_EResultOK");
		case k_EResultFail:
			return String("k_EResultFail");
		case k_EResultNoConnection:
			return String("k_EResultNoConnection");
		case k_EResultInvalidPassword:
			return String("k_EResultInvalidPassword");
		case k_EResultLoggedInElsewhere:
			return String("k_EResultLoggedInElsewhere");
		case k_EResultInvalidProtocolVer:
			return String("k_EResultInvalidProtocolVer");
		case k_EResultInvalidParam:
			return String("k_EResultInvalidParam");
		case k_EResultFileNotFound:
			return String("k_EResultFileNotFound");
		case k_EResultBusy:
			return String("k_EResultBusy");
		case k_EResultInvalidState:
			return String("k_EResultInvalidState");
		case k_EResultInvalidName:
			return String("k_EResultInvalidName");
		case k_EResultInvalidEmail:
			return String("k_EResultInvalidEmail");
		case k_EResultDuplicateName:
			return String("k_EResultDuplicateName");
		case k_EResultAccessDenied:
			return String("k_EResultAccessDenied");
		case k_EResultTimeout:
			return String("k_EResultTimeout");
		case k_EResultBanned:
			return String("k_EResultBanned");
		case k_EResultAccountNotFound:
			return String("k_EResultAccountNotFound");
		case k_EResultInvalidSteamID:
			return String("k_EResultInvalidSteamID");
		case k_EResultServiceUnavailable:
			return String("k_EResultServiceUnavailable");
		case k_EResultNotLoggedOn:
			return String("k_EResultNotLoggedOn");
		case k_EResultPending:
			return String("k_EResultPending");
		case k_EResultEncryptionFailure:
			return String("k_EResultEncryptionFailure");
		case k_EResultInsufficientPrivilege:
			return String("k_EResultInsufficientPrivilege");
		case k_EResultLimitExceeded:
			return String("k_EResultLimitExceeded");
		case k_EResultRevoked:
			return String("k_EResultRevoked");
		case k_EResultExpired:
			return String("k_EResultExpired");
		case k_EResultAlreadyRedeemed:
			return String("k_EResultAlreadyRedeemed");
		case k_EResultDuplicateRequest:
			return String("k_EResultDuplicateRequest");
		case k_EResultAlreadyOwned:
			return String("k_EResultAlreadyOwned");
		case k_EResultIPNotFound:
			return String("k_EResultIPNotFound");
		case k_EResultPersistFailed:
			return String("k_EResultPersistFailed");
		case k_EResultLockingFailed:
			return String("k_EResultLockingFailed");
		case k_EResultLogonSessionReplaced:
			return String("k_EResultLogonSessionReplaced");
		case k_EResultConnectFailed:
			return String("k_EResultConnectFailed");
		case k_EResultHandshakeFailed:
			return String("k_EResultHandshakeFailed");
		case k_EResultIOFailure:
			return String("k_EResultIOFailure");
		case k_EResultRemoteDisconnect:
			return String("k_EResultRemoteDisconnect");
		case k_EResultShoppingCartNotFound:
			return String("k_EResultShoppingCartNotFound");
		case k_EResultBlocked:
			return String("k_EResultBlocked");
		case k_EResultIgnored:
			return String("k_EResultIgnored");
		case k_EResultNoMatch:
			return String("k_EResultNoMatch");
		case k_EResultAccountDisabled:
			return String("k_EResultAccountDisabled");
		case k_EResultServiceReadOnly:
			return String("k_EResultServiceReadOnly");
		case k_EResultAccountNotFeatured:
			return String("k_EResultAccountNotFeatured");
		case k_EResultAdministratorOK:
			return String("k_EResultAdministratorOK");
		case k_EResultContentVersion:
			return String("k_EResultContentVersion");
		case k_EResultTryAnotherCM:
			return String("k_EResultTryAnotherCM");
		case k_EResultPasswordRequiredToKickSession:
			return String("k_EResultPasswordRequiredToKickSession");
		case k_EResultAlreadyLoggedInElsewhere:
			return String("k_EResultAlreadyLoggedInElsewhere");
		case k_EResultSuspended:
			return String("k_EResultSuspended");
		case k_EResultCancelled:
			return String("k_EResultCancelled");
		case k_EResultDataCorruption:
			return String("k_EResultDataCorruption");
		case k_EResultDiskFull:
			return String("k_EResultDiskFull");
		case k_EResultRemoteCallFailed:
			return String("k_EResultRemoteCallFailed");
		case k_EResultPasswordUnset:
			return String("k_EResultPasswordUnset");
		case k_EResultExternalAccountUnlinked:
			return String("k_EResultExternalAccountUnlinked");
		case k_EResultPSNTicketInvalid:
			return String("k_EResultPSNTicketInvalid");
		case k_EResultExternalAccountAlreadyLinked:
			return String("k_EResultExternalAccountAlreadyLinked");
		case k_EResultRemoteFileConflict:
			return String("k_EResultRemoteFileConflict");
		case k_EResultIllegalPassword:
			return String("k_EResultIllegalPassword");
		case k_EResultSameAsPreviousValue:
			return String("k_EResultSameAsPreviousValue");
		case k_EResultAccountLogonDenied:
			return String("k_EResultAccountLogonDenied");
		case k_EResultCannotUseOldPassword:
			return String("k_EResultCannotUseOldPassword");
		case k_EResultInvalidLoginAuthCode:
			return String("k_EResultInvalidLoginAuthCode");
		case k_EResultAccountLogonDeniedNoMail:
			return String("k_EResultAccountLogonDeniedNoMail");
		case k_EResultHardwareNotCapableOfIPT:
			return String("k_EResultHardwareNotCapableOfIPT");
		case k_EResultIPTInitError:
			return String("k_EResultIPTInitError");
		case k_EResultParentalControlRestricted:
			return String("k_EResultParentalControlRestricted");
		case k_EResultFacebookQueryError:
			return String("k_EResultFacebookQueryError");
		case k_EResultExpiredLoginAuthCode:
			return String("k_EResultExpiredLoginAuthCode");
		case k_EResultIPLoginRestrictionFailed:
			return String("k_EResultIPLoginRestrictionFailed");
		case k_EResultAccountLockedDown:
			return String("k_EResultAccountLockedDown");
		case k_EResultAccountLogonDeniedVerifiedEmailRequired:
			return String("k_EResultAccountLogonDeniedVerifiedEmailRequired");
		case k_EResultNoMatchingURL:
			return String("k_EResultNoMatchingURL");
		case k_EResultBadResponse:
			return String("k_EResultBadResponse");
		case k_EResultRequirePasswordReEntry:
			return String("k_EResultRequirePasswordReEntry");
		case k_EResultValueOutOfRange:
			return String("k_EResultValueOutOfRange");
		case k_EResultUnexpectedError:
			return String("k_EResultUnexpectedError");
		case k_EResultDisabled:
			return String("k_EResultDisabled");
		case k_EResultInvalidCEGSubmission:
			return String("k_EResultInvalidCEGSubmission");
		case k_EResultRestrictedDevice:
			return String("k_EResultRestrictedDevice");
		case k_EResultRegionLocked:
			return String("k_EResultRegionLocked");
		case k_EResultRateLimitExceeded:
			return String("k_EResultRateLimitExceeded");
		case k_EResultAccountLoginDeniedNeedTwoFactor:
			return String("k_EResultAccountLoginDeniedNeedTwoFactor");
		case k_EResultItemDeleted:
			return String("k_EResultItemDeleted");
		case k_EResultAccountLoginDeniedThrottle:
			return String("k_EResultAccountLoginDeniedThrottle");
		case k_EResultTwoFactorCodeMismatch:
			return String("k_EResultTwoFactorCodeMismatch");
		case k_EResultTwoFactorActivationCodeMismatch:
			return String("k_EResultTwoFactorActivationCodeMismatch");
		case k_EResultAccountAssociatedToMultiplePartners:
			return String("k_EResultAccountAssociatedToMultiplePartners");
		case k_EResultNotModified:
			return String("k_EResultNotModified");
		case k_EResultNoMobileDevice:
			return String("k_EResultNoMobileDevice");
		case k_EResultTimeNotSynced:
			return String("k_EResultTimeNotSynced");
		case k_EResultSmsCodeFailed:
			return String("k_EResultSmsCodeFailed");
		case k_EResultAccountLimitExceeded:
			return String("k_EResultAccountLimitExceeded");
		case k_EResultAccountActivityLimitExceeded:
			return String("k_EResultAccountActivityLimitExceeded");
		case k_EResultPhoneActivityLimitExceeded:
			return String("k_EResultPhoneActivityLimitExceeded");
		case k_EResultRefundToWallet:
			return String("k_EResultRefundToWallet");
		case k_EResultEmailSendFailure:
			return String("k_EResultEmailSendFailure");
		case k_EResultNotSettled:
			return String("k_EResultNotSettled");
		case k_EResultNeedCaptcha:
			return String("k_EResultNeedCaptcha");
		case k_EResultGSLTDenied:
			return String("k_EResultGSLTDenied");
		case k_EResultGSOwnerDenied:
			return String("k_EResultGSOwnerDenied");
		case k_EResultInvalidItemType:
			return String("k_EResultInvalidItemType");
		case k_EResultIPBanned:
			return String("k_EResultIPBanned");
		case k_EResultGSLTExpired:
			return String("k_EResultGSLTExpired");
		case k_EResultInsufficientFunds:
			return String("k_EResultInsufficientFunds");
		case k_EResultTooManyPending:
			return String("k_EResultTooManyPending");
		case k_EResultNoSiteLicensesFound:
			return String("k_EResultNoSiteLicensesFound");
		case k_EResultWGNetworkSendExceeded:
			return String("k_EResultWGNetworkSendExceeded");
		case k_EResultAccountNotFriends:
			return String("k_EResultAccountNotFriends");
		case k_EResultLimitedUserAccount:
			return String("k_EResultLimitedUserAccount");
		case k_EResultCantRemoveItem:
			return String("k_EResultCantRemoveItem");
		case k_EResultAccountDeleted:
			return String("k_EResultAccountDeleted");
		case k_EResultExistingUserCancelledLicense:
			return String("k_EResultExistingUserCancelledLicense");
		case k_EResultCommunityCooldown:
			return String("k_EResultCommunityCooldown");
		case k_EResultNoLauncherSpecified:
			return String("k_EResultNoLauncherSpecified");
		case k_EResultMustAgreeToSSA:
			return String("k_EResultMustAgreeToSSA");
		case k_EResultLauncherMigrated:
			return String("k_EResultLauncherMigrated");
		case k_EResultSteamRealmMismatch:
			return String("k_EResultSteamRealmMismatch");
		case k_EResultInvalidSignature:
			return String("k_EResultInvalidSignature");
		case k_EResultParseFailure:
			return String("k_EResultParseFailure");
		case k_EResultNoVerifiedPhone:
			return String("k_EResultNoVerifiedPhone");
		case k_EResultInsufficientBattery:
			return String("k_EResultInsufficientBattery");
		case k_EResultChargerRequired:
			return String("k_EResultChargerRequired");
		case k_EResultCachedCredentialInvalid:
			return String("k_EResultCachedCredentialInvalid");
		case K_EResultPhoneNumberIsVOIP:
			return String("K_EResultPhoneNumberIsVOIP");
	}
	return String("unmatched");
}

Dictionary SteamMultiplayerPeer::get_peer_info(int i) {
	return peerId_to_steamId[i]->collect_debug_data();
}

uint64_t SteamMultiplayerPeer::get_steam64_from_peer_id(int peer) {
	if (peer == this->unique_id) {
		return SteamUser()->GetSteamID().ConvertToUint64();
	} else if (peerId_to_steamId.has(peer)) {
		return peerId_to_steamId[peer]->steam_id.ConvertToUint64();
	} else {
		return -1;
	}
}

int SteamMultiplayerPeer::get_peer_id_from_steam64(uint64_t steamid) {
	if (steamid == SteamUser()->GetSteamID().ConvertToUint64()) {
		return this->unique_id;
	} else if (connections_by_steamId64.has(steamid)) {
		return connections_by_steamId64[steamid]->peer_id;
	} else {
		return -1;
	}
}

Dictionary SteamMultiplayerPeer::get_peer_map() {
	Dictionary output;
	for (auto E = connections_by_steamId64.begin(); E; ++E) {
		output[E->value->peer_id] = (uint64_t)E->value->steam_id.ConvertToUint64();
	}
	return output;
}
