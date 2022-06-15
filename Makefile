VENDORNAME=returnnull
PROJECTNAME=pong
CONTAINERNAME=dkp_compiler
USER=$(shell whoami)

.PHONY: build_image
build_image: clean_docker
	DOCKER_BUILDKIT=1 docker build -f docker/$(CONTAINERNAME)/Dockerfile . \
	-t $(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev --build-arg uid=1000 --build-arg user=$(USER)

.PHONY: run
run: compile
	mgba-qt -4 out/game.gba

.PHONY: compile
compile: clean_files 
	docker run \
		-v ${PWD}/code:/${USER} \
		-v ${PWD}/out:/out \
		$(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev

.PHONY: getincludes
getincludes: 
	-@docker run \
		--cidfile=c.cid \
		$(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev \
		"echo" "getting c.cid"
	docker cp $$(cat c.cid):/opt/devkitpro/libgba/include $$(pwd)/code/
	docker cp $$(cat c.cid):/opt/devkitpro/libtonc/include $$(pwd)/code/
	-@rm c.cid

# example "make grit_gB16 img=img/pong_tc.png"
.PHONY: grit_gB16
grit_gB16: 
	make grit img=$(img) args="-ftc -gb -gB16"

# example "make grit img=img/pong_tc.png args=gB8"
.PHONY: grit
grit:  
	docker run \
		-v ${PWD}/code:/${USER} \
		-v ${PWD}/out:/out \
		$(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev \
		"/opt/devkitpro/tools/bin/grit" $(img) $(args) "-o$(img)"

.PHONY: clean_all_files
clean_all_files: clean_files
	rm -rf $$(pwd)/code/build

.PHONY: clean_files
clean_files:
	rm -f $$(pwd)/out/*.elf
	rm -f $$(pwd)/out/*.gba
	rm -f $$(pwd)/out/*.sav

.PHONE: clean_docker
clean_docker:
	docker image rm -f $(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev
	
.PHONY: deleteincludes
deleteincludes:
	rm $$(pwd)/code/include/*.h
