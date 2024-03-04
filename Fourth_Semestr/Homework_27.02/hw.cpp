#include <iostream>
#include <string>

using namespace std;
class DateTimeSpan;

class DateTime {
    double date;
    friend class DateTimeSpan;
    friend DateTimeSpan operator-(const DateTime& d, const DateTime& d1);
    friend DateTime operator+(const DateTime& d, const DateTimeSpan& s);
    friend DateTime operator-(const DateTime& d, const DateTimeSpan& s);
public:
     DateTime(double data){
            date = data;
    }
    DateTime(string dateStr){
        int day = stoi(dateStr.substr(0, 2));
        int month = stoi(dateStr.substr(3, 2));
        int year = stoi(dateStr.substr(6, 4));
        int hour = stoi(dateStr.substr(11, 2));
        int minute = stoi(dateStr.substr(14, 2));
        int second = stoi(dateStr.substr(17, 2));
        date = 0;
        double countSeconds;
        if(year >= 2000){
            for(int i =year-2000; i>0; i--){ //для годов, положительных
                date += 365;
                if(i % 4 == 1){
                        if(i % 100 != 1)
                            date += 1;
                        else 
                            if (i % 400 == 1)
                                date += 1;
                }
            }
            for(int i=month-1; i>0; i--){ //для месяцов, положительных
                switch(i){
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        date += 30;
                        break;
                    case 2:
                        date += 28;
                        if(year % 4 == 0){
                            if(year % 100 != 0)
                                date += 1;
                            else if (year % 400 == 0)
                                date += 1;
                        }
                        break;
                    default:
                        date += 31;
                        break;
                }
            }
            date += day;
            countSeconds = (hour*60 + minute)*60 + second;
        } else {
                for(int i =2000-year; i>1; i--){ //для годов, отрицательных
                    date += 365;
                    if(i % 4 == 3){
                            if(i % 100 != 3)
                                date += 1;
                            else if (i % 400 == 3)
                                date += 1;
                    }
                }
                for(int i=month+1; i<=12; i++){ //для месяцов, отрицательных
                switch(i){
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        date += 30;
                        break;
                    case 2:
                        date += 28;
                        if(year % 4 == 0){
                            if(year % 100 != 0)
                                date += 1;
                            else if (year % 400 == 0)
                                date += 1;
                        }
                        break;
                    default:
                        date += 31;
                        break;
                }
            }
            switch(month){ //для дней, отрицательных
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        date += 30-day;
                        break;
                    case 2:
                        date += 28-day;
                        if(year % 4 == 0){
                            if(year % 100 != 0)
                                date += 1;
                            else if (year % 400 == 0)
                                date += 1;
                        }
                        break;
                    default:
                        date += 31-day;
                        break;
            }
            countSeconds = ((23-hour)*60 + (59-minute))*60 + (60-second);
        }
        date += countSeconds/(24*3600) - 1;
        if(year < 2000) date = -date;
    }
    
    double returnDateFloat() const {return date;}
    string returnDateStr() const {
        double data = date;
        int day, month, year, hour, minute, second;
        year = 2000;
        if(data < 0){ //меньше 2000
            year--;
            data = -data;
            bool overYears = false;
            while((int(data)/365 > 0)&&(!overYears)){
                year--;
                if(year % 4 == 0){
                    if(year % 100 != 0)
                            if(int(data)/366 > 0) data -= 1;
                            else overYears = true;
                    else 
                        if (year % 400 == 0){
                            if(int(data)/366 > 0) data -= 1;
                            else overYears = true;
                        }
                }
                data -= 365;
            }
            month = 12;
            bool overDays = false;
            while ((int(data)/28 > 0)&&(!overDays)){
                switch(month){
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        if(int(data) >= 30)data -= 30;
                        else overDays = true;
                        break;
                    case 2:
                        if(int(data) >= 28) {
                            data -= 28;
                            if(year % 4 == 0){
                                if(year % 100 != 0){
                                    if(int(data) >= 1) data -= 1;
                                    else overDays = true;
                                }
                                else if (year % 400 == 0){
                                    if(int(data) >= 1) data -= 1;
                                    else overDays = true;
                                }
                            }
                        } else overDays = true;
                        break;
                    default:
                        if(int(data) >= 31)
                            data -= 31;
                        else overDays = true;
                        break;
                }
                if(!overDays) month -= 1;
            }
            switch(month){ //для дней, отрицательных
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        day = 30 - int(data-1);
                        break;
                    case 2:
                        day = 28 - int(data-1);
                        if(year % 4 == 0){
                            if(year % 100 != 0)
                                day += 1;
                            else if (year % 400 == 0)
                                day += 1;
                        }
                        break;
                    default:
                        day = 31 - int(data-1);
                        break;
            }
            int days = int(data);
            int countSeconds = (data-days) * 24 * 3600;
            second = countSeconds % 60;
            int countMinutes = (countSeconds-second)/60;
            minute = countMinutes % 60;
            hour = (countMinutes - minute)/60;
            second = (59 - second)%60;
            minute = (59 - minute)%60;
            hour = (23 - hour)%24;
        }else{ //больше 2000
            data++;
            bool overYears = false;
            while ((int(data)/365 > 0)&&(!overYears)){
                year++;
                if(year % 4 == 1){
                    if(year % 100 != 1)
                            if(int(data)/366 > 0) data -= 1;
                            else overYears = true;
                    else {
                        if (year % 400 == 1){
                            if(int(data)/366 > 0) data -= 1;
                            else overYears = true;
                        }
                    }
                }
                data -= 365;
            }
            month = 1;
            bool overDays = false;
            while ((int(data)/28 >= 0)&&(!overDays)){
                switch(month){
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        if(int(data) > 30)data -= 30;
                        else overDays = true;
                        break;
                    case 2:
                        if(int(data) > 28) {
                            data -= 28;
                            if(year % 4 == 0){
                                if(year % 100 != 0){
                                    if(int(data) > 1) data -= 1;
                                    else overDays = true;
                                }
                                else if (year % 400 == 0){
                                    if(int(data) > 1) data -= 1;
                                    else overDays = true;
                                }
                            }
                        } else overDays = true;
                        break;
                    default:
                        if(int(data) > 31)
                            data -= 31;
                        else overDays = true;
                        break;
                }
                if(!overDays) month += 1;
            }
            day = (data) / 1;

            int countSeconds = (data-day) * 24 * 3600;
            second = countSeconds % 60;
            int countMinutes = (countSeconds - second)/60;
            minute = countMinutes % 60;
            hour = (countMinutes - minute)/60;
        }
        
        //красивый вывод, itos
        string dateStr = "";
        if(day < 10) dateStr += "0";
        dateStr += to_string(day) + ".";
        if(month < 10) dateStr += "0";
        dateStr += to_string(month) + "." + to_string(year) + " ";
        if(hour < 10) dateStr += "0";
        dateStr += to_string(hour) + ":";
        if(minute < 10) dateStr += "0";
        dateStr += to_string(minute) + ":";
        if(second < 10) dateStr += "0";
        dateStr += to_string(second);
        return dateStr;
    }

};

class DateTimeSpan{
    double span;
    friend class DateTime;
    friend DateTime operator+(const DateTime& d, const DateTimeSpan& s);
    friend DateTime operator-(const DateTime& d, const DateTimeSpan& s);
    public:
    DateTimeSpan(int days = 0, int hours = 0, int minutes = 0, int seconds = 0){
        span=0;
        double time = seconds + minutes*60  + hours*3600;
        span+=days+time/(24*3600);
    }
    DateTimeSpan(double span1=0){
        span = span1;
    }
    
    void printSpan(){
        cout << span << endl;
    }
    DateTimeSpan operator-() const {
        return DateTimeSpan{-span};
    };
    DateTimeSpan operator-(const DateTimeSpan& sp) const {
        return DateTimeSpan{span-sp.span};
    };
    DateTimeSpan operator+(const DateTimeSpan& sp) const {
        return DateTimeSpan{span+sp.span};
    };
};
    DateTimeSpan operator-(const DateTime& d, const DateTime& d1) {
        double new_span = d.date - d1.date;
        return DateTimeSpan{new_span};
    };
    DateTime operator+(const DateTime& d, const DateTimeSpan& s){
        cout << "Это для отладки " << d.date << "  " << s.span << endl;
        double new_date = d.date + s.span;
        cout << new_date << endl;
        return DateTime{new_date};
    };
    DateTime operator-(const DateTime& d, const DateTimeSpan& s){
        return DateTime{d.date - s.span};
    };

void testFunctionDateTime(){
    string a;
    getline(cin, a);
    DateTime d(a);
    cout << "Представление в double: " << d.returnDateFloat() << endl;
    cout << "Представление в строке: " << d.returnDateStr() << endl;
    
}
/*DateTime d, d1 и DateTimeSpan s, s1 выполнялось следующее:
s = d1 - d; // промежуток времени между d1 и d
d = d1 + s; // d1 отстоит от d на промежуток времени s
d = d1 - s; // d1 отстоит от d на промежуток времени s
s = -s;
s = s - s1; // промежуток s уменьшился на промежуток s1
s = s + s1; // промежуток s увеличился на промежуток s1*/
void testFunctionDateTimeSpan(){
    cout<<"Введите первую дату, затем вторую" << endl;
    string a, b;
    getline(cin, a);
    getline(cin, b);
    DateTime date(a);
    cout << date.returnDateStr() << endl;
    DateTime date1(b); 
    cout << date1.returnDateStr() << endl; 
    cout << "начало тестирования DateTimeSpan" << endl;
    DateTimeSpan span(date1 - date);
    date = date1 + span;
    cout << date.returnDateStr() << endl;
    date = date1 - span;
    cout << date.returnDateStr() << endl;
    span = -span;
    span.printSpan();
    DateTimeSpan span1(10);
    span = span - span1;
    span.printSpan();
    span = span + span1;
    span.printSpan();
}
int main(){
    const char * arr[] = {"02.01.2000 00:00:00",
                "03.01.2000 02:24:00",
                "02.01.2000 18:00:00",
                "31.12.1999 00:00:00",
                "30.12.1999 12:00:00",
                "30.12.1999 18:00:00",
                "30.12.1999 21:36:00"
                };
    for(int i=0; i<7; i++){
        DateTime d(arr[i]);
        cout << arr[i] << " " << d.returnDateFloat() << endl;
    }
    cout << "Окончание тестирования класса DateTime" << endl;

    for(int i=0; i<1; i++)
        testFunctionDateTimeSpan();
    cout << "Окончание тестирования класса DateTimeSpan" << endl;

}
/*1.0 <=> 2.01.2000 00:00:00 
2.1 <=> 3.01.2000 02:24:00
1.75 <=> 02.01.2000 18:00:00
-1.0 <=> 31.12.1999 00:00:00
-1.5 <=> 30.12.1999 12:00:00
-1.25 <=> 30.12.1999 18:00:00
-1.1 <=> 30.12.1999 21:36:00*/