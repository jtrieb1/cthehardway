#!/bin/bash
set -e

echo "Instantiating database w/ Valgrind..."
valgrind -s ./ec17 db_test.db c 256 100

echo "Adding 3 members..."
./ec17 db_test.db s 0 "Jeff" "jeff@jeff.jeff" "Jefferson" "13441344"
./ec17 db_test.db s 1 "Joe" "joe@jeff.jeff" "Joerson" "13441345"
./ec17 db_test.db s 2 "Chunko" "chunks@jeff.jeff" "Chunkerson" "13441346"

echo "Getting Jeff..."
./ec17 db_test.db g 0

echo "Finding Chunko by name..."
./ec17 db_test.db f "chunko"

echo "Finding Joe by email..."
./ec17 db_test.db f "joe@jeff"

echo "Finding Jeff by phone..."
./ec17 db_test.db f 13441344

echo "Finding Jeff by phone with hyphens..."
./ec17 db_test.db f "1-344-1344"

echo "Finding Chunko by city..."
./ec17 db_test.db f chunkerson

echo "Deleting Jeff..."
./ec17 db_test.db d 0

echo "Listing database..."
./ec17 db_test.db l

