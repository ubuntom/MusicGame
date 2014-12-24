uniform vec2 offset;
uniform vec2 scale;
uniform float rotation;

in vec2 in_Vertex;
in vec2 in_UV;


out vec2 pass_UV;




void main(void)
{
	
		vec2 rotated;
		rotated.x = in_Vertex.x*cos(rotation) - in_Vertex.y*sin(rotation);
		rotated.y = in_Vertex.x*sin(rotation) + in_Vertex.y*cos(rotation);
		
		
		gl_Position=projectionMatrix * vec4(rotated*scale + offset, 0, 1.0);
		//gl_Position=projectionMatrix * vec4(in_Vertex, 1.0);
		pass_UV = in_UV;

}
