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

    double minVelocity;
    double maxVelocity;

    double minAngle;
    double impactPower;

    int destroyed;
} EscortShip;

typedef struct
{
    char type;

    double x;
    double y;

    double maxVelocity;

    int destroyed;
} Battleship;


/* Convert degrees to radians */
double degreesToRadians(double angle)
{
    return angle * PI / 180.0;
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


/* Calculate projectile range */
double calculateRange(double velocity, double angle)
{
    double angleRadians;

    angleRadians = degreesToRadians(angle);

    return (velocity * velocity *
            sin(2 * angleRadians)) / GRAVITY;
}


/*
   Calculate a suitable firing angle for a target.

   We use the lower-angle solution of the projectile
   equation. The angle must also be above the ship's
   minimum angle.
*/
double calculateFiringAngle(double velocity,
                            double distance,
                            double minimumAngle)
{
    double value;
    double angle1;
    double angle2;

    value = (distance * GRAVITY) /
            (velocity * velocity);

    if (value > 1.0)
    {
        return -1.0;
    }

    angle1 = 0.5 * asin(value) * 180.0 / PI;
    angle2 = 90.0 - angle1;

    if (angle1 >= minimumAngle)
    {
        return angle1;
    }

    if (angle2 >= minimumAngle)
    {
        return angle2;
    }

    return -1.0;
}


/* Calculate projectile time of flight */
double calculateTimeOfFlight(double velocity,
                             double angle)
{
    double angleRadians;

    angleRadians = degreesToRadians(angle);

    return (2 * velocity *
            sin(angleRadians)) / GRAVITY;
}


/* Generate a random decimal number */
double randomDouble(double minimum, double maximum)
{
    return minimum +
           ((double)rand() / RAND_MAX) *
           (maximum - minimum);
}


/* Get the name of the Battleship */
const char* getBattleshipName(char type)
{
    if (type == 'U')
        return "USS Iowa";

    if (type == 'M')
        return "MS King George V";

    if (type == 'R')
        return "Richelieu";

    return "Sovetsky Soyuz-class";
}


/* Set Escort ship properties */
void setEscortProperties(EscortShip *ship,
                         double battleshipMaxVelocity)
{
    if (ship->type == 'A')
    {
        ship->minAngle = 20.0;
        ship->impactPower = 0.08;

        ship->minVelocity = randomDouble(
            0.2 * battleshipMaxVelocity,
            0.5 * battleshipMaxVelocity
        );

        ship->maxVelocity = randomDouble(
            0.8 * battleshipMaxVelocity,
            1.2 * battleshipMaxVelocity
        );
    }
    else if (ship->type == 'B')
    {
        ship->minAngle = 30.0;
        ship->impactPower = 0.06;

        ship->minVelocity = randomDouble(
            0.2 * battleshipMaxVelocity,
            0.5 * battleshipMaxVelocity
        );

        ship->maxVelocity = randomDouble(
            0.5 * battleshipMaxVelocity,
            0.9 * battleshipMaxVelocity
        );
    }
    else if (ship->type == 'C')
    {
        ship->minAngle = 25.0;
        ship->impactPower = 0.07;

        ship->minVelocity = randomDouble(
            0.2 * battleshipMaxVelocity,
            0.5 * battleshipMaxVelocity
        );

        ship->maxVelocity = randomDouble(
            0.5 * battleshipMaxVelocity,
            0.9 * battleshipMaxVelocity
        );
    }
    else if (ship->type == 'D')
    {
        ship->minAngle = 50.0;
        ship->impactPower = 0.05;

        ship->minVelocity = randomDouble(
            0.2 * battleshipMaxVelocity,
            0.5 * battleshipMaxVelocity
        );

        ship->maxVelocity = randomDouble(
            0.5 * battleshipMaxVelocity,
            0.9 * battleshipMaxVelocity
        );
    }
    else
    {
        ship->minAngle = 70.0;
        ship->impactPower = 0.04;

        ship->minVelocity = randomDouble(
            0.2 * battleshipMaxVelocity,
            0.5 * battleshipMaxVelocity
        );

        ship->maxVelocity = randomDouble(
            0.5 * battleshipMaxVelocity,
            0.9 * battleshipMaxVelocity
        );
    }
}


int main()
{
    int D;
    int N;
    int battleshipChoice;

    int bSunk = 0;
    int hitCount = 0;
    int sinkingEscort = -1;

    double battleEndTime = 0.0;

    Battleship B;
    EscortShip escorts[MAX_ESCORTS];

    char escortTypes[] = {'A', 'B', 'C', 'D', 'E'};

    FILE *initialFile;
    FILE *finalFile;
    FILE *resultFile;

    srand((unsigned int)time(NULL));


    /* =========================================
       PROGRAM TITLE
       ========================================= */

    printf("========================================\n");
    printf("      ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("========================================\n\n");


    /* =========================================
       BATTLEFIELD SETUP
       ========================================= */

    printf("Enter battlefield size D: ");
    scanf("%d", &D);

    printf("Enter number of escort ships: ");
    scanf("%d", &N);


    if (D <= 0)
    {
        printf("Invalid battlefield size.\n");
        return 1;
    }

    if (N <= 0 || N > MAX_ESCORTS)
    {
        printf("Invalid number of escort ships.\n");
        return 1;
    }


    /* =========================================
       BATTLESHIP SELECTION
       ========================================= */

    printf("\nSelect Battleship:\n");

    printf("1. USS Iowa\n");
    printf("2. MS King George V\n");
    printf("3. Richelieu\n");
    printf("4. Sovetsky Soyuz-class\n");

    printf("Enter choice: ");
    scanf("%d", &battleshipChoice);


    if (battleshipChoice < 1 ||
        battleshipChoice > 4)
    {
        printf("Invalid Battleship choice.\n");
        return 1;
    }


    if (battleshipChoice == 1)
        B.type = 'U';
    else if (battleshipChoice == 2)
        B.type = 'M';
    else if (battleshipChoice == 3)
        B.type = 'R';
    else
        B.type = 'S';


    /* =========================================
       BATTLESHIP POSITION
       ========================================= */

    printf("\nEnter Battleship X position: ");
    scanf("%lf", &B.x);

    printf("Enter Battleship Y position: ");
    scanf("%lf", &B.y);


    if (B.x < 0 || B.x > D ||
        B.y < 0 || B.y > D)
    {
        printf("Battleship position is outside "
               "the battlefield.\n");

        return 1;
    }


    /* =========================================
       BATTLESHIP VELOCITY
       ========================================= */

    printf("\nEnter Battleship maximum shell velocity: ");
    scanf("%lf", &B.maxVelocity);


    if (B.maxVelocity <= 0)
    {
        printf("Velocity must be greater than zero.\n");
        return 1;
    }


    /* =========================================
       GENERATE ESCORT SHIPS
       ========================================= */

    for (int i = 0; i < N; i++)
    {
        escorts[i].id = i + 1;

        escorts[i].type =
            escortTypes[rand() % 5];

        escorts[i].x =
            randomDouble(0.0, (double)D);

        escorts[i].y =
            randomDouble(0.0, (double)D);

        escorts[i].destroyed = 0;

        setEscortProperties(
            &escorts[i],
            B.maxVelocity
        );
    }


    /* =========================================
       SAVE INITIAL CONDITIONS
       ========================================= */

    initialFile =
        fopen("initial_conditions.txt", "w");


    if (initialFile == NULL)
    {
        printf("Error creating initial_conditions.txt\n");
        return 1;
    }


    fprintf(initialFile,
            "ADVANCED NAVAL BATTLE SIMULATOR\n");

    fprintf(initialFile,
            "====================================\n\n");

    fprintf(initialFile,
            "Battlefield Size: %d x %d\n",
            D, D);

    fprintf(initialFile,
            "Battleship: %s\n",
            getBattleshipName(B.type));

    fprintf(initialFile,
            "Battleship Type: %c\n",
            B.type);

    fprintf(initialFile,
            "Battleship Position: (%.2f, %.2f)\n",
            B.x, B.y);

    fprintf(initialFile,
            "Battleship Maximum Velocity: %.2f m/s\n\n",
            B.maxVelocity);


    fprintf(initialFile,
            "ESCORT SHIPS\n");
    fprintf(initialFile,
            "------------\n");


    for (int i = 0; i < N; i++)
    {
        fprintf(initialFile,
                "E%d\n",
                escorts[i].id);

        fprintf(initialFile,
                "Type: E%c\n",
                escorts[i].type);

        fprintf(initialFile,
                "Position: (%.2f, %.2f)\n",
                escorts[i].x,
                escorts[i].y);

        fprintf(initialFile,
                "Minimum Velocity: %.2f m/s\n",
                escorts[i].minVelocity);

        fprintf(initialFile,
                "Maximum Velocity: %.2f m/s\n",
                escorts[i].maxVelocity);

        fprintf(initialFile,
                "Minimum Angle: %.2f degrees\n",
                escorts[i].minAngle);

        fprintf(initialFile,
                "Impact Power: %.2f\n\n",
                escorts[i].impactPower);
    }


    fclose(initialFile);


    /* =========================================
       DISPLAY INITIAL CONDITIONS
       ========================================= */

    printf("\n========== INITIAL CONDITIONS ==========\n");

    printf("\nBattlefield: %d x %d\n",
           D, D);

    printf("Battleship: %s\n",
           getBattleshipName(B.type));

    printf("Battleship Position: "
           "(%.2f, %.2f)\n",
           B.x, B.y);

    printf("Battleship Maximum Velocity: "
           "%.2f m/s\n",
           B.maxVelocity);


    printf("\nEscort Ships:\n");


    for (int i = 0; i < N; i++)
    {
        printf("\nE%d\n",
               escorts[i].id);

        printf("Type: E%c\n",
               escorts[i].type);

        printf("Position: "
               "(%.2f, %.2f)\n",
               escorts[i].x,
               escorts[i].y);

        printf("Velocity Range: "
               "%.2f - %.2f m/s\n",
               escorts[i].minVelocity,
               escorts[i].maxVelocity);

        printf("Minimum Angle: "
               "%.2f degrees\n",
               escorts[i].minAngle);
    }


    /* =========================================
       BATTLE SIMULATION
       ========================================= */

    printf("\n========== BATTLE SIMULATION ==========\n");


    /*
       First B attacks every E that it can hit.
       In Part 1-A, one successful hit destroys E.
    */

    for (int i = 0; i < N; i++)
    {
        double distance;
        double angle;
        double timeToHit;

        distance = calculateDistance(
            B.x,
            B.y,
            escorts[i].x,
            escorts[i].y
        );


        angle = calculateFiringAngle(
            B.maxVelocity,
            distance,
            0.0
        );


        printf("\nB -> E%d\n",
               escorts[i].id);

        printf("Distance: %.2f m\n",
               distance);


        if (angle < 0)
        {
            printf("Result: OUT OF RANGE\n");
        }
        else
        {
            timeToHit =
                calculateTimeOfFlight(
                    B.maxVelocity,
                    angle
                );

            escorts[i].destroyed = 1;

            hitCount++;

            if (timeToHit > battleEndTime)
            {
                battleEndTime = timeToHit;
            }

            printf("Firing Angle: %.2f degrees\n",
                   angle);

            printf("Time to Hit: %.2f seconds\n",
                   timeToHit);

            printf("Result: E%d DESTROYED\n",
                   escorts[i].id);
        }
    }


    /*
       Now surviving E ships attack B.
       Each E can fire only once.
    */

    printf("\n========== ESCORT ATTACKS ==========\n");


    for (int i = 0; i < N; i++)
    {
        double distance;
        double velocity;
        double angle;
        double timeToHit;

        if (escorts[i].destroyed)
        {
            continue;
        }


        distance = calculateDistance(
            escorts[i].x,
            escorts[i].y,
            B.x,
            B.y
        );


        velocity = randomDouble(
            escorts[i].minVelocity,
            escorts[i].maxVelocity
        );


        angle = calculateFiringAngle(
            velocity,
            distance,
            escorts[i].minAngle
        );


        printf("\nE%d -> B\n",
               escorts[i].id);

        printf("Distance: %.2f m\n",
               distance);

        printf("Firing Velocity: %.2f m/s\n",
               velocity);


        if (angle < 0)
        {
            printf("Result: B OUT OF RANGE\n");
        }
        else
        {
            timeToHit =
                calculateTimeOfFlight(
                    velocity,
                    angle
                );

            printf("Firing Angle: %.2f degrees\n",
                   angle);

            printf("Time to Hit: %.2f seconds\n",
                   timeToHit);

            printf("Result: B DESTROYED\n");

            bSunk = 1;

            sinkingEscort =
                escorts[i].id;

            if (timeToHit > battleEndTime)
            {
                battleEndTime = timeToHit;
            }

            break;
        }
    }


    /* =========================================
       DISPLAY FINAL RESULT
       ========================================= */

    printf("\n========== FINAL RESULT ==========\n");


    if (bSunk)
    {
        printf("\nBATTLESHIP B HAS SUNK!\n");

        printf("E%d sank the Battleship.\n",
               sinkingEscort);
    }
    else
    {
        printf("\nBATTLESHIP B SURVIVED!\n");

        printf("Number of Escort ships destroyed: %d\n",
               hitCount);

        printf("Battle end time: %.2f seconds\n",
               battleEndTime);
    }


    /* =========================================
       SAVE BATTLE RESULTS
       ========================================= */

    resultFile =
        fopen("battle_results.txt", "w");


    if (resultFile == NULL)
    {
        printf("Error creating battle_results.txt\n");
        return 1;
    }


    fprintf(resultFile,
            "ADVANCED NAVAL BATTLE SIMULATOR\n");

    fprintf(resultFile,
            "BATTLE RESULTS\n");
    fprintf(resultFile,
            "============================\n\n");


    if (bSunk)
    {
        fprintf(resultFile,
                "Battleship B: DESTROYED\n");

        fprintf(resultFile,
                "E%d sank Battleship B.\n",
                sinkingEscort);
    }
    else
    {
        fprintf(resultFile,
                "Battleship B: SURVIVED\n");

        fprintf(resultFile,
                "Escort ships destroyed: %d\n",
                hitCount);

        fprintf(resultFile,
                "Battle end time: %.2f seconds\n\n",
                battleEndTime);


        fprintf(resultFile,
                "DESTROYED ESCORT SHIPS\n");

        for (int i = 0; i < N; i++)
        {
            if (escorts[i].destroyed)
            {
                double distance;
                double angle;
                double timeToHit;

                distance =
                    calculateDistance(
                        B.x,
                        B.y,
                        escorts[i].x,
                        escorts[i].y
                    );

                angle =
                    calculateFiringAngle(
                        B.maxVelocity,
                        distance,
                        0.0
                    );

                timeToHit =
                    calculateTimeOfFlight(
                        B.maxVelocity,
                        angle
                    );

                fprintf(resultFile,
                        "E%d - Time to hit: %.2f seconds\n",
                        escorts[i].id,
                        timeToHit);
            }
        }
    }


    fclose(resultFile);


    /* =========================================
       SAVE FINAL CONDITIONS
       ========================================= */

    finalFile =
        fopen("final_conditions.txt", "w");


    if (finalFile == NULL)
    {
        printf("Error creating final_conditions.txt\n");
        return 1;
    }


    fprintf(finalFile,
            "FINAL BATTLEFIELD CONDITIONS\n");

    fprintf(finalFile,
            "============================\n\n");

    fprintf(finalFile,
            "Battlefield: %d x %d\n",
            D, D);

    fprintf(finalFile,
            "Battleship: %s\n",
            getBattleshipName(B.type));

    fprintf(finalFile,
            "Battleship Position: "
            "(%.2f, %.2f)\n",
            B.x, B.y);

    fprintf(finalFile,
            "Battleship Status: %s\n\n",
            bSunk ? "DESTROYED" : "SURVIVED");


    fprintf(finalFile,
            "ESCORT SHIPS\n");
    fprintf(finalFile,
            "------------\n");


    for (int i = 0; i < N; i++)
    {
        fprintf(finalFile,
                "E%d | Type: E%c | "
                "Position: (%.2f, %.2f) | "
                "Status: %s\n",
                escorts[i].id,
                escorts[i].type,
                escorts[i].x,
                escorts[i].y,
                escorts[i].destroyed
                    ? "DESTROYED"
                    : "ALIVE");
    }


    fclose(finalFile);


    printf("\nOutput files created:\n");
    printf("1. initial_conditions.txt\n");
    printf("2. battle_results.txt\n");
    printf("3. final_conditions.txt\n");


    return 0;
}