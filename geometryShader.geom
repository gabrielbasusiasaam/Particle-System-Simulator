#version 330 core

layout (points) in;
layout(triangle_strip) out;
layout(max_vertices = 60) out;

in VS_OUT {
	vec4 color;
} gs_in[];

out vec4 pColor;

void main()
{
	float r = .01f;
	vec4 pos = gl_in[0].gl_Position;
	float pi = 3.14159;
	pColor = gs_in[0].color;
	for(int i = 0; i < 15; i++) {
		float theta = (pi / 180.0f) * 12 * i;
		float x = r * cos(theta);
		float y = r * sin(theta);
		gl_Position = pos + vec4(x, y, 0.0f, 0.0f);
		EmitVertex();		
		gl_Position = pos + vec4(x, -y, 0.0f, 0.0f);
		EmitVertex();
	}
	gl_Position = pos + (r * vec4(cos(0), sin(0), 0.0f, 0.0f));
	EmitVertex();
	EndPrimitive();
}	