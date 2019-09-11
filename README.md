<img align="left" width="100" height="100" src="img/XerxesTheGreat.jpg">

# Xerxes

[![Build Status](https://travis-ci.com/Nat-As/Xerxes.svg?branch=master)](https://travis-ci.com/Nat-As/Xerxes)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![SSL: LibreSSL v2.9.1](https://img.shields.io/badge/SSL-LibreSSL%20v2.9.1-green.svg)](https://www.libressl.org/)
[![Platform: Linux](https://img.shields.io/badge/Platform-Linux-blue.svg)](https://www.linux.org/)
[![Version: 2.0beta](https://img.shields.io/badge/Version-2.0beta-blue.svg)](https://github.com/sepehrdaddev/Xerxes.git)

Dos tool with multiple vectors available for stress testing internet services. Includes docker support with threading!
## Installation
### Requirements
1. Install cmake: ```sudo apt install cmake```
2. Install docker-ce if you don't already have it. https://docs.docker.com/install/linux/docker-ce/ubuntu/

### Build
1. Install Xerxes: ```./setup.sh```

## Usage

```
$ ./Xerxes -H
  --==[ Xerxes enhanced by Sepehrdad Sh ]==--

  bin/Xerxes {OPTIONS}

    Xerxes dos tool enhanced

  OPTIONS:

      -H, --help                        display this help menu
      -V, --version                     display version
      --vecs                            display available vectors
      -h[rhost], --rhost=[rhost]        remote host address [default 127.0.0.1]
      -p[rport], --rport=[rport]        remote host port [default 80]
      -b[bcast], --bcast=[bcast]        broadcast address [default 127.0.0.1]
      -v[vector], --vec=[vector]        attack vector [default 0]
      -d[delay], --dly=[delay]          attack delay [default 1 ns]
      -t[threads], --trds=[threads]     number of threads [default 10]
      -c[connections],
      --conn=[connections]              number of connections [default 25]
      --tls                             enable tls
      --rand-lhost                      enable local host randomization
      --rand-lport                      enable local port randomization
      --daemonize                       daemonize
```

## License

This software is distributed under the GNU General Public License version 3 (GPLv3)

## LEGAL NOTICE

THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL USE ONLY! IF YOU ENGAGE IN ANY ILLEGAL ACTIVITY THE AUTHOR DOES NOT TAKE ANY RESPONSIBILITY FOR IT. BY USING THIS SOFTWARE YOU AGREE WITH THESE TERMS.

## Get Involved

**Please, send us pull requests!**
