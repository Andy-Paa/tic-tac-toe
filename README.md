TicTacToe

2 players

play by mouse click

state board example : 021020010

123

456

789

0 stands for empty
1/2 stands for player


negamax: 

If the current node is a dead end, return index * evaluation.

Traverse all possible moves:

Simulate moves

Recursively call -negamax(…, -index);

Take the maximum value as the score for the current node.

Using the index (+1 / -1) and the negative sign, we transform "maximizing the opponent by switching" into "maximizing ourselves."
