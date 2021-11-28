#!/bin/sh

make gitclean; autoreconf -fisv && ./configure --enable-gcov && make && make gcov

