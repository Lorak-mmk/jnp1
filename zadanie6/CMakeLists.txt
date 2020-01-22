cmake_minimum_required(VERSION 3.15)
project(player)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(example File.cc Player.cc Playlist.cc SequenceMode.cc ShuffleMode.cc OddEvenMode.cc playlist_example.cc Media.cpp Media.h IMediaExtractor.h MediaFactory.h MediaFactory.cpp Audio.cc Audio.h Video.cc Video.h)
