#include "register_types.h"
#include "../godotsteam/godotsteam.h"
#include "core/config/engine.h"
#include "core/object/class_db.h"
// #include "steam_id.h"
#include "steam_id.h"
#include "steam_multiplayer_peer.h"

static Steam *SteamPtr = NULL;	// Not needed?

void initialize_godotsteam_multiplayer_module(ModuleInitializationLevel level) {
	if (level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<SteamMultiplayerPeer>();
	}
}

void uninitialize_godotsteam_multiplayer_module(ModuleInitializationLevel level) {
}