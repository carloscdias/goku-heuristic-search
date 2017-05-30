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
 * `-e mode`, sets the exploration mode. Each exploration mode may contain suboptions

### Explore modes

 * `min_cost_explore_rate`, explora o mapa posição à posição, decidindo a próxima posição de acordo com a menor taxa `custo_do_caminho/numero_regioes_exploradas`
 * `path`, segue um caminho pré-determinado de posições que precisam ser visitadas. Esta posição aceita uma subopção `-p file` ou `--path file` para o nome de um arquivo binário contendo as posições que serão utilizadas para o algoritmo. Por padrão, as posições são lidas do arquivo `path.bin`.

