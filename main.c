#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ESCORTS 100
#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define GRAVITY 9.81

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
   FUNCTIONS
   ========================================= */

double randomDouble(double min, double max)
{
    return min +
           ((double)rand() / RAND_MAX) *
           (max - min);
}


double degreesToRadians(double angle)
{
    return angle * PI / 180.0;
}


double calculateDistance(double x1, double y1,
                         double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    return sqrt(dx * dx + dy * dy);
}


/*
   Projectile range:

   R = V^2 sin(2theta) / g
*/
double calculateRange(double velocity, double angle)
{
    double radians = degreesToRadians(angle);

    return (velocity * velocity *
            sin(2.0 * radians)) / GRAVITY;
}


/*
   Find a possible firing angle.

   minimumAngle is used for Simulation 2
   after the gun becomes jammed.
*/
double calculateFiringAngle(double velocity,
                            double distance,
                            double minimumAngle)
{
    double value;
    double angle1;
    double angle2;

    if (velocity <= 0)
        return -1.0;

    value = (distance * GRAVITY) /
            (velocity * velocity);

    /*
       Target cannot be reached.
    */
    if (value > 1.0)
        return -1.0;

    /*
       Numerical protection.
    */
    if (value < 0.0)
        value = 0.0;

    angle1 =
        0.5 * asin(value) * 180.0 / PI;

    angle2 = 90.0 - angle1;

    /*
       Try the lower angle first.
    */
    if (angle1 >= minimumAngle &&
        angle1 <= 90.0)
    {
        return angle1;
    }

    /*
       Try the higher angle.
    */
    if (angle2 >= minimumAngle &&
        angle2 <= 90.0)
    {
        return angle2;
    }

    return -1.0;
}


double calculateTimeOfFlight(double velocity,
                             double angle)
{
    double radians =
        degreesToRadians(angle);

    return (2.0 * velocity *
            sin(radians)) / GRAVITY;
}


const char *getBattleshipName(char type)
{
    switch (type)
    {
        case 'U':
            return "USS Iowa";

        case 'M':
            return "MS King George V";

        case 'R':
            return "Richelieu";

        case 'S':
            return "Sovetsky Soyuz-class";

        default:
            return "Unknown";
    }
}


/*
   Set properties according to the assignment table.

   EA = impact 0.08, minimum angle 20
   EB = impact 0.06, minimum angle 30
   EC = impact 0.07, minimum angle 25
   ED = impact 0.05, minimum angle 50
   EE = impact 0.04, minimum angle 70
*/
void setEscortProperties(EscortShip *ship,
                         double battleshipMaxVelocity)
{
    switch (ship->type)
    {
        case 'A':
            ship->impactPower = 0.08;
            ship->minAngle = 20.0;

            ship->minVelocity =
                randomDouble(
                    0.2 * battleshipMaxVelocity,
                    0.5 * battleshipMaxVelocity
                );

            ship->maxVelocity =
                1.2 * battleshipMaxVelocity;
            break;


        case 'B':
            ship->impactPower = 0.06;
            ship->minAngle = 30.0;

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
            break;


        case 'C':
            ship->impactPower = 0.07;
            ship->minAngle = 25.0;

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
            break;


        case 'D':
            ship->impactPower = 0.05;
            ship->minAngle = 50.0;

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
            break;


        case 'E':
            ship->impactPower = 0.04;
            ship->minAngle = 70.0;

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
            break;
    }
}


/* =========================================
   COPY ESCORT SHIPS
   ========================================= */

void copyEscorts(EscortShip source[],
                 EscortShip destination[],
                 int N)
{
    for (int i = 0; i < N; i++)
    {
        destination[i] = source[i];
    }
}


/* =========================================
   SAVE INITIAL CONDITIONS
   ========================================= */

void saveInitialConditions(
    int D,
    int N,
    int k,
    int t,
    double thetaMin,
    Battleship *B,
    EscortShip escorts[],
    Point path[])
{
    FILE *file =
        fopen("initial_conditions.txt", "w");

    if (file == NULL)
    {
        printf("Error creating initial_conditions.txt\n");
        return;
    }

    fprintf(file,
            "ADVANCED NAVAL BATTLE SIMULATOR\n");
    fprintf(file,
            "PART 1-B - INITIAL CONDITIONS\n");
    fprintf(file,
            "========================================\n\n");

    fprintf(file,
            "Battlefield Size: %d x %d\n",
            D, D);

    fprintf(file,
            "Number of Escort Ships: %d\n",
            N);

    fprintf(file,
            "Number of Path Points k: %d\n",
            k);

    fprintf(file,
            "Gun Jam Iteration t: %d\n",
            t);

    fprintf(file,
            "Theta Minimum: %.2f degrees\n",
            thetaMin);

    fprintf(file,
            "Battleship: %s\n",
            getBattleshipName(B->type));

    fprintf(file,
            "Battleship Position: (%.2f, %.2f)\n",
            B->x, B->y);

    fprintf(file,
            "Battleship Maximum Shell Velocity: %.2f m/s\n\n",
            B->maxVelocity);


    fprintf(file,
            "BATTLESHIP PATH\n");
    fprintf(file,
            "----------------------------------------\n");

    for (int i = 0; i < k; i++)
    {
        fprintf(file,
                "Point %d: (%.2f, %.2f)\n",
                i + 1,
                path[i].x,
                path[i].y);
    }


    fprintf(file,
            "\nESCORT SHIPS\n");
    fprintf(file,
            "----------------------------------------\n");

    for (int i = 0; i < N; i++)
    {
        fprintf(file,
                "E%d | Type EA-E%c | "
                "Position (%.2f, %.2f) | "
                "Vmin %.2f | Vmax %.2f | "
                "Min Angle %.2f | "
                "Impact Power %.2f\n",

                escorts[i].id,
                escorts[i].type,

                escorts[i].x,
                escorts[i].y,

                escorts[i].minVelocity,
                escorts[i].maxVelocity,

                escorts[i].minAngle,
                escorts[i].impactPower);
    }

    fclose(file);
}


/* =========================================
   RUN ONE SIMULATION
   ========================================= */

void runSimulation(
    int simulationNumber,
    int D,
    int N,
    int k,
    int t,
    double thetaMin,
    Battleship B,
    EscortShip escorts[],
    Point path[])
{
    char filename[100];

    sprintf(filename,
            "simulation%d_results.txt",
            simulationNumber);

    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Error creating result file.\n");
        return;
    }


    fprintf(file,
            "PART 1-B - SIMULATION %d\n",
            simulationNumber);

    fprintf(file,
            "========================================\n\n");


    printf("\n\n");
    printf("========================================\n");

    if (simulationNumber == 1)
    {
        printf("      PART 1-B - SIMULATION 1\n");
    }
    else
    {
        printf("      PART 1-B - SIMULATION 2\n");
    }

    printf("========================================\n");


    int battleshipDestroyed = 0;

    int totalDestroyed = 0;

    double totalBattleTime = 0.0;


    /*
       ----------------------------------------
       ITERATIONS
       ----------------------------------------
    */

    for (int iteration = 0;
         iteration < k;
         iteration++)
    {
        double iterationTime = 0.0;

        int destroyedThisIteration = 0;


        /*
           Move B to the current path point.
        */
        B.x = path[iteration].x;
        B.y = path[iteration].y;


        /*
           Determine B's minimum firing angle.

           Simulation 1:
           0 degrees to 90 degrees.

           Simulation 2:
           Before t: 0 to 90
           After t: thetaMin to 90
        */
        double B_minimumAngle = 0.0;


        if (simulationNumber == 2 &&
            iteration + 1 > t)
        {
            B_minimumAngle = thetaMin;
        }


        printf("\n");
        printf("----------------------------------------\n");

        printf("ITERATION %d / %d\n",
               iteration + 1, k);

        printf("----------------------------------------\n");

        printf("B Position: (%.2f, %.2f)\n",
               B.x, B.y);


        fprintf(file,
                "\n----------------------------------------\n");

        fprintf(file,
                "ITERATION %d / %d\n",
                iteration + 1, k);

        fprintf(file,
                "----------------------------------------\n");

        fprintf(file,
                "B Position: (%.2f, %.2f)\n",
                B.x, B.y);


        if (simulationNumber == 2)
        {
            if (iteration + 1 <= t)
            {
                printf("B Gun: NORMAL (0 - 90 degrees)\n");

                fprintf(file,
                        "B Gun: NORMAL (0 - 90 degrees)\n");
            }
            else
            {
                printf("B Gun: JAMMED (%.2f - 90 degrees)\n",
                       thetaMin);

                fprintf(file,
                        "B Gun: JAMMED (%.2f - 90 degrees)\n",
                        thetaMin);
            }
        }


        /* =====================================
           B ATTACKS E SHIPS
           ===================================== */

        printf("\nBATTLESHIP B ATTACKS\n");

        fprintf(file,
                "\nBATTLESHIP B ATTACKS\n");


        for (int i = 0; i < N; i++)
        {
            if (escorts[i].destroyed)
            {
                continue;
            }


            double distance =
                calculateDistance(
                    B.x,
                    B.y,
                    escorts[i].x,
                    escorts[i].y
                );


            double angle =
                calculateFiringAngle(
                    B.maxVelocity,
                    distance,
                    B_minimumAngle
                );


            printf("\nB -> E%d\n",
                   escorts[i].id);

            printf("Distance: %.2f m\n",
                   distance);


            fprintf(file,
                    "\nB -> E%d\n",
                    escorts[i].id);

            fprintf(file,
                    "Distance: %.2f m\n",
                    distance);


            if (angle < 0)
            {
                printf("Result: OUT OF RANGE / ANGLE NOT AVAILABLE\n");

                fprintf(file,
                        "Result: OUT OF RANGE / ANGLE NOT AVAILABLE\n");
            }
            else
            {
                double timeToHit =
                    calculateTimeOfFlight(
                        B.maxVelocity,
                        angle
                    );


                escorts[i].destroyed = 1;

                destroyedThisIteration++;

                totalDestroyed++;


                if (timeToHit > iterationTime)
                {
                    iterationTime = timeToHit;
                }


                printf("Firing Angle: %.2f degrees\n",
                       angle);

                printf("Time to Hit: %.2f seconds\n",
                       timeToHit);

                printf("Result: E%d DESTROYED\n",
                       escorts[i].id);


                fprintf(file,
                        "Firing Angle: %.2f degrees\n",
                        angle);

                fprintf(file,
                        "Time to Hit: %.2f seconds\n",
                        timeToHit);

                fprintf(file,
                        "Result: E%d DESTROYED\n",
                        escorts[i].id);
            }
        }


        /* =====================================
           E SHIPS ATTACK B
           ===================================== */

        printf("\nESCORT ATTACKS\n");

        fprintf(file,
                "\nESCORT ATTACKS\n");


        for (int i = 0; i < N; i++)
        {
            /*
               Destroyed E ships cannot attack.
            */
            if (escorts[i].destroyed)
            {
                continue;
            }


            double distance =
                calculateDistance(
                    escorts[i].x,
                    escorts[i].y,
                    B.x,
                    B.y
                );


            /*
               Select a velocity from the
               generated velocity range.
            */
            double velocity =
                randomDouble(
                    escorts[i].minVelocity,
                    escorts[i].maxVelocity
                );


            double angle =
                calculateFiringAngle(
                    velocity,
                    distance,
                    escorts[i].minAngle
                );


            printf("\nE%d -> B\n",
                   escorts[i].id);

            printf("Distance: %.2f m\n",
                   distance);


            fprintf(file,
                    "\nE%d -> B\n",
                    escorts[i].id);

            fprintf(file,
                    "Distance: %.2f m\n",
                    distance);


            if (angle < 0)
            {
                printf("Result: OUT OF RANGE\n");

                fprintf(file,
                        "Result: OUT OF RANGE\n");
            }
            else
            {
                double timeToHit =
                    calculateTimeOfFlight(
                        velocity,
                        angle
                    );


                printf("Velocity: %.2f m/s\n",
                       velocity);

                printf("Angle: %.2f degrees\n",
                       angle);

                printf("Time to Hit: %.2f seconds\n",
                       timeToHit);

                printf("Result: B DESTROYED\n");


                fprintf(file,
                        "Velocity: %.2f m/s\n",
                        velocity);

                fprintf(file,
                        "Angle: %.2f degrees\n",
                        angle);

                fprintf(file,
                        "Time to Hit: %.2f seconds\n",
                        timeToHit);

                fprintf(file,
                        "Result: B DESTROYED\n");


                B.destroyed = 1;

                battleshipDestroyed = 1;


                if (timeToHit > iterationTime)
                {
                    iterationTime = timeToHit;
                }


                /*
                   Stop checking other E ships
                   because B is already destroyed.
                */
                break;
            }
        }


        /* =====================================
           ITERATION SUMMARY
           ===================================== */

        totalBattleTime += iterationTime;


        printf("\nIteration %d complete.\n",
               iteration + 1);

        printf("Escort ships destroyed: %d\n",
               destroyedThisIteration);

        printf("Iteration time: %.2f seconds\n",
               iterationTime);


        fprintf(file,
                "\nITERATION SUMMARY\n");

        fprintf(file,
                "Escort ships destroyed: %d\n",
                destroyedThisIteration);

        fprintf(file,
                "Iteration time: %.2f seconds\n",
                iterationTime);


        /*
           Show remaining E ships.
        */

        printf("Remaining Escort Ships: ");

        fprintf(file,
                "Remaining Escort Ships: ");


        int remaining = 0;

        for (int i = 0; i < N; i++)
        {
            if (!escorts[i].destroyed)
            {
                printf("E%d ",
                       escorts[i].id);

                fprintf(file,
                        "E%d ",
                        escorts[i].id);

                remaining++;
            }
        }


        if (remaining == 0)
        {
            printf("None");
            fprintf(file, "None");
        }


        printf("\n");

        fprintf(file, "\n");


        /*
           If B is destroyed, stop.
        */

        if (battleshipDestroyed)
        {
            printf("\nBATTLESHIP B HAS BEEN DESTROYED!\n");

            printf("Simulation stopped at iteration %d.\n",
                   iteration + 1);


            fprintf(file,
                    "\nBATTLESHIP B HAS BEEN DESTROYED!\n");

            fprintf(file,
                    "Simulation stopped at iteration %d.\n",
                    iteration + 1);

            break;
        }
    }


    /* =========================================
       FINAL RESULT
       ========================================= */

    printf("\n");
    printf("========================================\n");
    printf("FINAL RESULT - SIMULATION %d\n",
           simulationNumber);
    printf("========================================\n");


    fprintf(file,
            "\n========================================\n");

    fprintf(file,
            "FINAL RESULT - SIMULATION %d\n",
            simulationNumber);

    fprintf(file,
            "========================================\n");


    if (battleshipDestroyed)
    {
        printf("Battleship B: DESTROYED\n");

        fprintf(file,
                "Battleship B: DESTROYED\n");
    }
    else
    {
        printf("Battleship B: SURVIVED\n");

        fprintf(file,
                "Battleship B: SURVIVED\n");
    }


    printf("Escort ships destroyed: %d / %d\n",
           totalDestroyed, N);

    printf("Total simulation time: %.2f seconds\n",
           totalBattleTime);


    fprintf(file,
            "Escort ships destroyed: %d / %d\n",
            totalDestroyed, N);

    fprintf(file,
            "Total simulation time: %.2f seconds\n",
            totalBattleTime);


    /* =========================================
       FINAL ESCORT STATUS
       ========================================= */

    printf("\nFINAL ESCORT STATUS\n");

    fprintf(file,
            "\nFINAL ESCORT STATUS\n");


    for (int i = 0; i < N; i++)
    {
        printf("E%d: %s\n",
               escorts[i].id,
               escorts[i].destroyed
                   ? "DESTROYED"
                   : "ALIVE");


        fprintf(file,
                "E%d: %s\n",
                escorts[i].id,
                escorts[i].destroyed
                    ? "DESTROYED"
                    : "ALIVE");
    }


    fclose(file);


    printf("\nOutput saved to: %s\n",
           filename);
}


/* =========================================
   MAIN
   ========================================= */

int main()
{
    int D;
    int N;
    int k;

    int t;

    int battleshipChoice;

    int seed;

    double thetaMin;

    double battleshipMaxVelocity;


    Battleship B;

    EscortShip escorts[MAX_ESCORTS];

    EscortShip initialEscorts[MAX_ESCORTS];

    EscortShip simulationEscorts[MAX_ESCORTS];

    Point path[MAX_POINTS];


    char escortTypes[] =
        {'A', 'B', 'C', 'D', 'E'};


    /* =====================================
       TITLE
       ===================================== */

    printf("========================================\n");
    printf("      ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("========================================\n");


    /* =====================================
       RANDOM SEED
       ===================================== */

    printf("\nEnter random seed: ");
    scanf("%d", &seed);

    srand(seed);


    /* =====================================
       BATTLEFIELD
       ===================================== */

    printf("\nEnter battlefield size D: ");
    scanf("%d", &D);

    if (D <= 0)
    {
        printf("Invalid battlefield size.\n");
        return 1;
    }


    /* =====================================
       ESCORT SHIPS
       ===================================== */

    printf("Enter number of escort ships N: ");
    scanf("%d", &N);

    if (N <= 0 || N > MAX_ESCORTS)
    {
        printf("Invalid number of escort ships.\n");
        return 1;
    }


    /* =====================================
       PATH POINTS
       ===================================== */

    printf("Enter number of path points k: ");
    scanf("%d", &k);

    if (k <= 1 || k > MAX_POINTS)
    {
        printf("k must be greater than 1.\n");
        return 1;
    }


    /* =====================================
       SIMULATION 2 SETTINGS
       ===================================== */

    printf("\nEnter gun jam iteration t (0 < t < k): ");
    scanf("%d", &t);

    if (t <= 0 || t >= k)
    {
        printf("Invalid t. It must satisfy 0 < t < k.\n");
        return 1;
    }


    printf("Enter theta_min (0 < theta_min < 30): ");
    scanf("%lf", &thetaMin);

    if (thetaMin <= 0 || thetaMin >= 30)
    {
        printf("Invalid theta_min.\n");
        return 1;
    }


    /* =====================================
       BATTLESHIP
       ===================================== */

    printf("\nSelect Battleship:\n");

    printf("1. USS Iowa\n");
    printf("2. MS King George V\n");
    printf("3. Richelieu\n");
    printf("4. Sovetsky Soyuz-class\n");

    printf("Enter choice: ");
    scanf("%d", &battleshipChoice);


    switch (battleshipChoice)
    {
        case 1:
            B.type = 'U';
            break;

        case 2:
            B.type = 'M';
            break;

        case 3:
            B.type = 'R';
            break;

        case 4:
            B.type = 'S';
            break;

        default:
            printf("Invalid Battleship choice.\n");
            return 1;
    }


    printf("\nEnter Battleship maximum shell velocity: ");
    scanf("%lf", &battleshipMaxVelocity);

    if (battleshipMaxVelocity <= 0)
    {
        printf("Velocity must be greater than zero.\n");
        return 1;
    }


    B.maxVelocity = battleshipMaxVelocity;

    B.destroyed = 0;


    /* =====================================
       GENERATE ESCORT SHIPS
       ===================================== */

    printf("\nGenerating Escort Ships...\n");


    for (int i = 0; i < N; i++)
    {
        escorts[i].id = i + 1;

        escorts[i].type =
            escortTypes[rand() % 5];

        escorts[i].x =
            randomDouble(0, D);

        escorts[i].y =
            randomDouble(0, D);

        escorts[i].destroyed = 0;


        setEscortProperties(
            &escorts[i],
            B.maxVelocity
        );


        printf("E%d | Type EA-E%c | "
               "Position (%.2f, %.2f)\n",
               escorts[i].id,
               escorts[i].type,
               escorts[i].x,
               escorts[i].y);
    }


    /* =====================================
       SAVE ORIGINAL ESCORT CONDITIONS
       ===================================== */

    copyEscorts(
        escorts,
        initialEscorts,
        N
    );


    /* =====================================
       GENERATE B PATH
       ===================================== */

    printf("\nGenerating Battleship path...\n");


    for (int i = 0; i < k; i++)
    {
        path[i].x =
            randomDouble(0, D);

        path[i].y =
            randomDouble(0, D);


        printf("Point %d: (%.2f, %.2f)\n",
               i + 1,
               path[i].x,
               path[i].y);
    }


    /*
       Initial B position is first point.
    */

    B.x = path[0].x;
    B.y = path[0].y;


    /* =====================================
       SAVE INITIAL CONDITIONS
       ===================================== */

    saveInitialConditions(
        D,
        N,
        k,
        t,
        thetaMin,
        &B,
        initialEscorts,
        path
    );


    printf("\nInitial conditions saved.\n");


    /* =====================================
       SIMULATION 1
       ===================================== */

    copyEscorts(
        initialEscorts,
        simulationEscorts,
        N
    );


    /*
       Reset B.
    */

    B.destroyed = 0;

    B.x = path[0].x;
    B.y = path[0].y;


    runSimulation(
        1,
        D,
        N,
        k,
        t,
        thetaMin,
        B,
        simulationEscorts,
        path
    );


    /* =====================================
       SIMULATION 2
       ===================================== */

    printf("\n\n");
    printf("========================================\n");
    printf("RESETTING TO SAME INITIAL CONDITIONS\n");
    printf("FOR SIMULATION 2\n");
    printf("========================================\n");


    /*
       IMPORTANT:
       Start Simulation 2 from the original
       battlefield again.

       This satisfies the assignment's
       "same initial conditions" requirement.
    */

    copyEscorts(
        initialEscorts,
        simulationEscorts,
        N
    );


    B.destroyed = 0;

    B.x = path[0].x;
    B.y = path[0].y;


    runSimulation(
        2,
        D,
        N,
        k,
        t,
        thetaMin,
        B,
        simulationEscorts,
        path
    );


    /* =====================================
       FINISHED
       ===================================== */

    printf("\n");
    printf("========================================\n");
    printf("      PART 1-B COMPLETED\n");
    printf("========================================\n");

    printf("\nFiles created:\n");

    printf("1. initial_conditions.txt\n");
    printf("2. simulation1_results.txt\n");
    printf("3. simulation2_results.txt\n");

    printf("\nSimulation 1 = normal gun\n");
    printf("Simulation 2 = gun jams after t iterations\n");
    printf("Simulation 2 angle range = %.2f - 90 degrees\n",
           thetaMin);


    return 0;
}