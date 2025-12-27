## Tic Tac Toe in CLI

A Tic Tac Toe clone using the library `ncurses`.
The game uses object-oriented programming techniques for its implementation.

### How to Run
To run this game, you must compile the source code and link the `ncurses` library.

**1. Install Ncurses**

* **Debian/Ubuntu:** `sudo apt-get install libncurses5-dev libncursesw5-dev`
* **MacOS:** Pre-installed or via Homebrew: `brew install ncurses`
* **Windows:** Requires WSL (Windows Subsystem for Linux) or MinGW with ncurses support.

**2. Compile**
Save the code as `game.cpp` and run:

```bash
g++ game.cpp -o game -lncurses

```

**3. Execute**

```bash
./game

```

---

### Controls

The game is designed with a "hybrid" interface, allowing keyboard commands and mouse interaction.

| Action | Control |
| --- | --- |
| **Place X or O** | **Left Click** on any grid square |
| **Quit Game** | Press **`q`** |
| **Restart Game** | Press **`r`** (Only available on Game Over screen) |

---

### About the code

The codebase is split into three logical components: the Game Engine, the Rendering Logic, and the Event Loop.

#### 1. The Engine Class

The `Engine` class encapsulates the game state and business logic, ensuring the `main` loop handles only input/output.

* **State Management:**
* Uses a `uint8_t table[3][3]` 2D array to represent the board.
* `0`: Empty, `1`: Player X, `2`: Player O.


* **Win/Tie Logic (`checkIfWin`):**
* Returns `0` if the game is ongoing.
* Returns `1` or `2` if a player has won.
* Returns `-10` to signal a **Tie**. This is handled as a signed integer return type to avoid overflow issues with unsigned types.


* **Input Validation:**
* `placeOnPosition()` checks bounds (0-2) and ensures the target cell is empty before mutating the state.



#### 2. Rendering (`drawBoard`)

The rendering function decouples visual updates from game logic. It redraws the entire screen every frame where an event occurs.

* **Grid System:** The visual board is drawn using ASCII characters (`|`, `---`).
* **Coordinate Mapping:** The function maps the internal `[0..2]` array indices to specific screen coordinates:
* **Y-Axis (Rows):** Screen lines 3, 5, and 7.
* **X-Axis (Cols):** Screen columns 2, 6, and 10.



#### 3. The Main Event Loop

The `main()` function manages the `ncurses` lifecycle and input handling.

* **Initialization:** Sets up `stdscr`, disables line buffering (`cbreak`), hides the cursor, and enables keypad/mouse masking.
* **Mouse Handling:**
* Captures `KEY_MOUSE` events.
* Uses **Hit Box Logic** to translate click coordinates into grid indices.
* *Example:* A click on screen Y lines 3 or 4 maps to Row 0.


* **Turn Management:** Toggles the `player` variable (1 or 2) only upon a successful move validation from the Engine.

---

### Troubleshooting

**Q: Clicking the grid does nothing.**

* **Linux/Mac:** Ensure your terminal emulator supports mouse reporting (be sure, Gnome Terminal, iTerm2, Alacritty).
* **Windows:** If using CMD or PowerShell, right-click the title bar, go to **Properties**, and uncheck **"QuickEdit Mode"**. This mode often hijacks mouse clicks.

**Q: The screen is blank.**

* The game waits for input (`getch`) before refreshing in some states. Press any key or move the mouse to force a refresh.

**Q: Compilation fails with "undefined reference".**

* Ensure you included the flag `-lncurses` at the **end** of your compilation command.
