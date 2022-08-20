cmake_minimum_required(VERSION 3.20)
project(Progetto_TCF)

set(CMAKE_CXX_STANDARD 14)

add_executable(Progetto_TCF main.cpp include/file.hpp src/csvFile.cpp include/csvFile.hpp src/txtFile.cpp include/txtFile.hpp src/data.cpp include/data.hpp src/dataset.cpp include/dataset.hpp src/fileFactory.cpp include/fileFactory.hpp src/partder.cpp include/partder.hpp src/polyfit.cpp include/polyfit.hpp src/udouble.cpp include/udouble.hpp include_t/file_t.hpp src_t/csvFile_t.cpp include_t/csvFile_t.hpp src_t/data_t.cpp include_t/data_t.hpp src_t/dataset_t.cpp include_t/dataset_t.hpp src_t/fileFactory_t.cpp include_t/fileFactory_t.hpp src_t/partder_t.cpp include_t/partder_t.hpp src_t/polyfit_t.cpp include_t/polyfit_t.hpp src_t/txtFile_t.cpp include_t/txtFile_t.hpp src_t/udouble_t.cpp include_t/udouble_t.hpp)
target_link_libraries(Progetto_TCF Eigen matplotlib-cpp-master)