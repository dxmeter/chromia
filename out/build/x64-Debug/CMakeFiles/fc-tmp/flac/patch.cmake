cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing patch step for flac")

block(SCOPE_FOR VARIABLES)

execute_process(
  WORKING_DIRECTORY "C:/Users/chloe/Documents/Ecole/projet info/projet/chromia_portfolio/out/build/x64-Debug/_deps/flac-src"
  COMMAND_ERROR_IS_FATAL LAST
  COMMAND  [====[C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe]====] [====[-DFLAC_DIR=C:/Users/chloe/Documents/Ecole/projet info/projet/chromia_portfolio/out/build/x64-Debug/_deps/flac-src]====] [====[-P]====] [====[C:/Users/chloe/Documents/Ecole/projet info/projet/chromia_portfolio/out/build/x64-Debug/_deps/sfml-src/tools/flac/PatchFLAC.cmake]====]
)

endblock()
