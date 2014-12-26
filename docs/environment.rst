Development Environment
=======================

Operating System
----------------

In order for simplify to setup developing environment, I started from vagrant_.

   $ vagrant box add precise64
   $ vagrant init precise64
   $ vagrant up


Some tools
----------

`$ sudo apt-get install curl`
`$ sudo apt-get install wget`
`$ sudo apt-get install make`


Assembly Compiler
-----------------

`$ sudo apt-get install nasm`


C and C++ Compiler
------------------

`$ sudo apt-get install gcc`
`$ sudo apt-get install g++`


Bochs
-----

`$ sudo apt-get install bochs`
`$ sudo apt-get install bochs-x    # 'x' module for bochs`
`$ sudo apt-get install bochs-term    # or using terminal`


Bochs Debugger
--------------

`$ sudo apt-get install libncurses-dev`
`$ wget http://sourceforge.net/projects/bochs/files/bochs/2.4.6/bochs-2.4.6.tar.gz/download -O bochs-2.4.6.tar.gz`
`$ tar -zxvf bochs-2.4.6.tar.gz`
`$ cd bochs-2.4.6`
`$ configure --enable-debugger --enable-disasm --enable-debugger-gui --with-x --with-term`
`$ make`
`$ sudo cp ./bochs /usr/bin/bochs-dbg`


.. _vagrant: https://www.vagrantup.com/
