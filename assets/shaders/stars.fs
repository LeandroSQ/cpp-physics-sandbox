#version 330

precision mediump float;

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Bloom configuration constants
const int radius = 2;
const float threshold = 0.35;
const float intensity = 0.95;
const float size = 1.25;
const float PI = 3.14159;

float calculateLuminance(vec4 color) {
    return dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
}

void main() {
    // Texel size
    vec2 texelSize = 1.0 / textureSize(texture0, 0);
    vec2 normalizedFragTexCoord = vec2(fragTexCoord.x, 1.0 - fragTexCoord.y);

    // Flip vertically to match OpenGL's texture coordinate system
    vec4 texelColor = texture(texture0, normalizedFragTexCoord);

    // Apply gamma correction
    texelColor = pow(texelColor, vec4(2.2));

    // Calculate luminance
    float luminance = calculateLuminance(texelColor);

    // Apply bloom
    vec4 bloom = vec4(0.0);
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            vec2 offset = vec2(i, j) * texelSize * size;
            vec4 sample = texture(texture0, normalizedFragTexCoord + offset);
            float sampleLuminance = calculateLuminance(sample);
            if (sampleLuminance > threshold) {
                bloom += sample * intensity;
            }
        }
    }
    bloom /= (radius * radius * 4.0 + 2.0);
    texelColor += bloom * intensity;
    texelColor.a *= 0.75;

    // texelColor = vec4(1.0, 1.0, 0.0, 1.0);

    finalColor = texelColor * colDiffuse * fragColor;
}