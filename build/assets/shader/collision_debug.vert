#version 430 core
struct CollisionData { vec2 Position; vec2 Size; float Rotation; int Type; vec4 Color; };
layout(std430, binding = 0) buffer CollisionBuffer { CollisionData collision[]; };
uniform mat4 ViewProjection;
out vec4 vColor;
out vec2 vUV;
flat out int vType;
void main() {
    int id = gl_InstanceID;
    int v = gl_VertexID % 6;
    vec2 halfSize = collision[id].Size * 0.5;
    vec2 local;
    if (v == 0) local = vec2(-halfSize.x, -halfSize.y);
    else if (v == 1) local = vec2( halfSize.x, -halfSize.y);
    else if (v == 2) local = vec2( halfSize.x,  halfSize.y);
    else if (v == 3) local = vec2( halfSize.x,  halfSize.y);
    else if (v == 4) local = vec2(-halfSize.x,  halfSize.y);
    else local = vec2(-halfSize.x, -halfSize.y);
    vUV = local / halfSize; // in [-1,1]
    float c = cos(collision[id].Rotation), s = sin(collision[id].Rotation);
    vec2 worldPos = mat2(c, -s, s, c) * local + collision[id].Position;
    gl_Position = ViewProjection * vec4(worldPos, 0.0, 1.0);
    vColor = collision[id].Color;
    vType = collision[id].Type;
}