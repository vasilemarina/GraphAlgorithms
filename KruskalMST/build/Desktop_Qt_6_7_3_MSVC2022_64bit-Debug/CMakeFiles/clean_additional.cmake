# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\KruskalMST_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\KruskalMST_autogen.dir\\ParseCache.txt"
  "KruskalMST_autogen"
  )
endif()
