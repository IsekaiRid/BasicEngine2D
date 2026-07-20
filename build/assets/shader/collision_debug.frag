#version 430 core
in vec4 vColor;
in vec2 vUV;
flat in int vType;
out vec4 FragColor;
void main() {
    if (vType == 1) {
        if (length(vUV) > 1.0) discard;
    }
    FragColor = vColor;
}