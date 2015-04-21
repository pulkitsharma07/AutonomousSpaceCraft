#include <bits/stdc++.h>
#include<graphics.h>

using namespace std;

#include "neuralnet.h"
#include "GA.h"

#define d(args...) do {cerr << #args << ": "; dbg,args; cerr << endl;} while(0)
struct debugger
{
    template<typename T> debugger& operator , (const T& v)
    {
        cout<<v<<" ";
        return *this;
    }
} dbg;

#define SCREEN_W 1024
#define SCREEN_H 500
double gravity = 0.98;

int padx,pady;
vector<int> op;
struct ship
{

    int mass;
    double speed_x,speed_y,acc_x,acc_y;
    int x,y,orig_x,orig_y;
    int width,height;
    int ticks;
    neural_network mind;

    bool booster_a,booster_b,booster_c;

    double external_forces_x = 0,external_forces_y = 0;

    ship(int a,int b,int p ,int q):width(a),height(b),x(p),y(q)
    {
        orig_x = x;
        orig_y = y;
        mass = 100;
        speed_x = 0;
        speed_y = 0;
        ticks = 0;
        acc_x = 0;
        booster_a = false;
        booster_b = false;
        booster_c = false;
        acc_y = 0;


        //neural network configuration
        mind.add_layer(4);
        mind.add_layer(2);
        mind.add_layer(3);

        double wts11[] = {100 - rand()%200,100 - rand()%200,100 - rand()%200,100 - rand()%200};
        double wts12[] = {100 - rand()%200,100 - rand()%200,100 - rand()%200,100 - rand()%200};

        mind.layers[1].neurons[1].assign_wts(wts11);
        mind.layers[1].neurons[2].assign_wts(wts12);


        double wts21[] = {100 - rand()%200,1100 - rand()%200};
        double wts22[] = {100 - rand()%200,100 - rand()%200};
        double wts23[] = {100 - rand()%200,100 - rand()%200};

        mind.layers[2].neurons[1].assign_wts(wts21);
        mind.layers[2].neurons[2].assign_wts(wts22);
        mind.layers[2].neurons[3].assign_wts(wts23);
    }
    void draw()
    {
        rectangle(x-width/2,y-height/2,x+width/2,y+height/2);

        if(booster_a)
        {
            line(x-width/2,y,x-width/2-20,y-10);
            line(x-width/2,y,x-width/2-20,y+10);
        }

        if(booster_b)
        {
            line(x,y+height/2,x-20,y+50);
            line(x,y+height/2,x+20,y+50);
        }

        if(booster_c)
        {
            line(x+width/2,y,x+width/2+20,y-10);
            line(x+width/2,y,x+width/2+20,y+10);
        }
    }

    void think()
    {
        double params[] = {1/speed_x,1/speed_y,1/(double)(x-padx),1/(double)(y-pady)};
        //  d(1/speed_x,1/speed_y,1/(double)(x-padx),1/(double)(y-pady));
        mind.input(params);
        mind.forward_prop();
        ticks++;
    }

    void update()
    {
        external_forces_x = 0;
        external_forces_y = gravity;

        op = mind.output();

        if(op[0])
        {
            booster_a = true;
            external_forces_x += gravity;
        }
        if(op[1])
        {
            booster_b = true;
            external_forces_y -= gravity*2;
        }
        if(op[2])
        {
            booster_c = true;
            external_forces_x -= gravity;
        }

        // d(acc_x+external_forces_x,acc_y+external_forces_y);
        speed_x += acc_x+external_forces_x;
        speed_y += acc_y+external_forces_y;
        y += speed_y;
        x += speed_x;

        speed_x *= 0.95;
        speed_y *= 0.95;

        //d(speed_x,speed_y);
    }

    void release_all()
    {
        booster_a = false;
        booster_b = false;
        booster_c = false;

    }

    void reset(vector<int> wts)
    {
        x = orig_x;
        y = orig_y;

        mass = 100;
        speed_x = 0;
        speed_y = 0;
        ticks = 0;
        acc_x = 0;
        booster_a = false;
        booster_b = false;
        booster_c = false;
        acc_y = 0;

//        cout<<"Checking weights... \n";
//                 for(int i = 0 ; i < wts.size(); i++)cout<<wts[i]<<" ";
//                 cout<<endl;

        double wts11[] = {wts[0],wts[1],wts[2],wts[3]};
        double wts12[] = {wts[4],wts[5],wts[6],wts[7]};


        mind.layers[1].neurons[1].assign_wts(wts11);
        mind.layers[1].neurons[2].assign_wts(wts12);


        double wts21[] = {wts[8],wts[9]};
        double wts22[] = {wts[10],wts[11]};
        double wts23[] = {wts[12],wts[13]};

        mind.layers[2].neurons[1].assign_wts(wts21);
        mind.layers[2].neurons[2].assign_wts(wts22);
        mind.layers[2].neurons[3].assign_wts(wts23);

    }
};



struct landing_pad
{

    int x,y;
    int initx;
    int width,height;

    landing_pad(int a,int b,int p , int q):width(a),height(b),x(p),y(q)
    {
        initx = p;
    }

    void draw()
    {
        rectangle(x-width/2,y-height/2,x+width/2,y+height/2);         //floodfill(x,y,2);
    }

    void update()
    {

        x += 2;
        x %= SCREEN_W;
        padx = x;
    }

    void reset()
    {

        x = initx;
    }
};

int status(ship craft,landing_pad pad) // return 1 if landed, 2 if inflight, 3 if crashed
{

    //check if landed
    /*
    // putpixel(craft.x,craft.y+ craft.height/2,4);
    // putpixel(pad.x,pad.y- pad.width/2 + 15,4);

    d(craft.y + craft.height/2);
    d(pad.y - pad.height/2 + 15);
    d(craft.y + craft.height/2);
    d(pad.y);
    */
    if(craft.x - craft.width/2 >= pad.x - pad.width/2 && craft.x + craft.width/2 <= pad.x + pad.width/2)
    {



        if(craft.y + craft.height/2 >= (pad.y - pad.height/2) - 2 && craft.y + craft.height/2 <= pad.y)
        {
            return 1;
        }
        else
        {
            return 2;
        }

    }
    else
    {
        if(craft.y + craft.height/2 >=  SCREEN_H || craft.x < 0 || craft.x > SCREEN_W || craft.y < -50)
        {
            return 3;
        }
        else
        {
            return 2;
        }

    }

}


int main()
{
    initwindow(SCREEN_W, SCREEN_H, "First Sample");

    srand(time(NULL));

    ship lunar(50,26,100,400);

    landing_pad nasa(150,20,400,100);

    padx = nasa.x;
    pady = nasa.y;

    int ship_status;

    GA_world earth(50,8,14);
    bool landed = false;

    int iter = 0;

    while(1)
    {


        double maxx = 0;
        int index;

        iter ++;

        for(int i = 0 ; i < earth.population.size(); i++)
        {
            lunar.reset(earth.population[i].parse());
            nasa.reset();
            landed = false;
            while(1 && lunar.ticks < 1000)
            {
                lunar.think();
//                if(iter%100 == 0 && i == 10)
//                {
//                    lunar.draw();
//                    nasa.draw();
//                    delay(20);
//                    cleardevice();
//                }
                //getch();8
             //   nasa.update();
                lunar.release_all();
                ship_status = status(lunar,nasa);
                if(ship_status == 2)
                    lunar.update();
                else
                {
                    if(ship_status == 1)
                    {
                      //  d("Landed!!! ",lunar.speed_y,lunar.ticks,iter);
                        landed = true;
                        //  earth.population[i].fitness = 1/(double)lunar.ticks;
                       // system("pause");
                    }
                    else
                    {
                        earth.population[i].fitness = 0;
                        // d("Crashed!!",lunar.ticks);
                    }

                    earth.population[i].fitness = (lunar.ticks + (landed)?100:0)/(double)((padx - lunar.x)*(padx - lunar.x) + (pady - lunar.y)*(pady - lunar.y) + abs(lunar.speed_y) + abs(lunar.speed_x));
                    //if(landed)earth.population[i].fitness += 1.0f;

                    if(earth.population[i].fitness > maxx)
                    {
                        maxx = earth.population[i].fitness;
                       //,// if(landed)
                        index = i;
                       // else index = -1;
                    }

                    earth.total_fitness += earth.population[i].fitness;
                    break;
                }
               // d(1);
                //cleardevice();
            }


        }

        if(iter%1 == 0 && index >=0 )//&& maxx <= 0.009)
        {
            d(iter);

            lunar.reset(earth.population[index].parse());
            nasa.reset();
            vector<int> wts =earth.population[index].parse();

                    cout<<"Checking weights... \n";
                 for(int i = 0 ; i < wts.size(); i++)cout<<wts[i]<<" ";
                 cout<<endl;

            while(1)
            {
                 cleardevice();
                lunar.think();

                lunar.draw();
                nasa.draw();
                //d(lunar.booster_b);
                delay(30);

                lunar.release_all();
               // nasa.update();

                //  getch();

                ship_status = status(lunar,nasa);
                if(ship_status == 2)
                    lunar.update();
                else
                {
                    d(lunar.speed_y,ship_status);
                    break;
                }
            }

        }

        earth.evolve();

        //d(iter);
        //system("pause");
    }

    // while(1) {}
    // getch();
    return 0;
}
