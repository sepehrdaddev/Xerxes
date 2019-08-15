#!/bin/bash

# Installation Shell Script (To make life easier) must be sudo
# By: @Nat-As <jandrews7348@floridapoly.edu>
cat << EOF      

██╗  ██╗███████╗██████╗ ██╗  ██╗███████╗███████╗
╚██╗██╔╝██╔════╝██╔══██╗╚██╗██╔╝██╔════╝██╔════╝
 ╚███╔╝ █████╗  ██████╔╝ ╚███╔╝ █████╗  ███████╗
 ██╔██╗ ██╔══╝  ██╔══██╗ ██╔██╗ ██╔══╝  ╚════██║
██╔╝ ██╗███████╗██║  ██║██╔╝ ██╗███████╗███████║
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚══════╝
                                                

+++ This will compile, make, and build the best Ddos tool in the world +++
EOF
printf "Make sure you are running this with \033[1;33m./setupsh\033[0m and not \033[1;33msh setup.sh\033[0m\n"
printf "Press any key to continue..."
read pauser

printf "\033[1;33m Compiling File... \033[0m\n"

function compile(){
    cd ..
    mkdir Xerxes
    cmake -S Xerxes-master -B Xerxes
}

function compilationerr(){
    echo Failed to compile!!!
}

compile || compilationerr

printf "\033[1;33m Building File... \033[0m\n"

function Build(){
    sudo docker build -f Documents/Xerxes-master/Dockerfile .
}

function BuildErr(){
    echo Build Failed!
}

Build || BuildErr
