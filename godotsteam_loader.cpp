#include "godotsteam.h"

void *steam_library_handle = nullptr;

typedef ESteamAPIInitResult (*SteamInternal_SteamAPI_Init_t)(const char*, SteamErrMsg*);
SteamInternal_SteamAPI_Init_t pointer_SteamInternal_SteamAPI_Init = nullptr;

typedef HSteamUser (*SteamAPI_GetHSteamUser_t)();
SteamAPI_GetHSteamUser_t pointer_SteamAPI_GetHSteamUser = nullptr;

typedef bool (*SteamAPI_IsSteamRunning_t)();
SteamAPI_IsSteamRunning_t pointer_SteamAPI_IsSteamRunning = nullptr;

typedef void (*SteamAPI_RegisterCallResult_t)(class CCallbackBase *pCallback, SteamAPICall_t hAPICall);
SteamAPI_RegisterCallResult_t pointer_SteamAPI_RegisterCallResult = nullptr;

typedef void (*SteamAPI_RegisterCallback_t)(class CCallbackBase *pCallback, int iCallback);
SteamAPI_RegisterCallback_t pointer_SteamAPI_RegisterCallback = nullptr;

typedef void (*SteamAPI_RunCallbacks_t)();
SteamAPI_RunCallbacks_t pointer_SteamAPI_RunCallbacks = nullptr;

typedef void (*SteamAPI_UnregisterCallResult_t)(class CCallbackBase *pCallback, SteamAPICall_t hAPICall);
SteamAPI_UnregisterCallResult_t pointer_SteamAPI_UnregisterCallResult = nullptr;

typedef void (*SteamAPI_UnregisterCallback_t)(class CCallbackBase *pCallback);
SteamAPI_UnregisterCallback_t pointer_SteamAPI_UnregisterCallback = nullptr;

typedef HSteamUser (*SteamGameServer_GetHSteamUser_t)();
SteamGameServer_GetHSteamUser_t pointer_SteamGameServer_GetHSteamUser = nullptr;

typedef void* (*SteamInternal_ContextInit_t)(void*);
SteamInternal_ContextInit_t pointer_SteamInternal_ContextInit = nullptr;

typedef bool (*SteamAPI_RestartAppIfNecessary_t)(uint32);
SteamAPI_RestartAppIfNecessary_t pointer_SteamAPI_RestartAppIfNecessary = nullptr;

typedef void* (*SteamInternal_FindOrCreateGameServerInterface_t)(HSteamUser, const char *);
SteamInternal_FindOrCreateGameServerInterface_t pointer_SteamInternal_FindOrCreateGameServerInterface = nullptr;

typedef void* (*SteamInternal_FindOrCreateUserInterface_t)(HSteamUser, const char *);
SteamInternal_FindOrCreateUserInterface_t pointer_SteamInternal_FindOrCreateUserInterface = nullptr;

typedef void (*SteamAPI_Shutdown_t)();
SteamAPI_Shutdown_t pointer_SteamAPI_Shutdown = nullptr;

bool tried_loading_steam_dll = false;

void try_load_steam_dll() {
	if (tried_loading_steam_dll) {
		return;
	}
	tried_loading_steam_dll = true;
	String path;
 	if (OS::get_singleton()->has_feature("linuxbsd")) {
 		path = OS::get_singleton()->get_executable_path().get_base_dir().path_join("libsteam_api.so");
 		if (!FileAccess::exists(path)) {
 			path = OS::get_singleton()->get_executable_path().get_base_dir().path_join("../lib").path_join("libsteam_api.so");
 			if (!FileAccess::exists(path)) {
 				return;
 			}
 		}
 	} else if (OS::get_singleton()->has_feature("windows")) {
 		if (OS::get_singleton()->has_feature("64")) {
 			path = OS::get_singleton()->get_executable_path().get_base_dir().path_join("steam_api64.dll");
 		} else {
 			path = OS::get_singleton()->get_executable_path().get_base_dir().path_join("steam_api.dll");
 		}
 		if (!FileAccess::exists(path)) {
 			return;
 		}
 	} else if (OS::get_singleton()->has_feature("macos")) {
 		path = OS::get_singleton()->get_executable_path().get_base_dir().path_join("libsteam_api.dylib");
 		if (!FileAccess::exists(path)) {
 			path = OS::get_singleton()->get_executable_path().get_base_dir().path_join("../Frameworks").path_join("libsteam_api.dylib");
 			if (!FileAccess::exists(path)) {
 				return;
 			}
 		}
 	} else {
 		return;
 	}

 	Error err = OS::get_singleton()->open_dynamic_library(path, steam_library_handle);
 	if (err != OK) {
 		steam_library_handle = nullptr;
 		return;
 	}
 	print_verbose("Loaded SteamAPI library");

	// Load init function
 	void *symbol_handle = nullptr;
 	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamInternal_SteamAPI_Init", symbol_handle, true);
 	if (err != OK) {
		ERR_PRINT("Cannot load function SteamInternal_SteamAPI_Init");
		return;
 	} else {
 		pointer_SteamInternal_SteamAPI_Init = reinterpret_cast<ESteamAPIInitResult (*)(const char*, SteamErrMsg*)>(symbol_handle);
 	}

	// Load SteamAPI_GetHSteamUser function
 	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_GetHSteamUser", symbol_handle, true);
 	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_GetHSteamUser");
		return;
 	} else {
 		pointer_SteamAPI_GetHSteamUser = reinterpret_cast<HSteamUser (*)()>(symbol_handle);
 	}

	// Load SteamAPI_IsSteamRunning function
 	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_IsSteamRunning", symbol_handle, true);
 	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_IsSteamRunning");
		return;
 	} else {
 		pointer_SteamAPI_IsSteamRunning = reinterpret_cast<bool (*)()>(symbol_handle);
 	}

	// Load SteamAPI_RegisterCallResult function
 	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_RegisterCallResult", symbol_handle, true);
 	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_RegisterCallResult");
		return;
 	} else {
 		pointer_SteamAPI_RegisterCallResult = reinterpret_cast<void (*)(class CCallbackBase*, SteamAPICall_t)>(symbol_handle);
 	}

	// Load SteamAPI_RegisterCallback function
 	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_RegisterCallback", symbol_handle, true);
 	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_RegisterCallback");
		return;
 	} else {
 		pointer_SteamAPI_RegisterCallback = reinterpret_cast<void (*)(class CCallbackBase*, int)>(symbol_handle);
 	}

	// Load SteamAPI_RunCallbacks function
 	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_RunCallbacks", symbol_handle, true);
 	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_RunCallbacks");
		return;
 	} else {
 		pointer_SteamAPI_RunCallbacks = reinterpret_cast<void (*)()>(symbol_handle);
 	}

	// Load SteamAPI_UnregisterCallResult
 	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_UnregisterCallResult", symbol_handle, true);
 	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_UnregisterCallResult");
		return;
 	} else {
 		pointer_SteamAPI_UnregisterCallResult = reinterpret_cast<void (*)(class CCallbackBase*, SteamAPICall_t)>(symbol_handle);
 	}

	// Load SteamAPI_UnregisterCallback
	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_UnregisterCallback", symbol_handle, true);
	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_UnregisterCallback");
		return;
	} else {
		pointer_SteamAPI_UnregisterCallback = reinterpret_cast<void (*)(class CCallbackBase*)>(symbol_handle);
	}

	// Load SteamGameServer_GetHSteamUser
	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamGameServer_GetHSteamUser", symbol_handle, true);
	if (err != OK) {
		ERR_PRINT("Cannot load function SteamGameServer_GetHSteamUser");
		return;
	} else {
		pointer_SteamGameServer_GetHSteamUser = reinterpret_cast<HSteamUser (*)()>(symbol_handle);
	}

	// Load SteamInternal_ContextInit
	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamInternal_ContextInit", symbol_handle, true);
	if (err != OK) {
		ERR_PRINT("Cannot load function SteamInternal_ContextInit");
		return;
	} else {
		pointer_SteamInternal_ContextInit = reinterpret_cast<void *(*)(void*)>(symbol_handle);
	}

	// Load SteamAPI_RestartAppIfNecessary
	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_RestartAppIfNecessary", symbol_handle, true);
	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_RestartAppIfNecessary");
		return;
	} else {
		pointer_SteamAPI_RestartAppIfNecessary = reinterpret_cast<bool (*)(uint32)>(symbol_handle);
	}

	// Load SteamInternal_FindOrCreateGameServerInterface
	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamInternal_FindOrCreateGameServerInterface", symbol_handle, true);
	if (err != OK) {
		ERR_PRINT("Cannot load function SteamInternal_FindOrCreateGameServerInterface");
		return;
	} else {
		pointer_SteamInternal_FindOrCreateGameServerInterface = reinterpret_cast<void *(*)(HSteamUser, const char *)>(symbol_handle);
	}

	// Load SteamInternal_FindOrCreateUserInterface
	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamInternal_FindOrCreateUserInterface", symbol_handle, true);
	if (err != OK) {
		ERR_PRINT("Cannot load function SteamInternal_FindOrCreateUserInterface");
		return;
	} else {
		pointer_SteamInternal_FindOrCreateUserInterface = reinterpret_cast<void *(*)(HSteamUser, const char *)>(symbol_handle);
	}
	
	// Load SteamAPI_Shutdown
	err = OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_Shutdown", symbol_handle, true);
	if (err != OK) {
		ERR_PRINT("Cannot load function SteamAPI_Shutdown");
		return;
	} else {
		pointer_SteamAPI_Shutdown = reinterpret_cast<void (*)()>(symbol_handle);
	}
 }

S_API ESteamAPIInitResult S_CALLTYPE SteamInternal_SteamAPI_Init( const char *pszInternalCheckInterfaceVersions, SteamErrMsg *pOutErrMsg ) {
	try_load_steam_dll();
	if (pointer_SteamInternal_SteamAPI_Init != nullptr) {
		return pointer_SteamInternal_SteamAPI_Init(pszInternalCheckInterfaceVersions, pOutErrMsg);
	}
	return ESteamAPIInitResult::k_ESteamAPIInitResult_FailedGeneric;
}

S_API HSteamUser S_CALLTYPE SteamAPI_GetHSteamUser() {
	try_load_steam_dll();
	if (pointer_SteamAPI_GetHSteamUser != nullptr) {
		return pointer_SteamAPI_GetHSteamUser();
	}
	return 0;
}

S_API bool S_CALLTYPE SteamAPI_IsSteamRunning() {
	try_load_steam_dll();
	if (pointer_SteamAPI_IsSteamRunning != nullptr) {
		return pointer_SteamAPI_IsSteamRunning();
	}
	return false;
}

S_API void SteamAPI_RegisterCallResult S_CALLTYPE ( class CCallbackBase *pCallback, SteamAPICall_t hAPICall ) {
	try_load_steam_dll();
	if (pointer_SteamAPI_RegisterCallResult != nullptr) {
		pointer_SteamAPI_RegisterCallResult(pCallback, hAPICall);
	}
}

S_API void S_CALLTYPE SteamAPI_RegisterCallback( class CCallbackBase *pCallback, int iCallback ) {
	try_load_steam_dll();
	if (pointer_SteamAPI_RegisterCallback != nullptr) {
		pointer_SteamAPI_RegisterCallback(pCallback, iCallback);
	}
}

S_API void S_CALLTYPE SteamAPI_RunCallbacks() {
	try_load_steam_dll();
	if (pointer_SteamAPI_RunCallbacks != nullptr) {
		pointer_SteamAPI_RunCallbacks();
	}
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallResult( class CCallbackBase *pCallback, SteamAPICall_t hAPICall ) {
	try_load_steam_dll();
	if (pointer_SteamAPI_UnregisterCallResult != nullptr) {
		pointer_SteamAPI_UnregisterCallResult(pCallback, hAPICall);
	}
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallback( class CCallbackBase *pCallback ) {
	try_load_steam_dll();
	if (pointer_SteamAPI_UnregisterCallback != nullptr) {
		pointer_SteamAPI_UnregisterCallback(pCallback);
	}
}

S_API HSteamUser S_CALLTYPE SteamGameServer_GetHSteamUser() {
	try_load_steam_dll();
	if (pointer_SteamGameServer_GetHSteamUser != nullptr) {
		return pointer_SteamGameServer_GetHSteamUser();
	}
	return 0;
}

S_API void *S_CALLTYPE SteamInternal_ContextInit( void *pContextInitData ) {
	try_load_steam_dll();
	if (pointer_SteamInternal_ContextInit != nullptr) {
		return pointer_SteamInternal_ContextInit(pContextInitData);
	}
	return nullptr;
}

S_API bool S_CALLTYPE SteamAPI_RestartAppIfNecessary( uint32 unOwnAppID ) {
	try_load_steam_dll();
	if (pointer_SteamAPI_RestartAppIfNecessary != nullptr) {
		return pointer_SteamAPI_RestartAppIfNecessary(unOwnAppID);
	}
	return false;
}

S_API void *S_CALLTYPE SteamInternal_FindOrCreateGameServerInterface( HSteamUser hSteamUser, const char *pszVersion ) {
	try_load_steam_dll();
	if (pointer_SteamInternal_FindOrCreateGameServerInterface != nullptr) {
		return pointer_SteamInternal_FindOrCreateGameServerInterface(hSteamUser, pszVersion);
	}
	return nullptr;
}

S_API void *S_CALLTYPE SteamInternal_FindOrCreateUserInterface( HSteamUser hSteamUser, const char *pszVersion ) {
	try_load_steam_dll();
	if (pointer_SteamInternal_FindOrCreateUserInterface != nullptr) {
		return pointer_SteamInternal_FindOrCreateUserInterface(hSteamUser, pszVersion);
	}
	return nullptr;
}

S_API void S_CALLTYPE SteamAPI_Shutdown() {
	try_load_steam_dll();
	if (pointer_SteamAPI_Shutdown != nullptr) {
		pointer_SteamAPI_Shutdown();
	}
}