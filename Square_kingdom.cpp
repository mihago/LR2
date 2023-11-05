#include <iostream>
#include <assert.h>
#include <locale.h>
#include <windows.h>
#include <vector>
using namespace std;
const int MAX_HISTORY_SIZE = 3;
const int KINGDOM_SIZE = 3;

class WeatherData
{
public:
    WeatherData()
    {
        degrees=0;
        speed=0;
        temperature=0;
        pressure = 0;
    }
    WeatherData(double speed, int degrees, double pressure, double temperature)
    {
        this->degrees=degrees;
        this->speed=speed;
        this->pressure = pressure;
        this->temperature = temperature;
    }

    int getDegrees() const
    {
        return this->degrees;
    }

    double getSpeed() const
    {
        return this->speed;
    }

    double getPressure() const
    {

        return this->pressure;
    }
    double getTemperature() const
    {

        return this->temperature;
    }



private:
    int degrees;
    double speed;
    double temperature;
    double pressure;
};

class WindWane
{

public:
    virtual string whatsClass()const
    {
        return "WindWane";
    }
    int getDegrees() const
    {
        return this->degrees;
    }

    double getSpeed() const
    {
        return this->speed;
    }

    bool setSpeed(const double speed)
    {
        if (speed < 0||speed>200)
        {
            return false;
        }
        else
        {
            this->speed = speed;
            return true;
        }
    }

    bool setDegrees(const int degrees)
    {
        if (degrees < 0 || degrees > 360)
        {
            return false;
        }
        else
        {
            this->degrees = degrees;
            return true;
        }
    }
    virtual double getPressure()const
    {
        return -1000000;
    }
    virtual double getTemperature()const
    {
        return -1000000;

    }
    virtual bool setPressure(double pressure)
    {
        return false;

    }
    virtual bool setTemperature(double temperature)
    {
        return false;

    }



    WindWane()
    {
        this->setSpeed(0);
        this->setDegrees(0);
    }

    WindWane(double speed, int degrees):WindWane()
    {
        this->setSpeed(speed);
        this->setDegrees(degrees);
    }

    WindWane(const WindWane &wind)
    {
        this->setSpeed(wind.getSpeed());
        this->setDegrees(wind.getDegrees());
    }




private:
    double speed;
    int degrees;
};


class MeteoStation:public WindWane
{
public:
    string whatsClass()const
    {
        return "MeteoStation";
    }
    const vector<WeatherData> showHistory() const
    {
        return history;
    }

    double getPressure()const
    {
        return this->pressure;
    }
    double getTemperature()const
    {
        return this->temperature;
    }
    void pushToHistory()
    {
        if(history.size()==MAX_HISTORY_SIZE)
        {
            history.erase(history.begin());

        }
        history.push_back(WeatherData(getSpeed(),getDegrees(),getPressure(),getTemperature()));

    }

    bool setPressure(double pressure)
    {
        pushToHistory();
        if(pressure<0||pressure>1000)
        {
            return false;
        }
        else
        {

            this->pressure = pressure;
            return true;
        }
    }


    bool setTemperature(double temperature)
    {
        pushToHistory();
        if(temperature<-100||temperature>80)
        {
            return false;
        }
        else
        {

            this->temperature = temperature;

            return true;
        }
    }
    bool setSpeed(double speed)
    {
        pushToHistory();
        return WindWane::setSpeed(speed);

    }
    bool setDegrees(int degrees)
    {
        pushToHistory();
        return WindWane::setDegrees(degrees);

    }

    MeteoStation():WindWane()
    {
        pressure = 0;
        temperature = 0;
    }
    MeteoStation(double speed,int degrees,double pressure, double temperature):WindWane(speed,degrees)
    {
        if(temperature<-100||temperature>80)
        {
            this->temperature = 0;
        }
        else
        {
            this->temperature = temperature;
        }
        if(pressure<0||pressure>1000)
        {
            this->pressure = 0;
        }
        else
        {
            this->pressure = pressure;
        }

    }
    MeteoStation(const MeteoStation &meteostation):WindWane((WindWane) meteostation)
    {
        pressure = meteostation.pressure;
        temperature = meteostation.temperature;
    }

private:
    double pressure;
    double temperature;
    vector<WeatherData> history;
};

class MeteoCenter
{
public:
    MeteoCenter()
    {
        for(int i=0; i<=KINGDOM_SIZE; i++)
        {
            for(int j =0; j<=KINGDOM_SIZE; j++)
            {
                kingdomMap[i][j] =  new MeteoStation();
            }
        }
    }
    MeteoCenter(const MeteoCenter &meteocenter)
    {
//TODO:Вот тут надо думать
        for(int i=0; i<=KINGDOM_SIZE; i++)
        {
            for(int j =0; j<=KINGDOM_SIZE; j++)
            {
                if(meteocenter.kingdomMap[i][j]->whatsClass()=="MeteoStation")
                {
                    kingdomMap[i][j] =  new MeteoStation(*(MeteoStation*)meteocenter.kingdomMap[i][j]);

                }
                else
                {
                    kingdomMap[i][j] =  new WindWane(*(meteocenter.kingdomMap[i][j]));

                }
            }
        }
        //cout<<kingdomMap[0][0]->whatsClass();
        //cout<<kingdomMap[0][0]->getPressure();
        //cout<<kingdomMap[0][0]->getTemperature();
    }
    MeteoCenter(const WindWane* a[][KINGDOM_SIZE+1])
    {

        for(int i=0; i<=KINGDOM_SIZE; i++)
        {
            for(int j =0; j<=KINGDOM_SIZE; j++)
            {
                if(a[i][j]->whatsClass()=="MeteoStation")
                {
                    kingdomMap[i][j] =  new MeteoStation(*(MeteoStation*)a[i][j]);

                }
                else
                {
                    kingdomMap[i][j] =  new WindWane(*(a[i][j]));

                }
            }
        }
    }
    ~MeteoCenter()
    {
        for(int i=0; i<=KINGDOM_SIZE; i++)
        {
            for(int j =0; j<=KINGDOM_SIZE; j++)
            {
                delete  kingdomMap[i][j];
            }
        }

    }

    WindWane* kingdomMap[KINGDOM_SIZE+1][KINGDOM_SIZE+1];
};

int main()
{
    WeatherData wd1  = WeatherData();
    WeatherData wd2  = WeatherData(10.5,20,500,70);
    assert(wd1.getSpeed()==0);
    cout<<"Checkpoint 0.1 WeatherData getter for speed works\n";
    assert(wd1.getDegrees()==0);
    cout<<"Checkpoint 0.2 WeatherData getter for degrees works\n";
    assert(wd1.getPressure()==0);
    cout<<"Checkpoint 0.3 WeatherData getter for pressure works\n";
    assert(wd1.getTemperature()==0);
    cout<<"Checkpoint 0.4 WeatherData getter for temperature works\n";
    cout<<"Checkpoint 0.5 Default constructor for WeatherData works\n";

    assert(wd2.getSpeed()==10.5);
    assert(wd2.getDegrees()==20);
    assert(wd2.getPressure()==500);
    assert(wd2.getTemperature()==70);
    cout<<"Checkpoint 0.6 Default constructor for WeatherData works\n";
    WindWane windwane1;
    assert(windwane1.getDegrees()==0);
    cout<<"Checkpoint 1.1: WindWane getter for degrees  works\n";
    assert(windwane1.getSpeed()==0);
    cout<<"Checkpoint 1.2: WindWane getter for speed  works";
    cout<<"Checkpoint 1.3: default WindWane constructor and getters works correctly\n";

    WindWane windwane2(10.5,20);
    assert(windwane2.getSpeed()==10.5);
    assert(windwane2.getDegrees()==20);
    cout<<"Checkpoint 1.4: parameterised WindWane constructor works \n";

    WindWane windwane4(100000000.5,20);

    assert(windwane4.getSpeed()==0&&windwane4.getDegrees()==20);
    cout<<"Checkpoint 1.5: parameterised WindWane constructor works correctly and sets default values when parameters are out of possible range\n";

    //TODO: сделать проверку в конструкторе
    WindWane windwane3(windwane2);
    assert(windwane3.getSpeed()==10.5);
    assert(windwane3.getDegrees()==20);
    cout<<"Checkpoint 1.6: copying WindWane constructor works correctly\n";

    windwane1.setSpeed(15.3);
    assert(windwane1.getSpeed()==15.3);
    cout<<"Checkpoint 1.7:WindWane setter for speed  works \n";

    windwane1.setDegrees(60);
    assert(windwane1.getDegrees()==60);
    cout<<"Checkpoint 1.8:WindWane setter for degrees  works \n";


    double initSpeed = windwane1.getSpeed();
    windwane1.setSpeed(1045000);
    assert(windwane1.getSpeed() == initSpeed);
    cout<<"Checkpoint 1.9: WindWane setter for speed doesn't accept values that are out of possible range"<<endl;

    double initDegrees = windwane1.getDegrees();
    windwane1.setDegrees(1045000);
    assert(windwane1.getDegrees() == initDegrees);
    cout<<"Checkpoint 1.10: WindWane setter for degrees doesn't accept values that are out of possible range\n"<<endl;


    MeteoStation ms1 = MeteoStation();
    assert(ms1.getDegrees()==0);
    cout<<"Checkpoint 2.1: Meteo station getter(inherited) for degrees  works\n";
    assert(ms1.getSpeed()==0);
    cout<<"Checkpoint 2.2: Meteo station getter(inherited) for speed  works\n";
    assert(ms1.getPressure()==0);
    cout<<"Checkpoint 2.3: Meteo station getter(inherited) for pressure  works\n";
    assert(ms1.getTemperature()==0);
    cout<<"Checkpoint 2.4: Meteo station getter(inherited) for temperature  works\n";
    cout<<"Checkpoint 2.5: default WindWane constructor and getters works correctly\n";

    MeteoStation ms2 = MeteoStation(1,2,3,4);
    assert(ms2.getSpeed()==1);
    assert(ms2.getDegrees()==2);
    assert(ms2.getTemperature()==4);
    assert(ms2.getPressure() == 3);
    cout<<"Checkpoint 2.6: parameterised MeteoStation constructor works \n";
    MeteoStation ms3 = MeteoStation(-1100000,200000,3000000,465565);
    assert(ms3.getSpeed()==0);
    assert(ms3.getDegrees()==0);
    assert(ms3.getTemperature()==0);
    assert(ms3.getPressure() == 0);
    cout<<"Checkpoint 2.7: parameterised MeteoStation constructor works correctly and sets default values when parameters are out of possible range\n";


    MeteoStation ms4 = MeteoStation(ms2);
    assert(ms4.getSpeed()==1);
    assert(ms4.getDegrees()==2);
    assert(ms4.getTemperature()==4);
    assert(ms4.getPressure() == 3);
    cout<<"Checkpoint 2.8: Copying MeteoStation constructor works correctly\n";

    ms4.setPressure(555);
    assert(ms4.getPressure()==555);
    cout<<"Checkpoint 2.9: Meteostation setter for pressure works\n";
    double initPressure = ms4.getPressure();
    ms4.setPressure(5555550);
    assert(ms4.getPressure()==initPressure);
    cout<<"Checkpoint 2.10: Meteostation setter for pressure works correctly\n";

    ms4.setTemperature(55);
    assert(ms4.getTemperature()==55);
    cout<<"Checkpoint 2.11: Meteostation setter for temperature works\n";
    double initTemperature = ms4.getTemperature();
    ms4.setTemperature(5555);
    assert(ms4.getTemperature()==initTemperature);
    cout<<"Checkpoint 2.12: Meteostation setter for temperature works correctly\n";


    ms4.setSpeed(15.3);
    assert(ms4.getSpeed()==15.3);
    cout<<"Checkpoint 2.13:Meteostation setter for speed  works \n";

    initSpeed = ms4.getSpeed();
    ms4.setSpeed(1045000);
    assert(ms4.getSpeed() == initSpeed);
    cout<<"Checkpoint 2.14: Meteostation setter for speed doesn't accept values that are out of possible range"<<endl;


    ms4.setDegrees(60);
    assert(ms4.getDegrees()==60);
    cout<<"Checkpoint 2.15:Meteostation setter for degrees  works \n";

    initDegrees = ms4.getDegrees();
    ms4.setDegrees(1045000);
    assert(ms4.getDegrees() == initDegrees);
    cout<<"Checkpoint 2.16: Meteostation setter for degrees doesn't accept values that are out of possible range"<<endl;


    MeteoStation msh;
    vector<WeatherData> r = msh.showHistory();
    assert(r.size()==0);
    cout<<"Checkpoint 2.17: Meteostation history is empty after creating an instance"<<endl;
    msh.setDegrees(1);
    r = msh.showHistory();
    assert(r.at(0).getDegrees()==0);
    assert(r.at(0).getPressure()==0);
    assert(r.at(0).getTemperature()==0);
    assert(r.at(0).getSpeed()==0);
    msh.setTemperature(2);
    msh.setPressure(3);
    r = msh.showHistory();

    assert(r.at(0).getDegrees()==0&&r.at(0).getPressure()==0&&r.at(0).getTemperature()==0&&r.at(0).getSpeed()==0);
    assert(r.at(1).getDegrees()==1&&r.at(1).getPressure()==0&&r.at(1).getTemperature()==0&&r.at(1).getSpeed()==0);
    assert(r.at(2).getDegrees()==1&&r.at(2).getPressure()==0&&r.at(2).getTemperature()==2&&r.at(2).getSpeed()==0);
    cout<<"Checkpoint 2.18 Meteostation history can store three previous states correctly...\n";
    msh.setSpeed(4);
    r = msh.showHistory();
    assert(r.at(0).getDegrees()==1&&r.at(0).getPressure()==0&&r.at(0).getTemperature()==0&&r.at(0).getSpeed()==0);
    assert(r.at(1).getDegrees()==1&&r.at(1).getPressure()==0&&r.at(1).getTemperature()==2&&r.at(1).getSpeed()==0);
    assert(r.at(2).getDegrees()==1&&r.at(2).getPressure()==3&&r.at(2).getTemperature()==2&&r.at(2).getSpeed()==0);
    cout<<"Checkpoint 2.19... and then history is overflowed the oldest element is deleting!\n";


    assert(msh.whatsClass()=="MeteoStation");
    cout<<"Checkpoint 2.20 Identification method for MeteoStation works\n";
    WindWane &wdwn1 = msh;
    assert(wdwn1.whatsClass()=="MeteoStation");
    cout<<"Checkpoint 2.21 Identification method for MeteoStation works correctly\n";
    WindWane wdwn2;
    assert(wdwn2.whatsClass()=="WindWane");
    cout<<"Checkpoint 2.22 Identification method for WindWane works correctly\n"<<endl;
    MeteoCenter mc;
    cout<<mc.kingdomMap[0][0]->getPressure();
    MeteoCenter mc2(mc);
    WindWane* a[KINGDOM_SIZE+1][KINGDOM_SIZE+1];
    for(int i=0; i<=KINGDOM_SIZE; i++)
    {
        for(int j =0; j<=KINGDOM_SIZE; j++)
        {
            a[i][j] = (WindWane*) new MeteoStation((i+1)*5+j,(j+i)*10, i*100+j*20,(i-j)*5);
        }
    }
    a[1][1] = new WindWane(5,20);
    MeteoCenter mc3(a);



    //Ограничения
    //Speed = [0.0;200]
    //Degrees = [0;360]
    //Pressure = [0.0;1000]
    //Temperature = [-100.0;80]





    return 0;
}
