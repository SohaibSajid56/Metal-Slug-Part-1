# 🎮 Metal Slug (C++ - Procedural Version)

This is the **first version** of my custom-made **Metal Slug** game clone, built entirely using **C++** and the **raylib** graphics library — without using Object-Oriented Programming.  
It demonstrates the use of **core programming fundamentals (PF concepts)** like loops, conditionals, arrays, and functions to build a working 2D action game.

---

## 🧩 Project Overview

This version of the game focuses on implementing **game logic procedurally** — everything from player movement and enemy spawning to shooting mechanics and collision detection is handled using **functions and global variables** instead of classes or objects.

The goal of this phase was to:
- Understand how to structure a game loop.  
- Learn how rendering, input handling, and updates work together.  
- Build a playable and enjoyable 2D game using only PF concepts.  

---

## ⚙️ Features

- **Player Movement**
  - Move left and right using arrow keys.
  - Jump using the spacebar.

- **Shooting Mechanism**
  - Fire bullets using a single key (e.g., `Z`).
  - Bullet direction and speed controlled via simple vectors.

- **Enemies**
  - Basic enemy spawning and movement logic.
  - Enemies get destroyed when hit by player bullets.

- **Collision Detection**
  - Simple bounding-box (rectangular) collision checks between bullets, player, and enemies.

- **Menu and Difficulty Selection**
  - Basic main menu using `DrawText()`.
  - User can select difficulty level: Easy, Normal, or Hard (changes enemy speed/spawn rate).

---

## 🕹️ Controls

| Key | Action |
|-----|---------|
| `←` / `→` | Move left or right |
| `Space` | Jump |
| `Z` | Shoot |
| `Enter` | Start / Select |
| `Esc` | Exit the game |

---

## 🧱 Technical Details

- **Language:** C++  
- **Graphics Library:** [raylib](https://www.raylib.com/)  
- **Paradigm:** Procedural (No Classes or OOP)  
- **Core Concepts Used:**  
  - Functions and modular code  
  - Structs for grouping related data  
  - Arrays and loops for handling enemies and bullets  
  - Conditional logic for collision and game states  

---

## 📁 Project Structure

