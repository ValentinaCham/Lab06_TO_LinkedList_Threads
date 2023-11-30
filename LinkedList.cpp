#include <iostream>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <mutex>
using namespace std;

int random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Clase Nodo
template <typename T>
class Nodo {
private:
    T dato;
    Nodo* next;

public:
    Nodo(T valor) : dato(valor), next(nullptr) {} //Inicializador

    T getDato() const {
        return dato;
    }
    void setDato(T nuevoDato) {
        dato = nuevoDato;
    }

    Nodo* getNext() const {
        return next;
    }

    void setNext(Nodo* newNext) {
        next = newNext;
    }
};

template <typename T>
class LinkedList {
private:
    Nodo<T>* head;
    Nodo<T>* aux;
    std::mutex mutex;

public:
    LinkedList() : head(nullptr), aux(nullptr) {}

    ~LinkedList() {
        aux=head;
        while (aux != nullptr) {
            Nodo<T>* temp = aux;
            aux = aux->getNext();
            delete temp;
        }
    }

    void add(T valor) {
        Nodo<T>* newNode = new Nodo<T>(valor);
        if (head == nullptr) {
            head = newNode;
            aux = head;
        } else {
            aux->setNext(newNode);
            aux = newNode;
        }
    }

    Nodo<T>* search(T valor) {
        Nodo<T>* sr = head;
        while (sr != nullptr) {
            if(sr->getDato()==valor){
                return sr;
            }
            sr = sr->getNext();
        }
        return nullptr;
    }

    void remove(T valor) {
        Nodo<T>* rem = search(valor);
        if (rem == head && rem == aux) {
            head = nullptr;
            aux = nullptr;
            delete rem;
        } else if (rem == head) {
            head = head->getNext();
            delete rem;
        } else if (rem == aux) {
            Nodo<T>* temp = head;
            while (temp->getNext() != aux) {
                temp = temp->getNext();
            }
            aux = temp;
            aux->setNext(nullptr);
            delete rem;
        } else {
            Nodo<T>* temp = head;
            while (temp->getNext() != rem) {
                temp = temp->getNext();
            }
            temp->setNext(rem->getNext());
            delete rem;
        }
    }

    void modify(T valor, T valSuma) {
        Nodo<T>* mod = search(valor);
        if (mod != nullptr) {
            mod->setDato(mod->getDato() + valSuma);
        }
    }

    void show() {
        aux = head;
        while (aux->getNext() != nullptr) {
            std::cout << aux->getDato() << " ";
            aux = aux->getNext();
        }
        std::cout << aux->getDato() << " ";
        std::cout << std::endl;
    }

    // INSERTAR
    void insertar(T valor) {
        std::lock_guard<std::mutex> lock(mutex);
        add(valor);
        cout << "Insertando: " << valor << endl;
    }

    // BUSCAR
    bool buscar(T valor) {
        std::lock_guard<std::mutex> lock(mutex);
        Nodo<T>* resultado = search(valor);
        if (resultado != nullptr) {
            cout << "Buscado: " << valor << endl;
            return true;
        } else {
            cout << "No encontrado: " << valor << endl;
            return false;
        }
    }

    // ELIMINAR
    bool eliminar(T valor) {
        std::lock_guard<std::mutex> lock(mutex);
        bool eliminado = false;
        Nodo<T>* rem = search(valor);
        if (rem != nullptr) {
            remove(valor);
            cout << "Eliminando: " << valor << endl;
            eliminado = true;
        } else {
            cout << "No se eliminó: " << valor << endl;
        }
        return eliminado;
    }

    // MODIFICAR
    void modificar(T valor, T valSuma) {
        std::lock_guard<std::mutex> lock(mutex);
        Nodo<T>* mod = search(valor);
        if (mod != nullptr) {
            mod->setDato(mod->getDato() + valSuma);
            cout << "Modificando " << valor << " a " << mod->getDato() << endl;
        } else {
            cout << "No se encontró: " << valor << endl;
        }
    }
};

// Hilo 1: Eliminar elementos de la lista
void thread1(LinkedList<int>& lista) {
    for (int i = 0; i < 10; ++i) {
        int valRemove = random(1, 20);
        lista.eliminar(valRemove);
        this_thread::sleep_for(chrono::milliseconds(500));  // Para simular la ejecución en paralelo
    }
}

// Hilo 2: Insertar elementos en la lista
void thread2(LinkedList<int>& lista) {
    for (int i = 0; i < 10; ++i) {
        int valAdd = random(1, 20);
        lista.insertar(valAdd);
        this_thread::sleep_for(chrono::milliseconds(500));  // Para simular la ejecución en paralelo
    }
}

// Hilo 3: Buscar elementos en la lista
void thread3(LinkedList<int>& lista) {
    for (int i = 0; i < 10; ++i) {
        int valSearch = random(1, 20);
        lista.buscar(valSearch);
        this_thread::sleep_for(chrono::milliseconds(500));  // Para simular la ejecución en paralelo
    }
}

// Hilo 4: Modificar elementos de la lista
void thread4(LinkedList<int>& lista) {
    for (int i = 0; i < 10; ++i) {
        int valorModificar = random(1, 20);
        int incremento = random(1, 5);
        lista.modificar(valorModificar, incremento);
        this_thread::sleep_for(chrono::milliseconds(500));  // Para simular la ejecución en paralelo
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    LinkedList<int> myList;  // Puedes cambiar 'int' por el tipo de dato que desees

    // Crear los threads y ejecutarlos en paralelo
    thread t1(thread1, ref(myList));
    thread t2(thread2, ref(myList));
    thread t3(thread3, ref(myList));
    thread t4(thread4, ref(myList));

    // Esperar a que todos los threads terminen
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
/*
int main() {
    LinkedList<int> listNum;
    listNum.add(1);
    listNum.add(2);
    listNum.add(3);
    listNum.add(8);
    listNum.add(5);
    listNum.add(10);
    listNum.add(4);
    listNum.add(9);
    listNum.add(3);
    listNum.add(6);
    listNum.add(7);
    listNum.add(12);
    std::cout << "Lista de números: ";
    listNum.show();

    return 0;
}
*/
