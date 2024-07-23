cmake_minimum_required(VERSION 3.10)
project(HotelProject VERSION 1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SQLite3_INCLUDE_DIRS "C:/Users/User/Downloads/sqlite-amalgamation-3460000/sqlite-amalgamation-3460000")  
set(SQLite3_LIBRARIES "C:/Users/User/Downloads/sqlite-amalgamation-3460000/sqlite-amalgamation-3460000")

set(SOURCES
    src/main.cpp 
    src/Hotel.cpp 
    src/Room.cpp 
    src/User.cpp 
    src/sqlite3.c
)


add_executable(run ${SOURCES})

target_include_directories(run PRIVATE sqlite)