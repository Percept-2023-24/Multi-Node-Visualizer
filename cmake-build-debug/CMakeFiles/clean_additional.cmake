# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Percept__Multi_node_UI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Percept__Multi_node_UI_autogen.dir/ParseCache.txt"
  "Percept__Multi_node_UI_autogen"
  )
endif()
