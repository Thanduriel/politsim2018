extends Node

onready var global = get_node("/root/global")

func _ready():
	global.initialise()
