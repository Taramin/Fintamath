file(
  GLOB_RECURSE
  SOURCES
  CONFIGURE_DEPENDS
  "*.cpp"
  "*.hpp")

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
