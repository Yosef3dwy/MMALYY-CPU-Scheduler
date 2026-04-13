# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CPU_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CPU_autogen.dir\\ParseCache.txt"
  "CPU_autogen"
  )
endif()
