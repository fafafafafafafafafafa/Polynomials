#include <iostream>
#include <regex>
#include <list>
#include <cmath>
#include <windows.h>

#include "polynomial_part.h"
#include "operations.h"
#include "ordering.h"
#include "polynomial_adder.h"
#include "polynomial_subtractor.h"
#include "polynomial_scalar_multiplication.h"

using namespace std;


list<Polynomial_part> parse_input(string input) {
    regex parts_regex("\\-?(?:\\d+x?|x)(?:\\^\\-?\\d+)?");
    auto expression_begin = sregex_iterator(input.begin(), input.end(), parts_regex);
    auto expression_end = sregex_iterator();

    list<Polynomial_part> parts;

    for (sregex_iterator i = expression_begin; i != expression_end; ++i) {
        string expression = (*i).str();
        bool is_constant = expression.find("x") == string::npos;

        regex inner_parts_regex("(\\-?\\d+|\\-|^)(\\-?\\d*)");
        auto number_begin = sregex_iterator(expression.begin(), expression.end(), inner_parts_regex);
        auto number_end = sregex_iterator();

        for (sregex_iterator i = number_begin; i != number_end; ++i) {
            double coeff;
            //double coeff = (*i).str() == "-" ? -1 : stod((*i).str());
            if ((*i).str() == "-") coeff = -1;
            else if ((*i).str() == "") coeff = 1;
            else coeff = stod((*i).str());

            if (!is_constant) ++i;
            int parsed_power = i == number_end
                ? 1
                : stoi((*i).str());

            int power = is_constant ? 0 : parsed_power;
            // cout << "c -> " << coeff << " power -> " << power << " const: " << is_constant << endl;

            Polynomial_part polynom_into_list(coeff,power);
            parts.push_back(polynom_into_list);

            if (i == number_end) break;
        }
    }

    return parts;
}

bool should_remove (Polynomial_part part) {
        if(part.get_coeff() == 0) return 1;
        return 0;
    }

    bool compare(Polynomial_part part1, Polynomial_part part2){
        if(part1.get_power()>part2.get_power()) return 1;
        return 0;
    }

class Polynomial_scalar_multiplication {
private:
    list<Polynomial_part> internal_scalar_multiply(list<Polynomial_part> list1, double scalar){
        list<Polynomial_part> result_list;
        for(list<Polynomial_part>::iterator i = list1.begin(); i!=list1.end(); i++){
            Polynomial_part new_polynom_part((*i).get_coeff()*scalar,(*i).get_power());
            result_list.push_back(new_polynom_part);
        }
        return result_list;
    }
public:
    Polynomial_scalar_multiplication() {};
    list<Polynomial_part> operate(list<Polynomial_part> list1, double scalar){
        list<Polynomial_part> res = this->internal_scalar_multiply(list1, scalar);
        return res;
    }
};

class Polynomial_multiplication: virtual Ordering{
private:
    list<Polynomial_part> internal_multiply(list<Polynomial_part> list1, list<Polynomial_part> list2){
        list<Polynomial_part> result_list;
        for(list<Polynomial_part>::iterator i = list1.begin(); i!=list1.end(); i++){
            for(list<Polynomial_part>::iterator j = list2.begin(); j!=list2.end(); j++){
                Polynomial_part new_polymon_part((*i).get_coeff()*(*j).get_coeff(),(*i).get_power()+(*j).get_power());
                result_list.push_back(new_polymon_part);
            }
        }
        return result_list;
    }
public:
    Polynomial_multiplication(){};
    list<Polynomial_part> operate (list<Polynomial_part> list1, list<Polynomial_part> list2){
        list<Polynomial_part> res = this->internal_multiply(list1, list2);
        return order(res);
    }
};

class Polynomial_input {
private:
    list<Polynomial_part> polynomial;
public:
    Polynomial_input() {};

    void get_input(){
        int max_power=0;
        string input;
        cin>>input;

        list<Polynomial_part> list_unsorted = parse_input(input);
        for (list<Polynomial_part>::iterator i = list_unsorted.begin(); i!=list_unsorted.end();i++){
            if (max_power<(*i).get_power()) max_power = (*i).get_power();
        }

        this->polynomial = list_unsorted;
    }
    list<Polynomial_part> get_polynomial(){
        return this->polynomial;
    }


    void print() {
        for (list<Polynomial_part>::iterator i = this->polynomial.begin(); i!=this->polynomial.end();i++){
            (*i).print();
        }

        cout << endl;
    }
};

void check_for_given_x(list<Polynomial_part> p_list){
    double x;
    double res=0;
    cout<<"What should be substituted for x?"<<endl;
    cin>>x;

    for(list<Polynomial_part>::iterator i = p_list.begin(); i!=p_list.end(); i++){
        res=res+(*i).get_coeff()*pow(x,(*i).get_power());
    }
    cout<<"result is: "<<res<<endl;
}

void print(list<Polynomial_part> list1){
    int i = 0;
    for(auto elem:list1){
        if(i==0){
            if (elem.get_power()==0) cout<<elem.get_coeff();
             else if (elem.get_power()==1) cout<<elem.get_coeff()<<"x";
             else cout<<elem.get_coeff()<<"x^"<<elem.get_power();
        }else{
            if(elem.get_coeff()>0){
                if(elem.get_power()==0 && elem.get_coeff()==1) cout<<"+"<<elem.get_coeff();
                else if(elem.get_power()==0) cout<<"+"<<elem.get_coeff();
                else if(elem.get_power()==1 && elem.get_coeff()==1) cout<<"+x";
                else if (elem.get_coeff()==1) cout<<"+x^"<<elem.get_power();
                else cout<<"+"<<elem.get_coeff()<<"x^"<<elem.get_power();
            }else{
                if(elem.get_power()==0) cout<<elem.get_coeff();
                else if(elem.get_power()==1 && elem.get_coeff()==-1) cout<<"-x";
                else if(elem.get_power()==1) cout<<elem.get_coeff()<<"x";
                else if (elem.get_coeff()==1) cout<<"x^"<<elem.get_power();
                else cout<<elem.get_coeff()<<"x^"<<elem.get_power();
            }
        }
        i++;
    }
}

int switcharoo(){
    double a;
    Polynomial_input input;
    Polynomial_input input2;

    cout<<"Enter the first polynomial: "<<endl;
    input.get_input();

    cout<<"Enter the second polynomial: "<<endl;
    input2.get_input();

    Polynomial_adder adder;
    Polynomial_subtractor subtractor;
    Polynomial_scalar_multiplication scalar_mult;
    Polynomial_multiplication mult;

    map <string, int> options;
    options["add"] = 1;
    options["subtract"] = 2;
    options["scalar"] = 3;
    options["mult"] = 4;
    options["m_find"] = 5;
    options["x_find"] = 6;
    options["exit"] = 7;


    once_again:

    string c;
    cout<<"What operation do you choose?"<<endl;
    cout<<"list of possible operations:\n\tadd - adds two polynomials\n\tsubtract - subtracts polynomial 1 from polynomial 2\n\tscalar - performs scalar multiplication of chosen polynomial"<<endl;
    cout<<"\tmult - performs multiplication for given polynomials\n\tm_find - find result of multiplication by substituting x\n\tx_find - find result of chosen polynomial by substituting x\n\texit - exits program"<<endl<<endl;
    cin>>c;

switch (options[c]){
    case 1: {
        list<Polynomial_part>zmienna = adder.operate(input.get_polynomial(), input2.get_polynomial());
        print(zmienna);
    }break;

    case 2: {
        list<Polynomial_part>zmienna = subtractor.operate(input.get_polynomial(), input2.get_polynomial());
        print(zmienna);
    }break;

    case 3: {
        int a;
        double scalar;
        cout<<"What scalar?"<<endl;
        cin>>scalar;
        cout<<"which should we check? 1 or 2"<<endl;
        do{
            cout<<"Write number: "<<endl;
            cin>>a;
        }while(!(a==1 || a==2));
        if (a == 1) {
            list<Polynomial_part>zmienna = scalar_mult.operate(input.get_polynomial(), scalar);
            print(zmienna);
        }
        if (a == 2) {
            list<Polynomial_part>zmienna = scalar_mult.operate(input2.get_polynomial(), scalar);
            print(zmienna);
        }

    }break;

    case 4: {
        list<Polynomial_part>zmienna = mult.operate(input.get_polynomial(), input2.get_polynomial());
        print(zmienna);
    }break;

    case 5: {
        list<Polynomial_part>zmienna = mult.operate(input.get_polynomial(), input2.get_polynomial());
        check_for_given_x(zmienna);
    }break;

    case 6: {
        int a;
        cout<<"which should we check? 1 or 2"<<endl;
        do{
            cout<<"Write number: "<<endl;
            cin>>a;
        }while(!(a==1 || a==2));
        if (a == 1) check_for_given_x(input.get_polynomial());
        if (a == 2) check_for_given_x(input2.get_polynomial());
    }break;

    case 7: {
        cout<<"Bye bye!"<<endl;
        Sleep(5000);
        return 0;
    }break;

}
    char co;
    cout<<endl<<"Once again? Y/N"<<endl;
    cin>>co;
    cout<<"......................"<<endl;
    if (co=='Y' || co == 'y') goto once_again;
    if (co=='N' || co == 'n') {
            cout<<"Bye bye!"<<endl;
            Sleep(5000);
            return 0;
    }
}

int main()
{

switcharoo();
    return 0;
}

