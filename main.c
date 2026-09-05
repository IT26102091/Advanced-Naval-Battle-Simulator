#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_ESCORTS 100
#define MAX_POINTS 100
#define GRAVITY 9.81
#define PI 3.14159265358979323846


/* =========================================
   STRUCTURES
   ========================================= */

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


typedef struct
{
    double x;
    double y;

} Point;


/* =========================================
   DEGREE TO RADIAN
   ========================================= */

double degreesToRadians(double angle)
{
    return angle * PI / 180.0;
}


/* =========================================
   RANDOM DECIMAL NUMBER
   ========================================= */

double randomDouble(double minimum, double maximum)
{
    return minimum +
           ((double)rand() / RAND_MAX) *
           (maximum - minimum);
}


/* =========================================
   DISTANCE BETWEEN TWO POINTS
   ========================================= */

double calculateDistance(double x1, double y1,
                         double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    return sqrt(dx * dx + dy * dy);
}


/* =========================================
   PROJECTILE RANGE
   ========================================= */

double calculateRange(double velocity, double angle)
{
    double angleRadians;

    angleRadians = degreesToRadians(angle);

    return (velocity * velocity *
            sin(2 * angleRadians)) / GRAVITY;
}


/* =========================================
   FIND FIRING ANGLE
   ========================================= */

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

    angle1 = 0.5 *
             asin(value) *
             180.0 / PI;

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


/* =========================================
   PROJECTILE TIME
   ========================================= */

double calculateTimeOfFlight(double velocity,
                             double angle)
{
    double angleRadians;

    angleRadians = degreesToRadians(angle);

    return (2 * velocity *
            sin(angleRadians)) / GRAVITY;
}


/* =========================================
   BATTLESHIP NAME
   ========================================= */

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


/* =========================================
   ESCORT SHIP PROPERTIES
   ========================================= */

void setEscortProperties(EscortShip *ship,
                         double battleshipMaxVelocity)
{
    if (ship->type == 'A')
    {
        ship->minAngle = 20.0;
        ship->impactPower = 0.08;

        ship->minVelocity =
            randomDouble(
                0.2 * battleshipMaxVelocity,
                0.5 * battleshipMaxVelocity
            );

        ship->maxVelocity =
            randomDouble(
                0.8 * battleshipMaxVelocity,
                1.2 * battleshipMaxVelocity
            );
    }

    else if (ship->type == 'B')
    {
        ship->minAngle = 30.0;
        ship->impactPower = 0.06;

        ship->minVelocity =
            randomDouble(
                0.2 * battleshipMaxVelocity,
                0.5 * battleshipMaxVelocity
            );

        ship->maxVelocity =
            randomDouble(
                0.5 * battleshipMaxVelocity,
                0.9 * battleshipMaxVelocity
            );
    }

    else if (ship->type == 'C')
    {
        ship->minAngle = 25.0;
        ship->impactPower = 0.07;

        ship->minVelocity =
            randomDouble(
                0.2 * battleshipMaxVelocity,
                0.5 * battleshipMaxVelocity
            );

        ship->maxVelocity =
            randomDouble(
                0.5 * battleshipMaxVelocity,
                0.9 * battleshipMaxVelocity
            );
    }

    else if (ship->type == 'D')
    {
        ship->minAngle = 50.0;
        ship->impactPower = 0.05;

        ship->minVelocity =
            randomDouble(
                0.2 * battleshipMaxVelocity,
                0.5 * battleshipMaxVelocity
            );

        ship->maxVelocity =
            randomDouble(
                0.5 * battleshipMaxVelocity,
                0.9 * battleshipMaxVelocity
            );
    }

    else
    {
        ship->minAngle = 70.0;
        ship->impactPower = 0.04;

        ship->minVelocity =
            randomDouble(
                0.2 * battleshipMaxVelocity,
                0.5 * battleshipMaxVelocity
            );

        ship->maxVelocity =
            randomDouble(
                0.5 * battleshipMaxVelocity,
                0.9 * battleshipMaxVelocity
            );
    }
}


/* =========================================
   MAIN PROGRAM
   ========================================= */

int main()
{
    int D;
    int N;
    int k;
    int battleshipChoice;

    int battleshipDestroyed = 0;

    double battleshipMaxVelocity;

    Battleship B;
    EscortShip escorts[MAX_ESCORTS];
    Point path[MAX_POINTS];

    char escortTypes[] =
        {'A', 'B', 'C', 'D', 'E'};

    FILE *initialFile;
    FILE *resultFile;


    srand((unsigned int)time(NULL));


    /* =========================================
       TITLE
       ========================================= */

    printf("========================================\n");
    printf("      ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("========================================\n");


    /* =========================================
       INPUT BATTLEFIELD
       ========================================= */

    printf("\nEnter battlefield size D: ");
    scanf("%d", &D);

    if (D <= 0)
    {
        printf("Invalid battlefield size.\n");
        return 1;
    }


    /* =========================================
       INPUT ESCORT SHIPS
       ========================================= */

    printf("Enter number of escort ships: ");
    scanf("%d", &N);

    if (N <= 0 || N > MAX_ESCORTS)
    {
        printf("Invalid number of escort ships.\n");
        return 1;
    }


    /* =========================================
       INPUT NUMBER OF PATH POINTS
       ========================================= */

    printf("Enter number of path points k: ");
    scanf("%d", &k);

    if (k <= 0 || k > MAX_POINTS)
    {
        printf("Invalid number of path points.\n");
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
       BATTLESHIP VELOCITY
       ========================================= */

    printf("\nEnter Battleship maximum shell velocity: ");
    scanf("%lf", &battleshipMaxVelocity);

    if (battleshipMaxVelocity <= 0)
    {
        printf("Velocity must be greater than zero.\n");
        return 1;
    }

    B.maxVelocity = battleshipMaxVelocity;
    B.destroyed = 0;


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
       GENERATE BATTLESHIP PATH
       ========================================= */

    printf("\nGenerating Battleship path...\n");

    for (int i = 0; i < k; i++)
    {
        path[i].x =
            randomDouble(0.0, (double)D);

        path[i].y =
            randomDouble(0.0, (double)D);
    }


    /* =========================================
       INITIAL POSITION
       ========================================= */

    B.x = path[0].x;
    B.y = path[0].y;


    /* =========================================
       SAVE INITIAL CONDITIONS
       ========================================= */

    initialFile =
        fopen("initial_conditions_1B.txt", "w");


    if (initialFile == NULL)
    {
        printf("Error creating initial conditions file.\n");
        return 1;
    }


    fprintf(initialFile,
            "ADVANCED NAVAL BATTLE SIMULATOR\n");

    fprintf(initialFile,
            "PART 1-B - SIMULATION 1\n");

    fprintf(initialFile,
            "====================================\n\n");


    fprintf(initialFile,
            "Battlefield Size: %d x %d\n",
            D, D);

    fprintf(initialFile,
            "Number of Escort Ships: %d\n",
            N);

    fprintf(initialFile,
            "Number of Path Points: %d\n",
            k);

    fprintf(initialFile,
            "Battleship: %s\n",
            getBattleshipName(B.type));

    fprintf(initialFile,
            "Battleship Maximum Velocity: %.2f m/s\n\n",
            B.maxVelocity);


    /* =========================================
       SAVE PATH
       ========================================= */

    fprintf(initialFile,
            "BATTLESHIP PATH\n");

    fprintf(initialFile,
            "----------------\n");


    for (int i = 0; i < k; i++)
    {
        fprintf(initialFile,
                "Point %d: (%.2f, %.2f)\n",
                i + 1,
                path[i].x,
                path[i].y);
    }


    /* =========================================
       SAVE ESCORT SHIPS
       ========================================= */

    fprintf(initialFile,
            "\nESCORT SHIPS\n");

    fprintf(initialFile,
            "------------\n");


    for (int i = 0; i < N; i++)
    {
        fprintf(initialFile,
                "E%d | Type: E%c | "
                "Position: (%.2f, %.2f) | "
                "Velocity: %.2f - %.2f m/s | "
                "Minimum Angle: %.2f degrees | "
                "Impact Power: %.2f\n",

                escorts[i].id,
                escorts[i].type,

                escorts[i].x,
                escorts[i].y,

                escorts[i].minVelocity,
                escorts[i].maxVelocity,

                escorts[i].minAngle,
                escorts[i].impactPower);
    }


    fclose(initialFile);


    /* =========================================
       CREATE RESULT FILE
       ========================================= */

    resultFile =
        fopen("part1B_simulation1_results.txt", "w");


    if (resultFile == NULL)
    {
        printf("Error creating result file.\n");
        return 1;
    }


    fprintf(resultFile,
            "PART 1-B - SIMULATION 1 RESULTS\n");

    fprintf(resultFile,
            "====================================\n\n");


    /* =========================================
       SIMULATE EACH PATH POINT
       ========================================= */

    for (int point = 0;
         point < k;
         point++)
    {
        double iterationEndTime = 0.0;
        int destroyedThisIteration = 0;


        /* Move Battleship to this path point */

        B.x = path[point].x;
        B.y = path[point].y;


        printf("\n");
        printf("========================================\n");

        printf("ITERATION %d / %d\n",
               point + 1,
               k);

        printf("========================================\n");

        printf("B position: (%.2f, %.2f)\n",
               B.x,
               B.y);


        fprintf(resultFile,
                "\n====================================\n");

        fprintf(resultFile,
                "ITERATION %d / %d\n",
                point + 1,
                k);

        fprintf(resultFile,
                "====================================\n");

        fprintf(resultFile,
                "B Position: (%.2f, %.2f)\n\n",
                B.x,
                B.y);


        /* =====================================
           B ATTACKS SURVIVING E SHIPS
           ===================================== */

        printf("\nBATTLESHP B ATTACKS\n");


        for (int i = 0; i < N; i++)
        {
            double distance;
            double angle;
            double timeToHit;


            /* Destroyed ships do not participate */

            if (escorts[i].destroyed)
            {
                continue;
            }


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


            printf("\nB -> E%d\n",
                   escorts[i].id);

            printf("Distance: %.2f m\n",
                   distance);


            fprintf(resultFile,
                    "B -> E%d\n",
                    escorts[i].id);

            fprintf(resultFile,
                    "Distance: %.2f m\n",
                    distance);


            /* Check if target is in range */

            if (angle < 0)
            {
                printf("Result: OUT OF RANGE\n");

                fprintf(resultFile,
                        "Result: OUT OF RANGE\n\n");
            }

            else
            {
                timeToHit =
                    calculateTimeOfFlight(
                        B.maxVelocity,
                        angle
                    );


                escorts[i].destroyed = 1;

                destroyedThisIteration++;


                if (timeToHit >
                    iterationEndTime)
                {
                    iterationEndTime =
                        timeToHit;
                }


                printf("Angle: %.2f degrees\n",
                       angle);

                printf("Time to hit: %.2f seconds\n",
                       timeToHit);

                printf("Result: E%d DESTROYED\n",
                       escorts[i].id);


                fprintf(resultFile,
                        "Firing Angle: %.2f degrees\n",
                        angle);

                fprintf(resultFile,
                        "Time to Hit: %.2f seconds\n",
                        timeToHit);

                fprintf(resultFile,
                        "Result: E%d DESTROYED\n\n",
                        escorts[i].id);
            }
        }


        /* =====================================
           E SHIPS ATTACK B
           ===================================== */

        printf("\nESCORT ATTACKS\n");


        fprintf(resultFile,
                "\nESCORT ATTACKS\n");


        for (int i = 0; i < N; i++)
        {
            double distance;
            double velocity;
            double angle;
            double timeToHit;


            /* Destroyed E ships cannot attack */

            if (escorts[i].destroyed)
            {
                continue;
            }


            distance =
                calculateDistance(
                    escorts[i].x,
                    escorts[i].y,
                    B.x,
                    B.y
                );


            velocity =
                randomDouble(
                    escorts[i].minVelocity,
                    escorts[i].maxVelocity
                );


            angle =
                calculateFiringAngle(
                    velocity,
                    distance,
                    escorts[i].minAngle
                );


            printf("\nE%d -> B\n",
                   escorts[i].id);

            printf("Distance: %.2f m\n",
                   distance);


            fprintf(resultFile,
                    "\nE%d -> B\n",
                    escorts[i].id);

            fprintf(resultFile,
                    "Distance: %.2f m\n",
                    distance);


            if (angle < 0)
            {
                printf("Result: OUT OF RANGE\n");

                fprintf(resultFile,
                        "Result: OUT OF RANGE\n");
            }

            else
            {
                timeToHit =
                    calculateTimeOfFlight(
                        velocity,
                        angle
                    );


                printf("Angle: %.2f degrees\n",
                       angle);

                printf("Velocity: %.2f m/s\n",
                       velocity);

                printf("Time to hit: %.2f seconds\n",
                       timeToHit);

                printf("Result: B DESTROYED\n");


                fprintf(resultFile,
                        "Velocity: %.2f m/s\n",
                        velocity);

                fprintf(resultFile,
                        "Angle: %.2f degrees\n",
                        angle);

                fprintf(resultFile,
                        "Time to Hit: %.2f seconds\n",
                        timeToHit);

                fprintf(resultFile,
                        "Result: B DESTROYED\n");


                B.destroyed = 1;

                battleshipDestroyed = 1;


                if (timeToHit >
                    iterationEndTime)
                {
                    iterationEndTime =
                        timeToHit;
                }


                break;
            }
        }


        /* =====================================
           ITERATION SUMMARY
           ===================================== */

        printf("\nIteration %d complete.\n",
               point + 1);

        printf("Escort ships destroyed this iteration: %d\n",
               destroyedThisIteration);

        printf("Iteration end time: %.2f seconds\n",
               iterationEndTime);


        fprintf(resultFile,
                "\nITERATION SUMMARY\n");

        fprintf(resultFile,
                "Escort ships destroyed this iteration: %d\n",
                destroyedThisIteration);

        fprintf(resultFile,
                "Iteration end time: %.2f seconds\n",
                iterationEndTime);


        /* =====================================
           CHECK IF B WAS DESTROYED
           ===================================== */

        if (battleshipDestroyed)
        {
            printf("\nBATTLESHIP B HAS BEEN DESTROYED!\n");

            printf("Simulation stopped at iteration %d.\n",
                   point + 1);


            fprintf(resultFile,
                    "\nBATTLESHIP B HAS BEEN DESTROYED!\n");

            fprintf(resultFile,
                    "Simulation stopped at iteration %d.\n",
                    point + 1);

            break;
        }


        /* =====================================
           SHOW SURVIVING E SHIPS
           ===================================== */

        printf("\nSurviving Escort Ships:\n");

        fprintf(resultFile,
                "\nSurviving Escort Ships:\n");


        for (int i = 0; i < N; i++)
        {
            if (!escorts[i].destroyed)
            {
                printf("E%d ",
                       escorts[i].id);

                fprintf(resultFile,
                        "E%d ",
                        escorts[i].id);
            }
        }


        printf("\n");

        fprintf(resultFile,
                "\n");
    }


    /* =========================================
       FINAL RESULT
       ========================================= */

    printf("\n========================================\n");
    printf("          FINAL SIMULATION RESULT\n");
    printf("========================================\n");


    fprintf(resultFile,
            "\n====================================\n");

    fprintf(resultFile,
            "FINAL SIMULATION RESULT\n");

    fprintf(resultFile,
            "====================================\n");


    if (battleshipDestroyed)
    {
        printf("Battleship B: DESTROYED\n");

        fprintf(resultFile,
                "Battleship B: DESTROYED\n");
    }

    else
    {
        int totalDestroyed = 0;


        for (int i = 0; i < N; i++)
        {
            if (escorts[i].destroyed)
            {
                totalDestroyed++;
            }
        }


        printf("Battleship B: SURVIVED\n");

        printf("Escort ships destroyed: %d / %d\n",
               totalDestroyed,
               N);


        fprintf(resultFile,
                "Battleship B: SURVIVED\n");

        fprintf(resultFile,
                "Escort ships destroyed: %d / %d\n",
                totalDestroyed,
                N);
    }


    /* =========================================
       FINAL SHIP STATUS
       ========================================= */

    printf("\nFinal Escort Ship Status:\n");

    fprintf(resultFile,
            "\nFinal Escort Ship Status:\n");


    for (int i = 0; i < N; i++)
    {
        printf("E%d: %s\n",
               escorts[i].id,
               escorts[i].destroyed
                   ? "DESTROYED"
                   : "ALIVE");


        fprintf(resultFile,
                "E%d: %s\n",
                escorts[i].id,
                escorts[i].destroyed
                    ? "DESTROYED"
                    : "ALIVE");
    }


    fclose(resultFile);


    /* =========================================
       FILE INFORMATION
       ========================================= */

    printf("\nOutput files created:\n");

    printf("1. initial_conditions_1B.txt\n");

    printf("2. part1B_simulation1_results.txt\n");


    return 0;
}