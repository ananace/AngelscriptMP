#include "IntroState.hpp"

#include <Core/Math.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

#include <random>

#include <SFML/OpenGL.hpp>

#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#endif

struct Point3D {
#if SFML_VERSION_PATCH > 2
	sf::Glsl::Vec3 position;
	sf::Glsl::Vec3 colour;
#else
	sf::Vector3f position;
	sf::Vector3f colour;
#endif
};

sf::Shader shader;

std::vector<Point3D> vertices;

IntroState::IntroState() :
	mTime(0)
{
#if SFML_VERSION_PATCH > 2
	shader.loadFromFile("voxel.vert", "voxel.geom", "voxel.frag");
#else

#endif
	std::random_device rand;

	std::uniform_real_distribution<float> dist(-1.f, 1.f);
	std::uniform_real_distribution<float> col_dist(0.f, 1.f);

	vertices.resize(20);
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = {
			1 * dist(rand),
			1 * dist(rand),
			1 +  dist(rand)
		};

		vertices[i].colour = {
			col_dist(rand),
			col_dist(rand),
			col_dist(rand)
		};
	}
}
IntroState::~IntroState()
{

}

void IntroState::event(const sf::Event& ev)
{
	
}
void IntroState::tick(const Timespan&)
{

}
void IntroState::update(const Timespan& dt)
{
	mTime += dt;

	const float dtTime = std::chrono::duration_cast<std::chrono::duration<float>>(dt).count();
	const float time = std::chrono::duration_cast<std::chrono::duration<float>>(mTime).count();
	const float size = 0.25f + std::abs(std::sin(time)) * 0.25f;

#if SFML_VERSION_PATCH > 2
	shader.setUniform("size", sf::Vector3f(0.25, 0.25, 0.25));
#endif

	float mult = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 1.f : -1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		for (auto& p : vertices)
			p.position.z += dtTime * mult;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		for (auto& p : vertices)
			p.position.x += dtTime * mult;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		for (auto& p : vertices)
			p.position.y += dtTime * mult;
	}
}
void IntroState::draw(sf::RenderTarget& rt)
{
	//rt.pushGLStates();
	/*
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(1.f, -1.0f);
	*/
	glDepthRange(1.f, 0.f);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat pos[] = { 1, 1, 0, 0 };
	GLfloat amb[] = { 0, 0, 0, 1 };
	GLfloat spe[] = { 1, 1, 1, 1 };
	GLfloat dif[] = { 0.2, 0.2, 0.2, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dif);

	glClear(GL_DEPTH_BUFFER_BIT);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	const char* data = reinterpret_cast<const char*>(vertices.data());

	glVertexPointer(3, GL_FLOAT, sizeof(Point3D), data + 0);
	glColorPointer(3, GL_FLOAT, sizeof(Point3D), data + sizeof(Point3D::position));

	sf::Shader::bind(&shader);

	glDrawArrays(GL_POINTS, 0, vertices.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glDepthRange(0.f, 1.f);

	//rt.popGLStates();
}
void IntroState::drawUI(sf::RenderTarget& rt)
{

}
