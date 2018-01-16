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
set(freetype_DIR D:/lib/freetype/lib/cmake/freetype)
find_package(freetype REQUIRED)

#####################################
# include dirs
#####################################

set(INC_DIR
	include
	font_win32_tt
	${FREETYPE_INCLUDE_DIRS}
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
		${FREETYPE_LIBRARIES}
)

#####################################
# agg libs
#####################################

file(GLOB agg_SRC src/*.cpp)
source_group("" FILES ${agg_SRC})

file(GLOB agg_INC include/*.h)
source_group("" FILES ${agg_INC})

file(GLOB agg_ctrl_SRC src/ctrl/*.cpp)
source_group("ctrl" FILES ${agg_ctrl_SRC})

file(GLOB agg_ctrl_INC include/ctrl/*.h)
source_group("ctrl" FILES ${agg_ctrl_INC})

file(GLOB agg_platform_SRC src/platform/win32/*.cpp)
source_group("platfrom" FILES ${agg_platform_SRC})

file(GLOB agg_platform_INC inlcude/platform/*.h include/platform/win32/*.h)
source_group("platfrom" FILES ${agg_platform_INC})

file(GLOB gpc_SRC gpc/*.h gpc/*.c)
source_group("gpc" FILES ${gpc_SRC})

file(GLOB font_win32_tt_SRC font_win32_tt/*.h font_win32_tt/*.cpp)
source_group("font_win32_tt" FILES ${font_win32_tt_SRC})

file(GLOB font_freetype_SRC font_freetype/*.h font_freetype/*.cpp)
source_group("font_freetype" FILES ${font_freetype_SRC})

add_library(agg 
${agg_SRC} ${agg_INC} 
${agg_ctrl_SRC} ${agg_ctrl_INC} 
${agg_platform_SRC} ${agg_platform_INC} 
${gpc_SRC}
${font_win32_tt_SRC}
${font_freetype_SRC})

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