cmake_minimum_required(VERSION 4.0)
project(Food_Ordering_System)

set(CMAKE_CXX_STANDARD 20)

add_executable(Food_Ordering_System main.cpp Menu.h Menu.cpp MenuDAO.h MenuDAO.cpp
                Restaurant.h Restaurant.cpp RestaurantDAO.h RestaurantDAO.cpp
                Order.h Order.cpp OrderDAO.h OrderDAO.cpp Database.h Database.cpp
                sqlite3.h sqlite3.c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
