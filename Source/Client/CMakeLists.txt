SET(HEADERS
	Application.hpp
	ResourceManager.hpp
	ResourceManager.inl
	StateMachine.hpp
	StateMachine.inl

	States/IState.hpp
	States/IntroState.hpp
)

SET(SOURCES
	Application.cpp
	ResourceManager.cpp
	StateMachine.cpp

	States/IntroState.cpp

	main.cpp
)

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
	)
ENDIF()
