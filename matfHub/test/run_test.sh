display_options() {
    echo "Izaberite opciju za testiranje:"
    echo "1. Pokreni jedinicne testove za statistics"
    echo "2. Pokreni jedinicne testove za parser"
    echo "3. Pokreni jedinicne testove za matrix"
    echo "4. Pokreni jedinicne testove za plotter"
}

# Funkcija za izvrsavanje odabrane opcije
execute_option() {
    case $1 in
        1)
            echo "Pokrecu se testovi za statistics"
            ./scripts/run_stat_test.sh
            ;;
        2)
            echo "Pokrecu se testovi za parser"
            ./scripts/run_parser_test.sh
            ;;
        3)
            echo "Pokrecu se testovi za matrix"
            ./scripts/run_matrix_test.sh
            ;;
        4)
            echo "Pokrecu se testovi za plotter"
            ./scripts/run_plotter_test.sh
            ;;
        *)
            echo "Nepostojeca opcija."
            exit 1
            ;;
    esac
}

display_options
read -p "Izaberite opciju: " choice

execute_option $choice
