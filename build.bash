g++ main.cpp maps.cpp controls.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -O1
#the bash command i use to compile the project on my debian 13 machine
#raylib is included from the system include directory!!! if you don't have raylib there, you'll need to include it from somewhere else