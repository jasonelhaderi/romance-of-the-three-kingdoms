# Romance of the Three Kingdoms

_Romance of the Three Kingdoms_ is a variant of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) (CGoL) in which several key modifications have been made:
* The criterion for generating a live cell from a dead cell has been relaxed to encourage rapid growth.
* Three species of cell are present, each characterized by a unique appetite for growth and an inversely related sensitivity to overpopulation.
* Cells of different types are mutually toxic.

The work takes its name from the eponymous Chinese historical novel since it invokes notions of both trinity and chronicle. It was designed to be rendered with [Pixku](https://github.com/MuvikLabs/Pixku) and [ImageMagick](https://www.imagemagick.org/script/index.php).

## Description of the work
The genesis of _Romance of the Three Kingdoms_ lies in the synthesis of an interest in cellular automata with the technical considerations of Pixku.

### Cellular automata
Images structured on the principle of a grid are strongly implied by Pixku's fundamental graphical element — the pixel. This raises the question, "what generation procedures lead to _interesting_ grids?" CGoL and variants thereof, called _cellular automata_, constitute natural choices. Comparing the complexity of the resulting structures relative to simplicity of the corresponding evolution algorithm, it's tough to get a better bang for your buck. In fact, CGoL in its original form is described an evolution algorithm weighing in at a measly four lines.

The key concepts underpinning all cellular automata are as follows; a _state_ is initialized consisting of a grid of _cells_ marked dead or alive. New states are then generated iteratively. The conditions in each individual cell's immediate neighborhood are used as input to update the cell's state according to some fixed _rule_. This rule generally has the following structure:
* If a live cell is surrounded by too few or too many other live cells, it dies. Otherwise, it survives.
* If a dead cell satisfies some reproduction requirement, it comes to life (CGoL's requirement is that number of neighboring live cells be _precisely_ three).

Variation of the initial state (or evolution rule) leads to variation in the emergent structures.

### Technical considerations
It is at this point that Pixku's other technical constraints begin to come into play. Rendering CGoL is a context in which the range of Pixku's color palette could be called generous; for drawing cellular automata, eight colors is simply overkill. This surplus of color sparked the idea of introducing _species_ of cell, all mutually hostile. A palette of eight colors would allow up to seven different kinds of competitor to be drawn with the eighth color being reserved to denote dead cells. The decision to simulate only three species stems from the desire to show the recent _history_ of the cells as well. Each individual frame (or still) in _Romance of the Three Kingdoms_ is a chronicle unto itself:
* Bright red, green, and blue depict different cell species which are alive in the current state.
* Faded colors are used to represent cells of the corresponding species that were alive one state ago, but are currently dead.
* Dark gray is used to color dead areas which had _any_ species of live cell two states ago.
* Black is used for dead regions which have been dead longer than that.

### Rules of the game
Each of the three species evolves according to the same kind of modified CGoL algorithm, albeit with individual twists. All cells are mutually toxic — if a live cell is surrounded by more cells of a different species than cells of the same species, it dies. Further, the reproduction requirement has been loosened to encourage rapid cell growth (the number of live neighbors must be _greater than_ a magic number, rather than CGoL's requirement of exact equality). The green species has been designed to reproduce faster than the red species, which in turn reproduces faster than the blue species. Additionally, each strain of cell has a unique threshold for overpopulation. Green is the most sensitive and blue is the least, while red occupies a sensitivity middle-ground.

After some experimentation, the choice was made to initialize the cell strains concentrically at the center of a 256x256 grid via Gaussian distribution. Periodic boundary conditions are employed in evaluating neighbors for edge cells.

### Choice of presentation format
Evolutionary processes are inherently time-based; thus, they naturally lend themselves to depiction as movies. For this reason, the preferred presentation format of _Romance of the Three Kingdoms_ is in GIF. However, as a means of more quickly exploring the consequences of specific parameter choices, code has also been provided to rapidly produce _exclusively_ a single frame of output in PNG format. Sample output in both formats are included, `out.gif` and `out.png`.

## How to run the code
Two C files, `3k_gif.c` and `3k_still_life.c` are provided to generate GIF and and single PNG versions of _Romance of the Three Kingdoms_, respectively. Outside of structural differences optimizing production of many frames vs. quickly reaching a given final frame, they are identical. They can be copied into an installation of Pixku along with the corresponding `Makefile`, which consists of only nondestructive additions to the default `Makefile` packaged with Pixku.

### Compilation
To produce the images, simply place `3k_gif.c` and `3k_still_life.c` in your Pixku install directory along with the accompanying `Makefile`. The single PNG version can be made as follows:
```
make 3k_still NFRAMES=100
```
`NFRAMES` specifies how many grid states to iterate through before generating the file `./out.png`. For the accompanying example, `NFRAMES=1850` was used.
If `NFRAMES` isn't specified, it takes the default value 100.


Compilation of the GIF version using the makefile requires ImageMagick. To make the GIF, run:
```
make 3k_movie NFRAMES=100
```
The primary output is the file `./out.gif`. For the accompanying example, `NFRAMES=2100` was used. The secondary RNT and PNG files are written to the directories `./raw_rnt/` and `./imgs/`, which get automatically created if they don't yet exist. To clean up all secondary output, run:
```
make clean_3k
```
NOTE: 1000+ RNT files take up quite a bit of disk space. It is recommended to clean up secondary output when making the GIF version with larger values of `NFRAMES`.

### Suggested points of entry
Here are some possible points of entry for experimentation with the source code.
* Modify the `GRID_SIZE` and `CELL_SIZE` constants.
* Offset the centers of each strain's initialization.
* Try different standard deviations in the x and y directions.
* Modify the initial total number of live cells.
* Alter the virulence and overpopulation sensitivity of each strain (this requires mucking about in the `update_cells_3k()` function).
