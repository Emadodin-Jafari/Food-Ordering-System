cmake_minimum_required(VERSION 4.0)
project(Food_Ordering_System)

set(CMAKE_CXX_STANDARD 20)

add_executable(Food_Ordering_System main.cpp Restaurant.cpp Restaurant.h Menu.cpp
        Menu.h sqlite3.c sqlite3.h Database.cpp Database.h MenuDAO.cpp MenuDAO.h
        RestaurantDAO.cpp RestaurantDAO.h Order.cpp Order.h Order.cpp OrderDAO.h)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
