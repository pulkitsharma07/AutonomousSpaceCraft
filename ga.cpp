#include <bits/stdc++.h>
using namespace std;

#define d(args...) do {cerr << #args << ": "; dbg,args; cerr << endl;} while(0)
struct debugger
{
    template<typename T> debugger& operator , (const T& v)
    {
        cout<<v<<" ";
        return *this;
    }
} dbg;

//returns a float between 0 & 1
#define RANDOM_NUM		((float)rand()/(RAND_MAX+1))

struct chromosome
{

    string s;

    double fitness;

    int gene_length;
    int feature_count;

    double mutation_rate;
    //chromosome of n random bits
    chromosome() {}
    chromosome(int m,int d):gene_length(m),feature_count(d)
    {

        for(int i = 0 ; i<m*d; i++)
        {
            if(RANDOM_NUM > 0.5)s+='1';
            else
                s+='0';
        }
        //cout<<s<<endl;
        mutation_rate = 0.001;
    }

    void mutate()
    {
        for(int i = 0 ; i<s.size(); i++)
        {
            if(RANDOM_NUM < mutation_rate)
            {
                s[i] =(s[i]=='1')?'0':'1';
            }
        }
    }

    vector<int> parse()
    {
        vector <int> res;
        //cout<<"\""<<s<<"\"";
        for(int i = gene_length-1 ; i < s.size() ; i+=gene_length)
        {
            int num = 0,temp=1;
            for(int j = i ; j >= i - (gene_length - 1) ; j--)
            {
                if(s[j]=='1')
                {
                    num+=temp;
                }
                temp*=2;
            }
            //cout<< num<<" ";
            res.push_back(num);
        }
        return res;
    }

};
struct GA_world
{

    vector<chromosome> population;
    int gene_length;
    int chrom_length;
    int feature_count;
    double crossover_rate;
    double total_fitness;


    GA_world(int n,int m,int d):population(n),gene_length(m),feature_count(d)
    {

        chrom_length = m*d;

        for(int i = 0 ; i < n ; i++)
        {
            population[i] = *(new chromosome(m,d));
        }
        crossover_rate = 0.7;
        total_fitness = 0;
    }

    void crossover(int i, int j)//crossover the ith and jth chromosome
    {

        if (RANDOM_NUM < crossover_rate)
        {
            //create a random crossover point
            int crossover_pt = (int) (RANDOM_NUM * chrom_length);

            string t1 = population[i].s.substr(0, crossover_pt) + population[j].s.substr(crossover_pt, chrom_length);
            string t2 = population[i].s.substr(0, crossover_pt) + population[j].s.substr(crossover_pt,chrom_length);

            population[i].s = t1;
            population[j].s = t2;
        }

    }

    int roullete()
    {
        //generate a random number between 0 & total fitness count
        double Slice = (float)(RANDOM_NUM * total_fitness);

        //go through the chromosones adding up the fitness so far
        double FitnessSoFar = 0.0f;

        for (int i=0; i< population.size(); i++)
        {
            FitnessSoFar += population[i].fitness;

            //if the fitness so far > random number return the chromo at this point
            if (FitnessSoFar >= Slice)
                return i;
        }
    }

    void evolve()
    {
        vector<string> new_pop;
        while(new_pop.size() != population.size())
        {
            int a = roullete();
            int b = roullete();

            //d(a,b);
            crossover(a,b);

            population[a].mutate();
            population[b].mutate();

            new_pop.push_back(population[a].s);
            new_pop.push_back(population[b].s);

            // system("pause");
        }
        for(int i = 0 ; i < new_pop.size(); i++)
        {
            population[i].s = new_pop[i];
        }
        total_fitness = 0;
    }

};

bool IsPrime(int number)
{
    int i;

    for (i=2; i<number; i++)
    {
        if (number % i == 0)
        {
            return false;
        }
    }

    return true;
}

int fitness(vector<int> parsed) // maximum number of even terms
{
    int fitt = 0;
    for(int i = 0 ; i < parsed.size(); i++)
    {
        if(parsed[i])
        {
            fitt+=parsed[i];
        }
    }

    if(IsPrime(fitt))
        return fitt;
    else return 0;
}

int main()
{


    srand(time(NULL));

    GA_world earth(50,4,4);
    int iter =0;

    while(1)
    {
        int maxx = -1,index;

        for(int i = 0 ; i < earth.population.size(); i++)
        {
            earth.population[i].fitness = fitness(earth.population[i].parse());
            if(earth.population[i].fitness > maxx)
            {
                maxx = earth.population[i].fitness;
                index = i;
            }
            earth.total_fitness += earth.population[i].fitness;
            //    d(earth.population[i].fitness);
        }


        if(maxx >= 50)
        {
            vector<int> best = earth.population[index].parse();
            cout<<"\nAfter "<<iter<<endl;
            for(int i = 0 ; i < best.size(); i++)cout<<best[i]<<" ";
            cout<<endl;

            d(earth.population[index].s,maxx);
            system("pause");
        }

        earth.evolve();


        iter++;
        // system("pause");

    }
    return 0;
}
