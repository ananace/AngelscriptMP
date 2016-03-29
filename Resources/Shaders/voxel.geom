#version 400

uniform vec3 size;

layout(invocations = 6) in;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in mat4 rotation[];
in vec4 colors[];

out vec2 tex_coord;
out vec3 normal;
out vec4 color;

void main() {
	vec3 half_size = size / 2.f;

	for (int i = 0; i < gl_in.length(); i++) {
		vec3 ang_x = vec3(1, 0, 1.f);
		vec3 ang_y = vec3(0, 1, 1.f);
		vec3 pos = gl_in[i].gl_Position.xyz;

		if (gl_InvocationID == 0)
		{
			normal = vec3(rotation[0] * vec4(0, 0, -1, 1)).xyz;

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_y.xy * half_size.xy - ang_x.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 0.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 0.f);
			EmitVertex();
		}
		else if (gl_InvocationID == 1)
		{
			normal = vec3(rotation[0] * vec4(-1, 0, 0, 1)).xyz;

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 0.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 0.f);
			EmitVertex();
		}
		else if (gl_InvocationID == 2)
		{
			normal = vec3(rotation[0] * vec4(1, 0, 0, 1)).xyz;

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 0.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 0.f);
			EmitVertex();
		}
		else if (gl_InvocationID == 3)
		{
			normal = vec3(rotation[0] * vec4(0, -1, 0, 1)).xyz;

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 0.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 0.f);
			EmitVertex();
		}
		else if (gl_InvocationID == 4)
		{
			normal = vec3(rotation[0] * vec4(0, 1, 0, 1)).xyz;

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z - half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 0.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 0.f);
			EmitVertex();
		}
		else if (gl_InvocationID == 5)
		{
			normal = vec3(rotation[0] * vec4(0, 0, 1, 1)).xyz;

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_y.xy * half_size.xy - ang_x.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 1.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy - (ang_x.xy * half_size.xy - ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(1.f, 0.f);
			EmitVertex();

			gl_Position = rotation[0] * vec4(pos.xy + (ang_x.xy * half_size.xy + ang_y.xy * half_size.xy), pos.z + half_size.z, 1.f);
			color = colors[i];
			tex_coord = vec2(0.f, 0.f);
			EmitVertex();
		}

		EndPrimitive();
	}
}