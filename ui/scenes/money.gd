extends Label

onready var global = get_node("/root/global")

var v

func _ready():
	v = global.money
	text = str(v)
	pass

func _process(delta):
	if (v != global.money):
		v = global.money
		text = str(v)
	pass