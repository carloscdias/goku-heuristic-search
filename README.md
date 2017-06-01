# Goku Heuristic Search

Explore a 2D unknown environment and retrieve the seven dragonballs with the least cost as possible.

[Link to assingment (in portuguese)](https://github.com/carloscdias/goku-heuristic-search/blob/master/images/assignment.pdf)

![Running program](https://raw.githubusercontent.com/carloscdias/goku-heuristic-search/master/images/example.png)

## Controls

 * `s` turn on/off search
 * `g` turn on/off grid view on the map
 * `r` turn on/off radar view on the map
 * `d` turn on/off unknown dragonballs view on the map
 * `i` turn on/off information text view on the map
 * `<space>` restart everything
 * `<up_arrow><down_arrow><left_arrow><right_arrow>` moves goku to up, down, left and right respectively

## Search Algorithm

 * if there isn't any visible dragonball, explore map
 * if there is one visible dragonball, get it through the path with min cost
 * if there are more than one dragonball, calculate the best path to retrieve all of them
 * if got all dragonballs, return to the master Roshi's island

## Program options

 * `-m file` or `--map file`, loads a definition map
 * `-d x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7` or `--dragonballs x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7`, sets the dragonballs position
 * `-r` or `--rate` sets the exploration mode to `min cost explore rate`
 * `-p pathfile` or `--path pathfile` sets the exploration mode to path and expects the given file to be a binary file of positions2d_t to be

