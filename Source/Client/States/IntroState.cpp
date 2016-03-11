#include "IntroState.hpp"

#include <Core/Math.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <SFML/Graphics/VertexArray.hpp>

#include <random>

sf::Shader shader;
sf::VertexArray test(sf::Points, 100);

IntroState::IntroState() : 
	mTime(0)
{
	shader.loadFromMemory(
		// Vertex
		"uniform float angle;\n"
		"out float angles;\n"
		"void main() {\n"
		"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
		"  angles = angle;\n"
		"}",

		// Geometry
		"#version 150\n"
		"in float angles[];\n"
		"uniform vec2 size;\n"
		"layout (points) in;\n"
		"layout (triangle_strip, max_vertices = 32) out;\n"
		"out vec2 tex_coord;\n"
		"out vec4 color;\n"
		"void main() {\n"
		"  vec2 half_size = size / 2.f;\n"
		"  for (int i = 0; i < gl_in.length(); i++) {\n"
		"    vec2 ang_x = vec2(cos(angles[i]), sin(angles[i]));\n"
		"    vec2 ang_y = vec2(cos(angles[i] + 1.5707f), sin(angles[i] + 1.5707f));\n"
		"    vec2 pos = gl_in[i].gl_Position.xy;\n"
		"    gl_Position = vec4(pos - (ang_x * half_size + ang_y * half_size), 0.f, 1.f);\n"
		"    color = vec4(1.f, 0.f, 0.f, 1.f);\n"
		"    tex_coord = vec2(1.f, 1.f);\n"
		"    EmitVertex();\n"
		"    gl_Position = vec4(pos - (ang_y * half_size - ang_x * half_size), 0.f, 1.f);\n"
		"    color = vec4(0.f, 1.f, 0.f, 1.f);\n"
		"    tex_coord = vec2(0.f, 1.f);\n"
		"    EmitVertex();\n"
		"    gl_Position = vec4(pos - (ang_x * half_size - ang_y * half_size), 0.f, 1.f);\n"
		"    color = vec4(0.f, 0.f, 1.f, 1.f);\n"
		"    tex_coord = vec2(1.f, 0.f);\n"
		"    EmitVertex();\n"
		"    gl_Position = vec4(pos + (ang_x * half_size + ang_y * half_size), 0.f, 1.f);\n"
		"    color = vec4(1.f, 1.f, 1.f, 1.f);\n"
		"    tex_coord = vec2(0.f, 0.f);\n"
		"    EmitVertex();\n"
		"    EndPrimitive();\n"
		"  }\n"
		"}",

		// Fragment
		"#version 150\n"
		"//uniform sampler2D texture;\n"
		"in vec2 tex_coord;\n"
		"in vec4 color;\n"
		"void main() {\n"
		"  gl_FragColor = color;\n"
		"}"
		);

	std::random_device rand;

	std::uniform_real_distribution<float> dist(-1.f, 1.f);

	for (int i = 0; i < test.getVertexCount(); ++i)
	{
		test[i].position = {
			800 * dist(rand),
			600 * dist(rand)
		};
	}
}
IntroState::~IntroState()
{

}

void IntroState::event(const sf::Event&)
{

}
void IntroState::tick(const Timespan&)
{

}
void IntroState::update(const Timespan& dt)
{
	mTime += dt;

	const float time = std::chrono::duration_cast<std::chrono::duration<float>>(mTime).count();
	const float size = 0.25f + std::abs(std::sin(time)) * 0.25f;

	shader.setUniform("size", sf::Vector2f(size, size));
	shader.setUniform("angle", time * Math::PI2);
}
void IntroState::draw(sf::RenderTarget& rt)
{
	rt.draw(test, &shader);
}
void IntroState::drawUI(sf::RenderTarget& rt)
{

}