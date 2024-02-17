#version 330

precision mediump float;

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 resolution;

vec3 applyVignetteLinear(vec3 color) {
    // Apply vignette
    float radius = 0.95;
    float softness = 0.9999;
    float intensity = 0.6;
    vec2 position = gl_FragCoord.xy / resolution.xy;

    float vignette = smoothstep(radius, radius - softness, length(position - 0.5));
    return mix(color, color * vignette, intensity);
}

void main() {
    // DEBUG: Render gl_FragCoord gradient
    // finalColor = vec4(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y, 0.0, 1.0); return;

    finalColor = vec4(0.0, 0.06, 0.1, 1.0);

    // DEBUG: Apply gamma correction
    // finalColor.rgb = pow(finalColor.rgb, vec3(1.0 / 2.2));

    // Apply vignette
    finalColor.rgb = applyVignetteLinear(finalColor.rgb);
}