cmake_minimum_required(VERSION 2.8)

# debug output
message ("GTEST_ROOT: " ${GTEST_ROOT})
message ("ENV {GTEST_LIBRARY}: " $ENV{GTEST_LIBRARY})
message ("ENV {GTESTD_LIBRARY}: " $ENV{GTESTD_LIBRARY})
message ("BOOST_ROOT: " ${BOOST_ROOT})

set (test_executable_name uuidTest)

project(${test_executable_name})
enable_testing()

add_executable(${test_executable_name} testMain.cpp uuidTest.cpp uuid_to_time.hpp)

include_directories ("${PROJECT_SOURCE_DIR}" "${GTEST_ROOT}/include" "${BOOST_ROOT}")

if (WIN32)
  message ("---linking windows libs")
  if (DEFINED ENV{APPVEYOR})
    message ("---on appveoyr build machine")
    target_link_libraries (${test_executable_name} optimized $ENV{GTEST_LIBRARY})
    target_link_libraries (${test_executable_name} debug $ENV{GTESTD_LIBRARY})
  else()
    message ("---on local PC")
    target_link_libraries (${test_executable_name} optimized ${GTEST_ROOT}/msvc/gtest/Release/gtest.lib)
    target_link_libraries (${test_executable_name} debug ${GTEST_ROOT}/msvc/gtest/Debug/gtestd.lib)    
  endif()
	
	########################################################################
	# replacing MD with MT, MDd with MTd
  set(variables CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
	foreach(variable ${variables})
		if(${variable} MATCHES "/MD")
		  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
		elseif(${variable} MATCHES "/MDd")
		  string(REGEX REPLACE "/MDd" "/MTd" ${variable} "${${variable}}")
		endif()		
	endforeach()
	########################################################################
	
	if (MSVC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
	endif()
else()
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
  endif()

	message ("---linking linux gtest libs")
	target_link_libraries (${test_executable_name} ${GTEST_ROOT}/cmake/libgtest.so)
endif()

add_test(uuidTest ${test_executable_name})