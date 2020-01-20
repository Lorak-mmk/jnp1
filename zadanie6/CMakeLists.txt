cmake_minimum_required(VERSION 3.15)
project(player)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(example File.cc Video.cc Audio.cc Player.cc Playlist.cc SequenceMode.cc ShuffleMode.cc OddEvenMode.cc PlayerException.cc playlist_example.cc)
