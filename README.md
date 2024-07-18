At the start of the game, ask the player to select a difficulty 
level and then randomly assign mines to cells (10 mines for easy, 40 for intermediate
, and 99 for advanced). Display the board, number of mines, and 
timer to the player. Take input from the user as r, c, m where r
is the row, c is the column, and m is either L or R for left or right click.
If the click is left, expose the cell; if right, flag the cell and decrease 
the total number of mines. A flagged cell cannot be exposed.
If a cell is a mine, expose all mine cells, stop the timer, and end the game.
If a cell shows a positive number, just display the number. If a cell shows zero, expose all its adjacent cells.
If one or more adjacent cells are also zero, expose their adjacent cells, and so on.
At the start, the timer is zero and increments every second. Display the updated game whenever the time
updates or the board changes. If a player wins, stop the timer and check if the finish time is one of the best.
If it is, input the user's name and update the top five player list for that difficulty level.
