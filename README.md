# 2048
<p>
	<img src="https://github.com/maria-alexandru/2048/assets/149622435/c5c46e14-5909-4ef1-828d-257a205222d0" alt="2048" width="500">
</p>

- Name: Alexandru Maria-Mihaela
- The terminal window should be in its maximized state so that all the options
in the game are visible.

## How to play

1. **Start screen:**
	- Firstly the 2048 logo will appear. Press ENTER to continue.
	- The main menu will show up and you can navigate through the options using
the UP arrow key and the DOWN arrow key. Press ENTER to select an option.
	- <p> <img src="https://github.com/maria-alexandru/2048/assets/149622435/9a78ee3d-28d5-4fe7-a356-08e3bb7cd8c8" alt="2048" width="500" class="center"> </p>
	- The first option is the game mode. You can change it by pressing the RIGHT
arrow key or the LEFT arrow key. The existing modes are Small (3x3), Classic
(4x4) and Big (5x5). The 'New Game', 'Resume' and 'Top Scores' options refer to
the displayed mode.
	- If you select the 'New Game' option, a new game will start, resetting the
score and the playing time to 0.
	- The 'Resume' option will take you back to the last game, if it exists. If
a game was not previously started, you can not select this option.
	- If you choose the 'Top Scores' option, you will be able to see the best
eight scores, the time in which each was achieved, the name of the player who
achieved it and if it was a win or a loss.
	- If you enter the settings submenu, you will be able to modify the
appearance of the game by choosing the desired theme, you can configure the
automatic move option or you can return to the main menu. These will be the same
for all game modes.
	- If you select the 'Quit' option, the game will immediately close.

2. **Gameplay**
	- <p> <img src="https://github.com/maria-alexandru/2048/assets/149622435/320dd140-6ed5-400f-a820-0e01be752b8c" alt="2048" width="500" class="center"> </p>
 	-  When a new game is started, the player can move all the tiles to the left
by pressing the LEFT arrow key, to the right by pressing the RIGHT arrow key, up
by pressing the UP arrow key or down by pressing the DOWN arrow key.
	- If two adjacent tiles have the same value and are moved in the same
direction, they merge into a single tile which has the value 2 times higher than
the initial one.
	- By pressing 'U', the player can undo the last move. This option can undo
only one move, not multiples moves.
	- If the automatic move option is on and the player is inactive for a
certain period of time, the game will choose the move that empties the maximum
number of tiles and execute that move.
	- On the left side of the screen the player can see the current date and
time, the score and how much time has passed since the start of the game.
Information about the highest score is displayed below. The player can also
take a look at the valid commands, which are shown on this info panel.
	- If the player reach a 2048 value, it is a win and a corresponding message
is displayed.
	- If the game board is full and there are no more available moves, it is a
loss and a corresponding message is displayed.

3. **New high/top score**
	- When a game ends, if the score is a top score or a high score, a message
will appear on the screen and the player will have to enter their name in order
to save it.
	- <p> <img src="https://github.com/maria-alexandru/2048/assets/149622435/3e5c7f0c-88a2-4728-821d-6616af97ce18" alt="2048" width="500" class="center"> </p>
	- A score is considered a top score if it is in the top eight scores or if 
it is equal to one of them and has a shorter time.
	- A score is considered a high score if it is the highest score reached from
the first time the app was opened or if it is equal to the last high score and
was achieved in a shorter time.
	- Each game mode has different high/top scores.
 	- <p> <img src="https://github.com/maria-alexandru/2048/assets/149622435/8038727b-db4d-4f2c-90db-88c583e23299" alt="2048" width="500" class="center"> </p>

4. **Theme color menu**
	- The player has five options to choose from for the appearance of the game.
	- You can navigate through the themes by pressing the UP/DOWN arrow and you
can select one by pressing ENTER.
	- <p> <img src="https://github.com/maria-alexandru/2048/assets/149622435/dc6cc449-aae9-44fc-9ea3-9e7fe6119ec8" alt="2048" width="500" class="center"> </p>


5. **Automatic move option**
	- The automatic move option can be turned on/off from its settings page.
	- If the option is on, the player can set the desired number of seconds of
inactivity after which the game will move automatically.
	- <p> <img src="https://github.com/maria-alexandru/2048/assets/149622435/98250b98-679a-4c35-ba85-b6045ca6d217" alt="2048" width="500" class="center"> </p>


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
	- **Game modes**
		- Information for each game mode is stored in an array of structures.
When the game mode is modified, the data about the current game will be copied
from the array.
		- At the beginning of the game the selected game mode is initialized.
When the game ends, the array that contains information about each game mode is
updated.

	- **Game appearance**
		- Colors for each theme are loaded from 'colors.txt' file each time when
the app is started and they are memorized in a struct variable but also using
init_color and init_pair ncurses functions, with ids starting from 31.
		- A new theme is applied by changing the color pairs which are always
used (ids from 20 to 30) with the color pairs of the selected theme.

	- **Resume the game after closing the app**
		- When the app is opened for the first time, a file called 'game.txt' is
created. In this file are saved information about the selected theme, the
highscore, the game matrix, etc. for each game mode.
		- After each move and especially at the end of a game or before the
app is closed, these information are updated in the file.
		- Another file that is created when the app is used for the first time
is 'scores.txt' where are stored the best 8 scores for each game mode.
		- If the two files already exists, their content will be loaded so that
the player can continue the game as it was before exiting, with the same
settings.

	- **Undo button**
		- Before any move is made, the matrix of the game is saved in another
variable in order to restore it if the player wants to undo a move.

	- **Configure automatic move**
		- The seconds of inactivity are stored in a variable which can be
changed by the player. It has a value from 1 to 99 if the option is on and -1
if the option is turned off. It is initially set to five seconds.
