<img align="left" width="100" height="100" src="/XerxesTheGreat.jpg">

# Xerxes

[![Build Status](https://travis-ci.org/sepehrdaddev/Xerxes.svg?branch=experimental)](https://travis-ci.org/sepehrdaddev/Xerxes)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Xerxes Dos Tool Rewritten in C++

## Features
**Xerxes has many features, some of these features are:**
* SSL Support (for HTTPS connections)
* HTTP header randomization (for More Anonymity)
* Useragent randomization (for even more Anonymity)
* Multiprocessing support (for more performance)
* Multiple Attack vectors ( NullTCP, Slowloris and etc...)
* etc...

Not only that but also we are aggressively developing it and adding a lot more features and functionalities.

## Usage
`Usage: ./Xerxes -target <ip or url> -port <port> <options>`\
Options:\
&emsp;&emsp;&emsp;&emsp;&emsp;-h&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to HTTP\
&emsp;&emsp;&emsp;&emsp;&emsp;-i&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed ICMP Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-r&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Rudy\
&emsp;&emsp;&emsp;&emsp;&emsp;-s&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Slowloris\
&emsp;&emsp;&emsp;&emsp;&emsp;-b&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Black Nurse\
&emsp;&emsp;&emsp;&emsp;&emsp;-be&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Beast\
&emsp;&emsp;&emsp;&emsp;&emsp;-nu&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to NULLUDP\
&emsp;&emsp;&emsp;&emsp;&emsp;-nt&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to NULLTCP\
&emsp;&emsp;&emsp;&emsp;&emsp;-ft&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to TCPFlood\
&emsp;&emsp;&emsp;&emsp;&emsp;-fu&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to UDPFlood\
&emsp;&emsp;&emsp;&emsp;&emsp;-su&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed UDP Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-sy&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed Syn Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-sa&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed Ack Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-sr&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed Rst Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-sg&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed Urg Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-sp&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed Push Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-sf&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Spoofed Fin Flood\
&emsp;&emsp;&emsp;&emsp;&emsp;-td&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Teardrop\
&emsp;&emsp;&emsp;&emsp;&emsp;-ld&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Land\
&emsp;&emsp;&emsp;&emsp;&emsp;-sm&emsp;&emsp;&emsp;&emsp;&emsp;set attack vector to Smurf\
&emsp;&emsp;&emsp;&emsp;&emsp;-ss&emsp;&emsp;&emsp;&emsp;&emsp;enable SSL\
&emsp;&emsp;&emsp;&emsp;&emsp;-w&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;wait for response\
&emsp;&emsp;&emsp;&emsp;&emsp;-rh&emsp;&emsp;&emsp;&emsp;&emsp;randomize HTTP Header\
&emsp;&emsp;&emsp;&emsp;&emsp;-ru&emsp;&emsp;&emsp;&emsp;&emsp;randomize HTTP UserAgent\
&emsp;&emsp;&emsp;&emsp;&emsp;-rs&emsp;&emsp;&emsp;&emsp;&emsp;randomize Source IP\
&emsp;&emsp;&emsp;&emsp;&emsp;-rp&emsp;&emsp;&emsp;&emsp;&emsp;randomize Source Port\
&emsp;&emsp;&emsp;&emsp;&emsp;-qq&emsp;&emsp;&emsp;&emsp;&emsp;set verbosity to quiet quiet\
&emsp;&emsp;&emsp;&emsp;&emsp;-q&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set verbosity to quiet\
&emsp;&emsp;&emsp;&emsp;&emsp;-v&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set verbosity to verbose\
&emsp;&emsp;&emsp;&emsp;&emsp;-vv&emsp;&emsp;&emsp;&emsp;&emsp;set verbosity to very verbose\
&emsp;&emsp;&emsp;&emsp;&emsp;-D&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set delay in microseconds\
&emsp;&emsp;&emsp;&emsp;&emsp;-T&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set number of threads\
&emsp;&emsp;&emsp;&emsp;&emsp;-C&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set number of connections per thread\
&emsp;&emsp;&emsp;&emsp;&emsp;-B&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;set broadcast address\
&emsp;&emsp;&emsp;&emsp;&emsp;-target&emsp;&emsp;&emsp;set host ip or url\
&emsp;&emsp;&emsp;&emsp;&emsp;-port&emsp;&emsp;&emsp;&emsp;set target port number\
&emsp;&emsp;&emsp;&emsp;&emsp;-help&emsp;&emsp;&emsp;&emsp;show help\
&emsp;&emsp;&emsp;&emsp;&emsp;-version&nbsp;&emsp;&emsp;show version

## Questions ?
If you have any questions feel free to visit our <a href="https://github.com/sepehrdaddev/Xerxes/wiki">Wiki page</a>

## License
This software is distributed under the GNU General Public License version 3 (GPLv3)

## LEGAL NOTICE
THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL USE ONLY! IF YOU ENGAGE IN ANY ILLEGAL ACTIVITY THE AUTHOR DOES NOT TAKE ANY RESPONSIBILITY FOR IT. BY USING THIS SOFTWARE YOU AGREE WITH THESE TERMS.
