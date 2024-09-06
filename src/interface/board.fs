#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

void main() {
    if (mod(int(fragTexCoord.x * 8) + int(fragTexCoord.y * 8), 2) == 0) {
        finalColor = vec4(0.9333333333, 0.9333333333, 0.8235294117, 255);
    } else {
        finalColor = vec4(0.462745098, 0.58823529, 0.337254901, 255);
    }
}
