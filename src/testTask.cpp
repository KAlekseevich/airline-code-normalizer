#include <iostream>
#include <vector>

//проверяем, что максимальный размер строки - 7 символов 
bool isValidLength(const std::string& s) {
    return s.size() <= 7;
}

//проверяем, есть ли буква в строке
bool hasAlpha(const std::string& s) {
    for (char c : s) 
        if (isalpha(c)) return true;

    return false;
}

//проверяем, состоит ли строка только из цифр
bool isAllDigits(const std::string& s) {
    for (char c : s) 
        if (!isdigit(c)) return false;

    return !s.empty();
}

//проверяем, что строка состоит из валидного кода и номера
bool isValidFormat(const std::string& str) {

    //проверяем, что строка состоит из допустимых символов (латиница, цифры, пробел)
    for (char simv : str) {
        if (!(isalpha(simv)) && !(isdigit(simv)) && simv != ' ')
            return false;
    }

    //случай, если строка содержит пробел
    size_t space_pos = str.find(' ');
    if (space_pos != std::string::npos) {
        //если пробелов больше 1 то строка сразу не подходит
        if (str.find(' ', space_pos + 1) != std::string::npos) return false;

        //делим по пробелу на код и номер
        std::string code = str.substr(0, space_pos);
        std::string number = str.substr(space_pos + 1);

        //подойдет лишь строка, чей код ровно 2 символа в длину и содержит хотя бы 1 букву, а номер состоит из 1-5 цифр
        return code.size() == 2 &&
            hasAlpha(code) &&
            number.size() >= 1 &&
            number.size() <= 5 &&
            isAllDigits(number);
    }
    //теперь рассмотрим случаи, когда строка не содержит пробел

    //случай, когда код отсутствует и есть лишь номер, состоящий только из цифр:
    if (isAllDigits(str)) {
        return str.size() <= 5;
    }

    //теперь будем искать строки, подходящие под другие правила (отбросим те строки, длина которых меньше 3 символов)
    if (str.size() < 3)
        return false;

    //у оставшихся строк возьмем первые 3 символа и будем их проверять
    std::string first3 = str.substr(0, 3);
    std::string other = str.substr(3);

    //0 букв мы уже рассмотрели

    //рассматриваем случаи, когда в first3 лишь 1 буква

    if ((isalpha(first3[0]) && isdigit(first3[1]) && isdigit(first3[2])) //код подходит - буква стоит лишь на 1 позиции
        || (isdigit(first3[0]) && isalpha(first3[1]) && isdigit(first3[2]))) { //код подходит - буква стоит лишь на 2 позиции 
        if (other.empty() || isAllDigits(other)) //номер содержится в first3 и может содержаться в other
            return true;
    }

    if (isdigit(first3[0]) && isdigit(first3[1]) && isalpha(first3[2])) ///если буква стоит лишь на 3 позиции, код не подходит 
        return false;

    //рассматриваем случаи, когда в first3 лишь 2 буквы

    if ((isdigit(first3[0]) && isalpha(first3[1]) && isalpha(first3[2])) // когда буква на 2 и 3 позиции, код не подходит
        || (isalpha(first3[0]) && isdigit(first3[1]) && isalpha(first3[2]))) // когда буква на 1 и 3 позиции, код не подходит
        return false;

    if (isalpha(first3[0]) && isalpha(first3[1]) && isdigit(first3[2])) { //когда буква на 1 и 2 позиции, код подходит
        if (other.empty() || isAllDigits(other)) //теперь проверяем номер
            return true;
    }

    if (isalpha(first3[0]) && isalpha(first3[1]) && isalpha(first3[2])) { // если все буквы, код подходит
        if (isAllDigits(other)) //теперь проверяем номер
            return true;
    }

    return false; //я разобрал все удовлетворяющие случаи. если не подошло, то строка некорректна

}

//убираем незначащие нули из номера
std::string deleteZeros(const std::string& str) {

    size_t firstNotZero = str.find_first_not_of('0');
    if (firstNotZero == std::string::npos)        
        return "0"; //случай если весь номер состоит из нулей
    
    return str.substr(firstNotZero);
}

//форматируем строку (убираем пробелы и незначащие нули)
std::string formateString(const std::string& str) {

    size_t space_pos = str.find(' ');
    if (space_pos != std::string::npos) {

        //делим по пробелу на код и номер
        std::string code = str.substr(0, space_pos);
        std::string number = str.substr(space_pos + 1);

        deleteZeros(number);
        return (code + number);
    }

    if (isAllDigits(str))
        return deleteZeros(str);

    if (isalpha(str[0]) && isalpha(str[1]) && isalpha(str[2])) {
        std::string code = str.substr(0, 3);
        std::string number = str.substr(3);
        return (code + deleteZeros(number));
    }

    std::string code = str.substr(0, 2);
    std::string number = str.substr(2);

    return (code + deleteZeros(number)); 
}

//главная функция для итогового сравнения строк
bool compareStrings(const std::string& str1, const std::string& str2){

   //сначала проверим, что обе строки не длиннее 7 символов
    auto validateLength = [](const std::string& s) {
        if (!isValidLength(s)) {
            //std::cerr << "Строка \"" << s << "\" длиннее 7 символов\n";
            return false;
        }
        return true;
        };

    if (!validateLength(str1) || !validateLength(str2)) return false;

    //далее проверим, что строки подходят под формат кода и номера
    auto validateFormat = [](const std::string& s) {
        if (!isValidFormat(s)) {
            //std::cerr << "Строка \"" << s << "\" не подходит под формат кода и номера\n";
            return false;
        }
        return true;
        };

    if (!validateFormat(str1) || !validateFormat(str2)) return false;
  
    //после всех проверок будем сравнивать строки
    return (formateString(str1) == formateString(str2));
              
}

struct TestCase {
    std::string a;
    std::string b;
    bool expected;
    std::string comment;
};

//функция для выполнения тестов
void runTests() {
    std::vector<TestCase> tests = {
        // ==================== Корректные форматы ====================
        {"AFL1", "AFL0001", true, "Незначащие нули в номере"},
        {"D2 25", "D225", true, "Пробел удаляется из кода"},
        {"AB123", "AB0123", true, "Незначащие нули в номере"},
        {"SU 123", "SU123", true, "Пробел удаляется из кода"},
             
        // ==================== Некорректные длины ====================
        {"ABCDEFGH", "A1", false, "Первая строка слишком длинная"},
        {"A1", "12345678", false, "Вторая строка слишком длинная"},
        {"123", "000123", false, "Номер должен быть не длиннее 5 цифр"},

        // ==================== Некорректные форматы ====================
        {"A!1", "A1", false, "Недопустимый символ в коде"},
        {"a12", "A12", false, "Буква в нижнем регистре"},
        {"AB 12 3", "AB123", false, "Больше 1 пробела недопустимо"},
        {"1A", "1A", false, "Номер отсутствует"},
        {"A1B2", "A1B02", false, "Смешанный код с цифрами"},

        // ==================== Специальные случаи ====================
        {"", "", false, "Пустые строки - можно пустой код, но не номер"},
        {"0", "0000", true, "Нулевые номера"},
        {"A", "A0", false, "Код vs код+номер"},
        {"12A", "12A", false, "Буква после нескольких цифр (невалидный формат)"},

        // ==================== Разные коды/номера ====================
        {"AB12", "CD12", false, "Разные коды"},
        {"AB123", "AB124", false, "Разные номера"},
        {"123", "124", false, "Разные номера без кода"}
    };

    for (const auto& test : tests) {
        std::string inputDesc = "\"" + test.a + "\" vs \"" + test.b + "\"";
        //выравнивание описания входа вручную до 25 символов
        if (inputDesc.length() < 25)
            inputDesc += std::string(25 - inputDesc.length(), ' ');

        //преобразуем булевы значения в строку
        std::string expectedStr = test.expected ? "true " : "false";
        std::string resultStr = compareStrings(test.a, test.b) ? "true " : "false";

        std::cout << "TEST: " << inputDesc
            << " Expected: " << expectedStr
            << " Result: " << resultStr
            << " \t[" << test.comment << "]"
            << (compareStrings(test.a, test.b) == test.expected ? " OK" : " FAIL")
            << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    runTests();
}
