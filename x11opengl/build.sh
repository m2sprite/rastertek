#!/bin/bash
g++ -c application_class.cpp
g++ -c input_class.cpp
g++ -c main.cpp
g++ -c opengl_class.cpp
g++ -c system_class.cpp
g++ -o client application_class.o input_class.o main.o opengl_class.o system_class.o -lGL -lX11
