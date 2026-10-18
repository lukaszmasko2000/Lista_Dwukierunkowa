
//
//C++23 required and Scan sources for modules: ON
//


//Tytuł: ALGO2 IS1 203 LAB01
//Imię i Nazwisko: Łukasz Maśko
//Email: ml51668@zut.edu.pl

import std;
import "lista_dwukierunkowa.h";
import <Windows.h>;             
import "test_catch2.h";
//#include <cstdlib>
//#include <crtdbg.h>

//#define _CRTDBG_MAP_ALLOC

//------------------------------------------------------------------------------
//                             Komparatory Generyczne
//------------------------------------------------------------------------------
struct GenericEqual {
    template <typename T1, typename T2>
    bool operator()(const T1& a, const T2& b) const {
        if constexpr (std::is_pointer_v<T1> && std::is_pointer_v<T2>) {
            if (!a || !b) return a == b;
            return *a == *b; 
        }
        else {
            return a == b;  
        }
    }
};

struct GenericLess {
    template <typename T1, typename T2>
    bool operator()(const T1& a, const T2& b) const {
        if constexpr (std::is_pointer_v<T1> and std::is_pointer_v<T2>) {
            if (!a || !b) return a < b;
            return *a < *b;
        }
        else {
            return a < b;
        }
    }
};
//------------------------------------------------------------------------------


int main() {

    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    //---------------------------
    //Wymuszenie polskich znaków
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    //---------------------------
    
    
    static constexpr auto MAX_ORDER = 6; 
    
    //------------------------------------------------------------
    //z biblioteki <random>, bezpieczne liczby pseldolosowe w C++
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist_numbers(0, 10000);
    std::uniform_int_distribution<int> dist_chars('a', 'z');
    //------------------------------------------------------------


    std::print("=== URUCHAMIANIE TESTÓW CATCH2 v3 ===\n");

    Catch::Session session;

    session.configData().showSuccessfulTests = true;

    auto result = session.run();

    if (result != 0) {
        std::print("Błąd: Niektóre testy jednostkowe nie przeszły!\n");
        return result;
    }
    //------------------------------------------------------------------------

    std::print("\n=== TESTY ZALICZONE. ROZPOCZYNANIE EKSPERYMENTU ===\n\n");


    //Wrzucenie na stertę listy z strukturą dwuch zmiennych (int,char)
    DoubleLinkedList<objects*>* ll = new DoubleLinkedList<objects*>();


    for (auto o = 1; o <= MAX_ORDER; o++) { 

        //wielkosc n po 10^o (1,2,3,4,5,6)
        auto n = static_cast<int>(std::pow(10, o));     //konwersja na int, bo std::pow domyślnie jest double
        std::print("--- Rząd wielkości: 10^{} (n = {}) ---\n",o,n);     //std::print() jest rozwiązaniem C++23

        //---------------------------------------------------------------------------
        //z biblioteki <chrono>, dokladniejsze liczenie czasu
        //--------------------------------start--------------------------------------
        auto start_add = std::chrono::high_resolution_clock::now();     

        //----------------------------------------------------------------
        //dodawanie randomowych wartosci do poszczegolnej wielkosci listy
        //----------------------------------------------------------------
        for (int i = 0; i < n; i++) {
            auto rand_numb = dist_numbers(gen);
            auto rand_char = static_cast<char>(dist_chars(gen));
            auto* so = new objects{ rand_numb, rand_char };
            ll->add(so); 
            //ll->addOrderer(so,GenericLess{});   //wymuszony porządek tylko dla małych list.
        }

        auto end_add = std::chrono::high_resolution_clock::now();      
        //--------------------------------koniec--------------------------------------
        
        std::chrono::duration<double> elapsed_add = end_add - start_add;
        std::print("Czas dodawania {} elementów: {:.6f} s\n", n, elapsed_add.count());
        std::println("{}\n",ll->toString(10));
        

        const auto m = static_cast<int>(std::pow(10, 4));   //10^4 = 10000 możliwości usunięcia
        //--------------------------------start--------------------------------------
        auto start_search = std::chrono::high_resolution_clock::now();


        //----------------------------------------------------------------------------------------
        //znajdowanie i usuwanie listy jeśli będą pasowały obie wartości (a z tym różnie bywa xd), złożoność O(n^2)
        //----------------------------------------------------------------------------------------
        for (int i = 0; i < m; i++) {
            auto rand_numb = dist_numbers(gen);
            auto rand_char = static_cast<char>(dist_chars(gen));
            auto* so = new objects{ rand_numb, rand_char };
            ll->find_and_remove(so, GenericEqual{});
            delete so; 
        }

        auto end_search = std::chrono::high_resolution_clock::now();
        //--------------------------------koniec--------------------------------------
        std::chrono::duration<double> elapsed_search = end_search - start_search;
        std::print("Czas operacji wyszukiwania/usuwania: {:.3f} s\n", elapsed_search.count());
        
        std::println("{}\n", ll->toString(10));
        
        
        //----------------------------------------------------
        // Pobranie wartości z danego indeksu listy
        //----------------------------------------------------

            auto val = ll->getAt(2);
            std::cout << *val << '\n'; 
        
        
        
        
        ll->clear(true);    //true oznacza ze czysci wszystko

        std::cout << "\n";
    }

    delete ll; 
    return 0;
}