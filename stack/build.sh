#!/bin/bash

set -xe

gcc -O0 -S -g -fno-stack-protector -z execstack -o stack_smash.s main.c
