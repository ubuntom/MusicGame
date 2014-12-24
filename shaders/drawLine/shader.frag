uniform vec3 colour;



in float pass_Alpha;

out vec4 out_Scene;


void main(void)
{

	
	out_Scene = vec4(colour, pass_Alpha);

}
