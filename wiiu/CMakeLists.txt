cmake_minimum_required(VERSION 3.18.4)

project(SonicMania LANGUAGES C CXX)

add_executable(SonicMania WIN32
	"../SonicMania/Game.c"
	"../SonicMania/Objects/All.c"
)

target_include_directories(SonicMania PRIVATE
	"../SonicMania"
	"../SonicMania/Objects"
)

target_compile_definitions(SonicMania PRIVATE
	GAME_MAIN=main
)

add_subdirectory("../dependencies/RSDKv5/wiiu" EXCLUDE_FROM_ALL "RSDKv5")
target_link_libraries(SonicMania PRIVATE RSDKv5)

target_include_directories(RSDKv5 PRIVATE "../SonicMania")

if(WIIU)
	wut_create_rpx(SonicMania)
	wut_create_wuhb(SonicMania
		NAME      "Sonic Mania"
		SHORTNAME "Sonic Mania"
		AURTHOR   "Clownacy, Rubberduckycooly, chuliRMG"
		ICON      "../wiiu/icon.png"
		TVSPLASH  "../wiiu/tv.png"
		DRCSPLASH "../wiiu/drc.png"
	)
endif()
