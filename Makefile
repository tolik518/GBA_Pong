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

.PHONY: clean_all
clean_files: clean_files
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
