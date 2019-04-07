extends Area2D

onready var indicator = $"../indicator"

func _input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton and event.is_pressed():
		indicator.visible = not indicator.visible