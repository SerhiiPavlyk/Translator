
add_executable(${PROJECT_NAME}
	${CMAKE_SOURCE_DIR}/Translator_code/resource.h
	${CMAKE_SOURCE_DIR}/Translator_code/Resource.rc
	${CMAKE_SOURCE_DIR}/Translator_code/icon.ico
	${CMAKE_SOURCE_DIR}/Translator_code/Analizators.cpp
	${CMAKE_SOURCE_DIR}/Translator_code/Analizators.h
	${CMAKE_SOURCE_DIR}/Translator_code/CodeGenarator.cpp
	${CMAKE_SOURCE_DIR}/Translator_code/CodeGenarator.h
	${CMAKE_SOURCE_DIR}/Translator_code/Data.cpp
	${CMAKE_SOURCE_DIR}/Translator_code/Data.h
	${CMAKE_SOURCE_DIR}/Translator_code/Errors.cpp
	${CMAKE_SOURCE_DIR}/Translator_code/Errors.h
	${CMAKE_SOURCE_DIR}/Translator_code/Main.cpp
	)