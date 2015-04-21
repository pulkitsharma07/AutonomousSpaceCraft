struct neuron
{

    vector<double> weights;

    double activation=1;

    neuron() {}

    neuron(int n)  //n = number of dendrites
    {
        weights.resize(n);
        assign_random_wts();
    }

    void create(int n)
    {
        weights.resize(n);
        assign_random_wts();

    }

    void assign_random_wts()
    {
        for(int i =0 ; i < weights.size() ; i++)
        {
            weights[i] = 100 - rand()%200;
        }
    }

    void assign_wts(double wts[])
    {

        for(int i = 0 ; i < weights.size(); i++)
        {
            weights[i] = wts[i];
        }
    }
};

struct neural_layer
{
    vector<neuron> neurons;

    neural_layer(int m,int n) //  m = number of neurons in current layer, n = dendrites per neuron
    {
        neurons.resize(m+1); // one more neuron for the bias

        for(int i = 0 ; i < m+1 ; i++)
        {
            neurons[i].create(n);
        }
    }

};

struct neural_network
{

    vector <neural_layer> layers;

    void add_layer(int m)
    {
        if(!layers.size())
        {
            layers.push_back(neural_layer(m,0));
        }
        else
        {
            layers.push_back(neural_layer(m,layers[layers.size()-1].neurons.size()));
        }
    }

    void req_inp()
    {
        cout<<"Please give "<<layers[0].neurons.size()-1<<" inputs (parameters) to the Network ( 0 or 1)\n";

        for(int i = 1 ; i < layers[0].neurons.size(); i++)
        {
           // cout<<"Enter the "<<i<<" parameter (input)\n";
            cin>>layers[0].neurons[i].activation;
            //cout<<" 0 "<<i<<" = "<<layers[0].neurons[i].activation<<endl;
        }
    }

    void input(double params[])
    {
        for(int i = 1 ; i < layers[0].neurons.size(); i++)
        {
           // cout<<"Enter the "<<i<<" parameter (input)\n";
            layers[0].neurons[i].activation = params[i];
            //cout<<" 0 "<<i<<" = "<<layers[0].neurons[i].activation<<endl;
        }
    }

    double sigmoid(double val)
    {
        return (1/(1+exp(-val)));
    }

    void forward_prop()
    {
        for(int i = 1 ; i < layers.size(); i++) // for each layer
        {

            for(int j = 1 ; j < layers[i].neurons.size(); j++) // for each neuron in this layer (ignoring the bias)
            {
                double accum = 0;

                for(int k = 0 ; k < layers[i].neurons[j].weights.size(); k++) // for each neuron it is connected to in the previous layer
                {
                    accum += layers[i].neurons[j].weights[k] * layers[i-1].neurons[k].activation;

                }

                layers[i].neurons[j].activation = sigmoid(accum);
               // cout<<"For layer "<<i+1<<" , neuron "<<j+1<<" has activation : "<<round(layers[i].neurons[j].activation)<<" sigmoid("<<accum<<")"<<endl;
            }

        }
    }

    vector<int> output()
    {
       // cout<<"The final layer of the neural network is .... \n";
        vector<int> res;

        for(int i = 1 ; i < layers[layers.size()-1].neurons.size() ; i++)
        {
           //cout<<round(layers[layers.size()-1].neurons[i].activation)<<" ";
            res.push_back(round(layers[layers.size()-1].neurons[i].activation));
        }
        //cout<<endl;
       // cout<<"\n\n";
       return res;
    }


};
