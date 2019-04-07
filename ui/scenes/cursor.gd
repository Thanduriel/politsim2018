extends TextureRect

var self_destruct = false
var self_destruct_in = 5

func _process(delta):
	if self_destruct:
		self_destruct_in -= delta
		if self_destruct_in < 0:
			queue_free()
	else:
		rect_position = get_global_mouse_position() - rect_size * 0.5