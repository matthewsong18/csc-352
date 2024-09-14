cmake_minimum_required(VERSION 3.29)
project(assg1 C)

set(CMAKE_C_STANDARD 23)

add_executable(assg1 main.c
        prob1/isFactorial.c
        prob2/getPrime.c
        prob3/sumReverse.c
)
