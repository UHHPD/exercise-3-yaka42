#include <iostream>
#include <fstream>
#include <vector>


double poisson(double mu, int k) {
    double oben = pow(mu,k) * exp(-mu);
    double unten = tgamma(k+1);
    return oben/unten;
}


double prob2(std::vector<int> daten){
    double likli = 1.0;
    for(int k : daten){
        likli = likli * poisson(k, k);
    }
    return likli;
}

double prob(std::vector<int> daten, double mu){
    double likli = 1.0;
    for(int k : daten){
        likli = likli * poisson(mu, k);
    }
    return likli;
}

int main() {
    using namespace std;


    ifstream fin("datensumme.txt");
    vector<int> daten;
    int n_i;
    for(int i = 0 ; i < 234 ; ++i) {
        fin >> n_i;
        daten.push_back(n_i);
    }
    fin.close();
    std::cout << prob(daten, 3.11538) << std::endl;

    ofstream fout("likelihood.txt");
    for( int i = 0; i < 61; i++){
        fout << i* 0.1 << " : " << prob(daten, i * 0.1 )<< endl;
    }
    fout.close();

    ofstream fout2("nll.txt");
    for( int i = 0; i < 61; i++){
        fout2 << i* 0.1 << " : " << -2*log(prob(daten, i * 0.1 ))<< endl;
    }
    fout2.close();

    ofstream fout3("deltanll.txt");
    double min = 10000.0;
    double min_mu; 
    for( int i = 0; i < 61; i++){
        fout3 << i* 0.1 << " : " << -2*log(prob(daten, i * 0.1 )) + 2*log(prob(daten, 3.11538)) << endl;
        if(min > -2*log(prob(daten, i * 0.1 )) + 2*log(prob(daten, 3.11538))){
            min = -2*log(prob(daten, i * 0.1 )) + 2*log(prob(daten, 3.11538));
            min_mu = i* 0.1;
        }
    }
    cout << "Mimimum: " << min_mu << endl;
    fout3.close();

    for( int i = 0; i < 601; i++){
        if(-2*log(prob(daten, i * 0.01 )/prob(daten, min_mu)) < 1){
            cout << i*0.01 << endl;
        }
    }

    std::ifstream fin3("datensumme.txt");
    double summe = 0.0;
    int a;
    int N = 0;
    for(int i=1; i<235; i++){ // loop through lines
        fin >> a;
        summe += a;
        //std::cout << summe << std::endl;
    }
    fin3.close();
    double mean = summe/234;

    std::ifstream fin2("datensumme.txt");
    double variance_helper = 0;
    for(int i=1; i<235; i++){ // loop through lines
    fin2 >> a;
    variance_helper += pow(a - mean,2);
    //std::cout << summe << std::endl;
    }
    fin2.close();
    double variance = variance_helper/ 234;
    cout << "uncertainty on the sample mean: " << sqrt(variance)/sqrt(234.0) << endl;


    ofstream fout4("lambda.txt");
    for( int i = 0; i < 61; i++){
        fout4 << i* 0.1 << " : " << -2*log(prob(daten, i * 0.1 )/prob2(daten))<< endl;
    }
    fout4.close();

    ofstream fout5("Abweichung.txt");
    for( int i = 0; i < 61; i++){
        fout5 << i* 0.1 << " : " << (-2*log(prob(daten, i * 0.1 )/prob2(daten)) - 233)/ sqrt(233)<< endl;
    }
    fout5.close();
}