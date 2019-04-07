shader_type canvas_item;

uniform vec4 modulate;

void fragment() {
	COLOR = texture(TEXTURE, UV) * modulate;
}