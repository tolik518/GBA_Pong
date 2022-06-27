FROM bitnami/minideb:buster-amd64 AS dkp_downloader
LABEL maintainer="Anatolij 'Tolik518' Vasilev <function@returnnull.de>"

# fix for docker on windows, on some windows version you'd
# get an error that /etc/mtab may not exist 
RUN test -f /etc/mtab || ln -s /proc/self/mounts /etc/mtab 

RUN apt-get update && apt-get install apt-utils -y && apt-get install wget -y 

# Get and install the DevkitPro Pacman 
RUN wget --progress=dot:giga https://apt.devkitpro.org/install-devkitpro-pacman &&\
    chmod +x ./install-devkitpro-pacman &&\
    yes | ./install-devkitpro-pacman &&\
    rm ./install-devkitpro-pacman 

# Get and install the gba-dev ressources for developement 
RUN dkp-pacman -S --noconfirm gba-dev

# ----------------------------------------------------------
# This is the stage we'll be using for compiling our source 
FROM bitnami/minideb:buster-amd64 as dkp_compiler

ARG uid
ARG user

# Get 'make' to execute our Makefile
RUN apt-get update  && apt-get install apt-utils -y  && apt-get install make -y --no-install-recommends && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Copy the devkitpro from the dkp_downloader to our image
COPY --from=dkp_downloader /opt/devkitpro /opt/devkitpro

# Create new user who has the rights to the out folder where the .gba and .elf will be copied
RUN useradd -l -d /$user -m -u $uid $user && \
    chown -R $uid /$user && \
    mkdir /out &&  chown -R $uid /out

ENV HOME=/$user
ENV DEVKITPRO=/opt/devkitpro
ENV DEVKITARM=/opt/devkitpro/devkitARM

USER $user
WORKDIR /$user
VOLUME /$user

CMD make