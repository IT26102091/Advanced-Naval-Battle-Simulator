#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_ESCORTS 100
#define GRAVITY 9.81
#define PI 3.14159265358979323846

typedef struct
{
    int id;
    char type;
    double x;
    double y;
} EscortShip;

typedef struct
{
    char type;
    double x;
    double y;
} Battleship;


/* Calculate the horizontal range of a projectile */
double calculateRange(double velocity, double angle)
{
    double angleRadians;

    angleRadians = angle * PI / 180.0;

    return (velocity * velocity * sin(2 * angleRadians))
           / GRAVITY;
}


/* Calculate total time of flight */
double calculateTimeOfFlight(double velocity, double angle)
{
    double angleRadians;

    angleRadians = angle * PI / 180.0;

    return (2 * velocity * sin(angleRadians))
           / GRAVITY;
}


/* Calculate distance between two points */
double calculateDistance(double x1, double y1,
                         double x2, double y2)
{
    double dx;
    double dy;

    dx = x2 - x1;
    dy = y2 - y1;

    return sqrt(dx * dx + dy * dy);
}


/* Calculate the maximum attack range of Battleship */
double calculateBattleshipRange(double velocity)
{
    /*
       Battleship can fire between 0 and 90 degrees.
       Maximum horizontal range occurs at 45 degrees.
    */

    return calculateRange(velocity, 45.0);
}


int main()
{
    int D;
    int N;
    int battleshipChoice;

    double battleshipVelocity;
    double battleshipRange;
    double distance;

    Battleship B;
    EscortShip escorts[MAX_ESCORTS];

    char escortTypes[] = {'A', 'B', 'C', 'D', 'E'};

    srand(time(NULL));


    /* Program title */
    printf("========================================\n");
    printf("      ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("========================================\n\n");


    /* Get battlefield size */
    printf("Enter battlefield size D: ");
    scanf("%d", &D);


    /* Get number of escort ships */
    printf("Enter number of escort ships: ");
    scanf("%d", &N);


    if (N <= 0 || N > MAX_ESCORTS)
    {
        printf("Invalid number of escort ships.\n");
        return 1;
    }


    /* Select Battleship */
    printf("\nSelect Battleship:\n");
    printf("1. USS Iowa\n");
    printf("2. MS King George V\n");
    printf("3. Richelieu\n");
    printf("4. Sovetsky Soyuz-class\n");

    printf("Enter choice: ");
    scanf("%d", &battleshipChoice);


    if (battleshipChoice < 1 || battleshipChoice > 4)
    {
        printf("Invalid Battleship choice.\n");
        return 1;
    }


    /* Store Battleship type */
    if (battleshipChoice == 1)
    {
        B.type = 'U';
    }
    else if (battleshipChoice == 2)
    {
        B.type = 'M';
    }
    else if (battleshipChoice == 3)
    {
        B.type = 'R';
    }
    else
    {
        B.type = 'S';
    }


    /* Get Battleship starting position */
    printf("\nEnter Battleship X position: ");
    scanf("%lf", &B.x);

    printf("Enter Battleship Y position: ");
    scanf("%lf", &B.y);


    /* Get maximum Battleship shell velocity */
    printf("\nEnter Battleship maximum shell velocity: ");
    scanf("%lf", &battleshipVelocity);


    if (battleshipVelocity <= 0)
    {
        printf("Velocity must be greater than zero.\n");
        return 1;
    }


    /* Generate Escort ships */
    for (int i = 0; i < N; i++)
    {
        escorts[i].id = i + 1;

        escorts[i].x =
            (double)rand() / RAND_MAX * D;

        escorts[i].y =
            (double)rand() / RAND_MAX * D;

        int typeIndex = rand() % 5;

        escorts[i].type = escortTypes[typeIndex];
    }


    /* Calculate Battleship maximum attack range */
    battleshipRange =
        calculateBattleshipRange(battleshipVelocity);


    /* Display initial conditions */
    printf("\n========== INITIAL CONDITIONS ==========\n");

    printf("\nBattlefield: %d x %d\n", D, D);

    printf("Battleship Type: %c\n", B.type);

    printf("Battleship Position: (%.2f, %.2f)\n",
           B.x, B.y);

    printf("Battleship Maximum Velocity: %.2f m/s\n",
           battleshipVelocity);

    printf("Battleship Maximum Attack Range: %.2f m\n",
           battleshipRange);


    /* Display Escort ships */
    printf("\nEscort Ships:\n");

    for (int i = 0; i < N; i++)
    {
        printf("E%d  Type: E%c  Position: (%.2f, %.2f)\n",
               escorts[i].id,
               escorts[i].type,
               escorts[i].x,
               escorts[i].y);
    }


    /* Check which Escort ships are in range */
    printf("\n========== BATTLESHIP ATTACK RANGE ==========\n");

    for (int i = 0; i < N; i++)
    {
        distance = calculateDistance(
            B.x,
            B.y,
            escorts[i].x,
            escorts[i].y
        );

        printf("\nE%d:\n", escorts[i].id);

        printf("Distance from B: %.2f m\n",
               distance);

        if (distance <= battleshipRange)
        {
            printf("Status: IN ATTACK RANGE\n");
        }
        else
        {
            printf("Status: OUTSIDE ATTACK RANGE\n");
        }
    }


    return 0;
}