cmake_minimum_required(VERSION 2.8.3)
project(smb_controller_v2)

find_package(catkin REQUIRED
  COMPONENTS
    # any_measurements_ros
    # any_node
    glog_catkin
    # smb_common
    # smb_description
    smb_driver
    # smb_ros
    # message_logger
    roscpp
    std_msgs
    roslib
    # cosmo_ros
    # signal_logger
    # signal_logger_ros
)

# find_package(smb_common REQUIRED)

catkin_package(
  INCLUDE_DIRS
    include
  LIBRARIES
    ${PROJECT_NAME}
  CATKIN_DEPENDS
    # any_measurements_ros
    # any_node
    glog_catkin
    # smb_common
    # smb_description
    smb_driver
    # smb_ros
    # message_logger
    roscpp
    std_msgs
    roslib
    # cosmo_ros
    # signal_logger
    # signal_logger_ros
)

add_definitions(-std=c++11)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(
  ${PROJECT_NAME}
  src/smb_controller_v2/smb_controller_v2.cpp
)

add_dependencies(
  ${PROJECT_NAME}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(
  ${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

add_executable(
  ${PROJECT_NAME}_node
  src/smb_controller_v2_node.cpp
)
add_dependencies(
  ${PROJECT_NAME}_node
  ${catkin_EXPORTED_TARGETS}

)
target_link_libraries(
  ${PROJECT_NAME}_node
  ${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
install(TARGETS ${PROJECT_NAME}_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(DIRECTORY launch config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
