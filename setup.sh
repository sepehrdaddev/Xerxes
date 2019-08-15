#!/bin/bash

# Installation Shell Script (To make life easier) must be sudo
# By: @Nat-As <jandrews7348@floridapoly.edu>

clear
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
    mkdir Xerxes
    sudo cmake -S Xerxes-master/ -B Xerxes .
}

function cleanup(){
    rm -r Xerxes || printf "\033[0;31mCleanup Failed!\033[0m\n"
    exit 0
}

function compilationerr(){
    printf "\033[0;31mCompilation Failed!\033[0m\n"
    cleanup
}

compile || compilationerr

printf "\033[1;33m Building File... \033[0m\n"

function Build(){
    cd Xerxes/bin
    sudo docker build -f Dockerfile .
}

function BuildErr(){
    printf "\033[0;31mBuild Failed!\033[0m\n"
}

Build || BuildErr
