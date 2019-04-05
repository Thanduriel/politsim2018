extends Node2D

const libpolitsim = preload("res://lib/politsim.gdns")
onready var PolitSim = libpolitsim.new()

# Called when the node enters the scene tree for the first time.
func _ready():
	print(PolitSim.World_Test())
