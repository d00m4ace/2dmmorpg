# 2dmmorpg
Yet another 2d mmorpg in pure C.

At the current stage, this project is an extensive and ready-to-use pure C codebase for 2D MMORPG development, compatible with Windows, Linux and Android operating systems. Includes a developed 2D engine for displaying sprites, including characters, and a basic user interface. The server part, also made in pure C, provides multitasking processing of TCP/IP connections for a large number of clients and is integrated with PostgreSQL database. 

Also, the entire tool for working with resources is written in C,
and is included in the project. Namely a generator of sprite atlases, a generator of structures with functions for exchanging messages between server and client. 

# Development phase 1

## TODO CLIENT.
1 Make player login screen
2 Login to the game 
3 Display the player in the game world and other players
4 Basic player movement in the world
5 Exit the game

## TODO SERVER
1 Handling player login to the game, registering new players in the game database, logging login to the game
2 Adding a player to the game world
3 Processing all players and their movements in the world
4 Disconnecting a player from the game world
5 Logging every player in the world into the database

