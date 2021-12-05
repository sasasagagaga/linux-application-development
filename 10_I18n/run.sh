#!/bin/sh

make gitclean; autoreconf -fisv && ./configure && make

