#include "register_types.h"
#include "class_db.h"
#include "engine.h"
#include "godotsteam_server.h"

static SteamServer* SteamServerPtr = NULL;

void register_godotsteam_server_types(){
	ClassDB::register_class<SteamServer>();
	SteamServerPtr = memnew(SteamServer);
	Engine::get_singleton()->add_singleton(Engine::Singleton("SteamServer",SteamServer::get_singleton()));
}
void unregister_godotsteam_server_types(){
	memdelete(SteamServerPtr);
}
