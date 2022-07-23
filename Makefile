
all: bin/super_mario_mpi
	
	
bin/super_mario_mpi: build/main.o build/CoinElement.o build/HoleElement.o build/GoombaElement.o build/KoopaTroopaElement.o build/WorldElement.o build/World.o build/Mario.o build/GameController.o | bin/.
	mpic++ $^ -g -std=c++11 -o $@ 

build/%.o: src/%.cpp | build/.
	mpic++ -c -std=c++11 $^ -o $@

.PHONY: gitignore
gitignore:
	echo super_mario_mpi > .gitignore
	
.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

.PHONY: clean
clean:
	rm -rf bin build
