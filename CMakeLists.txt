CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Setting project name
PROJECT(apollon)

# Finding Geant4 packages
FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)
INCLUDE(${Geant4_USE_FILE})

# Header files
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

# Compiling header and source files
FILE(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

ADD_EXECUTABLE(sim main.cc ${sources} ${headers})
TARGET_LINK_LIBRARIES(sim ${Geant4_LIBRARIES})

ADD_CUSTOM_TARGET(apollon DEPENDS sim)