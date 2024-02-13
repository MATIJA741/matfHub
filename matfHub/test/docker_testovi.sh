#!/bin/bash

    echo "Pokretanje jedinicnog testa za stat"
    ./scripts/run_stat_test.sh

    echo "Pokretanje jedinicnog testa za parser"
    ./scripts/run_parser_test.sh

    echo "Pokretanje jedinicnog testa za plotter"
    ./scripts/run_plotter_test.sh

