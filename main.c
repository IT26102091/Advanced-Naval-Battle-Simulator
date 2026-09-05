#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ESCORTS 100
#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define GRAVITY 9.81

typedef struct {
    int id;
    char type;
    double x, y;
    double minVelocity, maxVelocity;
    double minAngle, impactPower;
    int destroyed;
} EscortShip;

typedef struct {
    char type;
    double x, y;
    double maxVelocity;
    double damage;
    int destroyed;
} Battleship;

typedef struct {
    double x, y;
} Point;


/* Generate random decimal number */
double randomDouble(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}


/* Convert degrees to radians */
double degreesToRadians(double angle) {
    return angle * PI / 180.0;
}


/* Calculate distance between two points */
double calculateDistance(double x1, double y1,
                         double x2, double y2) {

    double dx = x2 - x1;
    double dy = y2 - y1;

    return sqrt(dx * dx + dy * dy);
}


/* Calculate projectile range */
double calculateRange(double velocity, double angle) {

    double radians = degreesToRadians(angle);

    return (velocity * velocity * sin(2.0 * radians))
           / GRAVITY;
}


/* Find a possible firing angle */
double calculateFiringAngle(double velocity,
                            double distance,
                            double minimumAngle) {

    double value;
    double angle1;
    double angle2;

    if (velocity <= 0)
        return -1.0;

    value = (distance * GRAVITY)
            / (velocity * velocity);

    if (value > 1.0)
        return -1.0;

    if (value < 0.0)
        value = 0.0;

    angle1 = 0.5 * asin(value) * 180.0 / PI;
    angle2 = 90.0 - angle1;

    if (angle1 >= minimumAngle && angle1 <= 90.0)
        return angle1;

    if (angle2 >= minimumAngle && angle2 <= 90.0)
        return angle2;

    return -1.0;
}


/* Calculate time taken by shell to hit target */
double calculateTimeOfFlight(double velocity, double angle) {

    double radians = degreesToRadians(angle);

    return (2.0 * velocity * sin(radians))
           / GRAVITY;
}


/* Battleship names */
const char *getBattleshipName(char type) {

    switch (type) {

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


/* Set escort ship properties */
void setEscortProperties(EscortShip *ship,
                         double battleshipMaxVelocity) {

    switch (ship->type) {

        case 'A':

            ship->impactPower = 0.08;
            ship->minAngle = 20.0;

            ship->minVelocity =
                randomDouble(0.2 * battleshipMaxVelocity,
                             0.5 * battleshipMaxVelocity);

            ship->maxVelocity =
                1.2 * battleshipMaxVelocity;

            break;


        case 'B':

            ship->impactPower = 0.06;
            ship->minAngle = 30.0;

            ship->minVelocity =
                randomDouble(0.2 * battleshipMaxVelocity,
                             0.5 * battleshipMaxVelocity);

            ship->maxVelocity =
                randomDouble(0.5 * battleshipMaxVelocity,
                             0.9 * battleshipMaxVelocity);

            break;


        case 'C':

            ship->impactPower = 0.07;
            ship->minAngle = 25.0;

            ship->minVelocity =
                randomDouble(0.2 * battleshipMaxVelocity,
                             0.5 * battleshipMaxVelocity);

            ship->maxVelocity =
                randomDouble(0.5 * battleshipMaxVelocity,
                             0.9 * battleshipMaxVelocity);

            break;


        case 'D':

            ship->impactPower = 0.05;
            ship->minAngle = 50.0;

            ship->minVelocity =
                randomDouble(0.2 * battleshipMaxVelocity,
                             0.5 * battleshipMaxVelocity);

            ship->maxVelocity =
                randomDouble(0.5 * battleshipMaxVelocity,
                             0.9 * battleshipMaxVelocity);

            break;


        case 'E':

            ship->impactPower = 0.04;
            ship->minAngle = 70.0;

            ship->minVelocity =
                randomDouble(0.2 * battleshipMaxVelocity,
                             0.5 * battleshipMaxVelocity);

            ship->maxVelocity =
                randomDouble(0.5 * battleshipMaxVelocity,
                             0.9 * battleshipMaxVelocity);

            break;
    }
}


/* Copy escort ship data */
void copyEscorts(EscortShip source[],
                 EscortShip destination[],
                 int N) {

    for (int i = 0; i < N; i++) {
        destination[i] = source[i];
    }
}


/* Save initial conditions */
void saveInitialConditions(int D,
                           int N,
                           int k,
                           int t,
                           double thetaMin,
                           Battleship *B,
                           EscortShip escorts[],
                           Point path[]) {

    FILE *file;

    file = fopen("initial_conditions_part1C.txt", "w");

    if (file == NULL) {

        printf("Error creating initial conditions file.\n");
        return;
    }

    fprintf(file,
            "ADVANCED NAVAL BATTLE SIMULATOR\n");

    fprintf(file,
            "PART 1-C - INITIAL CONDITIONS\n");

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


    fprintf(file, "BATTLESHIP PATH\n");
    fprintf(file, "----------------------------------------\n");

    for (int i = 0; i < k; i++) {

        fprintf(file,
                "Point %d: (%.2f, %.2f)\n",
                i + 1,
                path[i].x,
                path[i].y);
    }


    fprintf(file, "\nESCORT SHIPS\n");
    fprintf(file, "----------------------------------------\n");

    for (int i = 0; i < N; i++) {

        fprintf(file,
                "E%d | Type EA-E%c | Position (%.2f, %.2f) | "
                "Vmin %.2f | Vmax %.2f | "
                "Min Angle %.2f | Impact Power %.2f\n",

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


/* Run Part 1-C simulation */
void runSimulation(int simulationNumber,
                   int D,
                   int N,
                   int k,
                   int t,
                   double thetaMin,
                   Battleship B,
                   EscortShip escorts[],
                   Point path[]) {

    char filename[100];

    sprintf(filename,
            "part1C_simulation%d_results.txt",
            simulationNumber);

    FILE *file = fopen(filename, "w");

    if (file == NULL) {

        printf("Error creating result file.\n");
        return;
    }


    printf("\n\n");
    printf("========================================\n");

    if (simulationNumber == 1)
        printf("      PART 1-C - SIMULATION 1\n");
    else
        printf("      PART 1-C - SIMULATION 2\n");

    printf("========================================\n");


    fprintf(file,
            "PART 1-C - SIMULATION %d\n",
            simulationNumber);

    fprintf(file,
            "========================================\n\n");


    int totalDestroyed = 0;
    int battleshipDestroyed = 0;

    double totalBattleTime = 0.0;


    /*
       Repeat simulation for every path point
    */

    for (int iteration = 0;
         iteration < k;
         iteration++) {

        double iterationTime = 0.0;

        int destroyedThisIteration = 0;


        /* Move B to current path point */

        B.x = path[iteration].x;
        B.y = path[iteration].y;


        /*
           In Simulation 2 the gun becomes jammed
           after t iterations.
        */

        double B_minimumAngle = 0.0;

        if (simulationNumber == 2 &&
            iteration + 1 > t) {

            B_minimumAngle = thetaMin;
        }


        printf("\n");
        printf("----------------------------------------\n");

        printf("ITERATION %d / %d\n",
               iteration + 1,
               k);

        printf("----------------------------------------\n");

        printf("B Position: (%.2f, %.2f)\n",
               B.x,
               B.y);

        printf("B Cumulative Damage: %.2f%%\n",
               B.damage * 100.0);


        fprintf(file, "\n");
        fprintf(file,
                "----------------------------------------\n");

        fprintf(file,
                "ITERATION %d / %d\n",
                iteration + 1,
                k);

        fprintf(file,
                "----------------------------------------\n");

        fprintf(file,
                "B Position: (%.2f, %.2f)\n",
                B.x,
                B.y);

        fprintf(file,
                "B Cumulative Damage: %.2f%%\n",
                B.damage * 100.0);


        /*
           Display gun status
        */

        if (simulationNumber == 2) {

            if (iteration + 1 <= t) {

                printf("B Gun: NORMAL (0 - 90 degrees)\n");

                fprintf(file,
                        "B Gun: NORMAL (0 - 90 degrees)\n");
            }

            else {

                printf("B Gun: JAMMED (%.2f - 90 degrees)\n",
                       thetaMin);

                fprintf(file,
                        "B Gun: JAMMED (%.2f - 90 degrees)\n",
                        thetaMin);
            }
        }


        /*
           ====================================
           B ATTACKS ESCORT SHIPS
           ====================================
        */

        printf("\nBATTLESHIP B ATTACKS\n");

        fprintf(file,
                "\nBATTLESHIP B ATTACKS\n");


        for (int i = 0; i < N; i++) {

            /*
               Destroyed E ships do not appear
               in later iterations.
            */

            if (escorts[i].destroyed)
                continue;


            double distance =
                calculateDistance(
                    B.x,
                    B.y,
                    escorts[i].x,
                    escorts[i].y);


            double angle =
                calculateFiringAngle(
                    B.maxVelocity,
                    distance,
                    B_minimumAngle);


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


            if (angle < 0) {

                printf("Result: OUT OF RANGE / ANGLE NOT AVAILABLE\n");

                fprintf(file,
                        "Result: OUT OF RANGE / ANGLE NOT AVAILABLE\n");
            }

            else {

                double timeToHit =
                    calculateTimeOfFlight(
                        B.maxVelocity,
                        angle);


                /*
                   B destroys E with one attack.
                */

                escorts[i].destroyed = 1;

                destroyedThisIteration++;
                totalDestroyed++;


                if (timeToHit > iterationTime)
                    iterationTime = timeToHit;


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


        /*
           ====================================
           ESCORT SHIPS ATTACK B
           ====================================
        */

        printf("\nESCORT ATTACKS\n");

        fprintf(file,
                "\nESCORT ATTACKS\n");


        for (int i = 0; i < N; i++) {

            /*
               A destroyed escort cannot attack.
            */

            if (escorts[i].destroyed)
                continue;


            double distance =
                calculateDistance(
                    escorts[i].x,
                    escorts[i].y,
                    B.x,
                    B.y);


            double velocity =
                randomDouble(
                    escorts[i].minVelocity,
                    escorts[i].maxVelocity);


            double angle =
                calculateFiringAngle(
                    velocity,
                    distance,
                    escorts[i].minAngle);


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


            if (angle < 0) {

                printf("Result: OUT OF RANGE\n");

                fprintf(file,
                        "Result: OUT OF RANGE\n");
            }

            else {

                double timeToHit =
                    calculateTimeOfFlight(
                        velocity,
                        angle);


                /*
                   =================================
                   PART 1-C IMPORTANT CHANGE
                   =================================

                   Add escort impact power to
                   B's cumulative damage.
                */

                B.damage += escorts[i].impactPower;


                if (B.damage > 1.0)
                    B.damage = 1.0;


                if (timeToHit > iterationTime)
                    iterationTime = timeToHit;


                printf("Velocity: %.2f m/s\n",
                       velocity);

                printf("Angle: %.2f degrees\n",
                       angle);

                printf("Time to Hit: %.2f seconds\n",
                       timeToHit);

                printf("Impact Power: %.2f\n",
                       escorts[i].impactPower);

                printf("Damage Added: %.2f%%\n",
                       escorts[i].impactPower * 100.0);

                printf("B Cumulative Damage: %.2f%%\n",
                       B.damage * 100.0);


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
                        "Impact Power: %.2f\n",
                        escorts[i].impactPower);

                fprintf(file,
                        "Damage Added: %.2f%%\n",
                        escorts[i].impactPower * 100.0);

                fprintf(file,
                        "B Cumulative Damage: %.2f%%\n",
                        B.damage * 100.0);


                /*
                   Check whether B has reached
                   100% damage.
                */

                if (B.damage >= 1.0) {

                    B.destroyed = 1;
                    battleshipDestroyed = 1;


                    printf("\nB HAS REACHED 100%% DAMAGE!\n");
                    printf("BATTLESHIP B HAS BEEN DESTROYED!\n");


                    fprintf(file,
                            "\nB HAS REACHED 100%% DAMAGE!\n");

                    fprintf(file,
                            "BATTLESHIP B HAS BEEN DESTROYED!\n");

                    break;
                }
            }
        }


        /*
           ====================================
           ITERATION SUMMARY
           ====================================
        */

        totalBattleTime += iterationTime;


        printf("\nIteration %d complete.\n",
               iteration + 1);

        printf("Escort ships destroyed: %d\n",
               destroyedThisIteration);

        printf("Iteration time: %.2f seconds\n",
               iterationTime);

        printf("B cumulative damage: %.2f%%\n",
               B.damage * 100.0);


        fprintf(file,
                "\nITERATION SUMMARY\n");

        fprintf(file,
                "Escort ships destroyed: %d\n",
                destroyedThisIteration);

        fprintf(file,
                "Iteration time: %.2f seconds\n",
                iterationTime);

        fprintf(file,
                "B cumulative damage: %.2f%%\n",
                B.damage * 100.0);


        /*
           Display remaining E ships.
        */

        printf("Remaining Escort Ships: ");

        fprintf(file,
                "Remaining Escort Ships: ");


        int remaining = 0;


        for (int i = 0; i < N; i++) {

            if (!escorts[i].destroyed) {

                printf("E%d ",
                       escorts[i].id);

                fprintf(file,
                        "E%d ",
                        escorts[i].id);

                remaining++;
            }
        }


        if (remaining == 0) {

            printf("None");

            fprintf(file,
                    "None");
        }


        printf("\n");

        fprintf(file,
                "\n");


        /*
           Stop simulation if B is destroyed.
        */

        if (battleshipDestroyed) {

            printf("\n");
            printf("BATTLESHIP B HAS BEEN DESTROYED!\n");

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


    /*
       ====================================
       FINAL RESULT
       ====================================
    */

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


    if (battleshipDestroyed) {

        printf("Battleship B: DESTROYED\n");

        fprintf(file,
                "Battleship B: DESTROYED\n");
    }

    else {

        printf("Battleship B: SURVIVED\n");

        fprintf(file,
                "Battleship B: SURVIVED\n");
    }


    printf("B Cumulative Damage: %.2f%%\n",
           B.damage * 100.0);

    printf("B Remaining Health: %.2f%%\n",
           (1.0 - B.damage) * 100.0);


    fprintf(file,
            "B Cumulative Damage: %.2f%%\n",
            B.damage * 100.0);

    fprintf(file,
            "B Remaining Health: %.2f%%\n",
            (1.0 - B.damage) * 100.0);


    printf("Escort ships destroyed: %d / %d\n",
           totalDestroyed,
           N);

    printf("Total simulation time: %.2f seconds\n",
           totalBattleTime);


    fprintf(file,
            "Escort ships destroyed: %d / %d\n",
            totalDestroyed,
            N);

    fprintf(file,
            "Total simulation time: %.2f seconds\n",
            totalBattleTime);


    /*
       Final E status
    */

    printf("\nFINAL ESCORT STATUS\n");

    fprintf(file,
            "\nFINAL ESCORT STATUS\n");


    for (int i = 0; i < N; i++) {

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


/* ==========================================
   MAIN PROGRAM
   ========================================== */

int main() {

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


    printf("========================================\n");
    printf("      ADVANCED NAVAL BATTLE SIMULATOR\n");
    printf("      PART 1-C - CUMULATIVE DAMAGE\n");
    printf("========================================\n");


    /*
       Random seed
    */

    printf("\nEnter random seed: ");
    scanf("%d", &seed);

    srand(seed);


    /*
       Battlefield size
    */

    printf("\nEnter battlefield size D: ");
    scanf("%d", &D);


    if (D <= 0) {

        printf("Invalid battlefield size.\n");
        return 1;
    }


    /*
       Number of escort ships
    */

    printf("Enter number of escort ships N: ");
    scanf("%d", &N);


    if (N <= 0 || N > MAX_ESCORTS) {

        printf("Invalid number of escort ships.\n");
        return 1;
    }


    /*
       Number of path points
    */

    printf("Enter number of path points k: ");
    scanf("%d", &k);


    if (k <= 1 || k > MAX_POINTS) {

        printf("k must be greater than 1.\n");
        return 1;
    }


    /*
       Gun jam iteration
    */

    printf("\nEnter gun jam iteration t (0 < t < k): ");
    scanf("%d", &t);


    if (t <= 0 || t >= k) {

        printf("Invalid t.\n");
        return 1;
    }


    /*
       Minimum angle after gun jam
    */

    printf("Enter theta_min (0 < theta_min < 30): ");
    scanf("%lf", &thetaMin);


    if (thetaMin <= 0 || thetaMin >= 30) {

        printf("Invalid theta_min.\n");
        return 1;
    }


    /*
       Battleship selection
    */

    printf("\nSelect Battleship:\n");

    printf("1. USS Iowa\n");
    printf("2. MS King George V\n");
    printf("3. Richelieu\n");
    printf("4. Sovetsky Soyuz-class\n");

    printf("Enter choice: ");
    scanf("%d", &battleshipChoice);


    switch (battleshipChoice) {

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


    /*
       Battleship shell velocity
    */

    printf("\nEnter Battleship maximum shell velocity: ");

    scanf("%lf",
          &battleshipMaxVelocity);


    if (battleshipMaxVelocity <= 0) {

        printf("Velocity must be greater than zero.\n");
        return 1;
    }


    B.maxVelocity = battleshipMaxVelocity;

    B.damage = 0.0;

    B.destroyed = 0;


    /*
       Generate escort ships
    */

    printf("\nGenerating Escort Ships...\n");


    for (int i = 0; i < N; i++) {

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
            B.maxVelocity);


        printf("E%d | Type EA-E%c | "
               "Position (%.2f, %.2f) | "
               "Impact Power %.2f\n",

               escorts[i].id,
               escorts[i].type,
               escorts[i].x,
               escorts[i].y,
               escorts[i].impactPower);
    }


    /*
       Save original escort conditions.
    */

    copyEscorts(
        escorts,
        initialEscorts,
        N);


    /*
       Generate Battleship path
    */

    printf("\nGenerating Battleship path...\n");


    for (int i = 0; i < k; i++) {

        path[i].x =
            randomDouble(0, D);

        path[i].y =
            randomDouble(0, D);


        printf("Point %d: (%.2f, %.2f)\n",
               i + 1,
               path[i].x,
               path[i].y);
    }


    B.x = path[0].x;
    B.y = path[0].y;


    /*
       Save initial conditions
    */

    saveInitialConditions(
        D,
        N,
        k,
        t,
        thetaMin,
        &B,
        initialEscorts,
        path);


    printf("\nInitial conditions saved.\n");


    /*
       =====================================
       PART 1-C SIMULATION 1
       =====================================
    */

    copyEscorts(
        initialEscorts,
        simulationEscorts,
        N);


    B.damage = 0.0;
    B.destroyed = 0;


    runSimulation(
        1,
        D,
        N,
        k,
        t,
        thetaMin,
        B,
        simulationEscorts,
        path);


    /*
       =====================================
       PART 1-C SIMULATION 2
       =====================================
    */

    printf("\n\n");

    printf("========================================\n");
    printf("RESETTING TO SAME INITIAL CONDITIONS\n");
    printf("FOR SIMULATION 2\n");
    printf("========================================\n");


    copyEscorts(
        initialEscorts,
        simulationEscorts,
        N);


    B.damage = 0.0;
    B.destroyed = 0;


    runSimulation(
        2,
        D,
        N,
        k,
        t,
        thetaMin,
        B,
        simulationEscorts,
        path);


    /*
       =====================================
       COMPLETE
       =====================================
    */

    printf("\n");
    printf("========================================\n");
    printf("      PART 1-C COMPLETED\n");
    printf("========================================\n");


    printf("\nFiles created:\n");

    printf("1. initial_conditions_part1C.txt\n");

    printf("2. part1C_simulation1_results.txt\n");

    printf("3. part1C_simulation2_results.txt\n");


    printf("\nSimulation 1 = normal gun\n");

    printf("Simulation 2 = gun jams after t iterations\n");

    printf("Cumulative damage is now applied to B.\n");


    return 0;
}