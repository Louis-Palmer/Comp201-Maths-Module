# Security Assignment 2022/2023

This project is a security assignment for comp280 which features a basic level system and shop system. The main features are being able to save, load and interact with the levels using a command-line interface. The code has many security issues which you must fix for your COMP280 security assignment.

## Building the project

In Visual Studio:
  1. file
  2. import cmake project
  3. find CMakeLists.txt in the root of this project

On CLI (Linux):

Assuming you have cmake and g++ installed:

```bash
cmake -S . -B build
cmake --build build
./build/comp280-a2-cpp/comp280-a2-cpp
```

## Running the environment

| Command | Effect                   |
| ------- | ------------------------ |
| ping    | Repond with pong         |
| quit    | exit the application     |
| new     | start a new 'game'       |
| load    | load a saved session     |
| save    | save the current session |

## Gameplay commands

| Command            | Effect                        |
| -------            | ----------------------------- |
| links              | Show the current destinations |
| go <location>      | Move to a new location        |
| inv                | see the current inventory     |
| shop               | see the current shop contents |
| buy <itemid> <qty> | buy from the shop             |
