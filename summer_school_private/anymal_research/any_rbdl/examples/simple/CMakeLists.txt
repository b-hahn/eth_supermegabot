PROJECT (ANY_RBDLEXAMPLE CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# We need to add the project source path to the CMake module path so that
# the FindRBDL.cmake script can be found.
LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR} )

# Search for the RBDL include directory and library
FIND_PACKAGE (ANY_RBDL REQUIRED)
FIND_PACKAGE (Eigen3 3.0.0 REQUIRED)

#C++11
add_definitions(-std=c++11)

# Add the include directory to the include paths
INCLUDE_DIRECTORIES ( ${ANY_RBDL_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} )

# Create an executable
ADD_EXECUTABLE (example example.cc)

# And link the library against the executable
TARGET_LINK_LIBRARIES (example
	${ANY_RBDL_LIBRARY}
	)
