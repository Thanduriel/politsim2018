extends TextureProgress

onready var global = get_node("/root/global")

func _ready():
	value = int(global.politic * 100)
	pass # Replace with function body.

func _process(delta):
	var v = int(global.politic * 100)
	if (v != value):
		value = v
	pass
