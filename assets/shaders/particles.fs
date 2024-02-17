#version 330

precision mediump float;

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main() {
    // Flip vertically to match OpenGL's texture coordinate system
    vec4 texelColor = fragColor;

    // Apply color correction
    texelColor = pow(texelColor, vec4(2.45));
    texelColor.r *= 0.95;
    texelColor.g *= 0.95;
    texelColor.b *= 1.05;

    finalColor = texelColor * colDiffuse;
}