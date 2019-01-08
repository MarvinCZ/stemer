cmake_minimum_required(VERSION 3.12)
project(stemer C)

set(CMAKE_C_STANDARD 90)

add_executable(stemer main.c trie.h bool.h trie.c algorithm.h algorithm.c steming.h steming.c)