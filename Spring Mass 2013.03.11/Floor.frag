#version 330

layout( location = 0 ) out vec4 FragColor;
uniform vec3 LineColor;

void main() {
    FragColor = vec4(LineColor, 1.0);
}
