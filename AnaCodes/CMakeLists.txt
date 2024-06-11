set(INCLUDES
  ${ROOT_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/build/include
  ${CMAKE_SOURCE_DIR}/hipo/hipo4)
include_directories(${INCLUDES})
link_directories(${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_VERBOSE_MAKEFILE ON)


################################  SkimZeroSuppression  ##########################################
add_executable(test1.exe test1.cc)
target_link_libraries(test1.exe Hipo4 XYHodoLib ${ROOT_LIBRARIES} )
#################################################################################################

################################ Installing Executable targets #############################
install( TARGETS test1.exe
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

################################ Installing Non target files ###############################
install( FILES test1.cc
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

################################ Installing needed directores ##############################
install( DIRECTORY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Figs )


