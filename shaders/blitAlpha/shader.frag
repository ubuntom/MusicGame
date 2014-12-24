uniform float alphaFactor = 1.f;
uniform vec3 colour = vec3(1.f, 1.f, 1.f);

uniform sampler2D colourTexture;

in vec2 pass_UV;

out vec4 out_Scene;


void main(void)
{

	float a = texture(colourTexture, pass_UV).a;
	
	//out_Scene = vec4(colour, a * alphaFactor);
	out_Scene = vec4(1,1,1,a);
}
