#version 330

precision mediump float;

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float aberration;

#define USE_CHROMATIC_ABERRATION
#define USE_BLOOM
#define USE_GAMMA_CORRECTION
#define USE_SATURATION
#define USE_CONTRAST
#define USE_BRIGHTNESS
#define USE_PIXELATION

void applyPixelation(inout vec2 normalizedFragTexCoord) {
#ifdef USE_PIXELATION
    float pixelation = 0.005;
    float pixelationIntensity = pow(aberration, 1.0 / 2.0) * 0.5;

    vec2 pixelatedFragTexCoord = floor(normalizedFragTexCoord / pixelation) * pixelation;
    normalizedFragTexCoord = mix(normalizedFragTexCoord, pixelatedFragTexCoord, pixelationIntensity);
#endif
}

void applyChromaticAberration(inout vec4 texelColor, in vec2 normalizedFragTexCoord, in vec2 texelSize) {
#ifdef USE_CHROMATIC_ABERRATION
    float edgeFactor = max(length(normalizedFragTexCoord - 0.5) * 2.0, 0.35);
    float aberrationIntensity = edgeFactor * aberration * 1.5;
    vec2 aberrationOffset = vec2(aberrationIntensity, -aberrationIntensity);
    float red = texture(texture0, normalizedFragTexCoord + aberrationOffset * texelSize).r;
    float green = texture(texture0, normalizedFragTexCoord).g;
    float blue = texture(texture0, normalizedFragTexCoord - aberrationOffset * texelSize).b;
    texelColor.rgb = vec3(red, green, blue);
#endif
}

void applyGammaCorrection(inout vec4 texelColor) {
#ifdef USE_GAMMA_CORRECTION
    float gammmaCorrectionIntensity = 0.25;
    texelColor.rgb = mix(texelColor.rgb, pow(texelColor.rgb, vec3(1.0 / 2.2)), gammmaCorrectionIntensity);
#endif
}

void applyBloom(inout vec4 texelColor, in vec2 normalizedFragTexCoord, in vec2 texelSize) {
#ifdef USE_BLOOM
    float bloomIntensity = 0.95;
    float bloomThreshold = 0.60;
    int bloomRadius = 4;
    vec4 bloomColor = vec4(0.0);
    for (int i = -bloomRadius; i <= bloomRadius; i++) {
        for (int j = -bloomRadius; j <= bloomRadius; j++) {
            vec2 offset = vec2(i, j) * texelSize;
            vec4 bloomTexelColor = texture(texture0, normalizedFragTexCoord + offset);
            if (length(bloomTexelColor.rgb) > bloomThreshold) {
                bloomColor += bloomTexelColor;
            }
        }
    }
    bloomColor /= float((bloomRadius * 2 + 1) * (bloomRadius * 2 + 1));
    // Tint the bloom color
    vec4 tint = vec4(1.0, 0.9, 0.7, 1.0);
    bloomColor *= tint;

    texelColor.rgb += bloomColor.rgb * bloomIntensity;
#endif
}

void applySaturation(inout vec4 texelColor) {
#ifdef USE_SATURATION
    float saturation = 1.25;
    float luminance = dot(texelColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    texelColor.rgb = mix(vec3(luminance), texelColor.rgb, saturation);
#endif
}

void applyContrast(inout vec4 texelColor) {
#ifdef USE_CONTRAST
    float contrast = 0.98;
    texelColor.rgb = (texelColor.rgb - 0.5) * pow(contrast, 2.0) + 0.5;
#endif
}

void applyBrightness(inout vec4 texelColor) {
#ifdef USE_BRIGHTNESS
    float brightness = 1.05;
    texelColor.rgb *= pow(brightness, 2.0);
#endif
}

void main() {
    vec2 texelSize = 1.0 / textureSize(texture0, 0);
    // Flip vertically to match OpenGL's texture coordinate system
    vec2 normalizedFragTexCoord = vec2(fragTexCoord.x, 1.0 - fragTexCoord.y);

    applyPixelation(normalizedFragTexCoord);

    vec4 texelColor = texture(texture0, normalizedFragTexCoord);

    applyChromaticAberration(texelColor, normalizedFragTexCoord, texelSize);
    applyGammaCorrection(texelColor);
    applyBloom(texelColor, normalizedFragTexCoord, texelSize);

    applySaturation(texelColor);
    applyContrast(texelColor);
    applyBrightness(texelColor);

    finalColor = texelColor * colDiffuse * fragColor;
}