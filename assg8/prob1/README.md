# CSC352 Assignment 8

## Purpose

This program is simulating a graph of cities, where the goal of the program is
to find the shortest distance between two cities. This is assuming that the
graph is connected and the edges have weights.

## User Behavior

1. Start program by receiving filename `inputFile` where the file contains the
   cities in the following format:

   ```
   name_1 name_2 dist_n
   ```

2. Query the shortest path between two cities by using the following format:

   ```
   city_1 city_2
   ```

3. The shortest distance will be printed.

## Approach

- Get inputFile filename

- Loop through inputFile

  - Read line

  - Add to graph

- Read in all user queries

- Calculate the shortest path using Djisktra's algorithm (Assuming there are no
  negative paths)

- Print the shortest distance
