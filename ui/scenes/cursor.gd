extends TextureRect

func _input(event):
	if event is InputEventMouseMotion:
		rect_position = event.position - rect_size * 0.5