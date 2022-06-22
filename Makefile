VENDORNAME=returnnull
PROJECTNAME=pong
CONTAINERNAME=dkp_compiler
USER=$(shell whoami)

# build the docker image
.PHONY: build_image
build_image: clean_docker
	DOCKER_BUILDKIT=1 docker build -f docker/$(CONTAINERNAME)/Dockerfile . \
	-t $(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev --build-arg uid=1000 --build-arg user=$(USER)

# compile the .gba file and run in in your emulator
.PHONY: run
run: compile
	mgba-qt -4 $$(pwd)/out/game.gba
#	NanoBoyAdvance $$(pwd)/out/game.gba

# compiles the code into a .gba files, found in the /out folder
.PHONY: compile
compile: clean_files 
	docker run \
		-v $$(pwd)/code:/${USER} \
		-v $$(pwd)/out:/out \
		$(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev

# gets include files from libgba and libtonc to the include folder
.PHONY: getincludes
getincludes: 
	-@docker run \
		--cidfile=c.cid \
		$(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev \
		"echo" "getting c.cid"
	docker cp $$(cat c.cid):/opt/devkitpro/libgba/include $$(pwd)/code/
	docker cp $$(cat c.cid):/opt/devkitpro/libtonc/include $$(pwd)/code/
	-@rm c.cid

# dont overwrite existing files
.PHONY: grit_all
grit_all: 
	for file in code/img/*.png; \
	   do test -f $${file%.*}.h || \
	   make grit img=$${file#*/} args="-ftc -gb -gB16"; \
	done

# overwrite existing .c and .h files
.PHONY: grit_all_force
grit_all_force: 	
	for file in code/img/*.png; \
	    do make grit img=$${file#*/} args="-ftc -gb -gB16"; \
	done

# example "make grit_gB16 img=img/pong_tc.png"
.PHONY: grit_gB16
grit_gB16: 
	make grit img=$(img) args="-ftc -gb -gB16"

# example "make grit img=img/pong_tc.png args="-ftc -gb -gB16""
.PHONY: grit
grit:  
	docker run \
		-v $$(pwd)/code:/${USER} \
		-v $$(pwd)/out:/out \
		$(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev \
		"/opt/devkitpro/tools/bin/grit" $(img) $(args) "-o$(img)"

# removes files from the out folder and the files from the build folder
.PHONY: clean_all_files
clean_all_files: clean_files
	rm -rf $$(pwd)/code/build

# removes the files from the out folder
.PHONY: clean_files
clean_files:
	rm -f $$(pwd)/out/*.elf
	rm -f $$(pwd)/out/*.gba
#	rm -f $$(pwd)/out/*.sav

# deletes the docker images
.PHONY: clean_docker
clean_docker:
	docker image rm -f $(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev

# delete all h files from the include folder (from devkitarm)	
.PHONY: deleteincludes
deleteincludes:
	rm $$(pwd)/code/include/*.h
