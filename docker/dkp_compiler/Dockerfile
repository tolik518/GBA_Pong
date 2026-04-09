FROM devkitpro/devkitarm:20220531 AS dkp_downloader

# ----------------------------------------------------------
# This is the stage we'll be using for compiling our source 
FROM bitnami/minideb:buster-amd64 as dkp_compiler
LABEL maintainer="Anatolij 'Tolik518' Vasilev <function@returnnull.de>"

ARG uid
ARG user

# Get 'make' to execute our Makefile
# Debian Buster is EOL; redirect to the archive mirror before updating
RUN printf 'deb http://archive.debian.org/debian buster main\n\
deb http://archive.debian.org/debian-security buster/updates main\n' \
    > /etc/apt/sources.list && \
    apt-get -o Acquire::Check-Valid-Until=false update && \
    apt-get install make -y --no-install-recommends && \
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