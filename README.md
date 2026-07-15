# so_long

A 2D top-down game built in C using the MiniLibX (MLX) graphics library.  

The goal is to guide a player character to collect all mushrooms (collectibles) and reach the exit while navigating a map surrounded by walls, with efficient resource management and zero memory leaks.

> Disclaimer: I have a version here inside  **`special_version`** folder that would never be approved by moulinette as it involves forbidden functions and closing the window and opening (MLX limitations made it this way), but i wanted to experiment and take the project further and create an intro screen!
---

<p align="center">
  <img src="https://raw.githubusercontent.com/tdanielsousa/tdanielsousa/main/gifs/so_long.gif" alt="so_long gameplay preview" width="600px" />
</p>

##  Game Architecture & Design

This project implements a complete game loop backed by structured state management.  
The entire status of the application is maintained within a single unified structure: `t_game`.

### Technical Specifications
* **Graphic Engine:** MiniLibX (configured for Linux via X11 Server).
* **Asset Resolution:** `64x64` pixel sprite matrices (`SIZE 64`).
* **Controls:** Desktop inputs managed via event hooking (hooked using X11 keycodes like `KeyRelease` and `DestroyNotify` for clean window destruction).

<div align="center">
    <br>
    <a href="https://github.com/tdanielsousa/42_School/blob/main/Subjects/solong_subject.pdf" target="_blank">
        <img src="https://img.shields.io/badge/View%20Subject-PDF-red?style=for-the-badge&logo=adobe-acrobat-reader&logoColor=white" alt="Subject PDF" height="40">
    </a>
    <p><i>Click on the button above to see the subject's pdf.</i></p>

</div>

---

##  Map Parsing & Validation Protocol

Before any graphics initialize, the program performs a series of dry runs to check the structure of the input `.ber` map.  
If any validation fails, the program prints a descriptive message beginning with `Error` and terminates cleanly.

### Phase 1: Boundary Verification
* **Extension Check (`is_map_extension_ok`):** Validates that the file name strictly terminates in `.ber`.
* **Geometry Test (`is_map_rectangle`):** Parses rows using `get_map_row_length` to verify the map forms a perfect rectangle.
* **Closed Walls Check (`is_firstlast_wall_ok` / `is_side_walls_ok`):** Verifies that the top, bottom, left, and right outer perimeters are composed entirely of `1` characters (Walls).

### Phase 2: Entity Auditing
* **Valid Characters (`is_there_only_allowed_chars`):** Only recognizes `1` (Wall), `0` (Floor), `C` (Collectible), `P` (Player starting position), and `E` (Exit).
* **Rule Verification (`is_there_exit_and_player`):** Guarantees the map has exactly 1 Player starting point (`P`), exactly 1 Exit (`E`), and at least 1 Collectible (`C`).

### Phase 3: Pathfinding Validation (Flood Fill)
Even if all entities are present, the map could have blocked pathways.   
The validation copies the map structure and runs a recursive **Flood Fill** algorithm starting from the player's position:

    // Core validation concept
    flood_fill(char **map_copy, int y, int x, t_game *game)

The recursive algorithm walks through adjacent tiles (up, down, left, right) to test accessibility:
* It collects collectibles and exits dynamically within the replica grid.
* Finally, it checks if `count_collec_flood == total_collectibles` and `count_exit_flood == 1`. 
* If any path is obstructed, it detects it, blocks execution, and avoids starting an unplayable game.

---

##  Textures & Asset Pointers

The game loads `.xpm` image assets into memory using MLX pointers stored dynamically inside your game structure:

* **`wall`:** Border barriers blocking movement.
* **`floor`:** Background tiles supporting standard movement.
* **`mush`:** Mushrooms that must be consumed before escape.
* **`player`:** The active character entity.
* **`exit`:** The door target that unlocks once all collectibles are gathered.

---

## Controls & Input Handling

The keyboard events are handled through an interface that intercepts player actions (`process_player_input`):

* **`W` / `A` / `S` / `D` (or Arrow Keys):** Moves the player across the map grid.
* **`ESC` / Window Red Cross Click:** Gracefully exits the application, releasing all loaded textures, MLX windows, memory-mapped grids, and process allocations.

### Performance Metrics
A move counter prints the current total steps inside your command-line console interface upon every successful block offset, ensuring that movement is counted correctly according to project guidelines.

---

## Installation & Build Steps

### 1. System Prerequisites
Ensure you have standard X11 headers and development libraries installed on your Linux machine (required by MiniLibX):

    sudo apt-get install make clang libx11-dev libxext-dev

### 2. Compile the Game
Build the project executables using your root Makefile:

    make

### 3. Run a Map
To run the game, launch the executable along with a valid `.ber` map file:

    ./so_long maps/ok.ber

---

## Testing

To test you can use my custom made so_long tester, it tests all the edge cases:

* **[so_long_tester](https://github.com/tdanielsousa/so_long_tester)**

Instructions are available on the tester repo!

---
