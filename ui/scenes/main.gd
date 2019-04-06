extends Node

const libpolitsim = preload("res://lib/politsim.gdns")
onready var PolitSim = libpolitsim.new()

func _ready():
#	PolitSim.init(42)
	print("foo")
	print(PolitSim.get_actor_positions())
	print(PolitSim.World_Test())
