#include <iostream>

class Vector{
    int *vector;
    int begin, size;
public:
    // френды класса, а именно перегруженные операторы <<, >>
    friend std::ostream& operator << (std::ostream& stream, const Vector& v);
    friend std::istream& operator >>(std::istream& stream, const Vector& v);
    // инициализация Vector-а
    Vector(int b1, int e1){
        size = e1-b1;
        vector = new int[size];
        begin = b1;
    }
    Vector(int e1){
        size = e1;
        vector = new int[size];
        begin = 0;
    }
    Vector(int * arr, int b1, int e1){
        size = e1-b1;
        begin = b1;
        vector = new int[size];
        for(int i=0; i<size; i++) 
            vector[i] = arr[i];
    }
    // инкапсуляция в классе 
    int returnSize() const{ return size;}
    int returnBegin() const { return begin;}
    // перегрузка операции [] для нормального взаимодействия с элементами
    int& operator[](unsigned i) const{
        return vector[(i-begin)%size];
    }
    // перегрузка операции +=, +, -=, -
    Vector operator += (const Vector& v){
        int* new_vector = new int[size];
        for(int i=begin; i < begin+size; i++){
            new_vector[i-begin] = vector[i-begin] + v[i];
        }
        return Vector{new_vector, begin, begin+size};
    }
    Vector operator + (const Vector& v){ return *this += v;}
    Vector operator -= (const Vector& v){
        int* new_vector = new int[size];
        for(int i=begin; i < begin+size; i++){
            new_vector[i-begin] = vector[i-begin] - v[i];
        }
        return Vector{new_vector, begin, begin+size};
    }
    Vector operator - (const Vector& v){ return *this -= v;}
    // перегрузка операции *, *=, /, /= дял скаляра
    Vector operator *= (const int& a){
        int* new_vector = new int[size];
        for(int i=0; i < size; i++){
            new_vector[i] = vector[i] * a;
        }
        return Vector{new_vector, begin, begin+size};
    }
    Vector operator * (const int& a){ return *this *= a;}
    Vector operator /= (const int& a){
        int* new_vector = new int[size];
        for(int i=0; i < size; i++){
            new_vector[i] = vector[i] / a;
        }
        return Vector{new_vector, begin, begin+size};
    }
    Vector operator / (const int& a){ return *this /= a;}
};
std::ostream& operator << (std::ostream& stream, const Vector& v){
    for(int i=v.returnBegin(); i<v.returnBegin() + v.returnSize(); i++){
        stream << v[i];
        stream << " ";
    }
    return stream;
}

std::istream& operator >> (std::istream& stream, const Vector& v){
    for(int i=v.returnBegin(); i<v.returnBegin() + v.returnSize(); i++){
        stream >> v[i];
    }
    return stream;
}
int main(){
    int begin, end, i, a, b;
    std::cout << "Введите начальный индекс векторов ";
    std::cin >> begin;
    std::cout << "Введите конечный индекс векторов ";
    std::cin >> end;
    Vector vector1{begin, end};
    Vector vector2{begin, end};
    std::cout << "Теперь перейдем к заполнению первого массива" << std::endl;
    std::cin >> vector1;
    std::cout << "Теперь перейдем к заполнению второго массива" << std::endl;
    std::cin >> vector2;
    std::cout << "Теперь проведем тесты для класса Vector "<<std::endl;
    std::cout << "Первый тест - обратимся к элементу первого массива с выбранными нами границами вектора "<<std::endl;
    std::cout << "Введите число, чтобы выполнилось vector1[i], где i - введенное число ";
    std::cin >> i;
    std::cout << vector1[i] << std::endl;
    std::cout << "Второй тест - возможность выполнения поэлементного сложения и вычитания" << std::endl;
    std::cout << "Поэлементное сложение  - " << vector1+vector2 << std::endl;
    std::cout << "Поэлементное вычитание - " << vector1-vector2 << std::endl;
    std::cout << "Третий тест - возможность выполнения умножения и деления вектора на скаляр"<<std::endl;
    std::cout << "Введите число, на которое будет умножен vector1 ";
    std::cin >> a;
    std::cout << "Результат умножения вектора " << vector1 * a << std::endl;
    std::cout << "Введите число, на которое будет поделен vector2 ";
    std::cin >> b;
    std::cout << "Результат деления вектора " <<vector2 / a << std::endl;
    std::cout << "Тесты вывода на экран одного элемента и целого массива продемонстрированы" << std::endl;
    std::cout << "Конец тестирования" << std::endl; 
}