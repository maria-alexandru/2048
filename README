# 2048

- Name: Alexandru Maria-Mihaela
- Group: 311CC

## How to play

1. **Start screen:**
	- The main menu will show up and you can navigate through the options using
the UP arrow key and the DOWN arrow key. Press ENTER to select an option.
	- If you select the 'New Game' option, a new game will start, resetting the
score and the playing time to 0.
	- The 'Resume' option will take you back to the last game, if it exists. If
a game was not previously started, you can not select this option.
	- If you choose the 'Top Scores' option, you will be able to see the best
eight scores, the time in which each was achieved, the name of the player who
achieved it and if it was a win or a loss.
	- If you enter the settings submenu, you will be able to modify the
appearance of the game by choosing the desired theme, you can configure the
automatic move option or you can return to the main menu.
	- If you select the 'Quit' option, the game will immediately close.

2. **Gameplay**
	- When a new game is started, the player can move all the tiles to the left
by pressing the LEFT arrow key, to the right by pressing the RIGHT arrow key, up by
pressing the UP arrow key or down by pressing the DOWN arrow key.
	- If two adjacent tiles have the same value and are moved in the same
direction, they merge into a single tile which has the value 2 times higher than
the initial one.
	- By pressing U, the player can undo the last move. This option can undo
only one move, not multiples moves.
	- If the automatic move option is on and the player is inactive for a
certain period of time, the game will choose the move that empties the maximum
number of tiles and execute that move.
	- On the left side of the screen the player can see the current date and
time, the score and how much time has passed since the start of the game.
Information about the highest scorest is displayed below. The player can also
take a look at the valid commands, which are shown on this info panel.
	- If the player reach a 2048 value, it is a win and a corresponding message
is displayed.
	- If the game board is full and there are no more available moves, it is a
loss and a corresponding message is displayed.

3. **New high/top score**
	- When a game ends, if the score is a top score or a high score, a message
will appear on the screen and the player will have to enter their name in order
to save it.
	- A score is considered a top score if it is in the top eight scores or if it
is equal to one of them and has a shorter time.
	- A score is considered a high score if it is the highest score reached from
the first time the app was opened or if it is equal to the last high score and
was achieved in a shorter time.

4. **Theme color menu**
	- The player has five options to choose from for the appearance of the game.
	- You can navigate through the themes by pressing the UP/DOWN arrow and you
can select one by pressing ENTER.

5. **Automatic move option**
	- The automatic move option can be turned on/off from its settings page.
	- If the option is on, the player can set the desired number of seconds of
inactivity after which the game will move automatically.

## Implementation
1. **First task**
	- For the first task, a structure was created to save the options of the
menu as well as the the number of available options and using the draw_menu
function the menu was displayed.
	- Besides the three mandatory options specified in the task, two more
options were added: 'Top Scores' and 'Settings'.
	- When a game is started, all the information about it is stored in a
struct variable so if 'Q' is pressed, the menu appears again but the game
information is not deleted. In this way, when the player wants to resume the
game, everything will be as it was before exiting.
	- If the player wants to start a new game, all the information about the
last game will be reset.

2. **Second task**
	- To create the game layout, a function which draws a rectangle was used,
for each tile and then for the entire layout.
	- Each value from a tile has a different color. These are stored using the
init_color and init_pair ncurses functions, with ids from 20 to 30. The function
fill_rectangle is used to draw multiple lines inside the rectangle to color it.

3. **Third task**
	- The four moves were implemented using four functions, one for each
direction.
	- A new value is generated randomly, by choosing a tile from the empty 
tiles.

4. **Fourth task**
	- To find out in which direction the game should move to empty the maximum
number of tiles, every move is executed on a copy of the game matrix and then
the best move is chosen.
	- To determine the seconds of inactivity, a difference is made between the
last time when a key was pressed and the current time.

5. **Fifth task**
	- The game status is constantly checked. If 2048 is reached, it means the
player won. The game is over when the auto_move function returns 0 because it
means that no more valid moves are available.

6. **Bonus tasks**
	- **Game appearance**
		- Colors for each theme are loaded from colors.txt file each time when
the app is started and they are memorized in a struct variable but also using
init_color and init_pair ncurses functions, with ids starting from 31.
		- A new theme is applied by changing the color pairs which are always
used (ids from 20 to 30) with the color pairs of the selected theme.

	- **Continue the game after closing the app**
		- When the app is opened for the first time, a file called 'game.txt' is
created. In this file are saved information about the selected theme, the
highscore, the game matrix, etc.
		- After each move and especially at the end of a game or before the
app is closed, these information are updated in the file.
		- Another file that is created when the app is used for the first time
is 'scores.txt' where are stored the best 8 scores.
		- If the two files already exists, their content will be loaded so that
the player can continue the game as it was before exiting, with the same
settings.

	- **Undo button**
		- Before any move is made, the matrix of the game is saved in another
variable in order to restore it if the player wants to undo a move.

	- **Configure automatic move**
		- The seconds of inactivity are stored in a variable which can be
changed by the player. It has a value from 1 to 99 if the option is on and -1
if the option is turned off.