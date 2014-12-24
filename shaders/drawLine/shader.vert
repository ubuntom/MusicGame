in vec3 in_Vertex;

out float pass_Alpha;




void main(void)
{
	
		gl_Position=projectionMatrix * vec4(in_Vertex.xy, 0, 1.0);
		//gl_Position=projectionMatrix * vec4(in_Vertex, 1.0);
		pass_Alpha = in_Vertex.z;

}
