extends Node

onready var global = get_node("/root/global")
var citizen_texture = load("res://assets/citizen/citizen1.tres")

var citizen = Node2D.new()

func _ready():
	global.initialise()
	
	add_child(citizen)
	
func _process(delta):
	var pos = global.World.get_actor_positions()
	var all_citizen = citizen.get_children()
	
	if pos.size() < all_citizen.size():
		var missing = all_citizen.size() - pos.size()
		for i in range(missing):
			var c = Sprite.new()
			c.set_texture(citizen_texture)
			citizen.add_child(c)
	
	for i in range(all_citizen.size()):
		var c = all_citizen[i]
		c.position = pos[i]
