# Adds convenience methods, see cmake/cleanCppExtensions.cmake
include(cleanCppExtensions)

# Creates options to turn on sanitizers, see cmake/sanitizers.cmake
include(sanitizers)

# Enable helpfull warnings and C++17 for all files
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  add_compile_options(/W3 )
else()
  add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic)
endif()

#------------------------------------------------------------------------------
# Libraries dependencies
find_package(OpenMP QUIET)



#-----------------------------------------------------------------------------------
# Prepare cmake package

include(CMakePackageConfigHelpers)
set(ConfigPackageLocation lib/cmake/Radium)


#------------------------------------------------------------------------------
# By using macro to add common dependencies you can avoid repetition when you have
# multiple binaries.
# Use with `configureTargetWithDefaultRaSettings(exampleApp)`
macro(configureTargetWithDefaultRaSettings TARGET)
  message("Configure library ${TARGET} with default settings")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_compile_definitions(${TARGET}
              PUBLIC
              _DEBUG
              )
  endif ()

  target_include_directories(${TARGET} PUBLIC
          $<BUILD_INTERFACE:${RADIUM_SRC_DIR}>
          $<INSTALL_INTERFACE:include>
          )
  target_compile_features(${TARGET} PUBLIC cxx_std_17)

  # Create Exported target properties
  set_target_properties(${TARGET} PROPERTIES VERSION ${RADIUM_VERSION})

  if(OPENMP_FOUND)
    target_link_libraries(${TARGET} PUBLIC OpenMP::OpenMP_CXX)
  endif(OPENMP_FOUND)

  install(TARGETS ${TARGET}
      EXPORT ${TARGET}Targets
      CONFIGURATIONS ${CMAKE_BUILD_TYPE}
      RUNTIME  DESTINATION bin
      OBJECTS  DESTINATION bin
      LIBRARY  DESTINATION lib
      ARCHIVE  DESTINATION lib
      INCLUDES DESTINATION include
      )

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/Radium/${TARGET}ConfigVersion.cmake"
    VERSION ${RADIUM_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

  # export for build tree
  export(EXPORT ${TARGET}Targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/Radium/${TARGET}Targets.cmake"
  )

  # export for the installation tree
  install(EXPORT ${TARGET}Targets
    FILE
      ${TARGET}Targets.cmake
    NAMESPACE
      Radium::
    DESTINATION
      ${ConfigPackageLocation}
  )

#  install(
#    FILES
#      ../cmake/${TARGET}.cmake
#      "${CMAKE_CURRENT_BINARY_DIR}/Radium/Radium${TARGET}ConfigVersion.cmake"
#    DESTINATION
#      ${ConfigPackageLocation}
#)
endmacro()

#-----------------------------------------------------------------------------------
# Include sources
if(RADIUM_GENERATE_LIB_CORE)
    add_subdirectory(Core)
endif()
if(RADIUM_GENERATE_LIB_IO)
    add_subdirectory(IO)
endif()
if(RADIUM_GENERATE_LIB_ENGINE)
    add_subdirectory(Engine)
endif()
if(RADIUM_GENERATE_LIB_GUIBASE)
    add_subdirectory(GuiBase)
endif()
if(RADIUM_GENERATE_LIB_PLUGINBASE)
    add_subdirectory(PluginBase)
endif()


#-----------------------------------------------------------------------------------
# Generate cmake package

configure_file(../cmake/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/Radium/RadiumConfig.cmake"
        @ONLY
)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/Radium/RadiumConfig.cmake"
  DESTINATION
    ${ConfigPackageLocation}
)
#install general scripts
install(
        FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/application_setup.cmake"
        DESTINATION
        ${ConfigPackageLocation}
)


# # #-------------------------------------------------------------------------------
# # # exampleApp executable setup

# # #set(ICON_RESOURCE "")
# if(WIN32) #windows icon resource file, provided from /packaging directory
#     set(ICON_RESOURCE "${PROJECT_BINARY_DIR}/exampleApp.icon.rc")
# elseif (APPLE) #osx icon ressource file, provided from /packaging directory
#     set(ICON_RESOURCE "${PROJECT_SOURCE_DIR}/packaging/exampleApp.icns")
# endif()

# add_executable(exampleApp MACOSX_BUNDLE
#     ${ICON_RESOURCE}
#     ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
#     SomeClass.cpp
#     SomeClass.h
#     main.cpp)
# add_dependencies(exampleApp versionFileTouchForRebuild) # We want precise time of build in version
# target_include_directories(exampleApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries (exampleApp PRIVATE Threads::Threads)

# if(APPLE)
#     set_target_properties(exampleApp PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "exampleApp")
#     set_target_properties(exampleApp PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.exampleApp.exampleApp")
#     set_target_properties(exampleApp PROPERTIES MACOSX_BUNDLE_ICON_FILE exampleApp.icns)
#     set_target_properties(exampleApp PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/packaging/MacOSXBundleInfo.plist.in)
#     set_source_files_properties(${PROJECT_SOURCE_DIR}/packaging/exampleApp.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
# endif()

# install(TARGETS exampleApp
#     BUNDLE DESTINATION "."
#     RUNTIME DESTINATION bin)

# Using macro from cmake/cleanCpppExtensions.cpp included on the top
# Provides exampleApp-run and exampleApp-dbg
#addRunAndDebugTargets(exampleApp)

# short convenience target from cmake/cleanCpppExtensions.cpp
#add_custom_target(run DEPENDS exampleApp-run)

# Manually specified convenience target for running the binary, we use macro
# add_custom_target(exampleApp-run
#     WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
#     USES_TERMINAL
#     COMMAND ./exampleApp
#     DEPENDS Install_)

# And this would add -DBAR_ENABLE_FEATURE or equivalent to build command
# target_compile_definitions(exampleApp PRIVATE BAR_ENABLE_FEATURE)

# Add compile time definition based on option
# option(OPTION_NAME "description for end user" OFF)
# if(OPTION_NAME)
#     # you can do anything here, eg. setup new optional executable
#     target_compile_definitions(exampleApp PRIVATE DEFINE_NAME)
# endif()

# Add building new shared library
# add_library(exampleLibrary SHARED ${LIBRARY_SOURCES})
# target_link_libraries (exampleLibrary ${CMAKE_THREAD_LIBS_INIT})
# set_property(TARGET exampleLibrary PROPERTY CXX_STANDARD 14) # we want C++14

# How to include and link OpenGL and GLUT libraries
# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
# target_include_directories(exampleApp SYSTEM ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )
# target_link_libraries(exampleApp ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

# How to use some other standard packages
# find_package(ZLIB REQUIRED)
# target_link_libraries (exampleApp PRIVATE ZLIB::ZLIB)
#
# find_package(OpenSSL REQUIRED)
# target_link_libraries (exampleApp PRIVATE OpenSSL::SSL OpenSSL::Crypto)
#
# find_package(CURL REQUIRED)
# target_link_libraries (exampleApp PRIVATE CURL::libcurl)

#------------------------------------------------------------------------------
# Unit tests

# add_subdirectory(unittest)

#-------------------------------------------------------------------------------
# Copy MINGW needed libraries for building on windows

if(MINGW)
    message(STATUS "MinGW detected")
    get_filename_component(GCC_PATH ${CMAKE_C_COMPILER} PATH)
    if(${GCC_PATH} MATCHES "mingw64/bin")
        set(libgcc "libgcc_s_seh-1.dll") #64bit
        message(STATUS "  64bit dlls is building")
    else()
        set(libgcc "libgcc_s_dw2-1.dll") #32bit
        message(STATUS "  32bit dlls is building")
    endif()

    install(FILES ${GCC_PATH}/${libgcc}
        ${GCC_PATH}/libstdc++-6.dll
        DESTINATION ./bin/
    )
endif(MINGW)


#------------------------------------------------------------------------------
# Other MISC targets

# Adds misc targets: format, cppcheck, tidy, see cmake/cleanCppExtensions.cmake
set( FORMAT_STYLE_FILENAME ${RADIUM_SRC_DIR}/scripts/clang-format )
addMiscTargets()


