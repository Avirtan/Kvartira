#version 430

// Tex coord input from vertex shader
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
out vec4 FragColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

void main()
{
	// Sample color from texture
    // FragColor = texture(uTexture, fragTexCoord);
    FragColor = vec4(1, 0.5,1,1);
}
