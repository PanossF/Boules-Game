# Boules-Game
A boules game which is implemented on C++ and some simple OpenGL commands. Although the game does not have good visuals it is created from scratch. Its code shows the implementation of different forces such as movement of the balls, applying friction on them and interaction between them.

The game is a simple Boules game in which 2 people are playing in the same court who are having 4 balls each. The goal of this game is for each player to reach their balls as close as possible to the main ball which is the jack, located alone approximately at the end of the court. The total score is calculated by the difference of the distance of each ball and the jack. The winner is who has the closest balls in distance next to jack ball. Results are printed in the console window.

The graphics of the game are simple but the code is complex, so this application indicate the complexity of the code, the advanced usage of maths and physics.


To run the game:
1. open a cmd window and execute ThreadedServer.exe with a parameter of a random port (for example 5555)
2. it will open 2 connections because I set it to with these settings!
3. open 2 new cmd windows and run the BoulesGame.exe with the keyword "localhost" and next to it the port numbers that Server opened( in this case one windows will have 5555 and the other one 5556)
4. write 2 random names and then press any key and enter.

Then the game runs for each client in the server.

To play the game:
1. Pressing the arrow keys, the direction of the ball changes
2. Press enter to hit the ball
3. Once all balls are released, then press enter to get the results
4. The results are displayed in the cmd window. Messages are sent also to the server.
