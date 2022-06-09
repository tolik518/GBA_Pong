FROM debian AS dkp_downloader
LABEL maintainer="Anatolij 'Tolik518' Vasilev <function@returnnull.de>"
ENV DEBIAN_FRONTEND='noninteractive' 

RUN apt-get update && apt-get install wget -y

# Get and install the DevkitPro Pacman 
RUN wget https://apt.devkitpro.org/install-devkitpro-pacman &&\
    chmod +x ./install-devkitpro-pacman &&\
    yes | ./install-devkitpro-pacman &&\
    rm ./install-devkitpro-pacman 

# Get and install the gba-dev ressources for developement 
RUN dkp-pacman -S --noconfirm gba-dev


# This is the stage we'll be using for compiling our source 
FROM bitnami/minideb:buster-amd64 as dkp_compiler

ARG uid
ARG user

# RUN apt-get update && apt-get install make -y 

COPY --from=dkp_downloader /opt/devkitpro /opt/devkitpro

RUN useradd -d /$user -m -u $uid $user && \
    chown -R $uid /$user && \
    mkdir /out &&  chown -R $uid /out

ENV HOME=/$user
ENV DEVKITPRO=/opt/devkitpro
ENV DEVKITARM=/opt/devkitpro/devkitARM

USER $user
WORKDIR /$user
VOLUME /$user

CMD make