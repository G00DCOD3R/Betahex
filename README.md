# Betahex
ai playing hex
:)

compile with: 
```
g++ -O3 a.cpp game.cpp player.cpp -o a
```
**Currently Betahex plays with itself**

To change that you should: 
1. comment lines in ```__init__()``` function in game.cpp
2. comment lines in a.cpp in ``main()```

I plan to add more convience way of changing modes 

**For testing / training purposes use train.cpp**


compilation:
```
g++ -O3 train.cpp game.cpp player.cpp -o t
```
basically game.cpp and player.cpp are libraries for managing game and choosing best move (compile as extra library)

decent strat (which is set as a default in a.cpp): 
```(91 91 310 3.14)```
