#!/usr/bin/env bash
set -xeuo pipefail

# test LR-1
cd ./LR1 \
  && c++ main.cpp -o main \
  && mv input.txt input_test.txt \
  && cp ../tests/LR1/input.txt input.txt \
  && ./main \
  && cmp output.txt ../tests/LR1/output.txt \
  && rm -rf input.txt \
  && rm -rf main \
  && rm -rf output.txt \
  && mv input_test.txt input.txt \
  && cd ..

# test LR-2
cd ./LR2 \
  && c++ main.cpp -o main \
  && mv input.txt input_test.txt \
  && cp ../tests/LR2/input.txt input.txt \
  && ./main \
  && cmp output.txt ../tests/LR2/output.txt \
  && rm -rf input.txt \
  && rm -rf main \
  && rm -rf output.txt \
  && mv input_test.txt input.txt \
  && cd ..

# test LR-3
cd ./LR3 \
  && c++ main.cpp -o main \
  && mv input.txt input_test.txt \
  && cp ../tests/LR3/input.txt input.txt \
  && ./main \
  && cmp output.txt ../tests/LR3/output.txt \
  && rm -rf input.txt \
  && rm -rf main \
  && rm -rf output.txt \
  && mv input_test.txt input.txt \
  && cd ..

# test LR-4
cd ./LR4 \
  && c++ main.cpp -o main \
  && mv input.txt input_test.txt \
  && cp ../tests/LR4/input.txt input.txt \
  && ./main \
  && cmp output.txt ../tests/LR4/output.txt \
  && rm -rf input.txt \
  && rm -rf main \
  && rm -rf output.txt \
  && mv input_test.txt input.txt \
  && cd ..