#ifndef STEAM_ID
#define STEAM_ID

#include "../godotsteam/godotsteam.h"
#include "core/os/os.h"

class SteamID : public RefCounted {
	GDCLASS(SteamID, RefCounted);

	CSteamID data;

public:
	SteamID() {
		data = CSteamID();
	}
	SteamID(CSteamID steamid) {
		data = steamid;
	}
	void setData(CSteamID value) {
		data = value;
	}
	uint64_t to_int() {
		return data.ConvertToUint64();
	}
	void from_int(uint64_t i) {
		data.SetFromUint64(i);
	}
	void from_CSteamID(CSteamID i) {
		data = i;
	}
	uint32 get_account_id() { return data.GetAccountID(); }
	uint32 get_un_account_instance() { return data.GetUnAccountInstance(); }

	enum AccountType {
		ACCOUNT_TYPE_INVALID = k_EAccountTypeInvalid,
		ACCOUNT_TYPE_INDIVIDUAL = k_EAccountTypeIndividual,
		ACCOUNT_TYPE_MULTISEAT = k_EAccountTypeMultiseat,
		ACCOUNT_TYPE_GAME_SERVER = k_EAccountTypeGameServer,
		ACCOUNT_TYPE_ANON_GAME_SERVER = k_EAccountTypeAnonGameServer,
		ACCOUNT_TYPE_PENDING = k_EAccountTypePending,
		ACCOUNT_TYPE_CONTENT_SERVER = k_EAccountTypeContentServer,
		ACCOUNT_TYPE_CLAN = k_EAccountTypeClan,
		ACCOUNT_TYPE_CHAT = k_EAccountTypeChat,
		ACCOUNT_TYPE_CONSOLE_USER = k_EAccountTypeConsoleUser,
		ACCOUNT_TYPE_ANON_USER = k_EAccountTypeAnonUser
	};
	AccountType get_account_type() { return AccountType(data.GetEAccountType()); }

	enum Universe {
		UNIVERSE_INVALID = k_EUniverseInvalid,
		UNIVERSE_PUBLIC = k_EUniversePublic,
		UNIVERSE_BETA = k_EUniverseBeta,
		UNIVERSE_INTERNAL = k_EUniverseInternal,
		UNIVERSE_DEV = k_EUniverseDev
	};
	Universe get_universe() { return Universe(data.GetEUniverse()); }

	inline bool operator==(const SteamID &val) const { return data == val.data; }
	inline bool operator!=(const SteamID &val) const { return data != val.data; }

	static void _bind_methods();
};

#endif // STEAM_ID