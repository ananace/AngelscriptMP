SET(HEADERS
	Application.hpp
	ResourceManager.hpp
	ResourceManager.inl
)

SET(SOURCES
	Application.cpp
	ResourceManager.cpp

	main.cpp
)

ADD_EXECUTABLE(Client ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES(Client
	Core
)
