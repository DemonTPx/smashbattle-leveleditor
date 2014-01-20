# define minimum cmake version
cmake_minimum_required(VERSION 2.6.2)
 
# Our project is called 'leveleditor' this is how it will be called in
# visual studio, and in our makefiles. 
project(leveleditor)
 
# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

set(wxWidgets_USE_UNICODE FALSE)
#set(wxWidgets_CONFIG_OPTIONS --ansi)

##---------------------------------------------------
## Please set your wxWidgets configuration here
##--------------------------------------------------- 
 
# Here you can define what libraries of wxWidgets you need for your
# application. You can figure out what libraries you need here;
# http://www.wxwidgets.org/manuals/2.8/wx_librarieslist.html
# We need the Find package for wxWidgets to work
# NOTE: if you're using aui, include aui in this required components list.
 
# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.
find_package(wxWidgets COMPONENTS core base adv REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(ZLIB)
 
##---------------------------------------------------
## Actual config file starts here
##--------------------------------------------------- 
 
# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )
 
# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
file(GLOB_RECURSE SB_LevelEdit_SOURCES "SB_LevelEdit/*.cpp")
file(GLOB_RECURSE SB_LevelEdit_HEADERS "SB_LevelEdit/*.h")
 
# Here we define the executable leveleditor.exe or leveleditor on other systems
# the above paths and defines will be used in this build
add_executable(SB_LevelEdit/leveleditor WIN32 ${SB_LevelEdit_SOURCES})
 
# We add to our target 'leveleditor' the wxWidgets libraries. These are
# set for us by the find script. If you need other libraries, you
# can add them here as well. 
target_link_libraries(SB_LevelEdit/leveleditor ${wxWidgets_LIBRARIES} ${ZLIB_LIBRARIES})
