# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GenericMST_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GenericMST_autogen.dir\\ParseCache.txt"
  "GenericMST_autogen"
  )
endif()
