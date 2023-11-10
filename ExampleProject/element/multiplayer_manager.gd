extends Node

@export var level = "res://level.tscn"

func _ready():
	multiplayer.connected_to_server.connect(connected_to_server)
	multiplayer.connection_failed.connect(connection_failed)
	multiplayer.peer_connected.connect(peer_connected)
	multiplayer.peer_disconnected.connect(peer_disconnected)
	multiplayer.server_disconnected.connect(server_disconnected)
	$MultiplayerSpawner.spawn_function = spawnLevel

func connected_to_server():
	pass
#	if multiplayer.get_unique_id() == 1:
#		get_tree().change_scene_to_file(level)

func connection_failed():
	pass

func peer_connected(id: int):
	pass
#	if multiplayer.get_unique_id() == 1:
#		set_scene.rpc_id(id,level)

func peer_disconnected(id: int):
	pass

func server_disconnected():
	multiplayer.multiplayer_peer = null
	pass


func setLevel(data:String):
	$MultiplayerSpawner.spawn(data)

func spawnLevel(data:String):
	var a = (load(data) as PackedScene).instantiate()
	return a
