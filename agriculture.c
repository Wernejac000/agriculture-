#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BUSHELS_PER_ACRE 85 //1 acre produces 85 bushels
#define BUSHELS_PER_PERSON 7.8 //takes 7.8 bushels to feed 1 person
#define PEOPLE_PER_ACRE 45 //can fit 45 people in 1 acre of land

//cost constants
#define BUY_LAND 12000  //per acre
#define SELL_LAND 8400
#define COST_TO_BUILD 20000 //1 acre 
#define COST_TO_PLANT 900 //1 acre of  food 
#define AVERAGE_INCOME 31133 //per person
#define FED_TAX_RATE .133 //13.3%

int year = 0;

typedef struct City{
	double money;
	float population;
	float housing; //acres dedicated to housing
	float agriculture; //acres  dedicated to food
	float freeLand; //undedicated land
	float totalLand; //total land in acres
	float foodInStock; //in bushels
	float landNeededToHouse; //how much land in acres needed to house people
	float produced; //amout produced in bushels
	float ate; //amount ate by people in bushels
	float deathsByStarvation;
	float deathsByFreezing;
}City;

City City1;

int randNum(int lower, int upper)
{
        //function returns a random number in range [lower, upper]
        int num = (rand()%(upper - lower + 1)) + lower;
        return num;
}

void disasters(void)
{
        /*function generates random number between [0,99]
        if number equal 5,10,15,20,25,or 30 a disaster happens
        else nothing*/
        int disaster = randNum(0,99);
        switch(disaster)
        {
                case(5):
                        //fire disaster
                        City1.housing /= 2;
                        City1.population /= 2;
                        printf("A fire has broken out in the city\n");
                        printf("You lose half the population and half the houses built\n");
                        break;
                case(10):
                        //flood disaster
                        City1.housing /= 2;
                        printf("There is a flood in the city\n");
                        printf("Half the houses are destroyed\n");
                        break;
                case(15):
                        //earthquake
                        City1.population /= 2;
                        City1.housing /= 2;
                        printf("An earthquake strikes the city\n");
                        printf("Half the people die and half the houses built are destroyed\n");
                        break;
                case(20):
                        //thief
                        City1.foodInStock /= 2;
                        printf("There appears to have been a thief around\n");
                        printf("Half the food in stock is missing\n");
                        break;
                case(25):
                        //depression
                        City1.money /= 2;
                        printf("The city is in a major depression\n");
                        printf("Half of the city's money is lost\n");
                        break;
                case(30):
                        //hurricane
                        City1.population /= 2;
                        City1.foodInStock /= 2;
                        City1.housing /= 2;
                        printf("A hurricane strikes\n");
                        printf("Half the people die, half the food in stock is ruined and half the houses built are destroyed\n");
                        break;
                default:
                        printf("You've avoided all disasters this year!\n");
                        break;

        }
}

bool status(void)
{
	//initialize and calculate status values
	City1.totalLand = City1.housing + City1.agriculture + City1.freeLand;
	City1.ate = City1.population * BUSHELS_PER_PERSON;
	City1.produced = City1.agriculture * BUSHELS_PER_ACRE;
	City1.landNeededToHouse = City1.population / PEOPLE_PER_ACRE;
	City1.foodInStock = City1.foodInStock - City1.ate  + City1.produced;
	City1.money += (FED_TAX_RATE * City1.population * AVERAGE_INCOME);
	
	//check status
	if(City1.population < 1)
	{
		City1.population = 0;
		printf("Congratulations!\n");
		printf("You've killed everyone in sight\n");
		printf("Game over\n");
		exit(0);
	}
	else if(City1.foodInStock <= 0)
	{
		printf("You do not have enough food in stock to feed %.0f people\n", City1.population);
		City1.deathsByStarvation = fabs(City1.foodInStock) / BUSHELS_PER_PERSON;
		City1.foodInStock = 0;
	}
	else if(City1.landNeededToHouse > City1.housing)
	{
		printf("You do not have enough land to house %.0f people\n", City1.population);
		City1.deathsByFreezing = fabs((City1.housing - City1.landNeededToHouse)) * PEOPLE_PER_ACRE;
	}
	else
	{
		City1.deathsByStarvation = 0;
		City1.deathsByFreezing = 0;
	}
	City1.population = City1.population - City1.deathsByStarvation - City1.deathsByFreezing;
	
	//print status
	printf("Year = %d\n", year);
	printf("$%.0f\n", City1.money);
	printf("Population = %.0f people\n", City1.population);
	printf("Land: \n");
        printf("\tHousing = %.0f acres\n", City1.housing);
        printf("\tAgriculture = %.0f acres\n", City1.agriculture);
        printf("\tFree land = %.0f acres\n", City1.freeLand);
        printf("\tTotal land = %.0f acres\n", City1.totalLand);
	printf("Food in stock = %.0f bushels\n", City1.foodInStock);
        printf("Ate = %.0f bushels\n", City1.ate);
	printf("Produced = %.0f bushels\n", City1.produced);
	printf("Land needed to house = %.0f acres\n", City1.landNeededToHouse);
        printf("Deaths:\n");
        printf("\tStarvation =  %.0f people\n", City1.deathsByStarvation);
        printf("\tFreezing = %.0f people\n", City1.deathsByFreezing);
        printf("1 acre of agriculture produces %d bushels\n", BUSHELS_PER_ACRE);
	disasters();
	year++;
	City1.population *= 1.2;
	return true;
}

bool getInput(void)
{
	char input; //user input
	int buy, sell, build, plant;
	
	label:
	printf("Enter:\n");
	printf("\t0 to Buy Land\n");
	printf("\t1 to Sell Land\n");
	printf("\t2 to Build\n");
	printf("\t3 to Plant\n");
	printf("\t4 to Quit\n");
	printf("\t5 Do Nothing\n");

	scanf(" %c", &input);

	switch (input)
	{
		case '0':
			//buy land
			printf("Cost per acre = %d\n\n", BUY_LAND);
			printf("How much land do you want to buy?\n");
			scanf(" %d", &buy);
			if(City1.money < (buy * BUY_LAND))
			{
				printf("You do not have enough money to buy that\n");
				goto label;
			}
			else
			{
				City1.freeLand += buy;
				City1.money -= (buy * BUY_LAND);
			}
			break;
		case '1':
			//sell land
			printf("Land sells at %d per acre\n\n", SELL_LAND);
			printf("How much land do you want to sell?\n");
			scanf(" %d", &sell);
			if(sell > City1.freeLand)
			{
				printf("You don't have that much free land to sell.\n");
				goto label;
			}
			else
			{
				City1.freeLand -= sell;
				City1.money += (sell * SELL_LAND);
			}
			break;
		case '2':
			//build
			printf("Cost to build = %d per acre\n\n", COST_TO_BUILD);
			printf("How much land do you want to build houses on?\n");
			scanf(" %d", &build);
			if(build > City1.freeLand)
			{
				printf("You don't have that much free land to build on.\n");
				goto label;
			}
			else if(City1.money < (build * COST_TO_BUILD))
			{
				printf("You do not have enough money to build on that much land\n");
				goto label;
			}

			else
			{
				City1.housing += build;
				City1.freeLand -= build;
				City1.money -= (build * COST_TO_BUILD);
			}
			break;
		case '3':
			//plant
			printf("Cost to plant = %d per acre\n\n", COST_TO_PLANT);
			printf("How much land do you want to plant seed on?\n");
			scanf(" %d", &plant);
			if(plant > City1.freeLand)
			{
				printf("You don't have that much free land to plant on.\n");
				goto label;
			}
			else if(City1.money < (plant * COST_TO_PLANT))
			{
				printf("You do not have enough money to buy this\n");
				goto label;
			}
			else
			{
				City1.agriculture += plant;
				City1.freeLand -= plant;
				City1.money -= (plant * COST_TO_PLANT);
			}
			break;
		case '4':
			//exit game
			exit(0);
			break;
		case '5':
			//do nothing
			break;
		default:
			printf("I don't understand\n");
			printf("try again\n");
			goto label;
			break;
	}
	return true;
}

int main()
{
	system("clear"); //linux terminal command 'clear'
	printf("Agriculture\n"); //title
	printf("\tProgrammed by: Jacob Werner\n\n");
	City1.money = 20000.0;
	City1.population = 100.0;
        City1.housing = 3.0;
	City1.agriculture = 10.0;
        City1.freeLand = 5.0;
	City1.foodInStock = 800.0;
	srand(time(0)); //use current time as seed for random number generator

	while(status() && getInput());
	return 0;
}
