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

function checklib(){
    printf "Checking file system...\n"
    ls lib/fmt/include
    ls lib/libressl/include
    ls lib/spdlog/include
}

function checkliberr(){
    printf "\033[0;31mFailed to check libraries!\033[0m\n"
    printf "\033[0;31mPlease make sure all the contents in the lib folder were cloned from git!\033[0m\n"
    exit 0
}
checklib || checkliberr

function installcmake(){
    printf "\033[1;33m Cmake not found! Installing it for you... \033[0m\n"
    sudo apt install cmake
    sudo cmake -S Xerxes-master -B Xerxes-master/Xerxes .
}

printf "\033[1;33m Compiling File... \033[0m\n"

function compile(){
    mkdir Xerxes
    cd ..
    sudo cmake -S Xerxes-master -B Xerxes-master/Xerxes . || installcmake
    cd Xerxes-master/Xerxes
    make
    cd ..
    cd ..
}

function cleanup(){
    rm -r Xerxes-master/Xerxes || printf "\033[0;31mCleanup Failed!\033[0m\n"
    exit 0
}

function compilationerr(){
    printf "\033[0;31mCompilation Failed!\033[0m\n"
    cleanup
}

compile || compilationerr

printf "\033[1;33m Building File... \033[0m\n"

function installcurl(){
    printf "\033[1;33m Curl not found! Installing it for you... \033[0m\n"
    sudo apt install curl
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
}

function installdocker(){
    printf "\033[1;33m Docker not found! Installing it for you... \033[0m\n"
    sudo apt-get remove docker docker-engine docker.io containerd runc
    sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add - || installcurl
    sudo apt-key fingerprint 0EBFCD88
    sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
    sudo apt-get update
    sudo apt-get install docker-ce docker-ce-cli containerd.io
    sudo docker build -f Dockerfile . || printf "\033[0;31mInstalling Docker Failed! Please install it yourself.\033[0m\n"

}
function Build(){
    sudo docker image ls || installdocker #Not having Docker installed is not the only cause of errors for the next build step.
    cd Xerxes-master/Xerxes/bin
    sudo docker build -t xerxes -f Dockerfile . || printf "\033[0;31mDocker build Failed!\033[0m\n"
}

function BuildErr(){
    printf "\033[0;31mBuild Failed!\033[0m\n"
}

Build || BuildErr
#Report Results
sudo docker image ls