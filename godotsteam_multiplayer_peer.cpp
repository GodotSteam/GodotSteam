///// HEADER INCLUDES
/////////////////////////////////////////////////
//
// Include GodotSteam header
#include "godotsteam_multiplayer_peer.h"

// Include Godot headers
#include "core/io/json.h"


///// DO IT
/////////////////////////////////////////////////
//
SteamMultiplayerPeer::SteamMultiplayerPeer() :
		callbackLobbyChatUpdate(this, &SteamMultiplayerPeer::lobby_chat_update),
		callbackLobbyDataUpdate(this, &SteamMultiplayerPeer::lobby_data_update),
		callbackLobbyJoined(this, &SteamMultiplayerPeer::lobby_joined),
		callbackLobbyMessage(this, &SteamMultiplayerPeer::lobby_message),
		callbackNetworkMessagesSessionFailed(this, &SteamMultiplayerPeer::network_messages_session_failed),
		callbackNetworkMessagesSessionRequest(this, &SteamMultiplayerPeer::network_messages_session_request)
		{
	if (SteamUser() != NULL) {
		steam_id = SteamUser()->GetSteamID();
	}
}


///// INTERNAL
/////////////////////////////////////////////////
//
void SteamMultiplayerPeer::add_connection_peer(const CSteamID &this_steam_id, int this_peer_id) {
	ERR_FAIL_COND_MSG(this_steam_id == SteamUser()->GetSteamID(), "Cannot add self as peer");

	Ref<ConnectionData> this_connection_data = Ref<ConnectionData>(memnew(ConnectionData(this_steam_id)));
	connections_by_steam_id64[this_steam_id.ConvertToUint64()] = this_connection_data;
	auto this_ping = this_connection_data->ping();
	if (this_ping != OK) {
		DEBUG_DATA_SIGNAL_V("add_connection_peer: Error sending ping", this_ping);
	}
	ERR_FAIL_COND_MSG(this_ping != OK, "Message failed to join?");
}

void SteamMultiplayerPeer::add_pending_peer(const CSteamID &this_steam_id) {
	add_connection_peer(this_steam_id, -1);
}

void SteamMultiplayerPeer::close() {
	ERR_FAIL_COND_MSG(lobby_id == CSteamID(), "Cannot leave lobby, not in a lobby");
	SteamMatchmaking()->LeaveLobby(lobby_id);
	lobby_state = LobbyState::LOBBY_STATE_NOT_CONNECTED;
}

void SteamMultiplayerPeer::disconnect_peer(int disconnecting_peer, bool use_force) {
	ERR_FAIL_MSG("ERROR:: SteamMultiplayerPeer::disconnect_peer not yet implemented");
	// This should be easy to implement
}

int SteamMultiplayerPeer::get_available_packet_count() const {
	return incoming_packets.size();
}

SteamMultiplayerPeer::ConnectionStatus SteamMultiplayerPeer::get_connection_status() const {
	if (lobby_state == LobbyState::LOBBY_STATE_NOT_CONNECTED) {
		return ConnectionStatus::CONNECTION_DISCONNECTED;
	}
	else if (lobby_state == LobbyState::LOBBY_STATE_CLIENT || lobby_state == LobbyState::LOBBY_STATE_HOSTING) {
		return ConnectionStatus::CONNECTION_CONNECTED;
	}
	else {
		return ConnectionStatus::CONNECTION_CONNECTING;
	}
}

int SteamMultiplayerPeer::get_max_packet_size() const {
	return k_cbMaxSteamNetworkingSocketsMessageSizeSend;
}

int SteamMultiplayerPeer::get_peer_by_steam_id(CSteamID this_steam_id) {
	ERR_FAIL_COND_V_MSG(connections_by_steam_id64.has(this_steam_id.ConvertToUint64()) == false, -1, "Steam ID not connected");
	return connections_by_steam_id64[this_steam_id.ConvertToUint64()]->peer_id;
}

Error SteamMultiplayerPeer::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	// Shouldn't this be a silent failure?
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, ERR_UNAVAILABLE, "No incoming packets available");

	delete next_received_packet;
	next_received_packet = incoming_packets.front()->get();
	r_buffer_size = next_received_packet->size;
	*r_buffer = (const uint8_t *)(&next_received_packet->data);
	incoming_packets.pop_front();
	return OK;
}

int SteamMultiplayerPeer::get_packet_channel() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, TRANSFER_MODE_RELIABLE, "Multiplayer instance is not currently active");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE, "No pending packets, cannot get channel");

	return incoming_packets.front()->get()->channel;
}

SteamMultiplayerPeer::TransferMode SteamMultiplayerPeer::get_packet_mode() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, TRANSFER_MODE_RELIABLE, "Multiplayer instance is not currently active");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE, "No pending packets, cannot get transfer mode");

	if (incoming_packets.front()->get()->transfer_mode & k_nSteamNetworkingSend_Reliable) {
		return TRANSFER_MODE_RELIABLE;
	} else {
		return TRANSFER_MODE_UNRELIABLE;
	}
}

int SteamMultiplayerPeer::get_packet_peer() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, 1, "Multiplayer instance is not currently active");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, 1, "No packets to get");

	return connections_by_steam_id64[incoming_packets.front()->get()->sender.ConvertToUint64()]->peer_id;
}

CSteamID SteamMultiplayerPeer::get_steam_id_by_peer(int peer) {
	ERR_FAIL_COND_V_MSG(peer_id_to_steam_id.has(peer) == false, CSteamID(), "Peer does not exist");
	return peer_id_to_steam_id[peer]->steam_id;
}

int SteamMultiplayerPeer::get_steam_transfer_flag() {
	auto this_transfer_mode = get_transfer_mode();

	auto flags = (k_nSteamNetworkingSend_NoNagle * no_nagle) |
			(k_nSteamNetworkingSend_NoDelay * no_delay);

	switch (this_transfer_mode) {
		case TransferMode::TRANSFER_MODE_RELIABLE:
			return k_nSteamNetworkingSend_Reliable | flags;
			break;
		case TransferMode::TRANSFER_MODE_UNRELIABLE:
			return k_nSteamNetworkingSend_Unreliable | flags;
			break;
		case TransferMode::TRANSFER_MODE_UNRELIABLE_ORDERED:
			// Steam has no equivalent of unreliable ordered so defaulting to reliable
			return k_nSteamNetworkingSend_Reliable | flags;
			break;
	}
	ERR_FAIL_V_MSG(-1, "Flags error");
}

int SteamMultiplayerPeer::get_unique_id() const {
	ERR_FAIL_COND_V_MSG(_is_active() == false, 0, "Multiplayer instance is not currently active");
	return unique_id;
}

bool SteamMultiplayerPeer::is_server() const {
	return unique_id == 1;
}

bool SteamMultiplayerPeer::is_server_relay_supported() const {
	return as_relay;
}

void SteamMultiplayerPeer::poll() {
	{
		SteamNetworkingMessage_t *messages[MAX_MESSAGE_COUNT];
		int count = SteamNetworkingMessages()->ReceiveMessagesOnChannel(ConnectionData::SIZE, messages, MAX_MESSAGE_COUNT);
		for (int i = 0; i < count; i++) {
			auto msg = messages[i];
			process_message(msg);
			msg->Release();
		}
	}
	{
		auto a = ConnectionData::PingPayload();
		for (auto E = connections_by_steam_id64.begin(); E; ++E) {
//			auto key = E->key; // This is unused?
			Ref<ConnectionData> value = E->value;
			auto t = value->last_msg_timestamp + MAX_TIME_WITHOUT_MESSAGE; // pretty sure this will wrap. Should I fix this? // Massively.

			if (value->peer_id == -1 || t < OS::get_singleton()->get_ticks_msec()) {
				value->ping(a);
			}
		}
	}
	{
		SteamNetworkingMessage_t *messages[MAX_MESSAGE_COUNT];
		int count = SteamNetworkingMessages()->ReceiveMessagesOnChannel(ConnectionData::PING_CHANNEL, messages, MAX_MESSAGE_COUNT);
		for (int i = 0; i < count; i++) {
			auto msg = messages[i];
			process_ping(msg);
			msg->Release();
		}
	}
}

void SteamMultiplayerPeer::process_message(const SteamNetworkingMessage_t *msg) {
	ERR_FAIL_COND_MSG(msg->GetSize() > k_cbMaxSteamNetworkingSocketsMessageSizeSend, "Packet too large, ignoring");

	auto packet = new ConnectionData::Packet;
	packet->channel = 0;
	packet->sender = msg->m_identityPeer.GetSteamID();
	packet->size = msg->GetSize();
	packet->transfer_mode = -1;
	auto rawData = (uint8_t *)msg->GetData();
	memcpy(packet->data, rawData, packet->size);
	incoming_packets.push_back(packet);
}

void SteamMultiplayerPeer::process_ping(const SteamNetworkingMessage_t *msg) {
	if (sizeof(ConnectionData::PingPayload) != msg->GetSize()) {
		print_error("Wrong size of payload");
		return;
	}
	auto data = (ConnectionData::PingPayload *)msg->GetData();
	if (data->peer_id == -1) {
		// respond to ping
		auto p = ConnectionData::PingPayload();
		p.peer_id = unique_id;
		p.steam_id = SteamUser()->GetSteamID();
		auto err = connections_by_steam_id64[msg->m_identityPeer.GetSteamID64()]->ping(p);
		if (err != OK) {
			DEBUG_DATA_SIGNAL_V("process_ping: ping failed?", err);
		}
	} else {
		auto connection = connections_by_steam_id64[data->steam_id.ConvertToUint64()];
		if (connection->peer_id == -1) {
			set_steam_id_peer(data->steam_id, data->peer_id);
		}
		// collect ping data
	}
}

Error SteamMultiplayerPeer::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	int this_transfer_mode = get_steam_transfer_flag();
	auto channel = get_transfer_channel() + ConnectionData::SIZE;

	if (target_peer == 0) {
		auto return_value = OK;
		for (auto E = connections_by_steam_id64.begin(); E; ++E) {
			auto packet = new ConnectionData::Packet(p_buffer, p_buffer_size, this_transfer_mode, channel);
			auto error_code = E->value->send(packet);
			if (error_code != OK) {
				DEBUG_DATA_SIGNAL_V("put_packet failed:", error_code);
				return_value = error_code;
			}
		}
		return return_value;
	}
	else {
		auto packet = new ConnectionData::Packet(p_buffer, p_buffer_size, this_transfer_mode, channel);
		return get_connection_by_peer(target_peer)->send(packet);
	}
}

void SteamMultiplayerPeer::removed_connection_peer(const CSteamID &this_steam_id) {
	int this_peer_id = get_peer_by_steam_id(this_steam_id);
	peer_id_to_steam_id.erase(this_peer_id);

	emit_signal("peer_disconnected", this_peer_id);
	connections_by_steam_id64.erase(this_steam_id.ConvertToUint64());
}

void SteamMultiplayerPeer::set_target_peer(int this_peer_id) {
	target_peer = this_peer_id;
};

void SteamMultiplayerPeer::set_steam_id_peer(CSteamID this_steam_id, int this_peer_id) {
	ERR_FAIL_COND_MSG(connections_by_steam_id64.has(this_steam_id.ConvertToUint64()) == false, "Steam ID missing");
	auto con = connections_by_steam_id64[this_steam_id.ConvertToUint64()];
	if (con->peer_id == -1) {
		con->peer_id = this_peer_id;
		peer_id_to_steam_id[this_peer_id] = con;
		emit_signal("peer_connected", this_peer_id);
	}
	else if (con->peer_id == this_peer_id) {
		WARN_PRINT("This peer already exists: " + this_peer_id);
	}
	else {
		DEBUG_DATA_SIGNAL_V("This Steam ID got wrong peer ID: ", (uint64_t)this_steam_id.ConvertToUint64());
		DEBUG_DATA_SIGNAL_V("Peer ID was: ", con->peer_id);
		DEBUG_DATA_SIGNAL_V("Trying to set as: ", this_peer_id);
	}
}

Ref<ConnectionData> SteamMultiplayerPeer::get_connection_by_peer(int this_peer_id) {
	if (peer_id_to_steam_id.has(this_peer_id)) {
		return peer_id_to_steam_id[this_peer_id];
	}
	return nullptr;
}

Dictionary SteamMultiplayerPeer::steam_id_to_dict(CSteamID input) {
	auto output = Dictionary();
	output["GetAccountID"] = input.GetAccountID();
	output["GetUnAccountInstance"] = input.GetUnAccountInstance();
	output["GetEAccountType"] = input.GetEAccountType();
	output["GetEUniverse"] = input.GetEUniverse();
	return output;
}


///// FUNCTIONS
/////////////////////////////////////////////////
//
Dictionary SteamMultiplayerPeer::collect_debug_data() {
	auto output = Dictionary();

	output["lobby_id"] = steam_id_to_dict(lobby_id);
	output["lobby_owner"] = steam_id_to_dict(lobby_owner);
	output["lobby_state"] = lobby_state;
	output["no_delay"] = no_delay;
	output["no_nagle"] = no_nagle;
	output["steam_id"] = steam_id_to_dict(SteamUser()->GetSteamID());
	output["target_peer"] = target_peer;
	output["unique_id"] = unique_id;

	Array net_connections;
	for (auto E = connections_by_steam_id64.begin(); E; ++E) {
		auto qwer = E->value->collect_debug_data();
		net_connections.push_back(qwer);
	}
	output["net_connections"] = net_connections;

	return output;
}

Error SteamMultiplayerPeer::create_lobby(LobbyType lobby_type, int max_players) {
	ERR_FAIL_COND_V_MSG(SteamMatchmaking() == NULL, ERR_DOES_NOT_EXIST, "`SteamMatchmaking()` is null.");
	ERR_FAIL_COND_V_MSG(lobby_state != LobbyState::LOBBY_STATE_NOT_CONNECTED, ERR_ALREADY_IN_USE, "Cannot create a lobby while in a lobby");

	SteamAPICall_t api_call = SteamMatchmaking()->CreateLobby((ELobbyType)lobby_type, max_players);
	callResultCreateLobby.Set(api_call, this, &SteamMultiplayerPeer::lobby_created);
	unique_id = 1;
	lobby_state = LobbyState::LOBBY_STATE_HOST_PENDING;
	return OK;
}

Dictionary SteamMultiplayerPeer::get_all_lobby_data() {
	Dictionary output;
	ERR_FAIL_COND_V_MSG(lobby_id.ConvertToUint64() == 0, output, "Cannot get lobby data, not in a lobby");
	auto c = SteamMatchmaking()->GetLobbyDataCount(lobby_id);
	for (int i = 0; i < c; i++) {
		char key[STEAM_BUFFER_SIZE];
		char value[STEAM_BUFFER_SIZE];
		SteamMatchmaking()->GetLobbyDataByIndex(lobby_id, i, key, STEAM_BUFFER_SIZE, value, STEAM_BUFFER_SIZE);
		output[key] = value;
	}
	return output;
}

Array SteamMultiplayerPeer::get_direct_messages() {
	Array output;
	return output;
}

String SteamMultiplayerPeer::get_lobby_data(String key) {
	ERR_FAIL_COND_V_MSG(lobby_id.ConvertToUint64() == 0, "null", "Cannot get lobby data, not in a lobby");
	return SteamMatchmaking()->GetLobbyData(lobby_id, (const char *)key.ptr());
}

uint64_t SteamMultiplayerPeer::get_lobby_id() {
	return lobby_id.ConvertToUint64();
}

Error SteamMultiplayerPeer::join_lobby(uint64_t joined_lobby_id) {
	ERR_FAIL_COND_V_MSG(SteamMatchmaking() == NULL, ERR_DOES_NOT_EXIST, "`SteamMatchmaking()` is null.");
	ERR_FAIL_COND_V_MSG(lobby_state != LobbyState::LOBBY_STATE_NOT_CONNECTED, ERR_ALREADY_IN_USE, "Cannot join a lobby while in a lobby");

	if (SteamMatchmaking() != NULL) {
		lobby_state = LobbyState::LOBBY_STATE_CLIENT_PENDING;
		this->lobby_id.SetFromUint64(joined_lobby_id);
		// unique_id = SteamUser()->GetSteamID().GetAccountID();
		unique_id = generate_unique_id();
		SteamMatchmaking()->JoinLobby(this->lobby_id);
	}
	return OK;
}

bool SteamMultiplayerPeer::send_direct_message(PackedByteArray message_data) {
	return SteamMatchmaking()->SendLobbyChatMsg(steam_id, (void *)message_data.ptr(), message_data.size());
}

bool SteamMultiplayerPeer::set_lobby_data(String key, String data) {
	ERR_FAIL_COND_V_MSG(lobby_id.ConvertToUint64() == 0, false, "Cannot set lobby data, not in a lobby");
	return SteamMatchmaking()->SetLobbyData(lobby_id, (const char *)key.ptr(), (const char *)data.ptr());
}

void SteamMultiplayerPeer::set_lobby_joinable(bool now_joinable){
	SteamMatchmaking()->SetLobbyJoinable(lobby_id, now_joinable);
}


///// PEER
/////////////////////////////////////////////////
//
int SteamMultiplayerPeer::get_peer_id_from_steam64(uint64_t find_steam_id) {
	if (find_steam_id == SteamUser()->GetSteamID().ConvertToUint64()) {
		return this->unique_id;
	}
	else if (connections_by_steam_id64.has(find_steam_id)) {
		return connections_by_steam_id64[find_steam_id]->peer_id;
	}
	else {
		return -1;
	}
}


Dictionary SteamMultiplayerPeer::get_peer_info(int this_peer_id) {
	return peer_id_to_steam_id[this_peer_id]->collect_debug_data();
}


Dictionary SteamMultiplayerPeer::get_peer_map() {
	Dictionary output;
	for (auto E = connections_by_steam_id64.begin(); E; ++E) {
		output[E->value->peer_id] = (uint64_t)E->value->steam_id.ConvertToUint64();
	}
	return output;
}


uint64_t SteamMultiplayerPeer::get_steam64_from_peer_id(int peer) {
	if (peer == this->unique_id) {
		return SteamUser()->GetSteamID().ConvertToUint64();
	}
	else if (peer_id_to_steam_id.has(peer)) {
		return peer_id_to_steam_id[peer]->steam_id.ConvertToUint64();
	}
	else {
		return -1;
	}
}


///// CALLBACKS / SIGNALS
/////////////////////////////////////////////////
//
void SteamMultiplayerPeer::lobby_chat_update(LobbyChatUpdate_t *call_data) {
	if (lobby_id != call_data->m_ulSteamIDLobby) {
		return;
	}
	CSteamID user_changed = CSteamID(call_data->m_ulSteamIDUserChanged);

	if (user_changed != SteamUser()->GetSteamID()) {
		if (call_data->m_rgfChatMemberStateChange == k_EChatMemberStateChangeEntered) {		
			add_pending_peer(user_changed);
		}
		else{
			removed_connection_peer(user_changed);
		}
	}
}

void SteamMultiplayerPeer::lobby_data_update(LobbyDataUpdate_t *call_data) {
	uint64_t member_id = call_data->m_ulSteamIDMember;
	uint64_t this_lobby_id = call_data->m_ulSteamIDLobby;
	uint8 success = call_data->m_bSuccess;
	emit_signal("lobby_data_update", success, this_lobby_id, member_id);
}

void SteamMultiplayerPeer::lobby_joined(LobbyEnter_t *lobbyData) {
	ERR_FAIL_COND_MSG(lobbyData->m_ulSteamIDLobby != this->lobby_id.ConvertToUint64(), "Joined a lobby that is not this lobby");

	if (lobbyData->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess) {
		lobby_owner = SteamMatchmaking()->GetLobbyOwner(lobby_id);
		if (unique_id == 1) {
			// don't do stuff if you're already the host
		}
		else {
			lobby_state = LobbyState::LOBBY_STATE_CLIENT;
			add_pending_peer(lobby_owner);

			int count = SteamMatchmaking()->GetNumLobbyMembers(lobby_id);
			for (int i = 0; i < count; i++) {
				CSteamID member = SteamMatchmaking()->GetLobbyMemberByIndex(lobby_id, i);
				if (member != SteamUser()->GetSteamID() && member != lobby_owner) {
					add_pending_peer(member);
				}
			}
		}
	}
	else {
		String output = "";
		switch (lobbyData->m_EChatRoomEnterResponse) {
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
				output = "Rate Limit Exceeded";
				break;
		};
		if (output.length() != 0) {
			ERR_PRINT("Failed to join lobby:" + output);
			lobby_state = LobbyState::LOBBY_STATE_NOT_CONNECTED;
			DEBUG_DATA_SIGNAL_V(output, lobbyData->m_EChatRoomEnterResponse);
			return;
		}
	}
}

void SteamMultiplayerPeer::lobby_message(LobbyChatMsg_t *call_data) {
	if (lobby_id != call_data->m_ulSteamIDLobby) {
		DEBUG_DATA_SIGNAL("lobby_message: received message for a diffrent lobby");
		return;
	}
	ConnectionData::Packet *packet = new ConnectionData::Packet;

	packet->sender = call_data->m_ulSteamIDUser;
	if (SteamUser()->GetSteamID() == packet->sender) {
		return;
	}

	uint8 chat_type = call_data->m_eChatEntryType;
	// Convert the chat type over
	EChatEntryType type = (EChatEntryType)chat_type;
	// Get the chat message data
	packet->size = SteamMatchmaking()->GetLobbyChatEntry(lobby_id, call_data->m_iChatID, &(packet->sender), &(packet->data), k_cbMaxSteamNetworkingSocketsMessageSizeSend, &type);
	packet->channel = -1;

	incoming_packets.push_back(packet);
}

void SteamMultiplayerPeer::network_messages_session_request(SteamNetworkingMessagesSessionRequest_t *t) {
	DEBUG_CON_DATA_SIGNAL(get_state() != LOBBY_STATE_HOSTING && get_state() != LOBBY_STATE_CLIENT, "Recieved a connection but not in a lobby");

	CSteamID requester = t->m_identityRemote.GetSteamID();
	int current_lobby_size = SteamMatchmaking()->GetNumLobbyMembers(lobby_id);
	for (int i = 0; i < current_lobby_size; i++) {
		if (SteamMatchmaking()->GetLobbyMemberByIndex(lobby_id, i) == requester) {
			bool accepted_session = SteamNetworkingMessages()->AcceptSessionWithUser(t->m_identityRemote);
			ERR_FAIL_COND_MSG(accepted_session, "Message failed to join?");
			return;
		}
	}
	ERR_PRINT(String("Connection attempt by player who is not in a lobby:") + String::num_uint64(requester.GetAccountID()));
}

// Why is this triggering an error print, sending a signal, and sending a debug signal?  Surely one would suffice.
void SteamMultiplayerPeer::network_messages_session_failed(SteamNetworkingMessagesSessionFailed_t *call_data) {
	SteamNetConnectionInfo_t info = call_data->m_info;
	/// Indicates who this session was with. Depending on the connection type and phase of the connection, we might not know.
	uint64_t other_end_steam_id = info.m_identityRemote.GetSteamID64();

	ERR_PRINT(String("Connection terminated. Reason: ") + String::num(info.m_eEndReason) + String("; Connection state: ") + String::num(info.m_eState));

	emit_signal("network_session_failed", other_end_steam_id, info.m_eEndReason, info.m_eState);

	/// Send a human-readable explanation of the problem to debug signal.
	/// But... again?
	DEBUG_DATA_SIGNAL_V("network_messages_session_failed",
	String::utf8(info.m_szEndDebug) + String("; ") + String::utf8(info.m_szConnectionDescription));
}


///// CALL RESULTS / SIGNALS
/////////////////////////////////////////////////
//	
void SteamMultiplayerPeer::lobby_created(LobbyCreated_t *lobby_data, bool io_failure) {
	if (io_failure) {
		lobby_state = LobbyState::LOBBY_STATE_NOT_CONNECTED;
		// I'm not sure this definitely says the lobby isn't created but perhaps the callback itself fails
		ERR_FAIL_MSG("Failed to create lobby due to Steam IO failure");
		// steamworksError("lobby_created");
	}
	else {
		lobby_state = LobbyState::LOBBY_STATE_HOSTING;
		int result = lobby_data->m_eResult;
		lobby_id = lobby_data->m_ulSteamIDLobby;
		uint64_t lobby = lobby_id.ConvertToUint64();
		emit_signal("lobby_created", result, lobby);
	}
}


///// BIND METHODS
/////////////////////////////////////////////////
//
void SteamMultiplayerPeer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("collect_debug_data"), &SteamMultiplayerPeer::collect_debug_data);
	ClassDB::bind_method(D_METHOD("connect_lobby", "lobby_id"), &SteamMultiplayerPeer::join_lobby);
	ClassDB::bind_method(D_METHOD("create_lobby", "lobby_type", "max_players"), &SteamMultiplayerPeer::create_lobby, DEFVAL(32));
	ClassDB::bind_method(D_METHOD("get_all_lobby_data"), &SteamMultiplayerPeer::get_all_lobby_data);
	ClassDB::bind_method(D_METHOD("get_as_relay"), &SteamMultiplayerPeer::get_as_relay);
	ClassDB::bind_method(D_METHOD("get_direct_messages"), &SteamMultiplayerPeer::get_direct_messages);
	ClassDB::bind_method(D_METHOD("get_lobby_data","key"), &SteamMultiplayerPeer::get_lobby_data);
	ClassDB::bind_method(D_METHOD("get_lobby_id"), &SteamMultiplayerPeer::get_lobby_id);
	ClassDB::bind_method(D_METHOD("get_no_delay"), &SteamMultiplayerPeer::get_no_delay);
	ClassDB::bind_method(D_METHOD("get_no_nagle"), &SteamMultiplayerPeer::get_no_nagle);
	ClassDB::bind_method(D_METHOD("get_state"), &SteamMultiplayerPeer::get_state);
	ClassDB::bind_method(D_METHOD("send_direct_message","message"), &SteamMultiplayerPeer::send_direct_message);
	ClassDB::bind_method(D_METHOD("set_as_relay"), &SteamMultiplayerPeer::set_as_relay);
	ClassDB::bind_method(D_METHOD("set_lobby_data","key","value"), &SteamMultiplayerPeer::set_lobby_data);
	ClassDB::bind_method(D_METHOD("set_lobby_joinable","joinable"), &SteamMultiplayerPeer::set_lobby_joinable);
	ClassDB::bind_method(D_METHOD("set_no_delay"), &SteamMultiplayerPeer::set_no_delay);
	ClassDB::bind_method(D_METHOD("set_no_nagle"), &SteamMultiplayerPeer::set_no_nagle);

	// PEER BINDS ///////////////////////////////
	ClassDB::bind_method(D_METHOD("get_peer_id_from_steam64","steam_id"), &SteamMultiplayerPeer::get_peer_id_from_steam64);
	ClassDB::bind_method(D_METHOD("get_peer_info", "peer_id"), &SteamMultiplayerPeer::get_peer_info);
	ClassDB::bind_method(D_METHOD("get_peer_map"), &SteamMultiplayerPeer::get_peer_map);
	ClassDB::bind_method(D_METHOD("get_steam64_from_peer_id","peer_id"), &SteamMultiplayerPeer::get_steam64_from_peer_id);


	// MATCHMAKING SIGNALS //////////////////////
	ADD_SIGNAL(MethodInfo("lobby_chat_update", PropertyInfo(Variant::INT, "lobby_id"), PropertyInfo(Variant::INT, "changed_id"), PropertyInfo(Variant::INT, "making_change_id"), PropertyInfo(Variant::INT, "chat_state")));
	ADD_SIGNAL(MethodInfo("lobby_created", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "lobby_id")));
	ADD_SIGNAL(MethodInfo("lobby_data_update", PropertyInfo(Variant::INT, "success"), PropertyInfo(Variant::INT, "lobby_id"), PropertyInfo(Variant::INT, "member_id")));
	ADD_SIGNAL(MethodInfo("lobby_joined", PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "permissions"), PropertyInfo(Variant::BOOL, "locked"), PropertyInfo(Variant::INT, "response")));
	ADD_SIGNAL(MethodInfo("lobby_message", PropertyInfo(Variant::INT, "lobby_id"), PropertyInfo(Variant::INT, "user"), PropertyInfo(Variant::STRING, "message"), PropertyInfo(Variant::INT, "chat_type")));
	ADD_SIGNAL(MethodInfo("network_session_failed", PropertyInfo(Variant::INT, "steam_id"), PropertyInfo(Variant::INT, "reason"), PropertyInfo(Variant::INT, "connection_state")));

	// DEBUG SIGNALS ////////////////////////////
	ADD_SIGNAL(MethodInfo("debug_data", PropertyInfo(Variant::DICTIONARY, "data")));


	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_delay"), "set_no_delay", "get_no_delay");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_nagle"), "set_no_nagle", "get_no_nagle");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "as_relay"), "set_as_relay", "get_as_relay");


	BIND_ENUM_CONSTANT(LOBBY_TYPE_PRIVATE);
	BIND_ENUM_CONSTANT(LOBBY_TYPE_FRIENDS_ONLY);
	BIND_ENUM_CONSTANT(LOBBY_TYPE_PUBLIC);
	BIND_ENUM_CONSTANT(LOBBY_TYPE_INVISIBLE);
 	BIND_ENUM_CONSTANT(LOBBY_TYPE_PRIVATE_UNIQUE);

	BIND_ENUM_CONSTANT(LOBBY_STATE_NOT_CONNECTED);
	BIND_ENUM_CONSTANT(LOBBY_STATE_HOST_PENDING);
	BIND_ENUM_CONSTANT(LOBBY_STATE_HOSTING);
	BIND_ENUM_CONSTANT(LOBBY_STATE_CLIENT_PENDING);
	BIND_ENUM_CONSTANT(LOBBY_STATE_CLIENT);
}


SteamMultiplayerPeer::~SteamMultiplayerPeer() {
	if (lobby_id != CSteamID()) {
		close();
	}
}