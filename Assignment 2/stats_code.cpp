/*
Abdulrahman Derbala
400301521
*/
#include <iostream>
#include <math.h>
using namespace std;

// Define classes here 
class Stats{
    public:
        Stats(int *arr, int len){
            length = len;
            array1 = new int[length];
            max = *arr;
            for(int i = 0; i < length; i++,arr++){
                array1[i] = *arr;
                if(array1[i]>max){max = array1[i];}
            }
        };
        virtual ~Stats(){
            delete [] array1;
        };
        double getMean() const{return mean;};
        double getStdev() const{return stdev;};
        virtual void computeStats(){
            double sum = 0;
            for(int i = 0; i < length; i++){
                sum+=array1[i];
            }
            mean = sum/length;
            double var = 0.0;
            for (int i=0; i<length; i++){
                var += pow(array1[i] - mean, 2);
            }
            var = var / length; // variance
            stdev = sqrt(var); // standard deviation
        };
        virtual void printStats(){cout << "mean = " << mean <<", stdev = " << stdev;};
    protected:
        int length;
        int *array1;
        int max;
    private:
        double mean, stdev;
};

class OneVarStats : public Stats{
    public:
        OneVarStats(int *arr, int len) : Stats(arr,len){
            histLength = 1 + Stats::max;
            hist = new int[histLength];
        };
        ~OneVarStats(){
            delete [] hist;
        }
        void computeStats(){
            Stats::computeStats();
            int count;
            for(int i = 0; i < histLength; i++){
                count = 0;
                for(int j = 0; j < Stats::length; j++){
                    if(Stats::array1[j]==i){count++;}
                }
                hist[i] = count;
            }
        }
        void printStats(){
            Stats::printStats();
            cout << endl;
            for(int i = 0; i < histLength; i++){
                cout << hist[i] << " ";
            }
            cout << endl;
        }
    private:
        int histLength;
        int *hist;
};

class TwoVarStats : public Stats{
    public:
        TwoVarStats(int *arr1, int *arr2, int len) : Stats(arr1,len){
            array2 = new int[len];
            for(int i = 0; i < len; i++,arr2++){
                array2[i] = *arr2;
            }
            corr_coef = computeCorrCoef();
        };
        ~TwoVarStats(){
            delete [] array2;
        }
        void computeStats(){
            Stats::computeStats();
        }
        void printStats(){
            Stats::printStats();
            cout << endl;
            cout << "corr coef = " << corr_coef;
            cout << endl;
        }
    private:
        int *array2;
        double computeCorrCoef();
        double corr_coef;
};

// the correlation coefficient function 
double TwoVarStats::computeCorrCoef() {  // works on array1 and array2 and count
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0;  // hold S(x), S(y), S(xy) respectively.
    double sumX2 = 0.0, sumY2 = 0.0;  // hold S(x^2), S(y^2) respectively.
    int count = Stats::length;
    for (int i=0; i< count; i++){
        sumX += array1[i];
        sumY += array2[i];
        sumXY += array1[i] * array2[i];

        sumX2 += array1[i] *  array1[i];
        sumY2 += array2[i] *  array2[i];
    } 
    double corr_coef = (count * sumXY - sumX * sumY)/ (sqrt((count * sumX2 - sumX * sumX) * (count * sumY2 -  sumY * sumY))); 

    return corr_coef;
}


int main( ) {
    int x[] = {2, 4, 7, 11, 5};
    int y[] = {5, 9, 14, 20, 10};
    int z[] = {14, 7, 4, 9, 21};

    int stats_len = 4;
    Stats* pp[stats_len];
    pp[0] = new Stats(x, 5);
    pp[1] = new OneVarStats (x,  5);
    pp[2] = new TwoVarStats (x, y, 5);
    pp[3] = new TwoVarStats (y, z, 5);

    for (int i=0; i < stats_len; i++){
        pp[i]->computeStats();
        cout << "\n";
    }

    for (int i=0; i < stats_len; i++){
        pp[i]->printStats();
        cout << "\n";
    }
}