file(GLOB_RECURSE TEST_SRC *.h *.cpp)

add_library(MyCoolLib_all
		STATIC
		${TEST_SRC}
)

set_target_properties(MyCoolLib_all
	PROPERTIES
	LINKER_LANGUAGE CXX
	PREFIX "lib"
)
