SET(HEADERS
	Application.hpp
	ResourceManager.hpp
	ResourceManager.inl
	StateMachine.hpp
	StateMachine.inl

	Graphics/Drawable3D.hpp
	Graphics/Transform3D.hpp
	Graphics/Transformable3D.hpp
	Graphics/Voxel.hpp

	Graphics/Renderer/GeomShader.hpp
	Graphics/Renderer/IRenderer.hpp
	Graphics/Renderer/VBO.hpp

	States/IState.hpp
	States/IntroState.hpp
)

SET(SOURCES
	Application.cpp
	ResourceManager.cpp
	StateMachine.cpp

	Graphics/Transform3D.cpp
	Graphics/Transformable3D.cpp

	Graphics/Renderer/GeomShader.cpp
	Graphics/Renderer/VBO.cpp

	States/IntroState.cpp

	main.cpp
)

SOURCE_GROUP("Header files\\Graphics\\" REGULAR_EXPRESSION "Graphics/[^./]+.(hpp|inl)")
SOURCE_GROUP("Source files\\Graphics\\" REGULAR_EXPRESSION "Graphics/[^./]+.cpp")
SOURCE_GROUP("Header files\\Graphics\\Renderer\\" REGULAR_EXPRESSION "Graphics/Renderer/[^./]+.(hpp|inl)")
SOURCE_GROUP("Source files\\Graphics\\Renderer\\" REGULAR_EXPRESSION "Graphics/Renderer/[^./]+.cpp")
SOURCE_GROUP("Header files\\States\\" REGULAR_EXPRESSION "States/[^./]+.(hpp|inl)")
SOURCE_GROUP("Source files\\States\\" REGULAR_EXPRESSION "States/[^./]+.cpp")

SOURCE_GROUP("Header files\\" REGULAR_EXPRESSION "[^./]+.(hpp|inl)")
SOURCE_GROUP("Source files\\" REGULAR_EXPRESSION "[^./]+.cpp")

ADD_EXECUTABLE(Client ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES(Client
	Core
)

IF (NOT WIN32)
	TARGET_LINK_LIBRARIES(Client
		pthread
		GL
	)
ENDIF()
