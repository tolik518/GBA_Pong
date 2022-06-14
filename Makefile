VENDORNAME=returnnull
PROJECTNAME=pong
CONTAINERNAME=dkp_compiler
USER=$(shell whoami)

.PHONY: build_image
build_image:
	docker build -f docker/$(CONTAINERNAME)/Dockerfile . \
	-t $(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev --build-arg uid=1000 --build-arg user=$(USER)

.PHONY: run
run: compile
	mgba-qt -4 out/game.gba

.PHONY: compile2
compile2: cleanup
	docker run \
		-v ${PWD}/code:/${USER} \
		-v ${PWD}/out:/out \
		-t -e "TERM=xterm-256color" \
		$(VENDORNAME)/$(PROJECTNAME)/$(CONTAINERNAME):dev

.PHONY: getincludes
getincludes:
	$(COMPOSE) up 
	docker cp $(CONTAINERNAME):/opt/devkitpro/libgba/include $$(pwd)/code/
	docker cp $(CONTAINERNAME):/opt/devkitpro/libtonc/include $$(pwd)/code/

.PHONY: cleanup
cleanup:
	rm -rf $$(pwd)/code/build; rm -f $$(pwd)/out/*.elf; rm -f $$(pwd)/out/*.gba; rm -f $$(pwd)/out/*.sav
	
.PHONY: deleteincludes
deleteincludes:
	rm $$(pwd)/code/include/*.h
