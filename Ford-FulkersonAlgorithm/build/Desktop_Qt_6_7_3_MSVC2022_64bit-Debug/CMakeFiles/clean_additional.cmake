# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "AG_Tema6_autogen"
  "CMakeFiles\\AG_Tema6_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\AG_Tema6_autogen.dir\\ParseCache.txt"
  )
endif()
