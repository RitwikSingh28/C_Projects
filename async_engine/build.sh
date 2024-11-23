#!/bin/bash

set -xe

gcc -g -O3 -W -Wall -Wextra engine.c -o engine

./engine
