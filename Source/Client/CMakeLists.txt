SET(HEADERS
	Application.hpp
)

SET(SOURCES
	Application.cpp

	main.cpp
)

ADD_EXECUTABLE(Client ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES(Client
	Core
)
