cmake_minimum_required(VERSION 3.28)

find_package(yaml-cpp REQUIRED)

add_library(databook)
set_target_properties(databook PROPERTIES
	VERSION 0.0.1
	LINKER_LANGUAGE CXX)

target_sources(databook 
	PUBLIC FILE_SET CXX_MODULES BASE_DIRS modules FILES 
      modules/formats/notebook_memory.ixx
      modules/formats/notebook_yaml.ixx
      modules/notebook/interface.ixx
	  modules/notebook/notebook.ixx
	  modules/serializable.ixx 
	  modules/databook.ixx)

 target_link_libraries(databook PUBLIC
  yaml-cpp::yaml-cpp
)

add_subdirectory(tests)
