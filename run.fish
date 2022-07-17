#!/usr/bin/env fish

gcc main.c -o ckana \
    -lncursesw \
    -Ibin/ncurses-6.3/include \
    -Lbin/ncurses-6.3/lib
./ckana
