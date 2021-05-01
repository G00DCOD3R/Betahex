# Betahex
ai playing hex
:)

Now it plays really bad, it's mainly because of time_per_move = 5.0
this variable determines how long one move will take
Preferably change it to at least 20.0 (it is in seconds), otherwise it makes just "good" random moves

compile with: 
```
g++ -O3 a.cpp game.cpp player.cpp -o a
```
**Currently Betahex plays with itself**

To change that you should: 
1. in a.cpp change variable chosen_mode (1 or 0)

**For testing / training purposes use train.cpp**


compilation:
```
g++ -O3 train.cpp game.cpp player.cpp -o t
```
basically game.cpp and player.cpp are libraries for managing game and choosing best move (compile as extra library)

decent strat (which is set as a default in a.cpp): 
```(91 91 310 3.14)```



**Todo**
- provide more game analysis (such as other good moves, and their win%)
- board hash
- board evaluation (as an extra parameter to the position value), now it's not that good 
