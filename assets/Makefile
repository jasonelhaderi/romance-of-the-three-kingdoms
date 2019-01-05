OBJ+=$(addprefix runt/, runt.o irunt.o basic.o)
OBJ+=lodepng/lodepng.o
OBJ += parse.o
NFRAMES = 100
RATE = 2
CFLAGS += -Irunt -Ilodepng -std=c99 -Wall -pedantic

default: pixku testimage

pixku: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# for making a single three_kingdoms PNG at the NFRAMESth generation
3k_still: pixku 3k_still_life
	./3k_still_life $(NFRAMES)
	./pixku run.rnt

# for making a three_kingdoms GIF
./imgs/:
	mkdir ./imgs

./raw_rnt/:
	mkdir ./raw_rnt

3k_movie: 3k_gif ./raw_rnt/ ./imgs/
	./3k_gif $(NFRAMES)
	for f in ./raw_rnt/*.rnt; do ./pixku $$f; done
	convert -delay $(RATE) -loop 0 ./imgs/*.png ./out.gif

clean_3k:
	rm ./imgs/* ./raw_rnt/*

clean:
	$(RM) pixku $(OBJ) testimage $(OBJ) 3k_still_life 3k_gif
