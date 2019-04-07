extends Node

onready var global = get_node("/root/global")
onready var template1 = preload("res://scenes/citizen.tscn")
onready var template2 = preload("res://scenes/citizen2.tscn")

var citizen = Node2D.new()

func _ready():
	global.initialise()
	
	add_child(citizen)
	
	$sound/BG1.play()
	$sound/BG2.play()
	$sound/BG3.play()
	$sound/BG4.play()
	
	
func _process(delta):
	var pos = global.World.get_actor_positions()
	var all_citizen = citizen.get_children()
	
	global.World.update(delta)
	global.money = global.World.get_money()
	global.politic = global.World.get_politic()
	
	if all_citizen.size() < pos.size():
		var missing = pos.size() - all_citizen.size()
		for i in range(missing):
			var c
			if randf() > 0.5:
				c = template1.instance()
			else:
				c = template2.instance()
			c.scale *= 0.3
			citizen.add_child(c)
	
	for i in range(all_citizen.size()):
		# 0.25/tile => 4 tiles/1unit
		all_citizen[i].position = pos[i] * 4 * 160 * $tiles.scale