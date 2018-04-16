#include <iostream>
#include <string.h>
using namespace std;

class Game{
private:
    float aura[2];//代表生命元 0 blue 1 red
    int time;
public:
    Game(float blueAura,float redAura){
        aura[0] = blueAura;
        aura[1] = redAura;
        time = 0;
    }
    void consume(int team,float number){
        aura[team] -= number;
    }
    void enhance(int team,float number){
        aura[team] += number;
    }
    float getAura(int team){
        return aura[team];
    }
    int getTime(){return time;}

    int timeAdd(){
        time++;
        if(time%100>24)
            time += 75;//24 小时制
    }
};


class Warrior{
private:
    int id[2];
    int hp;
    static int idCounter[2];//0 代表蓝队 1 代表红队
public:
    void init_warrior(int hp,int team, Game &game){
        this->hp = hp;
        idCounter[team]++;
        this->id[team] = idCounter[team];
    }

    int getId(int team){return id[team];}

    int getHp(){return hp;}

    static int getIdCounter(int team){return idCounter[team];}
};


class dragon: public Warrior{
private:
    float morale;
    int weapon;
    static int counter[2];
public:
    void init_dragon(int hp,int team,Game &game)
    {
        init_warrior(hp,team,game);
        this->weapon = (this->getId(team)%3);
        this->counter[team]++;
        game.consume(team,hp);
        this->morale = (game.getAura(team)/hp);
        char str[10];
        if(!team)
            strcpy(str,"blue");
        else
            strcpy(str,"red");
        printf("%03d",game.getTime());
        cout<<" "<<str<<" dragon "<<this->getId(team)<<" born with strength "<< this->getHp() <<", "<<counter[team]<<" dragon in "<<str<<" headquarter\n";
        char wuqi[10];
        if(weapon == 0)
            strcpy(wuqi,"sword");
        else if(weapon == 1)
            strcpy(wuqi,"bomb");
        else
            strcpy(wuqi,"arrow");
        printf("It has a %s, and its morale is %.2f\n",wuqi,morale);
    }

    float getMorale(){return morale;}

    int getWeapon(){return weapon;}

    static int getCounter(int team){return counter[team];}
};


class ninja: public Warrior{
private:
    int weapon[2];
    static float consume;
    static int counter[2];
public:
    void init_ninja(int hp,int team,Game &game)
    {
        init_warrior(hp,team,game);
        this->weapon[0] = (this->getId(team)%3);
        this->weapon[1] = ((this->getId(team)+1)%3);
        this->counter[team]++;
        game.consume(team,hp);
        char str[10];
        if(!team)
            strcpy(str,"blue");
        else
            strcpy(str,"red");
        printf("%03d",game.getTime());
        cout<<" "<<str<<" ninja "<<this->getId(team)<<" born with strength "<< this->getHp() <<", "<<counter[team]<<" ninja in "<<str<<" headquarter\n";
        char wuqi[2][10];
        for(int k=0;k<2;k++)
        {
            if(weapon[k] == 0)
                strcpy(wuqi[k],"sword");
            else if(weapon[k] == 1)
                strcpy(wuqi[k],"bomb");
            else
                strcpy(wuqi[k],"arrow");
        }
        printf("It has a %s and a %s\n",wuqi[0],wuqi[1]);
    }

    int getWeapon(int id){return weapon[id];}

    static int getCounter(int team){return counter[team];}

    static float getConsume(){return consume;}
};


class iceman: public Warrior{
private:
    int weapon;
    static float consume;
    static int counter[2];
public:
    void init_iceman(int hp,int team,Game &game)
    {
        init_warrior(hp,team,game);
        this->weapon = (this->getId(team)%3);
        this->counter[team]++;
        game.consume(team,hp);
        char str[10];
        if(!team)
            strcpy(str,"blue");
        else
            strcpy(str,"red");
        printf("%03d",game.getTime());
        cout<<" "<<str<<" iceman "<<this->getId(team)<<" born with strength "<< this->getHp() <<", "<<counter[team]<<" iceman in "<<str<<" headquarter\n";
        char wuqi[10];
        if(weapon == 0)
            strcpy(wuqi,"sword");
        else if(weapon == 1)
            strcpy(wuqi,"bomb");
        else
            strcpy(wuqi,"arrow");
        printf("It has a %s\n",wuqi);
    }

    int getWeapon(){return weapon;}

    static int getCounter(int team){return counter[team];}

    static float getConsume(){return consume;}
};


class lion: public Warrior{
private:
    int loyalty;
    static float consume;
    static int counter[2];
public:
    void init_lion(int hp,int team,Game &game)
    {
        init_warrior(hp,team,game);
        this->counter[team]++;
        game.consume(team,hp);
        this->loyalty = int(game.getAura(team));
        char str[10];
        if(!team)
            strcpy(str,"blue");
        else
            strcpy(str,"red");
        printf("%03d",game.getTime());
        cout<<" "<<str<<" lion "<<this->getId(team)<<" born with strength "<< this->getHp() <<", "<<counter[team]<<" lion in "<<str<<" headquarter\n";
        printf("Its loyalty is %d\n",loyalty);
    }

    int getLoyalty(){return loyalty;}

    static int getCounter(int team){return counter[team];}

    static float getConsume(){return consume;}
};


class wolf: public Warrior{
private:
    static float consume;
    static int counter[2];
public:
    void init_wolf(int hp,int team,Game &game)
    {
        init_warrior(hp,team,game);
        this->counter[team]++;
        game.consume(team,hp);
        char str[10];
        if(!team)
            strcpy(str,"blue");
        else
            strcpy(str,"red");
        printf("%03d",game.getTime());
        cout<<" "<<str<<" wolf "<<this->getId(team)<<" born with strength "<< this->getHp() <<", "<<counter[team]<<" wolf in "<<str<<" headquarter\n";
    }

    static int getCounter(int team){return counter[team];}

    static float getConsume(){return consume;}
};


int Warrior::idCounter[2] = {0,0};
int dragon::counter[2] = {0,0};
int ninja::counter[2] = {0,0};
int iceman::counter[2] = {0,0};
int lion::counter[2] = {0,0};
int wolf::counter[2] = {0,0};

int main(){
    int n;//测试组数
    cin>>n;
    for(int i=0;i<n;i++) {
        int m,hp[5];
        int min=INT_MAX;
        cin>>m;
        for(int j=0;j<5;j++)//[0]-> dragon,[1]-> ninja,[2]-> iceman,[3]->lion,[4]->wolf
        {
            cin>>hp[j];
            if(hp[j]<min)
                min = hp[j];
        }

        Game game(m, m);

        dragon a;
        ninja b;
        iceman c;
        lion d;
        wolf e;

        int red=0,blue=0;
        int red_flag = 1, blue_flag = 1;

        while(red_flag || blue_flag)
        {
            int p=0;
            for(p;p<5;p++)
            {
                int k=(red+p)%5;
                if(k==0&&hp[2]<=game.getAura(1))
                {
                    c.init_iceman(hp[2],1,game);
                    break;
                }
                if(k==1&&hp[3]<=game.getAura(1))
                {
                    d.init_lion(hp[3],1,game);
                    break;
                }
                if(k==2&&hp[4]<=game.getAura(1))
                {
                    e.init_wolf(hp[4],1,game);
                    break;
                }
                if(k==3&&hp[1]<=game.getAura(1))
                {
                    b.init_ninja(hp[1],1,game);
                    break;
                }
                if(k==4&&hp[0]<=game.getAura(1))
                {
                    a.init_dragon(hp[0],1,game);
                    break;
                }
            }
            red += p+1;

            int q=0;
            for(q;q<5;q++)
            {
                int k=(blue+q)%5;
                if(k==0&&hp[3]<=game.getAura(0))
                {
                    d.init_lion(hp[3],0,game);
                    break;
                }
                if(k==1&&hp[0]<=game.getAura(0))
                {
                    a.init_dragon(hp[0],0,game);
                    break;
                }
                if(k==2&&hp[1]<=game.getAura(0))
                {
                    b.init_ninja(hp[1],0,game);
                    break;
                }
                if(k==3&&hp[2]<=game.getAura(0))
                {
                    c.init_iceman(hp[2],0,game);
                    break;
                }
                if(k==4&&hp[4]<=game.getAura(0))
                {
                    e.init_wolf(hp[4],0,game);
                    break;
                }
            }
            blue += q+1;
            game.timeAdd();
            if(min>game.getAura(1)&&red_flag)
            {
                printf("%03d red headquarter stops making warriors\n",game.getTime());
                red_flag = 0;
            }
            if(min>game.getAura(0)&&blue_flag)
            {
                printf("%03d blue headquarter stops making warriors\n",game.getTime());
                blue_flag = 0;
            }
        }
    }
    return 0;
}