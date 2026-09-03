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

    return (velocity * velocity * sin(2 * angleRadians)) / GRAVITY;
}


/* Calculate the total time of flight of a projectile */
double calculateTimeOfFlight(double velocity, double angle)
{
    double angleRadians;

    angleRadians = angle * PI / 180.0;

    return (2 * velocity * sin(angleRadians)) / GRAVITY;
}


int main()
{
    int D;
    int N;
    int battleshipChoice;

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


    /* Check maximum number of escort ships */
    if (N > MAX_ESCORTS)
    {
        printf("Maximum number of escort ships is %d.\n",
               MAX_ESCORTS);

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


    /* Check Battleship choice */
    if (battleshipChoice < 1 || battleshipChoice > 4)
    {
        printf("Invalid battleship choice.\n");

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


    /* Display initial conditions */
    printf("\n========== INITIAL CONDITIONS ==========\n");

    printf("\nBattlefield: %d x %d\n", D, D);

    printf("Battleship Type: %c\n", B.type);

    printf("Battleship Position: (%.2f, %.2f)\n",
           B.x, B.y);


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


    /* Projectile calculation test */
    printf("\n========== PROJECTILE TEST ==========\n");

    double testVelocity = 100.0;
    double testAngle = 45.0;

    printf("Velocity: %.2f m/s\n",
           testVelocity);

    printf("Angle: %.2f degrees\n",
           testAngle);

    printf("Range: %.2f m\n",
           calculateRange(testVelocity, testAngle));

    printf("Time of flight: %.2f seconds\n",
           calculateTimeOfFlight(testVelocity, testAngle));


    return 0;
}