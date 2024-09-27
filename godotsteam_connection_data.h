#ifndef GODOTSTEAM_CONNECTION_DATA
#define GODOTSTEAM_CONNECTION_DATA


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

// Include GodotSteam's Steam SDK headers
#include "../godotsteam/sdk/public/steam/steam_api_flat.h"

#include "godotsteam_constants.h"


// TIME FOR CLASS
/////////////////////////////////////////////////
//
class ConnectionData : public RefCounted {
		GDCLASS(ConnectionData, RefCounted);


public:

	ConnectionData(){};
	~ConnectionData() {
		SteamNetworkingMessages()->CloseSessionWithUser(network_identity);
		while (pending_retry_packets.size()) {
			delete pending_retry_packets.front()->get();
			pending_retry_packets.pop_front();
		}
	}


	// ENUMS
	/////////////////////////////////////////////
	//
	enum ChannelManagement {
		PING_CHANNEL,
		SIZE
	};


	// STRUCTS
	/////////////////////////////////////////////
	//
	struct Packet {
		uint8_t data[k_cbMaxSteamNetworkingSocketsMessageSizeSend];
		uint32_t size = 0;
		CSteamID sender = CSteamID();
		int channel = 0;
		int transfer_mode = k_nSteamNetworkingSend_Reliable;
		Packet() {}
		Packet(const void *p_buffer, uint32 p_buffer_size, int transferMode, int packet_channel) {
			ERR_FAIL_COND_MSG(p_buffer_size > k_cbMaxSteamNetworkingSocketsMessageSizeSend, "[STEAM] Tried sending a packet larger than MAX_STEAM_PACKET_SIZE");
			memcpy(this->data, p_buffer, p_buffer_size);
			this->size = p_buffer_size;
			this->sender = CSteamID();
			this->channel = packet_channel;
			this->transfer_mode = transferMode;
		}
	};

	struct PingPayload {
		int peer_id = -1;
		CSteamID steam_id = CSteamID();
	};


	// PROPERTIES
	/////////////////////////////////////////////
	uint64_t last_msg_timestamp;
	SteamNetworkingIdentity network_identity;
	int peer_id;
	List<Packet *> pending_retry_packets;
	CSteamID steam_id;


	ConnectionData(CSteamID connection_steam_id) {
		this->peer_id = -1;
		this->steam_id = connection_steam_id;
		this->last_msg_timestamp = 0;
		network_identity = SteamNetworkingIdentity();
		network_identity.SetSteamID(connection_steam_id);
	}


	// FUNCTIONS
	/////////////////////////////////////////////
	//
	bool operator==(const ConnectionData &data) { return steam_id == data.steam_id; }

	void add_packet(Packet *packet) { pending_retry_packets.push_back(packet); }

	Dictionary collect_debug_data() {
		Dictionary output;
		output["peer_id"] = peer_id;
		output["steam_id"] = steam_id.GetAccountID();
		output["pending_packet_count"] = pending_retry_packets.size();

		SteamNetConnectionRealTimeStatus_t info;
		SteamNetworkingMessages()->GetSessionConnectionInfo(network_identity, nullptr, &info);
		switch (info.m_eState) {
			case k_ESteamNetworkingConnectionState_None:
				output["connection_status"] = "None";
				break;
			case k_ESteamNetworkingConnectionState_Connecting:
				output["connection_status"] = "Connecting";
				break;
			case k_ESteamNetworkingConnectionState_FindingRoute:
				output["connection_status"] = "Finding route";
				break;
			case k_ESteamNetworkingConnectionState_Connected:
				output["connection_status"] = "Connected";
				break;
			case k_ESteamNetworkingConnectionState_ClosedByPeer:
				output["connection_status"] = "Closed by peer";
				break;
			case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
				output["connection_status"] = "Problem detected locally";
				break;
			case k_ESteamNetworkingConnectionState_FinWait:
				output["connection_status"] = "Final wait";
				break;
			case k_ESteamNetworkingConnectionState_Linger:
				output["connection_status"] = "Linger";
				break;
			case k_ESteamNetworkingConnectionState_Dead:
				output["connection_status"] = "Dead";
				break;
			default:
				output["connection_status"] = "Unknown";
				break;
		}
		output["packets_out_per_sec"] = info.m_flOutPacketsPerSec;
		output["bytes_out_per_sec"] = info.m_flOutBytesPerSec;
		output["packets_in_per_sec"] = info.m_flInPacketsPerSec;
		output["bytes_in_per_sec"] = info.m_flInBytesPerSec;
		output["connection_quality_local"] = info.m_flConnectionQualityLocal;
		output["connection_quality_remote"] = info.m_flConnectionQualityRemote;
		output["send_rate_bytes_per_second"] = info.m_nSendRateBytesPerSecond;
		output["pending_unreliable"] = info.m_cbPendingUnreliable;
		output["pending_reliable"] = info.m_cbPendingReliable;
		output["sent_unacked_reliable"] = info.m_cbSentUnackedReliable;
		output["queue_time"] = (int64_t)info.m_usecQueueTime;

		output["ping"] = info.m_nPing;
		return output;
	}

	Error ping(const PingPayload &p) {
		last_msg_timestamp = OS::get_singleton()->get_ticks_msec(); // only ping once per maxDeltaT;

		auto packet = new Packet((void *)&p, sizeof(PingPayload), MultiplayerPeer::TRANSFER_MODE_RELIABLE, PING_CHANNEL);
		return send(packet);
	}

	Error ping() {
		auto p = PingPayload();
		return ping(p);
	}
	
	Error send(Packet *packet) {
		add_packet(packet);
		return sendPending();
	}

	EResult raw_send(Packet *packet) {
		if (packet->channel == ChannelManagement::PING_CHANNEL) {
			if (packet->size != sizeof(PingPayload)) {
				print_error("Ping is the wrong size, ignoring");
				return k_EResultFail;
			}
		}
		return SteamNetworkingMessages()->SendMessageToUser(network_identity, packet->data, packet->size, packet->transfer_mode, packet->channel);
	}

	Error sendPending() {
		while (pending_retry_packets.size() != 0) {
			auto packet = pending_retry_packets.front()->get();
			auto error_code = raw_send(packet);
			if (error_code == k_EResultOK) {
				delete packet;
				pending_retry_packets.pop_front();
			}
			else {
				if (packet->transfer_mode & k_nSteamNetworkingSend_Reliable) {
					WARN_PRINT(vformat("Send error! Reliable, will retry. EResult %s, refer to Main class in docs.", error_code));
					break;
					//break and try resend later
				}
				else {
					WARN_PRINT(vformat("Send error! Unreliable, won't retry. EResult %s, refer to Main class in docs.", error_code));
					delete packet;
					pending_retry_packets.pop_front();
					//toss the unreliable packet and move on?
				}
			}
		}
		return OK;
	}

};


#endif // GODOTSTEAM_CONNECTION_DATA_H