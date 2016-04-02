#pragma once

#include "Drawable3D.hpp"
#include "Transformable3D.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/InputStream.hpp>

#include <string>
#include <vector>

namespace Graphics
{

class Voxel : public Transformable3D, public Drawable3D
{
public:
	Voxel();
	Voxel(const Voxel&) = default;
	Voxel(Voxel&&) = default;
	~Voxel() = default;

	Voxel& operator=(const Voxel&) = default;

	bool loadFromFile(const std::string& file);
	bool loadFromStream(sf::InputStream& stream);

	void draw(Renderer::IRenderer& renderer) const;

private:
#pragma pack(push, 1)
	struct VoxelData
	{
		sf::Vector3f Position;
		sf::Color Color;
	};
#pragma pack(pop)

	std::vector<VoxelData> mData;
};

}
