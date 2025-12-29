# What is Smol
Smol is a simple "game engine" made for micro games. It's goal is to be so simple that it's embedable and portable to as many platforms as possible.

# Structure

-> smol.h : defines some basic structures
=> smolscript : a very simple language to define the behaviour of the game. It compiles dot smol files. 
Those files contains the graphics and the logic of each games.
=> smollib : a library for reading, decoding, and playing smol files. It provides a number of tools to then implement your own interpreter or compiler.
