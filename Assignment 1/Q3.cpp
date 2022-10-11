/*
Name: Abdulrahman Derbala
MacID: derbalaa
Student#: 400301521
*/
#include <iostream>
#include <math.h>
using namespace std;
class Point{
    private:
        double x;
        double y;
    public:
        Point(){//constructor
            x = 0;
            y = 0;
        }
        Point(double x1,double y1){
            x = x1;
            y = y1;
        }
        double get_x() const{return x;}
        double get_y() const{return y;}
        double distanceTo(Point otherP){
            return sqrt(pow(x-otherP.x,2)+pow(y-otherP.y,2));
        }
        friend istream &operator>>(istream &inp, Point &p1){
            cout << "Please enter the x value: ";
            inp >> p1.x;
            cout << "Now the y value: ";
            inp >> p1.y;
            return inp;
        };
        friend ostream &operator<<(ostream &out, const Point &p1){
            out << "P(" << p1.x << "," << p1.y << ")";
            return out;
        };
};
class Polygon{
    private:
        int numOfPoints;
        Point *points;
    public:
        Polygon(int numOfPoints1 = 10){
            numOfPoints = numOfPoints1;
            points = new Point[numOfPoints];
        }
        Polygon(int numOfPoints1,Point *ptr){
            numOfPoints = numOfPoints1;
            points = new Point[numOfPoints];
            for(int i = 0;i < numOfPoints;i++){
                points[i] = Point(ptr[i].get_x(),ptr[i].get_y());
            }
        }
        ~Polygon(){
            delete [] points;
        }
        double perimeter(){
            double perimeter = 0;
            for(int i = 0;i < numOfPoints;i++){
                if(i == numOfPoints-1)
                    perimeter += points[i].distanceTo(points[0]);
                else
                    perimeter += points[i].distanceTo(points[i+1]);
            }
            return perimeter;
        }
        friend istream &operator>>(istream &inp, Polygon &poly){
            for(int i = 0;i<poly.numOfPoints;i++){
                cout << "Point " << i+1 << ":\n";
                inp>>poly.points[i];
            }
            return inp;
        }
        friend ostream &operator<<(ostream &out, const Polygon &poly){
            for(int i = 0;i<poly.numOfPoints;i++){
                out<<""<<poly.points[i]<<" ";
            }
            return out;
        }
};
int main(){
    Point p1;
    cin>>p1;
    cout<<p1<<endl;
    
    Point* points_list = new Point[4];
    points_list[0] = Point(0, 0);
    points_list[1] = Point(0, 1);
    points_list[2] = Point(1, 1);
    points_list[3] = Point(1, 0);
    Polygon rectangle(4, points_list);
    cout<<rectangle.perimeter()<<endl;
    Polygon poly2(4);
    cin >> poly2;
    cout << poly2<<endl;
    cout<<poly2.perimeter()<<endl;
    return 0;
}