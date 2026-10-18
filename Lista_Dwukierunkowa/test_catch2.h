#pragma once
// Definicja macro Catch2 (wersja v2) dostarczająca własny punkt wejścia main
#define CATCH_CONFIG_MAIN
import "catch_amalgamated.hpp"; 
import "lista_dwukierunkowa.h";
import <stdexcept>;


TEST_CASE("Lista dwukierunkowa - operacje podstawowe na strukturze", "[DoublyLinkedList]") {

    // Tworzymy listę przechowującą wskaźniki na objects*
    DoubleLinkedList<objects*>* ll = new DoubleLinkedList<objects*>();

    SECTION("Lista po utworzeniu powinna być pusta") {
        REQUIRE(ll->is_empty() == true);
        REQUIRE(ll->list_size() == 0);
    }

    SECTION("Dodawanie elementów na koniec i początek") {
        objects* obj1 = new objects{ 10, 'a' };
        objects* obj2 = new objects{ 20, 'b' };

        ll->add(obj1);           // dodanie na koniec
        ll->addToDLLHead(obj2);  // dodanie na początek

        REQUIRE(ll->is_empty() == false);
        REQUIRE(ll->list_size() == 2);
    }

    SECTION("Usuwanie elementów i zarządzanie pamięcią") {
        objects* obj1 = new objects{ 1, 'x' };
        objects* obj2 = new objects{ 2, 'y' };

        ll->add(obj1);
        ll->add(obj2);

        REQUIRE(ll->list_size() == 2);

        // Czyszczenie listy wraz z usuwaniem danych z pamięci (true)
        ll->clear(true);

        REQUIRE(ll->is_empty() == true);
        REQUIRE(ll->list_size() == 0);
    }

    // Sprzątamy wskaźnik na samą listę
    delete ll;
}

TEST_CASE("Testowanie wyjątków i niepoprawnych indeksów", "[DoublyLinkedList_Errors]") {
    DoubleLinkedList<int>* intList = new DoubleLinkedList<int>();

    SECTION("Próba pobrania elementu z pustej listy powinna wyrzucić wyjątek") {
        REQUIRE_THROWS_AS(intList->getAt(0), std::out_of_range);
    }

    delete intList;
}