# Goku Heuristic Search

Search for the best path to retrieve the dragonballs...

![Running program](https://raw.githubusercontent.com/carloscdias/goku-heuristic-search/master/images/example.png)

Algoritmo de busca:

 * caso não tenha visto nenhuma esfera, explorar o mapa.
 * caso tenha visto uma esfera, calcular o melhor caminho para chegar até a esfera.
 * caso tenha visto mais de uma esfera, calcular o melhor caminho para pegar todas as esferas visíveis
 * caso tenha pego todas as esferas, retornar para a ilha do mestre kame pelo melhor caminho possível

Opções do programa:

 * `-m file` ou `--map file`, carrega um arquivo de definição de mapa.
 * `-d x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7` ou `--dragonballs x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7`, define a posição das esferas do dragão para a partida.
 * `-e mode`, define o modo de exploração. Cada modo de exploração poderá conter subopções de acordo com o modo.

Modos de exploração:

 * `min_cost_explore_rate`, explora o mapa posição à posição, decidindo a próxima posição de acordo com a menor taxa `custo_do_caminho/numero_regioes_exploradas`
 * `path`, segue um caminho pré-determinado de posições que precisam ser visitadas. Esta posição aceita uma subopção `-p file` ou `--path file` para o nome de um arquivo binário contendo as posições que serão utilizadas para o algoritmo. Por padrão, as posições são lidas do arquivo `path.bin`.

