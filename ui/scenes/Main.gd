extends Node2D

const libpolitsim = preload("res://lib/politsim.gdns")
onready var PolitSim = libpolitsim.new()

func _ready():
	print(PolitSim.World_Test())
