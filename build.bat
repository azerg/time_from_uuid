rem RMDIR /S /Q build
cmake . -B"build" -G"Visual Studio 14" -Wno-dev -DBOOST_ROOT:STRING="F:\Work_Installs\boost_1_56_0" -DGTEST_ROOT:STRING="F:\Work_Installs\gtest-1.7.0"
cmake --build "build" --config Release