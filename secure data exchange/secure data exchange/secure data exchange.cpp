/*
- Создайте класс Data, содержащий в качестве полей скалярные данные и мьютекс.- Создайте функцию swap, которая принимает ссылки на два объекта класса Data и обменивает их местами.
- В функциях нужно сначала захватить мьютексы обоих объектов, а затем выполнить обмен данными.
- Реализуйте три варианта этой функции: при помощи lock, scoped_lock и unique_lock.------
*/


#include <mutex>
#include<iostream>

class Data 
{
 public:
    std::mutex mutex;
    Data(int value) : value(value) {}

    int getValue() 
    {
        
        return value;
    }
    void setValue(int newValue)
    {
        
        value = newValue;
    }
    void outData()
    {
        std::cout << value;
    }
 private:
    int value;    
};

void swapGL(Data& a, Data& b) 
{
    std::lock_guard<std::mutex> lockA(a.mutex);
    std::lock_guard<std::mutex> lockB(b.mutex);

    int tempValue = a.getValue();
    a.setValue(b.getValue());
    b.setValue(tempValue); 
    // следующая строка только для вывода результата
    std::cout << "swapGL: data1 = " << a.getValue() << "; data2 = " << b.getValue() << ";" << std::endl;   
}

void swapSL(Data& a, Data& b) 
{
    std::scoped_lock<std::mutex, std::mutex> lock(a.mutex, b.mutex);

    int tempValue = a.getValue();
    a.setValue(b.getValue());
    b.setValue(tempValue);  
    // следующая строка только для вывода результата
    std::cout << "swapSL: data1 = " << a.getValue() << "; data2 = " << b.getValue() << ";" << std::endl;   
}

void swapUL(Data& a, Data& b) 
{
    std::unique_lock<std::mutex> lockA(a.mutex, std::defer_lock);
    std::unique_lock<std::mutex> lockB(b.mutex, std::defer_lock);
    std::lock(lockA, lockB);

    int tempValue = a.getValue();
    a.setValue(b.getValue());
    b.setValue(tempValue);    
    // следующая строка только для вывода результата
    std::cout << "swapUL: data1 = " << a.getValue() << "; data2 = " << b.getValue() << ";" << std::endl;    
}

int main()
{
    Data data1(11);
    Data data2(99);

    std::cout << "Input data1 = " << data1.getValue() << "; data2 = " << data2.getValue() << ";" << std::endl;
    std::thread T1(swapGL, std::ref(data1), std::ref(data2));
    std::thread T2(swapSL, std::ref(data1), std::ref(data2));
    std::thread T3(swapUL, std::ref(data1), std::ref(data2));
    T1.join();
    T2.join();
    T3.join();

    return 0;
}