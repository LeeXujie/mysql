cmake_minimum_required(VERSION 2.8.3)
project(mysql_test)

add_compile_options(-std=c++11)

set(MySQL_INCLUDES_DIRS /usr/include/mysql)
set(MySQL_LIBS /usr/lib/x86_64-linux-gnu/libmysqlclient.so)
# cat /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc
# find_package(mysql REQUIRED)

include_directories(
  include
  ${MySQL_INCLUDES_DIRS}
)

add_executable(${PROJECT_NAME} test/main.cpp
  src/mydb.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${MySQL_LIBS}
)

