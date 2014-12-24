uniform float alphaFactor;
uniform vec3 colour;

uniform float outlineThreshold = 0.91;
uniform float outlineMin = 0.0;
uniform float outlineMax = 1.0;

uniform sampler2D colourTexture;

in vec2 pass_UV;

out vec4 out_Scene;


void main(void)
{

	vec4 rgba = texture(colourTexture, pass_UV).rgba;
	
	out_Scene = vec4(rgba.rgb * colour, rgba.a * alphaFactor);

}
