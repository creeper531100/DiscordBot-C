cmake_minimum_required (VERSION 3.6)
project(example)

if(NOT SLEEPY_DISCORD_CMAKE)
	set(ENABLE_VOICE ON CACHE BOOL "")
	add_subdirectory(deps/sleepy-discord)
endif()

add_executable(discord_bot_c source.cpp "core.cpp" "core.h" "Client.cpp" "EmbedMessage.h"  "EmbedMessage.cpp" "request_data.h"  "userfile.cpp" "info.cpp" "crawler.cpp" "JsonHandle.h"   "userfile.h" "info.h" "crawler.h" "userfileadd.h" "userfileadd.cpp" "userfiledel.cpp" "userfiledel.h" "userfilelist.cpp")

if(NOT ENABLE_VOICE)
	message(FATAL_ERROR "ENABLE_VOICE needs to be set to on to be able to compile sound-player example.")
endif()

target_link_libraries(discord_bot_c sleepy-discord)