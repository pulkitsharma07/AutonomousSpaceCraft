
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
       // cout<<"\""<<s<<"\"";
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
            assert(s[i - (gene_length - 1)] == '1' || s[i - (gene_length - 1)] == '0');

            if(s[i - (gene_length - 1)] && s[i - (gene_length - 1)] == '1') num = - num;
           // cout<< num<<" ";
            res.push_back(num);
        }
        //system("pause");
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
        crossover_rate = 0.007;
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
