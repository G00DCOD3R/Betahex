# Betahex
ai playing hex
:)

compile with: 
```
g++ -O3 a.cpp game.cpp player.cpp -o a
```
to play with ai you should: 
in game.cpp comment some lines in ```__init__()```
there is a brief description what to do

in a.cpp comment/uncomment elses in main()

train.cpp is for training ai, to compile
```
g++ -O3 train.cpp game.cpp player.cpp -o t
```
basically game.cpp and player.cpp are libraries for managing game and choosing best move (compile as extra library)

decent strat (which is set as a default): 
```(91 91 310 3.14)```
