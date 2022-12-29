#!/bin/bash
ls
cmake-build-debug/bulk_server 9000 3 & (seq 0 9 | nc localhost 9000) && fg
