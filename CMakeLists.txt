# CMakeList.txt : CMake project for Genshin_Artifact_Simulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Genshin_Artifact_Simulator")

ADD_COMPILE_OPTIONS ( -g -Wall -analyze )

find_package(ROOT 6.26 REQUIRED)

include(${ROOT_USE_FILE})

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/header/*.hh)
file(GLOB HEADERS_ARTIFACT ${PROJECT_SOURCE_DIR}/header/Artifact/*.hh)
file(GLOB HEADERS_ARTSETSTAT ${PROJECT_SOURCE_DIR}/header/ArtSetStat/*.hh)
file(GLOB HEADERS_CHARACTER ${PROJECT_SOURCE_DIR}/header/Character/*.hh)
file(GLOB HEADERS_SKILL ${PROJECT_SOURCE_DIR}/header/Skill/*.hh)
file(GLOB HEADERS_WEAPON ${PROJECT_SOURCE_DIR}/header/Weapon/*.hh)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB SOURCES_CHARACTER ${PROJECT_SOURCE_DIR}/src/Character/*.cc)
file(GLOB SOURCES_SKILL ${PROJECT_SOURCE_DIR}/src/Skill/*.cc)

message("SRCS : ${SOURCES}")
message("SRCS : ${SOURCES_CHARACTER}")
message("SRCS : ${SOURCES_SKILL}")
message("HEADERS : ${HEADERS}")
message("HEADERS : ${HEADERS_ARTIFACT}")
message("HEADERS : ${HEADERS_ARTSETSTAT}")
message("HEADERS : ${HEADERS_CHARACTER}")
message("HEADERS : ${HEADERS_SKILL}")
message("HEADERS : ${HEADERS_WEAPON}")

add_executable(Genshin_Artifact_Simulator Simulator_Raiden.cpp 
                        ${SOURCES} 
                        ${SOURCES_CHARACTER} 
                        ${SOURCES_SKILL} 
                        ${HEADERS} 
                        ${HEADERS_ARTIFACT} 
                        ${HEADERS_ARTSETSTAT} 
                        ${HEADERS_CHARACTER} 
                        ${HEADERS_SKILL} 
                        ${HEADERS_WEAPON} )

target_link_libraries(Genshin_Artifact_Simulator PUBLIC
        ROOT::Core
        ROOT::Imt
        ROOT::RIO
        ROOT::Net
        ROOT::Hist
        ROOT::Graf
        ROOT::Graf3d
        ROOT::Gpad
        ROOT::ROOTVecOps
        ROOT::Tree
        ROOT::TreePlayer
        ROOT::Rint
        ROOT::Postscript
        ROOT::Matrix
        ROOT::Physics
        ROOT::MathCore
        ROOT::Thread
        ROOT::ROOTDataFrame)