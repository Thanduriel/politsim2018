extends TextureRect

func _process(delta):
	rect_position = get_global_mouse_position() - rect_size * 0.5