#!/bin/bash
rm results.csv
for i in {0..100}
do
  ./ass1 10 10 $i >> results.csv
done
