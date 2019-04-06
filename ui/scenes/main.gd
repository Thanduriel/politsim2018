extends Node

onready var global = get_node("/root/global")
var citizen_texture = load("res://assets/citizen/citizen1.tres")

var citizen = Node2D.new()

onready var offset = Vector2(0.125, 0.125)

func _ready():
	global.initialise()
	
	add_child(citizen)
	
func _process(delta):
	var pos = global.World.get_actor_positions()
	var all_citizen = citizen.get_children()
	
	# global.World.update(delta)
	
	if all_citizen.size() < pos.size():
		var missing = pos.size() - all_citizen.size()
		for i in range(missing):
			var c = Sprite.new()
			c.scale *= 0.3
			c.set_texture(citizen_texture)
			citizen.add_child(c)
	
	for i in range(all_citizen.size()):
		# 0.25/tile => 4 tiles/1unit
		# var v = tile_to_pixel(world_to_tile(pos[i]))
		all_citizen[i].position = pos[i] * 4 * 160 * $tiles.scale
		# all_citizen[i].position = $tiles.map_to_world(pos[i] * 4)
