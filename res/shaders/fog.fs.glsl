#version 330

// Input vertex data
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec4 fragColor;

// Output data
out vec4 finalColor;

// Uniforms
uniform sampler2D texture0;
uniform vec3 fogColor;
uniform float fogNear;
uniform float fogFar;

void main()
{
    // Sample texture color
    vec4 texColor = texture(texture0, fragTexCoord);

    // Calculate fog factor
    float fogFactor = (fogFar - fragColor.a) / (fogFar - fogNear);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Mix fog color and texture color
    vec4 color = mix(vec4(fogColor, 1.0), texColor, fogFactor);

    // Calculate final color
    finalColor = color * fragColor;
}

