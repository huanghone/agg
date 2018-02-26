cmake_minimum_required (VERSION 2.6)
project(agg-2.5)

#####################################
# cmake settings
#####################################

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")  
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#####################################
# external libraries
#####################################
#set(freetype_DIR D:/lib/freetype/lib/cmake/freetype)
#find_package(freetype REQUIRED)

#####################################
# include dirs
#####################################

set(INC_DIR
	src
	include
	font_win32_tt
	#${FREETYPE_INCLUDE_DIRS}
)

include_directories(${INC_DIR})

#####################################
# compiler flags
#####################################

set (COMPILE_FLAG
"${CMAKE_CXX_FLAGS}
-DNOMINMAX
/wd4996
/wd4267
/wd4244
/MP
")

# 4996: 
# 4267: size to int
# 4244: float to int

set(CMAKE_CXX_FLAGS ${COMPILE_FLAG})
set(CMAKE_C_FLAGS ${COMPILE_FLAG})

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

#####################################
# dep libs
#####################################

set(DEP_LIBS
		#${FREETYPE_LIBRARIES}
)

#####################################
# agg libs
#####################################

file(GLOB agg_gfx_SRC
	src/gfx/*.h
	src/gfx/*.hh
	src/gfx/*.c
	src/gfx/*.cc
	src/gfx/*.cpp)
source_group("gfx" FILES ${agg_gfx_SRC})

file(GLOB agg_ui_SRC
src/ui/*.h
src/ui/*.hh
src/ui/*.c
src/ui/*.cc
src/ui/*.cpp)
source_group("ui" FILES ${agg_ui_SRC})

file(GLOB agg_ui_ctrl_SRC
src/ui/ctrl/*.h
	src/ui/ctrl/*.hh
	src/ui/ctrl/*.c
	src/ui/ctrl/*.cc
	src/ui/ctrl/*.cpp)
source_group("ui/ctrl" FILES ${agg_ui_ctrl_SRC})

add_library(agg 
	${agg_gfx_SRC}
	${agg_ui_SRC}
	${agg_ui_ctrl_SRC}
)

set(DEP_LIBS ${DEP_LIBS} agg)

#####################################
# agg test
#####################################
add_executable(aa_demo WIN32 examples/aa_demo.cpp)
target_link_libraries(aa_demo ${DEP_LIBS})

add_executable(aa_test WIN32 examples/aa_test.cpp)
target_link_libraries(aa_test ${DEP_LIBS})

add_executable(alpha_gradient WIN32 examples/alpha_gradient.cpp)
target_link_libraries(alpha_gradient ${DEP_LIBS})

add_executable(alpha_mask WIN32 examples/alpha_mask.cpp examples/parse_lion.cpp)
target_link_libraries(alpha_mask ${DEP_LIBS})

add_executable(alpha_mask2 WIN32 examples/alpha_mask2.cpp examples/parse_lion.cpp)
target_link_libraries(alpha_mask2 ${DEP_LIBS})

add_executable(alpha_mask3 WIN32 examples/alpha_mask3.cpp examples/parse_lion.cpp examples/make_arrows.cpp examples/make_gb_poly.cpp)
target_link_libraries(alpha_mask3 ${DEP_LIBS})

add_executable(bezier_div WIN32 examples/bezier_div.cpp)
target_link_libraries(bezier_div ${DEP_LIBS})

add_executable(blend_color WIN32 examples/blend_color.cpp)
target_link_libraries(blend_color ${DEP_LIBS})

add_executable(blur WIN32 examples/blur.cpp)
target_link_libraries(blur ${DEP_LIBS})

add_executable(bspline WIN32 examples/bspline.cpp examples/interactive_polygon.h examples/interactive_polygon.cpp)
target_link_libraries(bspline ${DEP_LIBS})

add_executable(rasterizers WIN32 examples/rasterizers.cpp)
target_link_libraries(rasterizers ${DEP_LIBS})

add_executable(truetype_test WIN32 examples/truetype_test.cpp)
target_link_libraries(truetype_test ${DEP_LIBS})

add_executable(raster_text WIN32 examples/raster_text.cpp)
target_link_libraries(raster_text ${DEP_LIBS})

add_executable(image1 WIN32 examples/image1.cpp)
target_link_libraries(image1 ${DEP_LIBS})

add_executable(flash_rasterizer WIN32 examples/flash_rasterizer.cpp)
target_link_libraries(flash_rasterizer ${DEP_LIBS})

add_executable(flash_rasterizer2 WIN32 examples/flash_rasterizer2.cpp)
target_link_libraries(flash_rasterizer2 ${DEP_LIBS})

set(TEST_TARGET
	aa_demo
	aa_test
	alpha_gradient
	alpha_mask
	alpha_mask2
	alpha_mask3
	bezier_div
	blend_color
	blur
	bspline
	rasterizers
	truetype_test
	raster_text
	image1
	flash_rasterizer
	flash_rasterizer2
)


set_target_properties(${TEST_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_ROOT})