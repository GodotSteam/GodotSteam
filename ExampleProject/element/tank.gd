extends CharacterBody2D


@export_category("Movement")
@export var moveSpeed = 100.0
@export var rotationSpeed = 1.0
@export var turretSpeed = 2.0
@export_category("Nodes")
@export var turret:Node2D
@export var barrelTip:Node2D
@export var bullet:PackedScene
@export var spawner:MultiplayerSpawner

@export_category("Settables")
@export_range(0.0,360.0) var color:float = 0.0:
	set(value):
		material.set_shader_parameter("hueOffset",color)

func _ready():
	var s = 200
	position += Vector2(randf_range(-s,s),randf_range(-s,s))
	if is_multiplayer_authority():
		color = 180.0
		%Camera2D.enabled = true
	else:
		%Camera2D.enabled = false
	spawner.spawn_function = spawnBullet


func _physics_process(delta):
	if is_multiplayer_authority() == false:
		return

	var forward = Input.get_axis("forward","back")
	var turn = Input.get_axis("left","right")
	var turretTurn = Input.get_axis("turretLeft","turretRight")
	
	rotation += turn * delta * rotationSpeed
	velocity = Vector2.DOWN.rotated(rotation) * moveSpeed * forward
	move_and_slide()
	
	turret.rotation += turretTurn * delta * turretSpeed
	if Input.is_action_just_pressed("fire"):
		spawner.spawn(bullet)

func spawnBullet(data):
	var b:Node2D = bullet.instantiate()
	b.global_transform = barrelTip.global_transform
	b.shooter = self
	var auth = get_multiplayer_authority()
	b.set_multiplayer_authority(auth)
	return b
