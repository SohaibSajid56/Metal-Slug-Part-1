# 🎮 Metal Slug (C++ with Raylib - Procedural Version)

This is the **first version** of my custom-made **Metal Slug** game clone, built entirely using **C++** and the **raylib** graphics library — without using any Object-Oriented Programming.  
It demonstrates how a complete 2D shooting game can be made using only **Programming Fundamentals (PF concepts)** such as loops, functions, structs, and arrays.

---

## 🧩 Overview

In this version, everything — from player controls and enemy spawning to shooting and collision detection — is implemented using **procedural logic**.  
There are **no classes or objects**; instead, the game is built through functions, global variables, and simple data structures.

The goal of this stage was to:
- Build a playable Metal Slug-style game from scratch using raylib.  
- Learn how to design a **game loop**, handle **input**, and draw **sprites** on screen.  
- Understand how each subsystem (player, enemies, bullets) interacts before applying OOP.

---

## ⚙️ Features

- 🧍 **Player Controls**
  - Move left and right with arrow keys.
  - Jump with spacebar.
  - Shoot bullets using `Z`.

- 💥 **Enemy System**
  - Enemies spawn automatically at intervals.
  - Enemies move toward the player.
  - When hit by bullets, enemies are destroyed.

- 🔫 **Shooting Mechanics**
  - Bullets fired by the player move across the screen.
  - Collision detection with enemies using simple bounding boxes.

- 🧮 **Collision Detection**
  - Uses rectangular overlap checks between bullets and enemies.

- 🕹️ **Game Menu**
  - Basic text-based main menu built with `DrawText()`.
  - Includes difficulty selection (Easy, Normal, Hard).

---

## 🕹️ Controls

| Key | Action |
|-----|---------|
| `←` / `→` | Move left / right |
| `Space` | Jump |
| `Z` | Shoot |
| `Enter` | Start game |
| `Esc` | Exit game |

---

## 🧱 Technical Details

- **Language:** C++  
- **Library:** [raylib](https://www.raylib.com/)  
- **Paradigm:** Procedural Programming (no classes or OOP)  
- **Key Concepts Used:**  
  - Functions and modular design  
  - Structs for storing player/enemy data  
  - Loops and conditionals for gameplay logic  
  - Arrays for handling multiple enemies and bullets  
  - raylib functions for drawing sprites, handling input, and managing time  

---

## 📁 Project Structure

