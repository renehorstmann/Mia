cmake_minimum_required(VERSION 3.5)
project(mia C)

add_definitions(-DMIA_TITLE=Mia)


# options
set(USE_MIXER false)
set(USE_TTF false)  # currently not supported by mia

set(USE_THREAD true)
set(USE_SOCKET true)
set(USE_FETCH true)
set(USE_GAMEPAD true)

set(USE_SANITIZER true)
set(USE_TESTS false)
set(USE_GL_CHECK true)

# build internal apps
set(USE_APP_EX true)        # Examples
set(USE_APP_MP true)        # Mia Paint


# Bundling a module (or all, see below) into a single module source file speeds up linking
set(USE_BUNDLE_O false)
set(USE_BUNDLE_M false)
set(USE_BUNDLE_S false)
set(USE_BUNDLE_R false)
set(USE_BUNDLE_A false)
set(USE_BUNDLE_U false)
set(USE_BUNDLE_W false)
set(USE_BUNDLE_X false)
set(USE_BUNDLE_APP_EX false)
set(USE_BUNDLE_APP_MP false)

set(USE_BUNDLES true)

if(USE_BUNDLES)
    set(USE_BUNDLE_O true)
    set(USE_BUNDLE_M true)
    set(USE_BUNDLE_S true)
    set(USE_BUNDLE_R true)
    set(USE_BUNDLE_A true)
    set(USE_BUNDLE_U true)
    set(USE_BUNDLE_W true)
    set(USE_BUNDLE_X true)
    set(USE_BUNDLE_APP_EX true)
    set(USE_BUNDLE_APP_MP true)
endif()

# see logo/ dirs; mainly for the emscripten build
set(LOGO_VERSION debug)
#set(LOGO_VERSION ex_tea)


# Platforms definitions:
# MIA_PLATFORM_MSVC         Microsoft C++ Compiler
# MIA_PLATFORM_MINGW        Windows Msys2 Mingw-w64
# MIA_PLATFORM_UNIX         Ubuntu, etc.
# MIA_PLATFORM_CXXDROID     Android Cxx compiler App
# MIA_PLATFORM_EMSCRIPTEN   emscripten web compiler
# MIA_PLATFORM_ANDROID      AndroidStudio

# Platform collections:
# MIA_PLATFORM_WINDOWS:     MIA_PLATFORM_MSVC + MIA_PLATFORM_MINGW
# MIA_PLATFORM_DESKTOP:     MIA_PLATFORM_WINDOWS + MIA_PLATFORM_UNIX

# Optional definitions
# MIA_OPTION_GLES           for OpenGLES3.0 instead of OpenGL3.3
# MIA_OPTION_GLEW           if GLEW is available
# MIA_OPTION_TTF            for ttf text render support using SDL_ttf
# MIA_OPTION_THREAD         to use threading stuff
# MIA_OPTION_SOCKET         to use sockets (uses SDL_net)
# MIA_OPTION_FETCH          to use fetch   (http rest with curl or another implementation)
# MIA_OPTION_GAMEPAD        loads a gamepad (game controller) if available
# MIA_OPTION_SANITIZER      use sanitizer checks for debugging
# MIA_OPTION_TESTS          start module test in test/* within o_init (will call o_exit on failure...)
# MIA_OPTION_GL_CHECK       checks for gl errors
# MIA_TERMINALCOLOR_OFF     turn off terminal colors
# MIA_LOG_COMPACT           disable time and file in logs
# MIA_BUNDLE_<MODULE>       to bundle a module into a single source file, speeds up linking
# MIA_TITLE                 set to the title, else Mia
# MIA_INIT_FULLSCREEN       if set, screen mode default is fullscreen, else window


set(CMAKE_C_STANDARD 11)
message("Compiler: ${CMAKE_C_COMPILER_ID}")

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

# Glob all source files for each module
file(GLOB INCLUDE_SRCS "${PROJECT_SOURCE_DIR}/include/*")
file(GLOB SRC_SRCS "${PROJECT_SOURCE_DIR}/src/*")


# Module O
if(USE_BUNDLE_O)
    message("Unity bundle build for module O")
    add_definitions(-DMIA_BUNDLE_O)
    set(O_SRCS "${PROJECT_SOURCE_DIR}/src/o/o.c")
else()
    file(GLOB O_SRCS "${PROJECT_SOURCE_DIR}/src/o/*.c")
    list(REMOVE_ITEM O_SRCS "${PROJECT_SOURCE_DIR}/src/o/o.c")
endif()

# Module M
if(USE_BUNDLE_M)
    message("Unity bundle build for module M")
    add_definitions(-DMIA_BUNDLE_M)
    set(M_SRCS "${PROJECT_SOURCE_DIR}/src/m/m.c")
else()
    file(GLOB M_SRCS "${PROJECT_SOURCE_DIR}/src/m/*.c")
    list(REMOVE_ITEM M_SRCS "${PROJECT_SOURCE_DIR}/src/m/m.c")
endif()

# Module S
if(USE_BUNDLE_S)
    message("Unity bundle build for module S")
    add_definitions(-DMIA_BUNDLE_S)
    set(S_SRCS "${PROJECT_SOURCE_DIR}/src/s/s.c")
else()
    file(GLOB S_SRCS "${PROJECT_SOURCE_DIR}/src/s/*.c")
    list(REMOVE_ITEM S_SRCS "${PROJECT_SOURCE_DIR}/src/s/s.c")
endif()

# Module R
if(USE_BUNDLE_R)
    message("Unity bundle build for module R")
    add_definitions(-DMIA_BUNDLE_R)
    set(R_SRCS "${PROJECT_SOURCE_DIR}/src/r/r.c")
else()
    file(GLOB R_SRCS "${PROJECT_SOURCE_DIR}/src/r/*.c")
    list(REMOVE_ITEM R_SRCS "${PROJECT_SOURCE_DIR}/src/r/r.c")
endif()

# Module A
if(USE_BUNDLE_A)
    message("Unity bundle build for module A")
    add_definitions(-DMIA_BUNDLE_A)
    set(A_SRCS "${PROJECT_SOURCE_DIR}/src/a/a.c")
else()
    file(GLOB A_SRCS "${PROJECT_SOURCE_DIR}/src/a/*.c")
    list(REMOVE_ITEM A_SRCS "${PROJECT_SOURCE_DIR}/src/a/a.c")
endif()

# Module U
if(USE_BUNDLE_U)
    message("Unity bundle build for module U")
    add_definitions(-DMIA_BUNDLE_U)
    set(U_SRCS "${PROJECT_SOURCE_DIR}/src/u/u.c")
else()
    file(GLOB U_SRCS "${PROJECT_SOURCE_DIR}/src/u/*.c")
    list(REMOVE_ITEM U_SRCS "${PROJECT_SOURCE_DIR}/src/u/u.c")
endif()

# Module W
if(USE_BUNDLE_W)
    message("Unity bundle build for module W")
    add_definitions(-DMIA_BUNDLE_W)
    set(W_SRCS "${PROJECT_SOURCE_DIR}/src/w/w.c")
else()
    file(GLOB W_SRCS "${PROJECT_SOURCE_DIR}/src/w/*.c")
    list(REMOVE_ITEM W_SRCS "${PROJECT_SOURCE_DIR}/src/w/w.c")
endif()

# Module X
if(USE_BUNDLE_X)
    message("Unity bundle build for module X")
    add_definitions(-DMIA_BUNDLE_X)
    set(X_SRCS "${PROJECT_SOURCE_DIR}/src/x/x.c")
else()
    file(GLOB X_SRCS "${PROJECT_SOURCE_DIR}/src/x/*.c")
    list(REMOVE_ITEM X_SRCS "${PROJECT_SOURCE_DIR}/src/x/x.c")
endif()


# App Module EX
set(APP_EX_SRCS "")
if(USE_APP_EX)
    message("Building app module EX")
    if(USE_BUNDLE_APP_EX)
        message("Unity bundle build for app module EX")
        add_definitions(-DMIA_BUNDLE_APP_EX)
        set(APP_EX_SRCS "${PROJECT_SOURCE_DIR}/src/app/ex/ex.c")
    else()
        file(GLOB APP_EX_SRCS "${PROJECT_SOURCE_DIR}/src/app/ex/*.c")
        list(REMOVE_ITEM APP_EX_SRCS "${PROJECT_SOURCE_DIR}/src/app/ex/ex.c")
    endif()
endif()

# App Module
set(APP_MP_SRCS "")
if(USE_APP_MP)
    message("Building app module MP")
    if(USE_BUNDLE_APP_MP)
        message("Unity bundle build for app module MP")
        add_definitions(-DMIA_BUNDLE_APP_MP)
        set(APP_MP_SRCS "${PROJECT_SOURCE_DIR}/src/app/mp/mp.c")
    else()
        file(GLOB APP_MP_SRCS "${PROJECT_SOURCE_DIR}/src/app/mp/*.c")
        list(REMOVE_ITEM APP_MP_SRCS "${PROJECT_SOURCE_DIR}/src/app/mp/mp.c")
    endif()
endif()


# Combine all source lists
list(APPEND SRCS
        ${INCLUDE_SRCS}
        ${SRC_SRCS}
        ${O_SRCS}
        ${M_SRCS}
        ${S_SRCS}
        ${R_SRCS}
        ${A_SRCS}
        ${U_SRCS}
        ${W_SRCS}
        ${X_SRCS}
        ${APP_EX_SRCS}
        ${APP_MP_SRCS}
)

if(USE_TESTS)
    file(GLOB SRCS_TESTS
            "${PROJECT_SOURCE_DIR}/test/*"
            "${PROJECT_SOURCE_DIR}/test/o/*"
            "${PROJECT_SOURCE_DIR}/test/r/*"
            )
endif ()

add_executable(mia ${SRCS} ${SRCS_TESTS})


# set default configuration
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to Debug as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()
message("Using build type: " ${CMAKE_BUILD_TYPE})

# check platform
if (MINGW)
    message("Platform: Windows MinGW")
    add_definitions(-DMIA_PLATFORM_MINGW)
    add_definitions(-DMIA_PLATFORM_WINDOWS)
    add_definitions(-DMIA_PLATFORM_DESKTOP)
    set(MIA_PLATFORM_MINGW true)
elseif (WIN32)
    message("Platform: Windows MSVC")
    add_definitions(-DMIA_PLATFORM_MSVC)
    add_definitions(-DMIA_PLATFORM_WINDOWS)
    add_definitions(-DMIA_PLATFORM_DESKTOP)
    set(MIA_PLATFORM_MSVC true)
elseif (EMSCRIPTEN)
    message("Platform: Emscripten")
    add_definitions(-DMIA_PLATFORM_EMSCRIPTEN)
    set(MIA_PLATFORM_EMSCRIPTEN true)
else ()
    find_library(GL_LIB GL)
    find_library(GLES_LIB GLESv3)
    if (GL_LIB)
        message("Platform: Unix")
        add_definitions(-DMIA_PLATFORM_UNIX)
        add_definitions(-DMIA_PLATFORM_DESKTOP)
        set(MIA_PLATFORM_UNIX true)
    elseif (GLES_LIB)
        message("Platform: CxxDroid")
        add_definitions(-DMIA_PLATFORM_CXXDROID)
        set(MIA_PLATFORM_CXXDROID true)
    else ()
        message(FATAL_ERROR "Platform: unknown, neither GL nor GLES found!")
    endif ()
endif ()


# turn off invalid options
if (MIA_PLATFORM_EMSCRIPTEN)
	if (USE_THREAD)
		message("USE_THREAD -> OFF: MIA_PLATFORM_EMSCRIPTEN does not support threads by default")
		set(USE_THREAD false)
	endif()
	if (USE_SOCKET)
		message("USE_SOCKET -> OFF: MIA_PLATFORM_EMSCRIPTEN does not support sockets by default")
		set(USE_SOCKET false)
	endif()
	if (USE_SANITIZER)
		message("USE_SANITIZER -> OFF: Emscripten is able to do it, but may crash the app")
		set(USE_SANITIZER false)
	endif()
endif()


# all platforms:
if (USE_MIXER)
    message("USE_MIXER")
endif ()
if (USE_TTF)
    message("USE_TTF")
    add_definitions(-DMIA_OPTION_TTF)
endif ()
if (USE_THREAD)
    message("USE_THREAD")
    add_definitions(-DMIA_OPTION_THREAD)
endif ()
if(USE_SOCKET)
    message("USE_SOCKET")
    add_definitions(-DMIA_OPTION_SOCKET)
endif()
if (USE_FETCH)
    message("USE_FETCH")
    add_definitions(-DMIA_OPTION_FETCH)
endif ()
if (USE_GAMEPAD)
    message("USE_GAMEPAD")
    add_definitions(-DMIA_OPTION_GAMEPAD)
endif ()
if(USE_TESTS)
    message("USE_TESTS")
    add_definitions(-DMIA_OPTION_TESTS)
endif()
if(USE_GL_CHECK)
    message("USE_GL_CHECK")
    add_definitions(-DMIA_OPTION_GL_CHECK)
endif()




# windows mingw
if (MIA_PLATFORM_MINGW)
    message("Setup Windows MinGW")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long -Wno-unused-function -Wno-unused-variable -Wno-missing-braces -Wno-unused-parameter -Wno-format-truncation")

    # mingw is not shipped with a find_package(SDL2) script
    add_definitions(-Dmain=SDL_main)
    add_definitions(-DMIA_OPTION_GLEW)

    # mingw may not find a few libraries, due to missing CMAKE_LIBRARY_PATH lib paths
    find_library(OPENGL_LIB opengl32 REQUIRED)
    find_library(GLEW_LIB glew32 REQUIRED)
    find_library(GLU_LIB glu32 REQUIRED)

    target_link_libraries(mia
            m
            mingw32
            SDL2main
            SDL2
            SDL2_image
            comdlg32        # e_ext_sfd
            ${OPENGL_LIB}
            ${GLEW_LIB}
            ${GLU_LIB}
            )

    if (USE_SOCKET)
        target_link_libraries(mia SDL2_net ws2_32)
    endif ()

    if (USE_MIXER)
        target_link_libraries(mia SDL2_mixer)
    endif ()
    if (USE_TTF)
        target_link_libraries(mia SDL2_ttf)
    endif ()
    if (USE_FETCH)
        target_link_libraries(mia curl)
    endif ()
endif ()

# windows msvc
if (MIA_PLATFORM_MSVC)
    message("Setup Windows MSVC")

    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4267 /wd4244 /wd4305 /wd4996 /wd5105")

    add_definitions(-Dmain=SDL_main)
    add_definitions(-DMIA_OPTION_GLEW)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(GLEW REQUIRED)

    target_link_libraries(mia
            SDL2::SDL2
            SDL2::SDL2main
            SDL2_image::SDL2_image
            GLEW::GLEW
            comdlg32        # e_ext_sfd
            )

    if (USE_SOCKET)
        find_package(SDL2_net CONFIG REQUIRED)
        target_link_libraries(mia SDL2_net::SDL2_net ws2_32)
    endif ()

    if (USE_MIXER)
        find_package(SDL2_mixer CONFIG REQUIRED)
        target_link_libraries(mia SDL2_mixer::SDL2_mixer)
    endif ()
    if (USE_TTF)
        find_package(SDL2_ttf CONFIG REQUIRED)
        target_link_libraries(mia SDL2_ttf::SDL2_ttf)
    endif ()
    if (USE_FETCH)
        find_package(CURL CONFIG REQUIRED)
        target_link_libraries(mia CURL::libcurl)
    endif ()
endif ()


# unix
if (MIA_PLATFORM_UNIX)
    message("Setup Unix")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long -Wno-unused-function -Wno-unused-variable -Wno-missing-braces -Wno-unused-parameter")

    if (USE_SANITIZER)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            message("Using Google sanitizer")
            add_definitions(-DMIA_OPTION_SANITIZER)
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
            set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
        endif ()
    endif ()

    find_package(SDL2 REQUIRED)
    find_library(SDL2_IMAGE_LIBRARIES SDL2_image REQUIRED)

    find_library(GL_LIB GL REQUIRED)

    target_link_libraries(mia
            m
            ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES}
            ${GL_LIB}
            )

    # not necessary in unix
    find_library(GLEW_LIB GLEW)
    if (GLEW_LIB)
        add_definitions(-DMIA_OPTION_GLEW)
        target_link_libraries(mia ${GLEW_LIB})
    endif ()

    if (USE_SOCKET)
        find_library(SDL2_NET_LIBRARIES SDL2_net REQUIRED)
        target_link_libraries(mia ${SDL2_NET_LIBRARIES})
    endif ()

    if (USE_MIXER)
        find_library(SDL2_MIXER_LIBRARIES SDL2_mixer REQUIRED)
        target_link_libraries(mia ${SDL2_MIXER_LIBRARIES})
    endif ()
    if (USE_TTF)
        find_library(SDL2_TTF_LIBRARIES SDL2_ttf REQUIRED)
        target_link_libraries(mia ${SDL2_TTF_LIBRARIES})
    endif ()
    if (USE_FETCH)
        find_package(CURL REQUIRED)
        if(NOT CURL_LIBRARIES)
            # stupid cmake bug...
            message("USE_FETCH -> Failed to collect \"CURL_LIBRARIES\", setting it to \"/usr/lib/x86_64-linux-gnu/libcurl.so\"")
            set(CURL_LIBRARIES "/usr/lib/x86_64-linux-gnu/libcurl.so")
        endif()
        target_link_libraries(mia ${CURL_LIBRARIES})
    endif ()
endif ()



# emscripten
if (MIA_PLATFORM_EMSCRIPTEN)
    message("Setup Emscripten")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long -Wno-unused-function -Wno-unused-variable -Wno-missing-braces -Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments")

    if (USE_SANITIZER)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            message("Using Google sanitizer")
            add_definitions(-DMIA_OPTION_SANITIZER)
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
            set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
        endif ()
    endif ()

    add_definitions(-DMIA_OPTION_GLES)
    add_definitions(-DMIA_TERMINALCOLOR_OFF)
    add_definitions(-DMIA_LOG_COMPACT)

    # libs
    set(EMS_LIBS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\", \"jpg\"]'")
    if (USE_MIXER)
		set(EMS_LIBS "${EMS_LIBS} -s USE_SDL_MIXER=2")
    endif ()
    if (USE_TTF)
		set(EMS_LIBS "${EMS_LIBS} -s USE_SDL_TTF=2")
    endif ()

    # compile step needs the libs flags, so emscripten ports are automatically installed
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EMS_LIBS}")

    # link flags
    set(EMS_LINK_FLAGS "${EMS_LIBS} -s FULL_ES3=1")
    if (USE_FETCH)
        set(EMS_LINK_FLAGS "${EMS_LINK_FLAGS} -s FETCH=1")
    endif ()

	set(EMS_LINK_FLAGS "${EMS_LINK_FLAGS} -s EXPORTED_FUNCTIONS='[\"_main\", \"_o_file_record__idbfs_synced\", \"_o_file_upload__done\"]'")
	set(EMS_LINK_FLAGS "${EMS_LINK_FLAGS} -s EXPORTED_RUNTIME_METHODS=FS,ccall")
	set(EMS_LINK_FLAGS "${EMS_LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1")
    set(EMS_LINK_FLAGS "${EMS_LINK_FLAGS} -lidbfs.js")
    set(EMS_LINK_FLAGS "${EMS_LINK_FLAGS} --preload-file res")

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("Compiling with support for emrun")
        set(EMS_LINK_FLAGS "${EMS_LINK_FLAGS} --emrun -Wno-limited-postlink-optimizations")
    endif()

	set_target_properties(mia PROPERTIES LINK_FLAGS ${EMS_LINK_FLAGS})


    set_target_properties(mia PROPERTIES OUTPUT_NAME "index" SUFFIX ".js")

    # Custom index.html
    configure_file(${CMAKE_SOURCE_DIR}/web/index.html ${CMAKE_BINARY_DIR}/index.html COPYONLY)

	# icons used by the web/index.html
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/logo/${LOGO_VERSION}/favicon.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/logo/${LOGO_VERSION}/icon180.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/logo/${LOGO_VERSION}/icon196.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # debug website
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/web/debug.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

endif ()


# CxxDroid
if (MIA_PLATFORM_CXXDROID)
    message("Setup CxxDroid")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long -Wno-unused-function -Wno-unused-variable -Wno-missing-braces -Wno-unused-parameter")

    add_definitions(-DMIA_OPTION_GLES)
    add_definitions(-DMIA_TERMINALCOLOR_OFF)
    add_definitions(-DMIA_LOG_COMPACT)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)

    find_library(GLES_LIB GLESv3 REQUIRED)

    target_link_libraries(mia
            m
            ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES}
            ${GLES_LIB}
            )

    if (USE_SOCKET)
        find_package(SDL2_net REQUIRED)
        target_link_libraries(mia ${SDL2_NET_LIBRARIES})
    endif ()

    if (USE_MIXER)
        find_package(SDL2_mixer REQUIRED)
        target_link_libraries(mia ${SDL2_MIXER_LIBRARIES})
    endif ()
    if (USE_TTF)
        find_package(SDL2_ttf REQUIRED)
        target_link_libraries(mia ${SDL2_TTF_LIBRARIES})
    endif ()
    if (USE_FETCH)
        find_package(CURL REQUIRED)
        target_link_libraries(mia ${CURL_LIBRARIES})
    endif ()
endif ()

# res
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (MIA_PLATFORM_MSVC)
    # also copy res to MSVC subdirs Debug and Release
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
endif ()
