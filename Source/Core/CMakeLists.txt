SET(HEADERS
	Engine.hpp
	Engine.inl
	FileWatcher.hpp
	Math.hpp
	ScriptManager.hpp
	ScriptManager.inl
	Time.hpp

	AS_Addons/scriptarray/scriptarray.h
	AS_Addons/scriptbuilder/scriptbuilder.h
	AS_Addons/scriptmath/scriptmath.h
	AS_Addons/scriptstdstring/scriptstdstring.h
	AS_Addons/serializer/serializer.h

	AS_SFML/AS_SFML.hpp
	AS_SFML/Shared.hpp
	AS_SFML/Shared.inl
)

SET(SOURCES
	Engine.cpp
	FileWatcher.cpp
	Math.cpp
	ScriptException.cpp
	ScriptManager.cpp
	Time.cpp

	AS_Addons/scriptarray/scriptarray.cpp
	AS_Addons/scriptbuilder/scriptbuilder.cpp
	AS_Addons/scriptmath/scriptmath.cpp
	AS_Addons/scriptstdstring/scriptstdstring.cpp
	AS_Addons/scriptstdstring/scriptstdstring_utils.cpp
	AS_Addons/serializer/serializer.cpp

	AS_SFML/AS_SFML.cpp
	AS_SFML/Color.cpp
	AS_SFML/Font.cpp
	AS_SFML/Joystick.cpp
	AS_SFML/Keyboard.cpp
	AS_SFML/Listener.cpp
	AS_SFML/Mouse.cpp
	AS_SFML/Music.cpp
	AS_SFML/Rect.cpp
	AS_SFML/Renderer.cpp
	AS_SFML/Shapes.cpp
	AS_SFML/Shared.cpp
	AS_SFML/Sound.cpp
	AS_SFML/Sprite.cpp
	AS_SFML/Text.cpp
	AS_SFML/Texture.cpp
	AS_SFML/Vector2.cpp
	AS_SFML/Vector3.cpp
	AS_SFML/View.cpp
)

SOURCE_GROUP("Header files\\AS_Addons" REGULAR_EXPRESSION "AS_Addons/[^.]+.h")
SOURCE_GROUP("Source files\\AS_Addons" REGULAR_EXPRESSION "AS_Addons/[^.]+.cpp")
SOURCE_GROUP("Header files\\AS_SFML" REGULAR_EXPRESSION "AS_SFML/[^./]+.(hpp|inl)")
SOURCE_GROUP("Source files\\AS_SFML" REGULAR_EXPRESSION "AS_SFML/[^./]+.cpp")

SOURCE_GROUP("Header files\\" REGULAR_EXPRESSION "[^./]+.(hpp|inl)")
SOURCE_GROUP("Source files\\" REGULAR_EXPRESSION "[^./]+.cpp")

ADD_LIBRARY(Core STATIC ${HEADERS} ${SOURCES})
TARGET_LINK_LIBRARIES(Core
	${Angelscript_LIBRARIES}
	${SFML_LIBRARIES}
)
