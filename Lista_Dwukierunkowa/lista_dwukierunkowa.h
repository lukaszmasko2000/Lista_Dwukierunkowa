#pragma once
#ifndef LISTA_DWUKIERUNKOWA_H
#define LISTA_DWUKIERUNKOWA_H


import std; 



//-------------------------------------------------------------------
// integer and character struct object with std::ostream& operator<<
//-------------------------------------------------------------------
struct objects {
	int number;
	char character;


	//automatyczny operator od C++20
	auto operator<=>(const objects&) const = default;

	// Przeciążenie operatora wypisania dla strumieni (dla wartości)
	friend std::ostream& operator<<(std::ostream& os, const objects& obj) {
		return os << "[" << obj.number << ", '" << obj.character << "'], ";
	}

	// Przeciążenie operatora dla wskaźnika (jeśli przechowujesz objects*)
	friend std::ostream& operator<<(std::ostream& os, const objects* obj) {
		if (!obj) return os << "null";
		return os << *obj;
	}
};



//----------------------------------------------------------------------------|
//	|*prev|value|*next|->--------------| 
//			 |--------------<-|*prev|value|*next|->------------| 
//										|-----------<-|*prev|value|*next|
//----------------------------------------------------------------------------|

//-----------------------------------------------
// Double Linked List Node (Struct and Template)
//-----------------------------------------------
template <class T>
struct DLLNode {
	

	DLLNode* next, * prev;
	T info;
	
	//constructor
	//-----------------------------------------
	DLLNode() : next(nullptr), prev(nullptr) {}
	//-----------------------------------------
	
	//new node "DLLNode new(15,*prev,*next);"
	//------------------------------------------------------
	DLLNode(const T& el, DLLNode* n = nullptr, DLLNode* p = nullptr)
		: info(el), next(n), prev(p) {}
	//------------------------------------------------------
};
//-----------------------------------------
// Double Linked List (Class and Template)
//-----------------------------------------
template <class T>
class DoubleLinkedList {
public:
	
	//constructor
	//-------------------------------------------------------------
	DoubleLinkedList() : head(nullptr), tail(nullptr), count(0) {}
	//-------------------------------------------------------------

	//destructor
	//-------------------------------
	~DoubleLinkedList() { clear(); }
	//-------------------------------

	//functions
	//------------------------------------------------------------------------
	[[nodiscard]] bool is_empty() const noexcept { return head == nullptr; }
	[[nodiscard]] size_t list_size() const noexcept { return count; }
	void clear(bool deleteDataElements = false) {
		while (!is_empty()) {
			auto* temp = head;
			head = head->next;

			if (deleteDataElements) {
				if constexpr (std::is_pointer_v<T>) {
					delete temp->info;
				}
			}
			delete temp;
		}
		tail = nullptr;
		count = 0;
	}

	void add(const T& val) {
		addToDLLTail(val);
	}
	
	void addToDLLHead(const T&);
	void addToDLLTail(const T&);
	T deleteFromDLLHead();
	T deleteFromDLLTail();
	[[nodiscard]] T& getAt(size_t);
	void setAt(size_t, const T&);
	[[nodiscard]] DLLNode<T>* search(const T&) const;
	[[nodiscard]] bool searchAndRemove(const T&);
	[[nodiscard]] std::string toString(size_t) const;

	template<typename Comparator>
	void addOrderer(const T& el, Comparator comp);

	template<typename Comparator>
	bool find_and_remove(const T&, Comparator);
	//------------------------------------------------------------------------


protected:
	// protected variables (constructor not needed)
	//-------------------------------------------
	DLLNode<T>* head, * tail;	
	size_t count;
	//-------------------------------------------
};

//--------------------------------------------
//-- Dodanie wartosci na samym początku listy
//--------------------------------------------
template<class T>
void DoubleLinkedList<T>::addToDLLHead(const T& el) {

	if (head != nullptr) {
		head = new DLLNode<T>(el, head, nullptr);
		head->next->prev = head;
	}
	else head = tail = new DLLNode<T>(el);
	
	++count;
}


//-----------------------------------------
//-- Dodanie wartosci na samym końcu listy
//-----------------------------------------
template<class T>
void DoubleLinkedList<T>::addToDLLTail(const T& el) {
	
	if (tail != nullptr)
	{
		tail = new DLLNode<T>(el, nullptr, tail);
		tail->prev->next = tail;
	}
	else head = tail = new DLLNode<T>(el);

	++count;
}


//--------------------------------------
//-- Usunięcie wartosci z początku listy
//--------------------------------------
template<class T>
T DoubleLinkedList<T>::deleteFromDLLHead() {

	if (is_empty()) throw std::underflow_error("Lista jest pusta");

	auto el = tail->info;
	if (head == tail) {
		delete head;
		head = tail = nullptr;
	}
	else {
		tail = tail->prev;
		delete tail->next;
		tail->next = nullptr;
	}
	--count;
	return el;
}


//------------------------------------
//-- Usunięcie wartosci z końca listy
//------------------------------------
template<class T>
T DoubleLinkedList<T>::deleteFromDLLTail() {

	if (is_empty()) throw std::underflow_error("Lista jest pusta");

	auto el = head->info;
	if (head == tail) {
		delete head;
		head = tail = nullptr;
	}
	else {
		head = head->next;
		delete head->next;
		head->next = nullptr;
	}
	--count;
	return el;
}

//--------------------------------------------
//-- Pobranie elementu z danego indeksu listy
//--------------------------------------------
template<class T>
T& DoubleLinkedList<T>::getAt(size_t index) {

	if (index >= count)
		throw std::out_of_range("Indeks poza zakresem listy!");

	auto* curr = head;
	for (auto i{ 0uz }; i < index && curr->next != nullptr; ++i) curr = curr->next;
	return curr->info;
}

//-------------------------------------------------
//-- Ustawienie danej wartości z po danym indeksie
//-------------------------------------------------
template<class T>
void DoubleLinkedList<T>::setAt(size_t index, const T& newEl) {

	if (index >= count)
		throw std::out_of_range("Indeks poza zakresem listy!");

	auto* curr = head;
	for (auto i{ 0uz }; i < index && curr->next != nullptr; ++i)  curr = curr->next;

	curr->info = newEl;
}

//---------------------------------------
//-- Szukanie elementu listy po wartości
//---------------------------------------
template<class T>
DLLNode<T>* DoubleLinkedList<T>::search(const T& key) const {
	auto* curr = head;

	while (curr != nullptr) {
		if (curr->info.number == key.number and curr->info.character == key.character) {
			return curr; 
		}
		curr = curr->next;
	}
	
	return nullptr;
}

//-------------------------------------------------------
//-- Szukanie elementu listy po wartości i usunięcie jej
//-------------------------------------------------------
template<class T>
bool DoubleLinkedList<T>::searchAndRemove(const T& key) {
	auto* target = search(key);
	if (target == nullptr) {
		return false;
	}

	if (target == head) {
		deleteFromDLLHead();
	}
	else if (target == tail) {
		deleteFromDLLTail();
	}
	else {
		target->prev->next = target->next;
		target->next->prev = target->prev;
		delete target;
		count--;
	}
	return true;
}

//--------------------------------------------------
//-- Dodanie elementu listy z wymuszonym porządkiem
//--------------------------------------------------
template<class T>
template<typename Comparator>
void DoubleLinkedList<T>::addOrderer(const T& el, Comparator comp) {

	if (is_empty() || comp(el, head->info) || (!comp(head->info, el) && !comp(el, head->info))) {
		addToDLLHead(el);
		return;
	}

	if (!comp(el, tail->info)) {
		addToDLLTail(el);
		return;
	}

	auto* curr = head->next;

	while (curr != nullptr && comp(curr->info, el)) {
		curr = curr->next;
	}

	if (curr == nullptr) {
		addToDLLTail(el);
	}
	else {
		auto* newNode = new DLLNode<T>(el, curr, curr->prev);
		if (curr->prev != nullptr) {
			curr->prev->next = newNode;
		}
		curr->prev = newNode;
		count++;
	}
}

//-------------------------------------------------------------------
//-- Zwrócenie napisowej reprezentacji listy (z agrumentem dlugosci)
//-------------------------------------------------------------------
template<class T>
std::string DoubleLinkedList<T>::toString(size_t limit) const {
	std::ostringstream oss;
	oss << "DoubleLinkedList [Rozmiar: " << count << ", Adres: " << this << "] -> ";
	auto* curr = head;
	auto displayed = 0uz;

	oss << "{ ";
	while (curr != nullptr && displayed < limit) {
		oss << *curr->info;
		curr = curr->next;
		displayed++;
	}
	if (curr != nullptr) {
		oss << "... (pominięto resztę)";
	}
	oss << "}";
	return oss.str();
}



//-------------------------------------------
//-- Szukanie i usuwanie węzła
//-------------------------------------------
template<class T>
template<typename Comparator>
bool DoubleLinkedList<T>::find_and_remove(const T& key, Comparator comp) {
	DLLNode<T>* curr = head;

	while (curr != nullptr) {
		if (comp(curr->info, key)) {
			if (curr == head) {
				deleteFromDLLHead();
			}
			else if (curr == tail) {
				deleteFromDLLTail();
			}
			else {
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				delete curr->info;
				delete curr;
				count--;
			}
			return true;
		}
		curr = curr->next;
	}
	return false;
}








#endif // LISTA_DWUKIERUNKOWA_H