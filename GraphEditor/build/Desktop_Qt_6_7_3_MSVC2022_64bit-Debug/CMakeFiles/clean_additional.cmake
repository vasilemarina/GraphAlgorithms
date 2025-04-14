# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GraphEditor_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GraphEditor_autogen.dir\\ParseCache.txt"
  "GraphEditor_autogen"
  )
endif()
