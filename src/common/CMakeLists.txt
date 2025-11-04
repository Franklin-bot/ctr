

add_library(common INTERFACE)
add_library(common::common ALIAS common)


target_include_directories(common
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
